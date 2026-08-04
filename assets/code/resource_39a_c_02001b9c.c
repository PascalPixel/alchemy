#include "types.h"

extern s32 Data_0200a488;

extern void Func_02003ed8(void);
extern void Func_02003f8e(s32, s32);
/* Returns a value: the reference sets r1 before r0 at this site. */
extern s32 Func_02003efe(s32, s32);
extern void Func_02003edc(s32);
extern void Func_02003e86(s32);
extern void Func_02003f0a(void);

void Func_02001b9c(void)
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
