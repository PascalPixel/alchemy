#ifndef IMIRU_FUCHIN_H
#define IMIRU_FUCHIN_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 gKeysHeld[];

enum {

    ITEM_DRAGONS_EYE = 230
};

#define NULL ((void *)0)

/* The engine's heap-allocation cache: each slot holds its kind's block. */
extern void *gWorkSlot[];

/* The work in slot 36 tracks an actor. */
struct TrackingWork {
    u8 unknown_00[24];
    struct FieldActor *actor;
};

/* The cave's heading tables, placed Dragon's Eye and exits. */
extern s16 gImiruFuchinKeyHeadings[];
extern s16 gImiruFuchinHeadings[];
extern struct FieldActor *gImiruFuchinDragonsEye;
extern s32 gImiruFuchinExits[];

void ImiruFuchin_BlowCaveMouthDust(void);
void SceneActor_TurnTowardTableAngle(union FieldObject *object);

/* The cave's shared engine services. */
void BattleFx_RunPageEffectForSlot();
void BattleFx_SetPhaseRequest();
void Object_SetPosition();
void Object_CommitPosition();
s32 PartyInventory_FindOwner();
void PartyInventory_Discard();
void Battle_ResetEffectCounter();
void Audio_PlayCueFromEventWork();
void ObjectDispatch_ApplyValueToChildren();
void *ObjectTable_Get();
void Vector_AddPolarOffset();
s32 GetMapCellCollision();
s32 Map_GetTerrainHeight();

void ImiruFuchin_HopBy(s32 dx, s32 dz);

#endif
