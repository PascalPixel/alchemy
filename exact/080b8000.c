#include "types.h"
#include "motion_object.h"

struct BattleObjectSlot *Func_080b7dd0(s32 arg0);
void Func_08009140(struct MotionObject *);
void Func_08009150(struct MotionObject *, s32, s32, s32);
s32 Func_080044d0(s32, s32);

void Func_080b8000(s32 arg0) {
    s32 var_r0; s32 zero;
    struct BattleObjectSlot *slot;
    struct MotionObject *object;
    slot = Func_080b7dd0(arg0);
    object = slot->object;
    zero = 0;
    object->acceleration = 0x20000;
    object->speed_limit = 0x80000;
    object->vertical_motion_strength = 0xAB85;
    object->velocity_y = zero;
    object->vertical_motion_phase = zero;
    object->auto_face_motion = zero;
    object->snap_to_target = 1;
    Func_08009140(object);
    Func_08009150(object, slot->anchor_x, 0, slot->anchor_z);
    var_r0 = slot->anchor_z;
    if (var_r0 < 0) { var_r0 += 7; }
    *(s16 *)((u8 *)object + 6) =
        (s16)(Func_080044d0(var_r0 >> 3, slot->anchor_x) - 0x8000);
}
