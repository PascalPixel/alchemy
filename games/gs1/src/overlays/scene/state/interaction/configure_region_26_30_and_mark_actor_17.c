#include "types.h"
#include "scene.h"

extern u8 *State_Run(s32);

extern u8 *State_unk2_4(s32);

void State_ConfigureRegion26_30AndMarkActor17(void)
{
    u8 *rec = State_Run(17);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 p5 = 23;
    s32 p6 = 34;

    State_SetRect(26, 30, 1, 1, p5, p6);

    if (rec != 0) {
        /* The record is reloaded with the same selector before this store. */
        State_unk2_4(17)[85] = 0;
        rec[35] = 1;
    }

    State_Do(0x201);
}
