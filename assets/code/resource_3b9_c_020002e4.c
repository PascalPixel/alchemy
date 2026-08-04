#include "types.h"

extern void Func_02002edc(void);
extern s32 Func_02002eaa(s32 flagId);
extern void Func_02002fc2(s32 id, s32 arg1, s32 arg2);
extern void Func_02002f98(s32 dialogueId);
extern void Func_02002fb0(s32 id, s32 arg1);
extern void Func_02002fa8(s32 dialogueId);
extern void Func_02002fc0(s32 id, s32 arg1);
extern void Func_02002f1c(void);

void Func_020002e4(void)
{
    Func_02002edc();
    if (Func_02002eaa(0x962)) {
        Func_02002fc2(13, 258, 40);
        Func_02002f98(0x2254);
        Func_02002fb0(13, 0);
    } else {
        Func_02002fa8(0x205c);
        Func_02002fc0(13, 0);
    }
    Func_02002f1c();
}
