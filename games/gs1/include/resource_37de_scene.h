#ifndef ALCHEMY_RESOURCE_37DE_SCENE_H
#define ALCHEMY_RESOURCE_37DE_SCENE_H

#include "effect_runtime.h"

struct Resource37deSceneRuntime {
    u8 padding_000[0x1c0];
    s32 scene_request_1c0;
    u8 padding_1c4[4];
    s32 scene_setup_word_1c8;
};

#define RESOURCE37DE_SCENE_OFFSET(field) \
    ((u32)&(((struct Resource37deSceneRuntime *)0)->field))
typedef char Resource37deSceneRuntime_scene_request_offset[
    RESOURCE37DE_SCENE_OFFSET(scene_request_1c0) == 0x1c0 ? 1 : -1
];
typedef char Resource37deSceneRuntime_scene_setup_word_offset[
    RESOURCE37DE_SCENE_OFFSET(scene_setup_word_1c8) == 0x1c8 ? 1 : -1
];
#undef RESOURCE37DE_SCENE_OFFSET

#define RESOURCE37DE_SCENE_RUNTIME \
    ((struct Resource37deSceneRuntime *)EFFECT_RUNTIME)

s32 TestSceneFlag(s32);
void SetCameraZoom(s32, s32, s32);
void QueueSceneSound(s32);
void InitializeSceneRecordBuffer(void);

#endif
