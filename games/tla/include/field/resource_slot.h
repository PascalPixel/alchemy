#ifndef ALCHEMY_FIELD_RESOURCE_SLOT_H
#define ALCHEMY_FIELD_RESOURCE_SLOT_H

/// @file
/// What the resource_slot object lends the field and the scenes: a few of its
/// routines through their far calls, and the sprite animation reset.

#include "types.h"
#include "resource_ids.h"

struct Actor;
struct SpriteAnimation;

s32 MapGrid_IsClimbableCellFar(s32 x, s32 z, s32 kind);
s32 Actor_GetCellIndexFar(struct Actor *actor);
s32 FlagBlock_PlaceAllMarkingEdgesFar(struct Actor *actor);
s32 Actor_CheckBlockedFar(s32 arg);
void SpriteAnimation_Reset(struct SpriteAnimation *state);

#endif
