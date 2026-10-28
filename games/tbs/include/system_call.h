#ifndef ALCHEMY_SYSTEM_CALL_H
#define ALCHEMY_SYSTEM_CALL_H

/// @file
/// The system call layer: one typed inline for each of the thirteen main-image
/// routines below that the scene, menu and debug overlays import; every overlay
/// call of them goes through it.
///
/// It is a habit of the whole game written once: a layer, not a
/// compiler-steering device. An overlay writes
/// `Task_AddFrame(func, TASK_PASS_FRAME)` and the inline
/// calls Task_Add, which resolves to the overlay's import
/// stub. Main-image code calls the routines as ordinary functions; only the
/// shared scene/shared/scroll_initialize_hblank_dma.h, which
/// field/event_commands.c includes as lemurian_ship.c does, reaches the
/// layer there. event_inline.h includes this layer; rooms without the event
/// command layer use it directly.

// Passing arguments through inline parameters rematerializes constants at each
// call.

#include "types.h"
#include "module/field/event_commands.h"
#include "module/system/system.h"
#include "module/window/window.h"
#include "module/game/game.h"
#include "module/import.h"

struct Actor;
struct FieldPosition;
struct MenuSprite;

/// Adds `func` as a task of priority `priority` (TASK_PASS_FRAME
/// runs it once a frame), or gives the task already running it that
/// priority. Returns the slot it wrote, which the sort in the same call may
/// already have moved, or -1 when all 20 task slots are taken. Task_Add under
/// the name overlays import it by; event.h's Task_AddOrUpdateFrame
/// calls it too.
static inline s32 Task_AddFrame(TaskFunc func, s32 priority) {
  return Task_Add(func, priority);
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

static inline void Field_SetCameraBounds(s32 minX, s32 minY, s32 maxX,
                                         s32 maxY) {
  MapGrid_SetCameraBounds(minX, minY, maxX, maxY);
}

static inline void FieldActor_JumpDownLedge(s32 actorId, s32 mode, s32 action) {
  Actor_JumpDownLedge(actorId, mode, action);
}

static inline s32
FieldActor_CheckMovementCollision(struct Actor *actor,
                                  struct FieldPosition *position) {
  return MapGrid_CheckCollision(actor, position);
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
/// `destination`. Scenes, the Title and the File Screen reach the decoder
/// through it; main-image code calls Decompress_TaggedLz without it,
/// LuckyDice_Run through call.h's Call2.
static inline s32 FieldDecompress_TaggedLz(const void *source,
                                           void *destination) {
  return Decompress_TaggedLz(source, destination);
}

static inline void LineScroll_StartWave(s32 mode, s32 freqX, s32 stepX,
                                        s32 ampX, s32 freqY, s32 stepY,
                                        s32 ampY) {
  LineScroll_InitWave(mode, freqX, stepX, ampX, freqY, stepY, ampY);
}

#endif
