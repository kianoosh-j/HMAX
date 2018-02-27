#include <iostream>
#include <vector>
#include <list>
#include <string>


using namespace std;


int main(){


	vector<int> a;

	list<int> b;

	for ( int i=0 ; i<10  ;i++ ){
		b.push_back(i*i);
	}

	a.insert ( a.end() , b.begin() , b.end() );

	for ( vector<int>::iterator iter = a.begin() ; iter != a.end() ; iter++)
		cout<<*iter<<endl;

	return 0;
	
}
