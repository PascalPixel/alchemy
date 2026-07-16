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
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_0808adf0();
s32 Func_0808ae74(s32, s32);

s32 Func_0808b048(s32 arg0, s32 arg1) {
    return Func_0808ae74(Func_0808adf0(), arg1);
}
