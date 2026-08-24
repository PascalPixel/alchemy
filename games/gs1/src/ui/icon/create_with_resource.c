#include "types.h"

extern s32 Resource_FindFreeSlot(void);
extern s32 Resource_CopyData();
extern s32 UiIcon_DrawWithFlags();
extern u8 Data_080aea4c[];
s32 UiIcon_CreateWithResource(s32 first, s32 unused, s32 second, s32 third)
{
    s32 entry_no;
    s32 result;
    result = 0;
    entry_no = Resource_FindFreeSlot();
    if (entry_no != 0) {
        Resource_CopyData(entry_no, 0x80, Data_080aea4c);
        result = UiIcon_DrawWithFlags(entry_no, 0x40000000, first, second, third);
    }
    return result;
}
