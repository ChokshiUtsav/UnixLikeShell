#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


#include "Parser.h"
#include "Intialization.h"
#include "BuiltIn.h"
#include "Execution.h"

using namespace std;

extern FILE* history_file_pointer;
extern vector<string> history_line_vector;

/**
	@brief Main Entry Point.
	@param argc Argument Count.
	@param argv Argument Vector.
	@return status code.
**/

int main(int argc, char* argv[]){
	
	// Intializing shell and setting up environment
	myshell_intialize();

	// Keeping shell into infinite loop
	myshell_loop();

	fclose(history_file_pointer);

	return EXIT_SUCCESS;
}