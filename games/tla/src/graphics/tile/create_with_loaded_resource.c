#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "types.h"
#include "resource.h"
#include "render_input.h"
#include "system.h"
#include "ram_buffer.h"

struct ItemMenuBlocks
{
    u8  unk000[0x4ee];
    s16 resourceSlots[2];
};

extern char ResourceId_ItemMenuTilesA;
extern char ResourceId_ItemMenuTilesB;

void Resource_LoadPairedBlocksIfAvailable(void)
{
    s32                    firstSlot;
    s32                    secondSlot;
    struct ItemMenuBlocks *activeState;

    activeState = Ram_HeapSlots->menuRuntime;
    firstSlot = Resource_FindFreeEntry();
    activeState->resourceSlots[0] = (s16)firstSlot;
    if (firstSlot != -1)
    {
        VramBlock_LoadResourceFar(firstSlot, 0x80, (s32)&ResourceId_ItemMenuTilesA);
    }
    secondSlot = Resource_FindFreeEntry();
    activeState->resourceSlots[1] = (s16)secondSlot;
    if (secondSlot != -1)
    {
        VramBlock_LoadResourceFar(secondSlot, 0x80, (s32)&ResourceId_ItemMenuTilesB);
    }
}

void Menu_ResetTwoResourceEntries(void)
{
    struct ItemMenuBlocks *state = Ram_HeapSlots->menuRuntime;

    /* FAKEMATCH: ⚓️ loads the state before it builds the slot offset */
    asm volatile("" : "+l"(state));
    Resource_ResetEntry((u16)state->resourceSlots[0]);
    Resource_ResetEntry((u16)state->resourceSlots[1]);
}

s32 UiIcon_LoadResourceIntoSlot(s32 resourceId, s32 slot)
{
    s32 resourceInfo;
    s32 selectedSlot;
    s32 buffer;
    u8 *allocation;

    allocation = Runtime_AllocateBlock(0x44, 0x608);
    selectedSlot = slot;
    Ability_LoadGlyphFar(resourceId, 0, &selectedSlot, &resourceInfo, 1);
    buffer = Resource_GetBuffer(slot, (s32)(allocation + 0x400));
    Runtime_ReleaseHeapBlock(0x44);
    return buffer;
}

s32 UiIcon_CreateWithLoadedResource(struct RenderInput *window, s32 x, s32 y, s32 resourceId)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    if (slot != 0x60)
    {
        UiIcon_LoadResourceIntoSlot(resourceId, slot);
        RenderOutput_CreateFar(slot, 0x40000000, window, x, y);
    }
}

// Creates the stat change arrow from the up or down arrow resource the
// menu loaded. The Broken Seal's UiIcon_CreateStatChangeArrow.
s32 UiIcon_CreateStatChangeArrow(struct RenderInput *window, s32 x, s32 y, s32 variant)
{
    struct RenderOutput   *object;
    u32                    resource;
    s32                    v = variant;
    struct RenderInput    *w = window;
    s32                    xx = x;
    struct ItemMenuBlocks *data = Ram_HeapSlots->menuRuntime;

    if (v == 0)
        resource = (u16)data->resourceSlots[0];
    else
        resource = (u16)data->resourceSlots[1];
    object = RenderOutput_CreateFar(resource, 0x40000000, w, xx, y);
    if (object == 0)
    {
        /* FAKEMATCH: the redundant failure branch keeps the arguments' lifetimes through CSE, as in The Broken Seal; a plain return -1 gives other registers (differ 20). */
        if (variant || xx)
            return -1;
        else
            return -1;
    }
    object->kind = 0;
    object->animStep = 0;
    object->active = 1;
    return 1;
}

// As UiIcon_CreateStatChangeArrow, with the arrow raised three pixels for
// the first variant and four for the second. The Broken Seal's
// UiIcon_DrawVariantWithTileOffset.
s32 Func_08104ba8(struct RenderInput *window, s32 x, s32 y, s32 variant)
{
    struct RenderOutput   *object;
    u32                    resource;
    s32                    v = variant;
    struct RenderInput    *w = window;
    s32                    xx = x;
    struct ItemMenuBlocks *data = Ram_HeapSlots->menuRuntime;

    if (v == 0)
    {
        resource = (u16)data->resourceSlots[0];
        y -= 3;
    }
    else
    {
        resource = (u16)data->resourceSlots[1];
        y -= 4;
    }
    object = RenderOutput_CreateFar(resource, 0x40000000, w, xx, y);
    if (object == 0)
    {
        /* FAKEMATCH: the redundant failure branch keeps the arguments' lifetimes through CSE, as in The Broken Seal; a plain return -1 gives other registers (differ 26). */
        if (v || x)
            return -1;
        else
            return -1;
    }
    object->kind = 0;
    object->animStep = 0;
    object->active = 1;
    return 1;
}
