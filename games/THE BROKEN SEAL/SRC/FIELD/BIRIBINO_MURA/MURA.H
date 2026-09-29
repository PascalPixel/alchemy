/* Declarations shared by Bilibin village's sources (resource_38b). */
#ifndef MURA_H
#define MURA_H

#include "TYPES.H"

#define NULL ((void *)0)

#include "FACING_OBJECT.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

enum {
    /* Message 0x182 + 194. */
    ITEM_HARD_NUT = 194
};

enum PushPuzzleMessage {
    MSG_ROBIN_PEERED_INTO = 0x947,
    MSG_ITS_TREE_BUT_ALMOST_LOOKS = 0x13ae,
    MSG_DID_SEE_TREE_AT_ENTRANCE = 0x13b3,
    MSG_HAVE_TRIED_HEADING_SOUTHEAST_FROM = 0x13b7,
    MSG_MCCOYS_HIDDEN_WAREHOUSE_DO_NOT = 0x146e,
    MSG_THERE_TREE_LOOKS_LIKE_PERSON = 0x1470,
    MSG_AREA_OFF_LIMITS_THOSE_WITHOUT = 0x1472,
    MSG_WAS_TURNED_INTO_TREE_FOR = 0x16bf,
    MSG_CURSE_WAS_BROKEN_THANKS_EFFORTS = 0x16c8,
    MSG_HAVE_EVER_BEEN_VILLAGE_IMIL = 0x16cc,
    MSG_THANK_SAVED_ME_FROM_BEING = 0x1774,
    MSG_YOURE_GUY = 0x1775,
    MSG_JILL_GAVE_ROBIN_SPECIAL_GIFT = 0x177a,
    MSG_BOTTOM_NOT_VISIBLE_LOOKS_VERY = 0x29dd
};

/* The scene's tables, laid out after the code. */
extern u8 Placement_Messages[];
/* The village's three facing villagers' action table. */
extern u8 Mura_VillagerActions[];
/* Per door: the animated cell steps and where they are drawn. */
extern const u16 *const Mura_DoorCellSteps[];
extern const s16 Mura_DoorCellOrigins[];
/* The ten cells the push puzzle repaints, as tile x and z pairs. */
extern u8 Mura_RepaintCells[];
/* The spawned kind-24 effect's animation script. */
extern const s32 Mura_SpawnScript[];

s32 ArcTan2(s32, s32);
void BattleFx_RunPageEffectForSlot(s32, s32, s32);
void RunSceneTransitionEffect();
void Object_SetTargetAndCallback();
void AnimationObjects_SelectAnimation();

/* FAKEMATCH: Call sites spelled through these wrappers pass their constants straight
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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Shared cross-overlay scene-record block; +450 is the scene sub-state. */

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ u16 ReadU16Elem(u16 *base, s32 idx)
{

    return *(u16 *)(base + idx);
}

static __inline__ void bump_step_020001ec(s32 amount)
{
    gEventWork->message += amount;
}


#endif
