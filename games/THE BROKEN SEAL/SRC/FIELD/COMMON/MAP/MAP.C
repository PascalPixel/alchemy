#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "RAM_BUFFER.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"
#include "MAP_SCROLL.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"
#include "RUNTIME_MEM.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
void Map_SetCameraCenter(s32 x, s32 y);

/* FAKEMATCH: helper scope gives each cell its own table-pointer lifetime,
   following the independently matched neighbouring column renderer. */
static __inline__ void CopyCell(u32 *map, u8 *base, u32 rowmod, u32 colmod)
{
    u32 *tiles;
    u8 *dest;
    u32 index = (*map << 20) >> 19;

    tiles = (u32 *)Ram_MapBlocks;
    tiles += index;
    dest = base + (rowmod + colmod) * 2;
    *(u32 *)dest = *tiles;
    tiles = (u32 *)(Ram_MapBlocks + 4);
    tiles += index;
    *(u32 *)(dest + 64) = *tiles;
}

static __inline__ void CopyBlock(u32 *map, u8 *base, u32 rowmod, u32 colmod, u32 parity)
{
    u16 *colors;
    u8 *destination;
    u32 index = ((*map << 20) >> 18) + parity;

    colors = (u16 *)Ram_MapBlocks;
    colors += index;
    destination = base + (rowmod + colmod + parity) * 2;
    *(u16 *)destination = *colors;
    colors = (u16 *)(Ram_MapBlocks + 4);
    colors += index;
    *(u16 *)(destination + 64) = *colors;
}

extern struct MapRenderWork *gMapWork;
void Map_UpdateLayerScroll(void);

static __inline__ void CopyCameraCell(u32 *map, u8 *base, u32 rowmod, u32 colmod)
{
    u32 *tiles;
    u8 *dest;
    u32 index = (*map << 20) >> 19;

    tiles = (u32 *)Ram_MapBlocks;
    tiles += index;
    /* FAKEMATCH: integer address addition preserves the add operand order. */
    dest = (u8 *)((rowmod + colmod) * 2 + (u32)base);
    *(u32 *)dest = *tiles;
    tiles = (u32 *)(Ram_MapBlocks + 4);
    tiles += index;
    *(u32 *)(dest + 64) = *tiles;
}

/* A layered scene's resources, as rows counted from the first field map:
   the map itself, then the palette, the three tile sheets and the layer
   data. */
struct SceneEntry {
    u16 resources[6];
};

struct SceneLayerSource {
    u8 x;
    u8 y;
    s8 scroll_x;
    s8 scroll_y;
    s8 speed_x;
    s8 speed_y;
    u8 period_x;
    u8 period_y;
};

/* The head of a field map resource (SRC/MAP/MAP.INC). */
struct SceneHeader {
    u8 origin[4];
    u8 priority[3];
    u8 screen[3];
    u16 unknown_0a;
    struct SceneLayerSource layers[3];
    s32 tiles;
    s32 collision;
    s32 tilemap;
    s32 animation;
    s32 blend;
    s32 script;
};

struct SceneLayer {
    s32 x;
    s32 y;
    s32 base_x;
    s32 base_y;
    s32 scroll_x;
    s32 scroll_y;
    s32 speed_x;
    s32 speed_y;
    s32 phase_x;
    s32 phase_y;
    u32 period_x : 16;
    u32 period_y : 16;
    u32 *cells;
};

struct SceneWork {
    u8 unknown_00[0x10];
    u8 *script;
    u32 blend_control : 16;
    u32 unknown_16 : 16;
    u8 unknown_18[0xcc];
    s32 scale_x;
    s32 scale_y;
    s32 origin[4];
    u8 unknown_fc[4];
    u8 priority[3];
    u8 unknown_103;
    struct SceneLayer layers[3];
};

extern struct SceneEntry Map_LayeredScenes[];
extern u8 gMapLayerData[];

