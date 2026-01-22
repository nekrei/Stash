#include<bits/stdc++.h>
using namespace std;
//add these
vector<int> component, sz, rnk;


int find_set(int v) {
    if (v == component[v])
        return v;
    return component[v] = find_set(component[v]);
}

//by size
void make_set(int v) {
    component[v] = v;
    sz[v] = 1;
}

void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (sz[a] < sz[b])
            swap(a, b);
        component[b] = a;
        sz[a] += sz[b];
    }
}

//by rank
void make_set(int v) {
    component[v] = v;
    rnk[v] = 0;
}

void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (rnk[a] < rnk[b])
            swap(a, b);
        component[b] = a;
        if (rnk[a] == rnk[b])
            rnk[a]++;
    }
}