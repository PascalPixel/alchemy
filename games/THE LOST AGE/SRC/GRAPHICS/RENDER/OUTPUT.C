#include "RENDER_INPUT.H"
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "RESOURCE.H"

void *RenderOutput_AcquireFree(void);
void Resource_ResetEntry(u32);
s32 RenderOutput_AppendToList(void *, void *);

struct TableEntry {
    u16 unused;
    u16 value;
};

s32 Resource_LoadByMode(s32 mode, s32 value);

s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2);

void Ui_BuildPairedPatternsToSlot(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg4);
void *RenderOutput_CreateWithTransform(
    s32 arg0,
    struct RenderInput *arg1,
    s32 arg2,
    s32 arg3)
{
    s32 count;
    s32 unused;
    u8 *result;

    count = Resource_FindFreeEntry();
    if (count == 0x60) {
        return NULL;
    }
    Ui_BuildPairedPatternsToSlot(arg0, 1, &count, &unused, 1);
    result = RenderOutput_Create(count, 0x40000000, arg1, arg2, arg3);
    result[15] = 251;
    return result;
}

