#ifndef ALCHEMY_FIELD_INTERNAL_H
#define ALCHEMY_FIELD_INTERNAL_H

/// @file
/// What every part of the field object shares: its includes, in their
/// original order, and the names two or more parts use. field.c includes it
/// before the parts, and each part includes it too, so that a part read on its
/// own still resolves. It places no bytes.

#include "types.h"
#include "scene.h"
#include "sprite.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "dma.h"
#include "heap_release.h"
#include "iwram_call.h"
#include "resource.h"
#include <affine.h>
#include "sprite_metadata.h"
#include "actor_view.h"

#endif
