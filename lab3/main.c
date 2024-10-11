/* 
 * File:   main.c
 * Author: a4najafi
 *
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/wait.h> 
#include <unistd.h> 

int main(int argc, char** argv) {
    char *command;
    
    do {
        char *arguments[100];
        command = (char *)malloc(100*sizeof(char));
        strncpy(command,"\0",100);
        
        int command_index = 0, argument_index = 0, ch, should_fork;
            printf("Your Command>");
        
        while(( ch = getchar()) != '\n'){
            if(command_index == 0){
                arguments[argument_index] = command + command_index;
                *(command + command_index) = ch;
                    command_index++;
                    argument_index++;
            
            }else if(ch==' '){
                arguments[argument_index]= command + command_index + 1;
                *(command + command_index) = '\0';
                    command_index++;
                    argument_index++;
            }else{
                *(command + command_index) = ch;
                    command_index++;
            }
        }
        *(command + command_index)= '\0';
        arguments[argument_index]=NULL;
        
        if(*(command + command_index - 1) == '&'){
            *(command + command_index-1)= '\0';
            should_fork=1;
        }else{
            should_fork=0;
        }
        
        pid_t pid = fork();
        
        if (pid < 0){
            fprintf(stderr,"Error in creating child process");
        }
        if(pid > 0){
            
            if(!should_fork){
                wait(NULL);  
            }
        }else{
            
            int return_value = execvp(command, arguments);
            exit(return_value);
        }
        
        } while (strncmp(command,"exit",4)!=0);
            printf("Exit");
    return (EXIT_SUCCESS);
}