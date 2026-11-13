#ifndef ALCHEMY_RESOURCE_SLOT_INTERNAL_H
#define ALCHEMY_RESOURCE_SLOT_INTERNAL_H

/// @file
/// What every part of the resource_slot object shares: its includes.
/// resource_slot.c includes it before the parts, and each part includes it too.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"

struct Actor;
struct FieldPosition;
/// Defined in map/tile_animation.c.
s32 MapGrid_CheckCollision(struct Actor *actor, struct FieldPosition *position);

#endif // ALCHEMY_RESOURCE_SLOT_INTERNAL_H
