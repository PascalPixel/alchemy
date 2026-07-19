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

extern s32 Func_080150a8(s32, s32, s32, s32, s32);
extern void Func_08015080(s32, s32, s32, s32);
extern u8 Data_02000240[];

void Func_080a23c0(s32 arg0) {
    u8 *base;
    base = Data_02000240;
    Func_080150a8(M2C_FIELD(base, s32 *, 0x10), 7, arg0, 8, 0);
    Func_08015080(0xB0B, arg0, 0x40, 0);
}
