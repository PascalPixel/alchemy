#ifndef RUNPA_JO_FORTRESS_H
#define RUNPA_JO_FORTRESS_H

/* The Lunpa fortress (resource_3bf): the declarations, views and call
 * helpers its scene sources share. */

#include "TYPES.H"
#include "JO.H"
#include "FIELD_SCENE.H"
#include "ITEM_IDS.H"

#define SHARED_RECORD_FIELD_448 (*(u32 *)(*(u8 **)&gEventWork + 448))
#define PRIMARY_ID 24
#define DERIVED_ID 25

#include "STAGED_ACTOR.H"
#include "OBJECT_RUNTIME.H"

typedef struct SceneActor {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} SceneActor;

typedef struct DirectionalSceneActor {
    u8 pad0[6];
    u16 dir;        /* 0x06 */
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} DirectionalSceneActor;
extern u32 gRunpaJoRandomPick;
extern s32 gRunpaJoPairTableA[];
extern s32 gRunpaJoPairTableB[];
extern s32 gRunpaJoPairTableC[];

s32 TryStartActorInteraction(s32 actor_id, s32 interaction_id);

s32 IsActorInteractionAvailable(s32 actor_id);

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Actor_SetPosition(actor, x, y);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{

    f(a0, a1, a2);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{

    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void SetSceneValue(s16 *field, s32 value)
{

    *field = value;
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

s32 SelectPrimarySceneData(void);

s32 GetEmptySceneData(void);

s32 SelectSecondarySceneData(void);

s32 SelectTertiarySceneData(void);

s32 SelectQuaternarySceneData(void);

void ConfigureSceneActor12(void);

void RunSceneObjectSetup(void);

void FieldScene_StartActorTwelveTransition(void);

void FieldScene_UpdateActorTwelveTransition(void);

void PlaceActorTwelveAndFinishScene(void);

void PlaceSceneObjectPairFromTableA(s32 table_index);

void FieldScene_UpdateObjectPairA(void);

void FieldScene_UpdateObjectPairB(void);

void PlaceSceneObjectPairFromTableB(s32 table_index);

void FieldScene_UpdateTableBObjectPair(void);

void PlaceSceneObjectPairFromTableC(s32 table_index);

void FieldScene_UpdateObjectPairC(void);

void CellDoor_Touch(void);

void LockedDoor_Touch(void);

void Actor8_Interact(void);

void Actor9_Interact(void);

void Actor10_Interact(void);

void Actor11_Interact(void);

s32 TryStartActorInteraction(s32 actor_id, s32 interaction_id);

void NoOpSceneCallbackA(void);

void NoOpSceneCallbackB(void);

void NoOpSceneCallbackC(void);

void NoOpSceneCallbackD(void);

void CellKey_PickUp(void);

s32 IsPlayerInAccidentTriggerArea(void);

void FieldScene_UpdateActorPairInteraction(void);

void ConfigureSceneActor9(void);

s32 AreSceneActorsInPassingLane(void);

void FieldScene_UpdateActorSeventeenInteraction(void);

void ActivateSceneActor17(void);

s32 IsPlayerInSecondaryTriggerArea(void);

void FieldScene_UpdateActorEighteenInteraction(void);

void ActivateSceneActor18(void);

s32 IsPlayerOutsideSceneRectangle(void);

void FieldScene_RunScene3bfSequenceA(void);

void RunActor17SceneStep(void);

void TriggerSceneStage95FromActor12(void);

void FieldScene_RunScene3bfSequenceB(void);

void FieldScene_RunScene3bfSequenceC(void);

s32 IsSceneActorVerticallyNearPlayer(s32 actor_id);

s32 IsSceneActorHorizontallyNearPlayer(s32 actor_id);

s32 IsActorInteractionAvailable(s32 actor_id);

s32 IsSceneActorWithinFourSteps(s32 actor_id);

s32 IsSceneActorWithinTriggerBox(s32 actor_id);

void TriggerScene41AtVillagePath(void);

void TriggerScene40AtVillagePath(void);

void RunActor9ScriptedSequence(void);

void RunActorScriptedSequenceA(s32 actor_id);

void TurnActorToSceneDirection(s32 actor_id);

void RunActorScriptedSequenceB(s32 handle);

void RunActorScriptedSequenceC(s32 actor_id);

void FieldScene_RunScene3bf_02001cf0(s32 a0);

void RunActorScriptedSequenceD(s32 actor_id);

void InspectOrdinaryObject(void);

void InspectEmptyChest(void);

void FieldScene_RunSupplementalSequenceOne(void);

void FieldScene_RunSequenceTail(void);

void InspectEmptySceneObject(void);

void RunActor12InteractionSequence(void);

void RunActors13And21InteractionSequence(void);

void ConfigureInteractionRegionA(void);

void ConfigureInteractionRegionB(void);

void ConfigureInteractionRegionC(void);

void InspectVillageWell(void);

void RunSecondaryMapInteraction(void);

void ConfigurePrimaryInteractionRegions(void);

void ConfigureSecondaryInteractionRegions(void);

void InspectWardrobe(void);

void InspectFirewood(void);

void InspectBooks(void);

void NoOpInteractionCallback(void);

void FieldScene_RunScene3bf_0200252c(void);

void FieldScene_RunScene3bf_020025f8(void);

void FieldScene_RunScene3bf_0200269c(void);

void FieldScene_RunScene3bf_02002718(void);

void PlayStoryScene(void);

void FieldScene_RunMainScriptSequence(void);

void FieldScene_SelectActorTwentyOneMessage(void);

void FieldScene_RunActorTwentyOneSequence(void);

void FieldScene_RunDonpaSleepingSequence(void);

void SelectActor25SceneVariant(void);

void SelectActor24SceneVariant(void);

void FieldScene_RunSupplementalSequenceTwo(void);

void ConfigureSceneActor26(void);

void ConfigureSceneActor14(void);

void ConfigureSceneActor13(void);

void ConfigureSceneActor12Variant(void);

void ConfigureSceneActor18(void);

void RunActor20SceneSequence(void);

void FinishActor20SceneSequence(void);

void NoOpActorCallback(void);

void ConfigureActor13Interaction(void);

void ConfigureActor13SceneResource(void);

#include "OBJECT_RUNTIME.H"

struct DispatcherEventRuntime {
    u8 unknown_000[0x1c0];
    s32 value_1c0;
};

union DispatcherEventWork {
    struct {
        u8 unknown_000[0x1c0];
        u16 first;
        u16 second;
    } pair;
    u32 words[0x1c4 / 4];
};

static __inline__ void Scene_Call1(void (*func)(u32), u32 value)
{
    func(value);
}

static __inline__ void Scene_Call3(
    void (*func)(s32, s32, s32), s32 id, s32 x, s32 z)
{
    func(id, x, z);
}

s32 FieldScene_DispatchActorUpdate(void);

void FieldScene_InstallSceneTasks(void);

void FieldScene_SetupActorsForScene(void);

void FieldScene_RestoreActorsFromFlags(void);

void FieldScene_ActivateThreeActorGroup(void);

void FieldScene_ActivateTwoActorGroup(void);

void FieldScene_ActivateAlternateActorGroup(void);

void ActivateFiveActorGroupFromFlags(void);

/* SET_POSITION_PAIRS.C */
void FieldScene_SetPositionPairs(s32 idx);

#endif