void Blend_SetDarkenTarget0(s32 value);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_DecodeType2(const void *source, void *destination);
void Tilemap_DecodeStagedBuffer(void);
void Tilemap_ConvertBuffer(void);
void MapAnimation_StartChannels(void *channels);
void DisplayBlend_StartScript(void *script);
s32 GameFlag_IsSet(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
void Runtime_BumpFree(void *allocation);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 key);

/* Iwram_Call2 (IWRAM_CALL.H) with r3 among the registers the routine may
   change; the ROM keeps nothing in r3 across the two layer multiplies. */
static __inline__ s32 Scene_Call2(s32 left, s32 right, void *routine)
{
    /* FAKEMATCH: the call's operands in their registers, as Iwram_Call2. */
    register s32 result __asm__("r0") = left; /* FAKEMATCH: as above. */
    register s32 factor __asm__("r1") = right;

    /* FAKEMATCH: the same call, with r3 listed as changed. */
    __asm__ volatile(".align 2\n\tmov ip, pc\n\tbx %2\n\t"
                     : "+r"(result)
                     : "r"(factor), "r"(routine)
                     : "r1", "r2", "r3", "ip", "cc");
    return result;
}

/* Loads a layered scene: decodes the map's cells, collision and tilemap and
   starts its optional tile animation and blend script, sets the three
   layers' origins, scroll rates and periods, writes the layers' background
   controls, and, unless the flag that keeps the current pictures is set,
   decodes the scene's palette, tile sheets and layer data. */
