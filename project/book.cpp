#include "book.h"
#include <sstream>
std::set<std::string> Book::keywords() const
{
	std::set<std::string> keySet(parseStringToWords(name_));
	keySet.insert(isbn_);
	std::set<std::string> author_name_key(parseStringToWords(author_));
	keySet.insert(author_name_key.begin(),author_name_key.end());
	return keySet;
}

std::string Book::displayString() const
{
	std::stringstream i;
	i<<name_<<std::endl<<"Author: "<<author_<<" ISBN: "<<isbn_<<std::endl;
	i<<price_<<' '<<qty_<<" left.";
	return i.str();
}


Book::Book(const std::string category, const std::string name, 
		double price, int qty, std::string isbn, std::string author)
		: Product(category,name,price,qty),isbn_(isbn),author_(author)
{
}

void Book::dump(std::ostream & os) const
{
	Product::dump(os);
	os<<isbn_<<'\n'<<author_<<'\n';
}
