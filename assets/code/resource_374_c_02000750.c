typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define NULL ((void *)0)

extern u8 Data_0200beb4[];
void Func_02002fa2(s32);
void Func_02002d9c(void *, s32, s32);
void Func_02002e66(s32, s32, s32);
void Func_02000e84(s32);

void Func_02000750(void) {
    Func_02002fa2(158);
    Func_02002d9c(Data_0200beb4, 44, 7);
    Func_02002e66(0, 248, 0x117);
    Func_02000e84(1);
}
