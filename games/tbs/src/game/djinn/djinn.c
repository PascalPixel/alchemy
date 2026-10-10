#include "btlcalc.h"
#include "types.h"
#include "scene.h"
#include "item.h"
#include "inventory.h"
#include "battle_random.h"
#include "btltype.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "fixed_math.h"
#include "battle_effect_chance.h"
#include "runtime_interfaces.h"
#include "game_flags.h"
#include "owner_state.h"
#include "party_state.h"
#include "character.h"
#include "btlpty.h"
#include "game_state.h"

s32 Equipment_GetUnleashRateBonus(struct BtlUnit *owner)
{
    s32 sum;
    s32 i;
    s32 j;
    struct ItemDefinition *item;

    sum = 0;
    for (i = 0; i < 15; i++) {
        if (owner->inventory[i] & 0x200) {
            item = Item_GetDirect(owner->inventory[i]);
            for (j = 0; j < 4; j++) {
                if (item->effects[j].kind == 23)
                    sum += item->effects[j].amount;
            }
        }
    }
    if (sum < 0)
        sum = 0;
    return sum;
}

#define BattleRandomPercent ((s32 (*)(void))BattleRandomPercent)

s32 Djinn_AddToOwner(s32 owner, s32 index, s32 bit);
u32 *Trade_AddOffer(u32 owner, u32 index, u32 bit);
s32 Trade_RemoveOffer(s32 owner, s32 index, s32 bit);
u32 Djinn_IsActive(s32 owner, s32 index, s32 bit);
u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit);
s32 Djinn_Activate(s32 owner, s32 index, s32 bit);
const u16 *Djinn_GetDefinition(u32 group, u32 index);

s32 RollWeaponUnleash(struct BtlUnit *owner)
{
    struct ItemDefinition *item;
    s32 rate;

    if (owner->class_index == 0) {
        return 1;
    }
    item = Inventory_GetEquippedDefinition(owner, 1);
    if (item == NULL) {
        return 1;
    }
    if (item->description_message == 0) {
        return 1;
    }
    rate = ((Equipment_GetUnleashRateBonus(owner) +
         (item->secondary_flags * 5)) << 0x10) / 100;
    if (rate > (s32)(BtlRnd() & 0xFFFF)) {
        return item->description_message;
    }
    return 1;
}

s32 BattleFx_GetBaseSuccessRate(s32 effect_id)
{
    s32 battle_result;
    u32 entry_index;

    entry_index = effect_id - 8;
    switch (entry_index) {
    case 4:
    case 5:
        return 0x46;
    case 8:
    case 9:
        return 0x4B;
    case 14:
        return 0x1E;
    case 15:
        return 0x28;
    case 16:
        return 0x2D;
    case 10:
    case 11:
    case 17:
        return 0x37;
    case 18:
        return 0x19;
    case 19:
        return 0x14;
    case 12:
    case 23:
        return 0x41;
    case 13:
    case 26:
        return 0x23;
    case 27:
        return 0x32;
    case 48:
        battle_result = 0x3C;
        goto block_18;
    case 49:
        battle_result = 0x5A;
        goto block_18;
    case 0:
    case 1:
    case 20:
    case 24:
        return 0x3C;
    default:
        battle_result = 0x64;
        break;
    }
block_18:
    return 0 - battle_result;
}

s32 BattleTarget_IsWeakToEffect(const struct BtlUnit *target, s32 effect_id)
{
    const u8 *entries;
    s32 i;

    if (target->class_index == 0) {
        entries = Owner_GetRecord(target->class_id) + 0x48;
        for (i = 0; i < 3; i++) {
            if (*entries == effect_id)
                return 1;
            entries++;
        }
    } else {
        entries = Owner_GetRecordStride84(target->class_index)->unknown_50;
        for (i = 0; i < 3; i++) {
            if (*entries++ == effect_id)
                return 1;
        }
    }
    return 0;
}

s32 BtlEfxRevChk(s32 effect_id)
{
    if ((effect_id == 5) || (effect_id == 0x38) || (effect_id == 0x39)) {
        return 1;
    }
    return 0;
}

