/* NONMATCHING: 2026-10-01 brief Wave2 Goma_SetSpeed plain-source attempt.
 * Removing this one source device changes FieldScene_RunActor13Departure.
 * Remaining difference: a direct call changes FieldScene_RunActor13Departure from mov r0, #13 to lsl r2, r2, #9 (82/82 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * Production retains the measured helper with a body-local FAKEMATCH reason.
 */
/*
 * Actor 13's departure and the scene initialiser that restores the pillars.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"

extern u8 *gWork;
/* The game state read as halfwords: 225 is the entrance. */
extern s16 Data_02000240[];

/* Frames actor 13 has idled; ACTOR13_IDLE.C owns it. */
extern s32 GomaHashira_Actor13Frames;

/* The scene's tables, laid out after the code. */
extern u8 GomaHashira_Scripts[];
extern u8 GomaHashira_Messages[];
extern u8 GomaHashira_Actors[];

/* Imports this overlay shares with the staged-actor module, by its names. */
void Map_CopyCellAttributeRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);

void BattleFx_SetPhaseRequest(s32, s32);
void GomaHashira_SpawnPillarEffect();
void FieldScene_RunPillarSequence(void);
void GomaHashira_DriveActor13Idle(void);

/*
 * The event services under the names the staged-actor module gives their
 * imports, spelled as FIELD_EVENT.H spells its own.
 */


#include "TYPES.H"
#include "CALL.H"
extern u8 MsgGomaGotWowThatsPrettyImpressive[];

/* Sets bits in an actor's flag byte. */
static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}

/* Draft context: these removed shared adapters isolate this one attempted device. */
static inline void Event_Begin(void)
{
    Engine_EventBegin();
}

static inline void Event_End(void)
{
    Engine_EventEnd();
}

static inline void Event_SetMessage(s32 message)
{
    Engine_EventSetMessage(message);
}

static inline void Actor_SetAnimationAndWait(s32 actor, s32 animation)
{
    Engine_ActorSetAnimationAndWait(actor, animation);
}

static inline void Actor_RunRepeatedMotion(s32 actor, s32 repeats)
{
    Engine_ActorRunRepeatedMotion(actor, repeats);
}

void FieldScene_RunActor13Departure(void)
{
    Engine_TaskRemoveCallback((s32)GomaHashira_DriveActor13Idle);
    Event_Begin();
    Actor_ShowEmote(13, 0x100, 30);
    Actor_RunRepeatedMotion(13, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Event_SetMessage((s32)MsgGomaGotWowThatsPrettyImpressive);
    Event_ShowMessage(13, 0);
    Actor_SetAnimationAndWait(13, 3);
    Battle_WaitMode0(30);
    *(u8 *)((u8 *)Object_GetById(10) + 35) &= 253;
    ObjectMotion_SetSpeedParameters(13, 0x20000, 0x10000);
    Actor_WalkToAndWait(13, 0x258, 216);
    Actor_WalkToAndWait(13, 0x258, 248);
    Actor_WalkToAndWait(13, 0x238, 0x128);
    Actor_SetPosition(13, 0, 0);
    SetFlagBits((u8 *)Object_GetById(10) + 35, 2);
    GameFlag_Set(0x869);
    Event_End();
}

s32 FieldScene_SetupPillarsOnEntry(void)
;
