#ifndef _CLOG_H_
#define _CLOG_H_

void Clog(char* msg, char* log_level);
static char* Create_Log_Directory();
static void Write_Log(char* msg, char* log_dir, char* log_level);
void Delete_Log(char* log_level);
void Delete_Logs();

#endif
