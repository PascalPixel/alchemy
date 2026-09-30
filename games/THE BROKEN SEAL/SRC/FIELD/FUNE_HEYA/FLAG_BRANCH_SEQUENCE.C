#include "TYPES.H"
#include "HEYA.H"
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

void Party_SetFields1ceAnd1d0();
void BattleFx_SetWeightedResult();

void FuneHeya_RunFlagBranchSequence(void)
{
    s32 base3_2000240;

    Engine_EventBegin();
    Battle_ResetEffectCounterFar();
    base3_2000240 = (s32)gGameState;
    *(u8 *)((base3_2000240 + 0x22b)) = 3;
    /* FAKEMATCH: the do/while orders the flag store before this call. */
    do {
        Engine_GameFlagClear(0x8f0);
    } while (0);
    if (Engine_GameFlagIsSet(0x928) == 0) {
        Party_SetFields1ceAnd1d0((s32)&SceneId_FuneHeya, 16);
        BattleFx_SetWeightedResult(62, 0);
    } else {
        if (Engine_GameFlagIsSet(0x929) == 0) {
            Party_SetFields1ceAnd1d0((s32)&SceneId_FuneHeya, 18);
            BattleFx_SetWeightedResult(62, 1);
        } else {
            if (Engine_GameFlagIsSet(0x92a) == 0) {
                Party_SetFields1ceAnd1d0((s32)&SceneId_FuneHeya, 20);
                BattleFx_SetWeightedResult(62, 2);
            }
        }
    }
    Engine_EventEnd();
}
