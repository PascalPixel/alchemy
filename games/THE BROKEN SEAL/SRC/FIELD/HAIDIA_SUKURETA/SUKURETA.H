/* Declarations shared by Sukureta's cottage sources in Vale (resource_375). */
#ifndef SUKURETA_H
#define SUKURETA_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FACING_OBJECT.H"

/*
 * Sukureta's cottage in Vale. Outside it, Saturos and Menardi are overheard
 * plotting to use Sukureta and let the children go; inside, Sukureta and the
 * children decide to see for themselves whether the strangers have been to
 * Mt. Aleph and Sol Sanctum.
 */

enum CottageActor {
    ACTOR_SUKURETA = ACTOR_FIRST_PLACED + 5,
    ACTOR_SATUROS,
    ACTOR_MENARDI
};

enum CottageFlag {
    FLAG_SANCTUM_VISIT_PLANNED = 0x801,
    FLAG_MET_SATUROS_AND_MENARDI = 0x808
};

#define NULL ((void *)0)

enum CottageMessage {
    MSG_SATUROS_GO = 0xf98,
    MSG_SUKURETA_OH_ROBIN = 0xfa6,
    MSG_SUKURETA_I_WAITED_YEARS_FOR_THE_SANCTUM = 0xfb1,
    MSG_JASMINE_THEY_MIGHT_BE_THIEVES = 0xfb2,
    MSG_SUKURETA_WE_ONLY_CHECK_THE_MOUNTAIN = 0xfbd,
    MSG_SUKURETA_WE_ONLY_CHECK_MT_ALEPH = 0xfbe,
    MSG_JASMINE_OUR_SECRET = 0xfc2,
    MSG_GERALD_YOU_CAN_HANDLE_THE_DANGER = 0xfc6,
    MSG_GERALD_ILL_TAKE_OVER_IF_NERVOUS = 0xfc9,
    MSG_SUKURETA_OUR_BEST_BET = 0xfcc,
    MSG_ILL_CLIMB_THE_FENCE_SOMEDAY = 0x11c4,
    MSG_MEMORIES_OF_THIS_COTTAGE = 0x1c96
};

/* The scene's tables, laid out after the code. */
extern u8 Placement_Scripts[];
extern u8 Placement_Messages[];
extern u8 Placement_Actors[];
/* One effect table per stage of the story: before flag 0x834, after it and
 * after flag 0x87a. */
extern u8 Placement_Effects[];
extern u8 Placement_Effects834[];
extern u8 Placement_Effects87a[];
/* The cottage gate's animated cell steps. */
extern const u16 Sukureta_GateCells[];

s32 ArcTan2(s32, s32);
void BattleFx_SetWeightedResult(s32, s32);

/* FAKEMATCH: Call sites spelled through these wrappers pass their constants
 * straight into the argument registers; a direct call precomputes a costly
 * constant into a pseudo that the compiler then shares with later uses in
 * the block. A value-returning call also sets r0 last of its arguments. */
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

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void Scene_SetActorDirection(s32 actor, s32 angle, s32 frames)
{
    Actor_FaceDirection(actor, angle, frames);
}

#endif
