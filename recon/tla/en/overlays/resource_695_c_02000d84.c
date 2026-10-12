/* Near miss, differ 22 of 50: the native builds the zero for motion_flags
 * inside the loop over the actors (the draft hoists it into r6, which
 * shifts the event work to r7), and it sets the mode argument before it
 * loads the leader id for Object_SetModeById. Whole-byte bit-field stores
 * leave differ 20. */
#include "../../../../games/tla/src/field/overlay_695/local.h"

void Func_02000d84(void)
{
    struct Ov695EventWork *work = Ram_HeapSlots->event_work;
    u32 id;

    Func_02001080();
    Func_020011e0(0);
    for (id = 8; id < 64; id++) {
        struct ObjectRuntime *object = Object_GetById(id);

        if (object)
            object->motion_flags = 0;
    }
    Object_GetById(gPartyState.current_owner)->motion_flags = 0;
    Object_SetModeById(gPartyState.current_owner, 2);
    ObjectMotion_CenterOnColumn(gPartyState.current_owner, 2, -8);
    Battle_WaitMode0(5);
    Func_02001210(123);
    Func_02001198(work->value_154);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02001088();
}
