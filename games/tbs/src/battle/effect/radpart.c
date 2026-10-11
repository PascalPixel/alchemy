#define OWN_Object_CreateFar
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
#include "mem.h"
#include "motobj.h"
#include "fx_scene.h"
#include "dma.h"
#include "callback_scheduler.h"
#include "effect_slot.h"
#include "io_reg.h"
#include "field_event.h"
#include "fixed_math.h"
#include "animspr.h"
#include "objdisp.h"
#include "party_state.h"
#include "resource.h"
#include "palbuf.h"

void BattleFx_ClearActiveSlotsAndScheduleUpdates(void);
void BattleFx_RunPageEffectForSlot(s32 slot, s32 elm, void *djinn);

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
#define Object_GetById ((struct HopActor * (*)(s32 id)) Object_GetById)
void Battle_Reset(void);
void WaitFrames(s32 frames);
void AudioCommand_PlayFar(s32 cue);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void Script_WaitForEventTimeoutFar(struct HopActor *actor);
void ObjectDispatch_ApplyArgumentToChildrenFar(struct HopActor *actor, s32 mode);
void ObjectMotion_Launch(s32 id, s32 height, s32 frames);
void BattleFx_FinishAction(void);
#define gEventWork (*(struct MenuState * (*)) & gEventWork)

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
	void			 Animation_ApplyChildValuesFar(void *actor, s32 value);
	void			 ObjectDispatch_SetSingleChildFlagsFar(void *actor, s32 value);
	void			 ObjectMotion_Launch(s32 id, s32 height, s32 frames);
	void *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
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

#undef Object_GetById
#undef gEventWork

#define Camera_WorldToScreen ((void (*)(struct Vec *))Camera_WorldToScreen)

struct MapActor
{
	s16 id;
	s16 event;
};
#define BattleFx_RunPageEffectForSlot \
	((void (*)(s32 actor, s32 element, s32 index))BattleFx_RunPageEffectForSlot)

extern struct BattleFxScene *gEffectWork;
void						 BattleFx_ClearOwnedSlot(struct EffectSlot *);

struct Vec
{
	s32 x;
	s32 y;
	s32 z;
};

void Vector_AddPolarOffset(s32, s32, struct Vec *);

void Djinn_ResolvePendingEvent(s32 capture)
{
	struct GameState *st = &gGameState;
	s16				 *pending = &st->pending_djinn_event;
	s32				  kind = *pending & 0xf000;
	s32				  flag = *pending & 0xfff;

	if (capture == 0)
	{
		if (kind == 0)
		{
			flag &= 0x7ff;
			if (flag < 300 || flag > 380)
				return;
			if (st->scene_change_reason > 0 && st->scene_change_reason != 999)
				return;
			GameFlag_SetBitFar(flag - 172);
			*pending = kind;
		}
		else if (kind == 0x1000)
		{
			if (st->scene_change_reason == 1)
				GameFlag_SetBitFar(flag);
			*pending = capture;
		}
		return;
	}
	if (kind == 0)
	{
		flag &= 0x7ff;
		if (flag >= 300 && flag <= 380)
		{
			flag &= 0x7ff;
			if (st->scene_change_reason > 0)
			{
				s32 djinn = flag - 300;
				s32 element = djinn / 20;
				s32 index = djinn % 20;
				s32 actor;

				for (actor = 8; actor <= 65; actor++)
				{
					struct MapActor *found = BattleAction_FindDescriptor(actor);
					if (found != 0 && found->event - 48 == flag - 300)
					{
						WaitFrames(40);
						BattleFx_RunPageEffectForSlot(actor, element, index);
						break;
					}
				}
			}
		}
	}
	gGameState.pending_djinn_event = 0;
}

void BattleFx_UpdateAllSlots(void)
{
	struct EffectSlot *slot;
	s32				   remaining_slots;

	slot = gEffectWork->slots;
	remaining_slots = 0x17;
	do
	{
		remaining_slots -= 1;
		EffectSlot_Update(slot);
		slot++;
	} while (remaining_slots >= 0);
}

void BattleFx_InitializeSlots(void)
{
	void		*work;
	volatile u32 zero;
	work = Runtime_AllocateHeapBlock(56, sizeof(struct BattleFxScene));
	zero = 0;
	Dma_Set(&zero, work, 0x85000000 | (sizeof(struct BattleFxScene) / 4), REG_DMA3);
	Scheduler_AddOrUpdateCallback(BattleFx_UpdateAllSlots, 3200);
}

void BattleFx_ClearActiveSlotsAndScheduleUpdates(void)
{
	struct BattleFxScene *scene = gEffectWork;
	struct EffectSlot	 *slot;
	s32					  i;

	Scheduler_RemoveCallback(BattleFx_UpdateAllSlots);
	for (i = 0; i < 24; i++)
	{
		slot = &scene->slots[i];
		if (slot->active != 0)
			BattleFx_ClearOwnedSlot(slot);
	}
	Runtime_ReleaseHeapBlock(56);
	WaitFrames(1);
}

