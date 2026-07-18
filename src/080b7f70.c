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

struct Fields_080b7f70 {
    u8 filler[0x50];
    void *field50;
    u8 field54;
};

void *Func_080b7f70(struct Fields_080b7f70 *arg0, s32 arg1) {
    s32 type = arg0->field54 & 0xF;
    if (type == 1) {
        if (arg1 == 0) {
            return arg0->field50;
        }
    } else if (type == 2) {
        return ((void **)arg0->field50)[arg1];
    }
    return NULL;
}
