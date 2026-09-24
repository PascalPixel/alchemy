#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"

struct BattleMotionRecord {
    u8 unknown_00[0x18];
    s32 scale_18;
};

void Camera_ApplyTransformByFlag(void);
s32 Render_ProjectPoint(const s32 *, s32 *);
u32 Battle_GetObjectTableValue(s32);


s32 BattleMotion_ProjectScaledPosition(s32 id, s32 *projected)
{
    struct MotionObject *object = GetBattleObjectSlot(id)->object;
    struct BattleMotionRecord *record = GetMotionRecord(object, 0);
    s32 position[3]; /* FAKEMATCH: unused; it only reserves the 12-byte frame the reference allocates. */
    s32 scaled;
    s32 factor;

    Camera_ApplyTransformByFlag();
    scaled = Render_ProjectPoint(&object->x, projected);
    factor = Iwram_MulQ16(scaled, record->scale_18);
    scaled = Iwram_MulQ16(factor, (s32)Battle_GetObjectTableValue(id) >> 16);
    projected[1] -= scaled;
    return 0;
}
