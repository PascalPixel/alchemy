#include "types.h"

typedef void (*Transfer_080bd850)(void *, u32);

struct Sprite_080bd850 {
    u8 unknown_00[28];
    u8 shape;
    u8 unknown_1d[3];
    u8 width;
    u8 height;
};

struct ShapeEntry_080bd850 {
    u16 unknown_00;
    u16 tile_offset;
};

extern struct ShapeEntry_080bd850 Data_03001b10[];

void Func_080072f0(void *destination, u32 count, Transfer_080bd850 transfer);

/*
 * Upload one sprite's tiles.  The shape index selects a tile offset from the
 * runtime shape table, which places the destination inside the second object
 * VRAM bank, and the sprite's width times its height gives the byte count.
 * The transfer itself is the IWRAM-relocated ARM helper at 0x03000164, reached
 * through the call-via-r3 slot.
 * Called from 0x080bdf36.
 */
void Func_080bd850(struct Sprite_080bd850 *sprite)
{
    void *destination =
        (void *)(0x06010000 + Data_03001b10[sprite->shape].tile_offset);

    Func_080072f0(destination, (u32) (sprite->width * sprite->height),
                  (Transfer_080bd850) 0x03000164);
}
