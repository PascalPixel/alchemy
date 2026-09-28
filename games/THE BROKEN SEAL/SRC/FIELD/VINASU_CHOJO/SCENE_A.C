/* The first scene step. */
#include "CHOJO.H"

void FieldScene_RunScene3c9_02001280(s32 a0, s32 a1)
{
    u32 i;
    s32 record;

    if (a1 != 0) {
        Actor_SetChildValue(a0, 0);
        record = Actor_Get(a0);
        Actor_SetSpriteFlags(record, 1);
        Actor_SetSpeed(a0, 0xcccc, 0x6666);
    } else {
        Actor_SetChildValue(a0, 15);
        record = Actor_Get(a0);
        Actor_SetSpriteFlags(record, 0);
    }
}
