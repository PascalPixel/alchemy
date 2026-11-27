#ifndef ALCHEMY_FIELD_RESOURCE_SLOT_H
#define ALCHEMY_FIELD_RESOURCE_SLOT_H

/* What the resource_slot object lends the field and the scenes: a few of its
   functions through their stub calls, and the sprite animation reset. */

#include "types.h"
#include "resource_ids.h"

struct ACTOR;
struct SPR_ANIMATION;

Sint32 fdMapGridIsClimbableMetatileStub(Sint32 x, Sint32 z, Sint32 kind);
Sint32 fdActorGetMetatileIndexStub(struct ACTOR *actor);
Sint32 fdFlagBlockPlaceAllMarkingEdgesStub(struct ACTOR *actor);
Sint32 fdActorCheckBlockedStub(Sint32 arg);

#endif
