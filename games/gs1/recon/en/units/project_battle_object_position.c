#include "types.h"
#include "motion_object.h"

typedef s32 (*BattleFixedMultiply)(s32, s32);

struct BattleMotionRecord {
    u8 unknown_00[0x18];
    s32 scale_18;
};

void Func_080b7ed8(void);
s32 Func_08005268(const s32 *, s32 *);

#define BattleMotion_ProjectPosition Func_080b7f20

s32 BattleMotion_ProjectPosition(s32 id, s32 *projected)
{
    BattleFixedMultiply multiply;
    struct MotionObject *object = GetBattleObjectSlot(id)->object;
    struct BattleMotionRecord *record = GetMotionRecord(object, 0);
    s32 position[3];
    s32 scaled;

    Func_080b7ed8();
    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z;
    scaled = Func_08005268(position, projected);
    multiply = (BattleFixedMultiply)0x03000118;
    (void)multiply(scaled, record->scale_18);
    return 0;
}
