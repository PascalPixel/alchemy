#ifndef ALCHEMY_SYSTEM_CALL_H
#define ALCHEMY_SYSTEM_CALL_H

/// @file
/// The system call layer: one typed inline for each of the main-image routines
/// below that the scene and debug overlays import; every overlay call of them
/// goes through it.
///
/// It is a habit of the whole game written once: a layer, not a
/// compiler-steering device. An overlay writes
/// `Task_AddFrame(func, TASK_PASS_FRAME)` and the inline calls Task_Add, which
/// resolves to the overlay's import stub. event_inline.h includes this layer.

#include "event.h"

/// Adds `func` as a task of priority `priority` (TASK_PASS_FRAME
/// runs it once a frame), or gives the task already running it that
/// priority. Returns the slot it wrote, or -1 when all 20 task slots are
/// taken. Task_Add under the name overlays import it by.
static inline s32 Task_AddFrame(TaskFunc func, s32 priority) {
  return Task_Add(func, priority);
}

/// Task_AddFrame, for a caller that leaves the slot unread.
static inline void FieldTask_AddFrame(TaskFunc func, s32 priority) {
  Task_Add(func, priority);
}

static inline void FieldMessage_DecodeRawUnbounded(s32 message, u16 *buffer) {
  Message_DecodeRawUnbounded(message, buffer);
}

static inline void Field_SetCameraBounds(s32 minX, s32 minY, s32 maxX,
                                         s32 maxY) {
  MapGrid_SetCameraBounds(minX, minY, maxX, maxY);
}

static inline s32 FieldGameFlag_GetByte(s32 flag) {
  return GameFlag_GetByte(flag);
}

static inline s32 FieldParty_ListMembers(s16 *owners) {
  return Party_ListActiveOwners(owners);
}

static inline s32 FieldUnit_AddHp(s32 owner, s32 delta) {
  return Unit_AddHp(owner, delta);
}

/// Lets frames of the game pass (wait()).
static inline void System_Wait(s32 frames) { wait(frames); }

/// Task_Remove, from an overlay.
static inline void FieldTask_Remove(TaskFunc func) { Task_RemoveFar(func); }

#endif
