#ifndef ALCHEMY_UNIT_MAP_COMMON_VAULT_RANGE_H
#define ALCHEMY_UNIT_MAP_COMMON_VAULT_RANGE_H

#include "types.h"

struct ObjectEvent;

s32 Vault_StopIfFacingNear(struct ObjectEvent *objectEvent,
                           struct ObjectEvent *target, s32 range, s32 force);

#endif
