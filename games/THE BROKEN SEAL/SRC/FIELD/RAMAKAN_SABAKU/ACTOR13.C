#include "RAMAKAN.H"

void FieldScene_ApplyActor13Values3And3(void) { BattleFx_RunPageEffectForSlot(13, 3, 3); }

/*
 * The countdown is tested at the top of the loop and decremented inside the
 * body, after the call.  A post-decrement test would move the subtract ahead
 * of the call.
 */
void OverlayObject_WaitUntilField12BelowLimit(u8 *o, s32 limit)
{
    s32 frames = 60;

    while (frames != 0) {
        Task_Wait(1);
        frames--;
        if (*(s32 *)(o + 12) <= limit) break;
    }
}

