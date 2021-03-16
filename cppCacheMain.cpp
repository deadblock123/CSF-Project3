#include <iostream>
#include <fstream>
#include <string>
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


