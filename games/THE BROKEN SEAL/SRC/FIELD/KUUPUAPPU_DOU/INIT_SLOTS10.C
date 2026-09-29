#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "TYPES.H"

u8 *Object_GetById(s32);

#include "TYPES.H"

void Vector_AddPolarOffset(s32, s32, s32 *);
void Object_SetMoveTarget(s32 *, s32, s32, s32);

#include "TYPES.H"

void KuupuappuDou_PushBlockAhead();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
s32 SceneActor_LiftLowActorOnSubjectTile(s32 subject_actor);

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

#include "TYPES.H"

void Scheduler_AddOrUpdateCallback();

#include "TYPES.H"

void KuupuappuDou_SpawnPuffs();
s32 IwramUnsignedRemainder();

/* The scene step counter at 0x1d8 of the shared scene work record. */

static __inline__ void Call1_02000754(void (*f)(), s32 a0)
{

    f(a0);
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

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{

    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call1_02000aa0(void (*f)(), s32 a0)
{

    f(a0);
}

#include "TYPES.H"

#include "TYPES.H"

#include "TYPES.H"

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
};

extern void KuupuappuDou_RaiseActorPriorities(void);

#include "TYPES.H"

/* Deliberate no-op callback. */

#include "TYPES.H"

#include "TYPES.H"
void FieldScene_RunOpeningAuxiliarySequence(void);
void FieldScene_RunScene3a7SequenceD(void);

enum SelectByRuntimeSelectorMessage {
    MSG_DOOR_TIGHTLY_LOCKED = 0x953,
    MSG_ROBIN_FLIPPED_SWITCH = 0x1528
};

extern u8 *Object_GetById(s32);

void SceneActor_InitSlots10To15AndStartTask(void)
{
    s32 selector = 10;
    s32 remaining = 5;

    do {
        s32 *record;

        Actor_SetSpriteFlags(Actor_Get(selector), 0);
        record = Actor_Get(selector);
        record[17] = 0x1999;
        record[18] = 0;
        remaining--;
        record[3] = 0x00ff0000;
        selector++;
    } while (remaining >= 0);

    {
        s32 rank = 0xc80;

        Scheduler_AddOrUpdateCallback((s32)FieldScene_RunScene3a7SequenceD, rank);
    }
}

void SceneActor_SetupActors11To14AndInstallTask(void)
{
    s32 no = 11;
    s32 i = 0;

    do {
        s32 *rec;

        Actor_SetSpriteFlags(Actor_Get(no), 0);
        rec = Actor_Get(no);
        rec[17] = 0x1999;
        rec[18] = 0;
        rec[3] = 0x00ff0000;
        Actor_SetSpritePriority(i + 11, 1);
        i++;
        no++;
    } while (i <= 3);

    {
        s32 rate = 0xc80;

        Scheduler_AddOrUpdateCallback((s32)FieldScene_RunOpeningAuxiliarySequence, rate);
    }
}
