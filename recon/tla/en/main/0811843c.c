// Differ 30 of 183 (tla-en, 2026-11-08, -mtune=gba).
// 2026-11-08, second pass: the loop step order is the one stable gap (ours steps
// the copies, tags, vitals offsets; the native the reverse). Declaration orders,
// unit declared outside the loop, i < 5, != 0, zero set elsewhere or in a once-only
// block, the tag loop with its steps as statements or in the for list: 30 to 32;
// PP as snap->vitals 61, HP through Data_0200ffec 161, the id table indexed 227.
// 2026-11-08: the pin re-measured (141 without); the destination set before the
// source, in any spelling, 125; zero set elsewhere, i < 5, >= 1 and a local
// snapshot pointer 30 to 32, a local copies pointer 34. A 10-minute permute found
// nothing under 30.
// 2026-11-07: reading the first snapshot byte through a local holding 0 puts 304 in r0 as a
// reload, so the copies rotate r0, r1, r2 as natively (88 to 30, found by the permuter; the
// copies written to Data_0200ff6c directly, 34 through the local pointer). Left: the PP store's
// Data_0200ffec still a pseudo in r3, the tag loop's destination built after its source, and
// the first copy's address folded into ldrb. A both-arms-alike `if (unit)` around the HP
// stores measured 26; not kept.
// The note before, at 88 of 183 (2026-11-06), one pin. The tag loop's source pointer, pinned to r2,
// is busy where the destination's reload of the snapshot pointer is made, so reload takes r1
// there and r1 joins the spill set, the cause named below. Left: reload's rotation over r0 to
// r3 starts one register later than the native's (the copies' offsets run r1, r2, r0 where the
// native runs r0, r1, r2), and the PP store's `ldr r0` of Data_0200ffec is still pseudo 57 in
// r3. Adding a pin of that pointer to r0 as well measured 90. Without the pin, these pointers
// measure 139; the index loop with the pinned source 97.
// The note before this one, when the draft was plain C at 121 of 183:
// Differ 121 of 183 (tla-en, 2026-11-06), plain C with no device. The 86 form before this
// declared j and ids both as asm("r8") beside an empty asm: the tag loop's j then overwrote the
// id table's pointer for the next unit, so it compiled a different program, and it is not kept.
// A correct pin (ids in r9: 127) or the empty asm alone (129) gains nothing. Left: as below,
// reload's spill set (the native's rotation runs over r0 to r3, ours over r0, r2, r3).
// The notes of that form follow.
// Differ 86 of 183 (tla-en, 2026-11-06): reload's spill set only, as below; untagged empty asm, pin and
// one-pass forms need S2 tags on adoption.
// 2026-11-05: without the pin and barrier below, allocation matches the listing wholly and 121 of 183 differ, every row reload rotation over r0, r2, r3 where the native rotates over r0 to r3: r1 must enter the spill set, as the second round below says. Not reached.
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
  s32 j;
  s32 zero;
  struct Snapshot *snap = &Data_0200ff58;
  u8 *ids = Data_0812a16c;

  for (i = 0; i <= 4; i++) {
    struct SnapshotUnit *unit = Unit_Get(ids[i]);

    snap->listed[i] = 0;
    if (BattleParty_IsUnitListed(ids[i])) {
      snap->listed[i] = 1;
      if (i > 2) {
        snap->vitals[i].hp = unit->hp > 0 ? unit->hp : 1;
        Data_0200ffec[i].unk2 = unit->unk3a;
      }
      if (i <= 2) {
        /* FAKEMATCH(pin): the form kept; as u8 *src, 141 of 183 instructions differ in tla-en. */
        register u8 *src asm("r2") = unit->tags;
        u8 *dst = snap->tags[i];

        for (j = 0; j <= 3; j++)
          *dst++ = *src++;
      }
      // zero holds 0: indexing the first source byte through it keeps 304 in a reload
      // register, as the native does (88 of 183 as snapshot[0]).
      zero = 0;
      Data_0200ff6c[i][0] = unit->snapshot[zero];
      Data_0200ff6c[i][1] = unit->snapshot[1];
      Data_0200ff6c[i][2] = unit->snapshot[2];
      Data_0200ff6c[i][3] = unit->snapshot[3];
      Data_0200ff6c[i][4] = unit->snapshot[4];
      Data_0200ff6c[i][5] = unit->snapshot[5];
      Data_0200ff6c[i][6] = unit->snapshot[6];
      Data_0200ff6c[i][7] = unit->snapshot[7];
      Data_0200ff6c[i][8] = unit->snapshot[8];
      Data_0200ff6c[i][9] = unit->snapshot[9];
      Data_0200ff6c[i][10] = unit->snapshot[10];
      Data_0200ff6c[i][11] = unit->snapshot[11];
      Data_0200ff6c[i][12] = unit->snapshot[12];
      Data_0200ff6c[i][13] = unit->snapshot[13];
      Data_0200ff6c[i][14] = unit->snapshot[14];
      Data_0200ff6c[i][15] = unit->snapshot[15];
      Data_0200ff6c[i][16] = unit->snapshot[16];
      Data_0200ff6c[i][17] = unit->snapshot[17];
      Data_0200ff6c[i][18] = unit->snapshot[18];
      Data_0200ff6c[i][19] = unit->snapshot[19];
      Data_0200ff6c[i][20] = unit->snapshot[20];
      Data_0200ff6c[i][21] = unit->snapshot[21];
      Data_0200ff6c[i][22] = unit->snapshot[22];
      Data_0200ff6c[i][23] = unit->snapshot[23];
      Data_0200ff6c[i][24] = unit->snapshot[24];
    }
  }
}
