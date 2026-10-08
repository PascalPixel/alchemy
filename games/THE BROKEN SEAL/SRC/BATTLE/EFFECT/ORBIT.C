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
	s32 x, y, z;
};

void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct BurstPosition *pos);
void Object_SetMode(struct FieldActor *object, s32 mode);
extern struct BattleFxScene *gEffectWork;
void						 Object_SetPosition(struct FieldActor *object, s32 x, s32 y, s32 z);
s32	 Object_CheckMovementCollision(struct FieldActor *object, struct BurstPosition *pos);
void Animation_ApplyChildValuesFar(struct FieldActor *object, s32 value);
struct FieldActor *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void			   Audio_PlayCue(s32 cue);

static __inline__ void RaisedPosition(struct FieldActor *object, struct BurstPosition *pos)
{
	pos->x = object->x.fixed;
	pos->y = object->y.fixed + 0x100000;
	pos->z = object->z.fixed;
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
	s32 anchor_x;
	u8	unknown_3c[12];
	s32 speed;
	u8	unknown_4c[9];
	u8	mode;
	u8	unknown_56[8];
	u16 unknown_5e;
	u8	unknown_60[4];
	s16 phase;
	u8	unknown_66[6];
	void (*update)(struct SparkObject *);
};

void RunBattleEffect04(void)
{
	struct FieldActor	   *child;
	s32						event_context;
	struct FieldActor	   *spawned[4];
	struct BurstPosition	pos;
	struct FieldActor	   *object;
	struct FieldActor	   *copy;
	struct FieldActor	   *target;
	struct BattleFxScene   *scene;
	struct EffectSlot	   *slot;
	struct AnimationObject *resource;
	s32						event;
	s32						scale;
	s32						index;
	u8						resource_id;

	scene = gEffectWork;
	child = scene->child;
	BattleEffect_InitializeSharedScene();
	Audio_PlayCue(0x82);
	slot = scene->slots;

	for (index = 0; index <= 11; index++)
	{
		target = scene->main_object;
		RaisedPosition(target, &pos);
		Camera_WorldToScreen(&pos);
		EffectSlot_Initialize(slot, 0x11c, pos.x, pos.z);
		EffectSlot_SetCallback(slot, BattleFx_UpdateRadialBurst);
		EffectSlot_SetObjectMode(slot, 7);
		ObjectGroup_SetChildValueUnlessFifteenFar((s32)slot->object, 9);
		slot->scale_y = 0xb333;
		slot->scale_x = 0xb333;
		WaitFrames(2);
		slot++;
	}

	target = scene->main_object;
	pos.x = target->x.fixed;
	pos.y = target->y.fixed + 0x100000;
	pos.z = target->z.fixed;
	Vector_AddPolarOffset(0x80000, scene->angle, &pos);
	object = Object_Spawn(0xd7, pos.x, pos.y, pos.z);
	if (object == NULL)
	{
		BattleFx_PrepareBufferInterpolation();
		return;
	}
	object->scale_y = 0x4000;
	object->scale_x = 0x4000;
	object->facing = scene->angle;
	object->speed = 0x40000;
	object->acceleration = 0x40000;
	object->motion_flags = 0;
	Object_SetMode(object, 5);
	Animation_ApplyChildValuesFar(object, 3);
	scale = object->scale_x;
	if (scale < 0x10000)
	{
		do
		{
			scale += 0x500;
			object->scale_y = scale;
			object->scale_x = scale;
			WaitFrames(1);
			scale = object->scale_x;
		} while (scale <= 0xffff);
	}
	WaitFrames(3);
	resource = NULL;
	for (index = 2; index >= 0; index--)
	{
		copy = spawned[index] =
			Object_Spawn(0xd7, object->x.fixed, object->y.fixed, object->z.fixed);
		if (copy != NULL)
		{
			copy->scale_y = 0xf000;
			copy->scale_x = 0xf000;
			copy->facing = scene->angle;
			copy->speed = 0x40000;
			copy->acceleration = 0x40000;
			copy->motion_flags = 0;
			Object_SetMode(copy, 5);
			Animation_ApplyChildValuesFar(copy, 2);
			resource =
				Object_ReplaceResourceEntry((struct AnimationObject *)copy->sprite, resource);
		}
	}
	resource_id = resource->slot;
	if (scene->enabled != 0)
	{
		target = scene->main_object;
		pos.x = target->x.fixed;
		pos.y = target->y.fixed + 0x100000;
		pos.z = target->z.fixed;
		Vector_AddPolarOffset(0x380000, scene->angle, &pos);
	}
	else
	{
		pos.x = scene->x;
		pos.y = scene->y + 0x100000;
		pos.z = scene->z;
	}
	Object_SetPosition(object, pos.x, pos.y, pos.z);
	ObjectDispatch_InitializeFar(
		(struct DispatchObject *)object, BattleFx_BurstParticleObjectScript + 4);
	for (index = 0; index < 3; index++)
	{
		copy = spawned[index];
		if (copy != NULL)
		{
			WaitFrames(3);
			Object_SetPosition(copy, pos.x, pos.y, pos.z);
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)copy, BattleFx_CommonParticleScript + 1);
		}
	}
	index = 0;
	if (((struct ScriptMotionObject *)object)->script != NULL)
	{
	wait_script:
		WaitFrames(1);
		index++;
		if (index <= 59 && ((struct ScriptMotionObject *)object)->script != NULL)
			goto wait_script;
	}
	if (child != NULL && scene->child_mode == 0)
	{
		if (scene->child_option != 0)
			child->velocity_y = 0x80000;
		pos.x = child->x.fixed;
		pos.y = child->y.fixed;
		pos.z = child->z.fixed;
		Vector_AddPolarOffset(0x100000, scene->angle, &pos);
		if (Object_CheckMovementCollision(child, &pos) == 0
			&& ScriptObject_CheckOverlapFar(child, &pos) == 0)
		{
			child->acceleration = 0x10000;
			child->speed = 0x10000;
			Object_SetPosition(child, pos.x, pos.y, pos.z);
		}
	}
	event = BattleFx_FindMatchingEvent(0x50000005, 4, &event_context);
	if (event != 0)
		BattleFx_RunEventAction((void *)event, gGameState.selected_actor, event_context);
	WaitFrames(10);
	BattleFx_PrepareBufferInterpolation();
	WaitFrames(20);
	if (resource_id != 96)
		Resource_ResetEntry(resource_id);
}

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

