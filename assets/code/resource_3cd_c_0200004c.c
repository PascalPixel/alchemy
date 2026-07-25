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

u8 Func_020008f2(s32);
u8 Func_0200090e();

void Func_0200004c(void) {
    Func_020008f2(0x23CD);
    Func_0200090e(0xD, 0x102, 0);
    Func_0200090e(0xD, 0);
}
