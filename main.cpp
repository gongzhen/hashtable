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

#define PRIME 547
// #define PRIME 5

static const int  __num_prime = 28;
static const unsigned long __prime_list[__num_prime] = {
	53,         97,           193,         389,       769,
	1543,       3079,         6151,        12289,     24593,
	49157,      98317,        196613,      393241,    786433,
	1572869,    3145739,      6291469,     12582917,  25165843,
	50331653,   100663319,    201326611,   402653189, 805306457, 
	1610612741, 3221225473ul, 4294967291ul	
};

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
		HashTable(); /* constructor initialize the hash table */
		~HashTable(); /* destructor */

		void addHashTblEntryLinkedList(long key);
		void addHashTblEntryOpenAddressing(long key);
		bool lookup(long key);
		void del(int, struct HashTbl*);
		void print();
		void printFile();
	private:
		int size(); /* return the hash table size */
		struct HashTbl hashEntry[PRIME];
		int hash(long n);	/* create hash value */
		int hash_key(long n);
};

void readFile(HashTable* hashTable, int line_num);

HashTable::HashTable()
{
	for (int i = 0; i < PRIME; ++i)
	{
		hashEntry[i].next = NULL;
		hashEntry[i].value = 0;
		hashEntry[i].info = EMPTY;
	}
}

HashTable::~HashTable()
{
	for (int i = 0; i < PRIME; ++i)
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

int HashTable::hash_key(long n)
{
	return 1;
}

int HashTable::hash(long n)
{
	return n%PRIME;
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
	for(int i=0; i<PRIME; i++)
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
	for(int i=0; i<PRIME; i++)
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
			myfile << hashEntry[i].value << endl;
		}
	}
	myfile.close();
}


int main()
{
	HashTable* hashTable = new HashTable();
	readFile(hashTable, 1000);
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
			hashTable->addHashTblEntryLinkedList(atoi(line));
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