s32 BattleFx_RollSuccess(
    s32 caster,
    s32 target,
    s32 resistance_category,
    s32 effect_id,
    s32 success_scale) {
    /* FAKEMATCH: the byte-status view and pointer setup prevent adjacent reads from merging into wider loads. */
    u8 *state = (u8 *)UnitGet(target);
    struct BtlUnit *unit = (struct BtlUnit *)state;
    s32 attempts = 1;
    s32 score;
    s32 attempt;
    s8 *poison;
    u8 *delusion;
    u8 *confusion;
    u8 *charm;
    u8 *stun;
    u8 *sleep;

    if (BtlEfxRevChk(effect_id)!= 0 &&
        unit->hp != 0) {
        return 0;
    }

    if (effect_id == 3 && *(s8 *)(state + (u32)&((struct BtlUnit *)0)->doku) == 0) {
        goto fail;
    }

    goto action4_check;
action4_tail:
    if (state[(u32)&((struct BtlUnit *)0)->shibire] == 0 && state[(u32)&((struct BtlUnit *)0)->nemuri] == 0 &&
        state[(u32)&((struct BtlUnit *)0)->fuuji] == 0 && state[(u32)&((struct BtlUnit *)0)->shiryo] == 0) {
        goto fail;
    }
    goto action4_done;
action4_check:
    if (effect_id == 4) {
        if (state[(u32)&((struct BtlUnit *)0)->maboroshi] == 0 && state[(u32)&((struct BtlUnit *)0)->konran] == 0 && state[(u32)&((struct BtlUnit *)0)->charm] == 0) {
            goto action4_tail;
        }
    }

action4_done:
    poison = (s8 *)(state + (u32)&((struct BtlUnit *)0)->doku);
    delusion = state + (u32)&((struct BtlUnit *)0)->maboroshi;
    confusion = state + (u32)&((struct BtlUnit *)0)->konran;
    charm = state + (u32)&((struct BtlUnit *)0)->charm;
    stun = state + (u32)&((struct BtlUnit *)0)->shibire;
    sleep = state + (u32)&((struct BtlUnit *)0)->nemuri;
    if (effect_id == 0x40 &&
        *poison == 0 &&
        *delusion == 0 &&
        *confusion == 0 &&
        *charm == 0 &&
        *stun == 0 &&
        *sleep == 0 &&
        state[(u32)&((struct BtlUnit *)0)->fuuji] == 0 &&
        state[(u32)&((struct BtlUnit *)0)->shiryo] == 0 &&
        state[(u32)&((struct BtlUnit *)0)->akuryo] == 0) {
        return 0;
    }

    if (effect_id == 0x1C && state[(u32)&((struct BtlUnit *)0)->shiryo] == 1) {
        return 0;
    }

    score = BattleFx_GetBaseSuccessRate(effect_id);
    if (score > 0) {
        s32 difference = Owner_GetResistanceValue(caster, resistance_category) -
            Owner_GetResistanceValue(target, resistance_category) -
            (unit->luck >> 1);
        score += difference * 3;
        if (BattleTarget_IsWeakToEffect(unit, effect_id) != 0) {
            score += 25;
        }
    } else {
        score = -score;
    }

    if (effect_id == 0x43) {
        attempts = 3;
    }

    for (attempt = 0; attempt < attempts; attempt++) {
        if (score *success_scale / 100 >= BattleRandomPercent()) {
            return 1;
        }
    }
fail:
    return 0;
}

const u16 *Djinn_GetDefinition(u32 group, u32 index)
{
    s32 entry;

    entry = 0;
    if ((group <= 3U) && (index <= 0x13U)) {
        entry = (group * 0x14) + index;
    }
    return (const u16 *)((u8 *)Djinn_DefinitionTable + entry * 0xC);
}

s32 Djinn_AddToLeastLoadedOwner(s32 index, u8 *state)
{
	u32 GameFlag_SetBit(u32 flag);
    void *entry = state + index * 20 + 48;
    s32 best_no = 0;
    s32 best_val = 999;
    s32 count;
    s32 i;
    s32 j;

    if (GameFlag_Test(entry) != 0)
        return -1;

    count = Party_CountActiveOwners();
    for (i = 0; i < count; i++) {
        struct BtlUnit *unit = UnitGet(gGameState.active_owners[i]);

        if (unit->djinn_owned_counts[index] <= 9) {
            s32 value = 0;

            for (j = 0; j < 4; j++)
                value += unit->djinn_owned_counts[j];
            if (best_val > value) {
                best_val = value;
                best_no = gGameState.active_owners[i];
            }
        }
    }

    if (best_val == 999)
        return -2;

    Djinn_AddToOwner(best_no, index, (s32)state);
    Trade_AddOffer(best_no, index, (u32)state);
    GameFlag_SetBit((u32)entry);
    return best_no;
}

