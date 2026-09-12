#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "resource.h"
#include "map_render_work.h"
#include "curve.h"
#include "map.h"

extern u8 RomBytes_080132fc[];
extern u8 RomBytes_080133fc[];

/* map/shared/load_default_cells_and_update_block.c */
/* map/shared/load_default_cells_and_update_block.c */
extern u8 gVal[];
void WaitFrames(s32);

struct MapInitWork {
    u8 unknown_000[0x100];
    s16 first;
    s16 second;
};

void Map_LoadDefaultCellsAndUpdateBlock(void)
{
    struct MapInitWork *work = *(struct MapInitWork **)ADDR_03001E70;
    *(s32 *)ADDR_03001CFC = (s32)FunctionHead_0801161c;
    work->first = 0;
    work->second = 0x9f;
    WaitFrames(1U);
    Map_Apply((s32)GetResource((s32)gVal), 0x02010000);
    Map_UpdateCurrentTileBlock();
    FunctionHead_0800439c((u32)Map_unk3_4);
    WaitFrames(1U);
}

/* map/shared/clear_layer_entry_flag.c */
extern u8 *volatile gCam;

void Map_ClearLayerEntryFlag(u32 no)
{
    u8 *base = gCam;
    u8 *entry = base + no * 12;
    u32 value = 0;
    *(u16 *)(entry + 0x22) = value;
}

void FunctionHead_080118c0(u32 no)
{
    u8 *base = gCam;
    u8 *entry = base + no * 12;
    u32 value = 1;
    *(u16 *)(entry + 0x22) = value;
}

/* map/shared/enable_update_callback.c */
void Map_EnableUpdateCallback(void)
{
    if (gCam->active == 0)
        FunctionHead_080042c8((u32)FunctionHead_0801179c);
}

/* map/shared/disable_update_callback.c */
void Map_DisableUpdateCallback(void)
{
    if (gCam->active == 0)
        FunctionHead_080042c8((u32)FunctionHead_0801179c);
}

/* display/blend/blend_run_script.c */
/* display/blend/blend_run_script.c */
/* display/blend/run_script.c */
struct BlendScriptState {
    u16 *script;
    u16 *cursor;
    u16 delay;
    u16 paused;
};

extern struct MapState *gCam;

void DisplayBlend_RunScript(void)
{
    u8 *base = gCam;
    struct BlendScriptState *state;
    u16 *cursor;
    u32 command;
    u32 value;
    u16 test;

    state = (struct BlendScriptState *)(base + 0xd8);
    if (state->script == 0)
        return;
    if (state->paused != 0)
        return;

again:
    value = state->delay;
    test = value;
    if (test != 0)
        goto tick;

    cursor = state->cursor;
    command = *cursor;
    cursor++;
    if (command == 0xffff) {
        state->cursor = state->script;
        goto again;
    }

    if ((command & 0xff00) == 0xfe00) {
        value = command & 0xff;
        if (value == 0xff)
            return;
        state->cursor = (u16 *)((u8 *)state->script + value * 4);
        goto again;
    }

    if ((command & 0xf000) == 0x3000) {
        *(volatile u16 *)0x04000050 = command;
        base[0x103] = command;
        state->cursor = (u16 *)((u8 *)state->cursor + 2);
        goto again;
    }

    if ((base[0x103] & 0xc0) == 0x40)
        *(volatile u16 *)0x04000052 = command;
    else
        *(volatile u16 *)0x04000054 = command;
    state->delay = cursor[0];
    state->cursor = (u16 *)((u8 *)state->cursor + 4);
    goto again;

tick:
    state->delay = value + 0xffff;
}

/* display/blend/enable_run_script.c */
void DisplayBlend_RunScript(void);

void DisplayBlend_EnableRunScript(void)
{
    FunctionHead_080042c8((u32)DisplayBlend_RunScript);
}

/* display/blend/disable_run_script.c */

void DisplayBlend_DisableRunScript(void)
{
    FunctionHead_0800439c((u32)DisplayBlend_RunScript);
}

/* runtime/allocate_and_clear_queue.c */
/* runtime/memory/allocate_and_clear_queue.c */
struct SubQueueItem_08011b00 {
    u32 unknown_00;
    u16 unknown_04;
    u16 unknown_06;
    u16 unknown_08;
    u16 unknown_0a;
    u16 unknown_0c[16];
};

struct Queue_08011b00 {
    struct SubQueueItem_08011b00 slots[4];
    u16 count;
};

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

