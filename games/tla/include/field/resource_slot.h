#ifndef ALCHEMY_FIELD_RESOURCE_SLOT_H
#define ALCHEMY_FIELD_RESOURCE_SLOT_H

/* @file */
/* What the resource_slot object lends the field and the scenes: a few of its */
/* functions through their stub calls, and the sprite animation reset. */

#include "types.h"
#include "resource_ids.h"

struct ACTOR;
struct SPR_ANIMATION;

s32 fdMapGridIsClimbableCellStub(s32 x, s32 z, s32 kind);
s32 fdActorGetCellIndexStub(struct ACTOR *actor);
s32 fdFlagBlockPlaceAllMarkingEdgesStub(struct ACTOR *actor);
s32 fdActorCheckBlockedStub(s32 arg);

#endif
