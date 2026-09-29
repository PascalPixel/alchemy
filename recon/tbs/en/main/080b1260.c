#include "SHOP.H"
#include "BATTLE_RUNTIME.H"
extern struct ShopRuntime *gMenuWork;

/* main:080b1260 Shop_DrawEquipComparison - hand-written draft, 192 of 264
   halfwords differ (500 of 528 bytes). Left: the ROM dumps two literal
   pools mid-function (one holding the 0x200 equipped bit and the 0x1ff
   item mask), keeps the tried item in its parameter register, and keeps
   the luck pointer on the stack; the stat loop's registers differ.

   Draws how equipping an item would change a member's attack, defense
   and agility: the stat with the item tried on and without it, an arrow
   for each change, and the name of the item it would replace.
   2026-09-29 alchemy permute (seed 1, 3 jobs, 10 minutes): 24,345
   candidates; the best scored 2787 against 3586 (49 register-only, 12
   stack-only, 12 operand, 11 reordered, 5 inserted, 11 deleted) after 6
   rewrites (change loop form, reorder independent statements, reorder local
   declarations, introduce a temporary), none of them kept. The gain comes
   from rewriting the stat row loop as a guarded do-while with a comma-cast
   step, which no programmer writes. The pooled 0xc98 label base is an
   unnamed message; 0x182 is MsgItemName in the catalogs. */

/* Bounded reprise: whole [080b1260,080b1470), 528 bytes including pools.
 * Baseline confirmed: 500/528 bytes, 192 differing halfwords, 130 aligned
 * edits; frame 48 versus 56. Inventory/attack/defense/agility are u16;
 * luck and item type are u8. Canonical Inventory_FindEquipped takes s32
 * type, not u8; text/drawing callees are void and renderer returns a pointer.
 * H1: preserve the actual pool modes: message 0xc8e and mask 0x1ff are
 * halfwords, while the trial item's 0x200 flag is a word. Transfer the
 * existing Value_ halfword idiom without touching loop/declaration order.
 * Prediction: both mid-function pools return, exposing remaining frame and
 * scalar-lifetime disagreements separately. Gate: whole owner exact plus
 * compare-all/test/coverage/verify. Budget: one model plus two justified
 * follow-ups, at most 25 minutes; every result is committed in this header.
 * H1 result: 504/528 bytes, 236 differing halfwords, 136 aligned edits.
 * Frame remains 48. The narrowed linked message forces a pool at +0x5c
 * (too early; ROM +0xc0); the narrowed linked mask is widened through the
 * AND and remains in the final pool at +0x1e4 (ROM +0x10c). Equipped flag
 * now uses its required pool load. No adoption; debug allocator text drifts,
 * so its role assignments are not used as evidence for a register sweep.
 * H2: model the packed-item value boundaries instead of narrowing link
 * constants: u16 trial item (caller 080b0aac passes an ldrh), and s16 replaced
 * item (masked ID or -1). Plain numeric masks operate on these narrow values.
 * The raw .2byte/zero padding does not by itself prove the old pool's mode;
 * H1's interpretation was a hypothesis, disproved by its placement result.
 * Prediction: the trial-item OR and replacement mask acquire the short pool
 * constraints naturally, while the message remains a normal s32 argument.
 * H2 result: 512/528 bytes, 240 differing halfwords, 142 aligned edits.
 * The narrow incoming argument creates sign/zero-extension shifts absent in
 * the ROM; neither required mid-function pool returns, and frame stays 48.
 * Rejected: halfword storage at the caller does not imply a halfword ABI
 * parameter. Keep this negative result; do not repeat parameter narrowing.
 * The canonical draft below restores the word-width input/replacement model.
 */

extern u8 Value_00000c98[];
extern u8 MsgItemName[];

struct ItemDefinition *Item_Get(s32 item);
void UiWindow_Clear(s32 window);
void UiText_DrawMessageAt(s32 message, s32 window, s32 x, s32 y);
s32 Item_CanOwnerEquip(s32 unit_id, s32 item_id);
s32 Inventory_FindEquippedFar(s32 unit_id, s32 kind);
struct ShopCursorAnchor *RenderOutput_CreateFar(u32 resource, u32 flags, s32 window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 y, s32 width, s32 style);

void Shop_DrawEquipComparison(s32 window, s32 unit_id, s32 item_id)
{
    struct ShopRuntime *shop = gMenuWork;
    struct BattleUnit *unit = Owner_GetStateFar(unit_id);
    struct ItemDefinition *item = Item_Get(item_id);
    s32 replaced = -1;
    s32 slot;
    u32 saved;
    s32 i;
    s32 y;
    s32 row;
    s32 x;
    s32 with[4];
    s32 without[4];

    if (window == 0)
        return;
    UiWindow_Clear(window);
    if (!Item_CanOwnerEquip(unit_id, item_id)) {
        UiText_DrawMessageAt(0xc8e, window, 8, 24);
        return;
    }
    slot = Inventory_FindEquippedFar(unit_id, item->type);
    if (slot == replaced) {
        for (i = 0; i <= 14; i++) {
            if (!(unit->inventory[i] & 0x200))
                break;
        }
        if (i == 15) {
            for (i = 0; i <= 14; i++) {
                if (Item_Get(unit->inventory[i])->type == 6)
                    break;
            }
            if (i == 15)
                i = 0;
        }
        slot = i;
    } else {
        replaced = unit->inventory[slot] & 0x1ff;
    }
    item_id |= 0x200;
    saved = unit->inventory[slot];
    unit->inventory[slot] = item_id;
    BattleUnit_Recalculate(unit_id);
    with[0] = unit->attack;
    with[1] = unit->defense;
    with[2] = unit->agility;
    with[3] = unit->luck;
    unit->inventory[slot] = saved;
    BattleUnit_Recalculate(unit_id);
    without[0] = unit->attack;
    without[1] = unit->defense;
    without[2] = unit->agility;
    without[3] = unit->luck;
    row = 2;
    for (i = 0, y = 0; i <= 2; i++, y += 16) {
        if (without[i] > with[i]) {
            RenderOutput_CreateFar(shop->stat_up_icon, 0x40000000, window, 56, y - 4)->unknown_00[4] = 0;
        } else if (without[i] < with[i]) {
            RenderOutput_CreateFar(shop->stat_down_icon, 0x40000000, window, 56, y - 4)->unknown_00[4] = 0;
        }
        UiText_DrawNumberInWindowFar(without[i], 3, window, 32, y);
        if (without[i] != with[i])
            UiText_DrawNumberInWindowFar(with[i], 3, window, 72, y);
        UiText_DrawCharacterAtOffsetFar((s32)Value_00000c98 + i, window, 0, y);
        UiWindow_DrawDividerLineFar(window, 0, row, 13, row);
        row += 2;
    }
    if (replaced != -1)
        UiText_DrawCharacterAtOffsetFar(replaced + (s32)MsgItemName, window, 0, 48);
}
