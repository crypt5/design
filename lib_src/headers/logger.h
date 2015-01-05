#ifndef LOGGER_H_
#define LOGGER_H_

typedef struct logger_t LOGGER;

extern LOGGER *logger_init(char* filename);
extern void logger_log(LOGGER *log,char* message);
extern void logger_shutdown(LOGGER *log);

#endif
