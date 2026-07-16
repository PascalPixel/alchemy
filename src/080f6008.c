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

s32 Func_080f7460();
s32 Func_080f9010(s32);
extern u8 Data_02000240[];

s32 Func_080f6008(void) {
    register u8 *base asm("r3");

    *(s16 *)0x04000000 = 0x40;
    base = Data_02000240;
    *(s32 *)0x03001CB4 = M2C_FIELD(base, s32 *, 4);
    Func_080f9010(9);
    Func_080f7460();
    return 0;
}
