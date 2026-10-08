//--------------------------------------------------------------------------
// hop.c
// 跳ねて分身を引く演出、ジン加入の演出、シーンのオブジェクトの再配置
//--------------------------------------------------------------------------

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "object_runtime.h"
#include "game_state.h"
#include "global_cells.h"
#include "btlsys.h"

#define Motion_SetTargetPositionFromMagnitudeAngle \
	((void (*)(struct HopActor *, s32, s32))Motion_SetTargetPositionFromMagnitudeAngle)

#define Object_ReplaceResourceEntry \
	((void *(*)(struct HopSprite *, void *))Object_ReplaceResourceEntry)

void ObjectMotion_MoveHalfwayTowardTarget(void);

struct HopSprite
{
	u8	 unknown_00[8];
	u16	 tile : 10;	   // OAM 属性2
	u16	 priority : 2; // 表示優先度 (0 が最前面)
	u16	 palette : 4;
	u8	 unknown_0a[30];
	s16 *resource; // 先頭がオブジェクトの種類
};

struct HopActor
{
	u8				  unknown_00[6];
	u16				  facing; // 向き (0x10000 で一周)
	s32				  x;	  // 座標 (16.16 固定小数)
	s32				  y;
	s32				  z;
	u8				  unknown_14[4];
	s32				  scale_x; // 拡大率 (0x10000 で等倍)
	s32				  scale_y;
	u8				  unknown_20[3];
	u8				  priority_flags;
	s32				  velocity_x;
	s32				  velocity_y;
	s32				  velocity_z;
	u8				  unknown_30[8];
	s32				  target_x; // 0x80000000 で目標なし
	s32				  target_y;
	s32				  target_z;
	u8				  unknown_44[12];
	struct HopSprite *sprite;
	u8				  unknown_54;
	u8				  mode;
	u8				  unknown_56[4];
	u8				  collision_flags;
	u8				  unknown_5b[13];
	struct HopActor	 *linked;	// 追いかける相手
	void			 *callback; // 毎フレームの更新処理
};

struct HopActor *Object_GetById(s32 id);
void			 Battle_Reset(void);
void			 WaitFrames(s32 frames);
void			 AudioCommand_PlayFar(s32 cue);
void			 ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void			 ObjectMotion_Launch(s32 id, s32 height, s32 frames);
void			 Script_WaitForEventTimeoutFar(struct HopActor *actor);
struct HopActor *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
void			 Animation_ApplyChildValuesFar(struct HopActor *actor, s32 value);
void			 ObjectDispatch_ApplyArgumentToChildrenFar(struct HopActor *actor, s32 mode);
void			 ObjectDispatch_SetSingleChildFlagsFar(struct HopActor *actor, s32 value);
void			 BattleFx_FinishAction(void);

extern struct MenuState *gEventWork;

struct MenuState
{
	u8	padding_000[20];
	s32 slots[4];
	u8	padding_024[414 - 36];
	s16 mode;
};

void Battle_SetObjectFlag5bWhenMode3(void);
void Battle_ClearObjectFlag5bWhenMode3(void);

struct ActorDescriptor
{
	s16 action;
	s16 scene; // 配置先のシーン番号 - 252
	u8	pad4[4];
	s32 x; // 配置座標 (16.16 固定小数)
	s32 y;
	s32 z;
};

#define BUNSHIN_MAX 8 // 分身の数

