#include "TYPES.H"
#include "B5_CONTEXT.H"

/* Mode entries of the projectile volley effect. */

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Object_SetMode(s32, s32);
s32 Object_SetAction(s32, s32);
s32 Func_080dea70(void *effect, s32 mode);

void BattleFx_RunMode0(void *effect)
{
    Func_080dea70(effect, 0);
}

void BattleFx_RunMode1(void *effect)
{
    Func_080dea70(effect, 1);
}

void BattleFx_RunMode2(void *effect)
{
    Func_080dea70(effect, 2);
}

void BattleFx_RunMode3(void *effect)
{
    Func_080dea70(effect, 3);
}

void BattleFx_RunMode11(void *effect)
{
    Func_080dea70(effect, 0xB);
}

void BattleFx_RunMode4(void *effect)
{
    Func_080dea70(effect, 4);
}

void BattleEffect_RunMode5WithAction(void *effect)
{
    s32 object;

    object =
        (s32)GetBattleObjectSlotFar(FIELD_AT_OFFSET(effect, s32 *, 8))->object;
    Object_SetMode(object, 2);
    Object_SetAction(object, 0x30);
    Func_080dea70(effect, 5);
    Object_SetAction(object, 0x10);
}

void BattleFx_RunMode6WithAction(void *effect)
{
    s32 object;

    object =
        (s32)GetBattleObjectSlotFar(FIELD_AT_OFFSET(effect, s32 *, 8))->object;
    Object_SetMode(object, 2);
    Object_SetAction(object, 0x30);
    Func_080dea70(effect, 6);
    Object_SetAction(object, 0x10);
}

void BattleFx_RunMode7(void *effect)
{
    Func_080dea70(effect, 7);
}

void BattleFx_RunMode10(void *effect)
{
    Func_080dea70(effect, 0xA);
}

void BattleFx_RunMode9WithAction(void *effect)
{
    s32 object;

    object =
        (s32)GetBattleObjectSlotFar(FIELD_AT_OFFSET(effect, s32 *, 8))->object;
    Object_SetMode(object, 2);
    Object_SetAction(object, 0x30);
    Func_080dea70(effect, 9);
    Object_SetAction(object, 0x10);
}
