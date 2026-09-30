#include "TYPES.H"
#include "SCENE_IDS.H"
#include "CALL.H"
extern struct EventWork *gEventWork;

void RamakanSabaku_ClaimSandEffectVram();
void Engine_TaskAddCallback();
void Engine_MapCopyCells();
void BattleFx_SetQueuedSoundAndPlay();
void RamakanSabaku_RaiseQuarterTriggers(void);
void RamakanSabaku_ApplyEntryState();

extern u16 gGameState[][2];

/* Lamakan Desert: reset the area timers and, outside the town variant, open
 * the map cells of the stored area variant before applying the entry state. */
s32 RamakanSabaku_EnterArea(s32 a0, s32 a1)
{
    s32 v5;

    gGameState[139][0] = 0x258;
    gGameState[139][1] = 0;
    gGameState[140][0] = 0x119;
    if ((s16)gGameState[112][0] == (s32)&SceneId_RamakanSabaku4) {
    } else {
        *(s32 *)((*(s32 *)&gEventWork + 0x1c0)) = 0x100;
        RamakanSabaku_ClaimSandEffectVram();
        Engine_TaskAddCallback((s32)RamakanSabaku_RaiseQuarterTriggers, 0xc80);
        if ((s16)gGameState[112][0] == (s32)&SceneId_RamakanSabaku1) {
            Call6(Engine_MapCopyCells, 22, 7, 4, 2, 64, 126);
            Engine_MapCopyCells(8, 10, 4, 2, 68, 126);
            Engine_MapCopyCells(23, 21, 4, 2, 72, 126);
            Engine_MapCopyCells(16, 42, 4, 2, 76, 126);
            Engine_MapCopyCells(36, 44, 4, 2, 80, 126);
            Engine_MapCopyCells(14, 55, 4, 2, 84, 126);
        } else {
            if ((s16)gGameState[112][0] != (s32)&SceneId_RamakanSabaku2) {
                goto third_area;
            }
            Call6(Engine_MapCopyCells, 42, 5, 4, 2, 64, 126);
            Engine_MapCopyCells(20, 11, 4, 2, 68, 126);
            Engine_MapCopyCells(14, 12, 4, 2, 72, 126);
            Engine_MapCopyCells(56, 18, 4, 2, 76, 126);
            Engine_MapCopyCells(7, 22, 4, 2, 80, 126);
            Engine_MapCopyCells(44, 23, 4, 2, 84, 126);
            Engine_MapCopyCells(38, 24, 4, 2, 88, 126);
            Engine_MapCopyCells(26, 28, 4, 2, 92, 126);
            Engine_MapCopyCells(17, 35, 4, 2, 96, 126);
            Engine_MapCopyCells(50, 36, 4, 2, 100, 126);
            Engine_MapCopyCells(34, 43, 4, 2, 104, 126);
            Engine_MapCopyCells(6, 46, 4, 2, 108, 126);
            Engine_MapCopyCells(27, 55, 4, 2, 112, 126);
            Engine_MapCopyCells(43, 56, 4, 2, 116, 126);
        }
        goto apply_entry_state;
        third_area:;
        if ((s16)gGameState[112][0] == (s32)&SceneId_RamakanSabaku3) {
            v5 = 124;
            BattleFx_SetQueuedSoundAndPlay(169);
            Engine_MapCopyCells(8, 14, 4, 4, 64, v5);
            Engine_MapCopyCells(6, 18, 4, 4, 68, v5);
            Engine_MapCopyCells(10, 21, 4, 4, 72, v5);
        }
        apply_entry_state:;
        RamakanSabaku_ApplyEntryState();
    }
    return 0;
}
