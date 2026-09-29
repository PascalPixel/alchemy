#ifndef HAIDIA_BABI_H
#define HAIDIA_BABI_H

#include "TYPES.H"
#include "FACING_OBJECT.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/*
 * The Vale house scene: the hooks its entry veneers export, the villagers'
 * dialogue, the Mythril Bag scene and its sparkle effects, and the Psynergy
 * steps.
 */

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

enum {
    /* Message 0x182 + 224. */
    ITEM_MYTHRIL_BAG = 224
};

enum ValeMessage {
    MSG_THE_THREE_TRAVELERS_SEEM_ODD = 0xf58,
    MSG_HEY_BOY = 0xf5b,
    MSG_THE_VISITORS_CAUSED_THE_ERUPTION = 0x11a9,
    MSG_THE_MASKED_MAN_WAS_GARCIA = 0x11aa,
    MSG_YOUVE_GROWN_SO_MUCH = 0x1c13,
    MSG_YOU_CAME_BACK = 0x1c14,
    MSG_HOME_JUST_TO_STAY = 0x1c15,
    MSG_DORA_WAS_STRUCK_WITH_ILLNESS = 0x1c1a,
    MSG_DORA_WOULDNT_LET_HIM_STAY = 0x1c1b
};

/* The scene's tables, in the overlay's read-only data. */
extern s32 gHaidiaBabiEntrances[];
extern s32 gHaidiaBabiExits2[];
extern s32 gHaidiaBabiExits[];
extern s32 gHaidiaBabiPlacements4[];
extern s32 gHaidiaBabiPlacements3[];
extern s32 gHaidiaBabiPlacements2[];
extern s32 gHaidiaBabiPlacements[];
extern s32 gHaidiaBabiEvents6[];
extern s32 gHaidiaBabiEvents5[];
extern s32 gHaidiaBabiEvents4[];
extern s32 gHaidiaBabiEvents3[];
extern s32 gHaidiaBabiEvents2[];
extern s32 gHaidiaBabiEvents[];

void SceneState_SetWork448To521AndRun(s32 exit);
void ActorPresentation_SetTwoSceneCells(void);
void FieldScene_RunSupplementalSequenceOne();
void SceneEffect_UpdateAnchoredRiseFrame();
void OverlayObject_UpdateArcFromAnchor();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

#endif
