#include "game_state.h"
#include "unit/import.h"
#include "unit/field/vinasu_heya/heya.h"
#include "unit/field/common/effect/move.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object2.h"
#include "unit/battle/effect/object4.h"
#include "unit/battle/effect/object6.h"
#include "unit/battle/effect/object8.h"
#include "unit/battle/effect/object9.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/field/common/script/field.h"
#include "unit/field/common/script/field2.h"
#include "unit/field/common/script/field7.h"
#include "unit/field/common/script/field10.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime7.h"
#include "iowrite.h"
#include "object_effect.h"

#define NULL ((void *)0)
#include "global_cells.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_effect.h"
#include "overlay_object.h"
#include "staged_actor.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "iwram_call.h"
#include "call.h"
#include "text/msg_ids.h"
#include "field_service.h"
#include "io_reg.h"
#include "scene_ids.h"
#include "map.h"

// 前方宣言
struct DirXZ;
static void VinasuHeya_StepActorWithDust(struct DirXZ *dir);
static void Scene_RunPairedParticleWaveSequence(void);
static void SceneState_CallWith432And32(void);
static void FieldScene_CallWith560And44(void);
static void Scene_RunParticleWaveSequence(void);
static void Scene_RunEastParticleWaveSequence(void);
static void FieldScene_RunLeaderDropSequence(void);

struct DirXZ
{
	s32 x;
	s32 unused;
	s32 z;
};

struct Vec
{
	s32 x;
	s32 y;
	s32 z;
};

union VecView
{
	struct Vec v;
	s16		   h[6];
};

static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
	*flags |= bits;
}

struct EventWorkState
{
	u8	unknown_000[0xcb6];
	u16 field_cb6;
	u8	unknown_cb8[2];
	u16 field_cba;
};

static void ConfigureOverlayObject(struct FieldActor *object, s32 palette)
{
	object->motion_flags = 0;
	object->collision_flags = 8;
	Engine_ActorSetSpriteFlags(object, 0);
	ObjectGroup_SetChildValue(object, palette);
}

static void *OverlayObject_SpawnWithMode14(s32 x, s32 y, s32 z, s32 type)
{
	struct FieldActor *obj;

	obj = Engine_ObjectCreate(type, x, y, z);
	if (obj != 0)
	{
		((struct FieldSpritePriority *)obj->sprite)->priority = 0;
		ConfigureOverlayObject(obj, 14);
		Engine_ObjectSetBlendMode(obj, 1);
		return obj;
	}
	return 0;
}

static void *OverlayObject_PrepareObjectWithCommand15(s32 x, s32 y, s32 z, s32 type)
{
	struct FieldActor *object;

	object = Engine_ObjectCreate(type, x, y, z);
	if (object != 0)
	{
		((struct FieldSpritePriority *)object->sprite)->priority = 1;
		ConfigureOverlayObject(object, 15);
		object->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
		return object;
	}
	return 0;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/effect/move.c"
#include "../common/effect/spawn.c"
#include "ability_ids.h"
#include "objcmd.h"

static s32 FixedPoint_Distance(s32 *first_position, s32 *second_position)
{
	s32 delta_x = (*first_position++ - *second_position++) >> 16;
	s32 delta_y = (*first_position++ - *second_position++) >> 16;
	s32 delta_z = (*first_position - *second_position) >> 16;
	s32 delta_x_squared = delta_x * delta_x;
	s32 delta_y_squared = delta_y * delta_y;
	s32 delta_z_squared = delta_z * delta_z;

	return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}

static void *SceneData_FindSlotAtPosition(void *position, void *self)
{
	s32				   *pos = position;
	struct FieldActor **slots = gEventWork->actors;
	u32					i;

	for (i = 8; i <= 65; i++)
	{
		struct FieldActor *p = slots[i];

		if ((pos[0] >> 20) == (p->x.fixed >> 20) && (pos[1] >> 20) == (p->y.fixed >> 20)
			&& (pos[2] >> 20) == (p->z.fixed >> 20))
		{
			return p;
		}
	}
	return 0;
}

static void RunStagedActorTransition(void)
{
	s32					target_position[3];
	struct StagedActor *leader;
	struct StagedActor *actor;
	struct StagedActor *blocking_actor;
	s32					direction_index;
	u32					packed_step;
	s32					move_rate;
	s32					transition_busy;

	leader = Object_GetById(ACTOR_PARTY_LEADER);
	direction_index = leader->direction_and_kind >> 12;
	packed_step = StagedActor_DirectionSteps[direction_index];
	target_position[0] = leader->x.value + (packed_step & 0xffff0000);
	target_position[1] = leader->y;
	packed_step <<= 16;
	target_position[2] = leader->z.value + packed_step;
	actor = SceneData_FindSlotAtPosition(target_position, leader);
	if (actor == 0)
		return;

	packed_step = StagedActor_DirectionSteps[direction_index];
	target_position[0] = actor->x.value + (packed_step & 0xffff0000);
	target_position[1] = actor->y;
	packed_step <<= 16;
	target_position[2] = actor->z.value + packed_step;
	blocking_actor = SceneData_FindSlotAtPosition(target_position, actor);
	if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0)
		return;

	target_position[0] = actor->x.value;
	target_position[1] = actor->y + 0x100000;
	target_position[2] = actor->z.value;
	blocking_actor = SceneData_FindSlotAtPosition(target_position, actor);
	if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0)
		return;

	actor->transition_mode = 2;
	packed_step = StagedActor_DirectionSteps[direction_index];
	target_position[0] = actor->x.value + (packed_step & 0xffff0000);
	target_position[1] = actor->y;
	packed_step <<= 16;
	target_position[2] = actor->z.value + packed_step;
	if (Object_CheckMovementCollision(actor, target_position) > 0)
		return;

	transition_busy = actor->transition_busy;
	if (transition_busy != 0)
		return;

	Object_SetMode(leader, 8);
	move_rate = 0x3333;
	Engine_TaskWait(15);
	Engine_AudioPlayCue(185);
	actor->move_rate_x = move_rate;
	actor->move_rate_z = move_rate;
	Engine_ObjectSetPosition(actor, target_position[0], target_position[1], target_position[2]);
	leader->move_rate_x = move_rate;
	leader->move_rate_z = move_rate;
	Engine_ObjectSetPosition(leader, target_position[0], target_position[1], target_position[2]);
	Engine_ObjectCommitPosition(actor);
	BattleFx_PlayQueuedSound();
	actor->x.value = target_position[0];
	actor->z.value = target_position[2];
	actor->velocity_x = transition_busy;
	actor->velocity_z = transition_busy;
	leader->target_x = ACTOR_NO_TARGET;
	leader->target_z = ACTOR_NO_TARGET;
	leader->velocity_x = transition_busy;
	leader->velocity_z = transition_busy;
	leader->x.value = leader->x.parts.cell << 16;
	leader->z.value = leader->z.parts.cell << 16;
	Object_SetMode(leader, 1);
}

static s32 StagedActor_FillGridAttributeRectangle(
	u32 layer, s32 x, s32 z, u32 width, u32 height, s32 value)
{
	struct MapState *map = gMapWork[0];
	struct MapCell	*cells;
	u32				 row;
	u32				 column;

	if (map != 0)
	{
		if (layer <= 2)
		{
			cells = map->layers[layer].cells;
		}
		else
		{
			cells = (struct MapCell *)gMapCellBuffer;
		}
		cells += x + (z << 7);
		for (row = 0; row < height; row++)
		{
			struct MapCell *cell = cells + (row << 7);

			for (column = 0; column < width; column++)
			{
				cell->collision_code = value;
				cell++;
			}
		}
	}
	return 0;
}

static void SceneActor_ApplySlotsMatchingKind212(void)
{
	struct FieldActor **actors = gEventWork->placed_actors;
	struct FieldActor  *actor;
	u32					i;

	for (i = 8; i <= 65; i++)
	{
		actor = actors[i - 8];
		if (actor->timer >> 4 == 0x212)
			ObjectGroup_SetChildValue(actor, actor->timer & 15);
	}
}

static s32 VinasuHeya_UpdateChildFromTimer(struct FieldActor *obj)
{
	ObjectGroup_SetChildValue(obj, obj->timer & 15);
	return 0;
}

static s32 OverlayObject_UpdateEveryFourFrames(void *obj)
{
	if ((gFrameCount & 3) == 0)
		ObjectGroup_SetChildValue(obj, 7);
	else
		ObjectGroup_SetChildValue(obj, 0);

	if ((gFrameCount & 7) == 0)
		Engine_AudioPlayCue(138);
	return 0;
}

static s32 SceneEffect_SpawnRandomEveryEightFramesB(struct FieldActor *object)
{
	struct EffectOptions params;
	s32					 phase, x, y, speed;
	phase = gFrameCount & 7;
	if (phase != 0)
		goto done;
	params.priority = 3 - ((u32)(Engine_RandomNext() * 2) >> 16);
	params.start_scale_x = 0x6666;
	params.start_scale_y = 0x6666;
	params.palette = 14;
	x = object->x.fixed + ((((u32)(Engine_RandomNext() * 9) >> 16) - 4) << 16);
	y = object->y.fixed + ((32 - ((u32)(Engine_RandomNext() * 32) >> 16)) << 16);
	speed = Math_Divide((((u32)(Engine_RandomNext() * 5) >> 16) << 16) + 0x00050000, 10);
	Effect_Spawn(x, y, object->z.fixed, 0, speed, phase, 0x000b0000, &params);
done:
	return 0;
}

static s32 OverlayObject_ApplyZero(void *obj)
{
	Engine_ActorSetSpriteFlags(obj, 0);
	return 0;
}

static s32 SceneEffect_SpawnTwoRandomizedParticles(struct FieldActor *obj)
{
	struct EffectOptions params;
	s32					 speed;
	s32					 phase;

	params.start_scale_x = 0x0000cccc;
	params.start_scale_y = 0x0000cccc;
	params.priority = 0;

	speed = ((u32)(Engine_RandomNext() * 8) >> 16) * 0x3333;
	phase = gFrameCount & 15;

	Effect_Spawn(obj->x.fixed + ((8 - phase) << 16), obj->y.fixed + 0x001a0000, obj->z.fixed, 0,
		-speed, 0, 0x000a0000, &params);

	phase = gFrameCount & 15;
	if (phase == 0)
	{
		params.start_scale_x = 0x00008000;
		params.start_scale_y = 0x00008000;
		Effect_Spawn(obj->x.fixed + ((((u32)(Engine_RandomNext() * 9) >> 16) - 4) << 16),
			obj->y.fixed, obj->z.fixed, 0, 0, 0, 0x000a0000, &params);
	}

	return 0;
}

static void SceneEffect_RunObjectZeroColorSequence(void)
{
	struct FieldActor *obj;
	struct EventWork  *state;

	state = gEventWork;
	obj = Object_GetById(ACTOR_PARTY_LEADER);
	Engine_EventBegin();
	Engine_AudioPlayCue(228);
	obj->update = (void (*)(union FieldObject *))SceneEffect_SpawnTwoRandomizedParticles;
	obj->speed = 0x3333;
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
	Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
	Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
	obj->update = 0;
	Engine_EventWait(30);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(state->touched_trigger);
	Engine_EventEnd();
}

static void FieldScene_RunOpeningAuxiliarySequence(void)
{
	struct FieldActor *leader;

	leader = Actor_Get(ACTOR_PARTY_LEADER);
	if (GameFlag_IsSet(0x109) == 0)
	{
		Engine_EventBegin();
		Camera_MoveTo(-1, -1, -1, 0);
		leader->motion_flags = 0;
		Actor_SetPosition(ACTOR_PARTY_LEADER, leader->x.part.pixel << 16,
			(leader->z.part.pixel << 16) - 0x100000);
		Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
		Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
		Engine_EventOpenScreen();
		Engine_EventWaitForScreen();
		Event_PlayCue(228);
		leader->update = (void (*)(union FieldObject *))SceneEffect_SpawnTwoRandomizedParticles;
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
		Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
		Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
		Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 1);
		Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
		leader->motion_flags = 3;
		leader->update = 0;
		BattleFx_PlayQueuedSound();
		Engine_EventEnd();
	}
}

static void EV32_TOUCH_3(void)
{
	*(s32 *)(gActorEffectWork + 24) = Object_GetById(ACTOR_PARTY_LEADER);
}

static void EV31_TOUCH_3(void)
{
	*(s32 *)(gActorEffectWork + 24) = 0;
}

static s32 SceneActor_SetFlagBitByRelativeDepth(struct FieldActor *actor)
{
	struct FieldActor *ref;
	u8				  *fp;
	u8				   flags;
	ref = Object_GetById(ACTOR_PARTY_LEADER);
	fp = &actor->priority_flags;
	flags = *fp | 2;
	*fp = flags;
	if (ref->z.fixed < actor->z.fixed)
	{
		s32 diff = actor->z.fixed - ref->z.fixed;
		s32 lim;
		diff += 0x00040000;
		lim = actor->y.fixed;
		lim += diff;
		if (ref->y.fixed <= lim)
		{
			flags &= 0xfd;
			*fp = flags;
		}
	}
	return 0;
}

static void SceneActor_SwapPositionsByDepth(s32 group, s32 index)
{
	struct FieldActor *first;
	struct FieldActor *second;
	s32				   value;

	first = Object_GetById(group);
	second = Object_GetById(index);
	if (first->z.fixed <= second->z.fixed)
	{
		value = first->x.fixed;
		first->x.fixed = second->x.fixed;
		second->x.fixed = value;

		value = first->y.fixed;
		first->y.fixed = second->y.fixed;
		second->y.fixed = value;

		value = first->z.fixed;
		first->z.fixed = second->z.fixed;
		second->z.fixed = value;
		Engine_TaskWait(1);
	}
}

static void OverlayObject_WaitUntilIdle(struct FieldActor *obj)
{
	s32 cnt;
	s32 busy;

	cnt = 60;
	while (cnt != 0)
	{
		Engine_TaskWait(1);
		busy = obj->velocity_y;
		cnt--;
		if (busy == 0)
			break;
	}
}

static void Effect_AdvanceMotion(union FieldObject *object)
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

static void FieldScene_RunSupplementalSequenceOne(void)
{
	struct FieldActor	*actor;
	u32					 i;
	struct EffectOptions options;

	actor = Actor_Get(ACTOR_PARTY_LEADER);
	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	actor->y.fixed = 0x820000;
	actor->gravity = 0x8000;
	actor->bounce = 0;
	actor->motion_flags = 0;
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(30);
	Event_PlayCue(204);
	actor->motion_flags = 3;
	Engine_EventWait(24);
	options.palette = 7;
	options.update = (void (*)(union FieldObject *))Effect_AdvanceMotion;
	options.start_scale_x = 0xcccc;
	options.start_scale_y = 0xcccc;
	for (i = 0; i <= 16; i++)
	{
		s32 angle;
		s32 velocity[3];

		angle = i << 12;
		velocity[0] = Engine_MathCos(angle);
		velocity[1] = 0;
		velocity[2] = Engine_MathSin(angle);
		velocity[0] += velocity[0] / 2;
		Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, velocity[0], velocity[1],
			velocity[2], 0x1090001, &options);
	}
	Event_PlayCue(188);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 22);
	Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_MapRenderWaitForValues();
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x100);
	BattleFx_PlayQueuedSound();
	actor->gravity = 0x10000;
	actor->bounce = 0x4000;
	Engine_EventEnd();
}

static void SceneEffect_SpawnNineRadialEffects(s32 actor)
{
	struct FieldActor	*object;
	struct Vec			 vec;
	struct EffectOptions params;
	u32					 i;
	s32					 v;
	s32					 x;
	s32					 z;

	object = Object_GetById(actor);
	params.priority = 1;
	params.palette = 7;
	params.update = Effect_AdvanceMotion;
	for (i = 0; i <= 16; i += 2)
	{
		v = i << 12;
		vec.x = Engine_MathCos(v);
		vec.y = 0;
		z = Engine_MathSin(v);
		x = vec.x;
		vec.z = z;
		x += Math_Divide(x, 3);
		vec.x = x;
		Effect_Spawn(
			object->x.fixed, object->y.fixed, object->z.fixed, x, vec.y, z, 0x01030001, &params);
	}
}

static s32 SceneEffect_SpawnRandomizedParticleEveryFourFrames(struct FieldActor *obj)
{
	struct EffectOptions params;
	s32					 y;
	s32					 a;
	s32					 b;
	s32					 rnd;

	if ((gFrameCount & 3) != 0)
	{
		return 0;
	}
	if ((u32)(Engine_RandomNext() * 6) >> 16 == 0)
	{
		if (obj->target_x != ACTOR_NO_TARGET || obj->target_z != ACTOR_NO_TARGET)
		{
			Engine_AudioPlayCue(246);
		}
	}
	y = 0;
	params.type = 286;
	params.start_scale_x = 0x10000;
	params.start_scale_y = 0x10000;
	params.target_scale_x = -327;
	params.target_scale_y = -327;
	rnd = Engine_RandomNext();
	a = Math_Divide(((((u32)(rnd * 9)) >> 16) - 4) << 16, 10);
	rnd = Engine_RandomNext();
	b = Math_Divide(((((u32)(rnd * 9)) >> 16) - 4) << 16, 10);
	Effect_Spawn(obj->x.fixed, obj->y.fixed, obj->z.fixed - 0x10000, a, y, b, 0x001c0001, &params);
	return 0;
}

static struct FieldActor *SceneEffect_SpawnEffect284AtCell(s32 x, s32 z, s32 arg2)
{
	struct FieldActor *obj;
	s32				   sx;
	s32				   sz;

	sx = x << 16;
	sz = z << 16;
	obj = Engine_ObjectCreate(284, sx, 0, sz);
	if (obj == 0)
	{
		return 0;
	}
	obj->speed = 0x10000;
	obj->acceleration = 0x10000;
	Engine_ActorSetSpriteFlags(obj, 0);
	Object_SetMode(obj, 7);
	obj->motion_flags = 0;
	obj->y.fixed = 0;
	obj->collision_flags = 0;
	obj->priority_flags = 2;
	obj->update = (void (*)(union FieldObject *))SceneEffect_SpawnRandomizedParticleEveryFourFrames;
	obj->rise_enabled = 0;
	Engine_ObjectSetScript(obj, arg2);
	return obj;
}

