#ifndef ALCHEMY_STAGED_ACTOR_H
#define ALCHEMY_STAGED_ACTOR_H

#include "types.h"
#include "animspr.h"

union StagedActorCoordinate
{
	s32 value;
	struct
	{
		s16 fraction;
		s16 cell;
	} parts;
};

struct StagedActor
{
	u8							unk00[6];
	u16							directionAndKind;
	union StagedActorCoordinate x;
	s32							y;
	union StagedActorCoordinate z;
	u8							unk14[0x0e];
	u8							transitionMode;
	u8							unk23;
	s32							velocityX;
	s32							velocityY;
	s32							velocityZ;
	s32							moveRateX;
	s32							moveRateZ;
	s32							targetX;
	s32							targetY;
	s32							targetZ;
	u8							unk44[0x0c];
	struct AnimationObject	   *animation;
	u8							unk54[5];
	u8							collisionFlags;
	u8							unk5a[8];
	u8							transitionBusy;
	u8							unk63[3];
	s16							verticalMotionDirection;
	struct FieldActor		   *linkedObject;
	u32							movementCallback;
};

#define STAGED_ACTOR_OFFSET(field) ((u32) & (((struct StagedActor *)0)->field))
typedef char StagedActor_size[sizeof(struct StagedActor) == 0x70 ? 1 : -1];
typedef char StagedActor_x_offset[STAGED_ACTOR_OFFSET(x) == 0x08 ? 1 : -1];
typedef char
	StagedActor_transition_mode_offset[STAGED_ACTOR_OFFSET(transitionMode) == 0x22 ? 1 : -1];
typedef char StagedActor_animation_offset[STAGED_ACTOR_OFFSET(animation) == 0x50 ? 1 : -1];
typedef char
	StagedActor_collision_flags_offset[STAGED_ACTOR_OFFSET(collisionFlags) == 0x59 ? 1 : -1];
typedef char
	StagedActor_transition_busy_offset[STAGED_ACTOR_OFFSET(transitionBusy) == 0x62 ? 1 : -1];
typedef char StagedActor_vertical_motion_direction_offset
	[STAGED_ACTOR_OFFSET(verticalMotionDirection) == 0x66 ? 1 : -1];
typedef char
	StagedActor_movement_callback_offset[STAGED_ACTOR_OFFSET(movementCallback) == 0x6c ? 1 : -1];
#undef STAGED_ACTOR_OFFSET

struct StagedActorProbe
{
	s32 footprintIndex;
	s32 actorSlot;
	s32 positionX;
	s32 positionY;
	s32 positionZ;
	void (*callback)(void);
};

struct StagedActorProbePosition
{
	s32 x;
	s32 y;
	s32 z;
};

struct StagedActorFootprint
{
	s32 x0;
	s32 z0;
	s32 x1;
	s32 z1;
};

#define STAGED_ACTOR_PROBE_OFFSET(field) ((u32) & (((struct StagedActorProbe *)0)->field))
typedef char StagedActorProbe_size[sizeof(struct StagedActorProbe) == 0x18 ? 1 : -1];
typedef char
	StagedActorProbe_position_x_offset[STAGED_ACTOR_PROBE_OFFSET(positionX) == 0x08 ? 1 : -1];
typedef char
	StagedActorProbe_position_z_offset[STAGED_ACTOR_PROBE_OFFSET(positionZ) == 0x10 ? 1 : -1];
#undef STAGED_ACTOR_PROBE_OFFSET

typedef char StagedActorFootprint_size[sizeof(struct StagedActorFootprint) == 0x10 ? 1 : -1];

struct StagedActorProbePoint
{
	s32 x;
	s32 y;
	s32 z;
};

typedef char StagedActorProbePoint_size[sizeof(struct StagedActorProbePoint) == 0x0c ? 1 : -1];

#endif
