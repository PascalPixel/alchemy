#ifndef ALCHEMY_OUTPUT_H
#define ALCHEMY_OUTPUT_H

#include "edition.h"
#include "render_input.h"

struct WindowSprite *WindowSprite_AcquireFree(void);
void WindowSprite_ReleaseFree(struct WindowSprite *entry);
void WindowSprite_AppendToList(struct WindowSpriteList *list,
                               struct WindowSprite *node);
/// Queues a sprite at a depth slot of this frame's OAM list (system.h lists the
/// slots). The screens object calls it with no declaration in scope.
void Oam_AddSpriteAtDepth(void *entry, s32 slot);

#endif
