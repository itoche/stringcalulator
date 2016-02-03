#pragma once

#include <algorithm>

// From http://stackoverflow.com/a/1493195/1448419
template < class ContainerT >
void tokenize(const std::string& str, ContainerT& tokens,
		const std::string& delimiters = " ", bool trimEmpty = false)
{
	std::string::size_type pos, lastPos = 0;

	using value_type = typename ContainerT::value_type;
	using size_type  = typename ContainerT::size_type;

	while(true)
	{
		pos = str.find_first_of(delimiters, lastPos);
		if(pos == std::string::npos)
		{
			pos = str.length();

			if(pos != lastPos || !trimEmpty)
				tokens.push_back(value_type(str.data()+lastPos,
						(size_type)pos-lastPos ));

			break;
		}
		else
		{
			if(pos != lastPos || !trimEmpty)
				tokens.push_back(value_type(str.data()+lastPos,
						(size_type)pos-lastPos ));
		}

		lastPos = pos + 1;
	}
}


// From http://stackoverflow.com/a/6500499/1448419
inline std::string trim(const std::string& str, const std::string &delimiters = " ")
{
	std::string newString(str);
	newString.erase(0, newString.find_first_not_of(delimiters));       //prefixing spaces
	newString.erase(newString.find_last_not_of(delimiters)+1);         //suffixing spaces
	return newString;
}

