#include "layout_guard.h"
#include "types.h"

enum {
    WIPE_EXTENT_080CD104 = 128,
    WIPE_TILE_COLUMNS_080CD104 = 16,
};

typedef struct DisplayState_080cd104 {
    u8 padding0000[0x7824];
    s32 image_dirty;
} DisplayState_080cd104;

LAYOUT_OFFSET_GUARD(
    DisplayState080cd104_ImageDirty,
    DisplayState_080cd104,
    image_dirty,
    0x7824);

extern DisplayState_080cd104 *Data_03001eec;
extern s8 *Data_03001ef0;

void Func_080030f8(u32);
u32 Func_08004458(void);

static void WriteTiledPixel_080cd104(
    s8 *image,
    s32 x,
    s32 y,
    s8 value)
{
    u32 tile = (u32)(y >> 3) * WIPE_TILE_COLUMNS_080CD104 +
               (u32)(x >> 3);
    u32 pixel = tile * 64 + (u32)(y & 7) * 8 + (u32)(x & 7);

    image[pixel] = value;
}

static void PresentWipeStep_080cd104(DisplayState_080cd104 *display)
{
    display->image_dirty = 1;
    Func_080030f8(1);
}

/*
 * Reveal or erase a 128x128 tiled image with one randomized diagonal per row.
 * Direction 1 advances x while scanning rows; the other direction advances y
 * while scanning columns.  Each expanding frontier is presented for one
 * frame, producing the two differently paced wipe animations.
 */
void Func_080cd104(s32 direction, s32 displayed_value)
{
    DisplayState_080cd104 *display = Data_03001eec;
    s8 *image = Data_03001ef0;
    u8 offsets[WIPE_EXTENT_080CD104];
    s8 value = (s8)(1 - displayed_value);
    s32 index;

    for (index = 0; index < WIPE_EXTENT_080CD104; index++)
        offsets[index] = Func_08004458() & 0x3f;

    if (direction == 1) {
        s32 frontier = 0;
        s32 frontier_step = 1;
        s32 x = 0;

        do {
            frontier += frontier_step;
            frontier_step++;

            while (x != frontier) {
                s32 y;

                for (y = 0; y < WIPE_EXTENT_080CD104; y++) {
                    s32 shifted_x = x - offsets[y];

                    if (shifted_x >= 0 && shifted_x < WIPE_EXTENT_080CD104)
                        WriteTiledPixel_080cd104(
                            image, shifted_x, y, value);
                }
                x++;
            }

            PresentWipeStep_080cd104(display);
        } while (frontier <= 0x100);
    } else {
        s32 frontier = 0;
        u32 frontier_step = 1;
        s32 y = 0;

        do {
            frontier +=
                (s32)((frontier_step >> 31) + frontier_step) >> 1;
            frontier_step += 4;

            while (y != frontier) {
                s32 x;

                for (x = 0; x < WIPE_EXTENT_080CD104; x++) {
                    s32 shifted_y = y - offsets[x];

                    if (shifted_y >= 0 && shifted_y < WIPE_EXTENT_080CD104)
                        WriteTiledPixel_080cd104(
                            image, x, shifted_y, value);
                }
                y++;
            }

            PresentWipeStep_080cd104(display);
        } while (frontier <= 0xbf);
    }
}
