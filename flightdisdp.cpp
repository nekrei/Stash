#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

vector<vector<pair<ll,ll>>> al(100005);

ll inf = 1e15;
void dijsktra(int src, vector<vector<ll>> &v){
    priority_queue<vector<ll>> dis;
   
    dis.push({0,src,0});
    dis.push({0,src,1});
    while(!dis.empty()){
        ll wt = -dis.top()[0];
        ll dest = dis.top()[1];
        ll state = dis.top()[2];
        dis.pop();
        
        if(state==1){
            if(v[dest][1]>wt){
                v[dest][1] = wt;
                for(auto x: al[dest]){
                    ll nx = x.first;
                    ll w = x.second + v[dest][1];
                    if(v[nx][1]> w){
                        dis.push({-w,nx,1});
                    }
                }
            }
        }else{
            if(v[dest][0]>wt){
                v[dest][0] = wt;
                for(auto x: al[dest]){
                    ll nx = x.first;
                    ll w = x.second + v[dest][0];
                    ll w1 = x.second/2 + v[dest][0];
                    if(v[nx][0]> w){
                        dis.push({-w,nx,0});
                    }
                    if(v[nx][1]>w1){
                        dis.push({-w1,nx,1});
                    }

                }
            }
        }
    }
}
void solve(){
    int n,m;
    cin>>n>>m;
    for(int i=0; i<m; i++){
        int a,b,w;
        cin>>a>>b>>w;
        al[a].push_back({b,w});
    }
    vector<vector<ll>> d(n+1,vector<ll>(2,inf));
    dijsktra(1,d);
    ll ans = 1e15;

    // for(auto c: fd) cout<<c<<" ";
    // cout<<"\n";
    // for(auto c: rd) cout<<c<<" ";
    // cout<<"\n";

    cout<<min(d[n][0], d[n][1])<<"\n";
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
        solve();
    return 0;
}