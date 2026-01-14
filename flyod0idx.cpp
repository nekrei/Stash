#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

const ll INF = 1e15;
void solve(){
    int n,m;
    cin>>n>>m;
    vector<vector<ll>> am(n+1, vector<ll>(n+1,INF));
    for(int i=0; i<n; i++)
        am[i][i] = 0;
    for(int i=0; i<m; i++){ 
        int a,b,w;
        cin>>a>>b>>w;
        am[a][b]=w;
    }
    
    for(int k=0; k<n; k++){
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++)
                am[i][j] = min(am[i][j], am[i][k] + am[k][j]);
        }
    }

}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
        solve();
    return 0;
}
