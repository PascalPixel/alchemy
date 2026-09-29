/* 2026-09-29 alchemy permute: score 1115 to 400 (with the build's names) on the permuter's scorer
   (0 is exact); remaining 3 register-only, 8 operand, 2 reordered, 1
   inserted. Kept rewrites: 4x reorder independent statements, 3x reorder
   local declarations, 3x introduce a temporary, 2x swap commutative
   operands, 2x test truth or compare with zero, 1x drop a same-width cast,
   1x change loop form, 1x pointer arithmetic or indexing, 1x move an
   assignment into or out of a condition, 1x toggle register. FAKEMATCH:
   the permuter's temporaries, register hints and swapped operand orders
   below only steer allocation and scheduling; no programmer would write
   them, so they stay tagged until a natural spelling replaces them. */
#include "TYPES.H"

/* main:08091a58 PartyInventory_GiveItem - draft, 95 of 226
   halfwords differ (436 of 452 bytes, 2026-09-26). The discard quantity
   now decrements, matching the operation. Branch-local result lifetime
   and a volatile publication tail do not change the 436-byte result.
   Residual: its copy is too early;
   the ROM keeps three
   separate stores of the saved message position and a separate call in
   the leader branch of the discard path, where this C is cross-jumped; the
   position pointer and loop message share r7 in the ROM.

   Gives the party one item. When no member has room, the player picks an
   item to throw away (or gives up the new one), and the text box position
   the event had is restored afterwards. Returns the member who received the
   item, or -1. */

extern u8 Value_0000096a[];
extern u8 Value_00000977[];
extern u8 Value_00000978[];

struct ItemData {
    u8 unknown_00[3];
    u8 flags;
};

struct EventWork {
    u8 unknown_000[472];
    s16 message_position;
};

extern struct EventWork *gEventWork;
extern s32 gGameState[];

s32 PartyInventory_AddFar(s32 item);
struct ItemData *Item_Get(s32 item);
void *Owner_GetStateFar(s32 owner);
s32 Shop_GetSelectionState(s32 owner, s32 slot);
void Inventory_DiscardFar(s32 owner, s32 slot);
void Item_AdjustCounterFar(s32 item, s32 delta);
void UiWork_PushValueSlotFar(s32 value, s32 slot);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 position);
void UiWork_FinalizePendingCoreFar(void);
s32 Shop_PickUnitItemFar(s32 *owner, s32 *slot);
s32 Object_CallSpawnRoutineAtOrigin(s32 mode);
void Audio_PlayCue(s32 cue);

s32 PartyInventory_GiveItem(s32 item)
{
    struct EventWork *work;
    s16 *position;
    s32 owner;
    s16 saved;
    s32 message;
    s32 text;
    s32 result;
    s32 member;
    s32 slot;
    register s32 count;

    work = gEventWork;
    position = &work->message_position;
    saved = *position;
    owner = PartyInventory_AddFar(item);
    if (owner == -1) {
        UiWork_PushValueSlotFar(item, 2);
        UiText_ShowPositionedMessageAndWaitFar((s32)Value_0000096a, 1);
        UiText_ShowPositionedMessageAndWaitFar((s32)Value_00000977, 1);
    retry:
        while (1) {
            message = (s32)Value_00000978;
            UiText_ShowPositionedMessageAndWaitFar(message, 1);
            result = Shop_PickUnitItemFar(&member, &slot);
            if (result == -1) {
                s32 tmp2;
                if (Item_Get(item)->flags & 8) {
                    UiWork_PushValueSlotFar(item, 2);
                    UiText_ShowPositionedMessageAndWaitFar(message + 4, 1);
                    goto retry;
                }
                UiWork_PushValueSlotFar(item, 2);
                UiText_ShowPositionedMessageAndWaitFar(message + 1, 5);
                tmp2 = Object_CallSpawnRoutineAtOrigin(1);
                UiWork_FinalizePendingCoreFar();
                result = tmp2;
                if (result != 0)
                    goto retry;
                Item_AdjustCounterFar(item, 1);
                UiWork_PushValueSlotFar(item, 2);
                UiText_ShowPositionedMessageAndWaitFar(2 + message, 1);
                work->message_position = saved;
            } else {
                s32 tmp;
                s32 tmp3;
                Owner_GetStateFar(member);
                if ((result = Shop_GetSelectionState(member, slot)) > 0) {
                    count = result;
                    do {
                        Inventory_DiscardFar(member, slot);
                        count--;
                    } while (count != 0);
                }
                tmp3 = PartyInventory_AddFar(item);
                tmp = tmp3;
                owner = tmp;
                Audio_PlayCue(83);
                if (owner == gGameState[125]) {
                    UiWork_PushValueSlotFar(item, 2);
                    UiText_ShowPositionedMessageAndWaitFar((s32)Value_0000096a, 3);
                } else {
                    UiWork_PushValueSlotFar(item, 2);
                    UiWork_PushValueSlotFar(owner, 1);
                    UiText_ShowPositionedMessageAndWaitFar(1 + (s32)Value_0000096a, 3);
                }
                /* FAKEMATCH: distinguish this published position restore
                   from the discard branch's otherwise identical tail. */
                *&work->message_position = saved;
                return owner;
            }
            if (!0)
                break;
        }
    } else {
        Audio_PlayCue(83);
        UiWork_PushValueSlotFar(item, 2);
        text = (s32)Value_0000096a;
        UiText_ShowPositionedMessageAndWaitFar(text, 3);
        if (owner != gGameState[125]) {
            UiWork_PushValueSlotFar(item, 2);
            UiWork_PushValueSlotFar(owner, 1);
            UiText_ShowPositionedMessageAndWaitFar(text + 1, 3);
        }
        position[0] = saved;
    }
    return owner;
}
