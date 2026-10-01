/* NONMATCHING: 2026-10-01 brief Wave2 direct-call adapter attempt.
 * Removing Actor_SetAttachedEffect from games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/JO.H changes:
 * games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/RUNPA_JO.C FieldScene_RunMainScriptSequence: push {r5, lr} => push {r5, r6, lr} (2171/2169 assembly lines).
 * Production source retains this helper with its measured reason.
 */
#ifndef RUNPA_JO_FORTRESS_H
#define RUNPA_JO_FORTRESS_H

/* The Lunpa fortress (resource_3bf): the declarations, views and call
 * helpers its scene sources share. */

#include "TYPES.H"
#ifndef RUNPA_JO_JO_H
#define RUNPA_JO_JO_H

/*
 * What the Lunpa fortress scene sees of the engine. The fortress links beside
 * the staged-actor code (FIELD/COMMON/OBJECT/STAGED_ACTOR.C), and several of
 * the overlay's import veneers carry that code's names, among them the sound
 * cue, which FIELD_EVENT.H spells as an inline service; so the fortress takes
 * the scene state and services it uses from this view instead, each service
 * reaching its veneer by the one name the overlay gives it.
 */

#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "FIELD_SCENE.H"
#include "SYSTEM.H"

#include "GAME_STATE.H"

/* Counts the frames the game has drawn. */
extern u32 gFrameCount;

/* The engine's event work. */
struct EventWork {
    u8 unknown_000[0x34];
    /* Actors placed by the scene, with ids 8 through 65. */
    struct FieldActor *placed_actors[58];
    u8 unknown_11c[0x50];
    /* The map trigger the leader has run into, handled on the next frame. */
    s16 touched_trigger;
    u8 unknown_16e[4];
    u16 unknown_172;
    u8 unknown_174[0xa];
    /* Psynergy the player has asked to use, handled on the next frame. */
    s16 psynergy_request;
    u8 unknown_180[2];
    /* A trigger a script raises for the events that answer it. */
    s16 raised_trigger;
    u8 unknown_184[0x3c];
    /* How the screen opens when the scene starts. */
    s32 start_transition;
    u8 unknown_1c4[4];
    /* The frames the screen takes to open or close. */
    s32 transition_frames;
    u8 unknown_1cc[0x0c];
    /* The message the next dialogue line shows. */
    u16 message;
    u8 unknown_1da[6];
    /* The actor at the centre of the area where actors are active. */
    struct FieldActor *view_center;
};

extern struct EventWork *gEventWork;

/* A scene transition is a style in the high byte and its variant in the low byte. */
enum SceneTransitionStyle {
    TRANSITION_FADE = 0,
    /* Fades in from the colour behind the background layers. */
    TRANSITION_BACKDROP_FADE = 1,
    TRANSITION_WINDOW = 2
};

#define SCENE_TRANSITION(style, variant) ((style) << 8 | (variant))

/* A 16.16 fixed-point map coordinate; its high half is the whole pixel. */
union FieldCoordinate {
    s32 fixed;
    struct {
        u16 fraction;
        s16 pixel;
    } part;
};

struct FieldSprite;
union FieldObject;

/* The runtime actor as event scripts see it. */
struct FieldActor {
    u8 unknown_00[6];
    u16 facing;
    union FieldCoordinate x;
    union FieldCoordinate y;
    union FieldCoordinate z;
    u8 unknown_14[4];
    /* The sprite's scale on each axis; 0x10000 draws it at full size. */
    s32 scale_x;
    s32 scale_y;
    /* How near another actor must come to touch this one. */
    u16 radius;
    u8 unknown_22;
    u8 priority_flags;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 speed;
    s32 acceleration;
    /* The fixed-point point the actor moves toward, or ACTOR_NO_TARGET. */
    s32 target_x;
    s32 target_y;
    s32 target_z;
    u8 unknown_44[0x0c];
    struct FieldSprite *sprite;
    /* Cleared while the actor is outside the area where actors are active. */
    u8 active;
    u8 motion_flags;
    u8 unknown_56[3];
    u8 collision_flags;
    u8 unknown_5a;
    u8 unknown_5b;
    u8 unknown_5c;
    u8 unknown_5d[5];
    /* Counts the frames of a rise while rise_enabled is set. */
    u8 rise_counter;
    u8 rise_enabled;
    u16 unknown_64;
    u16 unknown_66;
    u8 unknown_68[4];
    /* Called each frame while set. */
    void (*update)(union FieldObject *object);
};

/* An actor's priority flags. */
enum ActorPriorityFlag {
    /* Cleared when a script sets the sprite priority. */
    ACTOR_PRIORITY_AUTOMATIC = 0x01,
    /*
     * Set on objects the party passes over or stands on: effects, a glint
     * on the ground, and a pillar while the leader stands on top of it.
     */
    ACTOR_PRIORITY_UNDERFOOT = 0x02
};

/* The map work cell; the event work pointer follows it at +0x4c. */
extern u8 gCam[];

/* Import veneers named by the staged-actor code. */
void Battle_WaitMode0(s32 frames);
void *Object_GetById(s32 actor);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void Object_SetModeById(s32 actor, s32 animation);
void Map_CopyCellAttributeRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Audio_PlayCue(s32 cue);

/* The other import veneers the fortress calls. */
s32 PartyInventory_FindOwner(s32 item_id);
s32 BattleFx_PlayCueAndStartEmitterOnTarget(s32 value, s32 actor, s32 item);
void BattleFx_SetWeightedResult(s32 slot, s32 value);
void BattleFx_RunPageEffectForSlot(s32 slot, s32 first, s32 second);
void Party_SetFields1ceAnd1d0(s32 first, s32 second);
void Object_RefreshSelectorById(s32 actor);
void Map_SetWorkFlagBits9To11();
void Engine_ActorFaceActor(s32 actor, s32 target, s32 frames);
void Engine_ActorFaceDirection(s32 actor, s32 facing, s32 frames);
void Engine_ActorFaceEachOther(s32 actor, s32 other, s32 frames);
void Engine_ActorRunRepeatedMotion(s32 actor, s32 repeats);
void Engine_ActorSetPosition(s32 actor, s32 fixed_x, s32 fixed_z);
void Engine_ActorSetSpriteFlags(struct FieldActor *actor, s32 flags);
void Engine_ActorShowEmote(s32 actor, s32 emote, s32 frames);
void Engine_ActorWalkTo(s32 actor, s32 x, s32 z);
void Engine_CameraSetSpeed(s32 speed, s32 acceleration);
struct FieldActor *Engine_EventGetViewCenter();
void Engine_EventSetMessage(s32 message);
void Engine_EventShowMessage(s32 speaker, s32 flags);
s32 Engine_GameFlagIsSet(s32 flag);
void Engine_ObjectDispatchRelease(struct FieldActor *object);
s32 Engine_TaskAddCallback(void (*callback)(void), s32 priority);
s32 Engine_TaskRemoveCallback(void (*callback)(void));
void Engine_ActorEnableActionCallback(s32 actor, const u8 *table);
void Engine_ActorJump(s32 actor, s32 height, s32 frames);
void Engine_ActorSetAnimationAndWait(s32 actor, s32 animation);
void Engine_ActorSetAttachedEffect(s32 actor, s32 effect);
void Engine_ActorSetChildValue(s32 actor, s32 value);
void Engine_ActorSetDestination(s32 actor, s32 x, s32 z);
void Engine_ActorSetSpritePriority(s32 actor, s32 priority);
void Engine_ActorStartRepeatedMotion(s32 actor, s32 repeats);
void Engine_ActorStop(s32 actor);
void Engine_ActorWalkToAndWait(s32 actor, s32 x, s32 z);
void Engine_CameraFollowActor(s32 actor, s32 keep_position);
void Engine_CameraMoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z, s32 pan);
void Engine_CameraMoveToActor(s32 actor, s32 pan);
void Engine_CameraWaitForMove(void);
s32 Engine_EventAskYesNo(s32 speaker, s32 flags);
void Engine_EventBegin(void);
s32 Engine_EventChooseYesNo(s32 actor, s32 flags);
void Engine_EventCloseScreen(void);
void Engine_EventEnd(void);
s32 Engine_EventOpenMessage(s32 speaker, s32 flags);
void Engine_EventOpenScreen(void);
void Engine_EventRequestExit(s32 exit);
void Engine_GameFlagClear(s32 flag);
s32 Engine_GameFlagSet(s32 flag);
void Engine_ItemShowFound(s32 item, s32 height);
void Engine_MapCopyCells(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Engine_MapRedraw(void);
void Engine_MessageShowCentered(s32 message, s32 flags);
s32 Engine_PartyGiveItem(s32 item, s32 flags);
void Engine_PsynergyCancel(void);
s32 Engine_RandomNext(void);
void Engine_WorkSetValuesIfNonNegative(s32 first, s32 second, s32 third);

static inline void Actor_EnableActionCallback(s32 actor, const u8 *table)
{
    Engine_ActorEnableActionCallback(actor, table);
}

static inline void Actor_FaceActor(s32 actor, s32 target, s32 frames)
{
    Engine_ActorFaceActor(actor, target, frames);
}

static inline void Actor_FaceDirection(s32 actor, s32 facing, s32 frames)
{
    Engine_ActorFaceDirection(actor, facing, frames);
}

static inline void Actor_FaceEachOther(s32 actor, s32 other, s32 frames)
{
    Engine_ActorFaceEachOther(actor, other, frames);
}

static inline void Actor_Jump(s32 actor, s32 height, s32 frames)
{
    Engine_ActorJump(actor, height, frames);
}

static inline void Actor_RunRepeatedMotion(s32 actor, s32 repeats)
{
    Engine_ActorRunRepeatedMotion(actor, repeats);
}

static inline void Actor_SetAnimation(s32 actor, s32 animation)
{
    Object_SetModeById(actor, animation);
}

static inline void Actor_SetAnimationAndWait(s32 actor, s32 animation)
{
    Engine_ActorSetAnimationAndWait(actor, animation);
}





static inline void Actor_SetDestination(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers; see the retained direct-call draft. */
    Engine_ActorSetDestination(actor, x, z);
}

static inline void Actor_SetDestinationOffset(s32 actor, s32 dx, s32 dz)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers; see the retained direct-call draft. */
    ObjectMotion_OffsetPositionAndResetMotion(actor, dx, dz);
}

static inline void Actor_SetPosition(s32 actor, s32 fixed_x, s32 fixed_z)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers; see the retained direct-call draft. */
    Engine_ActorSetPosition(actor, fixed_x, fixed_z);
}

static inline void Actor_SetSpeed(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers; see the retained direct-call draft. */
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}





static inline void Actor_ShowEmote(s32 actor, s32 emote, s32 frames)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers; see the retained direct-call draft. */
    Engine_ActorShowEmote(actor, emote, frames);
}







static inline void Actor_WalkTo(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers; see the retained direct-call draft. */
    Engine_ActorWalkTo(actor, x, z);
}

static inline void Actor_WalkToAndWait(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers; see the retained direct-call draft. */
    Engine_ActorWalkToAndWait(actor, x, z);
}



static inline void Camera_MoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z, s32 pan)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers; see the retained direct-call draft. */
    Engine_CameraMoveTo(fixed_x, fixed_y, fixed_z, pan);
}



static inline void Camera_SetSpeed(s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers; see the retained direct-call draft. */
    Engine_CameraSetSpeed(speed, acceleration);
}



























static inline s32 GameFlag_IsSet(s32 flag)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers; see the retained direct-call draft. */
    return Engine_GameFlagIsSet(flag);
}

static inline s32 GameFlag_Set(s32 flag)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers; see the retained direct-call draft. */
    return Engine_GameFlagSet(flag);
}





static inline void Map_CopyCells(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x,
                                 s32 dest_y)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers; see the retained direct-call draft. */
    Engine_MapCopyCells(src_x, src_y, width, height, dest_x, dest_y);
}













static inline void Work_SetValuesIfNonNegative(s32 first, s32 second, s32 third)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers; see the retained direct-call draft. */
    Engine_WorkSetValuesIfNonNegative(first, second, third);
}

#endif

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



static __inline__ void SetSceneValue(s16 *field, s32 value)
{

    *field = value;
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

s32 GetEmptySceneData(void);

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

void RunpaJo_RunGuardChallenge(void);

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

#include "CALL.H"
#include "SCENE_IDS.H"

extern const struct SceneEntrance gRunpaJoEntrances1[];
extern const struct SceneEntrance gRunpaJoEntrances2[];
extern const struct SceneEntrance gRunpaJoEntrances3[];
extern const struct SceneEntrance gRunpaJoEntrancesOther[];

extern const u32 gRunpaJoExits2[];
extern const u32 gRunpaJoExits3And4[];
extern const u32 gRunpaJoExitsOther[];

extern const struct ScenePlacement gRunpaJoPlacementsRunpaDou[];
extern const struct ScenePlacement gRunpaJoPlacements1[];
extern const struct ScenePlacement gRunpaJoPlacements2[];
extern const struct ScenePlacement gRunpaJoPlacements3[];
extern const struct ScenePlacement gRunpaJoPlacements4[];
extern const struct ScenePlacement gRunpaJoPlacementsOther[];

extern const struct SceneEvent gRunpaJoEvents1[];
extern const struct SceneEvent gRunpaJoEvents2[];
extern const struct SceneEvent gRunpaJoEvents3[];
extern const struct SceneEvent gRunpaJoEventsOther[];

/* The support pairs, labelled where they lie among the overlay's data. */
extern s32 gRunpaJoSupportPairs[];

extern u8 MsgFieldDoorTightlyLocked[];

extern u8 MsgRunpaWho2[];

extern u8 MsgRunpaGuardWhosThat[];
extern u8 MsgRunpaIntruder[];
extern u8 MsgRunpaScoundrel[];
extern u8 MsgRunpaShiftAlready[];

extern u8 MsgRunpaWho3[];

extern u8 MsgFieldFlippedSwitch[];
extern u8 MsgRunpaPrepareBecomeMonster[];

extern u8 MsgRunpaBackMoreGuess[];
extern u8 MsgRunpaDodonpaPulledLever[];
extern u8 MsgRunpaTimeEat[];

extern u8 MsgRunpaAbleGet[];
extern u8 MsgRunpaDad[];
extern u8 MsgRunpaUhnnGetOff[];

extern u8 MsgRunpaDodonpasOrdersAbsolute[];
extern u8 MsgRunpaHammetGreatMerchant[];
extern u8 MsgRunpaLeftGuardHearsSomeone[];
extern u8 MsgRunpaSighBadCouldnt[];
extern u8 MsgRunpaStrangeSwearSomeone[];
extern u8 MsgRunpaTakeCareAnybody[];
extern u8 MsgRunpaToldStandGuard[];
extern u8 MsgRunpaWhoDisruptingSleep[];
extern u8 MsgRunpaZZZ[];

extern u8 MsgRunpaDifficultDonpaRight[];
extern u8 MsgRunpaDonpaGrateful[];
extern u8 MsgRunpaDonpaKnowsCoddled[];
extern u8 MsgRunpaFatherSorryDodonpa[];
extern u8 MsgRunpaFatherStayAngry[];
extern u8 MsgRunpaMaybeDodonpasEyes[];
extern u8 MsgRunpaOwwwDontHurt[];
extern u8 MsgRunpaShhhPleaseDont[];
extern u8 MsgRunpaSomeonePunishDodonpa[];
extern u8 MsgRunpaThankHelpDodonpa[];
extern u8 MsgRunpaWellWorriedDodonpa[];
extern u8 MsgRunpaZZZZ[];

extern u8 MsgRunpaGuysTougherThought[];
extern u8 MsgRunpaKnowWhereDodonpa[];
extern u8 MsgRunpaRightRightGive[];

extern u8 MsgRunpaDontLookNearly[];
extern u8 MsgRunpaMaybeMerchantReason[];
extern u8 MsgRunpaWhWhWho[];
extern u8 MsgRunpaWontTellAnyone[];

extern u8 MsgRunpaCantBelieveWhen[];
extern u8 MsgRunpaWrongTurnOver[];

/* Where the party appears on each floor of the fortress. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_RunpaJo1) {
        return gRunpaJoEntrances1;
    }
    if (scene == (s32)&SceneId_RunpaJo2) {
        return gRunpaJoEntrances2;
    }
    if (scene == (s32)&SceneId_RunpaJo3) {
        return gRunpaJoEntrances3;
    }
    return gRunpaJoEntrancesOther;
}

/* The Lunpa fortress: the scene table slot that holds nothing. */
s32 GetEmptySceneData(void) { return 0; }

/* Where the fortress's exits lead; the third and fourth rows share theirs. */
const u32 *Scene_GetExits(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_RunpaJo2) {
        return gRunpaJoExits2;
    }
    if (scene == (s32)&SceneId_RunpaJo3 || scene == (s32)&SceneId_RunpaJo4) {
        return gRunpaJoExits3And4;
    }
    return gRunpaJoExitsOther;
}

