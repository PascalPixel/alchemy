#include "types.h"
#include "global_cells.h"
#include "ram_buffer.h"
#include "map.h"
#include "object_runtime.h"
#include "fieldrun.h"
#include "fieldobj.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "project.h"
#include "scene.h"
#include "dma.h"
#include "iwram_call.h"
#include "map_scroll.h"
#include "mapframe.h"
#include "heap_state.h"
#include "io_reg.h"
#include "animspr.h"
#include "callback_scheduler.h"
#include "frame.h"
#include "keystate.h"
#include "resource.h"
#include "resource_ids.h"
#include "metadata_lookup.h"
#include "btlpty.h"
#include "b5_context.h"

extern u8 gMapCellBuffer[];

typedef s32 (*TerrainHeightFn)(u8 *heights, s32 x, s32 y);

extern TerrainHeightFn Map_TerrainHeightFunctions[16];

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 y)
{
    struct MapState *work = gMapWork[0];
    struct MapCell *cells;
    u8 *attributes;

    x >>= 16;
    y >>= 16;
    cells = (struct MapCell *)Ram_MapCellBuffer;
    if (work != NULL) {
        cells = work->layers[layer & 3].cells;
    }
    cells += x / 16 + (y / 16 << 7);
    attributes = Ram_MapCollision + cells->attribute_b * 4;
    return Map_TerrainHeightFunctions[*attributes++ & 15](attributes, x & 15, y & 15);
}

s32 Map_GetCellAttributeLowNibble(s32 index, s32 x, s32 y)
{
    struct MapState *state = gMapWork[0];
    struct MapCell *map;
    s32 col;
    s32 row;
    u32 attr;

    x >>= 16;
    y >>= 16;
    map = (struct MapCell *)gMapCellBuffer;
    if (state != 0) {
        map = state->layers[index & 3].cells;
    }
    col = x / 16;
    row = y / 16;
    map += col + (row << 7);
    attr = map->attribute_b;
    return *(Ram_MapCollision + attr * 4) & 15;
}

u8 GetMapCellCollision(s32 layer, s32 x, s32 y)
{
    struct MapState *state;
    struct MapCell *cells;

    state = gMapWork[0];
    x >>= 20;
    y >>= 20;
    cells = (struct MapCell *)gMapCellBuffer;
    if (state != NULL) {
        cells = state->layers[layer & 3].cells;
    }
    cells += x + (y << 7);
    return cells->collision_code;
}

void SetMapCellCollision(u32 layer, s32 x, s32 y, u32 collision_code)
{
    struct MapState *state = gMapWork[0];

    x >>= 20;
    y >>= 20;
    if (state != NULL) {
        struct MapCell *cells = state->layers[layer & 3].cells;

        cells += x + (y << 7);
        cells->collision_code = collision_code;
    }
}

s32 Map_GetCellHighFlags(s32 x, s32 y)
{
    s32 tile_x = x / 16;
    s32 tile_y = y / 16;
    struct MapCell *cell = (struct MapCell *)gMapCellBuffer + (tile_x + tile_y * 128);

    return ((u8 *)&cell->metatile_and_flags)[1] >> 6;
}

s32 Map_CheckObjectStep(struct ObjectRuntime *object, struct FieldPosition *position)
{
    struct MapState *state;
    s32 x;
    s32 z;
    struct MapCell *cell;
    u8 *attributes;
    s32 height;
    s32 delta;

    x = ((union FieldCoordinate *)&position->x)->part.pixel;
    z = ((union FieldCoordinate *)&position->z)->part.pixel;
    state = gMapWork[0];
    if (state == NULL)
        return 0;
    if (object->terrain_id <= 2)
        cell = state->layers[object->terrain_id].cells;
    else
        cell = (struct MapCell *)Ram_MapCellBuffer;
    cell += x / 16 + (z / 16 << 7);
    if (cell->collision_code == 0xff)
        return 2;
    attributes = Ram_MapCollision + cell->attribute_b * 4;
    height = Map_TerrainHeightFunctions[*attributes++ & 15](attributes, x & 15, z & 15);
    delta = height - object->terrain_height;
    if (delta > 0x80000)
        return 1;
    if (delta < -0xc0000)
        return -1;
    return 0;
}

extern u8 gDecodeBuffer[];

typedef void (*TransformFn)(const s32 *source, s32 *destination);

static __inline__ void Transform(const s32 *source, s32 *destination, TransformFn routine)
{
    /* FAKEMATCH: needed to match: a direct call holds the output vector in r9 instead of rematerializing sp+16. */
    routine(source, destination);
}

