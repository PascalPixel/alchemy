#ifndef ALCHEMY_RESOURCE_37DE_MAP_H
#define ALCHEMY_RESOURCE_37DE_MAP_H

/// @file
/// A scene overlay's imports: queueing a scene sound and setting up its record
/// buffer.

#include "global_cells.h"
#include "static_assert.h"

void QueueMapSound(s32);
void InitializeSparkleRecordBuffer(void);

#endif
