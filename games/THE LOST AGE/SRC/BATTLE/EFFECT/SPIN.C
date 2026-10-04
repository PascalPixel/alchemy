#include "TYPES.H"
#include "MOTION_OBJECT.H"

void Func_080e3074(void);

/* A sixteenth of a turn a frame: BattleFx_AdvanceSpinAngle again, for
   this effect's own tables. */
void BattleFx_SpinObject(struct MotionObject *object)
{
    object->angle += 0x1000;
}

void Func_080e306c(void)
{
    Func_080e3074();
}
