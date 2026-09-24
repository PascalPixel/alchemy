#include "TYPES.H"

s32 Runtime_ReleaseHeapBlock(s32);
s32 Resource_GetBuffer(s32 index, s32 value);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void Ui_PrepareTransferFromTableEntry(u32 index);

s32 Resource_LoadTableEntryToBuffer(s32 resource, s32 index)
{
    s32 result;
    u8 *work;

    work = Runtime_AllocateBlock(0x11, 0x608);
    Ui_PrepareTransferFromTableEntry(resource);
    result = Resource_GetBuffer(index, (s32)(work + 0x400));
    Runtime_ReleaseHeapBlock(0x11);
    return result;
}

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

void Ability_LoadGlyph(s32, s32, s32 *, s32 *, s32);

s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index)
{
    s32 out;
    s32 cur;
    s32 ret;
    u8 *work;

    work = Runtime_AllocateBlock(0x11, 0x608);
    cur = index;
    Ability_LoadGlyph(resource, 0, &cur, &out, 1);
    ret = Resource_GetBuffer(index, (s32)(work + 0x400));
    Runtime_ReleaseHeapBlock(0x11);
    return ret;
}

s32 Func_0801a4fc(u32, s32, s32 *, s32 *, s32, s32);
s32 GameFlag_IsSet(s32);

s32 Ui_LoadEntryForKind(u32 kind, s32 value)
{
    s32 out;
    s32 cur;
    u32 no;

    no = kind;
    cur = value;
    if (no > 7U) {
        no = 0;
    }
    if (GameFlag_IsSet(0x20) != 0) {
        switch (no) {
        case 0:
            no = 0x38;
            break;
        case 1:
            no = 0x39;
            break;
        }
    }
    Func_0801a4fc(no, 0, &cur, &out, 0xE, 1);
    return out;
}
