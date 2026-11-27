#ifndef ALCHEMY_WINDOW_SPRITE_LIST_H
#define ALCHEMY_WINDOW_SPRITE_LIST_H

/* @file */
/* Taking, freeing and listing window sprites, and queueing a sprite for OAM. */

#include "edition.h"
#include "render_input.h"
#include "system.h"

WINDOW_SPR *mnWindowSprAcquireFree(void);
void mnWindowSprReleaseFree(WINDOW_SPR *sprite);
void mnWindowSprAppendToList(WINDOW_SPR_LIST *list, WINDOW_SPR *sprite);

#endif
