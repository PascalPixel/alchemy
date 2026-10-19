#ifndef TLA_SAVE_STATE_H
#define TLA_SAVE_STATE_H

#include "types.h"
#include "static_assert.h"

/// A save file's summary for the file screen: the leader's name, level and
/// class, the area, play time, coins, Djinn and party.
struct SaveSummary {
  u8 slotHeader[16]; ///< room for the record's SaveSlotHeader
  u8 name[12];       ///< the leader's
  u8 level;          ///< the leader's
  u8 classId;        ///< the leader's
  u16 area;          ///< area name number (Scene_GetAreaNameIndex)
  u32 playTime;      ///< frames
  u32 coins;
  u8 djinn[4]; ///< the party's Djinn of each element
  s8 party[5]; ///< up to four party members, ended by -1
  s8 sendFlag; ///< copied from byte 0x22f of gPartyState
  s8 flagCount; ///< how many of game flags 48 to 127 are set
  s8 hasFlag34; ///< game flag 34 is set
  u8 paletteGlow;    ///< gPartyState.paletteGlow[0]
  u8 paletteGlowSub; ///< gPartyState.paletteGlow[1]
  u16 frames; ///< low half of the build stamp saved with the game
  /// Filled on reading, from the record's next sector (offset 0x10, or 0x110
  /// for the extra summary).
  u8 unk38[4];
  u32 checksum; ///< the sum of the 968 words from gGameFlagBytes
};

/// The save work: which of the 15 flash sectors hold a record, their record
/// ids and sequence numbers, a record's three sectors and the summaries.
struct SaveWorkspace {
  u8 occupied[15];  ///< 1 for a sector holding the current copy of a record
  u8 recordId[15];  ///< each sector's record
  u16 sequence[15]; ///< each sector's sequence number
  u8 slot[0x3000];  ///< one record's sectors, read from or written to flash
  struct SaveSummary summary[3]; ///< each file's, from records 0 to 2
  struct SaveSummary extra;      ///< record 3's summary
  void (*progress)(s32 step); ///< called between sector writes, if set
  u16 sectorCount; ///< 0 when a write starts
  u16 sectorTotal; ///< 3 or 6 sectors to write
};

/// The header of a save sector in flash: its signature, the record it holds,
/// its checksum and its sequence number.
struct SaveSlotHeader {
  u8 signature[7]; ///< `CAMELOT`, unterminated
  u8 recordId;
  u16 checksum;
  u16 sequence;
  u8 reserved[4]; ///< not named by ⚓️ code
};

STATIC_ASSERT(sizeof(struct SaveSummary) == 64);
STATIC_ASSERT(offsetof(struct SaveWorkspace, sequence) == 0x1e);
STATIC_ASSERT(offsetof(struct SaveWorkspace, summary) == 0x303c);
STATIC_ASSERT(offsetof(struct SaveWorkspace, extra) == 0x30fc);
STATIC_ASSERT(sizeof(struct SaveWorkspace) == 0x3144);
STATIC_ASSERT(sizeof(struct SaveSlotHeader) == 0x10);

u32 SaveState_FindLatestSlot(s32 recordId);

#endif
