#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

vector <ll> component, rnk;

ll find_set(ll u){
    if(component[u]==u) return u;
    return component[u] = find_set(component[u]);
}
void union_set(ll u, ll v){
    u = find_set(u);
    v = find_set(v);
    if(u==v) return;
    if(rnk[u]<rnk[v]) component[u] = v;
    else if(rnk[v]<rnk[u]) component[v] = u;
    else{
        component[v] = u;
        rnk[u]++;
    }
}
void solve(){
    int n,m,r;
    cin>>n>>m;
    vector<vector<ll>> el;
    for(int i=0; i<m; i++){ 
        int a,b,w;
        cin>>a>>b>>w;
        el.push_back({w, a, b});
    }
    cin>>r;

    sort(el.begin(), el.end());
    component.resize(n);
    rnk.assign(n,0);
    iota(component.begin(), component.end(), 0);
    vector<pair<ll,ll>> tel;
    ll tcost = 0;

    for(int i=0; i<m; i++){
        ll w = el[i][0];
        ll u = el[i][1];
        ll v = el[i][2];        
        if(find_set(u)!=find_set(v)){
            tcost += w;
            tel.push_back({u,v});
            union_set(u,v);
        }
        // for(auto c: component)
        //     cout<<c<<" ";
        // cout<<"----\n";
    }
    
    cout<<"Total weight "<<tcost<<'\n';
    for(auto pr: tel)
        cout<<pr.first<<" "<<pr.second<<'\n';
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
        solve();
    return 0;
}