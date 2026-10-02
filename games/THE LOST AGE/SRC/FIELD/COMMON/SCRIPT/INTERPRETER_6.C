#include "SCRIPT.H"

void Object_SetMoveTarget(void *, s32, s32, s32);

s32 Script_ApplyLocalOffsetPosition(u8 *arg0)
{
    s32 done;
    s32 cursor;
    s32 offset[3];

    Object_SetMoveTarget(
        arg0,
        *(s32 *)(arg0 + 8) + offset[0],
        *(s32 *)(arg0 + 12) + offset[1],
        *(s32 *)(arg0 + 16) + offset[2]
    );
    cursor = (u16)*(u16 *)(arg0 + 4);
    done = 1;
    asm volatile("" : "+l"(done)); /* FAKEMATCH: ⚓️ sets the result between the cursor load and store */
    *(u16 *)(arg0 + 4) = cursor + 3;
    return done;
}
