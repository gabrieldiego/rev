#ifndef __DEBUG_H__
#define __DEBUG_H__

#define DEBUG_LINE (fprintf(stderr,"Line %d of file %s\n",__LINE__,__FILE__));
#define DEBUG_PTR(ptr) (fprintf(stderr,"Ptr %s (%p) in line %d of file %s\n",#ptr,(ptr),__LINE__,__FILE__));
#define DEBUG_INT(x) (fprintf(stderr,"Int %s %d in line %d of file %s\n",#x,(x),__LINE__,__FILE__));
#define DEBUG_STR(...) fprintf(stderr,__VA_ARGS__);

#endif /* __DEBUG_H__ */
