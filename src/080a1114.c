typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)

s32 Func_08015018(void *);

void Func_080a1114(void **arg0) {
    if (*arg0 != NULL) {
        Func_08015018(*arg0);
        *arg0 = NULL;
    }
}
