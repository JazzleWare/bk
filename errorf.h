#ifndef ERRORF_H
#define ERRORF_H
void noop(const char *fmt, ...) {}
#ifndef HAS_ERRORF
#define errorf noop
#else
#define errorf printf

#endif 
#endif 