s32 Map_LoadLayeredScene(s32 index)
{
    struct SceneEntry *entry;
    struct SceneWork *work;
    struct SceneHeader *header;
    struct SceneLayer *layer;
    struct SceneLayerSource *source;
    s32 *scale_x;
    s32 *scale_y;
    s32 i;
    u8 *buf;
    s32 backdrop;
    u8 *src;
    s32 cnt;

    *(volatile u16 *)0x04000000 &= 0xc1ff;
    Blend_SetDarkenTarget0(0);
    entry = &Map_LayeredScenes[index];
    work = Runtime_AllocateBlock(8, sizeof(struct SceneWork));
    Iwram_ClearWords(work, sizeof(struct SceneWork));
    header = (struct SceneHeader *)Resource_GetTableEntry(entry->resources[0] + (u32)&ResourceId_Map001);
    src = (u8 *)header + header->tiles;
    Resource_DecodeType01(src, Ram_MapCellBuffer + 1);
    Tilemap_DecodeStagedBuffer();
    src = (u8 *)header + header->collision;
    Resource_DecodeType01(src, Ram_MapCollision);
    src = (u8 *)header + header->tilemap;
    Resource_DecodeType01(src, Ram_MapCellBuffer);
    Tilemap_ConvertBuffer();
    /* FAKEMATCH: the optional blocks' offsets pass through the pointer
       variable, which keeps them in r0 as the ROM does. */
    src = (u8 *)header->animation;
    if (src != 0) {
        Resource_DecodeType01((u8 *)header + (s32)src, Ram_MapCollision + 0x1000);
        MapAnimation_StartChannels(Ram_MapCollision + 0x1000);
    }
    src = (u8 *)header->blend;
    if (src != 0) {
        Resource_DecodeType01((u8 *)header + (s32)src, Ram_MapCollision + 0x1e00);
        DisplayBlend_StartScript(Ram_MapCollision + 0x1e00);
    }
    work->script = (u8 *)header + header->script;
    work->origin[0] = header->origin[0] << 19;
    work->origin[1] = header->origin[1] << 19;
    work->origin[2] = header->origin[2] << 19;
    work->origin[3] = header->origin[3] << 19;
    scale_x = &work->scale_x;
    *scale_x = 0;
    scale_y = &work->scale_y;
    *scale_y = 0;
    work->priority[0] = header->priority[0];
    work->priority[1] = header->priority[1];
    work->priority[2] = header->priority[2];
    layer = work->layers;
    source = header->layers;
    for (i = 0; i < 3; i++) {
        u32 x = source->x;
        u32 y = source->y;
        s32 scroll_x;
        s32 scroll_y;
        s32 base_x;
        s32 base_y;

        layer->base_x = base_x = x << 19;
        layer->base_y = base_y = y << 19;
        layer->speed_x = source->speed_x << 12;
        layer->speed_y = source->speed_y << 12;
        layer->period_x = source->period_x;
        layer->period_y = source->period_y;
        layer->phase_x = 0;
        layer->phase_y = 0;
        scroll_x = source->scroll_x << 12;
        scroll_y = source->scroll_y << 12;
        layer->scroll_x = scroll_x;
        layer->scroll_y = scroll_y;
        layer->cells = (u32 *)Ram_MapCellBuffer + (y >> 1) * 128 + (x >> 1);
        layer->x = Scene_Call2(*scale_x, scroll_x, IwramMulQ16ReturnIp) + base_x;
        layer->y = Scene_Call2(*scale_y, scroll_y, IwramMulQ16ReturnIp) + base_y;
        source++;
        layer++;
    }
    work->blend_control = 0x1000;
    if (work->priority[0] != 0)
        work->blend_control = 0x1800;
    if (work->priority[1] != 0)
        work->blend_control |= 0x400;
    if (work->priority[2] != 0)
        work->blend_control |= 0x200;
    cnt = work->priority[0] | (header->screen[0] << 2) | 0x500;
    *(volatile u16 *)0x0400000e = cnt;
    /* FAKEMATCH: one-pass loops hold the second and third background
       control writes in source order. */
    do {
        cnt = work->priority[1] | (header->screen[1] << 2) | 0x600;
        *(volatile u16 *)0x0400000c = cnt;
    } while (0);
    do {
        cnt = work->priority[2] | (header->screen[2] << 2) | 0x700;
        *(volatile u16 *)0x0400000a = cnt;
    } while (0);
    if (GameFlag_IsSet(0x170) != 0) {
        GameFlag_ClearBitFar(0x170);
    } else {
        buf = (u8 *)Runtime_BumpAllocate(0x4000);
        if (buf != NULL) {
            backdrop = *(s16 *)0x05000000;
            Resource_DecodeType01(Resource_GetTableEntry(entry->resources[1] + (u32)&ResourceId_Map001), buf);
            *(s16 *)buf = backdrop;
            Iwram_CopyWords((void *)0x05000000, buf, 0x1c0);
            Resource_DecodeType2(Resource_GetTableEntry(entry->resources[2] + (u32)&ResourceId_Map001), buf);
            Iwram_CopyWords((void *)0x06004000, buf, 0x4000);
            Resource_DecodeType2(Resource_GetTableEntry(entry->resources[3] + (u32)&ResourceId_Map001), buf);
            Iwram_CopyWords((void *)0x06008000, buf, 0x4000);
            Resource_DecodeType2(Resource_GetTableEntry(entry->resources[4] + (u32)&ResourceId_Map001), buf);
            Iwram_CopyWords((void *)0x0600c000, buf, 0x4000);
            Resource_DecodeType2(Resource_GetTableEntry(entry->resources[5] + (u32)&ResourceId_Map001), gMapLayerData);
            Runtime_BumpFree(buf);
        }
    }
    cnt = 0;
    *(volatile u16 *)0x0400004c = cnt;
    *(volatile u16 *)0x04000050 = cnt;
    cnt = 0x140;
    *(volatile u16 *)0x04000000 = cnt;
    Scheduler_AddOrUpdateCallback(Map_UpdateLayerScroll, 0xc85);
    return 2;
}

void Map_ApplyWorkOriginAndSpan(void)
{
    s32 first;
    s32 third;
    s32 second;
    s32 *p;

    p = **(s32 ***)((u32)&gCam);
    first = 0;
    second = 0;
    third = 0;
    if (p != NULL) {
        first = *p++;
        second = *p++;
        third = *p;
    }
    Map_SetCameraCenter(first, (s32)((u32)third - (u32)second));
    Map_UpdateLayerScroll();
}

