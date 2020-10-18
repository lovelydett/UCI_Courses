#define LOGERR(...)                  \
    printf("%s:line %d:\t",__FILE__,__LINE__);\
	printf(__VA_ARGS__);\
	printf("\n");\

 
#define __format(__fmt__) "%d " __fmt__ "\n"
 
#define LOGINF(__fmt__, ...)                                  \
    do                                                     \
    {                                                      \
        __log(__format(__fmt__), __LINE__,__FILE__,##__VA_ARGS__);  \
    }while(0)
 
#include <cstdio>
int main() {
	LOGERR("Hello %s", "world");
	return 0;
}