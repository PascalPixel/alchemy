//--------------------------------------------------------------------------
// burstobj.c
// エナジー演出：イマジンの打ち切り、演出 12 の粒、演出 7 で対象を持ち上げる
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
#include "fieldobj.h"
#include "types.h"
#include "game_state.h"
#include "fx_scene.h"
#include "objdisp.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "sound_ids.h"
#include "global_cells.h"
#include "fixed_math.h"
#include "object_efx.h"
#include "motobj.h"
#include "object_dispatch.h"
#include "field_sprite.h"
#include "object_runtime.h"
#include "resource.h"
#include "animspr.h"
#include "script_motion.h"
#include "frame.h"
#include "effect_slot.h"
#include "battle_effect_runtime.h"
#include "iwram_call.h"

void RunBattleEffect04(void);

void AudioCommand_PlayFar(s32 cue);

#define Motion_SetTargetPositionFromMagnitudeAngle \
	((void (*)(void *, s32, s32))Motion_SetTargetPositionFromMagnitudeAngle)

#define BattleFx_ApplyColorToTargetBuffer ((void (*)(u32, s32))BattleFx_ApplyColorToTargetBuffer)
#define ObjectGroup_ApplyRandomChildValues ((void (*)(void))ObjectGroup_ApplyRandomChildValues)

#define ENA_EV_4 0x40000005 // エナジーのイベント、段階 4
#define FX_IMAJIN 8			// イマジンの演出
#define TSUBU_MAX 24		// 粒の数
#define TSUBU_OBJ 284		// 粒の物体
#define FALL_OBJ 217		// 落ちる物体
#define FALL_LAST 4			// 落ちる物体は 0-4 の 5 個

void ObjectDispatch_SetSingleChildFlagsFar(void *, s32);

// イベントワークのうち、イマジンの状態
typedef struct
{
	u8	reserved000[0xcb8];
	s16 active;			  // イマジンの効果中
	s16 transition_timer; // 残りのフレーム
} SceneTransitionState;

// ワークのスロット 31 のうち、ここで使う部分
typedef struct
{
	u8 reserved000[0x53c];
	u8 transition_status;
	u8 transition_mode;
	u8 transition_phase;
} SceneTransitionScene;

void						*BattleFx_FindMatchingEvent(u32 kind, u32 entry_index, s32 *size);
void						 BattleFx_ApplyColorToSourceBuffer(u32 battle_value, s32 enabled);
extern struct BattleFxScene *gEffectWork;

void ObjectDispatch_ApplyArgumentToChildrenFar(void *, s32);

struct BurstParticleVector
{
	s32 values[3]; // x, y, z
};

void  Vector_AddPolarOffset(s32, s32, struct BurstParticleVector *);
void *Object_Spawn(s32, s32, s32, s32);

// 粒のスプライトの属性
struct BurstParticleVisual
{
	u8 reserved_00[5];
	u8 flags_a_low : 5;
	u8 flip : 1;
	u8 flags_a_high : 2;
	u8 reserved_06;
	u8 flags_b_low : 6;
	u8 flags_b_high : 2;
	union
	{
		struct
		{
			u16 shape : 10;
			u16 unused : 6;
		} bits;
		struct
		{
			u8 lo;
			u8 high_low : 4;
			u8 palette : 4;
		} bytes;
	} attributes;
};

struct BurstParticleVisualGroup
{
	struct BurstParticleVisual primary;
	struct BurstParticleVisual child;
};

void Animation_ApplyChildValuesFar(struct FieldActor *, s32);

//--------------------------------------------------------------------------
// イマジンの効果中なら打ち切る
// 見張りをやめ、色を黒から 30 フレームで戻し、イマジンの段階 4 のイベントを実行する
//--------------------------------------------------------------------------
void ResetSceneTransitionEffect(void)
{
	// gEffectWork はスロット 56。0x74 前がイベントワーク (27)、0x64 前がスロット 31
	struct BattleFxScene **cell = &gEffectWork;
	struct BattleFxScene  *work = *cell;
	SceneTransitionScene  *scene = *(SceneTransitionScene **)((u8 *)cell - 0x64);
	SceneTransitionState  *state = *(SceneTransitionState **)((u8 *)cell - 0x74);
	s16					   zero;
	s32					   tid;
	void				  *ev;

	if (state->active != 0)
	{
		AudioCommand_PlayFar(SOUND_SCENE_TRANSITION);
		Scheduler_RemoveCallback(FieldEffect_WatchLeaderDistance);

		zero = 0;
		state->active = zero;
		state->transition_timer = zero;
		ObjectDispatch_ApplyValueToKind200Children(0);

		BattleFx_ApplyColorToTargetBuffer(PALETTE_TRANSFORM_PLAIN, 1);
		BattleFx_StartBufferInterpolation(1);
		BattleFx_ApplyColorToSourceBuffer(0, 0);
		BattleFx_ApplyColorToTargetBuffer(PALETTE_TRANSFORM_PLAIN, 0);
		BattleFx_StartBufferInterpolation(30);
		WaitFrames(1);

		ev = BattleFx_FindMatchingEvent(ENA_EV_4, FX_IMAJIN, &tid);
		if (ev != NULL)
			BattleFx_RunEventAction(ev, gGameState.selected_actor, tid);

		if (work->child_option == 0)
		{
			scene->transition_phase = 0;
			scene->transition_status = 1;
			scene->transition_mode = 1;
			WaitFrames(10);
		}
	}
}

