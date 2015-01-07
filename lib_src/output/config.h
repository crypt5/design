#ifndef CONFIG_H_
#define CONFIG_H_

#define TRUE 0
#define FALSE 1

#define VALUE_NOT_FOUND 52637489

typedef struct config_t CONFIG;

extern CONFIG* config_init(); //done
extern void config_load_file(CONFIG* c,char* filename); //done
extern void config_destroy(CONFIG* c); //done

extern int config_get_int(CONFIG* c, char* key); //done
extern double config_get_double(CONFIG* c, char* key);//done
extern char* config_get_string(CONFIG* c, char* key);//done
extern int config_get_boolean(CONFIG* c, char* key);


#endif
