#include "TYPES.H"
#include "SCENE_IDS.H"

extern u8 gGameState[];
s32 Engine_GameFlagIsSet();
void Engine_GameFlagClear();
void Engine_EventBegin();
void Engine_EventEnd();
void Battle_ResetEffectCounterFar();
void Func_02007fcc();
void Func_02007fe8();
void Func_02008004();

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
void Party_SetFields1ceAnd1d0();
void BattleFx_SetWeightedResult();

void FuneHeya_RunFlagBranchSequence(void)
{
    s32 base3_2000240;

    Engine_EventBegin();
    Battle_ResetEffectCounterFar();
    base3_2000240 = (s32)gGameState;
    *(u8 *)((base3_2000240 + 0x22b)) = 3;
    /* FAKEMATCH: the do/while orders the flag store before this call, and
     * 0x6f comes from the literal pool through a link symbol. */
    do {
        Call1(Engine_GameFlagClear, 0x8f0);
    } while (0);
    if (Value1(Engine_GameFlagIsSet, 0x928) == 0) {
        Party_SetFields1ceAnd1d0((s32)&SceneId_FuneHeya, 16);
        BattleFx_SetWeightedResult(62, 0);
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x929) == 0) {
            Party_SetFields1ceAnd1d0((s32)&SceneId_FuneHeya, 18);
            BattleFx_SetWeightedResult(62, 1);
        } else {
            if (Value1(Engine_GameFlagIsSet, 0x92a) == 0) {
                Party_SetFields1ceAnd1d0((s32)&SceneId_FuneHeya, 20);
                BattleFx_SetWeightedResult(62, 2);
            }
        }
    }
    Engine_EventEnd();
}
