#define FIELD_STAGED_ACTOR_IMPORTS
#include "fieldobj.h"
#include "types.h"
#include "text/msg_ids.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_effect.h"
#include "staged_actor.h"
#include "spawn_configured_effect.h"
#include "edition.h"
#include "scene_ids.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "call.h"

void Effect_AdvanceMotion();
s32		   OverlayObject_SetYAboveLinkedActor();
s32 SceneActor_SetFlagBitByRelativeDepth();

struct Vec
{
	s32 x;
	s32 y;
	s32 z;
};

struct EffectParams
{
	s32 unk00;
	s32 mode;
	u8	filler08[0x1c];
	s32 callback;
};

struct Slot02000f10
{
	u8	head[8];
	s32 x;
	s32 y;
	s32 rank;
};

typedef struct Actor_02000cd0
{
	u8	pad0[6];
	u16 tag;
	s32 x;
	s32 y;
	s32 z;
	u8	pad20[20];
	s32 speedZ;
	s32 pad44;
	s32 speedX;
	s32 speedY;
	u8	pad56[29];
	u8	flags;
} Actor_02000cd0;

extern const s32 BabiChika_FlickerScript[];

typedef struct Target_02000cd0
{
	union FieldCoordinate x;
	s32					  pad4;
	union FieldCoordinate z;
} Target_02000cd0;

typedef struct Record
{
	s32 pad0;
	s32 pad4;
	s32 w8;
	s32 w12;
	s32 w16;
} Record;

struct Actor_02000ec8
{
	u8	unknown_00[8];
	s32 x;
	s32 y;
	s32 z;
	u8	unknown_14[0xf];
	u8	flatla3;
};

struct Track02001038
{
	u8	head[12];
	s32 value;
	u8	gap16[4];
	s32 limit;
	u8	gap24[16];
	s32 state;
	u8	gap44[16];
	s32 mark;
};

struct MotionEffect
{
	u8			   pad00[8];
	s32			   position[3];
	u8			   pad14[4];
	s32			   accum18;
	s32			   accum1c;
	u8			   pad20[3];
	u8			   flatla3;
	u8			   pad24[12];
	s32			   rate30;
	s32			   rate34;
	u8			   pad38[12];
	s32			   velocity[3];
	struct Sprite *sprite;
	u8			   pad54;
	u8			   mode55;
	u8			   pad56[14];
	u16			   step64;
	u8			   pad66[6];
	u32			   callback;
};

extern u8 Data_0200b8f4[];
extern u8 Data_0200ba74[];
extern u8 Data_0200bc0c[];
extern u8 Data_0200bef4[];
s32	 Object_CheckMovementCollision();
s32	 Vector_AddPolarOffset();
s32	 Battle_GetWorkObject1e0();
void BattleFx_RunRisingObjectSequence(s32, s32, s32);
struct Record_02000ec8;

s32 SceneActor_SetFlagBitByRelativeDepth();

void SceneEffect_SpawnNineRadialEffects(s32 actor);
void Effect_AdvanceMotion();

extern u8  Data_0200b85c[];
extern u16 Data_0200b3ec[];
extern u16 Data_0200b40c[];
s32		   OverlayObject_SetYAboveLinkedActor();

void OverlayObject_SetOwnerMode(u8 *object, s32 mode)
{
	struct FieldActor *actor = (struct FieldActor *)object;

	actor->sprite->priority = mode;
}

u8 *OverlayObject_CreateAndInitialize(s32 x, s32 y, s32 z, s32 kind)
{
	struct FieldActor *ret;
	struct FieldActor *obj = Engine_ObjectCreate(kind, x, y, z);

	if (obj != 0)
	{
		struct FieldSprite *owner = obj->sprite;
		owner->priority = 0;
		obj->motion_flags = 0;
		obj->collision_flags = 8;
		Engine_ActorSetSpriteFlags(obj, 0);
		ObjectGroup_SetChildValue(obj, 14);
		Engine_ObjectSetBlendMode(obj, 1);
		ret = obj;
	}
	else
	{
		ret = 0;
	}
	return (u8 *)ret;
}

u8 *OverlayObject_PrepareSpawnedObjectMode4(s32 x, s32 y, s32 z, s32 kind)
{
	struct FieldActor *ret;
	struct FieldActor *obj = Engine_ObjectCreate(kind, x, y, z);

	if (obj != 0)
	{
		struct FieldSprite *rec = obj->sprite;
		rec->priority = 1;
		obj->motion_flags = 0;
		obj->collision_flags = 8;
		Engine_ActorSetSpriteFlags(obj, 0);
		ObjectGroup_SetChildValue(obj, 15);
		obj->priority_flags = (obj->priority_flags & 0xfe) | 2;
		ret = obj;
	}
	else
	{
		ret = 0;
	}
	return (u8 *)ret;
}

s32 SceneActor_CopyActor8PositionWhenAtRow10(Record *record);

#define FIELD_STAGED_ACTOR_IMPORTS
extern u8 *gActorEffectWork;

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/effect/move.c"
#include "../common/effect/spawnov.c"

s32 SceneState_ApplyArgMode0AndReturnZero(s32 no)
{
	Engine_ActorSetSpriteFlags(no, 0);
	return 0;
}

s32 SceneActor_MoveActorZeroToTarget(const Target_02000cd0 *target)
{
	Actor_02000cd0 *actor = Object_GetById(ACTOR_PARTY_LEADER);
	u8				saved = actor->flags;
	s32				probe[3];

	probe[0] = (actor->x & 0xfff00000) + 0x00080000;
	probe[1] = actor->y;
	probe[2] = (actor->z & 0xfff00000) + 0x00080000;

	Vector_AddPolarOffset(0x00100000, (actor->tag + 0x2000) & 0xc000, probe);

	if (Object_CheckMovementCollision(actor, probe) == 1)
	{
		goto refuse;
	}
	if (Object_CheckMovementCollision(actor, target) != 0)
	{
		goto refuse;
	}

	Engine_EventBegin();
	Object_SetMode(actor, 6);
	Engine_TaskWait(6);
	Engine_AudioPlayCue(152);
	Object_SetMode(actor, 7);

	actor->speedX = 0x00030000;
	actor->speedY = 0x00020000;
	actor->speedZ = 0x00040000;
	actor->flags &= 0x7e;

	Engine_ActorSetSpriteFlags(actor, 0);
#if EDITION_INTERNATIONAL
	Engine_ActorMoveToAndWait(
		ACTOR_PARTY_LEADER, ((target->x.fixed >> 20) << 4) + 8, ((target->z.fixed >> 20) << 4) + 8);
#else
	Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, target->x.part.pixel, target->z.part.pixel);
#endif
	Object_SetMode(actor, 6);
	Engine_ActorSetSpriteFlags(actor, 1);
	Engine_TaskWait(6);

	actor->flags = saved;
	Engine_EventEnd();
	return 0;

refuse:
	return 1;
}

void SceneActor_PassRaisedPointOfActorZero(void)
{
	s32				   pos[3];
	struct FieldActor *p = Object_GetById(ACTOR_PARTY_LEADER);

	pos[0] = p->x.fixed;
	pos[1] = p->y.fixed;
	pos[2] = p->z.fixed + 0x200000;
	SceneActor_MoveActorZeroToTarget(pos);
}

