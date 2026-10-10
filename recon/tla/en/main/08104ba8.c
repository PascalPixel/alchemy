/* Pin-free trial (tla-en): listing 42 instructions, draft 40, common 24, differ 34.
 * Earlier register-allocation notes describe the prior pinned form.
 */
#include "types.h"
#include "render_input.h"

extern u8 *gMenuWork;

s32 UiIcon_DrawVariantWithTileOffset(struct RenderInput *window, s32 x, s32 y, s32 variant)
{
    struct RenderOutput *object;
    u32 resource;
    s32 v = variant; /* Earlier pinned trial: keeps the variant in r5 */
    struct RenderInput *w = window; /* Earlier pinned trial: keeps the window in r4 */
    s32 xx = x; /* Earlier pinned trial: keeps x in r6 */
    u8 *data = gMenuWork;

    if (v == 0) {
        resource = *(u16 *)(data + 0x392);
        y -= 3;
    } else {
        resource = *(u16 *)(data + 0x394);
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