//--------------------------------------------------------------------------
// 対象のある演出 12：対象のまわりに粒を出し、対象を足元の優先度にする
//--------------------------------------------------------------------------
void BattleFx_RunBurstParticleMainObject(void)
{
	struct FieldActor *tgt;

	tgt = gEffectWork->child;
	if (tgt != NULL)
	{
		BattleEffect_SpawnBurstParticleField();
		ObjectDispatch_ApplyArgumentToChildrenFar(tgt, 2);
		tgt->collision_flags = 0;
		ObjectDispatch_SetSingleChildFlagsFar(tgt, 0);
		tgt->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
		WaitFrames(10);
		AudioCommand_PlayFar(SOUND_RECOVERY);
		WaitFrames(40);
		BattleFx_PrepareBufferInterpolation();
	}
}

//--------------------------------------------------------------------------
// 対象のない演出 12：粒を出したあと、まわり 4-10 ドットに落ちる物体を 5 個出す
//--------------------------------------------------------------------------
void BattleFx_RunBurstParticles(void)
{
	struct BattleFxScene	   *work = gEffectWork;
	struct BurstParticleVector	pos;
	struct BurstParticleVector *p;
	s32							n;

	BattleEffect_SpawnBurstParticleField();
	AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
	p = &pos;
	n = FALL_LAST;
	do
	{
		struct FieldActor *obj;
		s32				   dist;

		p->values[0] = work->x;
		p->values[2] = work->z;
		dist = (Random16() * 6) + 0x40000;
		Vector_AddPolarOffset(dist, Random16(), p);
		p->values[1] = work->y;
		obj = Object_Spawn(FALL_OBJ, p->values[0], p->values[1], p->values[2]);
		if (obj != 0)
		{
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)obj, BattleFx_BurstParticleObjectScript);
			obj->motion_flags = ACTOR_FALLS;
		}
		WaitFrames(((Random16() * 2) >> 16) + 2);
		n--;
	} while (n >= 0);
	WaitFrames(30);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// 使う者の前から粒を 24 個、2 フレームおきに出し、演出の位置のまわりへ飛ばす
// 出る位置：向き 0x4000 は 10 ドット上、0xc000 は 24 ドット上、ほかは 10 ドット上の 10 ドット前
//--------------------------------------------------------------------------
void BattleEffect_SpawnBurstParticleField(void)
{
	struct BattleFxScene *work;
	struct FieldActor	 *user;
	s32					  pos[3];
	s32					  n;

	work = gEffectWork;
	user = work->main_object;
	BattleEffect_InitializeSharedScene();

	for (n = 0; n < TSUBU_MAX; n++)
	{
		struct FieldActor		   *obj;
		struct BurstParticleVisual *visual;
		struct BurstParticleVisual *child;
		s32							dist;

		if (work->angle == 0x4000)
		{
			pos[0] = user->x.fixed;
			pos[1] = user->y.fixed + 0xa0000;
			pos[2] = user->z.fixed;
		}
		else if (work->angle == 0xc000)
		{
			pos[0] = user->x.fixed;
			pos[1] = user->y.fixed + 0x180000;
			pos[2] = user->z.fixed;
		}
		else
		{
			pos[0] = user->x.fixed;
			pos[1] = user->y.fixed + 0xa0000;
			pos[2] = user->z.fixed;
			Vector_AddPolarOffset(0xa0000, work->angle, pos);
		}

		// 子のスプライトに、向きと形と色を写す
		obj = Object_Spawn(TSUBU_OBJ, pos[0], pos[1], pos[2]);
		visual = &((struct BurstParticleVisualGroup *)obj->sprite)->primary;
		child = &((struct BurstParticleVisualGroup *)obj->sprite)->child;
		child->flip = visual->flip;
		child->flags_a_high = visual->flags_a_high;
		child->flags_b_high = visual->flags_b_high;
		child->attributes.bits.shape = visual->attributes.bits.shape;
		child->attributes.bytes.palette = visual->attributes.bytes.palette;

		if (obj != 0)
		{
			obj->scale_y = 0xb333; // 0.7 倍
			obj->scale_x = 0xb333;
			obj->acceleration = 0x18000;
			obj->speed = 0x18000;
			obj->motion_flags = 0;
			Animation_ApplyChildValuesFar(obj, 11);
			ObjectDispatch_ApplyArgumentToChildrenFar(obj, 7);
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)obj, BattleFx_CommonParticleScript + 1);
			ObjectDispatch_SetSingleChildFlagsFar(obj, 1);

			// 行き先：演出の位置から 4-10 ドット
			pos[0] = work->x;
			pos[1] = work->y;
			pos[2] = work->z;
			if (work->angle == 0xc000)
				Vector_AddPolarOffset(0xe0000, work->angle, pos);
			dist = Random16() * 6 + 0x40000;
			Vector_AddPolarOffset(dist, Random16(), pos);
			Object_SetMoveTargetFar(obj, pos[0], pos[1], pos[2]);
		}
		AudioCommand_PlayFar(131);
		WaitFrames(2);
	}
	WaitFrames(8);
}