void SceneActor_PassActorZeroOffsetPoint(void)
{
	s32				   pos[3];
	struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

	pos[0] = actor->x.fixed;
	pos[1] = actor->y.fixed;
	pos[2] = actor->z.fixed + 0xFFE00000;
	SceneActor_MoveActorZeroToTarget(pos);
}

void BabiChika_UpdateTrackedActor(void)
{
	struct FieldActor *actor;
	u8				  *work;
	s32				   limit;

	actor = Object_GetById(0);
	work = gActorEffectWork;
	limit = 0;
	if (gGameState.scene == (s32)&SceneId_BabiChika1)
	{
		switch (gGameState.entrance)
		{
		case 3:
		case 4:
			limit = 94;
			break;
		case 8:
		case 9:
			limit = 74;
			break;
		case 12:
		case 13:
			limit = 118;
			break;
		}
	}
	else if (gGameState.entrance == 12)
	{
		limit = 93;
	}
	if ((actor->z.fixed >> 19) <= limit)
	{
		*(s32 *)(work + 24) = 0;
	}
	else
	{
		*(s32 *)(work + 24) = (s32)actor;
	}
}

s32 SceneActor_SetFlagBitByRelativeDepth(struct Actor_02000ec8 *actor)
{
	struct Actor_02000ec8 *ref;
	u8					  *fp;
	u8					   flag;
	ref = Object_GetById(ACTOR_PARTY_LEADER);
	fp = &actor->flatla3;
	flag = *fp | 2;
	*fp = flag;
	if (ref->z < actor->z)
	{
		s32 lim = actor->z - ref->z;
		s32 ay;
		lim += 0x00040000;
		ay = actor->y;
		ay += lim;
		if (ref->y <= ay)
		{
			flag &= 0xfd;
			*fp = flag;
		}
	}
	return 0;
}

void SceneState_SwapSlotPairByRank(s32 first, s32 second)
{
	struct Slot02000f10 *a = Object_GetById(first);
	struct Slot02000f10 *b = Object_GetById(second);

	if (a->rank <= b->rank)
	{
		s32 t;

		t = a->x;
		a->x = b->x;
		b->x = t;
		t = a->y;
		a->y = b->y;
		b->y = t;
		t = a->rank;
		a->rank = b->rank;
		b->rank = t;
		Engine_TaskWait(1);
	}
}

s32 BabiChika_UpdateFlickerEffect(struct FieldActor *object)
{
	struct EffectOptions options;
	s32					 x;
	s32					 z;

	if (gFrameCount & 2)
		Object_SetMode(object, 1);
	else
		Object_SetMode(object, 2);
	if (gFrameCount & 3)
		return 0;
	options.start_scale_x = 0x4ccc;
	options.start_scale_y = 0x4ccc;
	options.palette = 5;
	x = object->x.fixed + ((((u32)(Engine_RandomNext() * 7) >> 16) - 3) << 16);
	z = object->z.fixed + ((((u32)(Engine_RandomNext() * 7) >> 16) - 3) << 16);
	Effect_Spawn(x, object->y.fixed + 0x100000, z, 0, 0, 0, 0x90001, &options);
	return 0;
}

s32 SceneActor_CopyActor8PositionWhenAtRow10(Record *record)
{
	Record *ref = Object_GetById(ACTOR_PARTY_LEADER);

	if (ref->w12 > -0x300000 && (Object_GetById(8)->z.fixed >> 20) == 10)
	{
		record->w8 = Object_GetById(8)->x.fixed;
		record->w12 = -0x200000;
		record->w16 = Object_GetById(8)->z.fixed;
	}
	else
	{
		record->w8 = 0;
		record->w12 = 0;
		record->w16 = 0;
	}
	return 0;
}

void SceneActor_WaitValueBelowLimit(struct Track02001038 *track)
{
	s32 cnt = 60;
	s32 limit;

	for (;;)
	{
		if (cnt != 0)
		{
			s32 value;

			Engine_TaskWait(1);
			value = track->value;
			limit = track->limit;
			cnt--;
			if (value <= limit)
			{
				break;
			}
			continue;
		}
		limit = track->limit;
		break;
	}
	track->state = 0;
	track->value = limit;
	track->mark = 0x80000000;
}

void Effect_AdvanceMotion(struct MotionEffect *effect)
{
	struct Sprite *sprite;

	effect->position[0] += effect->velocity[0];
	effect->position[1] += effect->velocity[1];
	effect->position[2] += effect->velocity[2];

	effect->velocity[0] -= effect->velocity[0] / 18;
	effect->velocity[2] -= effect->velocity[2] / 16;

	effect->accum18 += effect->rate30;
	effect->accum1c += effect->rate34;

	sprite = effect->sprite;
	sprite->angle += effect->step64;
}

void BabiChika_RunLeaderSpray(void)
{
	struct FieldActor	*leader = Object_GetById(0);
	struct EffectOptions options;
	u32					 i;

	Engine_EventBegin();
	Object_SetMode(leader, 6);
	ObjectMotion_WaitForAnimationChange(0);
	Object_SetMode(leader, 1);
	Engine_ActorSetSpriteFlags(leader, 0);
	leader->motion_flags |= 2;
	Engine_AudioPlayCue(152);
	leader->velocity_y = 0x40000;
	Engine_ObjectSetPosition(leader, leader->x.fixed, leader->y.fixed, leader->z.fixed + 0xc0000);
	Engine_TaskWait(6);
	leader->motion_flags = 0;
	options.update = (void (*)(union FieldObject *))Effect_AdvanceMotion;
	Engine_AudioPlayCue(127);
	for (i = 0; i < 8; i++)
	{
		leader->y.fixed -= 0x20000;
		leader->target_y = leader->y.fixed;
		Engine_TaskWait(1);
		if (i & 1)
		{
			s32 m = (u32)Engine_RandomNext() % 10 - 5;
			s32 vx = m * 0x3332;
			s32 vz;

			vz = (u32)Engine_RandomNext() % 10 * -0x1999 - 0x7ffd;
			Effect_Spawn(
				leader->x.fixed, leader->y.fixed, leader->z.fixed, vx, 0, vz, 0x1000001, &options);
		}
	}
	Engine_ActorSetSpriteFlags(leader, 1);
	leader->motion_flags = 3;
	Engine_EventEnd();
}

#define SceneActor_MoveActorZeroToTarget ((int (*)())SceneActor_MoveActorZeroToTarget)
#define SceneActor_PassActorZeroOffsetPoint ((int (*)())SceneActor_PassActorZeroOffsetPoint)
#define SceneActor_WaitValueBelowLimit ((int (*)())SceneActor_WaitValueBelowLimit)
#define SceneState_SwapSlotPairByRank ((int (*)())SceneState_SwapSlotPairByRank)

void BattleFx_StartFadeOverlay(s32 value);
#define ACTOR_UPDATE_IDLE ((void (*)(union FieldObject *))SceneActor_SetFlagBitByRelativeDepth)
#define ACTOR_UPDATE_PANEL ((void (*)(union FieldObject *))OverlayObject_SetYAboveLinkedActor)

