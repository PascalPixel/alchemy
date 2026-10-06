#include "MOTION_OBJECT.H"
#include "B5_CONTEXT.H"
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"

void BattleFx_InitializeRisingObject(s32 slot)
{
    struct BattleObjectSlot *context = GetBattleObjectSlotFar(slot);
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
