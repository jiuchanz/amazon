#include <vector>
#include <iostream>
using namespace std;

template<class T, class Comparator>
void merge(vector<T>& mylist, vector<T>& output,int s1, int e1, int s2, int e2,Comparator comp)
{
	for(int i=s1;i<e2;i++)
	{
		if(s1==e1)
		{
			
			output[i]=mylist[s2];
			s2++;
		}
		else if(s2==e2)
		{
			
			output[i]=mylist[s1];
			s1++;
			
		}
		else
		{
			
			if(comp(mylist[s1],mylist[s2]))
			{
				output[i]=mylist[s1];
				s1++;
			}
			else
			{
				output[i]=mylist[s2];
				s2++;
			}
			
			
		}
	}
	mylist=output;
}

template<class T,class Comparator>
void msort(vector<T>& mylist,
			vector<T>& output,
			int start, int end,
			Comparator comp)
{
	
	if((end-start)==1)
		return;
	else
	{
		int mid = (start+end)/2;
		msort(mylist, output, start, mid,comp);
		msort(mylist, output, mid, end,comp);
		merge(mylist, output, start, mid, mid, end,comp);
	}
}

template<class T, class Comparator>
void mergesort(vector<T>& myArray, Comparator comp)
{
	if(myArray.size()<=1)
		return;
	vector<T> other(myArray);
	msort(other,myArray,0,(int) myArray.size(),comp);
}




