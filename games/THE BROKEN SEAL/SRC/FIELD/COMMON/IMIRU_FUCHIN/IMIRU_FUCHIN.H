#ifndef IMIRU_FUCHIN_H
#define IMIRU_FUCHIN_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 gKeysHeld[];

enum {
    /* Message 0x182 + 230. */
    ITEM_DRAGONS_EYE = 230
};

#define NULL ((void *)0)

/* The engine's heap-allocation cache: each slot holds its kind's block. */
extern void *gWorkSlot[];

/* The work in slot 36; its word at +24 is the actor it tracks. */
struct TrackingWork {
    u8 unknown_00[24];
    struct FieldActor *actor;
};

/* The cave's tables and variables, in the overlay's read-only data: the
 * headings for each held direction, the Dragon's Eye object once it is
 * placed, and the exits. */
extern s16 gImiruFuchinKeyHeadings[];
extern s16 gImiruFuchinHeadings[];
extern s32 gImiruFuchinDragonsEye[];
extern s32 gImiruFuchinExits[];

void ImiruFuchin_BlowCaveMouthDust(void);
void SceneActor_TurnTowardTableAngle(union FieldObject *object);

/* The main-image services the cave calls through its import veneers. The
 * declarations are old-style because one name is reached with different
 * argument counts. */
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

/* Imports; the queried ones are typed for their return value. */

/*
 * Run this scene transition's six setup services.  The literal word following
 * the return belongs to this owner and supplies 0x121 to Engine_AudioPlayCue.
 */

/* Imports, named by the address each call site computes. */

/*
 * In-image heading table at 0x0200a464.  0x02000240 is below the link base,
 * so it is a resident table; its word at byte offset 500 selects the subject.
 */

/* Installed callback, named by the linked address of its call word. */

/*
 * Imports named by the address their call site computes, not by a runtime
 * address.  Declarations are old-style because arity varies between sites.
 */
#endif
