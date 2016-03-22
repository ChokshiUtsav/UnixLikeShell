using namespace std;

// Global varibale that admininstrates shell's execution 
int keep_running = 1;

// Global variable that keeps track of number of entries in history file
int num_entries_history = 1;

// Global file pointer to history file
FILE* history_file_pointer;

// Global vector for storing history entries
vector<string> history_line_vector;

/**
 	@brief Signal Handler for SIGINT
 	@param sig_num Signal Number 
 **/
void sigint_handler(int sig_num){
	return;
}

/**
	@brief Reads history file and fills vector
**/

void read_history(){
	int tempNum = 0;
	char* tempLine = (char*)malloc(sizeof(char)*256);
	string str;
	string newLineStr = "\n";

	while(fscanf(history_file_pointer,"%d %[^\n]*c", &tempNum, tempLine) != EOF){
		str = tempLine;
		str = str + newLineStr;
		history_line_vector.push_back(str);
		num_entries_history++;
	}
}

/**
	@brief Intializes shell environment and variables
**/

void myshell_intialize(){

	signal(SIGINT, sigint_handler);
	string username = getenv("USER");
	string path = "/home/"+username+"/201505581_myshell_history.txt";
	char* HISTORY_PATH = (char*)path.c_str(); 
	history_file_pointer = fopen(HISTORY_PATH,"a+");	
	read_history();
}