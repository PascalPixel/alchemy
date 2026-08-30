#include "types.h"

#define FieldScene_DispatchByFacingAndFlags Func_020013b8

extern u8 * Func_020057c2();
extern void Func_020057a0();
extern void Func_0200226c(void);
extern void Func_02001fde(void);
extern void Func_020021b2(void);
extern void Func_020020d8(void);
extern void Func_020058f4();
extern void Func_02005914();
extern void Func_02005910(void);
extern s32 Func_020057e6();
extern s32 Func_020057f0();
extern void Func_0200246a();
extern void Func_020025a4(void);
extern void Func_0200582c();
extern s32 Func_0200580c();
extern void Func_02002486();
extern s32 Func_0200581e();
extern void Func_020028fe(void);
extern void Func_0200322c(void);
extern void Func_02005858();
void FieldScene_DispatchByFacingAndFlags(void)
{
    u8 *record = Func_020057c2(0);
    u16 facing;

    Func_020057a0();

    facing = *(u16 *)(record + 6);
    if ((u16)(facing - 0x2000) <= 0x3fff) {
        Func_0200226c();
    } else if ((u16)(facing - 0x6000) <= 0x3fff) {
        Func_02001fde();
    } else if ((u16)(facing + 0x6000) <= 0x3fff) {
        Func_020021b2();
    } else {
        Func_020020d8();
    }

    Func_020058f4(0x10000, 0x2000);
    Func_02005914(20, 1);
    Func_02005910();

    if (*(s16 *)(record + 18) <= 209) {
        if (Func_020057e6(0x89a) == 0) goto scene0;
        if (Func_020057f0(0x89b) != 0) goto scene0;
        goto scene1;
scene0:
        Func_0200246a(0);
        goto firstSceneComplete;
scene1:
        Func_020025a4();
firstSceneComplete:
        Func_0200582c();
        return;
    }

    if (Func_0200580c(0x89b) != 0) {
        Func_02002486(2);
    } else if (Func_0200581e(0x89a) == 0) {
        Func_020028fe();
    } else {
        Func_0200322c();
    }
    Func_02005858();
}
