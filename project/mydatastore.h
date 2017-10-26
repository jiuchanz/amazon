#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include "datastore.h"
#include "util.h"
#include "user.h"
#include "product.h"
#include <map>
#include <string>
#include <queue>


class MyDataStore: public DataStore
{
public:
	MyDataStore(){};
	~MyDataStore();
	void addProduct(Product* p);
	void addUser(User* u);
	std::vector<Product*> search(std::vector<std::string>& terms, int type);
	void dump(std::ostream& ofile);
	void addCart(std::string userName,unsigned int hitNum,
		const std::vector<Product*>& searchResult);  //hitNum =index+1
	void viewCart(std::string userName);
	std::vector<std::string> cartNameList(std::string userName);
	void buyCart(std::string userName);
	void addReview(Review* r);
	std::vector<std::string> userNameList();
	double getRating(std::string);
	void removeCartItem(int removeIndex,std::string userName);
	std::vector<Review*> getReviews(std::string);

	
private:


	std::set<Product*> productSet;
	std::map<std::string,std::set<Product*> > keywordMap;
	std::map<std::string,User*> userMap; 
	std::map<std::string,std::queue<Product*> > cartMap;
	std::map<std::string,std::vector<Review*> > reviewMap;
};

#endif
