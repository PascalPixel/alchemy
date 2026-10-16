#ifndef ALCHEMY_RESOURCE_37DE_SCENE_H
#define ALCHEMY_RESOURCE_37DE_SCENE_H

#include "global_cells.h"
#include "static_assert.h"

struct Resource37deSceneRuntime {
  u8 padding000[0x1c0];
  s32 sceneRequest1c0;
  u8 padding1c4[4];
  s32 sceneSetupWord1c8;
};

STATIC_ASSERT(offsetof(struct Resource37deSceneRuntime, sceneRequest1c0) ==
              0x1c0);
STATIC_ASSERT(offsetof(struct Resource37deSceneRuntime, sceneSetupWord1c8) ==
              0x1c8);

void QueueSceneSound(s32);
void InitializeSceneRecordBuffer(void);

#endif
