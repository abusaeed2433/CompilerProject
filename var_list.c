#include<stdio.h>
#include <stddef.h>
#include<string.h>
#include<stdlib.h>
#include "var_list.h"

struct VARIABLE{
    char name[200];
    double value;
    struct VARIABLE *prev;
    struct VARIABLE *next;
};

struct VARIABLE *head = NULL;
struct VARIABLE *tail = NULL;

struct VARIABLE* createNode(const char *name, double value) {
    struct VARIABLE *newNode = (struct VARIABLE*) malloc(sizeof(struct VARIABLE));
    if(!newNode) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    strncpy(newNode->name, name, sizeof(newNode->name) - 1);
    newNode->value = value;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

void insertVariable(char *name, double val) {
    struct VARIABLE *var = createNode(name,val);
    
    if(tail == NULL) {
        head = var;
        tail = var;
    }
    else {
        var->prev = tail;
        tail->next = var;
        tail = var;
    }
}

void updateVariable(char *name, double val){
    struct VARIABLE *ptr;
    ptr = head;
    while(ptr != NULL){
        if( strcmp(ptr->name,name) == 0 ){
            ptr->value = val;
            break;
        }
        ptr = ptr->next;
    }
}

void deleteVariable(char *name){
    struct VARIABLE *ptr;
    ptr = head;
    while(ptr != NULL){
        if( strcmp(ptr->name, name) == 0 ){
            // first delete
            if(ptr == head){
                //single node
                if(ptr == tail) tail == NULL;
                head = ptr->next;
            }
            else if(ptr == tail){ // last delete
                tail = tail->prev;
                tail->next = NULL;
            }
            else{
                ptr->prev->next = ptr->next;
                ptr->next->prev = ptr->prev;
            }
        }
        ptr = ptr->next;
    }
}

void printAll(){
    if(head == NULL) return;

    struct VARIABLE *ptr;
    ptr = head;
    while(ptr != NULL){
        printf("%s %lf -> ",ptr->name,ptr->value);
        ptr = ptr->next;
    }
    printf("\n");
}

void printAll2(){
    if(head == NULL) return;

    struct VARIABLE *ptr;
    ptr = tail;
    while(ptr != NULL){
        printf("%s %lf -> ",ptr->name,ptr->value);
        ptr = ptr->prev;
    }
    printf("\n");
}

int main(){
    
    insertVariable("a",1);
//    insertVariable("b",2);
    //insertVariable("c",3);

    printAll();
    printAll2();

    deleteVariable("a");
    printAll();
    printAll2();



}
