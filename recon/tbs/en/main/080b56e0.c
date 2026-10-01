/* NONMATCHING: main [080b56e0,080b5864), 388 bytes with its pool.
 * 2026-10-01 (☀️ matcher 1): rewritten from the listing as plain C; 384 of
 * 388 bytes, permuter score 260 (the earlier permuter body was 2105). The
 * debug battle menu restarts through a goto (a for (;;) lets loop.c hoist
 * 0x80 and loses the reference's 362 in r5), and the quick encounter
 * follows the held-SELECT block, whose distance gives the bne/b pair.
 * Remaining, two spots:
 * - The reference stores special = 29 from the literal pool (ldr r3, =29,
 *   an HImode constant move); here store_field expands the u16 field store
 *   as a read-modify-write that combine folds to movs r3, #29, which also
 *   drops the pool word (4 bytes). Storing through a cast pointer gives the
 *   pool load but loses the hoisted address pair (34 lines); pointer
 *   locals for the two fields give 20 to 30, s16 array views 22 to 36.
 * - sched2 puts movs r3, #5 after mov r2, r9 for the rule store; the
 *   reference has it before.
 * A 300 s permuter run (131,234 candidates) found nothing below 260. The
 * listing calls GameState_InitDefaultsFar as bl 0x08077098. */
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

/* Debug battle menu: hold SELECT to pick an encounter (left/right by one,
   up/down by ten) and a party preset (R/L) before A starts it; B locks the
   battle rule to 5. Without SELECT, encounter 257 runs at once. */
void Unnamed_080b56e0(void)
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
    if (gKeysHeld & 0x80) {
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
                if ((gKeysRepeat & 2) || locked) {
                    locked = 1;
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
    GameFlag_SetBitFar(354);
    Battle_RunEncounter(257);
    goto restart;
}