static s32 SceneActor_TryMoveActorZeroTwoTilesAhead(void)
{
	struct FieldActor *obj;
	struct Vec		   vec;
	u8				  *state;
	u8				   old;
	s32				   m;
#if !EDITION_INTERNATIONAL
	u32 step;
#endif

	obj = Object_GetById(ACTOR_PARTY_LEADER);
	state = &obj->motion_flags;
	old = *state;
#if EDITION_INTERNATIONAL
	vec.x = (obj->x.fixed & 0xfff00000) + 0x80000;
	vec.y = obj->y.fixed;
	vec.z = (obj->z.fixed & 0xfff00000) + 0x80000;
	m = (obj->facing + 0x2000) & 0xc000;
	Vector_AddPolarOffset(0x100000, m, &vec);
#else
	m = obj->facing >> 12;
	step = StagedActor_DirectionSteps[m];
	vec.x = obj->x.fixed + (step & 0xffff0000);
	vec.y = obj->y.fixed;
	step <<= 16;
	vec.z = obj->z.fixed + step;
#endif
	if (Object_CheckMovementCollision(obj, &vec) != 1
		&& SceneData_FindSlotAtPosition(&vec, obj) == 0)
	{
#if EDITION_INTERNATIONAL
		vec.x = (obj->x.fixed & 0xfff00000) + 0x80000;
		vec.y = obj->y.fixed;
		vec.z = (obj->z.fixed & 0xfff00000) + 0x80000;
		Vector_AddPolarOffset(0x200000, (obj->facing + 0x2000) & 0xc000, &vec);
#else
		step = StagedActor_DirectionSteps[m];
		vec.x = obj->x.fixed + ((step & 0xffff0000) << 1);
		vec.y = obj->y.fixed;
		step <<= 17;
		vec.z = obj->z.fixed + step;
#endif
		if (SceneData_FindSlotAtPosition(&vec, obj) == 0
			&& Object_CheckMovementCollision(obj, &vec) == 0)
		{
			Engine_EventBegin();
			Object_SetMode(obj, 6);
			Engine_TaskWait(6);
			Engine_AudioPlayCue(152);
			Object_SetMode(obj, 7);
			obj->speed = 0x30000;
			obj->acceleration = 0x20000;
			obj->velocity_y = 0x40000;
			*state &= 0x7e;
			Engine_ActorSetSpriteFlags(obj, 0);
			Engine_ActorMoveToAndWait(
				ACTOR_PARTY_LEADER, ((union VecView *)&vec)->h[1], ((union VecView *)&vec)->h[5]);
			Object_SetMode(obj, 6);
			Engine_ActorSetSpriteFlags(obj, 1);
			*state = old;
			Engine_EventEnd();
			return 1;
		}
	}
	return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 v = gGameState.scene;

	if (v == (s32)&SceneId_VinasuHeya1)
	{
		return gVinasuHeyaEntrances1;
	}
	if (v == (s32)&SceneId_VinasuHeya3)
	{
		return gVinasuHeyaEntrances3;
	}
	if (v == (s32)&SceneId_VinasuHeya4)
	{
		return gVinasuHeyaEntrances4;
	}
	if (v == (s32)&SceneId_VinasuHeya5)
	{
		return gVinasuHeyaEntrances5;
	}
	if (v == (s32)&SceneId_VinasuHeya6)
	{
		return gVinasuHeyaEntrances6;
	}
	return gVinasuHeyaEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetPrimaryTable(void)
{
	return gVinasuHeyaPrimaryTable;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	u8 *ret;
	s16 v;

	v = gGameState.scene;
	if (v == (s32)&SceneId_VinasuHeya1)
	{
		return gVinasuHeyaPlacements1;
	}
	if (v == (s32)&SceneId_VinasuHeya2)
	{
		ret = gVinasuHeyaPlacements2;
	}
	else if (v == (s32)&SceneId_VinasuHeya3)
	{
		ret = gVinasuHeyaPlacements3;
	}
	else if (v == (s32)&SceneId_VinasuHeya4)
	{
		ret = gVinasuHeyaPlacements4;
	}
	else if (v == (s32)&SceneId_VinasuHeya5)
	{
		ret = gVinasuHeyaPlacements5;
	}
	else if (v == (s32)&SceneId_VinasuHeya6)
	{
		ret = gVinasuHeyaPlacements6;
	}
	else
	{
		goto no_match;
	}
	FieldScene_PrepareActors((void *)ret);
	return (const struct ScenePlacement *)ret;

no_match:
	return gVinasuHeyaPlacementsOther;
}

static void EV25_SEARCH(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgFieldDoorTightlyLocked, 1);
	Engine_EventEnd();
}

static void FieldScene_RunActorEightTenStepLoop(void)
{
	u32 n;
	u32 w;
	s32 a;
	s32 b;

	Engine_EventBegin();
	Engine_ActorRunRepeatedMotion(8, 3);
	Engine_EventSetMessage((s32)MsgFieldVenusLighthouseWasAttackedBy);
	n = 10;
	w = 8;
	Engine_EventShowMessageAndWait(8, 0, 20);
	do
	{
		Engine_ActorSetChildValue(8, 15);
		Engine_TaskWait(2);
		Engine_ActorSetChildValue(8, 0);
		Engine_TaskWait(w);
		if (w > 3)
		{
			w--;
		}
		n--;
	} while (n != 0);
	Engine_GameFlagSet(0x981);
	Engine_ActorSetPosition(8, 0, 0);
	a = 7;
	b = 16;
	Engine_MapCopyCellAttributes(7, 17, 2, 1, a, b);
	Engine_EventEnd();
}

static void EV11_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgVinasuIveWaitedLongSeeIts);
	Engine_EventShowMessageAndWait(11, 0, 20);
	Engine_ActorRunRepeatedMotion(11, 2);
	Engine_EventShowMessage(11, 0);
	Engine_EventEnd();
}

static void FieldScene_SetFlag987AtActorTwelveTile(void)
{
	struct FieldActor *s;

	s = Object_GetById(12);
	Engine_EventBegin();
	if (s->x.fixed >> 20 == 54 || s->z.fixed >> 20 == 6)
	{
		Engine_GameFlagSet(0x987);
	}
	Engine_EventEnd();
}

static void SceneDialogue_ReadRelief(void)
{
#if defined(TBS_EDITION_EN)
	Engine_EventBegin();
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_MessageShowCentered((s32)MSGNO(MsgVinasuThereWordsCarvedIntoRelief), 1);
	Engine_EventEnd();
#else
	struct EventWork *work = gEventWork;

	Engine_EventBegin();
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_MessageShowCentered(
		work->facing_trigger + MSGNO(MsgVinasuThereWordsCarvedIntoRelief) - 35, 1);
	Engine_EventEnd();
#endif
}

static void SceneState_ApplySixRectsAfter161(void)
{
	s32 x;
	s32 y;
	s32 a;
	s32 b;

#if EDITION_INTERNATIONAL
	Engine_GameFlagClear(0x161);
#endif
	x = 23;
	y = 8;
	Engine_MapCopyCellAttributes(35, 8, 1, 3, x, y);
	a = 3;
	b = 1;
	Engine_MapCopyCellsTo(35, 8, 23, 8, b, a);
	Engine_MapCopyCellsTo(99, 8, 87, 8, b, a);
	x = 46;
	y = 55;
	Engine_MapCopyCellAttributes(57, 55, 3, 3, x, y);
	Engine_MapCopyCellsTo(57, 55, 46, 55, a, a);
	Engine_MapCopyCellsTo(121, 55, 110, 55, a, a);
}

static void SceneState_ApplySixRectsAfterFlag161(void)
{
	s32 x;
	s32 y;
	s32 a;
	s32 b;

#if EDITION_INTERNATIONAL
	Engine_GameFlagSet(0x161);
#endif
	x = 23;
	y = 8;
	Engine_MapCopyCellAttributes(36, 8, 1, 3, x, y);
	a = 3;
	b = 1;
	Engine_MapCopyCellsTo(36, 8, 23, 8, b, a);
	Engine_MapCopyCellsTo(100, 8, 87, 8, b, a);
	x = 46;
	y = 55;
	Engine_MapCopyCellAttributes(53, 55, 3, 3, x, y);
	Engine_MapCopyCellsTo(53, 55, 46, 55, a, a);
	Engine_MapCopyCellsTo(117, 55, 110, 55, a, a);
}

static void EV45_TOUCH(void)
{
	struct FieldActor *actor;
	s32				   z;

	actor = Actor_Get(ACTOR_PARTY_LEADER);
	Engine_EventBegin();
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, gVinasuLeaderApproachScript);
	Object_RefreshSelectorById(0);
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 6);
	actor->velocity_y = 0x40000;
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x40000, 0x20000);
	if (actor->z.fixed >> 20 <= 54)
	{
		Actor_Get(ACTOR_PARTY_LEADER)->steering_flags &= ~ACTOR_FACES_MOTION;
		z = 210;
	}
	else
	{
		Actor_Get(ACTOR_PARTY_LEADER)->steering_flags &= ~ACTOR_FACES_MOTION;
		z = 238;
	}
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, actor->x.part.pixel, z << 2);
	Engine_EventWait(1);
	SetFlagBits(&Actor_Get(ACTOR_PARTY_LEADER)->steering_flags, 1);
	Engine_EventWait(20);
	actor->update = (void (*)(union FieldObject *))SceneEffect_SpawnRandomEveryEightFramesB;
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
	actor->update = 0;
	Engine_EventEnd();
}

static void EV41_SEARCH(void)
{
	struct EventWorkState *work;

	work = (struct EventWorkState *)gEventWork;
	work->field_cba = 0;
	work->field_cb6 = 1;
	Engine_EventBegin();
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_MessageShowCentered((s32)MsgVinasuStatueSpeaksRobinSoulYe, 1);
	ColorBuffer_ApplySource(0x10000, 0);
	ColorBuffer_ApplyTarget(0x10005, 0);
	Engine_ColorBufferInterpolate(120);
	Engine_EventWait(100);
	Event_PlayCue(142);
	Engine_EventWait(30);
	ColorBuffer_ApplyTarget(0x7fff, 0);
	Engine_ColorBufferInterpolate(60);
	Engine_EventWait(70);
	if (GameFlag_IsSet(0x982) == 0)
	{
		if (GameFlag_IsSet(0x983) == 0)
		{
			if ((gFrameCount & 1) != 0)
			{
				GameFlag_Set(0x982);
			}
			else
			{
				GameFlag_Set(0x983);
			}
		}
	}
	if (GameFlag_IsSet(0x982) == 0)
	{
		GameFlag_Set(0x982);
		GameFlag_Clear(0x983);
		Map_CopyCellsTo(103, 27, 89, 27, 7, 8);
		Map_CopyCellsTo(41, 90, 27, 92, 3, 2);
		Map_CopyCellsTo(41, 90, 29, 93, 3, 2);
		Map_CopyCellsTo(41, 90, 27, 94, 3, 2);
		Map_CopyCellsTo(41, 90, 27, 96, 3, 2);
		Map_CopyCellsTo(41, 90, 29, 97, 3, 2);
		Map_CopyCellsTo(41, 96, 25, 91, 3, 2);
		Map_CopyCellsTo(41, 92, 25, 93, 3, 2);
		Map_CopyCellsTo(41, 96, 25, 95, 3, 2);
		Map_CopyCellsTo(41, 96, 25, 97, 3, 2);
		Map_CopyCellsTo(41, 96, 27, 96, 3, 2);
		Map_CopyCellsTo(41, 96, 29, 97, 3, 2);
	}
	else
	{
		GameFlag_Set(0x983);
		GameFlag_Clear(0x982);
		Map_CopyCellsTo(111, 27, 89, 27, 7, 8);
		Map_CopyCellsTo(41, 90, 25, 91, 3, 2);
		Map_CopyCellsTo(41, 90, 25, 93, 3, 2);
		Map_CopyCellsTo(41, 90, 25, 95, 3, 2);
		Map_CopyCellsTo(41, 90, 25, 97, 3, 2);
		Map_CopyCellsTo(41, 90, 27, 96, 3, 2);
		Map_CopyCellsTo(41, 90, 29, 97, 3, 2);
		Map_CopyCellsTo(41, 94, 27, 92, 3, 2);
		Map_CopyCellsTo(41, 96, 29, 93, 3, 2);
		Map_CopyCellsTo(41, 94, 27, 94, 3, 2);
		Map_CopyCellsTo(41, 96, 27, 96, 3, 2);
		Map_CopyCellsTo(41, 96, 29, 97, 3, 2);
	}
	ColorBuffer_ApplyTarget(0x10000, 0);
	Engine_ColorBufferInterpolate(20);
	Engine_EventWait(40);
	Camera_SetSpeed(0x8000, 0x1000);
	Camera_MoveTo(0x1c80000, -1, 0x21e0000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(50);
	Camera_MoveTo(0x1c80000, -1, 0x1a70000, 1);
	Engine_CameraWaitForMove();
	Engine_EventEnd();
	work->field_cb6 = 0;
}

static void FieldScene_RunFlag986ActorOneScene(void)
{
	struct FieldActor *o;
	struct FieldActor *u;
	s32				   g;
	s32				   m1;
	s32				   m2;
	s32				   h;
	s32				   k;

	g = 0x986;
	m1 = 0xcccc;
	m2 = 0x6666;
	h = 0x100;
	k = 0x338;
	Engine_EventBegin();
	o = Object_GetById(12);
	if (o->x.fixed >> 20 == 53)
	{
		if (Engine_GameFlagIsSet(g) == 0)
		{
			Engine_GameFlagSet(g);
			o = Object_GetById(ACTOR_PARTY_LEADER);
			if (o != 0)
			{
				Engine_ActorSetPosition(GERALD, o->x.fixed, o->z.fixed);
			}
			Engine_ActorSetSpeed(GERALD, m1, m2);
			Engine_ActorWalkToAndWait(GERALD, k, 88);
			Engine_ActorWalkToAndWait(GERALD, k, 104);
			Engine_ActorFaceEachOther(GERALD, ACTOR_PARTY_LEADER, 0);
			Engine_EventWait(20);
			Engine_ActorSetAnimationAndWait(GERALD, 4);
			Engine_EventWait(20);
			Engine_EventSetMessage((s32)MsgVinasuHmmmWeCantPushBlock);
			Engine_EventShowMessageAndWait(GERALD, 0, 20);
			Engine_ActorFaceDirection(GERALD, 0, 10);
			Engine_ActorShowEmote(GERALD, h, 60);
			Engine_ActorFaceActor(GERALD, ACTOR_PARTY_LEADER, 0);
			Engine_EventWait(20);
			Engine_ActorRunRepeatedMotion(GERALD, 2);
			Engine_EventWait(20);
			Engine_EventShowMessageAndWait(GERALD, 0, 20);
			Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
			Engine_ActorSetAnimationAndWait(GERALD, 3);
			Engine_EventWait(30);
			Engine_ActorWalkToAndWait(GERALD, k, 88);
			Engine_ActorSetAnimation(GERALD, 2);
			u = Object_GetById(ACTOR_PARTY_LEADER);
			if (u != 0)
			{
				Engine_ActorSetDestination(GERALD, u->x.part.pixel, u->z.part.pixel);
			}
			Engine_ActorWaitForMove(GERALD);
			Engine_ActorSetPosition(GERALD, 0, 0);
			Engine_EventEnd();
		}
	}
}

static void EV50_SCRIPT(void)
{

	Engine_EventBegin();
	RunStagedActorTransition();
	Engine_EventWait(20);
	Engine_EventEnd();
	FieldScene_RunFlag986ActorOneScene();
}

static void EV13_PSYNERGY(void)
{
	struct FieldActor *obj;
	s32				   val;
	s32				   a;
	s32				   b;

	obj = Object_GetById(13);
	Engine_EventBegin();
	if (obj->x.fixed >> 20 == 42)
	{
		Engine_EventWait(30);
		Engine_AudioPlayCue(188);
		obj->motion_flags = 0;
		val = 0xfffe0000;
		obj->terrain_height = val;
		obj->y.fixed = val;
		Engine_GameFlagSet(0x200);
		a = 3;
		b = 5;
		Engine_MapCopyCellsTo(44, 117, 41, 117, a, b);
	}
	Engine_EventEnd();
}

static void EV46_TOUCH(void)
{
	struct FieldActor *leader = Object_GetById(0);
	s32				   x = leader->x.part.pixel;
	s32				   z = leader->z.part.pixel;

	if ((u32)(x - 676) > 7 || z < 788 || z >= 796)
	{
		Map_CopyCellsTo(53, 50, 42, 49, 1, 1);
		Map_CopyCellsTo(55, 117, 41, 117, 3, 5);
		GameFlag_Clear(0x201);
		leader->motion_flags |= 1;
		leader->terrain_height = 0;
		leader->y.fixed = 0;
	}
	else if (!GameFlag_IsSet(0x201))
	{
		Engine_EventBegin();
		Engine_EventWait(5);
		Map_CopyCellsTo(52, 50, 42, 49, 1, 1);
		Map_CopyCellsTo(52, 117, 41, 117, 3, 5);
		GameFlag_Set(0x201);
		Engine_AudioPlayCue(161);
		leader->motion_flags &= ~1;
		leader->terrain_height = -0x20000;
		leader->y.fixed = -0x20000;
		Engine_EventEnd();
	}
}

static void EV10_TALK(void)
{
	struct EventWorkState *work;

	work = (struct EventWorkState *)gEventWork;
	work->field_cba = 0;
	work->field_cb6 = 1;
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgVinasuThoughtIdExploreAfterDoor);
	Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(10, 0, 20);
	Actor_FaceDirection(10, 57344, 0);
	Camera_SetSpeed(65536, 8192);
	Camera_MoveTo(29360128, -1, 28311552, 1);
	Engine_CameraWaitForMove();
	Event_ShowMessage(10, 0);
	Engine_EventEnd();
}

static void EV35_TOUCH_3(void)
{
	struct FieldActor *rec7;
	s32				   record;
	s32				   zero;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
	u8 *state;
#endif

#if !defined(TBS_EDITION_ES) && !defined(TBS_EDITION_FR) && !defined(TBS_EDITION_IT)
	zero = 0;
#endif
	rec7 = Object_GetById(0);
	Engine_EventBegin();
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
	state = (u8 *)&rec7->motion_flags;
	zero = 0;
	*state = zero;
#endif
	Camera_MoveTo(-1, -1, -1, 0);
	record = (s32)Object_GetById(0);
	Engine_ActorSetSpriteFlags(record, 0);
	{
		u16 shown = 0x4000;

		rec7->facing = shown;
	}
	Actor_SetSpeed(0, 0x30000, 0x18000);
	Engine_ActorMoveToAndWait(0, rec7->x.part.pixel, 0x228);
	Engine_EventWait(10);
	Engine_ActorSetAnimation(0, 22);
	Engine_EventWait(30);
	Actor_SetAttachedEffect(0, 0x102);
	Engine_ActorRunRepeatedMotion(0, 2);
	Engine_EventWait(20);
	{
		s32 shown = 0xc000;

		rec7->facing = shown;
	}
	Engine_ActorSetAnimation(0, 5);
	Object_SetActionById(0, 24);
	Engine_EventWait(40);
	rec7->gravity = 0x9999;
	{
		s32 z = rec7->z.fixed + 0x480000;

		rec7->bounce = zero;
		OverlayObject_SpawnWithMode14(rec7->x.fixed, 0, z, 223);
	}
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
	Map_CopyCellAttributes(33, 35, 7, 1, 33, 34);
	*state = 3;
#else
	Engine_MapCopyCellAttributes(34, 35, 5, 1, 34, 34);
#endif
	OverlayObject_WaitUntilIdle(0);
	Engine_ActorSetChildValue(0, 15);
	Engine_EventRequestExit(20);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventEnd();
}

static void FieldScene_PlaceAndPinSlots8To10(void)
{

	u32				   i;
	struct FieldActor *rec;
	s32				   x;
	s32				   y;
	s32				   a;
	s32				   b;

	Object_GetById(8);
	Engine_EventBegin();
	x = 12;
	y = 44;
	Engine_MapCopyCellAttributes(19, 44, 4, 1, x, y);
	x = 11;
	y = 51;
	Engine_MapCopyCellAttributes(17, 51, 2, 2, x, y);
	i = 0;
	do
	{
		rec = Object_GetById(i + 8);
		a = rec->x.fixed >> 20;
		b = rec->z.fixed >> 20;
		Engine_MapCopyCellAttributes(12, 50, 1, 1, a, b);
		i++;
	} while (i <= 2);
	SceneActor_SwapPositionsByDepth(10, 9);
	Engine_EventEnd();
}

static void SceneState_ApplyRectAt19_44AndRunThree(void)
{
	s32 x;
	s32 y;

	Engine_EventBegin();
	x = 12;
	y = 44;
	Engine_MapCopyCellAttributes(19, 44, 4, 1, x, y);
	RunStagedActorTransition();
	FieldScene_PlaceAndPinSlots8To10();
	Engine_EventEnd();
}

static void SceneActor_ApplyKind45AtActorsElevenAndTwelve(void)
{
	u32				   i;
	struct FieldActor *p;

	i = 0;
	do
	{
		p = Object_GetById(i + 11);
		i++;
		SetMapCellCollision(0, p->x.fixed, p->z.fixed, 45);
	} while (i <= 1);
}

static void SceneActor_ApplyPositionsOfActors11And12(void)
{
	u32				   i;
	struct FieldActor *p;

	i = 0;
	do
	{
		p = Object_GetById(i + 11);
		if (p->y.fixed > -0x100000)
		{
			SetMapCellCollision(0, p->x.fixed, p->z.fixed, 255);
		}
		i++;
	} while (i <= 1);
}

static void EV45_SCRIPT_3(void)
{

	Engine_EventBegin();
	if (SceneActor_TryMoveActorZeroTwoTilesAhead() == 0)
	{
		SceneActor_ApplyKind45AtActorsElevenAndTwelve();
		RunStagedActorTransition();
		SceneActor_ApplyPositionsOfActors11And12();
	}
	Engine_EventEnd();
}

static void SceneState_MarkActorAndApplyRectAtTile(struct FieldActor *obj)
{
	s32 x;
	s32 z;

	obj->priority_flags |= 2;
	obj->motion_flags = 0;
	x = obj->x.fixed >> 20;
	z = obj->z.fixed >> 20;
	Engine_MapCopyCellAttributes(9, 24, 1, 1, x, z);
}

