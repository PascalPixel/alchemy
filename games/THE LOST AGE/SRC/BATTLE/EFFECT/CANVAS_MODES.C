#include "TYPES.H"
#include "MOTION_OBJECT.H"

/* Mode entries of the canvas effect that follows them: the count picks
   mode 1 or 2, and mode 3 runs with the object's action, as the projectile
   volley's mode 6 does. */

s32 Object_SetMode(s32, s32);
s32 ObjectDispatch_ApplyValueToChildrenFar(s32, s32);
void Func_0818f620(void *effect, s32 mode);

void BattleFx_RunCanvasModeByCount(s32 *effect)
{
    if (*effect > 199)
        Func_0818f620(effect, 2);
    else
        Func_0818f620(effect, 1);
}

void BattleFx_RunCanvasMode0(void *effect)
{
    Func_0818f620(effect, 0);
}

void BattleFx_RunCanvasMode3WithAction(void *effect)
{
    s32 object;

    object = (s32)GetBattleObjectSlotFar(*(s32 *)((u8 *)effect + 8))->object;
    Object_SetMode(object, 2);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x1c);
    Func_0818f620(effect, 3);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}
