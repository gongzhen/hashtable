#include <cstdio>
#include <cstdlib>
#include <ctype.h>
#include <iostream>
using std::cerr;
using std::cout;
using std::cin;
using std::endl;

#define MAX 10000000
#define PRIME 5


const char INFILE[] = "test.txt";
struct HashNode
{
    struct HashNode *next;
    int value;
};

class HashTable{
	public:
		HashTable(); /* constructor initialize the hash table */
		~HashTable(); /* destructor */
	private:
		struct HashNode hashNode[PRIME];
		

};

int main()
{
	FILE *fp = fopen(INFILE,"r");
	char line[10];
	long hash[MAX+1];

	if (fp==NULL) 
	{
		cerr<<"Error opening file!"<<endl;
	}
	else{
		while(fgets(line, 10, fp))
		{
			hash[atoi(line)]++;
		}
	}
	return 0;
}