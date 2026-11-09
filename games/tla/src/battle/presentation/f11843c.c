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
          /* FAKEMATCH(pin): the PP base held in r0; as a plain local, 4 of
           * 183 instructions differ in tla-en. */
          register struct SnapshotVitals *base asm("r0");
          u32 pp;

          /* FAKEMATCH(temporary): the PP value held in a local while pv is
           * built; as pv->pp = unit->unk3a, 121 of 183 instructions differ
           * in tla-en. */
          pp = unit->unk3a;
          base = Data_0200ffec;
          // The address as an integer sum, i * 4 first: &base[i] puts the
          // base first in the add.
          pv = (struct SnapshotVitals *)(i * 4 + (s32)base);
          /* FAKEMATCH(dead): a store the tag loop's init overwrites; its
           * test builds 300 in a reload register while pp and pv hold r2
           * and r3, which puts r1 among reload's registers; without it, 121
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