//--------------------------------------------------------------------------
// 対象のある演出 7
// 左右 32 ドットに物体を出して 14 ドット動かし、対象といっしょに 32 ドット持ち上げる
//--------------------------------------------------------------------------
void BattleEffect_RunTargetedItemBreak(void)
{
	void Script_WaitForEventTimeoutFar(void *object);
	struct BattleFxScene  *work;
	struct DispatchObject *user;
	struct FieldActor	  *tgt;
	struct MotionObject	  *pair[2];
	s32					   pos[3];
	s32					   x, y, z;
	s32					   n;
	s32					   start_y;

	work = gEffectWork;
	tgt = work->child;
	user = work->main_object;
	if (tgt == NULL)
		return;

	BattleEffect_InitializeSharedScene();
	user->argument = (s32)tgt;
	ObjectDispatch_InitializeFar(user, BattleFx_CommonParticleScript + 3);

	x = work->x;
	pos[0] = x;
	y = work->y + 0x100000; // 16 ドット上
	pos[1] = y;
	z = work->z;
	pos[2] = z;
	pair[0] = BattleFx_SpawnItemBreakMode3(x + 0x200000, y, z, 0x8000);
	pair[1] = BattleFx_SpawnItemBreakMode3(pos[0] - 0x200000, pos[1], pos[2], 0);

	WaitFrames(15);
	for (n = 0; n < 2; n++)
	{
		struct MotionObject *obj = pair[n];
		if (obj != NULL)
			Motion_SetTargetPositionFromMagnitudeAngle(obj, 0xe0000, obj->angle);
	}

	Script_WaitForEventTimeoutFar(pair[0]);
	tgt->update = (void (*)(union FieldObject *))ObjectGroup_ApplyRandomChildValues;
	AudioCommand_PlayFar(130);
	tgt->motion_flags = 4;

	// 1/4 ドットずつ 32 ドット上げる
	start_y = tgt->y.fixed;
	if (pair[0] != NULL && pair[1] != NULL && start_y <= start_y + 0x200000)
	{
		do
		{
			pair[0]->y += 0x4000;
			pair[1]->y += 0x4000;
			tgt->y.fixed += 0x4000;
			WaitFrames(1);
		} while (tgt->y.fixed <= start_y + 0x200000);
	}

	UpdateRisingParticleBurst(pair[0]);
	UpdateRisingParticleBurst(pair[1]);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------

// anchor.c

// エナジー演出 7：左右の二つの物体と、飛び散る破片

//--------------------------------------------------------------------------

void						 AudioCommand_PlayFar(s32 sound);
extern const s32			 BattleFx_FragmentScript[];

#define HAHEN_MAX 24 // 破片の数

//--------------------------------------------------------------------------
// 対象のない演出 7
// 演出の位置の左右 32 ドットに物体を出して動かし、破片を 24 個散らす
//--------------------------------------------------------------------------
void RunBattleEffect07(void)
{
	void						 ObjectDispatch_ReleaseFar(void *object);
	void						 Script_WaitForEventTimeoutFar(void *object);
	struct BattleFxScene  *work;
	struct MotionObject	  *obj;
	struct MotionObject	  *pair[2];
	struct ScatterPosition pos;
	struct MotionObject	 **walk;
	s32					   n;
	s32					   dist;

	work = gEffectWork;
	BattleEffect_InitializeSharedScene();
	pos.x = work->x;
	pos.y = work->y + 0x100000; // 16 ドット上
	pos.z = work->z;
	pair[0] = BattleFx_SpawnItemBreakMode3(pos.x + 0x200000, pos.y, pos.z, 0x8000);
	pair[1] = BattleFx_SpawnItemBreakMode3(pos.x - 0x200000, pos.y, pos.z, 0);

	// それぞれの向きへ 24 ドット動かす
	WaitFrames(15);
	walk = pair;
	for (n = 0; n < 2; n++)
	{
		obj = *walk++;
		if (obj != 0)
			Motion_SetTargetPositionFromMagnitudeAngle(obj, 0x180000, obj->angle);
	}

	// 破片：8-32 ドット先へ散る
	Script_WaitForEventTimeoutFar(pair[0]);
	AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
	for (n = 0; n < HAHEN_MAX; n++)
	{
		pos.x = work->x;
		pos.y = work->y + 0x100000;
		pos.z = work->z;
		obj = Object_Spawn(285, pos.x, pos.y, pos.z);
		if (obj != 0)
		{
			ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_FragmentScript);
			obj->speed_limit = Random16() + 0x20000;
			obj->acceleration = 0x20000;
			obj->motion_flags = 0;
			dist = Random16() * 24 + 0x80000;
			Motion_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());
		}
	}

	ObjectDispatch_ReleaseFar(pair[0]);
	ObjectDispatch_ReleaseFar(pair[1]);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// 物体 215 を出し、回しながら 1/4 から等倍まで 24 フレームで大きくする
// angle : 大きくなったあとの向き
//--------------------------------------------------------------------------
struct MotionObject *BattleFx_SpawnItemBreakMode3(s32 x, s32 y, s32 z, s32 angle)
{
	struct MotionObject *obj;
	s32					 scale;

	AudioCommand_PlayFar(SOUND_ITEM_BREAK);
	obj = Object_Spawn(215, x, y, z);
	if (obj != NULL)
	{
		obj->scale_x = obj->scale_y = 0x4000;
		obj->acceleration = obj->speed_limit = 0x30000; // 3 ドット/フレーム
		((struct FieldSprite *)obj->records)->priority = 0;
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 3);
		scale = obj->scale_x;
		if (scale < 0x10000)
		{
			do
			{
				scale += 0x800;
				obj->scale_y = scale;
				obj->scale_x = scale;
				obj->angle += 0x2000; // 1/8 回転
				WaitFrames(1);
				scale = obj->scale_x;
			} while (scale <= 0xffff);
		}
		obj->angle = angle;
	}
	return obj;
}

