#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define MAXPIPELENGTH 100
#define MAXARGS 100

extern int keep_running;
extern void myshell_cd(char*);
extern void myshell_pwd();
extern void myshell_history(char*);
extern void myshell_export(char*);

int check_and_execute_shell_buitin(char** arg, char* buf){

  int isShellBuitIn = 0;


  if(strcmp(arg[0],"exit") == 0){
    isShellBuitIn = 1;
    cout << "bye..." << endl;
    keep_running = 0;
  }
  else if(strcmp(arg[0],"cd") == 0){
      isShellBuitIn = 1;
      myshell_cd(buf);
  } 
  else if(strcmp(arg[0],"export") == 0){
    isShellBuitIn = 1;
    myshell_export(arg[1]);
  }
  return isShellBuitIn;
}

int check_for_history_and_pwd(char** arg){

  int isShellBuitIn = 0;

  if(strcmp(arg[0],"pwd") == 0){
    isShellBuitIn = 1;
    myshell_pwd();
  }
  else if(strcmp(arg[0],"history") == 0){
    isShellBuitIn = 1;
    myshell_history(arg[1]);
  }
  
  return isShellBuitIn;
}

void tokenize(char* command,char* arg[], char* delim){

    char temp[250]; 
    strcpy(temp,command);    

    char* token;
    int j =0;

    token = strtok(temp,delim);
    while(token!=NULL){
      arg[j] = token;
      token = strtok(NULL,delim);
      j++;
    }

    arg[j] = NULL; 
}

void myshell_execute(char* args){
  
  char actualArg[512];
  strcpy(actualArg,args);

  char* cmd[MAXPIPELENGTH];
  int numCmds = 0;
  char delim[2]="|";

  char temp[250]; 
  strcpy(temp,actualArg);
  char* token;

  int j=0;

  token = strtok(temp,delim);

  while(token!=NULL){
    cmd[j] = token;
    token = strtok(NULL,delim);
    j++;
  }

  numCmds = j; 

  char* arg[MAXARGS];
  int len = strlen(cmd[0]);
  cmd[0][len-1]='\0';
  
  tokenize(cmd[0],arg,(char*)" ");

  char buf[250];
  if(arg[1]!=NULL){
    strcpy(buf,arg[1]);  
  }
  else{
    strcpy(buf,"");
  }

  int n = check_and_execute_shell_buitin(arg,buf);

  cmd[0][len-1]='\n';  
  if(n==0){
              if( fork() == 0){
                for(j=0; j<numCmds-1; j++){

                int pd[2];
                pipe(pd);

                if(!fork()){
                    dup2(pd[1], 1);
                    char* arg[MAXARGS];
                    int len = strlen(cmd[j]);
                    cmd[j][len-1]='\0';
                    tokenize(cmd[j],arg,(char*)" ");
                    if(check_for_history_and_pwd(arg) == 0){
                      execvp(arg[0], arg);
                      perror("exec");
                      abort();
                    }
                    abort();
                  }
                   dup2(pd[0], 0);
                   close(pd[1]);
                }

                char* arg[MAXARGS];
                int len = strlen(cmd[j]);
                cmd[j][len-1]='\0';
                tokenize(cmd[j],arg,(char*)" ");
                if(check_for_history_and_pwd(arg) == 0){
                    execvp(arg[0], arg);
                    perror("exec");
                    abort();
                }
                abort();
            }
            else{
              wait(0);
            }
    }
}