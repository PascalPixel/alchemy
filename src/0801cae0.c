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

s16 Func_0801cbd4(s32, s32, s32, s32);

void Func_0801cae0(s32 arg0) {
    *(s16 *)0x050001E8 = Func_0801cbd4(arg0, 0xEEEE, 0xCCCC, 0x11110);
    *(s16 *)0x050001EA = Func_0801cbd4(arg0, 0xD555, 0xBBBB, 0xEEEE);
    *(s16 *)0x050001EC = Func_0801cbd4(arg0, 0xBBBB, 0xAAAA, 0xCCCC);
    *(s16 *)0x050001EE = Func_0801cbd4(arg0, 0xA221, 0x9999, 0xAAAA);
    *(s16 *)0x050001F0 = Func_0801cbd4(arg0, 0x10888, 0xDDDD, 0x13333);
    *(s16 *)0x050001F2 = Func_0801cbd4(arg0, 0x12221, 0xEEEE, 0x15555);
    *(s16 *)0x050001F4 = Func_0801cbd4(arg0, 0x13BBB, 0x10000, 0x17777);
}