void SceneEffect_SpawnNineRadialEffects(s32 actor)
{
	struct FieldActor  *object;
	struct Vec			vec;
	struct EffectParams params;
	u32					i;
	s32					v;
	s32					x;
	s32					z;

	object = Object_GetById(actor);
	params.unk00 = 1;
	params.mode = 7;
	params.callback = (s32)Effect_AdvanceMotion;
	for (i = 0; i <= 16; i += 2)
	{
		v = i << 12;
		vec.x = Engine_MathCos(v);
		vec.y = 0;
		z = Engine_MathSin(v);
		x = vec.x;
		vec.z = z;
		x = x + Math_Divide(x, 3);
		vec.x = x;
		((void (*)())Effect_Spawn)(
			object->x.fixed, object->y.fixed, object->z.fixed, x, vec.y, z, 0x01030001, &params);
	}
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 v = gGameState.scene;

	if (v == (s32)&SceneId_BabiChika1)
	{
		return gBabiChikaEntrances1;
	}
	if (v == (s32)&SceneId_BabiChika2)
	{
		return gBabiChikaEntrances2;
	}
	return gBabiChikaEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	if (gGameState.scene == (s32)&SceneId_BabiChika2)
	{
		return gBabiChikaRegions2;
	}
	return 0;
}

u8 *BabiChika_GetExits(void)
{
	return Data_0200b85c;
}

u8 *SceneData_SelectAndApplyTableBySceneId(void)
{
	u8 *tbl;

	if (gGameState.scene == (s32)&SceneId_BabiChika1)
	{
		tbl = Data_0200b8f4;
	}
	else
	{
		tbl = Data_0200ba74;
	}
	FieldScene_PrepareActors((struct SceneRegionEntry *)tbl);
	return tbl;
}

void SceneState_RunRect73x38Step(void)
{
	Engine_EventBegin();
	Map_CopyCellAttributes(73, 38, 5, 5, 9, 38);
	StagedActor_AdvancePair();
	FieldScene_PlaceAndPinSlots8And9();
	Engine_EventEnd();
}

void SceneActor_ApplyPointLeftOfActorZero(void)
{
	s32				   point[3];
	struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

	point[0] = actor->x.fixed + 0xFFE00000;
	point[1] = actor->y.fixed;
	point[2] = actor->z.fixed;
	SceneActor_MoveActorZeroToTarget(point);
}

void FieldScene_RunLayoutAt93By30(void)
{
	Engine_EventBegin();
	Map_CopyCellAttributes(93, 30, 6, 5, 29, 30);
	StagedActor_AdvancePair();
	FieldScene_PlaceAndPinSlots10And11();
	Engine_EventEnd();
}

void FieldScene_RunStepWith6(void)
{
	BattleFx_RunRisingObjectSequence(0, 6, 0);
}

void SceneActor_PassPointTwoRightOfActorZero(void)
{
	s32				   pos[3];
	struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

	pos[0] = actor->x.fixed + 0x200000;
	pos[1] = actor->y.fixed;
	pos[2] = actor->z.fixed;
	SceneActor_MoveActorZeroToTarget(pos);
}

void SceneState_ApplyTwoRectsAndRunThree(void)
{
	Engine_EventBegin();
	Engine_MapCopyCellAttributes(89, 49, 3, 2, 25, 49);
	Engine_MapCopyCellAttributes(89, 51, 8, 5, 25, 51);
	StagedActor_AdvancePair();
	BabiChika_RestorePlatformCells();
	Engine_EventEnd();
}

void SceneActor_CheckTwoUnitsAboveActorZero(void)
{
	struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
	s32				   target[3];

	target[0] = actor->x.fixed;
	target[1] = actor->y.fixed;
	target[2] = actor->z.fixed + 0x00200000;
	if (SceneActor_MoveActorZeroToTarget(target) != 0)
	{
		SceneState_ApplyTwoRectsAndRunThree();
	}
}

void SceneActor_MirrorFlag201IntoSlot14(void)
{
	u8 *flags;
	u8	value;

	Engine_GameFlagSet(0x200);
	if (Engine_GameFlagIsSet(0x201) != 0)
	{
		Object_GetById(14)->rise_counter = 0;
		Object_GetById(14)->collision_flags &= 0xf7;
	}
	else
	{
		Object_GetById(14)->rise_counter = 1;
		flags = Object_GetById(14);
		flags += 89;
		value = 8;
		value |= *flags;
		*flags = value;
	}
}

void BabiChika_ConfigureActorFourteenRise(void)
{
	u8 *p;
	u8	val;

	Engine_GameFlagSet(0x201);
	if (Engine_GameFlagIsSet(0x200) != 0)
	{
		Object_GetById(14)->rise_counter = 0;
		Object_GetById(14)->collision_flags &= 0xf7;
	}
	else
	{
		Object_GetById(14)->rise_counter = 1;
		p = Object_GetById(14);
		p += 89;
		val = 8;
		val |= *p;
		*p = val;
	}
}

void SceneState_ApplyFlag970(void)
{
	Engine_GameFlagSet(0x970);
}

void SceneState_RunUnlessActorZeroAtTile32x50(void)
{
	struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

	if ((actor->x.fixed >> 20) != 32 || (actor->z.fixed >> 20) != 50)
	{
		SceneActor_ApplyPointLeftOfActorZero();
	}
}

void SceneState_RunUnlessActorZeroAt30_52(void)
{
	struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

	if ((actor->x.fixed >> 20) != 30 || (actor->z.fixed >> 20) != 52)
	{
		SceneActor_PassActorZeroOffsetPoint();
	}
}

void FieldScene_RunSupplementalSequenceTwo(void)
{
	u32				   i;
	s32				   rec2;
	s32				   rec7;
	struct FieldActor *actor;
	s32				   value;
	s32				   v5;
	s32				   v6;
	u8				   slot16[40];
	u8				  *frame;

	Engine_EventBegin();
	actor = Object_GetById(18);
	if ((actor->x.fixed >> 20) == 46)
	{
		Engine_EventWait(30);
		rec2 = OverlayObject_CreateAndInitialize(0x2e80000, 0, 0xb80000, 253);
		frame = slot16;
		*(s32 *)(frame + 8) = 0x9999;
		*(s32 *)(frame + 12) = 0x9999;
		*(s32 *)(frame + 4) = 7;
		Actor_Get(18)->motion_flags = 0;
		Engine_AudioPlayCue(185);
		for (i = 0; i < 16; i++)
		{
			Engine_TaskWait(3);
			Actor_Get(18)->y.fixed -= 0x10000;
			rec7 = Random_Next();
			rec7 = ((((u32)(rec7 << 4) >> 16) << 16) + 0x2e00000);
			value = Engine_RandomNext();
			((void (*)())Effect_Spawn)(rec7, 0,
				((((u32)(((value << 3) + value) << 1) >> 16) << 16) + 0x800000), 0, 0, 0, 0x90000,
				frame);
		}
		Map_CopyCellAttributes(51, 8, 1, 1, 49, 8);
		Engine_EventWait(30);
		Actor_Get(18)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
		Engine_ActorSetAnimation(18, 3);
		Engine_ObjectDispatchRelease(rec2);
		Map_CopyCellAttributes(45, 4, 1, 1, 46, 8);
		Actor_SetPosition(20, 0x2e80000, 0x880000);
		v5 = 1;
		v6 = 3;
		Engine_AudioPlayCue(188);
		Map_CopyCellsTo(58, 8, 49, 8, v5, v6);
		Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
		Work_SetValuesIfNonNegative(-1, -1, 0xe666);
		Engine_EventWait(20);
		Engine_AudioPlayCue(188);
		Map_CopyCellsTo(59, 8, 49, 8, v5, v6);
		Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
		Work_SetValuesIfNonNegative(-1, -1, 0xe666);
		Engine_MapRenderWaitForValues();
		Engine_EventWait(10);
		GameFlag_Set(0x971);
	}
	Engine_EventEnd();
}

