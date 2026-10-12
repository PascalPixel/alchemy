/* Near miss: Func_02000944 differs 30 as a goto loop and 15 as a for loop. The listing addresses the motion
 * flags inside the loop and keeps the zero in r7 for the byte after it; this draft hoists the address. */
#include "../../../../games/tla/src/field/overlay_6ad/room.h"

void Func_0200055c(s32 id);

void Func_02000944(void)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    s32 frame;

    Func_02005608();
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Object_SetModeById(gPartyState.current_owner, 28);
    Battle_WaitMode0(16);
    frame = 0;
    /* FAKEMATCH: a goto loop, which GCC does not treat as a loop: ⚓️ addresses the motion flags inside it */
step:
    if (frame == 5)
        Func_020056c0(204);
        object->motion_flags = 0;
        WORD_AT(object, 24) += 0xfffffc00;
        WORD_AT(object, 28) += 0xfffffd00;
        object->y += 0xffff6667;
    frame++;
    WaitFrames(1);
    if (frame <= 39)
        goto step;
    BYTE_AT(Object_GetById(gPartyState.current_owner), 84) = 0;
    SCENE_WORK->unknown_1ac = 521;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020055f0(16);
}

