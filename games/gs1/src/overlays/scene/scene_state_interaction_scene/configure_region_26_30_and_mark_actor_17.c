#include "types.h"

extern u8 *Func_0200168c(s32);
extern void Func_02001642(s32, s32, s32, s32, s32, s32);
extern u8 *Func_020016ac(s32);
extern void Func_02001698(s32);

void SceneState_ConfigureRegion26_30AndMarkActor17(void)
{
    u8 *rec = Func_0200168c(17);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 p5 = 23;
    s32 p6 = 34;

    Func_02001642(26, 30, 1, 1, p5, p6);

    if (rec != 0) {
        /* The record is reloaded with the same selector before this store. */
        Func_020016ac(17)[85] = 0;
        rec[35] = 1;
    }

    Func_02001698(0x201);
}
