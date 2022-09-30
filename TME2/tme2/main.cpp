#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
using namespace std;
using namespace std::chrono;

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
    return 0;
}


