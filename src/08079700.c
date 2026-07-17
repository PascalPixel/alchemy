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
struct Wallet {
    u8 pad[0x10];
    s32 coins;
};

extern struct Wallet Data_02000240;

s32 Func_08079700(s32 arg0) {
    register s32 value asm("r3") = Data_02000240.coins + arg0;
    if (value > 999999)
        value = 999999;
    if (value < 0)
        value = 0;
    Data_02000240.coins = value;
    return value;
}
