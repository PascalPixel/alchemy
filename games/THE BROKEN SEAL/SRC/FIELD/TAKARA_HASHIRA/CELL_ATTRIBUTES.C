#include "TYPES.H"

struct MapCell {
    u32 tile : 12;
    u32 layer : 2;
    u32 kind : 2;
    u32 height : 8;
    u32 event : 8;
};

struct MapLayer {
    struct MapCell *cells;
    u8 pad04[44];
};

struct MapState {
    u8 pad000[0x130];
    struct MapLayer layers[1];
};

/* Copy one map cell's attribute fields (all but the tile) from src into the
 * cell at x, y of a map layer. */
void TakaraHashira_SetCellAttributes(s32 layer, s32 x, s32 y, struct MapCell *src)
{
    struct MapState *map = *(struct MapState **)0x03001e70;

    if (map != 0) {
        struct MapCell *cell = map->layers[layer].cells;

        cell += x + (y << 7);

        cell->layer = src->layer;
        /* FAKEMATCH: the kind is read as the top of the source byte, not
         * through the bitfield, as the reference loads it. */
        cell->kind = ((u8 *)src)[1] >> 6;
        cell->height = src->height;
        cell->event = src->event;
    }
}
