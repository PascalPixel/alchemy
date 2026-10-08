#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "types.h"
#include "io_reg.h"
#include "system.h"
#include "game_state.h"
#include "resource_ids.h"
#include "callback_scheduler.h"

#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)

#define Battle_RunEncounter ((void (*)(s32))Battle_RunEncounter)

extern volatile u32 gKeysHeld;

s32 DebugParty_LoadPreset(s32 preset);

void DebugBattle_RunMenu(void)
{
    s32 locked = 0;
    s32 encounter;
    s32 preset;
    s32 loaded;

    GameState_InitDefaultsFar();
restart:
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    Scheduler_ResetTaskTable();
    Runtime_InitializeHeap();
    Resource_InitializeTable();
    GameFlag_SetBitFar(362);
    encounter = 257;
    if (gKeysHeld & KEY_DOWN) {
        loaded = -1;
        GameFlag_ClearBitFar(362);
        preset = 0;
        for (;;) {
            GameFlag_ClearBitFar(32);
            WaitFrames(1);
            for (;;) {
                if (gKeysRepeat & KEY_RIGHT)
                    encounter++;
                if (gKeysRepeat & KEY_LEFT)
                    encounter--;
                if (gKeysRepeat & KEY_UP)
                    encounter -= 10;
                if (gKeysRepeat & KEY_DOWN)
                    encounter += 10;
                if (gKeysRepeat & KEY_R)
                    preset++;
                if (gKeysRepeat & KEY_L)
                    preset--;
                if (gKeysRepeat & KEY_A)
                    break;
                if (gKeysRepeat & KEY_START)
                    DebugBattle_ViewMessages();
                if (gKeysRepeat & KEY_SELECT)
                    Battle_ReservedNoOp2A08();
                if ((gKeysRepeat & KEY_B) || locked) {
                    locked = 1;
                    /* FAKEMATCH: a one-pass loop is a sched2 barrier, so the 5 is loaded before the byte's address as in the reference */
                    do {
                        gGameState.battle_start = 5;
                    } while (0);
                }
                if (preset != loaded) {
                    GameState_InitDefaultsFar();
                    DebugParty_LoadPreset(preset);
                    loaded = preset;
                }
                WaitFrames(1);
            }
            if (gKeysHeld & KEY_DOWN)
                GameFlag_SetBitFar(364);
            UnitCalcFar(0);
            gGameState.special = (s32)&ResourceId_SuharaGateBackdrop;
            if (encounter == 28)
                GameFlag_SetBitFar(366);
            GameFlag_SetBitFar(354);
            Battle_RunEncounter(encounter);
            Ui_LoadWindowGraphics();
            Bg0_ClearTilemap();
            Scheduler_ResetTaskTable();
            Runtime_InitializeHeap();
            Resource_InitializeTable();
        }
    }
    GameFlag_SetBitFar(354);
    Battle_RunEncounter(257);
    goto restart;
}
