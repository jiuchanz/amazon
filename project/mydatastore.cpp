#include "mydatastore.h"
#include <iostream>

void MyDataStore::addReview(Review* r)
{
	reviewMap[r->prodName].push_back(r);
}
void MyDataStore::addProduct(Product* p)
{
	productSet.insert(p);
	//insert into keyword map to search
	std::set<std::string> prodKeyword=p->keywords();
	for (std::set<std::string>::iterator it=prodKeyword.begin();
		it!=prodKeyword.end();it++)
	{
		keywordMap[*it].insert(p);
	}
}

void MyDataStore::addUser(User* u)
{
	userMap.insert(std::pair<std::string,User*>(u->getName(),u));
}


MyDataStore::~MyDataStore()
{
	for(std::map<std::string,User*>::iterator it=userMap.begin();
		it!=userMap.end();it++)
		delete it->second;
	for(std::set<Product*>::iterator it=productSet.begin();
		it!=productSet.end();it++)
		delete *it;
	for(std::map<std::string,std::vector<Review*> >::iterator it=reviewMap.begin();
		it!=reviewMap.end();it++)
	{
		std::vector<Review*> vect=it->second;
		for(unsigned int i=0;i<vect.size();i++)
		{
			delete vect[i];
		}
	}
}

/**
     * Performs a search of products whose keywords match the given "terms"
     *  type 0 = AND search (intersection of results for each term) while
     *  type 1 = OR search (union of results for each term)
     */
std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type) 
{
	std::set<Product*> found_set;
	if (type) //OR 
	{	
		

		for(unsigned int i=0;i<terms.size();i++)
		{
			
			if(keywordMap.find(terms[i])!=keywordMap.end())
			{
			
				found_set=setUnion(found_set,keywordMap[terms[i]]);
			}
		
		}


	}
	else //AND 
	{
		bool first_iteration=1;
		for(unsigned int i=0;i<terms.size();i++)
		{
			std::set<Product*> temp;
			if(keywordMap.find(terms[i])!=keywordMap.end())
			{
				temp=keywordMap[terms[i]];
			}
			if(first_iteration)
			{
				found_set=temp;
				first_iteration=0;
				continue;
			}
			found_set=setIntersection(found_set,temp);
		}
	}

	std::vector<Product*> found_vector;
	for(std::set<Product*>::iterator it=found_set.begin();
		it!=found_set.end();it++)
	{
		found_vector.push_back(*it);
	}
	
	return found_vector;


}


 /**
     * Reproduce the database file from the current Products and User values
     */
void MyDataStore::dump(std::ostream& ofile) 
{
	//product
	ofile<<"<products>"<<'\n';
	for(std::set<Product*>::iterator it=productSet.begin();
		it!=productSet.end();it++)
	{
		(*it)->dump(ofile);
	}
	ofile<<"</products>"<<'\n';

	//user
	ofile<<"<users>"<<'\n';
	for(std::map<std::string,User*>::iterator is=userMap.begin();
		is!=userMap.end();is++)
	{
		(is->second)->dump(ofile);
	}
	ofile<<"</users>"<<'\n';
	//review
	ofile<<"<reviews>"<<'\n';
	for(std::map<std::string,std::vector<Review*> >::iterator is=reviewMap.begin();
		is!=reviewMap.end();is++)
	{
		for(unsigned int i=0;i<is->second.size();i++)
		{
			ofile<<is->second[i]->prodName<<'\n';
			ofile<<is->second[i]->rating<<' '<<is->second[i]->username<<' '<<is->second[i]->date<<' '<<is->second[i]->reviewText<<'\n';
		}
	}
	ofile<<"</reviews>"<<'\n';
}	


void MyDataStore::addCart(
	std::string userName,unsigned int hitNum,
	const std::vector<Product*>& searchResult)
{
	if(userMap.find(userName)==userMap.end())
	{
		std::cout<<"Invalid request\n";
		return;
	}
	if((hitNum<1)||(hitNum>searchResult.size()))
	{
		std::cout<<"Invalid request\n";
		return;
	}
	hitNum--; //index start at 0
	cartMap[userName].push(searchResult[hitNum]);
	//searchResult[hitNum]->subtractQty(1);
	//userMap[userName]->deductAmount(searchResult[hitNum]->getPrice());
}

void MyDataStore::viewCart(std::string userName)
{	
	if(userMap.find(userName)==userMap.end())
	{
		std::cout<<"Invalid username\n";
		return;
	}
	std::queue<Product*> queueCopy(cartMap[userName]);
	int size_temp=queueCopy.size();
	for(int i=0;i<size_temp;i++)
	{
		std::cout<<"Item "<<i+1<<'\n';
		std::cout<<(queueCopy.front())->displayString()<<std::endl;
		queueCopy.pop();
	}
	std::cout<<std::endl;
}

void MyDataStore::buyCart(std::string userName) 
{
	if(userMap.find(userName)==userMap.end())
	{
		std::cout<<"Invalid username\n";
		return;
	}
	std::queue<Product*> cartNoPay;
	std::queue<Product*>& cart=cartMap[userName];
	while(cart.size()>0)
	{
		if((cart.front()->getQty()<=0)||
			(userMap[userName]->getBalance()<cart.front()->getPrice()))
		{
			cartNoPay.push(cart.front());
			cart.pop();
			continue;
		}
		cart.front()->subtractQty(1);
		userMap[userName]->deductAmount(cart.front()->getPrice());
		cart.pop();
	}
	while(cartNoPay.size()!=0)
	{
		cart.push(cartNoPay.front());
		cartNoPay.pop();
	}

}

std::vector<std::string> MyDataStore::userNameList()
{
	std::vector<std::string> list;
	for(std::map<std::string,User*>::iterator it=userMap.begin();it!=userMap.end();it++)
	{
		list.push_back(it->first);
	}
	return list;
}

double MyDataStore::getRating(std::string str)
{
	double ratingTotal=0;
	std::vector<Review*> reviews=reviewMap[str];
	if(reviews.size()==0)
		return ratingTotal;
	for(unsigned int i=0;i<reviews.size();i++)
	{
		ratingTotal+=reviews[i]->rating;
	}
	return ratingTotal/reviews.size();
}

std::vector<std::string> MyDataStore::cartNameList(std::string userName)
{
	
	std::vector<std::string> nameVector;
	std::queue<Product*> productQue=cartMap[userName];
	while(!productQue.empty())
	{
		nameVector.push_back(productQue.front()->getName());
		productQue.pop();
	}
	return nameVector;
}

void MyDataStore::removeCartItem(int removeIndex,std::string userName)
{
	std::queue<Product*> queueTemp;
	int i=0;
	while(!cartMap[userName].empty())
	{
		if(i!=removeIndex)
			queueTemp.push(cartMap[userName].front());
		cartMap[userName].pop();
		i++;
	}
	cartMap[userName]=queueTemp;
}
std::vector<Review*> MyDataStore::getReviews(std::string prodName)
{
	return reviewMap[prodName];
}