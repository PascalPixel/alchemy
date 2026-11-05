// Near miss, differ 86 of 183 (tla-en, 2026-11-05), with untagged empty asms, one-pass
// blocks and locals found by search; each needs its S2 tag on adoption, measured then.
// Draft, differ 121 of 183, skeleton gap 28 (tla-en, 2026-11-01): snapshots
// five fixed units (Felix, Piers, Jenna and two at 0x80 and 0x81,
// Data_0812a16c) into EWRAM: whether each is in the battle party, the first
// three's four bytes at unit+300, each one's 25 bytes at unit+304, and the
// last two's HP (at least 1) and unit+58. No ☀️ twin found. The snapshot, the
// copies and the id table read through local pointers keep them in r8, r6 and
// r9 as natively; HP goes through the snapshot and PP through Data_0200ffec.
//
// Left: one cause, reload's spill set. The statements, loops and pseudos line
// up; every remaining row is a reload register. The native's rotation runs
// over r0, r1, r2, r3, ours over r0, r2, r3 (greg "Using reg" never says 1),
// which is why the 25 offsets cycle through three registers stepping by 3
// natively and two stepping by 2 here, and why the prologue, r9 and r8 reloads
// differ. r1 can only join the set at an insn needing a reload with r3 and r2
// busy and r1 free. The one such place is `adds r3, r4, r0` for PP: r3 its
// output, r2 the unit+58 value. So the native's `ldr r0` of Data_0200ffec is a
// reload of a constant, not a pseudo: that symbol has no hard register there.
// Here it is pseudo 57, allocated to r3.
//
// Tried for that: a set-once local at the top or at the top of the loop body
// (takes r11, push list grows; 144); in the `if (i > 2)` block or just before
// it (136: live across the ldrsh with all four low registers busy, reload must
// evict one for the scratch, and that is the mechanism wanted, but pseudo costs
// tie at 4 and the tie takes r3, so the raw HP goes to ip); set after the
// ternary (121, no change). What would close it: the local at cost below 4, or
// the raw HP above 4. s16 vitals.hp makes expand's "(A op 0) ? A : B" path load
// HP first into one pseudo, cost 6, but loses `mov r1, sl` before the branch and
// gives `cmp #1; bge` and a threaded second `i` test (149). Reversed or `< 1`
// ternary, if/else (170), a local s16 or u16 HP (140), (s16) cast on a u16 field
// (136), Data_0200ff58.vitals[i].unk2 (135) and no snapshot local (138) all lost.
// Second round, same day, re-read against reload1.c and local-alloc.c: the
// native needs all four of r0 to r3 in reload's spill set, and find_reg takes a
// new one only where every earlier register is busy. r0 comes from the ldrsh
// (r3, r2 and the offset in r1 busy); r1 needs a second site with r3 and r2
// busy and r1 free. Evicting the local at the ldrsh cannot work: spill cost is
// REG_N_REFS, the local, raw HP and the offset all cost 4, ties go r3, r2, r1,
// so raw HP goes first; below 4 needs a set outside the loop, which takes r11.
// Other sites measured: s32 hp = unit->hp first frees r1 at the ldrsh, so r1
// joins but r0 drops out (135, spill set 1 2 3); with the local before it the
// local really lands in r0, the native's `ldr r0` and strh order, but r0 is
// then no spill register (141). Source live at the tag loop's first add would
// also do, needing the source set first and in r2: hand-stepped pointers set it
// first but put source in r1 and destination in r2, whatever the declaration
// order (139); `*src++` with an indexed destination the same (139); u8 *src
// read by index is propagated away (121); a tag pointer in the loop body the
// same (121), set before the loop 152. Also lost: u16 value (155), a pointer to
// the vitals entry (138), ((u16 *)Data_0200ffec)[i * 2 + 1] (130). No device
// was in the draft to remove, and none was added.
// Earlier: one 168-byte struct (196), a 25-byte struct copy (ldmia/stmia: agscc
// rounds structs to words; 194), the table as const, if/else on i (244).
#include "types.h"

struct SnapshotVitals {
  u16 hp;
  u16 unk2;
};

struct Snapshot {
  u8 listed[8];
  u8 tags[3][4];
  u8 unk14[0x80];
  struct SnapshotVitals vitals[5];
};

struct SnapshotUnit {
  u8 unk00[0x38];
  s16 hp;
  u16 unk3a;
  u8 unk3c[0xf0];
  u8 tags[4];
  u8 snapshot[25];
};

extern struct Snapshot Data_0200ff58;
extern u8 Data_0200ff6c[][28];
extern struct SnapshotVitals Data_0200ffec[];
extern u8 Data_0812a16c[];

struct SnapshotUnit *Unit_Get(s32 unitId);
u32 BattleParty_IsUnitListed(u32 id);

void Func_0811843c(void) {
  s32 i;
  register s32 j asm("r8");
  struct Snapshot *snap = &Data_0200ff58;
  u8 (*copies)[28] = Data_0200ff6c;
  register u8 *ids asm("r8") = Data_0812a16c;

  for (i = 0; i <= 4; i++) {
    struct SnapshotUnit *unit = Unit_Get(ids[i]);

    snap->listed[i] = 0;
    if (BattleParty_IsUnitListed(ids[i])) {
      snap->listed[i] = 1;
      if (i > 2) {
        snap->vitals[i].hp = unit->hp > 0 ? unit->hp : 1;
        Data_0200ffec[i].unk2 = unit->unk3a;
      }
  asm("");
      if (i <= 2) {
        for (j = 0; j <= 3; j++)
          snap->tags[i][j] = unit->tags[j];
      }
      copies[i][0] = unit->snapshot[0];
      copies[i][1] = unit->snapshot[1];
      copies[i][2] = unit->snapshot[2];
      copies[i][3] = unit->snapshot[3];
      copies[i][4] = unit->snapshot[4];
      copies[i][5] = unit->snapshot[5];
      copies[i][6] = unit->snapshot[6];
      copies[i][7] = unit->snapshot[7];
      copies[i][8] = unit->snapshot[8];
      copies[i][9] = unit->snapshot[9];
      copies[i][10] = unit->snapshot[10];
      copies[i][11] = unit->snapshot[11];
      copies[i][12] = unit->snapshot[12];
      copies[i][13] = unit->snapshot[13];
      copies[i][14] = unit->snapshot[14];
      copies[i][15] = unit->snapshot[15];
      copies[i][16] = unit->snapshot[16];
      copies[i][17] = unit->snapshot[17];
      copies[i][18] = unit->snapshot[18];
      copies[i][19] = unit->snapshot[19];
      copies[i][20] = unit->snapshot[20];
      copies[i][21] = unit->snapshot[21];
      copies[i][22] = unit->snapshot[22];
      copies[i][23] = unit->snapshot[23];
      copies[i][24] = unit->snapshot[24];
    }
  }
}