//--------------------------------------------------------------------------

// impact.c

// 散って集まる粒の動きと、重い衝撃の破片

//--------------------------------------------------------------------------

#undef Motion_SetTargetPositionFromMagnitudeAngle

#define Camera_WorldToScreen ((void (*)(struct EffectPosition *))Camera_WorldToScreen)

struct EffectPosition
{
	s32 x; // 16.16 固定小数
	s32 y;
	s32 z;
};

void BattleFx_ClearOwnedSlot(struct EffectSlot *effect);

//--------------------------------------------------------------------------
// 粒の動き : 原点のまわりへ散ってから、主オブジェクトのそばへ集まって消える
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialBurst(struct EffectSlot *slot)
{
	struct BattleFxScene *scene = gEffectWork;
	struct EffectPosition pos;
	s32					  state;

again:
	state = slot->state;
	if (state == 0)
	{
		// 原点から 25 ドット、でたらめな方向へ
		pos.x = slot->origin_x;
		pos.z = slot->origin_z;
		Vector_AddPolarOffset(25 << 16, (u16)Random16(), &pos);
		slot->target_x = pos.x;
		slot->target_z = pos.z;
		slot->acceleration = 3 << 16;
		slot->max_speed = 3 << 16;
		slot->flag42 = 0;
		slot->state++;
		return;
	}

	if (state == 1)
	{
		// 着いたらすぐ次へ
		if (BattleFx_HasReachedTarget(slot) == 0)
		{
			slot->state++;
			goto again;
		}
		return;
	}

	if (state == 2)
	{
		// 主オブジェクトの 16 ドット上から scene->angle へ 8 ドット、
		// 画面座標にして 4 ドットの揺らぎを足す
		struct MotionObject *obj = scene->main_object;

		pos.x = obj->x;
		pos.y = obj->y + (16 << 16);
		pos.z = obj->z;
		Vector_AddPolarOffset(8 << 16, scene->angle, &pos);
		Camera_WorldToScreen(&pos);
		Vector_AddPolarOffset(4 << 16, Random16(), &pos);
		slot->target_x = pos.x;
		slot->target_z = pos.z;
		slot->max_turn_step = 0x800;
		slot->flag42 = 1;
		slot->state++;
		return;
	}

	if (state == 3 && BattleFx_HasReachedTarget(slot) == 0)
		BattleFx_ClearOwnedSlot(slot);
}

//--------------------------------------------------------------------------
// 重い衝撃 : y を 32 ドット下げた位置に 0x11b を置き、破片を 12 個まき散らす
// src : 衝撃の位置になるオブジェクト
//--------------------------------------------------------------------------
s32 SpawnHeavyImpactEffect(struct MotionObject *src)
{
	struct EffectPosition pos;
	struct ObjectRuntime *obj;
	struct ObjectRuntime *frag;
	s32					  i;

	AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
	pos.x = src->x;
	pos.y = src->y;
	pos.z = src->z;
	obj = Object_Spawn(0x11b, pos.x, pos.y - (32 << 16), pos.z);
	if (obj != NULL)
	{
		obj->flags = 0;
		obj->unknown_5e = 20;
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
	}

	// 破片 : 速さ 1〜2 ドット、8〜32 ドット先のでたらめな方向へ
	for (i = 0; i < 12; i++)
	{
		frag = Object_Spawn(0x11d, pos.x, pos.y, pos.z);
		if (frag != NULL)
		{
			ObjectDispatch_InitializeFar((struct DispatchObject *)frag, BattleFx_FragmentScript);
			frag->speed_limit = Random16() + (1 << 16);
			frag->acceleration = 1 << 16;
			frag->flags = 0;
			Motion_SetTargetPositionFromMagnitudeAngle(
				frag, Random16() * 24 + (8 << 16), Random16());
		}
	}
	return 0;
}

//--------------------------------------------------------------------------
// 戦闘エフェクト 4 番を呼ぶ
//--------------------------------------------------------------------------
void BattleFx_CallEffect04(void)
{
	RunBattleEffect04();
}

//--------------------------------------------------------------------------

// orbit.c

// フィールドのエナジー演出：4 番（押し出す弾）、6 番（渦を巻いて集まる粒）

//--------------------------------------------------------------------------

#undef Camera_WorldToScreen

void Audio_PlayCue(s32 cue);

#define ObjectGroup_SetChildValueUnlessFifteenFar \
	((void (*)(s32, s32))ObjectGroup_SetChildValueUnlessFifteenFar)
#define ScriptObject_CheckOverlapFar \
	((s32 (*)(struct FieldActor *, struct BurstPosition *))ScriptObject_CheckOverlapFar)
#define Object_ReplaceResourceEntry                                                     \
	((struct AnimationObject * (*)(struct AnimationObject *, struct AnimationObject *)) \
			Object_ReplaceResourceEntry)
#define Camera_WorldToScreen ((void (*)(struct BurstPosition *))Camera_WorldToScreen)

struct BurstPosition
{
	s32 x, y, z; // 16.16 ドット
};
s32	 Map_CheckObjectStepFar(struct FieldActor *object, struct BurstPosition *pos);
void Animation_ApplyChildValuesFar(struct FieldActor *object, s32 value);

#define UE (16 << 16) // 16 ドット上

//--------------------------------------------------------------------------
// obj の 16 ドット上の位置
//--------------------------------------------------------------------------
static __inline__ void PosAbove(struct FieldActor *obj, struct BurstPosition *pos)
{
	pos->x = obj->x.fixed;
	pos->y = obj->y.fixed + UE;
	pos->z = obj->z.fixed;
}

