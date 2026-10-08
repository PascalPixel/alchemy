//--------------------------------------------------------------------------
// circle.c
// バトル演出：輪に散る粒子と、中心のまわりを回るオブジェクト
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
#include "script_motion.h"
#include "motobj.h"
#include "types.h"
#include "objdisp.h"
#include "field_effect.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "fx_scene.h"
#include "fieldobj.h"
#include "scene.h"
#include "global_cells.h"
#include "effect_slot.h"
#include "sound_ids.h"
#include "object_lookup.h"
#include "game_state.h"
#include "animspr.h"

void BattleEffect_RunFallbackObjectTransition(void);
void RunBattleEffect14(void);

#define Object_SetPositionAndResetMotionFar \
	((void (*)(void *, s32, s32, s32))Object_SetPositionAndResetMotionFar)

extern const s32 BattleFx_CommonParticleScript[];

struct OrbitEffect
{
	u8					unknown_00[6];
	u16					angle; // 進む向き・回る角度
	s32					x;
	s32					y;
	s32					z;
	u8					unknown_14[20];
	s32					scale;
	u8					unknown_2c[28];
	s32					speed;
	u8					unknown_4c[24];
	u16					timer;	// 経過フレーム
	s16					pause;	// 向きを大きく変える残りフレーム
	struct OrbitEffect *anchor; // 回る中心
	void (*update)(struct OrbitEffect *);
};

//--------------------------------------------------------------------------
// 粒子の毎フレーム処理
// 速度で進み、x と z の速度を少しずつ落とし、拡大率と回転を変える
//--------------------------------------------------------------------------
void BattleFx_UpdateParticleMotionAndScale(union FieldObject *object)
{
	struct FieldEffect *efx = &object->effect;

	efx->x += efx->velocity_x;
	efx->y += efx->velocity_y;
	efx->z += efx->velocity_z;
	efx->velocity_x -= efx->velocity_x / 18;
	efx->velocity_z -= efx->velocity_z / 16;
	efx->scale_x += efx->scale_rate_x;
	efx->scale_y += efx->scale_rate_y;
	efx->sprite->rotation += efx->spin;
}

//--------------------------------------------------------------------------
// origin の位置から、水平に輪を描いて広がる粒子を 17 個出す
// x 方向の速度は z 方向の 1.5 倍
//--------------------------------------------------------------------------
void BattleFx_SpawnRadialParticleRing(struct MotionObject *origin)
{
	struct EffectOptions opt;
	s32					 vel[3];
	s32					 vz;
	s32					 angle;
	u32					 i;

	opt.palette = 0;
	opt.update = BattleFx_UpdateParticleMotionAndScale;
	opt.start_scale_x = 0xcccc; // 0.8 倍
	opt.start_scale_y = 0xcccc;
	i = 0;
	do
	{
		angle = i << 12; // 1/16 周ずつ
		vel[0] = Trig_Cos(angle) * 3 / 2;
		vel[1] = 0;
		vz = Trig_Sin(angle);
		vel[2] = vz;
		Effect_SpawnResident(origin->x, origin->y, origin->z, vel[0], vel[1], vz, 0x01090001, &opt);
		i++;
	} while (i <= 16);
}

//--------------------------------------------------------------------------
// 中心を離れてさまよう（毎フレーム）
// 1 フレームに 2〜3 ドット進み、ときどき向きを大きく変える
// 101 フレームで共通の粒子スクリプトへ移る
//--------------------------------------------------------------------------
void BattleFx_WanderAroundAnchor(struct OrbitEffect *efx)
{
	s32 len;
	s32 angle;
	s32 dx;
	s32 dz;

	len = Random16() + 0x20000;
	angle = efx->angle;
	dx = Iwram_MulQ16(len, Trig_Cos(angle));
	dz = Iwram_MulQ16(len, Trig_Sin(angle));
	efx->x += dx;
	efx->z += dz;
	efx->angle += 0xfff0;
	if (efx->pause != 0)
	{
		efx->pause--;
		efx->angle += 0x800;
	}
	else if ((Random16() << 5) >> 16 == 0) // 1/32 の確率
	{
		efx->pause = ((Random16() << 4) >> 16) + 8; // 8〜23 フレーム
	}
	if (++efx->timer == 101)
		ObjectDispatch_InitializeFar((struct DispatchObject *)efx, BattleFx_CommonParticleScript);
}

