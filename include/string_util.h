#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <vector>
#include <string>

inline std::vector<std::string> split(std::string line, std::string delimiter){
	std::vector<std::string> vec;
	int i=0;
	int j=0;
	while (j<line.length()){
		if (line.substr(j, 1)==delimiter){
			vec.push_back(line.substr(i, j-i));
			i=j+1;
		}
		j++;

	}
	
	vec.push_back(line.substr(i,j-i));
	return vec;
	
}

#endif