struct SparkObject
{
	u8	unknown_00[8];
	s32 x;
	s32 y;
	s32 z;
	u8	unknown_14[4];
	s32 scale_x;
	s32 scale_y;
	u8	unknown_20[24];
	s32 anchor_x; // ゆれの中心（FieldActor の target_x の位置）
	u8	unknown_3c[12];
	s32 gravity; // FieldActor と同じ位置
	u8	unknown_4c[9];
	u8	motion_flags; // FieldActor と同じ位置
	u8	unknown_56[8];
	u16 unknown_5e;
	u8	unknown_60[4];
	s16 phase; // ゆれの位相（128 フレームで 1 周）
	u8	unknown_66[6];
	void (*update)(struct SparkObject *);
};

//--------------------------------------------------------------------------
// エナジー 4 番 : 使う者の前に弾を出して大きくし、残像 3 つと前へ飛ばす
// 当たった物（scene->child）は 16 ドット押し出す
//--------------------------------------------------------------------------
void RunBattleEffect04(void)
{
	struct FieldActor	   *tgt;
	s32						ctx;
	struct FieldActor	   *copies[4];
	struct BurstPosition	pos;
	struct FieldActor	   *obj;
	struct FieldActor	   *copy;
	struct FieldActor	   *src;
	struct BattleFxScene   *scene;
	struct EffectSlot	   *slot;
	struct AnimationObject *res;
	s32						ev;
	s32						scale;
	s32						i;
	u8						res_slot;

	scene = gEffectWork;
	tgt = scene->child;
	BattleEffect_InitializeSharedScene();
	AudioCommand_PlayFar(130);
	slot = scene->slots;

	// 使う者の頭上から、光を 2 フレームおきに 12 個はじけさせる
	for (i = 0; i <= 11; i++)
	{
		src = scene->main_object;
		PosAbove(src, &pos);
		Camera_WorldToScreen(&pos);
		EffectSlot_Initialize(slot, 0x11c, pos.x, pos.z);
		EffectSlot_SetCallback(slot, BattleFx_UpdateRadialBurst);
		EffectSlot_SetObjectMode(slot, 7);
		ObjectGroup_SetChildValueUnlessFifteenFar((s32)slot->object, 9);
		slot->scale_y = 0xb333; // 0.7 倍
		slot->scale_x = 0xb333;
		WaitFrames(2);
		slot++;
	}

	// 弾：使う者の向きに 8 ドット先で、0.25 倍から等倍まで大きくする
	src = scene->main_object;
	pos.x = src->x.fixed;
	pos.y = src->y.fixed + UE;
	pos.z = src->z.fixed;
	Vector_AddPolarOffset(8 << 16, scene->angle, &pos);
	obj = Object_Spawn(0xd7, pos.x, pos.y, pos.z);
	if (obj == NULL)
	{
		BattleFx_PrepareBufferInterpolation();
		return;
	}
	obj->scale_y = 0x4000;
	obj->scale_x = 0x4000;
	obj->facing = scene->angle;
	obj->speed = 4 << 16;
	obj->acceleration = 4 << 16;
	obj->motion_flags = 0;
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 5);
	Animation_ApplyChildValuesFar(obj, 3);
	scale = obj->scale_x;
	if (scale < 0x10000)
	{
		do
		{
			scale += 0x500;
			obj->scale_y = scale;
			obj->scale_x = scale;
			WaitFrames(1);
			scale = obj->scale_x;
		} while (scale <= 0xffff);
	}
	WaitFrames(3);

	// 残像 3 つ。絵の資源はひとつを分けあう
	res = NULL;
	for (i = 2; i >= 0; i--)
	{
		copy = copies[i] = Object_Spawn(0xd7, obj->x.fixed, obj->y.fixed, obj->z.fixed);
		if (copy != NULL)
		{
			copy->scale_y = 0xf000;
			copy->scale_x = 0xf000;
			copy->facing = scene->angle;
			copy->speed = 4 << 16;
			copy->acceleration = 4 << 16;
			copy->motion_flags = 0;
			ObjectDispatch_ApplyArgumentToChildrenFar(copy, 5);
			Animation_ApplyChildValuesFar(copy, 2);
			res = Object_ReplaceResourceEntry((struct AnimationObject *)copy->sprite, res);
		}
	}
	res_slot = res->slot;

	// 行き先：scene->enabled なら使う者の 56 ドット先、ほかは scene の位置
	if (scene->enabled != 0)
	{
		src = scene->main_object;
		pos.x = src->x.fixed;
		pos.y = src->y.fixed + UE;
		pos.z = src->z.fixed;
		Vector_AddPolarOffset(56 << 16, scene->angle, &pos);
	}
	else
	{
		pos.x = scene->x;
		pos.y = scene->y + UE;
		pos.z = scene->z;
	}
	Object_SetMoveTargetFar(obj, pos.x, pos.y, pos.z);
	ObjectDispatch_InitializeFar(
		(struct DispatchObject *)obj, BattleFx_BurstParticleObjectScript + 4);
	for (i = 0; i < 3; i++)
	{
		copy = copies[i];
		if (copy != NULL)
		{
			WaitFrames(3);
			Object_SetMoveTargetFar(copy, pos.x, pos.y, pos.z);
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)copy, BattleFx_CommonParticleScript + 1);
		}
	}

	// 弾のスクリプトが終わるまで、60 フレームまで待つ
	i = 0;
	if (((struct ScriptMotionObject *)obj)->script != NULL)
	{
	wait_script:
		WaitFrames(1);
		i++;
		if (i <= 59 && ((struct ScriptMotionObject *)obj)->script != NULL)
			goto wait_script;
	}

	// 当たった物を、ぶつからなければ 16 ドット押し出す（child_option なら跳ねる）
	if (tgt != NULL && scene->child_mode == 0)
	{
		if (scene->child_option != 0)
			tgt->velocity_y = 8 << 16;
		pos.x = tgt->x.fixed;
		pos.y = tgt->y.fixed;
		pos.z = tgt->z.fixed;
		Vector_AddPolarOffset(16 << 16, scene->angle, &pos);
		if (Map_CheckObjectStepFar(tgt, &pos) == 0 && ScriptObject_CheckOverlapFar(tgt, &pos) == 0)
		{
			tgt->acceleration = 1 << 16;
			tgt->speed = 1 << 16;
			Object_SetMoveTargetFar(tgt, pos.x, pos.y, pos.z);
		}
	}

	// 応じるマップのイベントがあれば動かす
	ev = BattleFx_FindMatchingEvent(0x50000005, 4, &ctx);
	if (ev != 0)
		BattleFx_RunEventAction((void *)ev, gGameState.selected_actor, ctx);
	WaitFrames(10);
	BattleFx_PrepareBufferInterpolation();
	WaitFrames(20);
	if (res_slot != 96)
		Resource_ResetEntry(res_slot);
}

