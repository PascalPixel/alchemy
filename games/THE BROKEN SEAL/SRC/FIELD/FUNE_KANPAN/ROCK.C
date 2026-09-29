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

extern struct MapWork *gMapWork;

/* The deck's work, laid out in order just past the overlay's image: the
 * phases of the four drifting slots and the slot work the deck scenes keep,
 * the two wave angles, the scroll of layer 5 and its speed. */
s32 FuneKanpan_SlotPhase[2] = { 0 };
s32 FuneKanpan_WaveAngleY = 0;
s32 FuneKanpan_SlotWork[5] = { 0 };
s32 FuneKanpan_LayerScroll[2] = { 0 };
s32 FuneKanpan_WaveAngleX = 0;
s32 FuneKanpan_DeckSpare = 0;
s32 FuneKanpan_LayerSpeed[2] = { 0 };

/* Rocks the ship's deck: sways the camera by the cosine and sine of two
 * slowly, randomly advancing angles and scrolls map layer 5 by its speed,
 * wrapping the scroll within two cells. */
void FuneKanpan_RockDeck(void)
{
    struct MapWork *map = gMapWork;
    s32 *camera = map->camera;
    s32 dx = Engine_MathCos(FuneKanpan_WaveAngleX);
    s32 dy = Engine_MathSin(FuneKanpan_WaveAngleY);
    struct MapLayer *layer;

    *camera++ += dx >> 1;
    *camera += dy;
    FuneKanpan_WaveAngleX += (u32)(Engine_RandomNext() * 3 << 7) >> 16;
    {
        s32 turn = FuneKanpan_WaveAngleY + ((u32)(Engine_RandomNext() << 9) >> 16);

        FuneKanpan_WaveAngleX &= 0xffff;
        FuneKanpan_WaveAngleY = turn & 0xffff;
    }
    layer = &map->layers[5];
    layer->x = FuneKanpan_LayerScroll[0];
    FuneKanpan_LayerScroll[0] -= FuneKanpan_LayerSpeed[0];
    if (FuneKanpan_LayerScroll[0] < 0) {
        FuneKanpan_LayerScroll[0] += 0x200000;
    }
    if (FuneKanpan_LayerScroll[0] > 0x200000) {
        FuneKanpan_LayerScroll[0] -= 0x200000;
    }
    layer->y = FuneKanpan_LayerScroll[1];
    FuneKanpan_LayerScroll[1] -= FuneKanpan_LayerSpeed[1];
    if (FuneKanpan_LayerScroll[1] < 0) {
        FuneKanpan_LayerScroll[1] += 0x200000;
    }
}
