#ifndef ALCHEMY_HEAP_SLOTS_H
#define ALCHEMY_HEAP_SLOTS_H

/// @file
/// The heap slots of gHeapSlots, read through their own names.

#include "types.h"
#include "system/heap.h"

struct ScreenTransitionState;

/// The heap slots from HEAP_SLOT_MAP_GRID on: gMapGridState[0] is the map's
/// state (struct MapGridScrollState, or the world map's struct WorldMapView).
#define gMapGridState (*(void *(*)[]) & gHeapSlots[HEAP_SLOT_MAP_GRID])
/// The menu state, heap slot HEAP_SLOT_MENU: each menu screen and the shop
/// read it through their own struct while they are open.
#define gMenuState (*(void **)&gHeapSlots[HEAP_SLOT_MENU])

/// The heap slots from HEAP_SLOT_FIELD_BLEND on: gFieldBlendState[0] is the
/// field's colour blend state.
#define gFieldBlendState (*(void *(*)[]) & gHeapSlots[HEAP_SLOT_FIELD_BLEND])
/// The screen transition's state, heap slot HEAP_SLOT_SCREEN_TRANSITION.
#define gScreenTransitionState                                                 \
  (*(struct ScreenTransitionState **)&gHeapSlots[HEAP_SLOT_SCREEN_TRANSITION])
/// HEAP_SLOT_BATTLE_ENTRY_TIMER: the battle's entry transition timer in frames.
#define gBattleEntryTimer (*(s32 **)&gHeapSlots[HEAP_SLOT_BATTLE_ENTRY_TIMER])
/// The running Psynergy effect's state (struct FieldEffectState), heap slot
/// HEAP_SLOT_FIELD_EFFECT.
#define gFieldEffectState (*(void **)&gHeapSlots[HEAP_SLOT_FIELD_EFFECT])

// EWRAM buffers
/// The map's cells at 0x02010000, 128 by 128 words (struct MapGridCell): 64 KB,
/// so while a map is loaded they run on through gActorSpriteSlots and
/// gDecodeBuffer.
extern u32 gMapGridCellBuffer[];
/// 16 KB of EWRAM at 0x0201c000.
extern u8 gDecodeBuffer[];
/// 16 KB of EWRAM at 0x02018000, where the battle's actor sprite buffers
/// start, 16 KB a slot; the map's cells fill it while a map is loaded.
extern u8 gActorSpriteSlots[];

#endif
