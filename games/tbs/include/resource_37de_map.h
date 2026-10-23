#ifndef ALCHEMY_RESOURCE_37DE_MAP_H
#define ALCHEMY_RESOURCE_37DE_MAP_H

/// @file
/// A map overlay's imports: queueing a map sound and setting up its record
/// buffer.

#include "heap_slots.h"
#include "static_assert.h"

void QueueMapSound(s32);
void InitializeSparkleRecordBuffer(void);

#endif
