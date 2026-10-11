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
	u8							unknown_00[6];
	u16							direction_and_kind;
	union StagedActorCoordinate x;
	s32							y;
	union StagedActorCoordinate z;
	u8							unknown_14[0x0e];
	u8							transition_mode;
	u8							unknown_23;
	s32							velocity_x;
	s32							velocity_y;
	s32							velocity_z;
	s32							move_rate_x;
	s32							move_rate_z;
	s32							target_x;
	s32							target_y;
	s32							target_z;
	u8							unknown_44[0x0c];
	struct AnimationObject	   *animation;
	u8							unknown_54[5];
	u8							collision_flags;
	u8							unknown_5a[8];
	u8							transition_busy;
	u8							unknown_63[3];
	s16							vertical_motion_direction;
	struct FieldActor		   *linked_object;
	u32							movement_callback;
};

#define STAGED_ACTOR_OFFSET(field) ((u32) & (((struct StagedActor *)0)->field))
typedef char StagedActor_size[sizeof(struct StagedActor) == 0x70 ? 1 : -1];
typedef char StagedActor_x_offset[STAGED_ACTOR_OFFSET(x) == 0x08 ? 1 : -1];
typedef char
	StagedActor_transition_mode_offset[STAGED_ACTOR_OFFSET(transition_mode) == 0x22 ? 1 : -1];
typedef char StagedActor_animation_offset[STAGED_ACTOR_OFFSET(animation) == 0x50 ? 1 : -1];
typedef char
	StagedActor_collision_flags_offset[STAGED_ACTOR_OFFSET(collision_flags) == 0x59 ? 1 : -1];
typedef char
	StagedActor_transition_busy_offset[STAGED_ACTOR_OFFSET(transition_busy) == 0x62 ? 1 : -1];
typedef char StagedActor_vertical_motion_direction_offset
	[STAGED_ACTOR_OFFSET(vertical_motion_direction) == 0x66 ? 1 : -1];
typedef char
	StagedActor_movement_callback_offset[STAGED_ACTOR_OFFSET(movement_callback) == 0x6c ? 1 : -1];
#undef STAGED_ACTOR_OFFSET

struct StagedActorProbe
{
	s32 footprint_index;
	s32 actor_slot;
	s32 position_x;
	s32 position_y;
	s32 position_z;
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
	StagedActorProbe_position_x_offset[STAGED_ACTOR_PROBE_OFFSET(position_x) == 0x08 ? 1 : -1];
typedef char
	StagedActorProbe_position_z_offset[STAGED_ACTOR_PROBE_OFFSET(position_z) == 0x10 ? 1 : -1];
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
