//--------------------------------------------------------------------------
// ARCSTATE.C
// エナジー演出 16 (プルーフ)：使う者の動きと、効果が切れたときのメッセージ
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
#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "FIXED_MATH.H"
#include "SCENE_IDS.H"
#include "SCENE.H"
#include "GAME_STATE.H"
#include "FX_SCENE.H"
#include "ANIMSPR.H"
#include "text/MSG_IDS.H"
void UiText_ShowPositionedMessageAndWaitFar(s32 no, s32 flags);

#define BattleFx_SpawnDescendingArcParticles \
	((void (*)(void *))BattleFx_SpawnDescendingArcParticles)
#define UiText_ShowPositionedMessageAndWaitFar ((void (*)(void *, s32))UiText_ShowPositionedMessageAndWaitFar)

#define ENA_PURUFU 150 // プルーフ
#define ITEM_HANE 236  // せいなるはね

#ifndef MODERN // modern GCC keeps the headers' volatile declaration
extern u32 gFrameCount;
#endif

void UiWork_PushValueSlotFar(s32 arg0, s32 arg1);

//--------------------------------------------------------------------------
// 演出の終わり：使う者の表示の flags を 5 フレームの周期で 3 と 1 に切り替える
//--------------------------------------------------------------------------
void BattleFx_UpdateEffect16State(void)
{
	struct ArcEffectObject *obj;
	struct AnimationObject *rec;
	struct AnimationEntry  *entry;
	s32						phase;

	obj = ObjectTable_Get(gGameState.selected_actor);
	rec = obj->records;
	entry = rec->entries[0];
	phase = gFrameCount % 5;
	if (phase == 0)
	{
		rec->dirty = 1;
		rec->flags = 3;
	}
	else if (phase == 2)
	{
		entry->param = 0; // 色を戻す
		rec->dirty = 1;
		rec->flags = 1;
	}
}

//--------------------------------------------------------------------------
// 演出中の使う者：5 フレームごと (ワールドマップでは 7) に降る弧の粒を出し、
// phase 1 では回る
//--------------------------------------------------------------------------
void BattleFx_UpdatePairedArcSpawner(struct ArcEffectObject *obj)
{
	s16 phase;
	s16 cnt;

	phase = obj->phase;
	cnt = obj->step++;

	if (gGameState.map == (s32)&SceneId_Clear)
	{
		if (__modsi3(cnt, 7) == 0)
			BattleFx_SpawnDescendingArcParticles(obj);
	}
	else if (__modsi3(cnt, 5) == 0)
	{
		BattleFx_SpawnDescendingArcParticles(obj);
	}

	if (phase == 1)
		obj->angle += 0xc00;
}

//--------------------------------------------------------------------------
// プルーフ (せいなるはね) の効果が切れた
//--------------------------------------------------------------------------
void FieldEvent_ShowStatusMessage(void)
{
	gGameState.avoid_distance = 0;
	if (gGameState.avoid_by_item == 0)
	{
		UiWork_PushValueSlotFar(ENA_PURUFU, 4);
		UiText_ShowPositionedMessageAndWaitFar(MsgAbilityWoreOff, 1);
		return;
	}
	UiWork_PushValueSlotFar(ITEM_HANE, 2);
	UiText_ShowPositionedMessageAndWaitFar(MsgItemWoreOff, 1);
}
