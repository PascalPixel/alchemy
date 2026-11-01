// Draft, differ 136 of 189, skeleton gap 25 (tla-en, 2026-11-01)
// Twin of The Broken Seal's PartyInventory_GiveItem: give an item, and when no
// one can carry it, prompt for an item to drop and retry. Same flow, same
// message order and the same cap of 30; this game also refreshes the member's
// Psynergy and stats after the discard, and has no drop counter call.
// Left: allocation and frame. The native keeps the event work in sl, the
// message field's address in r8 and the saved message in r9 (r8 to r10 saved,
// frame 8), and the owner in r7; this draft keeps the work in fp, the field
// address in r7 and the owner in r8, spills the saved message to sp (r8 to fp
// saved, frame 12), which moves member and slot from sp+4 and sp to sp+8 and
// sp+4. The native holds the drop prompt's address in r8 and recomputes the
// field address from sl (226 * 2) at the retry path's two returns, and it does
// not cross-jump the first return into the second as this draft does.
// The cause is one step: the gcse pass hoists &member and &slot over the entry
// edge to the retry label, and with three back edges the label sits at loop
// depth 3, so those two pseudos outrank the saved message, which is spilled.
// The twin's matched bytes do exactly this (fp used, saved on the stack, both
// addresses set before the label), so this draft already has the twin's shape
// and the native differs from it here: it computes both addresses in the loop.
// Tried without gain: for (;;) and do with continue (184 instructions, gap 34
// and 29; the addresses still hoist), member and slot in an inner scope or
// swapped, an array or a struct for them (195 instructions), saved as s16 or
// u16, a pointer to the message field, an unprototyped picker. One exit for
// all paths, dropping the inner returns, gives differ 133 but gap 26 and 25.
#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "item.h"
#include "text/msg_ids.h"

struct DailaEventWork {
  u8 unk000[0x1c4];
  s16 message;
};

s32 PartyInventory_Add(s32 item);
void Func_02000ccc(s32 value, s32 slot);
struct ItemDefinition *Func_02000ce4(s32 item);
s32 Func_02000df4(s32 *member, s32 *slot);
s32 Func_02000dbc(s32 mode);
void Func_02000dfc(s32 cue);
void FieldMessage_Show(s32 msg, s32 mode);
void Message_CloseFinishedWindows(void);
u8 *Unit_Get(s32 owner);
void Unit_Calculate(s32 owner);
s32 Inventory_GetQuantity(s32 member, s32 slot);
s32 Inventory_CountItem(s32 member, s32 item);
void Inventory_Discard(s32 member, s32 slot);
void Unit_RefreshPsynergy(s32 member);

s32 Func_02000878(s32 item, s32 unused) {
  struct DailaEventWork *work;
  s32 saved;
  s32 owner;
  s32 result;
  s32 count;
  s32 i;
  s32 member;
  s32 slot;

  work = Ram_HeapSlots->eventWork;
  saved = work->message;
  owner = PartyInventory_Add(item);
  if (owner == -1) {
    Func_02000ccc(item, 2);
    FieldMessage_Show((s32)MsgItemReceived, 1);
    FieldMessage_Show((s32)MsgItemCannotCarry, 1);
  retry:
    FieldMessage_Show((s32)MsgItemDropPrompt, 1);
    result = Func_02000df4(&member, &slot);
    if (result == -1) {
      if (Func_02000ce4(item)->flags & 8) {
        Func_02000ccc(item, 2);
        FieldMessage_Show((s32)MsgItemDropPrompt + 4, 1);
        goto retry;
      }
      Func_02000ccc(item, 2);
      FieldMessage_Show((s32)MsgItemDropPrompt + 1, 5);
      result = Func_02000dbc(1);
      Message_CloseFinishedWindows();
      if (result != 0)
        goto retry;
      Func_02000ccc(item, 2);
      FieldMessage_Show((s32)MsgItemDropPrompt + 2, 1);
      work->message = saved;
      return owner;
    } else {
      Unit_Get(member);
      count = Inventory_GetQuantity(member, slot);
      if (Inventory_CountItem(member, item) > 29) {
        Func_02000ccc(member, 1);
        Func_02000ccc(item, 2);
        FieldMessage_Show((s32)MsgItemDropPrompt + 7, 1);
        goto retry;
      }
      for (i = 0; i < count; i++)
        Inventory_Discard(member, slot);
      Unit_RefreshPsynergy(member);
      Unit_Calculate(member);
      owner = PartyInventory_Add(item);
      Func_02000dfc(83);
      if (owner == gMainRoutineWork.currentOwner) {
        Func_02000ccc(item, 2);
        FieldMessage_Show((s32)MsgItemReceived, 3);
      } else {
        Func_02000ccc(item, 2);
        Func_02000ccc(owner, 1);
        FieldMessage_Show((s32)MsgItemGiven, 3);
      }
      work->message = saved;
      return owner;
    }
  } else {
    Func_02000dfc(83);
    Func_02000ccc(item, 2);
    FieldMessage_Show((s32)MsgItemReceived, 3);
    if (owner != gMainRoutineWork.currentOwner) {
      Func_02000ccc(item, 2);
      Func_02000ccc(owner, 1);
      FieldMessage_Show((s32)MsgItemReceived + 1, 1);
    }
    work->message = saved;
  }
  return owner;
}
