#ifndef ALCHEMY_UNIT_MAP_COMMON_VAULT_RANGE_H
#define ALCHEMY_UNIT_MAP_COMMON_VAULT_RANGE_H

#include "types.h"

struct Actor;

s32 Vault_StopIfFacingNear(struct Actor *actor, struct Actor *target, s32 range,
                           s32 force);

#endif
