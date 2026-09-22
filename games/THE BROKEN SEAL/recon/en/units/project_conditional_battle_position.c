#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"

struct BattleMotionRecord {
    u8 unknown_00[0x18];
    s32 scale_18;
};

void Func_080b7ed8(void);
s32 Func_08005268(const s32 *, s32 *);
u8 *Func_08077008(s32);
s32 Func_080c23c0(s32);

#define BattleMotion_ProjectConditionalPosition Func_080b84c0

s32 BattleMotion_ProjectConditionalPosition(s32 id, s32 *projected)
{
    struct MotionObject *object = GetBattleObjectSlot(id)->object;
    struct BattleMotionRecord *record = GetMotionRecord(object, 0);
    s32 scaled;
    s32 factor;

    Func_080b7ed8();
    scaled = Func_08005268(&object->x, projected);
    scaled = Iwram_MulQ16(scaled, record->scale_18);
    if (Func_080c23c0(Func_08077008(id)[0x128]) != 0)
        factor = 24;
    else
        factor = 48;
    projected[1] -= Iwram_MulQ16(scaled, factor);
    return 0;
}
