#include "motobj.h"
#include "b5_context.h"
#include "types.h"
#include "object_runtime.h"
#include "iwram_call.h"

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

// Moves the object rate percent of the way to the target's object in the
// given frames (The Broken Seal's BattleFx_SetApproachMotion, which always
// moves 80 percent).
void Func_08157530(s32 id, s32 tgt_id, s32 frames, s32 rate)
{
    struct BtlObj *slot = GetBattleObjectSlotFar(id);
    struct BtlObj *tgt_slot = GetBattleObjectSlotFar(tgt_id);
    struct ObjectRuntime *obj = (struct ObjectRuntime *)slot->object;
    struct ObjectRuntime *tgt = (struct ObjectRuntime *)tgt_slot->object;
    s32 diff_x = tgt->x - obj->x;
    s32 start_x = obj->x;
    s32 dx = rate * diff_x / 100;
    s32 diff_z = tgt->z - obj->z;
    s32 start_z = obj->z;
    s32 dz = rate * diff_z / 100;
    s32 x = start_x + dx;
    s32 z = start_z + dz;
    s32 sx = dx >> 8; // the distance in 1/256 pixels
    s32 sz = dz >> 8;
    s32 dist;

    dist = Iwram_Sqrt(sx * sx + sz * sz);
    dist = (dist << 8) / frames;
    obj->acceleration = dist;
    obj->speed_limit = dist;
    obj->snap_to_target = 1;
    obj->gravity = 0xab85;
    obj->velocity_y = 0;
    obj->bounce = 0;
    obj->action_flags = 1;
    Object_ResetMotion(obj);
    Object_SetPosition(obj, x, 0, z);
    Object_SetMode(obj, 2);
}
