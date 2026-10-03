#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "GAME_STATE.H"
#include "RESOURCE_IDS.H"
#include "CALLBACK_SCHEDULER.H"

enum {
    KEY_A = 0x001,
    KEY_B = 0x002,
    KEY_SELECT = 0x004,
    KEY_START = 0x008,
    KEY_RIGHT = 0x010,
    KEY_LEFT = 0x020,
    KEY_UP = 0x040,
    KEY_DOWN = 0x080,
    KEY_R = 0x100,
    KEY_L = 0x200
};

extern volatile u32 gKeysHeld;
extern volatile u32 gKeysRepeat;

void GameState_InitDefaultsFar(void);
void Ui_LoadWindowGraphics(void);
void Bg0_ClearTilemap(void);
void Resource_InitializeTable(void);
void GameFlag_SetBitFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
void DebugBattle_ViewMessages(void);
s32 DebugParty_LoadPreset(s32 preset);
void Battle_RunEncounter(s32 encounter);
void Battle_ReservedNoOp2A08(void);
void Owner_RecalculateStatsFar(s32 owner);

/* The debug battle menu. Entered with DOWN held it waits on a blank screen:
 * RIGHT and LEFT step the encounter by one, UP and DOWN by ten, R and L
 * step the party preset, which is loaded as it changes, and A starts the
 * battle on the Suhara gate backdrop; B locks byte 0x22b of the game state
 * to 5 for the rest of the session. Entered without DOWN, encounter 257 runs
 * again and again. */
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
                    /* FAKEMATCH: a one-pass loop is a sched2 barrier, so the
                     * 5 is loaded before the byte's address as in the
                     * reference */
                    do {
                        gGameState.unknown_200[0x22b - 0x200] = 5;
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
            Owner_RecalculateStatsFar(0);
            gGameState.special = (u16)(s32)&ResourceId_SuharaGateBackdrop;
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
