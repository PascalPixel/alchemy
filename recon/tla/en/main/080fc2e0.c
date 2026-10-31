// Near miss, differ 2 of 179 (tla-en, 2026-10-31): instruction order only,
// sched2 issues `lsls r3, r3, #1`, the second half of the slot offset 362,
// before the unit's store to [sp, #4]; the native stores between the halves.
// In the sched2 dump the shift outranks the store by one priority (78 to 77),
// so no tie is left for the order to settle.
// The size local moves the copy-back's `lsls r2, r2, #1` after `mov r1, fp` as
// in the native: with 332 written there, its pseudo is set ahead of the call
// address and wins the tie by order (the permuter found it, 2026-10-31).
// Reading the slot and member into locals, as the Broken Seal twin does,
// replaced the permuter's SelectedTargetSlot wrapper (8 with it, 6 without
// either device); swapping slot and member measured 55, a size local set at
// the top 120. Earlier (2026-10-26): no single empty asm or one-pass block
// reaches 0.
// First draft of Func_080fc2e0: after giving or trading the selected item to
// the member in pane 1, offers to equip it there; the twin of ☀️'s
// ItemMenu_ConfirmEquip (games/tbs/src/menu/menu/item.c) in its first-release
// layout (Yes and No at x 96, the cursor at 184). Returns 0 when it is
// equipped; 1 when the player says No, backs out, or the member cannot equip
// it, and the member is left as before.
//
// The native passes the owner to Func_080fae8c a second time, as its target.
// With gInput.pressed volatile the form without the locals measured 15 with
// three arguments and 55 with four.
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
