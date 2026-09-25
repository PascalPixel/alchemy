#include "TYPES.H"
#include "FIELD_EVENT.H"

struct MapLayer {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    u8 unknown_10[32];
};

struct MapWork {
    s32 *camera;
    u8 unknown_04[16];
    struct MapLayer layers[8];
};

extern struct MapWork *Data_03001e70;
/* The two wave angles, the scroll of layer 5 and its speed. */
extern s32 Data_0200db58;
extern s32 Data_0200db38;
extern s32 Data_0200db50[2];
extern s32 Data_0200db60[2];

/* Rocks the ship's deck: sways the camera by the cosine and sine of two
 * slowly, randomly advancing angles and scrolls map layer 5 by its speed,
 * wrapping the scroll within two cells. */
void FuneKanpan_RockDeck(void)
{
    struct MapWork *map = Data_03001e70;
    s32 *camera = map->camera;
    s32 dx = Engine_MathCos(Data_0200db58);
    s32 dy = Engine_MathSin(Data_0200db38);
    struct MapLayer *layer;

    *camera++ += dx >> 1;
    *camera += dy;
    Data_0200db58 += (u32)(Engine_RandomNext() * 3 << 7) >> 16;
    {
        s32 turn = Data_0200db38 + ((u32)(Engine_RandomNext() << 9) >> 16);

        Data_0200db58 &= 0xffff;
        Data_0200db38 = turn & 0xffff;
    }
    layer = &map->layers[5];
    layer->x = Data_0200db50[0];
    Data_0200db50[0] -= Data_0200db60[0];
    if (Data_0200db50[0] < 0) {
        Data_0200db50[0] += 0x200000;
    }
    if (Data_0200db50[0] > 0x200000) {
        Data_0200db50[0] -= 0x200000;
    }
    layer->y = Data_0200db50[1];
    Data_0200db50[1] -= Data_0200db60[1];
    if (Data_0200db50[1] < 0) {
        Data_0200db50[1] += 0x200000;
    }
}
