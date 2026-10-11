#ifndef IMIRU_FUCHIN_H
#define IMIRU_FUCHIN_H

#include "types.h"
#include "keystate.h"
#include "field_event.h"
#include "field_scene.h"

enum
{

	ITEM_DRAGONS_EYE = 230
};

#define NULL ((void *)0)

extern void *gWorkSlot[];

struct TrackingWork
{
	u8				   unknown_00[24];
	struct FieldActor *actor;
};

extern s16				  gImiruFuchinKeyHeadings[];
extern s16				  gImiruFuchinHeadings[];
extern struct FieldActor *gImiruFuchinDragonsEye;
extern s32				  gImiruFuchinExits[];

void ImiruFuchin_BlowCaveMouthDust(void);
void SceneActor_TurnTowardTableAngle(union FieldObject *object);

void BattleFx_RunPageEffectForSlot();
void BattleFx_SetPhaseRequest();
void Object_CommitPosition();
s32	 PartyInventory_FindOwner();
void PartyInventory_Discard();
void Battle_ResetEffectCounter();
void Audio_PlayCueFromEventWork();
void ObjectDispatch_ApplyValueToChildren();
void Vector_AddPolarOffset();
s32	 GetMapCellCollision();
s32	 Map_GetTerrainHeight();

void ImiruFuchin_HopBy(s32 dx, s32 dz);

#endif
