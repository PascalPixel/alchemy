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


void BattleFx_StepVector2(s32 *vector, s32 damping, s32 gravity)
{
    BattleFx_IntegrateVector2(vector, damping, gravity);
}

void BattleFx_StepVector3(s32 *vector, s32 damping, s32 gravity)
{
    BattleFx_IntegrateVector3(vector, damping, gravity);
}
