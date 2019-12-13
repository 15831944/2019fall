#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<dirent.h>
#include<regex.h>
#include<string.h>
#include<time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>

#define TRUE 1
#define FALSE 0
#define MAX_RESULT 500

int nameToggle=FALSE;
int pruneToggle=FALSE;
int mtimeToggle=FALSE;
int ctimeToggle=FALSE;
int printToggle=FALSE;
int execToggle=FALSE;
 
int namePosition=0;
int prunePosition=0;
int mtimePosition=0;
int ctimePosition=0;
int execPosition=0;
 
char* result[MAX_RESULT];
int resultPosition=0;

int matchName(char* string,char* pattern);
int matchMtime(struct stat stat_buf,int n);
int matchCtime(struct stat stat_buf,int n);
char* getPath(char* path,char* filename);
void printHelp();
void initArgvPosition(int argc,char*argv[]);
void findFunc(int argc,char* argv[],char * path);
void execFunc(int argc,char* argv[]);

int dp(int i, int j);

int main ( int argc, char *argv[] )
{
	if(argc<=2){
		printf("args not enough\n");
		printHelp();
		exit(EXIT_FAILURE);
	}
	initArgvPosition(argc,argv);
	findFunc(argc,argv,argv[1]);
	if(execToggle==TRUE){
		execFunc(argc,argv);
	}
	return EXIT_SUCCESS;
}


int matchName(char* s,char* p){
 
   int i = 0, j = 0;
    int memo[50][50] = {{0,},};
    int flag[50][50] = {{0,},};
    int dp(int i, int j)
	{
    	int ret;
    	if (flag[i][j]) return memo[i][j];
    	if (!p[j]) return (!s[i]);
    	int first = s[i] && (p[j] == s[i] || p[j] == '?');
    	if (p[j+1] == '*')
    	{
        	ret = dp(i, j+2) || (first && dp(i+1, j));
    	}
    	else
    	{
        	ret = first && dp(i+1, j+1);
    	}
    	memo[i][j] = ret;
    	flag[i][j] = 1;
    	return ret;
	}
    return dp(0, 0);
}


int matchMtime(struct stat stat_buf,int n){
 
	time_t now=time(NULL);
	time_t mtime=stat_buf.st_mtime;
	int day=(int)difftime(now,mtime)/(60*60*24);
	if(n < 0){
		if(day + n < 0) return TRUE;
	}
	else{
	if(day >= n) return TRUE;
	}
	return FALSE; 
}


int matchCtime(struct stat stat_buf,int n){
 
	time_t now=time(NULL);
	time_t ctime_=stat_buf.st_ctime;
	int day=(int)difftime(now,ctime_)/(60*60*24);
	if(n < 0){
		if(day + n < 0) return TRUE;
	}
	else{
	if(day >= n) return TRUE;
	}
	return FALSE; 
}


char* getPath(char* path,char* filename){
 
	int len1 = strlen(path);
	int len2 = strlen(filename);
	char* str = (char*)malloc(len1 + len2 + 2);
	str[0] = '\0';
	str = strcat(str,path);
	if(path[len1 - 1] != '/') str = strcat(str,"/");
	str = strcat(str,filename);
	return str; 
}


void printHelp(){
	printf("USAGE: ./myfind PATH -name||-ctime||mtime argument\n");
}


void initArgvPosition(int argc,char*argv[]){
 
	int i;
	if(argv[2][0] != '-'){
		printHelp();
		exit(EXIT_FAILURE); 
	}
	for(i = 2; i < argc; i++){ 
		if(argv[i][0] != '-')
			continue;
		if(strcmp(argv[i],"-name")==0){
 
			nameToggle=TRUE;
			namePosition=i;
 
		}
		else if(strcmp(argv[i],"-mtime")==0){
 
			mtimeToggle=TRUE;
			mtimePosition=i;
 
		}
		else if(strcmp(argv[i],"-ctime")==0){
 
			ctimeToggle=TRUE;
			ctimePosition=i;
 
		}
	} 
}


void findFunc(int argc,char* argv[],char * path){
 
	struct dirent *dir_struct;
	DIR* dir;
	struct stat stat_buf;
	if(path == NULL) path = argv[1];
	char* filepath;
 
	if((dir = opendir(path)) == NULL){
		perror("function opendir:");
		printf("dir:%s\n", path);
		return;  
	}
	do{
		errno=0;
		if((dir_struct = readdir(dir)) != NULL){
		int matchToggle=TRUE;
		filepath = getPath(path, dir_struct->d_name);
		if(lstat(filepath, &stat_buf) != 0){
			perror("function stat error");
			printf("stat path:%s\n", filepath);
			continue;
		}
		if(nameToggle == TRUE){
			int len = strlen(argv[namePosition + 1]);
			if(argv[namePosition + 1][0] == '"' && argv[namePosition + 1][len - 1] == '"'){
				int index = 0;
				for(; index < len - 2; index++)
					argv[namePosition + 1][index] = argv[namePosition + 1][index + 1];
				argv[namePosition + 1][len - 2] = '\0';
			}
			if(!matchName(dir_struct->d_name, argv[namePosition + 1]))
				matchToggle=FALSE;
		}
		if(mtimeToggle == TRUE){
			if(!matchMtime(stat_buf, atoi(argv[mtimePosition + 1])))
				matchToggle=FALSE;
		}
		if(ctimeToggle == TRUE){
			if(!matchCtime(stat_buf, atoi(argv[ctimePosition + 1])))
				matchToggle=FALSE;
		}
		if(matchToggle == TRUE){
			if(execToggle == TRUE){
				if(resultPosition<500)
					result[resultPosition++]=filepath;
			}
			else
				printf("%s\n",filepath);
		}
		if(S_ISDIR(stat_buf.st_mode)
			&&strcmp(dir_struct->d_name, ".") != 0
			&&strcmp(dir_struct->d_name, "..") != 0){
			if(pruneToggle == TRUE){
				struct stat stat_buf1, stat_buf2;
				if(lstat(argv[prunePosition + 1], &stat_buf1) != 0){
					perror("function lstat error");
					printf("stat path:%s\n", argv[prunePosition + 1]);
					exit(EXIT_FAILURE);
				}
				if(lstat(filepath, &stat_buf2) != 0){
					perror("function lstat error");
					printf("stat path:%s\n", filepath);
					exit(EXIT_FAILURE);
				}
				if(stat_buf1.st_ino == stat_buf2.st_ino
					&&stat_buf1.st_dev == stat_buf2.st_dev)
					continue;
				}
				findFunc(argc,argv,filepath);
			} 
		}
	}while(dir_struct != NULL);
	if(errno != 0){
		perror("readdir error");
		exit(EXIT_FAILURE); 
	}
	closedir(dir); 
}


void execFunc(int argc,char* argv[]){

	pid_t pid;
	pid = fork();
	if(pid == 0){
		char* cmd[resultPosition + 2];
		int i, j = 0;
		for(i = execPosition + 1; i < argc && argv[i][0] != '-'; i++){
			cmd[j++] = argv[i];
		}
		for(i=0;i<resultPosition-1;i++){
			cmd[j++]=result[i];
		}
		cmd[resultPosition]=(char*)0;
		execvp(cmd[0],cmd);
	}
	else if(pid > 0){ 
		wait(NULL); 
	} 
}