static void OverlayObject_ResetObjectWhenFlatbs2Set(struct FieldActor *o)
{
	Struct_Sub *q;
	s32			v;
	s32			z;
	s32			t;
	s32			m;

	q = (Struct_Sub *)o->sprite;
	v = q->unk9;
	if ((v & 12) == 12)
	{
		m = -13;
		m &= v;
		m |= 4;
		{
			u8 *pq = &q->unk9;
			*pq = m;
		}
		z = 0;
		o->bounce = z;
		t = OverlayObject_SpawnWithMode14(o->x.fixed, 0, 0x2000000, 223);
		OverlayObject_WaitUntilIdle(o);
		o->x.fixed = z;
		o->z.fixed = z;
		Engine_ObjectDispatchRelease(t);
	}
	else
	{
		SceneActor_ApplyPositionsOfActors11And12();
	}
}

static void SceneActor_UpdateSlots11And12ByTile(void)
{
	struct FieldActor *o;

	Engine_EventBegin();
	o = Object_GetById(11);
	if (o->x.fixed >> 20 == 8)
	{
		OverlayObject_WaitUntilIdle(o);
		SceneState_MarkActorAndApplyRectAtTile(o);
	}
	else
	{
		OverlayObject_ResetObjectWhenFlatbs2Set(o);
	}
	o = Object_GetById(12);
	if (o->x.fixed >> 20 == 7)
	{
		OverlayObject_WaitUntilIdle(o);
		SceneState_MarkActorAndApplyRectAtTile(o);
	}
	else
	{
		OverlayObject_ResetObjectWhenFlatbs2Set(o);
	}
	Engine_EventEnd();
}

struct MapWork
{
	u8				   unknown_00[0x164];
	struct ScrollLayer layer;
};

static void EV35_TOUCH_4(void)
{
	struct EffectOptions options;
	struct ScrollLayer	*layer;
	struct FieldActor	*leader;
	s32					 x;
	s32					 z;
	s32					 countdown;
	u32					 i;
	s32					 dust_x;

	layer = &((struct MapWork *)gMapWork[0])->layer;
	leader = Object_GetById(0);
	x = leader->x.part.pixel;
	z = leader->z.part.pixel;
	leader->y.fixed = 0;
	if (x >= 308 && x <= 315 && z >= 532 && z < 540)
	{
		leader->y.fixed = -0x20000;
		if (!GameFlag_IsSet(0x300))
		{
			Engine_EventBegin();
			Engine_AudioPlayCue(161);
			GameFlag_Set(0x300);
			Engine_MapCopyCellsTo(26, 33, 19, 33, 1, 1);
			Engine_EventWait(30);
			Engine_AudioPlayCue(239);
			Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
			Engine_EventWait(20);
			dust_x = 0x1200000;
			x = 29;
			countdown = 40;
			for (i = 0; i <= 479; i++, countdown--)
			{
				layer->x += 0x3333;
				dust_x += -0x3333;
				options.priority = 2;
				options.start_scale_x = ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
				options.start_scale_y = ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
				options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
				Effect_Spawn(dust_x, 0, 0x2100000, 0, -(((gFrameCount & 1) * 3) << 16), 0, 0x8a0000,
					&options);
				if (i == 240)
				{
					dust_x += -0x300000;
				}
				if (countdown == 0)
				{
					countdown = 40;
					if (i <= 240)
					{
						x -= 4;
						Map_CopyCellsTo(x, 50, 15, 32, 3, 4);
					}
					else
					{
						x += 4;
						Map_CopyCellsTo(x, 45, 9, 32, 3, 4);
					}
				}
				Engine_TaskWait(1);
			}
			layer->x += 0x8000;
			layer->x = layer->x / 0x10000 << 16;
			Engine_MapCopyCellAttributes(15, 32, 3, 1, 9, 32);
			Engine_MapCopyCellAttributes(12, 32, 3, 1, 15, 32);
			Engine_AudioPlayCue(288);
			Engine_AudioPlayCue(188);
			Work_SetValuesIfNonNegative(-1, -1, 0xe666);
			Engine_MapRenderWaitForValues();
			gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
			Engine_EventRequestExit(11);
			Engine_EventEnd();
		}
	}
}

static void EV09_PSYNERGY(void)
{
	u8				  *flags;
	s32				   z;
	s32				   x;
	s32				   set;
	struct FieldActor *leader;
	struct FieldActor *block;
	struct FieldActor *actor;
	s32				   row;
	s32				   marker;

	marker = 0;
	block = Object_GetById(9);
	leader = Object_GetById(0);
	row = 45;
	Engine_EventBegin();
	Engine_MapCopyCellAttributes(109, 43, 7, 5, row, 43);
	flags = &block->priority_flags;
	if ((2 & block->priority_flags) != 0)
	{
		Engine_MapCopyCellAttributes(45, 45, 1, 1, 46, row);
	}
	else
	{
		Map_CopyCellAttributes(48, 42, 1, 1, block->x.fixed >> 20, block->z.fixed >> 20);
	}
	x = block->x.fixed >> 20;
	if (x != 46)
	{
	}
	else
	{
		z = block->z.fixed >> 20;
		if (z != 45)
		{
		}
		else
		{
			set = Engine_GameFlagIsSet(0x301);
			if (set != 0)
			{
			}
			else
			{
				if ((leader->z.fixed >> 20) <= 45)
				{
					marker = OverlayObject_PrepareObjectWithCommand15(0x2e80000, 0, 0x2c00000, 20);
					Engine_ActorSetSpritePriority(0, 3);
				}
				actor = Object_GetById(9);
				Engine_ActorSetSpriteFlags(actor, 0);
				block->terrain_id = set;
				block->motion_flags = 3;
				block->gravity = 0x1999;
				block->bounce = set;
				Engine_MapCopyCellAttributes(43, 45, 1, 1, x, z);
				OverlayObject_WaitUntilIdle(block);
				Engine_AudioPlayCue(188);
				block->motion_flags = set;
				block->y.fixed = -0x100000;
				Engine_ActorSetSpritePriority(9, 3);
				{
					u8 two = 2;

					*flags = two;
				}
				Engine_MapCopyCellAttributes(45, 45, 1, 1, x, z);
				Object_GetById(0)->priority_flags |= 1;
				Engine_ObjectDispatchRelease(marker);
				block->collision_flags = set;
				*flags |= 2;
				Object_GetById(10)->collision_flags = set;
				Object_GetById(10)->priority_flags |= 2;
				Engine_ActorSetPosition(10, 0x3280000, 0x2d80000);
				Engine_ActorEnableActionCallback(10, gVinasuPushScript);
				Object_RefreshSelectorById(10);
				Engine_EventWait(30);
				Engine_AudioPlayCue(158);
				Engine_MapAnimateCells(gVinasuPushCells, 110, 41);
				Map_CopyCellAttributes(46, 41, 1, 1, x, 42);
				Engine_GameFlagSet(0x301);
			}
		}
	}
	Engine_EventEnd();
}

static void EV36_SCRIPT(void)
{
	s32 x;
	s32 y;

	Engine_EventBegin();
	if (SceneActor_TryMoveActorZeroTwoTilesAhead() == 0)
	{
		x = 45;
		y = 43;
		Engine_MapCopyCellAttributes(109, 43, 7, 5, x, y);
		RunStagedActorTransition();
	}
	Engine_EventEnd();
	EV09_PSYNERGY();
}

static void VinasuHeya_SettlePushedBlocks(void)
{
	struct FieldActor	*marker = 0;
	struct FieldActor	*leader = Object_GetById(0);
	struct FieldActor	*block;
	struct FieldActor	*other;
	struct SwitchEffect *first;
	struct SwitchEffect *second;
	u8					*flags;
	u32					 id;
	u32					 i;
	u32					 slot;
	s32					 priority;

	Engine_EventBegin();
	Map_CopyCellAttributes(108, 39, 13, 7, 44, 39);
	for (id = 9; id <= 11; id++)
	{
		block = Object_GetById(id);
		flags = &block->priority_flags;
		if (*flags != 2)
		{
			Map_CopyCellAttributes(47, 39, 1, 1, block->x.fixed >> 20, block->z.fixed >> 20);
		}
		else
		{
			Map_CopyCellAttributes(46, 39, 1, 1, block->x.fixed >> 20, block->z.fixed >> 20);
		}
		slot = 5;
		for (i = 0; i <= 3; i++)
		{
			if (block->x.fixed >> 20 == gVinasuSwitchCells[i].x
				&& block->z.fixed >> 20 == gVinasuSwitchCells[i].z && block->y.fixed >= 0)
			{
				slot = i;
				break;
			}
		}
		if (slot == 5)
		{
			continue;
		}
		for (i = 9; i <= 11; i++)
		{
			other = Object_GetById(i);
			if (id != i && block->x.fixed >> 20 == other->x.fixed >> 20
				&& block->z.fixed >> 20 == other->z.fixed >> 20)
			{
				slot = 5;
				break;
			}
		}
		if (slot == 5)
		{
			continue;
		}
		priority = leader->sprite->priority;
		if (leader->z.fixed >> 20 <= gVinasuSwitchCells[slot].z)
		{
			marker = OverlayObject_PrepareObjectWithCommand15(
				block->x.fixed, block->y.fixed, block->z.fixed - 0x40000, 20);
			Engine_ActorSetSpritePriority(0, 3);
		}
		Engine_ActorSetSpriteFlags(Object_GetById(id), 0);
		block->terrain_id = 0;
		block->motion_flags = 3;
		block->bounce = 0;
		block->gravity = 0x1999;
		Map_CopyCellAttributes(
			42, 41, 1, 1, gVinasuSwitchCells[slot].x, gVinasuSwitchCells[slot].z);
		OverlayObject_WaitUntilIdle(block);
		Engine_AudioPlayCue(188);
		block->collision_flags = 0;
		block->motion_flags = 0;
		block->y.fixed = -0x100000;
		Engine_ActorSetSpritePriority(id, 3);
		{
			u8 two = 2;

			*flags = two;
		}
		Map_CopyCellAttributes(
			46, 39, 1, 1, gVinasuSwitchCells[slot].x, gVinasuSwitchCells[slot].z);
		Engine_ActorSetSpritePriority(0, priority);
		Object_GetById(0)->priority_flags |= 1;
		if (marker != 0)
		{
			Engine_ObjectDispatchRelease(marker);
		}
		if (Engine_GameFlagIsSet(0x304))
		{
			Engine_EventEnd();
			return;
		}
		if ((Object_GetById(9)->priority_flags & Object_GetById(10)->priority_flags
				& Object_GetById(11)->priority_flags & 2)
			== 0)
		{
			continue;
		}
		first = (struct SwitchEffect *)SceneEffect_SpawnEffect284AtCell(
			888, 680, (s32)gVinasuSettleScriptA);
		second = (struct SwitchEffect *)SceneEffect_SpawnEffect284AtCell(
			888, 680, (s32)gVinasuSettleScriptB);
		while (first->active != 0 || second->active != 0)
		{
			if (first->finished != 0 || second->finished != 0)
			{
				Engine_EventWait(30);
				Engine_AudioPlayCue(158);
				Engine_MapAnimateCells(gVinasuSettleCells, 109, 37);
				Map_CopyCellAttributes(45, 37, 1, 1, 45, 38);
				if (Object_GetById(9)->x.fixed >> 20 == gVinasuSwitchCells[0].x
					&& Object_GetById(9)->z.fixed >> 20 == gVinasuSwitchCells[0].z)
				{
					Engine_GameFlagSet(0x302);
				}
				else
				{
					Engine_GameFlagSet(0x303);
				}
				Engine_GameFlagSet(0x304);
				break;
			}
			Engine_TaskWait(1);
		}
	}
	Engine_EventEnd();
}

static void EV35_SCRIPT(void)
{
	Engine_EventBegin();
	if (SceneActor_TryMoveActorZeroTwoTilesAhead() == 0)
	{
		s32 k5 = 44, k6 = 39;
		Engine_MapCopyCellAttributes(108, 39, 13, 7, k5, k6);
		RunStagedActorTransition();
	}
	Engine_EventEnd();
	VinasuHeya_SettlePushedBlocks();
}

static s32 SceneActor_SetHeightAboveLinkedRecord(struct FieldActor *obj)
{
	struct FieldActor *rec;

	rec = Object_GetById(obj->timer);
	obj->y.fixed = rec->y.fixed + 0x100000;
	return 0;
}

