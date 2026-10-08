//--------------------------------------------------------------------------
// ORB.C
// 姿を隠すあいだの足もとの確認と、戦闘エフェクト 5 番 (光の玉と降る粒)
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
#include "SCRIPT_MOTION.H"
#include "ANIMSPR.H"
#include "EVENT_RUNTIME.H"
#include "OBJECT_RUNTIME.H"
#include "FIELDOBJ.H"
#include "OBJECT_LOOKUP.H"
#include "OBJDISP.H"
#include "TYPES.H"
#include "SCENE.H"
#include "MAP.H"
#include "GAME_STATE.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIXED_MATH.H"
#include "OBJECT_EFX.H"

extern u8 gMapCellBuffer[];

void Audio_PlayCue(s32);
void Object_SetMode(struct ObjectRuntime *, s32);

void *ObjectTable_Get(u32);

void *Object_Spawn(s32, s32, s32, s32);

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

	slot = (u32)&gWork;
	ev = gWork;
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

	Audio_PlayCue(154);
	Scheduler_RemoveCallback(CheckObjectMapTile);
	Object_SetMode(obj, 0);
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

	Audio_PlayCue(130);
	Object_SetMode(obj, 0);
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

extern struct Effect05State *gEffectWork;
void						 Vector_AddPolarOffset(s32, s32, struct Effect05Point *);
void						 ObjectDispatch_ReleaseFar(struct Effect05Object *);
void						 Animation_ApplyChildValuesFar(struct Effect05Object *, s32);
void						 AudioCommand_PlayFar(s32);

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
				Object_SetMode((struct ObjectRuntime *)mote, 8);
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
