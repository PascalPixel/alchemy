#include "OBJECT_RUNTIME.H"

/* Turns an object toward its action angle, cutting a turn wider than 4096
   to 2048, and returns the turn. Both the main image and overlay 6A5 carry
   it. */
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