s32 CheckMapPositionCellOccupied(struct WorldPosition *position)
{
    s32 x;
    s32 y;
    s32 tile_x;
    s32 tile_y;
    struct MapState *work;
    struct MapCell *cell;

    x = position->x / 65536;
    y = (position->y - position->height) / 65536;
    work = gMapWork[0];
    if (work == NULL)
        return 0;
    cell = work->layers[2].cells;
    tile_x = x / 16;
    tile_y = y / 16;
    cell += tile_x + tile_y * 128;
    return (cell->collision_code != 0xff) - 1;
}

s32 GetWorldMapCollision(struct WorldPosition *position)
{
    s32 x_step = position->x >> 17;
    s32 y = position->y;
    s32 y_step = y >> 17;
    u32 cell;
    u32 pixel_offset;
    u8 tile;
    u8 packed_pixels;
    u32 result;

    cell = (((u32)(y_step / 8) & 63) << 6) +
        ((u32)(x_step / 8) & 63);
    pixel_offset = (((u32)(y_step / 2) & 3) << 1) +
        ((u32)(x_step / 4) & 1);

    tile = BG_SCREEN_BLOCK(10)[cell];
    packed_pixels = *(Ram_MapCollision + 0x800 + ((u32)tile << 3) + pixel_offset);
    if (packed_pixels != 0) {
        if ((u32)x_step & 2)
            result = packed_pixels >> 4;
        else
            result = packed_pixels & 15;
        if (result != 0)
            return result;
    }

    tile = BG_SCREEN_BLOCK(8)[cell];
    packed_pixels = *(Ram_MapCollision + ((u32)tile << 3) + pixel_offset);
    if (packed_pixels != 0) {
        if ((u32)x_step & 2)
            result = packed_pixels >> 4;
        else
            result = packed_pixels & 15;
        if (result != 0)
            return result;
    }

    return 7;
}

s32 CheckWorldMapCollisionRange(s32 unused, struct WorldPosition *position)
{
    if ((u32)(GetWorldMapCollision(position) - 5) <= 7U) {
        return 0;
    }
    return -1;
}

u8 GetWorldMapTerrainBehavior(struct WorldPosition *position, s32 *terrain_kind)
{
    s32 selector = GetWorldMapCollision(position);
    s32 x = position->x;
    s32 y;
    s32 flag = 0;
    u32 tile_x;
    u32 index;
    u32 *tile;

    if (x < 0)
        x += 0x1fffff;
    tile_x = (x >> 21) & 31;
    y = position->y;
    if (y < 0)
        y += 0x1fffff;

    index = tile_x + (((y >> 21) & 31) << 5);
    tile = gMapBlocks + index;
    if (((u8 *)tile)[3] & 0x80)
        flag = 0x10;

    *terrain_kind = (*tile << 1) >> 25;
    if (*terrain_kind == 21)
        flag = 0x20;

    return WorldMap_TerrainBehaviorTable[flag + selector];
}

void Runtime_SetWorkTripleIfNonNegative(s32 value0, s32 value1, s32 value2)
{
    struct MapScrollWork *work;

    work = gMapWork[0];
    if (value0 >= 0) {
        work->shake_x = value0;
    }
    if (value1 >= 0) {
        work->shake_y = value1;
    }
    if (value2 >= 0) {
        work->shake_decay = value2;
    }
}

void Map_WaitWorkValuesBelow256(void)
{
    struct MapScrollWork *work;
    s32 cnt;

    work = gMapWork[0];
    cnt = 0;
    if (work->shake_x > 255 || work->shake_y > 255) {
        goto body;
body:
        WaitFrames(1);
        ++cnt;
        if (cnt >= 300) {
            goto done;
        }
        if (work->shake_x > 255) {
            goto body;
        }
        if (work->shake_y > 255) {
            goto body;
        }
    }
done:
    work->shake_decay = 0;
}

void Resource_RunCopiedDecoder(s32 a, s32 b)
{
    u8 *base;
    u32 size;
    void *code;

    base = gDecodeBuffer;
    /* FAKEMATCH: needed to match: the do-whiles order the size load and the call. */
    do { size = (u32)TileMap_DrawRowsCodeSize; } while (0);
    code = Runtime_AllocateHeapBlock(49, size);
    Dma_Set((const void *)TileMap_DrawRows, code, DMA_ENABLE32 | DMA_32BIT | size >> 2, REG_DMA3);
    do { ((void (*)(s32, s32, void *, void *))gWorkSlot[49])(a, b, gBgTileBuffer + 0x4000, base + 0x1000); } while (0);
    Runtime_ReleaseHeapBlock(49);
}

