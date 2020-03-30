#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <iostream>

using namespace boost::uuids;

int main()
{
	// uuid generator example
	random_generator gen;
	uuid id = gen();
	std::cout << id << '\n';
	// convert uuid to string
	std::string s = to_string(id);
	std::cout << s << '\n';
	std::cout << boost::lexical_cast<std::string>(id) << '\n';
	
	return 0;
}