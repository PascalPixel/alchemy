/*
 * Draft: Resource_LoadIndexedEntryToBuffer does not yet match; it does not compile against ⚓️'s headers yet.
 * Links as recon/tla/raw/080452bc.s.
 */
#include "TYPES.H"

s32 Runtime_ReleaseHeapBlock(s32);
s32 Resource_GetBuffer(s32 index, s32 value);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void Ui_PrepareTransferFromTableEntry(u32 index);

s32 ItemIcon_Compose(s32, s32);

void Ability_LoadGlyph(s32, s32, s32 *, s32 *, s32);

s32 Resource_LoadTableEntryToBuffer(s32 resource, s32 index)
{
    s32 result;
    u8 *work;

    work = Runtime_AllocateBlock(0x44, 0x608);
    Ui_PrepareTransferFromTableEntry(resource);
    result = Resource_GetBuffer(index, (s32)(work + 0x400));
    Runtime_ReleaseHeapBlock(0x44);
    return result;
}

s32 Resource_LoadKind26EntryToBuffer(s32 resource, s32 index)
{
    s32 result;
    u8 *work;

    work = Runtime_AllocateBlock(0x44, 0x608);
    ItemIcon_Compose(resource, 0x1a);
    result = Resource_GetBuffer(index, (s32)(work + 0x400));
    Runtime_ReleaseHeapBlock(0x44);
    return result;
}

s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index)
{
    s32 out;
    s32 cur;
    s32 ret;
    u8 *work;

    work = Runtime_AllocateBlock(0x44, 0x608);
    cur = index;
    Ability_LoadGlyph(resource, 0, &cur, &out, 1);
    ret = Resource_GetBuffer(index, (s32)(work + 0x400));
    Runtime_ReleaseHeapBlock(0x44);
    return ret;
}
