#include "scene.h"
#include "object_runtime.h"

void Func_02001c5c(void);
void Func_020023cc(void);

// Func_02002318's counterpart for actors 21 and 22: actor 22 becomes the
// followed actor, and until flag 0x109 actor 21 is placed and given the
// update Func_020023cc.
void Func_020026a0(void)
{
    struct ObjectRuntime *leader = (struct ObjectRuntime *)Object_GetById(21);
    struct ObjectRuntime *object;
    struct FieldActor **followed;

    Scene_TaskAdd(Func_02002180, 0x480);
    /* FAKEMATCH: through the pointer, taken here, Data_0200480c's address loads before the call that fetches actor 22, as in the native (as in Func_02002318). */
    followed = &Data_0200480c;
    object = (struct ObjectRuntime *)Object_GetById(22);
    object->priority_flags |= 2;
    *followed = (struct FieldActor *)object;
    object->scale_x = 0x14ccc;
    object->scale_y = 0x14ccc;
    Func_020040f8(22);
    (*followed)->motion_flags = 3;
    ObjectMotion_SetActionVariant(22, 3);
    Scene_SetSpeed(21, 0x19999, 0xcccc);
    Object_SetPartAttribute((struct FieldActor *)leader, 15);
    ObjectMotion_SetActionVariant(21, 3);
    Func_020040f8(21);
    Object_GetById(21)->motion_flags = 3;
    if (!GameFlag_Test(0x109)) {
        Scene_Call4098(21, 232 << 16, 664 << 16, 0);
        *((u8 *)leader + 98) = 0;
        leader->update = Func_020023cc;
    }
    Func_020021d4();
    Scheduler_AddOrUpdateCallback(Func_02001c5c, 0x480);
}