s32 Djinn_AddToOwner(s32 owner, s32 index, s32 bit)
{
    struct BtlUnit *state = UnitGet(owner);

    if (state->djinn_owned_counts[index] > 9)
        return -1;
    if ((state->djinn_available[index] & (1 << bit)) != 0)
        return -1;
    state->djinn_owned_counts[index]++;
    state->djinn_available[index] |= 1 << bit;
    return 0;
}

s32 Trade_CanOfferDjinn(s32 owner, s32 index, s32 bit)
{
    struct BtlUnit *state =
        (struct BtlUnit *)UnitGet(owner);
    struct DjinnRecoveryList *table;
    s32 i;
    s32 status;

    if (state->djinn_owned_counts[index] == 0)
        return 0;
    if (state->djinn_active_counts[index] > 9) {
        state->djinn_active_counts[index] = 10;
        return 0;
    }
    if ((state->djinn_available[index] & (1 << bit)) == 0)
        return 0;
    if ((state->djinn_active[index] & (1 << bit)) != 0)
        return 0;

    table = &((struct DjinnRecoveryTable *)Trade_GetOfferState((owner < 0 || owner > 7)))->list;
    for (i = 0; i < table->count; i++) {
        if (index == table->entries[i].element && bit == table->entries[i].index)
            break;
    }
    if (i == table->count ||
        ((status = table->entries[i].turns) <= 0 && status != -2))
        return 1;
    return 0;
}

u32 Djinn_IsActive(s32 owner, s32 index, s32 bit)
{
    s32 value =
        ((struct BtlUnit *)UnitGet(owner))->djinn_active[index] &
        (1 << bit);

    return (u32)(-value | value) >> 31;
}

s32 Djinn_Activate(s32 owner, s32 index, s32 bit)
{
    struct BtlUnit *state =
        (struct BtlUnit *)UnitGet(owner);
    s32 result = Trade_CanOfferDjinn(owner, index, bit);

    if (result != 0) {
        if (state->djinn_available[index] & (1 << bit)) {
            state->djinn_active[index] |= 1 << bit;
        } else {
            return 0;
        }
        state->djinn_active_counts[index]++;
        Owner_RefreshDerivedData(owner);
    }
    return result;
}

u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit)
{
    struct BtlUnit *state =
        (struct BtlUnit *)UnitGet(owner);
    u32 present = Djinn_IsActive(owner, index, bit);

    if (present != 0) {
        state->djinn_active_counts[index]--;
        state->djinn_active[index] &= ~(1 << bit);
        Owner_RefreshDerivedData(owner);
    }
    return present;
}

s32 Trade_RemoveOffer(s32 owner, s32 index, s32 bit)
{
    struct DjinnRecoveryList *table;
    s32 found = 0;
    s32 i;

    table = &((struct DjinnRecoveryTable *)Trade_GetOfferState((owner < 0 || owner > 7)))->list;
    for (i = 0; i < table->count; i++) {
        if (index == table->entries[i].element && bit == table->entries[i].index) {
            table->count--;
            found = 1;
            break;
        }
    }
    for (; i < table->count; i++) {
        table->entries[i] = table->entries[i + 1];
    }
    return found;
}

u32 *Trade_AddOffer(u32 kind, u32 first, u32 second)
{
    struct DjinnRecoveryList *table;
    s32 count;

    Trade_RemoveOffer(kind, first, second);
    table = &((struct DjinnRecoveryTable *)Trade_GetOfferState(kind > 7))->list;
    count = table->count;
    table->entries[count].element = first;
    table->entries[count].index = second;
    table->entries[count].unit_id = kind;
    table->entries[count].turns = -1;
    table->count = count + 1;
    return (u32 *)&table->count;
}

s32 Djinn_Transfer(s32 source, s32 index, s32 bit, s32 target)
{
    struct BtlUnit *state = UnitGet(source);
    u32 present;

    if ((state->djinn_available[index] & (1 << bit)) != 0) {
        present = Djinn_IsActive(source, index, bit);
        if (Djinn_AddToOwner(target, index, bit) == 0) {
            Djinn_Deactivate(source, index, bit);
            state->djinn_available[index] &= ~(1 << bit);
            state->djinn_owned_counts[index]--;

            if (present != 0) {
                Djinn_Activate(target, index, bit);
            } else {
                Trade_RemoveOffer(source, index, bit);
                Trade_AddOffer(target, index, bit);
            }
            return 0;
        }
    }
    return -1;
}