void WorldMap_BuildScanlineTable(s32 depth, s32 *position, struct MapScanline *row)
{
    s32 ground[3];
    s32 viewed[3];
    s32 horizon;
    s32 focus;
    s32 *view;
    register s32 line;
    s32 distance;
    s32 difference;
    s32 scale;
    s32 x;
    s32 y;
    s32 diagonal;
    s32 vertical;
    s32 zero;
    s32 horizontal;

    zero = 0;
    ground[0] = position[0];
    ground[1] = zero;
    ground[2] = position[2];
    Transform(ground, viewed, Iwram_TransformVector);
    view = viewed;
    horizon = view[1] - Iwram_MulQ16(view[2], depth);
    focus = -gProjection.focal;
    for (line = 0; line < 160; line++) {
        distance = Iwram_RatioMulQ14(focus, (gProjection.center_y - line) << 16);
        difference = distance - depth;
        if (difference == 0)
            difference = 1;
        scale = Iwram_RatioMulQ14(difference, horizon);
        if (scale < 0) {
            diagonal = Iwram_MulQ16(-scale, 0x8000);
            row->scale = Iwram_RatioMulQ14(gProjection.focal, diagonal);
            diagonal = Iwram_MulQ16(scale, distance);
            horizontal = (view[2] - scale) >> 4;
            vertical = (diagonal - view[1]) >> 4;
            x = Iwram_MulQ16(horizontal, horizontal);
            y = Iwram_MulQ16(vertical, vertical);
            y = Iwram_Sqrt(x + y) << 12;
            if (vertical < 0)
                y = -y;
            row->depth = Iwram_MulQ16(y, 0x8000);
        } else {
            row->scale = 0;
            row->depth = 0;
        }
        row->offset_x = 0;
        row->offset_y = 0;
        row++;
    }
}

struct IconColumn {
    s16 no;
    s16 animation;
    u8 on;
    s8 priority;
    s8 param;
};

s32 RuntimeDispatch_ReturnZero();
s32 ResourceMetadata_Register(struct AnimationObject *, s32);

