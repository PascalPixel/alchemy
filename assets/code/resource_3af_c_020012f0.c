#include "types.h"

extern void Func_02005706(s32, s32);
extern void Func_02005720(s32, s32, s32, s32);
extern void Func_0200572c(void);
extern void Func_020055fa(s32);
extern void Func_02001f0a(void);
extern void Func_020055c2(s32, s32, s32, s32, s32, s32);
extern void Func_02005618(s32);
extern void Func_020056ae(s32, s32, s32);
extern void Func_02005668(s32, s32, s32);
extern void Func_020056b2(s32, s32, s32);
extern void Func_020056fc();
extern void Func_020056e4(s32, s32);
extern void Func_02005768(s32, s32, s32);
extern void Func_02005722(s32, s32, s32);
extern void Func_0200571a(s32, s32);
extern void Func_02005740(s32);
extern void Func_02005762(s32, s32, s32);
extern void Func_02005796(s32, s32, s32);
extern void Func_02005684(s32);

extern u8 Value_0000032e;
extern u8 Value_00001d8d;
extern u8 Value_00000923;

void Func_020012f0(void)
{
    Func_02005706(0x19999, 0x3333);
    Func_02005720(0xD80000, -1, 0x3380000, 1);
    Func_0200572c();
    Func_020055fa(20);
    Func_02001f0a();
    {
        s32 x = 1, y = 2;
        Func_020055c2(30, 108, 13, 108, x, y);
    }
    Func_02005618(10);
    Func_020056ae(20, 0xD80000, 0x3200000);
    Func_02005668(20, 0x13333, 0x9999);
    Func_020056b2(20, 0xD8, (s32)&Value_0000032e);
    Func_020056fc(0, 20, 10);
    Func_020056e4(20, 4);
    Func_020056fc(20, 2);
    Func_02005768(20, 0x100, 20);
    Func_02005722(20, 0, 20);
    Func_0200571a(20, 2);
    Func_02005740((s32)&Value_00001d8d);
    Func_02005762(20, 0, 20);
    Func_02005796(20, 0x102, 0);
    Func_02005684((s32)&Value_00000923);
}
