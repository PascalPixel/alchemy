#include "TYPES.H"
#include "SCENE_IDS.H"
extern struct EventWork *gEventWork;

void RamakanSabaku_ClaimSandEffectVram();
void Engine_TaskAddCallback();
void Engine_MapCopyCells();
void BattleFx_SetQueuedSoundAndPlay();
void Func_02000cd0(void);
void RamakanSabaku_ApplyEntryState();

extern u16 gGameState[][2];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

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
        Call2(Engine_TaskAddCallback, (s32)Func_02000cd0, 0xc80);
        if ((s16)gGameState[112][0] == (s32)&SceneId_RamakanSabaku1) {
            Call6(Engine_MapCopyCells, 22, 7, 4, 2, 64, 126);
            Call6(Engine_MapCopyCells, 8, 10, 4, 2, 68, 126);
            Call6(Engine_MapCopyCells, 23, 21, 4, 2, 72, 126);
            Call6(Engine_MapCopyCells, 16, 42, 4, 2, 76, 126);
            Call6(Engine_MapCopyCells, 36, 44, 4, 2, 80, 126);
            Call6(Engine_MapCopyCells, 14, 55, 4, 2, 84, 126);
        } else {
            if ((s16)gGameState[112][0] != (s32)&SceneId_RamakanSabaku2) {
                goto L_020017fe;
            }
            Call6(Engine_MapCopyCells, 42, 5, 4, 2, 64, 126);
            Call6(Engine_MapCopyCells, 20, 11, 4, 2, 68, 126);
            Call6(Engine_MapCopyCells, 14, 12, 4, 2, 72, 126);
            Call6(Engine_MapCopyCells, 56, 18, 4, 2, 76, 126);
            Call6(Engine_MapCopyCells, 7, 22, 4, 2, 80, 126);
            Call6(Engine_MapCopyCells, 44, 23, 4, 2, 84, 126);
            Call6(Engine_MapCopyCells, 38, 24, 4, 2, 88, 126);
            Call6(Engine_MapCopyCells, 26, 28, 4, 2, 92, 126);
            Call6(Engine_MapCopyCells, 17, 35, 4, 2, 96, 126);
            Call6(Engine_MapCopyCells, 50, 36, 4, 2, 100, 126);
            Call6(Engine_MapCopyCells, 34, 43, 4, 2, 104, 126);
            Call6(Engine_MapCopyCells, 6, 46, 4, 2, 108, 126);
            Call6(Engine_MapCopyCells, 27, 55, 4, 2, 112, 126);
            Call6(Engine_MapCopyCells, 43, 56, 4, 2, 116, 126);
        }
        goto L_02001842;
        L_020017fe:;
        if ((s16)gGameState[112][0] == (s32)&SceneId_RamakanSabaku3) {
            v5 = 124;
            BattleFx_SetQueuedSoundAndPlay(169);
            Call6(Engine_MapCopyCells, 8, 14, 4, 4, 64, v5);
            Call6(Engine_MapCopyCells, 6, 18, 4, 4, 68, v5);
            Call6(Engine_MapCopyCells, 10, 21, 4, 4, 72, v5);
        }
        L_02001842:;
        RamakanSabaku_ApplyEntryState();
    }
    return 0;
}
