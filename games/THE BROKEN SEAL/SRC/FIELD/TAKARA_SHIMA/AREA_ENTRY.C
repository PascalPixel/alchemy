#include "TYPES.H"
#include "SCENE_IDS.H"
extern u8 TakaraShima_EntranceCells[];

s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
s32 Party_SetFields1ceAnd1d0();
void BattleFx_SetWeightedResult();
void Engine_EventEnd();
void Engine_MapAnimateCells();
void Engine_AudioPlayCue();
void Engine_EventRequestExit();


extern s16 gGameState[][1];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

/* Crossbone Isle: on the first visit to an area (flag 0x8c8 + area) show its
 * title card; later visits animate the entrance cells and leave. */
void TakaraShima_RunAreaEntry(void)
{
    if (Engine_GameFlagIsSet(gGameState[224][0] + (0x8c8 - (s32)&SceneId_TakaraShima6)) == 0) {
        Engine_EventBegin();
        Party_SetFields1ceAnd1d0(gGameState[224][0], 5);
        ((u8 *)gGameState)[0x22b] = 3;
        switch (gGameState[224][0] - (s32)&SceneId_TakaraShima6) {
        case 0:
            BattleFx_SetWeightedResult(63, 0);
            break;
        case 1:
            BattleFx_SetWeightedResult(63, 1);
            break;
        case 2:
            BattleFx_SetWeightedResult(63, 2);
            break;
        case 3:
            BattleFx_SetWeightedResult(63, 3);
            break;
        case 4:
            BattleFx_SetWeightedResult(84, 0);
            break;
        case 5:
            BattleFx_SetWeightedResult(84, 1);
            break;
        case 6:
            BattleFx_SetWeightedResult(84, 2);
            break;
        case 7:
            BattleFx_SetWeightedResult(84, 3);
            break;
        case 8:
            BattleFx_SetWeightedResult(84, 4);
            break;
        }
        Engine_EventEnd();
    } else {
        Engine_MapAnimateCells((s32)TakaraShima_EntranceCells, 44, 7);
        Engine_AudioPlayCue(183);
        Engine_EventRequestExit(3);
    }
}
