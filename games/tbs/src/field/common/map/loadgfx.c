#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "global_cells.h"
#include "heap_state.h"
#include "dma.h"
#include "map.h"
#include "types.h"
#include "callback_scheduler.h"
#include "resource_ids.h"
#include "ram_buffer.h"
#include "map_scroll.h"
#include "mapframe.h"
#include "frame.h"
#include "io_reg.h"
#include "mem.h"
#include "palqueue.h"

extern u8 gMapCellBuffer[];
extern u8 gDecodeBuffer[];

s32 Resource_DecodeByteLz(const void *source, void *destination);

void MapAnimation_PresentFrame(void)
{
    s32 control = 0x682;

    /* FAKEMATCH: needed to match: the do-while keeps the BG1CNT store before the DMA source and destination loads. */
    do {
        REG_BG1CNT = control;
    } while (0);
    Dma_Set((const void *)gMapCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00, DMA_ENABLE32 | DMA_32BIT | 0x2580, REG_DMA3);
}

void MapAnimation_Start(void)
{
    struct MapFrameWork *pages = WORK_SLOT(7);
    struct PerspectiveWork *work = WORK_SLOT(8);
    s32 one = 1;

    work->animation_active = one;
    Scheduler_EnableCallbacks(MapAnimation_Update);
    Dma_Set(BG_CHAR_BLOCK(1), (void *)gDecodeBuffer, DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
    WaitFrames(1);
    Resource_RunCopiedDecoder((s32)pages->pages[gFrameCount & one], (s32)gMapCellBuffer);
    work->window_top = 200;
    work->window_bottom = 255;
    gVBlankHook = MapAnimation_PresentFrame;
}

void Map_ShowBg1FromBuffer(void)
{
    u32 mode = 0x501;
    u16 *bg1cnt = (u16 *)&REG_BG1CNT;

    do { *bg1cnt = mode; } while (0); /* FAKEMATCH: needed to match: the wrap orders the store before the DMA operands. */
    Dma_Set((const void *)gBgTileBuffer, BG_CHAR_BLOCK(2), 0x84002000,
            REG_DMA3);
}

void Map_LoadAreaGraphics(void)
{
    struct PerspectiveWork *state;
    u32 *resources;
    u8 *buffer;
    s16 value;

    state = gMapWork[0];
    buffer = gMapCellBuffer;
    resources = state->resources;
    value = *(s16 *)BG_PLTT;
    Resource_DecodeByteLz(Resource_GetTableEntry(resources[0]), buffer);
    *(s16 *)buffer = value;
    Dma_Set(buffer, (void *)BG_PLTT, DMA_ENABLE32 | DMA_32BIT | 0x70, REG_DMA3);
    Resource_DecodeType01(Resource_GetTableEntry(resources[1]), gBgTileBuffer);
    Resource_DecodeType01(Resource_GetTableEntry(resources[2]), Ram_BgTileBuffer + 0x2000);
    Resource_DecodeType01(Resource_GetTableEntry(resources[3]), Ram_BgTileBuffer + 0x4000);
    Resource_DecodeType01(Resource_GetTableEntry(resources[4]), Ram_BgTileBuffer + 0x6000);
    gVBlankHook = Map_ShowBg1FromBuffer;
    state->window_top = 0;
    state->window_bottom = 159;
    WaitFrames(1);
    Resource_DecodeType01(Resource_GetTableEntry((u32)&ResourceId_DefaultMapCells), buffer);
    Map_UpdateCurrentTileBlock();
    state->animation_active = 0;
    Scheduler_DisableCallbacks(MapAnimation_Update);
    WaitFrames(1);
}

void Map_LoadDefaultCellsAndUpdateBlock(void)
{
    struct PerspectiveWork *work = gMapWork[0];
    gVBlankHook = Map_ShowBg1FromBuffer;
    work->window_top = 0;
    work->window_bottom = 0x9f;
    WaitFrames(1);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), gMapCellBuffer);
    Map_UpdateCurrentTileBlock();
    Scheduler_DisableCallbacks(MapAnimation_Update);
    WaitFrames(1);
}

