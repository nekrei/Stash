#pragma once
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;

template<typename K, typename V>
class HashTable{
    private:
        ll tableSize, elemCount;
        double loadFactor;
        vector<list<pair<K,V>>> table;
        ull (*hashFunc)(K);
        function<pair<ull, ull>(K)> collisionRes;
        enum EntryState {EMPTY, OCCUPIED, DELETED};
        string collisionResolutor;
        pair<ll, ll> userConstants;
        vector<EntryState> entryStates;

        inline static vector<ll> primes = [](){
            vector<ll> s;
            ll n = 1e7;
            vector<bool> sieve(n+1, true);
            sieve[0] = sieve[1] = false;
            for(ll i=2; i<=n; i++){
                if(sieve[i]){
                    if(i>=13) s.push_back(i);
                    for(ll val = i*i; val<=n; val+=i){
                        sieve[val] = false;
                    }
                }
            }
            return s;
        }();
        void rehash(ull newSize){
            if(newSize == tableSize) return;

            //totalColl = 0; // unsure about this

            vector<list<pair<K,V>>> old = table;
            vector<EntryState> oldState = entryStates;
            tableSize = newSize;
            elemCount = 0;
            table.clear();
            table.assign(tableSize, list<pair<K,V>>());
            entryStates.assign(tableSize, EMPTY);
            for(int i=0; i<old.size(); i++){
                if(oldState[i] == OCCUPIED){
                    for(auto pr: old[i]){
                        //cout<<"\033[32mDEBUG: \033[0m" << pr.first << " -> " << pr.second << endl;
                        insert(pr.first, pr.second);
                    }
                }
            }
        }
    public:
        ull totalColl = 0;
        class marker{
            private:
                ull bidx;
                typename list<pair<K,V>>::iterator eidx;
                HashTable& ht;
                bool isEnd;
            public:
                ull colcnt = 0;
                marker(HashTable& ht, ull bidx, typename list<pair<K,V>>::iterator eidx, ull colcnt=0, bool isEnd=false): ht(ht), bidx(bidx), eidx(eidx), colcnt(colcnt), isEnd(isEnd){}
                const marker& operator =(const marker& it){
                    ht = it.ht;
                    bidx = it.bidx;
                    eidx = it.eidx;
                    colcnt = it.colcnt;
                    isEnd = it.isEnd;
                    return *this;
                }
                bool operator ==(const marker& it){
                    if(isEnd && it.isEnd && &ht == &it.ht) return true;
                    return &ht == &it.ht && bidx == it.bidx && eidx==it.eidx && isEnd == it.isEnd;
                }
                bool operator !=(const marker& it){
                    if(isEnd && it.isEnd && &ht == &it.ht) return false;
                    return &ht != &it.ht || bidx != it.bidx || eidx!=it.eidx || isEnd != it.isEnd;
                }
                pair<K,V> operator *(){
                    return eidx.operator*();
                }
                pair<K,V>* operator->(){
                    return eidx.operator->();
                }
        };
        static ull hash1(K keyinp){
            //implemented polynomial rolling hash here, ref: https://cp-algorithms.com/string/string-hashing.html
            string key;
            if constexpr (is_same<K, string>::value) key = keyinp;
            else key = to_string(keyinp);
            ull p = 31;
            ull M = 1e9+9;
            ull hash = 0;
            for(auto it = key.rbegin(); it!=key.rend(); it++){
                hash = (hash*p + (*it)) % M;
            }
            return hash;
        }

        static ull hash2(K keyinp){
            //djb2hash (bernstein), ref: http://www.cse.yorku.ca/~oz/hash.html
            ull hash = 5381;
            string key;
            if constexpr (is_same<K, string>::value) key = keyinp;
            else key = to_string(keyinp);
            for(auto c: key){
                hash = ((hash<<5) + hash)^c;
            }
            return hash;
        }
    
    private:
        static ull auxHash(K keyinp){
            //sdbm, ref: http://www.cse.yorku.ca/~oz/hash.html
            ull hash = 0;
            string key;
            if constexpr (is_same<K, string>::value) key = keyinp;
            else key = to_string(keyinp);
            for(auto c: key){
                hash = (hash<<16) + (hash<<6) - hash + c;
            }
            return hash;
        }