void Ui_RunIconMonitor(void)
{
    struct IconColumn ent[4];
    struct AnimationMetadata *metadata;
    struct AnimationObject *tbl;
    volatile s32 zero;
    s32 no;
    s32 x;
    s32 y;
    u32 mode;
    s32 flag;
    struct AnimationPosition *work;
    s32 slot;
    u32 cnt;
    s32 alt;
    s32 moved;
    volatile u32 *held;
    volatile u32 *repeat;
    u32 i;

    x = 144;
    y = 96;
    mode = 1;
    flag = 1;
    slot = 0;
    work = Runtime_AllocateBlock(9, 10 * sizeof(*work));
    gDecodeFillByte = 3;

    zero = 0;
    Dma_Set(&zero, work, 0x85000001, REG_DMA3);
    zero = 0;
    Dma_Set(&zero, ent, 0x85000008, REG_DMA3);

    no = Ui_FindNextNumberWithMetadata(-1, 1);
    for (i = 0; i <= 3; i++) {
        ent[i].animation = 1;
        ent[i].priority = 1;
        ent[i].no = no;
    }
    ent[0].on = 1;
    gDebugMode = 2;
    REG_BLDCNT = 0x3F42;
    PLTT_BACKDROP = 0x01E0;
    REG_DISPCNT = 0x1140;
    Blend_SetDarkenTarget0(1);

restart:
    Runtime_InitializeHeap();
    Scheduler_ResetTaskTable();
    work = Runtime_AllocateBlock(9, 10 * sizeof(*work));
    Resource_InitializeTable();
    ObjectSystem_Initialize(2);
    ResourceSlot_Load(0, (u32 *)Ram_MapCellBuffer, ent[0].no, 0);
    metadata = Resource_GetMetadataRecordFar(ent[0].no);
    if (metadata->draw_kind == 20) {
        ResourceSlot_Load(1, (u32 *)Ram_ActorSpriteSlots, ent[0].no + 1, 0);
    }

    for (i = 0; i <= 9; i++) {
        metadata = Resource_GetMetadataRecordFar(ent[0].no);
        alt = 0;
        if (metadata->draw_kind == 20 && (i & 1) != 0) {
            alt = 1;
        }
        alt = (s32)ResourceObject_Create(ent[0].no + alt + (alt << 12));
        ResourceMetadata_Register((struct AnimationObject *)alt, ent[1].no);
        ResourceMetadata_Register((struct AnimationObject *)alt, ent[2].no);
        ResourceMetadata_Register((struct AnimationObject *)alt, ent[3].no);
        /* FAKEMATCH: needed to match: the native adds the flags offset to the object's own register (r5) in place; a separate address takes a copy. */
        alt = (s32)&((struct AnimationObject *)alt)->flags;
        *(u8 *)alt = flag;
    }

    for (i = 0; i <= 3; i++) {
        if (ent[i].on != 0) {
            Ui_SetGridColumnPriority(i, ent[i].priority);
        } else {
            Ui_SetGridColumnPriority(i, 8);
        }
        Ui_SetGridColumnParameter(i, ent[i].param);
        Ui_FillGridColumnFromMetadata(i, ent[i].animation);
    }

    Map_BuildProbeRing(x, y, work);
    Scheduler_AddOrUpdateCallback(Battle_PlaceActorsByFormationKind, 3200);

next:
    WaitFrames(1);
    held = &gKeysHeld;
    repeat = &gKeysRepeat;
    for (;;) {

        if ((*held & KEY_START) != 0) {
            if ((*held & KEY_LEFT) != 0) {
                x--;
            }
            if ((*held & KEY_RIGHT) != 0) {
                x++;
            }
            if ((*held & KEY_UP) != 0) {
                y--;
            }
            if ((*held & KEY_DOWN) != 0) {
                y++;
            }
            Map_BuildProbeRing(x, y, work);
        } else {
            (void)gKeysRepeat;
            if ((*repeat & KEY_UP) != 0) {
                slot = (slot - 1) & 3;
            }
            if ((gKeysRepeat & KEY_DOWN) != 0) {
                slot = (slot + 1) & 3;
            }
            if ((gKeysRepeat & KEY_LEFT) != 0) {
                mode = (mode - 1) & 3;
            }
            if ((gKeysRepeat & KEY_RIGHT) != 0) {
                mode++;
                mode = mode & 3;
            }
        }

        if ((gKeyState & KEY_START) != 0) {
            tbl = gSpriteObjects;
            flag = flag ^ 1;
            for (i = 0; i <= 9; i++) {
                tbl->flags = flag;
                tbl++;
            }
        }

        switch (mode) {
        case 0:
            if (slot == 1) {
                break;
            }
            if ((gKeysRepeat & KEY_L) != 0) {
                ent[slot].animation--;
                if (ent[slot].animation < 0) {
                    ent[slot].animation = 0;
                }
                if (slot == 0) {
                    ent[1].animation = ent[0].animation;
                }
                goto next;
            }
            if ((gKeysRepeat & KEY_R) == 0) {
                break;
            }
            ent[slot].animation++;
            if (ent[slot].animation > 99) {
                ent[slot].animation = 99;
            }
            if (slot == 0) {
                ent[1].animation = ent[0].animation;
            }
            goto next;

        case 1:
            moved = 0;
            cnt = 1;
            if ((*held & KEY_START) != 0) {
                cnt = 10;
            }
            if ((*repeat & KEY_L) != 0) {
                for (i = 0; i < cnt; i++) {
                    ent[slot].no = Ui_FindNextNumberWithMetadata(ent[slot].no, -1);
                }
                moved = 1;
            }
            if ((*repeat & KEY_R) != 0) {
                for (i = 0; i < cnt; i++) {
                    ent[slot].no = Ui_FindNextNumberWithMetadata(ent[slot].no, 1);
                }
                moved = 1;
            }
            if (moved == 0) {
                break;
            }
            if (slot == 0) {
                goto restart;
            }
            if (ent[slot].on == 0) {
                goto next;
            }
            Ui_SetGridColumnNumber(slot, ent[slot].no);
            Ui_SetGridColumnPriority(slot, ent[slot].priority);
            Ui_SetGridColumnParameter(slot, ent[slot].param);
            Ui_FillGridColumnFromMetadata(slot, ent[slot].animation);
            goto next;

        case 2:
            moved = 0;
            if ((*repeat & KEY_L) != 0) {
                ent[slot].priority--;
                if (ent[slot].priority < 0) {
                    ent[slot].priority = 3;
                }
                moved = 1;
            }
            if ((*repeat & KEY_R) != 0) {
                ent[slot].priority++;
                if (ent[slot].priority > 3) {
                    ent[slot].priority = 0;
                }
                moved = 1;
            }
            if (moved == 0) {
                break;
            }
            if (ent[slot].on == 0) {
                goto next;
            }
            Ui_SetGridColumnPriority(slot, ent[slot].priority);
            goto next;

        case 3:
            moved = 0;
            if ((*repeat & KEY_L) != 0) {
                ent[slot].param--;
                if (ent[slot].param < 0) {
                    ent[slot].param = 15;
                }
                moved = 1;
            }
            if ((*repeat & KEY_R) != 0) {
                ent[slot].param++;
                if (ent[slot].param > 15) {
                    ent[slot].param = 0;
                }
                moved = 1;
            }
            if (moved == 0) {
                break;
            }
            if (ent[slot].on == 0) {
                goto next;
            }
            Ui_SetGridColumnParameter(slot, ent[slot].param);
            goto next;

        default:
            break;
        }

        if ((*repeat & KEY_A) != 0) {
            if (mode == 0) {
                if (slot != 1) {
                    if (ent[slot].on == 0) {
                        goto next;
                    }
                    Ui_FillGridColumnFromMetadata(slot, ent[slot].animation);
                    if (slot != 0) {
                        goto next;
                    }
                    Ui_FillGridColumnFromMetadata(1, ent[1].animation);
                    goto next;
                }
            } else if (slot != 0) {
                ent[slot].on = ent[slot].on ^ 1;
                Ui_SetGridColumnPriority(slot, ent[slot].on != 0 ? ent[slot].priority : 8);
                goto next;
            }
        }

        if ((*repeat & KEY_SELECT) != 0) {
            Scheduler_ResetTaskTable();
            if ((*held & KEY_B) != 0) {
                RuntimeDispatch_ReturnZero((s32)&ResourceId_ResourceFarCalls, FarCall_ResourceTable);
            } else {
                RuntimeDispatch_ReturnZero((s32)&ResourceId_ResourceFarCallsAlternate, FarCall_ResourceTable);
            }
            goto restart;
        }
        WaitFrames(1);
    }
}

