#ifndef _READCONF_H
#define _READCONF_H
typedef struct item_t {
    char *key;
    char *value;
}ITEM;

extern int read_conf_value(const char *key,char *value,const char *file);
extern int write_conf_value(const char *key,char *value,const char *file);
#endif
