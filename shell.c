#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>


/**
 * sh_num_builtins - number of builtin commands
 * Return: 1
 */
int sh_num_builtins(void)
{
return (0);
}
/**
 * sh_launch - launches the processes
 * and wait for the child to exit.
 * @args: the words passed to the stdin
 * Return: 1
 */
int sh_launch(char **args)
{
pid_t pid;
int status;

pid = fork();
if (pid == 0)
{
if (execve(args[0], args, NULL) == -1)
{
perror("./shell");
}
exit(EXIT_FAILURE);

}
else if (pid < 0)
{
perror("Froking Error");
}
else
{
do {
waitpid(pid, &status, WUNTRACED);
} while (!WIFEXITED(status) && !WIFSIGNALED(status));
}
return (1);
}

/**
 * sh_execute - executes the passed arguments
 * @args: the words passed to stdin
 * Return: 1
 */
int sh_execute(char **args)
{
if (args[0] == NULL)
{
return (1);
}
return (sh_launch(args));
}


/**
 * sh_read_line - reads the line
 * Return: the read line
 */
char *sh_read_line(void)
{
char *line = NULL;
size_t bufsize = 0;

if (getline(&line, &bufsize, stdin) == -1)
{
if (feof(stdin))
{
exit(EXIT_SUCCESS);
}
else
{
perror("readline");
exit(EXIT_FAILURE);
}
}
return (line);
}


#define SH_TOK_BUFSIZE 64
#define SH_TOK_DELIM " \t\r\n\a"
/**
 * sh_split_line - splits the line into tokens
 * @line: the line to be split
 * Return: tokens
 */

char **sh_split_line(char *line)
{
int bufsize = SH_TOK_BUFSIZE;
int position = 0;
char **tokens = malloc(bufsize * sizeof(char *));
char *token;

if (!tokens)
{
fprintf(stderr, "sh: allocation error\n");
exit(EXIT_FAILURE);
}
token = strtok(line, SH_TOK_DELIM);
while (token != NULL)
{
tokens[position] = token;
position++;
if (position >= bufsize)
{
bufsize += SH_TOK_BUFSIZE;
tokens = realloc(tokens, (bufsize * (sizeof(char *))));
if (!tokens)
{
fprintf(stderr, "sh: allocation error\n");
exit(EXIT_FAILURE);
}
}
token = strtok(NULL, SH_TOK_DELIM);
}
tokens[position] = NULL;
return (tokens);
}
/**
 * sh_loop - loops the programms of the shell
 */


void sh_loop(void)
{
char *line;
char **args;
int status;

do {
printf("#cisfun$ ");
line = sh_read_line();
args = sh_split_line(line);
status = sh_execute(args);

free(line);
free(args);
} while (status);

}

/**
 * main - function that contains the shell loop
 * Return: 1
 */

int main(void)
{
sh_loop();
return (1);
}

