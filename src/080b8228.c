#include "types.h"
#include "motion_object.h"

struct BattleObjectSlot *Func_080b7dd0(s32 arg0);
u8 *Func_08077008(s32);
void Func_08009140(struct MotionObject *);
void Func_08009150(struct MotionObject *, s32, s32, s32);
void Func_08009080(struct MotionObject *, s32);
s32 Func_080022ec(s32, s32);

extern s32 Data_080c59a4[];
extern s32 Data_080c59c4[];
extern s32 Data_080c59e4[];
extern s32 Data_080c5a04[];

void Func_080b8228(s32 id, s32 variant)
{
    struct BattleObjectSlot *slot;
    struct MotionObject *object;
    s32 index;
    s32 x;
    s32 *table;
    s32 scale;

    slot = Func_080b7dd0(id);
    object = slot->object;
    if (Func_08077008(id)[0x128] != 0x94) {
        table = Data_080c59a4;
        index = variant * 4;
        object->acceleration = *(s32 *)((u8 *)table + index);
        table = Data_080c59c4;
        object->speed_limit = *(s32 *)((u8 *)table + index);
        if (object->y == 0 || variant > 4) {
            table = Data_080c59e4;
            object->velocity_y = *(s32 *)((u8 *)table + index);
        }
        object->vertical_motion_strength = 0x9999;
        object->vertical_motion_phase = 0;
        object->auto_face_motion = 0;
        Func_08009140(object);
        scale = slot->anchor_x;
        table = Data_080c5a04;
        x = Func_080022ec(scale * *(s32 *)((u8 *)table + index), 100);
        Func_08009150(object, x, 0, slot->anchor_z);
    }
    Func_08009080(object, 5);
}
