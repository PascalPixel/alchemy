/* Near miss, differ 2 of 104: the listing loads the party state address two instructions
 * earlier, before it builds 0x204. Barriers on that address change the allocation. */
#include "../../../../games/tla/src/field/overlay_67f/ov67f.h"

s32 Func_020012dc(void)
{
    struct Ov67fEventWork *work = Ram_HeapSlots->event_work;
    struct ObjectRuntime *object;
    s32 height;

    work->field_1ac = 0x204;
    Object_GetById(gPartyState.current_owner)->priority_flags |= 0x20;
    Func_020014e0(0);
    if (gPartyState.scene == (s32)&SceneId_Ov67FRoom1) {
        Func_02001510(Data_02001654);
        Func_0200100c((u16 *)Data_02001674);
        Func_020014f8(Data_02001696);
    } else {
        Object_GetById(10)->priority_flags |= 0x20;
        Object_GetById(11)->priority_flags |= 0x20;
        Func_02001510(Data_02001658);
        Func_0200100c((u16 *)Data_02001688);
        if (GameFlag_Test(0x941)) {
            Call6((void (*)())Func_02001460, 7, 2, 8, 7, 7, 12);
            Call6((void (*)())Func_02001450, 7, 66, 7, 76, 8, 9);
            Func_02001448();
            WaitFrames(1);
            if (GameFlag_Test(0x109)) {
                object = Object_GetById(gPartyState.current_owner);
                height = Map_GetTerrainHeight(object->terrain_id, object->x, object->z - object->y);
                object->terrain_height = height;
                object->y = height;
                Object_AttachWorkTargetToObject(gPartyState.current_owner, 0);
            }
        }
    }
    return 0;
}
