#ifndef ALCHEMY_ACTOR_CREATE_H
#define ALCHEMY_ACTOR_CREATE_H

/// @file
/// FieldActor_Create, which creates an object.

#include "types.h"

void *FieldActor_Create(u32 kind, u32 index, u32 resource, u32 position,
                        u32 parameter);

#endif
