typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define NULL ((void *)0)

void Func_02002cd4(void);
s32 Func_02002cb2(s32);
void Func_02002dac(s32);
void Func_02002db4(s32);
void Func_02002cd2(s32);
void Func_02002dd2(s32, s32);
void Func_02002d06(void);

void Func_02000634(void) {
    Func_02002cd4();
    if (Func_02002cb2(0x302) != 0) {
        Func_02002dac(0x1be4);
    } else {
        Func_02002db4(0x1be3);
        Func_02002cd2(0x302);
    }
    Func_02002dd2(11, 0);
    Func_02002d06();
}