//--------------------------------------------------------------------------
// 後ろへ 3 回跳ね、分身を引いて前へ跳ぶ
// id : 跳ねるオブジェクト
//--------------------------------------------------------------------------
void EventActor_RunHopAndLeapSequence(s32 id)
{
	struct HopActor *actor;
	struct HopActor *copy;
	struct HopActor *prev;
	void			*res;
	s32				 dir;
	s32				 back;
	s32				 dist;
	s32				 kind;
	s32				 value;
	s32				 i;
	struct HopActor *copies[BUNSHIN_MAX];

	// 選択中のオブジェクトの向きを 90 度単位に丸める
	actor = Object_GetById(id);
	dir = (Object_GetById(gGameState.selected_actor)->facing + 0x2000) & 0xc000;
	Battle_Reset();
	WaitFrames(10);
	AudioCommand_PlayFar(173);
	Motion_SetVarCbAndRefresh(id, 1);
	AudioCommand_PlayFar(175);
	Motion_SetVarCbAndRefresh(id, 1);
	back = dir + 0x8000;
	WaitFrames(20);
	ObjectMotion_ArmCallback(id, back, 0);
	WaitFrames(10);
	actor->sprite->priority = 0;
	actor->facing = back;
	Object_GetById(id)->collision_flags &= ~1;
	actor->mode = 2;

	// 向いた方の逆へ 16 ドットずつ 3 回跳ねる
	dist = 16 << 16;
	Motion_SetTargetPositionFromMagnitudeAngle(actor, dist, dir);
	AudioCommand_PlayFar(152);
	ObjectMotion_Launch(id, 4, 0);
	Script_WaitForEventTimeoutFar(actor);
	Motion_SetTargetPositionFromMagnitudeAngle(actor, dist, dir);
	AudioCommand_PlayFar(152);
	ObjectMotion_Launch(id, 4, 0);
	Script_WaitForEventTimeoutFar(actor);
	Motion_SetTargetPositionFromMagnitudeAngle(actor, dist, dir);
	AudioCommand_PlayFar(152);
	ObjectMotion_Launch(id, 4, 0);
	Script_WaitForEventTimeoutFar(actor);
	WaitFrames(20);

	// 同じスプライトの分身を作り、ひとつ前を追いかけるようにつなぐ
	kind = *actor->sprite->resource;
	value = 9;
	if (kind == 90)
		value = 2;
	if (kind == 92)
		value = 10;
	if (kind == 91)
		value = 9;
	res = NULL;
	prev = actor;
	for (i = 0; i < BUNSHIN_MAX; i++)
	{
		copy = Object_CreateFar(kind, actor->x, actor->y, actor->z);
		copies[i] = copy;
		if (copy != NULL)
		{
			copy->scale_y = 0xf000;
			copy->scale_x = 0xf000;
			copy->mode = 0;
			copy->priority_flags = 2;
			copy->collision_flags |= 1;
			copy->callback = (void *)ObjectMotion_MoveHalfwayTowardTarget;
			copy->facing = actor->facing;
			copy->sprite->priority = 0;
			Animation_ApplyChildValuesFar(copy, value);
			ObjectDispatch_ApplyArgumentToChildrenFar(copy, 0);
			ObjectDispatch_SetSingleChildFlagsFar(copy, 0);
			res = Object_ReplaceResourceEntry(copy->sprite, res);
			copy->linked = prev;
			prev = copy;
		}
	}

	// 向いた方へ 64 ドット跳び、動きを止める
	Motion_SetTargetPositionFromMagnitudeAngle(actor, 64 << 16, dir + 0x8000);
	AudioCommand_PlayFar(136);
	ObjectMotion_Launch(id, 12, 0);
	WaitFrames(24);
	actor->mode = 0;
	actor->velocity_x = 0;
	actor->velocity_z = 0;
	actor->velocity_y = 0;
	actor->target_x = 0x80000000;
	actor->target_z = 0x80000000;
	actor->target_y = 0x80000000;
	ObjectDispatch_ApplyArgumentToChildrenFar(actor, 0);
	actor->sprite->priority = 2;
	BattleFx_FinishAction();
}

//--------------------------------------------------------------------------
// ジンを仲間に加え、属性ごとの演出のあとに加入メッセージを出す
// slot  : 演出の対象 (-1 で演出なし)
// elm   : ジンの属性
// djinn : 属性内のジン番号
//--------------------------------------------------------------------------
void BattleFx_RunPageEffectForSlot(s32 slot, s32 elm, void *djinn)
{
	struct MenuState *work;
	s32				  owner;

	// 持ち主を決める (フラグ 366 のときは 0 番にし、入手フラグだけ立てる)
	work = gEventWork;
	if (GameFlag_TestFar(366) != 0)
	{
		owner = 0;
		GameFlag_SetBitFar((u8 *)djinn + elm * 20 + 48); // ジン入手フラグ
	}
	else
	{
		owner = Djinn_AddToLeastLoadedOwnerFar(elm, djinn);
	}

	if (owner >= 0)
	{
		Battle_Reset();
		Battle_SetObjectFlag5bWhenMode3();
		if (slot != -1)
		{
			if (work->mode == 3)
				BattleEffect_ClearOutOfBoundsObjects();

			if (elm == 0) // ヴィーナス
				BattleFx_RunVenusDjinnCapture(slot);
			else if (elm == 1) // マーキュリー
				BattleFx_RunMercuryDjinnCapture(slot);
			else if (elm == 2)
				BattleEffect_RunPhasedRadialParticleSequence(slot);
			else if (elm == 3)
				BattleFx_FinishHeavyImpact(slot);

			work->slots[slot] = 0;
			if (work->mode == 3)
				BattleEffect_ClearAllObjects();
		}

		Djinn_ShowJoinedMessageFar(owner, elm, djinn);
		Battle_ClearObjectFlag5bWhenMode3();
		BattleFx_FinishAction();
	}
}

//--------------------------------------------------------------------------
// 保留中のジンイベントのシーン (300〜380) に属するオブジェクトを置きなおす
// 0x800 が立っていれば、選択中のオブジェクトから z を 32 ドット引いた位置へ
//--------------------------------------------------------------------------
void FieldObject_PlaceSceneActors(void)
{
	s32 flags = gGameState.pending_djinn_event & 0xf000;
	u32 scene = gGameState.pending_djinn_event & 0xfff;
	s32 id;

	if (GameFlag_TestFar(0x109) != 0 && flags == 0)
	{
		flags = scene & 0x800;
		scene &= 0x7ff;
		if (scene - 300 <= 80 && gGameState.scene_change_reason > 0)
		{
			for (id = 8; id <= 65; id++)
			{
				struct ActorDescriptor *desc = BattleAction_FindDescriptor(id);
				struct ObjectRuntime   *obj;

				if (desc == NULL || desc->scene - 48 != scene - 300)
					continue;
				obj = ObjectTable_Get(id);
				if (obj == NULL)
					continue;
				if (flags == 0)
				{
					obj->terrain_height = flags;
					obj->flags = 3;
					Object_SetPositionAndResetMotionFar(obj, desc->x, desc->y, desc->z);
				}
				else
				{
					struct ObjectRuntime *sel = ObjectTable_Get(gGameState.selected_actor);

					Object_SetPositionAndResetMotionFar(obj, sel->x, sel->y, sel->z - (32 << 16));
				}
				ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
			}
		}
	}
}
