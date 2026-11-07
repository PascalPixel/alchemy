// Matches (tla-en, 2026-11-07).
// Plain C through struct WindowSprite: the sprite pointer is taken after the
// output's own x and y are moved and before the next link is read (it was
// differ 18 with the sprite taken first and empty asms).
// Moves every window sprite of a list by dx, dy: the output's position and
// its queued sprite's OAM x and y.
#include "types.h"
#include "graphics/render_input.h"

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
