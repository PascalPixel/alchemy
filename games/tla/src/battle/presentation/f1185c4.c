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

struct SnapshotUnit *Unit_Get(s32 unitId);
u32 BattleParty_IsUnitListed(u32 id);
void Unit_SetHpFar(struct SnapshotUnit *unit, s32 hp);
void Unit_SetPpFar(struct SnapshotUnit *unit, s32 pp);

/// Restores the five units Func_0811843c snapshotted: for each one still in
/// the battle party and listed, the first three's four tag bytes and each
/// one's 25 snapshot bytes, and the last two's HP and PP through
/// Unit_SetHpFar and Unit_SetPpFar. PP is read through the global, which
/// reloads the base as the native does.
void Func_081185c4(void) {
  s32 i;
  s32 j;
  struct Snapshot *snap = &Data_0200ff58;
  u8 (*copies)[28] = Data_0200ff6c;

  for (i = 0; i <= 4; i++, copies++) {
    struct SnapshotUnit *unit = Unit_Get(Data_0812a16c[i]);

    if (BattleParty_IsUnitListed(Data_0812a16c[i]) && snap->listed[i] != 0) {
      if (i <= 2) {
        for (j = 0; j < 4; j++)
          unit->tags[j] = snap->tags[i][j];
      }
      unit->snapshot[0] = (*copies)[0];
      unit->snapshot[1] = (*copies)[1];
      unit->snapshot[2] = (*copies)[2];
      unit->snapshot[3] = (*copies)[3];
      unit->snapshot[4] = (*copies)[4];
      unit->snapshot[5] = (*copies)[5];
      unit->snapshot[6] = (*copies)[6];
      unit->snapshot[7] = (*copies)[7];
      unit->snapshot[8] = (*copies)[8];
      unit->snapshot[9] = (*copies)[9];
      unit->snapshot[10] = (*copies)[10];
      unit->snapshot[11] = (*copies)[11];
      unit->snapshot[12] = (*copies)[12];
      unit->snapshot[13] = (*copies)[13];
      unit->snapshot[14] = (*copies)[14];
      unit->snapshot[15] = (*copies)[15];
      unit->snapshot[16] = (*copies)[16];
      unit->snapshot[17] = (*copies)[17];
      unit->snapshot[18] = (*copies)[18];
      unit->snapshot[19] = (*copies)[19];
      unit->snapshot[20] = (*copies)[20];
      unit->snapshot[21] = (*copies)[21];
      unit->snapshot[22] = (*copies)[22];
      unit->snapshot[23] = (*copies)[23];
      unit->snapshot[24] = (*copies)[24];
      if (i > 2) {
        Unit_SetHpFar(unit, snap->vitals[i].hp);
        Unit_SetPpFar(unit, Data_0200ff58.vitals[i].pp);
      }
    }
  }
}
