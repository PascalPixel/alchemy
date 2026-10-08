#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "FIXED_POINT_POSITION.H"
#include "MAP_SCROLL.H"
#include "MAPFRAME.H"
#include "BTLDISP.H"
#include "PROJECT.H"
#include "TYPES.H"
#include "FRAME.H"
#include "RAM_BUFFER.H"
#include "GLOBAL_CELLS.H"
#include "IWRAM_CALL.H"
#include "FIXED_MATH.H"
#include "TRANSFORM.H"

extern void *gWorkSlot[];

#define Camera_StoreSceneParameters ((void (*)(s32,s32,s32))Camera_StoreSceneParameters)
#define SceneTransform_ApplyPosition ((void (*)(s32*))SceneTransform_ApplyPosition)

static __inline__ void CopyEntry(u8 *map, u8 *destination)
{
    u32 palette = *(u16 *)map;
    u16 *colors = (u16 *)Ram_MapCellBuffer;

    colors += palette * 2;
    *(u16 *)destination = *colors++;
    *(u16 *)(destination + 64) = *colors;
}

u32 Random16(void);

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

void WorldMap_UpdateView(void)
{
    /* FAKEMATCH: retain the existing slot 12-relative cell reads. Loading slots 7/8 independently in 1a5404f7 added eight bytes in all editions. */
    void **slot = (void **)&gCameraWork;
    struct BattleCamera *cam = slot[0];
    struct MapFrameWork *map = slot[-5];
    struct PerspectiveWork *view = slot[-4];
    s32 *pos = cam->pos;
    s32 *target = view->origin;
    struct MapAffinePair (*buffer)[160] = map->pages;
    s32 distance = view->far_plane;
    s32 height = view->distance;
    struct FixedPointPosition local;

    Map_UpdateCurrentTileBlockUntilBlocked();
    if (target != NULL) {
        s32 x;
        s32 y;
        s32 col;
        s32 row;

        y = target[2];
        x = target[0];
        if (view->shake_x != 0) {
            s32 r = Random16();
            s32 amp;

            r -= Random16();
            amp = view->shake_x;
            x += Iwram_MulQ16(amp, r);
            view->shake_x = Iwram_MulQ16(amp, view->shake_decay);
        }
        if (view->shake_y != 0) {
            s32 r = Random16();
            s32 amp;

            r -= Random16();
            amp = view->shake_y;
            y += Iwram_MulQ16(amp, r);
            view->shake_y = Iwram_MulQ16(amp, view->shake_decay);
        }
        col = x / 0x100000;
        row = y / 0x100000;
        if ((view->view_x ^ x) & 0x100000) {
            if (view->view_x < x)
                Map_RenderPaletteMappedColumn(col + 16);
            else
                Map_RenderPaletteMappedColumn(col - 16);
        }
        if ((view->view_y ^ y) & 0x100000) {
            if (view->view_y < y)
                Map_RenderPaletteMappedRow(row + 12);
            else
                Map_RenderPaletteMappedRow(row - 18);
        }
        view->view_x = x;
        view->view_y = y;
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
    local.x = 0;
    local.y = 0;
    local.z = height + 0x10000;
    Iwram_TransformVector((s32 *)&local, (s32 *)cam);
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork((u8 *)cam, pos);
    if (gScanlinePitch != view->pitch) {
        s32 c = Trig_Cos(view->pitch);
        s32 s = Trig_Sin(view->pitch);

        WorldMap_BuildScanlineTable(Iwram_RatioMulQ14(c, s), pos, map->scanlines);
        gScanlineAngle = 0;
        gScanlinePitch = view->pitch;
    }
    ((s32 (*)(void *, s32 *, void *, void *))gWorkSlot[46])(cam, pos, map->scanlines, buffer[gFrameCount & 1]);
}


void Map_UpdateCurrentTileBlock(void)
{
    struct PerspectiveWork *window = gMapWork[0];
    s32 x0 = 0;
    s32 y0 = 0;
    u32 layer;
    u32 row;
    u32 col;
    s32 bias;
    s32 tile;

    if (window->origin != NULL) {
        s32 *p = window->origin;

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
                tile = window->tile_ids[tile];
                tile += bias;
                Map_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 1);
            }
        }
        bias += 320;
    }
}

void Map_UpdateCurrentTileBlockUntilBlocked(void)
{
    struct PerspectiveWork *window = gMapWork[0];
    s32 x0 = 0;
    s32 y0 = 0;
    u32 layer;
    u32 row;
    u32 col;
    s32 tile;

    if (window->origin != NULL) {
        s32 *p = window->origin;

        x0 = *p++;
        y0 = p[1];
    }
    x0 = (x0 - 0x1000000) >> 25;
    y0 = (y0 - 0x1400000) >> 25;
    for (layer = 0; layer < 2; layer++) {
        for (row = 0; row < 2; row++) {
            for (col = 0; col < 2; col++) {
                tile = (((y0 + row) & 15) << 4) + ((x0 + col) & 15);
                tile = window->tile_ids[tile];
                tile += layer * 320;
                if (Map_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 0) != 0)
                    return;
            }
        }
    }
}
