/* Draft, differ 44 of 57 (register choice): the native keeps the timer in r3 and the offsets and the mask 7 in r2, this draft the other way round. The goto loop keeps the offsets and the counter in r8 as the native has them (a do-while hoists them, differ 60). The same update is overlay 6A8's Func_02004114. Permute 1 minute: differ 36 only through an inline helper. */
#include "../../../../games/tla/src/field/overlay_68f/ov68f.h"

struct Spark {
    s32 position[3];
    s32 velocity[3];
    s32 timer;
};

struct SparkSprite {
    u8 unknown_00[8];
    u16 tile : 10;
    u16 unknown_08_10 : 2;
    u16 unknown_08_12 : 4;
    u8 unknown_0a[0x1e];
};

struct SparkWork {
    u8 unknown_0000[0x500];
    struct Spark sparks[64];
    struct SparkSprite sprites[64];
    s16 next;
    u16 tile_base;
    s16 resource;
};

void Func_02003198(struct SparkSprite *sprite, struct Spark *spark);
void Func_020031a0(struct Spark *spark, s32 a, s32 b);

void Func_02002cc0(void)
{
    struct SparkWork *work = Ram_HeapSlots->menu_runtime;
    struct Spark *spark = work->sparks;
    struct SparkSprite *sprite = work->sprites;
    s32 i = 63;

loop:
        if ((u32)spark->timer < 20) {
            sprite->tile = work->tile_base + ((spark->timer >> 2) & 7) * 8;
            Func_02003198(sprite, spark);
            Func_020031a0(spark, 63, -0x8000);
            spark->timer++;
        }
        i--;
        sprite++;
        spark++;
    if (i >= 0)
        goto loop;
}
