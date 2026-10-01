/* Near miss: score 75. The selection work is ⚓️'s heap slot
   menu_select_work. ⚓️ copies the entry number into r10 straight after
   loading the work, before copying the work pointer; this draft copies it
   after reading the entry count. 45 s of permuting found nothing. */
#include "TYPES.H"
#include "RESOURCE.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"

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

    base = Ram_HeapSlots->menu_select_work;
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
