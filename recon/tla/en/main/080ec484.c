/* Draft, differ 24 of 38: the native takes the sprite half of the output through a pointer at
 * +16 (adds r1, r4; adds r1, #16; [r1, #4], [r1, #6]) and reads its x after the stores to the
 * output's x and y, with the next link loaded right after; this form reads the sprite's x before
 * those stores. Direct oam fields address it at +20 and +22 from a copy of the output (differ 25). */
#include "types.h"
#include "render_input.h"

struct RenderOutputSprite {
    u8 unknown_00[4];
    struct RenderOam oam;
};

void Func_080ec484(struct RenderOutputList *list, s32 dx, s32 dy)
{
    struct RenderOutput *output;

    if (list == NULL)
        return;
    output = list->head;
    while (output != NULL) {
        struct RenderOutputSprite *sprite = (struct RenderOutputSprite *)output->unknown_10;

        output->x += dx;
        output->y += dy;
        output = output->next;
        sprite->oam.x += dx;
        sprite->oam.y += dy;
    }
}
