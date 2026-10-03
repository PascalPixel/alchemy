/* Status screens: create one of two icon variants, shifting its tile index by the variant. */
#include "TYPES.H"
#include "RENDER_INPUT.H"

extern u8 *gMenuWork;



s32 UiIcon_DrawVariantWithTileOffset(struct RenderInput *window, s32 x, s32 y, s32 variant)
{
    struct RenderOutput *object;
    u32 resource;
    register s32 v asm("r5") = variant; /* FAKEMATCH: keeps the variant in r5 */
    register struct RenderInput *w asm("r4") = window; /* FAKEMATCH: keeps the window in r4 */
    register s32 xx asm("r6") = x; /* FAKEMATCH: keeps x in r6 */
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
    object->unknown_0c = 0;
    object->active = 1;
    return 1;
}
