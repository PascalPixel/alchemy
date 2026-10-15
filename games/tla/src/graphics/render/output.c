#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "render_input.h"
#include "types.h"
#include "ram_buffer.h"
#include "resource.h"
#include "window.h"

s32 Localization_LookupEntryId(s32 key);
void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile, s32 palette, s32 reuse);

struct RenderOutput *RenderOutput_CreateWithTransform(
    s32 arg0,
    struct RenderInput *arg1,
    s32 arg2,
    s32 arg3)
{
    s32 count;
    s32 unused;
    struct RenderOutput *result;

    count = Resource_FindFreeEntry();
    if (count == 0x60) {
        return NULL;
    }
    Ui_BuildPairedPatternsToSlot(arg0, 1, &count, &unused, 1);
    result = RenderOutput_Create(count, 0x40000000, arg1, arg2, arg3);
    result->sentinel = 251;
    return result;
}


// Loads the character entry for the key into the given side, or into a free
// one (resource id 999) when the side is 2 or more, and creates its output
// with that side's palette (14 + side).
struct RenderOutput *CreateSideObject(s32 object_kind, s32 position, s32 side, s32 arg3, s32 arg4, s32 arg5)
{
    struct UiRenderWork *state = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
    struct RenderOutput *object = 0;
    s32 first;
    s32 second;
    s32 id;
    s32 slot;

    id = Localization_LookupEntryId(object_kind);
    if (id == -1)
        return object;

    if ((u32)side > 1) {
        u32 current;

        current = state->resourceIds[1];
        /* FAKEMATCH: the barrier loads the second side's id before 999 is
           built and the side is set; plain, both come first (one reordered
           row, differ 2). */
        asm("");
        side = 1;
        if (current != 999) {
            side = 0;
            if (state->resourceIds[0] != 999)
                return object;
        }
    }

    slot = 14 + side;
    UiGlyph_LoadEntryWithPalette(id, position, &first, &second, slot, 0);
    object = RenderOutput_Create(first, 0x80000000, (struct RenderInput *)arg3, arg4, arg5);
    if (object != 0) {
        object->oam.palette = slot;
        object->kind = 2;
    }

    state->resourceIds[side] = id;
    state->outputIds[side] = first;
    return object;
}
