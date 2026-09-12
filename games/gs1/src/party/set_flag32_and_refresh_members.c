#include "scene.h"
#include "game_flags.h"
#include "inventory.h"
#include "party_state.h"
#include "owner_state.h"
#include "types.h"
#include "item.h"
#include "battle_runtime.h"

/* party/set_flag32_and_refresh_members.c */
/* party/set_flag32_and_refresh_members.c */
void Owner_RefreshDerivedData(s32 arg0);

void Party_SetFlag32AndRefreshMembers(void)
{
    GameFlag_Set(0x20);
    Owner_RefreshDerivedData(0);
    Owner_RefreshDerivedData(1);
    Owner_RefreshDerivedData(5);
    Party_Check(0);
    Party_Check(1);
    Party_Check(5);
}

/* party/apply_state_preset.c */
struct OwnerWork {
    u8 pad0[20];
    s16 hp_rate;
    s16 pp_rate;
    u8 pad1[28];
    s16 max_hp;
    s16 max_pp;
    s16 hp;
    s16 pp;
    u8 pad2[0xd8 - 60];
    u16 inventory[15];
};

void Owner_RecalculateStats(s32 id);
extern s32 FixedPoint_Ratio(s32, s32);
s32 OwnerAction_Add(s32 id, s32 value);

void Party_ApplyStatePreset(void)
{
    s32 id;

    GameFlag_Clear(32);
    GameFlag_Clear(33);
    GameFlag_Set(0x901);
    Owner_RefreshDerivedData(5);
    Owner_RecalculateStats(5);
    GameFlag_Clear(0x11b);
    GameFlag_Set(282);

    for (id = 0; id <= 1; id++) {
        struct OwnerWork *unit = (struct OwnerWork *)OwnerState_Get(id);
        s32 ratio;
        s32 rate;
        s32 slot;

        /* Keep both halfwords live through both stores for GCC's copy shape. */
        do {
            *(u16 *)((u8 *)unit + 0x38) = *(u16 *)((u8 *)unit + 0x34);
            *(u16 *)((u8 *)unit + 0x3a) = *(u16 *)((u8 *)unit + 0x36);
        } while (0);
        {
            s16 max_hp = *(s16 *)((u8 *)unit + 0x34);

            ratio = FixedPoint_Ratio(max_hp << 14, max_hp);
        }
        rate = 0x4000;
        if (ratio <= rate) {
            rate = 0;
            if (ratio >= 0) {
                rate = ratio;
            }
        }
        unit->hp_rate = rate;
        if ((rate << 16) == 0 && unit->hp != 0) {
            unit->hp_rate = 1;
        }

        ratio = FixedPoint_Ratio(unit->pp << 14, unit->max_pp);
        rate = 0x4000;
        if (ratio <= rate) {
            rate = 0;
            if (ratio >= 0) {
                rate = ratio;
            }
        }
        unit->pp_rate = rate;
        if ((rate << 16) == 0 && unit->pp != 0) {
            unit->pp_rate = 1;
        }

        for (slot = 0; slot <= 14; slot++) {
            if ((unit->inventory[slot] & 0x1ff) == 15) {
                unit->inventory[slot] = 16;
                Inventory_Equip(id, slot);
                break;
            }
        }

        Owner_RefreshDerivedData(id);
        Owner_RecalculateStats(id);
    }

    OwnerAction_Add(0, 140);
    OwnerAction_Add(0, 149);
    OwnerAction_Add(1, 140);
    OwnerAction_Add(2, 141);

    PARTY_STATE.money += 300;
}

/* owner/refresh_active_ratios.c */
/* owner/refresh_active_ratios.c */

