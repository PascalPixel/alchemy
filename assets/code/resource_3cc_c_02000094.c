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

void Func_0200015c(s32);
void Func_0200015e(s32, s32, s32);

void Func_02000094(void) {
    Func_0200015e(0xB, 3, 0);
    Func_0200015c(0x6C);
}
