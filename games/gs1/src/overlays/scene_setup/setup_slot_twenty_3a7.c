#include "types.h"

extern u8 *Func_02001778(s32);
extern void Func_0200172e(s32, s32, s32, s32, s32, s32);
extern void Func_0200174a(u8 *, s32);
extern u8 *Func_020017a0(s32);
extern void Func_0200178e(s32);

void SceneActor_SetupSlotTwenty(void)
{
    u8 *rec = Func_02001778(20);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 26;
    s32 sixth = 34;

    Func_0200172e(26, 30, 1, 1, fifth, sixth);

    if (rec != 0) {
        Func_0200174a(rec, 0);
        /* The rec is reloaded with the same selector before this store. */
        Func_020017a0(20)[85] = 0;
        rec[35] = 1;
    }

    Func_0200178e(0x204);
}
