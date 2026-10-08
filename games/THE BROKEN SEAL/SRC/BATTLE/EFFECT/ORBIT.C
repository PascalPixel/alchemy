//--------------------------------------------------------------------------
// ORBIT.C
// フィールドのエナジー演出：4 番（押し出す弾）、6 番（渦を巻いて集まる粒）
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
#include "RESOURCE.H"
#include "ANIMSPR.H"
#include "OBJECT_RUNTIME.H"
#include "SCRIPT_MOTION.H"
#include "FIELDOBJ.H"
#include "FRAME.H"
#include "TYPES.H"
#include "GAME_STATE.H"
#include "FX_SCENE.H"
#include "OBJDISP.H"
#include "EFFECT_SLOT.H"
#include "BATTLE_EFFECT_RUNTIME.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "OBJECT_EFX.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"

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

extern struct BattleFxScene *gEffectWork;

void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct BurstPosition *pos);
void Object_SetMode(struct FieldActor *object, s32 mode);
void Object_SetPosition(struct FieldActor *object, s32 x, s32 y, s32 z);
s32	 Object_CheckMovementCollision(struct FieldActor *object, struct BurstPosition *pos);
void Animation_ApplyChildValuesFar(struct FieldActor *object, s32 value);
void Audio_PlayCue(s32 cue);

struct FieldActor *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);

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
	Audio_PlayCue(130);
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
	Object_SetMode(obj, 5);
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
			Object_SetMode(copy, 5);
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
	Object_SetPosition(obj, pos.x, pos.y, pos.z);
	ObjectDispatch_InitializeFar(
		(struct DispatchObject *)obj, BattleFx_BurstParticleObjectScript + 4);
	for (i = 0; i < 3; i++)
	{
		copy = copies[i];
		if (copy != NULL)
		{
			WaitFrames(3);
			Object_SetPosition(copy, pos.x, pos.y, pos.z);
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
		if (Object_CheckMovementCollision(tgt, &pos) == 0
			&& ScriptObject_CheckOverlapFar(tgt, &pos) == 0)
		{
			tgt->acceleration = 1 << 16;
			tgt->speed = 1 << 16;
			Object_SetPosition(tgt, pos.x, pos.y, pos.z);
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
		cnt = particle->timer - 1;
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
		cnt = particle->timer - 1;
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
	Audio_PlayCue(115);

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
	Audio_PlayCue(115);
	WaitFrames(50);

	if (tgt != NULL && scene->enabled == 0)
	{
		Audio_PlayCue(212);

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
			Audio_PlayCue(220);
			Object_SetMode(tgt, 2);
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
		cnt = obj->timer - 1;
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
