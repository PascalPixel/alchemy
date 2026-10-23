#ifndef ALCHEMY_ENGINE_CALL_H
#define ALCHEMY_ENGINE_CALL_H

/// @file
/// The engine call layer: one typed inline for each of the thirteen main-image
/// routines below that the map, menu and debug overlays import; every overlay
/// call of them goes through it.
///
/// It is a habit of the whole game written once: a layer, not a
/// compiler-steering device (AGENTS.md S2). An overlay writes
/// `FieldScheduler_AddCallback(callback, TASK_PRIORITY_MAP)` and the inline
/// calls Scheduler_AddOrUpdateCallback, which resolves to the overlay's import
/// stub. Main-image code calls the routines as ordinary functions; only the
/// shared map/common/scroll_initialize_hblank_dma.h, which
/// object/psynergy_effects.c includes as lemurian_ship.c does, reaches the
/// layer there. field_service.h includes this layer; rooms without the script
/// command layer use it directly.

// Passing arguments through inline parameters rematerializes constants at each
// call.

#include "types.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "unit/window/window.h"
#include "unit/game/game.h"
#include "unit/import.h"

struct ObjectEvent;
struct FieldPosition;
struct MenuSprite;

/// Schedules `callback` under the task key `priority` (TASK_PRIORITY_MAP
/// runs it every frame in the main loop pass), or gives the task already
/// running it that key. Returns the slot it wrote, which the sort in the same
/// call may already have moved, or -1 when all 20 task slots are taken. The
/// map service layer's
/// Task_AddCallback and FieldScheduler_AddOrUpdateCallback call it too, so
/// overlays import the routine under one name.
static inline s32 FieldScheduler_AddCallback(SchedulerCallback callback,
                                             s32 priority) {
  return Scheduler_AddOrUpdateCallback(callback, priority);
}

static inline void FieldUiTextResource_Initialize(struct MenuSprite *cursor,
                                                  s32 *slot) {
  UiTextResource_Initialize(cursor, slot);
}

static inline void FieldUiTextResource_SetPosition(struct MenuSprite *cursor,
                                                   s32 x, s32 y) {
  UiTextResource_SetPosition(cursor, x, y);
}

static inline void FieldUi_AdjustValueWithoutLimit(s32 message, u16 *buffer) {
  Ui_AdjustValueWithoutLimit(message, buffer);
}

static inline void Field_SetViewBounds(s32 minX, s32 minY, s32 maxX, s32 maxY) {
  MapGrid_SetViewBounds(minX, minY, maxX, maxY);
}

static inline void FieldObjectEvent_JumpDownLedge(s32 localId, s32 mode,
                                                  s32 action) {
  ObjectEvent_JumpDownLedge(localId, mode, action);
}

static inline s32
FieldObject_CheckMovementCollision(struct ObjectEvent *objectEvent,
                                   struct FieldPosition *position) {
  return Object_CheckMovementCollision((struct ObjectRuntime *)objectEvent,
                                       position);
}

static inline s32 FieldGameFlag_GetByte(s32 flag) {
  return GameFlag_GetByte(flag);
}

static inline void FieldGameFlag_SetByte(s32 flag, s32 value) {
  GameFlag_SetByte(flag, value);
}

static inline s32 FieldParty_ListActiveOwners(s16 *owners) {
  return Party_ListActiveOwners(owners);
}

static inline s32 FieldUnit_AddHp(s32 owner, s32 delta) {
  return Unit_AddHp(owner, delta);
}

/// Decodes type-01 compressed data from `source` into `destination`. Maps
/// and the title and clear menus reach the decoder through it; main-image code
/// calls Resource_DecodeType01 without it, LuckyDice_Run through call.h's
/// Value2.
static inline s32 FieldResource_DecodeType01(const void *source,
                                             void *destination) {
  return Resource_DecodeType01(source, destination);
}

static inline void DisplayScroll_StartHBlankDma(s32 mode, s32 freqX, s32 stepX,
                                                s32 ampX, s32 freqY, s32 stepY,
                                                s32 ampY) {
  Engine_DisplayScrollStartHBlankDma(mode, freqX, stepX, ampX, freqY, stepY,
                                     ampY);
}

#endif
