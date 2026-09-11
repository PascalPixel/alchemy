#include "types.h"
#include "scene.h"
#include "abi/battle/motion/apply_variant_motion.h"
#include "motion_object.h"

struct BattleObjectSlot *GetBattleObjectSlot(s32 arg0);
u8 *Runtime_GetObject(s32);
void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);
void Object_SetMode(struct MotionObject *, s32);
s32 FixedPoint_Ratio(s32, s32);

extern s32 gRom[];
extern s32 gRom2[];
extern s32 gRom3[];
extern s32 gRom4[];

void BattleMotion_ApplyVariantMotion(s32 id, s32 variant)
{
    struct BattleObjectSlot *slot;
    struct MotionObject *object;
    s32 index;
    s32 x;
    s32 *table;
    s32 scale;

    slot = GetBattleObjectSlot(id);
    object = slot->object;
    if (Runtime_GetObject(id)[0x128] != 0x94) {
        table = gRom;
        index = variant * 4;
        object->acceleration = *(s32 *)((u8 *)table + index);
        table = gRom2;
        object->speed_limit = *(s32 *)((u8 *)table + index);
        if (object->y == 0 || variant > 4) {
            table = gRom3;
            object->velocity_y = *(s32 *)((u8 *)table + index);
        }
        object->vertical_motion_strength = 0x9999;
        object->vertical_motion_phase = 0;
        object->auto_face_motion = 0;
        Object_ResetMotion(object);
        scale = slot->anchor_x;
        table = gRom4;
        x = FixedPoint_Ratio(scale * *(s32 *)((u8 *)table + index), 100);
        Object_SetPosition(object, x, 0, slot->anchor_z);
    }
    Object_SetMode(object, 5);
}
