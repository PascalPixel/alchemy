#include "TYPES.H"
#include "MOTION_OBJECT.H"

void Func_080e3074(void);

void BattleFx_SpinObject(struct MotionObject *object)
{
    object->angle += 0x1000;
}

void BattleFx_CallSpinEffect(void)
{
    Func_080e3074();
}
