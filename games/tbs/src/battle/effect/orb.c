//--------------------------------------------------------------------------
// orb.c
// 姿を隠すあいだの足もとの確認と、戦闘エフェクト 5 番 (光の玉と降る粒)
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
#include "animspr.h"
#include "event_runtime.h"
#include "object_runtime.h"
#include "fieldobj.h"
#include "object_lookup.h"
#include "objdisp.h"
#include "types.h"
#include "scene.h"
#include "map.h"
#include "game_state.h"
#include "callback_scheduler.h"
#include "fixed_math.h"
#include "object_efx.h"
#include "object_effect.h"
#include "field_event.h"
#include "keystate.h"
#include "io_reg.h"
#include "fx_scene.h"
#include "sound_ids.h"
#include "motobj.h"
#include "field_effect.h"

extern u8 gMapCellBuffer[];

void AudioCommand_PlayFar(s32);
void ObjectDispatch_ApplyArgumentToChildrenFar(struct ObjectRuntime *, s32);

void *Object_Spawn(s32, s32, s32, s32);
extern void *gEffectWork;

#define CLOAK_TILE 0xfb	 // 隠れていられるタイルの属性
#define CLOAK_END 0x2092 // 隠れていられないときの psynergy_request

//--------------------------------------------------------------------------
// 毎フレームの処理 : 選択中のオブジェクトの足もとのタイルが
// CLOAK_TILE でなければ、CLOAK_END を要求する
// イベントの mode が 3 なら 32 ドット四方の gMapBlocks、
// ほかは 16 ドット四方の地形のマップを見る
//--------------------------------------------------------------------------
void CheckObjectMapTile(void)
{
	u32					  slot;
	struct EventRuntime	 *ev;
	struct ObjectRuntime *obj;
	u8					 *tile;
	s32					  x;
	s32					  y;

	slot = (u32)&gEventWork;
	ev = gEventWork;
	obj = ObjectTable_Get(gGameState.selected_actor);
	tile = (u8 *)*(struct MapState **)(slot - 76); // gMapWork[0]

	if (ev->mode_19e == 3)
	{
		u32 tx;
		u32 ty;

		x = obj->x;
		if (x < 0)
			x += 0x1fffff;
		tx = (x >> 21) & 31;

		y = obj->z;
		if (y < 0)
			y += 0x1fffff;
		ty = (y >> 21) & 31;

		tile = (u8 *)gMapBlocks + ((tx + (ty << 5)) << 2);
	}
	else
	{
		if (obj->terrain_id <= 2)
		{
			tile = (u8 *)((struct MapState *)tile)->layers[obj->terrain_id].cells;
		}
		else
			tile = (u8 *)gMapCellBuffer;

		x = obj->x;
		if (x < 0)
			x += 0xfffff;
		{
			u32 tx = x >> 20;

			y = obj->z;
			if (y < 0)
				y += 0xfffff;

			tile = (u8 *)(tile + ((tx + ((y >> 20) << 7)) << 2));
		}
	}

	if (tile[2] != CLOAK_TILE)
		((struct EventWork *)ev)->psynergy_request = CLOAK_END;
}

//--------------------------------------------------------------------------
// 姿を戻す : 確認を止め、点滅させて cloaked を消す
//--------------------------------------------------------------------------
void MapEvent_RunTileTriggerSequence(void)
{
	struct ObjectRuntime   *obj;
	struct AnimationObject *anim;
	struct AnimationEntry  *entry;
	u32						i;

	obj = ObjectTable_Get(gGameState.selected_actor);
	anim = obj->animation;
	entry = anim->entries[0];

	AudioCommand_PlayFar(154);
	Scheduler_RemoveCallback(CheckObjectMapTile);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
	((struct ScriptMotionObject *)obj)->hook = 0;

	for (i = 0; i < 5; ++i)
	{
		entry->param = 7;
		anim->dirty = 1;
		anim->flags = 2;
		WaitFrames(2);
		anim->dirty = 1;
		anim->flags = 0;
		WaitFrames(2);
	}

	for (i = 0; i < 5; ++i)
	{
		entry->param = 7;
		anim->dirty = 1;
		anim->flags = 0;
		WaitFrames(2);
		entry->param = 0;
		anim->dirty = 1;
		WaitFrames(2);
	}

	anim->flags = 1;
	gGameState.cloaked = 0;
}

