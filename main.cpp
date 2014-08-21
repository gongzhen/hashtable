#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

using std::cerr;
using std::cout;
using std::cin;
using std::endl;
using std::ofstream;
using std::ios;

// #define PRIME 547

static const int  __num_prime = 28;
static const unsigned long __prime_list[__num_prime] = {
	53,         97,           193,         389,       769,
	1543,       3079,         6151,        12289,     24593,
	49157,      98317,        196613,      393241,    786433,
	1572869,    3145739,      6291469,     12582917,  25165843,
	50331653,   100663319,    201326611,   402653189, 805306457, 
	1610612741, 3221225473ul, 4294967291ul	
};

inline unsigned long __get_next_prime(unsigned long n)
{
	const unsigned long *first = __prime_list;
	const unsigned long *last = __prime_list + __num_prime;
	const unsigned long *pos = std::lower_bound(first,last,n);
	return pos == last ? *(last - 1) : *pos;	
}

const char INFILE[] = "HashInt.txt";
enum EntryType { ACTIVE, EMPTY, DELETED };

struct HashTbl
{
    struct HashTbl *next;
    int value;
    EntryType info;		/*indicates if entry is ACTIVE, EMPTY, DELETED */
};

class HashTable{
	public:
		HashTable(int n);
		HashTable(); /* constructor initialize the hash table */
		~HashTable(); /* destructor */

		void addHashTblEntryLinkedList(long key);
		void addHashTblEntryOpenAddressing(long key);
		bool lookup(long key);
		void del(int, struct HashTbl*);
		void print();
		void printFile();
	private:
		int size(); 		/* return the number of elements */
		int bucket(); 		/* return the size of hashtable */
		struct HashTbl* hashEntry;
		int hash(long n);	/* create hash value */
		int next_size(int n)const;
		void initialize_buckets(int n);

		int num_elements; /* the size of actual elements. */
		int num_buckets; /* the size of hashtable*/
};

void readFile(HashTable* hashTable, int line_num);


HashTable::HashTable(int n)
{
	initialize_buckets(n);
}

HashTable::HashTable()
{
	initialize_buckets(5);
}

HashTable::~HashTable()
{
	for (int i = 0; i < num_buckets; ++i)
	{
		if(hashEntry[i].next!=NULL)
		{
			HashTbl* ptr = hashEntry[i].next;
			while(ptr!=NULL)
			{
				// cout << "desctrutor: "<<ptr->value<<" will be deleted."<<endl;
				HashTbl* temp = ptr;
				ptr = ptr->next;
				delete temp;
				temp=NULL;
			}
		}else {
			delete hashEntry[i].next;
			hashEntry[i].next=NULL;
			hashEntry[i].info=EMPTY;
		}
	}
}

void HashTable::initialize_buckets(int n)
{
	num_buckets = next_size(n);
	hashEntry = new HashTbl[num_buckets];
	for (int i = 0; i < num_buckets; ++i)
	{
		hashEntry[i].next = NULL;
		hashEntry[i].value = 0;
		hashEntry[i].info = EMPTY;
	}	
}

int HashTable::next_size(int n)const
{
	return __get_next_prime(n);
}

int HashTable::size()
{
	return num_elements;
}


int HashTable::bucket()
{
	return num_buckets;
}


int HashTable::hash(long n)
{
	return n % num_buckets;
}

void HashTable::addHashTblEntryLinkedList(long key)
{	
	int index = hash(key);
	// cout<<"index: "<< index  << endl;
	if(hashEntry[index].info == EMPTY)
	{	
		hashEntry[index].value = key;
		hashEntry[index].info = ACTIVE;
		// cout<<"value: "<< hashEntry[index].value <<endl;		
	}else {
		HashTbl* ptr = &hashEntry[index];
		
		while(ptr!=NULL && ptr->next!=NULL)
		{	
			ptr = ptr->next;
		}

		ptr->next = new HashTbl(); 
		ptr->next->value = key;
		ptr->next->info = ACTIVE;
		ptr->next->next = NULL;
		ptr =NULL;
		delete ptr;
	}
}

void HashTable::addHashTblEntryOpenAddressing(long key)
{
	int start = hash(key);
	for(int i=0; i<num_buckets; i++)
	{
		int index = hash(i+start);
		if(hashEntry[index].info == EMPTY || hashEntry[index].info==DELETED)
		{
			hashEntry[index].value = key;
			hashEntry[index].next = NULL;
			hashEntry[index].info = ACTIVE;
			return;
		}
	}
}

