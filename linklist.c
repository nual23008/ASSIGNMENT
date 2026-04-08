#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USB_STORAGE 32768 //Mb

typedef struct {
    int day, month, year;
} date;

typedef struct {
    char name[20];
    int size;
    date timestamp;
} file;

typedef struct node{
    file data;
    struct node* next;
} node;

node* create_node(const char* name, int size, int day, int month, int year) {
    node* new_node = (node*)malloc(sizeof(node));
    strcpy(new_node->data.name, name);
    new_node->data.size = size;
    new_node->data.timestamp.day = day;
    new_node->data.timestamp.month = month;
    new_node->data.timestamp.year = year;
    new_node->next = NULL;
    return new_node;
}

void create_list(node** head) {
    *head = NULL;
}

void insert_file(node** orig_file, node** selected_file) {
    node* q = *selected_file;

    while(q != NULL) {
        node* next_q = q->next;
        node* p = *orig_file;
        node* prev = NULL;

        while (p != NULL) {
            if(q->data.timestamp.year < p->data.timestamp.year ||
               q->data.timestamp.year == p->data.timestamp.year && q->data.timestamp.month < p->data.timestamp.month ||
               q->data.timestamp.year == p->data.timestamp.year && q->data.timestamp.month == p->data.timestamp.month && q->data.timestamp.day < p->data.timestamp.day) {        
                break;
               }
            prev = p;
            p = p->next;   
        }

        if(prev == NULL) {
            q->next = *orig_file;
            *orig_file = q;
        }
        else {
            prev->next = q;
            q->next = p;
        }

        q = next_q;
    }

    *selected_file = NULL;
} 

int size_compute(node* file) {
    node* p = file;
    int total_size = 0;
    while(p != 0) {
        total_size += p->data.size;
        p = p->next;
    }

    return total_size;
}

void Add_to_USB(node** file) {
    int file_size = size_compute(*file);

    while(file_size > USB_STORAGE && *file != NULL) {
        node* p = *file;
        node* prev = NULL;
        node* min = p;
        node* min_prev = NULL;
        int min_size = p->data.size;

        while(p != NULL) {
            if(p->data.size < min_size) {
                min_size = p->data.size;
                min = p;
                min_prev = prev;
            }

            prev = p;
            p = p->next;
        }

        if(min_prev == NULL) {
            *file = min->next;
        }
        else {
            min_prev->next = min->next;
        }

        file_size -= min->data.size;
        
        free(min);
    }
    
}

void print_file(node* file) {
    node *p = file;

    while(p != NULL) {
        printf(" - [%02d/%02d/%04d] %-15s : %d MB\n", 
        p->data.timestamp.day, p->data.timestamp.month, p->data.timestamp.year, 
        p->data.name, p->data.size);
        p = p->next;
    }
}

int main() {
    node* orig_file;
    create_list(&orig_file);

    node* f1 = create_node("Video_KyYeu.mp4", 150, 15, 6, 2023); 
    node* f2 = create_node("TaiLieu_C.pdf", 10, 10, 1, 2023);   
    node* f3 = create_node("Game_HanhDong.exe", 20000, 20, 8, 2023); 
    node* f4 = create_node("Nhac_Tre.mp3", 50, 5, 4, 2023);

    orig_file = f1;
    f1->next = f2;
    f2->next = f3;
    f3->next = f4;

    print_file(orig_file);

    printf("============================================= \n");

    node* f5 = create_node("Game_hay.nana", 25, 12, 8, 2023);
    node* f6 = create_node("nhacviet.dd", 443, 26, 3, 2023);
    node* f7 = create_node("lobster.fff", 2224, 11, 1, 2023);
    node* f8 = create_node("cado.xyz", 977, 23, 8, 2023);
    
    node* selected_file = f5;
    f5->next = f6;
    f6->next = f7;
    f7->next = f8;
    

    insert_file(&orig_file, &selected_file);
    print_file(orig_file);

    printf("============================================= \n");


    Add_to_USB(&orig_file);

    print_file(orig_file);

    node* p = orig_file;
    while(p != NULL) {
        node* temp = p;
        p = p->next;
        free(temp);
    }

    return 0;
}