//--------------------------------------------------------------------------
// 隠れているなら、足もとの確認を始める
//--------------------------------------------------------------------------
void BattleFx_ScheduleCloakedMapCheck(void)
{
	if (gGameState.cloaked != 0)
	{
		Scheduler_AddOrUpdateCallback((SchedulerCallback)CheckObjectMapTile, 0xc80);
	}
}

//--------------------------------------------------------------------------
// 姿を隠す : 10 回点滅させて cloaked を立て、足もとを確かめる
// その場で隠れられなければ、すぐに姿を戻す
//--------------------------------------------------------------------------
void BattleFx_RunFlashingCallbackSequence(void)
{
	struct EventWork	   *ev = gEventWork;
	struct ObjectRuntime   *obj = ObjectTable_Get(gGameState.selected_actor);
	struct AnimationObject *anim = obj->animation;
	struct AnimationEntry  *entry = anim->entries[0];
	u32						n;
	void (*check)(void);

	AudioCommand_PlayFar(130);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
	((struct ScriptMotionObject *)obj)->hook = 0;
	n = 0;
	do
	{
		entry->param = 7;
		anim->dirty = 1;
		anim->flags = 2;
		WaitFrames(2);
		anim->dirty = 1;
		anim->flags = 0;
		WaitFrames(2);
		n++;
	} while (n <= 9);
	n = 0;
	entry->param = n;
	anim->flags = 2;
	anim->dirty = 1;
	check = CheckObjectMapTile;
	Scheduler_AddOrUpdateCallback(check, 0xc80);
	gGameState.cloaked = 1;
	check();
	if (ev->psynergy_request == CLOAK_END)
	{
		MapEvent_RunTileTriggerSequence();
		ev->psynergy_request = n;
	}
}

//--------------------------------------------------------------------------
// 戦闘エフェクト 5 番
//--------------------------------------------------------------------------
struct Effect05Point
{
	s32 x, y, z; // 16.16 固定小数
};

struct Effect05Target
{
	u8					 reserved_00[8];
	struct Effect05Point position;
};

struct Effect05Sprite
{
	u32 unknown_00[2];
	u8	unknown_08;
	u32 unknown_09_0 : 2;
	u32 mode : 2;
	u32 unknown_09_4 : 4;
};

struct Effect05Object
{
	u8					   reserved_00[8];
	s32					   x, y, z;
	s32					   altitude; // 落ちて止まる高さ
	s32					   scale_x, scale_y;
	u8					   reserved_20[8];
	s32					   velocity_y;
	u8					   reserved_2c[4];
	s32					   speed;
	u8					   reserved_34[0x1c];
	struct Effect05Sprite *sprite;
	u8					   reserved_54;
	u8					   flag;
	u8					   reserved_56[8];
	s16					   phase;
	u8					   reserved_60[0xc];
	void				  *callback; // 毎フレームの処理
};

struct Effect05State
{
	s32					   angle;
	s32					   x, y, z; // 玉の行き先の足もと
	struct Effect05Target *target;
	s32					   initialized;
	u8					   reserved_18[8];
	u8					   high_arc;
	u8					   reserved_21[0x13];
	u8					   variant;
	u8					   reserved_35[0x10];
	u8					   alternate;
};

//--------------------------------------------------------------------------
// 毎フレームの処理 : 落ちる粒が altitude に着いたら、小さな粒を 3 つ散らす
//--------------------------------------------------------------------------
void BattleFx_SpawnRandomAngleTriplet(struct Effect05Object *obj)
{
	s32					   i;
	struct Effect05Object *p;
	s32					   phase = 2;
	s32					   phase2;
	s16					  *pp;

	if (obj->y <= obj->altitude)
	{
		obj->phase = phase;
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
		p = NULL;
		obj->callback = p;

		// 半分の大きさで、上へ 1 ドットの速さで跳ね、32 ドット先のでたらめな方向へ
		for (i = 0; i <= 2; i++)
		{
			p = Object_Spawn(0xf0, obj->x, obj->y, obj->z);
			if (p == NULL)
			{
				break;
			}
			p->scale_y = 0x8000;
			p->scale_x = 0x8000;
			p->flag = 2;
			p->velocity_y = 1 << 16;
			p->speed = Random16() + 0x13333;
			Motion_SetTargetPositionFromMagnitudeAngle(
				(struct ObjectRuntime *)p, 32 << 16, Random16());
			pp = &p->phase;
			phase2 = 6;
			*pp = phase2;
			ObjectDispatch_InitializeFar((struct DispatchObject *)p, BattleFx_CommonParticleScript);
		}
	}
}

