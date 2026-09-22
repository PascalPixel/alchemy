#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"

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
    struct MotionObject *object = GetBattleObjectSlot(id)->object;
    struct BattleMotionRecord *record = GetMotionRecord(object, 0);
    /* GCC 2.96 retains this sibling-style position frame even when unused. */
    s32 position[3];
    s32 scaled;

    Func_080b7ed8();
    scaled = Func_08005268(&object->x, projected);
    scaled = Iwram_MulQ16(scaled, record->scale_18);
    scaled = Iwram_MulQ16(scaled, (s32)Func_080b8530(id) >> 16);
    projected[1] -= scaled;
    return 0;
}
