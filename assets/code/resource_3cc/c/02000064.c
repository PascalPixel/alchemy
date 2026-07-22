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

void Func_0200012c(s32);
void Func_0200012e(s32, s32, s32);

void Func_02000064(void) {
    Func_0200012e(9, 1, 0);
    Func_0200012c(0x44);
}
