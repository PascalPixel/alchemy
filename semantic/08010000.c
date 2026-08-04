#include "types.h"

/*
 * Advance the three parallax layers of the walking map for one frame.
 *
 * The camera is taken from the walker position, biased by a per-axis constant
 * and clamped into the map's bounds, then the two camera velocities are
 * decayed exactly as 0x080111b4 decays them.  Each layer scales the clamped
 * camera by its own factor, adds its scrolling drift with an optional power-
 * of-two wrap, notices a whole-tile crossing on either axis, and publishes the
 * whole part of its position into Data_03001ad0 - the same six halfwords that
 * 0x08094544 reads as its scroll bases, written back to front, layer 0 into
 * the pair at +12 and layer 2 into the pair at +4.
 *
 * The mov ip,pc ; bx r3 / bx r9 sequences at 0x080100ac, 0x080100b8,
 * 0x080100e4, 0x080100f0, 0x08010118 and 0x08010124 are ordinary indirect
 * calls to the pooled 0x03000118, the IWRAM-relocated ARM fixed-point
 * multiply, each landing site consuming r0 in the same frame, as recorded for
 * 0x0800ebec.  The movs r0,r0 at 0x080100e2 is inside the instruction stream,
 * aligning the return address that mov ip,pc captures so it lands past the bx;
 * the other five sites are already aligned and carry no nop.
 *
 * if (v < 0) v += 0x7ffff; v >>= 19 at 0x08010164 and 0x08010172 is the
 * truncating fixed-point-to-whole conversion at 19 fractional bits; the xor
 * against 0x80000 and 0x100000 that follows is the "did that bit change"
 * boundary test.  The wrap mask is (u16 field << 19) | 0x0007ffff, so the
 * layer wraps at a whole-tile multiple.
 *
 * The epilogue at 0x080101fe unwinds the frame and ends pop {r0}; bx r0, so
 * the owner is void.  One direct caller, 0x0800feba; also published at
 * 0x0800fe94 and 0x080a00e8.
 */

struct Position_08010000 {
    s32 x;
    s32 y;
    s32 z;
};

struct Layer_08010000 {
    s32 previous_x;
    s32 previous_y;
    s32 offset_x;
    s32 offset_y;
    s32 scale_x;
    s32 scale_y;
    s32 drift_x;
    s32 drift_y;
    s32 accumulator_x;
    s32 accumulator_y;
    u16 wrap_x;
    u16 wrap_y;
    u8 unknown_2c[4];
};

struct Map_08010000 {
    struct Position_08010000 *walker;
    s32 velocity_x;
    s32 velocity_y;
    s32 damping;
    u8 unknown_10[212];
    s32 camera_x;
    s32 camera_y;
    s32 bound_low_x;
    s32 bound_low_y;
    s32 bound_high_x;
    s32 bound_high_y;
    u8 unknown_fc[8];
    struct Layer_08010000 layers[3];
};

struct Base_08010000 {
    u8 unknown_00[4];
    s16 low[6];
};

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_08010000)(s32 a, s32 b);

extern struct Map_08010000 *Data_03001e70;
extern struct Base_08010000 Data_03001ad0;

s32 Func_08004458(void);
void Func_0800ff54(s32 layer, s32 tile_x, s32 tile_y);
void Func_0800fec8(s32 layer, s32 tile_x, s32 tile_y);

void Func_08010000(void)
{
    struct Map_08010000 *map = Data_03001e70;
    struct Layer_08010000 *layer = map->layers;
    struct Position_08010000 *walker = map->walker;
    Multiply_08010000 multiply = (Multiply_08010000)0x03000118;
    s32 camera_x;
    s32 camera_y;
    s32 low_x;
    s32 low_y;
    s32 high_x;
    s32 high_y;
    s32 velocity_x;
    s32 velocity_y;
    s32 index;

    if (walker == 0)
        return;

    camera_x = walker->x + (s32)0xff880000;
    camera_y = (walker->z - walker->y) + (s32)0xffa00000;

    velocity_x = map->velocity_x;
    low_x = map->bound_low_x + velocity_x;
    high_x = map->bound_high_x - velocity_x + (s32)0xff100000;

    velocity_y = map->velocity_y;
    low_y = map->bound_low_y + velocity_y;
    high_y = map->bound_high_y - velocity_y + (s32)0xff600000;

    if (high_x < low_x)
        high_x = low_x;
    if (high_y < low_y)
        high_y = low_y;

    if (camera_x < low_x)
        camera_x = low_x;
    if (camera_x > high_x)
        camera_x = high_x;
    if (camera_y < low_y)
        camera_y = low_y;
    if (camera_y > high_y)
        camera_y = high_y;

    if (velocity_x != 0) {
        s32 first = Func_08004458();
        s32 second = Func_08004458();
        s32 velocity = map->velocity_x;

        camera_x += multiply(velocity, first - second);
        map->velocity_x = multiply(velocity, map->damping);
        velocity_y = map->velocity_y;
    }
    if (velocity_y != 0) {
        s32 first = Func_08004458();
        s32 second = Func_08004458();
        s32 velocity = map->velocity_y;

        camera_y += multiply(velocity, first - second);
        map->velocity_y = multiply(velocity, map->damping);
    }

    map->camera_x = camera_x;
    map->camera_y = camera_y;

    for (index = 0; index <= 2; index++) {
        s32 x = multiply(map->camera_x, layer->scale_x);
        s32 y = multiply(map->camera_y, layer->scale_y);
        s32 tile_x;
        s32 tile_y;
        s32 previous;

        if (layer->drift_x != 0) {
            s32 drifted = layer->accumulator_x + layer->drift_x;

            x += drifted;
            layer->accumulator_x = drifted;
            x &= ((s32)layer->wrap_x << 19) | 0x0007ffff;
        }
        if (layer->drift_y != 0) {
            s32 drifted = layer->accumulator_y + layer->drift_y;

            y += drifted;
            layer->accumulator_y = drifted;
            y &= ((s32)layer->wrap_y << 19) | 0x0007ffff;
        }

        x += layer->offset_x;
        y += layer->offset_y;

        tile_x = (x < 0 ? x + 0x7ffff : x) >> 19;
        tile_y = (y < 0 ? y + 0x7ffff : y) >> 19;

        previous = layer->previous_x;
        if (((previous ^ x) & 0x80000) != 0) {
            if (previous < x)
                Func_0800ff54(index, tile_x + 30, tile_y);
            else
                Func_0800ff54(index, tile_x, tile_y);
        }

        previous = layer->previous_y;
        if (((previous ^ y) & 0x100000) != 0) {
            if (previous < y)
                Func_0800fec8(index, tile_x, tile_y + 20);
            else
                Func_0800fec8(index, tile_x, tile_y);
        }

        Data_03001ad0.low[(2 - index) * 2] = (s16)(x >> 16);
        Data_03001ad0.low[(2 - index) * 2 + 1] = (s16)(y >> 16);

        layer->previous_x = x;
        layer->previous_y = y;
        layer++;
    }
}
