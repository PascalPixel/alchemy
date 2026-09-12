#ifndef ALCHEMY_OBJECT_FACTORY_H
#define ALCHEMY_OBJECT_FACTORY_H

#include "types.h"

void *Object_Create(
    u32 kind, u32 index, u32 resource, u32 position, u32 parameter);

#endif
