#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
using namespace std;

extern int keep_running;
extern FILE* history_file_pointer;
extern int num_entries_history;
extern vector<string> history_line_vector;
extern void myshell_execute(char*);
extern string myshell_bang(string,int*);

/**
  @brief Prints a prompt
**/

void print_prompt(){

  char buf[256];
  getcwd(buf,256);
  cout << buf << " : My Shell >"; 
}



/**
	@brief Reads a line of input from stdin
	@return line from stdin
**/

char* myshell_read_line(void){

  char *line = NULL;
  size_t bufsize = 0; // have getline allocate a buffer for us
  getline(&line, &bufsize, stdin);

  return line;
}

/**
  @brief Searches and replaces the bang operator in command
  @return string without bang operator
**/

char* search_for_bang(char* cmd){

  string inputStr = cmd;
  int position = inputStr.find("!");
  int length = 0;
  int error = 0;

  if(position != (int)string::npos){

    // Finding sub string that contains !
    length = inputStr.find(" ",position+1);
    length = length - position;
    string bang_string = inputStr.substr(position,length);

    // Searching in history file for proper replacement, if any
    string string_to_replace = myshell_bang(bang_string,&error);
    if(error == 1){
      cout << string_to_replace << endl;
      return NULL;
    }

    // Removing new line character
    if(string_to_replace.at(string_to_replace.length()-1) == '\n'){
      string_to_replace.erase(string_to_replace.length()-1); 
    }
    inputStr.replace(position,length,string_to_replace);
    
    if(inputStr.at(inputStr.length()-1) != '\n'){
      inputStr.append("\n");
    }
  }

  char* bangless_string = (char*)inputStr.c_str();
  return bangless_string;
}


/**
  @brief Appends command to history file
  @param cmd Complete command with args
**/


void myshell_add_to_history(char* cmd){

  fprintf(history_file_pointer, "%d %s",num_entries_history, cmd);
  string str = cmd;
  history_line_vector.push_back(str);
  num_entries_history++;
}

/**
	@brief Loop that reads input, parses it and executes command
**/
void myshell_loop(){

	char* line;
  char* cmd;

	do{
		print_prompt();
		line = myshell_read_line();
    cmd = search_for_bang(line);
    if(cmd != NULL){
        myshell_add_to_history(cmd);
        myshell_execute(cmd);
    }
	}
	while(keep_running);
}