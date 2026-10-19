// Draft, differ 123 of 183 (240 before): snapshots five fixed units (Felix,
// Piers, Jenna and two at 0x80 and 0x81, Data_0812a16c) into EWRAM: whether
// each is in the battle party, the first three's four bytes at unit+300,
// each one's 25 bytes at unit+304, and the last two's HP (at least 1) and
// unit+58. No ☀️ twin found. The snapshot, the copies and the id table read
// through local pointers keep them in r8, r6 and r9 as natively; HP goes
// through the snapshot and PP through Data_0200ffec, as the native's pool
// shows. Left: the 25 copies' offsets cycle through three registers stepping
// by 3 natively (move2add's reuse of 304, 305 and 306), two stepping by 2
// here, which shifts most of the copy rows, and three prologue rows. Tried
// before: one 168-byte struct (196), a 25-byte struct copy (ldmia/stmia:
// agscc rounds structs to words; 194), the table as const, if/else on i
// (244).
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

struct SnapshotUnit *UnitGet(s32 unitId);
u32 BattleParty_IsUnitListed(u32 id);

void Func_0811843c(void) {
  s32 i;
  s32 j;
  struct Snapshot *snap = &Data_0200ff58;
  u8 (*copies)[28] = Data_0200ff6c;
  u8 *ids = Data_0812a16c;

  for (i = 0; i <= 4; i++) {
    struct SnapshotUnit *unit = UnitGet(ids[i]);

    snap->listed[i] = 0;
    if (BattleParty_IsUnitListed(ids[i])) {
      snap->listed[i] = 1;
      if (i > 2) {
        snap->vitals[i].hp = unit->hp > 0 ? unit->hp : 1;
        Data_0200ffec[i].unk2 = unit->unk3a;
      }
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