void MapAnimation_Update(void)
{
    /* FAKEMATCH: needed to match: indexed channel expressions preserve ip/lr allocation and the sentinel/VRAM pool order. */
    struct MapState *state = gMapWork[0];
    u32 i;

    for (i = 0; i <= 15; i++) {
        const u16 *script;
        u32 op;
        u32 count;
        u32 dst;

        if ((state->anim + i)->start == NULL || (state->anim + i)->paused != 0)
            continue;
    next:
        if ((state->anim + i)->timer == 0) {
            script = (state->anim + i)->cursor;
            op = *script++;
            if (op == 0xffff) {
                (state->anim + i)->cursor = (state->anim + i)->start;
                goto next;
            }
            if ((op & 0xff00) == 0xfe00) {
                if ((op & 0xff) == 0xff)
                    continue;
                (state->anim + i)->cursor = (state->anim + i)->start + (op & 0xff) * 2;
                goto next;
            }
            count = *script++;
            dst = script[0];
            (state->anim + i)->timer = script[1];
            if (state->wide_tiles == 0) {
                if (op >= 0x600)
                    Dma_Set(Ram_DecodeBuffer + op * 32, BG_CHAR_BLOCK(1) + dst * 32, DMA_ENABLE32 | DMA_32BIT | count * 8, REG_DMA3);
                else
                    Dma_Set(BG_CHAR_BLOCK(1) + op * 32, BG_CHAR_BLOCK(1) + dst * 32, DMA_ENABLE32 | DMA_32BIT | count * 8, REG_DMA3);
            } else {
                if (op >= 0x200)
                    Dma_Set(Ram_MapBlocks + op * 64, BG_CHAR_BLOCK(2) + dst * 64, DMA_ENABLE32 | DMA_32BIT | count * 16, REG_DMA3);
                else
                    Dma_Set(BG_CHAR_BLOCK(2) + op * 64, BG_CHAR_BLOCK(2) + dst * 64, DMA_ENABLE32 | DMA_32BIT | count * 16, REG_DMA3);
            }
            (state->anim + i)->cursor += 4;
            goto next;
        } else {
            (state->anim + i)->timer--;
        }
    }
}

void Map_ClearLayerEntryFlag(u32 no)
{
    struct MapState *state = gMapWork[0];
    struct MapAnimation *anim = &state->anim[no];

    anim->paused = 0;
}

void Map_SetLayerEntryFlag(u32 no)
{
    struct MapState *state = gMapWork[0];
    struct MapAnimation *anim = &state->anim[no];

    anim->paused = 1;
}

void MapAnimation_StartChannels(const u16 *script)
{
    struct MapState *state;
    s32 count;
    u32 command;
    volatile u32 zero;

    state = gMapWork[0];
    count = 0;
    zero = 0;
    Dma_Set((const void *)&zero, state->anim, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x30, REG_DMA3);
    command = *script++;
    while (command != 0xffff) {
        if ((command & 0xff00) == 0xfd00) {
            struct MapAnimation *anim;
            u32 idx = command & 15;
            u32 flag = 0;
            if (command & 0x80)
                flag = 1;
            anim = &state->anim[idx];
            anim->start = script;
            anim->cursor = script;
            anim->timer = 0;
            anim->paused = flag;
            count++;
        }
        command = *script++;
    }
    if (count != 0)
        Scheduler_AddOrUpdateCallback(MapAnimation_Update, 0xc80);
}

void Map_EnableUpdateCallback(void)
{
    if (((struct MapScrollWork *)gMapWork[0])->animation_active == 0)
        Scheduler_EnableCallbacks(MapAnimation_Update);
}

void Map_DisableUpdateCallback(void)
{
    if (((struct MapScrollWork *)gMapWork[0])->animation_active == 0)
        Scheduler_DisableCallbacks(MapAnimation_Update);
}

