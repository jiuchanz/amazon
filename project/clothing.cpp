#include "clothing.h"
#include <sstream>
Clothing::Clothing(const std::string category, const std::string name, 
		double price, int qty, std::string size, std::string brand)
		:Product(category,name,price,qty),size_(size),brand_(brand)
{

}

std::set<std::string> Clothing::keywords() const
{
	std::set<std::string> keySet(parseStringToWords(name_));
	std::set<std::string> brand_name_key(parseStringToWords(brand_));
	keySet.insert(brand_name_key.begin(),brand_name_key.end());
	return keySet;
}


std::string Clothing::displayString() const
{
	std::stringstream i;
	i<<name_<<std::endl<<"Size: "<<size_<<" Brand: "<<brand_<<std::endl;
	i<<price_<<' '<<qty_<<" left.";
	return i.str();
}

void Clothing::dump(std::ostream& os) const
{
	Product::dump(os);
	os<<size_<<std::endl<<brand_<<std::endl;
}