/* The actors placed on each floor of the fortress, and a table for the
 * Lunpa cave's row that this overlay never serves. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_RunpaDou) {
        return gRunpaJoPlacementsRunpaDou;
    }
    if (scene == (s32)&SceneId_RunpaJo3) {
        return gRunpaJoPlacements3;
    }
    if (scene == (s32)&SceneId_RunpaJo2) {
        return gRunpaJoPlacements2;
    }
    if (scene == (s32)&SceneId_RunpaJo1) {
        return gRunpaJoPlacements1;
    }
    if (scene == (s32)&SceneId_RunpaJo4) {
        return gRunpaJoPlacements4;
    }
    return gRunpaJoPlacementsOther;
}

/* What each floor of the fortress answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_RunpaJo1) {
        return gRunpaJoEvents1;
    }
    if (scene == (s32)&SceneId_RunpaJo2) {
        return gRunpaJoEvents2;
    }
    if (scene == (s32)&SceneId_RunpaJo3) {
        return gRunpaJoEvents3;
    }
    return gRunpaJoEventsOther;
}

/* The Lunpa fortress: actor 12's drop and the first bridge supports. */
void ConfigureSceneActor12(void)
{

    s32 actor_slot = 15;
    u8 *actor;

    Map_CopyCellAttributeRect(15, 20, 1, 1, actor_slot, 22);
    Map_CopyCellAttributeRect(17, 23, 1, 3, actor_slot, 23);
    actor = Object_GetById(12);
    if (actor != 0) {
        Engine_ActorSetSpriteFlags(actor, 0);
        actor[0x55] = 0;
        actor[0x23] = 2;
    }
}

void RunSceneObjectSetup(void)
{

    StagedActor_AdvancePair();
}

void FieldScene_StartActorTwelveTransition(void)
{

    Actor_SetSpeed(12, 0x10000, 0x8000);
    Actor_SetDestination(12, 248, 0x178);
    ObjectMotion_CommitCurrentPositionAndActivate(12);
    Audio_PlayCue(215);
    Battle_WaitMode0(60);
    ConfigureSceneActor12();
    GameFlag_Set(0x943);
}

void FieldScene_UpdateActorTwelveTransition(void)
{
    struct FieldActor *actor;

    actor = (struct FieldActor *)Object_GetById(12);
    if ((actor->z.fixed >> 20) > 22) {
        Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Audio_PlayCue(144);
        Map_CopyCellAttributeRect(15, 20, 1, 1, 15, 22);
        Map_CopyCellAttributeRect(17, 23, 1, 3, 15, 23);
        actor = (struct FieldActor *)Value1(Object_GetById, 12);
        if (actor != NULL) {
            Engine_ActorSetSpriteFlags(actor, 0);
            actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
        }
        GameFlag_Set(0x943);
    }
}

void PlaceActorTwelveAndFinishScene(void)
{
    Actor_SetPosition(12, 0x00f80000, 0x01780000);
    ConfigureSceneActor12();
}

void PlaceSceneObjectPairFromTableA(s32 table_index)
{

    s32 position_x = gRunpaJoPairTableA[table_index * 2];
    s32 position_z = gRunpaJoPairTableA[table_index * 2 + 1];

    Map_CopyCells(0, 0x4d, 1, 3, position_x, position_z);
    Map_CopyCells(1, 0x4d, 1, 1, position_x + 1, position_z);
    Map_CopyCells(position_x, position_z - 0x30, 1, 1, position_x, position_z - 0x2e);
}

void FieldScene_UpdateObjectPairA(void)
{

    struct EventWork *work;
    s32 trigger;
    s32 index;

    work = gEventWork;
    if (PartyInventory_FindOwner(234) != -1) {
        trigger = work->touched_trigger;
        index = trigger - 40;
        if (GameFlag_IsSet(0x941) == 0 || index != 4) {
            PlaceSceneObjectPairFromTableA(index);
            Audio_PlayCue(157);
            Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            GameFlag_Set(trigger + 0x328);
        }
    }
}

/*
 * The Lunpa Fortress bridge: pairs of position words for the movable
 * supports. Each indexed pair is a left and a right support that travel
 * together; the top pair stays level while the second is raised or lowered
 * in steps, and the lowest pair is only written once the pair index says it
 * is the bottom of the run.
 */
void FieldScene_SetPositionPairs(s32 idx)
{
    s32 top_x;
    s32 top_y;
    s32 bottom_y;

    top_x = gRunpaJoSupportPairs[idx * 2];
    top_y = gRunpaJoSupportPairs[idx * 2 + 1];
    Engine_MapCopyCells(0, 77, 1, 3, top_x, top_y);
    Engine_MapCopyCells(1, 77, 1, 1, top_x + 1, top_y);
    bottom_y = top_y - 44;
    Map_CopyCellAttributeRect(top_x, top_y - 45, 1, 1, top_x, bottom_y);
    if (idx == 1)
        Call6(Map_CopyCellAttributeRect, top_x, bottom_y, 1, 1, top_x, top_y - 43);
}

/* The Lunpa fortress: the other supports, the cell doors, the guards' items
 * and the cell key. */
void FieldScene_UpdateObjectPairB(void)
{

    struct EventWork *work;
    s32 trigger;

    work = gEventWork;
    if (PartyInventory_FindOwner(234) != -1) {
        trigger = work->touched_trigger;
        FieldScene_SetPositionPairs(trigger - 40);
        Audio_PlayCue(157);
        Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        GameFlag_Set(trigger + 0x32d);
    }
}