void FieldScene_RunFourCallSequenceB(void)
{
	typedef s32 (*Handler_02001a10)(struct Record_02000ec8 *record);

	Engine_EventBegin();
	StagedActor_AdvancePair();
	Engine_EventEnd();
	FieldScene_RunSupplementalSequenceTwo();
}

void ActorPresentation_ConfigureActorTwentyAndFlag200(void)
{
	u8 *flags;

	Engine_ActorSetAnimation(20, 1);
	Engine_ActorSetChildValue(20, 0);
	Engine_ActorSetAnimation(20, 2);
	flags = &Object_GetById(20)->priority_flags;
	*flags &= 0xFD;
	Engine_GameFlagSet(0x200);
}

void FieldScene_RunSupplementalSequenceOne(void)
{
	u32				   i;
	s32				   rec2;
	s32				   rec7;
	struct FieldActor *actor;
	s32				   value;
	s32				   arg0;
	s32				   arg2;
	s32				   v5;
	s32				   v6;
	u8				   slot16[40];
	u8				  *slot;

	Engine_EventBegin();
	actor = Object_GetById(19);
	if ((actor->x.fixed >> 20) == 48)
	{
		if (GameFlag_IsSet(0x202) != 0)
		{
			Engine_EventWait(30);
			rec2 = OverlayObject_CreateAndInitialize(0x3020000, 0, 0x1120000, 223);
			slot = slot16;
			*(s32 *)(slot + 8) = 0x9999;
			*(s32 *)(slot + 12) = 0x9999;
			*(s32 *)(slot + 4) = 7;
			Actor_Get(19)->motion_flags = 0;
			Engine_AudioPlayCue(185);
			for (i = 0; i < 16; i++)
			{
				Engine_TaskWait(3);
				Actor_Get(19)->y.fixed -= 0x10000;
				rec7 = Engine_RandomNext();
				arg0 = ((((u32)(rec7 << 4) >> 16) << 16) + 0x3000000);
				value = Engine_RandomNext();
				arg2 = ((((u32)(((value << 3) + value) << 1) >> 16) << 16) + 0xe00000);
				((void (*)())Effect_Spawn)(arg0, 0, arg2, 0, 0, 0, 0x90000, slot);
			}
			Map_CopyCellAttributes(51, 8, 1, 1, 45, 14);
			Engine_EventWait(30);
			Actor_Get(19)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
			Engine_ActorSetAnimation(19, 3);
			Engine_ObjectDispatchRelease(rec2);
			Map_CopyCellAttributes(45, 4, 1, 1, 48, 14);
			Actor_SetPosition(21, 0x3080000, 0xe80000);
			v5 = 1;
			v6 = 3;
			Engine_AudioPlayCue(188);
			Map_CopyCellsTo(58, 8, 45, 14, v5, v6);
			Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
			Work_SetValuesIfNonNegative(-1, -1, 0xe666);
			Engine_EventWait(20);
			Engine_AudioPlayCue(188);
			Map_CopyCellsTo(59, 8, 45, 14, v5, v6);
			Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
			Work_SetValuesIfNonNegative(-1, -1, 0xe666);
			Engine_MapRenderWaitForValues();
			Engine_EventWait(10);
			GameFlag_Set(0x972);
		}
	}
	Engine_EventEnd();
}

void FieldScene_RunFourStepSequenceA(void)
{
	typedef s32 (*Handler_02001a10)(struct Record_02000ec8 *record);

	Engine_EventBegin();
	StagedActor_AdvancePair();
	Engine_EventEnd();
	FieldScene_RunSupplementalSequenceOne();
}

void BabiChika_InstallActorNineteenFlicker(void)
{
	Engine_ActorEnableActionCallback(19, BabiChika_FlickerScript);
}

void SceneState_SetValue202ThenCall(void)
{
	Engine_GameFlagSet(0x202);
	FieldScene_RunSupplementalSequenceOne();
}

void SceneActor_ConfigureSlot21AndSetFlag201(void)
{
	u8 *flags;

	Engine_ActorSetAnimation(21, 1);
	Engine_ActorSetChildValue(21, 0);
	Engine_ActorSetAnimation(21, 2);
	flags = &Object_GetById(21)->priority_flags;
	*flags &= 0xFD;
	Engine_GameFlagSet(0x201);
}

void SceneDialogue_RunFlag982Or983Dialogue(void)
{
	Engine_EventBegin();
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	if (Engine_GameFlagIsSet(0x982) != 0 || Engine_GameFlagIsSet(0x983) != 0)
	{
		Engine_MessageShowCentered(MSGNO(MsgBabiChikaStatueSpeaksAfterAnswer), 1);
	}
	else
	{
		Engine_MessageShowCentered(MSGNO(MsgBabiChikaStatueSpeaksRobinSoulYe), 1);
	}
	Engine_EventEnd();
}

void FieldScene_RunTwoStepSequence(void)
{
	typedef s32 (*Handler_02001a10)(struct Record_02000ec8 *record);

	Engine_EventBegin();
	Engine_EventEnd();
}

void FieldScene_RunFourStepSequenceB(void)
{
	typedef s32 (*Handler_02001a10)(struct Record_02000ec8 *record);

	Engine_EventBegin();
	StagedActor_AdvancePair();
	Engine_EventEnd();
	FieldScene_RunTwoStepSequence();
}

void SceneActor_InstallSlotNineHandler(void)
{
	typedef s32 (*Handler_02001a10)(struct Record_02000ec8 *record);

	struct FieldActor *owner;

	Engine_ActorEnableActionCallback(8, BabiChika_FlickerScript);
	Engine_GameFlagSet(0x203);
	owner = Object_GetById(9);
	owner->update = (void (*)(union FieldObject *))SceneActor_CopyActor8PositionWhenAtRow10;
}

void SceneActor_SetupSlotNineAndInstallHandler(void)
{
	typedef s32 (*Handler_02001a10)(struct Record_02000ec8 *record);

	u8 *desc;
	s32 col;
	s32 row;

	Engine_EventBegin();
	Engine_ActorSetSpritePriority(9, 1);
	Engine_ActorSetAnimation(9, 1);
	Engine_ActorSetChildValue(9, 0);
	Engine_ActorSetAnimation(9, 2);

	{
		u8 *flag = &Object_GetById(9)->priority_flags;
		*flag &= 0xfd;
	}

	Engine_GameFlagSet(0x204);

	col = Object_GetById(9)->x.fixed;
	row = Object_GetById(9)->z.fixed >> 20;
	Engine_MapCopyCellAttributes(26, 8, 1, 1, col >> 20, row);

	desc = Object_GetById(9);
	*(Handler_02001a10 *)(desc + 108) = SceneActor_SetFlagBitByRelativeDepth;

	desc = Object_GetById(8);
	*(Handler_02001a10 *)(desc + 108) = SceneActor_SetFlagBitByRelativeDepth;

	((u8 * (*)()) Engine_EventEnd)(desc);
}