void BattleFx_AdvanceSpinAngle(struct MotionObject *object)
{
	object->angle += 0x2000;
}

void BattleFx_RunAngledApproachPhases(struct EffectSlot *actor)
{
	struct MotionObject *target;
	struct Vec			 pos;

	target = Object_GetById(gGameState.selected_actor);
	if (actor->state == 0)
	{
		actor->orbit_radius += 1;
		actor->orbit_angle += 1;
		if ((s16)actor->age == 60)
		{
			actor->age = 0;
			actor->state += 1;
		}
	}
	else if (actor->state == 1)
	{
		actor->orbit_angle += 1;
		if ((s16)actor->age == 40)
		{
			actor->age = 0;
			actor->state += 1;
		}
	}
	else if (actor->state == 2)
	{
		actor->orbit_angle += 1;
		pos.x = target->x;
		pos.y = target->y + 0x140000;
		pos.z = target->z;
		Camera_WorldToScreen(&pos);
		actor->origin_x += (pos.x - actor->origin_x) / 8;
		actor->origin_z += (pos.z - actor->origin_z) / 8;
		if ((s16)actor->age == 40)
		{
			actor->age = 0;
			actor->state += 1;
		}
	}
	else if (actor->state == 3)
	{
		actor->orbit_radius -= 1;
		actor->orbit_angle += 1;
		if ((s16)actor->age == 60)
		{
			actor->age = 0;
			actor->state += 1;
		}
	}
	else if (actor->state == 4)
	{
		BattleFx_ClearOwnedSlot(actor);
	}
	pos.x = actor->origin_x;
	pos.z = actor->origin_z;
	Vector_AddPolarOffset(actor->orbit_radius << 16, actor->orbit_angle << 11, &pos);
	actor->x = pos.x;
	actor->z = pos.z;
}

#undef BattleFx_RunPageEffectForSlot

//--------------------------------------------------------------------------

// heavy_impact_finish.c

// フィールド効果：ジンが仲間になる演出 (属性3)

//--------------------------------------------------------------------------

#undef Camera_WorldToScreen

#define ObjectDispatch_ApplyArgumentToChildrenFar          \
	((void (*)(struct FieldActor * object, s32 animation)) \
			ObjectDispatch_ApplyArgumentToChildrenFar)
#define Object_GetById ((struct MotionObject * (*)(s32 id)) Object_GetById)

//--------------------------------------------------------------------------
// ジンが跳ねて光の粒になり、操作キャラへ集まる
// jin_id : ジンのオブジェクト番号
//--------------------------------------------------------------------------
void BattleFx_FinishHeavyImpact(s32 jin_id)
{
	struct
	{
		s32 x;
		s32 y;
		s32 z;
	} pos;
	struct MotionObject	 *jin;
	struct BattleFxScene *scene;
	struct EffectSlot	 *slot;
	s32					  scale;
	s32					  cnt;

	jin = Object_GetById(jin_id);
	/* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling */
	do
	{
		if (jin == NULL)
			return;
		BattleFx_InitializeSlots();
		scene = gEffectWork;
		Shop_RestoreSceneTilesFar(0x20118c);
		AudioCommand_PlayFar(173);
		Motion_SetVarCbAndRefresh(jin_id, 1);
	} while (0);
	AudioCommand_PlayFar(174);
	Motion_SetVarCbAndRefresh(jin_id, 1);
	/* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling */
	do
	{
		AudioCommand_PlayFar(175);
		Motion_SetVarCbAndRefresh(jin_id, 1);
		WaitFrames(20);

		// 回転してから高く跳ねる
		AudioCommand_PlayFar(140);
		jin->update = BattleFx_AdvanceSpinAngle;
		WaitFrames(40);
		AudioCommand_PlayFar(153);
		ObjectMotion_Launch(jin_id, 12, 22);

		// ジンの画面位置から光の粒を 24 個出す
		pos.x = jin->x;
		pos.y = jin->y;
		pos.z = jin->z;
		Camera_WorldToScreen(&pos);
		ObjectDispatch_ReleaseFar(jin);
		AudioCommand_PlayFar(164);
		slot = scene->slots;
		cnt = 23;
		do
		{
			EffectSlot_Initialize(slot, 284, pos.x, pos.z);
			EffectSlot_SetCallback(
				slot, BattleFx_RunAngledApproachPhases); // 回ってから操作キャラへ
			EffectSlot_SetObjectMode(slot, 7);
			ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, (u32)(Random16() * 7) >> 16);
			scale = (u32)Random16() / 3 + 0x10000; // 1.0〜1.33 倍
			slot->scale_y = scale;
			slot->scale_x = scale;
			cnt--;
			WaitFrames(1);
			slot++;
		} while (cnt >= 0);
	} while (0);

	// 操作キャラを 0x4000 (90°) の向きへ回し、動作 28 にする
	WaitFrames(60);
	ObjectMotion_ArmCallback(gGameState.selected_actor, 0x4000, 0);
	WaitFrames(20);
	ObjectDispatch_ApplyArgumentToChildrenFar(Object_GetById(gGameState.selected_actor), 28);
	WaitFrames(40);
	AudioCommand_PlayFar(164);
	WaitFrames(100);
	Shop_InitEffectFar();
	BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