//--------------------------------------------------------------------------
// 毎フレームの処理 : 左右にゆれながらゆっくり落ち、地面に着いたら消える
//--------------------------------------------------------------------------
void BattleFx_UpdateDriftingFallObject(struct ObjectRuntime *obj)
{
	s32 r;

	obj->y -= 0x4ccc;
	r = Random16();
	obj->x += r - Random16();
	if (obj->y <= obj->terrain_height)
	{
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
	}
}

void BattleFx_CallEffect05(void)
{
	RunBattleEffect05();
}

//--------------------------------------------------------------------------
// 0〜10 段で start から end へ
//--------------------------------------------------------------------------
static __inline__ s32 Lerp10(s32 start, s32 end, s32 step)
{
	return start + step * (end - start) / 10;
}

//--------------------------------------------------------------------------
// 光の玉を対象の 16 ドット上から行き先の 32 ドット上 (variant なら 80 ドット上) へ
// 10 フレームで運びながら大きくし、粒を降らせてから戻す
// 行き先は、まだ決まっていなければ対象から angle の向きへ 16 ドット
//--------------------------------------------------------------------------
void RunBattleEffect05(void)
{
	void						 Animation_ApplyChildValuesFar(void *, s32);
	void						 ObjectDispatch_ReleaseFar(void *);
	void						 Vector_AddPolarOffset(s32, s32, void *);
	struct Effect05Point   pos;
	struct Effect05Point   start;
	struct Effect05Point   end;
	struct Effect05State  *work = gEffectWork;
	struct Effect05Target *target = work->target;
	struct Effect05Object *orb;
	struct Effect05Point  *from;
	struct Effect05Point  *to;
	s32					   i;
	s32					   steps = 11;
	s32					   count;
	s32					   y;
	struct Effect05Object *mote;

	orb = Object_Spawn(0xef, i = 0, 0, 0);
	if (orb == 0)
		return;
	BattleEffect_InitializeSharedScene();
	AudioCommand_PlayFar(138);
	if (work->initialized == 0)
	{
		work->x = target->position.x;
		work->z = target->position.z;
		Vector_AddPolarOffset(16 << 16, work->angle, (struct Effect05Point *)&work->x);
		work->y = Map_GetTerrainHeightFar(0, work->x, work->z);
	}
	from = &start;
	from->x = target->position.x;
	from->y = target->position.y + (16 << 16);
	from->z = target->position.z;
	to = &end;
	to->x = work->x;
	y = work->y;
	to->y = y + (32 << 16);
	to->z = work->z;
	if ((s8)work->variant != 0)
		to->y = y + (80 << 16);

	// 運びながら 1/4 から等倍まで大きくする
	for (; i < steps; i++)
	{
		s32 scale;

		{
			struct Effect05Point *a = &start;
			struct Effect05Point *b = &end;
			s32					  e, t;

			e = b->x;
			t = a->x;
			t += i * (e - t) / 10;
			orb->x = t;
			e = b->y;
			t = a->y;
			t += i * (e - t) / 10;
			orb->y = t;
			e = b->z;
			t = a->z;
			t += i * (e - t) / 10;
			orb->z = t;
		}
		scale = Lerp10(0x4000, 0x10000, i);
		orb->scale_x = scale;
		orb->scale_y = scale;
		WaitFrames(1);
	}
	WaitFrames(10);

	if ((s8)work->alternate == 0)
	{
		// 玉のまわり 3〜8 ドットから粒を 6 フレームおきに落とす (最後は玉の真下)
		count = 10;
		if ((s8)work->high_arc == 0)
			count = 24;
		for (i = 0; i < count; i++)
		{
			pos.x = orb->x;
			pos.y = orb->y;
			pos.z = orb->z;
			Vector_AddPolarOffset(Random16() * 5 + (3 << 16), Random16(), &pos);
			if (i == count - 1)
			{
				WaitFrames(25);
				pos.x = orb->x;
				pos.y = orb->y;
				pos.z = orb->z;
			}
			mote = Object_Spawn(0xf0, pos.x, pos.y, pos.z);
			if (mote != 0)
			{
				mote->altitude = pos.y - (32 << 16);
				mote->callback = (void *)BattleFx_SpawnRandomAngleTriplet;
				mote->flag = 2;
			}
			AudioCommand_PlayFar(132);
			WaitFrames(6);
		}
		WaitFrames(10);
	}
	else
	{
		// 玉のまわり 3〜8 ドットから、ゆれて落ちる粒を 6 フレームおきに出す
		if ((s8)work->high_arc != 0)
			count = 10;
		else
			count = 30;
		for (i = 0; i < count; i++)
		{
			pos.x = orb->x;
			pos.y = orb->y;
			pos.z = orb->z;
			Vector_AddPolarOffset(Random16() * 5 + (3 << 16), Random16(), &pos);
			mote = Object_Spawn(0x11c, pos.x, pos.y, pos.z);
			if (mote != 0)
			{
				mote->callback = (void *)BattleFx_UpdateDriftingFallObject;
				mote->flag = 0;
				mote->sprite->mode = 2;
				ObjectDispatch_ApplyArgumentToChildrenFar((struct ObjectRuntime *)mote, 8);
				Animation_ApplyChildValuesFar(mote, 7);
			}
			WaitFrames(6);
		}
		WaitFrames(70);
	}

	// 等倍から 1/4 まで縮めながら、対象の上へ戻す
	i = 0;
	do
	{
		s32 scale;

		{
			struct Effect05Point *a = &end;
			struct Effect05Point *b = &start;
			s32					  e, t;

			e = b->x;
			t = a->x;
			t += i * (e - t) / 10;
			orb->x = t;
			e = b->y;
			t = a->y;
			t += i * (e - t) / 10;
			orb->y = t;
			e = b->z;
			t = a->z;
			t += i * (e - t) / 10;
			orb->z = t;
		}
		scale = Lerp10(0x10000, 0x4000, i);
		orb->scale_x = scale;
		orb->scale_y = scale;
		WaitFrames(1);
		i++;
	} while (i < steps);
	ObjectDispatch_ReleaseFar(orb);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------

// particles.c

// バトル効果：持ち上げた物のまわりに落ちる粒

//--------------------------------------------------------------------------

struct FallPos
{
	s32 x;
	s32 y;
	s32 z;
};

struct FallSource
{
	u8	reserved_00[8];
	s32 x; // 16.16 ドット
	s32 y;
	s32 z;
};

struct FallScene
{
	u8				   reserved_00[20];
	struct FallSource *child; // 持ち上げている物
};

struct FallParticle
{
	u8	reserved_00[72];
	s32 gravity; // FieldActor と同じ位置
	u8	reserved_4c[9];
	u8	motion_flags; // FieldActor と同じ位置
	u8	reserved_56[8];
	u16 unknown_5e;
};

LAYOUT_SIZE_GUARD(FallPos_Size, struct FallPos, 0x0c);
LAYOUT_OFFSET_GUARD(FallSource_X, struct FallSource, x, 0x08);
LAYOUT_OFFSET_GUARD(FallScene_Child, struct FallScene, child, 0x14);
LAYOUT_OFFSET_GUARD(FallParticle_Gravity, struct FallParticle, gravity, 0x48);
LAYOUT_OFFSET_GUARD(FallParticle_MotionFlags, struct FallParticle, motion_flags, 0x55);
LAYOUT_OFFSET_GUARD(FallParticle_Unknown5e, struct FallParticle, unknown_5e, 0x5e);

//--------------------------------------------------------------------------
// 毎フレームの処理：持ち上げている物のまわりに、落ちる粒をひとつ出す
// 高さは物から 8〜24 ドット上、横はでたらめな向きに 0〜48 ドット
//--------------------------------------------------------------------------
void BattleFx_SpawnFallingParticles(void)
{
	void				 Vector_AddPolarOffset(s32, s32, void *);
	struct FallSource	*src;
	struct FallPos		 pos;
	struct FallParticle *obj;
	u32					 dist;

	src = ((struct FallScene *)gEffectWork)->child;
	pos.x = src->x;
	pos.y = src->y - Random16() * 16 + (24 << 16);
	pos.z = src->z;

	dist = Random16() * 3;
	Vector_AddPolarOffset(dist * 16, Random16(), &pos);

	obj = Object_Spawn(0x11d, pos.x, pos.y, pos.z);
	if (obj != 0)
	{
		obj->motion_flags = ACTOR_FALLS;
		obj->gravity = 0x1999; // 重力 0.1 ドット
		ObjectDispatch_ApplyArgumentToChildrenFar((struct FieldActor *)obj, 0);
		obj->unknown_5e = 12;
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
	}
}

//--------------------------------------------------------------------------

// lift_drop.c

// 対象を 2 つの持ち手で持ち上げ、方向キーで動かして落とす

//--------------------------------------------------------------------------

#define Random16 ((s32 (*)(void))Random16)

#define Motion_SetTargetPositionFromMagnitudeAngle \
	((void (*)(struct FxObject *, s32, s32))Motion_SetTargetPositionFromMagnitudeAngle)

#define BattleFx_SpawnItemBreakMode1 \
	((struct FxObject * (*)(s32, s32, s32, s32)) BattleFx_SpawnItemBreakMode1)
#define Object_SetMoveTargetFar \
	((void (*)(struct FxObject *, s32, s32, s32))Object_SetMoveTargetFar)
#define ObjectGroup_ApplyRandomChildValues ((void (*)(void))ObjectGroup_ApplyRandomChildValues)
#define BattleFx_GetCycledTableWord ((s32 (*)(u32))BattleFx_GetCycledTableWord)
#define ScriptObject_CheckOverlapFar \
	((s32 (*)(struct FxObject *, struct FxPosition *))ScriptObject_CheckOverlapFar)
#define BattleFx_SpawnRadialParticleRing \
	((void (*)(struct FxObject *))BattleFx_SpawnRadialParticleRing)

struct FxPosition
{
	s32 x; // 16.16 固定小数
	s32 y;
	s32 z;
};

struct FxObject
{
	u8				 unknown_00[8];
	s32				 x; // 座標 (16.16 固定小数、y が高さ)
	s32				 y;
	s32				 z;
	s32				 ground; // 足もとの地面の高さ
	u8				 unknown_18[10];
	u8				 mode;
	u8				 unknown_23;
	s32				 velocity_x;
	s32				 velocity_y;
	s32				 velocity_z;
	s32				 speed;
	s32				 lift;
	u8				 unknown_38[12];
	s32				 gravity;
	u8				 unknown_48[13];
	u8				 flag;
	u8				 unknown_56[4];
	u8				 unknown_5a;
	u8				 unknown_5b;
	u8				 unknown_5c[12];
	struct FxObject *linked;
	void			*callback;
};
extern const s32			 BattleFx_FragmentScript[];
extern volatile u32			 gFrameCount;

void			 WaitFrames(s32 frames);
void			 AudioCommand_PlayFar(s32 cue);
void			 ObjectDispatch_SetSingleChildFlagsFar(struct FxObject *object, s32 value);
s32				 Map_CheckObjectStepFar(struct FxObject *object, struct FxPosition *position);

#define SHARD_MAX 20 // 破片の数

//--------------------------------------------------------------------------
// 足もとを 16 ドット上げて、pos へ動いたときにぶつかるかを調べる
//--------------------------------------------------------------------------
static __inline__ s32 ObjProbeUp(struct FxObject *obj, struct FxPosition *pos)
{
	s32 hit;

	obj->ground += 16 << 16;
	hit = Map_CheckObjectStepFar(obj, pos);
	obj->ground -= 16 << 16;
	return hit;
}

//--------------------------------------------------------------------------
// 対象を左右の持ち手で 24 ドットの高さまで持ち上げ、方向キーの向きへ
// 16 ドット動かして落とす (A・B・L・R ならその場で落とす)
// 行き先がふさがっていれば、ブザーを鳴らして待つ
//--------------------------------------------------------------------------
void BattleFx_SteerLiftedTarget(s32 id)
{
	void			 Animation_ApplyChildValuesFar(void *object, s32 value);
	void			 ObjectDispatch_ReleaseFar(void *object);
	void			 Script_WaitForEventTimeoutFar(void *object);
	void			 Vector_AddPolarOffset(s32 magnitude, s32 angle, void *position);
	struct BattleFxScene *work;
	struct FxObject		 *src;
	struct FxObject		 *target;
	struct FxObject		 *shards[SHARD_MAX];
	struct FxPosition	  pos;
	struct FxObject		 *hands[2];
	struct FxObject		 *obj;
	struct FxObject		 *right;
	s32					  i;
	s32					  angle;
	s32					  dist;
	struct FxPosition	 *p;

	work = gEffectWork;
	src = work->main_object;
	target = work->child;
	if (target == NULL)
		return;
	BattleEffect_InitializeSharedScene();
	src->linked = target;
	ObjectDispatch_InitializeFar((struct DispatchObject *)src, BattleFx_SourceHoldScript);

	// 持ち手を左右 32 ドットに出し、対象の左右 16 ドットへ動かす
	/* FAKEMATCH: needed to match: the position is reached through this pointer everywhere but in the steering loop's first part, which names the position itself */
	p = &pos;
	p->x = work->x;
	p->y = work->y + (16 << 16);
	p->z = work->z;
	obj = BattleFx_SpawnItemBreakMode1(p->x + (32 << 16), p->y, p->z, 0x8000);
	hands[0] = obj;
	right = BattleFx_SpawnItemBreakMode1(p->x - (32 << 16), p->y, p->z, 0);
	hands[1] = right;
	if (hands[0] == NULL || right == NULL)
	{
		BattleFx_PrepareBufferInterpolation();
		return;
	}
	WaitFrames(15);
	p->x = target->x;
	p->y = target->y + (16 << 16);
	p->z = target->z;
	Object_SetMoveTargetFar(obj, p->x + (16 << 16), p->y, p->z);
	Object_SetMoveTargetFar(right, p->x - (16 << 16), p->y, p->z);
	Script_WaitForEventTimeoutFar(obj);
	Script_WaitForEventTimeoutFar(right);
	obj->x = p->x + (16 << 16);
	obj->velocity_x = 0;
	right->x = p->x - (16 << 16);
	right->velocity_x = 0;

	// 地面から 24 ドットの高さまで持ち上げる
	target->callback = ObjectGroup_ApplyRandomChildValues;
	Scheduler_AddOrUpdateCallback(BattleFx_SpawnFallingParticles, 0xc80);
	AudioCommand_PlayFar(130);
	target->flag = 4;
	ObjectDispatch_SetSingleChildFlagsFar(target, 0);
	if (hands[0] != NULL && hands[1] != NULL)
	{
		while (target->y - target->ground <= 24 << 16)
		{
			obj->y += 0x6000;
			right->y += 0x6000;
			target->y += 0x6000;
			WaitFrames(1);
		}
	}
	hands[0]->speed = 4 << 16;
	hands[0]->lift = 0x8000;
	hands[1]->speed = 4 << 16;
	hands[1]->lift = 0x8000;
	target->speed = 0x6666;
	target->lift = 0x3333;
	target->unknown_5a = 0;
	target->mode = 2;

	//------------------------------------------------------------------
	// 方向キーで動かす
	//------------------------------------------------------------------
	for (;;)
	{
		WaitFrames(1);
		if ((gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) != 0)
			break;

		// 方向キーなし : 持ち手を対象の左右に戻す
		angle = (u16)BattleFx_GetCycledTableWord(gKeysHeld);
		if (angle == 0xffff)
		{
			pos.x = target->x;
			pos.y = target->y + (16 << 16);
			pos.z = target->z;
			Object_SetMoveTargetFar(hands[0], pos.x + (16 << 16), pos.y, pos.z);
			Object_SetMoveTargetFar(hands[1], pos.x - (16 << 16), pos.y, pos.z);
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 1);
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 1);
			continue;
		}

		// 持ち手をその向きへ 2 ドット寄せ、16 ドット先の地面を調べる
		pos.x = target->x;
		pos.y = target->y + (16 << 16);
		pos.z = target->z;
		Vector_AddPolarOffset(2 << 16, angle, &pos);
		Object_SetMoveTargetFar(hands[0], pos.x + (16 << 16), pos.y, pos.z);
		Object_SetMoveTargetFar(hands[1], pos.x - (16 << 16), pos.y, pos.z);
		Script_WaitForEventTimeoutFar(hands[0]);
		Script_WaitForEventTimeoutFar(hands[1]);
		pos.x = target->x;
		pos.y = target->ground;
		pos.z = target->z;
		Vector_AddPolarOffset(16 << 16, angle, &pos);
		i = ScriptObject_CheckOverlapFar(target, &pos);
		/* FAKEMATCH: needed to match: entering the blocked path from both tests keeps it ahead of the drop path, as the ROM lays them out */
		if (i != 0)
			goto blocked;
		if (ObjProbeUp(target, p) > 0)
		{
		blocked: // ふさがっている : 16 フレームごとにブザー
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 4);
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 4);
			if ((gFrameCount & 15) == 0)
				AudioCommand_PlayFar(SOUND_MENU_ERROR);
			continue;
		}
		// 空いている : 対象をそこへ動かす
		{
			struct FxObject *second;
			s32				 x;
			s32				 z;

			AudioCommand_PlayFar(175);
			x = p->x;
			z = p->z;
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 4);
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 4);
			WaitFrames(15);
			target->unknown_5b = i;
			target->speed = 0x3333;
			target->lift = 0x3333;
			Object_SetMoveTargetFar(target, p->x, p->y, p->z);
			hands[0]->speed = 0x3333;
			hands[0]->lift = 0x3333;
			second = hands[1];
			second->speed = 0x3333;
			second->lift = 0x3333;
			Object_SetMoveTargetFar(hands[0], p->x + (16 << 16), p->y, p->z);
			Object_SetMoveTargetFar(second, p->x - (16 << 16), p->y, p->z);
			Script_WaitForEventTimeoutFar(target);
			target->x = x;
			target->z = z;
			target->velocity_x = i;
			target->velocity_z = i;
			WaitFrames(10);
			break;
		}
	}

	//------------------------------------------------------------------
	// 対象の位置から破片を飛ばして持ち手を消し、対象を跳ねさせて落とす
	//------------------------------------------------------------------
	ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 4);
	ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 4);
	Scheduler_RemoveCallback(BattleFx_SpawnFallingParticles);
	AudioCommand_PlayFar(135);
	WaitFrames(15);
	AudioCommand_PlayFar(135);
	WaitFrames(15);
	p->x = target->x;
	p->y = target->y + (16 << 16);
	p->z = target->z;
	{
		struct FxObject **walk = shards;

		// 破片 : 速さ 2〜3 ドット、8〜32 ドット先のでたらめな方向へ
		for (i = 0; i < SHARD_MAX; i++)
		{
			obj = Object_Spawn(0x11d, p->x, p->y, p->z);
			*walk++ = obj;
			if (obj != NULL)
			{
				ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_FragmentScript);
				obj->speed = Random16() + (2 << 16);
				obj->lift = 2 << 16;
				obj->flag = 0;
				dist = Random16() * 24 + (8 << 16);
				Motion_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());
			}
		}
	}
	AudioCommand_PlayFar(131);
	ObjectDispatch_ReleaseFar(hands[0]);
	ObjectDispatch_ReleaseFar(hands[1]);

	// 対象の見た目と動きを戻し、上へ 10 ドットの速さで跳ねさせる
	Animation_ApplyChildValuesFar(target, work->saved_palette);
	ObjectDispatch_InitializeFar((struct DispatchObject *)target, work->saved_script);
	target->callback = work->saved_callback;
	target->flag = 3;
	target->velocity_y = 10 << 16;
	target->gravity = 0x3333;
	target->mode = 0;
	src->callback = 0;
	Animation_ApplyChildValuesFar(src, 0);
	if (work->child_option != 0)
	{
		// 上がりきって落ちきるまで (それぞれ最長 90 フレーム) 待ち、輪を出す
		for (i = 0; i < 90 && target->velocity_y >= 0; i++)
			WaitFrames(1);
		WaitFrames(1);
		for (i = 0; i < 90 && target->velocity_y < 0; i++)
			WaitFrames(1);
		BattleFx_SpawnRadialParticleRing(target);
		BattleFx_PrepareBufferInterpolation();
		WaitFrames(30);
	}
	else
	{
		BattleFx_PrepareBufferInterpolation();
	}
}

