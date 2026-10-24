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
/// calls Task_Add, which resolves to the overlay's import
/// stub. Main-image code calls the routines as ordinary functions; only the
/// shared map/common/scroll_initialize_hblank_dma.h, which
/// overworld/field_services.c includes as lemurian_ship.c does, reaches the
/// layer there. field_service.h includes this layer; rooms without the script
/// command layer use it directly.

// Passing arguments through inline parameters rematerializes constants at each
// call.

#include "types.h"
#include "unit/overworld/field_services.h"
#include "unit/runtime/runtime.h"
#include "unit/window/window.h"
#include "unit/game/game.h"
#include "unit/import.h"

struct ObjectEvent;
struct FieldPosition;
struct MenuSprite;

/// Adds `callback` as a task of priority `priority` (TASK_PRIORITY_MAP
/// runs it every frame in the main loop pass), or gives the task already
/// running it that priority. Returns the slot it wrote, which the sort in the
/// same call may already have moved, or -1 when all 20 task slots are taken.
/// The map service layer's Task_AddCallback and
/// FieldScheduler_AddOrUpdateCallback call it too, so overlays import the
/// routine under one name.
static inline s32 FieldScheduler_AddCallback(TaskFunc callback, s32 priority) {
  return Task_Add(callback, priority);
}

static inline void FieldMenuArrow_Create(struct MenuSprite *cursor, s32 *slot) {
  MenuArrow_Create(cursor, slot);
}

static inline void FieldMenuArrow_SetPosition(struct MenuSprite *cursor, s32 x,
                                              s32 y) {
  MenuArrow_SetPosition(cursor, x, y);
}

static inline void FieldMessage_DecodeRawUnbounded(s32 message, u16 *buffer) {
  Message_DecodeRawUnbounded(message, buffer);
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
  return MapGrid_CheckCollision((struct Object *)objectEvent, position);
}

static inline s32 FieldGameFlag_GetByte(s32 flag) {
  return GameFlag_GetByte(flag);
}

static inline void FieldGameFlag_SetByte(s32 flag, s32 value) {
  GameFlag_SetByte(flag, value);
}

static inline s32 FieldParty_ListMembers(s16 *owners) {
  return Party_ListMembers(owners);
}

static inline s32 FieldUnit_AddHp(s32 owner, s32 delta) {
  return Unit_AddHp(owner, delta);
}

/// Decodes a tagged LZ stream (a first byte of 0 or 1) from `source` into
/// `destination`. Maps and the title screen and main menu reach the decoder
/// through it; main-image code calls Decompress_TaggedLz without it,
/// LuckyDice_Run through call.h's Call2.
static inline s32 FieldDecompress_TaggedLz(const void *source,
                                           void *destination) {
  return Decompress_TaggedLz(source, destination);
}

static inline void ScanlineEffect_StartWave(s32 mode, s32 freqX, s32 stepX,
                                            s32 ampX, s32 freqY, s32 stepY,
                                            s32 ampY) {
  ScanlineEffect_InitWave(mode, freqX, stepX, ampX, freqY, stepY, ampY);
}

#endif
