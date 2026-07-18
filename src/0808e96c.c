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

s32 Func_0808e4b4(s32, u16, void *);

s32 Func_0808e96c(s32 arg0) {
    s32 local;
    s32 result = Func_0808e4b4(0x70000005, arg0, &local);
    return (u32)((-result) | result) >> 31;
}