void PlaceSceneObjectPairFromTableB(s32 table_index)
{

    s32 position_x = gRunpaJoPairTableB[table_index * 2];
    s32 position_z = gRunpaJoPairTableB[table_index * 2 + 1];

    Map_CopyCells(0x37, 0x79, 1, 3, position_x, position_z);
    Map_CopyCells(0x38, 0x79, 1, 1, position_x + 1, position_z);
    Map_CopyCells(position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}

void FieldScene_UpdateTableBObjectPair(void)
{

    struct EventWork *work;
    s32 trigger;

    work = gEventWork;
    if (PartyInventory_FindOwner(234) != -1) {
        trigger = work->touched_trigger;
        PlaceSceneObjectPairFromTableB(trigger - 40);
        Audio_PlayCue(157);
        Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        GameFlag_Set(trigger + 0x330);
    }
}

void PlaceSceneObjectPairFromTableC(s32 table_index)
{

    s32 position_x = gRunpaJoPairTableC[table_index * 2];
    s32 position_z = gRunpaJoPairTableC[table_index * 2 + 1];

    Map_CopyCells(1, 0x50, 1, 3, position_x, position_z);
    Map_CopyCells(2, 0x50, 1, 1, position_x + 1, position_z);
    Map_CopyCellAttributeRect(position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}

void FieldScene_UpdateObjectPairC(void)
{

    struct EventWork *work;
    s32 trigger;

    work = gEventWork;
    if (PartyInventory_FindOwner(234) != -1) {
        trigger = work->touched_trigger;
        PlaceSceneObjectPairFromTableC(trigger - 40);
        Audio_PlayCue(157);
        Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        GameFlag_Set(trigger + 0x332);
    }
}

void CellDoor_Touch(void)
{
    if (PartyInventory_FindOwner(ITEM_CELL_KEY) == -1) {
        Engine_MessageShowCentered((s32)MsgFieldDoorTightlyLocked, 1);
    }
}

void LockedDoor_Touch(void)
{
    Engine_MessageShowCentered((s32)MsgFieldDoorTightlyLocked, 1);
}

void Actor8_Interact(void)
{
    if (TryStartActorInteraction(8, 8) != 0) {
        GameFlag_Set(0xf2a);
    }
}

void Actor9_Interact(void)
{
    if (TryStartActorInteraction(9, 7) != 0) {
        GameFlag_Set(0xf2b);
    }
}

void Actor10_Interact(void)
{
    if (TryStartActorInteraction(10, 6) != 0) {
        GameFlag_Set(0xf2c);
    }
}

void Actor11_Interact(void)
{
    if (TryStartActorInteraction(11, 5) != 0) {
        GameFlag_Set(0xf2d);
    }
}

s32 TryStartActorInteraction(s32 actor_id, s32 interaction_id)
{
    s32 started = 0;
    s32 interaction;

    Engine_EventBegin();
    interaction = BattleFx_PlayCueAndStartEmitterOnTarget(0, actor_id, interaction_id);
    if (Engine_PartyGiveItem(interaction_id, 0) != -1) {
        Actor_SetAnimation(actor_id, 2);
        started = 1;
    } else {
        Audio_PlayCue(0x7d);
        Actor_SetAnimation(actor_id, 5);
    }
    Engine_ObjectDispatchRelease(interaction);
    Engine_EventEnd();
    return started;
}

void NoOpSceneCallbackA(void)
{
}

void NoOpSceneCallbackB(void)
{
}

void NoOpSceneCallbackC(void)
{
}

void NoOpSceneCallbackD(void)
{
}

void CellKey_PickUp(void)
{

    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x108, 0x318);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_PARTY_LEADER);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Battle_WaitMode0(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_ItemShowFound(ITEM_CELL_KEY, 3);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_PartyGiveItem(ITEM_CELL_KEY, 0);
    GameFlag_Set(0xf2e);
    Actor_SetPosition(8, 0, 0);
}

s32 IsPlayerInAccidentTriggerArea(void)
{
    SceneActor *player = Object_GetById(0);
    s32 z = player->z;
    s32 x;
    s32 zz, xx;

    if (z < 0) {
        z += 0xfffff;
    }
    x = player->x;
    zz = z >> 20;
    if (x < 0) {
        x += 0xfffff;
    }
    xx = x >> 20;
    if ((u32)(zz - 5) <= 2 && xx <= 10) {
        return 1;
    }
    if ((u32)(xx - 8) <= 1 && zz > 22) {
        return 1;
    }
    return 0;
}

/* Lunpa fortress: each frame the two guards sway with the map, and unless
 * the alarm is already raised they watch for the party: a cloaked party
 * that walks within four steps of either guard is caught, and an uncloaked
 * one they can talk to sets the alarm. */
void FieldScene_UpdateActorPairInteraction(void)
{
    struct ObjectRuntime *actor = Object_GetById(9);
    struct ObjectRuntime *other = Object_GetById(10);
    s32 *work = (s32 *)(*(u8 **)gCam + 0x164);
    s16 *scene = *(s16 **)(gCam + 0x4c);

    if (gFrameCount & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (GameFlag_IsSet(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
        other->movement_state = 1;
    } else if (!GameFlag_IsSet(0x214)) {
        actor->movement_state = 0;
        other->movement_state = 0;
        if (!GameFlag_IsSet(0x214) && actor->movement_state == 0) {
            work[8] = 8912896.0 - actor->x;
        }
        if (!IsPlayerInAccidentTriggerArea()) {
            if (gGameState.cloaked != 0) {
                if (IsSceneActorWithinFourSteps(9) && gGameState.cloaked != 0) {
                    SetSceneValue(&scene[191], 0x2092);
                    return;
                }
                if (IsSceneActorWithinFourSteps(10) && gGameState.cloaked != 0) {
                    SetSceneValue(&scene[191], 0x2092);
                    return;
                }
            }
            if (gGameState.cloaked == 0) {
                if (IsActorInteractionAvailable(9)) {
                    GameFlag_Set(0x215);
                    GameFlag_Set(0x214);
                }
                if (IsActorInteractionAvailable(10)) {
                    GameFlag_Set(0x215);
                    GameFlag_Set(0x214);
                }
            }
            if (GameFlag_IsSet(0x214)) {
                SetSceneValue(&scene[193], 91);
            }
        }
    }
}

/* The Lunpa fortress: the patrolling guards and the village path triggers. */
void ConfigureSceneActor9(void)
{
    Engine_EventBegin();
    Engine_ActorStop(9);
    Actor_SetDestinationOffset(9, 0, 0);
    Actor_SetAnimation(9, 0);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Actor_ShowEmote(9, 256, 0);
    RunActorScriptedSequenceA(10);
    Engine_EventEnd();
}

s32 AreSceneActorsInPassingLane(void)
{
    SceneActor *player = Object_GetById(0);
    SceneActor *passing_actor = Object_GetById(17);
    s32 ox = player->x;
    s32 pz;
    s32 px;
    s32 oxx, pzz, pxx;

    if (ox < 0) {
        ox += 0xfffff;
    }
    oxx = ox >> 20;
    pz = passing_actor->z;
    if (pz < 0) {
        pz += 0xfffff;
    }
    px = passing_actor->x;
    pzz = pz >> 20;
    if (px < 0) {
        px += 0xfffff;
    }
    pxx = px >> 20;
    if (oxx == 52 && pxx == 57 && pzz > 34 && pzz <= 40) {
        return 1;
    }
    if (oxx == 57 && pxx == 52 && pzz > 34 && pzz <= 40) {
        return 1;
    }
    return 0;
}

void FieldScene_UpdateActorSeventeenInteraction(void)
{

    struct ObjectRuntime *actor = Object_GetById(17);
    s32 *work = (s32 *)(*(u8 **)gCam + 0x164);
    s16 *scene = *(s16 **)(gCam + 0x4c);

    Engine_EventGetViewCenter(actor);
    if (gFrameCount & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (GameFlag_IsSet(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
    } else if (!GameFlag_IsSet(0x214)) {
        actor->movement_state = 0;
        if (!GameFlag_IsSet(0x214) && actor->movement_state == 0) {
            work[8] = 0x3400000 - actor->x;
            work[9] = 0x2400000 - actor->z;
        }
        if (!AreSceneActorsInPassingLane()) {
            IsSceneActorWithinTriggerBox(17);
            if (IsSceneActorWithinFourSteps(17) && gGameState.cloaked != 0) {
                SetSceneValue(&scene[191], 0x2092);
                return;
            }
            if (gGameState.cloaked == 0) {
                if (IsActorInteractionAvailable(17)) {
                    GameFlag_Set(0x215);
                    GameFlag_Set(0x214);
                }
            }
            if (GameFlag_IsSet(0x214)) {
                SetSceneValue(&scene[193], 92);
            }
        }
    }
}

void ActivateSceneActor17(void)
{
    RunActorScriptedSequenceA(17);
    Engine_EventEnd();
}

s32 IsPlayerInSecondaryTriggerArea(void)
{
    SceneActor *player = Object_GetById(0);
    s32 zz = player->z / 0x100000;
    s32 xx = player->x / 0x100000;

    if ((u32)(xx - 41) <= 3 && zz > 25 && zz <= 28) {
        return 1;
    }
    if (xx == 41 && zz > 37 && zz <= 41) {
        return 1;
    }
    if ((u32)(xx - 54) <= 2 && zz > 30 && zz <= 40) {
        return 1;
    }
    return 0;
}

void FieldScene_UpdateActorEighteenInteraction(void)
{

    struct ObjectRuntime *actor = Object_GetById(18);
    s32 *work = (s32 *)(*(u8 **)gCam + 0x164);
    s16 *scene = *(s16 **)(gCam + 0x4c);

    if (gFrameCount & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (GameFlag_IsSet(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
    } else if (!GameFlag_IsSet(0x214)) {
        actor->movement_state = 0;
        if (!GameFlag_IsSet(0x214) && actor->movement_state == 0) {
            work[8] = 0x2f00000 - actor->x;
            work[9] = 0x1f00000 - actor->z;
        }
        if (!IsPlayerInSecondaryTriggerArea()) {
            if (IsSceneActorWithinFourSteps(18) && gGameState.cloaked != 0) {
                SetSceneValue(&scene[191], 0x2092);
                return;
            }
            if (gGameState.cloaked == 0) {
                if (IsActorInteractionAvailable(18)) {
                    GameFlag_Set(0x215);
                    GameFlag_Set(0x214);
                }
            }
            if (GameFlag_IsSet(0x214)) {
                SetSceneValue(&scene[193], 93);
            }
        }
    }
}

void ActivateSceneActor18(void)
{
    RunActorScriptedSequenceA(18);
    Engine_EventEnd();
}

s32 IsPlayerOutsideSceneRectangle(void)
{
    SceneActor *player = Object_GetById(0);
    s32 zz = player->z / 0x100000;
    s32 xx = player->x / 0x100000;

    if (xx > 45 && zz > 14 && xx <= 64 && zz <= 16) {
        return 0;
    }
    return 1;
}

void FieldScene_RunScene3bfSequenceA(void)
{

    struct EventWork *work;

    work = gEventWork;
    if (GameFlag_IsSet(0x214) == 0) {
        if (IsPlayerOutsideSceneRectangle() == 0) {
            if (gGameState.cloaked == 0) {
                if (IsActorInteractionAvailable(17) != 0) {
                    GameFlag_Set(0x215);
                    GameFlag_Set(0x214);
                }
            }
            if (GameFlag_IsSet(0x214) != 0) {
                work->raised_trigger = 94;
            }
        }
    }
}

void RunActor17SceneStep(void)
{
    RunActorScriptedSequenceA(17);
    Engine_EventEnd();
}

void TriggerSceneStage95FromActor12(void)
{

    u8 *scene_state = ((u8*)gEventWork);

    if (IsActorInteractionAvailable(12) != 0 && gGameState.cloaked == 0) {
        s16 *scene_stage;
        s32 next_stage;

        Engine_TaskRemoveCallback(TriggerSceneStage95FromActor12);
        scene_stage = (s16 *)(scene_state + 386);
        next_stage = 95;
        *scene_stage = next_stage;
    }
}

void FieldScene_RunScene3bfSequenceB(void)
{

    struct EventWork *work;

    work = gEventWork;
    if (GameFlag_IsSet(0x225) == 0) {
        if (IsActorInteractionAvailable(13) != 0) {
            if (gGameState.cloaked == 0) {
                GameFlag_Set(0x225);
                Engine_TaskRemoveCallback((s32)FieldScene_RunScene3bfSequenceB);
                Engine_TaskRemoveCallback((s32)FieldScene_RunScene3bfSequenceC);
                work->raised_trigger = 96;
            }
        }
    }
}

void FieldScene_RunScene3bfSequenceC(void)
{

    struct EventWork *work;

    work = gEventWork;
    if (GameFlag_IsSet(0x225) == 0) {
        if (IsActorInteractionAvailable(21) != 0) {
            if (gGameState.cloaked == 0) {
                GameFlag_Set(0x225);
                Engine_TaskRemoveCallback((s32)FieldScene_RunScene3bfSequenceC);
                Engine_TaskRemoveCallback((s32)FieldScene_RunScene3bfSequenceB);
                work->raised_trigger = 96;
            }
        }
    }
}

s32 IsSceneActorVerticallyNearPlayer(s32 actor_id)
{
    SceneActor *scene_actor = Object_GetById(actor_id);
    SceneActor *player = Object_GetById(0);
    s32 actor_z = scene_actor->z / 0x100000;
    s32 actor_x = scene_actor->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 z_distance = actor_z - player_z;

    if (z_distance >= -6 && z_distance <= 6 && actor_x - 1 < player_x && actor_x + 1 > player_x) {
        return 1;
    }
    return 0;
}

s32 IsSceneActorHorizontallyNearPlayer(s32 actor_id)
{
    SceneActor *scene_actor = Object_GetById(actor_id);
    SceneActor *player = Object_GetById(0);
    s32 actor_z = scene_actor->z / 0x100000;
    s32 actor_x = scene_actor->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 x_distance = actor_x - player_x;

    if (x_distance < -6 || x_distance > 6) {
        return 0;
    }
    if (actor_z - 2 < player_z && actor_z + 2 > player_z) {
        return 1;
    }
    return 0;
}

s32 IsActorInteractionAvailable(s32 actor_id)
{
    if (IsSceneActorWithinTriggerBox(actor_id) == 0) {
        return 0;
    }
    if (IsSceneActorVerticallyNearPlayer(actor_id)!= 0) {
        return 1;
    }
    {
        s32 result = IsSceneActorHorizontallyNearPlayer(actor_id);

        /* branchless "result != 0" */
        return (u32)(result | -result) >> 31;
    }
}

s32 IsSceneActorWithinFourSteps(s32 actor_id)
{
    SceneActor *scene_actor = Object_GetById(actor_id);
    SceneActor *player = Object_GetById(0);
    s32 actor_z = scene_actor->z / 0x100000;
    s32 actor_x = scene_actor->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 x_distance = actor_x - player_x;
    s32 z_distance;

    actor_z += 1;
    if (x_distance < 0) {
        x_distance = -x_distance;
    }
    z_distance = actor_z - player_z;
    if (z_distance < 0) {
        z_distance = -z_distance;
    }
    if (x_distance + z_distance <= 4) {
        return 1;
    }
    return 0;
}

s32 IsSceneActorWithinTriggerBox(s32 actor_id)
{
    SceneActor *scene_actor = Object_GetById(actor_id);
    SceneActor *player = Engine_EventGetViewCenter();
    s32 actor_x = scene_actor->x / 0x100000;
    s32 actor_z = scene_actor->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 x_distance = actor_x - player_x;
    s32 z_distance;

    if (x_distance < 0) {
        x_distance = -x_distance;
    }
    z_distance = actor_z - player_z;
    if (z_distance < 0) {
        z_distance = -z_distance;
    }
    if (x_distance > 7 || z_distance > 5) {
        return 0;
    }
    return 1;
}

void TriggerScene41AtVillagePath(void)
{

    SceneActor *player = Object_GetById(0);

    if (GameFlag_IsSet(859) == 0) {
        s32 player_x = player->x / 0x100000;
        s32 player_z = player->z / 0x100000;

        if (player_x == 43 && player_z > 28 && player_z <= 31) {
            s16 *q = (s16 *)(((u8*)gEventWork) + 364);
            s32 v = 41;

            *q = v;
            FieldScene_UpdateObjectPairC();
        }
    }
}

void TriggerScene40AtVillagePath(void)
{

    DirectionalSceneActor *player = Object_GetById(0);

    if (GameFlag_IsSet(856) == 0) {
        s32 player_x = player->x / 0x100000;
        s32 player_z = player->z / 0x100000;

        if (player_x == 16 && player_z > 55 && player_z <= 58
            && (player->dir == 0xc000 || player->dir == 0x4000)) {
            s16 *q = (s16 *)(((u8*)gEventWork) + 364);
            s32 v = 40;

            *q = v;
            FieldScene_UpdateTableBObjectPair();
        }
    }
}

/* The Lunpa fortress: a guard who catches the party asks who they are, and
 * the party is put out of the fortress. */
void RunActor9ScriptedSequence(void)
{
    Engine_EventBegin();
    Actor_SetDestinationOffset(9, 0, 0);
    Actor_EnableActionCallback(9, 1);
    Engine_ActorStop(9);
    Actor_SetAnimation(9, 0);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, 1);
    {
        s32 t = (s32)MsgRunpaWho2;

        Engine_EventSetMessage(t);
        Engine_EventShowMessage(9, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
        Engine_EventSetMessage(t + 1);
    }
    Engine_EventShowMessage(9, 0);
    Engine_EventRequestExit(60);
    Engine_EventCloseScreen();
    Engine_EventEnd();
}

void RunActorScriptedSequenceA(s32 actor_id)
{
    Engine_EventBegin();
    Engine_EventBegin();
    Actor_ShowEmote(actor_id, 256, 1);
    Actor_SetDestinationOffset(actor_id, 0, 0);
    Actor_EnableActionCallback(actor_id, 1);
    Actor_SetAnimation(actor_id, 0);
    Actor_FaceActor(actor_id, ACTOR_PARTY_LEADER, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_SetDestinationOffset(actor_id, 0, 0);
    Actor_EnableActionCallback(actor_id, 1);
    Engine_ActorStop(actor_id);
    Actor_SetAnimation(actor_id, 0);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, 1);
    {
        s32 t = (s32)MsgRunpaWho2;

        Engine_EventSetMessage(t);
        Engine_EventShowMessage(actor_id, 0);
        Actor_FaceActor(ACTOR_PARTY_LEADER, actor_id, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
        Engine_EventSetMessage(t + 1);
    }
    Engine_EventShowMessage(actor_id, 0);
    Engine_EventCloseScreen();
    Battle_WaitMode0(60);
    Engine_EventRequestExit(60);
    Engine_EventEnd();
}

/* The Lunpa fortress: a guard turns to the party and says one of his lines. */
void TurnActorToSceneDirection(s32 actor_id)
{

    Actor_FaceActor(actor_id, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, actor_id, 0);
    switch ((s32)gRunpaJoRandomPick & 3) {
    case 0:
        RunActorScriptedSequenceB(actor_id);
        break;
    case 1:
        RunActorScriptedSequenceC(actor_id);
        break;
    case 2:
        FieldScene_RunScene3bf_02001cf0(actor_id);
        break;
    case 3:
        RunActorScriptedSequenceD(actor_id);
        break;
    default:
        RunActorScriptedSequenceC(actor_id);
        break;
    }
}

/* The Lunpa fortress: the guards' lines and the first searchable objects. */
void RunActorScriptedSequenceB(s32 handle)
{
    s32 id;

    Actor_RunRepeatedMotion(handle, 1);
    id = (s32)MsgRunpaIntruder;
    Engine_EventSetMessage(id);
    Engine_EventShowMessage(handle, 0);
    Actor_ShowEmote(handle, 258, 60);
    Engine_EventSetMessage(id + 1);
    Engine_EventShowMessage(handle, 0);
    id += 2;
    Actor_SetAnimationAndWait(handle, 4);
    Engine_EventSetMessage(id);
    Engine_EventShowMessage(handle, 0);
}

void RunActorScriptedSequenceC(s32 actor_id)
{
    u8 *t = MsgRunpaGuardWhosThat;

    Engine_EventSetMessage((s32)t);
    Engine_EventShowMessage(actor_id, 0);
    Actor_RunRepeatedMotion(actor_id, 1);
    Engine_EventSetMessage((s32)(t + 1));
    Engine_EventShowMessage(actor_id, 0);
    Actor_SetAnimationAndWait(actor_id, 4);
    Engine_EventSetMessage((s32)(t + 2));
    Engine_EventShowMessage(actor_id, 0);
}

void FieldScene_RunScene3bf_02001cf0(s32 a0)
{
    u32 i;
    s32 record;
    s32 intruder;

    intruder = (s32)MsgRunpaShiftAlready;
    Engine_EventSetMessage(intruder);
    Engine_EventShowMessage(a0, 0);
    Battle_WaitMode0(120);
    Actor_ShowEmote(a0, 0x101, 60);
    Engine_EventSetMessage((intruder + 1));
    Engine_EventShowMessage(a0, 0);
    Actor_RunRepeatedMotion(a0, 1);
    Engine_EventSetMessage((intruder + 2));
    Engine_EventShowMessage(a0, 0);
    Actor_SetAnimationAndWait(a0, 4);
    Engine_EventSetMessage((intruder + 3));
    Engine_EventShowMessage(a0, 0);
}

void RunActorScriptedSequenceD(s32 actor_id)
{
    u8 *t = (s32)MsgRunpaScoundrel;

    Engine_EventSetMessage((s32)t);
    Engine_EventShowMessage(actor_id, 0);
    Actor_SetAnimationAndWait(actor_id, 4);
    Engine_EventSetMessage((s32)(t + 1));
    Engine_EventShowMessage(actor_id, 0);
    Actor_RunRepeatedMotion(actor_id, 1);
    Engine_EventSetMessage((s32)(t + 2));
    Engine_EventShowMessage(actor_id, 0);
    Actor_SetAnimationAndWait(actor_id, 3);
    Engine_EventSetMessage((s32)(t + 3));
    Engine_EventShowMessage(actor_id, 0);
}

void InspectOrdinaryObject(void)
{

    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Audio_PlayCue(113);
    Actor_ShowEmote(15, 256, 60);
    TurnActorToSceneDirection(15);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    gGameState.unknown_200[0x22b - 0x200] = 3;
    BattleFx_SetWeightedResult(98, 2);
    Actor_SetPosition(15, 0, 0);
    Engine_EventEnd();
    GameFlag_Set(2380);
}

void InspectEmptyChest(void)
{

    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Audio_PlayCue(113);
    Actor_ShowEmote(11, 256, 60);
    TurnActorToSceneDirection(11);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    gGameState.unknown_200[0x22b - 0x200] = 3;
    BattleFx_SetWeightedResult(98, 2);
    Actor_SetPosition(11, 0, 0);
    Engine_EventEnd();
    GameFlag_Set(2377);
}

/* Lunpa fortress: the guards challenge the party ("Who are you!?"),
 * talk it over and send the party back out to the fortress's second scene
 * at entrance 31. */
void RunpaJo_RunGuardChallenge(void)
{
    s32 line;

    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_SetAnimation(12, 1);
    Actor_SetAnimation(13, 1);
    Actor_SetAnimation(14, 1);
    Audio_PlayCue(113);
    Actor_ShowEmote(12, 0x100, 0);
    Battle_WaitMode0(30);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    line = (s32)MsgRunpaWho3;
    Engine_EventSetMessage(line);
    Engine_EventShowMessage(12, 0);
    Actor_ShowEmote(13, 0x100, 0);
    Actor_ShowEmote(14, 0x100, 0);
    Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Battle_WaitMode0(65);
    Actor_FaceDirection(13, 0x5000, 0);
    Actor_FaceDirection(14, 0xd000, 0);
    Engine_EventSetMessage((line + 1));
    Engine_EventShowMessage(13, 0);
    Actor_SetAnimationAndWait(14, 3);
    Engine_EventSetMessage((line + 2));
    Engine_EventShowMessage(14, 0);
    Engine_EventSetMessage((line + 3));
    Engine_EventShowMessage(12, 0);
    Actor_RunRepeatedMotion(13, 1);
    Engine_EventSetMessage((line + 4));
    Engine_EventShowMessage(13, 0);
    Actor_SetAnimationAndWait(14, 3);
    Engine_EventSetMessage((line + 5));
    Engine_EventShowMessage(14, 0);
    Actor_SetAnimationAndWait(14, 3);
    Battle_WaitMode0(60);
    Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(70);
    Actor_WalkTo(12, 0x2a0, 88);
    ObjectMotion_CommitCurrentPositionAndActivate(12);
#if !defined(TBS_EDITION_JA)
    /* The localized sequence adds this turn after actor 12 is placed. */
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
#endif
    Actor_SetAnimationAndWait(12, 3);
    Battle_WaitMode0(30);
    Engine_EventSetMessage((line + 6));
    Engine_EventShowMessage(12, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    Party_SetFields1ceAnd1d0((s32)&SceneId_RunpaJo2, 31);
    gGameState.unknown_200[0x22b - 0x200] = 3;
    BattleFx_SetWeightedResult(98, 3);
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(14, 0, 0);
    Engine_EventEnd();
    GameFlag_Set(0x94a);
}

/* The Lunpa fortress: the end of a sequence and an empty object. */
void FieldScene_RunSequenceTail(void)
{
    Engine_EventBegin();
    Actor_SetPosition(12, 45088768, 5767168); /* object_id 12, x, z */
    Actor_SetPosition(13, 46137344, 5767168); /* object_id 13, x, z */
    Actor_SetPosition(14, 47185920, 6291456); /* object_id 14, x, z */
    Actor_SetAnimation(12, 5); /* object_id 12, action 5 */
    Actor_SetAnimation(13, 5); /* object_id 13, action 5 */
    Actor_SetAnimation(14, 5); /* object_id 14, action 5 */
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Engine_EventEnd();
    Engine_EventOpenScreen(); /* main:0808a360 */
}

void InspectEmptySceneObject(void)
{

    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Audio_PlayCue(113);
    Actor_ShowEmote(16, 256, 60);
    TurnActorToSceneDirection(16);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    gGameState.unknown_200[0x22b - 0x200] = 3;
    BattleFx_SetWeightedResult(98, 2);
    Actor_SetPosition(16, 0, 0);
    Engine_EventEnd();
    GameFlag_Set(2379);
}

/* The Lunpa fortress: the other guards' challenges, which also put the party
 * out and set a flag. */
void RunActor12InteractionSequence(void)
{
    Engine_EventBegin();
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, 1);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    Audio_PlayCue(113);
    Actor_ShowEmote(12, 256, 60);
    {
        s32 t = (s32)MsgRunpaWho2;

        Engine_EventSetMessage(t);
        Engine_EventShowMessage(12, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 50);
        Engine_EventSetMessage(t + 1);
    }
    Engine_EventShowMessage(12, 0);
    Engine_EventCloseScreen();
    Battle_WaitMode0(60);
    Engine_EventRequestExit(60);
    Engine_EventEnd();
    GameFlag_Set(548);
}

void RunActors13And21InteractionSequence(void)
{
    u32 i;
    s32 record;
    s32 msg;

    Engine_EventBegin();
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, 1);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Audio_PlayCue(113);
    Actor_ShowEmote(21, 0x100, 0);
    Actor_ShowEmote(13, 0x100, 60);
    Actor_FaceActor(21, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
    msg = (s32)MsgRunpaWho2;
    Engine_EventSetMessage(msg);
    Engine_EventShowMessage(13, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 30);
    Engine_EventSetMessage(msg + 1);
    Engine_EventShowMessage(13, 0);
    Engine_EventCloseScreen();
    Battle_WaitMode0(60);
    Engine_EventRequestExit(60);
    Engine_EventEnd();
    GameFlag_Set(0x225);
}

/* The Lunpa fortress: the interaction regions and the searchable objects. */
void ConfigureInteractionRegionA(void)
{
    Map_CopyCells(2, 82, 1, 2, 21, 81);
    Map_CopyCellAttributeRect(21, 32, 1, 1, 21, 34);
}

void ConfigureInteractionRegionB(void)
{
    Map_CopyCells(2, 84, 1, 2, 6, 55);
    Map_CopyCellAttributeRect(5, 9, 1, 1, 6, 10);
}

void ConfigureInteractionRegionC(void)
{
    Map_CopyCells(2, 86, 1, 2, 27, 62);
    Map_CopyCellAttributeRect(26, 16, 1, 1, 27, 17);
}

void InspectVillageWell(void)
{

    if (*(s16 *)(((u8*)gEventWork) + 0xcb8) != 0) {
        if (GameFlag_IsSet(0x947) == 0) {
            Engine_MessageShowCentered((s32)MsgFieldFlippedSwitch, 1);
            Audio_PlayCue(188);
            Battle_WaitMode0(1);
            Map_CopyCells(6, 77, 1, 2, 17, 82);
            Battle_WaitMode0(5);
            Map_CopyCells(7, 77, 1, 2, 17, 82);
            Battle_WaitMode0(1);
            ConfigureInteractionRegionA();
            GameFlag_Set(0x947);
        }
    }
}

void RunSecondaryMapInteraction(void)
{

    if (*(s16 *)(((u8*)gEventWork) + 0xcb8) != 0) {
        if (GameFlag_IsSet(0x948) == 0) {
            Engine_MessageShowCentered((s32)MsgFieldFlippedSwitch, 1);
            Audio_PlayCue(188);
            Battle_WaitMode0(1);
            Map_CopyCells(6, 77, 1, 2, 3, 55);
            Battle_WaitMode0(5);
            Map_CopyCells(7, 77, 1, 2, 3, 55);
            Battle_WaitMode0(1);
            ConfigureInteractionRegionB();
            GameFlag_Set(0x948);
        }
    }
}

void ConfigurePrimaryInteractionRegions(void)
{
    Map_CopyCells(5, 77, 1, 2, 17, 82);
    Map_CopyCells(5, 77, 1, 2, 3, 55);
    Map_CopyCellAttributeRect(15, 33, 1, 1, 17, 35);
    Map_CopyCellAttributeRect(3, 8, 1, 1, 3, 10);
}

void ConfigureSecondaryInteractionRegions(void)
{
    Map_CopyCells(8, 77, 1, 2, 17, 82);
    Map_CopyCells(8, 77, 1, 2, 3, 55);
    Map_CopyCellAttributeRect(18, 35, 1, 1, 17, 35);
    Map_CopyCellAttributeRect(2, 10, 1, 1, 3, 10);
}

void InspectWardrobe(void)
{
    GameFlag_Set(2372);
    Engine_GameFlagClear(535);
    Actor_SetPosition(8, 0, 0);
}

void InspectFirewood(void)
{
    GameFlag_Set(2373);
    ConfigureInteractionRegionC();
    Actor_SetPosition(9, 0, 0);
}

void InspectBooks(void)
{
    GameFlag_Set(2374);
    Engine_GameFlagClear(536);
    Actor_SetPosition(10, 0, 0);
}

void NoOpInteractionCallback(void)
{
}

void FieldScene_RunScene3bf_0200252c(void)
{
    struct FieldActor *actor;

    actor = (struct FieldActor *)Object_GetById(0);
    if (actor != NULL) {
        Actor_SetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
    }
    actor = (struct FieldActor *)Object_GetById(0);
    if (actor != NULL) {
        Actor_SetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
    }
    actor = (struct FieldActor *)Object_GetById(0);
    if (actor != NULL) {
        Actor_SetPosition(ACTOR_GERALD, actor->x.fixed, actor->z.fixed);
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetSpeed(ACTOR_IVAN, 0xb333, 0x5999);
    Actor_WalkTo(ACTOR_IVAN, 0x1c8, 192);
    Actor_SetSpeed(ACTOR_MIA, 0xb333, 0x5999);
    Actor_WalkTo(ACTOR_MIA, 0x1b8, 184);
    Actor_SetSpeed(ACTOR_GERALD, 0xb333, 0x5999);
    Actor_WalkTo(ACTOR_GERALD, 0x1c0, 240);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_IVAN);
    Actor_FaceActor(ACTOR_IVAN, 12, 0);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_GERALD);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_MIA);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Actor_FaceActor(ACTOR_MIA, 12, 0);
    Battle_WaitMode0(15);
}

void FieldScene_RunScene3bf_020025f8(void)
{
    u32 i;
    s32 record;

    Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
    Audio_PlayCue(141);
    Battle_WaitMode0(80);
    Audio_PlayCue(0x120);
    Battle_WaitMode0(5);
    Audio_PlayCue(145);
    Map_CopyCells(16, 75, 7, 4, 26, 55);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 0);
    Engine_ActorShowEmote(12, 0x100, 0);
    Battle_WaitMode0(60);
}

void FieldScene_RunScene3bf_0200269c(void)
{
    u32 i;
    s32 record;

    Engine_CameraMoveToActor(11, 1);
    Engine_CameraWaitForMove();
    Battle_WaitMode0(60);
    Engine_EventSetMessage((s32)MsgRunpaPrepareBecomeMonster);
    Engine_EventShowMessage(13, 0);
    Actor_SetSpeed(11, 0x10000, 0x8000);
    Actor_SetSpeed(15, 0x10000, 0x8000);
    Actor_WalkTo(11, 0x1d8, 180);
    Actor_WalkTo(15, 0x1d8, 180);
    Engine_CameraFollowActor(11, 1);
    ObjectMotion_CommitCurrentPositionAndActivate(11);
    Actor_SetAnimation(11, 4);
    Battle_WaitMode0(30);
}

void FieldScene_RunScene3bf_02002718(void)
{
    u32 i;
    s32 record;

    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_IVAN, 0x1f8, 216);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_MIA, 0x1b8, 232);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_GERALD, 0x1e0, 224);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_MIA);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
}

/* Lunpa fortress: Dodonpa's story. Unless the story has already been told,
 * the party meets him and, on the first visit, hears it at length; either
 * way the scene ends by sending the party on to the fortress's fourth
 * scene. */

/* The dialogue lines count on from each sequence's first message. */
void PlayStoryScene(void)
{
    s32 text_line;

    if (GameFlag_IsSet(769) != 0)
        return;
    GameFlag_Set(624);
    Engine_EventBegin();
    if (GameFlag_IsSet(2370) != 0) {
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 32768, 16384);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 456, 216);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
        FieldScene_RunScene3bf_0200252c();
        Actor_ShowEmote(12, 256, 60);
        Actor_FaceDirection(12, 32768, 0);
        Actor_Jump(12, 4, 0);
        Engine_ActorSetSpriteFlags(Object_GetById(12), 1);
        Battle_WaitMode0(30);
        Actor_SetSpeed(ACTOR_IVAN, 45875, 22937);
        Engine_ActorWalkTo(2, 464, 192);
        ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_IVAN);
        Battle_WaitMode0(30);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 0);
        Actor_FaceDirection(ACTOR_IVAN, 16384, 0);
        Actor_FaceDirection(ACTOR_GERALD, 16384, 0);
        Actor_FaceDirection(ACTOR_MIA, 16384, 0);
        Actor_SetPosition(13, 29884416, 20971520);
        Engine_CameraSetSpeed(131072, 16384);
        text_line = (s32)MsgRunpaBackMoreGuess;
        Engine_EventSetMessage(text_line);
        Engine_EventShowMessage(13, 0);
        Engine_ActorWalkTo(13, 458, 272);
        ObjectMotion_CommitCurrentPositionAndActivate(13);
        Engine_ActorFaceDirection(13, 20480, 0);
        Battle_WaitMode0(40);
        Call3(ObjectMotion_OffsetPositionAndResetMotion, 13, -8, 8);
        ObjectMotion_CommitCurrentPositionAndActivate(13);
        Battle_WaitMode0(60);
        Audio_PlayCue(155);
        Engine_MessageShowCentered(text_line + 1, 1);
        ObjectMotion_OffsetPositionAndResetMotion(13, 8, -8);
        FieldScene_RunScene3bf_020025f8();
        Battle_WaitMode0(120);
        Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Engine_ActorStartRepeatedMotion(ACTOR_IVAN, 2);
        Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
        Engine_ActorStartRepeatedMotion(ACTOR_MIA, 2);
        Battle_WaitMode0(20);
        text_line += 2;
        FieldScene_RunScene3bf_02002718();
        Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
        Engine_EventSetMessage(text_line);
        Engine_EventShowMessage(13, 0);
        FieldScene_RunScene3bf_0200269c();
        SHARED_RECORD_FIELD_448 = 512;
        Battle_WaitMode0(1);
        /* FAKEMATCH: removing this one-pass block changes instruction scheduling; see its retained draft. */
        do {
            gGameState.unknown_200[0x22b - 0x200] = 3;
        } while (0);
        Party_SetFields1ceAnd1d0((s32)&SceneId_RunpaJo4, 4);
        BattleFx_SetWeightedResult(98, 4);
    } else {
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
        Audio_PlayCue(17);
        Battle_WaitMode0(30);
        text_line = (s32)MsgRunpaTimeEat;
        Engine_EventSetMessage(text_line);
        Engine_EventShowMessage(12, 0);
        Actor_FaceEachOther(ACTOR_PARTY_LEADER, 12, 0);
        Battle_WaitMode0(140);
        Actor_FaceDirection(12, 32768, 0);
        Actor_Jump(12, 4, 0);
        Engine_ActorSetSpriteFlags(Object_GetById(12), 1);
        Engine_EventSetMessage(text_line + 1);
        Engine_EventShowMessage(12, 0);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 32768, 16384);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 456, 216);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
        FieldScene_RunScene3bf_0200252c();
        Actor_SetSpeed(ACTOR_IVAN, 45875, 22937);
        Engine_ActorWalkTo(2, 464, 192);
        ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_IVAN);
        Battle_WaitMode0(30);
        Engine_EventSetMessage(text_line + 2);
        Engine_EventShowMessage(ACTOR_IVAN, 0);
        Engine_ActorShowEmote(12, 256, 0);
        Battle_WaitMode0(110);
        Audio_PlayCue(60);
        Engine_EventSetMessage(text_line + 3);
        Engine_EventShowMessage(12, 0);
        Battle_WaitMode0(30);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        Battle_WaitMode0(20);
        Actor_SetAnimation(ACTOR_IVAN, 1);
        Engine_ActorStartRepeatedMotion(12, 1);
        Battle_WaitMode0(20);
        Actor_SetSpeed(12, 26214, 13107);
        Engine_ActorWalkTo(12, 520, 208);
        ObjectMotion_CommitCurrentPositionAndActivate(12);
        Actor_SetAnimation(12, 1);
        Battle_WaitMode0(20);
        Engine_ActorFaceDirection(12, 45056, 0);
        Battle_WaitMode0(30);
        Call3(Engine_ActorFaceDirection, 12, 20480, 0);
        Battle_WaitMode0(30);
        Actor_FaceActor(12, ACTOR_IVAN, 0);
        Battle_WaitMode0(20);
        Engine_EventSetMessage(text_line + 4);
        Engine_EventShowMessage(12, 0);
        Battle_WaitMode0(40);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        Battle_WaitMode0(20);
        Call3(Engine_ActorShowEmote, 12, 264, 0);
        Battle_WaitMode0(120);
        Engine_EventSetMessage(text_line + 5);
        Engine_EventShowMessage(12, 0);
        Battle_WaitMode0(25);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        Battle_WaitMode0(30);
        Actor_SetAnimationAndWait(12, 3);
        Battle_WaitMode0(40);
        Engine_ActorWalkTo(2, 480, 200);
        ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_IVAN);
        Actor_FaceEachOther(ACTOR_IVAN, 12, 0);
        Battle_WaitMode0(60);
        Engine_EventSetMessage(text_line + 6);
        Engine_EventShowMessage(ACTOR_IVAN, 0);
        Battle_WaitMode0(20);
        Actor_SetAnimation(12, 4);
        Battle_WaitMode0(80);
        Engine_EventSetMessage(text_line + 7);
        Engine_EventShowMessage(12, 0);
        Engine_ActorSetPosition(13, 29884416, 20971520);
        Audio_PlayCue(19);
        Engine_EventSetMessage(text_line + 8);
        Engine_EventShowMessage(13, 0);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
        Actor_FaceActor(ACTOR_IVAN, 13, 0);
        Actor_FaceActor(ACTOR_GERALD, 13, 0);
        Battle_WaitMode0(5);
        Actor_FaceDirection(ACTOR_MIA, 16384, 0);
        Actor_FaceActor(12, 13, 0);
        Battle_WaitMode0(30);
        Audio_PlayCue(61);
        Camera_SetSpeed(131072, 16384);
        Engine_CameraMoveToActor(13, 1);
        Engine_CameraWaitForMove();
        Actor_SetSpeed(13, 52428, 26214);
        Actor_WalkTo(13, 456, 304);
        Engine_CameraFollowActor(13, 1);
        ObjectMotion_CommitCurrentPositionAndActivate(13);
        Engine_CameraFollowActor(ACTOR_GERALD, 1);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 0);
        Actor_ShowEmote(ACTOR_IVAN, 258, 0);
        Actor_ShowEmote(ACTOR_GERALD, 258, 0);
        Actor_ShowEmote(ACTOR_MIA, 258, 0);
        Call3(Engine_ActorShowEmote, 12, 258, 0);
        Battle_WaitMode0(60);
        Actor_FaceActor(12, 13, 0);
        Engine_ActorStartRepeatedMotion(12, 2);
        Battle_WaitMode0(60);
        Engine_EventSetMessage(text_line + 9);
        Engine_EventShowMessage(12, 0);
        Actor_FaceActor(13, 12, 0);
        Engine_EventSetMessage(text_line + 10);
        Engine_EventShowMessage(13, 0);
        Battle_WaitMode0(60);
        Actor_FaceActor(13, ACTOR_IVAN, 0);
        Battle_WaitMode0(30);
        Engine_EventSetMessage(text_line + 11);
        Engine_EventShowMessage(13, 0);
        Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
        Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
        Battle_WaitMode0(60);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
        Actor_FaceActor(ACTOR_IVAN, 13, 0);
        Actor_FaceActor(ACTOR_GERALD, 13, 0);
        Actor_FaceActor(ACTOR_MIA, 13, 0);
        Engine_ActorStartRepeatedMotion(13, 1);
        Battle_WaitMode0(60);
        Engine_EventSetMessage(text_line + 12);
        Engine_EventShowMessage(13, 0);
        Call3(Engine_ActorShowEmote, 1, 259, 0);
        Battle_WaitMode0(60);
        Actor_SetAnimation(13, 4);
        Engine_EventSetMessage(text_line + 13);
        Engine_EventShowMessage(13, 0);
        Call3(Engine_ActorWalkTo, 1, 456, 248);
        ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_GERALD);
        Call3(Engine_ActorFaceDirection, 1, 16384, 0);
        Engine_EventSetMessage(text_line + 14);
        Engine_EventShowMessage(ACTOR_GERALD, 0);
        Engine_ActorWalkTo(2, 472, 216);
        ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_IVAN);
        Engine_ActorFaceDirection(2, 16384, 0);
        Battle_WaitMode0(10);
        Engine_EventSetMessage(text_line + 15);
        Engine_EventShowMessage(ACTOR_IVAN, 0);
        Engine_ActorShowEmote(12, 261, 0);
        Battle_WaitMode0(60);
        Engine_EventSetMessage(text_line + 16);
        Engine_EventShowMessage(12, 0);
        Engine_ActorWalkTo(3, 440, 216);
        ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_MIA);
        Actor_FaceActor(ACTOR_MIA, 13, 0);
        Actor_SetAnimationAndWait(ACTOR_MIA, 3);
        Battle_WaitMode0(10);
        Engine_EventSetMessage(text_line + 17);
        Engine_EventShowMessage(ACTOR_MIA, 0);
        Actor_SetAnimation(13, 4);
        Engine_EventSetMessage(text_line + 18);
        Engine_EventShowMessage(13, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 0);
        Actor_ShowEmote(ACTOR_GERALD, 258, 0);
        Actor_ShowEmote(ACTOR_MIA, 258, 0);
        Actor_ShowEmote(ACTOR_IVAN, 258, 0);
        Engine_ActorShowEmote(13, 264, 0);
        Battle_WaitMode0(60);
        Engine_EventSetMessage(text_line + 19);
        Engine_EventShowMessage(13, 0);
        Battle_WaitMode0(20);
        Engine_ActorShowEmote(1, 259, 0);
        Battle_WaitMode0(60);
        Engine_EventSetMessage(text_line + 20);
        Engine_EventShowMessage(ACTOR_GERALD, 0);
        Engine_ActorStartRepeatedMotion(13, 1);
        Battle_WaitMode0(60);
        Engine_EventSetMessage(text_line + 21);
        Engine_EventShowMessage(13, 0);
        Engine_ActorWalkTo(13, 456, 280);
        ObjectMotion_CommitCurrentPositionAndActivate(13);
        Engine_ActorFaceDirection(13, 20480, 0);
        Battle_WaitMode0(80);
        Call3(ObjectMotion_OffsetPositionAndResetMotion, 13, -8, 8);
        ObjectMotion_CommitCurrentPositionAndActivate(13);
        Battle_WaitMode0(60);
        Audio_PlayCue(155);
        Engine_MessageShowCentered((s32)MsgRunpaDodonpaPulledLever, 1);
        Actor_SetDestinationOffset(13, 8, -8);
        Actor_FaceActor(13, 11, 0);
        FieldScene_RunScene3bf_020025f8();
        Audio_PlayCue(52);
        Engine_EventSetMessage(text_line + 23);
        Engine_EventShowMessage(13, 0);
        Battle_WaitMode0(60);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
        Actor_FaceActor(ACTOR_GERALD, 11, 0);
        Actor_FaceActor(ACTOR_IVAN, 11, 0);
        Actor_FaceActor(ACTOR_MIA, 11, 0);
        Actor_FaceActor(12, 11, 0);
        FieldScene_RunScene3bf_02002718();
        FieldScene_RunScene3bf_0200269c();
        GameFlag_Set(2370);
        SHARED_RECORD_FIELD_448 = 512;
        Battle_WaitMode0(1);
        /* FAKEMATCH: removing this one-pass block changes instruction scheduling; see its retained draft. */
        do {
            gGameState.unknown_200[0x22b - 0x200] = 3;
        } while (0);
        Party_SetFields1ceAnd1d0((s32)&SceneId_RunpaJo4, 4);
        BattleFx_SetWeightedResult(98, 4);
    }
    Engine_EventEnd();
}

