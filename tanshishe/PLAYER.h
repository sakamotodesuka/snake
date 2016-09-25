#include<string>
#include<stack>
using namespace std;

class PLAYER{
public:
	string playername;
	int score;
	static int nplayer;
	PLAYER();
	PLAYER(const string&);
};