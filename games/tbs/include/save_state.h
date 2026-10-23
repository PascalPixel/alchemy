#ifndef GUARD_SAVE_STATE_H
#define GUARD_SAVE_STATE_H

/// @file
/// The save workspace: slot headers and summaries.

#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "types.h"
#include "static_assert.h"
#include "dma.h"
#include "global_cells.h"
#include "fixed_math.h"
#include "flash.h"

/// The header of a save sector in flash: its signature, the record it holds,
/// its checksum and its sequence number.
struct SaveSlotHeader {
  u8 signature[7]; ///< `CAMELOT`, unterminated
  /// File n's records are n and n + 3; SAVE_RECORD_NONE (runtime/save.c) marks
  /// a free sector.
  u8 recordId;
  u16 checksum;   ///< low half of the payload's checksum
  u16 sequence;   ///< the highest is the current copy; restarts after 65000
  u8 reserved[4]; ///< not named by TBS code
};

/// A save file's summary for the file screen: the leader's name, level and
/// class, the area, play time, coins, Djinn and party.
struct SaveSummary {
  u8 slotHeader[16]; ///< room for the record's SaveSlotHeader
  u8 name[12];       ///< the leader's
  u8 level;          ///< the leader's; 0 for an empty file
  u8 classId;        ///< the leader's
  u16 area;          ///< area name number (MapSec_GetNameIndex)
  u32 playTime;      ///< frames
  u32 coins;
  u8 djinn[4]; ///< the party's Djinn of each element
  s8 party[5]; ///< up to four party members, ended by -1
  /// Copied from GameState.unk20f, which saving at the end of the game sets;
  /// the file menu for cleared data lists a file without it as not cleared.
  s8 sendFlag;
  s8 flagCount; ///< how many Djinn flags (FLAG_DJINN_FIRST on) are set
  u8 hasFlag20; ///< FLAG_CHILDHOOD is set; remaps the party's sprites
  /// bits: 15..8 paletteGlowSub | 7..0 paletteGlow
  ///@{
  u16 paletteGlow : 8;    ///< GameState.paletteGlow[0]
  u16 paletteGlowSub : 8; ///< GameState.paletteGlow[1]
  ///@}
  /// Low half of GameState.buildStamp; a file below the current stamp is
  /// listed as from an older build.
  u16 frames;
  /// Filled on reading: the play time from the file's second record. A file
  /// whose playTime differs is listed as corrupted.
  u8 unk38[4];
  u32 checksum; ///< the sum of the 968 words from gGameFlagBytes
};

/// The save work: which of the 16 flash sectors hold a record, their record
/// ids and sequence numbers, one sector's buffer and the three files'
/// summaries.
///
/// It lives in heap slot HEAP_SLOT_SAVE (gSaveWorkspace) only around each
/// save, load, copy or delete and each scan of the files, as the title menu's
/// (SaveState_ScanRecordFlags): SaveState_InitializeWorkspace takes it and
/// scans the sectors into it, and SaveState_ReleaseWorkspace frees it.
struct SaveWorkspace {
  u8 occupied[16];  ///< 1 for a sector holding the current copy of a record
  u8 recordId[16];  ///< each sector's record; 16 for none
  u16 sequence[16]; ///< each sector's sequence number
  union {
    struct {
      struct SaveSlotHeader header;
      u8 payload[0xff0];
    } record;
    u8 bytes[0x1000];
  } slot;                        ///< one sector, read from or written to flash
  struct SaveSummary summary[3]; ///< each file's, read from its first record
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

/// The save work, heap slot HEAP_SLOT_SAVE under its own name; null between
/// save operations.
extern struct SaveWorkspace *gSaveWorkspace;

/// DMA 3's enable bit in its count and control word, which the hardware
/// clears when the transfer is done.
#define DMA3_BUSY 0x80000000

/// Starts a DMA 3 transfer; control_ is the unit count with the DMA_ flags
/// << 16.
#define START_DMA(source_, destination_, control_)                             \
  Dma_Set((const void *)(source_), (void *)(destination_), (control_), REG_DMA3)

static __inline__ void WaitDma(struct DmaRegisters volatile *channel) {
  while ((channel->control & DMA3_BUSY) != 0) {
  }
}

/// Waits until a START_DMA transfer is done.
#define WAIT_DMA() WaitDma(DMA3)

/// Nonzero offers Send on the title menu: SaveState_ScanRecordFlags sets it
/// when a file has its send flag set and R and Left are held.
extern s16 gTitleSendOptionEnabled;

#endif
