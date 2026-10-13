#ifndef ALCHEMY_UNIT_SCENE_COMMON_KUUPUAPPU_RANGE_H
#define ALCHEMY_UNIT_SCENE_COMMON_KUUPUAPPU_RANGE_H

#include "types.h"

struct FieldActor;

s32 SceneActor_CheckFacingAndRange(struct FieldActor *actor, struct FieldActor *target, s32 range, s32 force);

#endif
