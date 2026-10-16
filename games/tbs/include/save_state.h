#ifndef GUARD_SAVE_STATE_H
#define GUARD_SAVE_STATE_H

#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "types.h"
#include "static_assert.h"
#include "dma.h"
#include "global_cells.h"
#include "fixed_math.h"
#include "flash.h"

struct SaveSlotHeader {
  u8 signature[7];
  u8 recordId;
  u16 checksum;
  u16 sequence;
  u8 reserved[4];
};

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
  u8 hasFlag20;
  u16 paletteGlow : 8;
  u16 paletteGlowSub : 8;
  u16 frames;
  u8 unk38[4];
  u32 checksum;
};

struct SaveWorkspace {
  u8 occupied[16];
  u8 recordId[16];
  u16 sequence[16];
  union {
    struct {
      struct SaveSlotHeader header;
      u8 payload[0xff0];
    } record;
    u8 bytes[0x1000];
  } slot;
  struct SaveSummary summary[3];
};

STATIC_ASSERT(sizeof(struct SaveSummary) == 64);
STATIC_ASSERT(offsetof(struct SaveSummary, level) == 0x1c);
STATIC_ASSERT(offsetof(struct SaveSummary, sendFlag) == 0x31);
STATIC_ASSERT(offsetof(struct SaveSummary, checksum) == 0x3c);
STATIC_ASSERT(offsetof(struct SaveWorkspace, slot) == 0x40);
STATIC_ASSERT(offsetof(struct SaveWorkspace, summary) == 0x1040);

STATIC_ASSERT(sizeof(struct SaveSlotHeader) == 0x10);
STATIC_ASSERT(sizeof(struct SaveWorkspace) == 0x1100);
STATIC_ASSERT(offsetof(struct SaveWorkspace, recordId) == 0x10);
STATIC_ASSERT(offsetof(struct SaveWorkspace, sequence) == 0x20);
STATIC_ASSERT(offsetof(struct SaveSummary, flagCount) == 0x32);
STATIC_ASSERT(offsetof(struct SaveSummary, unk38) == 0x38);

extern struct SaveWorkspace *gSaveWorkspace;

#define DMA3 ((volatile struct DmaRegisters *)0x040000d4)
#define DMA3_BUSY 0x80000000

#define START_DMA(source_, destination_, control_)                             \
  Dma_Set((const void *)(source_), (void *)(destination_), (control_),         \
          (volatile u32 *)0x040000d4)

static __inline__ void WaitDma(struct DmaRegisters volatile *channel) {
  while ((channel->control & DMA3_BUSY) != 0) {
  }
}

#define WAIT_DMA() WaitDma(DMA3)

extern s16 gTitleSendOptionEnabled;

#endif
