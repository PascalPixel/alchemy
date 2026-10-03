#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"

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