//--------------------------------------------------------------------------
// 中心のまわりを半径 8 ドットで回る（毎フレーム）
// 121 フレーム回ったら、さまよう動きに切り替える
//--------------------------------------------------------------------------
void BattleFx_CircleAnchor(struct OrbitEffect *efx)
{
	struct OrbitEffect *anchor;
	s32					angle;
	s32					r;
	s32					dx;
	s32					dz;

	anchor = efx->anchor;
	r = 0x80000;
	angle = efx->angle;
	dx = Iwram_MulQ16(r, Trig_Cos(angle));
	dz = Iwram_MulQ16(r, Trig_Sin(angle));
	efx->x = anchor->x + dx;
	efx->z = anchor->z + dz;
	efx->angle += 0x800;
	if (++efx->timer == 121)
	{
		efx->update = BattleFx_WanderAroundAnchor;
		efx->timer = 0;
		efx->pause = 0;
		efx->speed = 0x1999;
		efx->scale = 0x30000;
		efx->angle = Random16();
	}
}

//--------------------------------------------------------------------------
// 毎フレーム縮め、0.5 倍を切ったら位置を戻して処理を外す
//--------------------------------------------------------------------------
void BattleFx_ShrinkObjectScaleUntilHalf(struct MotionObject *obj)
{
	s32 scale;

	scale = obj->scale_x - 0x80;
	obj->scale_y = scale;
	obj->scale_x = scale;
	if (scale < 0x8000)
	{
		Object_SetPositionAndResetMotionFar(obj, 0, 0, 0);
		((struct ScriptMotionObject *)obj)->hook = 0;
	}
}

void BattleFx_CallEffect14(void)
{
	RunBattleEffect14();
}

//--------------------------------------------------------------------------

// radial.c

// フィールドのエナジー演出：14 番（運ばれる玉と、まわりを回る粒）ほか

//--------------------------------------------------------------------------

#define BattleFx_ShrinkObjectScaleUntilHalf ((void (*)(void))BattleFx_ShrinkObjectScaleUntilHalf)
#define BattleFx_CircleAnchor ((void (*)(void))BattleFx_CircleAnchor)

struct EffectParticle
{
	u8					   reserved_00[6];
	s16					   angle; // 0x10000 で 1 周
	s32					   x;	  // 16.16 ドット
	s32					   y;
	s32					   z;
	u8					   reserved_14[4];
	s32					   scale_x; // 0x10000 で等倍
	s32					   scale_y;
	u8					   reserved_20[0x64 - 0x20];
	s16					   timer;  // BattleFx_CircleAnchor が数える
	s16					   pause;  // 同じく
	struct EffectParticle *anchor; // まわる中心
	void (*callback)(void);
};

extern struct BattleFxScene *gEffectWork;
void  AudioCommand_PlayFar(s32);
void  Vector_AddPolarOffset(s32, s32, struct EffectVector *);

