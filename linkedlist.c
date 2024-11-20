#include<stdio.h>
#include<stdlib.h>
#include<math.h>

struct list{
    struct list* prev;
    double x;
    double y;
    struct list* next;
};

struct list* begin;
struct list* end;

void create(double x,double y){
    struct list* p = malloc(sizeof(struct list));
    (*p).prev = end;
    (*p).next = NULL;
    (*p).x  = x;
    (*p).y = y;
    (*end).next = p;
    end = p; 
}
struct list* find(double x, double y){
    struct list * it;
    struct list *ans = NULL;
    for(it = (*begin).next; it ; it = it->next){
        if(fabs(x-(it->x)) < 1e-7 && fabs(y- (it->y)) < 1e-7) {ans = it; break;}
    }
    return ans;
}
//void delete(double x, double y){
//    struct list *addr = find(x,y);
//    struct list *b4 = addr->prev;
//    struct list *n4 = addr->next;
//    b4->next = n4;
//    n4->prev = b4;
//    free(addr);
//}

void delete(struct list* addr){
    struct list *b4 = addr->prev;
    struct list *n4 = addr->next;
    b4->next = n4;
    n4->prev = b4;
    free(addr);
}

int main(){
    begin = malloc(sizeof(struct list));
    end = begin;
    
    create(12.3,23.4);
    create(11.2, 11.3);
    create(22, 15.66);

    struct list * it = (*begin).next;
    for(it = (*begin).next; it ; it = it->next)
        printf("%lf %lf\n",it->x, it->y);

    delete(find(11.2, 11.3));

    for(it = (*begin).next; it ; it = it->next)
        printf("%lf %lf\n",it->x, it->y);

    return 0;
}