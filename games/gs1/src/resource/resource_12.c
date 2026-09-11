#include "types.h"
#include "scene.h"

/* resource/load_table_entry_to_buffer.c */
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

/* resource/load_kind26_entry_to_buffer.c */
s32 Resource_GetBuffer(s32 index, s32 value);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

s32 Resource_LoadKind26EntryToBuffer(s32 resource, s32 index)
{
    s32 result;
    u8 *work;

    work = Runtime_AllocateBlock(0x11, 0x608);
    Sys_Apply(resource, 0x1a);
    result = Resource_GetBuffer(index, (s32)(work + 0x400));
    Sys_Check(0x11);
    return result;
}

/* resource/load_indexed_entry_to_buffer.c */
s32 Resource_GetBuffer(s32 index, s32 value);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index)
{
    s32 out;
    s32 cur;
    s32 ret;
    u8 *work;

    work = Runtime_AllocateBlock(0x11, 0x608);
    cur = index;
    Sys_SetRange(resource, 0, &cur, &out, 1);
    ret = Resource_GetBuffer(index, (s32)(work + 0x400));
    Sys_Check(0x11);
    return ret;
}
