#include "types.h"
#include "scene.h"
#include "abi/resource/load_table_entry_to_buffer.h"

s32 Resource_GetBuffer(s32 index, s32 value);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

s32 Resource_LoadTableEntryToBuffer(s32 resource, s32 index)
{
    s32 result;
    u8 *work;

    work = Runtime_AllocateBlock(0x11, 0x608);
    Sys_Do(resource);
    result = Resource_GetBuffer(index, (s32)(work + 0x400));
    Sys_Check(0x11);
    return result;
}
