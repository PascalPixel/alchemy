/* Draft, not exact (2026-09-29, Mercury): score 2105 (was 2325) on the
   permuter's scorer. The debug battle menu reads its keys and the game
   state's rule byte and special halfword as named fields of gGameState;
   the ROM derives the second field's address from the first (movs #85;
   negs; add), keeps 362 in r5 across the first two flag calls and
   allocates the menu values differently. */
#include "TYPES.H"
#include "GLOBAL_CELLS.H"

extern volatile u32 gKeysHeld;
extern volatile u32 gKeysRepeat;

struct DebugBattleGameState {
    u8 unknown_000[0x1d6];
    u16 special;                    /* 0x1d6 */
    u8 unknown_1d8[0x53];
    u8 battle_rule;                 /* 0x22b */
};

extern struct DebugBattleGameState gGameState;

void GameState_InitDefaultsFar(void);
void Ui_LoadWindowGraphics(void);
void Bg0_ClearTilemap(void);
void Runtime_InitializeHeap(void);
void GameFlag_SetBitFar(s32);
void GameFlag_ClearBitFar(s32);
void Unnamed_080b5534(void);
s32 DebugParty_LoadPreset(s32);
void Battle_RunEncounter(s32);
void WaitFrames(s32);
void Resource_InitializeTable(void);
void Scheduler_ResetTaskTable(void);
void Battle_ReservedNoOp2A08(void);
void BattleUnit_Recalculate(s32);

void Unnamed_080b56e0(void)
{
    s32 held;
    s32 encounter;
    s32 preset;
    s32 loaded;

    held = 0;
    GameState_InitDefaultsFar();
    for (;;) {
        Ui_LoadWindowGraphics();
        Bg0_ClearTilemap();
        Scheduler_ResetTaskTable();
        Runtime_InitializeHeap();
        Resource_InitializeTable();
        GameFlag_SetBitFar(362);
        encounter = 257;
        if (!(gKeysHeld & 0x80)) {
            GameFlag_SetBitFar(354);
            Battle_RunEncounter(257);
            continue;
        }
        loaded = -1;
        GameFlag_ClearBitFar(362);
        preset = 0;
        for (;;) {
            GameFlag_ClearBitFar(32);
            WaitFrames(1);
            for (;;) {
                if (gKeysRepeat & 0x10)
                    encounter++;
                if (gKeysRepeat & 0x20)
                    encounter--;
                if (gKeysRepeat & 0x40)
                    encounter -= 10;
                if (gKeysRepeat & 0x80)
                    encounter += 10;
                if (gKeysRepeat & 0x100)
                    preset++;
                if (gKeysRepeat & 0x200)
                    preset--;
                if (gKeysRepeat & 1)
                    break;
                if (gKeysRepeat & 8)
                    Unnamed_080b5534();
                if (gKeysRepeat & 4)
                    Battle_ReservedNoOp2A08();
                if ((gKeysRepeat & 2) || held) {
                    held = 1;
                    gGameState.battle_rule = 5;
                }
                if (preset != loaded) {
                    GameState_InitDefaultsFar();
                    DebugParty_LoadPreset(preset);
                    loaded = preset;
                }
                WaitFrames(1);
            }
            if (gKeysHeld & 0x80)
                GameFlag_SetBitFar(364);
            BattleUnit_Recalculate(0);
            gGameState.special = 29;
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
}