void BattleFx_UpdateOrbitingParticleLeft(struct FieldActor *particle)
{
	struct BurstPosition pos;
	s16					 life;
	s32					 count;

	if (particle != NULL)
	{
		count = particle->timer - 1;
		particle->timer = count;
		life = count;
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

void BattleFx_UpdateOrbitingParticleRight(struct FieldActor *particle)
{
	struct BurstPosition pos;
	s16					 life;
	s32					 count;

	if (particle != NULL)
	{
		count = particle->timer - 1;
		particle->timer = count;
		life = count;
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

void BattleFx_StartOrbitingParticles(void)
{
	struct BattleFxScene *state = gEffectWork;
	struct FieldActor	 *child = state->child;

	if (child != 0)
	{
		if (state->child_mode != 0)
		{
			state->enabled = 1;
		}
		child->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
		BattleFx_RunOrbitingParticles();
	}
}

void BattleFx_RunOrbitingParticles(void)
{
	s32					  resource_size;
	struct BurstPosition  position;
	struct BurstPosition *p;
	struct BattleFxScene *scene;
	struct FieldActor	 *main_particle;
	struct FieldActor	 *particle;
	void				 *resource;
	s32					  entry_count;

	scene = gEffectWork;
	main_particle = scene->child;
	BattleEffect_InitializeSharedScene();
	Audio_PlayCue(0x73);

	p = &position;
	entry_count = 15;
	do
	{
		particle = Object_Spawn(0xe8, 0, 0, 0);
		if (particle != NULL)
		{
			u32 initial_scale;
			s32 magnitude;

			initial_scale = (Random16() >> 1) + 0x8000;
			particle->scale_y = initial_scale;
			particle->scale_x = initial_scale;
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
			magnitude = (Random16() << 2) + 0x20000;
			Vector_AddPolarOffset(magnitude, Random16(), p);
			particle->target_x = p->x;
			particle->target_y = p->y;
			particle->target_z = p->z;
		}

		WaitFrames(3);
		entry_count--;
	} while (entry_count >= 0);

	WaitFrames(10);
	Audio_PlayCue(0x73);
	WaitFrames(50);

	if (main_particle != NULL && scene->enabled == 0)
	{
		Audio_PlayCue(0xd4);

		entry_count = 15;
		do
		{
			Animation_ApplyChildValuesFar(main_particle, 7);
			WaitFrames(1);
			Animation_ApplyChildValuesFar(main_particle, 0);
			WaitFrames(4);
			entry_count--;
		} while (entry_count >= 0);

		if (scene->child_option == 0)
		{
			Audio_PlayCue(0xdc);
			Object_SetMode(main_particle, 2);
		}

		main_particle->update = (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleMain;
		resource = BattleFx_FindMatchingEvent(0x50000005, 6, &resource_size);
		if (resource != NULL)
		{
			BattleFx_RunEventAction(resource, gGameState.selected_actor, resource_size);
		}
		WaitFrames(20);
	}

	BattleFx_PrepareBufferInterpolation();
}
void BattleFx_SwayParticle(struct SparkObject *object)
{
	s16 *phase;

	phase = &object->phase;
	object->x = object->anchor_x + Iwram_MulQ16(0x40000, Trig_Sin(*phase << 9));
	(*phase)++;
	*phase = (*phase + 128) % 128;
}

void BattleFx_UpdateShrinkingOrbitObject(struct FieldActor *object)
{
	struct BattleFxScene *scene = gEffectWork;
	struct BurstPosition  pos;
	s16					  life;
	s32					  count;

	if (object != NULL)
	{
		count = object->timer - 1;
		object->timer = count;
		life = count;
		if (life != 0)
		{
			pos.x = scene->x;
			pos.y = scene->y + 0xa0000;
			pos.z = scene->z;
			Vector_AddPolarOffset(life << 16, (s16)object->work + (life << 11), &pos);
			object->x.fixed = pos.x;
			object->y.fixed = pos.y;
			object->z.fixed = pos.z;
		}
		else
		{
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)object, BattleFx_CommonParticleScript);
		}
	}
}

void BattleFx_RunSparkEmitter(struct SparkObject *object)
{
	struct BattleFxScene *scene;
	s16					 *phase;
	struct SparkObject	 *spark;
	s32					  position[3];

	phase = &object->phase;
	scene = gEffectWork;
	if (*phase != -1)
	{
		object->x = scene->x + Iwram_MulQ16(0x60000, Trig_Sin(*phase << 10));
		object->y = scene->y + 0x100000;
		object->z = scene->z;
		(*phase)++;
		*phase = (*phase + 64) % 64;
	}
	if (gFrameCount % 3 == 0)
	{
		position[0] = object->x;
		position[1] = object->y + 0x20000;
		position[2] = object->z;
		Vector_AddPolarOffset(Random16() * 6, Random16(), position);
		spark = Object_Spawn(0x11d, position[0], position[1], position[2]);
		if (spark != 0)
		{
			spark->update = BattleFx_SwayParticle;
			spark->scale_x = spark->scale_y = 0x9999;
			spark->mode = 2;
			spark->speed = 458;
			spark->phase = Random16() >> 9;
			spark->anchor_x = spark->x;
			Animation_ApplyChildValuesFar(spark, 9);
			spark->unknown_5e = 72;
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)spark, BattleFx_CommonParticleScript);
		}
	}
}
