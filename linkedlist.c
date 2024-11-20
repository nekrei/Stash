#include<stdio.h>
#include<stdlib.h>

struct list{
    struct list* prev;
    double x;
    double y;
    struct list* next;
};


void create(double x,double y, struct list** end){
    struct list* p = malloc(sizeof(struct list));
    (*p).prev = *end;
    (*p).next = NULL;
    (*p).x  = x;
    (*p).y = y;
    (**end).next = p;
    *end = p; 
}
int main(){
    struct list* begin;
    struct list* end;
    begin = malloc(sizeof(struct list));
    end = begin;
    
    create(12.3,23.4,&end);
    create(11.2, 11.3,&end);
    create(22, 15.66,&end);
    return 0;
}