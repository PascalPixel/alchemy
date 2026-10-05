#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "EFFECT_STEP.H"


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
