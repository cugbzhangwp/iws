#include <sys/sysinfo.h>
typedef struct oso_n_statfs{
	struct statfs diskInfo;
	char tgt[50];
}oso_n_statfs;