s32 Ui_FindNextNumberWithMetadata(s32 start, s32 step)
{
    for (;;) {
        start += step;
        if (start < 0) {
            start = 0x200;
            continue;
        }
        {
            /* FAKEMATCH: needed to match: the upper bound in a local compares as cmp against 0x200 with blt; the literal compares against 0x1ff from the pool. */
            s32 limit = 0x200;
            if (start >= limit) {
                start = -1;
                continue;
            }
        }
        if (Resource_GetMetadataRecordFar(start)->width) {
            return start;
        }
    }
}

extern s32 gFrameTick;

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *point);

void Map_BuildProbeRing(s32 x, s32 z, struct AnimationPosition *out)
{
    struct AnimationPosition *pair;
    s32 angle;
    s32 i;
    u16 turn;
    u8 kind;

    angle = 0;
    kind = gSpriteObjects->entries[0]->kind;
    if ((gKeysHeld & KEY_B) != 0)
        angle = (s16)(gFrameTick << 8);
    switch ((u32)kind) {
    case 3:
        for (i = 0; i < 6; i++) {
            angle <<= 16;
            out->x = x << 16;
            angle = (u32)angle >> 16;
            out->y = 0;
            out->z = z << 16;
            Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
            out++;
            angle = (s16)(angle + 0x2aaa);
        }
        for (; i < 10; i++)
            out++;
        break;
    case 5:
    case 8:
    case 44:
    case 88:
        for (i = 0; i < 8; i++) {
            angle <<= 16;
            out->x = x << 16;
            angle = (u32)angle >> 16;
            out->y = 0;
            out->z = z << 16;
            Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
            out++;
            angle = (s16)(angle + 0x2000);
        }
        for (; i < 10; i++)
            out++;
        break;
    case 4:
    case 6:
        for (i = 0; i < 10; i++) {
            angle <<= 16;
            out->x = x << 16;
            angle = (u32)angle >> 16;
            out->y = 0;
            out->z = z << 16;
            Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
            out++;
            angle = (s16)(angle + 0x1999);
        }
        break;
    case 20:
        turn = angle;
        angle = (s16)(turn + 0x4000);
        i = 0;
        pair = out;
        while (i < 2) {
            pair[i * 2].x = x << 16;
            pair[i * 2].y = 0;
            pair[i * 2].z = z << 16;
            angle <<= 16;
            angle = (u32)angle >> 16;
            Vector_AddPolarOffset(0x280000, angle, &out[0].x);
            pair[i * 2 + 1].x = x << 16;
            {
                s32 *point = &out[1].x;
                pair[i * 2 + 1].y = 0;
                pair[i * 2 + 1].z = z << 16;
                Vector_AddPolarOffset(0x280000, (u16)angle, point);
            }
            angle = (angle + 0x8000) << 16;
            i++;
            out += 2;
            angle >>= 16;
        }
        break;
    default:
        angle = ((angle << 16) + 0x20000000) >> 16;
        for (i = 0; i < 4; i++) {
            angle <<= 16;
            angle = (u32)angle >> 16;
            out->x = x << 16;
            out->y = 0;
            out->z = z << 16;
            Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
            out++;
            angle = (s16)(angle + 0x4000);
        }
        for (i = 4; i < 10; i++)
            out++;
        break;
    }
}

