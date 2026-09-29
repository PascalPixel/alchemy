/*
 * main:0809b5dc BattleFx_UpdatePairedArcSpawner - draft; the range links as
 * disassembly (recon/tbs/raw/0809b5dc.s).
 *
 * Remaining: the reference loads the 1 it compares the battle mode with from
 * the literal pool and compares two registers. Written as == 1 below, GCC
 * compares with an immediate and interleaves the two field addresses at the
 * head (mov r1, r5 before the field64 load, zero offset in r2), so the head
 * and the compare differ and the pool word goes. The unit matched only while
 * the 1 was a link-time symbol named after its own value.
 * 2026-09-29 alchemy permute (seed 1, 4 jobs, 10 minutes): 32,803
 * candidates, none below the draft's score 300 (2 register-only, 3 operand,
 * 2 reordered, 1 deleted), 23,994 level with it. The pool word 1 is the
 * same in all six editions. A one-case switch, a 1LL, u32 or pointer
 * comparison and a ?: divisor all compare with an immediate instead; GCC
 * only loads a small number from the pool when it is a link-time symbol, so
 * this draft stays until the 1 has a real name.
 */
#include "TYPES.H"
#include "FIXED_MATH.H"

struct BattleEffect16GlobalState {
    u8 unknown_000[0x1DA];
    s16 value_1da;
    u8 unknown_1dc[0x18];
    u32 active_object_id;
};

extern struct BattleEffect16GlobalState gGameState;
void BattleFx_SpawnDescendingArcParticles(void *);

void BattleFx_UpdatePairedArcSpawner(void *arg0)
{
    s16 field64;
    s16 counter;

    field64 = *(s16 *)((u8 *)arg0 + 0x64);
    counter = (*(u16 *)((u8 *)arg0 + 0x66))++;

    if (gGameState.value_1da == 1) {
        if (Math_Mod(counter, 7) == 0)
            BattleFx_SpawnDescendingArcParticles(arg0);
    } else if (Math_Mod(counter, 5) == 0) {
        BattleFx_SpawnDescendingArcParticles(arg0);
    }

    if (field64 == 1)
        *(u16 *)((u8 *)arg0 + 6) += 0xC00;
}
