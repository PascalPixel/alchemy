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

enum KyudenMessage {
    MSG_DO_THINK_THESE_BILIBINS_GREAT = 0x1432,
    MSG_WE_REALLY_GIVING_OUR_TREASURE = 0x1434,
    MSG_GOOD_TREASURE_IF_GET_TURNED = 0x1436,
    MSG_YEHVE_HAD_CHANGE_HEART_YEH = 0x1442,
    MSG_WEVE_BROUGHT_WARRIORS_MILORD = 0x1728,
    MSG_HUMBLY_THANK = 0x1737,
    MSG_NAE_YEH_DINNAE_NEED_TAE = 0x1738,
    MSG_WAS_BUT_WORRIED_YEH_MIGHT = 0x1739,
    MSG_PLEASE_TAKE_YOUR_REWARD_BEFORE = 0x1748,
    MSG_ALWAYS_WELCOME_IN_PALACE_LORD = 0x1749,
    MSG_MAY_CHOOSE_ONLY_ONE_ITEM = 0x174b,
    MSG_NONETHELESS_IF_YOUR_LUCK_SOUR = 0x174c,
    MSG_ROBIN_CHECKED_CHEST_BUT_WAS = 0x174d,
    MSG_TREASURE_CHEST_LOCKED = 0x174e,
    MSG_FINE_WARRIOR = 0x1750,
    MSG_SOMETIMES_WE_NEED_CHILDREN_REMIND = 0x1756,
    MSG_AAAAH = 0x1764,
    MSG_ITS_VERY_RECKLESS_FOR_SUCH = 0x1768,
    MSG_WAS_WATCHING_FROM_HERE_AFTER = 0x1769,
    MSG_LOOKED_VERY_COURAGEOUS_WALKING_TOWARD = 0x176c,
    MSG_REWARD_RECEIVED_WAS_INDEED_GREATEST = 0x176e,
    MSG_GOT_PRETTY_NICE_REWARD_BUT = 0x176f
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
