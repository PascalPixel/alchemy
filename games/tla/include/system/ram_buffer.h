#ifndef ALCHEMY_RAM_BUFFER_H
#define ALCHEMY_RAM_BUFFER_H

#include "system/heap.h"
#include "types.h"

/// EWRAM's buffers at fixed addresses: the map's cells, the decode buffer, the
/// map's collision shapes, its scene points and the BG tiles.
#define Ram_MapCellBuffer FIXED_ADDRESS((u8 *), 0x02010000, gMapGridCellBuffer)
#define Ram_DecodeBuffer FIXED_ADDRESS((u8 *), 0x0201c000, gDecodeBuffer)
#define Ram_MapCollision FIXED_ADDRESS((u8 *), 0x0202c000, gMapGridShapes)
#define Ram_MapScenePoints FIXED_ADDRESS((u8 *), 0x0202e008, gMapScenePoints)
#define Ram_BgTileBuffer FIXED_ADDRESS((u8 *), 0x02038000, gBgTileBuffer)
/// The cell picture table: each cell picture's top two tile entries, then its
/// bottom two. MapGrid_RenderCellPictureRow reads it at constant addresses
/// inside its loop; as the symbol the two table addresses hoist out of the
/// loop.
#define Ram_MapBlocks FIXED_ADDRESS((u8 *), 0x02020000, gCellPictures)
#define Ram_MapShapeGrid FIXED_ADDRESS((u8 *), 0x02024000, gMapShapeGrid)

/// The sound driver's mixer, SoundMainRAM, 932 bytes. m4aSoundInit copies
/// SOUND_MIXER_CODE_SIZE (1 KB) there at once, so the 92 bytes after the mixer
/// in the ROM land just after it.
#define Ram_SoundMixer FIXED_ADDRESS((u8 *), 0x03006000, SoundMainRAM)

/// The end of the IWRAM heap, which the allocators test against, and the
/// bottom of the stacks.
#define Ram_IwramHeapEnd                                                       \
  FIXED_ADDRESS((u8 *), 0x03006fc0, IwramSoundMixWorkspace)

/// The frame counter at its constant address, as the overlays of Airs Rock,
/// Atteka Inlet and Ancient Lemuria read it: each test reloads the address,
/// where the symbol would combine the two loads.
#define Ram_FrameCount FIXED_ADDRESS((u32 *), 0x0300122c, gFrameCount)

/// The world map screen's scroll (x, then y, 16.16 pixels).
/// WorldMapPicture_UpdateScroll reads it at constant addresses: the y word
/// loads from its own literal, or as the x word's address plus 4 where that is
/// live, which only constants give.
#define Ram_WorldMapScroll                                                     \
  FIXED_ADDRESS((s32 *), 0x0202a630, gWorldPicture_ScrollX)

/// The world map screen's work. Its setup (WorldPicture_Initialize) reaches it
/// at a constant address: the work's fields are read again after stores to the
/// screen values whose addresses it passes out, which only a constant gives.
#define Ram_WorldMapWork                                                       \
  FIXED_ADDRESS((struct WorldMapWork *), 0x0202a000, gWorldMapWork)
/// Where WorldMapPicture_UpdateScroll reads the cursor from (x, then y, 16.16
/// pixels): the setup points it at the work's cursor.
#define Ram_WorldMapCursor                                                     \
  FIXED_ADDRESS((s32 **), 0x0202a62c, gWorldPicture_Cursor)
/// The cursor's place when the screen opens (x, then y, 16.16 pixels).
#define Ram_WorldMapCursorStart                                                \
  FIXED_ADDRESS((s32 *), 0x0202a64c, gWorldPicture_CursorStartX)

/// The map's shape grid, a byte a cell, which MapGrid_LoadMap decodes from
/// the map header's shapes and the field's actor cells read.
extern u8 gMapShapeGrid[];

/// The layout's cell pictures, 32 KB of EWRAM at 0x02020000: each cell picture
/// 8 bytes, its four tile entries. MapGrid_LoadMap decodes them in.
extern u32 gCellPictures[];

/// Called once by the next VBlank, which clears it first; the world map sets it
/// to show its next picture.
extern void (*gVBlankCallback)(void);

/// A scene's own state in EWRAM, which each scene lays out as it needs.
extern s8 gSceneLocalState[];
/// Where gSceneLocalState keeps, by element, the Djinni Party_AddIsaacsParty
/// picked at random from The Broken Seal's Djinn the party lacks: its place in
/// its element plus one, or 0 when the party has them all. An element's last
/// Djinni stands for it in actor placements and Djinni battles.
#define SCENE_STATE_CHOSEN_DJINN 6480

/// Where gSceneLocalState keeps, by element, the reunion's pick of a Djinni
/// of The Broken Seal the party lacks, plus one (0 when it lacks none): the
/// Djinni an element's last Djinn slot stands for (Party_AddIsaacsParty).
#define SCENE_STATE_REUNION_DJINN 6480

#endif