void DisplayBlend_RunScript(void)
{
    struct MapState *work = gMapWork[0];
    struct BlendScriptState *state;
    u16 *cursor;
    u32 command;
    u32 value;
    u16 test;

    state = &work->blend;
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
        state->cursor = state->script + value * 2;
        goto again;
    }

    if ((command & 0xf000) == 0x3000) {
        REG_BLDCNT = command;
        work->blend_control = command;
        state->cursor++;
        goto again;
    }

    if ((work->blend_control & 0xc0) == 0x40)
        REG_BLDALPHA = command;
    else
        REG_BLDY = command;
    state->delay = cursor[0];
    state->cursor += 2;
    goto again;

tick:
    state->delay = value + 0xffff;
}

void DisplayBlend_StartScript(u16 *script)
{
    struct BlendScriptState *state;
    s32 started;
    volatile u32 zero;

    started = 0;
    zero = 0;
    state = &((struct MapState *)gMapWork[0])->blend;
    Dma_Set((const void *)&zero, state, 0x85000003, REG_DMA3);
    if (*script != 0xffff) {
        state->script = script;
        state->cursor = script;
        state->delay = 0;
        state->paused = 0;
        started = 1;
    }
    if (started)
        Scheduler_AddOrUpdateCallback(DisplayBlend_RunScript, 0xc80);
}

void DisplayBlend_EnableRunScript(void)
{
    Scheduler_EnableCallbacks(DisplayBlend_RunScript);
}

void DisplayBlend_DisableRunScript(void)
{
    Scheduler_DisableCallbacks(DisplayBlend_RunScript);
}

void Runtime_AllocateAndClearQueue(void)
{
    struct PaletteSnapshotWork *queue;
    struct PaletteSnapshot *entry;
    u16 i;
    u16 j;

    queue = Runtime_AllocateBlock(28, sizeof(struct PaletteSnapshotWork));
    entry = queue->entries;
    for (i = 0; i != 4; i++) {
        entry->source = 0;
        entry->position = 0;
        entry->timer = 0;
        entry->delay = 0;
        entry->count = 0;
        for (j = 0; j != 16; j++) {
            entry->colors[j] = 0;
        }
        entry++;
    }
    queue->count = 0;
}

s32 PaletteQueue_Add(s16 bank, s16 index, s16 delay, s16 count)
{
    struct PaletteSnapshotWork *work;
    struct PaletteSnapshot *entry;
    u16 *source;
    u32 slot;
    u16 size;

    work = gPaletteWork;
    slot = work->count;
    if (slot > 3)
        return -1;
    entry = &work->entries[slot];
    source = (u16 *)BG_PLTT + (((u16)bank << 4) + (u16)index);
    size = count;
    entry->position = 0;
    entry->timer = 0;
    entry->count = size;
    entry->source = source;
    entry->delay = delay;
    Dma_Set(source, entry->colors, 0x80000000 | size, REG_DMA3);
    work->count++;
    return 0;
}

void Runtime_ScheduleCallbackAndReleaseBlock28(void)
{
    Scheduler_RemoveCallback(PaletteQueue_StepCycles);
    Runtime_ReleaseHeapBlock(0x1C);
}

void Scheduler_ScheduleCallbackBAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback(PaletteQueue_StepCycles, 0xc80);
}

void PaletteQueue_StepCycles(void)
{
    struct PaletteSnapshotWork *work = gPaletteWork;
    u8 i;
    u16 count = work->count & 3;
    u16 buf[16];

    for (i = 0; i < count; i++) {
        struct PaletteSnapshot *cycle = &work->entries[i];
        if (cycle->timer == 0) {
            u16 start = cycle->position;
            u16 len = cycle->count;
            void *dest = cycle->source;
            u16 *src = cycle->colors;
            u8 j;
            for (j = len - start; j < len; j++)
                buf[j] = *src++;
            for (j = 0; j < len - start; j++)
                buf[j] = *src++;
            Dma_Set(buf, dest, 0x80000000 | len, REG_DMA3);
            start++;
            if (start >= len)
                start = 0;
            cycle->position = start;
            cycle->timer = cycle->delay;
        } else {
            cycle->timer--;
        }
    }
}
