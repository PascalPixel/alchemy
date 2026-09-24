#include "TYPES.H"

s32 Engine_ActorGet();
void Engine_EventBegin();
s32 StagedActor_FillGridAttributeRectangle();
s32 Engine_GameFlagIsSet();
void Engine_EventWait();
void Engine_AudioPlayCue();
void Engine_GameFlagSet();
void Engine_EventEnd();



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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Crossbone Isle: block actor 10's cell and, the first time it reaches
 * column 16 (flag 0x204), drop it into the floor with cue 159. */
void TakaraHashira_DropActorTen(void)
{
    u8 *rec7;
    s32 rec8;

    rec7 = Value1(Engine_ActorGet, 10);
    Engine_EventBegin();
    StagedActor_FillGridAttributeRectangle(2, (*(s32 *)((s32)rec7 + 8) >> 20), (*(s32 *)((s32)rec7 + 16) >> 20), 1, 1, 255);
    if ((*(s32 *)((s32)rec7 + 8) >> 20) == 16) {
        rec8 = Value1(Engine_GameFlagIsSet, 0x204);
        if (rec8 == 0) {
            Engine_EventWait(10);
            Engine_AudioPlayCue(159);
            rec7[85] = rec8;
            *(s32 *)((s32)rec7 + 20) = -0x20000;
            *(s32 *)((s32)rec7 + 12) = -0x20000;
            Call1(Engine_GameFlagSet, 0x204);
        }
    }
    Engine_EventEnd();
}
