#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#define writeAllocate 0
#define noWriteAllocate 1
#define writeThrough 0
#define writeBack 1
#define lru 0
#define fifo 1

using namespace std;

vector<string> data[5];

string hex_char_to_bin(char c) {
        switch(toupper(c)) {
                case '0': return "0000";
                case '1': return "0001";
                case '2': return "0010";
                case '3': return "0011";
                case '4': return "0100";
                case '5': return "0101";
                case '6': return "0110";
                case '7': return "0111";
                case '8': return "1000";
                case '9': return "1001";
                case 'A': return "1010";
                case 'B': return "1011";
                case 'C': return "1100";
                case 'D': return "1101";
                case 'E': return "1110";
                case 'F': return "1111";
        }
}

class cache {
	public:
        //data fields
        map<string, vector<string>> cacheData;
        int sets;
        int blocksPerSet;
        int bytesPerBlock;

        //settings fields
        int toMemoryProtocol;
        int toCacheProtocol;
        int evictionProtocol;

        //output fields
        int loads;
        int stores;
        int loadHits;
        int loadMisses;
        int storeHits;
        int storeMisses;
        int cycles;

        //functions
        cache (int _sets, int _blocksPerSet, int _bytesPerBlock, int _toMemoryProtocol, int _toCacheProtocol, int _evicitionProtocol);
        void perform (string storeOrLoad, string index, string tag);
        void write (string index, string tag);
        void read (string index, string tag);
        void insert (string index, string tag);
        bool containsElement(string index, string tag);

        void printResults ();
};

cache::cache (int _sets, int _blocksPerSet, int _bytesPerBlock, int _toMemoryProtocol, int _toCacheProtocol, int _evicitionProtocol) {

        //data fields
        sets = _sets;
        blocksPerSet = _blocksPerSet;
        bytesPerBlock = _bytesPerBlock;

        //settings fields
        toMemoryProtocol = _toMemoryProtocol;
        toCacheProtocol = _toCacheProtocol;
        evictionProtocol = _evicitionProtocol;

        //output fields
        loads = 0;
        stores = 0;
        loadHits = 0;
        loadMisses = 0;
        storeHits = 0;
        storeMisses = 0;
        cycles = 0;
}


//argv[1] is amount of sets
//argv[2] is the amount of blocks per set
//argv[3] is the amount of bytes per block
//argv[4] write-allocate or no write-allocate
//argv[5] write through or write back
//argv[6] lru or fifo 
//argv[8] trace file

int main(int argc, char** argv) {
	//setting up variables 
	char * p;
	int indexVar = 0;
	string word;

	//allocate all relevant data from trace file
	while (cin >> word) {
		if (indexVar ==0 || indexVar == 1) {
			data[indexVar].push_back(word);
		}

		if(indexVar <= 1) {
			indexVar++;
		}
		else {
			indexVar = 0;
		}
	}

	//TODO: gracefully handle invalid parameters
	int sets = strtol(argv[1], &p, 10);
	int blocks = strtol(argv[2], &p, 10);
	int bytes = strtol(argv[3], &p, 10);
	
	int offsetSize = 0;
        for (int i = bytes; i > 1; i /= 2) {
                offsetSize++;
        }

        int indexSize = 0;
        for (int i = sets; i > 1; i /= 2) {
                indexSize++;
        }

        int tagSize = 32-indexSize-offsetSize;

	string str1 = "write-allocate";
	string str2 = "no-write-allocate";
	string str3 = "write-through";
	string str4 = "write-back";
	string str5 = "fifo";
	string str6 = "lru";
	int toCacheProtocol;
	if (str1.compare(argv[4]) == 0) {
		toCacheProtocol = writeAllocate;
	}
	else if (str2.compare(argv[4])) { 
		toCacheProtocol = noWriteAllocate;
	}

	int toMemoryProtocol;
	if (str3.compare(argv[5]) == 0) {
                toMemoryProtocol = writeThrough;
        }
        else if (str4.compare(argv[5])) {
                toMemoryProtocol = writeBack;
        }
	
	int evictionProtocol;
	if (str5.compare(argv[6]) == 0) {
                evictionProtocol = fifo;
        }
        else if (str6.compare(argv[6])) {
                evictionProtocol = lru;
        }

	for (int i = 0; i < data[1].size(); i++) {
		string binaryRepresentation = "";
		for (int j = 2; j < data[1].at(i).length(); j++) {
			binaryRepresentation.append(hex_char_to_bin(data[1].at(i).at(j)));
		}
		data[2].push_back(binaryRepresentation);
		data[3].push_back(binaryRepresentation.substr(0, tagSize));
		data[4].push_back(binaryRepresentation.substr(tagSize-1, indexSize));
	}

	cache mainCache(sets, blocks, bytes, toMemoryProtocol, toCacheProtocol, evictionProtocol);

	for (int i = 0; i < data[1].size(); i++) {
                mainCache.perform(data[0].at(i), data[4].at(i), data[3].at(i));
        }

	mainCache.printResults();
}

