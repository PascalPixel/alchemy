typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define NULL ((void *)0)

s32 Func_0200496c(s32, s32);
void Func_020049ec(s32, s32);

void Func_02002370(s32 arg0) {
    volatile s32 *p = (volatile s32 *)0x03001e40;
    if ((*p & 1) != 0) {
        s32 t = Func_0200496c((u32)*p >> 1, 6);
        Func_020049ec(arg0, t);
    }
}
