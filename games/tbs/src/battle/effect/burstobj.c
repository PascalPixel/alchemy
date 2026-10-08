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
#include "system.h"
#include "object_efx.h"
#include "motobj.h"
#include "object_dispatch.h"
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
void Script_WaitForEventTimeoutFar(void *object);

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
