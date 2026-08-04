#include "types.h"

extern s32 Func_080770c0(s32 flag);
extern void Func_080000c0(s32 frames);
extern void Func_080091b8(s32, s32, s32, s32, s32, s32);
extern void Func_080091c0(s32, s32, s32, s32, s32, s32);
extern void Func_02000918(void);

/* Optionally run the encounter, then install its story-dependent windows. */
void Func_02000f58(s32 runEncounter)
{
    if (runEncounter != 0 && Func_080770c0(0x109) == 0)
        Func_02000918();

    Func_080000c0(1);
    if (Func_080770c0(0x844) != 0) {
        Func_080091b8(121, 34, 3, 1, 93, 10);
        Func_080091b8(46, 38, 1, 1, 30, 43);
        Func_080091c0(0, 0, 1, 2, 30, 9);
        Func_080091c0(26, 3, 1, 2, 10, 8);
        Func_080091b8(26, 35, 1, 4, 10, 40);
    } else {
        Func_080091c0(11, 8, 1, 2, 10, 8);
    }
}
