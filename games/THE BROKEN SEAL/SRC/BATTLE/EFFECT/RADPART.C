#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "ANIMSPR.H"
#include "MOTOBJ.H"
#include "OBJECT_RUNTIME.H"
#include "TYPES.H"
#include "FX_SCENE.H"
#include "OBJDISP.H"
#include "PARTY_STATE.H"
#include "FIXED_MATH.H"
#include "EFFECT_SLOT.H"
#include "SYSTEM.H"

#define Motion_SetTargetPositionFromMagnitudeAngle \
	((void (*)(struct CaptureObject *, s32, s32))Motion_SetTargetPositionFromMagnitudeAngle)

#define ObjectGroup_SetChildValueUnlessFifteenFar \
	((void (*)(s32, s32))ObjectGroup_SetChildValueUnlessFifteenFar)
#define BattleFx_AdvanceSpinAngle ((void (*)(void))BattleFx_AdvanceSpinAngle)
#define EffectSlot_Initialize ((void (*)(void *, s32, s32, s32))EffectSlot_Initialize)
#define ObjectGroup_SetChildValueUnlessFifteenFar \
	((void (*)(s32, s32))ObjectGroup_SetChildValueUnlessFifteenFar)

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

struct ObjectRuntime *Object_GetById(u32 actor);
void				  BattleFx_ClearOwnedSlot(struct EffectSlot *effect);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct EffectVector *position);
void AudioCommand_PlayFar(s32 cue);

extern struct BattleFxScene *gEffectWork;
void						 Motion_SetVarCbAndRefresh(s32, s32);

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
	u8	  unknown_68[4];
	void *callback;
};

void				  ObjectMotion_Launch(s32 id, s32 height, s32 frames);
void				  ObjectDispatch_ApplyArgumentToChildrenFar(struct ObjectRuntime *object, s32 mode);
void				  ObjectMotion_ArmCallback(s32 id, s32 value, s32 flags);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void				  Animation_ApplyChildValuesFar(struct CaptureObject *object, s32 value);
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