/* The Lunpa fortress: Dodonpa freed and reunited with his father. */

/* Long fixed sequence of setup, positioning, and per-actor animation calls
 * against actor slots 0-3, 11-15, driven by three script line tables, with
 * two two-way branches on the outcome of a query call. Ends by writing the
 * scene phase word and issuing a final batch of calls. */
void FieldScene_RunMainScriptSequence(void)
{
    u32 i;
    u8 *record;
    s32 script_a;
    s32 script_b;
    s32 script_c;

    GameFlag_Set(0x301);
    GameFlag_Set(0x941);
    Map_CopyCells(16, 75, 7, 4, 26, 55);
    PlaceSceneObjectPairFromTableA(4);
    Engine_EventBegin();
    record = Object_GetById(12);
    Engine_ActorSetSpriteFlags(record, 1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x1c80000, 0xb80000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_SetPosition(ACTOR_GERALD, 0x1b80000, 0xc00000);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_SetPosition(ACTOR_MIA, 0x1e80000, 0xb80000);
    Actor_FaceDirection(ACTOR_MIA, 0x4000, 0);
    Actor_SetPosition(ACTOR_IVAN, 0x1d80000, 0xb80000);
    Actor_FaceDirection(ACTOR_IVAN, 0x4000, 0);
    Actor_SetPosition(12, 0x2080000, 0xe00000);
    Actor_FaceDirection(12, 0xb000, 0);
    Actor_SetPosition(11, 0x1c00000, 0xed0000);
    Actor_FaceDirection(11, 0x8000, 0);
    Actor_SetPosition(15, 0x1c00000, 0xee0000);
    Engine_ActorSetSpritePriority(15, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetPosition(13, 0x1ca0000, 0xf30000);
    Actor_FaceDirection(13, 0x4000, 0);
    Actor_SetAnimation(13, 5);
    Engine_EventOpenScreen();
    Battle_WaitMode0(120);
    /* Script line bases are overlay data symbols: an integer base would be
     * constant-propagated into every offset instead of staying in r5. */
    script_a = (s32)MsgRunpaUhnnGetOff;
    Engine_EventSetMessage(script_a);
    Engine_EventShowMessage(13, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Engine_EventSetMessage((script_a + 1));
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 1);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 1);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 1);
    Battle_WaitMode0(60);
    Actor_FaceActor(12, 13, 0);
    Battle_WaitMode0(60);
    Engine_EventSetMessage((script_a + 2));
    Engine_EventShowMessage(12, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Actor_FaceActor(ACTOR_IVAN, 12, 0);
    Actor_FaceActor(ACTOR_MIA, 12, 0);
    Battle_WaitMode0(60);
    Actor_WalkTo(12, 0x200, 232);
    Actor_SetAnimation(12, 4);
    Battle_WaitMode0(60);
    Engine_EventSetMessage((script_a + 3));
    Engine_EventShowMessage(12, 0);
    Battle_WaitMode0(15);
    Engine_ActorStartRepeatedMotion(13, 2);
    Engine_EventSetMessage((script_a + 4));
    Engine_EventShowMessage(13, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Actor_FaceActor(ACTOR_IVAN, 13, 0);
    Actor_FaceActor(ACTOR_MIA, 13, 0);
    Actor_FaceActor(ACTOR_GERALD, 13, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_IVAN, 1);
    Engine_EventSetMessage((script_a + 5));
    Engine_EventShowMessage(ACTOR_IVAN, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x105, 0);
    Actor_SetAnimation(ACTOR_MIA, 4);
    Battle_WaitMode0(80);
    Actor_SetAnimation(ACTOR_MIA, 1);
    Engine_EventSetMessage((script_a + 6));
    Engine_EventShowMessage(ACTOR_MIA, 0);
    Actor_ShowEmote(12, 0x102, 65);
    Engine_ActorStartRepeatedMotion(12, 2);
    Battle_WaitMode0(100);
    Engine_EventSetMessage((script_a + 7));
    Engine_EventShowMessage(13, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(10);
    Engine_EventSetMessage((script_a + 8));
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_IVAN, 1);
    Battle_WaitMode0(30);
    Engine_EventSetMessage((script_a + 9));
    Engine_EventShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
    Engine_EventSetMessage((script_a + 10));
    Engine_EventOpenMessage(ACTOR_MIA, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Actor_SetAnimation(ACTOR_IVAN, 3);
        Actor_SetAnimation(ACTOR_GERALD, 3);
        Actor_SetAnimation(ACTOR_MIA, 3);
        Actor_FaceActor(12, 13, 0);
        Battle_WaitMode0(60);
    } else {
        Engine_EventSetMessage((script_a + 11));
        Engine_EventShowMessage(ACTOR_MIA, 0);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
        Actor_FaceActor(ACTOR_GERALD, 12, 0);
        Actor_FaceActor(ACTOR_IVAN, 12, 0);
        Actor_FaceActor(ACTOR_MIA, 12, 0);
        Battle_WaitMode0(20);
        Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
        Battle_WaitMode0(60);
        Actor_FaceActor(12, 13, 0);
        Battle_WaitMode0(80);
        Actor_SetAnimationAndWait(12, 3);
        Battle_WaitMode0(30);
        Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
        Engine_EventSetMessage((script_a + 12));
        Engine_EventShowMessage(12, 0);
        Battle_WaitMode0(60);
        Actor_FaceActor(ACTOR_IVAN, 12, 0);
        Actor_FaceActor(ACTOR_GERALD, 12, 0);
        Actor_FaceActor(ACTOR_MIA, 12, 0);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
        Actor_SetAnimation(ACTOR_IVAN, 3);
        Actor_SetAnimation(ACTOR_GERALD, 3);
        Actor_SetAnimation(ACTOR_MIA, 3);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
        Battle_WaitMode0(80);
        Actor_FaceActor(12, 13, 0);
        Actor_FaceActor(ACTOR_IVAN, 13, 0);
        Actor_FaceActor(ACTOR_GERALD, 13, 0);
        Actor_FaceActor(ACTOR_MIA, 13, 0);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    }
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_MIA, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_IVAN, 0x8000, 0x4000);
    Actor_WalkTo(ACTOR_GERALD, 0x1a0, 216);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_GERALD);
    Actor_WalkTo(ACTOR_GERALD, 0x1a0, 248);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_GERALD);
    Actor_WalkTo(ACTOR_GERALD, 0x1b8, 248);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1b8, 216);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_PARTY_LEADER);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_WalkTo(ACTOR_MIA, 0x1e8, 248);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_MIA);
    Actor_WalkTo(ACTOR_MIA, 0x1c8, 248);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_MIA);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_WalkTo(ACTOR_IVAN, 0x1c8, 216);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 0);
    Actor_FaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
    Battle_WaitMode0(60);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Battle_WaitMode0(100);
    Audio_PlayCue(226);
    Actor_SetAnimation(13, 7);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1999, 0xccc);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, -24, 0);
    Actor_SetSpeed(ACTOR_GERALD, 0x1999, 0xccc);
    Actor_SetDestinationOffset(ACTOR_GERALD, -24, 0);
    Actor_SetSpeed(ACTOR_MIA, 0x1999, 0xccc);
    Actor_SetDestinationOffset(ACTOR_MIA, -24, 0);
    Actor_SetSpeed(ACTOR_IVAN, 0x1999, 0xccc);
    Actor_SetDestinationOffset(ACTOR_IVAN, -24, 0);
    Actor_SetSpeed(11, 0x1999, 0xccc);
    Actor_SetSpeed(15, 0x1999, 0xccc);
    Actor_SetDestinationOffset(11, -24, 0);
    Actor_SetDestinationOffset(15, -24, 0);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_PARTY_LEADER);
    Audio_PlayCue(0x120);
    Battle_WaitMode0(60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x4000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Battle_WaitMode0(60);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Battle_WaitMode0(100);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xb333, 0x5999);
    Actor_SetSpeed(ACTOR_GERALD, 0xb333, 0x5999);
    Actor_SetSpeed(ACTOR_IVAN, 0xb333, 0x5999);
    Actor_SetSpeed(ACTOR_MIA, 0xb333, 0x5999);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1c8, 184);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_WalkTo(ACTOR_GERALD, 0x1d0, 0x100);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_GERALD);
    Actor_WalkTo(ACTOR_GERALD, 0x1e0, 248);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_GERALD);
    Actor_WalkTo(ACTOR_GERALD, 0x1b8, 192);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_GERALD);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_WalkTo(ACTOR_MIA, 0x1e8, 248);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_MIA);
    Actor_WalkTo(ACTOR_MIA, 0x1e8, 184);
    Actor_WalkTo(ACTOR_IVAN, 0x1d8, 184);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_IVAN);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_IVAN, 0x4000, 0);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_MIA);
    Actor_SetAnimation(ACTOR_MIA, 1);
    Actor_FaceDirection(ACTOR_MIA, 0x4000, 0);
    Battle_WaitMode0(30);
    Battle_WaitMode0(60);
    script_b = (s32)MsgRunpaAbleGet;
    Engine_EventSetMessage(script_b);
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Actor_SetAnimation(13, 6);
    Battle_WaitMode0(120);
    Engine_ActorStartRepeatedMotion(13, 2);
    Battle_WaitMode0(60);
    Actor_SetAnimation(13, 7);
    Engine_EventSetMessage((script_b + 1));
    Engine_EventShowMessage(13, 0);
    Battle_WaitMode0(20);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
    Battle_WaitMode0(10);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 80);
    Actor_FaceActor(ACTOR_MIA, 13, 0);
    Actor_FaceActor(ACTOR_IVAN, 13, 0);
    Engine_EventSetMessage((script_b + 2));
    Engine_EventShowMessage(ACTOR_MIA, 0);
    Actor_ShowEmote(13, 0x102, 70);
    Engine_ActorStartRepeatedMotion(13, 2);
    Battle_WaitMode0(60);
    Actor_SetAnimation(13, 5);
    Battle_WaitMode0(70);
    Engine_EventSetMessage((script_b + 3));
    Engine_EventShowMessage(13, 0);
    Actor_SetAnimation(13, 7);
    Actor_ShowEmote(ACTOR_IVAN, 0x108, 40);
    Engine_EventSetMessage((script_b + 4));
    Engine_EventShowMessage(ACTOR_IVAN, 0);
    Actor_SetAnimationAndWait(12, 3);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 60);
    Actor_WalkTo(ACTOR_GERALD, 0x1b8, 208);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_GERALD);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Engine_EventSetMessage((script_b + 5));
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Battle_WaitMode0(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 0);
    Actor_ShowEmote(12, 0x101, 0);
    Battle_WaitMode0(70);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 75);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_SetAnimation(ACTOR_GERALD, 4);
    Engine_EventSetMessage((script_b + 6));
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Actor_ShowEmote(12, 0x101, 0);
    Battle_WaitMode0(60);
    Engine_EventSetMessage((script_b + 7));
    Engine_EventShowMessage(12, 0);
    Actor_WalkTo(ACTOR_GERALD, 0x1c0, 208);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_GERALD);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Engine_EventSetMessage((script_b + 8));
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Engine_ActorStartRepeatedMotion(12, 1);
    Battle_WaitMode0(60);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Battle_WaitMode0(60);
    Engine_EventSetMessage((script_b + 9));
    Engine_EventShowMessage(12, 0);
    Actor_SetAnimation(ACTOR_GERALD, 4);
    Battle_WaitMode0(60);
    Engine_EventSetMessage((script_b + 10));
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_IVAN, 1);
    Battle_WaitMode0(30);
    Engine_EventSetMessage((script_b + 11));
    Engine_EventShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_MIA, 0);
    Battle_WaitMode0(20);
    Engine_EventSetMessage((script_b + 12));
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Battle_WaitMode0(30);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 80);
    Actor_FaceActor(ACTOR_MIA, ACTOR_GERALD, 0);
    Engine_EventSetMessage((script_b + 13));
    Engine_EventShowMessage(ACTOR_MIA, 0);
    Actor_FaceActor(ACTOR_MIA, 13, 0);
    Engine_EventSetMessage((script_b + 14));
    Engine_EventShowMessage(ACTOR_MIA, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Actor_FaceActor(ACTOR_IVAN, 13, 0);
    Actor_FaceActor(ACTOR_GERALD, 13, 0);
    Actor_FaceActor(ACTOR_MIA, 13, 0);
    Battle_WaitMode0(120);
    Actor_ShowEmote(13, 0x102, 30);
    Engine_ActorStartRepeatedMotion(13, 1);
    Battle_WaitMode0(120);
    Engine_ActorStartRepeatedMotion(12, 1);
    Battle_WaitMode0(60);
    Engine_EventSetMessage((script_b + 15));
    Engine_EventShowMessage(12, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x107, 110);
    Actor_WalkTo(ACTOR_GERALD, 0x1c8, 212);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_GERALD);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Engine_EventSetMessage((script_b + 16));
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_GERALD, 0);
    Battle_WaitMode0(60);
    Engine_ActorStartRepeatedMotion(ACTOR_IVAN, 1);
    Engine_EventSetMessage((script_b + 17));
    Engine_EventShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, 13, 0);
    Battle_WaitMode0(80);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Battle_WaitMode0(60);
    Actor_FaceActor(ACTOR_GERALD, 13, 0);
    Battle_WaitMode0(30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Actor_FaceActor(ACTOR_MIA, 13, 0);
    Actor_FaceActor(13, 13, 0);
    Actor_FaceActor(12, 13, 0);
    Battle_WaitMode0(80);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Battle_WaitMode0(30);
    Engine_EventSetMessage((script_b + 18));
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Engine_ActorStartRepeatedMotion(13, 2);
    Battle_WaitMode0(70);
    Engine_EventSetMessage((script_b + 19));
    Engine_EventShowMessage(13, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 60);
    Engine_EventSetMessage((script_b + 20));
    Engine_EventShowMessage(ACTOR_MIA, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(80);
    Engine_EventSetMessage((script_b + 21));
    Engine_EventOpenMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    Actor_SetPosition(14, 0x1c80000, 0x1300000);
    Actor_SetSpeed(14, 0x8000, 0x4000);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Audio_PlayCue(19);
        Engine_EventSetMessage((script_b + 22));
        Engine_EventShowMessage(14, 0);
    } else {
        Audio_PlayCue(19);
        Engine_EventSetMessage((script_b + 23));
        Engine_EventShowMessage(14, 0);
    }
    Actor_FaceActor(ACTOR_PARTY_LEADER, 14, 0);
    Actor_FaceActor(ACTOR_GERALD, 14, 0);
    Actor_FaceActor(ACTOR_MIA, 14, 0);
    Actor_FaceActor(ACTOR_IVAN, 14, 0);
    Actor_FaceActor(12, 14, 0);
    Actor_FaceActor(13, 14, 0);
    Camera_MoveTo(0x1c80000, -1, 0xf00000, 1);
    Engine_ActorStartRepeatedMotion(13, 1);
    Battle_WaitMode0(60);
    Audio_PlayCue(8);
    script_c = (s32)MsgRunpaDad;
    Engine_EventSetMessage(script_c);
    Engine_EventShowMessage(13, 0);
    Actor_WalkTo(14, 0x1c8, 0x118);
    ObjectMotion_CommitCurrentPositionAndActivate(14);
    Actor_WalkTo(14, 0x1b8, 0x100);
    Camera_MoveTo(0x1c80000, -1, 0xe00000, 1);
    Engine_EventSetMessage((script_c + 1));
    Engine_EventShowMessage(14, 0);
    ObjectMotion_CommitCurrentPositionAndActivate(14);
    Actor_FaceDirection(14, 0xd000, 0);
    Actor_ShowEmote(13, 0x102, 80);
    Actor_SetAnimation(14, 4);
    Battle_WaitMode0(89);
    Engine_EventSetMessage((script_c + 2));
    Engine_EventShowMessage(14, 0);
    Actor_RunRepeatedMotion(13, 2);
    Actor_SetAnimationAndWait(14, 3);
    Engine_EventSetMessage((script_c + 3));
    Engine_EventShowMessage(14, 0);
    Actor_ShowEmote(13, 0x100, 80);
    Actor_SetAnimation(14, 4);
    Battle_WaitMode0(80);
    Actor_FaceDirection(14, 0x3000, 0);
    Battle_WaitMode0(20);
    Engine_EventSetMessage((script_c + 4));
    Engine_EventShowMessage(14, 0);
    Engine_ActorStartRepeatedMotion(13, 1);
    Engine_EventSetMessage((script_c + 5));
    Engine_EventShowMessage(13, 0);
    Battle_WaitMode0(30);
    Actor_SetAnimationAndWait(14, 3);
    Actor_FaceActor(14, 13, 0);
    Battle_WaitMode0(20);
    Engine_EventSetMessage((script_c + 6));
    Engine_EventShowMessage(14, 0);
    Engine_ActorStartRepeatedMotion(13, 1);
    Actor_ShowEmote(13, 0x102, 80);
    Engine_EventSetMessage((script_c + 7));
    Engine_EventShowMessage(13, 0);
    Actor_ShowEmote(14, 0x103, 60);
    Engine_EventSetMessage((script_c + 8));
    Engine_EventShowMessage(14, 0);
    Actor_SetAnimationAndWait(14, 3);
    Engine_EventSetMessage((script_c + 9));
    Engine_EventShowMessage(14, 0);
    Battle_WaitMode0(20);
    Engine_ActorSetAttachedEffect(13, 0x101);
    Battle_WaitMode0(80);
    Engine_ActorSetAttachedEffect(13, 0);
    Actor_WalkTo(14, 0x1f0, 240);
    ObjectMotion_CommitCurrentPositionAndActivate(14);
    Actor_FaceActor(14, 12, 0);
    Battle_WaitMode0(20);
    Actor_FaceActor(12, 14, 0);
    Engine_EventSetMessage((script_c + 10));
    Engine_EventShowMessage(14, 0);
    Engine_ActorStartRepeatedMotion(12, 1);
    Engine_EventSetMessage((script_c + 11));
    Engine_EventShowMessage(12, 0);
    Battle_WaitMode0(40);
    Actor_SetAnimationAndWait(14, 3);
    Battle_WaitMode0(20);
    Engine_EventSetMessage((script_c + 12));
    Engine_EventShowMessage(14, 0);
    Actor_SetAnimation(14, 3);
    Actor_ShowEmote(12, 0x102, 60);
    Engine_EventSetMessage((script_c + 13));
    Engine_EventShowMessage(12, 0);
    Actor_ShowEmote(14, 0x100, 70);
    Engine_EventSetMessage((script_c + 14));
    Engine_EventShowMessage(14, 0);
    Actor_SetAnimation(12, 3);
    Battle_WaitMode0(140);
    Actor_SetAnimation(14, 3);
    Battle_WaitMode0(120);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 14, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Battle_WaitMode0(120);
    Actor_ShowEmote(14, 0x108, 180);
    Engine_EventSetMessage((script_c + 15));
    Engine_EventShowMessage(14, 0);
    Actor_ShowEmote(12, 0x101, 80);
    Engine_EventSetMessage((script_c + 16));
    Engine_EventShowMessage(12, 0);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
    Battle_WaitMode0(80);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(60);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 14, 0);
    Actor_FaceActor(ACTOR_GERALD, 14, 0);
    Actor_FaceActor(ACTOR_IVAN, 14, 0);
    Actor_FaceActor(ACTOR_MIA, 14, 0);
    Battle_WaitMode0(60);
    Engine_EventSetMessage((script_c + 17));
    Engine_EventShowMessage(14, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 0);
    Battle_WaitMode0(100);
    Engine_EventSetMessage((script_c + 18));
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_IVAN, 1);
    Battle_WaitMode0(60);
    Engine_EventSetMessage((script_c + 19));
    Engine_EventShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(14, ACTOR_IVAN, 0);
    Battle_WaitMode0(20);
    Engine_EventSetMessage((script_c + 20));
    Engine_EventShowMessage(14, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 0);
    Battle_WaitMode0(120);
    Actor_SetAnimation(14, 4);
    Battle_WaitMode0(120);
    Engine_EventSetMessage((script_c + 21));
    Engine_EventShowMessage(14, 0);
    Actor_ShowEmote(14, 0x102, 90);
    Engine_EventSetMessage((script_c + 22));
    Engine_EventShowMessage(14, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(12, 3);
    Battle_WaitMode0(80);
    Actor_SetAnimation(14, 4);
    Battle_WaitMode0(120);
    Engine_EventSetMessage((script_c + 23));
    Engine_EventShowMessage(14, 0);
    Actor_ShowEmote(12, 0x100, 60);
    Engine_EventSetMessage((script_c + 24));
    Engine_EventShowMessage(12, 0);
    Actor_FaceActor(14, 12, 0);
    Battle_WaitMode0(20);
    Engine_ActorStartRepeatedMotion(14, 1);
    Battle_WaitMode0(50);
    Engine_EventSetMessage((script_c + 25));
    Engine_EventShowMessage(14, 0);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Battle_WaitMode0(70);
    Engine_EventSetMessage((script_c + 26));
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Engine_EventSetMessage((script_c + 27));
    Engine_EventShowMessage(ACTOR_MIA, 0);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Actor_SetAnimationAndWait(14, 3);
    Battle_WaitMode0(20);
    Engine_EventSetMessage((script_c + 28));
    Engine_EventShowMessage(14, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 90);
    Engine_EventSetMessage((script_c + 29));
    Engine_EventShowMessage(ACTOR_IVAN, 0);
    Actor_FaceDirection(14, 0x5000, 0);
    Battle_WaitMode0(20);
    Engine_EventSetMessage((script_c + 30));
    Engine_EventShowMessage(14, 0);
    Engine_ActorStartRepeatedMotion(12, 1);
    Battle_WaitMode0(60);
    Engine_EventSetMessage((script_c + 31));
    Engine_EventShowMessage(12, 0);
    Actor_FaceActor(14, 12, 0);
    Battle_WaitMode0(20);
    Actor_SetAnimationAndWait(14, 3);
    Battle_WaitMode0(20);
    Engine_EventSetMessage((script_c + 32));
    Engine_EventShowMessage(14, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 70);
    Engine_EventSetMessage((script_c + 33));
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(30);
    Engine_EventSetMessage((script_c + 34));
    Engine_EventShowMessage(14, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 80);
    Engine_EventSetMessage((script_c + 35));
    Engine_EventShowMessage(ACTOR_MIA, 0);
    Actor_SetAnimationAndWait(14, 3);
    Engine_EventSetMessage((script_c + 36));
    Engine_EventShowMessage(14, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(30);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Battle_WaitMode0(100);
    Engine_EventSetMessage((script_c + 37));
    Engine_EventOpenMessage(ACTOR_IVAN, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        Engine_EventSetMessage((script_c + 38));
        Engine_EventShowMessage(14, 0);
    }
    Battle_WaitMode0(30);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Battle_WaitMode0(100);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 12, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(12, 3);
    Battle_WaitMode0(100);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetAnimation(ACTOR_IVAN, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_IVAN, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_SetAnimation(ACTOR_MIA, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_MIA, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Battle_WaitMode0(30);
    Actor_SetSpeed(12, 0x6666, 0x3333);
    Actor_WalkTo(12, 0x1d8, 184);
    ObjectMotion_CommitCurrentPositionAndActivate(12);
    Actor_SetAnimation(12, 1);
    Actor_FaceDirection(12, 0x3000, 0);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Actor_SetAnimation(14, 3);
    Battle_WaitMode0(20);
    Actor_FaceActor(14, 13, 0);
    Battle_WaitMode0(20);
    Engine_EventSetMessage((script_c + 39));
    Engine_EventShowMessage(14, 0);
    Actor_RunRepeatedMotion(13, 2);
    Engine_EventSetMessage((script_c + 40));
    Engine_EventShowMessage(13, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Battle_WaitMode0(30);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_SetSpeed(12, 0x10000, 0x8000);
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1e0, 248);
    Battle_WaitMode0(40);
    Engine_ActorSetSpritePriority(12, 0);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 0);
    Actor_WalkTo(12, 0x1e0, 216);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_PARTY_LEADER);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 14, 0);
    ObjectMotion_CommitCurrentPositionAndActivate(12);
    Actor_FaceActor(12, 14, 0);
    Actor_FaceDirection(12, 0x3000, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Battle_WaitMode0(5);
    Actor_SetAnimation(12, 3);
    Battle_WaitMode0(100);
    Actor_SetAnimation(14, 3);
    Battle_WaitMode0(100);
    Actor_WalkTo(12, 0x1e0, 248);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1c8, 248);
    ObjectMotion_CommitCurrentPositionAndActivate(12);
    Actor_WalkTo(12, 0x1c8, 248);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1c8, 0x168);
    ObjectMotion_CommitCurrentPositionAndActivate(12);
    Actor_WalkTo(12, 0x1c8, 0x168);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x160, 0x168);
    ObjectMotion_CommitCurrentPositionAndActivate(12);
    Actor_WalkTo(12, 0x160, 0x168);
    Battle_WaitMode0(20);
    Audio_PlayCue(17);
    gEventWork->start_transition = 0x203;
    Engine_EventCloseScreen();
    Battle_WaitMode0(1);
    Battle_WaitMode0(210);
    Engine_EventRequestExit(4);
    Engine_EventEnd();
}

