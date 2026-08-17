#include "types.h"

extern u8 *Func_0200487a(s32 actor);
extern s32 Func_020047ba(void (*callback)(void), s32 interval);
extern s32 Func_020047c4(void (*callback)(void), s32 interval);
extern void Func_02004946(s32, s32, s32);
extern s32 Func_0200486c(s32 flag);
extern void Func_020048f0(s32 actor, s32 pose);
extern void Func_02003e3c(void);
extern void Func_02003fa0(void);
extern void Func_02004074(void);
extern void Func_020041d4(void);
extern void Func_020041f8(void);
extern s32 Func_0200492c(s32 mode, s32 actor);
extern u8 *Func_020048d2(s32 actor);
extern u8 *Func_020048dc(s32 actor);
extern void Func_0200419c(void);
extern s32 Func_020048b4(s32 flag);
extern void Func_02003d38(void);
extern void Func_0200493c(s32 actor, s32 pose);
extern s32 Func_020048ca(s32 flag);
extern s32 Func_020048d4(s32 flag);
extern void Func_02004958(s32, s32, s32);
extern u8 *Func_02004926(s32 actor);
extern s32 Func_020048fa(s32 flag);
extern void Func_0200497c(s32, s32, s32);
extern u8 *Func_0200494a(s32 actor);
extern void Func_020048c4(u8 *actor, s32 value);
extern s32 Func_02004924(s32 flag);
extern void Func_020049a6(s32, s32, s32);
extern void Func_020049b6(s32, s32, s32);

/* Install the scene callbacks and position its flag-selected actors. */
void Func_02002334(void)
{
    u8 *actor;
    s32 flag;

    Func_0200487a(14)[85] = 0;
    Func_020047ba((void (*)(void))0x02009e95, 3200);
    Func_020047c4((void (*)(void))0x02009edd, 3200);
    Func_02004946(107, 0, 0);
    if (Func_0200486c(0xed9) != 0)
        Func_020048f0(14, 2);

    Func_02003e3c();
    Func_02003fa0();
    Func_02004074();
    Func_020041d4();
    Func_020041f8();
    Func_0200492c(8, 3);
    Func_020048d2(11)[85] = 0;
    Func_020048dc(12)[85] = 0;
    Func_0200419c();

    if (Func_020048b4(0x200) != 0) {
        Func_02003d38();
        Func_0200493c(13, 5);
    }
    if (Func_020048ca(0x109) != 0)
        return;

    flag = Func_020048d4(0x9ca);
    if (flag != 0) {
        Func_02004958(15, 0x3580000, 0x3380000);
        actor = Func_02004926(15);
        *(void (**)(void))(actor + 108) = (void (*)(void))0x02008aa9;
    } else if (Func_020048fa(0x9c9) != 0) {
        Func_0200497c(15, 0x3780000, 0x2980000);
        actor = Func_0200494a(15);
        *(u16 *)(*(u8 **)(actor + 80) + 30) = 0;
        Func_020048c4(actor, 16);
    } else if (Func_02004924(0x9c8) != 0) {
        Func_020049a6(15, 0x2480000, 0x2a80000);
    } else {
        Func_020049b6(15, 0x2480000, 0x2980000);
    }
}