s32 OverlayObject_SetYAboveLinkedActor(u8 *owner)
{
	s16				  *id = (s16 *)(owner + 100);
	struct FieldActor *actor = Object_GetById(*id);

	*(s32 *)(owner + 12) = actor->y.fixed + 0x100000;
	return 0;
}

void BabiChika_SettleSteps(s32 wait)
{
	u32 i;

	Actor_SetSpeed(10, 0x8000, 0x4000);
	Actor_SetSpeed(11, 0x8000, 0x4000);
	if (wait != 0)
	{
		Engine_AudioPlayCue(180);
	}
	Engine_ObjectSetPosition(Object_GetById(10), Object_GetById(10)->x.fixed,
		Data_0200b350[Object_GetById(10)->timer], Object_GetById(10)->z.fixed);
	Engine_ObjectSetPosition(Object_GetById(11), Object_GetById(11)->x.fixed,
		Data_0200b350[Object_GetById(11)->timer], Object_GetById(11)->z.fixed);
	Engine_ActorWaitForMove(10);
	Engine_ActorWaitForMove(11);
	Object_GetById(10)->y.fixed = Data_0200b350[Object_GetById(10)->timer];
	Object_GetById(11)->y.fixed = Data_0200b350[Object_GetById(11)->timer];
	if (wait != 0)
	{
		Engine_AudioPlayCue(0x121);
	}
	for (i = 0; i < 5; i++)
	{
		if (Object_GetById(i + 10)->y.fixed / 0x10000 < 0
			&& Object_GetById(i + 10)->y.fixed / 0x10000 > -30)
		{
			Engine_MapCopyCellAttributes(4, 9, 1, 1, Object_GetById(i + 10)->x.fixed >> 20,
				Object_GetById(i + 10)->z.fixed >> 20);
		}
	}
	Engine_EventWait(wait);
}

void SceneActor_LandOnHighestPlatform(s32 subject)
{
	s32 best = -0x500000;
	u32 i;

	for (i = 0; i <= 4; i++)
	{
		s32 slot = i + 10;

		if (slot == subject)
			continue;

		if ((Object_GetById(slot)->x.fixed >> 20) != (Object_GetById(subject)->x.fixed >> 20))
			continue;
		if ((Object_GetById(slot)->z.fixed >> 20) != (Object_GetById(subject)->z.fixed >> 20))
			continue;

		if (best > Object_GetById(slot)->y.fixed + 0x100000)
			continue;

		best = Object_GetById(slot)->y.fixed + 0x100000;
		Object_GetById(subject)->timer = slot;
	}

	Engine_ActorSetSpeed(subject, 0x40000, 0x20000);

	{
		struct FieldActor *target = Object_GetById(subject);
		struct FieldActor *from = Object_GetById(subject);
		s32				   z = Object_GetById(subject)->z.fixed;

		Engine_ObjectSetPosition(target, from->x.fixed, best, z);
	}

	Engine_ActorWaitForMove(subject);
	Engine_AudioPlayCue(188);
	SceneEffect_SpawnNineRadialEffects(subject);
	Engine_EventWait(30);
}

void FieldScene_RunMiddleSequence(void)
{
	struct FieldActor saved;
	s32				  i;
	s32				  j;
	s32				  found;

	Engine_EventBegin();
	for (i = 0; i <= 2; i++)
	{
		if (Object_GetById(i + 12)->sprite->priority == 3 && GameFlag_IsSet(i + 0x200) == 0)
		{
			Actor_Get(i + 12);
			SceneActor_WaitValueBelowLimit();
			Actor_SetPosition(i + 12, 0, 0);
			GameFlag_Set(i + 0x200);
			break;
		}
		if ((Object_GetById(i + 12)->z.fixed >> 20) == 9 && GameFlag_IsSet(i + 0x200) == 0)
		{
			Object_GetById(i + 12)->terrain_height = 0;
			Object_GetById(i + 12)->velocity_y = 0;
			Object_GetById(i + 12)->target_y = ACTOR_NO_TARGET;
			Actor_Get(i + 12)->motion_flags = 0;
			Object_GetById(i + 12)->timer = 0;
			found = i;
			for (j = 0; j < i; j++)
			{
				if (GameFlag_IsSet(0x200 + j) == 0)
				{
					saved.x.fixed = Object_GetById(i + 12)->x.fixed;
					saved.y.fixed = Object_GetById(i + 12)->y.fixed;
					saved.z.fixed = Object_GetById(i + 12)->z.fixed;
					Actor_Get(i + 12)->x.fixed = Object_GetById(j + 12)->x.fixed;
					Object_GetById(i + 12)->y.fixed = Object_GetById(j + 12)->y.fixed;
					Object_GetById(i + 12)->z.fixed = Object_GetById(j + 12)->z.fixed;
					Actor_Get(j + 12)->x.fixed = saved.x.fixed;
					Actor_Get(j + 12)->y.fixed = saved.y.fixed;
					Actor_Get(j + 12)->z.fixed = saved.z.fixed;
					found = j;
					break;
				}
			}
			Object_GetById(found + 12)->terrain_height = 0;
			Actor_Get(found + 12)->velocity_y = 0;
			Object_GetById(found + 12)->target_y = ACTOR_NO_TARGET;
			Actor_Get(found + 12)->motion_flags = 0;
			Object_GetById(found + 12)->timer = 0;
			Engine_CameraSetSpeed(0x30000, 0x6000);
			((struct FieldActor *)Battle_GetWorkObject1e0())->motion_flags = 0;
			Camera_MoveTo(0xa80000, 0x80000, 0xb80000, 1);
			Engine_CameraWaitForMove();
			SceneActor_LandOnHighestPlatform(found + 12);
			if ((Object_GetById(found + 12)->x.fixed >> 20) == 8)
			{
				Object_GetById(10)->timer++;
				(Object_GetById(11)->timer)--;
			}
			else
			{
				(Object_GetById(10)->timer)--;
				Object_GetById(11)->timer++;
			}
			Actor_Get(found + 12)->update =
				(void (*)(union FieldObject *))OverlayObject_SetYAboveLinkedActor;
			BabiChika_SettleSteps(40);
			Actor_Get(found + 12)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
			GameFlag_Set(0x200 + found);
			break;
		}
	}
	Engine_EventEnd();
}

void FieldScene_RunThreeStepSequence(void)
{
	typedef s32 (*Handler_02001a10)(struct Record_02000ec8 *record);

	Engine_EventBegin();
	StagedActor_AdvancePair();
	Engine_EventEnd();
}

void SceneState_SetSlot17And18Selectors(void)
{
	Engine_EventBegin();

	if ((Object_GetById(17)->x.fixed >> 20) == 45)
	{
		Engine_GameFlagSet(0x974);
	}
	else
	{
		Engine_GameFlagClear(0x974);
	}

	if ((Object_GetById(18)->x.fixed >> 20) == 46)
	{
		Engine_GameFlagSet(0x975);
	}
	else
	{
		Engine_GameFlagClear(0x975);
	}

	BabiChika_MarkActorCells();
	Engine_EventEnd();
}

