#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

const ll INF = 1e16;
ll n,k;
vector<vector<pair<ll, ll>>> adj;

void dijkstra(ll s, vector<vector<ll>> & d) {
    ll n = adj.size();
    d.assign(n, vector<ll>());

    priority_queue<pair<ll, ll>> q;
    q.push({0, s});
    while (!q.empty()) {
        ll dis = -q.top().first;
        ll v = q.top().second;
        q.pop();
        if(d[v].size()>=k) continue;
        d[v].push_back(dis);

        for (auto edge : adj[v]) {
            ll to = edge.first;
            ll len = edge.second;
            q.push({-(dis+len), to});
        }
    }
}

int main(){
    int m;
    cin>>n>>m>>k;
    adj.assign(n+1,vector<pair<ll,ll>>());
    for(int i=0; i<m; i++){
        int a,b,w;
        cin>>a>>b>>w;
        adj[a].push_back({b,w});
    }
    vector<vector<ll>> ans;
    dijkstra(1,ans);
    for(auto c: ans[n])
        cout<<c<<" ";
    cout<<'\n';
}