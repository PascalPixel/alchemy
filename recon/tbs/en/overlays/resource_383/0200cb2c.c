/* Draft of the stop scene (FieldScene_RunScene383_02004b2c), resource_383 at
 * 0x0200cb2c (split from FIELD/KUUPUAPPU_HEYA/PROMPT.C); it would link as
 * FIELD/KUUPUAPPU_HEYA/STOPS.C with its stop timer in the overlay's .bss,
 * which the linker places at the image's end as the ROM expects.
 * Remaining difference: with the stop-update callback passed by its name,
 * GCC loads the timer's address into r2 and the zero into r3; the ROM, as a
 * literal callback address compiles, loads them the other way round (three
 * halfwords at +0xfc). Spellings tried: direct and wrapped calls, typed and
 * old-style prototypes, a local callback, s16, volatile and array timers,
 * extern and global timers, the callback defined in the same file.
 * The listing keeps these rows. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

void FieldScene_ConfigurePairedActors(void);
void Map_SetWorkFourValues();
void Scheduler_AddOrUpdateCallback();
void KuupuappuHeya_UpdateActorStops(void);

/* Frames of the stop updates; it follows the overlay's image. */
static u16 sStopFrames;

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers. */
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void FieldScene_RunScene383_02004b2c(void)
{
    Event_Begin();
    Camera_MoveTo(0xa80000, -1, 0x2900000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 0x2b8);
    Actor_SetPosition(ACTOR_GERALD, 0xf80000, 0x2b80000);
    Actor_SetPosition(ACTOR_IVAN, 0xf80000, 0x2b80000);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 200, 0x2b8);
    Actor_WalkTo(ACTOR_GERALD, 248, 0x2c8);
    Actor_WalkToAndWait(ACTOR_IVAN, 232, 0x2b8);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 0);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_GERALD, 12);
    FieldScene_ConfigurePairedActors();
    Call4(Map_SetWorkFourValues, 0x300000, 0x2400000, 0x1200000, 0x2e00000);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0xc000, 0x6000);
    Actor_SetSpeed(24, 0x10000, 0x13333);
    Actor_SetSpeed(25, 0x18000, 0x18000);
    sStopFrames = 0;
    Call2(Scheduler_AddOrUpdateCallback, (s32)KuupuappuHeya_UpdateActorStops, 0xc94);
    Engine_GameFlagClear(0x1ff);
    Event_End();
    Audio_PlayCue(9);
}
