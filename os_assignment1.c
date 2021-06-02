#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#define TRUE 1

void  interpret(char *input, char **argv)
{
     while (*input != '\0') {       /* check if it is the end of line  */ 
          while (*input == ' ' || *input == '\t' || *input == '\n')
               *input++ = '\0';     /* replace white spaces with 0  */
          *argv++ = input;          /* save the position of the argument */
          while (*input != '\0' && *input != ' ' && 
                 *input != '\t' && *input != '\n') 
               input++;             /* skip all the non-white spaces in the argument */
     }
     *argv = '\0';                 /* mark the end of argument list  */
}

void  implement(char **argv)
{
     pid_t  pid;
     int res;
     
     if ((pid = fork()) < 0) {     /* fork a child process */
          printf("ERROR: forking a child process failed execution\n");
          exit(1);
     }
     else if(pid != 0){    /*if pid is not 0, execute parent process */
          res = waitpid(pid,NULL,0);     /* wait for the child process to complete */
          printf("Waited for %d\n", res); /* Process with pid is waiting */      
     }

     else if (pid == 0) {          /* if pid is 0 , execute child process  */
          if (execvp(*argv, argv) < 0) {     /* execute the command  */
               printf("ERROR: Execution failed\n");     
               exit(1);
          }
     }
     
}

int  main(void)
{
     char  input[1024];             /* the input line  */
     char  *argv[64];              /* the command line argument */

     while (TRUE) {                   /* repeat until done  */
          printf("Enter your Shell command here: ");     /*   display a prompt */
          gets(input);              /*   read in the command line     */
          printf("\n");
          interpret(input, argv);       /* call interpret function  */
          if (strcmp(argv[0], "exit") == 0)  /* check if it is an exit */
               exit(0);            /*   if it is, exit */
          implement(argv);           /* otherwise, call implement function to execute the command */
     }
}