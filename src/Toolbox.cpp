using namespace std;
#include "Toolbox.hpp"
#include <vector>
#include <string>

Toolbox::Toolbox() {
	// please do not recycle this.
}
vector<string> Toolbox::explode(const string &str, const string &delim) {
	vector<string> arr;
	int i = 0;
	int k = 0;
	int strlen = str.length();
	int dellen = delim.length();
	if(dellen == 0)
		return arr;
	while(i < strlen) {
		int j = 0;
		while(i + j < strlen && j < dellen && str[i + j] == delim[j])
			j++;
		if(j == dellen) {
			arr.push_back(str.substr(k, i - k));
			i += dellen;
			k = i;
		} else {
			i++;
		}
	}
	arr.push_back(str.substr(k, i - k));
	return arr;
}

