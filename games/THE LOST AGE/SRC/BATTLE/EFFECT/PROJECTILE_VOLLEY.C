#include "TYPES.H"
#include "MOTION_OBJECT.H"

/* Mode entries of the projectile volley effect: ☀️'s, with ⚓️'s further
   modes 12 to 18. */

s32 Object_SetMode(s32, s32);
s32 ObjectDispatch_ApplyValueToChildrenFar(s32, s32);
s32 BattleFx_RunProjectileVolley(void *effect, s32 mode);

void BattleFx_RunMode0(void *effect)
{
    BattleFx_RunProjectileVolley(effect, 0);
}

void BattleFx_RunMode1(void *effect)
{
    BattleFx_RunProjectileVolley(effect, 1);
}

void BattleFx_RunMode2(void *effect)
{
    BattleFx_RunProjectileVolley(effect, 2);
}

void BattleFx_RunMode3(void *effect)
{
    BattleFx_RunProjectileVolley(effect, 3);
}

void BattleFx_RunMode11(void *effect)
{
    BattleFx_RunProjectileVolley(effect, 11);
}

void BattleFx_RunMode4(void *effect)
{
    BattleFx_RunProjectileVolley(effect, 4);
}

void BattleEffect_RunMode5WithAction(void *effect)
{
    s32 object;

    object = (s32)GetBattleObjectSlotFar(*(s32 *)((u8 *)effect + 8))->object;
    Object_SetMode(object, 2);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x30);
    BattleFx_RunProjectileVolley(effect, 5);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}

void BattleFx_RunMode6WithAction(void *effect)
{
    s32 object;

    object = (s32)GetBattleObjectSlotFar(*(s32 *)((u8 *)effect + 8))->object;
    Object_SetMode(object, 2);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x30);
    BattleFx_RunProjectileVolley(effect, 6);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}

void BattleFx_RunMode7(void *effect)
{
    BattleFx_RunProjectileVolley(effect, 7);
}

void BattleFx_RunMode9WithAction(void *effect)
{
    s32 object;

    object = (s32)GetBattleObjectSlotFar(*(s32 *)((u8 *)effect + 8))->object;
    Object_SetMode(object, 2);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x30);
    BattleFx_RunProjectileVolley(effect, 9);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}

void BattleFx_RunMode12WithAction(void *effect)
{
    s32 object;

    object = (s32)GetBattleObjectSlotFar(*(s32 *)((u8 *)effect + 8))->object;
    Object_SetMode(object, 2);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x30);
    BattleFx_RunProjectileVolley(effect, 12);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}

void BattleFx_RunMode13WithAction(void *effect)
{
    s32 object;

    object = (s32)GetBattleObjectSlotFar(*(s32 *)((u8 *)effect + 8))->object;
    Object_SetMode(object, 2);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x30);
    BattleFx_RunProjectileVolley(effect, 13);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}

void BattleFx_RunMode14WithAction(void *effect)
{
    s32 object;

    object = (s32)GetBattleObjectSlotFar(*(s32 *)((u8 *)effect + 8))->object;
    Object_SetMode(object, 2);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x30);
    BattleFx_RunProjectileVolley(effect, 14);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}

void BattleFx_RunMode15(void *effect)
{
    BattleFx_RunProjectileVolley(effect, 15);
}

void BattleFx_RunMode16(void *effect)
{
    BattleFx_RunProjectileVolley(effect, 16);
}

void BattleFx_RunMode17(void *effect)
{
    BattleFx_RunProjectileVolley(effect, 17);
}

void BattleFx_RunMode10(void *effect)
{
    BattleFx_RunProjectileVolley(effect, 10);
}

void BattleFx_RunMode18(void *effect)
{
    BattleFx_RunProjectileVolley(effect, 18);
}
