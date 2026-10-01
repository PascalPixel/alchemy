/* NONMATCHING: main [080113e4,080114a0), 188 bytes with its pool.
 * 2026-10-01 (☀️ matcher 1): rewritten as plain C; permuter score 1163
 * (the previous permuter-built body scored 1518), 45 aligned differing
 * lines. Passing y0 + row and x0 + col straight to the call lets loop.c
 * strength-reduce the row coordinate and copy it for the call (the
 * reference's mov r8, r6), and the layer/row/column structure, the stack
 * slots for the window, x0 and y0 and every pool word line up.
 * Remaining: the reference loads 15 twice inside the loops (movs r2, #15
 * before each and), but here loop.c hoists one shared 15 into a callee-saved
 * register, which pushes the row copy into r4 and a spill slot (frame 20
 * instead of 16) and moves the column increment after the call. A row-level
 * base offset, u32 coordinates, % 16, a two-dimensional tile array, a tile
 * temporary and the bias added first do not change it (45 to 55); a 240 s
 * permuter run reached 694 only through temporaries and register hints. */
#include "TYPES.H"

struct MapPosition_080113e4 {
    s32 x;
    s32 y;
    s32 z;
};

struct MapTileWindow_080113e4 {
    struct MapPosition_080113e4 *position;
    u8 unknown_004[0x134];
    u16 tiles[256];
};

extern struct MapTileWindow_080113e4 *gMapWork;

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);

/* Redraws the 2 x 2 cells around the camera's position on both layers, the
   second layer's tiles 320 on from the first's. */
void Map_UpdateCurrentTileBlock(void)
{
    struct MapTileWindow_080113e4 *window = gMapWork;
    s32 x0 = 0;
    s32 y0 = 0;
    u32 layer;
    u32 row;
    u32 col;
    s32 bias;

    if (window->position != NULL) {
        s32 *p = &window->position->x;

        x0 = *p++;
        y0 = p[1];
    }
    x0 = (x0 - 0x1000000) >> 25;
    y0 = (y0 - 0x1400000) >> 25;
    bias = 0;
    for (layer = 0; layer < 2; layer++) {
        for (row = 0; row < 2; row++) {
            for (col = 0; col < 2; col++) {
                Map_WriteLayerCellTile(layer, x0 + col, y0 + row,
                    window->tiles[(((y0 + row) & 15) << 4) + ((x0 + col) & 15)] + bias, 1);
            }
        }
        bias += 320;
    }
}
