#ifndef ALCHEMY_RESOURCE_37DE_SCENE_H
#define ALCHEMY_RESOURCE_37DE_SCENE_H

/// @file
/// A scene overlay's imports: queueing a scene sound and setting up its record
/// buffer.

#include "global_cells.h"
#include "static_assert.h"

void QueueSceneSound(s32);
void InitializeSceneRecordBuffer(void);

#endif
