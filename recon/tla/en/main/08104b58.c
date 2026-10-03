/* Status screens: create the up or down arrow beside a changed stat. */
#include "TYPES.H"
#include "RENDER_INPUT.H"

struct ArrowResources {
    u8 unknown_000[0x392];
    u16 resource[2];
};

extern struct ArrowResources *gMenuWork;



s32 UiIcon_CreateStatChangeArrow(struct RenderInput *window, s32 x, s32 y, s32 variant)
{
    struct RenderOutput *object;
    u32 resource;
    register s32 v asm("r5") = variant; /* FAKEMATCH: keeps the variant in r5 */
    register struct RenderInput *w asm("r4") = window; /* FAKEMATCH: keeps the window in r4 */
    register s32 xx asm("r6") = x; /* FAKEMATCH: keeps x in r6 */
    struct ArrowResources *data = gMenuWork;

    if (v == 0)
        resource = data->resource[0];
    else
        resource = data->resource[1];
    object = RenderOutput_CreateFar(resource, 0x40000000, w, xx, y);
    if (object == 0)
        return -1;
    object->kind = 0;
    object->unknown_0c = 0;
    object->active = 1;
    return 1;
}
