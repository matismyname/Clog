#include "clog.h"
#include <string.h> //strlen, strcpy etc.
#include <unistd.h> //To check if the file exists
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

void Clog(char* msg, char* log_level)
{
	char* log_dir = Create_Log_Directory();
	Write_Log(msg, log_dir, log_level);
}

static char* Create_Log_Directory()
{
	//Create the name of the log directory
	char* cwd = getcwd(NULL, 0);
	char* logdir = "/logs/";
	char* full_log_dir = malloc(strlen(cwd) + strlen(logdir));
	strcpy(full_log_dir, cwd);
	strcat(full_log_dir, logdir);

	//See if the log directory exists
	DIR* dir = opendir(full_log_dir);
	//If the log directory exists, change into it
	//Otherwise, create it by a syscall
	if (dir)
	{
		return full_log_dir;
	}
	else
	{
		mkdir(full_log_dir, 0700); //Read and write
	}
	return full_log_dir;
}

static void Write_Log(char* msg, char* log_dir, char* log_level)
{
	chdir(log_dir);
	char* filename = strcat(log_dir, log_level);
	time_t rawtime;
	char* time_string;
	rawtime = time(NULL);
	time_string = strtok(ctime(&rawtime), "\n");
	char* msg_dash = "--";
	char* pipe = "|";
	char* final_msg = strcat(time_string, msg_dash);
	final_msg = strcat(final_msg, pipe);
	final_msg = strcat(final_msg, log_level);
	final_msg = strcat(final_msg, pipe);
	final_msg = strcat(final_msg, msg);

	FILE *fptr = fopen(filename, "a");
	if (fptr == NULL)
	{
		fprintf(stderr, "An error occured trying to open the log file for the level %s \n", log_level);
		perror("open");
		exit(-1);
	}
	fputs(final_msg, fptr);
	fclose(fptr);
	chdir("..");
}

void Delete_Log(char* log_level)
{
	char* log_dir = Create_Log_Directory();
	DIR* dir = opendir(log_dir);
	char* filename = log_level;

	if(dir == NULL)
	{
		fprintf(stderr, "An error occured trying to change to the directory %s \n", log_dir);
		perror("chdir");
	}
	chdir(log_dir);
	//If the file exists, delete it
	if(access(filename, F_OK) == 0)
	{
		remove(filename);
	}
	chdir("..");
}

void Delete_Logs()
{
	char* log_levels[] = {"ERROR", "DEBUG", "MESSAGE"};
	for(int i = 0; i<3; i++)
	{
		Delete_Log(log_levels[i]);
	}
}
