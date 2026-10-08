#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"


void BattleFx_StepVector2(s32 *vector, s32 damping, s32 gravity)
{
    BattleFx_IntegrateVector2(vector, damping, gravity);
}

void BattleFx_StepVector3(s32 *vector, s32 damping, s32 gravity)
{
    BattleFx_IntegrateVector3(vector, damping, gravity);
}
