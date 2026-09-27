/* Draft, not exact (2026-09-26): 880 of 868 bytes, 338 differing halfwords.
   Written from the listing; the entry table at 0x08013784 is the map load
   table in FIELD/COMMON/LOAD_TABLE.JSON (resource ids biased by 0x128, which
   the ROM loads from the pool, hence Value_00000128).
   What lined up: the call sequence, the three-layer loop body and its two
   Iwram_MulQ16 calls, the BGnCNT writes and the palette/VRAM bank copies.
   Remaining: the ROM keeps the entry pointer and both scale pointers on the
   stack (sub sp, #12), the loop counter in fp and 0x03000118 in r9, stores
   base_x before base_y (here base_x is sunk after the cell pointer), builds
   0x500 with movs/lsls instead of a pool word, and loads the unconditional
   resource offsets into r3 but the optional ones into r0.

   2026-09-26: complete owner [0800fb38,0800fe9c), 868 bytes including
   both own pools. Fresh baseline: 876 bytes, 332 differing halfwords,
   205 aligned edits, equal topology. The field far-call table at 09110
   targets this entry; the input selects a six-resource load-table row.
   H1 follows the audited f9f4 body: its signed size argument is the result
   of DecodeType01 at fb92, carried in r0 directly to the next call.
   Predict unchanged call adjacency with an explicit result dependency;
   inspect the full normalized difference and frame/pools. Exact 868 bytes
   plus compare/coverage/verify required. One trial for this correction,
   at most three distinct structural hypotheses and 25 minutes total.
   Preserve outcomes here and in commits; no argument-spelling sweep.
   H1 result: byte-identical to baseline, 876 bytes / 205 aligned edits.
   The missing source dependency is repaired without altering the adjacent
   machine calls; the remaining frame/loop/I/O disagreements are independent.
   H2: the layer initializer owns its source coordinates and temporary
   bases, as the matched metatile family's helper owns each cell. Predict
   earlier base-X storage and separate saved lifetimes for both scale
   pointers, restoring the 12-byte frame. Keep all declarations/calls
   outside that layer body unchanged; score once and read the full diff.
   H2 result: byte-identical to H1, still 876 bytes / 205 aligned edits.
   The helper does not change the eight-byte frame or the late base-X
   store. Complete normalized difference inspected; this scope axis is
   closed without further declaration or spelling variants.
   H3: independent named BG register objects and a typed final display
   register block own the halfword stores. Predict immediate 0500/0600/0700
   and 0140 construction, separate BG address loads, and the shared final
   4c/50/00 address walk. One trial, full normalized difference; then stop
   this owner regardless of score. No loop-lifetime changes in this trial.
   H3 result: 880 bytes, 338 differing halfwords, 197 aligned edits;
   conditional topology equal, full normalized difference inspected.
   Separate BG address loads appear, but 0500/0600/0700 remain pool words.
   The final stores build 0140 immediately but add volatile member reads
   and rematerialize the display base. The frame remains eight bytes.
   Three hypotheses complete: stop here. Prior 876-byte draft preserved
   in ed46b755c; no claim of an exact owner or credited bytes. */
#include "TYPES.H"
#include "IWRAM_CALL.H"

typedef void (*FillWordsFn)(void *destination, s32 size);
typedef void (*WordCopyFn)(void *destination, const void *source, s32 size);

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

struct SceneHeader {
    u8 origin[4];
    u8 layer_priority[3];
    u8 layer_screen[3];
    u8 unk_0a[2];
    struct SceneLayerSource layers[3];
    s32 tiles;
    s32 palette;
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
    u16 period_x;
    u16 period_y;
    u32 *cells;
};

struct SceneWork {
    u8 unk_00[0x10];
    u8 *script;
    u16 blend_control;
    u8 unk_16[0xce];
    s32 scale_x;
    s32 scale_y;
    s32 origin[4];
    u8 unk_fc[4];
    u8 priority[3];
    u8 unk_103;
    struct SceneLayer layers[3];
};

extern u8 Value_00000128;
extern struct SceneEntry Data_08013784[];

struct SceneRegister {
    u16 value;
};

struct SceneDisplay {
    u16 control;
    u8 unk_02[0x4a];
    u16 mosaic;
    u16 unk_4e;
    u16 blend;
};

extern volatile struct SceneRegister Data_0400000e;
extern volatile struct SceneRegister Data_0400000c;
extern volatile struct SceneRegister Data_0400000a;

/* FAKEMATCH: per-layer helper bounds the coordinate and base lifetimes. */
static __inline__ void InitializeLayer(struct SceneLayer *layer,
    struct SceneLayerSource *source, s32 *scale_x, s32 *scale_y)
{
    u32 x = source->x;
    u32 y = source->y;
    s32 base_x;
    s32 base_y;
    s32 scroll_x;
    s32 scroll_y;

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
    layer->cells = (u32 *)0x02010000 + (y >> 1) * 128 + (x >> 1);
    layer->x = Iwram_MulQ16(*scale_x, scroll_x) + base_x;
    layer->y = Iwram_MulQ16(*scale_y, scroll_y) + base_y;
}

