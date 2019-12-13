#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>



int in_cd(char** args);
int in_help(char** args);
int in_exit(char** args);
int in_history(char** args);


char* builtin_str[] = {
	"cd", 
	"help", 
	"exit",
	"history"
};


int (*builtin_func[]) (char**) = {
	&in_cd, 
	&in_help, 
	&in_exit, 
	&in_history
};


int num_builtins()
{
	return sizeof(builtin_str) / sizeof(char*);
}


int in_cd(char** args)
{
	if(args[1] == NULL)
	{
		fprintf(stderr, "expected arg to be \"cd\".\n");
	}else{
		if(chdir(args[1]) != 0)
		{
			perror("myshell");
		}
	}
	return 1;
}


int in_help(char** args)
{
	printf("USAGE: <command> <args>\n");
	printf("built in: \n");
	for(int i = 0; i < num_builtins(); i++)
	{
		printf("	%s\n", builtin_str[i]);
	}
	printf("Use the `man` command for more information on other commands.\n");
	return 1;
}


int in_exit(char** args)
{
	return 0;
}


char* read_line()
{
	char* line = NULL;
	ssize_t buf_size = 0;
	getline(&line, &buf_size, stdin);
	return line;
}


char** split_line(char* line)
{
	int buf_size = 64;
	int position = 0;
	char** tokens = malloc(buf_size * sizeof(char*));
	char* token;

	if(!tokens)
	{
		fprintf(stderr, "alloc err.\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, " \t\r\n\a");
	while(token != NULL)
	{
		tokens[position] = token;
		position++;

		if(position >= buf_size)
		{
			buf_size += 64;
			tokens = realloc(tokens, buf_size * sizeof(char*));
			if(!tokens)
			{
				fprintf(stderr, "alloc err.\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, " \t\r\n\a");
	}
	tokens[position] = NULL;
	return tokens;
}


int launch(char** args)
{
	pid_t pid;
	pid_t wpid;
	int status;

	pid = fork();
	if(pid == 0)
	{
		if(execvp(args[0], args) == -1)
		{
			perror("myshell");
		}
		exit(EXIT_FAILURE);
	}else if(pid < 0){
		perror("myshell");
	}else{
		do{
			wpid = waitpid(pid, &status, WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}


int in_history(char** args)
{
	FILE* f;
	char ch;
	if(!(f = fopen(".history", "r")))
	{
		printf("error\n");
		exit(EXIT_FAILURE);
	}
	ch = fgetc(f);
	while(!feof(f)){
		putchar(ch);
		ch = fgetc(f);
	}
	fclose(f);
	return 1;
}


int execute(char** args)
{
	if(args[0] == NULL)
	{
		return 1;
	}
	for(int i = 0; i < num_builtins(); i++)
	{
		if(strcmp(args[0], builtin_str[i]) == 0)
		{
			return (*builtin_func[i])(args);
		}
	}

	return launch(args);
}


void loop()
{
	char * line;
	char ** args;
	int status;
	FILE* fp = fopen(".history", "w");	
	do{
		printf("> ");
		line = read_line();
		if(line[0] != '\n')
			fputs(line, fp);
		args = split_line(line);
		status = execute(args);

		free(line);
		free(args);
	}while(status);
	fclose(fp);
}


int main()
{
	loop();
	return 0;
}