void FieldScene_RunFourCallSequence(void)
{
	typedef s32 (*Handler_02001a10)(struct Record_02000ec8 *record);

	Engine_EventBegin();
	StagedActor_AdvancePair();
	SceneState_SetSlot17And18Selectors();
	Engine_EventEnd();
}

void FieldScene_RunLateSequenceHead(void)
{
	Map_CopyCellAttributes(72, 49, 1, 1, 8, 49);
	Map_CopyCellAttributes(113, 43, 1, 1, 49, 43);
	MapObject_SetPosition(100, 0, 0);
	MapObject_SetPosition(101, 0, 0);
	Actor_SetPosition(15, 8912896, 51904512);
	Actor_SetPosition(16, 51904512, 45613056);
}

void FieldScene_RunLateSequenceSecond(void)
{
	Map_CopyCellAttributes(8, 113, 1, 1, 8, 49);
	Map_CopyCellAttributes(49, 107, 1, 1, 49, 43);
	MapObject_SetPosition(100, -1, -1);
	MapObject_SetPosition(101, -1, -1);
	Actor_SetPosition(15, 0, 0);
	Actor_SetPosition(16, 0, 0);
}

void FieldScene_RunScene3c4SequenceA(void)
{
	struct FieldActor *record;
	s32				   v6;
	s32				   p5;
	s32				   q;

	v6 = 0;
	Engine_EventBegin();
	Map_CopyCellAttributes(83, 45, 11, 8, 19, 45);
	record = Object_GetById(19);
	p5 = record->x.fixed;
	q = Object_GetById(19)->z.fixed;
	q >>= 20;
	p5 >>= 20;
	Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
	record = Object_GetById(20);
	p5 = record->x.fixed;
	q = Object_GetById(20)->z.fixed;
	q >>= 20;
	p5 >>= 20;
	Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
	record = Object_GetById(21);
	p5 = record->x.fixed;
	q = Object_GetById(21)->z.fixed;
	q >>= 20;
	p5 >>= 20;
	Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
	record = Object_GetById(22);
	p5 = record->x.fixed;
	q = Object_GetById(22)->z.fixed;
	q >>= 20;
	p5 >>= 20;
	Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
	record = Object_GetById(23);
	p5 = record->x.fixed;
	q = Object_GetById(23)->z.fixed;
	q >>= 20;
	p5 >>= 20;
	Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
	record = Object_GetById(19);
	if ((record->x.fixed >> 20) == 25)
	{
		record = Object_GetById(19);
		if ((record->z.fixed >> 20) == 49)
		{
			v6 = 1;
		}
	}
	record = Object_GetById(20);
	if ((record->x.fixed >> 20) == 23)
	{
		record = Object_GetById(20);
		if ((record->z.fixed >> 20) == 49)
		{
			v6 = (v6 + 1);
		}
	}
	record = Object_GetById(21);
	if ((record->x.fixed >> 20) == 25)
	{
		record = Object_GetById(21);
		if ((record->z.fixed >> 20) == 47)
		{
			v6 = (v6 + 1);
		}
	}
	record = Object_GetById(22);
	if ((record->x.fixed >> 20) == 23)
	{
		record = Object_GetById(22);
		if ((record->z.fixed >> 20) == 47)
		{
			v6 = (v6 + 1);
		}
	}
	record = Object_GetById(23);
	if ((record->x.fixed >> 20) == 24)
	{
		record = Object_GetById(23);
		if ((record->z.fixed >> 20) == 48)
		{
			v6 = (v6 + 1);
		}
	}
	if (v6 == 5)
	{
		if (GameFlag_IsSet(0x984) != 0)
		{
			Engine_EventEnd();
			goto done;
		}
		Engine_EventWait(20);
		Camera_SetSpeed(0xcccc, 0x1999);
		Camera_MoveTo(0x1d80000, -1, 0x30c0000, 1);
		Engine_CameraWaitForMove();
		Engine_EventWait(30);
		GameFlag_Set(0x984);
		Engine_AudioPlayCue(158);
		Map_AnimateCells(Data_0200b3ec, 32, 46);
		Map_CopyCellAttributes(24, 60, 1, 1, 32, 47);
		Engine_EventWait(40);
	}
	else
	{
		if (GameFlag_IsSet(0x984) != 0)
		{
			Engine_EventWait(20);
			Camera_SetSpeed(0xcccc, 0x1999);
			Camera_MoveTo(0x1d80000, -1, 0x30c0000, 1);
			Engine_CameraWaitForMove();
			Engine_EventWait(30);
			GameFlag_Clear(0x984);
			Engine_AudioPlayCue(159);
			Map_AnimateCells(Data_0200b40c, 32, 46);
			Map_CopyCellAttributes(31, 47, 1, 1, 32, 47);
			Engine_EventWait(40);
		}
	}
	Engine_EventEnd();
done:;
}

void FieldScene_RunLayoutAt83By45(void)
{
	Engine_EventBegin();
	{
		s32 width = 19;
		s32 height = 45;

		Engine_MapCopyCellAttributes(83, 45, 11, 8, width, height);
	}
	StagedActor_AdvancePair();
	FieldScene_RunScene3c4SequenceA();
	Engine_EventEnd();
}

void SceneState_SetValue268bInScene(void)
{
	Engine_EventBegin();
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_MessageShowCentered(MSGNO(MsgBabiChikaStatueSeemsSpeakYourSoul), 1);
	Engine_EventEnd();
}

void FieldScene_RunScriptedStep953(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered(MSGNO(MsgFieldDoorTightlyLocked), 1);
	Engine_EventEnd();
}

s32 BabiChika_SelectEvents(void)
{
	if (gGameState.scene == (s32)&SceneId_BabiChika1)
	{
		return (s32)Data_0200bc0c;
	}
	return (s32)Data_0200bef4;
}

void FieldScene_PlaceAndPinSlots8And9(void)
{
	typedef s32 (*Handler_02001a10)(struct Record_02000ec8 *record);

	s32 row;

	{
		s32 p5 = 9, p6 = 38;
		Engine_MapCopyCellAttributes(73, 38, 5, 5, p5, p6);
	}
	SceneState_SwapSlotPairByRank(9, 8);

	{
		s32 col = Object_GetById(8)->x.fixed >> 20;
		row = Object_GetById(8)->z.fixed >> 20;
		Engine_MapCopyCellAttributes(2, 36, 1, 1, col, row);
	}

	{
		s32 col = Object_GetById(9)->x.fixed >> 20;
		row = Object_GetById(9)->z.fixed >> 20;
		Engine_MapCopyCellAttributes(2, 36, 1, 1, col, row);
	}
}

void FieldScene_PlaceAndPinSlots10And11(void)
{
	s32 row;

	{
		s32 k5 = 29, k6 = 30;
		Engine_MapCopyCellAttributes(93, 30, 6, 5, k5, k6);
	}
	SceneState_SwapSlotPairByRank(11, 10);

	{
		s32 col20 = Object_GetById(10)->x.fixed >> 20;
		row = Object_GetById(10)->z.fixed >> 20;
		Engine_MapCopyCellAttributes(2, 36, 1, 1, col20, row);
	}

	{
		s32 col20 = Object_GetById(11)->x.fixed >> 20;
		row = Object_GetById(11)->z.fixed >> 20;
		Engine_MapCopyCellAttributes(2, 36, 1, 1, col20, row);
	}
}

