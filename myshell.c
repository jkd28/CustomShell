/*
John Dott
COE 0449: Project 4

A simple shell implementation with support for UNIX commands, changing directory,
time command, and input/output redirection on the command line.

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
int main(){
	const int CLK_TICKS = 1000000; // arbitrary standard number of clock ticks 
	const char *READING = "r";
	const char *WRITING = "w+";
	// declarations and instantiations
	char delimiters[] = {'(', ')', '<', '>', '|', '&', ';', '\n', '\t', ' '};
	char *input = (char *)malloc(100*sizeof(char));
	char *args[50];
	int size = 100;
	int count = 0;
	int pid, i;
	char *next;
	char *path;
	char *command;
	int input_redirect = 0, output_redirect = 0, time = 0;
	struct tms *all_times = (struct tms *)malloc(sizeof(struct tms *));
	
	
	//begin shell code
	while(1){
		//getcwd(cwd, 100);
		printf("> ");
		fgets(input, size, stdin); // take input from command line

		//check for redirection
		if(strstr(input, "<") != NULL) {
			input_redirect =1;
		} else if(strstr(input, ">") != NULL){
			output_redirect=1;
		}
		
		//get first token, or the command
		next = strtok(input, delimiters);
		
		//check for built-in commands
		if(strcmp(next, "time") == 0){ // check for time command
			// keep track of time
			time = 1;
			next = strtok(NULL, delimiters); // get command after time
		}
		//no else, 'time' runs regardless of the rest of the input
		if(strcmp(next, "exit")==0){ //check for exit command
			free(input);
			exit(0); //exit program
		} else if(strcmp(next, "cd") == 0){ // check for cd command
			path = strtok(NULL, delimiters); // get cd path 
			if(path != NULL){ // check for lack of path
				if(chdir(path) != 0){ //check for error in directory change
					printf("Unable to change directory\n");
				}
			} else {
				printf("No path specified for 'cd' operation.\n");
			}
		} else { // execute a command
			pid = fork();
			if(pid == 0){
				// we are in the CHILD
				//arrange the command
				while((next != NULL)){
					args[count] = (char *)malloc(100*sizeof(char));
					strcpy(args[count], next);
					next = strtok(NULL, delimiters);
					count++;
				}
				args[count] = NULL;
				//check for redirection
				if(output_redirect){ //there is output redirection
					if(freopen(args[count-1], WRITING, stdout) == NULL){
						printf("Error redirecting output.\n");
						exit(1);
					}
					args[count-1] = NULL; // remove from the args array so commands dont use it as input
					free(args[count-1]);
					count--;
					output_redirect = 0;
				} else if(input_redirect){ //there is input redirection
					if(freopen(args[count-1], READING, stdin)){
						printf("Error redirecting input.\n");
						exit(1);
					}
					args[count-1] = NULL; // remove from the args array so commands dont use it as input
					free(args[count-1]);
					count--;
					input_redirect = 0;
				}
				
				//execute the command
				if (execvp(args[0], args) == -1){
					printf("Error processing command\n");
					exit(1);
				}
				//free memory as necessary
				free(input);
				for(i = 0; i < count; i++){
					free(args[i]);
				}
				exit(0);
			} else {
				// we are in the parent
				wait(NULL);
				if(time != 0){
					//display our time and child times
					if(times(all_times) == -1){
						printf("Error calculating times of processes.");
						time=0;
					} else {
						printf("User Time: %fs\n", (all_times->tms_cutime)/ CLK_TICKS );
						printf("System Time: %fs\n", (all_times->tms_cstime)/CLK_TICKS);
					}
					time=0;
				}
			}
		}
	}
	// free memory allocations
	free(all_times);
	free(input);
}
