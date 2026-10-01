/* NONMATCHING: 2026-10-01 brief Wave2 DrawPlacement plain-source attempt.
 * Removing this one source device changes FieldScene_RunTile10x20Transition, Scene_Initialize.
 * Remaining difference: a direct call changes FieldScene_RunTile10x20Transition from mov r2, #17 to mov r3, #19 (79/79 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * Production retains the measured helper with a body-local FAKEMATCH reason.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KORIMA_MURA.H"
#include "STAGED_ACTOR.H"
#include "STAGED_ACTOR_EFFECT.H"
extern u8 MsgKorimaHesAsStumpedAsWe[];
extern u8 MsgKorimaKnowThoseFieldsWere[];
extern u8 MsgKorimaMatterIvan[];
extern u8 MsgKorimaSparklyStuffOnGround[];
extern u8 MsgKorimaThatsReliefRobinThoughtYoud[];
extern u8 MsgKorimaWasOurPsynergy[];
extern u8 MsgKorimaWatchOutItsHappeningAgain[];
extern u8 MsgKorimaYoureRobinThereIsntMuch[];

struct Struct3848 {
    u8 pad00[8];
    u32 field08;
    s32 field0c;
    u32 field10;
};

struct Struct2798 {
    u8 pad00[0x18];
    s32 field18;
    u8 pad1c[0x38 - 0x1c];
    s32 field38;
    s32 field3c;
    s32 field40;
};

struct Sub { u8 pad00[9]; u8 f09; u8 pad0a[28]; u8 f26; };

struct Obj {
    u8 pad00[0x18];
    s32 f18;
    u8 pad1c[7];
    u8 f23;
    u8 pad24[12];
    s32 f30;
    s32 f34;
    u8 pad38[24];
    struct Sub *f50;
    u8 pad54[1];
    u8 f55;
};

struct Struct288c {
    u8 pad00[8];
    s32 field08;
    u8 pad0c[4];
    s32 field10;
};

/* The bridge's own work, past the overlay image. */
s32 KorimaHashi_TriggerPending __attribute__((section(".bss")));
s32 KorimaHashi_PartyFlag __attribute__((section(".bss")));
s32 KorimaHashi_SparkleSound __attribute__((section(".bss")));

/* Tables laid out after the code. */
extern const struct SceneEntrance KorimaHashi_Entrances[];
extern const u32 KorimaHashi_Exits[];
extern const struct ScenePlacement KorimaHashi_Placements[];
extern const struct SceneEvent KorimaHashi_Events[];
extern u8 KorimaHashi_Object26Script[];
extern u8 KorimaHashi_TriggerScript[];


/* The main-image services this bridge reaches through veneers the
   staged-actor module names. */
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void Object_SetModeById(s32 actor, s32 mode);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 dx, s32 dz);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);

/* Motion services in the shape of the engine header's, through the names
   the staged-actor module gives their veneers. */


static __inline__ void Actor_OffsetDestination(s32 actor, s32 dx, s32 dz)
{
    /* FAKEMATCH: a direct call changes FieldScene_RunBranchingFormationPresentation from mov r0, #3 to neg r1, r1 (2674/2674 assembly lines). */
    ObjectMotion_OffsetPositionAndResetMotion(actor, dx, dz);
}
void Map_CopyCellAttributeRect(s32 x, s32 z, s32 width, s32 height, s32 dest_x, s32 dest_z);
void Object_SetPosition(struct Obj *object, s32 x, s32 y, s32 z);

s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe);


s32 Object_CheckMovementCollision(struct StagedActorEffect *actor,
                                  struct StagedActorEffectRequest *request);

/* Clear the pending trigger after restoring the object's mode. */
s32 OverlayObject_ClearPendingAndRestoreMode(u8 *object)
;

s32 SceneActor_UpdateRandomCounterMode(u8 *object)
;

const struct SceneEntrance *Scene_GetEntrances(void) { return KorimaHashi_Entrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) ;

const struct ScenePlacement *Scene_GetPlacements(void) { return KorimaHashi_Placements; }

/* Placement query followed by the tile-(10,20) scene transition. */
/* Draft context: these removed shared adapters isolate this one attempted device. */
static inline void Event_Begin(void)
{
    Engine_EventBegin();
}

static inline void Event_End(void)
{
    Engine_EventEnd();
}

static inline void Actor_SetSpriteFlags(struct FieldActor *actor, s32 flags)
{
    Engine_ActorSetSpriteFlags(actor, flags);
}

void FieldScene_RunTile10x20Transition(void)
{
    struct StagedActorProbe res;
    Event_Begin();

    if (StagedActor_FindClearPosition(&res)) {
        SceneActor_MoveAndRedraw(res);
        if (res.actor_slot == 10 && (res.position_x >> 20) == 20) {
            u8 *actor;
            s32 zero;

            Object_SetModeById(10, 3);
            Actor_OffsetDestination(10, -18, 6);
            Battle_WaitMode0(30);
            Audio_PlayCue(240);
            Object_SetModeById(10, 8);
            ((u8 *)Object_GetById(10))[35] = 2;
            zero = 0;
            Map_CopyCellAttributeRect(0, 17, 2, 4, 19, 17);
            StagedActor_FillGridAttributeRectangle(2, 20, 17, 1, 4, zero);
            GameFlag_Set(0x200);
            actor = (void *)Object_GetById(10);
            Actor_SetSpriteFlags((struct FieldActor *)actor, 0);
        }
    }

    Event_End();
}

s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request)
;

/* The selected actor is read through a register offset into the game state. */
void SceneActor_PassSubjectOffsetPosition(void)
;

const struct SceneEvent *Scene_GetEvents(void) { return KorimaHashi_Events; }

void FieldScene_RunBranchingFormationPresentation(void);

s32 Scene_Initialize(void)
;

void Object_RefreshSelectorById(s32 id);
void SceneActor_SetPairZeroAndValue(s32 actor, s32 facing, s32 frames);
void Event_SayThenWait(s32 speaker, s32 frames);
void Object_SetActionCallbackAndRefreshById(s32 id, s32 callback);
void Audio_PlayCueFromEventWork(void);
void SceneActor_AlternateSlots13To16Field0c(void);

extern u8 KorimaHashi_GeraldAction[];
extern u8 KorimaHashi_IvanAction[];
extern u8 KorimaHashi_MiaAction[];
extern u8 KorimaHashi_PartyAction[];
extern u8 KorimaHashi_ResetAction[];
extern u8 KorimaHashi_FormationAction[];
extern u8 KorimaHashi_FinishAction[];
extern u8 KorimaHashi_EntryAction[];

void FieldScene_RunBranchingFormationPresentation(void)
;

/* Shows the next line of dialogue, then holds the scene for a moment. */
void Event_SayThenWait(s32 speaker, s32 frames)
;

void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
;

s32 SceneEffect_AdvanceAngleAndFinishWhenParked(struct Struct2798 *p)
;

void SceneEffect_SpawnObject26EveryEightFrames(void)
;

s32 OverlayObject_SelectValueByFrameBit1(s32 obj)
;

s32 SceneActor_CheckRegionTrigger(struct Struct288c *arg0)
;

