#include "types.h"
#include "heap_release.h"
#include "input.h"
#include "flag_call.h"
#include "iwram_call.h"
#include "ram_buffer.h"

struct InventoryMenuState {
  u8 unk000[0xf0];
  s32 messageWindow;
  u8 unk0f4[0x74];
  u16 selectedSlots[2];
  u8 unk16c[0xaa];
  u8 paneOwner[2];
};

extern u8 MsgYes[];           // the ☀️ twin's name
extern u8 MsgEquipThisItem[]; // the ☀️ twin's name

void *Unit_Get(s32 unit);
void Func_080fae8c(s32 owner, s32 slot, s32 mode, s32 target);
s32 Inventory_EquipFar(s32 owner, s32 slot);
void Text_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Window_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width,
                                  s32 height);
void Func_080f8ab4(s32 x, s32 y);
void Func_080f8a44(s32 x, s32 y);
void Sound_PlayFar(s32 cue);
void Unit_CalculateFar(s32 unit);
void Unit_RefreshPsynergyFar(s32 unit);
void wait(s32 frames);

s32 Func_080fc2e0(s32 unused) {
  s32 rows = 2;
  s32 selection = 0;
  s32 changed = 1;
  struct InventoryMenuState *menu =
      (struct InventoryMenuState *)Ram_HeapSlots->menuRuntime;
  void *unit = Unit_Get(menu->paneOwner[1]);
  void *saved;
  s32 window;
  s32 slot;
  s32 member;
  s32 size;

  slot = menu->selectedSlots[1];
  member = menu->paneOwner[1];
  Func_080fae8c(member, slot, 0, member);
  /* FAKEMATCH(temporary): the unit size held in size for the allocation and
   * both copies; as 332 at each, 6 of 179 instructions differ in tla-en. */
  size = 332;
  saved = Heap_Alloc(size);
  Iwram_CopyWords(saved, unit, size);
  window = menu->messageWindow;
  if ((u32)(Inventory_EquipFar(menu->paneOwner[1], menu->selectedSlots[1]) +
            2) <= 1) {
    selection = 1;
  } else {
    Text_DrawCharacterAtOffsetFar((s32)MsgYes, window, 96, 0);
    Text_DrawCharacterAtOffsetFar((s32)MsgYes + 1, window, 96, 16);
    Window_ClearInteriorTilesFar(window, 16, 16, 96, 24);
    Text_DrawCharacterAtOffsetFar((s32)MsgEquipThisItem, window, 0, 16);
    Func_080f8ab4(184, 5);
    for (;;) {
      if (GameFlag_Test(336))
        break;
      if (changed) {
        changed = 0;
        selection = (selection + rows) % rows;
      }
      if (gInput.pressed & 1) {
        Sound_PlayFar(175);
        break;
      }
      if (gInput.pressed & 2) {
        Sound_PlayFar(113);
        selection = 1;
        break;
      }
      Func_080f8a44(184, (selection << 4) + 5);
      if (gInput.repeat & 64) {
        selection--;
        changed = 1;
        Sound_PlayFar(111);
      }
      if (gInput.repeat & 128) {
        selection++;
        changed = 1;
        Sound_PlayFar(111);
      }
      wait(1);
    }
  }
  if (GameFlag_Test(336))
    selection = 1;
  if (selection == 1)
    Iwram_CopyWords(unit, saved, size);
  Heap_Release(saved);
  Unit_RefreshPsynergyFar(menu->paneOwner[1]);
  Unit_CalculateFar(menu->paneOwner[1]);
  return selection;
}
