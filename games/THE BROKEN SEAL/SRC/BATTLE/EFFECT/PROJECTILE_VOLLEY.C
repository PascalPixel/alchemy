//--------------------------------------------------------------------------
// PROJECTILE_VOLLEY.C
// バトル効果：BattleFx_RunProjectileVolley の型ごとの入り口
//--------------------------------------------------------------------------

#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "B5_CONTEXT.H"
#include "MOTOBJ.H"
#include "FIELD_EVENT.H"
#include "BATTLE_EFFECT_WORK.H"
void ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *object, s32 animation);

#define ObjectDispatch_ApplyValueToChildrenFar \
	((s32 (*)(s32, s32))ObjectDispatch_ApplyValueToChildrenFar)

#define ARG(e) ((struct BattleEffectArgument *)(e))

#define STEP_HAYAI 0x30 // 絵のアニメの step（ふだんは 0x10）
#define STEP_FUTSU 0x10

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

//--------------------------------------------------------------------------
// 型 5 : 使う者を mode 2 にし、アニメを速めているあいだに放つ
//--------------------------------------------------------------------------
void BattleEffect_RunMode5WithAction(void *effect)
{
	s32 obj;

	obj = (s32)GetBattleObjectSlotFar(ARG(effect)->actor)->object;
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 2);
	ObjectDispatch_ApplyValueToChildrenFar(obj, STEP_HAYAI);
	BattleFx_RunProjectileVolley(effect, 5);
	ObjectDispatch_ApplyValueToChildrenFar(obj, STEP_FUTSU);
}

//--------------------------------------------------------------------------
// 型 6 : 型 5 と同じく、アニメを速めて放つ
//--------------------------------------------------------------------------
void BattleFx_RunMode6WithAction(void *effect)
{
	s32 obj;

	obj = (s32)GetBattleObjectSlotFar(ARG(effect)->actor)->object;
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 2);
	ObjectDispatch_ApplyValueToChildrenFar(obj, STEP_HAYAI);
	BattleFx_RunProjectileVolley(effect, 6);
	ObjectDispatch_ApplyValueToChildrenFar(obj, STEP_FUTSU);
}

void BattleFx_RunMode7(void *effect)
{
	BattleFx_RunProjectileVolley(effect, 7);
}

void BattleFx_RunMode10(void *effect)
{
	BattleFx_RunProjectileVolley(effect, 10);
}

//--------------------------------------------------------------------------
// 型 9 : 型 5 と同じく、アニメを速めて放つ
//--------------------------------------------------------------------------
void BattleFx_RunMode9WithAction(void *effect)
{
	s32 obj;

	obj = (s32)GetBattleObjectSlotFar(ARG(effect)->actor)->object;
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 2);
	ObjectDispatch_ApplyValueToChildrenFar(obj, STEP_HAYAI);
	BattleFx_RunProjectileVolley(effect, 9);
	ObjectDispatch_ApplyValueToChildrenFar(obj, STEP_FUTSU);
}
