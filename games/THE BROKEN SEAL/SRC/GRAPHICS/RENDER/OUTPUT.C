#include "RENDER_INPUT.H"
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "VRAM_BLOCK.H"
#include "RESOURCE.H"

struct RenderOutput *RenderOutput_AcquireFree(void);
s32 RenderOutput_AppendToList(struct RenderOutputList *, struct RenderOutput *);

struct RenderOutput *RenderOutput_Create(
    s32 slot,
    s32 flags,
    struct RenderInput *input,
    s32 offset_x,
    s32 offset_y)
{
    /* FAKEMATCH: the ignored scalar declaration of the true void list helper
       keeps one mov after the field stores; a void declaration moves it before. */
    s32 x;
    struct RenderOutput *output;
    s32 y;

    output = RenderOutput_AcquireFree();
    if (output == NULL) {
        Resource_ResetEntry((u32)slot);
        return 0;
    }
    x = offset_x + (input->x * 8) + 8;
    y = offset_y + (input->y * 8) + 8;
    x &= 0x1ff;
    y &= 0xff;
    /* Xをbit16～24、Yをbit0～7へ置き、flagsのフラグを重ねる。 */
    output->packed = (x << 16) | y | flags;
    output->table.value =
        ((struct VramBlockCacheEntry *)Ram_VramBlockCache)[slot].offset >> 5;
    output->sentinel = 0xff;
    output->next = 0;
    output->x = x;
    output->y = (s16)y;
    output->index = (s8)slot;
    output->kind = 1;
    output->active = 1;
    RenderOutput_AppendToList((struct RenderOutputList *)input, output);
    return output;
}

s32 Resource_LoadByMode(s32 mode, s32 value);
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

s32 UiIcon_CopyResourceToSlot(s32 code, s32 layers, s32 slot);
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

void Ui_BuildPairedPatternsToSlot(s32 icon, s32 frame, s32 *slot, s32 *tile, s32 reuse);
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