/* The Lunpa fortress: actor 21's line by the day's draw, and the guard who
 * thinks he heard someone. */
void FieldScene_SelectActorTwentyOneMessage(void)
{

    switch (gRunpaJoRandomPick) {
    case 0:
        Engine_EventSetMessage((s32)MsgRunpaHammetGreatMerchant);
        Engine_EventShowMessage(21, 0);
        break;
    case 1:
        Engine_EventSetMessage((s32)MsgRunpaZZZ);
        Engine_EventShowMessage(21, 0);
        break;
    case 2:
        Engine_EventSetMessage((s32)MsgRunpaDodonpasOrdersAbsolute);
        Engine_EventShowMessage(21, 0);
        break;
    case 3:
        Engine_EventSetMessage((s32)MsgRunpaSighBadCouldnt);
        Engine_EventShowMessage(21, 0);
        break;
    case 4:
        Engine_EventSetMessage((s32)MsgRunpaStrangeSwearSomeone);
        Engine_EventShowMessage(21, 0);
        break;
    case 6:
        Engine_EventSetMessage((s32)MsgRunpaToldStandGuard);
        Engine_EventShowMessage(21, 0);
        break;
    case 7:
        Engine_EventSetMessage((s32)MsgRunpaWhoDisruptingSleep);
        Engine_EventShowMessage(21, 0);
        break;
    case 5:
        Actor_FaceDirection(21, 0xd000, 0);
        Battle_WaitMode0(50);
        Actor_FaceDirection(21, 0xb000, 0);
        Battle_WaitMode0(50);
        Actor_FaceDirection(21, 0x5000, 0);
        Battle_WaitMode0(50);
        Engine_EventSetMessage((s32)MsgRunpaTakeCareAnybody);
        Engine_EventShowMessage(21, 0);
        break;
    }
}

