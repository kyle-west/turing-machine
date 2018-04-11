#ifndef TURING_H
#define TURING_H

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <utility>
using namespace std;

struct State {
   string write_value;
   string direction;
   string next_state;
};

typedef map<int, string>::const_iterator TapeIterator;

class Turing {
   private:
      map <string, State> instructions;
      string current_state;
      string starting_state;
      string default_value;
      map <int, string> tape;
      int tape_index;


   
   public:
      void parseInstructions (string filename);
      void runSimulation ();
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

      Turing  () {}
      ~Turing () {}
};


void Turing::parseInstructions (string filename) {
   cout << "READING: " << filename << endl;
   ifstream infile(filename.c_str());
   string current_state, read_value, write_value, next_state, direction;

   infile >> default_value >> starting_state;

   while (infile >> current_state >> read_value >> write_value >> next_state >> direction)
   {
      write_value = write_value.substr(2, write_value.length() - 5);
      direction = direction.substr(1, direction.length() - 2);
      cout << current_state << read_value << " | " << write_value << direction << " | " << next_state << "\n";

      State instruction;
      instruction.write_value = write_value;
      instruction.direction = direction;
      instruction.next_state = next_state;

      instructions.insert(pair<string, State>(current_state + read_value, instruction));
   }
}

void Turing::runSimulation () {
   tape_index = 0;
   tape[tape_index] = default_value;
   current_state = starting_state;

   while (current_state != "halt") {
      string read_value = tape[tape_index];
      State instruction = instructions[current_state + "(" + read_value + ")"];
      if (instruction.next_state == "") {
         cerr << "STATE(READ_VALUE) NOT FOUND: '" << current_state + "(" + read_value + ")" << "'" << endl;
         break;
      }
      cout << current_state + "(" + read_value + ")" << "::" << instruction.write_value << instruction.direction << instruction.next_state << endl;
      tape[tape_index] = instruction.write_value;
      current_state = instruction.next_state;
      if (instruction.direction == "left")  goLeft();
      if (instruction.direction == "right") goRight();
   }

   cout << "TAPE CONTENTS: \n";
   for (TapeIterator iter = tape.begin(); iter != tape.end(); iter++) {
      cout << " | " << iter->second; 
   }
   cout << " |\n";
}

#endif // TURING_H