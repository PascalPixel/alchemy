#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "render_input.h"
#include "window.h"
#include "types.h"
#include "ram_buffer.h"
#include "vram_block.h"
#include "resource.h"

struct RenderOutput *RenderOutput_Create(
    s32 slot,
    s32 flags,
    struct RenderInput *input,
    s32 offset_x,
    s32 offset_y)
{
    /* FAKEMATCH: needed to match: the ignored scalar-return call keeps mov after the stores; direct void-call forms reorder it. */
    s32 x;
    struct RenderOutput *output;
    s32 y;

    output = RenderOutput_AcquireFree();
    if (output == NULL) {
        Resource_ResetEntry(slot);
        return 0;
    }
    x = offset_x + (input->x * 8) + 8;
    y = offset_y + (input->y * 8) + 8;
    x &= 0x1ff;
    y &= 0xff;
    output->packed = (x << 16) | y | flags;
    output->table.value =
        ((struct VramBlockCacheEntry *)Ram_VramBlockCache)[slot].offset >> 5;
    output->sentinel = 0xff;
    output->next = 0;
    output->x = x;
    output->y = y;
    output->index = slot;
    output->kind = 1;
    output->active = 1;
    ((s32 (*)(struct RenderOutputList *, struct RenderOutput *))
        RenderOutput_AppendToList)((struct RenderOutputList *)input, output);
    return output;
}

struct RenderOutput *RenderOutput_CreateFromResource(
    s32 mode,
    s32 resource,
    struct RenderInput *input,
    s32 x,
    s32 y)
{
    s32 no;

    no = Resource_LoadByMode(mode, resource);
    if (no < 0) {
        return NULL;
    }
    return RenderOutput_Create(no, 0x40000000, input, x, y);
}

struct RenderOutput *RenderOutput_CreateLoaded(
    s32 code,
    s32 layers,
    struct RenderInput *input,
    s32 x,
    s32 y)
{
    s32 no;
    struct RenderOutput *result;

    no = Resource_FindFreeEntry();
    result = NULL;
    if (no != 0x60) {
        UiIcon_CopyResourceToSlot(code, layers, no);
        result = RenderOutput_Create(no, 0x40000000, input, x, y);
    }
    return result;
}

struct RenderOutput *RenderOutput_CreateWithTransform(
    s32 icon,
    struct RenderInput *input,
    s32 x,
    s32 y)
{
    s32 count;
    s32 unused;
    struct RenderOutput *result;

    count = Resource_FindFreeEntry();
    if (count == 0x60) {
        return NULL;
    }
    Ui_BuildPairedPatternsToSlot(icon, 1, &count, &unused, 1);
    result = RenderOutput_Create(count, 0x40000000, input, x, y);
    result->sentinel = 251;
    return result;
}

struct RenderOutput *RenderOutput_CreateFromTable(
    s32 table_entry,
    struct RenderInput *input,
    s32 x,
    s32 y)
{
    s32 slot;
    struct RenderOutput *output;

    slot = Resource_FindFreeEntry();
    output = NULL;
    if (slot != 0x60) {
        RenderResource_LoadTableEntry(table_entry, 0, slot);
        output = RenderOutput_Create(slot, 0x40000000, input, x, y);
    }
    return output;
}