        pair<ull, ull> chaining(K key){
            ull idx = (*hashFunc)(key) % tableSize;
            if(table[idx].empty()) return {idx,0};
            return {idx,1};
        }
        pair<ull, ull> doubleHashing(K key){
            ull hashidx = (*hashFunc)(key);
            ull auxidx = auxHash(key)%tableSize;
            if(!auxidx) auxidx++;
            //cout<<"\033[32mDEBUG: \033[0mObtained " << hashidx<<"\t"<<auxidx <<" for Key "<<key<<endl;
            ull idx,i;
            for(idx = hashidx%tableSize, i=0; i<tableSize && entryStates[idx] == OCCUPIED && table[idx].front().first != key;
                idx = (idx + auxidx)%tableSize, i++){/*cout<<"\033[31mDHash: \033[0mObtained " << idx <<" for Key "<<key<<endl;*/}
            //cout<<"\033[31mDHash Final: \033[0mObtained " << idx <<" for Key "<<key<<endl;
            return {idx,i};
        }
        pair<ull, ull> customProbing(K key){
            ull hashidx = (*hashFunc)(key);
            ull auxidx = auxHash(key)%tableSize;
            if(!auxidx) auxidx++;
            ull idx,i, step = 0;
            for(idx = hashidx%tableSize, i=0; i<tableSize &&entryStates[idx] == OCCUPIED && table[idx].front().first != key;
                idx = (hashidx + step)%tableSize, i++){
                    step = (userConstants.first*auxidx*i) + (userConstants.second*i*i);
                    if(!step%tableSize) step++;
                    //cout<<"\033[32mCProbe: \033[0mObtained " << idx <<" for Key "<<key<<endl;
                }
            //cout<<"\033[32mCProbe Final: \033[0mObtained " << idx <<" for Key "<<key<<endl;
            return {idx,i};
        }
    public:
        HashTable(int size=13, ull (*hf)(K) = hash1, string collres = "chain", double loadFactorCut=0.5, ull constA=1, ull constB = 1){
            auto it = lower_bound(primes.begin(), primes.end(), size);
            tableSize = (it != primes.end()) ? *it : 99999991;
            elemCount = 0;
            table.assign(tableSize, list<pair<K,V>>());
            hashFunc = hf;
            collisionResolutor = collres;
            if(collres == "double_hash") collisionRes = [this](K key){ return doubleHashing(key);};
            else if(collres == "custom_probe") collisionRes = [this](K key){ return customProbing(key);};
            else collisionRes = [this](K key){ return chaining(key);};
            loadFactor = loadFactorCut;
            userConstants = {constA, constB};
            entryStates.assign(tableSize, EMPTY);
        }

        int insert(K key, V value){
            if(search(key) != end()) return -1;
            pair<ull,ull> pr = collisionRes(key);
            ull idx = pr.first, colcnt = pr.second;
            entryStates[idx] = OCCUPIED;
            table[idx].push_back({key,value});
            elemCount++;
            if(elemCount > loadFactor*tableSize){
                ll nsize = *(lower_bound(primes.begin(), primes.end(), 2*tableSize));
                rehash(nsize);
            }
            totalColl += colcnt;
            return colcnt;
        }
        HashTable<K,V>::marker end(ull hits=0){return HashTable<K,V>::marker(*this, tableSize, table[0].end(), hits);}
        
        /**
         * returns a HashTable::marker -> similar to entry in java
         * ->first for key
         * ->second for value
         */
        HashTable<K,V>::marker search(K key){
            if(collisionResolutor == "chain"){
                pair<ull,ull> pr = collisionRes(key);
                ull idx = pr.first, i=1;
                for(auto it = table[idx].begin(); it!= table[idx].end(); it++, i++){
                    if(it->first == key)
                        return HashTable<K,V>::marker(*this, idx, it, i);
                }
                return end(i);
            }
            ull hashidx = (*hashFunc)(key);
            ull auxidx = auxHash(key)%tableSize;
            if(!auxidx) auxidx++;
            ull idx,i=1,step=0;
            
            for(idx = hashidx%tableSize; i<tableSize && entryStates[idx] != EMPTY;
                idx = (idx + step)%tableSize, i++){
                if(entryStates[idx] == OCCUPIED && table[idx].front().first == key)
                    return HashTable<K,V>::marker(*this, idx, table[idx].begin(), i);
                if(collisionResolutor == "double_hash"){step = auxidx;}
                else{
                    step = ((userConstants.first*auxidx*i) + (userConstants.second*i*i));
                    if(!step%tableSize) step++;
                }
            }
            return end(i);
            
        }
        
