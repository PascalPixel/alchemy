#include "motobj.h"
#include "b5_context.h"
#include "types.h"
#include "object_runtime.h"

void BattleFx_InitializeRisingObject(s32 slot)
{
    struct BtlObj *context = GetBattleObjectSlotFar(slot);
    struct ObjectRuntime *object = context->object;

    object->acceleration = 0x20000;
    object->speed_limit = 0x80000;
    object->velocity_y = 0x40000;
    object->gravity = 0xAB85;
    object->bounce = 0;
    object->action_flags = 0;
    object->snap_to_target = 1;
    Object_ResetMotion(object);
    Object_SetPosition(object, context->anchor_x, 0, context->anchor_z);
    Object_SetMode(object, 1);
}
