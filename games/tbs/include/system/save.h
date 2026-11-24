#ifndef ALCHEMY_SAVE_H
#define ALCHEMY_SAVE_H

/// @file
/// The save state: sector headers and file summaries.

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "gamepak/flash.h"
#include "system/dma.h"
#include "system/heap.h"
#include "system/heap_slots.h"
#include "window/screens.h"
#include "window/window.h"

/// The save files; file n is parts n and n + SAVE_FILE_COUNT.
#define SAVE_FILE_COUNT 3

/// The header of a save sector in flash: its signature, the part it holds,
/// its checksum and its counter.
struct SaveSectorHeader {
  u8 signature[7]; ///< `CAMELOT`, unterminated
  /// File n's parts are n and n + SAVE_FILE_COUNT; SAVE_PART_NONE
  /// (system/system_save.c) marks a free sector.
  u8 partId;
  u16 checksum; ///< low half of the payload's checksum
  u16 counter;  ///< the highest is the current copy; restarts after 65000
  u8 reserved[4];
};

/// A save file's summary for the file list: the leader's name, level and
/// class, the area, play time, coins, Djinn and party.
struct SaveFileSummary {
  u8 slotHeader[16]; ///< room for the part's SaveSectorHeader
  u8 name[12];       ///< the leader's
  u8 level;          ///< the leader's; 0 for an empty file
  u8 classId;        ///< the leader's
  u16 area;          ///< area name number (Area_GetNameIndex)
  u32 playTime;      ///< frames
  u32 coins;
  u8 djinn[4]; ///< the party's Djinn of each element
  s8 party[5]; ///< up to four party members, ended by -1
  /// Copied from MainRoutineState.sendFlag, which saving at the end of the
  /// game sets; the file list for cleared data lists a file without it as not
  /// cleared.
  s8 sendFlag;
  s8 flagCount; ///< how many Djinn flags (FLAG_DJINN_FIRST on) are set
  u8 childhood; ///< FLAG_CHILDHOOD is set; remaps the party's sprites
  /// bits: 15..8 windowBrightness | 7..0 windowHue
  ///@{
  u16 windowHue : 8;        ///< MainRoutineState.windowHue
  u16 windowBrightness : 8; ///< MainRoutineState.windowBrightness
  ///@}
  /// Low half of MainRoutineState.buildStamp; a file below the current stamp
  /// is listed as from an older build.
  u16 frames;
  /// Filled on reading: the play time from the file's second part. A file
  /// whose playTime differs is listed as corrupted.
  u32 secondPartPlayTime;
  u32 checksum; ///< the sum of the 968 words from gGameFlagBytes
};

/// The save state: which of the 16 flash sectors hold a part, their part ids
/// and counters, one sector's buffer and the three files' summaries.
struct SaveState {
  u8 occupied[16]; ///< 1 for a sector holding the current copy of a part
  u8 partId[16];   ///< each sector's part; 16 for none
  u16 counter[16]; ///< each sector's counter
  union {
    struct {
      struct SaveSectorHeader header;
      u8 payload[4080];
    } part;
    u8 bytes[4096];
  } slot; ///< one sector, read from or written to flash
  /// Each file's summary, read from its first part.
  struct SaveFileSummary summary[SAVE_FILE_COUNT];
};

STATIC_ASSERT(sizeof(struct SaveFileSummary) == 64);
STATIC_ASSERT(offsetof(struct SaveFileSummary, level) == 28);
STATIC_ASSERT(offsetof(struct SaveFileSummary, sendFlag) == 49);
STATIC_ASSERT(offsetof(struct SaveFileSummary, checksum) == 60);
STATIC_ASSERT(offsetof(struct SaveState, slot) == 64);
STATIC_ASSERT(offsetof(struct SaveState, summary) == 4160);

STATIC_ASSERT(sizeof(struct SaveSectorHeader) == 16);
STATIC_ASSERT(sizeof(struct SaveState) == 4352);
STATIC_ASSERT(offsetof(struct SaveState, partId) == 16);
STATIC_ASSERT(offsetof(struct SaveState, counter) == 32);
STATIC_ASSERT(offsetof(struct SaveFileSummary, flagCount) == 50);
STATIC_ASSERT(offsetof(struct SaveFileSummary, secondPartPlayTime) == 56);

/// The save state, heap slot HEAP_SLOT_SAVE under its own name; null between
/// save operations.
#define gSaveState (*(struct SaveState **)&gHeapSlots[HEAP_SLOT_SAVE])

/// The word, counted from gSceneLocalState (system/ewram.s), which opens a
/// file's second part, that holds the second copy of the play time
/// (Save_BuildFileSummary): gInventorySnapshot's last, past gSceneLocalState.
#define SECOND_PART_PLAY_TIME_WORD 64

/// Starts a DMA 3 transfer; control_ is the unit count with the DMA_ flags
/// << 16.
#define START_DMA(source_, destination_, control_)                             \
  Dma_Set((const void *)(source_), (void *)(destination_), (control_), REG_DMA3)

/// Waits until `channel`'s transfer is done.
static __inline__ void WaitDma(struct DmaRegisters volatile *channel) {
  while ((channel->control & DMA_BUSY) != 0) {
  }
}

/// Waits until a START_DMA transfer is done.
#define WAIT_DMA() WaitDma(DMA3)

/// Nonzero offers Send on the File Screen: Save_ScanFileSummaries sets it
/// when a file has its send flag set and R and Left are held.
extern s16 gTitleSendOptionEnabled;

#endif
