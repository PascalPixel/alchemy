#include "BATTLE_EFFECT_WORK.H"
#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "FIELD_EVENT.H"

/* Mode entries of the canvas effect that follows them: the count picks
   mode 1 or 2, and mode 3 runs with the object's action, as the projectile
   volley's mode 6 does. */

s32 ObjectDispatch_ApplyValueToChildrenFar(s32, s32);
void Func_0818f620(void *effect, s32 mode);

void BattleFx_RunCanvasModeByCount(s32 *effect)
{
    if (*effect > 199)
        Func_0818f620(effect, 2);
    else
        Func_0818f620(effect, 1);
}

void BattleFx_RunCanvasMode0(struct BattleEffectArgument *effect)
{
    Func_0818f620(effect, 0);
}

void BattleFx_RunCanvasMode3WithAction(struct BattleEffectArgument *effect)
{
    s32 object;

    object = (s32)GetBattleObjectSlotFar(effect->actor)->object;
    Object_SetMode(object, 2);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x1c);
    Func_0818f620(effect, 3);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}
