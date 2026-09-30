#include "TYPES.H"
#include "RESOURCE.H"
#include "SYSTEM.H"
extern u8 Data_03001f38[];

struct MenuSelectionState {
    u8 padding000[0x78];
    void *work;
    u8 padding07c[8];
    u8 resource_ids[8];
    s16 selection;
    s16 item_count;
    s16 field090;
    s16 resource_base;
};

extern struct MenuSelectionState *gMenuSelectWork;
extern u8 Menu_SelectionStepDelays[];
extern u8 MsgCommandName;

void RenderOutput_PrepareForRedraw(void *work);
void UiText_DrawCharacterAtOffset(s32 resource_id, void *work, s32 x, s32 y);
void Audio_PlayCue(s32 sound_id);

void Menu_AppendResourceEntry(s32 no)
{
    u8 *base;
    u8 *entry;
    s16 index;
    s32 slot;
    s32 off;
    s32 flags;

    base = *(u8 **)((u32)&Data_03001f38);
    index = *(s16 *)(base + 142);
    if (index <= 5)
    {
        *(u16 *)(base + 142) = *(u16 *)(base + 142) + 1;
        entry = base + index * 20;
        slot = Resource_FindFreeEntry();
        Menu_LoadResourceSlot(slot, no);
        *(u16 *)(entry + 12) = index * 24 + 32;
        flags = 136;
        *(u16 *)(entry + 14) = flags;
        off = index + 132;
        *(u16 *)(entry + 18) = slot;
        base[off] = (u8)no;
    }
}
