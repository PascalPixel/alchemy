#ifndef ALCHEMY_OUTPUT_H
#define ALCHEMY_OUTPUT_H

#include "edition.h"
#include "render_input.h"

struct WindowSprite *WindowSprite_AcquireFree(void);
void WindowSprite_ReleaseFree(struct WindowSprite *entry);
void WindowSprite_AppendToList(struct WindowSpriteList *list,
                               struct WindowSprite *node);

#endif
