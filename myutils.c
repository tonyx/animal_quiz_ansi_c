#ifndef STDIO_INCLUDED 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "mystringutils.h"
#endif 
#define STDIO_INCLUDED

void add_element_to_list(Str_list** list, char* element) {
    if (*list==NULL) {
        *list = malloc(sizeof(Str_list));
        (*list)->element = element; 
        (*list)->next = NULL;
    } 
    else {
        add_element_to_list(&(*list)->next,element);
    }
}

char* head_element_of_list(Str_list** list) {
    if (*list==NULL) {
        return NULL;
    } else {
        return (*list)->element;
    }
}

int has_elements(Str_list** list) {
    return (*list!=NULL);
}

void free_str_list(Str_list** list) {
    if ((*list)==NULL) {
        return;
    } else {
        free_str_list(&(*list)->next);
        free(*list);
    }
}


struct str_list* tail_of_list(struct str_list* list) {
    return list->next;
}



char * concatenate_strings(int num, ...) {
    char* to_return;
    va_list valist;
    int total_size=0;
    va_start(valist,num);
    
    for (int i=0;i<num;i++) {
        total_size+=strlen(va_arg(valist,char*));
    }
    va_end(valist);
    total_size+=20;
    to_return = malloc(total_size);
    strcpy(to_return,"");

    va_start(valist,num);
    for (int i=0;i<num;i++) {
        strcat(to_return,(va_arg(valist,char*)));
    }
    return to_return;
}

void get_user_input(char* user_input) {
        fgets(user_input,97,stdin);
        if (user_input==NULL||strlen(user_input)==0) {
            user_input="";
        } else {
            char * eof = strchr(user_input,'\n');
            if (eof!=0) {
                *eof = '\0'; 
            }
        }
}