#include "EDITION.H"
#include "FIXED_MATH.H"
#include "INVENTORY_MENU.H"
#include "GLOBAL_CELLS.H"
#include "INVENTORY.H"
#include "ITEM.H"
#include "SOUND_IDS.H"
#include "SYSTEM.H"
extern u8 Data_03001f2c[];
extern u8 MsgItemBroke[];

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
s32 InventoryMenu_ShowModalMessage(s32, s32, s32);
s32 Audio_PlayCue(s32);
void RenderOutput_RedrawSavedRectFar(s32 window);

void ItemMenu_TryBreak(void)
{
    void *menu;

    menu = *(void **)((u32)&Data_03001f2c);
    if ((FIELD(Item_Get(0x1FF & FIELD(menu, u16 *, 0x178)), u8 *, 0xC) == 2) && (Random16() < 0x2000U)) {
        Inventory_BreakFar(
            FIELD(menu, u8 *, 0x21A),
            FIELD(menu, u16 *, 0x174));
        Audio_PlayCue(SOUND_ITEM_BREAK);
        InventoryMenu_ShowModalMessage((s32)MsgItemBroke, 0, -1);
#if !EDITION_INTERNATIONAL
        /* The Japanese menu redraws the info window after the message. */
        RenderOutput_RedrawSavedRectFar(FIELD(menu, s32 *, 0x2C));
#endif
    }
}