void FieldScene_RunActorTwentyOneSequence(void)
{

    s32 msg;

    Actor_ShowEmote(21, 0x101, 30);
    Actor_FaceDirection(21, 0xd000, 0);
    Battle_WaitMode0(50);
    Actor_FaceDirection(21, 0xb000, 0);
    Battle_WaitMode0(50);
    Actor_FaceDirection(21, 0x5000, 0);
    Battle_WaitMode0(50);
    msg = (s32)MsgRunpaLeftGuardHearsSomeone;
    Engine_EventSetMessage(msg);
    Engine_EventShowMessage(21, 0);
    Actor_SetAnimation(21, 4);
    Battle_WaitMode0(60);
    Actor_FaceDirection(21, 0xb000, 0);
    Battle_WaitMode0(40);
    Engine_EventSetMessage(msg + 1);
    Engine_EventShowMessage(21, 0);
}

/* The Lunpa fortress: actor 25 asking the party not to wake Donpa, the scene
 * variants of actors 24 and 25 and the second supplemental sequence. */
void FieldScene_RunDonpaSleepingSequence(void)
{
    u32 i;
    s32 record;
    s32 msg;
    s32 msg2;

    Engine_EventBegin();
    if (GameFlag_IsSet(0x941) != 0) {
        Engine_EventSetMessage((s32)MsgRunpaDonpaGrateful);
        Engine_EventShowMessage(18, 0);
        Engine_EventEnd();
    } else {
        if (GameFlag_IsSet(0x313) != 0) {
            Engine_EventSetMessage((s32)MsgRunpaMaybeDodonpasEyes);
            Engine_EventOpenMessage(25, 0);
            Engine_EventEnd();
        } else {
            Actor_ShowEmote(25, 0x102, 30);
            Actor_FaceActor(25, ACTOR_PARTY_LEADER, 0);
            msg = (s32)MsgRunpaShhhPleaseDont;
            Engine_EventSetMessage(msg);
            Engine_EventShowMessage(25, 0);
            Actor_FaceActor(25, 24, 0);
            Engine_CameraMoveToActor(24, 1);
            Engine_CameraWaitForMove();
            Battle_WaitMode0(60);
            Engine_CameraMoveToActor(0, 1);
            Battle_WaitMode0(20);
            Actor_ShowEmote(25, 0x105, 60);
            Engine_EventSetMessage(msg + 1);
            Engine_EventShowMessage(25, 0);
            Actor_ShowEmote(25, 0x107, 60);
            Engine_EventSetMessage(msg + 2);
            Engine_EventShowMessage(25, 0);
            Battle_WaitMode0(70);
            Actor_ShowEmote(25, 0x100, 60);
            Actor_FaceActor(25, ACTOR_PARTY_LEADER, 0);
            Engine_EventSetMessage(msg + 3);
            Engine_EventOpenMessage(25, 0);
            if (Engine_EventChooseYesNo(0, 0) == 0) {
                Engine_EventSetMessage(msg + 4);
                Engine_EventOpenMessage(25, 0);
            } else {
                Engine_EventSetMessage(msg + 5);
                Engine_EventOpenMessage(25, 0);
            }
            Battle_WaitMode0(60);
            Actor_ShowEmote(25, 0x105, 60);
            msg2 = (s32)MsgRunpaDonpaKnowsCoddled;
            Engine_EventSetMessage(msg2);
            Engine_EventOpenMessage(25, 0);
            Actor_RunRepeatedMotion(25, 1);
            Engine_EventSetMessage(msg2 + 1);
            Engine_EventOpenMessage(25, 0);
            Actor_SetAnimationAndWait(25, 3);
            Engine_EventSetMessage(msg2 + 2);
            Engine_EventOpenMessage(25, 0);
            GameFlag_Set(0x313);
            Engine_EventEnd();
        }
    }
}

