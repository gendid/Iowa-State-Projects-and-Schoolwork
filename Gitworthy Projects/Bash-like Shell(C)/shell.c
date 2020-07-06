//Created by Will Springer. Custom shell for CPRE 308. Runs built in and program functions.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

int main( int argc, char* argv[])
{

  char* prompt="308sh>";
  char str[60]= "";
  char input;
  // Checks if the shell was started with -p. If it was, change the prompt.
  if (argc==3)
    {
      if(strcmp(argv[1], "-p")==0)
	{
	  prompt=argv[2];
	}
	else
	  {
	    printf("Invalid parameter, reverting to default...\n");
	  }
	  
	} 

      printf(prompt);
      //Infinite loop for running the shell
      while(1)
	{
	  //Get user input, convert it to a useable string.
	  fgets(str, 60, stdin);
	  str[strlen(str)-1]='\0';
	  //Exit condition, breaks from shell.
	  if (strcmp(str, "exit")==0)
	    {
	      exit(0);
	    }
	  //PID command, returns the pid of the current process.
	  else if (strcmp(str, "pid")==0)
	    {
	      printf("%d \n", getpid());
	    }
	  //PPID command, returns the pid of the parent of this process.
	  else if(strcmp(str, "ppid")==0)
	    {
	      printf("%d \n", getppid());
	    }
	  //PWD command. Prints current working directory.
	  else if(strcmp(str, "pwd")==0)
	    {
	      char cwd[PATH_MAX];
	      if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
		  printf("%s \n", cwd);
		}
	      else
		{
		  perror("Error in getcwd");
		}
	    }
	  //CD command. Changes directory to specified location. If nothing specified, changes to home.
	  else if(strncmp(str, "cd", 2)==0)
	    {
	      if(strlen(str)==2)
		{
		  chdir(getenv("HOME"));
		}
	      else
		{
		  char args[strlen(str)-3];
		  memcpy(args, str+3, strlen(str)-3);
		  chdir(args);
		  if (chdir(args)==-1)
		    {
		      perror("No such directory\n");
		    }
		}

	    }
	  //Program commands, made to get anything else or error if it's crap.
	  else
	    {
	      //Child process functionality, creates a child to run the program command.
	      int child=fork();
	      if(child==0)
		{
		  printf("Child created. PID: %d \n", getpid());
		  char * path;
		  char * pathArgs[2000];
		  int i=0;
		  //Parse the user string. First char pointer is the path, all others are args.
		  path= strtok(str, " ");

		  char * temp= path;
		  while (path != NULL)
		    {
		      pathArgs[i]= path;
		      i++;
		      path= strtok(NULL, " ");
		    }
		  //Executes the given path and arguments. If they don't exist, throw an error.
		  execvp(temp, pathArgs);
		  if (execvp(temp, pathArgs)==-1)
		    {
		      perror("Improper command, try again");
		    }

		      exit(getpid());
		}
	      else
		//Parent functionality. Waits for child to finish, then prints.
		{

		      waitpid(child);
		      printf("Child %d has exited with status %d \n", child, WEXITSTATUS(child));
		}
	    }
	  
	  //Reprint the prompt.
	      printf(prompt);
	    	  
	}

  return 0;

}
