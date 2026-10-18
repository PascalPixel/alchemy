// Draft, differ 240 of 183: snapshots five fixed units (Felix, Piers, Jenna
// and two at 0x80 and 0x81, Data_0812a16c) into EWRAM: whether each is in the
// battle party, the first three's four bytes at unit+300, each one's 25
// bytes at unit+304, and the last two's HP (at least 1) and unit+58. No ☀️
// twin found. The control flow and every load and store now match in kind
// (the two separate if tests on i, the 25 byte copies written out, the pooled
// halfword 1); left is register choice throughout: the native keeps i in r7
// and i*4 in r4 (spilled over the calls), the unit in r5 and the 28-byte
// record pointer in r6, and the 25 copies' offsets cycle through three
// registers stepping by 3; here i is in r4, the unit in r6 and two offset
// registers step by 2, which shifts nearly every row. Tried: one 168-byte
// struct (196), a 25-byte struct copy (ldmia/stmia: ARM GCC rounds structs
// to words; 194), the table as const (reads it once), if/else on i (244).
#include "types.h"

struct SnapshotVitals {
  u16 hp;
  u16 unk2;
};

struct Snapshot {
  u8 listed[8];
  u8 tags[3][4];
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

  for (i = 0; i <= 4; i++) {
    struct SnapshotUnit *unit = UnitGet(Data_0812a16c[i]);

    Data_0200ff58.listed[i] = 0;
    if (BattleParty_IsUnitListed(Data_0812a16c[i])) {
      Data_0200ff58.listed[i] = 1;
      if (i > 2) {
        Data_0200ffec[i].hp = unit->hp > 0 ? unit->hp : 1;
        Data_0200ffec[i].unk2 = unit->unk3a;
      }
      if (i <= 2) {
        for (j = 0; j <= 3; j++)
          Data_0200ff58.tags[i][j] = unit->tags[j];
      }
      Data_0200ff6c[i][0] = unit->snapshot[0];
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