void Owner_RefreshActiveRatios(s32 arg0)
{
    s32 count;
    s32 n;
    u8 *obj;
    s32 t;
    s32 v14;
    s32 v16;
    s32 one;
    s16 v34;

    count = Party_CountActiveOwners();
    for (n = 0; n < count; n++) {
        obj = OwnerState_Get(gCell.active_owners[n]);

        do {
            *(u16 *)(obj + 0x38) = *(u16 *)(obj + 0x34);
            *(u16 *)(obj + 0x3A) = *(u16 *)(obj + 0x36);
        } while (0);

        v34 = *(s16 *)(obj + 0x34);
        t = FixedPoint_Ratio(v34 << 14, v34);
        v14 = 0x4000;
        if (t <= 0x4000) {
            v14 = 0;
            if (t >= 0) {
                v14 = t;
            }
        }
        *(s16 *)(obj + 0x14) = (s16)v14;
        if (((v14 << 16) == 0) && (*(s16 *)(obj + 0x38) != 0)) {
            one = 1;
            *(s16 *)(obj + 0x14) = (s16)one;
        }

        t = FixedPoint_Ratio(*(s16 *)(obj + 0x3A) << 14, *(s16 *)(obj + 0x36));
        v16 = 0x4000;
        if (t <= 0x4000) {
            v16 = 0;
            if (t >= 0) {
                v16 = t;
            }
        }
        *(s16 *)(obj + 0x16) = (s16)v16;
        if (((v16 << 16) == 0) && (*(s16 *)(obj + 0x3A) != 0)) {
            one = 1;
            *(s16 *)(obj + 0x16) = (s16)one;
        }

        if (arg0 == 1) {
            *(s8 *)(obj + 0x131) = 0;
            *(s8 *)(obj + 0x140) = 0;
        }
    }
}

/* owner/refresh_ratios_on_flag.c */
extern const u8 gRomRefreshActiveRatios[];

void Owner_RefreshRatiosOnFlag(void)
{
    s32 count;
    s32 n;
    u8 ownerId;
    s32 group;
    s32 doRefresh;
    u8 *obj;
    s32 t;
    s32 v14;
    s32 v16;
    s32 v34;
    s32 v36;
    s32 v38;
    s32 v3A;
    s32 one;

    count = Party_CountActiveOwners();
    for (n = 0; n < count; n++) {
        ownerId = gCell.active_owners[n];
        group = gRomRefreshActiveRatios[ownerId];
        doRefresh = 0;
        if (group == 0) {
            if (GameFlag_Test(0x110) || GameFlag_Test(0x112)) {
                doRefresh = 1;
            }
        } else {
            if (GameFlag_Test(0x111) || GameFlag_Test(0x113)) {
                doRefresh = 1;
            }
        }

        if (doRefresh == 0) {
            continue;
        }

        obj = OwnerState_Get(ownerId);
        do {
            *(u16 *)(obj + 0x3A) = *(u16 *)(obj + 0x36);
        } while (0);

        v38 = *(s16 *)(obj + 0x38);
        v34 = *(s16 *)(obj + 0x34);
        t = FixedPoint_Ratio(v38 << 14, v34);
        v14 = 0x4000;
        if (t <= 0x4000) {
            v14 = 0;
            if (t >= 0) {
                v14 = t;
            }
        }
        *(s16 *)(obj + 0x14) = (s16)v14;
        if (((v14 << 16) == 0) && (*(s16 *)(obj + 0x38) != 0)) {
            one = 1;
            *(s16 *)(obj + 0x14) = (s16)one;
        }

        v3A = *(s16 *)(obj + 0x3A);
        v36 = *(s16 *)(obj + 0x36);
        t = FixedPoint_Ratio(v3A << 14, v36);
        v16 = 0x4000;
        if (t <= 0x4000) {
            v16 = 0;
            if (t >= 0) {
                v16 = t;
            }
        }
        *(s16 *)(obj + 0x16) = (s16)v16;
        if (((v16 << 16) == 0) && (*(s16 *)(obj + 0x3A) != 0)) {
            one = 1;
            *(s16 *)(obj + 0x16) = (s16)one;
        }
    }
}

/* owner/reserved_no_op_a.c */
void Owner_ReservedNoOp78228(void)
{
}

/* owner/recalculate_ratios.c */

struct OwnerRatioState {
    u8 unknown_00[0x14];
    s16 value_14;
    s16 value_16;
    u8 unknown_18[0x1c];
    s16 divisor_34;
    s16 divisor_36;
    s16 value_38;
    s16 value_3a;
};

