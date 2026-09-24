#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"

struct BattleMotionRecord {
    u8 unknown_00[0x18];
    s32 scale_18;
};

void Camera_ApplyTransformByFlag(void);
s32 Render_ProjectPoint(const s32 *, s32 *);


s32 BattleMotion_ProjectPosition(s32 id, s32 *projected)
{
    struct MotionObject *object = GetBattleObjectSlot(id)->object;
    struct BattleMotionRecord *record = GetMotionRecord(object, 0);
    s32 position[3];
    s32 scaled;

    Camera_ApplyTransformByFlag();
    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z;
    scaled = Render_ProjectPoint(position, projected);
    (void)Iwram_MulQ16(scaled, record->scale_18);
    return 0;
}
