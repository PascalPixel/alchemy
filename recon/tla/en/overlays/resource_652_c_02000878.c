/* First draft, differ 136 of 189 (189 instructions each): give an item, and
 * when no one can carry it, prompt for an item to drop and retry. Left:
 * allocation and frame. The native keeps the event work in sl, the message
 * field's address in r8 and the saved message in r9 (r8 to r10 saved, frame
 * 8), and the owner in r7; this draft keeps the work in fp, the field address
 * in r7 and the owner in r8, spills the saved message to sp (r8 to fp saved,
 * frame 12), which moves member and slot from sp+4 and sp to sp+8 and sp+4.
 * The native holds the drop prompt's address in r8 and recomputes the field
 * address from sl (226 * 2) at the retry path's two returns. */
#include "types.h"
#include "party_state.h"
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
void UiText_ShowPositionedMessageAndWait(s32 msg, s32 mode);
void UiWork_FinalizePendingCore(void);
u8 *UnitGet(s32 owner);
void UnitCalc(s32 owner);
s32 Inventory_GetQuantity(s32 member, s32 slot);
s32 Inventory_CountItem(s32 member, s32 item);
void Inventory_Discard(s32 member, s32 slot);
void Owner_RefreshClassActions(s32 member);

s32 Func_02000878(s32 item, s32 unused)
{
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
        UiText_ShowPositionedMessageAndWait((s32)MsgItemReceived, 1);
        UiText_ShowPositionedMessageAndWait((s32)MsgItemCannotCarry, 1);
    retry:
        UiText_ShowPositionedMessageAndWait((s32)MsgItemDropPrompt, 1);
        result = Func_02000df4(&member, &slot);
        if (result == -1) {
            if (Func_02000ce4(item)->flags & 8) {
                Func_02000ccc(item, 2);
                UiText_ShowPositionedMessageAndWait((s32)MsgItemDropPrompt + 4, 1);
                goto retry;
            }
            Func_02000ccc(item, 2);
            UiText_ShowPositionedMessageAndWait((s32)MsgItemDropPrompt + 1, 5);
            result = Func_02000dbc(1);
            UiWork_FinalizePendingCore();
            if (result != 0)
                goto retry;
            Func_02000ccc(item, 2);
            UiText_ShowPositionedMessageAndWait((s32)MsgItemDropPrompt + 2, 1);
            work->message = saved;
            return owner;
        } else {
            UnitGet(member);
            count = Inventory_GetQuantity(member, slot);
            if (Inventory_CountItem(member, item) > 29) {
                Func_02000ccc(member, 1);
                Func_02000ccc(item, 2);
                UiText_ShowPositionedMessageAndWait((s32)MsgItemDropPrompt + 7, 1);
                goto retry;
            }
            for (i = 0; i < count; i++)
                Inventory_Discard(member, slot);
            Owner_RefreshClassActions(member);
            UnitCalc(member);
            owner = PartyInventory_Add(item);
            Func_02000dfc(83);
            if (owner == gPartyState.currentOwner) {
                Func_02000ccc(item, 2);
                UiText_ShowPositionedMessageAndWait((s32)MsgItemReceived, 3);
            } else {
                Func_02000ccc(item, 2);
                Func_02000ccc(owner, 1);
                UiText_ShowPositionedMessageAndWait((s32)MsgItemGiven, 3);
            }
            work->message = saved;
            return owner;
        }
    } else {
        Func_02000dfc(83);
        Func_02000ccc(item, 2);
        UiText_ShowPositionedMessageAndWait((s32)MsgItemReceived, 3);
        if (owner != gPartyState.currentOwner) {
            Func_02000ccc(item, 2);
            Func_02000ccc(owner, 1);
            UiText_ShowPositionedMessageAndWait((s32)MsgItemReceived + 1, 1);
        }
        work->message = saved;
    }
    return owner;
}
