/* Near miss, differ 26 of 42 (2026-10-12; 34 before): this game reads the two arrow resources at 0x4ee and 0x4f0 of the menu runtime slot. Same instructions; the registers differ: the native copies variant from r3 to r5 so that the runtime pointer takes r3 and y stays in r2 until it is stored for the call; this draft leaves variant in r3, puts the pointer in r2 and moves y to r4. */
#include "types.h"
#include "render_input.h"

#include "ram_buffer.h"

s32 UiIcon_DrawVariantWithTileOffset(struct RenderInput *window, s32 x, s32 y, s32 variant)
{
    struct RenderOutput *object;
    u32 resource;
    u8 *data = Ram_HeapSlots->menu_runtime;

    if (variant == 0) {
        resource = *(u16 *)(data + 0x4ee);
        y -= 3;
    } else {
        resource = *(u16 *)(data + 0x4f0);
        y -= 4;
    }
    object = RenderOutput_CreateFar(resource, 0x40000000, window, x, y);
    if (object == 0)
        return -1;
    object->kind = 0;
    object->anim_step = 0;
    object->active = 1;
    return 1;
}
