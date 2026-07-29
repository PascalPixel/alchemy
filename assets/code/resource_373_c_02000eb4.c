typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern s32 Func_02006e56(s32);
extern s32 Func_02006e60(s32);
extern void Func_02006e88(void);
extern void Func_02006f56(s32);
extern s32 Func_02006f66(s32, s32);
extern s32 Func_02006ebe(s32, s32);
extern void Func_02006f8c(s32, s32, s32);
extern void Func_02006f8c_a(s32, s32);
extern void Func_02006ec4(s32);
extern void Func_02006fac(s32, s32);
extern void Func_02006ee0(void);
extern void Func_02007070(s32);
extern void Func_02006e5a(s32, s32, s32);
extern void Func_02006f58(s32, s32, s32);
extern void Func_0200703e(s32);
extern s32 Data_0200f55a[];

void Func_02000eb4(void)
{
    if (Func_02006e56(0x815) != 0 && Func_02006e60(0x87a) == 0) {
        Func_02006e88();
        Func_02006f56(0x11b6);
        Func_02006f66(21, 0);
        if (Func_02006ebe(0, 0) == 0) {
            Func_02006f8c(21, 0, 60);
            Func_02006f8c_a(21, 0);
        } else {
            u8 *b = *(u8 **)0x03001ebc;
            u16 *h = (u16 *)(b + 0x1d8);
            *h = *h + 2;
            Func_02006ec4(40);
            Func_02006fac(21, 0);
        }
        Func_02006ee0();
    } else {
        Func_02007070(0x9e);
        Func_02006e5a((s32)Data_0200f55a, 50, 44);
        Func_02006f58(0, 0x154, 0x378);
        Func_0200703e(7);
    }
}
