/* Near miss, differ 2 of 64 (one reordered pair). As a goto loop with the frame counted after the
 * wait, (void)&object (GCSE otherwise hoists the motion flags' address out of the loop) and a
 * one-pass block around the last byte store, all but the first scale update match: sched2 ranks
 * the field load (four dependents) above the pool load of 0xfffffc00 (three), both priority 11,
 * where the native loads the constant first. Tried: the constant first in the sum, a delta local,
 * an asm barrier or a one-pass block after the flag store, a flags pointer, the updates reordered,
 * permuter 3 minutes. */
#include "../../../../games/tla/src/field/overlay_6ad/room.h"

void Func_02000944(void)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    s32 frame;

    (void)&object;
    Func_02005608();
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Object_SetModeById(gPartyState.current_owner, 28);
    Battle_WaitMode0(16);
    frame = 0;
step:
    if (frame == 5)
        Func_020056c0(204);
    object->motion_flags = 0;
    WORD_AT(object, 24) += 0xfffffc00;
    WORD_AT(object, 28) += 0xfffffd00;
    object->y += 0xffff6667;
    WaitFrames(1);
    if (++frame <= 39)
        goto step;
    do {
        BYTE_AT(Object_GetById(gPartyState.current_owner), 84) = 0;
    } while (0);
    SCENE_WORK->unknown_1ac = 521;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020055f0(16);
}
