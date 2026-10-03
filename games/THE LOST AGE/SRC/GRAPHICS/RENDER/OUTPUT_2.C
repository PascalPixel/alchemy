#include "RENDER_INPUT.H"
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "RESOURCE.H"


struct TableEntry {
    u16 unused;
    u16 value;
};

s32 Resource_LoadByMode(s32 mode, s32 value);

s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2);
struct RenderOutput *RenderOutput_CreateLoaded(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    s32 no;
    struct RenderOutput *result;

    no = Resource_FindFreeEntry();
    result = NULL;
    if (no != 0x60) {
        UiIcon_CopyResourceToSlot(arg0, arg1, no);
        result = RenderOutput_Create(no, 0x40000000, arg2, arg3, arg4);
    }
    return result;
}