void BabiChika_RestorePlatformCells(void)
{
	struct FieldActor *record;
	s32				   p5;

	Map_CopyCellAttributes(89, 49, 3, 2, 25, 49);
	Map_CopyCellAttributes(89, 51, 8, 5, 25, 51);
	Object_GetById(14)->terrain_id = 1;
	record = Object_GetById(12);
	p5 = record->x.fixed;
	record = Object_GetById(12);
	p5 = p5 >> 20;
	Map_CopyCellAttributes(22, 52, 1, 1, p5, (record->z.fixed >> 20));
	record = Object_GetById(13);
	p5 = record->x.fixed;
	record = Object_GetById(13);
	p5 = p5 >> 20;
	Map_CopyCellAttributes(22, 52, 1, 1, p5, (record->z.fixed >> 20));
	record = Object_GetById(14);
	p5 = record->x.fixed;
	record = Object_GetById(14);
	p5 = p5 >> 20;
	Map_CopyCellAttributes(22, 52, 1, 1, p5, (record->z.fixed >> 20));
}

void BabiChika_MarkActorCells(void)
{
	Map_CopyCellAttributes(108, 19, 4, 1, 44, 19);
	StagedActor_FillGridAttributeRectangle(
		0, Object_GetById(17)->x.fixed >> 20, Object_GetById(17)->z.fixed >> 20, 1, 1, 0xff);
	StagedActor_FillGridAttributeRectangle(
		0, Object_GetById(18)->x.fixed >> 20, Object_GetById(18)->z.fixed >> 20, 1, 1, 0xff);
}

