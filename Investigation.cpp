#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

const ll INF = 1e16;
const ll MOD = 1e9+7;
vector<vector<pair<ll, ll>>> adj;

vector<ll> cnt;
vector<ll> mvert;
vector<ll> xvert;

void dijkstra(ll s, vector<ll> & d, vector<ll> & p) {
    ll n = adj.size();
    d.assign(n, INF);
    p.assign(n, -1);

    d[s] = 0;
    set<pair<ll, ll>> q;
    q.insert({0, s});
    while (!q.empty()) {
        ll v = q.begin()->second;
        q.erase(q.begin());

        for (auto edge : adj[v]) {
            ll to = edge.first;
            ll len = edge.second;

            if (d[v] + len < d[to]) {
                q.erase({d[to], to});
                d[to] = d[v] + len;
                p[to] = v;
                cnt[to]=cnt[v];
                mvert[to] = mvert[v] +1;
                xvert[to] = xvert[v] +1; 
                q.insert({d[to], to});
            }else if(d[v] + len == d[to]){
                cnt[to]=(cnt[to]+cnt[v])%MOD;
                mvert[to] = min(mvert[to], mvert[v]+1);
                xvert[to] = max(xvert[to], xvert[v]+1);
            }
        }
    }
}
void solve(){
    int n,m;
    cin>>n>>m;
    cnt.assign(n+1,0);
    cnt[1]=1;
    mvert.assign(n+1,INF);
    mvert[1]=0;
    xvert.assign(n+1,0);
    adj.assign(n+1,vector<pair<ll,ll>>());
    for(int i=0; i<m; i++){
        int a,b,w;
        cin>>a>>b>>w;
        adj[a].push_back({b,w});
    }
    vector<ll> d,p;
    dijkstra(1,d,p);
    cout<<d[n]<<" "<<cnt[n]<<" "<<mvert[n]<<" "<<xvert[n]<<"\n";
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();
    return 0;
}