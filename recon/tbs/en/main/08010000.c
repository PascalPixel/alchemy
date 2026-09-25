/* NONMATCHING: first function 560 bytes, candidate 552, 269 differing halfwords.
 * The retained 1796-byte listing contains additional functions after 0x08010230.
 * Remaining: layer-state layout, frame size and loop allocation differ.
 * WALL: structural-topology; reference uses an 8-byte frame and keeps work in r8.
 */
#include "TYPES.H"
#include "IWRAM_CALL.H"

struct MapLayerScroll {
    s32 x;
    s32 y;
    s32 offset_x;
    s32 offset_y;
    const s32 *scale_x;
    const s32 *scale_y;
    s32 speed_x;
    s32 speed_y;
    s32 phase_x;
    s32 phase_y;
    u16 mask_x;
    u16 mask_y;
    s32 unknown_2c;
};

struct MapScrollWork {
    s32 *origin;
    s32 shake_x;
    s32 shake_y;
    s32 shake_decay;
    u8 unused[0xe4 - 0x10];
    s32 view_x;
    s32 view_y;
    s32 min_x;
    s32 min_y;
    s32 max_x;
    s32 max_y;
    u8 pad[8];
    struct MapLayerScroll layers[3];
};

extern s32 Random16(void);
void Map_RenderPaletteMappedBlock(s32 layer, s32 x, s32 y);
void Func_0800fec8(s32 layer, s32 x, s32 y);

void Map_UpdateLayerScroll(void)
{
    struct MapScrollWork *work;
    struct MapLayerScroll *layer;
    s32 *origin;
    s32 x;
    s32 y;
    s32 min_x;
    s32 min_y;
    s32 max_x;
    s32 max_y;
    s32 base_x;
    s32 base_y;
    s32 scale_x;
    s32 scale_y;
    s32 i;

    work = *(struct MapScrollWork **)0x03001e70;
    origin = work->origin;
    if (origin == 0)
        return;

    base_x = origin[0] - 0x880000;
    base_y = origin[2] - origin[1] - 0x600000;
    min_x = work->min_x + work->shake_x;
    max_x = work->max_x - work->shake_x - 0x100000;
    min_y = work->min_y + work->shake_y;
    max_y = work->max_y - work->shake_y - 0xa00000;

    if (min_x > max_x)
        max_x = min_x;
    if (min_y > max_y)
        max_y = min_y;
    if (base_x < min_x)
        base_x = min_x;
    if (base_x > max_x)
        base_x = max_x;
    if (base_y < min_y)
        base_y = min_y;
    if (base_y > max_y)
        base_y = max_y;

    if (work->shake_x != 0) {
        s32 amplitude = work->shake_x;
        s32 random_delta = Random16() - Random16();
        base_x += Iwram_MulQ16(amplitude, random_delta);
        work->shake_x = Iwram_MulQ16(amplitude, work->shake_decay);
    }
    if (work->shake_y != 0) {
        s32 amplitude = work->shake_y;
        s32 random_delta = Random16() - Random16();
        base_y += Iwram_MulQ16(amplitude, random_delta);
        work->shake_y = Iwram_MulQ16(amplitude, work->shake_decay);
    }

    work->view_x = base_x;
    work->view_y = base_y;
    layer = work->layers;
    for (i = 0; i < 3; i++, layer++) {
        scale_x = Iwram_MulQ16(base_x, *layer->scale_x);
        scale_y = Iwram_MulQ16(base_y, *layer->scale_y);
        if (layer->speed_x != 0) {
            layer->phase_x += layer->speed_x;
            scale_x = (scale_x + layer->phase_x) &
                (((u32)layer->mask_x << 19) | 0x7ffff);
        }
        if (layer->speed_y != 0) {
            layer->phase_y += layer->speed_y;
            scale_y = (scale_y + layer->phase_y) &
                (((u32)layer->mask_y << 19) | 0x7ffff);
        }
        scale_x += layer->offset_x;
        scale_y += layer->offset_y;
        x = scale_x;
        y = scale_y;
        if (x < 0)
            x += 0x7ffff;
        if (y < 0)
            y += 0x7ffff;
        x >>= 19;
        y >>= 19;

        if (((layer->x ^ scale_x) & 0x80000) != 0) {
            if (layer->x < scale_x)
                Map_RenderPaletteMappedBlock(i, x + 30, y);
            else
                Map_RenderPaletteMappedBlock(i, x, y);
        }
        if (((layer->y ^ scale_y) & 0x100000) != 0) {
            if (layer->y < scale_y)
                Func_0800fec8(i, x, y + 20);
            else
                Func_0800fec8(i, x, y);
        }
        *(volatile u16 *)(0x03001ad0 + (3 - i) * 4) = (u16)(scale_x >> 16);
        *(volatile u16 *)(0x03001ad0 + (3 - i) * 4 + 2) = (u16)(scale_y >> 16);
        layer->x = scale_x;
        layer->y = scale_y;
    }
}