#undef ObjectDispatch_ApplyArgumentToChildrenFar
#undef Object_GetById

#undef Motion_SetTargetPositionFromMagnitudeAngle

#define Motion_SetTargetPositionFromMagnitudeAngle \
	((void (*)(struct CaptureObject *, s32, s32))Motion_SetTargetPositionFromMagnitudeAngle)

#define ObjectGroup_SetChildValueUnlessFifteenFar \
	((void (*)(s32, s32))ObjectGroup_SetChildValueUnlessFifteenFar)
#define BattleFx_AdvanceSpinAngle ((void (*)(void))BattleFx_AdvanceSpinAngle)
#define EffectSlot_Initialize ((void (*)(void *, s32, s32, s32))EffectSlot_Initialize)

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))
extern u32 gFrameTick;
void	   ObjectDispatch_ReleaseFar();

struct EffectOwner
{
	u8						unknown_00[8];
	struct EffectVector		position;
	u8						unknown_14[0x3c];
	struct AnimationObject *sprite;
};
#define Object_GetById ((struct ObjectRuntime * (*)(u32 actor)) Object_GetById)
void BattleFx_ClearOwnedSlot(struct EffectSlot *effect);
#define Vector_AddPolarOffset \
	((void (*)(s32 magnitude, s32 angle, struct EffectVector * position)) Vector_AddPolarOffset)
void Motion_SetVarCbAndRefresh(s32, s32);

struct PhasedRadialSequenceObject
{
	u8	  unknown_00[8];
	s32	  x;
	s32	  y;
	s32	  z;
	u8	  unknown_14[4];
	s32	  scale_x;
	s32	  scale_y;
	u8	  unknown_20[68];
	u16	  timer;
	u8	  unknown_66[6];
	void *callback;
};

struct OrbitPosition
{
	s32 x;
	s32 y;
	s32 z;
};

struct DescendingOrbitObject
{
	u8	unknown_00[8];
	s32 x;
	s32 y;
	s32 z;
	u8	unknown_14[80];
	u16 timer;
	s16 angle;
};

struct SpreadPosition
{
	s32 x;
	s32 y;
	s32 z;
};

void AudioCommand_PlayFar(s32);

extern const s32 BattleFx_CommonParticleScript[];

struct CaptureObject
{
	u8	  unknown_00[8];
	s32	  x;
	s32	  y;
	s32	  z;
	s32	  base_y;
	s32	  scale_x;
	s32	  scale_y;
	u8	  unknown_20[8];
	s32	  gravity;
	u8	  unknown_2c[4];
	s32	  speed;
	u8	  unknown_34[33];
	u8	  mode;
	u8	  unknown_56[5];
	u8	  visible;
	u8	  unknown_5c[2];
	u16	  lifetime;
	u8	  unknown_60[4];
	u16	  timer;
	u16	  angle;
	struct FieldActor *linked_object;
	void *callback;
};
#define ObjectDispatch_ApplyArgumentToChildrenFar \
	((void (*)(struct ObjectRuntime * object, s32 mode)) ObjectDispatch_ApplyArgumentToChildrenFar)
void				  ObjectMotion_ArmCallback(s32 id, s32 value, s32 flags);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void				  BattleFx_ClearActiveSlotsAndScheduleUpdates(void);

struct LinkedCaptureObject
{
	u8							padding000[8];
	s32							x;
	s32							y;
	s32							z;
	u8							padding014[84];
	struct LinkedCaptureObject *target;
};

void BattleFx_SetObjectAlternatingWords(u8 *object)
{
	u32 *table = BattleFx_PulseScales;
	u32	 index = (gFrameTick >> 2) & 1;
	u32	 value = index[table];
	((struct MotionObject *)object)->scale_x = value;
	((struct MotionObject *)object)->scale_y = value;
}

void BattleFx_AdvanceAngleWithRamp(void *obj)
{
	u32 step;

	step = FIELD_AT_OFFSET(obj, s16 *, 0x64) * 0x50;
	((struct MotionObject *)obj)->angle = ((struct MotionObject *)obj)->angle + step + 0x1000;
	if (step < 0x1000U)
	{
		FIELD_AT_OFFSET(obj, s16 *, 0x64) = FIELD_AT_OFFSET(obj, s16 *, 0x64) + 1;
	}
}

void BattleFx_ShrinkObjectAndDestroySlow(void *obj)
{
	s32 scale;

	scale = ((struct MotionObject *)obj)->scale_x + 0xFFFFFE40;
	((struct MotionObject *)obj)->scale_y = ((struct MotionObject *)obj)->scale_y + 0xFFFFFE40;
	((struct MotionObject *)obj)->angle = ((struct MotionObject *)obj)->angle + 0x2000;
	((struct MotionObject *)obj)->scale_x = scale;
	if (scale < 0x3000)
	{
		ObjectDispatch_ReleaseFar();
	}
}

