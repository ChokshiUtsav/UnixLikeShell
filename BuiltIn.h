extern int num_entries_history;
extern vector<string> history_line_vector;



void myshell_cd(char* arg){

	int ret = 0;
	string home_path = getenv("HOME");

	if(arg==NULL || arg==(char*)" " || arg==(char*)""){
		ret = chdir(home_path.c_str());
	}
	else{
		string provided_path = arg;
		if (arg[0] == '~'){
			provided_path.replace(0,1,home_path);
		}
		ret = chdir(provided_path.c_str());
		if(ret == -1){
			perror("chdir");
		}
	}
}



void myshell_pwd(){

	char buf[256];
	getcwd(buf,256);
	printf("%s\n",buf);
}

void myshell_export(char* args){
	
	string inputStr = args;
	int num = 0;
	char* varName;
	char* value;
	int len = 0;

	if(inputStr.at(0) == '$'){
		num = inputStr.find('=');
		len = num-1;
		string temp = inputStr.substr(1,len);
		varName = (char*)temp.c_str();
		temp = inputStr.substr(num+1);
		value = (char*)temp.c_str();
		setenv(varName,value,1);
	}
}

void myshell_history(char* arg){

	int index=0;
	int num = 0;

	if(arg == NULL){
		for(index=1; index<num_entries_history; index++){
			cout << index << " " << history_line_vector[index-1];
		}
	}
	else{
		num = atoi(arg);
		if(num >= num_entries_history){
			for(index=1; index<num_entries_history; index++){
				cout << index << " " << history_line_vector[index-1];
			}		
		}
		else{
			for(index=num_entries_history-num; index<num_entries_history; index++){
				cout << index << " " << history_line_vector[index-1];
			}	
		}			
	}
}

string myshell_bang(string bang_string, int* error){

	string text_to_replace="";
	string inputStr = bang_string;
	int num = 0;
	int index = 0;
	int flag = 0;

	if(inputStr.at(0) == '!'){
		if(inputStr.at(1) == '!'){
			*error = 0;
			text_to_replace = history_line_vector[num_entries_history-2];
		}
		else if(inputStr.at(1) == '-'){
			num = atoi(inputStr.substr(2).c_str());
			if(num > (int)history_line_vector.size()){
				*error = 1;	
			}
			else{
				*error = 0;
				text_to_replace = history_line_vector[num_entries_history-1-num];
			}
		}
		else if(isdigit(inputStr.at(1))){
			num = atoi(inputStr.substr(1).c_str());
			if(num > (int)history_line_vector.size()){
				*error = 1;	
			}
			else{
				*error = 0;
				text_to_replace = history_line_vector[num-1];	
			}
		}
		else{
			string prefix = inputStr.substr(1);
			flag = 0;
			for(index=num_entries_history-2; index>=0; index--){
				text_to_replace = history_line_vector[index];
				if( text_to_replace.substr(0,prefix.size()) == prefix ){
					flag = 1;
					break;
				}
			}

			if(flag == 0){
				*error = 1;
			}
			else{
				*error = 0;
			}
		}
	}

	if(*error == 1){
		
		if(inputStr.at(inputStr.length()-1) == '\n'){
     		 inputStr.erase(inputStr.length()-1); 
    	}
    	text_to_replace = "My Shell : " + inputStr + ": event not found";
	}

	return text_to_replace;
}