struct Vec2 {
    s32 x;
    s32 y;
};

void Ui_SetGridColumnParameter(s32 slot, s32 value)
{
    struct AnimationObject *object = gSpriteObjects;
    s32 i;

    slot &= 3;
    for (i = 0; i < 10; i++) {
        object->entries[slot]->param = value;
        object++;
    }
}

void Ui_SetGridColumnPriority(s32 slot, s32 value)
{
    struct AnimationObject *object = gSpriteObjects;
    s32 i;

    slot &= 3;
    for (i = 0; i < 10; i++) {
        object->entries[slot]->priority = value;
        object++;
    }
}

void Ui_FillGridColumnFromMetadata(s32 slot, s32 value)
{
    s32 index;
    s32 count;
    s32 offset;
    struct AnimationMetadata *metadata;
    struct AnimationEntry *entry;
    struct AnimationObject *object;

    object = gSpriteObjects;
    count = 0;
    /* FAKEMATCH: needed to match: the precomputed slot lane keeps the offset addition outside the loop; typed indexing moves it inside. */
    offset = (3 & slot) * sizeof(void *) + (u32)&((struct AnimationObject *)0)->entries;
    index = 0;
    do {
        entry = *(struct AnimationEntry **)((u8 *)object + offset);
        if (entry->scripts != 0) {
            metadata = Resource_GetMetadataRecordFar(entry->anim_id);
            if (value < metadata->animation_count) {
                entry->kind = metadata->draw_kind;
                entry->script = entry->scripts[value];
                entry->timer = count * 0x10;
                entry->step = 0x10;
                entry->pos = index;
                entry->frame_base = index;
                entry->frame = 0xff;
            }
            object->offset_y = metadata->adjust_y;
            object->rotation = index;
        }
        count++;
        object++;
    } while (count <= 9);
}

void Ui_SetGridColumnNumber(s32 slot, s32 no)
{
    struct AnimationObject *object = gSpriteObjects;
    s32 i;

    Resource_GetMetadataRecordFar(no);
    slot &= 3;
    for (i = 0; i < 10; i++) {
        struct AnimationEntry *entry = object->entries[slot];

        entry->anim_id = no;
        Animation_InitWorkFromMetadata(entry);
        object++;
    }
}

void Battle_PlaceActorsByFormationKind(void)
{
    struct AnimationObject *actor = gSpriteObjects;
    u32 kind = actor->entries[0]->kind;
    struct Vec2 scale;
    struct AnimationPosition *table;
    u16 angle;
    u16 step;
    u16 odd = 0;
    u32 count;
    u32 i;

    scale = Battle_FormationPlacementScale;
    /* FAKEMATCH: needed to match: loading heap slot 9 relative to slot 4 avoids a second RAM-base load and eight bytes. */
    table = *(struct AnimationPosition **)((u8 *)&gSpriteObjects + (9 - 4) * sizeof(((union HeapState *)0)->slots[0]));

    switch (kind) {
    case 3:
        angle = 0;
        step = 0x2aaa;
        count = 6;
        break;
    case 5:
    case 8:
    case 44:
    case 88:
        angle = 0;
        step = 0x2000;
        count = 8;
        break;
    case 4:
    case 6:
        angle = 0;
        step = 0x1999;
        count = 10;
        break;
    case 20:
        angle = 0;
        step = 0;
        odd = 0x8000;
        count = 4;
        break;
    default:
        angle = 0x2000;
        step = 0x4000;
        count = 4;
        break;
    }

    for (i = 0; i < count; i++) {
        Render_ApplyProjectedPlacement(actor, &table[i].x, (s32 *)&scale, angle);
        actor++;
        angle += step;
        if (i & 1)
            angle += odd;
    }
}
