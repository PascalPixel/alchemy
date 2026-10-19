#ifndef ALCHEMY_OAM_H
#define ALCHEMY_OAM_H

#include "edition.h"
#include "types.h"
#include "static_assert.h"

struct OamUsage {
  u16 used;
  u16 limit;
};

STATIC_ASSERT(sizeof(struct OamUsage) == 4);
STATIC_ASSERT(offsetof(struct OamUsage, limit) == 2);

extern struct OamUsage gOamUsage;
void Render_BuildOamList(void *work);

#endif