void cache::perform (string storeOrLoad, string index, string tag) {
	if (storeOrLoad.compare("s") == 0) { 
		write(index, tag);
	}
	else if (storeOrLoad.compare("l") == 0) {
		read(index, tag);
	}

}

void cache::write (string index, string tag) {
	vector<string> set = cacheData[index];
	
	stores++;
	
	if(containsElement(index, tag)) {
		storeHits++;

		cycles += (bytesPerBlock / 4);

		if (evictionProtocol == lru) {
			for (int i = 0; i < set.size(); i++) {
				if (set[i].compare(tag) == 0) {
					set.erase(set.begin());
					set.push_back(tag);
				}
			}
		}
	}
	else {
		storeMisses++;
		if (toMemoryProtocol == writeAllocate) {
                        //loads block into cache
                        cycles += (bytesPerBlock / 4) + 100*(bytesPerBlock / 4);
			insert(index, tag);

                        if (toCacheProtocol == writeThrough) {
                                //has to write to cache and to main memory
                                cycles += (bytesPerBlock / 4) + 100*(bytesPerBlock / 4);
                        }
                        else {
                                //implies writeBack which means that we only write to the cache
                                cycles += (bytesPerBlock / 4);
                        }
                }
                else {
                        //has to write to main memory
                        cycles += 100*(bytesPerBlock / 4);
                }
	}

	cacheData[index] = set;
}

void cache::read (string index, string tag) {
	loads++;

	if(containsElement(index, tag)) {
		loadHits++;

		cycles += (bytesPerBlock / 4);

		if (evictionProtocol == lru) {
                        for (int i = 0; i < cacheData[index].size(); i++) {
                                if (cacheData[index][i].compare(tag) == 0) {
                                        cacheData[index].erase(cacheData[index].begin()+i);
                                        cacheData[index].push_back(tag);
                                }
                        }
                }
	
	}
	else {
		loadMisses++;

		cycles += (bytesPerBlock / 4) + 100*(bytesPerBlock / 4);

		insert(index, tag);		
	}

}

void cache::insert (string index, string tag) {
	if (cacheData[index].size() == blocksPerSet) {
		cacheData[index].erase(cacheData[index].begin());
	}
	cacheData[index].push_back(tag);
}

bool cache::containsElement(string index, string tag) {
	vector<string> set = cacheData[index];

	for (int i = 0; i < set.size(); i++) {
		if (set.at(i).compare(tag) == 0) {
			return 1;
		}
	}

	return 0;
}

void cache::printResults() {

	cout << "Total loads: " << loads << "\n";
        cout << "Total stores: " << stores << "\n";
        cout << "Load hits: " << loadHits << "\n";
        cout << "Load misses: " << loadMisses << "\n";
        cout << "Store hits: " << storeHits << "\n";
        cout << "Store misses: " << storeMisses << "\n";
        cout << "Total cycles: " << cycles << "\n";

}
