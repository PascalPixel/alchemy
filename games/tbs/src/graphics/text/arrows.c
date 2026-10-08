#include "types.h"
#include "render_input.h"
#include "inventory_menu.h"

s32 UiIcon_CreateStatChangeArrow(struct RenderInput *window, s32 x, s32 y, s32 variant)
{
    struct RenderOutput *object;
    u32 resource;
    register s32 v asm("r5") = variant; /* FAKEMATCH: keeps the variant in r5 */
    struct RenderInput *w = window;
    s32 xx = x;
    struct InventoryMenuState *data = gMenuWork;

    if (v == 0)
        resource = (u16)data->resource_slots[0];
    else
        resource = (u16)data->resource_slots[1];
    object = RenderOutput_CreateFar(resource, 0x40000000, w, xx, y);
    if (object == 0)
        return -1;
    object->kind = 0;
    object->anim_step = 0;
    object->active = 1;
    return 1;
}

s32 UiIcon_DrawVariantWithTileOffset(struct RenderInput *window, s32 x, s32 y, s32 variant)
{
    struct RenderOutput *object;
    u32 resource;
    register s32 v asm("r5") = variant; /* FAKEMATCH: keeps the variant in r5 */
    struct RenderInput *w = window;
    s32 xx = x;
    struct InventoryMenuState *data = gMenuWork;

    if (v == 0) {
        resource = (u16)data->resource_slots[0];
        y -= 3;
    } else {
        resource = (u16)data->resource_slots[1];
        y -= 4;
    }
    object = RenderOutput_CreateFar(resource, 0x40000000, w, xx, y);
    if (object == 0)
        return -1;
    object->kind = 0;
    object->anim_step = 0;
    object->active = 1;
    return 1;
}