s32 FieldScene_InitializeActorGroups(void)
{
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	if (gGameState.scene == (s32)&SceneId_BabiChika1
		|| gGameState.scene == (s32)&SceneId_BabiChika2)
	{
		BattleFx_StartFadeOverlay(0);
		gGameState.retreat_entrance = 1;
		gGameState.retreat_scene = (s32)&SceneId_BabiChika1;
	}
	if (gGameState.scene == (s32)&SceneId_BabiChika1)
	{
		switch (gGameState.entrance)
		{
		case 1:
		case 2:
			if (Engine_GameFlagIsSet(0x982))
			{
				Map_CopyCellsTo(121, 4, 74, 9, 5, 8);
				Map_CopyCellsTo(18, 83, 9, 73, 3, 2);
				Engine_MapCopyCellsTo(18, 81, 9, 75, 3, 2);
				Engine_MapCopyCellsTo(18, 83, 9, 77, 3, 2);
				Engine_MapCopyCellsTo(18, 83, 9, 79, 3, 2);
				Engine_MapCopyCellsTo(18, 83, 11, 78, 3, 2);
				Engine_MapCopyCellsTo(18, 83, 13, 79, 3, 2);
			}
			else
			{
				if (!Engine_GameFlagIsSet(0x983))
					break;
				Map_CopyCellsTo(121, 13, 74, 9, 5, 8);
				Map_CopyCellsTo(18, 85, 11, 74, 3, 2);
				Engine_MapCopyCellsTo(18, 83, 13, 75, 3, 2);
				Engine_MapCopyCellsTo(18, 85, 11, 76, 3, 2);
				Engine_MapCopyCellsTo(18, 83, 11, 78, 3, 2);
				Engine_MapCopyCellsTo(18, 83, 13, 79, 3, 2);
			}
			break;
		case 3:
		case 4:
			FieldScene_PlaceAndPinSlots8And9();
			Object_GetById(8)->motion_flags = 0;
			Object_GetById(9)->motion_flags = 0;
			Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
			Object_GetById(8)->update = ACTOR_UPDATE_IDLE;
			Object_GetById(9)->update = ACTOR_UPDATE_IDLE;
			Engine_TaskAddCallback(BabiChika_UpdateTrackedActor, 0xc80);
			break;
		case 5:
		case 6:
		case 7:
			if (Engine_GameFlagIsSet(0x982))
				Map_CopyCells(23, 17, 1, 2, 30, 8);
			if (Engine_GameFlagIsSet(0x983))
				Map_CopyCells(23, 17, 1, 2, 32, 10);
			break;
		case 8:
		case 9:
			FieldScene_PlaceAndPinSlots10And11();
			Object_GetById(10)->motion_flags = 0;
			Object_GetById(11)->motion_flags = 0;
			Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
			Object_GetById(10)->update = ACTOR_UPDATE_IDLE;
			Object_GetById(11)->update = ACTOR_UPDATE_IDLE;
			Engine_TaskAddCallback(BabiChika_UpdateTrackedActor, 0xc80);
			break;
		case 10:
		case 11:
			Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(19), 0);
			Engine_ActorSetAnimation(18, 2);
			Engine_ActorSetSpriteFlags(Object_GetById(20), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(21), 0);
			Engine_ActorSetChildValue(20, 15);
			Engine_ActorSetChildValue(21, 15);
			if (Engine_GameFlagIsSet(0x971))
			{
				Map_CopyCellsTo(59, 8, 49, 8, 1, 3);
				Engine_MapCopyCellAttributes(51, 8, 1, 1, 49, 8);
				Object_GetById(18)->priority_flags |= 2;
				Engine_ActorSetAnimation(18, 3);
				Engine_MapCopyCellAttributes(45, 4, 1, 1, 46, 8);
				Actor_SetPosition(18, 186 << 18, 136 << 16);
				Object_GetById(18)->y.fixed = -0x100000;
				Actor_SetPosition(20, 186 << 18, 136 << 16);
			}
			if (Engine_GameFlagIsSet(0x200))
			{
				Engine_ActorSetChildValue(20, 0);
				Engine_ActorSetAnimation(20, 5);
			}
			if (GameFlag_IsSet(0x202))
				Engine_ActorSetAnimation(19, 2);
			if (Engine_GameFlagIsSet(0x972))
			{
				Map_CopyCellsTo(59, 8, 45, 14, 1, 3);
				Engine_MapCopyCellAttributes(51, 8, 1, 1, 45, 14);
				Object_GetById(19)->priority_flags |= 2;
				Engine_ActorSetAnimation(19, 3);
				Engine_MapCopyCellAttributes(45, 4, 1, 1, 48, 14);
				Actor_SetPosition(19, 194 << 18, 232 << 16);
				Object_GetById(19)->y.fixed = -0x100000;
				Engine_ActorSetPosition(21, 194 << 18, 232 << 16);
				Engine_GameFlagSet(0x202);
			}
			if (Engine_GameFlagIsSet(0x201))
			{
				Engine_ActorSetChildValue(21, 0);
				Engine_ActorSetAnimation(21, 5);
			}
			break;
		case 12:
		case 13:
			BabiChika_RestorePlatformCells();
			Object_GetById(12)->motion_flags = 0;
			Object_GetById(13)->motion_flags = 0;
			Engine_ActorSetSpriteFlags(Object_GetById(15), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(16), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(17), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
			Object_GetById(12)->update = ACTOR_UPDATE_IDLE;
			Object_GetById(13)->update = ACTOR_UPDATE_IDLE;
			Object_GetById(14)->update = ACTOR_UPDATE_IDLE;
			Engine_TaskAddCallback(BabiChika_UpdateTrackedActor, 0xc80);
			break;
		}
	}
	else
	{
		switch (gGameState.entrance)
		{
		case 0:
			break;
		case 1:
		case 2:
		case 3:
			gGameState.retreat_entrance = 1;
			gGameState.retreat_scene = (s32)&SceneId_BabiIriguchi3;
			Engine_GameFlagClear(0x12f);
			Engine_ActorSetChildValue(17, 6);
			Engine_ActorSetChildValue(18, 6);
			if (Engine_GameFlagIsSet(0x974))
				Actor_SetPosition(17, 182 << 18, 156 << 17);
			if (Engine_GameFlagIsSet(0x975))
				Actor_SetPosition(18, 186 << 18, 156 << 17);
			BabiChika_MarkActorCells();
			break;
		case 6:
		case 7:
			Engine_ActorSetSpritePriority(8, 1);
			Object_GetById(8)->motion_flags = 0;
			Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
			Engine_ActorSetSpritePriority(9, 1);
			Engine_ActorSetChildValue(9, 15);
			Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
			Object_GetById(9)->motion_flags = 0;
			if (!Engine_GameFlagIsSet(0x204))
				break;
			Engine_ActorSetChildValue(9, 0);
			Engine_ActorSetAnimation(9, 5);
			Engine_MapCopyCellAttributes(
				26, 8, 1, 1, Object_GetById(9)->x.fixed >> 20, Object_GetById(9)->z.fixed >> 20);
			Object_GetById(9)->update = ACTOR_UPDATE_IDLE;
			Object_GetById(8)->update = ACTOR_UPDATE_IDLE;
			break;
		case 4:
		case 5:
			if (!Engine_GameFlagIsSet(0x109))
			{
				Object_GetById(10)->motion_flags = 0;
				Object_GetById(11)->motion_flags = 0;
				Object_GetById(10)->y.fixed = -0x300000;
				Object_GetById(11)->y.fixed = -0x300000;
				Object_GetById(10)->priority_flags |= 2;
				Object_GetById(11)->priority_flags |= 2;
				Object_GetById(10)->collision_flags &= 0xfe;
				Object_GetById(11)->collision_flags &= 0xfe;
				Object_GetById(10)->timer = 3;
				Object_GetById(11)->timer = 3;
				Engine_ActorSetSpritePriority(10, 1);
				Engine_ActorSetSpritePriority(11, 1);
				Object_GetById(12)->motion_flags = 0;
				Object_GetById(13)->motion_flags = 0;
				Object_GetById(14)->motion_flags = 0;
				Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
				Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
				Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
				Object_GetById(12)->timer = 0;
				Object_GetById(13)->timer = 0;
				Object_GetById(14)->timer = 0;
				if (gGameState.entrance != 5)
					break;
				Object_GetById(10)->y.fixed = -0x200000;
				Object_GetById(11)->y.fixed = -0x400000;
				Object_GetById(10)->timer = 2;
				Object_GetById(11)->timer = 4;
				Actor_SetPosition(12, 200 << 16, 152 << 16);
				Object_GetById(12)->timer = 11;
				Object_GetById(12)->update = ACTOR_UPDATE_PANEL;
				Object_GetById(12)->priority_flags |= 2;
				Actor_SetPosition(13, 200 << 16, 152 << 16);
				Object_GetById(13)->timer = 12;
				Object_GetById(13)->update = ACTOR_UPDATE_PANEL;
				Object_GetById(13)->priority_flags |= 2;
				Engine_ActorSetPosition(14, 136 << 16, 152 << 16);
				Object_GetById(14)->timer = 10;
				Object_GetById(14)->update = ACTOR_UPDATE_PANEL;
				Object_GetById(14)->priority_flags |= 2;
				Engine_EventWait(2);
				Engine_GameFlagSet(0x200);
				Engine_GameFlagSet(0x201);
				Engine_GameFlagSet(0x202);
			}
			BabiChika_SettleSteps(0);
			break;
		case 8:
		case 9:
		case 10:
		case 11:
			if (Engine_GameFlagIsSet(0x982))
				Map_CopyCells(10, 30, 1, 2, 16, 30);
			if (Engine_GameFlagIsSet(0x983))
				Map_CopyCells(10, 30, 1, 2, 22, 30);
			Engine_GameFlagSet(0x973);
			break;
		case 12:
			Map_CopyCellAttributes(8, 49, 1, 1, 8, 113);
			FieldScene_RunLateSequenceHead();
			Engine_TaskAddCallback(BabiChika_UpdateTrackedActor, 0xc80);
			break;
		case 13:
		case 14:
			Engine_EventWait(1);
			if (Engine_GameFlagIsSet(0x984))
			{
				Map_CopyCells(24, 59, 1, 2, 32, 46);
				Actor_SetPosition(19, 204 << 17, 198 << 18);
				Actor_SetPosition(20, 188 << 17, 198 << 18);
				Actor_SetPosition(21, 204 << 17, 190 << 18);
				Actor_SetPosition(22, 188 << 17, 190 << 18);
				Actor_SetPosition(23, 196 << 17, 194 << 18);
			}
			Object_GetById(19)->motion_flags &= 0xfe;
			Object_GetById(20)->motion_flags &= 0xfe;
			Object_GetById(21)->motion_flags &= 0xfe;
			Object_GetById(22)->motion_flags &= 0xfe;
			Object_GetById(23)->motion_flags &= 0xfe;
			Engine_ActorSetChildValue(19, 4);
			Engine_ActorSetChildValue(20, 1);
			Engine_ActorSetChildValue(21, 4);
			Engine_ActorSetChildValue(22, 10);
			Engine_ActorSetChildValue(23, 0);
			Engine_ActorSetAnimation(19, 2);
			Engine_ActorSetAnimation(23, 2);
			Engine_MapCopyCellAttributes(
				20, 56, 1, 1, Object_GetById(19)->x.fixed >> 20, Object_GetById(19)->z.fixed >> 20);
			Engine_MapCopyCellAttributes(
				20, 56, 1, 1, Object_GetById(20)->x.fixed >> 20, Object_GetById(20)->z.fixed >> 20);
			Engine_MapCopyCellAttributes(
				20, 56, 1, 1, Object_GetById(21)->x.fixed >> 20, Object_GetById(21)->z.fixed >> 20);
			Engine_MapCopyCellAttributes(
				20, 56, 1, 1, Object_GetById(22)->x.fixed >> 20, Object_GetById(22)->z.fixed >> 20);
			Engine_MapCopyCellAttributes(
				20, 56, 1, 1, Object_GetById(23)->x.fixed >> 20, Object_GetById(23)->z.fixed >> 20);
			break;
		case 17:
			Map_CopyCellAttributes(49, 43, 1, 1, 49, 107);
			FieldScene_RunLateSequenceHead();
			break;
		}
	}
	return 0;
}

#undef SceneActor_MoveActorZeroToTarget
#undef SceneActor_PassActorZeroOffsetPoint
#undef SceneActor_WaitValueBelowLimit
#undef SceneState_SwapSlotPairByRank