void Owner_RecalculateRatios(s32 owner_no)
{
    s32 first;
    s32 second;
    s32 first_value;
    s32 second_value;
    struct OwnerRatioState *owner;

    owner = Owner_GetState(owner_no);
    first = FixedPoint_Ratio(
        (s32)((u32)(s32)owner->value_38 << 14), owner->divisor_34);
    first_value = 0x4000;
    if (first <= 0x4000) {
        first_value = 0;
        if (first >= 0) {
            first_value = first;
        }
    }
    owner->value_14 = first_value;
    if ((((u32)first_value << 16) == 0) && (owner->value_38 != 0)) {
        first_value = 1;
        owner->value_14 = first_value;
    }
    second = FixedPoint_Ratio(
        (s32)((u32)(s32)owner->value_3a << 14), owner->divisor_36);
    second_value = 0x4000;
    if (second <= 0x4000) {
        second_value = 0;
        if (second >= 0) {
            second_value = second;
        }
    }
    owner->value_16 = second_value;
    if ((((u32)second_value << 16) == 0) && (owner->value_3a != 0)) {
        second_value = 1;
        owner->value_16 = second_value;
    }
}

/* owner/update_ratio_pair.c */
struct OwnerRatioPairState {
    u8 padding0[20];
    s16 outputX;
    s16 outputY;
    u8 padding1[28];
    s16 limitX;
    s16 limitY;
    s16 inputX;
    s16 inputY;
};


void Owner_UpdateRatioPair(struct OwnerRatioPairState *state, s32 input)
{
    s32 value;

    if (input > state->limitX) {
        value = state->limitX;
    } else {
        value = 0;
        if (input >= 0) {
            value = input;
        }
    }
    state->inputX = value;
    value = FixedPoint_Ratio((value << 16) >> 2, state->limitX);

    {
        s32 output = 0x4000;

        if (value <= output) {
            output = 0;
            if (value >= 0) {
                output = value;
            }
        }
        state->outputX = output;
        if ((output << 16) == 0 && state->inputX != 0) {
            state->outputX = 1;
        }
    }

    value = FixedPoint_Ratio(state->inputY << 14, state->limitY);
    {
        s32 output = 0x4000;

        if (value <= output) {
            output = 0;
            if (value >= 0) {
                output = value;
            }
        }
        state->outputY = output;
        if ((output << 16) == 0 && state->inputY != 0) {
            state->outputY = 1;
        }
    }
}

/* owner/adjust_first_value.c */
struct OwnerFirstValueState { u8 padding[0x34]; s16 max; s16 unk; s16 pos; };
struct OwnerSecondValueState *Owner_GetState(s32);
void Owner_RecalculateRatios(s32 arg0);
s16 Owner_AdjustFirstValue(s32 o, s32 d)
{
    struct OwnerFirstValueState *s = Owner_GetState(o);
    s32 cur = s->pos;
    s32 mx = s->max;
    s32 p = cur + d;
    s32 r;
    if (p > mx) r = mx;
    else { r = 0; if (p >= 0) r = p; }
    s->pos = (s16)r;
    Owner_RecalculateRatios(o);
    return s->pos;
}

/* owner/adjust_second_value.c */
struct OwnerSecondValueState { u8 padding[0x36]; s16 max; s16 pad2; s16 pos; };
s16 Owner_AdjustSecondValue(s32 o, s32 d){ struct OwnerSecondValueState *s=Owner_GetState(o);
 s32 cur=s->pos; s32 mx=s->max; s32 p=cur+d;
 s32 r; if (p>mx) r=mx; else { r=0; if (p>=0) r=p; }
 s->pos=r; Owner_RecalculateRatios(o); return s->pos; }

/* item/get_direct.c */
/* item/get_definition.c */
extern struct ItemDefinition gRomGetDirect[];

struct ItemDefinition *Item_GetDirect(s32 item_id)
{
    return gRomGetDirect + (item_id & 0x1ff);
}

/* item/can_owner_equip.c */
s32 Item_CanOwnerEquipDirect(s32 owner_id, s32 item_id)
{
    struct OwnerInventoryState *owner = OwnerState_Get(owner_id);
    struct ItemDefinition *item = Item_GetDirect(item_id);
    u32 class_id = owner->class_id;
    s32 result = item->equip_mask;

    if (class_id > 7) {
        result = 0;
    } else {
        result >>= owner->class_id;
        result &= 1;
    }
    return result;
}

/* item/is_compatible_with_owner.c */
s32 Item_IsCompatibleWithOwner(s32 owner_id, s32 item_id)
{
    if (Item_GetEquipmentGroup(item_id) == 0) {
        return 1;
    }
    return Item_CanOwnerEquipDirect(owner_id, item_id);
}

