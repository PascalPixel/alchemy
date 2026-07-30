typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define NULL ((void *)0)

extern u8 Value_00000eb0;
void Func_02002c88(void);
void Func_02002d42(s32, s32, s32);
s32 Func_02002c72(s32);
void Func_02002d6c(s32);
void Func_02002d84(s32, s32);
void Func_02002d7c(s32);
void Func_02002d94(s32, s32);
void Func_02002cc8(void);

void Func_020005e8(void) {
    Func_02002c88();
    Func_02002d42(16, 0, 10);
    if (Func_02002c72(0x840) != 0) {
        Func_02002d6c(0xeb1);
        Func_02002d84(16, 0);
    } else {
        Func_02002d7c((s32)&Value_00000eb0);
        Func_02002d94(16, 0);
    }
    Func_02002cc8();
}
