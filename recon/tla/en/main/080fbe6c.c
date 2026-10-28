// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 8 of 298 in tla-en. The native spills its arguments to the stack before it
// builds the heap slots' address; this builds the address first (row 8). No
// single empty asm or one-pass block reaches 0.
// First draft of Func_080fbe6c: "How many?", the item menu's quantity
// selector for a stack of the selected item. Left and Right move the count
// from 1 to range, the window shows what the owner in pane 0 keeps and, in
// mode 0, what the member in pane 1 will have; A returns the choice less one
// and B -1. Two arrows built from gMenu_QuantityTiles flank the count.
//
// Near miss, differ 12 of 298, instruction order only: the native stores the
// two stacked parameters before it builds the heap address, stores the 1 for
// changed after the 0 for targetCount, and twice loads the selected item
// before it builds the 0x1ff mask. value initialized from base before
// anything else (the copy then lives longest and takes r8, the window r7) and
// entry declared before changed (the spill slots' order) took 150 to 12.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "io_reg.h"
#include "dma.h"

struct QuantityIcon {
  u8 unk00[5];
  u8 state;
  u8 unk06[0x12];
  u16 tile : 10;
  u16 attr2High : 6;
};

struct QuantityMenu {
  u8 unk000[0xf0];
  void *messageWindow;
  u8 unk0f4[0x78];
  u16 selectedItems[2];
  u8 unk170[0xa6];
  u8 paneOwner[2];
  struct QuantityIcon *selectedItemIcon;
};

// "How many?"
extern u8 MsgHowMany[];
extern u8 MsgItemName[];
extern const u8 gMenu_QuantityTiles[];

void *Runtime_AllocateBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void ItemMenu_SetMessageWindowHeight7(void);
void WindowSprite_RedrawSavedRectFar(void *window);
void WindowSprite_ClearListFar(void *window);
s32 InventoryMenu_GetItemQuantity(s32 owner, s32 item);
s32 Resource_FindFreeEntry(void);
void VramCache_Load(s32 entry, s32 size, void *tiles);
struct QuantityIcon *WindowSprite_CreateFar(s32 entry, u32 attributes,
                                            void *window, s32 x, s32 y);
void Func_080f8ab4(s32 x, s32 y);
void Func_080f8a44(s32 x, s32 y);
void Text_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);
void Text_DrawNumberInWindowFar(s32 value, s32 digits, void *window, s32 x,
                                  s32 y);
void Text_DrawStringAtOffsetFar(void *text, void *window, s32 x, s32 y);
void Shop_FillSelectorFar(s32 value, s32 column, void *tiles);
void *Unit_Get(s32 owner);
s32 GameFlag_Test(s32 flag);
void Sound_PlayFar(s32 cue);
void WaitFrames(s32 frames);

s32 Func_080fbe6c(s32 base, s32 range, s32 mode) {
  s32 value = base;
  struct QuantityMenu *menu = Ram_HeapSlots->menuRuntime;
  u8 *tiles = Runtime_AllocateBlock(56, 0x400);
  s32 entry;
  s32 changed = 1;
  s32 targetCount = 0;
  void *window = menu->messageWindow;
  s32 ownerCount;
  struct QuantityIcon *arrow;

  ItemMenu_SetMessageWindowHeight7();
  WindowSprite_RedrawSavedRectFar(window);
  if (mode == 0)
    targetCount = InventoryMenu_GetItemQuantity(menu->paneOwner[1],
                                                menu->selectedItems[0] & 0x1ff);
  ownerCount = InventoryMenu_GetItemQuantity(menu->paneOwner[0],
                                             menu->selectedItems[0] & 0x1ff);
  entry = Resource_FindFreeEntry();
  if (entry != 96) {
    VramCache_Load(entry, 0x100, 0);
    WindowSprite_CreateFar(entry, 0x40004000, window, 48, 32);
    arrow = WindowSprite_CreateFar(entry, 0x40004000, window, 80, 32);
    arrow->tile = arrow->tile + 4;
    Func_080f8ab4(128, 40);
    while (!GameFlag_Test(0x150)) {
      if (changed) {
        changed = 0;
        value = (value + range) % range;
        WindowSprite_RedrawSavedRectFar(window);
        Text_DrawCharacterAtOffsetFar((s32)MsgHowMany, window, 32, 0);
        Dma_Set(gMenu_QuantityTiles, tiles, (DMA_ENABLE | DMA_32BIT) << 16 | 64,
                REG_DMA3);
        Shop_FillSelectorFar(30, 14, tiles);
        Shop_FillSelectorFar(base + range, 0, tiles);
        Shop_FillSelectorFar(base + value + 1, 10, tiles);
        Shop_FillSelectorFar(base, 2, tiles);
        VramCache_Load(entry, 0x100, tiles);
        Text_DrawNumberInWindowFar(value + 1, 2, window, 32, 32);
        Text_DrawCharacterAtOffsetFar(
            (menu->selectedItems[0] & 0x1ff) + (s32)MsgItemName, window, 16, 8);
        Text_DrawNumberInWindowFar(ownerCount - value - 1, 2, window, 16, 24);
        if (mode == 0)
          Text_DrawNumberInWindowFar(targetCount + value + 1, 2, window, 80,
                                       24);
        Text_DrawStringAtOffsetFar(Unit_Get(menu->paneOwner[0]), window, 16,
                                     16);
        if (mode == 0)
          Text_DrawStringAtOffsetFar(Unit_Get(menu->paneOwner[1]), window, 80,
                                       16);
      }
      if (gInput.pressed & 1) {
        Sound_PlayFar(112);
        break;
      }
      if (gInput.pressed & 2) {
        value = -1;
        Sound_PlayFar(113);
        break;
      }
      Func_080f8a44(128, 40);
      if (gInput.repeat & 32) {
        value--;
        changed = 1;
        Sound_PlayFar(111);
      }
      if (gInput.repeat & 16) {
        value++;
        changed = 1;
        Sound_PlayFar(111);
      }
      WaitFrames(1);
    }
  }
  WindowSprite_RedrawSavedRectFar(window);
  WindowSprite_ClearListFar(window);
  Runtime_ReleaseHeapBlock(56);
  menu->selectedItemIcon->state = 13;
  if (GameFlag_Test(0x150))
    value = -1;
  return value;
}
