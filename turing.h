#ifndef TURING_H
#define TURING_H

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <utility>
using namespace std;

// a simple container for our instruction-states
struct State {
   string write_value;
   string direction;
   string next_state;
};

// shorthand for the iterator we need to parse the tape
typedef map<int, string>::const_iterator TapeIterator;

class Turing {
   private:
      map <string, State> instructions;

      string _state;
      string starting_state;
      string default_value;

      map <int, string> tape;
      int tape_index;

   
   public:
      void parseInstructions (string filename);
      void runSimulation ();

      // move on the tape
      void goLeft () {
         if (tape.find(--tape_index) == tape.end()) {
            tape[tape_index] = default_value;
         }
      }
      void goRight () {
         if (tape.find(++tape_index) == tape.end()) {
            tape[tape_index] = default_value;
         }
      }

      void printTapeValues () {
         bool pleaseClose = false;
         for (TapeIterator iter = tape.begin(); iter != tape.end(); iter++) {
            if (pleaseClose) {
               cout << " ] ";
               pleaseClose = false;
            } else if (iter->first == tape_index) {
               cout << " [ ";
               pleaseClose = true;
            }
            else {
               cout << " | ";
            }
            cout << iter->second; 
         }
         if (pleaseClose) {
            cout << " ]\n";
         } else {
            cout << " |\n";
         }
      }

      Turing  () {}
      ~Turing () {}
};

/*****************************************************************************
* read the file in and parse it into an instruction set
*****************************************************************************/
void Turing::parseInstructions (string filename) {
   ifstream infile(filename.c_str());
   string tape_state, current_state, read_value, write_value, next_state, direction;

   infile >> tape_state >> default_value >> starting_state;

   tape_index = 0;
   _state = starting_state;
   if (tape_state == "||") {
      tape[tape_index] = default_value;
   } else {
      string value = "";
      for (int i = 1; i < tape_state.length(); i++) {
         if (tape_state[i] == '|') {
            tape[tape_index++] = value;
            value = "";
         } else {
            value += tape_state[i];
         }
      }
      tape_index = 0;
   }
   
   while (infile >> current_state >> read_value >> write_value >> next_state >> direction)
   {
      write_value = write_value.substr(2, write_value.length() - 5);
      direction = direction.substr(1, direction.length() - 2);

      State instruction;
      instruction.write_value = write_value;
      instruction.direction = direction;
      instruction.next_state = next_state;

      instructions.insert(pair<string, State>(current_state + read_value, instruction));
   }
}


/*****************************************************************************
* run the instruction set!
*****************************************************************************/
void Turing::runSimulation () {
   while (_state != "halt") {
      printTapeValues();
      string read_value = tape[tape_index];
      State instruction = instructions[_state + "(" + read_value + ")"];
      if (instruction.next_state == "") {
         cerr << "STATE(READ_VALUE) NOT FOUND: '" 
              << _state + "(" + read_value + ")" 
              << "'" << endl;
         break;
      }
      cout << _state + "(" + read_value + ")" 
           << " :: write(" << instruction.write_value 
           <<  "), move(" << instruction.direction 
           << "), changeToState("
           << instruction.next_state << ")\n\n";
      
      tape[tape_index] = instruction.write_value;
      _state = instruction.next_state;
      if (instruction.direction == "left")  goLeft();
      if (instruction.direction == "right") goRight();
   }

   printTapeValues();
}

#endif // TURING_H