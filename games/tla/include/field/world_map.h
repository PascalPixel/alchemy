#ifndef ALCHEMY_WORLD_MAP_H
#define ALCHEMY_WORLD_MAP_H

/// @file
/// The world map screen's work and the destinations it marks.

#include "types.h"
#include "static_assert.h"

/// gWorldMapWork, the world map screen's state while it is open: its
/// resources, the cursor and the marker sprites.
struct WorldMapWork {
  u16 vramBlock;  ///< owns the marker tiles
  u16 windowSlot; ///< the resource slot of the name window's tiles
  s32 x;          ///< the cursor's screen column, 16.16 pixels
  s32 z;          ///< the cursor's screen row, 16.16 pixels
  u8 padding0c[6];
  s16 shown; ///< the FlagEntry whose name is drawn; -1 for none
  u8 padding14[4];
  s32 speed;            ///< the cursor's speed, 16.16 pixels per frame
  void *window;         ///< the name window
  u32 markers[129 * 3]; ///< the marker sprites' OAM entries
  s32 *cursor;          ///< the cursor the picture scrolls to
  u8 padding630[0x10];
  s16 centered; ///< nonzero draws resource 35 at the origin (Func_080ece20)
  u16 mode;     ///< 0 to look, nonzero to choose a destination
  s32 unk644;
  s32 unk648;
};

STATIC_ASSERT(offsetof(struct WorldMapWork, window) == 0x1c);
STATIC_ASSERT(offsetof(struct WorldMapWork, cursor) == 0x62c);
STATIC_ASSERT(offsetof(struct WorldMapWork, mode) == 0x642);
STATIC_ASSERT(offsetof(struct WorldMapWork, unk648) == 0x648);

/// A world map destination, keyed by its flag (Func_080c9de0 sets game flag
/// 0x180 + flag); FlagEntries_GetTable returns them, ended by a zero flag.
struct FlagEntry {
  u8 flag;
  u8 value;
  s16 unk02;
  s16 unk04;
  s16 unk06;
  s16 unk08;
  s16 unk0a;
  s16 id;
  s16 variant;  ///< -1 matches any
  s16 scene;    ///< where choosing it leads
  s16 entrance; ///< that scene's entrance
};

STATIC_ASSERT(offsetof(struct FlagEntry, id) == 0x0c);
STATIC_ASSERT(offsetof(struct FlagEntry, scene) == 0x10);
STATIC_ASSERT(sizeof(struct FlagEntry) == 20);

/// Data_02000500: the destination last chosen on the world map.
struct WorldMapChoice {
  s16 flag; ///< the chosen FlagEntry's flag; -1 when cancelled
};

extern struct WorldMapWork gWorldMapWork;
extern struct WorldMapChoice Data_02000500;
struct FlagEntry *FlagEntries_GetTable(void);

#endif