void SelectActor25SceneVariant(void)
{
    if (GameFlag_IsSet(0x941)) {
        Engine_EventSetMessage((s32)MsgRunpaDifficultDonpaRight);
        Engine_EventShowMessage(25, 0);
    } else {
        Engine_EventSetMessage((s32)MsgRunpaSomeonePunishDodonpa);
        Engine_EventShowMessage(25, 0);
    }
}

void SelectActor24SceneVariant(void)
{
    if (GameFlag_IsSet(0x941)) {
        Engine_EventSetMessage((s32)MsgRunpaFatherStayAngry);
        Engine_EventShowMessage(24, 0);
    } else {
        Engine_EventSetMessage((s32)MsgRunpaFatherSorryDodonpa);
        Engine_EventShowMessage(24, 0);
    }
}

/* Runs a gated sequence of parameterized calls on PRIMARY_ID (24) and, once
 * derived partway through, DERIVED_ID (25); the thanks for helping Dodonpa
 * and the three lines after it are shown one apart. Each of the two outer
 * gating checks has its own short fallback branch on PRIMARY_ID. */
void FieldScene_RunSupplementalSequenceTwo(void)
{
    s32 sequence_id;

    if (GameFlag_IsSet(2369) != 0) {
        if (GameFlag_IsSet(2382) == 0 && GameFlag_IsSet(788) == 0) {
        sequence_id = (s32)MsgRunpaThankHelpDodonpa;
        Engine_EventSetMessage(sequence_id);
        Engine_EventShowMessage(PRIMARY_ID, 0);
        Engine_ActorRunRepeatedMotion(PRIMARY_ID, 1);
        Battle_WaitMode0(30);
        Actor_SetSpeed(PRIMARY_ID, 6553, 3276);
        ObjectMotion_OffsetPositionAndResetMotion(PRIMARY_ID, -4, 0);
        ObjectMotion_CommitCurrentPositionAndActivate(PRIMARY_ID);
        Object_SetModeById(PRIMARY_ID, 3);
        Battle_WaitMode0(60);
        Actor_SetSpeed(PRIMARY_ID, 13107, 6553);
        Actor_SetDestinationOffset(PRIMARY_ID, -6, 0);
        Engine_ActorFaceActor(PRIMARY_ID, 0, 0);
        ObjectMotion_CommitCurrentPositionAndActivate(PRIMARY_ID);
        Engine_EventSetMessage(sequence_id + 1);
        Engine_EventShowMessage(PRIMARY_ID, 0);
        Actor_RunRepeatedMotion(PRIMARY_ID, 1);
        Engine_ActorFaceActor(DERIVED_ID, PRIMARY_ID, 0);
        Engine_EventSetMessage(sequence_id + 2);
        Engine_EventShowMessage(PRIMARY_ID, 0);
        Battle_WaitMode0(70);
        Object_SetModeById(DERIVED_ID, 3);
        Battle_WaitMode0(60);
        Actor_SetSpeed(DERIVED_ID, 26214, 13107);
        Engine_ActorWalkTo(DERIVED_ID, 880, 112);
        ObjectMotion_CommitCurrentPositionAndActivate(DERIVED_ID);
        Engine_ActorFaceDirection(DERIVED_ID, 53248, 0);
        Engine_EventSetMessage(sequence_id + 3);
        Engine_EventShowMessage(PRIMARY_ID, 0);
        Object_SetModeById(PRIMARY_ID, 3);
        Battle_WaitMode0(70);
        ObjectMotion_OffsetPositionAndResetMotion(PRIMARY_ID, 8, 0);
        ObjectMotion_CommitCurrentPositionAndActivate(PRIMARY_ID);
        Object_SetModeById(PRIMARY_ID, 5);
        Engine_EventSetMessage(sequence_id + 4);
        Engine_EventShowMessage(PRIMARY_ID, 0);
        Engine_ActorWalkTo(0, 896, 120);
        ObjectMotion_CommitCurrentPositionAndActivate(0);
        Engine_ActorFaceEachOther(0, DERIVED_ID, 0);
        Battle_WaitMode0(60);
        Actor_SetAnimation(DERIVED_ID, 3);
        Battle_WaitMode0(30);
        GameFlag_Set(788);
        } else {
            Engine_EventSetMessage((s32)MsgRunpaWellWorriedDodonpa);
            Engine_EventShowMessage(PRIMARY_ID, 0);
        }
    } else {
        Engine_EventSetMessage((s32)MsgRunpaZZZZ);
        Engine_EventShowMessage(PRIMARY_ID, 0);
    }
}