/* Exact candidate: whole owner [0800fec8, 0800ff54), 140 bytes, four own pool
   words. Baseline 2026-09-26: 144 bytes, 64 differing halfwords, 59
   aligned edits. No callees. Raw caller 08010000 selects this row update
   on a vertical scroll boundary and ff54 on a horizontal boundary.
   H1: use the adopted ff54 family's typed per-cell helper and explicit
   outer row/base lifetimes, with paired word stores for this row owner.
   Prediction: only r8/sl saved, base in ip, all four pools reloaded where
   the reference owns them. Accept only exact 140 bytes plus landing gates.
   H1 result: exact 140/140 bytes, zero differing halfwords/aligned edits,
   topology equal; complete normalized comparison read. The same-source
   family evidence is the adopted ff54 implementation, not another project.
   The ff54 owner is unchanged. */
void Map_RenderMetatileRow(u32 a0, s32 a1, s32 a2)
{
    u8 *dest = (u8 *)(0x06002800 + (a0 << 11));
    u32 row = ((a2 / 2) & 0x7F) << 7;
    u32 rowmod = (a2 & 30) << 5;
    u32 col = (a1 / 2) & 0x7F;
    u32 colmod = a1 & 30;
    u32 counter;

    for (counter = 0; counter <= 15; counter++) {
        u32 *map = (u32 *)Ram_MapCellBuffer;

        map += row + col;
        CopyCell(map, dest, rowmod, colmod);
        col = (col + 1) & 0x7F;
        colmod = (colmod + 2) & 30;
    }
}

void Map_RenderPaletteMappedBlock(u32 a0, s32 a1, s32 a2)
{
    u8 *destination = (u8 *)(0x06002800 + (a0 << 11));
    u32 row = ((a2 / 2) & 0x7F) << 7;
    u32 rowmod = (a2 & 30) << 5;
    u32 col = (a1 / 2) & 0x7F;
    u32 colmod = a1 & 30;
    u32 parity = a1 & 1;
    u32 counter;

    for (counter = 0; counter <= 10; counter++) {
        u32 *map = (u32 *)Ram_MapCellBuffer;

        map += row + col;
        CopyBlock(map, destination, rowmod, colmod, parity);

        row = (row + 128) & 0x3F80;
        rowmod = (rowmod + 64) & 0x3C0;
    }
}