/* item/get_equipment_group.c */
s32 Item_GetEquipmentGroup(s32 item_id)
{
    s32 group;
    struct ItemDefinition *item;

    item = Item_GetDirect(item_id);
    group = 0;
    if (item->type == 1) {
        group = 1;
        goto end;
    }
    if (item->type == 2) {
        goto two;
    }
    if (item->type == 3) {
        goto two;
    }
    if (item->type == 4) {
        goto two;
    }
    if (item->type == 5) {
        goto two;
    }
    if (item->type == 9) {
        goto two;
    }
    goto end;
two:
    group = 2;
end:
    return group;
}

/* inventory/get_quantity.c */
/* inventory/get_quantity.c */
s32 Inventory_GetQuantity(s32 owner, s32 slot)
{
    s32 item_id;

    owner = ((struct OwnerInventoryState *)OwnerState_Get(owner))->inventory[slot];
    item_id = 0x1ff;
    item_id &= owner;
    owner = (u32)owner >> 11;
    owner++;
    if (item_id == 0) {
        owner = 0;
    }
    return owner;
}

/* inventory/count.c */
s32 Inventory_Count(s32 owner)
{
    struct OwnerInventoryState *inv = OwnerState_Get(owner);
    s32 count = 0;

    if (inv->inventory[count] != 0) {
        do {
            count++;
            if (count > 14)
                break;
        } while (inv->inventory[count] != 0);
    }
    return count;
}

/* inventory/party_has_space.c */
s32 PartyInventory_HasSpace(void)
{
    s16 owners[10];
    s32 owner_count;
    s32 owner_index;
    s16 *owner_cursor;

    if (Inventory_Count(gCell.current_owner) != 15)
        return 1;
    owner_count = Party_ListActiveOwners(owners);
    owner_cursor = owners;
    owner_index = 0;
    if (owner_index < owner_count) {
        do {
            if (Inventory_Count(*owner_cursor++) != 15)
                return 1;
            owner_index++;
        } while (owner_index < owner_count);
    }
    return 0;
}

/* inventory/count_party_free_slots.c */
s32 PartyInventory_CountFreeSlots(void)
{
    s16 owners[10];
    s32 owner_count = Party_ListActiveOwners(owners);
    s32 cnt = 0;
    s16 *owner_cursor = owners;

    if (cnt < owner_count) {
        s32 n = owner_count;

        do {
            cnt = cnt - Inventory_Count(*owner_cursor++) + 15;
            n--;
        } while (n != 0);
    }
    return cnt;
}

/* inventory/add.c */
/* 所持品追加。積み重ね可能な品は同一番号の枠を探して個数を増やし、
   そうでなければ空き枠へ入れる。戻り値は枠番号、失敗は -1。 */
s32 Inventory_Add(s32 owner_id, s32 item_id)
{
    struct OwnerInventoryState *inv = OwnerState_Get(owner_id);
    struct ItemDefinition *item = Item_GetDirect(item_id);
    s32 slot;

    if ((item->flags & 0x10) != 0) {
        slot = 0;
        if (((inv->inventory[slot] ^ item_id) & 0x1ff) != 0) {
            do {
                slot++;
                if (slot > 14)
                    break;
            } while (((inv->inventory[slot] ^ item_id) & 0x1ff) != 0);
        }
        if (slot != 15) {
            s32 entry = inv->inventory[slot];
            u32 count = ((u32)entry >> 11) + 1;

            if (count > 29)
                return -1;
            {
                s32 value = 0x7ff;

                value &= entry;
                value |= count << 11;
                inv->inventory[slot] = value;
            }
            return slot;
        }
    }

    slot = 0;
    do {
        if (inv->inventory[slot] == 0) {
            inv->inventory[slot] = item_id;
            return slot;
        }
        slot++;
    } while (slot <= 14);
    return -1;
}

/* inventory/add_to_party.c */
s32 PartyInventory_Add(s32 item_id)
{
    s16 owners[10];
    s32 owner_count;
    s32 owner_index;
    s16 *owner_cursor;

    owner_count = Party_ListActiveOwners(owners);
    owner_cursor = owners;
    owner_index = 0;
    if (owner_index < owner_count) {
        do {
            s16 owner_id = *owner_cursor++;

            if (Inventory_Add(owner_id, item_id) >= 0)
                return owner_id;
            owner_index++;
        } while (owner_index < owner_count);
    }
    return -1;
}

