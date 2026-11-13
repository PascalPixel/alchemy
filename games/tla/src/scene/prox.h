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

#endif
