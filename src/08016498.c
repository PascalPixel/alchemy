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
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080170f8(u16, u16, u16, u16);

void Func_08016498(void *arg0) {
    Func_080170f8(M2C_FIELD(arg0, u16 *, 0xC), M2C_FIELD(arg0, u16 *, 0xE), M2C_FIELD(arg0, u16 *, 8), M2C_FIELD(arg0, u16 *, 0xA));
}
