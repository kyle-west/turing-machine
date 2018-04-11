#include <iostream>
#include <string>
#include "turing.h"
using namespace std;

int main (int argc, char ** argv) {
   string filename;

   if (argc < 2) {
      cout << "Please Enter the file name of a TuringOS script: ";
      cin >> filename;
   } else {
      filename = argv[1];
   }
   cout << "Running Turing Machine Emulator on '" << filename << "'\n";

   Turing TM;

   TM.parseInstructions(filename);
   TM.runSimulation();

   return 0;
}