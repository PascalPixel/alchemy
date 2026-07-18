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

void Func_080a1038(void *);
s32 Func_080153b0(s32, s32, s32);

s32 Func_080b0634(s32 arg0) {
    u8 **slot = (u8 **)(*(u8 **)0x03001F2C + 0x380);
    u8 saved = (*slot)[5];
    Func_080a1038(*slot);
    arg0 = Func_080153b0(7, 5, arg0);
    (*slot)[5] = saved;
    return arg0;
}
