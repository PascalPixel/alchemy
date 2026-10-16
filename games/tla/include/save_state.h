#ifndef TLA_SAVE_STATE_H
#define TLA_SAVE_STATE_H

#include "types.h"
#include "static_assert.h"

struct SaveSummary {
  u8 slotHeader[16];
  u8 name[12];
  u8 level;
  u8 classId;
  u16 area;
  u32 playTime;
  u32 coins;
  u8 djinn[4];
  s8 party[5];
  s8 sendFlag;
  s8 flagCount;
  s8 hasFlag34;
  u8 paletteGlow;
  u8 paletteGlowSub;
  u16 frames;
  u8 unk38[4];
  u32 checksum;
};

struct SaveWorkspace {
  u8 occupied[15];
  u8 recordId[15];
  u16 sequence[15];
  u8 slot[0x3000];
  struct SaveSummary summary[3];
  struct SaveSummary extra;
  void (*progress)(s32 step);
  u16 sectorCount;
  u16 sectorTotal;
};

struct SaveSlotHeader {
  u8 signature[7];
  u8 recordId;
  u16 checksum;
  u16 sequence;
  u8 reserved[4];
};

STATIC_ASSERT(sizeof(struct SaveSummary) == 64);
STATIC_ASSERT(offsetof(struct SaveWorkspace, sequence) == 0x1e);
STATIC_ASSERT(offsetof(struct SaveWorkspace, summary) == 0x303c);
STATIC_ASSERT(offsetof(struct SaveWorkspace, extra) == 0x30fc);
STATIC_ASSERT(sizeof(struct SaveWorkspace) == 0x3144);
STATIC_ASSERT(sizeof(struct SaveSlotHeader) == 0x10);

u32 SaveState_FindLatestSlot(s32 recordId);

#endif
