#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "MOTOBJ.H"


void BattleFx_SpinObject(struct MotionObject *object)
{
    object->angle += 0x1000;
}

void BattleFx_CallSpinEffect(void)
{
    Func_080e3074();
}
