/*
 * Sets a flag on actor 8 according to the party leader's depth.
 */
#include "types.h"

extern u8 *Func_02001e6e(s32);
extern u8 *Func_02001e7e(s32);
extern u8 *Func_02001e8e(s32);
void SceneActor_SetActor8FlagByLeaderDepth(void)
{
    u8 *obj = Func_02001e6e(0);
    if (*(s32 *)(obj + 12) >= 0x100000) {
        u8 *p = Func_02001e7e(8);
        /*
         * The flag is a local initialised first and ORed with the loaded
         * byte afterwards; folding it into one expression swaps the two
         * registers.
         */
        u32 flag = 2;
        flag = flag | p[0x23];
        p[0x23] = (u8)flag;
    } else {
        Func_02001e8e(8)[0x23] = 1;
    }
}
