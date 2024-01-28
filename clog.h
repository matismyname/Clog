#ifndef _CLOG_H_
#define _CLOG_H_

void Clog(const char* msg, const char* log_level);
void Clog_char(const char* fmsg, const char* smsg, const char* log_level);
void Clog_int(const char* msg, int d, const char* log_level);
void Clog_float(const char* msg, float f, const char* log_level);
void Clog_double(const char* msg, double dd, const char* log_level);
const char* CreateLogDirectory(void);
void WriteLog(const char* msg, const char* log_dir, const char* log_level);
void DeleteLog(const char* log_level);
void DeleteLogs(void);
const char* CombineMessages_char(const char* msg1, const char* msg2);
const char* CombineMessages_int(const char* msg1, int d);
const char* CombineMessages_float(const char* msg1, float f);
const char* CombineMessages_double(const char* msg1, double dd);

#endif