//--------------------------------------------------------------------------
// 毎フレームの処理：8 フレームごとに 2 フレームだけアニメ 2 にして光らせる
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleMain(struct FieldActor *particle)
{
	s32 frame = gFrameCount & 7;
	if (frame == 0)
	{
		Animation_ApplyChildValuesFar(particle, 2);
	}
	else if (frame == 2)
	{
		Animation_ApplyChildValuesFar(particle, 0);
	}
}

//--------------------------------------------------------------------------
// 毎フレームの処理：1/16 ずつ縮み、1/16 以下で消える
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleFade(struct FieldActor *particle)
{
	s32 scale;

	if (particle != NULL)
	{
		scale = particle->scale_x - 0x1000;
		particle->scale_y -= 0x1000;
		particle->scale_x = scale;
		if (scale <= 0x1000)
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)particle, BattleFx_CommonParticleScript);
	}
}

//--------------------------------------------------------------------------
// 毎フレームの処理：target のまわりを左まわりに渦を巻いて寄る
// 半径は残りフレーム × 2 ドット、1 フレームに 0x800 まわる
// work は始めの向き、timer は残りフレーム。0 で縮んで消える
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleLeft(struct FieldActor *particle)
{
	struct BurstPosition pos;
	s16					 life;
	s32					 cnt;

	if (particle != NULL)
	{
		cnt = (u16)particle->timer - 1;
		particle->timer = cnt;
		life = cnt;
		if (life != 0)
		{
			pos.x = particle->target_x;
			pos.y = particle->target_y;
			pos.z = particle->target_z;
			Vector_AddPolarOffset(life << 17, (s16)particle->work + (life << 11), &pos);
			particle->x.fixed = pos.x;
			particle->y.fixed = pos.y;
			particle->z.fixed = pos.z;
		}
		else
		{
			particle->update = (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleFade;
		}
	}
}

//--------------------------------------------------------------------------
// 毎フレームの処理：左まわりと同じで、右まわり
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleRight(struct FieldActor *particle)
{
	struct BurstPosition pos;
	s16					 life;
	s32					 cnt;

	if (particle != NULL)
	{
		cnt = (u16)particle->timer - 1;
		particle->timer = cnt;
		life = cnt;
		if (life != 0)
		{
			pos.x = particle->target_x;
			pos.y = particle->target_y;
			pos.z = particle->target_z;
			Vector_AddPolarOffset(life << 17, (s16)particle->work - (life << 11), &pos);
			particle->x.fixed = pos.x;
			particle->y.fixed = pos.y;
			particle->z.fixed = pos.z;
		}
		else
		{
			particle->update = (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleFade;
		}
	}
}

//--------------------------------------------------------------------------
// エナジー 6 番をかけられた物（scene->child）から始める
// child_mode があれば、物は変えずに粒だけ出す
//--------------------------------------------------------------------------
void BattleFx_StartOrbitingParticles(void)
{
	struct BattleFxScene *scene = gEffectWork;
	struct FieldActor	 *tgt = scene->child;

	if (tgt != 0)
	{
		if (scene->child_mode != 0)
		{
			scene->enabled = 1;
		}
		tgt->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
		BattleFx_RunOrbitingParticles();
	}
}

//--------------------------------------------------------------------------
// エナジー 6 番 : 粒 16 個が scene の位置へ渦を巻いて集まり、
// 物を 16 回光らせる。child_option がなければ物を mode 2 にする
//--------------------------------------------------------------------------
void BattleFx_RunOrbitingParticles(void)
{
	s32					  ctx;
	struct BurstPosition  pos;
	struct BurstPosition *p;
	struct BattleFxScene *scene;
	struct FieldActor	 *tgt;
	struct FieldActor	 *particle;
	void				 *ev;
	s32					  cnt;

	scene = gEffectWork;
	tgt = scene->child;
	BattleEffect_InitializeSharedScene();
	AudioCommand_PlayFar(115);

	// 3 フレームおきに粒を出す。大きさ 0.5〜1 倍、まわる向きはでたらめ
	// 中心は scene の位置から 2〜6 ドットずらす
	p = &pos;
	cnt = 15;
	do
	{
		particle = Object_Spawn(0xe8, 0, 0, 0);
		if (particle != NULL)
		{
			u32 scale;
			s32 dist;

			scale = (Random16() >> 1) + 0x8000;
			particle->scale_y = scale;
			particle->scale_x = scale;
			if ((Random16() & 1) != 0)
				particle->update =
					(void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleLeft;
			else
				particle->update =
					(void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleRight;

			particle->facing = Random16();
			particle->timer = 60;
			particle->work = Random16();
			Animation_ApplyChildValuesFar(particle, 9);

			p->x = scene->x;
			p->y = scene->y;
			p->z = scene->z;
			dist = (Random16() << 2) + (2 << 16);
			Vector_AddPolarOffset(dist, Random16(), p);
			particle->target_x = p->x;
			particle->target_y = p->y;
			particle->target_z = p->z;
		}

		WaitFrames(3);
		cnt--;
	} while (cnt >= 0);

	WaitFrames(10);
	AudioCommand_PlayFar(115);
	WaitFrames(50);

	if (tgt != NULL && scene->enabled == 0)
	{
		AudioCommand_PlayFar(212);

		// 5 フレームごとに 1 フレーム光る
		cnt = 15;
		do
		{
			Animation_ApplyChildValuesFar(tgt, 7);
			WaitFrames(1);
			Animation_ApplyChildValuesFar(tgt, 0);
			WaitFrames(4);
			cnt--;
		} while (cnt >= 0);

		if (scene->child_option == 0)
		{
			AudioCommand_PlayFar(220);
			ObjectDispatch_ApplyArgumentToChildrenFar(tgt, 2);
		}

		tgt->update = (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleMain;
		ev = BattleFx_FindMatchingEvent(0x50000005, 6, &ctx);
		if (ev != NULL)
		{
			BattleFx_RunEventAction(ev, gGameState.selected_actor, ctx);
		}
		WaitFrames(20);
	}

	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// 毎フレームの処理：anchor_x を中心に左右 4 ドット、128 フレームで 1 往復ゆれる
//--------------------------------------------------------------------------
void BattleFx_SwayParticle(struct SparkObject *obj)
{
	s16 *phase;

	phase = &obj->phase;
	obj->x = obj->anchor_x + Iwram_MulQ16(4 << 16, Trig_Sin(*phase << 9));
	(*phase)++;
	*phase = (*phase + 128) % 128;
}

//--------------------------------------------------------------------------
// 毎フレームの処理：scene の位置の 10 ドット上を中心に、渦を巻いて寄る
// 半径は残りフレームと同じドット数。timer が 0 で消える
//--------------------------------------------------------------------------
void BattleFx_UpdateShrinkingOrbitObject(struct FieldActor *obj)
{
	struct BattleFxScene *scene = gEffectWork;
	struct BurstPosition  pos;
	s16					  life;
	s32					  cnt;

	if (obj != NULL)
	{
		cnt = (u16)obj->timer - 1;
		obj->timer = cnt;
		life = cnt;
		if (life != 0)
		{
			pos.x = scene->x;
			pos.y = scene->y + (10 << 16);
			pos.z = scene->z;
			Vector_AddPolarOffset(life << 16, (s16)obj->work + (life << 11), &pos);
			obj->x.fixed = pos.x;
			obj->y.fixed = pos.y;
			obj->z.fixed = pos.z;
		}
		else
		{
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)obj, BattleFx_CommonParticleScript);
		}
	}
}

//--------------------------------------------------------------------------
// 毎フレームの処理：scene の位置の 16 ドット上で左右 6 ドット（64 フレーム）ゆれ、
// 3 フレームごとに、ゆれながらゆっくり落ちる粒を出す
// phase が -1 なら、その場から動かない
//--------------------------------------------------------------------------
void BattleFx_RunSparkEmitter(struct SparkObject *obj)
{
	struct BattleFxScene *scene;
	s16					 *phase;
	struct SparkObject	 *spark;
	s32					  pos[3];

	phase = &obj->phase;
	scene = gEffectWork;
	if (*phase != -1)
	{
		obj->x = scene->x + Iwram_MulQ16(6 << 16, Trig_Sin(*phase << 10));
		obj->y = scene->y + UE;
		obj->z = scene->z;
		(*phase)++;
		*phase = (*phase + 64) % 64;
	}
	if (gFrameCount % 3 == 0)
	{
		// 2 ドット上から、でたらめな向きに 0〜6 ドットずらす
		pos[0] = obj->x;
		pos[1] = obj->y + (2 << 16);
		pos[2] = obj->z;
		Vector_AddPolarOffset(Random16() * 6, Random16(), pos);
		spark = Object_Spawn(0x11d, pos[0], pos[1], pos[2]);
		if (spark != 0)
		{
			spark->update = BattleFx_SwayParticle;
			spark->scale_x = spark->scale_y = 0x9999; // 0.6 倍
			spark->motion_flags = ACTOR_FALLS;
			spark->gravity = 458;
			spark->phase = Random16() >> 9;
			spark->anchor_x = spark->x;
			Animation_ApplyChildValuesFar(spark, 9);
			spark->unknown_5e = 72;
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)spark, BattleFx_CommonParticleScript);
		}
	}
}

//--------------------------------------------------------------------------

// call_effect_03_and_stop.c

// 演出 03：渦を巻いて集まる 8 個の粒と、火花を出して飛び去る 1 個

//--------------------------------------------------------------------------

#undef Object_ReplaceResourceEntry

#define Motion_SetTargetPositionFromMagnitudeAngle \
	((void (*)(struct BattleEffect03Object *, s32, s32))Motion_SetTargetPositionFromMagnitudeAngle)

#define BattleFx_UpdateShrinkingOrbitObject ((void (*)(void))BattleFx_UpdateShrinkingOrbitObject)
#define BattleFx_RunSparkEmitter ((void (*)(void))BattleFx_RunSparkEmitter)
#define Object_ReplaceResourceEntry                                         \
	((struct BattleEffect03Link * (*)(void *, struct BattleEffect03Link *)) \
			Object_ReplaceResourceEntry)

//--------------------------------------------------------------------------
// 演出 03 を流し、いまのオブジェクトを止める
//--------------------------------------------------------------------------
void BattleFx_CallEffect03AndStop(void)
{
	RunBattleEffect03();
	EffectRuntime_StopCurrentObject();
}

struct BattleEffect03Object
{
	u8	  reserved_00[8];
	s32	  x;
	s32	  y;
	s32	  z;
	u8	  reserved_14[4];
	s32	  scale_x;
	s32	  scale_y;
	u8	  reserved_20[16];
	s32	  velocity_x;
	s32	  velocity_y;
	u8	  reserved_38[24];
	void *visual;
	u8	  reserved_54;
	u8	  mode;
	u8	  reserved_56[4];
	u8	  unknown_5a;
	u8	  reserved_5b[9];
	s16	  angle; // 渦の粒では残りフレーム（BattleFx_UpdateShrinkingOrbitObject）
	s16	  phase; // 渦の粒では最初の角度
	u8	  reserved_68[4];
	void *callback;
};

struct BattleEffect03Link
{
	u8 reserved_00[28];
	u8 marker;
};
void						 WaitFrames(s32);
void						 AudioCommand_PlayFar(s32);

//--------------------------------------------------------------------------
// 演出 03
// 8 個の粒が渦を巻いて中心へ集まり、中心に現れた 1 個が火花を出してから
// 遠くへ飛び去る
//--------------------------------------------------------------------------
void RunBattleEffect03(void)
{
	void						 ObjectDispatch_ReleaseFar(void *);
	void						 Script_WaitForEventTimeoutFar(void *);
	struct BattleFxScene		*scn = gEffectWork;
	struct BattleEffect03Object *target = scn->main_object;
	struct BattleEffect03Object *object;
	struct BattleEffect03Link	*last;
	u8							 marker;
	s32							 i;

	BattleEffect_InitializeSharedScene();

	// 渦の粒：1 フレームに 1 個ずつ、1/8 周ずらして出す
	last = 0;
	for (i = 0; i < 8; i++)
	{
		object = Object_Spawn(233, target->x, target->y + 0x200000, target->z);
		if (object != 0)
		{
			object->scale_y = 0xb333; // 0.7 倍
			object->scale_x = 0xb333;
			object->callback = BattleFx_UpdateShrinkingOrbitObject;
			object->angle = 120;
			object->phase = i << 13;
			object->mode = 4;
			last = Object_ReplaceResourceEntry(object->visual, last);
		}
		WaitFrames(1);
	}

	marker = last->marker;
	AudioCommand_PlayFar(130);
	WaitFrames(110);

	// 中心の 1 個
	target = Object_Spawn(233, 0, 0, 0);
	object = target;
	if (target != 0)
	{
		target->scale_y = 0xb333;
		target->scale_x = 0xb333;
		target->x = scn->x;
		target->y = scn->y + 0x100000;
		target->z = scn->z;
		target->mode = 4;
		Animation_ApplyChildValuesFar(target, 7);
	}

	AudioCommand_PlayFar(131);
	WaitFrames(12);
	if (target != 0)
	{
		// 2 フレームごとに子の値を切り替える（4 回に 1 回は 10）
		for (i = 0; i < 30; i++)
		{
			/* FAKEMATCH: needed to match: the ROM keeps object as a second register copy of the spark (r5 beside r6); this redundant reassignment stops gcse copy-propagating object = target away, and loop then drops it (differ 1 without it). */
			target = object;
			if (i & 3)
				Animation_ApplyChildValuesFar(object, 9);
			else
				Animation_ApplyChildValuesFar(object, 10);
			WaitFrames(2);
		}
	}

	// 火花
	Animation_ApplyChildValuesFar(object, 0);
	AudioCommand_PlayFar(84);
	if (object != 0)
	{
		target->callback = BattleFx_RunSparkEmitter;
		target->angle = 0;
		if (scn->enabled != 0)
			WaitFrames(128);
		else
			WaitFrames(192);
	}

	// 飛び去る
	if (target != 0)
	{
		target->angle = -1;
		target->velocity_x = 0x50000;
		target->velocity_y = 0x6666;
		target->unknown_5a = 0;
		Motion_SetTargetPositionFromMagnitudeAngle(target, 0xc00000, 0xe800);
		Script_WaitForEventTimeoutFar(target);
		ObjectDispatch_ReleaseFar(target);
	}
	if (marker != 0x60)
		Resource_ResetEntry(marker);
	if (scn->finish_callback != 0)
		scn->finish_callback();
	BattleFx_PrepareBufferInterpolation();
}
