/* Near miss, differ 20 of 38 (2026-10-12; 28 before): this game reads the two arrow resources at 0x4ee and 0x4f0 of the menu runtime slot. Same instructions; the registers differ as in 08104ba8: the native copies variant to r5, keeps the runtime pointer in r3 and y in r2; this draft leaves variant in r3, puts the pointer in r2 and moves y to r4. Tried 2026-10-12: variant copied to a local and the parameter reused for the pointer (no change), a local copy of y (no change). */
#include "types.h"
#include "render_input.h"

struct ArrowResources {
    u8 unknown_000[0x4ee];
    u16 resource[2];
};

#include "ram_buffer.h"

s32 UiIcon_CreateStatChangeArrow(struct RenderInput *window, s32 x, s32 y, s32 variant)
{
    struct RenderOutput *object;
    u32 resource;
    struct ArrowResources *data = Ram_HeapSlots->menu_runtime;

    if (variant == 0)
        resource = data->resource[0];
    else
        resource = data->resource[1];
    object = RenderOutput_CreateFar(resource, 0x40000000, window, x, y);
    if (object == 0)
        return -1;
    object->kind = 0;
    object->anim_step = 0;
    object->active = 1;
    return 1;
}