void Runtime_AllocateAndClearQueue(void)
{
    struct Queue_08011b00 *queue;
    struct SubQueueItem_08011b00 *entry;
    u16 i;
    u16 j;

    queue = (struct Queue_08011b00 *)Runtime_AllocateBlock(28, sizeof(struct Queue_08011b00));
    entry = queue->slots;
    for (i = 0; i != 4; i++) {
        entry->unknown_00 = 0;
        entry->unknown_04 = 0;
        entry->unknown_06 = 0;
        entry->unknown_08 = 0;
        entry->unknown_0a = 0;
        for (j = 0; j != 16; j++) {
            entry->unknown_0c[j] = 0;
        }
        entry++;
    }
    queue->count = 0;
}

/* runtime/memory/schedule_callback_and_release_block_28.c */
extern u8 gRomAndClearQueue;

void ScheduleCallback(void *);

void Runtime_ScheduleCallbackAndReleaseBlock28(void)
{
    ScheduleCallback(&gRomAndClearQueue);
    Sys_Do(0x1C);
}

/* runtime/scheduler/schedule_callback_b_after_frames.c */
s32 ScheduleCallbackAfterFrames(s32, s32);

void Scheduler_ScheduleCallbackBAfterFrames(void)
{
    ScheduleCallbackAfterFrames((s32)&gRomAndClearQueue, 0xc80);
}

/* math/curve/curve_get_first_sample_a.c */
/* math/curve/get_first_sample_a.c */
s32 Curve_GetFirstSampleA(const s8 *samples)
{
    return samples[0] << CURVE_VALUE_SHIFT;
}

/* math/curve/lerp_two_samples.c */
s32 Curve_LerpTwoSamples(const s8 *samples, s32 position)
{
    s32 start;
    s32 delta;

    start = samples[0] << CURVE_VALUE_SHIFT;
    delta = ((samples[1] << CURVE_VALUE_SHIFT) - start) * position;
    if (delta < 0) {
        delta += CURVE_FULL_STEPS - 1;
    }
    return start + (delta >> 4);
}

/* math/curve/lerp_two_samples_b.c */
s32 Curve_LerpTwoSamplesB(const s8 *samples, s32 unused, s32 position)
{
    s32 base;
    s32 offset;

    base = samples[0] << CURVE_VALUE_SHIFT;
    offset = ((samples[1] << CURVE_VALUE_SHIFT) - base) * position;
    if (offset < 0) {
        offset += CURVE_FULL_STEPS - 1;
    }
    return base + (offset >> 4);
}

/* math/curve/step_at_summed_position.c */
s32 Curve_StepAtSummedPosition(const s8 *samples, s32 start, s32 end)
{
    s32 first;
    s32 second;
    s32 maximum;
    u32 position;

    first = samples[0] << CURVE_VALUE_SHIFT;
    second = samples[1] << CURVE_VALUE_SHIFT;
    maximum = first;
    if (second > first) {
        maximum = second;
    }
    position = start + end;
    if (position == CURVE_FULL_STEPS - 1) {
        return maximum;
    }
    if (position < CURVE_FULL_STEPS - 1) {
        second = first;
    }
    return second;
}

/* math/curve/step_at_difference_position.c */
s32 Curve_StepAtDifferencePosition(const s8 *samples, s32 start, s32 end)
{
    s32 first;
    s32 second;
    s32 maximum;
    s32 difference;
    u32 position = start;

    first = samples[0] << CURVE_VALUE_SHIFT;
    second = samples[1] << CURVE_VALUE_SHIFT;
    maximum = first;
    if (second > first) {
        maximum = second;
    }
    difference = end - position;
    position = difference + CURVE_FULL_STEPS - 1;
    if (position == CURVE_FULL_STEPS - 1) {
        return maximum;
    }
    if (position < CURVE_FULL_STEPS - 1) {
        second = first;
    }
    return second;
}

/* math/curve/lerp_three_samples_by_ratio.c */
s32 FixedPoint_Ratio(s32 dividend, s32 divisor);

s32 Curve_LerpThreeSamplesByRatio(const s8 *samples, s32 start, s32 end)
{
    s32 first;
    s32 middle;
    s32 last;
    s32 result;

    first = *samples++ << CURVE_VALUE_SHIFT;
    middle = *samples << CURVE_VALUE_SHIFT;
    last = samples[1] << CURVE_VALUE_SHIFT;
    start = start + end;
    if ((u32)start == CURVE_FULL_STEPS - 1) {
        result = middle;
    } else if ((u32)start < CURVE_FULL_STEPS - 1) {
        result = first + FixedPoint_Ratio(
            (middle - first) * start,
            CURVE_FULL_STEPS - 1);
    } else {
        start = start - (CURVE_FULL_STEPS - 1);
        result = middle + FixedPoint_Ratio(
            (last - middle) * start,
            CURVE_FULL_STEPS - 1);
    }
    return result;
}