#undef BattleFx_SpawnItemBreakMode1
#undef Motion_SetTargetPositionFromMagnitudeAngle
#undef Random16

#define Motion_SetTargetPositionFromMagnitudeAngle \
	((void (*)(void *, s32, s32))Motion_SetTargetPositionFromMagnitudeAngle)
void						 AudioCommand_PlayFar(s32 sound);

void RunBattleEffect11(void)
{
	void						 ObjectDispatch_ReleaseFar(void *object);
	void						 Script_WaitForEventTimeoutFar(void *object);
	struct BattleFxScene  *scene;
	struct MotionObject	  *obj;
	struct MotionObject	  *anchors[2];
	struct ScatterPosition position;
	struct MotionObject	 **walk;
	s32					   index;
	s32					   magnitude;

	scene = gEffectWork;
	BattleEffect_InitializeSharedScene();
	position.x = scene->x;
	position.y = scene->y + 0x100000;
	position.z = scene->z;
	anchors[0] =
		BattleFx_SpawnItemBreakMode1(position.x + 0x200000, position.y, position.z, 0x8000);
	anchors[1] = BattleFx_SpawnItemBreakMode1(position.x - 0x200000, position.y, position.z, 0);

	WaitFrames(15);
	walk = anchors;
	for (index = 0; index < 2; index++)
	{
		obj = *walk++;
		if (obj != 0)
			Motion_SetTargetPositionFromMagnitudeAngle(obj, 0x180000, obj->angle);
	}

	Script_WaitForEventTimeoutFar(anchors[0]);
	AudioCommand_PlayFar(134);
	for (index = 0; index < 24; index++)
	{
		position.x = scene->x;
		position.y = scene->y + 0x100000;
		position.z = scene->z;
		obj = Object_Spawn(0x11d, position.x, position.y, position.z);
		if (obj != 0)
		{
			ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_FragmentScript);
			obj->speed_limit = Random16() + 0x20000;
			obj->acceleration = 0x20000;
			obj->motion_flags = 0;
			magnitude = Random16() * 24 + 0x80000;
			Motion_SetTargetPositionFromMagnitudeAngle(obj, magnitude, Random16());
		}
	}

	ObjectDispatch_ReleaseFar(anchors[0]);
	ObjectDispatch_ReleaseFar(anchors[1]);
	BattleFx_PrepareBufferInterpolation();
}

