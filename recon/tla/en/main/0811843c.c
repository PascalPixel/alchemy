// Draft, differ 28 of 183 (tla-en, 2026-11-08)
// 2026-11-08 (devices pass), unchanged at 28: Data_0200ffec pinned to r0 for the PP store gives the native ldr r0 above the HP strh (30), and with the PP address also pinned to r3, 26; the tag loop's snap reload then still takes r0 for r1, and pinning or a barrier on the tag destination costs 50 to 157. Two pins for two rows is no progress, so neither is kept. An empty asm on the PP address (46 to 149) or after the HP store (46) is worse.
// Written now as its matched twin Func_081185c4 (the restore) is: the copies
// pointer stepped in the for list after i (the loop end then steps i, the tag
// offset, the vitals offset, the copies, as natively), the tag loop indexed
// through both structs (its destination built before its source, snap + 4i in
// the native's operand order, no pin), and i set between snap and ids (the
// prologue's order). Plain, that form is 119: reload's spill set is r0, r2, r3.
// The one device left is the temporary below, which puts r1 in the spill set at
// the first copy; it replaces the pin of the tag source (30 before).
//
// Left, one cause: the PP store's Data_0200ffec. Natively it is a reload,
// `ldr r0` hoisted above the HP strh, and find_reg takes r1 there (r3 the add's
// output, r2 the PP value); that reload is also the rotation step that sends
// the tag loop's snap reload to r1, 304 to r0 and 305 to r1. Here it is pseudo
// 57 in r3 (local-alloc), so it stays below the strh, the tag reload takes r0
// and the copies' rotation runs one register early. Why the native's symbol
// has no hard register is open: global allocation and retry_global_alloc would
// give a spilled pseudo ip or r11, and update_equiv_regs replaces only a pseudo
// set from the symbol itself (ours is set from the pool).
//
// Measured without gain on this form: the HP clamp as "< 1 ? 1 :" or ">= 1"
// (28), as if/else (157), as a store then a fix (152); PP before HP (34); PP
// through Data_0200ff58.vitals (52), a vitals pointer in the block (145) or at
// the top (142); unit, src or j in other scopes (28); a local holding 0 or 1
// at the first copies (125, 147). A 10-minute permute of the plain form found
// only devices at the copies (30) or extra instructions.
//
// Earlier forms (2026-11-01 to 11-08): Data_0200ff6c[i] indexed copies (their
// giv was reduced last, so the loop end stepped them first), a `*dst++ =
// *src++` tag loop with src pinned to r2 (30, with a local holding 0 at the
// first copy; 141 unpinned), the copies through a local pointer in the loop
// body (83), ids as the global (223), one 168-byte struct (196), a 25-byte
// struct copy (194).
#include "types.h"

struct SnapshotVitals {
  u16 hp;
  u16 pp;
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
  s32 j;
  struct Snapshot *snap = &Data_0200ff58;
  // Set here, between snap and ids: the native sets i before it loads ids.
  s32 i = 0;
  u8 *src;
  u8 *ids = Data_0812a16c;
  u8 (*copies)[28] = Data_0200ff6c;

  for (; i <= 4; i++, copies++) {
    struct SnapshotUnit *unit = Unit_Get(ids[i]);

    snap->listed[i] = 0;
    if (BattleParty_IsUnitListed(ids[i])) {
      snap->listed[i] = 1;
      if (i > 2) {
        snap->vitals[i].hp = unit->hp > 0 ? unit->hp : 1;
        Data_0200ffec[i].pp = unit->unk3a;
      }
      if (i <= 2) {
        for (j = 0; j <= 3; j++)
          snap->tags[i][j] = unit->tags[j];
      }
      /* FAKEMATCH(temporary): the form kept; as (*copies)[1] =
       * unit->snapshot[1], 119 of 183 instructions differ in tla-en. */
      src = &unit->snapshot[1];
      (*copies)[0] = unit->snapshot[0];
      (*copies)[1] = *src;
      (*copies)[2] = unit->snapshot[2];
      (*copies)[3] = unit->snapshot[3];
      (*copies)[4] = unit->snapshot[4];
      (*copies)[5] = unit->snapshot[5];
      (*copies)[6] = unit->snapshot[6];
      (*copies)[7] = unit->snapshot[7];
      (*copies)[8] = unit->snapshot[8];
      (*copies)[9] = unit->snapshot[9];
      (*copies)[10] = unit->snapshot[10];
      (*copies)[11] = unit->snapshot[11];
      (*copies)[12] = unit->snapshot[12];
      (*copies)[13] = unit->snapshot[13];
      (*copies)[14] = unit->snapshot[14];
      (*copies)[15] = unit->snapshot[15];
      (*copies)[16] = unit->snapshot[16];
      (*copies)[17] = unit->snapshot[17];
      (*copies)[18] = unit->snapshot[18];
      (*copies)[19] = unit->snapshot[19];
      (*copies)[20] = unit->snapshot[20];
      (*copies)[21] = unit->snapshot[21];
      (*copies)[22] = unit->snapshot[22];
      (*copies)[23] = unit->snapshot[23];
      (*copies)[24] = unit->snapshot[24];
    }
  }
}