/* inventory/find.c */
s32 Inventory_Find(s32 owner, s32 item_id)
{
    struct OwnerInventoryState *inv = OwnerState_Get(owner);
    s32 slot = 0;
    u16 *entry = inv->inventory;

    do {
        if (((*entry++) & 0x1ff) == item_id) {
            return slot;
        }
        slot++;
    } while (slot <= 14);
    return -1;
}

/* inventory/find_party_owner.c */
s32 PartyInventory_FindOwner(s32 item_id)
{
    s16 owners[10];
    s32 owner_count;
    s32 owner_index;
    s16 *owner_cursor;
    s16 owner;

    if (Inventory_Find(gCell.current_owner, item_id) != -1)
        return gCell.current_owner;
    owner_count = Party_ListActiveOwners(owners);
    owner_cursor = owners;
    owner_index = 0;
    if (owner_index < owner_count) {
        do {
            owner = *owner_cursor++;
            if (Inventory_Find(owner, item_id) != -1)
                return owner;
            owner_index++;
        } while (owner_index < owner_count);
    }
    return -1;
}

/* inventory/equip.c */
s32 Inventory_Equip(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = OwnerState_Get(owner);
    unsigned int mask;
    unsigned int item_id = inv->inventory[slot];
    struct ItemDefinition *item;
    u8 type;
    s32 other;

    if (Item_CanOwnerEquipDirect(owner, item_id) == 0)
        return -1;
    mask = 0x200;
    if (item_id & mask)
        return 0;

    item = Item_GetDirect(item_id);
    type = item->type;
    if (type != 6) {
        for (other = 0, item_id = 0xd8;
             other <= 14;
             item_id += 2, other++) {
            unsigned int m = mask;
            unsigned int flags = *(u16 *)(item_id + (unsigned int)inv);

            flags &= m;
            if (flags == 0)
                continue;
            if (Item_GetDirect(
                    *(volatile u16 *)(item_id + (unsigned int)inv))->type
                == type)
                break;
        }

        if (other != 15) {
            item = Item_GetDirect(inv->inventory[other]);
            if (item->flags & 2)
                return -2;
            inv->inventory[other] &= 0xfdff;
        }
    }

    inv->inventory[slot] |= 0x200;
    FunctionHead_08078bf0(owner);
    Owner_RecalculateStats(owner);
    return 0;
}

/* inventory/find_equipped.c */
s32 Inventory_FindEquipped(s32 owner, s32 type)
{
    u8 *base = OwnerState_Get(owner);
    s32 index;
    s32 offset;
    struct ItemDefinition *item;

    for (index = 0, offset = 216; index <= 14; index++) {
        if (*(u16 *)((u8 *)offset + (s32)base) & 0x200) {
            item = Item_GetDirect(
                *(u16 *)((u8 *)offset + (s32)base));
            if (item->type == type) break;
        }
        offset += 2;
    }
    if (index == 15) index = -1;
    return index;
}

/* inventory/get_equipped_definition.c */
struct ItemDefinition *Inventory_GetEquippedDefinition(
    struct OwnerInventoryState *inv,
    s32 type)
{
    s32 slot;
    struct ItemDefinition *item;

    for (slot = 0; slot <= 14; slot++) {
        if (inv->inventory[slot] & 0x200) {
            item = Item_GetDirect(inv->inventory[slot]);
            if (item->type == type) {
                return item;
            }
        }
    }
    return 0;
}

/* inventory/get_equipped_item.c */
s32 Inventory_GetEquippedItem(struct OwnerInventoryState *inv, s32 type)
{
    s32 slot;

    for (slot = 0; slot <= 14; slot++) {
        if (inv->inventory[slot] & 0x200) {
            struct ItemDefinition *item =
                Item_GetDirect(inv->inventory[slot]);

            if (item->type == type) {
                return inv->inventory[slot] & 0x1ff;
            }
        }
    }
    return 0;
}