bool HashTable::lookup(long key)
{
	int index = hash(key);
	if(hashEntry[index].info!=EMPTY || hashEntry[index].info!=DELETED)
	{
		return true;
	}
	HashTbl* ptr = &hashEntry[index];
	while(ptr!=NULL && ptr->value!=key)
	{
		ptr = ptr->next;
	}
	if(ptr==NULL) 
		return false;
	else 
		return true;
}

void HashTable::print()
{
	for(int i=0; i<num_buckets; i++)
	{
		if(hashEntry[i].next!=NULL)
		{
			HashTbl* ptr = &hashEntry[i];
			while(ptr!=NULL)
			{
				cout << ptr->value <<"-->";
				ptr = ptr->next;
			}
			cout<<"\n";
			delete ptr;
			ptr= NULL;
		}else{
			cout << hashEntry[i].value << endl;
		}
	}
}

void HashTable::printFile()
{
	ofstream myfile;
	myfile.open ("output.txt", ios::out);
	for(int i=0; i<num_buckets; i++)
	{
		if(hashEntry[i].next!=NULL)
		{
			HashTbl* ptr = &hashEntry[i];
			while(ptr!=NULL)
			{
				myfile << ptr->value <<"-->";
				ptr = ptr->next;
			}
			myfile<<"\n";
			delete ptr;
			ptr= NULL;
		}else{
			myfile << "[" << hashEntry[i].value <<": "<<hashEntry[i].info<<"]" << endl;
		}
	}
	myfile.close();
}


int main()
{
	int lineOfNum = 1000;
	HashTable* hashTable = new HashTable(lineOfNum);
	readFile(hashTable, lineOfNum);
	// cout << "Hash Table size : " << hashTable.size() << endl;
	// cout << "Hash Table bucket count : " << hashTable.bucket_count() << endl;
	// cout << "Hash Table max bucket count : " << hashTable.max_bucket_count() << endl;
	// hashTable.addHashTblEntryLinkedList(8);
	// hashTable.addHashTblEntryLinkedList(13);
	// hashTable.addHashTblEntryLinkedList(10);
	// if(hashTable.lookup(13))
	// {
	// 	cout << "value: 13 is found." << endl;
	// } else {
	// 	cout << "value: 13 is not found." << endl;
	// }
	// char line[10];
	// long* hash = new long[MAX+1];
	// readFile(hash, line);
	// for(int i=0; i<50; i++)
	// {
	// 	cout<<hash[i]<<endl;
	// }
	// for(int i=0; i<50; i++)
	// {
	// 	hashTable.insert(hash[i]);
	// }
	hashTable->printFile();
	return 0;
}

void readFile(HashTable* hashTable, int line_num)
{
	FILE *fp = fopen(INFILE,"r");
	char line[10];
	if (fp==NULL) 
	{
		cerr<<"Error opening file!"<<endl;
	}
	else{
		int i=0;
		while(i!=line_num && fgets(line, 10, fp))
		{
			hashTable->addHashTblEntryOpenAddressing(atoi(line));
			i++;
		}
	}
}

/* 
void main()
{
	char str[32];
	HashTable ht;

	ht.addHashTblEntry("register");
	ht.addHashTblEntry("asm");
	ht.addHashTblEntry("union");
	ht.addHashTblEntry("goto");
	ht.addHashTblEntry("do");
	ht.addHashTblEntry("public");
	ht.addHashTblEntry("extern");
	ht.addHashTblEntry("main");
	ht.addHashTblEntry("break");
	ht.addHashTblEntry("this");
	ht.addHashTblEntry("float");
	ht.addHashTblEntry("if");
	ht.addHashTblEntry("void");
	ht.addHashTblEntry("new");
	ht.addHashTblEntry("case");
	ht.addHashTblEntry("sizeof");

	ht.addHashTblEntry("goto"); 
	
	ht.printHashTbl();

	strcpy(str,"float");
	if((ht.queryHashTbl(str))!=NULL){ printf("found %s\n",str); }
	else{ printf("did NOT find %s\n",str); }

	strcpy(str,"tax_audit");
	if((ht.queryHashTbl(str))!=NULL){ printf("found %s\n",str); }
	else{ printf("did NOT find %s\n",str); }

	strcpy(str,"extern");
	if((ht.queryHashTbl(str))!=NULL){ printf("found %s\n",str); }
	else{ printf("did NOT find %s\n",str); }

	strcpy(str,"technobabble");
	if((ht.queryHashTbl(str))!=NULL){ printf("found %s\n",str); }
	else{ printf("did NOT find %s\n",str); }


	return;
	
}
*/
/*end main*/