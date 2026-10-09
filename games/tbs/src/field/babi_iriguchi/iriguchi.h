#ifndef IRIGUCHI_H
#define IRIGUCHI_H

#include "mapcopy.h"

#define FIELD_STAGED_ACTOR_IMPORTS
#include "types.h"
#include "field_event.h"
#include "field_scene.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

#include "field_effect.h"
#include "staged_actor.h"
#include "spawn_configured_effect.h"
#include "select_overlay_data_by_runtime_selector.h"
extern u8 gEffectWork[];

struct Owner
{
	u8 unk0[9];
	u8 unk9_0 : 2;
	u8 mode : 2;
	u8 unk9_4 : 4;
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

extern u8 gBabiIriguchiExits[];

void BabiIriguchi_OpenTruthDoor(void);
void BabiIriguchi_CloseTruthDoor(void);

struct StagedActor *BabiIriguchi_FindActorAhead(struct StagedActor *actor);

void Engine_TaskWait(s32 frames);
void Engine_EventWait(s32 frames);
void Engine_ActorSetSpeed(s32 actor, s32 speed, s32 acceleration);
void Engine_ActorSetDestinationOffset(s32 actor, s32 dx, s32 dz);
void Engine_ActorWaitForMove(s32 actor);
void Engine_ActorSetAnimation(s32 actor, s32 mode);
void Engine_MapCopyCellsLayered(
	s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height);
void BattleFx_SetPhaseRequest();
void Battle_ResetEffectCounter();
void Motion_LaunchFromFocusedObject();
void BattleFx_RunRisingObjectSequence();
s32	 FieldEffect_UpdateGridPlacement();
s32	 battle_owner_69();
void BabiIriguchi_JumpFromLedge();

static __inline__ s32 Iriguchi_Divide(s32 dividend, s32 divisor)
{
	return dividend / divisor;
}

#endif
