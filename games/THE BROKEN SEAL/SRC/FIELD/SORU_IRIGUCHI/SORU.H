#ifndef ALCHEMY_SORU_H
#define ALCHEMY_SORU_H

/* The sanctum entrance (resource_37f): what its files share. */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/* Sukureta answers Gerald's "What's wrong, Kraden?" as placed actor 8. */
enum {
    ACTOR_SUKURETA = ACTOR_FIRST_PLACED
};

enum SanctumEntranceFlag {
    /* Set after the party is asked whether this is its first visit. */
    FLAG_SOL_SANCTUM_ENTERED = 0x802,
    /* Set once the party decides to help Sukureta look for the hidden passage. */
    FLAG_SEARCHING_FOR_HIDDEN_PASSAGE = 0x804
};


#define GATE_ID 2081
#define RECORD_A32(rec) (*(s32 *)((rec) + 8))
#define RECORD_B32(rec) (*(s32 *)((rec) + 16))
#define RECORD_A16(rec) (*(s16 *)((rec) + 10))
#define RECORD_B16(rec) (*(s16 *)((rec) + 18))

/* The cue timer, the overlay's work past its image; ENTRY_STATE.C owns it. */
extern s32 SoruIriguchi_CueTimer;
extern u8 *gWork;

void Battle_ResetEffectCounter(void);

void Scene_RunActorFormation(s32 a0);
void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2);

/*
 * FAKEMATCH: call sites spelled through these wrappers pass their constants
 * straight into the argument registers; a direct call precomputes a costly
 * constant into a pseudo that the compiler then shares with later uses in
 * the block. A value-returning call also sets r0 last of its arguments.
 */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

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

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Scene_Call(void (*func)(s32), s32 arg)
{
    func(arg);
}


#endif
