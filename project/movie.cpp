#include "movie.h"
#include <sstream>
std::set<std::string> Movie::keywords() const
{
	std::set<std::string> keySet(parseStringToWords(name_));
	keySet.insert(convToLower(genre_));
	return keySet;
}

std::string Movie::displayString() const
{
	std::stringstream i;
	i<<name_<<std::endl<<"Genre: "<<genre_<<" Rating: "<<rating_<<std::endl;
	i<<price_<<' '<<qty_<<" left.";
	return i.str();
}

Movie::Movie(const std::string category, const std::string name, 
		double price, int qty, std::string genre, std::string rating)
		: Product(category,name,price,qty),genre_(genre),rating_(rating)
{

}

void Movie::dump(std::ostream& os) const
{
	Product::dump(os);
	os<<genre_<<std::endl<<rating_<<std::endl;
}