//--------------------------------------------------------------------------
// エナジー 14 番 : 玉を使う者から scene の位置（4 ドット上）まで 10 フレームで運び、
// 0.25 倍から大きくする。scene->enabled がなければ、かけられた物を縮め、
// 玉のまわりに 6 フレームおきに粒を 16 個出して回らせる
//--------------------------------------------------------------------------
void RunBattleEffect14(void)
{
	void  ObjectDispatch_ApplyArgumentToChildrenFar(void *, s32);
	void  ObjectDispatch_ReleaseFar(void *);
	void *Object_Spawn(s32, s32, s32, s32);
	struct BattleFxScene  *scene = gEffectWork;
	struct EffectParticle *src = scene->main_object;
	struct EffectParticle *tgt = scene->child;
	struct EffectVector	   pos;
	struct EffectVector	   from;
	struct EffectVector	   to;
	struct EffectParticle *tama;
	s32					   step;
	struct EffectParticle *particle;
	s32					   steps = 11;

	step = 0;
	from.x = src->x;
	from.y = src->y;
	from.z = src->z;
	to.x = scene->x;
	to.y = scene->y - (4 << 16);
	to.z = scene->z;
	tama = Object_Spawn(0xda, 0, 0, 0);
	if (tama == 0)
	{
		return;
	}
	BattleEffect_InitializeSharedScene();
	ObjectDispatch_ApplyArgumentToChildrenFar(tama, 2);
	do
	{
		s32 scale;
		tama->x = from.x + step * (to.x - from.x) / 10;
		tama->y = from.y + step * (to.y - from.y) / 10;
		tama->z = from.z + step * (to.z - from.z) / 10;
		scale = 0x4000 + step * 0x10ccc / 10; // 0.25 倍から 1.3 倍へ
		tama->scale_x = scale;
		tama->scale_y = scale;
		WaitFrames(1);
		step++;
	} while (step < steps);
	tama->scale_x = 0x1b333; // 横 1.7 倍
	tama->scale_y = 0x14ccc; // 縦 1.3 倍
	AudioCommand_PlayFar(163);
	WaitFrames(20);
	if (scene->enabled == 0)
	{
		if (tgt != 0)
			tgt->callback = BattleFx_ShrinkObjectScaleUntilHalf;

		// 粒は玉の 4 ドット上から 0.8 ドットずつ高く、3〜8 ドット離して出す
		step = 0;
		do
		{
			s32 dist;

			pos.x = tama->x;
			pos.y = tama->y + step * 0xcccc + (4 << 16);
			pos.z = tama->z;
			dist = Random16() * 5 + (3 << 16);
			Vector_AddPolarOffset(dist, Random16(), &pos);
			particle = Object_Spawn(0xf9, pos.x, pos.y, pos.z);
			if (particle != 0)
			{
				particle->callback = BattleFx_CircleAnchor;
				particle->anchor = tama;
				particle->timer = 0;
				particle->pause = 0;
				particle->angle = Random16();
			}
			WaitFrames(6);
			step++;
		} while (step <= 15);
		WaitFrames(20);
		WaitFrames(120);
	}
	ObjectDispatch_ApplyArgumentToChildrenFar(tama, 1);
	WaitFrames(30);
	AudioCommand_PlayFar(136);
	WaitFrames(20);
	ObjectDispatch_ReleaseFar(tama);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// スロットの粒の動き（毎フレーム）
// state 0 : 出た所から 40〜70 ドット離れた所へ向かう
// state 1 : 着くまで待ち、着いたら state 2 へ
// state 2 : scene の位置の 8 ドット上（画面の位置）のまわり 4 ドットへ向かう
// state 3 : 着いたらスロットを空ける
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialCamera(struct EffectSlot *effect)
{
	struct BattleFxScene *scene;
	struct EffectVector	  pos;
	s8					 *st;
	s16					  angle;
	s32					  state;

	scene = gEffectWork;
	st = &effect->state;
top:
	state = *st;
	if (state == 0)
	{
		pos.x = effect->origin_x;
		pos.z = effect->origin_z;
		angle = Random16();
		Vector_AddPolarOffset(Random16() * 30 + (40 << 16), (u16)angle, &pos);
		effect->target_x = pos.x;
		effect->target_z = pos.z;
		effect->acceleration = 4 << 16;
		effect->max_speed = 4 << 16;
		effect->flag42 = state;
		goto advance;
	}
	if (state == 1)
	{
		if (BattleFx_HasReachedTarget(effect) != 0)
			return;
		*st = *st + 1;
		goto top;
	}
	else if (state == 2)
	{
		pos.x = scene->x;
		pos.y = scene->y + (8 << 16);
		pos.z = scene->z;
		Camera_WorldToScreen(&pos);
		Vector_AddPolarOffset(4 << 16, Random16(), &pos);
		effect->target_x = pos.x;
		effect->target_z = pos.z;
		effect->max_turn_step = 0x1000;
		effect->flag42 = 1;
	advance:
		*st = *st + 1;
		return;
	}
	else if (state == 3)
	{
		if (BattleFx_HasReachedTarget(effect) == 0)
			BattleFx_ClearOwnedSlot(effect);
		return;
	}
	else
	{
		return;
	}
}

//--------------------------------------------------------------------------
// かけられた物（scene->child）に paused を立て、
// BattleEffect_RunFallbackObjectTransition の演出を動かす
//--------------------------------------------------------------------------
void BattleFx_MarkChildAndRunFallbackTransition(void)
{
	struct FieldActor *tgt = gEffectWork->child;

	tgt->paused = 1;
	BattleEffect_RunFallbackObjectTransition();
}

//--------------------------------------------------------------------------

// fallback.c

// フィールド効果：対象のない場所へ飛ばして散らす

//--------------------------------------------------------------------------

#define Camera_WorldToScreen ((void (*)(s32 *))Camera_WorldToScreen)
#define ObjectGroup_SetChildValueUnlessFifteenFar \
	((void (*)(struct MotionObject *, s32))ObjectGroup_SetChildValueUnlessFifteenFar)

struct FallbackPosition
{
	s32 x;
	s32 y;
	s32 z;
};
void				 WaitFrames(s32);
u32					 Random16(void);

//--------------------------------------------------------------------------
// 使い手から効果の位置へオブジェクトを 10 フレームで飛ばし、16 個の粒にして散らす
//--------------------------------------------------------------------------
void BattleEffect_RunFallbackObjectTransition(void)
{
	void				 ObjectDispatch_ApplyArgumentToChildrenFar(struct MotionObject *, s32);
	void				 ObjectDispatch_ReleaseFar(struct MotionObject *);
	struct MotionObject *Object_Spawn(s32, s32, s32, s32);
	struct BattleFxScene   *scene = gEffectWork;
	struct MotionObject	   *user = scene->main_object;
	struct MotionObject	   *obj;
	struct EffectSlot	   *slot;
	struct FallbackPosition pos;
	struct FallbackPosition from;
	struct FallbackPosition to;
	s32						step;
	s32						i;

	scene->y = user->y;
	obj = Object_Spawn(250, 0, 0, 0);
	step = 0;
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
	if (obj == 0)
		return;

	// 使い手の 16 ドット上から、効果の位置の 8 ドット上へ
	BattleEffect_InitializeSharedScene();
	from.x = user->x;
	from.y = user->y + (16 << 16);
	from.z = user->z;
	to.x = scene->x;
	to.y = scene->y + (8 << 16);
	to.z = scene->z;
	for (;;)
	{
		s32 steps = 11;
		s32 scale;

		obj->x = from.x + (to.x - from.x) * step / 10;
		obj->y = from.y + (to.y - from.y) * step / 10;
		obj->z = from.z + (to.z - from.z) * step / 10;
		scale = BattleFx_Interpolate(0x10000, 0x4000, step); // 0.25 倍から等倍へ
		obj->scale_x = scale;
		obj->scale_y = scale;
		WaitFrames(1);
		step++;
		if (step >= steps)
			break;
	}

	WaitFrames(5);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
	AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);
	WaitFrames(10);
	AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);
	WaitFrames(10);
	AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);
	WaitFrames(10);
	AudioCommand_PlayFar(SOUND_TRIPLE_TONE_HIGH);

	// その周り 4 ドットに粒を置き、外へ飛ばしてから効果の位置へ戻す
	slot = &scene->slots[0];
	for (i = 0; i < 16; i++)
	{
		pos.x = obj->x;
		pos.y = obj->y + (8 << 16);
		pos.z = obj->z;
		Camera_WorldToScreen(&pos.x);
		Vector_AddPolarOffset(4 << 16, Random16(), &pos.x);
		EffectSlot_Initialize(slot, 285, pos.x, pos.z);
		EffectSlot_SetCallback(slot, BattleFx_UpdateRadialCamera);
		ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, 7);
		slot++;
	}

	pos.x = obj->x;
	pos.y = obj->y + (8 << 16);
	pos.z = obj->z;
	WaitFrames(8);
	ObjectDispatch_ReleaseFar(obj);
	WaitFrames(4);
	WaitFrames(30);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------

// object_pause.c

// オブジェクトを止めて表の値をでたらめに当て続け、もとの処理に戻す

//--------------------------------------------------------------------------

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
