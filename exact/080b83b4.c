#include "types.h"
#include "motion_object.h"

struct BattleObjectSlot *Func_080b7dd0(s32);
void Func_080b83b0(void *, s32);

void Func_080b83b4(s32 arg0, s32 arg1) {
    struct BattleObjectSlot *second_slot;
    struct MotionObject *obj1;
    struct MotionObject *obj2;
    s32 a1;
    s32 a2;
    s32 b1;
    s32 b2;
    struct {
        s32 x;
        s32 y;
        s32 z;
    } pos;

    obj1 = Func_080b7dd0(arg0)->object;
    second_slot = Func_080b7dd0(arg1);
    obj2 = second_slot->object;

    a1 = obj1->target_x;
    if (a1 == (s32)0x80000000) {
        a1 = obj1->x;
    }
    b1 = obj1->target_z;
    if (b1 == (s32)0x80000000) {
        b1 = obj1->z;
    }
    a2 = obj2->target_x;
    if (a2 == (s32)0x80000000) {
        a2 = obj2->x;
    }
    b2 = obj2->target_z;
    if (b2 == (s32)0x80000000) {
        b2 = obj2->z;
    }

    pos.x = (a1 + a2) / 2;
    pos.y = 0;
    pos.z = (b1 + b2) / 2;

    Func_080b83b0(&pos, 0x1000);
}
