// Draft, differ 24 of 38: the native takes the sprite half of the output
// through a pointer at +16 (adds r1, r4; adds r1, #16; [r1, #4], [r1, #6]) and
// reads its x after the stores to the output's x and y, with the next link
// loaded right after; this form reads the sprite's x before those stores.
// Direct oam fields address it at +20 and +22 from a copy of the output (differ
// 25).
// 2026-10-19: an empty asm with a memory clobber after the output's y store
// keeps the sprite's x load after the stores (20), but the sprite pointer and x
// then take r0 and r1 the other way round and the next link still loads last;
// four minutes of permuter found nothing better.
#include "types.h"
#include "render_input.h"

struct RenderOutputSprite {
  u8 unk00[4];
  struct RenderOam oam;
};

void Func_080ec484(struct WindowSpriteList *list, s32 dx, s32 dy) {
  struct WindowSprite *output;

  if (list == NULL)
    return;
  output = list->head;
  while (output != NULL) {
    struct RenderOutputSprite *sprite =
        (struct RenderOutputSprite *)output->unk10;

    output->x += dx;
    output->y += dy;
    output = output->next;
    sprite->oam.x += dx;
    sprite->oam.y += dy;
  }
}
