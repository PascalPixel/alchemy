#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "effect_step.h"


void EffectStep_RunAnimationMode6Or7Or8B(struct EffectStep *step)
{
    if (step->variant == 0) {
        BattlePres_RunBurstScene(step, 6);
        return;
    }
    if (step->variant == 1) {
        BattlePres_RunBurstScene(step, 7);
        return;
    }
    BattlePres_RunBurstScene(step, 8);
}
