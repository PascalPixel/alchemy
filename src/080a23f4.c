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

struct Fields_080a23f4 {
    u8 filler[8];
    u16 field8;
    u16 field10;
    u16 field12;
    u16 field14;
};

void Func_080a23f4(struct Fields_080a23f4 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    if (arg0 != NULL) {
        arg0->field8 = arg3;
        arg0->field12 = arg1;
        arg0->field10 = arg4;
        arg0->field14 = arg2;
    }
}
