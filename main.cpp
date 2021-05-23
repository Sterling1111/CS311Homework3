#include "Lexer.hpp"
#include "timer.hpp"
#include <iostream>

using namespace std;

//no memory leaks. Its good quality code!!!

int main(int argc, char* argv[]) {

    string sourceFile = "source.txt";

    //if argc is 1 then we dont have the file name that we need
    //if it is 2 then we will assume that the second c string represents
    //the file name of sourceCode. If more than 3 we dont know whats what so
    //just return
/*	switch(argc) {
	case 1:
		return 0;
	case 2:
		sourceFile = argv[1];
		break;
	default:
		return 0;
	}*/

    auto* timer = new Timer<seconds>;
    Lexer lexLuthor(sourceFile);
    lexLuthor.run();
    string result = lexLuthor.generateReport();
    delete timer;
    cout << result << endl << endl;
    return 0;
}