struct MotionObject *BattleFx_SpawnItemBreakMode1(s32 x, s32 y, s32 z, s32 angle)
{
	struct MotionObject *obj;
	s32					 scale;

	AudioCommand_PlayFar(SOUND_ITEM_BREAK);
	obj = Object_Spawn(215, x, y, z);
	if (obj != NULL)
	{
		obj->scale_y = 0x4000;
		obj->scale_x = 0x4000;
		obj->speed_limit = 0x30000;
		obj->acceleration = 0x30000;
		obj->auto_face_motion = 0;
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
		scale = obj->scale_x;
		if (scale < 0x10000)
		{
			do
			{
				scale += 0x800;
				obj->scale_y = scale;
				obj->scale_x = scale;
				obj->angle += 0x2000;
				WaitFrames(1);
				scale = obj->scale_x;
			} while (scale <= 0xffff);
		}
		obj->angle = angle;
	}
	return obj;
}

void BattleFx_UpdateObjectMotionScaleAndLinkedAngle(union FieldObject *object)
{
	object->effect.x += object->effect.velocity_x;
	object->effect.y += object->effect.velocity_y;
	object->effect.z += object->effect.velocity_z;
	object->effect.scale_x += object->effect.scale_rate_x;
	object->effect.scale_y += object->effect.scale_rate_y;
	object->effect.sprite->rotation += object->effect.spin;
}
