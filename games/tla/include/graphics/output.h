#ifndef ALCHEMY_OUTPUT_H
#define ALCHEMY_OUTPUT_H

/// @file
/// Taking, freeing and listing window sprites, and queueing a sprite for OAM.

#include "edition.h"
#include "render_input.h"

struct WindowSprite *WindowSprite_AcquireFree(void);
void WindowSprite_ReleaseFree(struct WindowSprite *sprite);
void WindowSprite_AppendToList(struct WindowSpriteList *list,
                               struct WindowSprite *sprite);
/// Queues a sprite at a depth slot of this frame's OAM list (system.h lists the
/// slots).
void Oam_AddSpriteAtDepth(void *entry, s32 slot);

#endif