void BattleEffect_UpdatePhasedRadialParticle(struct EffectSlot *effect)
{
	struct EffectOwner *owner;
	struct EffectVector position;
	s32					state;

	owner = (struct EffectOwner *)Object_GetById(gGameState.selected_actor);
	state = effect->state;

	if (state == 0)
	{
		effect->x = effect->origin_x;
		effect->z = effect->origin_z;
		position.x = effect->x;
		position.z = effect->z;
		Vector_AddPolarOffset(0x780000,
			((Random16() * 3 << 11) >> 16) - ((Random16() * 3 << 11) >> 16) + 0xc000, &position);
		effect->target_x = position.x;
		effect->target_z = position.z;
		effect->acceleration = 0x50000;
		effect->max_speed = 0x50000;
		effect->flag42 = state;
		effect->state++;

		((struct AnimationObject *)effect->object)->part[0].priority =
			owner->sprite->part[0].priority;
		effect->flags = 0;
		effect->age = 0;
		if ((gFrameTick & 1) != 0)
			AudioCommand_PlayFar(134);
	}
	else if (state == 1)
	{
		if ((s16)effect->age == 3)
		{
			((struct AnimationObject *)effect->object)->part[0].priority = 0;
			effect->flags = 4;
		}
		if (BattleFx_HasReachedTarget(effect) == 0)
			effect->state--;
	}
	else if (state == 2)
	{
		if (BattleFx_HasReachedTarget(effect) == 0)
		{
			effect->origin_x = effect->x;
			effect->origin_z = effect->z;
			((struct AnimationObject *)effect->object)->part[0].priority = 0;
			effect->flags = 4;
			effect->render = 0;
			effect->state++;
			effect->callback_delay = 40;
		}
	}
	else if (state == 3)
	{
		effect->render = 1;
		effect->x = effect->origin_x;
		effect->z = effect->origin_z;
		position.x = owner->position.x;
		position.y = owner->position.y + 0x140000;
		position.z = owner->position.z;
		Camera_WorldToScreen(&position);
		Vector_AddPolarOffset(0x40000, Random16(), &position);
		effect->target_x = position.x;
		effect->target_z = position.z;
		effect->state++;
		if ((gFrameTick & 1) != 0)
			AudioCommand_PlayFar(145);
	}
	else if (state == 4)
	{
		if (BattleFx_HasReachedTarget(effect) == 0)
			effect->state--;
	}
	else if (state == 5)
	{
		if (BattleFx_HasReachedTarget(effect) == 0)
			BattleFx_ClearOwnedSlot(effect);
	}
}

