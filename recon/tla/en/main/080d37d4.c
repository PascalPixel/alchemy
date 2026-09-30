#include "OBJECT_RUNTIME.H"
#include "FIELD_EVENT.H"

void ObjectDispatch_InitializeFar(struct ObjectRuntime *, const void *);
void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetMode(struct ObjectRuntime *, s32);
void Battle_WaitMode0(s32);
extern const u8 ObjectMotion_StepAngleScript[];

s32 ObjectMotion_StepAngle(struct ObjectRuntime *object)
{
    s32 delta = 0;

    if (object != NULL) {
        s32 target_angle = (u16)object->action;
        s32 current_angle = object->angle;
        delta = (s16)(target_angle - current_angle);
        if (delta != 0) {
            if (delta > 4096)
                delta = 2048;
            if (delta < -4096)
                delta = -2048;
            object->angle = current_angle + delta;
        }
    }
    return delta;
}
