#include "TYPES.H"
#include "SCENE.H"
#include "MOTION_OBJECT.H"

/* Turns the object by a step that grows by 0x50 each frame until it
   reaches 0x1000, on top of a constant 0x1000. */
void BattleFx_AdvanceObjectField6WithRamp(void *obj)
{
    u32 step;

    step = FIELD_AT_OFFSET(obj, s16 *, 0x64) * 0x50;
    ((struct MotionObject *)obj)->angle = (u16)(((struct MotionObject *)obj)->angle + step + 0x1000);
    if (step < 0x1000U) {
        FIELD_AT_OFFSET(obj, s16 *, 0x64) = (s16)((u16)FIELD_AT_OFFSET(obj, s16 *, 0x64) + 1);
    }
}
