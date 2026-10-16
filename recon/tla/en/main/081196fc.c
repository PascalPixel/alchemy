#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "mem.h"

struct SaveSlotRecord {
  u8 unk00[0x1c];
  u8 used;
  u8 unk1d[0x23];
};

struct SaveWork {
  u8 unk0000[0x303c];
  struct SaveSlotRecord slots[3];
};

void BattleCamera_Create(void) {
  s32 *camera = Runtime_AllocateBlock(48, 0x4c);

  camera[3] = 0;
  camera[4] = 0x400000;
  camera[5] = 0;
  camera[1] = 0x5a0000;
  ((s16 *)camera)[27] = 0x2800;
  ((s16 *)camera)[26] = 0x5000;
  camera[0] = 0;
  camera[2] = 0x400000;
  camera[8] = 0x1000000;
}
