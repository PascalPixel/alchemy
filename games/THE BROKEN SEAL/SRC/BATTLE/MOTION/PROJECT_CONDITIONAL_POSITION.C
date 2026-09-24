#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"

struct BattleMotionRecord {
    u8 unknown_00[0x18];
    s32 scale_18;
};

void Camera_ApplyTransformByFlag(void);
s32 Render_ProjectPoint(const s32 *, s32 *);
u8 *Owner_GetStateFar(s32);
s32 Summon_IsEntryFlagged(s32);


s32 BattleMotion_ProjectConditionalPosition(s32 id, s32 *projected)
{
    struct MotionObject *object = GetBattleObjectSlot(id)->object;
    struct BattleMotionRecord *record = GetMotionRecord(object, 0);
    s32 scaled;
    s32 factor;

    Camera_ApplyTransformByFlag();
    scaled = Render_ProjectPoint(&object->x, projected);
    factor = Iwram_MulQ16(scaled, record->scale_18);
    if (Summon_IsEntryFlagged(Owner_GetStateFar(id)[0x128]) != 0)
        scaled = Iwram_MulQ16(factor, 24);
    else
        scaled = Iwram_MulQ16(factor, 48);
    projected[1] -= scaled;
    return 0;
}
