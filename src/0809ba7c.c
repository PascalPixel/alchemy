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

struct Fields_0809ba7c {
    u8 filler[0x34];
    s32 field34;
    s16 field38;
    s16 field3A;
    u8 filler3C[4];
    u8 field40;
};

void Func_0809ba7c(struct Fields_0809ba7c *arg0, s32 arg1) {
    arg0->field34 = arg1;
    arg0->field3A = 0;
    arg0->field38 = 0;
    arg0->field40 = 0;
}
