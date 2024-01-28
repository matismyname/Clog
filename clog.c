#include "clog.h"
#include <string.h> //strlen, strcpy etc.
#include <unistd.h> //To check if the file exists
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#LOGDIR //Your logdir here

void Clog(const char* msg, const char* log_level)
{
	const char* log_dir = CreateLogDirectory();
	WriteLog(msg, log_dir, log_level);
}

void Clog_char(const char* fmsg, const char* smsg, const char* log_level)
{
	const char* final_msg = CombineMessages_char(fmsg, smsg);
	const char* log_dir = CreateLogDirectory();
	WriteLog(final_msg, log_dir, log_level);
}

void Clog_int(const char* msg, int d, const char* log_level)
{
	const char* final_msg = CombineMessages_int(msg, d);
	const char* log_dir = CreateLogDirectory();
	WriteLog(final_msg, log_dir, log_level);
}

void Clog_float(const char* msg, float f, const char* log_level)
{
	const char* final_msg = CombineMessages_float(msg, f);
	const char* log_dir = CreateLogDirectory();
	WriteLog(final_msg, log_dir, log_level);
}

void Clog_double(const char* msg, double dd, const char* log_level)
{
	const char* final_msg = CombineMessages_double(msg, dd);
	const char* log_dir = CreateLogDirectory();
	WriteLog(final_msg, log_dir, log_level);
}

const char* CreateLogDirectory(void)
{
	//See if the log directory exists
	DIR* dir = opendir(LOGDIR);
	//If the log directory exists, change into it
	//Otherwise, create it by a syscall
	if (dir)
	{
		closedir(dir);
		return LOGDIR;
	}
	else
	{
		closedir(dir);
		mkdir(LOGDIR, 0700); //Read and write
	}
	return LOGDIR;
}

void WriteLog(const char* msg, const char* log_dir, const char* log_level)
{
	if (!log_dir)
	{
		char errmsg[256];
		strcpy(errmsg, "The directory ");
		strcat(errmsg, log_dir);
		strcat(errmsg, " does not exist!\n");
		fprintf(stderr, errmsg);
		exit(EXIT_FAILURE);
	}

	// Change the working directory
	if (chdir(log_dir) != 0)
	{
		char errmsg[256];
		strcpy(errmsg, "Could not change into the directory ");
		strcat(errmsg, log_dir);
		strcat(errmsg, "!\n");
		fprintf(stderr, errmsg);
		perror("chdir");
		exit(EXIT_FAILURE);
	}
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));

	// Create a filename using snprintf
	char filename[256];
	snprintf(filename, sizeof(filename), "%s/%s", log_dir, log_level);

	// Get the current time
	setlocale(LC_TIME, "C");
	time_t rawtime;
	time(&rawtime);
	struct tm* timeinfo = localtime(&rawtime);
	if (timeinfo == NULL) {
		fprintf(stderr, "The timeinfo is null!\n");
		exit(EXIT_FAILURE);
	}

	char time_string[128];
	strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", timeinfo);

	// Create the final log message
	char final_msg[512];
	snprintf(final_msg, sizeof(final_msg), "%s--|%s|%s \n", time_string, log_level, msg);

	// Open the file for appending
	FILE *fptr = fopen(filename, "a");
	if (fptr == NULL) {
		fprintf(stderr, "An error occurred trying to open the log file for the level %s\n", log_level);
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	if (fputs(final_msg, fptr) == EOF)
	{
		fprintf(stderr, "Error writing to file!\n");
		perror("fputs");
		exit(EXIT_FAILURE);
	}

	if (fclose(fptr) == EOF)
	{
	    fprintf(stderr, "An error occurred trying to close the log file for the level %s\n", log_level);
	    perror("fclose");
	    exit(EXIT_FAILURE);
	}

	// Change back to the original directory
	if (chdir("..") != 0)
	{
		char errmsg[256];
		strcpy(errmsg, "Could not change into the parent directory from the log directory!\n");
		fprintf(stderr, errmsg);
		perror("chdir");
		exit(EXIT_FAILURE);
	}
}

void DeleteLog(const char* log_level)
{
	const char* log_dir = CreateLogDirectory();
	DIR* dir = opendir(log_dir);

	if (chdir(log_dir) != 0)
	{
		char errmsg[256];
		strcpy(errmsg, "Could not open the logs directory ");
		strcat(errmsg, log_dir);
		strcat(errmsg, "!\n");
		fprintf(stderr, errmsg);
		perror("opendir");
		exit(EXIT_FAILURE);

	}
	chdir(log_dir);
	closedir(dir);

	const char* filename = log_level;
	//If the file exists, delete it
	if(access(filename, F_OK) == 0)
	{
		remove(filename);
	}

	if (chdir("..") != 0) {
		char errmsg[256];
		strcpy(errmsg, "Could not change into the parent directory from the log directory!\n");
		fprintf(stderr, errmsg);
		perror("chdir");
		exit(EXIT_FAILURE);
	}
}

void DeleteLogs(void)
{
	const char* log_levels[] = {"ERROR", "DEBUG", "MESSAGE"}; //Log levers, change to your liking
	for(int i = 0; i<3; i++)
	{
		DeleteLog(log_levels[i]);
	}
}

//Since Clog takes only two parameters
const char* CombineMessages_char(const char* msg1, const char* msg2)
{
	size_t combl = strlen(msg1) + strlen(msg2) + 1;
	char* combm = (char*)malloc(combl);
	if (combm != NULL)
	{
		strcpy(combm, msg1);
		strcat(combm, msg2);
		return combm;
	}
	else
	{
		fprintf(stderr, "Memory allocation failed in CombineMessages! \n");
		exit(EXIT_FAILURE);
	}
}

const char* CombineMessages_int(const char* msg, int d)
{
	char istring[64];
	sprintf(istring, "%d", d);	
	return CombineMessages_char(msg, istring);
}

const char* CombineMessages_float(const char* msg, float f)
{
	char istring[64];
	snprintf(istring, 64, "%0g", f);	
	return CombineMessages_char(msg, istring);
}

const char* CombineMessages_double(const char* msg, double dd)
{
	char istring[64];
	snprintf(istring, 64, "%0g", dd);	
	return CombineMessages_char(msg, istring);
}