void BattleEffect_RunPhasedRadialParticleSequence(s32 arg)
{
	s8 *object_pointer;
	struct
	{
		s32 x;
		s32 y;
		s32 z;
	} position;
	s32								   value;
	s32								   remaining;
	struct PhasedRadialSequenceObject *source_object;
	void							  *effect_slots;
	struct PhasedRadialSequenceObject *target_object;
	void							  *slot;
	s8								  *slot_state;

	source_object = (struct PhasedRadialSequenceObject *)Object_GetById(arg);
	target_object = (struct PhasedRadialSequenceObject *)Object_GetById(gGameState.selected_actor);
	if (source_object == NULL)
		return;

	BattleFx_InitializeSlots();
	effect_slots = gEffectWork;
	Shop_RestoreSceneTilesFar(0x201090);
	WaitFrames(30);
	ObjectMotion_ArmCallback(arg, 0x4000, 0);
	WaitFrames(20);
	AudioCommand_PlayFar(173);
	Motion_SetVarCbAndRefresh(arg, 1);
	AudioCommand_PlayFar(174);
	Motion_SetVarCbAndRefresh(arg, 1);
	AudioCommand_PlayFar(175);
	Motion_SetVarCbAndRefresh(arg, 1);
	WaitFrames(20);
	AudioCommand_PlayFar(140);
	source_object->callback = (void *)BattleFx_AdvanceAngleWithRamp;
	source_object->timer = 0;
	WaitFrames(80);
	source_object->callback = (void *)BattleFx_ShrinkObjectAndDestroySlow;
	ObjectDispatch_ApplyArgumentToChildrenFar((struct ObjectRuntime *)source_object, 3);

	position.x = source_object->x;
	position.y = source_object->y;
	position.z = source_object->z;
	Camera_WorldToScreen(&position);

	slot = (u8 *)effect_slots + 88;
	remaining = 23;
	do
	{
		EffectSlot_Initialize(slot, 284, position.x, position.z);
		EffectSlot_SetCallback(slot, (void *)BattleEffect_UpdatePhasedRadialParticle);
		EffectSlot_SetObjectMode(slot, 7);
		object_pointer = slot;
		value = *(s32 *)object_pointer;
		ObjectGroup_SetChildValueUnlessFifteenFar(value, 10);
		value = Random16() / 3 + 0x10000;
		((struct EffectSlot *)slot)->scale_y = value;
		((struct EffectSlot *)slot)->scale_x = value;
		remaining--;
		WaitFrames(1);
		slot = (u8 *)slot + 72;
	} while (remaining >= 0);

	WaitFrames(60);
	ObjectMotion_ArmCallback(gGameState.selected_actor, 0x4000, 0);
	WaitFrames(20);
	ObjectDispatch_ApplyArgumentToChildrenFar(Object_GetById(gGameState.selected_actor), 28);
	{
		s32 next_state;

		WaitFrames(20);
		next_state = 2;
		slot_state = effect_slots;
		slot_state += 152;
		remaining = 23;
		do
		{
			if (slot_state[5] != 0)
				slot_state[0] = next_state;
			remaining--;
			slot_state += 72;
		} while (remaining >= 0);
	}

	WaitFrames(60);
	target_object->callback = (void *)BattleFx_SetObjectAlternatingWords;
	WaitFrames(100);

	{
		s32 next_state;

		slot_state = effect_slots;
		next_state = 5;
		slot_state += 152;
		remaining = 23;
		do
		{
			if (slot_state[5] != 0)
				slot_state[0] = next_state;
			remaining--;
			slot_state += 72;
		} while (remaining >= 0);
	}

	WaitFrames(10);
	slot = NULL;
	target_object->callback = slot;
	target_object->scale_x = 0x10000;
	target_object->scale_y = 0x10000;
	WaitFrames(30);
	Shop_InitEffectFar();
	BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

void BattleFx_ShrinkObjectAndDestroyFast(void *obj)
{
	s32 scale;

	scale = ((struct MotionObject *)obj)->scale_x + 0xFFFFFC00;
	((struct MotionObject *)obj)->scale_y = ((struct MotionObject *)obj)->scale_y + 0xFFFFFC00;
	((struct MotionObject *)obj)->angle = ((struct MotionObject *)obj)->angle + 0x2000;
	((struct MotionObject *)obj)->scale_x = scale;
	if (scale < 0x3000)
	{
		ObjectDispatch_ReleaseFar();
	}
}

void BattleFx_UpdateDescendingOrbitObject(struct DescendingOrbitObject *arg)
{
	struct OrbitPosition		  local;
	struct DescendingOrbitObject *other;
	s32							  raw;
	s16							  value;
	s32							  y;

	other = (struct DescendingOrbitObject *)Object_GetById(gGameState.selected_actor);
	raw = arg->timer - 1;
	arg->timer = raw;
	value = arg->timer;
	local.x = other->x;
	local.z = other->z;
	Vector_AddPolarOffset(value * 0x6666, (value << 11) + arg->angle, &local);
	arg->x = local.x;
	arg->z = local.z;
	y = arg->y + 0xFFFF0000;
	arg->y = y;
	if (y < other->y + 0x140000)
		ObjectDispatch_ReleaseFar(arg);
}

void BattleFx_UpdateRadialSpread(struct EffectSlot *effect)
{
	struct SpreadPosition position;
	struct ObjectRuntime *source;
	s32					  state;
	u32					  random;

	source = (struct ObjectRuntime *)Object_GetById(gGameState.selected_actor);
	state = effect->state;

	if (state == 0)
	{
		position.x = source->x;
		position.y = source->y;
		position.z = source->z;

		random = Random16() * 10 + 0xa0000;
		Vector_AddPolarOffset(random, Random16(), &position);
		Camera_WorldToScreen(&position);

		effect->origin_x = position.x;
		effect->origin_z = position.z;
		effect->x = position.x;
		effect->z = position.z;
		position.x = effect->x;
		position.z = effect->z;

		Vector_AddPolarOffset(0x780000, 0xc000, &position);
		effect->target_x = position.x;
		effect->target_z = position.z;
		effect->acceleration = 0x10000;
		effect->max_speed = 0x50000;
		effect->flag42 = state;
		effect->state++;

		if ((gFrameTick & 1) != 0)
			AudioCommand_PlayFar(0x90);
	}
	else if (state == 1)
	{
		if (BattleFx_HasReachedTarget(effect) == 0)
			effect->state--;
	}
	else if (state == 2)
	{
		if (BattleFx_HasReachedTarget(effect) == 0)
			BattleFx_ClearOwnedSlot(effect);
	}
}

void BattleFx_RunVenusDjinnCapture(s32 arg)
{
	void				  Animation_ApplyChildValuesFar(void *object, s32 value);
	void				  ObjectMotion_Launch(s32 id, s32 height, s32 frames);
	struct CaptureObject *leader;
	struct CaptureObject *djinni;
	void				 *effect_slots;
	u8					 *slot;
	s8					 *slot_state;
	s32					  remaining;
	s32					  scale;
	s32					  position[3];

	leader = (struct CaptureObject *)Object_GetById(gGameState.selected_actor);
	djinni = (struct CaptureObject *)Object_GetById(arg);
	if (djinni == NULL)
		return;

	BattleFx_InitializeSlots();
	effect_slots = gEffectWork;
	Shop_RestoreSceneTilesFar(0x201204);
	WaitFrames(30);
	djinni->visible = 0;
	AudioCommand_PlayFar(152);
	ObjectMotion_Launch(arg, 4, 15);
	AudioCommand_PlayFar(152);
	ObjectMotion_Launch(arg, 4, 15);
	WaitFrames(30);
	djinni->callback = (void *)BattleFx_AdvanceSpinAngle;
	AudioCommand_PlayFar(153);
	ObjectMotion_Launch(arg, 8, 22);
	AudioCommand_PlayFar(140);
	Runtime_SetWorkTripleIfNonNegativeFar(0x14ccc, 0x14ccc, 0x10000);
	djinni->callback = (void *)BattleFx_ShrinkObjectAndDestroyFast;
	ObjectDispatch_ApplyArgumentToChildrenFar((struct ObjectRuntime *)djinni, 3);
	WaitFrames(90);
	ObjectMotion_ArmCallback(gGameState.selected_actor, 0x4000, 0);
	WaitFrames(20);
	ObjectDispatch_ApplyArgumentToChildrenFar(Object_GetById(gGameState.selected_actor), 28);
	WaitFrames(30);
	Runtime_SetWorkTripleIfNonNegativeFar(0x19999, 0x19999, 0x10000);

	position[0] = djinni->x;
	position[1] = djinni->y;
	position[2] = djinni->z;
	Camera_WorldToScreen(position);

	slot = (u8 *)effect_slots + 88;
	remaining = 23;
	do
	{
		EffectSlot_Initialize(slot, 284, position[0], position[2]);
		EffectSlot_SetCallback(slot, (void *)BattleFx_UpdateRadialSpread);
		EffectSlot_SetObjectMode(slot, 7);
		ObjectGroup_SetChildValueUnlessFifteenFar(*(s32 *)slot, 11);
		((struct EffectSlot *)slot)->scale_x = 0x8000;
		((struct EffectSlot *)slot)->scale_y = Random16() + 0x18000;
		remaining--;
		WaitFrames(1);
		slot += 72;
	} while (remaining >= 0);

	WaitFrames(140);
	{
		s32 next_state;

		next_state = 2;
		slot_state = effect_slots;
		slot_state += 152;
		remaining = 23;
		do
		{
			if (slot_state[5] != 0)
				slot_state[0] = next_state;
			remaining--;
			slot_state += 72;
		} while (remaining >= 0);
	}

	WaitFrames(20);
	Runtime_SetWorkTripleIfNonNegativeFar(1, 1, 1);
	WaitFrames(30);

	for (remaining = 0; remaining <= 23; remaining++)
	{
		position[0] = leader->x;
		position[1] = leader->y + 0x780000;
		position[2] = leader->z;
		djinni = (struct CaptureObject *)Object_Spawn(284, position[0], position[1], position[2]);
		if (djinni != NULL)
		{
			scale = Random16() / 3 + 0x10000;
			djinni->scale_y = scale;
			djinni->scale_x = scale;
			djinni->timer = 100;
			djinni->angle = (remaining << 16) / 24;
			djinni->callback = (void *)BattleFx_UpdateDescendingOrbitObject;
			djinni->mode = 0;
			ObjectDispatch_ApplyArgumentToChildrenFar((struct ObjectRuntime *)djinni, 7);
			Animation_ApplyChildValuesFar(djinni, 11);
		}
	}

	WaitFrames(100);
	AudioCommand_PlayFar(288);
	WaitFrames(1);
	AudioCommand_PlayFar(151);
	position[0] = leader->x;
	position[1] = leader->y + 0x120000;
	position[2] = leader->z;
	for (remaining = 0; remaining <= 7; remaining++)
	{
		djinni = (struct CaptureObject *)Object_Spawn(284, position[0], position[1], position[2]);
		if (djinni == NULL)
			break;
		djinni->scale_y = 0x9999;
		djinni->scale_x = 0x9999;
		djinni->mode = 2;
		djinni->gravity = 0x50000;
		djinni->base_y = djinni->y;
		djinni->speed = Random16() + 0x16666;
		Motion_SetTargetPositionFromMagnitudeAngle(djinni, 0x200000, Random16());
		Animation_ApplyChildValuesFar(djinni, 11);
		djinni->lifetime = 8;
		ObjectDispatch_InitializeFar(
			(struct DispatchObject *)djinni, BattleFx_CommonParticleScript);
	}

	WaitFrames(15);
	Shop_InitEffectFar();
	BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

void BattleFx_UpdateRandomTargetParticle(struct EffectSlot *effect)
{
	struct ObjectRuntime *source;
	struct EffectVector	  position;
	s8					 *state_pointer;
	s32					  state;

	source = Object_GetById(gGameState.selected_actor);
	state_pointer = &effect->state;
	state = *state_pointer;

	if (state == 0)
	{
		position.x = source->x;
		position.y = source->y + Random16() * 5 + 0xf0000;
		position.z = source->z;
		Camera_WorldToScreen(&position);
		Vector_AddPolarOffset(Random16() * 6 + 0x20000, Random16(), &position);

		effect->target_x = position.x;
		effect->target_z = position.z;
		effect->x = position.x;
		effect->z = position.z - 0x640000;
		effect->acceleration = 0x30000;
		effect->max_speed = Random16() * 3 + 0x30000;
		effect->scale_x = 0x10000;
		effect->scale_y = 0x10000;
		effect->flag42 = state;
		effect->stop_at_target = 1;
		(*state_pointer)++;
	}
	else if ((u8)(effect->state - 1) <= 1)
	{
		if (BattleFx_HasReachedTarget(effect) == 0)
		{
			position.x = effect->x;
			position.z = effect->z;
			Vector_AddPolarOffset(0xc0000, Random16(), &position);
			effect->target_x = position.x;
			effect->target_z = position.z;
			effect->stop_at_target = 0;
			effect->speed = 0x10000;
			effect->acceleration = 0;
			effect->max_speed = Random16() + 0x23333;
			effect->scale_x = 0x8000;
			effect->scale_y = 0x8000;
			AudioCommand_PlayFar(143);

			if (*state_pointer == 1)
				(*state_pointer)--;
			else
				(*state_pointer)++;
			effect->callback_delay = 6;
		}
	}
	else if (state == 3)
	{
		BattleFx_ClearOwnedSlot(effect);
	}
}

void BattleFx_HalveDistanceToTarget(struct LinkedCaptureObject *object)
{
	struct LinkedCaptureObject *target = object->target;

	object->x += (target->x - object->x) / 2;
	object->y += (target->y - object->y) / 2;
	object->z += (target->z - object->z) / 2;
}

#undef Object_GetById
#undef Vector_AddPolarOffset
#undef ObjectDispatch_ApplyArgumentToChildrenFar

//--------------------------------------------------------------------------

// run_mercury_djinn_capture.c

// フィールド演出：水のジンが仲間になるとき、跳ねて残像を引いて昇り、粒を降らせる

//--------------------------------------------------------------------------

#undef Object_ReplaceResourceEntry

#define Motion_SetVarCbAndRefresh ((int (*)())Motion_SetVarCbAndRefresh)

#define BattleFx_HalveDistanceToTarget ((void (*)(void))BattleFx_HalveDistanceToTarget)
#define Object_ReplaceResourceEntry                                                   \
	((struct CaptureResource * (*)(struct CaptureSprite *, struct CaptureResource *)) \
			Object_ReplaceResourceEntry)
#define Camera_WorldToScreen ((void (*)(s32 *))Camera_WorldToScreen)

struct CaptureSprite
{
	u8	 unknown_00[40];
	s16 *resource; // 先頭はオブジェクトの種類
};

struct CaptureObject_2
{
	u8						unknown_00[6];
	u16						facing;
	s32						x;
	s32						y;
	s32						z;
	u8						unknown_14[4];
	s32						scale_x;
	s32						scale_y;
	u8						unknown_20[3];
	u8						priority_flags;
	u8						unknown_24[44];
	struct CaptureSprite   *sprite;
	u8						unknown_54;
	u8						mode;
	u8						unknown_56[4];
	u8						collision_flags;
	u8						visible;
	u8						unknown_5c[12];
	struct CaptureObject_2 *linked;	  // 追いかける相手
	void				   *callback; // 毎フレームの処理
};

struct CaptureResource
{
	u8 unknown_00[28];
	u8 id;
};
#define Object_GetById ((struct CaptureObject * (*)(s32 id)) Object_GetById)
#define ObjectDispatch_ApplyArgumentToChildrenFar \
	((void (*)(struct CaptureObject * object, s32 mode)) ObjectDispatch_ApplyArgumentToChildrenFar)
#define ObjectDispatch_ReleaseFar \
	((void (*)(struct CaptureObject * object)) ObjectDispatch_ReleaseFar)

#define ZANZOU_MAX 8 // 残像の数
#define TSUBU_MAX 24 // 降らせる粒の数 (scene->slots)

//--------------------------------------------------------------------------
// 画面を青く染め、ジンを 3 回跳ねさせてから 8 つの残像を引いて 120 ドット
// 昇らせて消す。selected_actor を mode 28 にしたあと、そのまわりに粒を
// 1 フレームに 1 つずつ 24 個降らせ、最後に色を戻す
// id : ジンのオブジェクト番号
//--------------------------------------------------------------------------
void BattleFx_RunMercuryDjinnCapture(s32 id)
{
	void Animation_ApplyChildValuesFar(void *object, s32 value);
	void ObjectDispatch_SetSingleChildFlagsFar(void *object, s32 value);
	void ObjectMotion_Launch(s32 id, s32 height, s32 frames);
	void *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
	struct CaptureObject_2	*jin;
	struct CaptureObject_2	*copy;
	struct CaptureObject_2	*previous;
	struct CaptureResource	*resource;
	void					*scene;
	struct CaptureObject_2 **cursor;
	struct CaptureObject_2 **list;
	u8						*slot;
	s8						*slot_state;
	s32						 kind;
	s32						 remaining;
	s32						 resource_id;
	struct CaptureObject_2	*copies[ZANZOU_MAX];
	struct
	{
		s32 x;
		s32 y;
		s32 z;
	} position;

	jin = Object_GetById(id);
	if (jin == NULL)
		return;

	// 画面の色を青 (R4 G4 B16) で染める
	BattleFx_InitializeSlots();
	scene = gEffectWork;
	Shop_RestoreSceneTilesFar(PALETTE_TRANSFORM_TINT | (16 << 10) | (4 << 5) | 4);
	WaitFrames(30);
	jin->visible = 0;
	AudioCommand_PlayFar(173);
	Motion_SetVarCbAndRefresh(id, 1);
	AudioCommand_PlayFar(175);
	Motion_SetVarCbAndRefresh(id, 1);
	WaitFrames(20);

	// 3・5・7 ドット/フレームで 3 回跳ねる
	AudioCommand_PlayFar(152);
	ObjectMotion_Launch(id, 3, 14);
	AudioCommand_PlayFar(152);
	ObjectMotion_Launch(id, 5, 16);
	AudioCommand_PlayFar(152);
	ObjectMotion_Launch(id, 7, 18);
	WaitFrames(20);

	// 残像：同じ絵を 8 つ作り、それぞれ一つ前を半分ずつ追いかけさせる
	kind = *jin->sprite->resource;
	resource = NULL;
	previous = jin;
	cursor = copies;
	for (remaining = ZANZOU_MAX - 1; remaining >= 0; remaining--)
	{
		copy = Object_CreateFar(kind, jin->x, jin->y, jin->z);
		*cursor++ = copy;
		if (copy != NULL)
		{
			copy->scale_y = 0xf000;
			copy->scale_x = 0xf000;
			copy->mode = 0;
			copy->priority_flags = 2;
			copy->collision_flags |= 1;
			copy->callback = (void *)BattleFx_HalveDistanceToTarget;
			copy->facing = jin->facing;
			Animation_ApplyChildValuesFar(copy, 9);
			ObjectDispatch_ApplyArgumentToChildrenFar(copy, 0);
			ObjectDispatch_SetSingleChildFlagsFar(copy, 0);
			resource = Object_ReplaceResourceEntry(copy->sprite, resource);
			copy->linked = previous;
			previous = copy;
		}
	}

	// 8 ドットずつ 15 フレーム昇らせ、ジンと残像を消す
	resource_id = resource->id;
	AudioCommand_PlayFar(153);
	jin->mode = 0;
	for (remaining = 14; remaining >= 0; remaining--)
	{
		jin->y += 8 << 16;
		WaitFrames(1);
	}
	ObjectDispatch_ReleaseFar(jin);
	list = copies;
	for (remaining = ZANZOU_MAX - 1; remaining >= 0; remaining--)
		ObjectDispatch_ReleaseFar(*list++);
	if (resource_id != 96)
		Resource_ResetEntry(resource_id);
	WaitFrames(10);
	ObjectMotion_ArmCallback(gGameState.selected_actor, 0x4000, 0);
	WaitFrames(20);
	ObjectDispatch_ApplyArgumentToChildrenFar(Object_GetById(gGameState.selected_actor), 28);
	WaitFrames(20);

	// 粒 (種類 240) を 1 フレームに 1 つずつ出す。動きは BattleFx_UpdateRandomTargetParticle
	position.x = jin->x;
	position.y = jin->y;
	position.z = jin->z;
	Camera_WorldToScreen(&position.x);

	slot = (u8 *)scene + 88; // scene->slots[0]
	remaining = TSUBU_MAX - 1;
	do
	{
		EffectSlot_Initialize(slot, 240, position.x, position.z);
		EffectSlot_SetCallback((struct EffectSlot *)slot, BattleFx_UpdateRandomTargetParticle);
		EffectSlot_SetObjectMode((struct EffectSlot *)slot, 7);
		ObjectGroup_SetChildValueUnlessFifteenFar(*(s32 *)slot, 9);
		remaining--;
		WaitFrames(1);
		slot += 72; // sizeof(struct EffectSlot)
	} while (remaining >= 0);

	// 120 フレーム後、動いている粒 (active) の state を 2 にする
	WaitFrames(120);
	{
		s32 next_state;

		next_state = 2;
		slot_state = scene;
		slot_state += 152; // scene->slots[0].state
		remaining = TSUBU_MAX - 1;
		do
		{
			if (slot_state[5] != 0) // active
				slot_state[0] = next_state;
			remaining--;
			slot_state += 72;
		} while (remaining >= 0);
	}

	// 色を戻す
	WaitFrames(50);
	Shop_InitEffectFar();
	BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

#undef Motion_SetVarCbAndRefresh
#undef Object_GetById
#undef ObjectDispatch_ApplyArgumentToChildrenFar
#undef ObjectDispatch_ReleaseFar
