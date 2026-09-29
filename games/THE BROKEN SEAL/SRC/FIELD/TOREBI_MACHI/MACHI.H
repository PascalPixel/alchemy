#ifndef TOREBI_MACHI_MACHI_H
#define TOREBI_MACHI_MACHI_H

/* Tolbi town (resource_3b5): the declarations, views and call helpers its
 * scene sources share. */

/*
 * Overlay resource_3b5: shared scene work plus the actor setup, dialogue and
 * event sequences that drive this scene, in address order.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FIELD_EFFECT.H"
extern u8 gWindowWork[];

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

/*
 * Object-id slot at 0x1f4 of the shared scene work record, set once below to
 * the id of the object created earlier in the same function.
 */
#define SCENE_OBJECT_ID (*(s32 *)((u8 *)gEventWork + 0x1f4))

struct SceneSetup {
    u16 *commands;
    u16 first;
    u16 second;
};

/*
 * The sprite of an actor in this scene repeats its priority bits in the byte
 * at 0x15, beside the priority FieldSprite names at 0x09.
 */
struct SceneSprite {
    u8 unknown_00[0x15];
    u8 unknown_15_low : 2;
    u8 priority_15 : 2;
    u8 unknown_15_high : 4;
};

struct Actor {
    u8 pad00[0x23];
    u8 field23;
    u8 pad24[0x31];
    u8 field55;
};
extern u8 gTorebiMachiActor16Action[];
extern u8 gTorebiMachiActor17Action[];
extern u8 gTorebiMachiEntrances[];
extern u8 gTorebiMachiExits[];
extern u8 gTorebiMachiPlacements[];
extern u8 gTorebiMachiEvents3[];
extern u8 gTorebiMachiEvents2[];
extern u8 gTorebiMachiEvents[];
extern struct SceneSetup gTorebiMachiCellSteps[];

void BattleFx_RunPageEffectForSlot(s32, s32, s32);

s32 SceneActor_UpdatePlayerProximity();
void SetMapCellCollision();
void FieldScene_RunScene3b5_02000224();
void FieldScene_ResetActor9AndDrawTiles();
s32 UiWindow_CreateWithSideObject();
void UiText_OpenMessageWindow();

/*
 * Call sites spelled through the wrappers below pass their constants straight
 * into the argument registers. A direct call instead precomputes a costly
 * constant into a temporary that is then shared with later uses in the same
 * block. A value-returning call sets r0 last of its arguments.
 */
static __inline__ void Call1(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{

    return f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{

    f(a0, a1, a2, a3);
}

static __inline__ void bump_step(s32 amount)
{

    /* The scene step counter is at 0x1d8 of the shared scene work record. */
    u8 *work = (u8 *)gEventWork;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{

    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{

    f(a0, a1, a2);
}

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Engine_MapObjectSetPosition(actor, x, y);
}

static __inline__ void PlaceActor_02001104(s32 actor, s32 x, s32 y)
{
    Engine_MapObjectSetPosition(actor, x, y);
}

static __inline__ void bump_step_02000528(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void bump_step_02000568(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void Call1_02000644(void (*f)(), s32 a)
{

    f(a);
}

static __inline__ void Call3_02000644(void (*f)(), s32 a, s32 b, s32 c)
{

    f(a, b, c);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void bump_step_02000728(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void bump_step_02000980(s32 amount)
{

    u8 *work = (u8 *)gEventWork;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void SceneState_SetValues31_2_4(void);

s32 SceneActor_GetPositionDistance(s32 *a, s32 *b);

/*
 * Per-frame actor callback, whose argument is the owning actor record. The
 * initialiser stores this address plus the Thumb bit into field +0x6c of
 * actors 16 and 17. It returns a constant zero.
 *
 * The owner's eight-byte literal pool follows its return and belongs to it.
 * 0x03001e8c is a table of pointers whose entry 12 is 0x03001ebc, the overlay
 * work pointer the rest of this overlay loads directly.
 */
#include "TYPES.H"

struct SceneActor {
    u8 unk_00[6];
    u16 facing;
    s32 x, y, z;
    u8 unk_14[71];
    u8 active;
    u8 unk_5c[8];
    /*
     * Read by the partner-proximity callback: bit 0 chooses actor 17 rather
     * than 16 as the partner, and bit 1 is set while the party talks to the
     * actor.
     */
    u16 proximity_flags;
};

s32 SceneActor_GetPositionDistance(s32 *, s32 *);
u32 ArcTan2(s32, s32);

s32 SceneActor_UpdatePlayerProximity(struct SceneActor *actor, struct SceneActor *target, s32 range, s32 force);

s32 SceneActor_UpdatePartnerProximity(u8 *self);

u8 *TorebiMachi_GetEntrances(void);

s32 SceneData_ReturnZero(void);

u8 *TorebiMachi_GetExits(void);

u8 *TorebiMachi_GetPlacements(void);

void FieldScene_RunScene3b5_02000224(void);

void ConfigureAndPlaceActorOneHundredTwo(void);

void HideActorOneHundredTwo(void);

void SceneDialogue_RunMessage0e36(void);

void SceneDialogue_RunMessage0e37(void);

void FieldScene_RunSupplementalSequenceTwo(void);

void FieldScene_RunSiblingsTalk(void);

u8 *TorebiMachi_SelectEvents(void);

void SceneDialogue_RunActor15Message1f92(void);

void SceneDialogue_RunActor24Message1f9d(void);

void FieldScene_RunPatientTalk(void);

void FieldScene_RunScene3b5_02000568(void);

void SceneDialogue_RunActor27Message1fa3(void);

void SceneDialogue_RunActor24Message235f(void);

void FieldScene_RunScene3b5_020005dc(void);

void SceneScript_SetupActors(void);

void SceneActor_CopyPlayerModeToActor(union FieldObject *object);

s32 TorebiMachi_ApplyEntryState(s32 a0);

void FieldScene_RunScene3b5SequenceA(void);

void SceneState_SetValue30ThenCall(void);

void SceneState_PassWorkHalfword16C(void);

void FieldScene_RunPrimarySequence(void);

void FieldScene_ResetActor9AndDrawTiles(void);

#endif