/* math/curve/lerp_three_samples_at_difference.c */
s32 Curve_LerpThreeSamplesAtDifference(const s8 *samples, s32 start, s32 end)
{
    s32 first;
    s32 second;
    s32 third;
    s32 difference;
    s32 delta;

    first = *samples++ << CURVE_VALUE_SHIFT;
    second = samples[0] << CURVE_VALUE_SHIFT;
    third = samples[1];
    difference = end - start;
    start = difference + CURVE_FULL_STEPS - 1;
    third <<= CURVE_VALUE_SHIFT;

    if (start == CURVE_FULL_STEPS - 1)
        return second;

    if ((u32)start < CURVE_FULL_STEPS - 1) {
        delta = second - first;
        difference = delta *start;
        if (difference < 0)
            difference += CURVE_FULL_STEPS - 1;
        difference >>= 4;
        return first + difference;
    } else {
        difference *= third - second;
        if (difference < 0)
            difference += CURVE_FULL_STEPS - 1;
        difference >>= 4;
        return second + difference;
    }
}

/* math/curve/lerp_two_samples_by_table.c */
s32 Curve_LerpTwoSamplesByTable(const s8 *samples, s32 position, s32 row)
{
    s32 start;

    start = samples[0] << CURVE_VALUE_SHIFT;
    return start
        + (((samples[1] << CURVE_VALUE_SHIFT) - start)
           * RomBytes_080132fc[position + (row *CURVE_FULL_STEPS)]);
}

/* math/curve/step_at_half.c */
s32 Curve_StepAtHalf(const s8 *samples, u32 position)
{
    s32 value;
    if (position < CURVE_HALF_STEPS)
        value = samples[0];
    else
        value = samples[1];
    return value << CURVE_VALUE_SHIFT;
}

/* math/curve/step_at_half_b.c */
s32 Curve_StepAtHalfB(const s8 *samples, u32 unused, u32 position)
{
    s32 value;
    if (position < CURVE_HALF_STEPS)
        value = samples[0];
    else
        value = samples[1];
    return value << CURVE_VALUE_SHIFT;
}

/* math/curve/lerp_three_samples_at_half.c */
s32 Curve_LerpThreeSamplesAtHalf(s8 *samples, u32 position)
{
    s32 a;
    s32 b;
    s32 delta;

    a = *samples++ << CURVE_VALUE_SHIFT;
    b = *samples++ << CURVE_VALUE_SHIFT;
    if (position < CURVE_HALF_STEPS) {
        delta = (b - a) * position;
        if (delta < 0) {
            delta += CURVE_HALF_STEPS - 1;
        }
        return a + (delta >> 3);
    }

    {
        s32 pos;

        a = *samples << CURVE_VALUE_SHIFT;
        pos = position;
        a -= b;
        pos -= CURVE_HALF_STEPS;
        delta = a *pos;
        if (delta < 0) {
            delta += CURVE_HALF_STEPS - 1;
        }
        return b + (delta >> 3);
    }
}

/* math/curve/lerp_three_samples_at_half_b.c */
s32 Curve_LerpThreeSamplesAtHalfB(u8 *samples, u32 unused, u32 position)
{
    s32 a;
    s32 b;
    s32 pos;
    s32 delta;
    s32 tail;

    a = *samples++ << CURVE_VALUE_SHIFT;
    b = *samples++ << CURVE_VALUE_SHIFT;
    if (position < CURVE_HALF_STEPS) {
        delta = (b - a) * position;
        if (delta < 0) {
            delta += CURVE_HALF_STEPS - 1;
        }
        return a + (delta >> 3);
    }

    a = *samples << CURVE_VALUE_SHIFT;
    pos = position;
    a -= b;
    pos -= CURVE_HALF_STEPS;
    tail = a *pos;
    if (tail < 0) {
        tail += CURVE_HALF_STEPS - 1;
    }
    return b + (tail >> 3);
}

/* math/curve/lookup_sample_by_table.c */
s32 Curve_LookupSampleByTable(const s8 *samples, u32 position, u32 row)
{
    return samples[RomBytes_080133fc[position + (row << 4)]]
        << CURVE_VALUE_SHIFT;
}

/* math/curve/lookup_sample_by_table_reversed.c */
s32 Curve_LookupSampleByTableReversed(const s8 *samples, u32 position, u32 row)
{
    return samples[
        RomBytes_080133fc[((row << 4) - position) + CURVE_FULL_STEPS - 1]]
        << CURVE_VALUE_SHIFT;
}

/* math/curve/get_first_sample_b.c */
s32 Curve_GetFirstSampleB(const s8 *samples)
{
    return samples[0] << CURVE_VALUE_SHIFT;
}

/* math/curve/get_first_sample_c.c */
s32 Curve_GetFirstSampleC(const s8 *samples)
{
    return samples[0] << CURVE_VALUE_SHIFT;
}

