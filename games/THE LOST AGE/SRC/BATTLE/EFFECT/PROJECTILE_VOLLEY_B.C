#include "TYPES.H"
#include "MOTION_OBJECT.H"

/* Mode entries of ⚓️'s second projectile volley, a copy of the first with
   its own runner and mode numbering. */

s32 Object_SetMode(s32, s32);
s32 ObjectDispatch_ApplyValueToChildrenFar(s32, s32);
s32 BattleFx_RunProjectileVolleyB(void *effect, s32 mode);

void BattleFx_RunVolleyBMode0(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 0);
}

void BattleFx_RunVolleyBMode12(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 12);
}

void BattleFx_RunVolleyBMode2(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 2);
}

void BattleFx_RunVolleyBMode6(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 6);
}

void BattleFx_RunVolleyBMode3(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 3);
}

void BattleFx_RunVolleyBMode5(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 5);
}

void BattleFx_RunVolleyBMode23(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 23);
}

void BattleFx_RunVolleyBMode7(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 7);
}

void BattleFx_RunVolleyBMode21(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 21);
}

void BattleFx_RunVolleyBMode4(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 4);
}

void BattleFx_RunVolleyBMode8(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 8);
}

void BattleFx_RunVolleyBMode24(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 24);
}

void BattleFx_RunVolleyBMode22(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 22);
}

void BattleFx_RunVolleyBMode9(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 9);
}

void BattleFx_RunVolleyBMode13(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 13);
}

void BattleFx_RunVolleyBMode14(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 14);
}

void BattleFx_RunVolleyBMode15(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 15);
}

void BattleFx_RunVolleyBMode16(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 16);
}

void BattleFx_RunVolleyBMode17(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 17);
}

void BattleFx_RunVolleyBMode18(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 18);
}

void BattleFx_RunVolleyBMode19(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 19);
}

void BattleFx_RunVolleyBMode20(void *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 20);
}

void BattleFx_RunVolleyBMode10WithAction(void *effect)
{
    s32 object;

    object = (s32)GetBattleObjectSlotFar(*(s32 *)((u8 *)effect + 8))->object;
    Object_SetMode(object, 2);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x30);
    BattleFx_RunProjectileVolleyB(effect, 10);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}

void BattleFx_RunVolleyBMode11WithAction(void *effect)
{
    s32 object;

    object = (s32)GetBattleObjectSlotFar(*(s32 *)((u8 *)effect + 8))->object;
    Object_SetMode(object, 2);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x30);
    BattleFx_RunProjectileVolleyB(effect, 11);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}
