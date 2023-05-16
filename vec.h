#include <memory>
#include <algorithm>

#include <iostream>
using namespace std;

template <class T> class vec {

public:
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef size_t size_type;
    typedef T value_type;

    // konstruktoriai
    vec() { create(); }                                                         //default
    explicit vec (size_type n, const T& value = T{}) { create(n, value); }      //size n, filled with value
    vec (const vec& v) { create(v.begin(), v.end()); }                          //copy
    vec(std::initializer_list<T> init){create(init.begin(), init.end());}       //initializer list


    vec& operator=(const vec&);                                                 //priskyrimo operatorius
    ~vec(){ uncreate(); }                                                       //destruktorius


    // size, capacity
    size_type size() const { return avail - data; }
    size_type capacity() const { return limit-data; }


    // indeksavimas [ ]
    T& operator[](size_type i) { return data[i]; }
    const T& operator[](size_type i) const { return data[i]; }

    // front, back, at
    T& front() { if (data) return data[0]; else {cout<<"vec is empty"; throw(1);}}
    T& back() { if (data) return *(avail-1); else {cout<<"vec is empty"; throw(1);}}
    T& at(int index){
        if ((data+index)<avail) return data[index];
        else {cout<<"vec element does not exist"; throw(1);}
    }
    
    // iteratoriai
    iterator begin() { return data; }
    const_iterator begin() const { return data; }
    iterator end() { return avail; }
    const_iterator end() const { return avail; }


    // empty
    bool empty(){if (data) return false; else return true;}

    // clear
    void clear(){uncreate();}


    // push_back
    void push_back(const T& val) {
    if (avail == limit) grow(); // increase capacity
    unchecked_append(val) ;     // add new element
    }

    // pop_back
    void pop_back() {
        if (avail!=data+1) {avail--; alloc.destroy(avail);}
        else clear();
    }


    // resize
    void resize( int count, T value=0){
        int diff =0;
        if (count<size()){
            diff=size()-count;
            for(int a=0; a<diff; a++){ pop_back();}
        }
        else if (count>size()){
            diff=count-size();
            for(int a=0; a<diff; a++){ push_back(value);}
        }
    }

    // shrink to fit
     void shrink_to_fit(){
        size_type temp_size = size();
        iterator temp_data = alloc.allocate(temp_size);
        iterator temp_avail = std::uninitialized_copy(data, avail, temp_data);

        uncreate();

        data = temp_data;
        avail = temp_avail;
        limit = data + temp_size;  
     }

    // reserve
     void reserve(int newSpace){
    if (newSpace>=size()){
        size_type temp_size = newSpace;
        iterator temp_data = alloc.allocate(temp_size);
        iterator temp_avail = std::uninitialized_copy(data, avail, temp_data);

        uncreate();

        data = temp_data;
        avail = temp_avail;
        limit = data + temp_size;  }
     }

     // swap
     void swap( vec& other ){
        iterator tempdata;
        iterator tempavail;
        iterator templimit;

        tempavail=other.avail;
        templimit=other.limit;
        tempdata=other.data;

        other.avail=avail;
        other.limit=limit;
        other.data=data;

        avail=tempavail;
        limit=templimit;
        data=tempdata;
     };

    // erase
    void erase( iterator elem, iterator until=NULL ){   //until is not removed!
        if (until==NULL) {until=elem+1;}

        if (elem==end()){pop_back();}
        else if (elem==begin()){for (int a=0; a<until-elem;a++) {data++; alloc.destroy(data-1);} }
        else {  int newsize=size()-(until-elem);

                T* temparray= new T[newsize];
                iterator ptr;
                for (ptr=data; ptr<elem; ptr++){
                    temparray[ptr-data]=*ptr;
                }

                for (ptr=elem+(until-elem); ptr<avail; ptr++){
                    temparray[ptr-data-(until-elem)]=*ptr;
                }

                iterator newdata=alloc.allocate(newsize);
                iterator newavail=std::uninitialized_copy(temparray,temparray+newsize, newdata);

                uncreate();

                data = newdata;
                avail = newavail;
                limit = data + newsize;

                delete [] temparray;
            }
    };

    void erase_if(T value){
        for (int a=0; a<size(); a++){
            if(at(a)==value) {
                erase(begin()+a);
                a--;
            }
        }
    };


    // insert
    void insert( int index, T value ){
        if (index==size()) push_back(value);
        else {
            iterator indexx=data+index;

            int newsize=size()+1;

            T* temparray= new T[newsize];
            iterator ptr;
            for (ptr=data; ptr<indexx; ptr++){
                temparray[ptr-data]=*ptr;
            }

            temparray[index]=value;

            for (ptr=indexx++; ptr<avail; ptr++){
                temparray[ptr-data+1]=*ptr;
            }

            iterator newdata=alloc.allocate(newsize);
            iterator newavail=std::uninitialized_copy(temparray,temparray+newsize, newdata);

            uncreate();

            data = newdata;
            avail = newavail;
            limit = data + newsize;

            delete [] temparray;

        }

    };



    // comparison operators
    bool operator==(vec<T>& other){
        if (size()!=other.size()) return false;
        for (int a=0; a<size(); a++){
            if (at(a)!=other.at(a)) return false;
        }
        return true;
    }

    bool operator!=(vec<T>& other){
        if ((*this==other)==0) return true;
        return false;
    }

    bool operator<(vec<T>& other){
        int siz=size();
        if (size()>other.size()) siz=other.size();

        for (int a=0; a<siz; a++){
            if (at(a)<other.at(a)) return true;
        }
        return false;
    }

    bool operator<=(vec<T>& other){
        int siz=size();
        if (size()>other.size()) siz=other.size();

        for (int a=0; a<siz; a++){
            if (at(a)<=other.at(a)) return true;
        }
        return false;
    }

    bool operator>(vec<T>& other){
        int siz=size();
        if (size()>other.size()) siz=other.size();

        for (int a=0; a<siz; a++){
            if (at(a)>other.at(a)) return true;
        }
        return false;
    }

    bool operator>=(vec<T>& other){
        int siz=size();
        if (size()>other.size()) siz=other.size();

        for (int a=0; a<siz; a++){
            if (at(a)>=other.at(a)) return true;
        }
        return false;
    }



    //print
    void print(string optional=""){
        if (optional!="") cout<<"\n"<<optional<<":";
        cout<<"\nsize - "<<size()<<
        "\ncapacity - "<<capacity()<<
        "\nelements - ";

        for (int a=0; a<size(); a++) cout<<at(a)<<" ";
        cout<<endl;
    }





private:
    iterator data;      //first element
    iterator avail;     //last filled element + 1
    iterator limit;     //last possible element + 1

    std::allocator<T> alloc; // objektas atminties valdymui

    //create
    void create() ;
    void create(size_type, const T&);
    void create(const_iterator, const_iterator);

    void uncreate();


    void grow();
    void unchecked_append(const T&);
};