void ConfigureSceneActor26(void)
{
    BattleFx_RunPageEffectForSlot(26, 1, 5);
    GameFlag_Set(0x94e);
}

void ConfigureSceneActor14(void)
{
    Actor_RunRepeatedMotion(14, 2);
    Engine_EventSetMessage((s32)MsgRunpaOwwwDontHurt);
    Engine_EventShowMessage(14, 0);
}

/* The Lunpa fortress: three actors' lines. */
void ConfigureSceneActor13(void)
{
    Actor_RunRepeatedMotion(13, 2);
    Engine_EventSetMessage((s32)MsgRunpaRightRightGive);
    Engine_EventShowMessage(13, 0);
}

void ConfigureSceneActor12Variant(void)
{
    Actor_RunRepeatedMotion(12, 2);
    Engine_EventSetMessage((s32)MsgRunpaGuysTougherThought);
    Engine_EventShowMessage(12, 0);
}

void ConfigureSceneActor18(void)
{
    Engine_EventSetMessage((s32)MsgRunpaKnowWhereDodonpa);
    Engine_EventAskYesNo(18, 0);
}

/* The Lunpa fortress: actor 20's sequence and its end. */
void RunActor20SceneSequence(void)
{
    u32 i;
    s32 record;
    s32 msg;
    s32 msg2;

    if (GameFlag_IsSet(0x226) != 0) {
        Engine_EventSetMessage((s32)MsgRunpaWontTellAnyone);
        Engine_EventShowMessage(20, 0);
    } else {
        Engine_EventBegin();
        Actor_FaceActor(20, ACTOR_PARTY_LEADER, 0);
        if (GameFlag_IsSet(0x227) == 0) {
            Actor_Jump(20, 4, 0);
            Engine_ActorStop(20);
            Object_RefreshSelectorById(20);
            Battle_WaitMode0(20);
            msg = (s32)MsgRunpaWhWhWho;
            Engine_EventSetMessage(msg);
            Engine_EventShowMessage(20, 0);
            Actor_ShowEmote(20, 0x102, 30);
            Engine_EventSetMessage(msg + 1);
            Engine_EventShowMessage(20, 0);
            Battle_WaitMode0(30);
            Actor_SetAnimation(20, 4);
            Battle_WaitMode0(30);
        }
        msg2 = (s32)MsgRunpaDontLookNearly;
        Engine_EventSetMessage(msg2);
        Engine_EventShowMessage(20, 0);
        Actor_ShowEmote(20, 0x101, 40);
        Engine_EventSetMessage(msg2 + 1);
        Engine_EventOpenMessage(20, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventSetMessage(msg2 + 2);
            Engine_EventOpenMessage(20, 0);
            GameFlag_Set(0x226);
        } else {
            Engine_EventSetMessage(msg2 + 3);
            Engine_EventOpenMessage(20, 0);
        }
        GameFlag_Set(0x227);
        Engine_EventEnd();
    }
}

void FinishActor20SceneSequence(void)
{

    if (GameFlag_IsSet(0x226)) {
        Engine_EventSetMessage((s32)MsgRunpaMaybeMerchantReason);
        Engine_EventShowMessage(20, 0);
    } else {
        s16 *q = (s16 *)(((u8*)gEventWork) + 382);

        *q = 0;
        Engine_PsynergyCancel();
        RunActor20SceneSequence();
    }
}

void NoOpActorCallback(void)
{
}

/* The Lunpa fortress: actor 13's lines. */
void ConfigureActor13Interaction(void)
{
    s32 msg = (s32)MsgRunpaWrongTurnOver;

    Engine_EventSetMessage(msg);
    Engine_EventShowMessage(0x800d, 0);
    if (PartyInventory_FindOwner(234) != -1) {
        Engine_MessageShowCentered(msg + 2, 1);
    }
}

void ConfigureActor13SceneResource(void)
{
    Engine_EventSetMessage((s32)MsgRunpaCantBelieveWhen);
    Engine_EventShowMessage(13, 0);
}

/* The fortress scene start: pick the random guard, then set up each floor.
 * The second installs its tasks and actors, the third restores what the
 * party changed, and the fourth stages its actors by the entrance the party
 * came through. */
s32 FieldScene_DispatchActorUpdate(void)
{
    struct ObjectRuntime *actor;

    gRunpaJoRandomPick = (u32)Engine_RandomNext() * 7 >> 16;
    if ((s16)(*(union DispatcherEventWork *)&gGameState).pair.first == (s32)&SceneId_RunpaJo1) {
        Map_SetWorkFlagBits9To11(0xe00);
        FieldScene_InstallSceneTasks();
    }
    if ((s16)(*(union DispatcherEventWork *)&gGameState).pair.first == (s32)&SceneId_RunpaJo2)
        FieldScene_SetupActorsForScene();
    if ((s16)(*(union DispatcherEventWork *)&gGameState).pair.first == (s32)&SceneId_RunpaJo3)
        FieldScene_RestoreActorsFromFlags();
    if ((s16)(*(union DispatcherEventWork *)&gGameState).pair.first == (s32)&SceneId_RunpaJo4) {
        ((struct DispatcherEventRuntime*)gEventWork)->value_1c0 = 0x204;
        Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
        Engine_ActorFaceDirection(12, 0, 0);
        Object_SetModeById(12, 0);
        ActivateFiveActorGroupFromFlags();
        actor = Object_GetById(8);
        if (actor != 0)
            Engine_ActorSetSpriteFlags(actor, 0);
        actor->unknown_23 = 2;
        actor = Object_GetById(9);
        if (actor != 0)
            Engine_ActorSetSpriteFlags(actor, 0);
        actor->unknown_23 = 2;
        actor = Object_GetById(10);
        if (actor != 0)
            Engine_ActorSetSpriteFlags(actor, 0);
        actor->unknown_23 = 2;
        Map_SetWorkFlagBits9To11(0xe00);
        if ((s16)(*(union DispatcherEventWork *)&gGameState).pair.second == 4) {
            Map_SetWorkFlagBits9To11(0xc00);
            FieldScene_RunMainScriptSequence();
        }
        if ((s16)(*(union DispatcherEventWork *)&gGameState).pair.second == 3) {
            Map_SetWorkFlagBits9To11(0xc00);
            if (Engine_GameFlagIsSet(0x941) != 0) {
                Engine_ActorSetPosition(12, 0, 0);
                Call3(Engine_ActorSetPosition, 16, 0x1b00000, 0x1580000);
                Call3(Engine_ActorFaceDirection, 16, 0x5000, 0);
                Call3(Engine_ActorSetPosition, 13, 0x1c80000, 0x1200000);
                Call3(Engine_ActorFaceDirection, 13, 0x5000, 0);
                Call3(Engine_ActorSetPosition, 17, 0x1c80000, 0x1400000);
                Engine_ActorSetSpriteFlags(Object_GetById(17), 0);
            }
        }
        actor = Object_GetById(15);
        if (actor != 0)
            Engine_ActorSetSpriteFlags(actor, 0);
        actor->unknown_23 = 2;
        *(s32 *)&actor->unknown_18[0] = 0xcccc;
    }
    return 0;
}

/* The Lunpa fortress: the scene tasks and the actors restored from the story
 * flags. */
void FieldScene_InstallSceneTasks(void)
{
    FieldScene_ActivateThreeActorGroup();
    switch (gGameState.entrance) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Value2(Engine_TaskAddCallback, (s32)TriggerSceneStage95FromActor12, 3200);
        Value2(Engine_TaskAddCallback, (s32)FieldScene_RunScene3bfSequenceB, 3200);
        Engine_TaskAddCallback((s32)FieldScene_RunScene3bfSequenceC, 3200);
        Map_SetWorkFlagBits9To11(0xe00);
        break;
    case 12:
    case 19:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
        Map_SetWorkFlagBits9To11(0xc00);
        break;
    case 16:
    case 17:
    case 18:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Value2(Engine_TaskAddCallback, (s32)FieldScene_UpdateActorEighteenInteraction, 3200);
        Engine_TaskAddCallback((s32)TriggerScene41AtVillagePath, 3200);
        WaitFrames(1);
        Engine_MapRedraw();
        WaitFrames(1);
        Map_CopyCells(101, 9, 10, 8, 110, 9);
        Map_SetWorkFlagBits9To11(0xe00);
        break;
    case 13:
    case 14:
    case 15:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Engine_TaskAddCallback((s32)FieldScene_RunScene3bfSequenceA, 3200);
        break;
    default:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Map_SetWorkFlagBits9To11(0xe00);
        break;
    }
    Engine_ActorSetChildValue(18, 1);
    Engine_ActorSetChildValue(17, 1);
    Engine_ActorSetChildValue(21, 1);
    Engine_ActorSetChildValue(12, 1);
    Engine_ActorSetChildValue(13, 1);
    WaitFrames(1);
}

void FieldScene_SetupActorsForScene(void)
{
    struct ObjectRuntime *actor;

    FieldScene_ActivateTwoActorGroup();
    Engine_ActorSetChildValue(9, 1);
    Engine_ActorSetChildValue(10, 1);
    Engine_ActorSetChildValue(17, 1);
    if (GameFlag_IsSet(0x94c)) {
        Actor_SetPosition(15, 0, 0);
    }
    if (GameFlag_IsSet(0x949)) {
        Actor_SetPosition(11, 0, 0);
    }
    if (GameFlag_IsSet(0x94b)) {
        Actor_SetPosition(16, 0, 0);
    }
    if (GameFlag_IsSet(0xf2e)) {
        Actor_SetPosition(8, 0, 0);
    }
    switch (gGameState.entrance) {
    case 1:
    case 2:
    case 3:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Map_SetWorkFlagBits9To11(0xe00);
        Engine_TaskAddCallback(FieldScene_UpdateActorPairInteraction, 3200);
        WaitFrames(1);
        Engine_MapRedraw();
        WaitFrames(1);
        break;
    case 10:
    case 13:
    case 20:
    case 23:
    case 24:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
        Map_SetWorkFlagBits9To11(0xc00);
        Engine_ActorSetSpriteFlags(Object_GetById(24), 0);
        if (GameFlag_IsSet(0x314)) {
            Actor_SetPosition(25, 0x3680000, 0x780000);
        }
        break;
    case 21:
    case 22:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Map_SetWorkFlagBits9To11(0xe00);
        Engine_TaskAddCallback(FieldScene_UpdateActorSeventeenInteraction, 3200);
        WaitFrames(1);
        Engine_MapRedraw();
        WaitFrames(1);
        break;
    case 11:
    case 12:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        if (GameFlag_IsSet(0x94a)) {
            FieldScene_RunSequenceTail();
        }
        break;
    case 31:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        FieldScene_RunSequenceTail();
        break;
    case 14:
    case 15:
    case 16:
        Engine_TaskAddCallback(TriggerScene40AtVillagePath, 3200);
        break;
    default:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Map_SetWorkFlagBits9To11(0xe00);
        break;
    }
    actor = Object_GetById(8);
    Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
    Engine_ActorSetSpritePriority(8, 1);
    *(s32 *)&actor->unknown_18[0] = 0xc000;
    *(s32 *)&actor->unknown_18[4] = 0xc000;
}

void FieldScene_RestoreActorsFromFlags(void)
{
    struct ObjectRuntime *actor;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    FieldScene_ActivateAlternateActorGroup();
    if (GameFlag_IsSet(0x943)) {
        PlaceActorTwelveAndFinishScene();
    }
    GameFlag_Set(0x217);
    GameFlag_Set(0x218);
    if (GameFlag_IsSet(0x944)) {
        Actor_SetPosition(8, 0, 0);
        Engine_GameFlagClear(0x217);
    }
    if (GameFlag_IsSet(0x945)) {
        Actor_SetPosition(9, 0, 0);
        ConfigureInteractionRegionC();
    }
    if (GameFlag_IsSet(0x946)) {
        Actor_SetPosition(10, 0, 0);
        Engine_GameFlagClear(0x218);
    }
    if (GameFlag_IsSet(0x947)) {
        ConfigureInteractionRegionA();
    }
    if (GameFlag_IsSet(0x948)) {
        ConfigureInteractionRegionB();
    }
    Engine_EventBegin();
    actor = Object_GetById(8);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Object_GetById(9);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Object_GetById(10);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Object_GetById(11);
    if (actor != 0) {
        Engine_ActorSetSpriteFlags(actor, 0);
    }
    actor->unknown_23 = 2;
    actor = Object_GetById(12);
    if (actor != 0) {
        actor->unknown_56[3] |= 0x10;
    }
    Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
    Engine_EventEnd();
    Map_SetWorkFlagBits9To11(0xe00);
}

void FieldScene_ActivateThreeActorGroup(void)
{
    if (GameFlag_IsSet(0x35a)) {
        PlaceSceneObjectPairFromTableC(0);
    }
    if (GameFlag_IsSet(0x35b)) {
        PlaceSceneObjectPairFromTableC(1);
    }
    if (GameFlag_IsSet(0x35c)) {
        PlaceSceneObjectPairFromTableC(2);
    }
}

void FieldScene_ActivateTwoActorGroup(void)
{
    if (GameFlag_IsSet(0x358)) {
        PlaceSceneObjectPairFromTableB(0);
    }
    if (GameFlag_IsSet(0x359)) {
        PlaceSceneObjectPairFromTableB(1);
    }
}

void FieldScene_ActivateAlternateActorGroup(void)
{
    if (GameFlag_IsSet(0x355)) {
        FieldScene_SetPositionPairs(0);
    }
    if (GameFlag_IsSet(0x356)) {
        FieldScene_SetPositionPairs(1);
    }
    if (GameFlag_IsSet(0x357)) {
        FieldScene_SetPositionPairs(2);
    }
}

void ActivateFiveActorGroupFromFlags(void)
{
    if (GameFlag_IsSet(0x350)) {
        PlaceSceneObjectPairFromTableA(0);
    }
    if (GameFlag_IsSet(0x351)) {
        PlaceSceneObjectPairFromTableA(1);
    }
    if (GameFlag_IsSet(0x352)) {
        PlaceSceneObjectPairFromTableA(2);
    }
    if (GameFlag_IsSet(0x353)) {
        PlaceSceneObjectPairFromTableA(3);
    }
    if (GameFlag_IsSet(0x354)) {
        PlaceSceneObjectPairFromTableA(4);
    }
}
