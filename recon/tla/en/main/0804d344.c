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

void Menu_LoadResourceSlot(s32 slot, s32 index)
{
    s32 size = 1024;
    void *buffer = (void *)Runtime_BumpAllocate(size);
    u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

    /* 表内の相対位置から転送元を求める。 */
    Resource_DecodeByteLz((void *)((u32)base + base[index]), buffer);
    VramBlock_LoadCached(slot, size, buffer);
    Runtime_BumpFree(buffer);
}
