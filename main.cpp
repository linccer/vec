#include <iostream>
#include "vec.h"
#include <vector>
#include <string>
#include <chrono>

using std::vector;
using std::boolalpha;
using std::cout;
using std::endl;

void printv(vector<int> v);
void operatoriu_testavimas();
void kiti_testai();

int main() {

    //operatoriu_testavimas();
    //kiti_testai();
//------------------------------------------------------------------------------------------------------------------------------
    //spartos_analize
    cout<<endl<<endl;
    unsigned int sz[5] {10000, 100000, 1000000, 10000000, 100000000};
    vector<int> v;
    int count=0;

    for (int a=0; a<5; a++){

        //timer
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 1; i <= sz[a]; ++i){ 
            v.push_back(i);
            //if (v.capacity() == v.size()) count++;
        }

        //timer end
        auto stop = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> dur = stop-start;  

        cout<<sz[a]<<" elementu pildymas uztruko   "<<dur.count()<<endl;
        cout<<"atminties perskistymo kartai - "<<count<<endl;

        v.clear(); v.shrink_to_fit();
        count=0;
    }




}





void printv(vector<int> v){

    cout<<"\nvector: \nsize - "<<v.size()<<"\ncapacity - "<<v.capacity()<<"\nelements - ";
    for (int a=0; a<v.size(); a++) cout<<v.at(a)<<" ";
    cout<<endl;
}


void operatoriu_testavimas(){

    // vector<int> alice{1, 2, 3};
    // vector<int> bob{7, 8, 9, 10};
    // vector<int> eve{1, 2, 3};

    vec<int> alice{1, 2, 3};
    vec<int> bob{7, 8, 9, 10};
    vec<int> eve{1, 2, 3};
 
    cout << boolalpha;
 
    // Compare non equal containers
    cout << "alice == bob returns " << (alice == bob) << '\n';
    cout << "alice != bob returns " << (alice != bob) << '\n';
    cout << "alice <  bob returns " << (alice < bob) << '\n';
    cout << "alice <= bob returns " << (alice <= bob) << '\n';
    cout << "alice >  bob returns " << (alice > bob) << '\n';
    cout << "alice >= bob returns " << (alice >= bob) << '\n';
 
    cout << '\n';
 
    // Compare equal containers
    cout << "alice == eve returns " << (alice == eve) << '\n';
    cout << "alice != eve returns " << (alice != eve) << '\n';
    cout << "alice <  eve returns " << (alice < eve) << '\n';
    cout << "alice <= eve returns " << (alice <= eve) << '\n';
    cout << "alice >  eve returns " << (alice > eve) << '\n';
    cout << "alice >= eve returns " << (alice >= eve) << '\n';
};





void kiti_testai(){

    vector<int> vector{1,5,6};
    vec<int> vec{1,5,6};

    vec.print("vec");
    printv(vector);


    vec.push_back(5);
    vector.push_back(5);


    cout<<"\n----------po push_back(5)\n";
    vec.print("vec");
    printv(vector);


    vec.insert(3, 99); //(index, value)
    vector.insert(vector.begin()+3, 99);

    cout<<"\n----------po insert 99 at index 3\n";
    vec.print("vec");
    printv(vector);


    vec.pop_back();
    vector.pop_back();

    cout<<"\n----------po pop_back\n";
    vec.print("vec");
    printv(vector);


    vec.clear();
    vector.clear();

    cout<<"\n----------po clear()\n";
    vec.print("vec");
    printv(vector);
};