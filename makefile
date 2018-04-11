a.exe: turing.h os.cpp *.tm
	g++ os.cpp
	./a.exe example.tm
