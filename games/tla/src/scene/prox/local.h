#ifndef TLA_PROX_LOCAL_H
#define TLA_PROX_LOCAL_H

/// @file
/// Overlay 6A8's scene layer: the declarations its scenes share and the
/// inline helpers some of their calls go through, as overlay 694's do.

#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"

#define EVENT_WORK ((struct EventWork *)Ram_HeapSlots->eventWork)


static inline void Prox_MessageOpen(s32 actor, s32 flags) {
  /* FAKEMATCH(wrapper): scene calls reach the main image through one inline
   * layer; through Call2 the actor is finished after the flags, and 4
   * instructions differ in Func_02001cf0, 2 in Func_02000dac. */
  FieldMessage_ShowAtActor(actor, flags);
}

static inline void Prox_Jump(s32 actor, s32 height, s32 frames) {
  /* FAKEMATCH(wrapper): scene calls reach the main image through one inline
   * layer, which sets the actor last as the native does; through Call3, 38
   * instructions differ in Func_02001cf0 and 28 in Func_02000dac. */
  Func_080d47b4(actor, height, frames);
}

#endif
