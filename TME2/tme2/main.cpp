#include <iostream>
#include <forward_list>
#include <fstream>
#include <regex>
#include <chrono>
using namespace std;
using namespace std::chrono;


template<typename K,typename V>
class HashMap{
public:
    class Entry{
    public:
        const K key;
        const V value;
        Entry(const K k,const V v):key(k),value(v){}
        void toString(){
            cout<<"key: "<<key<<"value: "<<value<<endl;
        }
    };
    typedef vector<forward_list<Entry>> buckets_t;
    buckets_t buckets;
    size_t init;
    HashMap(size_t init=100):init(100){
        buckets.reserve(init);
        for(size_t i=0;i<init;++i){
            buckets.push_back(forward_list<Entry>());
        }
    }
    V* get(const K& key){
        size_t index=std::hash<K>()(key);
        index=index%this->init;
        forward_list<Entry> l=this->buckets.at(index);
        V *r=new V;
        for(auto i:l){
            if(i.key==key){

                *r=i.value;
                return r;
            }
        }
        *r="none";
        return r;
    }
    void put(const K k,const V v){
        int index=hash<K>()(k);
        cout<<"init: "<<init<<endl;
        cout<<"first index : "<<index<<endl;
        index=index%init;
        cout<<"index: "<<index<<endl;
        this->buckets.at(index).remove_if([k] (Entry e){
            return e.key==k;
        });
        this->buckets.at(index).push_front(Entry(k,v));
    }
};
void printMap(HashMap<int,string> m){
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

int main () {
	ifstream input = ifstream("WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
    vector<pair<string,int>> v;


	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
        // passe en lowercase
        transform(word.begin(),word.end(),word.begin(),::tolower);
/* exo2
        if(std::find(v.begin(),v.end(),word)==v.end()){
            v.push_back();
        }
*/


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





		// word est maintenant "tout propre"
        /*
		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": "<< word << endl;*/
		nombre_lu++;
	}
    input.close();

	cout << "Finished Parsing War and Peace" << endl;
	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";
    cout << "Found a total of " << nombre_lu << " words." << endl;
    //cout << "taille mots: " <<v.size()<<endl;

    affiche3Occur(v);

    HashMap<string,int> h(100);


    return 0;
}


