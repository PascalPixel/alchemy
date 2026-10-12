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