void Map_UpdateLayerScroll(void)
{
    struct MapScrollWork *work;
    struct MapLayerScroll *layer;
    s32 *origin;
    s32 x, y;
    s32 min_x, min_y, max_x, max_y;
    s32 base_x, base_y;
    u32 i;

    work = *(struct MapScrollWork **)&gMapWork;
    origin = work->origin;
    layer = work->layers;
    if (origin == 0)
        return;

    base_x = *origin++ - 0x780000;
    {
        s32 height = *origin++;
        base_y = *origin - height - 0x600000;
    }
    min_x = work->min_x + work->shake_x;
    max_x = work->max_x - work->shake_x - 0xf00000;
    min_y = work->min_y + work->shake_y;
    max_y = work->max_y - work->shake_y - 0xa00000;
    if (min_x > max_x)
        max_x = min_x;
    if (min_y > max_y)
        max_y = min_y;
    if (base_x < min_x)
        base_x = min_x;
    if (base_x > max_x)
        base_x = max_x;
    if (base_y < min_y)
        base_y = min_y;
    if (base_y > max_y)
        base_y = max_y;

    if (work->shake_x != 0) {
        s32 first = Random16();
        s32 second = Random16();
        s32 amplitude = work->shake_x;
        base_x += Iwram_MulQ16(amplitude, first - second);
        work->shake_x = Iwram_MulQ16(amplitude, work->shake_decay);
    }
    if (work->shake_y != 0) {
        s32 first = Random16();
        s32 second = Random16();
        s32 amplitude = work->shake_y;
        base_y += Iwram_MulQ16(amplitude, first - second);
        work->shake_y = Iwram_MulQ16(amplitude, work->shake_decay);
    }
    work->view_x = base_x;
    work->view_y = base_y;
    for (i = 0; i < 3; i++, layer++) {
        base_x = Iwram_MulQ16(work->view_x, layer->scale_x);
        base_y = Iwram_MulQ16(work->view_y, layer->scale_y);
        if (layer->speed_x != 0) {
            layer->phase_x += layer->speed_x;
            base_x = (base_x + layer->phase_x) &
                (((u32)layer->mask_x << 19) | 0x7ffff);
        }
        if (layer->speed_y != 0) {
            layer->phase_y += layer->speed_y;
            base_y = (base_y + layer->phase_y) &
                (((u32)layer->mask_y << 19) | 0x7ffff);
        }
        base_x += layer->offset_x;
        base_y += layer->offset_y;
        x = base_x / 0x80000;
        y = base_y / 0x80000;
        if ((layer->x ^ base_x) & 0x80000) {
            if (layer->x < base_x)
                Map_RenderPaletteMappedBlock(i, x + 30, y);
            else
                Map_RenderPaletteMappedBlock(i, x, y);
        }
        if ((layer->y ^ base_y) & 0x100000) {
            if (layer->y < base_y)
                Map_RenderMetatileRow(i, x, y + 20);
            else
                Map_RenderMetatileRow(i, x, y);
        }
        gBgScroll[3 - i].x = base_x >> 16;
        gBgScroll[3 - i].y = base_y >> 16;
        layer->x = base_x;
        layer->y = base_y;
    }
}

void Map_SetCameraCenter(s32 x, s32 y)
{
    u32 no;
    u8 *dest;
    struct MapScrollWork *work = gCam;
    struct MapLayerScroll *layer = work->layers;

    x -= 0x780000;
    y -= 0x600000;
    if (x < work->min_x)
        x = work->min_x;
    if (x > work->max_x - 0xf00000)
        x = work->max_x - 0xf00000;
    if (y < work->min_y)
        y = work->min_y;
    if (y > work->max_y - 0xa00000)
        y = work->max_y - 0xa00000;
    work->view_x = x;
    work->view_y = y;

    for (no = 0; no < 3; no++) {
        if (work->enabled[no]) {
            u32 rows = 22;
            u32 row, col, rowmod, colmod, i, j;

            x = Iwram_MulQ16(work->view_x, layer->scale_x);
            y = Iwram_MulQ16(work->view_y, layer->scale_y);
            if (layer->speed_x) {
                layer->phase_x += layer->speed_x;
                x = (x + layer->phase_x) & ((layer->mask_x << 19) | 0x7ffff);
            }
            if (layer->speed_y) {
                layer->phase_y += layer->speed_y;
                y = (y + layer->phase_y) & ((layer->mask_y << 19) | 0x7ffff);
                rows = 32;
            }
            x += layer->offset_x;
            y += layer->offset_y;
            layer++;
            x /= 0x80000;
            y /= 0x80000;
            dest = (u8 *)(0x06002800 + (no << 11));
            row = ((y / 2) & 127) << 7;
            rowmod = (y & 30) << 5;
            for (i = 0; i < rows / 2; i++) {
                col = (x / 2) & 127;
                colmod = x & 30;
                for (j = 0; j <= 15; j++) {
                    u32 *map = (u32 *)Ram_MapCellBuffer;
                    map += row + col;
                    CopyCameraCell(map, dest, rowmod, colmod);
                    col = (col + 1) & 127;
                    colmod = (colmod + 2) & 30;
                }
                row = (row + 128) & 0x3f80;
                rowmod = (rowmod + 64) & 0x3c0;
            }
        }
    }
}
