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
#include "battle_effect_work.h"
#include "motobj.h"
#include "field_event.h"

#define ObjectDispatch_ApplyValueToChildrenFar ((s32 (*)(struct FieldActor*,s32))ObjectDispatch_ApplyValueToChildrenFar)


void BattleFx_RunVolleyBMode0(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 0);
}

void BattleFx_RunVolleyBMode12(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 12);
}

void BattleFx_RunVolleyBMode2(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 2);
}

void BattleFx_RunVolleyBMode6(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 6);
}

void BattleFx_RunVolleyBMode3(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 3);
}

void BattleFx_RunVolleyBMode5(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 5);
}

void BattleFx_RunVolleyBMode23(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 23);
}

void BattleFx_RunVolleyBMode7(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 7);
}

void BattleFx_RunVolleyBMode21(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 21);
}

void BattleFx_RunVolleyBMode4(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 4);
}

void BattleFx_RunVolleyBMode8(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 8);
}

void BattleFx_RunVolleyBMode24(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 24);
}

void BattleFx_RunVolleyBMode22(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 22);
}

void BattleFx_RunVolleyBMode9(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 9);
}

void BattleFx_RunVolleyBMode13(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 13);
}

void BattleFx_RunVolleyBMode14(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 14);
}

void BattleFx_RunVolleyBMode15(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 15);
}

void BattleFx_RunVolleyBMode16(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 16);
}

void BattleFx_RunVolleyBMode17(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 17);
}

void BattleFx_RunVolleyBMode18(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 18);
}

void BattleFx_RunVolleyBMode19(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 19);
}

void BattleFx_RunVolleyBMode20(struct BattleEffectArgument *effect)
{
    BattleFx_RunProjectileVolleyB(effect, 20);
}

void BattleFx_RunVolleyBMode10WithAction(struct BattleEffectArgument *effect)
{
    struct FieldActor *object;

    object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
    Object_SetMode(object, 2);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x30);
    BattleFx_RunProjectileVolleyB(effect, 10);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}

void BattleFx_RunVolleyBMode11WithAction(struct BattleEffectArgument *effect)
{
    struct FieldActor *object;

    object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
    Object_SetMode(object, 2);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x30);
    BattleFx_RunProjectileVolleyB(effect, 11);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}
