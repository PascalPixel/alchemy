// Draft, differ 4 of 183 (tla-en, 2026-11-09); 4 in every edition
// 2026-11-09 (round 7), 8 to 4, no new device: the copies are a struct
// (SnapshotCopy, 25 bytes padded to 28). Their stores then carry that struct's
// alias set, which is disjoint from the unit's (the loads carry
// SnapshotUnit's), so sched2 sees no store-to-load dependence between copies
// and the first two strb fall behind the constants' second halves as listed
// (traced: with u8 rows, each store's priority is 3 above the next load's;
// without the dependence the constant half outranks it). Left: the PP base
// (4). Natively it is a reload at the add: its pseudo has no hard register
// and reload rematerializes the REG_EQUIV symbol, round-robin after the HP
// store's r2, into r0; find_reg would also take r1 there, which is what the
// dead test stands in for. Here the base is a local pseudo and local-alloc
// gives it r3. Measured without gain: 128 forms of the PP block (pp load,
// address, dead test and store in 8 orders; pp as u32, u16, s32, s16; the
// address as &Data_0200ffec[i], &Data_0200ff58.vitals[i], Data_0200ffec + i,
// &snap->vitals[i]): 4 at best; the store as Data_0200ff58.vitals[i].pp
// (133: CSE keeps 148 apart from the symbol); a function-level base pointer
// (142: global allocation gives it fp; with fp fixed, diagnostic only, it is
// rematerialized but the rotation is lost, 123); Unit_Get as malloc (8).
// 2026-11-09 (round 6), 12 to 8: the copies' two temporaries and the
// once-only block are gone, and the tag loop is a plain for over j. What they
// stood in for is r1 in reload's spill set (the listing gets it at the PP
// add, where r2 holds the PP value and r3 the address). Now the PP value
// waits in a local (tagged temporary) while pv = &Data_0200ffec[i] is built,
// and a tagged dead test between them (if (i == 300) j = 0;) builds 300 in a
// reload register with r2 and r3 busy: find_reg takes r1 there, the test's
// empty branch is gone after reload, and the copies rotate r0, r1, r2 as
// listed. pv as plain C is still needed (119 without). Left: the PP base is
// a pseudo in r3 where the listing reloads it into r0 above the HP strh (4),
// and the first two copies' stores come one slot early (4: ldrb then strb
// with one instruction between where the listing has two). The PP base as a
// reload is out of reach: spilled, retry_global_alloc gives it ip; hoisted,
// fp; gcse's cprop propagates only a bare SYMBOL_REF set (Thumb forces every
// symbol to the pool at expand); an inline helper, even with const
// parameters, copies its pointer argument through a pseudo (12). On this
// form: the once-only block, a first temporary (8 each); first and src back
// (10, 12); s8 snapshot or copies (8); the tag loop counted down (10).
// 2026-11-09 (later), 14 to 12, no new device: the tag loop counts j down
// from 3 in a do-while and indexes [3 - j]; j's init is then the first set of
// the block, so it holds r0 when reload rebuilds the destination's snap, and
// that reload takes the listing's r1 (traced). Left: movs r0, #3 is scheduled
// two rows early (it is first in RTL; the listing's is last, after the two
// pointers), the PP reload (4) and the second copy (6). With this tag loop the
// copies' temporaries and the once-only block are each still needed (121, 26,
// 14 without); the PP base pinned to r0 is 14; count-down *d++ = *s++ forms
// with d and s set after j: 18. Two pins settle the PP rows (8, not kept: the
// pins reach no further): pv in r0 = Data_0200ffec, then pa in r3 =
// (struct SnapshotVitals *)(i * 4 + (s32)pv) and pa->pp = unit->unk3a (pv[i],
// pv + i, i + pv or a u8 * sum put pv first in the add: 10; either pin alone
// 12 or 14). With them, src pinned to r3 in place of its temporary keeps 8
// (first's byte then takes r2); src pinned inside the once-only block, 117.
// The native's PP base is a reload at the add (r0, rotation after the HP
// store's r2); that one reload would also put r1 in reload's spill set (find_reg
// takes r1 with r3 the add's output and r2 the PP value), which is what the
// copies' temporaries stand in for. A 12-minute permute of this form (seed
// 2468): nothing below 12.
// 2026-11-09 (last stretch), 28 to 14: the permuter (seed 7) found an s32
// first holding snapshot[0] (a second tagged temporary, 28 without it), and
// (seed 31) a once-only block around the first two copies (16 to 14). Left,
// the same one cause: the PP store's Data_0200ffec should be a reload (r0,
// hoisted above the HP strh), the rotation step that sends the tag loop's
// snap reload to r1; the copies' two temporaries stand in for that step.
// The tag loop as a count-down `*dst++ = *s++` with its counter pinned to r0
// (set first, so the snap reload skips r0) gives the native mov r1, r8 but
// moves movs r0, #3, mov sl and add sl (16 to 18 on this form). Not better:
// a block or function-level PP pointer (it takes r0 or fp: 142 to 149), the
// PP pointer and address pinned (30), the tag destination spelled six ways,
// the PP base as an integer address (diagnostic only, and O2 forbids it: 18,
// CSE still loads it into a pseudo), a third permute (seed 777): nothing
// below 14. Why the PP pseudo has no register is still open: reload's
// find_reg breaks equal spill costs by REG_ALLOC_ORDER (r3, r2, r1, r0), and
// a PP pseudo live at the ldrsh's scratch costs 4 refs like the sl copy in r1.
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
struct SnapshotCopy {
  u8 bytes[25];
};

