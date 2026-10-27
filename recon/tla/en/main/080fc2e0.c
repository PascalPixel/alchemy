// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 8 of 179 in tla-en. First difference: this issues `lsls r3, r3, #1` earlier
// (row 25) than the native does (row 27). No single empty asm or one-pass block
// reaches 0.
// First draft of Func_080fc2e0: after giving or trading the selected item to
// the member in pane 1, offers to equip it there; the twin of ☀️'s
// ItemMenu_ConfirmEquip (games/tbs/src/menu/menu.c) in its first-release
// layout (Yes and No at x 96, the cursor at 184). Returns 0 when it is
// equipped; 1 when the player says No, backs out, or the member cannot equip
// it, and the member is left as before.
//
// Near miss, differ 8 of 179, instruction order only: four times the native
// issues a load between or ahead of a constant's halves (the slot offset 362
// around the unit store, the 332 copy size around the copy-back loads). The
// native passes the owner to Func_080fae8c a second time, as its target. With
// gInput.pressed volatile the form without the helper measured 15 with three
// arguments and 55 with four; the helper came from the permuter (2026-10-19).
#include "types.h"
#include "mem.h"
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
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width,
                                    s32 height);
void Func_080f8ab4(s32 x, s32 y);
void Func_080f8a44(s32 x, s32 y);
void Audio_PlayCue(s32 cue);
void Unit_CalculateFar(s32 unit);
void Unit_RefreshPsynergyFar(s32 unit);
void WaitFrames(s32 frames);

// FAKEMATCH(wrapper): the slot read through a helper at both uses; as
// menu->selectedSlots[1] in place, 55 of 179 instructions differ in tla-en.
static inline u16 SelectedTargetSlot(struct InventoryMenuState *menu) {
  return menu->selectedSlots[1];
}

s32 Func_080fc2e0(s32 unused) {
  s32 rows = 2;
  s32 selection = 0;
  s32 changed = 1;
  struct InventoryMenuState *menu =
      (struct InventoryMenuState *)Ram_HeapSlots->menuRuntime;
  void *unit = Unit_Get(menu->paneOwner[1]);
  void *saved;
  s32 window;

  Func_080fae8c(menu->paneOwner[1], SelectedTargetSlot(menu), 0,
                menu->paneOwner[1]);
  saved = MemAlloc(332);
  Iwram_CopyWords(saved, unit, 332);
  window = menu->messageWindow;
  if ((u32)(Inventory_EquipFar(menu->paneOwner[1], SelectedTargetSlot(menu)) +
            2) <= 1) {
    selection = 1;
  } else {
    UiText_DrawCharacterAtOffsetFar((s32)MsgYes, window, 96, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgYes + 1, window, 96, 16);
    UiWindow_ClearInteriorTilesFar(window, 16, 16, 96, 24);
    UiText_DrawCharacterAtOffsetFar((s32)MsgEquipThisItem, window, 0, 16);
    Func_080f8ab4(184, 5);
    for (;;) {
      if (GameFlag_Test(336))
        break;
      if (changed) {
        changed = 0;
        selection = (selection + rows) % rows;
      }
      if (gInput.pressed & 1) {
        Audio_PlayCue(175);
        break;
      }
      if (gInput.pressed & 2) {
        Audio_PlayCue(113);
        selection = 1;
        break;
      }
      Func_080f8a44(184, (selection << 4) + 5);
      if (gInput.repeat & 64) {
        selection--;
        changed = 1;
        Audio_PlayCue(111);
      }
      if (gInput.repeat & 128) {
        selection++;
        changed = 1;
        Audio_PlayCue(111);
      }
      WaitFrames(1);
    }
  }
  if (GameFlag_Test(336))
    selection = 1;
  if (selection == 1)
    Iwram_CopyWords(unit, saved, 332);
  MemFree(saved);
  Unit_RefreshPsynergyFar(menu->paneOwner[1]);
  Unit_CalculateFar(menu->paneOwner[1]);
  return selection;
}