/* map/shared/get_cell_attribute_low_nibble.c */
/* map/shared/get_cell_attribute_low_nibble.c */
s32 Map_GetCellAttributeLowNibble(s32 index, s32 x, s32 y)
{
    u8 *state = *(u8 **)ADDR_03001E70;
    u8 *map;
    s32 off;
    s32 col;
    s32 row;
    u32 attr;

    x >>= 16;
    y >>= 16;
    map = (u8 *)0x02010000;
    if (state != 0) {
        off = (index & 3) * 48 + 304;
        map = *(u8 **)(state + off);
    }
    col = x / 16;
    row = y / 16;
    map += (col + (row << 7)) * 4;
    attr = map[3];
    return *(u8 *)(0x0202c000 + attr * 4) & 15;
}

/* map/shared/get_cell_collision.c */
u8 GetMapCellCollision(s32 layer, s32 x, s32 y)
{
    struct MapState *state;
    s32 cell_address;
    s32 layer_offset;

    state = *(struct MapState **)ADDR_03001E70;
    x >>= 20;
    y >>= 20;
    cell_address = 0x02010000;
    if (state != 0) {
        layer_offset = ((layer & 3) * sizeof(struct MapLayer)) + 0x130;
        cell_address = *(s32 *)((u8 *)state + layer_offset);
    }
    cell_address += (x + (y << 7)) * sizeof(struct MapCell);
    return ((struct MapCell *)cell_address)->collision_code;
}

/* map/shared/set_cell_collision.c */

void SetMapCellCollision(u32 layer, s32 x, s32 y, u32 collision_code)
{
    struct MapState *state = gCam;

    x >>= 20;
    y >>= 20;
    if (state != 0) {
        struct MapCell *cells = state->layers[layer & 3].cells;
        u32 offset = (x + (y << 7)) * sizeof(struct MapCell);
        u8 *cell = (u8 *)cells;

        cell += offset;
        cell[2] = collision_code;
    }
}

/* map/shared/get_cell_high_flags.c */
s32 Map_GetCellHighFlags(s32 x, s32 y)
{
    s32 tile_x = x / 16;
    s32 tile_y = y / 16;
    u8 *cell = (u8 *)0x02010000 + (tile_x + tile_y * 128) * 4;

    return cell[1] >> 6;
}

/* map/shared/check_position_cell_occupied.c */
struct MapCollisionWork {
    u8 unknown_000[0x190];
    u8 *cells;
};

s32 CheckMapPositionCellOccupied(struct WorldPosition *position)
{
    s32 x;
    s32 y;
    s32 tile_x;
    s32 tile_y;
    struct MapCollisionWork *work;
    u8 *cell;

    x = position->x / 65536;
    y = (position->y - *(s32 *)((u8 *)position + 4)) / 65536;
    work = *(struct MapCollisionWork **)ADDR_03001E70;
    if (work == NULL)
        return 0;
    cell = work->cells;
    tile_x = x / 16;
    tile_y = y / 16;
    cell += (tile_x + tile_y * 128) * 4;
    return (cell[2] != 0xFF) - 1;
}

/* map/shared/get_world_map_collision.c */
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

    /* 1行64バイトで衝突判定タイル64個を表す。 */
    cell = (((u32)(y_step / 8) & 63) << 6) +
        ((u32)(x_step / 8) & 63);
    /* 4×4ドット・4bppの衝突判定タイルは8バイト。 */
    pixel_offset = (((u32)(y_step / 2) & 3) << 1) +
        ((u32)(x_step / 4) & 1);

    tile = *(u8 *)(0x06005000 + cell);
    packed_pixels = *(u8 *)(0x0202c800 + ((u32)tile << 3) + pixel_offset);
    if (packed_pixels != 0) {
        if ((u32)x_step & 2)
            result = packed_pixels >> 4;
        else
            result = packed_pixels & 15;
        if (result != 0)
            return result;
    }

    /* 前面が空なら背面マップを調べる。 */
    tile = *(u8 *)(0x06004000 + cell);
    packed_pixels = *(u8 *)(0x0202c000 + ((u32)tile << 3) + pixel_offset);
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

/* map/shared/check_world_map_collision_range.c */
struct WorldPosition;

s32 CheckWorldMapCollisionRange(s32 unused, struct WorldPosition *position)
{
    if ((u32)(GetWorldMapCollision(position)- 5) <= 7U) {
        return 0;
    }
    return -1;
}

/* map/shared/get_world_map_terrain_behavior.c */
extern u8 gRom[];

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
    tile = (u32 *)0x02020000 + index;
    if (((u8 *)tile)[3] & 0x80)
        flag = 0x10;

    *terrain_kind = (*tile << 1) >> 25;
    if (*terrain_kind == 21)
        flag = 0x20;

    return gRom[flag + selector];
}
