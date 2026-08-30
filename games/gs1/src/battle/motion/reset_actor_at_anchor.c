#include "types.h"
#include "fixed_math.h"
#include "motion_object.h"

struct BattleObjectSlot *GetBattleObjectSlot(s32 arg0);
void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);
s32 ArcTan2(s32, s32);

void Actor_ResetMotionAtAnchor(s32 slot_id) {
    s32 adjusted_anchor_z; s32 zero;
    struct BattleObjectSlot *slot;
    struct MotionObject *object;
    slot = GetBattleObjectSlot(slot_id);
    object = slot->object;
    zero = 0;
    object->acceleration = 0x20000;
    object->speed_limit = 0x80000;
    object->vertical_motion_strength = 0xAB85;
    object->velocity_y = zero;
    object->vertical_motion_phase = zero;
    object->auto_face_motion = zero;
    object->snap_to_target = 1;
    Object_ResetMotion(object);
    Object_SetPosition(object, slot->anchor_x, 0, slot->anchor_z);
    adjusted_anchor_z = slot->anchor_z;
    if (adjusted_anchor_z < 0) { adjusted_anchor_z += 7; }
    *(s16 *)((u8 *)object + 6) =
        (s16)(ArcTan2(adjusted_anchor_z >> 3, slot->anchor_x) - 0x8000);
}
