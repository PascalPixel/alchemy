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
