#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    char *username;
    bool running = true;
    bool background = false;
    bool redirect = false;
    char commandBuffer[128];
    char* token;
    char* tokenList[1024];
    int numTokens = 0;
    pid_t pid;

    //Grab the ULID from the system and check to make sure it worked
    if((username = getlogin()) == NULL)
    {
        fprintf(stderr, "Failed to grab username.");
        return (1);
    }

    //Allocate memory for the commands array
    tokenList[1024] = malloc(sizeof(char) * 1024);

    //Loop the program until exit command is received
    while (running)
    {

        //Grabbing username and setting up terminal
        printf("%s> ", username);
        fgets(commandBuffer, sizeof(commandBuffer), stdin);

        //Tokenize the command
        token = strtok(commandBuffer, " \n\t");
        while (token != NULL)
        {
            //Keep getting tokens until token is NULL
            if (strcmp("&", token) != 0)
            {
                tokenList[numTokens] = token;
                numTokens++;
            }
            else
            {
                background = true;
            }
            //Check for redirect
            if (strcmp("<*", token) == 0)
            {
                redirect = true;
                //printf("Redirect detected\n");
            }
            token = strtok(NULL, " \n\t");
        }

        if (strcasecmp(tokenList[numTokens - 1], "fg") == 0)
        {
            background = false;
            waitpid(pid, NULL, WUNTRACED);
        }

        //Checking for exit condition
        if (strcasecmp(tokenList[numTokens - 1], "exit") == 0)
        {
            exit(EXIT_SUCCESS);
        }

        // //Print out tokens for test purposes
        // for(int i = 0; i < numTokens; i++)
        // {
        //     printf("%s\n", tokenList[i]);
        // }

        //Create a child process to execute the command
        if (pid = fork() == 0)
        //Inside the child process
        {

            if(background)
            {
                //Output the command being run in the background
                fprintf(stderr, "Executing command %s ", tokenList[numTokens - 1]);
                for (int i = numTokens - 2; i >= 0; i--)
                {
                    fprintf(stderr, "%s ", tokenList[i]);
                }
                fprintf(stderr, "in the background.\nPID: %d PPID: %d\n", getpid(), getppid());
            }
            
            if (numTokens > 0)
            {
                //Switch case for all the numbers of tokens (horrible to look at)
                switch(numTokens)
                {
                    case 1:
                        if (execlp(tokenList[numTokens - 1], tokenList[numTokens - 1], NULL) == -1)
                        {
                            //fprintf(stderr, "Failed to execute command.\n");
                            exit(EXIT_FAILURE);
                        }
                    case 2:
                        if (execlp(tokenList[numTokens - 1], tokenList[numTokens - 1], tokenList[numTokens - 2], NULL) == -1)
                        {
                            //fprintf(stderr, "Failed to execute command.\n");
                            exit(EXIT_FAILURE);
                        }
                    case 3:
                        if (execlp(tokenList[numTokens - 1], tokenList[numTokens - 1], tokenList[numTokens - 2], tokenList[numTokens - 3], NULL) == -1)
                        {
                            //fprintf(stderr, "Failed to execute command.\n");
                            exit(EXIT_FAILURE);
                        }
                    case 4:
                        if (execlp(tokenList[numTokens - 1], tokenList[numTokens - 1], tokenList[numTokens - 2], tokenList[numTokens - 3], tokenList[numTokens - 4], NULL) == -1)
                        {
                            //fprintf(stderr, "Failed to execute command.\n");
                            exit(EXIT_FAILURE);
                        }
                    default:
                        if (execlp(tokenList[numTokens - 1], tokenList[numTokens - 1], tokenList[numTokens - 2], tokenList[numTokens - 3], tokenList[numTokens - 4], tokenList[numTokens - 5], NULL) == -1)
                        {
                            //fprintf(stderr, "Failed to execute command.\n");
                            exit(EXIT_FAILURE);
                        }
                }
            }
            //Terminate child process
            exit(EXIT_SUCCESS);
        }
        else if (pid < 0)
        {
            fprintf(stderr, "Failed to fork process.");
            exit(EXIT_FAILURE);
        }
        else
        //Parent process
        {
            //If asking to run in background, ignore waiting
            if(background)
            {
                //printf("Running process in background\n");
            }
            else
            {
                //printf("Waiting for child process to finish.\n");
                wait(NULL);
                //printf("Child process finished.\n");
            }
            
        }

        //Flush the tokenList and command buffer
        for (int i = 0; i < numTokens; i++)
        {
            tokenList[i] = '\0';
        }

        for (int i = 0; i < sizeof(commandBuffer); i++)
        {
            commandBuffer[i] = '\0';
        }

        //Reset token counter and background flag
        numTokens = 0;
        background = false;
        redirect = false;
    }

    return 0;
}
