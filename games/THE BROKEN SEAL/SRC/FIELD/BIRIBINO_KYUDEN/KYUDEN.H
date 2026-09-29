/* Declarations shared by McCoy's palace sources in Bilibin (resource_38d). */
#ifndef KYUDEN_H
#define KYUDEN_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FACING_OBJECT.H"

#define NULL ((void *)0)

enum {
    FLAG_REWARD_TAKEN = 0x84e
};


/* The scene's tables and action tables, laid out after the code. */
extern u8 Placement_Scripts[];
extern u8 Placement_Messages[];
extern const u8 Kyuden_FacingActions[];
extern const u8 Kyuden_PacingActions[];

/* Main-image code reached through the overlay's import veneers. Old-style
 * declarations: the scenes pass their arguments as plain words. */
s32 ArcTan2(s32, s32);
void Map_ClearLayerEntryFlag();
void Map_SetLayerEntryFlag();
void Object_RefreshSelectorById();
void Object_SetTargetAndCallback();
s32 BattleFx_PlayCueAndStartEmitterOnTarget(s32, s32, s32);

void FieldScene_RunChestStep(s32 flag);
void FieldScene_RunSlotSubjectBranch(s32 actor, s32 item, s32 flag);
void FieldScene_RunLongBranchingChoreography(void);
void FieldScene_RunPalaceGreeting(void);
void ConfigurePrimarySceneChannels(void);
void ConfigureSecondarySceneChannels(void);
void RunEventScript02(void);
void BiribinoKyuden_RunActorRowScene(void);

/* FAKEMATCH: Calls through these inline helpers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void AdvanceMessage(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void FaceActor(s32 actor, s32 direction, s32 frames)
{
    Actor_FaceDirection(actor, direction, frames);
}

#endif
