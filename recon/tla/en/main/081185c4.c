// Draft, differ 113 of 176: restores the five units Func_0811843c
// snapshotted: for each one still in the battle party and listed, the first
// three's four bytes at unit+300, the 25 bytes at unit+304, and the last two's
// HP and PP through Unit_SetHpFar and Unit_SetPpFar. Left: other: the native
// keeps the snapshot's base in r9 for the whole loop with i*4 as its own giv
// in r10 (the tag copy's source is base + i*4 + 8), and reloads the base from
// the pool only for PP; here the base is reloaded at each use and i*4 is
// recomputed, so r9 and r10 are not saved. The 25 copies differ only in
// where each adds r1, #1 sits (one slot before its load here, after it
// natively). Tried: the tag copy indexed (137) and as a pointer countdown
// (113).
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
  u8 unk00[0x12c];
  u8 tags[4];
  u8 snapshot[25];
};

extern struct Snapshot Data_0200ff58;
extern u8 Data_0200ff6c[][28];
extern u8 Data_0812a16c[];

struct SnapshotUnit *UnitGet(s32 unitId);
u32 BattleParty_IsUnitListed(u32 id);
void Unit_SetHpFar(struct SnapshotUnit *unit, s32 hp);
void Unit_SetPpFar(struct SnapshotUnit *unit, s32 pp);

void Func_081185c4(void) {
  s32 i;
  s32 j;

  for (i = 0; i <= 4; i++) {
    struct SnapshotUnit *unit = UnitGet(Data_0812a16c[i]);

    if (BattleParty_IsUnitListed(Data_0812a16c[i]) &&
        Data_0200ff58.listed[i] != 0) {
      if (i <= 2) {
        u8 *dst = unit->tags;
        u8 *src = Data_0200ff58.tags[i];

        for (j = 3; j >= 0; j--)
          *dst++ = *src++;
      }
      unit->snapshot[0] = Data_0200ff6c[i][0];
      unit->snapshot[1] = Data_0200ff6c[i][1];
      unit->snapshot[2] = Data_0200ff6c[i][2];
      unit->snapshot[3] = Data_0200ff6c[i][3];
      unit->snapshot[4] = Data_0200ff6c[i][4];
      unit->snapshot[5] = Data_0200ff6c[i][5];
      unit->snapshot[6] = Data_0200ff6c[i][6];
      unit->snapshot[7] = Data_0200ff6c[i][7];
      unit->snapshot[8] = Data_0200ff6c[i][8];
      unit->snapshot[9] = Data_0200ff6c[i][9];
      unit->snapshot[10] = Data_0200ff6c[i][10];
      unit->snapshot[11] = Data_0200ff6c[i][11];
      unit->snapshot[12] = Data_0200ff6c[i][12];
      unit->snapshot[13] = Data_0200ff6c[i][13];
      unit->snapshot[14] = Data_0200ff6c[i][14];
      unit->snapshot[15] = Data_0200ff6c[i][15];
      unit->snapshot[16] = Data_0200ff6c[i][16];
      unit->snapshot[17] = Data_0200ff6c[i][17];
      unit->snapshot[18] = Data_0200ff6c[i][18];
      unit->snapshot[19] = Data_0200ff6c[i][19];
      unit->snapshot[20] = Data_0200ff6c[i][20];
      unit->snapshot[21] = Data_0200ff6c[i][21];
      unit->snapshot[22] = Data_0200ff6c[i][22];
      unit->snapshot[23] = Data_0200ff6c[i][23];
      unit->snapshot[24] = Data_0200ff6c[i][24];
      if (i > 2) {
        Unit_SetHpFar(unit, Data_0200ff58.vitals[i].hp);
        Unit_SetPpFar(unit, Data_0200ff58.vitals[i].pp);
      }
    }
  }
}
