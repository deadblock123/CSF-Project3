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


string arrayContainer[40][3];

int main(int argc, char** argv) {
     string text;

     ifstream TraceFile(argv[1]);

     int positionPointer;
     int arrayPointer1 = 0;
     int arrayPointer2 = 0;
     while (getline (TraceFile, text)) {
	     cout << text << "\n" << endl;
	     while(text != "") {
	       positionPointer = text.find(" ");
	       if(positionPointer != -1) {
                  arrayContainer[arrayPointer1][arrayPointer2] = text.substr(0, positionPointer);
		  
		  if(arrayPointer2 >= 3) {
			  arrayPointer1++;
			  arrayPointer2 = 0;
		  } else {
			  arrayPointer2++;
		  }
                  
		  text = text.substr(positionPointer+1);

	       } else {
                 arrayContainer[arrayPointer1][2] = text.substr(0, positionPointer);
		 text = "";
	       }
	     }

     }
     cout << arrayContainer << endl;
     for(int i = 0; i < arrayPointer1+1; i++) {
          cout << arrayContainer[i][0] << endl;
	  cout << arrayContainer[i][1] << endl;
	  cout << arrayContainer[i][2] << endl;
	  cout << "Move on to next stack" << endl;
     }
     TraceFile.close();

}

class cache {
	
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
	public:
	cache (int _sets, int _blocksPerSet, int _bytesPerBlock, int _toMemoryProtocol, int _toCacheProtocol, int _evicitionProtocol);
	void perform (string storeOrLoad, string index, string tag);
	void write (string index, string tag);
	void read (string index, string tag);
	void eviction (vector<string> block);

	int getLoads ();
	int getStores ();
	int getloadHits ();
	int getloadMisses ();
	int getstoreHits ();
	int getStoreMisses ();
	int getCycles ();
	
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

void cache::perform (string storeOrLoad, string index, string tag) {
}
