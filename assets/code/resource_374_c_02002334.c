typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define NULL ((void *)0)

s32 Func_02004930(s32, s32);
void Func_020049b0(s32, s32);
void Func_020047a4(s32);

void Func_02002334(s32 arg0) {
    volatile s32 *p = (volatile s32 *)0x03001e40;
    if ((*p & 1) != 0) {
        s32 t = Func_02004930((u32)*p >> 1, 6);
        Func_020049b0(arg0, t);
    }
    if ((*p & 15) == 0) {
        Func_020047a4(arg0);
    }
}
