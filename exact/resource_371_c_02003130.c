#include "types.h"

void Func_02007410(void);
void Func_020075de(s32);
void Func_0200756e(s32, s32);
void Func_02007584(s32);
void Func_020072da(s32);
void Func_020075a4(void);
void Func_020075b8(void);
void Func_02007508(s32, s32);
u8 *Func_02007476(s32);
void Func_020073c4(u8 *, s32);
void Func_0200730a(s32);
void Func_0200757e(s32, s32);
void Func_02007328(void *, s32);
void Func_020075d0(s32, s32);
void Func_020075da(s32, s32);
void Func_020075f0(s32);
void Func_02007496(s32);
void Func_020075ee(s32, s32);
void Func_02007604(s32);
void Func_0200735a(s32);
void Func_020075e8(s32);
void Func_02007478(s32);
void Func_020074cc(void);

/* Stage and time the transition driven by the following owner callback. */
void Func_02003130(void)
{
    Func_02007410();
    Func_020075de(141);
    Func_0200756e(0, 0);
    Func_0200756e(0, 0);
    Func_02007584(1);
    Func_020072da(2);
    *(s32 *)(*(u8 **)0x03001ebc + 456) = 1;
    Func_020075a4();
    Func_020075b8();
    Func_02007508(0, 15);
    Func_020073c4(Func_02007476(0), 0);
    Func_0200730a(1);
    Func_0200757e(0x40000, 0x8000);
    {
        s32 delay = 3200;
        void *cb = (void *)0x0200b1fd;
        Func_02007328(cb, delay);
    }
    Func_020075d0(0, 0);
    Func_020075d0(0x10004, 1);
    Func_020075da(0x10000, 2);
    Func_020075f0(40);
    Func_02007496(240);
    Func_020075ee(0, 0);
    Func_02007604(80);
    Func_0200735a(90);
    Func_020075e8(109);
    Func_02007478(282);
    Func_020074cc();
}
