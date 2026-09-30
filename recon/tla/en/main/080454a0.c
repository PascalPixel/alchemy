/*
 * Draft: Resource_LoadKind26EntryToBuffer does not yet match; it does not compile against ⚓️'s headers yet.
 * Links as recon/tla/raw/080452bc.s.
 */
#include "TYPES.H"

s32 Runtime_ReleaseHeapBlock(s32);
s32 Resource_GetBuffer(s32 index, s32 value);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

s32 ItemIcon_Compose(s32, s32);

s32 Resource_LoadKind26EntryToBuffer(s32 resource, s32 index)
{
    s32 result;
    u8 *work;

    work = Runtime_AllocateBlock(0x11, 0x608);
    ItemIcon_Compose(resource, 0x1a);
    result = Resource_GetBuffer(index, (s32)(work + 0x400));
    Runtime_ReleaseHeapBlock(0x11);
    return result;
}
