#include  <stdio.h>
#include  <sys/types.h>
#include  <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/
int num_of_args;

char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
    	token[tokenIndex] = '\0';
    	if (tokenIndex != 0){
			tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
			strcpy(tokens[tokenNo++], token);
			tokenIndex = 0; 
      	}
    } 
	else {
      token[tokenIndex++] = readChar;
    }
  }
  num_of_args=tokenNo;
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}


int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;
	int bg=0,status;


	while(1) {			
        bg=0;
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();

		while((status = waitpid(0, NULL, WNOHANG))>0)
			printf("Shell: Background process finished: %d \n",status);

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

		//shell implementation
		if(tokens[0]==NULL){
			continue;
		}

		//printf("%d",num_of_args);

		if(strcmp(tokens[0],"cd")==0){
			if(tokens[1]==NULL){
				printf("Shell: Incorrect command\n");
				continue;
			}
			else{
				chdir(tokens[1]);
				continue;
			}

		}

		if(strcmp(tokens[num_of_args-1],"&")==0){
			bg = 1;
			tokens[num_of_args-1] = '\0';
		}

		

		int fc = fork();

		//fork failed
		if(fc < 0){
			fprintf(stderr,"%s\n", "Unable to create child process!!\n");
		}
		//child code
		else if(fc==0){
			execvp(tokens[0], tokens);
			printf("Command execution failed!!\n"); 
			_exit(1);
		}
		else{
			//printf("Parent:\n");
			if(bg!=1){
				int wc = wait(NULL);
			}
		}

		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);
	}
	return 0;
}