#include "types.h"
#include "motion_object.h"

typedef s32 (*BattleFixedMultiply)(s32, s32);

struct BattleMotionRecord {
    u8 unknown_00[0x18];
    s32 scale_18;
};

void Func_080b7ed8(void);
s32 Func_08005268(const s32 *, s32 *);
u32 Func_080b8530(s32);

#define BattleMotion_ProjectScaledPosition Func_080b845c

s32 BattleMotion_ProjectScaledPosition(s32 id, s32 *projected)
{
    BattleFixedMultiply multiply;
    struct MotionObject *object = GetBattleObjectSlot(id)->object;
    struct BattleMotionRecord *record = GetMotionRecord(object, 0);
    /* GCC 2.96 retains this sibling-style position frame even when unused. */
    s32 position[3];
    s32 scaled;

    Func_080b7ed8();
    scaled = Func_08005268(&object->x, projected);
    multiply = (BattleFixedMultiply)0x03000118;
    scaled = multiply(scaled, record->scale_18);
    scaled = multiply(scaled, (s32)Func_080b8530(id) >> 16);
    projected[1] -= scaled;
    return 0;
}
