#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

union Slot {
    s32 w;
    s16 h[2];
};

extern u8 LinkedMessage_TheresNothingWeCanDo[];

s32 BuildMotionCountdown(s32, s16);

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* The step value differs in the localized scene data. */

/* Offset of a flag byte on an actor record, cleared and set below. */

/* Pointer, held at fixed address 0x03001ebc, to the shared scene work
 * record. The phase/status word lives at offset 0x1c0 of that record. */

/* Call sites spelled through these wrappers pass their constants straight
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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)&gEventWork;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call1_020029d4(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ void Call1_02003a0c(void (*f)(), s32 a0)
{

    f(a0);
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#endif

#if defined(TBS_EDITION_JA)
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#else
#endif

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#endif
#if defined(TBS_EDITION_DE)
#endif

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

void SceneState_ApplyFiveRectsAtColumn78(void)
{
    Map_CopyCellsTo(78, 39, 78, 40, 5, 1);
    Map_CopyCellsTo(78, 39, 78, 41, 5, 1);
    Map_CopyCellsTo(78, 39, 79, 42, 4, 1);
    Map_CopyCellsTo(78, 39, 82, 43, 1, 1);
    {
        s32 x = 17;
        s32 y = 40;

        Map_CopyCellAttributes(17, 38, 5, 2, x, y);
    }
}

void DialogueLayout_ConfigureTwoRegions(void)
{
    Map_CopyCellsTo(66, 61, 64, 40, 5, 4);
    Map_CopyCellAttributes(0, 0, 5, 4, 5, 39);
}

void FieldScene_RunStepThen10(s32 a)
{
    Event_ShowMessage(a, 0);
    Event_Wait(10);
}

void FieldScene_CallPairWith10(s32 a, s32 b)
{
    Actor_FaceDirection(a, b, 10);
}
