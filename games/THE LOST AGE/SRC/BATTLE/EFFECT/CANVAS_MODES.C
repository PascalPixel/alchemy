#include "BATTLE_EFFECT_WORK.H"
#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "FIELD_EVENT.H"

s32 ObjectDispatch_ApplyValueToChildrenFar(struct FieldActor *, s32);
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
    struct FieldActor *object;

    object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
    Object_SetMode(object, 2);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x1c);
    Func_0818f620(effect, 3);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}
