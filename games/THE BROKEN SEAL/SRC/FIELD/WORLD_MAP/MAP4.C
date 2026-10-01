#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "GLOBAL_CELLS.H"
#include "IWRAM_CALL.H"

static __inline__ void CopyEntry(u8 *map, u8 *destination)
{
    u32 palette = *(u16 *)map;
    u16 *colors = (u16 *)Ram_MapCellBuffer;

    colors += palette * 2;
    *(u16 *)destination = *colors++;
    *(u16 *)(destination + 64) = *colors;
}

extern u8 gCameraWork[];

struct WorldTarget {
    s32 x;
    s32 z;
    s32 y;
};

struct WorldView {
    struct WorldTarget *target;
    s32 shake_x;
    s32 shake_y;
    s32 decay;
    u8 unk_10[0xd4];
    s32 last_x;
    s32 last_y;
    u8 unk_ec[0x2c];
    u16 pitch;
    u16 yaw;
    u8 unk_11c[0x22c];
    s32 distance;
    s32 height;
};

struct WorldTransfer {
    s32 first;
    s32 second;
    s32 third;
};

struct WorldScreen {
    u8 unk_00[12];
    s32 center_x;
    s32 center_y;
};

extern struct WorldScreen gProjection;
extern u32 Data_03001af4;
extern u32 Data_03001f60;
extern u32 Data_03001e40;
extern void *Data_03001e50[];
u32 Random16(void);
s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);
void Map_UpdateCurrentTileBlockUntilBlocked(void);
void Map_RenderPaletteMappedRow(u32 value);
void Map_RenderPaletteMappedColumn(u32 value);
void Camera_StoreSceneParameters(s32 distance, s32 half, s32 twice);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void Graphics_PrepareTransferInIwramWork(u8 *source, s32 *destination);
void WorldMap_BuildScanlineTable(s32 value, s32 *position, u8 *map);

void Map_RenderPaletteMappedRow(u32 value)
{
    u8 *map;
    u8 *destination;
    u32 counter;

    map = Ram_MapBlocks + ((((s32)value / 2) & 31) << 7);
    destination = (u8 *)(0x06004000 + ((value & 62) << 6));
    counter = 0;
    do {
        CopyEntry(map, destination);
        counter++;
        destination += 2;
        map += 4;
    } while (counter <= 31);

    destination += 0xfc0;
    map += 0xf80;
    counter = 0;
    do {
        CopyEntry(map, destination);
        counter++;
        destination += 2;
        map += 4;
    } while (counter <= 31);
}

void Map_RenderPaletteMappedColumn(u32 value)
{
    u8 *map;
    u8 *destination;
    u32 counter;

    map = Ram_MapBlocks + ((((s32)value / 2) & 31) << 2);
    destination = (u8 *)(0x06004000 + (value & 62));
    counter = 0;
    do {
        CopyEntry(map, destination);
        counter++;
        destination += 128;
        map += 128;
    } while (counter <= 63);
}

/* Per-frame world map view: follow the target (with a decaying random
   shake), redraw the palette-mapped column or row the view crossed, then
   rebuild the camera transform and hand the frame to the renderer. */
