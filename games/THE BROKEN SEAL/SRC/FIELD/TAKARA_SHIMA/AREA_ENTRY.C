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
