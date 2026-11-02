// Draft, differ 136 of 189, skeleton gap 25 (tla-en, 2026-11-01)
// Twin of The Broken Seal's PartyInventory_GiveItem: give an item, and when no
// one can carry it, prompt for an item to drop and retry. Same flow, same
// message order and the same cap of 30; this game also refreshes the member's
// Psynergy and stats after the discard, and has no drop counter call.
// Left: one cause, then a register swap. The statements below are right.
// 1. gcse's PRE hoists &member and &slot over the entry edge to the retry
// label (.07.gcse: "PRE/HOIST: edge (1,2)"). The two new pseudos are live
// across the loop, take r9 and sl, push the event work to fp and spill the
// saved message: frame 12, r8 to fp saved, and the second and first returns
// cross-jump. The native has no such pseudos, since it computes both addresses
// in the loop (add r0, sp, #4; mov r1, sp), keeps the saved message in r9 and
// the work in sl, and leaves r11 free. That also rules out the hoisted pseudos
// losing allocation and being rebuilt from their equivalence.
// Probe, not kept (S1, S2): an empty asm clobbering sfp at the label stops the
// hoist and gives differ 83, skeleton 18/11, frame 8, r8 to r10 saved, and both
// inner returns as the native has them. So the inner returns are the source's:
// one exit gives differ 133 here, but 82 and 23/11 under the same probe, and
// the first return merges. Nothing in plain C kills the expression; the entry
// edge and the back edges leave LCM no other choice.
// 2. Under the probe all that remains is r7 and r8 swapped: the native has the
// owner in r7 and the prompt in r8, this draft the reverse. The prompt has 15
// refs over 118 insns at this loop depth and outranks the owner's 8 over 144;
// the native's order needs the prompt's refs nearer 6. A local msg set at the
// label (the native's load is inside the loop) changes neither number.
// Tried without gain: for (;;) and do with continue, one back edge (for with
// break rotates the loop: differ 215), a struct or an array for member and slot
// (194 instructions, one hoisted base), member and slot in an inner scope or
// swapped, saved as s16 or u16, a pointer to the message field, an unprototyped
// picker, a shared exit label, each return kept alone (differ 133 and 142).
// Rejected, differ 132: msg += 4 and += 7 into one tail, a local reused.
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
