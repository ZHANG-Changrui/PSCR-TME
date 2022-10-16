#include <iostream>
#include <forward_list>
#include <fstream>
#include <regex>
#include <chrono>
using namespace std;
using namespace std::chrono;
//question 5
template<typename K,typename V>
class HashMap{
public:
    class Entry{
    public:
        const K key;
        V value;
        Entry(const K k,const V v):key(k),value(v){}
        void toString(){
            cout<<"key: "<<key<<"value: "<<value<<endl;
        }
    };
        /*
    class iterator{
    public:
        typedef vector<forward_list<Entry>> buckets_t;
        typedef forward_list<Entry> iter_t;
        buckets_t *buckets;
        int index;
        iterator(buckets_t &b):buckets(&b),index(0){}

        //It(buckets_t b):buckets(b){};
        iter_t* begin(){
            return &buckets->at(0);
        }
        iter_t* end(){
            return &(buckets->at(buckets->size()-1));
        }
        iter_t* operator()(){
            return &(buckets->at(index));
        }
        iter_t* operator++(){
            ++index;
            return &(buckets->at(index));
        }
    };
     */
    typedef vector<forward_list<Entry>> buckets_t;
    typedef forward_list<Entry> bucket_in;
    buckets_t buckets;
    size_t init;
    size_t nbrElm;
    int index_buckets=0;
    //buckets_t &iter=buckets;
    //bucket_in *vit;
    //entry_iterator lit;
    HashMap(size_t init=100):init(100){
        buckets.reserve(init);
        for(size_t i=0;i<init;++i){
            buckets.push_back(forward_list<Entry>());
        }
        //vit=&iter.at(index_buckets);
        //lit=vit->begin();
    }
    V* get(const K& k){
        size_t index=hash<K>()(k);
        index=index%this->init;
        forward_list<Entry> l=this->buckets.at((int)index);
        for(auto& i:l){
            if(i.key==k){
                //cout<<"v: "<<i.value<<endl;
               // cout<<"val "<<i.value<<endl;
                return new V(i.value);
            }
        }
        return new V(0);
    }
    /*
    void put(const K k,const V v){
        size_t index=hash<K>()(k);
        index=index%this->init;
        this->buckets.at(index).remove_if([k] (Entry e){
            return e.key==k;
        });
        this->buckets.at(index).push_front(Entry(k,v));
    }*/
    bool put(const K k,const V v){
        size_t index=hash<K>()(k) % this->init;
        for(auto &e:buckets.at(index)){
            if(e.key==k){
                e.value=v;
                return true;
            }
        }
        buckets.at(index).push_front(Entry(k,v));
        nbrElm++;
        return false;
    }
    class iterator{
    public:
        typename buckets_t::iterator end;
        typename buckets_t::iterator vit;
        typename bucket_in::iterator lit;
        iterator(const typename buckets_t::iterator &end,const typename buckets_t::iterator &vit,const typename bucket_in::iterator &lit):end(end),vit(vit),lit(lit){
        }
        iterator& operator++(){
            ++lit;
            if(lit==vit->end()){
                ++vit;

                while(vit->empty()){
                    ++vit;
                }
                if(vit!=end){
                    lit=vit->begin();
                }
            }
            return *this;
        }
        Entry& operator*(){
            return *lit;
        }
        bool operator!=(iterator i){
            return lit != i.lit|| vit!=i.vit;
        }
    };
   /* iterator begin() {
        typename buckets_t::iterator vit = buckets.begin();
        while (vit->empty() && vit != buckets.end()) {
            ++vit;
        }
        if (vit != buckets.end()) {
            return iterator(buckets.end(),vit,vit->begin());
        } else {
            return end();
        }
    }*/
    iterator begin(){
        typename buckets_t::iterator vit=buckets.begin();
        return iterator(buckets.end(),vit,vit->begin());
    }
    iterator end(){
        typename buckets_t::iterator vit=buckets.end();
        //cout<<"voici end: lit"<<e.value<<endl;
        return iterator(buckets.end(),vit,vit->end());
    }
};
void printMap(HashMap<string,int> m){
    cout<<"-----affiche---------"<<endl;
    int line=0;
    for(auto &i:m.buckets){
        cout<<line++<<" size:"<<distance(i.begin(),i.end())<<" - ";
        for(auto &j:i){
            cout<<j.key<<":"<<j.value<<" | ";
        }
        cout<<endl;
    }
    cout<<"----end affiche-------"<<endl;
}
bool cond(pair<string,int> p){
    return p.first=="war";
}
void affiche3Occur(vector<pair<string,int>> v){
    cout<<"taille vector: "<<v.size()<<endl;
    for(auto i:v){
        cout<<"|"<<i.first<<"|:"<<i.second<<endl;
    }
}
template<typename iterator>
size_t count(iterator begin,iterator end){
    //size_t size=end-begin;
    size_t cmp=0;
    for(auto it=begin,_end=end;it!=_end;it++){
        cmp++;
    }
    return cmp;
}
template<typename iterator,typename T>
size_t count_if_equal(iterator begin,iterator end,const T& val){
    size_t cmp=0;
    if(count(begin,end)==val){
        cmp++;
    }
    return cmp;
}
int main () {
    {
        ifstream input = ifstream("WarAndPeace.txt");
        //auto start = steady_clock::now();
        cout << "Parsing War and Peace" << endl;
        size_t nombre_lu = 0;
        // prochain mot lu
        string word;
        // une regex qui reconnait les caractères anormaux (négation des lettres)
        regex re(R"([^a-zA-Z])");
//TODO
        cout<<"-------------------1----------"<<endl;

        //vector<pair<string,int>> v;
        HashMap<string, int> map;
        cout<<"-----------------------------"<<endl;

        while (input >> word) {
            // élimine la ponctuation et les caractères spéciaux
            word = regex_replace(word, re, "");
            // passe en lowercase
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            // word est maintenant "tout propre"
//TODO
            /* exo2
            if(std::find(v.begin(),v.end(),word)==v.end()){
                v.push_back();
            }
            Question 3
            if(word=="toto"||word=="war"||word=="peace"){
                if(v.empty()){
                    pair<string,int> tmp(word,1);
                    v.push_back(tmp);
                }else{
                    const size_t sz=v.size();
                    bool t= false;
                    for(size_t i=0;i<sz;i++){
                        if(v.at(i).first==word){
                            t= true;
                            pair<string,int> tmp(word,v.at(i).second+1);
                            v.at(i)=tmp;
                        }
                    }
                    if(!t){
                        pair<string,int> tmp(word,1);
                        v.push_back(tmp);
                    }
                }
            }
    */
//question 6

           // if (word == "toto" || word == "war" || word == "peace") {
            int occr=*map.get(word)+1;
            //cout<<"mot: "<<word<<" taille: "<<occr<<endl;
            //cout<<occr<<endl;
            map.put(word,occr);
            //cout << word << ": " << *map.get(word) << endl;


             //}
            /*
            if (nombre_lu % 100 == 0)
                // on affiche un mot "propre" sur 100
                cout << nombre_lu << ": "<< word << endl;*/
            nombre_lu++;
        }

        std::vector<pair<string,int>> v;
        for(auto i:map.buckets){
            for(auto j:i){
                v.push_back(make_pair(j.key,j.value));
            }
        }

/*
        cout << "Finished Parsing War and Peace" << endl;
        auto end = steady_clock::now();
        cout << "Parsing took "
             << duration_cast<milliseconds>(end - start).count()
             << "ms.\n";
        cout << "Found a total of " << nombre_lu << " words." << endl;
        //cout << "taille mots: " <<v.size()<<endl;
        //affiche3Occur(v);
        int *toto = map.get("toto");
        int *war = map.get("war");
        cout << "occurence toto: " << *toto << endl;
        cout << "occurence war: " << *war << endl;
        cout << "occurence peace: " << *map.get("peace") << endl;
        //printMap(map);
        input.close();*/
        //int cmp=0;
        //for(auto i:map.buckets){
        //    cmp+=count(i.begin(),i.end());
        //}
        //cout<<"voici la taille: "<<count(map.buckets.begin(),map.buckets.end())<<"et cmp: "<<cmp<<endl;
        cout<<"taille v: "<<v.size()<<endl;
        std::sort(v.begin(),v.end(),[](pair<string,int> p1,pair<string,int> p2){
            return p1.second>p2.second;
        });
        for(auto i=v.begin();i< v.begin()+10;i++){
            cout<<"first: "<<i->first<<" seconde: "<<i->second<<endl;
        }
        //cout<<"before end"<<endl;
        //cout<<map.buckets.end()->end()->value<<endl;
        //auto end=map.end();
        //cout<<"after end"<<endl;
        //cout<<(*end).key<<" : "<<(*end).value<<endl;

        for(auto it=map.begin();it!=map.end();++it){
            cout<<"Key: "<<(*it).key<<" valeur: "<<(*it).value<<endl;
        }
        /*
        auto it = map.iterator;
        for(auto i=it.begin(),_end=it.end();i!=_end;i++){
            for(auto j=i->begin(),__end=i->end();j!=__end;j++){
                cout<<"key: "<<j->key<<" value: "<<j->value<<endl;
            }
        }
        */

    }
}