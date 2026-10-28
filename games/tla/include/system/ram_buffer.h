#ifndef ALCHEMY_RAM_BUFFER_H
#define ALCHEMY_RAM_BUFFER_H

#include "types.h"

#define Ram_MapCellBuffer FIXED_ADDRESS((u8 *), 0x02010000, gMapCellBuffer)
#define Ram_MapCollision FIXED_ADDRESS((u8 *), 0x0202c000, gMapCollision)
/// The cell picture table: each cell picture's top two tile entries, then its
/// bottom two. Func_0802abbc reads it at constant addresses inside its loop; as
/// the symbol the two table addresses hoist out of the loop.
#define Ram_MapBlocks FIXED_ADDRESS((u8 *), 0x02020000, gMapBlocks)

#define Ram_SoundMixer FIXED_ADDRESS((u8 *), 0x03006000, SoundDriver_Mixer)

#define Ram_IwramHeapEnd                                                       \
  FIXED_ADDRESS((u8 *), 0x03006fc0, IwramSoundMixWorkspace)

/// Overlay 693's Func_02000c14 reads the frame counter at a constant address.
/// Each test reloads the address; using the symbol combines the two loads.
#define Ram_FrameCount FIXED_ADDRESS((u32 *), 0x0300122c, gFrameCount)

/// The world map screen's scroll (x, then y, 16.16 pixels). Func_080ed0a0
/// reads it at constant addresses: the y word loads from its own literal, or
/// as the x word's address plus 4 where that is live, which only constants
/// give.
#define Ram_WorldMapScroll FIXED_ADDRESS((s32 *), 0x0202a630, Data_0202a630)

/// The world map screen's work. Its setup (Func_080ebf94) reaches it at a
/// constant address: the work's fields are read again after stores to the
/// screen values whose addresses it passes out, which only a constant gives.
#define Ram_WorldMapWork                                                       \
  FIXED_ADDRESS((struct WorldMapWork *), 0x0202a000, gWorldMapWork)
/// Where Func_080ed0a0 reads the cursor from (x, then y, 16.16 pixels): the
/// setup points it at the work's cursor.
#define Ram_WorldMapCursor FIXED_ADDRESS((s32 **), 0x0202a62c, Data_0202a62c)
/// The cursor's place when the screen opens (x, then y, 16.16 pixels).
#define Ram_WorldMapCursorStart                                                \
  FIXED_ADDRESS((s32 *), 0x0202a64c, Data_0202a64c)

struct BattleWork;
struct BattlePresentationTransition;
struct MapWork;
struct ObjectSystemWork;
struct ScreenTransitionState;

struct HeapSlots {
  void *nextEwram;
  void *nextIwram;
  u8 *transformStack;
  void *unk0c[2];
  void *scriptObjects;
  struct ObjectSystemWork *objectSystem;
  void *unk1c[1];
  struct MapWork *mapWork;
  struct BattleWork *battleWork;
  void *unk28[2];
  void *cameraWork;
  void *unk34[2];
  u8 *windowTiles;
  void *windowLayout;
  void *glyphWork;
  void *menuWork;
  void *unk4c[4];
  u8 *battleFxWork[2];
  void *unk64[2];
  void *eventWork;
  void *unk70[3];
  struct ScreenTransitionState *screenTransition;
  void *block32;
  void *unk84[11];
  struct BattlePresentationTransition *transitionWork;
  u16 *canvasMap;
  void *unkb8[5];
  void *serialWork;
  void *unkd0[3];
  void *menuRuntime;
  void *effectWork;
  void *unke4[1];
  void *menuSelectWork;
  void *unkec[3];
  void *block62;
  void *unkfc[1];
};

#define Ram_HeapSlots                                                          \
  FIXED_ADDRESS((struct HeapSlots *), 0x03000000, gHeapSlots)

#endif
