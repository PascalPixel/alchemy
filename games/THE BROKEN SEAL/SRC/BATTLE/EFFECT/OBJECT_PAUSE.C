//--------------------------------------------------------------------------
// OBJECT_PAUSE.C
// オブジェクトを止めて表の値をでたらめに当て続け、もとの処理に戻す
//--------------------------------------------------------------------------

#define OWN_BattleFx_ResumeObject

#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "GAME_STATE.H"
#include "FIELDOBJ.H"
#include "ANIMSPR.H"

#define ObjectDispatch_ApplyValueToChildrenFar \
	((void (*)(void *, s32))ObjectDispatch_ApplyValueToChildrenFar)

void Animation_ApplyChildValuesFar(void *, s32);

//--------------------------------------------------------------------------
// 止めているあいだの毎フレームの処理 : 表の 8 つからでたらめに選んで当てる
//--------------------------------------------------------------------------
void BattleEffect_SetRandomTableValueOnObject(void *obj)
{
	s8 *table = BattleFx_RandomChildValues;
	s32 r = Random16();
	Animation_ApplyChildValuesFar(obj, table[(u32)(r * 8) >> 16]);
}

//--------------------------------------------------------------------------
// id のオブジェクトを止める
// 毎フレームの処理と、動いていれば最初のアニメーションの param を
// gGameState に覚えておく
//--------------------------------------------------------------------------
void BattleEffect_PauseObject(s32 id)
{
	struct FieldActor	  *obj;
	struct AnimationEntry *entry;

	obj = ObjectTable_Get(id);
	if (obj != NULL)
	{
		gGameState.paused_update = (u32)obj->update;
		gGameState.paused_pose = 0;
		if (obj->active == 1)
		{
			entry = ((struct AnimationObject *)obj->sprite)->entries[0];
			if (entry != NULL)
			{
				gGameState.paused_pose = entry->param;
			}
		}
		obj->update = (void (*)(union FieldObject *))BattleEffect_SetRandomTableValueOnObject;
		obj->paused = 1;
		ObjectDispatch_ApplyValueToChildrenFar(obj, 0);
	}
}

//--------------------------------------------------------------------------
// id のオブジェクトを、覚えておいた毎フレームの処理と param に戻す
//--------------------------------------------------------------------------
void BattleFx_ResumeObject(s32 id)
{
	struct FieldActor *obj = ObjectTable_Get(id);
	if (obj != 0)
	{
		if (obj->update == (void (*)(union FieldObject *))BattleEffect_SetRandomTableValueOnObject)
		{
			struct GameState *state = &gGameState;
			obj->update = (void (*)(union FieldObject *))state->paused_update;
			state->paused_update = 0;
			Animation_ApplyChildValuesFar(obj, state->paused_pose);
		}
		obj->paused = 0;
		ObjectDispatch_ApplyValueToChildrenFar(obj, 16);
	}
}