//realizacijos
    //create
    template <class T> void vec<T>::create() {
        data = avail = limit = nullptr;
    }
    
    template <class T> void vec<T>::create(size_type n, const T& val) {
        data = alloc.allocate(n); // grąžina ptr į array pirmą elementą
        limit = avail = data + n; // sustato rodykles į vietas
        std::uninitialized_fill(data, limit, val); // inicializuoja elementus val reikšme
    }
    
    template <class T>
        void vec<T>::create(const_iterator i, const_iterator j) {
        data = alloc.allocate(j - i); // išskirti vietos j-i elementams
        limit = avail = std::uninitialized_copy(i, j, data); // nukopijuoja elementus iš intervalo
    }

    //uncreate
    template <class T> void vec<T>::uncreate() {
        if (data) {
            iterator it = avail;
            while (it != data) 
            alloc.destroy(--it);                    //destroy elements (backwards)
            alloc.deallocate(data, limit - data);       // free memory
        }

        data = limit = avail = nullptr;                 //reset pointers
    }




    //grow
    template <class T> void vec<T>::grow() {

        size_type new_size = std::max(2 * (limit - data), ptrdiff_t(1));        //double capacity
        iterator new_data = alloc.allocate(new_size);
        iterator new_avail = std::uninitialized_copy(data, avail, new_data);

        uncreate();

        data = new_data;
        avail = new_avail;
        limit = data + new_size;
    }


    // unchecked_append
    template <class T> void vec<T>::unchecked_append(const T& val) {
        alloc.construct(avail++, val);
    }

    // priskyrimo operatoriaus realizacija
    template <class T>  vec<T>& vec<T>::operator=(const vec& rhs) {
        if (&rhs != this) {
            uncreate(); //atlaisvina seną atmintį
            create(rhs.begin(), rhs.end()); //copy iš rhs į lhs (this)
        }
        return *this;
    }