/* inventory/discard.c */
s32 Inventory_Discard(s32 owner, s32 slot)
{
    s32 item =
        ((struct OwnerInventoryState *)OwnerState_Get(owner))->inventory[slot];
    s32 removed_slot = Inventory_Remove(owner, slot);

    if (removed_slot != -1) {
        FunctionHead_0808a548(Item_AdjustCounter(item, 1));
    }
    return removed_slot;
}

/* inventory/check_discard.c */
s32 Inventory_CheckDiscard(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = OwnerState_Get(owner);
    s32 item_id = inv->inventory[slot] & 0x1ff;
    struct ItemDefinition *item = Item_GetDirect(item_id);

    if (item_id == 0) {
        return -1;
    }
    if ((item->flags & 8) != 0) {
        return -4;
    }
    if ((inv->inventory[slot] & 0x200) != 0 &&
        (item->flags & 2) != 0) {
        return -3;
    }
    return 0;
}

/* inventory/remove_from_party.c */
s32 PartyInventory_Remove(s32 item_id)
{
    s32 owner = PartyInventory_FindOwner();

    if (owner == -1)
        return 0;
    Inventory_Remove(owner, Inventory_Find(owner, item_id));
    return 0;
}

/* inventory/discard_from_party.c */
s32 PartyInventory_Discard(s32 item_id)
{
    s32 owner = PartyInventory_FindOwner();

    if (owner == -1)
        return 0;
    Inventory_Discard(owner, Inventory_Find(owner, item_id));
    return 0;
}

/* inventory/break.c */
s32 Inventory_Break(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = OwnerState_Get(owner);
    if (inv->inventory[slot] == 0) {
        return -1;
    }
    inv->inventory[slot] |= 0x400;
    return 0;
}

/* inventory/repair.c */
s32 Inventory_Repair(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = OwnerState_Get(owner);
    if (inv->inventory[slot] == 0) {
        return -1;
    }
    inv->inventory[slot] &= ~0x400;
    return 0;
}

/* item/get_target_mode.c */
/* item/get_target_mode.c */
u8 Item_GetTargetMode(s32 item_id)
{
    return BattleAction_GetDirect(
        Item_GetDirect(item_id)->action_id)->target_mode;
}

/* item/adjust_counter_slot.c */
extern u8 gOv[128];

s32 ItemCounter_Adjust(s32 index, s32 delta)
{
    s32 counter_slot = index;
    u8 *data = gOv;

    index = 0;
    if (counter_slot <= 127) {
        s32 value = data[counter_slot];

        value += delta;

        if (value < 0) {
            value = 0;
        } else if (value > 99) {
            value = 99;
            index = 99;
        } else {
            index = value;
        }
        data[counter_slot] = value;
    }
    return index;
}

/* item/adjust_counter.c */
extern u8 gRomGetTargetMode[];

s32 Item_AdjustCounter(s32 item_id, s32 delta)
{
    s32 item_id_mask = 0x1ff;
    u8 counter;
    s32 result = 0;

    counter = gRomGetTargetMode[item_id & item_id_mask];
    if (counter != 0) {
        result = ItemCounter_Adjust(counter - 1, delta);
    }
    return result;
}

/* inventory/count_item.c */
/* inventory/count_item.c */
s32 Inventory_CountItem(s32 owner, s32 item_id)
{
    u8 *base = OwnerState_Get(owner);
    s32 count = 0;
    s32 target = item_id & 0x1ff;
    s32 index = 0;
    s32 offset = 216;

    do {
        if ((*(u16 *)((u8 *)offset + (s32)base) & 0x1FF) == target) {
            struct ItemDefinition *item = Item_GetDirect(target);

            if (item->flags & 0x10) {
                count = (*(u16 *)((u8 *)offset + (s32)base) >> 11) + 1;
                break;
            }
            count++;
        }
        index++;
        offset += 2;
    } while (index <= 14);
    return count;
}

/* inventory/count_party_item.c */
s32 PartyInventory_CountItem(s32 item_id)
{
    u16 owners[16];
    s32 item_count = 0;
    s32 owner_count = Party_ListActiveOwners(owners);

    if (item_count < owner_count) {
        u16 *owner_cursor = owners;
        s32 n = owner_count;

        do {
            item_count += Inventory_CountItem(*owner_cursor++, item_id);
            n--;
        } while (n != 0);
    }
    return item_count;
}
