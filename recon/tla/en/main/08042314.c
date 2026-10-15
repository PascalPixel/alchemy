#include "render_input.h"
#include "output.h"
#include "vram_tab.h"
#include "resource.h"

struct RenderOutput *RenderOutput_Create(
    s32 slot,
    s32 flags,
    struct RenderInput *input,
    s32 offsetX,
    s32 offsetY)
{
    s32 x;
    struct RenderOutput *output;
    s32 y;

    output = RenderOutput_AcquireFree();
    if (output == NULL) {
        Resource_ResetEntry((u32)slot);
        return NULL;
    }
    x = offsetX + (s16)input->x * 8 + 8;
    y = offsetY + (s16)input->y * 8 + 8;
    x &= 0x1ff;
    y &= 0xff;
    output->packed = (x << 16) | y | flags;
    output->table.value = gResourceTableEntries[slot].offset >> 5;
    output->sentinel = 254;
    output->x = x;
    output->y = (s16)y;
    output->next = NULL;
    output->index = (s8)slot;
    output->kind = 1;
    output->active = 1;
    /* FAKEMATCH: ⚓️ finishes the stores before it passes the list */
    asm volatile("");
    RenderOutput_AppendToList((struct RenderOutputList *)input, output);
    return output;
}
