#include "types.h"

extern unsigned char Value_0000240d;

void Func_020077f8(void);
void Func_02007862(s32, s32, s32);
void Func_0200783a(s32, s32);
void Func_0200788a(s32, s32);
void Func_02007990(s32);
void Func_0200790c(s32, s32, s32);
void Func_02007918(s32, s32, s32);
void Func_020078da(s32, s32, s32);
void Func_020078e4(s32, s32, s32);
void Func_02007904(s32);
void Func_0200791c(s32, s32);
void Func_0200794a(s32, s32, s32);
void Func_02007920(s32);
void Func_02007938(s32, s32);
void Func_020079bc(void);
void Func_02007872(s32);
void Func_020079a8(s32);
void Func_0200788c(void);

void Func_020021c4(void)
{
    Func_020077f8();
    Func_02007862(0, 0, 0);
    Func_0200783a(0, 1);
    Func_0200788a(0, 1);
    Func_02007990(113);
    Func_0200790c(21, 256, 0);
    Func_02007918(13, 256, 60);
    Func_020078da(21, 0, 0);
    Func_020078e4(13, 0, 0);
    {
    s32 t = (s32)&Value_0000240d;
    Func_02007904(t);
    Func_0200791c(13, 0);
    Func_0200794a(0, 258, 30);
    Func_02007920(t + 1);
    }
    Func_02007938(13, 0);
    Func_020079bc();
    Func_02007872(60);
    Func_020079a8(60);
    Func_0200788c();
    Func_02007872(549);
}