        bool remove(K key){
            pair<ull,ull> pr = collisionRes(key);
            ull idx = pr.first;
            auto todel = table[idx].end();
            for(auto it = table[idx].begin(); it!=table[idx].end(); it++){
                if(it->first == key){
                    todel = it; break;
                }
            }
            if(todel == table[idx].end()) return false; 
            table[idx].erase(todel);
            if(table[idx].empty()) entryStates[idx] = DELETED;
            elemCount--;
            if(elemCount < loadFactor*tableSize/2){
                ll nsize = *(lower_bound(primes.begin(), primes.end(), tableSize/2));
                rehash(nsize);
            }
            return true;
        }
        V get(K key){
            auto it = search(key);
            if(it == end()) throw runtime_error("key_not_found");
            return it->second;
        }
        void clear(){
            tableSize = 13;
            table.clear();
            table.assign(tableSize, list<pair<K,V>>());
            elemCount = 0;
            entryStates.assign(tableSize, EMPTY);
        }
};

// int main(){
//     HashTable<string,int> h(13, HashTable<string,int>::hash1, "chain", 2.0);
//     cout<<h.insert("hello", 1)<<endl;  
//     cout<<h.insert("world", 2)<<endl;
//     cout<<h.insert("foo", 3)<<endl;
//     cout<<h.insert("bar", 4)<<endl;
//     cout<<h.insert("baz", 5)<<endl;
//     cout<<h.insert("qux", 6)<<endl;
//     cout<<h.insert("quux", 7)<<endl;
//     cout<<h.insert("corge", 8)<<endl;
//     cout<<h.insert("grault", 9)<<endl;
//     cout<<h.insert("garply", 10)<<endl;
//     cout<<h.insert("waldo", 11)<<endl;
//     cout<<h.insert("fred", 12)<<endl;   
//     cout<<h.insert("plugh", 13)<<endl;
//     cout<<h.insert("xyzzy", 14)<<endl;  
//     cout<<h.insert("thud", 15)<<endl;
//     cout<<h.insert("mno", 16)<<endl;
//     cout<<h.insert("pqr", 17)<<endl;
//     auto it = h.search("waldo");
//     cout << "Total Collisions so far: " << h.totalColl << endl;
//     cout << "colls in search "<<it.colcnt << endl;
//     it = h.search("plugh"); cout << "colls in search "<<it.colcnt << endl;
//     it = h.search("hello"); cout << "colls in search "<<it.colcnt << endl;
//     if(it != h.end()){
//         cout << "Found: " << it->first << " -> " << it->second << endl;
//     } else {
//         cout << "Not Found" << endl;
//     }
//     h.remove("waldo");
//     it = h.search("waldo"); cout << "\tcolls in search "<<it.colcnt << endl;
//     if(it != h.end()){
//         cout << "Found: " << it->first << " -> " << it->second << endl;
//     } else {
//         cout << "Not Found" << endl;
//     }
//     it = h.search("hello"); cout << "colls in search "<<it.colcnt << endl;
//     if(it != h.end()){  
//         cout << "Found: " << (*it).first << " -> " << (*it).second << endl;
//     } else {
//         cout << "Not Found" << endl;
//     }
//     h.remove("hello");
//     h.remove("world");
//     h.remove("foo");
//     h.remove("bar");
//     h.remove("baz");
//     h.remove("quux");
//     h.remove("corge");
//     h.remove("grault");
//     h.remove("garply");
//     h.remove("waldo");
//     h.remove("fred");
//     h.remove("plugh");
//     cout << h.remove("hello") << endl; 
//     return 0;
// }