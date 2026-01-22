#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll INF = 1e16;
void solve(){
    int n,m,r;
    cin>>n>>m;
    vector<vector<pair<ll,ll>>> adj(n);
    vector<pair<ll,ll>> el;
    for(int i=0; i<m; i++){ 
        int a,b,w;
        cin>>a>>b>>w;
        adj[a].push_back({b,w});
        adj[b].push_back({a,w});
    }
    cin>>r;
    ll tcost = 0;
    vector<ll> mined(n, INF);
    vector<ll> p(n,-1);
    vector<bool> visited(n, false);
    priority_queue<pair<ll,ll>, vector<pair<ll,ll>>, greater<pair<ll,ll>>> q;
    q.push({0,r});
    while(!q.empty()){
        ll u = q.top().second;
        ll w = q.top().first;
        if(visited[u]){
            q.pop();continue;
        }
        visited[u] = true;
        tcost += w;
        if(p[u]!=-1) el.push_back({p[u], u});
        q.pop();
        for(auto pr: adj[u]){
            if(!visited[pr.first] && mined[pr.first] > pr.second){
                p[pr.first] = u;
                mined[pr.first] = pr.second;
                q.push({pr.second, pr.first});
            }
        }
    }
    cout<<"Total weight "<<tcost<<'\n';
    cout<<"Root node "<<r<<'\n';
    for(auto pr: el){
        cout<<pr.first<<' '<<pr.second<<'\n';
    }   
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
        solve();
    return 0;
}