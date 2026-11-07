/// @file
/// Moves a list of window sprites.

#include "types.h"
#include "graphics/render_input.h"

/// Moves every window sprite of a list by dx, dy: the output's position and its
/// queued sprite's OAM x and y.
void Func_080ec484(struct WindowSpriteList *list, s32 dx, s32 dy) {
  struct WindowSprite *output;
  struct QueuedSprite *sprite;

  if (list == NULL)
    return;
  output = list->head;
  while (output != NULL) {
    output->x += dx;
    output->y += dy;
    sprite = &output->sprite;
    output = output->next;
    sprite->oam.x += dx;
    sprite->oam.y += dy;
  }
}
