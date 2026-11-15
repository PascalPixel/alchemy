#ifndef ALCHEMY_SAVE_H
#define ALCHEMY_SAVE_H

#include "types.h"
#include "static_assert.h"

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
  s8 sendFlag;         ///< copied from byte 0x22f of gMainRoutineWork
  s8 flagCount;        ///< how many of game flags 48 to 127 are set
  s8 reunited;         ///< game flag 34 is set
  u8 windowHue;        ///< gMainRoutineWork.windowHue
  u8 windowBrightness; ///< gMainRoutineWork.windowBrightness
  u16 frames;          ///< low half of the build stamp saved with the game
  /// The play time saved at the start of the file's next sector (offset 0x10,
  /// or 0x110 for the extra summary), filled on reading; a file whose
  /// playTime differs from it was not written whole.
  u32 secondPartPlayTime;
  u32 checksum; ///< the sum of the 968 words from gGameFlagBytes
};

/// The save work: which of the 15 flash sectors hold a part, their part
/// ids and counters, one part's three sectors and the summaries.
struct SaveWorkspace {
  u8 occupied[15]; ///< 1 for a sector holding the current copy of a part
  u8 partId[15];   ///< each sector's part
  u16 counter[15]; ///< each sector's counter
  u8 slot[0x3000]; ///< one part's sectors, read from or written to flash
  struct SaveSummary summary[3]; ///< each file's, from parts 0 to 2
  struct SaveSummary extra;      ///< part 3's summary
  void (*progress)(s32 step);    ///< called between sector writes, if set
  u16 sectorCount;               ///< 0 when a write starts
  u16 sectorTotal;               ///< 3 or 6 sectors to write
};

/// The header of a save sector in flash: its signature, the part it holds,
/// its checksum and its counter.
struct SaveSectorHeader {
  u8 signature[7]; ///< `CAMELOT`, unterminated
  u8 partId;
  u16 checksum;
  u16 counter;
  u8 reserved[4]; ///< not named by ⚓️ code
};

/// Where a save image, EWRAM from gSaveBuffer as a part holds it, keeps the
/// game flags, gMainRoutineWork and gCharacterUnits. The Clear Data and the
/// data transferred from The Broken Seal come in this layout.
#define SAVE_IMAGE_GAME_FLAGS 0x40
#define SAVE_IMAGE_MAIN_ROUTINE_WORK 0x240
#define SAVE_IMAGE_UNITS 0x520

STATIC_ASSERT(sizeof(struct SaveSummary) == 64);
STATIC_ASSERT(offsetof(struct SaveWorkspace, counter) == 0x1e);
STATIC_ASSERT(offsetof(struct SaveWorkspace, summary) == 0x303c);
STATIC_ASSERT(offsetof(struct SaveWorkspace, extra) == 0x30fc);
STATIC_ASSERT(sizeof(struct SaveWorkspace) == 0x3144);
STATIC_ASSERT(sizeof(struct SaveSectorHeader) == 0x10);

u32 Save_FindLatestSector(s32 partId);

#endif
