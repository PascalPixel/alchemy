#include "types.h"
#include "scene.h"
#include "abi/ui/icon/create_with_resource.h"

extern s32 Resource_FindFreeSlot(void);
extern s32 Resource_CopyData();
extern s32 UiIcon_DrawWithFlags();
extern u8 gRom[];
s32 UiIcon_CreateWithResource(s32 first, s32 unused, s32 second, s32 third)
{
    s32 entry_no;
    s32 result;
    result = 0;
    entry_no = Resource_FindFreeSlot();
    if (entry_no != 0) {
        Resource_CopyData(entry_no, 0x80, gRom);
        result = UiIcon_DrawWithFlags(entry_no, 0x40000000, first, second, third);
    }
    return result;
}