void Blend_SetDarkenTarget0(s32 value);
void *Runtime_AllocateBlock(s32 slot, s32 size);
u8 *Resource_GetTableEntry(u32 index);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_DecodeType2(const void *source, void *destination);
void Tilemap_DecodeStagedBuffer(s32 size);
void Tilemap_ConvertBuffer(void);
void MapAnimation_StartChannels(void *channels);
void DisplayBlend_StartScript(void *script);
s32 GameFlag_TestFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
void *Runtime_BumpAllocate(u32 size);
void Runtime_BumpFree(void *allocation);
void Scheduler_AddOrUpdateCallback(void *callback, s32 key);
void Map_UpdateLayerScroll(void);

/*
 * Loads layered scene `index`: its tiles, map, animation and blend script
 * into the staging buffers, three scrolling BG layers (BG2CNT to BG0CNT),
 * and, unless flag 0x170 asks to keep them, a fresh palette and the three
 * VRAM tile banks. Map_UpdateLayerScroll then runs every frame.
 */
s32 Map_LoadLayeredScene(s32 index)
{
    struct SceneEntry *entry;
    struct SceneWork *work;
    struct SceneHeader *header;
    struct SceneLayer *layer;
    struct SceneLayerSource *source;
    s32 *scale_x;
    s32 *scale_y;
    s32 cnt;
    u8 *buf;
    s16 backdrop;

    *(volatile u16 *)0x04000000 &= 0xc1ff;
    Blend_SetDarkenTarget0(0);
    entry = &Data_08013784[index];
    work = Runtime_AllocateBlock(8, sizeof(struct SceneWork));
    ((FillWordsFn)0x03000164)(work, sizeof(struct SceneWork));
    header = (struct SceneHeader *)Resource_GetTableEntry(entry->resources[0] + (u32)&Value_00000128);
    Tilemap_DecodeStagedBuffer(
        Resource_DecodeType01((u8 *)header + header->tiles, (void *)0x02010001));
    Resource_DecodeType01((u8 *)header + header->palette, (void *)0x0202c000);
    Resource_DecodeType01((u8 *)header + header->tilemap, (void *)0x02010000);
    Tilemap_ConvertBuffer();
    if (header->animation != 0) {
        Resource_DecodeType01((u8 *)header + header->animation, (void *)0x0202d000);
        MapAnimation_StartChannels((void *)0x0202d000);
    }
    if (header->blend != 0) {
        Resource_DecodeType01((u8 *)header + header->blend, (void *)0x0202de00);
        DisplayBlend_StartScript((void *)0x0202de00);
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
    work->priority[0] = header->layer_priority[0];
    work->priority[1] = header->layer_priority[1];
    work->priority[2] = header->layer_priority[2];
    layer = work->layers;
    source = header->layers;
    for (cnt = 2; cnt >= 0; cnt--) {
        InitializeLayer(layer, source, scale_x, scale_y);
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
    Data_0400000e.value = work->priority[0] | (header->layer_screen[0] << 2) | 0x500;
    Data_0400000c.value = work->priority[1] | (header->layer_screen[1] << 2) | 0x600;
    Data_0400000a.value = work->priority[2] | (header->layer_screen[2] << 2) | 0x700;
    if (GameFlag_TestFar(0x170) != 0) {
        GameFlag_ClearBitFar(0x170);
    } else {
        buf = Runtime_BumpAllocate(0x4000);
        if (buf != NULL) {
            backdrop = *(s16 *)0x05000000;
            Resource_DecodeType01(Resource_GetTableEntry(entry->resources[1] + (u32)&Value_00000128), buf);
            *(s16 *)buf = backdrop;
            ((WordCopyFn)0x03001388)((void *)0x05000000, buf, 0x1c0);
            Resource_DecodeType2(Resource_GetTableEntry(entry->resources[2] + (u32)&Value_00000128), buf);
            ((WordCopyFn)0x03001388)((void *)0x06004000, buf, 0x4000);
            Resource_DecodeType2(Resource_GetTableEntry(entry->resources[3] + (u32)&Value_00000128), buf);
            ((WordCopyFn)0x03001388)((void *)0x06008000, buf, 0x4000);
            Resource_DecodeType2(Resource_GetTableEntry(entry->resources[4] + (u32)&Value_00000128), buf);
            ((WordCopyFn)0x03001388)((void *)0x0600c000, buf, 0x4000);
            Resource_DecodeType2(Resource_GetTableEntry(entry->resources[5] + (u32)&Value_00000128), (void *)0x02028000);
            Runtime_BumpFree(buf);
        }
    }
    {
        /* FAKEMATCH: typed I/O members keep immediate halfword stores. */
        volatile struct SceneDisplay *display = (volatile struct SceneDisplay *)0x04000000;
        display->mosaic = 0;
        display->blend = 0;
        display->control = 0x140;
    }
    Scheduler_AddOrUpdateCallback(Map_UpdateLayerScroll, 0xc85);
    return 2;
}