static void VinasuHeya_LowerFloatingBlocks(s32 wait)
{
	struct FieldActor *a = Object_GetById(8);
	struct FieldActor *b = Object_GetById(9);
	u32				   i;

	Actor_SetSpeed(8, 0x8000, 0x4000);
	Engine_ActorSetSpeed(9, 0x8000, 0x4000);
	if (wait != 0)
	{
		Engine_AudioPlayCue(180);
	}
	FieldObject_SetPosition((s32)a, a->x.fixed, gVinasuBlockHeights[a->timer], a->z.fixed);
	FieldObject_SetPosition((s32)b, b->x.fixed, gVinasuBlockHeights[b->timer], b->z.fixed);
	Engine_ActorWaitForMove(8);
	Engine_ActorWaitForMove(9);
	a->y.fixed = gVinasuBlockHeights[a->timer];
	b->y.fixed = gVinasuBlockHeights[b->timer];
	if (wait != 0)
	{
		Engine_AudioPlayCue(0x121);
	}
	for (i = 0; i < 5; i++)
	{
		struct FieldActor *actor = Object_GetById(i + 8);

		if (actor->y.fixed / 0x10000 < 0 && actor->y.fixed / 0x10000 > -30)
		{
			Map_CopyCellAttributes(4, 19, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
		}
	}
	Engine_EventWait(wait);
}

static void SceneActor_PickHighestSlotAtSameTileAndRelease(s32 selector)
{

	struct FieldActor *cand;
	s32				   highest = 0xffb00000;
	struct FieldActor *sel = 0;
	u32				   i;

	for (i = 0; i <= 5; i++)
	{
		s32 no = i + 8;
		s32 cand_y;

		if (no == selector)
		{
			continue;
		}

		cand = Object_GetById(no);
		sel = Object_GetById(selector);

		if ((cand->x.fixed >> 20) != (sel->x.fixed >> 20)
			|| (cand->z.fixed >> 20) != (sel->z.fixed >> 20))
		{
			continue;
		}

		cand_y = cand->y.fixed + 0x100000;
		if (highest <= cand_y)
		{
			sel->timer = no;
			highest = cand_y;
		}
	}

	Engine_ActorSetSpeed(selector, 0x40000, 0x20000);
	Engine_ObjectSetPosition(sel, sel->x.fixed, highest, sel->z.fixed);
	Engine_ActorWaitForMove(selector);
	Engine_AudioPlayCue(188);
	SceneEffect_SpawnNineRadialEffects(selector);
	Engine_EventWait(30);
}

static __inline__ void FloatingBlock_Settle(struct FieldActor *block, s32 value)
{
	block->target_y = ACTOR_NO_TARGET;
	block->terrain_height = value;
	block->velocity_y = value;
	block->motion_flags = value;
	block->timer = value;
}

static __inline__ void Actor_CopyPosition(struct FieldActor *to, struct FieldActor *from)
{
	to->x.fixed = from->x.fixed;
	to->y.fixed = from->y.fixed;
	to->z.fixed = from->z.fixed;
}

static void VinasuHeya_ResolveFloatingBlock(void)
{
	struct FieldActor  work;
	struct FieldActor *first = NULL;
	struct FieldActor *second = NULL;
	struct FieldActor *block;
	struct FieldActor *other;
	s32				   i;
	s32				   j;
	s32				   slot;
	s32				   x;
	s32				   z;

	Engine_EventBegin();
	for (i = 0; i < 4; i++)
	{
		block = Object_GetById(i + 10);
		x = block->x.fixed >> 20;
		if (x == 13 && (z = block->z.fixed >> 20) == 7 && !GameFlag_IsSet(0x200 + i))
		{
			OverlayObject_WaitUntilIdle(block);
			GameFlag_Set(0x200 + i);
			block->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
			block->collision_flags = 0;
			block->motion_flags = 0;
			Engine_MapCopyCellAttributes(4, 19, 1, 1, x, z);
			break;
		}
		if (block->sprite->priority == 3 && !GameFlag_IsSet(0x200 + i))
		{
			/* FAKEMATCH: the zero is taken before the priority call, in the search counter, so it is held in r5 across the call */
			j = 0;
			Engine_ActorSetSpritePriority(i + 10, 1);
			block->bounce = j;
			if (block->z.fixed >> 20 <= 12)
			{
				first = OverlayObject_SpawnWithMode14(block->x.fixed, 0, 0xe00000, 253);
				second = OverlayObject_SpawnWithMode14(block->x.fixed, 0, 0xf00000, 253);
			}
			OverlayObject_WaitUntilIdle(block);
			Engine_ActorSetPosition(i + 10, 0, 0);
			Engine_ObjectDispatchRelease(first);
			Engine_ObjectDispatchRelease(second);
			GameFlag_Set(0x200 + i);
			break;
		}
		if (block->z.fixed >> 20 == 19 && !GameFlag_IsSet(0x200 + i))
		{
			FloatingBlock_Settle(block, 0);
			slot = i;
			for (j = 0; j < i; j++)
			{
				if (!GameFlag_IsSet(0x200 + j))
				{
					other = Object_GetById(j + 10);
					Actor_CopyPosition(&work, block);
					Actor_CopyPosition(block, other);
					Actor_CopyPosition(other, &work);
					slot = j;
					break;
				}
			}
			other = Object_GetById(slot + 10);
			FloatingBlock_Settle(other, 0);
			Engine_CameraSetSpeed(0x30000, 0x6000);
			Battle_GetWorkObject1e0()->motion_flags = 0;
			Engine_CameraMoveTo(0x880000, 0x80000, 0x1580000, 1);
			Engine_CameraWaitForMove();
			SceneActor_PickHighestSlotAtSameTileAndRelease(slot + 10);
			if (Object_GetById(slot + 10)->x.fixed >> 20 == 6)
			{
				Object_GetById(8)->timer++;
				Object_GetById(9)->timer--;
			}
			else
			{
				Object_GetById(8)->timer--;
				Object_GetById(9)->timer++;
			}
			Object_GetById(slot + 10)->update =
				(void (*)(union FieldObject *))SceneActor_SetHeightAboveLinkedRecord;
			VinasuHeya_LowerFloatingBlocks(40);
			Object_GetById(slot + 10)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
			GameFlag_Set(slot + 0x200);
			break;
		}
	}
	Engine_EventEnd();
}

static void FieldScene_RunThreeCallSequence(void)
{

	Engine_EventBegin();
	RunStagedActorTransition();
	Engine_EventEnd();
}

static void EV40_TOUCH(void)
{
	struct EffectOptions options;
	struct ScrollLayer	*layer;
	struct FieldActor	*leader;
	s32					 x;
	s32					 z;
	s32					 countdown;
	u32					 i;
	s32					 dust_x;

	layer = &((struct MapWork *)gMapWork[0])->layer;
	leader = Object_GetById(0);
	x = leader->x.part.pixel;
	z = leader->z.part.pixel;
	leader->y.fixed = 0;
	if (x >= 532 && x <= 539 && z >= 324 && z < 332)
	{
		leader->y.fixed = -0x20000;
		if (!Engine_GameFlagIsSet(0x306))
		{
			Engine_EventBegin();
			Engine_MapCopyCellsTo(63, 29, 33, 20, 1, 1);
			Engine_AudioPlayCue(161);
			Engine_MapCopyCellsTo(44, 83, 44, 80, 3, 3);
			Engine_EventWait(30);
			Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
			Engine_AudioPlayCue(239);
			Engine_EventWait(20);
			dust_x = 0x2680000;
			x = 0;
			countdown = 60;
			for (i = 0; i <= 319; i++, countdown--)
			{
				layer->x += 0x3333;
				if (dust_x >= 0x2400000 && i > 40)
				{
					dust_x += -0x3333;
					options.priority = 2;
					options.start_scale_x =
						((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
					options.start_scale_y =
						((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
					options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
					Effect_Spawn(dust_x, 0, 0x1200000, 0, -(((gFrameCount & 1) * 3) << 16), 0,
						0x8a0000, &options);
					if (countdown == 0)
					{
						countdown = 40;
						x += 4;
						Map_CopyCellsTo(x, 56, 36, 17, 3, 4);
					}
				}
				Engine_TaskWait(1);
			}
			layer->x += 0x8000;
			layer->x = layer->x / 0x10000 << 16;
			Engine_AudioPlayCue(288);
			Engine_AudioPlayCue(188);
			Work_SetValuesIfNonNegative(-1, -1, 0xe666);
			Engine_MapRenderWaitForValues();
			gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
			Engine_EventRequestExit(18);
			Engine_EventEnd();
		}
	}
}

static void EV41_TOUCH(void)
{
	struct EffectOptions options;
	struct ScrollLayer	*layer;
	struct FieldActor	*leader;
	s32					 x;
	s32					 z;
	s32					 countdown;
	u32					 i;
	s32					 dust_x;

	layer = &((struct MapWork *)gMapWork[0])->layer;
	leader = Object_GetById(0);
	x = leader->x.part.pixel;
	z = leader->z.part.pixel;
	leader->y.fixed = 0;
	if (x >= 788 && x <= 795 && z >= 324 && z < 332)
	{
		leader->y.fixed = -0x20000;
		if (!Engine_GameFlagIsSet(0x307))
		{
			Engine_EventBegin();
			Engine_MapCopyCellsTo(63, 29, 49, 20, 1, 1);
			Engine_AudioPlayCue(161);
			Engine_EventWait(30);
			Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
			Engine_AudioPlayCue(239);
			Engine_EventWait(20);
			dust_x = 0x2c00000;
			x = 61;
			countdown = 60;
			for (i = 0; i <= 319; i++, countdown--)
			{
				layer->x += -0x3333;
				dust_x += 0x3333;
				if (dust_x >= 0x2c80000 && dust_x < 0x2f00000)
				{
					options.priority = 2;
					options.start_scale_x =
						((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
					options.start_scale_y =
						((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
					options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
					Effect_Spawn(dust_x, 0, 0x1200000, 0, -(((gFrameCount & 1) * 3) << 16), 0,
						0x8a0000, &options);
					if (countdown == 0)
					{
						countdown = 40;
						x -= 4;
						Map_CopyCellsTo(x, 56, 44, 17, 3, 4);
					}
				}
				Engine_TaskWait(1);
			}
			layer->x += 0x8000;
			layer->x = layer->x / 0x10000 << 16;
			Engine_AudioPlayCue(288);
			Engine_AudioPlayCue(188);
			Work_SetValuesIfNonNegative(-1, -1, 0xe666);
			Engine_MapRenderWaitForValues();
			gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
			Engine_EventRequestExit(19);
			Engine_EventEnd();
		}
	}
}

static void Scene_RunScene3c8SequenceA(void)
{
	struct FieldActor *effect;
	struct FieldActor *actor;
	struct FieldActor *other;
	struct FieldActor *leader;
	struct FieldActor *a;
	struct FieldActor *b;
	struct FieldActor *c;
	struct FieldActor *d;
	u8				  *flags;
	u8				  *motion;
	u32				   id;
	u32				   i;
	u32				   slot;
	u32				   priority;

	effect = 0;
	leader = Actor_Get(0);
	Event_Begin();
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
	Battle_ResetEffectCounter();
#endif
	Map_CopyCellAttributes(69, 48, 4, 2, 5, 48);
	Map_CopyCellAttributes(73, 37, 9, 13, 9, 37);
	for (id = 15; id <= 18; id++)
	{
		actor = Object_GetById(id);
		flags = &actor->priority_flags;
		if (*flags != 2)
			Map_CopyCellAttributes(72, 48, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
		else
			Map_CopyCellAttributes(73, 48, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);

		slot = 8;
		for (i = 0; i < 8; i++)
		{
			if ((actor->x.fixed >> 20) == gVinasuPillarCells[i].x
				&& (actor->z.fixed >> 20) == gVinasuPillarCells[i].z && actor->y.fixed >= 0)
			{
				slot = i;
				break;
			}
		}
		if (slot == 8)
			continue;
		for (i = 15; i <= 18; i++)
		{
			other = Object_GetById(i);
			if (id != i && (actor->x.fixed >> 20) == (other->x.fixed >> 20)
				&& (actor->z.fixed >> 20) == (other->z.fixed >> 20))
			{
				slot = 8;
				break;
			}
		}
		if (slot == 8)
			continue;

		priority = leader->sprite->priority;
		if ((leader->z.fixed >> 20) <= gVinasuPillarCells[slot].z)
		{
			effect = OverlayObject_PrepareObjectWithCommand15(
				actor->x.fixed, actor->y.fixed, actor->z.fixed - 0x40000, 20);
			Actor_SetSpritePriority(0, 3);
		}
		for (i = 15; i <= 18; i++)
		{
			other = Object_GetById(i);
			if (id != i && (actor->x.fixed >> 20) == (other->x.fixed >> 20)
				&& (actor->z.fixed >> 20) - 1 == (other->z.fixed >> 20))
				Actor_SetSpritePriority(i, 3);
		}
		Actor_SetSpriteFlags(Object_GetById(id), 0);
		actor->terrain_id = 0;
		motion = &actor->motion_flags;
		*motion = 3;
		((union FieldObject *)actor)->effect.velocity_y = 0x1999;
		((union FieldObject *)actor)->effect.velocity_x = 0;
		Map_CopyCellAttributes(6, 44, 1, 1, gVinasuPillarCells[slot].x, gVinasuPillarCells[slot].z);
		OverlayObject_WaitUntilIdle(actor);
		Event_PlayCue(188);
		actor->collision_flags = 0;
		*motion = 0;
		actor->y.fixed = -0x100000;
		Actor_SetSpritePriority(id, 3);
		*flags = 2;
		Map_CopyCellAttributes(
			73, 48, 1, 1, gVinasuPillarCells[slot].x, gVinasuPillarCells[slot].z);
		Actor_SetSpritePriority(0, priority);
		Object_GetById(0)->priority_flags |= 1;
		for (i = 15; i <= 18; i++)
		{
			other = Object_GetById(i);
			if (id != i && (actor->x.fixed >> 20) == (other->x.fixed >> 20)
				&& (actor->z.fixed >> 20) - 1 == (other->z.fixed >> 20))
			{
				Actor_SetSpritePriority(i, 1);
				Object_GetById(i)->priority_flags |= 1;
			}
		}
		Engine_ObjectDispatchRelease(effect);
		if (GameFlag_IsSet(0x308))
		{
			Event_End();
			return;
		}
		a = Actor_Get(15);
		b = Actor_Get(16);
		c = Actor_Get(17);
		d = Object_GetById(18);
		if ((a->priority_flags & b->priority_flags & c->priority_flags & d->priority_flags) & 2)
		{
			struct SwitchEffect *first;
			struct SwitchEffect *second;

			Camera_SetSpeed(0x10000, 0x2000);
			Object_PlaceCurrentWithinCameraBounds(14, 1);
			Camera_WaitForMove();
			first = (struct SwitchEffect *)SceneEffect_SpawnEffect284AtCell(
				136, 0x308, (s32)gVinasuFirstGateScript);
			Event_Wait(30);
			Camera_SetSpeed(0x6666, 0xccc);
			Camera_MoveTo(0xd80000, -1, 0x2780000, 1);
			ObjectDispatch_WaitForValue16(first);
			Object_SetScript((struct FieldActor *)first, gVinasuFirstGateOpenScript);
			second = (struct SwitchEffect *)SceneEffect_SpawnEffect284AtCell(
				216, 0x2f8, (s32)gVinasuSecondGateScript);
			while (first->active != 0 || second->active != 0)
			{
				if (first->finished != 0 || second->finished != 0)
				{
					Event_Wait(30);
					Map_AnimateCells(gVinasuGateCells, 77, 35);
					Map_CopyCellAttributes(13, 35, 1, 1, 13, 36);
					GameFlag_Set(0x308);
					break;
				}
				Task_Wait(1);
			}
		}
	}
	Event_End();
}

static void EV45_SCRIPT_6(void)
{

	Engine_EventBegin();
	if (SceneActor_TryMoveActorZeroTwoTilesAhead() == 0)
	{
		{
			s32 k5 = 5, k6 = 48;
			Engine_MapCopyCellAttributes(69, 48, 4, 2, k5, k6);
		}
		{
			s32 j5 = 9, j6 = 37;
			Engine_MapCopyCellAttributes(73, 37, 9, 13, j5, j6);
		}
		RunStagedActorTransition();
	}
	Engine_EventEnd();
	Scene_RunScene3c8SequenceA();
}

static void EV18_TOUCH(void)
{
	Engine_EventBegin();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x208, 0x2c8);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
	(void)OverlayObject_SpawnWithMode14(0x2080000, 0, 0x3100000, 223);
	BattleFx_RunRisingObjectSequence(0, 6, 0);
	Engine_EventWait(60);
	Engine_EventRequestExit(20);
	Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
	s32 v = gGameState.scene;

	if (v == (s32)&SceneId_VinasuHeya1)
	{
		return gVinasuHeyaEvents1;
	}
	if (v == (s32)&SceneId_VinasuHeya2)
	{
		return gVinasuHeyaEvents2;
	}
	if (v == (s32)&SceneId_VinasuHeya3)
	{
		return gVinasuHeyaEvents3;
	}
	if (v == (s32)&SceneId_VinasuHeya4)
	{
		return gVinasuHeyaEvents4;
	}
	if (v == (s32)&SceneId_VinasuHeya5)
	{
		return gVinasuHeyaEvents5;
	}
	if (v == (s32)&SceneId_VinasuHeya6)
	{
		return gVinasuHeyaEvents6;
	}
	return gVinasuHeyaEvents2;
}

static void SceneActor_ClearActorModeAndSetState5(s32 no)
{
	struct FieldActor *p;
	s32				   mask;

	p = Object_GetById(no);
	p->motion_flags = 0;
	mask = 252;
	mask &= p->collision_flags;
	p->collision_flags = mask;
	Engine_ActorSetSpriteFlags(p, 0);
	Object_SetMode(p, 5);
	Engine_ActorSetSpritePriority(no, 3);
	{
		s32 v = 2;
		v |= p->priority_flags;
		p->priority_flags = v;
	}
}

static void SceneState_ApplyStepToSlots15To18(void)
{
	u32 i;

	i = 15;
	do
	{
		Object_GetById(i);
		i++;
	} while (i <= 18);
}

s32 Scene_RunEntrySetup(void)
{
	struct FieldActor *actor;
	s32				   area;
	u32				   i;
	s32				   x;
	s32				   z;

	Engine_EventWait(1);
	if (Engine_GameFlagIsSet(0x109))
		SceneActor_ApplySlotsMatchingKind212();
	Engine_GameFlagSet(0x110);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	area = gGameState.scene;
	if (area == (s32)&SceneId_VinasuHeya1)
	{
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
		if (!Engine_GameFlagIsSet(0x981))
			SceneActor_ClearActorModeAndSetState5(8);
		else
			Map_CopyCellAttributes(7, 17, 2, 1, 7, 16);
		SceneActor_ClearActorModeAndSetState5(9);
		SceneActor_ClearActorModeAndSetState5(10);
		SceneActor_ClearActorModeAndSetState5(11);
		Engine_ActorSetSpritePriority(11, 2);
		SceneActor_ClearActorModeAndSetState5(12);
		Engine_ActorSetSpritePriority(12, 2);
		SceneActor_ClearActorModeAndSetState5(13);
		SceneActor_ClearActorModeAndSetState5(14);
		goto end;
	}
	if (area == (s32)&SceneId_VinasuHeya2)
	{
		switch (gGameState.entrance)
		{
		case 1:
		case 2:
			SceneActor_ClearActorModeAndSetState5(8);
			break;
		case 5:
			Engine_GameFlagClear(0x120);
		case 3:
		case 4:
		case 6:
			SceneActor_ClearActorModeAndSetState5(9);
			break;
		case 20:
		case 21:
			if (Engine_GameFlagIsSet(0x982))
			{
				Map_CopyCellsTo(103, 27, 89, 27, 7, 8);
				Map_CopyCellsTo(41, 90, 27, 92, 3, 2);
				Engine_MapCopyCellsTo(41, 90, 29, 93, 3, 2);
				Engine_MapCopyCellsTo(41, 90, 27, 94, 3, 2);
				Engine_MapCopyCellsTo(41, 90, 27, 96, 3, 2);
				Engine_MapCopyCellsTo(41, 90, 29, 97, 3, 2);
				Engine_MapCopyCellsTo(41, 96, 25, 91, 3, 2);
				Engine_MapCopyCellsTo(41, 92, 25, 93, 3, 2);
				Engine_MapCopyCellsTo(41, 96, 25, 95, 3, 2);
				Engine_MapCopyCellsTo(41, 96, 25, 97, 3, 2);
				Engine_MapCopyCellsTo(41, 96, 27, 96, 3, 2);
				Engine_MapCopyCellsTo(41, 96, 29, 97, 3, 2);
			}
			else
			{
				if (!Engine_GameFlagIsSet(0x983))
					break;
				Map_CopyCellsTo(111, 27, 89, 27, 7, 8);
				Map_CopyCellsTo(41, 90, 25, 91, 3, 2);
				Engine_MapCopyCellsTo(41, 90, 25, 93, 3, 2);
				Engine_MapCopyCellsTo(41, 90, 25, 95, 3, 2);
				Engine_MapCopyCellsTo(41, 90, 25, 97, 3, 2);
				Engine_MapCopyCellsTo(41, 90, 27, 96, 3, 2);
				Engine_MapCopyCellsTo(41, 90, 29, 97, 3, 2);
				Engine_MapCopyCellsTo(41, 94, 27, 92, 3, 2);
				Engine_MapCopyCellsTo(41, 96, 29, 93, 3, 2);
				Engine_MapCopyCellsTo(41, 94, 27, 94, 3, 2);
				Engine_MapCopyCellsTo(41, 96, 27, 96, 3, 2);
				Engine_MapCopyCellsTo(41, 96, 29, 97, 3, 2);
			}
			break;
		case 26:
			Engine_GameFlagClear(0x121);
		case 22:
		case 23:
			Engine_GameFlagClear(0x12f);
			if (Engine_GameFlagIsSet(0x200))
				Map_CopyCellsTo(44, 117, 41, 117, 3, 5);
			break;
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
			if (Engine_GameFlagIsSet(0x987))
			{
				Actor_SetPosition(12, 218 << 18, 176 << 15);
				actor = Object_GetById(12);
				actor->y.fixed = -0x180000;
				actor->target_y = ACTOR_NO_TARGET;
			}
			break;
		}
	}
	else if (area == (s32)&SceneId_VinasuHeya3)
	{
		switch (gGameState.entrance)
		{
		case 16:
			Engine_GameFlagClear(0x12f);
			break;
		case 21:
			Scene_RunPairedParticleWaveSequence();
			break;
		case 20:
			BattleFx_SetQueuedSoundAndPlay(170);
			if (!Engine_GameFlagIsSet(0x109))
				FieldScene_RunSupplementalSequenceOne();
			break;
		case 9:
		case 10:
			actor = Object_GetById(11);
			SceneActor_ApplyPositionsOfActors11And12();
			if (actor->x.fixed >> 20 == 8)
				SceneState_MarkActorAndApplyRectAtTile(actor);
			actor = Object_GetById(12);
			if (actor->x.fixed >> 20 == 7)
				SceneState_MarkActorAndApplyRectAtTile(actor);
			OverlayObject_SpawnWithMode14(206 << 16, 0, 0x1c10000, 223);
			OverlayObject_SpawnWithMode14(210 << 16, 0, 0x1c10000, 223);
			break;
		case 7:
		case 8:
			BattleFx_StartFadeOverlay(0);
			Engine_TaskWait(2);
			actor = Object_GetById(8);
			actor->motion_flags = 0;
			actor->update = SceneActor_SetFlagBitByRelativeDepth;
			actor = Object_GetById(9);
			actor->motion_flags = 0;
			actor->update = SceneActor_SetFlagBitByRelativeDepth;
			actor = Object_GetById(10);
			actor->motion_flags = 0;
			actor->update = SceneActor_SetFlagBitByRelativeDepth;
			FieldScene_PlaceAndPinSlots8To10();
			break;
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
			BattleFx_SetQueuedSoundAndPlay(170);
			BattleFx_StartFadeOverlay(0);
			Engine_TaskWait(2);
			if (Engine_GameFlagIsSet(0x300))
			{
				Map_CopyCellsTo(111, 5, 117, 5, 5, 2);
				Engine_MapCopyCellsTo(111, 10, 117, 10, 5, 2);
				Engine_MapCopyCellsTo(111, 7, 111, 5, 5, 2);
				Engine_MapCopyCellsTo(111, 7, 111, 10, 5, 2);
				Map_CopyCells(48, 3, 3, 10, 54, 3);
				Engine_MapCopyCells(55, 26, 3, 10, 48, 3);
			}
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 19:
			goto cue;
		}
	}
	else if (area == (s32)&SceneId_VinasuHeya4)
	{
		switch (gGameState.entrance)
		{
		case 2:
			FieldScene_RunOpeningAuxiliarySequence();
			goto cue;
		case 4:
		case 6:
			BattleFx_StartFadeOverlay(0);
			break;
		case 9:
		case 10:
			actor = Object_GetById(8);
			actor->motion_flags = 0;
			actor->y.fixed = 0;
			actor = Object_GetById(9);
			actor->motion_flags = 0;
			actor->collision_flags = 0;
			if (Engine_GameFlagIsSet(0x301))
			{
				Engine_TaskWait(1);
				Engine_MapCopyCellsTo(124, 41, 110, 41, 1, 2);
				Engine_MapCopyCellAttributes(46, 41, 1, 1, 46, 42);
				Actor_SetPosition(9, 186 << 18, 182 << 18);
				actor->motion_flags = 0;
				actor->y.fixed = -0x100000;
				Engine_ActorSetSpritePriority(9, 3);
				actor->priority_flags = 2;
				Engine_MapCopyCellAttributes(45, 45, 1, 1, 46, 45);
				Engine_ActorSetAnimation(10, 7);
				Engine_ActorSetSpritePriority(10, 1);
				actor = Object_GetById(10);
				actor->collision_flags = 0;
				actor->priority_flags = 2;
				Actor_SetPosition(10, 0x2e70000, 174 << 18);
				actor->update = (void (*)(
					union FieldObject *))SceneEffect_SpawnRandomizedParticleEveryFourFrames;
			}
			EV09_PSYNERGY();
			break;
		case 11:
			gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
			Object_GetById(0)->y.fixed = -0x20000;
		case 7:
		case 8:
			BattleFx_SetQueuedSoundAndPlay(170);
			DisplayBlend_EnableRunScript();
			{
				s32 zero = 0;

				/* FAKEMATCH: the one-pass block's loop notes keep movs r3, #0 before the register address load and the store before the next call's argument (sched2 barriers); the empty asm's in/out operand gives the zero four refs so local-alloc allocates it r3 ahead of the address (differ 6 without it, registers swapped). Plain forms measured 2026-10-11: REG_BLDCNT = 0 differ 95 (halfword constant from the pool); an s32 or u16 zero, an inline helper, Io_Set16 or a function-top zero differ 6; pointer-first forms differ 4; Io_Write16 differ 6. */
				asm("" : "+l"(zero));
				/* FAKEMATCH: needed to match: this CSE block keeps the blend
                 * store before the next call setup, at the same size and frame. */
				do
				{
					REG_BLDCNT = zero;
				} while (0);
			}
			if (Engine_GameFlagIsSet(0x300))
			{
				Engine_MapCopyCellsTo(15, 96, 9, 96, 3, 3);
				Engine_MapCopyCellsTo(12, 96, 15, 96, 3, 3);
				Engine_MapCopyCellsTo(5, 50, 15, 32, 3, 4);
				Engine_MapCopyCellsTo(25, 45, 9, 32, 3, 4);
				Map_CopyCellAttributes(15, 32, 3, 1, 9, 32);
				Engine_MapCopyCellAttributes(12, 32, 3, 1, 15, 32);
			}
			if (gGameState.entrance != 11)
				break;
			Engine_EventOpenScreen();
			Engine_EventWaitForScreen();
			gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
			break;
		case 1:
			goto cue;
		}
	}
	else if (area == (s32)&SceneId_VinasuHeya5)
	{
		switch (gGameState.entrance)
		{
		case 19:
			Scene_RunParticleWaveSequence();
			break;
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 20:
			BattleFx_SetQueuedSoundAndPlay(170);
			if (Engine_GameFlagIsSet(0x306))
			{
				Map_CopyCells(53, 12, 3, 13, 26, 12);
				Map_CopyCellsTo(81, 41, 89, 14, 9, 2);
				Engine_TaskWait(1);
				Engine_TaskAddCallback(SceneState_CallWith432And32, 0xc80);
			}
			if (Engine_GameFlagIsSet(0x307))
			{
				Map_CopyCells(58, 12, 3, 13, 34, 12);
				Map_CopyCellsTo(81, 41, 97, 14, 5, 2);
				Engine_TaskWait(1);
				Engine_TaskAddCallback(FieldScene_CallWith560And44, 0xc80);
			}
			if (gGameState.entrance == 11)
				FieldScene_RunOpeningAuxiliarySequence();
			else if (gGameState.entrance == 20)
				Scene_RunEastParticleWaveSequence();
			break;
		case 4:
		case 5:
			/* FAKEMATCH: value-returning cast prevents cross-jumping these blocks. */
			((s32 (*)(void))FieldScene_RunOpeningAuxiliarySequence)();
			BattleFx_SetQueuedSoundAndPlay(170);
			break;
		case 15:
		case 16:
			actor = Object_GetById(8);
			actor->motion_flags = 0;
			actor->y.fixed = 0;
			Object_GetById(9)->motion_flags = 0;
			Object_GetById(10)->motion_flags = 0;
			Object_GetById(11)->motion_flags = 0;
			if (Engine_GameFlagIsSet(0x304))
			{
				Engine_TaskWait(1);
				Map_CopyCellsTo(111, 59, 109, 37, 1, 2);
				Map_CopyCellAttributes(45, 37, 1, 1, 45, 38);
				if (Engine_GameFlagIsSet(0x302))
				{
					Actor_SetPosition(9, 194 << 18, 166 << 18);
					Actor_SetPosition(10, 210 << 18, 166 << 18);
					Actor_SetPosition(11, 194 << 18, 174 << 18);
				}
				else
				{
					Actor_SetPosition(9, 210 << 18, 166 << 18);
					Actor_SetPosition(10, 194 << 18, 174 << 18);
					Actor_SetPosition(11, 210 << 18, 174 << 18);
				}
				Engine_ActorSetSpritePriority(9, 3);
				actor = Object_GetById(9);
				actor->y.fixed = -0x100000;
				actor->priority_flags = 2;
				Engine_ActorSetSpritePriority(10, 3);
				actor = Object_GetById(10);
				actor->y.fixed = -0x100000;
				actor->priority_flags = 2;
				Engine_ActorSetSpritePriority(11, 3);
				actor = Object_GetById(11);
				actor->y.fixed = -0x100000;
				actor->priority_flags = 2;
				Engine_ActorSetAnimation(12, 7);
				Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
				Engine_ActorSetSpritePriority(12, 1);
				actor = Object_GetById(12);
				actor->collision_flags = 0;
				actor->priority_flags = 2;
				Actor_SetPosition(12, 0x2d70000, 158 << 18);
				actor->update = (void (*)(
					union FieldObject *))SceneEffect_SpawnRandomizedParticleEveryFourFrames;
			}
			VinasuHeya_SettlePushedBlocks();
			break;
		case 6:
			goto cue;
		}
	}
	else if (area == (s32)&SceneId_VinasuHeya6)
	{
		switch (gGameState.entrance)
		{
		case 1:
		case 2:
			if (Engine_GameFlagIsSet(0x109))
			{
				VinasuHeya_LowerFloatingBlocks(0);
				OverlayObject_WaitUntilIdle(Object_GetById(0));
				for (i = 0; i <= 3; i++)
				{
					actor = Object_GetById(i + 10);
					x = actor->x.fixed >> 20;
					if (x == 13)
					{
						z = actor->z.fixed >> 20;
						if (z == 7 && Engine_GameFlagIsSet(i + 0x200))
							goto found;
					}
				}
			}
			else
			{
				actor = Object_GetById(8);
				actor->motion_flags = 0;
				actor->y.fixed = -0x300000;
				actor->priority_flags |= 2;
				actor->collision_flags &= 0xfe;
				actor->timer = 3;
				Engine_ActorSetSpritePriority(8, 1);
				actor = Object_GetById(9);
				actor->motion_flags = 0;
				actor->y.fixed = -0x300000;
				actor->priority_flags |= 2;
				actor->collision_flags &= 0xfe;
				actor->timer = 3;
				Engine_ActorSetSpritePriority(9, 1);
				actor = Object_GetById(10);
				actor->motion_flags = 0;
				actor->timer = 0;
				Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
				actor = Object_GetById(11);
				actor->motion_flags = 0;
				actor->timer = 0;
				Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
				actor = Object_GetById(12);
				actor->motion_flags = 0;
				actor->timer = 0;
				Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
				actor = Object_GetById(13);
				actor->motion_flags = 0;
				actor->timer = 0;
				Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
			}
			break;
		case 13:
		case 14:
		cue:
			BattleFx_SetQueuedSoundAndPlay(170);
			break;
		case 18:
		case 19:
			gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
			Object_GetById(0)->y.fixed = -0x20000;
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 20:
			Object_GetById(20)->motion_flags = 4;
			Object_GetById(20)->priority_flags |= 2;
			Object_GetById(20)->y.fixed = -0x108000;
			DisplayBlend_EnableRunScript();
			Io_Write16(0, &REG_BLDCNT);
			if (Engine_GameFlagIsSet(0x306))
			{
				BattleFx_SetQueuedSoundAndPlay(170);
				Engine_MapCopyCellsTo(36, 81, 32, 81, 3, 2);
				Engine_MapCopyCellsTo(36, 83, 36, 81, 3, 2);
				Engine_MapCopyCellAttributes(36, 17, 3, 1, 32, 17);
				Engine_MapCopyCellAttributes(36, 18, 3, 1, 36, 17);
				Engine_MapCopyCellsTo(63, 29, 33, 20, 1, 1);
				Engine_MapCopyCellsTo(20, 56, 36, 17, 3, 4);
			}
			if (Engine_GameFlagIsSet(0x307))
			{
				Engine_MapCopyCellsTo(44, 81, 48, 81, 3, 2);
				Engine_MapCopyCellsTo(44, 83, 44, 81, 3, 2);
				Engine_MapCopyCellAttributes(44, 17, 3, 1, 48, 17);
				Engine_MapCopyCellAttributes(44, 18, 3, 1, 44, 17);
				Engine_MapCopyCellsTo(63, 29, 49, 20, 1, 1);
				Engine_MapCopyCellsTo(41, 56, 44, 17, 3, 4);
			}
			if (gGameState.entrance == 18 || gGameState.entrance == 19)
			{
				Engine_EventOpenScreen();
				Engine_EventWaitForScreen();
				gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
			}
			if (gGameState.entrance == 20)
				FieldScene_RunLeaderDropSequence();
			break;
		found:
			actor->priority_flags |= 2;
			actor->collision_flags = 0;
			actor->motion_flags = 0;
			Engine_MapCopyCellAttributes(4, 19, 1, 1, x, z);
			break;
		case 15:
		case 16:
			Engine_TaskWait(1);
			Engine_TaskAddCallback(SceneState_ApplyStepToSlots15To18, 0xc80);
			actor = Object_GetById(14);
			actor->motion_flags = 0;
			actor->y.fixed = 0;
			Object_GetById(15)->motion_flags = 0;
			Object_GetById(16)->motion_flags = 0;
			Object_GetById(17)->motion_flags = 0;
			Object_GetById(18)->motion_flags = 0;
			if (Engine_GameFlagIsSet(0x308))
			{
				Engine_TaskWait(1);
				Engine_MapCopyCellsTo(95, 56, 77, 35, 1, 2);
				Map_CopyCellAttributes(13, 35, 1, 1, 13, 36);
				Engine_ActorSetPosition(15, 132 << 17, 186 << 18);
				actor = Object_GetById(15);
				actor->y.fixed = -0x100000;
				actor->priority_flags = 2;
				Engine_ActorSetSpritePriority(15, 3);
				Actor_SetPosition(16, 184 << 16, 158 << 18);
				actor = Object_GetById(16);
				actor->y.fixed = -0x100000;
				actor->priority_flags = 2;
				Engine_ActorSetSpritePriority(16, 3);
				Engine_ActorSetPosition(17, 232 << 16, 174 << 18);
				actor = Object_GetById(17);
				actor->y.fixed = -0x100000;
				actor->priority_flags = 2;
				Engine_ActorSetSpritePriority(17, 3);
				Engine_ActorSetPosition(18, 184 << 16, 166 << 18);
				actor = Object_GetById(18);
				actor->y.fixed = -0x100000;
				actor->priority_flags = 2;
				Engine_ActorSetSpritePriority(18, 3);
				Engine_ActorSetAnimation(19, 7);
				Engine_ActorSetSpriteFlags(Object_GetById(19), 0);
				Engine_ActorSetSpritePriority(19, 1);
				actor = Object_GetById(19);
				actor->collision_flags = 0;
				actor->priority_flags = 2;
				Actor_SetPosition(19, 215 << 16, 150 << 18);
				actor->update = (void (*)(
					union FieldObject *))SceneEffect_SpawnRandomizedParticleEveryFourFrames;
			}
			Scene_RunScene3c8SequenceA();
			break;
		}
	}
end:
	return 0;
}

static void EV31_TOUCH_6(void)
{
	struct DirXZ args;

	args.x = 0;
	args.z = -1;
	VinasuHeya_StepActorWithDust(&args);
}

static void SceneState_PassRange0To1(void)
{
	struct DirXZ args;

	args.x = 0;
	args.z = 1;
	VinasuHeya_StepActorWithDust(&args);
}

static void EV32_TOUCH_6(void)
{
	struct DirXZ args;

	args.x = -1;
	args.z = 0;
	VinasuHeya_StepActorWithDust(&args);
}

static void EV33_TOUCH(void)
{
	struct DirXZ args;

	args.x = 1;
	args.z = 0;
	VinasuHeya_StepActorWithDust(&args);
}

static void VinasuHeya_StepActorWithDust(struct DirXZ *dir)
{
	s32					 id;
	struct FieldActor	*center;
	struct FieldActor	*actor;
	s32					 pos[3];
	struct EffectOptions params;
	s32					 hit;
	s32					 phase;
	s32					 x;

	{
		struct EventWork *event = gEventWork;

		id = gGameState.selected_actor;
		center = event->view_center;
	}
	actor = Object_GetById(id);
	pos[0] = actor->x.fixed + ((dir->x * 3) << 15);
	pos[1] = actor->y.fixed;
	pos[2] = actor->z.fixed + ((dir->z * 3) << 15);
	hit = Object_CheckMovementCollision(actor, pos);
	phase = gFrameCount & 4;
	if (phase == 0)
	{
		params.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
		x = actor->x.fixed + (((u32)(Engine_RandomNext() * 12) >> 16) << 16) - 0x60000;
		Effect_Spawn(x, actor->y.fixed, actor->z.fixed,
			(((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999 + 0x7ffd) * dir->x, phase,
			dir->z * (((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999 + 0x7ffd), 0x800000, &params);
	}
	if (hit < 0)
	{
		Engine_ActorSetAttachedEffect(id, 0x102);
		Engine_ObjectSetPosition(actor, actor->x.fixed, actor->y.fixed, actor->z.fixed + 0x80000);
		Object_SetMode(actor, 7);
		Engine_ObjectCommitPosition(actor);
		do
		{
			Engine_TaskWait(1);
		} while (actor->y.fixed != actor->terrain_height);
		Object_SetMode(actor, 6);
		Engine_TaskWait(3);
		return;
	}
	pos[0] = actor->x.fixed + (dir->x << 19);
	pos[1] = actor->y.fixed;
	pos[2] = actor->z.fixed + (dir->z << 19);
	hit = Object_CheckMovementCollision(actor, pos);
	if (hit > 0)
	{
		return;
	}
	pos[0] = actor->x.fixed + dir->x * 0x5b333 - dir->z * 0x5b333;
	pos[1] = actor->y.fixed;
	pos[2] = actor->z.fixed + dir->z * 0x5b333 - dir->x * 0x5b333;
	hit = Object_CheckMovementCollision(actor, pos);
	if (hit > 0)
	{
		actor->x.fixed += (dir->z * 3) << 15;
		actor->z.fixed += (dir->x * 3) << 15;
		return;
	}
	pos[0] = (dir->x + dir->z) * 0x5b333 + actor->x.fixed;
	pos[1] = actor->y.fixed;
	pos[2] = (dir->x + dir->z) * 0x5b333 + actor->z.fixed;
	hit = Object_CheckMovementCollision(actor, pos);
	if (hit > 0)
	{
		actor->x.fixed -= (dir->z * 3) << 15;
		actor->z.fixed -= (dir->x * 3) << 15;
		return;
	}
	center->x.fixed += (dir->x * 3) << 15;
	actor->x.fixed += (dir->x * 3) << 15;
	center->z.fixed += (dir->z * 3) << 15;
	actor->z.fixed += (dir->z * 3) << 15;
}

static __inline__ void Rubble_SetScaleAndSpin(struct EffectOptions *opts)
{
	opts->start_scale_x = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
	opts->start_scale_y = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
	opts->spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
}

static void Scene_RunPairedParticleWaveSequence(void)
{
	struct EffectOptions options;
	u32					 repeat;
	u32					 row;
	u32					 i;

	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventBegin();
	Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
	Engine_ActorSetChildValue(0, 15);
	BattleFx_SetQueuedSoundAndPlay(170);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Engine_AudioPlayCue(162);
	repeat = 0;
	for (row = 0; row < 10; row++)
	{
		options.start_scale_x = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
		options.start_scale_y = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
		options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
	again:
		for (i = 0; i < 4 && row < 8; i++)
		{
			Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x3600000, 0,
				0x300000 + row * 0x100000 + i * 0x40000, 0, 0, 0, 0x880000, &options);
		}
		Engine_TaskWait(3);
		if (row == 3 && repeat < 3)
		{
			repeat++;
			goto again;
		}
		Map_CopyCellsTo(48, row + 3, 54, row + 3, 3, 1);
	}
	Map_CopyCellsTo(111, 5, 117, 5, 5, 2);
	Map_CopyCellsTo(111, 10, 117, 10, 5, 2);
	Map_CopyCellsTo(111, 7, 111, 5, 5, 2);
	Map_CopyCellsTo(111, 7, 111, 10, 5, 2);
	for (row = 0; row < 10; row++)
	{
		Rubble_SetScaleAndSpin(&options);
		for (i = 0; i < 4 && row < 8; i++)
		{
			Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x3000000, 0,
				0x300000 + row * 0x100000 + i * 0x40000, 0, 0, 0, 0x880000, &options);
		}
		Engine_TaskWait(3);
		Map_CopyCellsTo(55, row + 26, 48, row + 3, 3, 1);
	}
	Engine_AudioPlayCue(289);
	Engine_EventWait(60);
	Engine_EventRequestExit(21);
	Engine_EventEnd();
}

static void VinasuHeya_SpawnRandomParticles(s32 x, s32 z)
{
	struct EffectOptions options;

	options.start_scale_x = 0xb333;
	options.start_scale_y = 0xb333;
	options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
	if ((gFrameCount & 3) == 0)
	{
		if (((u32)(Engine_RandomNext() << 1) >> 16) != 0)
		{
			Effect_Spawn((x + (((u32)(Engine_RandomNext() << 1) >> 16) << 4)) << 16, 0, z << 19, 0,
				0, (s32)((((u32)(Engine_RandomNext() * 5) >> 16) << 16) + 0x70000) / 10, 0x880000,
				&options);
		}
		else
		{
			Effect_Spawn((x + ((u32)(Engine_RandomNext() * 17) >> 16)) << 16, 0,
				(x << 19) - 0x40000, 0, 0, 0, 0x880000, &options);
		}
	}
}

static void SceneState_CallWith432And32(void)
{
	VinasuHeya_SpawnRandomParticles(432, 32);
}

static void FieldScene_CallWith560And44(void)
{
	VinasuHeya_SpawnRandomParticles(0x230, 44);
}

static void Scene_RunParticleWaveSequence(void)
{
	struct EffectOptions  options;
	u32					  repeat;
	s32					  zero;
	u32					  row;
	s32					  offset;
	s32					  z;
	u32					  i;
	struct EffectOptions *opts;

	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventBegin();
	Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
	Engine_ActorSetChildValue(0, 15);
	BattleFx_SetQueuedSoundAndPlay(170);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Engine_AudioPlayCue(162);
	Engine_CameraSetSpeed(0x8000, 0x1000);
	Engine_CameraMoveTo(0x1b80000, -1, 0x1680000, 1);
	repeat = 0;
	row = 0;
	opts = &options;
	offset = 0;
	do
	{
		options.start_scale_x = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
		options.start_scale_y = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
		options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
	again:
		z = 0xc00000;
		for (i = 0, zero = 0, z += offset; i <= 3; i++)
		{
			Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x1a00000, 0, z, 0, zero,
				zero, 0x880000, opts);
			z += 0x40000;
		}
		Engine_TaskWait(3);
		if (row == 3)
		{
			if (repeat <= 2)
			{
				repeat++;
				goto again;
			}
			if (repeat == 3)
				Engine_TaskAddCallback(SceneState_CallWith432And32, 3200);
		}
		Map_CopyCellsTo(53, row + 12, 26, row + 12, 3, 1);
		offset += 0x100000;
		row++;
	} while (row <= 12);
	Map_CopyCellsTo(81, 41, 89, 14, 9, 2);
	Engine_CameraWaitForMove();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_EventWait(60);
	Engine_GameFlagSet(0x306);
	Engine_EventRequestExit(19);
	Engine_EventEnd();
}

static void Scene_RunEastParticleWaveSequence(void)
{
	struct EffectOptions  options;
	u32					  repeat;
	s32					  zero;
	u32					  row;
	s32					  offset;
	s32					  z;
	u32					  i;
	struct EffectOptions *opts;

	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventBegin();
	Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
	Engine_ActorSetChildValue(0, 15);
	BattleFx_SetQueuedSoundAndPlay(170);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Engine_AudioPlayCue(162);
	Engine_CameraSetSpeed(0x8000, 0x1000);
	Engine_CameraMoveTo(0x2380000, -1, 0x1680000, 1);
	repeat = 0;
	row = 0;
	opts = &options;
	offset = 0;
	do
	{
		options.start_scale_x = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
		options.start_scale_y = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
		options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
	again:
		z = 0xc00000;
		for (i = 0, zero = 0, z += offset; i <= 3; i++)
		{
			Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x2200000, 0, z, 0, zero,
				zero, 0x880000, opts);
			z += 0x40000;
		}
		Engine_TaskWait(3);
		if (row == 3 && repeat <= 2)
		{
			repeat++;
			goto again;
		}
		Engine_TaskAddCallback(FieldScene_CallWith560And44, 3200);
		Map_CopyCellsTo(58, row + 12, 34, row + 12, 3, 1);
		offset += 0x100000;
		row++;
	} while (row <= 12);
	Map_CopyCellsTo(86, 41, 97, 14, 5, 2);
	Engine_CameraWaitForMove();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_EventWait(60);
	Engine_GameFlagSet(0x307);
	Engine_EventRequestExit(20);
	Engine_EventEnd();
}

static s32 VinasuHeya_UpdateRisingSpray(struct FieldActor *actor)
{
	struct EffectOptions  options;
	struct EffectOptions *o = &options;
	s32					  phase;
	s32					  x, y, z, rise;

	o->priority = 1;
	o->palette = 5;
	o->type = 286;
	o->script = gVinasuSprayScript;
	phase = gFrameCount & 3;
	if (phase == 0)
	{
		if ((gFrameCount & 7) == 0)
		{
			Engine_AudioPlayCue(246);
		}
		x = actor->x.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
		y = actor->y.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
		z = actor->z.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
		rise = (((u32)(Engine_RandomNext() * 4) >> 16) << 15) + 0x8000;
		Effect_Spawn(x, y, z, 0, rise, phase, 0x330000, o);
	}
	return 0;
}

static void FieldScene_RunLeaderDropSequence(void)
{
	u32				   i;
	struct FieldActor *leader;
	struct FieldActor *spray;

	leader = Object_GetById(ACTOR_PARTY_LEADER);
	spray = Object_GetById(20);
	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	leader->y.fixed = 130 << 16;
	leader->gravity = 0x8000;
	leader->bounce = 0;
	leader->motion_flags = 0;
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Event_PlayCue(204);
	Engine_EventWait(30);
	leader->motion_flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	Engine_EventWait(24);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 22);
	leader->motion_flags &= ~ACTOR_FOLLOWS_TERRAIN;
	spray->y.fixed += -3 << 16;
	leader->y.fixed += -3 << 16;
	leader->terrain_height += -3 << 16;
	Engine_TaskWait(2);
	spray->y.fixed += -2 << 16;
	leader->y.fixed += -2 << 16;
	leader->terrain_height += -2 << 16;
	Engine_TaskWait(10);
	spray->y.fixed += 2 << 16;
	leader->y.fixed += 2 << 16;
	leader->terrain_height += 2 << 16;
	Engine_TaskWait(4);
	spray->y.fixed += 2 << 16;
	leader->y.fixed += 2 << 16;
	leader->terrain_height += 2 << 16;
	Engine_TaskWait(4);
	spray->y.fixed += 1 << 16;
	leader->y.fixed += 1 << 16;
	leader->terrain_height += 1 << 16;
	leader->motion_flags = 0;
	spray->motion_flags = 0;
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x100);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_EventWait(40);
	leader->update = (void *)VinasuHeya_UpdateRisingSpray;
	Engine_EventWait(60);
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
	Engine_ActorSetSpritePriority(20, 1);
	Event_PlayCue(17);
	Event_PlayCue(0x134);
	GameFlag_Set(0x101);
	for (i = 0; i < 128; i++)
	{
		leader->y.fixed += 1 << 16;
		leader->terrain_height += 1 << 16;
		spray->y.fixed += 1 << 16;
		Engine_TaskWait(1);
	}
	Engine_EventRequestExit(21);
}

struct TrackAnimation
{
	s16 id;
};

struct TrackSprite
{
	u8					   unknown_00[0x24];
	u8					   shape;
	u8					   unknown_25[3];
	struct TrackAnimation *animations[4];
};

static s32 VinasuHeya_DispatchPushScript(struct FieldActor *actor)
{
	s32					pos[3];
	struct FieldActor  *piece;
	struct TrackSprite *sprite;
	const u8		   *shapes;
	u8					shape;
	u32					cell;
	s32					skip;
	const u8		   *cursor;

	pos[0] = actor->x.fixed;
	pos[1] = actor->y.fixed - 0x100000;
	pos[2] = actor->z.fixed;
	piece = SceneData_FindSlotAtPosition(pos, 0);
	sprite = (struct TrackSprite *)piece->sprite;
	if (sprite->animations[0]->id == 0x100)
	{
		s32 vx = actor->velocity_x;
		s32 ax = vx < 0 ? -vx : vx;
		s32 vz = actor->velocity_z;
		s32 az = vz < 0 ? -vz : vz;

		if (ax > az)
		{
			if (vx / 0x10000 < 0)
			{
				shapes = gVinasuShapesPushedLeft;
			}
			else
			{
				shapes = gVinasuShapesPushedRight;
			}
		}
		else
		{
			if (vz / 0x10000 < 0)
			{
				shapes = gVinasuShapesPushedUp;
			}
			else
			{
				shapes = gVinasuShapesPushedDown;
			}
		}
		for (; *shapes != 0; shapes++)
		{
			if (sprite->shape == *shapes)
			{
				break;
			}
		}
		if (*shapes == 0)
		{
			Engine_ObjectSetScript(actor, gVinasuRefusedPushScript);
			return 0;
		}
		if (gGameState.scene == (s32)&SceneId_VinasuHeya5)
		{
			for (cell = 0; cell <= 3
						   && !(actor->x.fixed >> 20 == gVinasuSwitchCells[cell].x
								&& actor->z.fixed >> 20 == gVinasuSwitchCells[cell].z);
				cell++)
			{
			}
			for (skip = 0;; skip++)
			{
				cursor = gVinasuSwitchShapes[cell];
				shape = *cursor;
				if (shape == 0)
				{
					Engine_ObjectSetScript(actor, gVinasuRefusedPushScript);
					return 0;
				}
				if (shape == ((struct TrackSprite *)piece->sprite)->shape)
				{
					break;
				}
				gVinasuSwitchShapes[cell] = cursor + 1;
			}
			Engine_ObjectSetScript(actor, gVinasuSwitchPushScripts[cell][skip]);
		}
		else
		{
			for (cell = 0; cell <= 7
						   && !(actor->x.fixed >> 20 == gVinasuPillarCells[cell].x
								&& actor->z.fixed >> 20 == gVinasuPillarCells[cell].z);
				cell++)
			{
			}
			for (skip = 0;; skip++)
			{
				cursor = gVinasuPillarShapes[cell];
				shape = *cursor;
				if (shape == 0)
				{
					Engine_ObjectSetScript(actor, gVinasuRefusedPushScript);
					return 0;
				}
				if (shape == ((struct TrackSprite *)piece->sprite)->shape)
				{
					break;
				}
				gVinasuPillarShapes[cell] = cursor + 1;
			}
			Engine_ObjectSetScript(actor, gVinasuPillarPushScripts[cell][skip]);
		}
	}
	else
	{
		Engine_ObjectSetScript(actor, gVinasuRefusedPushScript);
	}
	return 0;
}

s32 gEffectScript0[] = {
	SC_WAIT(1),
	SC_LOOP(22, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript1[] = {
	SC_WAIT(1),
	SC_LOOP(44, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript2[] = {
	SC_WAIT(1),
	SC_LOOP(126, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

// 向き (16 方向) ごとの一歩: 上位 16 ビットが x、下位 16 ビットが z
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

u32 StagedActor_DirectionSteps[] = {
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(16, 0),
};

struct SwitchCell gVinasuSwitchCells[] = {
	{ 48, 41 },
	{ 52, 41 },
	{ 48, 43 },
	{ 52, 43 },
};

u32 gVinasuBlockHeights[] = {
	0xfff00000, 0xfff00000, 0xffe00000, 0xffd00000, 0xffc00000, 0xffb00000, 0xffb00000,
};

struct SwitchCell gVinasuPillarCells[] = {
	{ 11, 39 },
	{ 14, 39 },
	{ 11, 41 },
	{ 16, 42 },
	{ 10, 43 },
	{ 14, 43 },
	{ 10, 46 },
	{ 16, 46 },
};

u8 gVinasuShapesPushedLeft[] = {
	5, 6, 7, 0,
};

u8 gVinasuShapesPushedRight[] = {
	4, 6, 8, 0,
};

u8 gVinasuShapesPushedUp[] = {
	3, 7, 8, 0,
};

u8 gVinasuShapesPushedDown[] = {
	3, 4, 5, 0,
};

u8 gVinasuSwitchShape0[] = {
	6, 7, 8, 0,
};

u8 gVinasuSwitchShape1[] = {
	6, 7, 8, 0,
};

u8 gVinasuSwitchShape2[] = {
	4, 5, 6, 0,
};

u8 gVinasuSwitchShape3[] = {
	4, 5, 6, 0,
};

u8 gVinasuPillarShape0[] = {
	7, 0,
};

u8 gVinasuPillarShape1[] = {
	0, 5,
};

u8 gVinasuPillarShape2[] = {
	0, 8,
};

u8 gVinasuPillarShape3[] = {
	0, 5,
};

u8 gVinasuPillarShape4[] = {
	0, 7, 8, 0,
};

u8 gVinasuPillarShape5[] = {
	3, 0,
};

u8 gVinasuPillarShape6[] = {
	3, 0,
};

const s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
};

s32 gVinasuHeyaPlacements2_0[] = {
	SC_SET(0x23, 0x2126),
	SC_SET(0x27, 1),
	SC_CALL(VinasuHeya_UpdateChildFromTimer),
	SC_END,
};

s32 gVinasuHeyaPlacements3_Data[] = {
	SC_SET(0x23, 0x2126),
	SC_CALL(VinasuHeya_UpdateChildFromTimer),
	SC_END,
};

s32 gVinasuLeaderApproachScript[] = {
	SC_ARG(12),
	SC_SET(SW_HOOK, (s32)OverlayObject_UpdateEveryFourFrames),
	SC_LABEL(1),
	SC_SET(SW_SCALE_X, 0x11999),
	SC_SET(SW_SCALE_Y, 0x11999),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0xe666),
	SC_SET(SW_SCALE_Y, 0xe666),
	SC_WAIT(2),
	SC_LOOP(10, SC_TO(1)),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_END,
};

s32 gVinasuHeyaPlacements2_1[] = {
	SC_SET(SW_SCALE_X, 0x9999),
	SC_SET(SW_SCALE_Y, 0x9999),
	SC_SET(SW_COLLISION, 0),
	SC_CALL(OverlayObject_ApplyZero),
	SC_END,
};

s32 gVinasuSprayScript[] = {
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_WAIT(6),
	SC_SET(SW_SCALE_Y, 0xa000),
	SC_SET(SW_SCALE_X, 0xa000),
	SC_WAIT(6),
	SC_SET(SW_SCALE_Y, 0x4000),
	SC_SET(SW_SCALE_X, 0x4000),
	SC_WAIT(6),
	SC_SET(SW_SCALE_Y, 0xa000),
	SC_SET(SW_SCALE_X, 0xa000),
	SC_WAIT(6),
	SC_LOOP(4, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gVinasuHeyaPlacements4_1[] = {
	SC_ARG(3),
	SC_END,
};

s32 gVinasuHeyaPlacements5_3[] = {
	SC_ARG(4),
	SC_END,
};

s32 gVinasuHeyaPlacements6_4[] = {
	SC_ARG(5),
	SC_END,
};

s32 gVinasuHeyaPlacements5_2[] = {
	SC_ARG(6),
	SC_END,
};

s32 gVinasuHeyaPlacements5_1[] = {
	SC_ARG(7),
	SC_END,
};

s32 gVinasuHeyaPlacements6_3[] = {
	SC_ARG(8),
	SC_END,
};

s32 gVinasuPushScript[] = {
	SC_CALL(OverlayObject_ApplyZero),
	SC_ARG(7),
	SC_SET(SW_SPEED, 0x10000),
	SC_SET(SW_HOOK, (s32)SceneEffect_SpawnRandomizedParticleEveryFourFrames),
	SC_MOVE(PIXELS(760), 0, PIXELS(728)),
	SC_SYNC,
	SC_POS(PIXELS(760), 0, PIXELS(728)),
	SC_MOVE(PIXELS(760), 0, PIXELS(744)),
	SC_SYNC,
	SC_POS(PIXELS(760), 0, PIXELS(744)),
	SC_MOVE(PIXELS(744), 0, PIXELS(744)),
	SC_SYNC,
	SC_POS(PIXELS(743), 0, PIXELS(744)),
	SC_MOVE(PIXELS(744), 0, PIXELS(696)),
	SC_SYNC,
	SC_POS(PIXELS(743), 0, PIXELS(696)),
	SC_WAIT(1),
	SC_END,
};

s32 gVinasuSettleScriptA[] = {
	SC_MOVE(PIXELS(872), 0, PIXELS(680)),
	SC_SYNC,
	SC_POS(PIXELS(872), 0, PIXELS(680)),
	SC_MOVE(PIXELS(872), 0, PIXELS(664)),
	SC_SYNC,
	SC_POS(PIXELS(872), 0, PIXELS(664)),
	SC_MOVE(PIXELS(840), 0, PIXELS(664)),
	SC_SYNC,
	SC_SET(SW_X, PIXELS(840)),
	SC_SET(SW_Z, PIXELS(664)),
	SC_CALL(VinasuHeya_DispatchPushScript),
	SC_END,
};

s32 gVinasuSettleScriptB[] = {
	SC_MOVE(PIXELS(888), 0, PIXELS(696)),
	SC_SYNC,
	SC_POS(PIXELS(888), 0, PIXELS(696)),
	SC_MOVE(PIXELS(840), 0, PIXELS(696)),
	SC_SYNC,
	SC_SET(SW_X, PIXELS(840)),
	SC_SET(SW_Z, PIXELS(696)),
	SC_CALL(VinasuHeya_DispatchPushScript),
	SC_END,
};

s32 gVinasuRefusedPushScript[] = {
	SC_SET(SW_HOOK, 0),
	SC_WAIT(10),
	SC_FREE,
};

s32 gVinasuSwitchPushScript0_0[] = {
	SC_MOVE(PIXELS(728), 0, PIXELS(664)),
	SC_SYNC,
	SC_POS(PIXELS(728), 0, PIXELS(664)),
	SC_MOVE(PIXELS(728), 0, PIXELS(632)),
	SC_SYNC,
	SC_POS(PIXELS(728), 0, PIXELS(632)),
	SC_SET(0x28, 1),
	SC_END,
};

s32 gVinasuSwitchPushScript0_1[] = {
	SC_MOVE(PIXELS(776), 0, PIXELS(696)),
	SC_SYNC,
	SC_SET(SW_X, PIXELS(776)),
	SC_SET(SW_Z, PIXELS(696)),
	SC_CALL(VinasuHeya_DispatchPushScript),
	SC_END,
};

s32 gVinasuSwitchPushScript1_0[] = {
	SC_MOVE(PIXELS(776), 0, PIXELS(664)),
	SC_SYNC,
	SC_SET(SW_X, PIXELS(776)),
	SC_SET(SW_Z, PIXELS(664)),
	SC_CALL(VinasuHeya_DispatchPushScript),
	SC_END,
};

s32 gVinasuSwitchPushScript1_1[] = {
	SC_MOVE(PIXELS(840), 0, PIXELS(696)),
	SC_SYNC,
	SC_SET(SW_X, PIXELS(840)),
	SC_SET(SW_Z, PIXELS(696)),
	SC_CALL(VinasuHeya_DispatchPushScript),
	SC_END,
};

s32 gVinasuSwitchPushScript2_1[] = {
	SC_MOVE(PIXELS(776), 0, PIXELS(664)),
	SC_SYNC,
	SC_SET(SW_X, PIXELS(776)),
	SC_SET(SW_Z, PIXELS(664)),
	SC_CALL(VinasuHeya_DispatchPushScript),
	SC_END,
};

s32 gVinasuSwitchPushScript2_0[] = {
	SC_MOVE(PIXELS(728), 0, PIXELS(696)),
	SC_SYNC,
	SC_POS(PIXELS(728), 0, PIXELS(696)),
	SC_MOVE(PIXELS(728), 0, PIXELS(632)),
	SC_SYNC,
	SC_POS(PIXELS(728), 0, PIXELS(632)),
	SC_SET(0x28, 1),
	SC_END,
};

s32 gVinasuSwitchPushScript3_1[] = {
	SC_MOVE(PIXELS(840), 0, PIXELS(664)),
	SC_SYNC,
	SC_SET(SW_X, PIXELS(840)),
	SC_SET(SW_Z, PIXELS(664)),
	SC_CALL(VinasuHeya_DispatchPushScript),
	SC_END,
};

s32 gVinasuSwitchPushScript3_0[] = {
	SC_MOVE(PIXELS(776), 0, PIXELS(696)),
	SC_SYNC,
	SC_SET(SW_X, PIXELS(776)),
	SC_SET(SW_Z, PIXELS(696)),
	SC_CALL(VinasuHeya_DispatchPushScript),
	SC_END,
};

s32 gVinasuFirstGateScript[] = {
	SC_MOVE(PIXELS(216), 0, PIXELS(776)),
	SC_SYNC,
	SC_POS(PIXELS(216), 0, PIXELS(776)),
	SC_MOVE(PIXELS(216), 0, PIXELS(760)),
	SC_SYNC,
	SC_POS(PIXELS(216), 0, PIXELS(760)),
	SC_END,
};

s32 gVinasuFirstGateOpenScript[] = {
	SC_MOVE(PIXELS(264), 0, PIXELS(760)),
	SC_SYNC,
	SC_POS(PIXELS(264), 0, PIXELS(760)),
	SC_MOVE(PIXELS(264), 0, PIXELS(744)),
	SC_SYNC,
	SC_SET(SW_X, PIXELS(264)),
	SC_SET(SW_Z, PIXELS(744)),
	SC_CALL(VinasuHeya_DispatchPushScript),
	SC_END,
};

s32 gVinasuPillarPushScript7_0[] = {
	SC_MOVE(PIXELS(264), 0, PIXELS(728)),
	SC_SYNC,
	SC_POS(PIXELS(264), 0, PIXELS(728)),
	SC_MOVE(PIXELS(232), 0, PIXELS(728)),
	SC_SYNC,
	SC_POS(PIXELS(232), 0, PIXELS(728)),
	SC_MOVE(PIXELS(232), 0, PIXELS(696)),
	SC_SYNC,
	SC_SET(SW_X, PIXELS(232)),
	SC_SET(SW_Z, PIXELS(696)),
	SC_CALL(VinasuHeya_DispatchPushScript),
	SC_END,
};

s32 gVinasuPillarPushScript5_1[] = {
	SC_MOVE(PIXELS(216), 0, PIXELS(696)),
	SC_SYNC,
	SC_POS(PIXELS(216), 0, PIXELS(696)),
	SC_MOVE(PIXELS(216), 0, PIXELS(664)),
	SC_SYNC,
	SC_POS(PIXELS(216), 0, PIXELS(664)),
	SC_MOVE(PIXELS(184), 0, PIXELS(664)),
	SC_SYNC,
	SC_SET(SW_X, PIXELS(184)),
	SC_SET(SW_Z, PIXELS(664)),
	SC_CALL(VinasuHeya_DispatchPushScript),
	SC_END,
};

s32 gVinasuPillarPushScript5_0[] = {
	SC_MOVE(PIXELS(264), 0, PIXELS(696)),
	SC_SYNC,
	SC_POS(PIXELS(264), 0, PIXELS(696)),
	SC_MOVE(PIXELS(264), 0, PIXELS(680)),
	SC_SYNC,
	SC_SET(SW_X, PIXELS(264)),
	SC_SET(SW_Z, PIXELS(680)),
	SC_CALL(VinasuHeya_DispatchPushScript),
	SC_END,
};

s32 gVinasuPillarPushScript2_0[] = {
	SC_MOVE(PIXELS(184), 0, PIXELS(632)),
	SC_SYNC,
	SC_SET(SW_X, PIXELS(184)),
	SC_SET(SW_Z, PIXELS(632)),
	SC_CALL(VinasuHeya_DispatchPushScript),
	SC_END,
};

s32 gVinasuPillarPushScript3_0[] = {
	SC_MOVE(PIXELS(248), 0, PIXELS(680)),
	SC_SYNC,
	SC_POS(PIXELS(248), 0, PIXELS(680)),
	SC_MOVE(PIXELS(248), 0, PIXELS(664)),
	SC_SYNC,
	SC_POS(PIXELS(248), 0, PIXELS(664)),
	SC_MOVE(PIXELS(264), 0, PIXELS(664)),
	SC_SYNC,
	SC_POS(PIXELS(264), 0, PIXELS(664)),
	SC_MOVE(PIXELS(264), 0, PIXELS(648)),
	SC_SYNC,
	SC_POS(PIXELS(264), 0, PIXELS(648)),
	SC_MOVE(PIXELS(248), 0, PIXELS(648)),
	SC_SYNC,
	SC_POS(PIXELS(248), 0, PIXELS(648)),
	SC_MOVE(PIXELS(248), 0, PIXELS(632)),
	SC_SYNC,
	SC_POS(PIXELS(248), 0, PIXELS(632)),
	SC_MOVE(PIXELS(232), 0, PIXELS(632)),
	SC_SYNC,
	SC_SET(SW_X, PIXELS(232)),
	SC_SET(SW_Z, PIXELS(632)),
	SC_CALL(VinasuHeya_DispatchPushScript),
	SC_END,
};

s32 gVinasuSecondGateScript[] = {
	SC_MOVE(PIXELS(216), 0, PIXELS(744)),
	SC_SYNC,
	SC_POS(PIXELS(216), 0, PIXELS(744)),
	SC_MOVE(PIXELS(184), 0, PIXELS(744)),
	SC_SYNC,
	SC_POS(PIXELS(184), 0, PIXELS(744)),
	SC_MOVE(PIXELS(184), 0, PIXELS(760)),
	SC_SYNC,
	SC_POS(PIXELS(184), 0, PIXELS(760)),
	SC_MOVE(PIXELS(168), 0, PIXELS(760)),
	SC_SYNC,
	SC_POS(PIXELS(168), 0, PIXELS(760)),
	SC_MOVE(PIXELS(168), 0, PIXELS(744)),
	SC_SYNC,
	SC_SET(SW_X, PIXELS(168)),
	SC_SET(SW_Z, PIXELS(744)),
	SC_CALL(VinasuHeya_DispatchPushScript),
	SC_END,
};

s32 gVinasuPillarPushScript6_0[] = {
	SC_MOVE(PIXELS(168), 0, PIXELS(728)),
	SC_SYNC,
	SC_POS(PIXELS(168), 0, PIXELS(728)),
	SC_MOVE(PIXELS(184), 0, PIXELS(728)),
	SC_SYNC,
	SC_POS(PIXELS(184), 0, PIXELS(728)),
	SC_MOVE(PIXELS(184), 0, PIXELS(696)),
	SC_SYNC,
	SC_POS(PIXELS(184), 0, PIXELS(696)),
	SC_MOVE(PIXELS(168), 0, PIXELS(696)),
	SC_SYNC,
	SC_SET(SW_X, PIXELS(168)),
	SC_SET(SW_Z, PIXELS(696)),
	SC_CALL(VinasuHeya_DispatchPushScript),
	SC_END,
};

s32 gVinasuPillarPushScript4_0[] = {
	SC_MOVE(PIXELS(168), 0, PIXELS(664)),
	SC_SYNC,
	SC_POS(PIXELS(168), 0, PIXELS(664)),
	SC_MOVE(PIXELS(184), 0, PIXELS(664)),
	SC_SYNC,
	SC_SET(SW_X, PIXELS(184)),
	SC_SET(SW_Z, PIXELS(664)),
	SC_CALL(VinasuHeya_DispatchPushScript),
	SC_END,
};

s32 gVinasuPillarPushScript0_0[] = {
	SC_MOVE(PIXELS(216), 0, PIXELS(632)),
	SC_SYNC,
	SC_POS(PIXELS(216), 0, PIXELS(632)),
	SC_MOVE(PIXELS(216), 0, PIXELS(600)),
	SC_SYNC,
	SC_POS(PIXELS(216), 0, PIXELS(600)),
	SC_SET(0x28, 1),
	SC_END,
};

u16 gVinasuPushCells[] = {
	MAP_CELL_STEP(124, 50, 1, 2, 6), MAP_CELL_STEP(124, 47, 1, 2, 6), MAP_CELL_STEP(124, 44, 1, 2, 6), MAP_CELL_STEP(124, 41, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 gVinasuSettleCells[] = {
	MAP_CELL_STEP(117, 59, 1, 2, 6), MAP_CELL_STEP(115, 59, 1, 2, 6), MAP_CELL_STEP(113, 59, 1, 2, 6), MAP_CELL_STEP(111, 59, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 gVinasuGateCells[] = {
	MAP_CELL_STEP(96, 58, 1, 2, 6), MAP_CELL_STEP(95, 58, 1, 2, 6), MAP_CELL_STEP(96, 56, 1, 2, 6), MAP_CELL_STEP(95, 56, 1, 2, 6), MAP_CELL_STEPS_END,
};

struct SceneEntrance gVinasuHeyaEntrances1[] = {
	{ 1, CONDITION_ALWAYS, 40, 0, 264, FACING_EAST, 0, 0, 0, 512, 544, 0 },
	{ 2, CONDITION_ALWAYS, 376, 0, 88, FACING_SOUTH, 0, 0, 0, 512, 544, 0 },
	{ 3, CONDITION_ALWAYS, 488, 0, 424, FACING_WEST, 0, 0, 0, 512, 544, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gVinasuHeyaEntrancesOther[] = {
	{ 1, CONDITION_ALWAYS, 120, 0, 200, FACING_NORTH, 0, 0, 56, 240, 216, 0 },
	{ 2, CONDITION_ALWAYS, 120, 0, 104, FACING_SOUTH, 0, 0, 56, 240, 216, 0 },
	{ 3, CONDITION_ALWAYS, 344, 0, 232, FACING_NORTH, 0, 256, 48, 512, 248, 0 },
	{ 4, CONDITION_ALWAYS, 472, 0, 184, FACING_SOUTH, 0, 256, 48, 512, 248, 0 },
	{ 5, CONDITION_ALWAYS, 376, 0, 184, FACING_SOUTH, 0, 256, 48, 512, 248, 0 },
	{ 6, CONDITION_ALWAYS, 344, 0, 104, FACING_SOUTH, 0, 256, 48, 512, 248, 0 },
	{ 7, CONDITION_ALWAYS, 840, 0, 248, FACING_NORTH, 0, 672, 32, 1008, 256, 0 },
	{ 8, CONDITION_ALWAYS, 968, 0, 168, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
	{ 9, CONDITION_ALWAYS, 856, 0, 88, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
	{ 10, CONDITION_ALWAYS, 808, 0, 88, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
	{ 11, CONDITION_ALWAYS, 712, 0, 168, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
	{ 12, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 16, 288, 256, 520, 0 },
	{ 13, CONDITION_ALWAYS, 104, 0, 504, FACING_NORTH, 0, 16, 288, 256, 520, 0 },
	{ 14, CONDITION_ALWAYS, 104, 0, 472, FACING_SOUTH, 0, 16, 288, 256, 520, 0 },
	{ 20, CONDITION_ALWAYS, 520, 0, 696, FACING_SOUTH, 0, 304, 344, 576, 728, 0 },
	{ 21, CONDITION_ALWAYS, 344, 0, 568, FACING_SOUTH, 0, 304, 344, 576, 728, 0 },
	{ 22, CONDITION_ALWAYS, 728, 0, 936, FACING_SOUTH, 0, 600, 736, 840, 952, 0 },
	{ 23, CONDITION_ALWAYS, 744, 0, 792, FACING_SOUTH, 0, 600, 736, 840, 952, 0 },
	{ 24, CONDITION_ALWAYS, 392, 0, 952, FACING_SOUTH, 0, 272, 808, 512, 984, 0 },
	{ 25, CONDITION_ALWAYS, 392, 0, 856, FACING_SOUTH, 0, 272, 808, 512, 984, 0 },
	{ 26, CONDITION_ALWAYS, 760, 0, 936, FACING_SOUTH, 0, 600, 736, 840, 952, 0 },
	{ 30, CONDITION_ALWAYS, 88, 0, 872, FACING_SOUTH, 0, 16, 600, 256, 920, 0 },
	{ 31, CONDITION_ALWAYS, 184, 0, 872, FACING_SOUTH, 0, 16, 600, 256, 920, 0 },
	{ 32, CONDITION_ALWAYS, 184, 0, 648, FACING_SOUTH, 0, 16, 600, 256, 920, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gVinasuHeyaEntrances3[] = {
	{ 1, CONDITION_ALWAYS, 456, 0, 552, FACING_SOUTH, 0, 376, 344, 736, 624, 0 },
	{ 2, CONDITION_ALWAYS, 456, 0, 592, FACING_NORTH, 0, 376, 344, 736, 624, 0 },
	{ 3, CONDITION_ALWAYS, 696, 0, 472, FACING_SOUTH, 0, 376, 344, 736, 624, 0 },
	{ 4, CONDITION_ALWAYS, 696, 0, 512, FACING_NORTH, 0, 376, 344, 736, 624, 0 },
	{ 5, CONDITION_ALWAYS, 424, 0, 136, FACING_SOUTH, 0, 344, 48, 592, 216, 0 },
	{ 6, CONDITION_ALWAYS, 520, 0, 176, FACING_NORTH, 0, 344, 48, 592, 216, 0 },
	{ 7, CONDITION_ALWAYS, 104, 0, 752, FACING_NORTH, 0, 56, 584, 304, 880, 0 },
	{ 8, CONDITION_ALWAYS, 200, 0, 680, FACING_SOUTH, 0, 56, 584, 304, 880, 0 },
	{ 9, CONDITION_ALWAYS, 88, 0, 344, FACING_SOUTH, 0, 40, 280, 288, 496, 0 },
	{ 10, CONDITION_ALWAYS, 248, 0, 432, FACING_NORTH, 0, 40, 280, 288, 496, 0 },
	{ 11, CONDITION_ALWAYS, 664, 0, 264, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
	{ 12, CONDITION_ALWAYS, 728, 0, 136, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
	{ 13, CONDITION_ALWAYS, 840, 0, 136, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
	{ 14, CONDITION_ALWAYS, 952, 0, 136, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
	{ 15, CONDITION_ALWAYS, 840, 0, 360, FACING_NORTH, 0, 616, 40, 1000, 360, 0 },
	{ 16, CONDITION_ALWAYS, 88, 0, 152, FACING_SOUTH, 0, 40, 40, 328, 208, 0 },
	{ 17, CONDITION_ALWAYS, 184, 0, 104, FACING_SOUTH, 0, 40, 40, 328, 208, 0 },
	{ 18, CONDITION_ALWAYS, 280, 0, 152, FACING_SOUTH, 0, 40, 40, 328, 208, 0 },
	{ 19, CONDITION_ALWAYS, 472, 0, 776, FACING_SOUTH, 0, 424, 688, 672, 848, 0 },
	{ 20, CONDITION_ALWAYS, 600, 0, 776, FACING_SOUTH, 0, 424, 688, 672, 848, 0 },
	{ 21, CONDITION_ALWAYS, 840, 0, 136, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gVinasuHeyaEntrances4[] = {
	{ 1, CONDITION_ALWAYS, 104, 0, 120, FACING_SOUTH, 0, 56, 56, 368, 272, 0 },
	{ 2, CONDITION_ALWAYS, 312, 0, 200, FACING_SOUTH, 0, 56, 56, 368, 272, 0 },
	{ 3, CONDITION_ALWAYS, 600, 0, 216, FACING_NORTH, 0, 552, 88, 848, 544, 0 },
	{ 4, CONDITION_ALWAYS, 696, 0, 312, FACING_SOUTH, 0, 552, 88, 848, 544, 0 },
	{ 5, CONDITION_ALWAYS, 648, 0, 536, FACING_NORTH, 0, 552, 88, 848, 544, 0 },
	{ 6, CONDITION_ALWAYS, 776, 0, 536, FACING_SOUTH, 0, 552, 88, 848, 544, 0 },
	{ 7, CONDITION_ALWAYS, 104, 0, 520, FACING_SOUTH, 0, 56, 392, 352, 608, 0 },
	{ 8, CONDITION_ALWAYS, 312, 0, 584, FACING_NORTH, 0, 56, 392, 352, 608, 0 },
	{ 9, CONDITION_ALWAYS, 744, 0, 696, FACING_SOUTH, 0, 648, 632, 896, 800, 0 },
	{ 10, CONDITION_ALWAYS, 808, 0, 696, FACING_SOUTH, 0, 648, 632, 896, 800, 0 },
	{ 11, CONDITION_ALWAYS, 312, 0, 536, FACING_SOUTH, 0, 56, 392, 352, 608, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gVinasuHeyaEntrances5[] = {
	{ 1, CONDITION_ALWAYS, 136, 0, 136, FACING_SOUTH, 0, 8, 56, 256, 224, 0 },
	{ 2, CONDITION_ALWAYS, 136, 0, 184, FACING_NORTH, 0, 8, 56, 256, 224, 0 },
	{ 3, CONDITION_ALWAYS, 136, 0, 392, FACING_NORTH, 0, 8, 264, 256, 432, 0 },
	{ 4, CONDITION_ALWAYS, 312, 0, 680, FACING_SOUTH, 0, 248, 600, 512, 768, 0 },
	{ 5, CONDITION_ALWAYS, 392, 0, 680, FACING_SOUTH, 0, 248, 600, 512, 768, 0 },
	{ 6, CONDITION_ALWAYS, 472, 0, 680, FACING_SOUTH, 0, 248, 600, 512, 768, 0 },
	{ 7, CONDITION_ALWAYS, 136, 0, 792, FACING_SOUTH, 0, 8, 728, 256, 928, 0 },
	{ 8, CONDITION_ALWAYS, 136, 0, 904, FACING_NORTH, 0, 8, 728, 256, 928, 0 },
	{ 9, CONDITION_ALWAYS, 312, 0, 232, FACING_NORTH, 0, 264, 56, 720, 480, 0 },
	{ 10, CONDITION_ALWAYS, 376, 0, 184, FACING_SOUTH, 0, 264, 56, 720, 480, 0 },
	{ 11, CONDITION_ALWAYS, 504, 0, 248, FACING_SOUTH, 0, 264, 56, 720, 480, 0 },
	{ 12, CONDITION_ALWAYS, 680, 0, 120, FACING_SOUTH, 0, 264, 56, 720, 480, 0 },
	{ 13, CONDITION_ALWAYS, 312, 0, 440, FACING_SOUTH, 0, 264, 56, 720, 480, 0 },
	{ 14, CONDITION_ALWAYS, 632, 0, 392, FACING_NORTH, 0, 264, 56, 720, 480, 0 },
	{ 15, CONDITION_ALWAYS, 728, 0, 632, FACING_SOUTH, 0, 680, 568, 928, 768, 0 },
	{ 16, CONDITION_ALWAYS, 888, 0, 632, FACING_SOUTH, 0, 680, 568, 928, 768, 0 },
	{ 17, CONDITION_ALWAYS, 136, 0, 568, FACING_SOUTH, 0, 8, 480, 256, 648, 0 },
	{ 18, CONDITION_ALWAYS, 136, 0, 600, FACING_NORTH, 0, 8, 480, 256, 648, 0 },
	{ 19, CONDITION_ALWAYS, 440, 0, 264, FACING_NORTH, 0, 264, 56, 720, 480, 0 },
	{ 20, CONDITION_ALWAYS, 568, 0, 264, FACING_NORTH, 0, 264, 56, 720, 480, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gVinasuHeyaEntrances6[] = {
	{ 1, CONDITION_ALWAYS, 120, 0, 248, FACING_SOUTH, 0, 72, 56, 320, 416, 0 },
	{ 2, CONDITION_ALWAYS, 280, 0, 280, FACING_NORTH, 0, 72, 56, 320, 416, 0 },
	{ 3, CONDITION_ALWAYS, 472, 0, 328, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
	{ 4, CONDITION_ALWAYS, 536, 0, 216, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
	{ 5, CONDITION_ALWAYS, 664, 0, 88, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
	{ 6, CONDITION_ALWAYS, 744, 0, 152, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
	{ 7, CONDITION_ALWAYS, 840, 0, 152, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
	{ 8, CONDITION_ALWAYS, 936, 0, 280, FACING_NORTH, 0, 424, 24, 992, 560, 0 },
	{ 9, CONDITION_ALWAYS, 472, 0, 472, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
	{ 10, CONDITION_ALWAYS, 616, 0, 456, FACING_NORTH, 0, 424, 24, 992, 560, 0 },
	{ 11, CONDITION_ALWAYS, 728, 0, 456, FACING_NORTH, 0, 424, 24, 992, 560, 0 },
	{ 12, CONDITION_ALWAYS, 824, 0, 456, FACING_NORTH, 0, 424, 24, 992, 560, 0 },
	{ 13, CONDITION_ALWAYS, 792, 0, 648, FACING_SOUTH, 0, 712, 584, 976, 864, 0 },
	{ 14, CONDITION_ALWAYS, 904, 0, 648, FACING_SOUTH, 0, 712, 584, 976, 864, 0 },
	{ 15, CONDITION_ALWAYS, 104, 0, 776, FACING_SOUTH, 0, 56, 536, 304, 848, 0 },
	{ 16, CONDITION_ALWAYS, 216, 0, 600, FACING_SOUTH, 0, 56, 536, 304, 848, 0 },
	{ 17, CONDITION_ALWAYS, 600, 0, 744, FACING_SOUTH, 0, 392, 648, 640, 832, 0 },
	{ 18, CONDITION_ALWAYS, 536, 0, 328, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
	{ 19, CONDITION_ALWAYS, 792, 0, 328, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
	{ 20, CONDITION_ALWAYS, 664, 0, 136, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
	{ SCENE_TABLE_END },
};

u32 gVinasuHeyaPrimaryTable[] = {
	SCENE_EXITS(181),
	SCENE_EXIT(1, 2, 32),
	SCENE_EXIT(2, 182, 1),
	SCENE_EXIT(3, 2, 61),
	SCENE_EXIT(0, 182, 0),
	SCENE_EXIT(1, 181, 2),
	SCENE_EXIT(2, 182, 3),
	SCENE_EXIT(3, 182, 2),
	SCENE_EXIT(4, 182, 31),
	SCENE_EXIT(5, 182, 30),
	SCENE_EXIT(6, 182, 7),
	SCENE_EXIT(7, 182, 6),
	SCENE_EXIT(8, 182, 32),
	SCENE_EXIT(9, 182, 22),
	SCENE_EXIT(10, 182, 12),
	SCENE_EXIT(11, 182, 13),
	SCENE_EXIT(12, 182, 10),
	SCENE_EXIT(13, 182, 11),
	SCENE_EXIT(14, 182, 24),
	SCENE_EXIT(20, 182, 26),
	SCENE_EXIT(21, 182, 25),
	SCENE_EXIT(22, 182, 9),
	SCENE_EXIT(23, 183, 16),
	SCENE_EXIT(24, 182, 14),
	SCENE_EXIT(25, 182, 21),
	SCENE_EXIT(26, 182, 20),
	SCENE_EXIT(30, 182, 5),
	SCENE_EXIT(31, 182, 4),
	SCENE_EXIT(32, 182, 8),
	SCENE_EXIT(0, 183, 0),
	SCENE_EXIT(1, 183, 19),
	SCENE_EXIT(2, 183, 9),
	SCENE_EXIT(3, 184, 1),
	SCENE_EXIT(4, 183, 5),
	SCENE_EXIT(5, 183, 4),
	SCENE_EXIT(6, 183, 13),
	SCENE_EXIT(7, 183, 14),
	SCENE_EXIT(8, 184, 4),
	SCENE_EXIT(9, 183, 2),
	SCENE_EXIT(10, 183, 11),
	SCENE_EXIT(11, 183, 10),
	SCENE_EXIT(12, 184, 7),
	SCENE_EXIT(13, 183, 6),
	SCENE_EXIT(14, 183, 7),
	SCENE_EXIT(15, 183, 17),
	SCENE_EXIT(16, 182, 23),
	SCENE_EXIT(17, 183, 15),
	SCENE_EXIT(18, 173, 16),
	SCENE_EXIT(19, 183, 1),
	SCENE_EXIT(20, 183, 20),
	SCENE_EXIT(21, 184, 11),
	SCENE_EXIT(0, 184, 0),
	SCENE_EXIT(1, 183, 3),
	SCENE_EXIT(2, 184, 3),
	SCENE_EXIT(3, 184, 2),
	SCENE_EXIT(4, 183, 8),
	SCENE_EXIT(5, 184, 10),
	SCENE_EXIT(6, 185, 17),
	SCENE_EXIT(7, 183, 12),
	SCENE_EXIT(8, 184, 9),
	SCENE_EXIT(9, 184, 8),
	SCENE_EXIT(10, 184, 5),
	SCENE_EXIT(11, 183, 21),
	SCENE_EXIT(0, 185, 0),
	SCENE_EXIT(1, 186, 1),
	SCENE_EXIT(2, 185, 4),
	SCENE_EXIT(3, 185, 5),
	SCENE_EXIT(4, 185, 2),
	SCENE_EXIT(5, 185, 3),
	SCENE_EXIT(6, 185, 9),
	SCENE_EXIT(7, 186, 5),
	SCENE_EXIT(8, 185, 11),
	SCENE_EXIT(9, 185, 6),
	SCENE_EXIT(10, 186, 4),
	SCENE_EXIT(11, 185, 8),
	SCENE_EXIT(12, 186, 7),
	SCENE_EXIT(13, 186, 9),
	SCENE_EXIT(14, 185, 15),
	SCENE_EXIT(15, 185, 14),
	SCENE_EXIT(16, 185, 18),
	SCENE_EXIT(17, 184, 6),
	SCENE_EXIT(18, 185, 16),
	SCENE_EXIT(19, 186, 18),
	SCENE_EXIT(20, 186, 19),
	SCENE_EXIT(0, 186, 0),
	SCENE_EXIT(1, 185, 1),
	SCENE_EXIT(2, 186, 3),
	SCENE_EXIT(3, 186, 2),
	SCENE_EXIT(4, 185, 10),
	SCENE_EXIT(5, 185, 7),
	SCENE_EXIT(6, 186, 17),
	SCENE_EXIT(7, 185, 12),
	SCENE_EXIT(8, 186, 16),
	SCENE_EXIT(9, 185, 13),
	SCENE_EXIT(10, 186, 13),
	SCENE_EXIT(11, 186, 14),
	SCENE_EXIT(12, 186, 15),
	SCENE_EXIT(13, 186, 10),
	SCENE_EXIT(14, 186, 11),
	SCENE_EXIT(15, 186, 12),
	SCENE_EXIT(16, 186, 8),
	SCENE_EXIT(17, 186, 6),
	SCENE_EXIT(18, 185, 19),
	SCENE_EXIT(19, 185, 20),
	SCENE_EXIT(20, 186, 20),
	SCENE_EXIT(21, 187, 1),
	SCENE_EXITS_END,
};

struct ScenePlacement gVinasuHeyaPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gVinasuHeyaPlacements1[] = {
	{ 0x98, 0x981, ACTOR_STAND,
		PIXELS(128), 0, PIXELS(268), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x70, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(352), 0, PIXELS(300), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x70, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(272), 0, PIXELS(428), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x70, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(368), 0, PIXELS(140), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(400), 0, PIXELS(172), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(416), 0, PIXELS(412), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(192), 0, PIXELS(364), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gVinasuHeyaPlacements2[] = {
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x70, 0x0, ACTOR_STAND,
		PIXELS(128), 0, PIXELS(140), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x71, 0x0, ACTOR_STAND,
		PIXELS(304), 0, PIXELS(204), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x70, 0x0, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(592), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x100, 0x0, ACTOR_STAND,
		PIXELS(872), 0, PIXELS(120), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x100, 0x0, ACTOR_STAND,
		PIXELS(840), 0, PIXELS(88), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x1f4, 0x0, (s32)gVinasuHeyaPlacements2_0,
		PIXELS(664), 0, PIXELS(792), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x114, 0x0, (s32)gVinasuHeyaPlacements2_1,
		PIXELS(488), 0, PIXELS(536), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x1f4, 0x0, (s32)gVinasuHeyaPlacements2_0,
		PIXELS(72), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x1f4, 0x0, (s32)gVinasuHeyaPlacements2_0,
		PIXELS(72), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x1f4, 0x0, (s32)gVinasuHeyaPlacements2_0,
		PIXELS(168), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x1f4, 0x0, (s32)gVinasuHeyaPlacements2_0,
		PIXELS(168), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x1f4, 0x0, (s32)gVinasuHeyaPlacements2_0,
		PIXELS(72), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x1f4, 0x0, (s32)gVinasuHeyaPlacements2_0,
		PIXELS(104), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x1f4, 0x0, (s32)gVinasuHeyaPlacements2_0,
		PIXELS(136), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x1f4, 0x0, (s32)gVinasuHeyaPlacements2_0,
		PIXELS(168), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x1f4, 0x0, (s32)gVinasuHeyaPlacements2_0,
		PIXELS(200), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gVinasuHeyaPlacements3[] = {
	{ 0xfd, 0x0, (s32)gVinasuHeyaPlacements3_Data,
		PIXELS(248), 0, PIXELS(712), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xfd, 0x0, (s32)gVinasuHeyaPlacements3_Data,
		PIXELS(184), 0, PIXELS(824), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xfd, 0x0, (s32)gVinasuHeyaPlacements3_Data,
		PIXELS(200), 0, PIXELS(824), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x100, 0x0, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(392), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x100, 0x0, ACTOR_STAND,
		PIXELS(88), 0, PIXELS(376), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gVinasuHeyaPlacements4[] = {
	{ 0x1f4, 0x0, (s32)gVinasuHeyaPlacements2_0,
		PIXELS(808), 0, PIXELS(728), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x100, 0x0, (s32)gVinasuHeyaPlacements4_1,
		PIXELS(776), 0, PIXELS(712), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x11c, CONDITION_ALWAYS, 7,
		PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gVinasuHeyaPlacements5[] = {
	{ 0x1f4, 0x0, (s32)gVinasuHeyaPlacements2_0,
		PIXELS(888), 0, PIXELS(680), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x100, 0x0, (s32)gVinasuHeyaPlacements5_1,
		PIXELS(792), 0, PIXELS(680), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x100, 0x0, (s32)gVinasuHeyaPlacements5_2,
		PIXELS(808), 0, PIXELS(648), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x100, 0x0, (s32)gVinasuHeyaPlacements5_3,
		PIXELS(824), 0, PIXELS(680), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x11c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gVinasuHeyaPlacements6[] = {
	{ 0x101, 0x0, 7,
		PIXELS(104), 0, PIXELS(312), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x101, 0x0, 7,
		PIXELS(168), 0, PIXELS(312), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x100, 0x0, ACTOR_STAND,
		PIXELS(120), 0, PIXELS(280), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x100, 0x0, ACTOR_STAND,
		PIXELS(136), 0, PIXELS(280), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x100, 0x0, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(280), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x100, 0x0, ACTOR_STAND,
		PIXELS(248), 0, PIXELS(120), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x1f4, 0x0, (s32)gVinasuHeyaPlacements2_0,
		PIXELS(136), 0, PIXELS(776), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x100, 0x0, (s32)gVinasuHeyaPlacements4_1,
		PIXELS(184), 0, PIXELS(616), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x100, 0x0, (s32)gVinasuHeyaPlacements5_1,
		PIXELS(200), 0, PIXELS(616), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x100, 0x0, (s32)gVinasuHeyaPlacements6_3,
		PIXELS(232), 0, PIXELS(616), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x100, 0x0, (s32)gVinasuHeyaPlacements6_4,
		PIXELS(248), 0, PIXELS(616), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x11c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0xf2, 0x0, 7,
		PIXELS(664), 0, PIXELS(136), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gVinasuHeyaEvents1[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorEightTenStepLoop) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgVinasuHeyaIodemBeHappyTheEntranceTo) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgVinasuHeyaOhhhHowCouldTheyAttackMere) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TALK) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgVinasuHeyaHowHowDidTheyOpenThe) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgVinasuHeyaTheyAreAlreadyHeadedForBabi) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgVinasuHeyaBabisSoldiersAreAllColossoVictors) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorEightTenStepLoop) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgVinasuHeyaUhnnnIfOnlyICouldMove) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgVinasuHeyaTheirPowersWereSoStrangeWho) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgVinasuHeyaIMustGetInsideVenusLighthouse) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgVinasuHeyaTheYoungBoySeemedToGlow) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgVinasuHeyaTheyWerentInVenusLighthouseVery) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgVinasuHeyaIThoughtIRecognizedOneOf) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gVinasuHeyaEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ 0x31, 4, CONDITION_ALWAYS, 4 },
	{ 0x31, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
	{ 0x31, 8, CONDITION_ALWAYS, 8 },
	{ 0x21, 9, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
	{ EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
	{ 0x21, 14, CONDITION_ALWAYS, 14 },
	{ 0x31, 20, CONDITION_ALWAYS, 20 },
	{ 0x31, 21, CONDITION_ALWAYS, 21 },
	{ 0x31, 22, CONDITION_ALWAYS, 22 },
	{ 0x31, 23, CONDITION_ALWAYS, 23 },
	{ 0x31, 24, CONDITION_ALWAYS, 24 },
	{ 0x21, 25, CONDITION_ALWAYS, 25 },
	{ 0x21, 26, CONDITION_ALWAYS, 26 },
	{ 0x21, 30, CONDITION_ALWAYS, 30 },
	{ 0x21, 31, CONDITION_ALWAYS, 31 },
	{ 0x21, 32, CONDITION_ALWAYS, 32 },
	{ EVENT_TOUCH, 45, 0x200, EVENT_SCRIPT(EV45_TOUCH) },
	{ EVENT_TOUCH, 46, CONDITION_ALWAYS, EVENT_SCRIPT(EV46_TOUCH) },
	{ 0x202, 50, CONDITION_ALWAYS, EVENT_SCRIPT(EV50_SCRIPT) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgVinasuHeyaKradenToldThemToStopBut) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgVinasuHeyaKradenWentInsideButThenCame) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgVinasuHeyaIfKradenHadntBeenThereWe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgVinasuHeyaKradenSaidSomethingAboutThisBeing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgVinasuHeyaIWonderWhatMysteriesTheStatue) },
	{ SEARCH(SEARCH_UNNAMED), 35, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ReadRelief) },
#if !EDITION_INTERNATIONAL
	{ SEARCH(SEARCH_UNNAMED), 36, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ReadRelief) },
	{ SEARCH(SEARCH_UNNAMED), 37, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ReadRelief) },
	{ SEARCH(SEARCH_UNNAMED), 38, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ReadRelief) },
	{ SEARCH(SEARCH_UNNAMED), 39, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ReadRelief) },
	{ SEARCH(SEARCH_UNNAMED), 40, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ReadRelief) },
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
	{ SEARCH(SEARCH_UNNAMED), 40, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ReadRelief) },
#endif
	{ SEARCH(SEARCH_UNNAMED), 41, CONDITION_ALWAYS, EVENT_SCRIPT(EV41_SEARCH) },
	{ SEARCH(SEARCH_CHEST), 100, 0xf37, FIND_ITEM(0xa1) },
	{ SEARCH(SEARCH_CHEST), 101, 0xf38, FIND_ITEM(0xce) },
	{ PSYNERGY_ON_ACTOR(0x8c), 12, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunFlag986ActorOneScene) },
	{ PSYNERGY_ON_ACTOR(0x8c), 13, 0x200, EVENT_SCRIPT(EV13_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0x93), 12, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetFlag987AtActorTwelveTile) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplySixRectsAfterFlag161) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplySixRectsAfter161) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gVinasuHeyaEvents3[] = {
	{ 0x31, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x21, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
	{ 0x21, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ 0x21, 12, CONDITION_ALWAYS, 12 },
	{ EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
	{ EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
	{ EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
	{ 0x21, 16, CONDITION_ALWAYS, 16 },
	{ EVENT_EXIT, 17, CONDITION_ALWAYS, 17 },
	{ 0x31, 18, CONDITION_ALWAYS, 18 },
	{ 0x21, 19, CONDITION_ALWAYS, 19 },
	{ EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_PassRange0To1) },
	{ EVENT_TOUCH, 35, CONDITION_ALWAYS, EVENT_SCRIPT(EV35_TOUCH_3) },
	{ 0x202, 31, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyRectAt19_44AndRunThree) },
	{ 0x202, 32, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyRectAt19_44AndRunThree) },
	{ EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(EV31_TOUCH_3) },
	{ EVENT_TOUCH, 32, CONDITION_ALWAYS, EVENT_SCRIPT(EV32_TOUCH_3) },
	{ 0x202, 45, CONDITION_ALWAYS, EVENT_SCRIPT(EV45_SCRIPT_3) },
	{ SEARCH(SEARCH_CHEST), 100, 0xf34, FIND_ITEM(0xa2) },
	{ SEARCH(SEARCH_UNNAMED), 110, 0x350, FIND_PSYNERGY_STONE },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_PlaceAndPinSlots8To10) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_PlaceAndPinSlots8To10) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_PlaceAndPinSlots8To10) },
	{ 0x10008c15, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_ApplyKind45AtActorsElevenAndTwelve) },
	{ 0x10008c15, 12, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_ApplyKind45AtActorsElevenAndTwelve) },
	{ 0x10009315, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_ApplyKind45AtActorsElevenAndTwelve) },
	{ 0x10009315, 12, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_ApplyKind45AtActorsElevenAndTwelve) },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_ApplyPositionsOfActors11And12) },
	{ PSYNERGY_ON_ACTOR(0x8c), 12, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_ApplyPositionsOfActors11And12) },
	{ PSYNERGY_ON_ACTOR(0x93), 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_UpdateSlots11And12ByTile) },
	{ PSYNERGY_ON_ACTOR(0x93), 12, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_UpdateSlots11And12ByTile) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gVinasuHeyaEvents4[] = {
	{ 0x31, 1, CONDITION_ALWAYS, 1 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(SceneEffect_RunObjectZeroColorSequence) },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ 0x31, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ 0x21, 6, CONDITION_ALWAYS, 6 },
	{ 0x31, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_TOUCH, 35, CONDITION_ALWAYS, EVENT_SCRIPT(EV35_TOUCH_4) },
	{ 0x202, 36, 0x301, EVENT_SCRIPT(EV36_SCRIPT) },
	{ SEARCH(SEARCH_UNNAMED), 25, CONDITION_ALWAYS, EVENT_SCRIPT(EV25_SEARCH) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, 0x301, EVENT_SCRIPT(EV09_PSYNERGY) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gVinasuHeyaEvents5[] = {
	{ 0x21, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(SceneEffect_RunObjectZeroColorSequence) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(SceneEffect_RunObjectZeroColorSequence) },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ 0x21, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ 0x21, 10, CONDITION_ALWAYS, 10 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneEffect_RunObjectZeroColorSequence) },
	{ 0x21, 12, CONDITION_ALWAYS, 12 },
	{ 0x21, 13, CONDITION_ALWAYS, 13 },
	{ EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
	{ EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
	{ EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
	{ 0x31, 17, CONDITION_ALWAYS, 17 },
	{ EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
	{ EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_PassRange0To1) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_PassRange0To1) },
	{ 0x202, 35, CONDITION_ALWAYS, EVENT_SCRIPT(EV35_SCRIPT) },
	{ SEARCH(SEARCH_UNNAMED), 25, CONDITION_ALWAYS, EVENT_SCRIPT(EV25_SEARCH) },
	{ SEARCH(SEARCH_CHEST), 100, 0xf35, FIND_ITEM(0x52) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, CONDITION_ALWAYS, EVENT_SCRIPT(VinasuHeya_SettlePushedBlocks) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, CONDITION_ALWAYS, EVENT_SCRIPT(VinasuHeya_SettlePushedBlocks) },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, CONDITION_ALWAYS, EVENT_SCRIPT(VinasuHeya_SettlePushedBlocks) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gVinasuHeyaEvents6[] = {
	{ 0x31, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ 0x31, 4, CONDITION_ALWAYS, 4 },
	{ 0x31, 5, CONDITION_ALWAYS, 5 },
	{ 0x21, 6, CONDITION_ALWAYS, 6 },
	{ 0x31, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ 0x31, 9, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
	{ EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
	{ EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
	{ EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
	{ EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
	{ 0x31, 17, CONDITION_ALWAYS, 17 },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_TOUCH) },
	{ 0x202, 35, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunThreeCallSequence) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 36, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_TryMoveActorZeroTwoTilesAhead) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 36, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_TryMoveActorZeroTwoTilesAhead) },
	{ 0x202, 36, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunThreeCallSequence) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 37, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_TryMoveActorZeroTwoTilesAhead) },
	{ 0x202, 37, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunThreeCallSequence) },
	{ EVENT_TOUCH, 40, CONDITION_ALWAYS, EVENT_SCRIPT(EV40_TOUCH) },
	{ EVENT_TOUCH, 41, CONDITION_ALWAYS, EVENT_SCRIPT(EV41_TOUCH) },
	{ EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_PassRange0To1) },
	{ EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(EV31_TOUCH_6) },
	{ EVENT_TOUCH, 32, CONDITION_ALWAYS, EVENT_SCRIPT(EV32_TOUCH_6) },
	{ EVENT_TOUCH, 33, CONDITION_ALWAYS, EVENT_SCRIPT(EV33_TOUCH) },
	{ 0x202, 45, CONDITION_ALWAYS, EVENT_SCRIPT(EV45_SCRIPT_6) },
	{ SEARCH(SEARCH_UNNAMED), 25, CONDITION_ALWAYS, EVENT_SCRIPT(EV25_SEARCH) },
	{ SEARCH(SEARCH_CHEST), 100, 0xf36, FIND_ITEM(0x9) },
	{ SEARCH(SEARCH_UNNAMED), 110, 0x351, FIND_PSYNERGY_STONE },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, CONDITION_ALWAYS, 0 },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, CONDITION_ALWAYS, 0 },
	{ PSYNERGY_ON_ACTOR(0x8c), 12, CONDITION_ALWAYS, 0 },
	{ PSYNERGY_ON_ACTOR(0x8c), 13, CONDITION_ALWAYS, 0 },
	{ PSYNERGY_ON_ACTOR(0x93), 10, CONDITION_ALWAYS, EVENT_SCRIPT(VinasuHeya_ResolveFloatingBlock) },
	{ PSYNERGY_ON_ACTOR(0x93), 11, CONDITION_ALWAYS, EVENT_SCRIPT(VinasuHeya_ResolveFloatingBlock) },
	{ PSYNERGY_ON_ACTOR(0x93), 12, CONDITION_ALWAYS, EVENT_SCRIPT(VinasuHeya_ResolveFloatingBlock) },
	{ PSYNERGY_ON_ACTOR(0x93), 13, CONDITION_ALWAYS, EVENT_SCRIPT(VinasuHeya_ResolveFloatingBlock) },
	{ PSYNERGY_ON_ACTOR(0x8c), 15, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunScene3c8SequenceA) },
	{ PSYNERGY_ON_ACTOR(0x8c), 16, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunScene3c8SequenceA) },
	{ PSYNERGY_ON_ACTOR(0x8c), 17, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunScene3c8SequenceA) },
	{ PSYNERGY_ON_ACTOR(0x8c), 18, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunScene3c8SequenceA) },
	{ SCENE_EVENTS_END },
};

u8 *gVinasuSwitchShapes[] = {
	gVinasuSwitchShape0,
	gVinasuSwitchShape1,
	gVinasuSwitchShape2,
	gVinasuSwitchShape3,
};

s32 *gVinasuSwitchPushScript0[] = {
	gVinasuSwitchPushScript0_0,
	gVinasuSwitchPushScript0_1,
	gVinasuRefusedPushScript,
	gVinasuRefusedPushScript,
};

s32 *gVinasuSwitchPushScript1[] = {
	gVinasuSwitchPushScript1_0,
	gVinasuSwitchPushScript1_1,
	gVinasuRefusedPushScript,
	gVinasuRefusedPushScript,
};

s32 *gVinasuSwitchPushScript2[] = {
	gVinasuSwitchPushScript2_0,
	gVinasuSwitchPushScript2_1,
	gVinasuSwitchPushScript2_0,
	gVinasuRefusedPushScript,
};

s32 *gVinasuSwitchPushScript3[] = {
	gVinasuSwitchPushScript3_0,
	gVinasuSwitchPushScript3_1,
	gVinasuSwitchPushScript3_0,
	gVinasuRefusedPushScript,
};

s32 * *gVinasuSwitchPushScripts[] = {
	gVinasuSwitchPushScript0,
	gVinasuSwitchPushScript1,
	gVinasuSwitchPushScript2,
	gVinasuSwitchPushScript3,
};

u8 *gVinasuPillarShapes[] = {
	gVinasuPillarShape0,
	gVinasuPillarShape1,
	gVinasuPillarShape1 + 1,
	gVinasuPillarShape2 + 1,
	gVinasuPillarShape3 + 1,
	gVinasuPillarShape4 + 1,
	gVinasuPillarShape5,
	gVinasuPillarShape6,
};

s32 *gVinasuPillarPushScript0[] = {
	gVinasuPillarPushScript0_0,
	gVinasuRefusedPushScript,
};

s32 *gVinasuPillarPushScript1[] = {
	gVinasuRefusedPushScript,
};

s32 *gVinasuPillarPushScript2[] = {
	gVinasuPillarPushScript2_0,
	gVinasuRefusedPushScript,
};

s32 *gVinasuPillarPushScript3[] = {
	gVinasuPillarPushScript3_0,
	gVinasuRefusedPushScript,
};

s32 *gVinasuPillarPushScript4[] = {
	gVinasuPillarPushScript4_0,
	gVinasuRefusedPushScript,
};

s32 *gVinasuPillarPushScript5[] = {
	gVinasuPillarPushScript5_0,
	gVinasuPillarPushScript5_1,
	gVinasuRefusedPushScript,
};

s32 *gVinasuPillarPushScript6[] = {
	gVinasuPillarPushScript6_0,
	gVinasuRefusedPushScript,
};

s32 *gVinasuPillarPushScript7[] = {
	gVinasuPillarPushScript7_0,
	gVinasuRefusedPushScript,
};

s32 * *gVinasuPillarPushScripts[] = {
	gVinasuPillarPushScript0,
	gVinasuPillarPushScript1,
	gVinasuPillarPushScript2,
	gVinasuPillarPushScript3,
	gVinasuPillarPushScript4,
	gVinasuPillarPushScript5,
	gVinasuPillarPushScript6,
	gVinasuPillarPushScript7,
};
