/* Pin-free trial (tla-en): listing 38 instructions, draft 36, common 23, differ 28.
 * Earlier register-allocation notes describe the prior pinned form.
 */
#include "types.h"
#include "render_input.h"

struct ArrowResources {
    u8 unknown_000[0x392];
    u16 resource[2];
};

extern struct ArrowResources *gMenuWork;

s32 UiIcon_CreateStatChangeArrow(struct RenderInput *window, s32 x, s32 y, s32 variant)
{
    struct RenderOutput *object;
    u32 resource;
    s32 v = variant; /* Earlier pinned trial: keeps the variant in r5 */
    struct RenderInput *w = window; /* Earlier pinned trial: keeps the window in r4 */
    s32 xx = x; /* Earlier pinned trial: keeps x in r6 */
    struct ArrowResources *data = gMenuWork;

    if (v == 0)
        resource = data->resource[0];
    else
        resource = data->resource[1];
    object = RenderOutput_CreateFar(resource, 0x40000000, w, xx, y);
    if (object == 0)
        return -1;
    object->kind = 0;
    object->anim_step = 0;
    object->active = 1;
    return 1;
}
