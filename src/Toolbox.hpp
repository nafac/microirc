using namespace std;
#include <vector>
#include <string>

class Toolbox {
	// empty
public:
	Toolbox();
	vector<string> explode(const string &str, const string &delim);
	bool Contains(const string haystack, const string needle);
};

