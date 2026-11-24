#ifndef ALCHEMY_SAVE_H
#define ALCHEMY_SAVE_H

/// @file
/// The save in flash: its sectors, parts and summaries, and the save's state.

#include "types.h"
#include "static_assert.h"

/// The save's place in flash and its limits.
enum {
  SAVE_FLASH_TIMER = 2, ///< the hardware timer handed to the flash library
  SAVE_FLASH_IDENTIFY_ATTEMPTS = 8, ///< tries, a frame apart
  /// Flash sectors used; as a sector number, "none" in the save functions.
  SAVE_SECTOR_COUNT = 15,
  SAVE_SECTORS_PER_PART = 3, ///< a part fills three sectors in a row
  SAVE_SECTOR_SIZE = 4096,
  SAVE_PART_NONE = 16, ///< the part id of a free or invalidated sector
  SAVE_FILE_COUNT = 3, ///< the save files, parts 0 to 2
  /// The part after the files, whose summary is the extra one in the save's
  /// state.
  SAVE_PART_EXTRA = 3,
  /// The highest counter a write keeps; a copy written past it is
  /// rewritten as 1.
  SAVE_COUNTER_LIMIT = 65000,
  /// Payload bytes summed into the checksum: a part's three sectors less its
  /// header.
  SAVE_CHECKSUM_BYTES = SAVE_SECTORS_PER_PART * SAVE_SECTOR_SIZE - 16
};

/// A save file's summary for the file screen: the leader's name, level and
/// class, the area, play time, coins, Djinn and party.
struct SaveSummary {
  u8 slotHeader[16]; ///< room for the part's SaveSectorHeader
  u8 name[12];       ///< the leader's
  u8 level;          ///< the leader's
  u8 classId;        ///< the leader's
  u16 area;          ///< area name number (Area_GetNameIndex)
  u32 playTime;      ///< frames
  u32 coins;
  u8 djinn[4];         ///< the party's Djinn of each element
  s8 party[5];         ///< up to four party members, ended by -1
  s8 sendFlag;         ///< gMainRoutineState.sendFlag: the file is cleared
  s8 djinnCount;       ///< the Djinn found: their flags from FLAG_DJINN_FIRST
  s8 reunited;         ///< FLAG_REUNION is set
  u8 windowHue;        ///< gMainRoutineState.windowHue
  u8 windowBrightness; ///< gMainRoutineState.windowBrightness
  u16 buildStamp;      ///< low half of the build stamp saved with the game
  /// The play time saved at the start of the file's next sector (offset 0x10,
  /// or 0x110 for the extra summary), filled on reading; a file whose
  /// playTime differs from it was not written whole.
  u32 secondPartPlayTime;
  u32 checksum; ///< the sum of the 968 words from gGameFlagBytes
};

/// The save's state: which of the SAVE_SECTOR_COUNT flash sectors hold a part,
/// their part ids and counters, one part's three sectors and the summaries.
struct SaveWorkspace {
  u8 occupied[SAVE_SECTOR_COUNT]; ///< 1 for a sector holding the current copy
                                  ///< of a part
  u8 partId[SAVE_SECTOR_COUNT];   ///< each sector's part
  u16 counter[SAVE_SECTOR_COUNT]; ///< each sector's counter
  u8 slot[SAVE_SECTORS_PER_PART *
          SAVE_SECTOR_SIZE]; ///< one part's sectors, read from or written to
                             ///< flash
  struct SaveSummary
      summary[SAVE_FILE_COUNT]; ///< each file's, from parts 0 to 2
  struct SaveSummary extra;     ///< part 3's summary
  void (*progress)(s32 step);   ///< called between sector writes, if set
  u16 sectorCount;              ///< 0 when a write starts
  /// Sectors to write: a part's, and as many again when an old copy is
  /// invalidated.
  u16 sectorTotal;
};

/// The header of a save sector in flash: its signature, the part it holds,
/// its checksum and its counter.
struct SaveSectorHeader {
  u8 signature[7]; ///< `CAMELOT`, unterminated
  u8 partId;
  u16 checksum;
  u16 counter;
  u8 reserved[4]; ///< no code names it
};

/// Where a save image, EWRAM from gSaveBuffer as a part holds it, keeps the
/// game flags, gMainRoutineState and gCharacterUnits. The Clear Data and the
/// data transferred from The Broken Seal come in this layout.
#define SAVE_IMAGE_GAME_FLAGS 64
#define SAVE_IMAGE_MAIN_ROUTINE_STATE 576
#define SAVE_IMAGE_UNITS 1312
/// Where The Broken Seal's save image keeps its units (gCharacterUnits), 32
/// bytes before this game's SAVE_IMAGE_UNITS: the Clear scene copies an image
/// it receives by link from the main routine's state on that much further on,
/// its units landing on this game's.
#define BROKEN_SEAL_SAVE_IMAGE_UNITS 1280

STATIC_ASSERT(sizeof(struct SaveSummary) == 64);
STATIC_ASSERT(offsetof(struct SaveWorkspace, counter) == 30);
STATIC_ASSERT(offsetof(struct SaveWorkspace, summary) == 12348);
STATIC_ASSERT(offsetof(struct SaveWorkspace, extra) == 12540);
STATIC_ASSERT(sizeof(struct SaveWorkspace) == 12612);
STATIC_ASSERT(sizeof(struct SaveSectorHeader) == 16);

u32 Save_FindLatestSector(s32 partId);

#endif
