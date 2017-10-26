#include "msort.h"
#include <iostream>
#include <vector>

using namespace std;
struct comp1
{
	bool operator()(const int& a,const int& b)
	{
		return a<b;
	}
};
struct comp2
{
	bool operator()(const string& a,const string& b)
	{
		return a<b;
	}
};

int main(int argc, char const *argv[])
{
	string myints[]={"joe","harry","diego","ball","wenxuan"};
	vector<string> a1(myints, myints+5);
	vector<int> a2;
	a2.push_back(2);
	a2.push_back(1);
	for (unsigned int i=0;i<a2.size();i++)
		cout<<a2[i]<<" ";
	cout<<endl;
	mergesort(a2,comp1());
	for (unsigned int i=0;i<a2.size();i++)
		cout<<a2[i]<<" ";
	cout<<endl;
	return 0;
}