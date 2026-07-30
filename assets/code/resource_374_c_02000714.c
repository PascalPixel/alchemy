typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define NULL ((void *)0)

s32 Func_02002d90(s32);
void Func_02002f20(void);
void Func_02002f0c(s32);

void Func_02000714(s32 arg0) {
    u8 *p;
    if (Func_02002d90(0x834) != 0) {
        Func_02002f20();
    }
    p = *(u8 **)0x03001ebc;
    *(s32 *)(p + 0x1c0) = 0x100;
    *(s32 *)(p + 0x1c8) = 16;
    Func_02002f0c(arg0);
}