extern struct SnapshotCopy Data_0200ff6c[];
extern struct SnapshotVitals Data_0200ffec[];
extern u8 Data_0812a16c[];

struct SnapshotUnit *Unit_Get(s32 unitId);
u32 BattleParty_IsUnitListed(u32 id);

void Func_0811843c(void) {
  s32 j;
  struct Snapshot *snap = &Data_0200ff58;
  // Set here, between snap and ids: the native sets i before it loads ids.
  s32 i = 0;
  u8 *ids = Data_0812a16c;
  struct SnapshotCopy *copies = Data_0200ff6c;

  for (; i <= 4; i++, copies++) {
    struct SnapshotUnit *unit = Unit_Get(ids[i]);

    snap->listed[i] = 0;
    if (BattleParty_IsUnitListed(ids[i])) {
      snap->listed[i] = 1;
      if (i > 2) {
        snap->vitals[i].hp = unit->hp > 0 ? unit->hp : 1;
        {
          struct SnapshotVitals *pv;
          u32 pp;

          /* FAKEMATCH(temporary): the PP value held in a local while pv is
           * built; as pv->pp = unit->unk3a, 123 of 183 instructions differ
           * in tla-en. */
          pp = unit->unk3a;
          pv = &Data_0200ffec[i];
          /* FAKEMATCH(dead): a store the tag loop's init overwrites; its
           * test builds 300 in a reload register while pp and pv hold r2
           * and r3, which puts r1 among reload's registers; without it, 119
           * of 183 instructions differ in tla-en. */
          if (i == 300)
            j = 0;
          pv->pp = pp;
        }
      }
      if (i <= 2) {
        for (j = 0; j < 4; j++)
          snap->tags[i][j] = unit->tags[j];
      }
      copies->bytes[0] = unit->snapshot[0];
      copies->bytes[1] = unit->snapshot[1];
      copies->bytes[2] = unit->snapshot[2];
      copies->bytes[3] = unit->snapshot[3];
      copies->bytes[4] = unit->snapshot[4];
      copies->bytes[5] = unit->snapshot[5];
      copies->bytes[6] = unit->snapshot[6];
      copies->bytes[7] = unit->snapshot[7];
      copies->bytes[8] = unit->snapshot[8];
      copies->bytes[9] = unit->snapshot[9];
      copies->bytes[10] = unit->snapshot[10];
      copies->bytes[11] = unit->snapshot[11];
      copies->bytes[12] = unit->snapshot[12];
      copies->bytes[13] = unit->snapshot[13];
      copies->bytes[14] = unit->snapshot[14];
      copies->bytes[15] = unit->snapshot[15];
      copies->bytes[16] = unit->snapshot[16];
      copies->bytes[17] = unit->snapshot[17];
      copies->bytes[18] = unit->snapshot[18];
      copies->bytes[19] = unit->snapshot[19];
      copies->bytes[20] = unit->snapshot[20];
      copies->bytes[21] = unit->snapshot[21];
      copies->bytes[22] = unit->snapshot[22];
      copies->bytes[23] = unit->snapshot[23];
      copies->bytes[24] = unit->snapshot[24];
    }
  }
}
