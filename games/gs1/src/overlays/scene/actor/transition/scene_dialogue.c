#include "types.h"

#define SceneDialogue_ShowLineF13WithWorkA488 Func_02001b9c
#define SceneDialogue_RunLine17e2 Func_02001f20
extern s32 Data_0200a488;

void Func_02003ed8(void);
void Func_02003f8e(s32, s32);
s32 Func_02003efe(s32, s32);
void Func_02003edc(s32);
void Func_02003e86(s32);
void Func_02003f0a(void);
void Func_0200425c(void);
void Func_0200422c(s32, s32);
void Func_02004270(void);

/* Returns a value: the reference sets r1 before r0 at this site. */

void SceneDialogue_ShowLineF13WithWorkA488(void)
{
    Func_02003ed8();

    /* r5 holds &Data_0200a488 across the calls; the word is reloaded before
     * the second test. */
    if (Data_0200a488 != 0) {
        Func_02003f8e(Data_0200a488, 3);
    }

    Func_02003efe((s32) 0xE6, 0);
    Func_02003edc((s32) 0xF13);

    if (Data_0200a488 != 0) {
        Func_02003e86(Data_0200a488);
    }

    Func_02003f0a();
}

void SceneDialogue_RunLine17e2(void)
{
    Func_0200425c();
    Func_0200422c(0x17E2, 1);
    Func_02004270();
}