s32 Trade_CountPendingOffers(u8 *counts)
{
    s32 found = 0;
    struct DjinnRecoveryTable *base = Trade_GetOfferState(0);
    struct DjinnRecoveryEntry *entry = base->list.entries;
    s32 index;

    if (counts != 0) {
        u8 *slot;

        slot = counts + 3;
        *slot = found;
        slot = counts + 2;
        *slot = found;
        slot = counts + 1;
        *slot = found;
        counts[0] = found;
    }
    index = 0;
    if (base->list.count != 0) {
        do {
            if (entry->turns == -1) {
                if (counts != 0)
                    counts[entry->element]++;
                found++;
            }
            index++;
            entry++;
        } while (index != base->list.count);
    }
    return found;
}

u16 Djinn_GetDefinitionHeader(u32 group, u32 index)
{
    return *Djinn_GetDefinition(group, index);
}

s32 Party_SumDjinnCounts(s32 index)
{
    u16 owners[16];
    s32 result = 0;
    s32 count = Party_ListActiveOwners((s16 *)owners);

    if (result < count) {
        u16 *owner = owners;
        s32 remaining = count;

        do {
            struct BtlUnit *state = UnitGet(*owner++);

            if (index == -1) {
                result += state->djinn_owned_counts[0];
                result += state->djinn_owned_counts[1];
                result += state->djinn_owned_counts[2];
                result += state->djinn_owned_counts[3];
            } else {
                result += state->djinn_owned_counts[index];
            }
            remaining--;
        } while (remaining != 0);
    }
    return result;
}

void Inventory_AddAndEquip(s32 owner, s32 target)
{
    struct BtlUnit *state = UnitGet(owner);
    u16 *entry;
    s32 index;

    Inventory_AddItem(owner, target);
    index = 0;
    entry = state->inventory;
    do {
        if (*entry++ == target)
            Inventory_Equip(owner, index);
        index++;
    } while (index < INVENTORY_SLOTS);
}

#define INVENTORY_SNAPSHOT_SENTINEL 0x6774
void Owner_RefreshActiveRatios(s32 mode);

void InventorySnapshot_Save(void)
{
	void GameFlag_SetBit(s32 flag);
    u16 *save;
    struct BtlUnit *unit;
    s16 *inventory;
    s32 owner;
    s32 i;
    s32 n;
    s16 first;
    s16 second;

    save = gInventorySnapshot;
    if (*save != INVENTORY_SNAPSHOT_SENTINEL) {
        *save++ = INVENTORY_SNAPSHOT_SENTINEL;
        first = gGameState.first_shortcut;
        second = gGameState.second_shortcut;
        for (owner = 0; owner < 4; owner++) {
            unit = (struct BtlUnit *)UnitGet(owner);
            for (n = 0; n < 15; n++)
                *save++ = unit->inventory[n];
            for (n = 0; n < 15; n++) {
                if (Item_GetDirect(unit->inventory[n])->type != 6)
                    unit->inventory[n] = 0;
            }
            n = 0;
            inventory = (s16 *)unit->inventory;
            for (i = 0; i < 15; i++) {
                if (inventory[i] != 0)
                    inventory[n++] = inventory[i];
            }
            for (; n < 15; n++)
                inventory[n] = 0;
            Owner_RefreshDerivedData(owner);
            UnitCalc(owner);
        }
        *save++ = first;
        *save++ = second;
        inventory = (s16 *)gGameState.active_owners;
        save[0] = inventory[0];
        save[1] = inventory[1];
        Inventory_AddAndEquip(0, 16);
        GameFlag_SetBit(0x952);
    }
    Owner_RefreshActiveRatios(1);
}

void InventorySnapshot_Restore(void)
{
    s32 owner;
    u16 *source = gInventorySnapshot;

    if (*source++ == INVENTORY_SNAPSHOT_SENTINEL) {
        owner = 0;
        do {
            u16 *inventory =
                ((struct BtlUnit *)UnitGet(owner))->inventory;
            s32 remaining = 14;

            do {
                *inventory++ = *source++;
                remaining--;
            } while (remaining >= 0);
            Owner_RefreshDerivedData(owner);
            UnitCalc(owner);
            owner++;
        } while (owner <= 3);

        gGameState.first_shortcut = *source++;
        gGameState.second_shortcut = *source++;
        *(u16 *)((u8 *)gGameState.active_owners) = *source++;
        *(u16 *)((u8 *)gGameState.active_owners + 2) = *source;
        source = gInventorySnapshot;
        *source = 0;
        GameFlag_ClearBit(0x952);
    }
}