void WorldMap_UpdateView(void)
{
    void **slot = (void **)((u32)&gCameraWork);
    u8 *cam = slot[0];
    u8 *map = slot[-5];
    struct WorldView *view = slot[-4];
    s32 *pos = (s32 *)(cam + 12);
    s32 *target = (s32 *)view->target;
    u8 *buffer = map + 0xc80;
    s32 distance = view->distance;
    s32 height = view->height;
    struct WorldTransfer local;

    Map_UpdateCurrentTileBlockUntilBlocked();
    if (target != NULL) {
        s32 x;
        s32 y;
        s32 col;
        s32 row;
        s32 *last;

        y = target[2];
        x = target[0];
        if (view->shake_x != 0) {
            s32 r = Random16();
            s32 amp;

            r -= Random16();
            amp = view->shake_x;
            x += Iwram_MulQ16(amp, r);
            view->shake_x = Iwram_MulQ16(amp, view->decay);
        }
        if (view->shake_y != 0) {
            s32 r = Random16();
            s32 amp;

            r -= Random16();
            amp = view->shake_y;
            y += Iwram_MulQ16(amp, r);
            view->shake_y = Iwram_MulQ16(amp, view->decay);
        }
        col = x / 0x100000;
        row = y / 0x100000;
        if ((view->last_x ^ x) & 0x100000) {
            if (view->last_x < x)
                Map_RenderPaletteMappedColumn(col + 16);
            else
                Map_RenderPaletteMappedColumn(col - 16);
        }
        if ((view->last_y ^ y) & 0x100000) {
            if (view->last_y < y)
                Map_RenderPaletteMappedRow(row + 12);
            else
                Map_RenderPaletteMappedRow(row - 18);
        }
        view->last_x = x;
        view->last_y = y;
    }
    gProjection.center_x = 120;
    gProjection.center_y = 96;
    Camera_StoreSceneParameters(distance, height / 2, height * 2);
    pos[0] = *target++;
    pos[1] = 0;
    pos[2] = target[1];
    Render_ResetTransformState();
    SceneTransform_ApplyPosition(pos);
    SceneTransform_ApplyYaw(view->yaw);
    SceneTransform_ApplyPitch(view->pitch);
    local.first = 0;
    local.second = 0;
    local.third = height + 0x10000;
    Iwram_TransformVector((s32 *)&local, (s32 *)cam);
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork(cam, pos);
    if (Data_03001af4 != view->pitch) {
        s32 c = Trig_Cos(view->pitch);
        s32 s = Trig_Sin(view->pitch);

        WorldMap_BuildScanlineTable(Iwram_RatioMulQ14(c, s), pos, map);
        Data_03001f60 = 0;
        Data_03001af4 = view->pitch;
    }
    ((s32 (*)(u8 *, s32 *, u8 *, u8 *))Data_03001e50[46])(cam, pos, map, buffer + (Data_03001e40 & 1) * 0x1400);
}

/* The world map's window on its tiles: the camera position and a 16 by 16
 * grid of tile numbers that wraps at its edges. */
struct WorldTilePosition {
    s32 x;
    s32 y;
    s32 z;
};

struct WorldTileWindow {
    struct WorldTilePosition *position;
    u8 unknown_004[0x134];
    u16 tiles[256];
};

extern struct WorldTileWindow *gMapWork;

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);

/* Redraws the 2 by 2 tiles around the camera's position on both layers,
 * the second layer's tiles 320 on from the first's. */
void Map_UpdateCurrentTileBlock(void)
{
    struct WorldTileWindow *window = gMapWork;
    s32 x0 = 0;
    s32 y0 = 0;
    u32 layer;
    u32 row;
    u32 col;
    s32 bias;
    s32 tile;

    if (window->position != NULL) {
        s32 *p = &window->position->x;

        x0 = *p++;
        y0 = p[1];
    }
    x0 = (x0 - 0x1000000) >> 25;
    y0 = (y0 - 0x1400000) >> 25;
    layer = 0;
    bias = 0;
    for (; layer < 2; layer++) {
        for (row = 0; row < 2; row++) {
            for (col = 0; col < 2; col++) {
                tile = (((y0 + row) & 15) << 4) + ((x0 + col) & 15);
                tile = window->tiles[tile];
                tile += bias;
                Map_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 1);
            }
        }
        bias += 320;
    }
}

/* Brings the same 2 by 2 tiles up to date without forcing them, and stops
 * at the first one that had to be drawn, so a frame draws at most one. */
void Map_UpdateCurrentTileBlockUntilBlocked(void)
{
    struct WorldTileWindow *window = gMapWork;
    s32 x0 = 0;
    s32 y0 = 0;
    u32 layer;
    u32 row;
    u32 col;
    s32 tile;

    if (window->position != NULL) {
        s32 *p = &window->position->x;

        x0 = *p++;
        y0 = p[1];
    }
    x0 = (x0 - 0x1000000) >> 25;
    y0 = (y0 - 0x1400000) >> 25;
    for (layer = 0; layer < 2; layer++) {
        for (row = 0; row < 2; row++) {
            for (col = 0; col < 2; col++) {
                tile = (((y0 + row) & 15) << 4) + ((x0 + col) & 15);
                tile = window->tiles[tile];
                tile += layer * 320;
                if (Map_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 0) != 0)
                    return;
            }
        }
    }
}
