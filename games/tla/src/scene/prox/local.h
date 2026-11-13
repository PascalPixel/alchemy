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
   * layer; as direct calls to FieldMessage_ShowAtActor, 29 of 936 instructions
   * of Func_02001cf0 differ in tla-en, 17 of 824 of Func_02000dac and 109 of
   * 1399 of Func_02002a8c. */
  FieldMessage_ShowAtActor(actor, flags);
}

#endif
