#ifndef ALCHEMY_UNIT_SCENE_COMMON_VAULT_RANGE_H
#define ALCHEMY_UNIT_SCENE_COMMON_VAULT_RANGE_H

#include "types.h"

struct FieldActor;

s32 Vault_StopIfFacingNear(struct FieldActor *actor, struct FieldActor *target,
                           s32 range, s32 force);

#endif
