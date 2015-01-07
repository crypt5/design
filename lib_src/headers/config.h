#ifndef CONFIG_H_
#define CONFIG_H_

#define TRUE 0
#define FALSE 1

#define VALUE_NOT_FOUND 52637489

typedef struct config_t CONFIG;

extern CONFIG* config_init();
extern void config_load_file(CONFIG* c,char* filename);
extern void config_destroy(CONFIG* c); 

extern int config_get_int(CONFIG* c, char* key); 
extern double config_get_double(CONFIG* c, char* key);
extern char* config_get_string(CONFIG* c, char* key);
extern int config_get_boolean(CONFIG* c, char* key);


#endif
