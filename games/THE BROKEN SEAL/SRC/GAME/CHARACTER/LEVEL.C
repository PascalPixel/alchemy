#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "OWNER_STATE.H"
#include "CHARACTER.H"
#include "MEM.H"
#include "TYPES.H"
#include "INVENTORY.H"
#include "BATTLE_UNIT.H"
#include "ITEM.H"
#include "text/MSG_IDS.H"

enum { OWNER_ACTION_CLASS = 0x8000, OWNER_ACTION_ITEM = 0x4000 };

s32 Owner_RefreshClassActions(s32 owner)
{
    struct BtlUnit *st;
    u16 *abil;
    struct ClassDefinition *cls;
    struct ItemDefinition *def;
    s32 i;
    s32 j;
    u16 mask;
    struct ClassAbility *cg;

    st = UnitGet(owner);
    abil = (u16 *)st->action_slots;
    cls = Owner_GetRecordStride84(st->class_index);
    if (st->class_index == 0)
        return 0;
    mask = 0x8000;
    for (j = 0; j < 32; j++) {
        if (abil[j * 2] & mask)
            abil[j * 2] = 0;
    }
    mask = 0x4000;
    for (j = 0; j < 32; j++) {
        if (abil[j * 2] & mask)
            abil[j * 2] = 0;
    }
    for (i = 31, j = 31; i >= 0;) {
        if (abil[i * 2] == 0) {
            i--;
        } else {
            abil[j * 2] = abil[i * 2];
            i--;
            j--;
        }
    }
    for (; j >= 0; j--)
        abil[j * 2] = 0;

    mask = OWNER_ACTION_CLASS;
    for (j = 0; j < 16; j++) {
        cg = &cls->abilities[j];
        if (cls->abilities[j].ability == 0)
            continue;
        if (st->level < cls->abilities[j].level)
            continue;
        i = 0;
        while (i < 32 && abil[i * 2] != cls->abilities[j].ability)
            i++;
        if (i != 32)
            continue;
        for (i = 0; i < 32; i++) {
            if (abil[i * 2] == 0) {
                abil[i * 2] = cg->ability | mask;
                break;
            }
        }
        if (i == 32)
            break;
    }

    for (j = 0; j < 15; j++) {
        if (st->inventory[j] == 0)
            continue;
        if (!(st->inventory[j] & INVENTORY_EQUIPPED))
            continue;
        def = Item_GetDirect(st->inventory[j]);
        if (def->use_type != 3)
            continue;
        for (i = 0; i < 32; i++) {
            if ((abil[i * 2] & OWNER_ACTION_ID_MASK) == def->action_id)
                break;
        }
        if (i != 32)
            continue;
        for (i = 0; i < 32; i++) {
            if (abil[i * 2] == 0) {
                abil[i * 2] = def->action_id | OWNER_ACTION_ITEM;
                break;
            }
        }
        if (i == 32)
            break;
    }

    for (i = 0, j = 0; i < 32;) {
        if (abil[i * 2] == 0) {
            i++;
        } else {
            abil[j * 2] = abil[i * 2];
            i++;
            j++;
        }
    }
    for (; j < 32; j++)
        abil[j * 2] = 0;
    return 0;
}



struct LevelUpWork {
    s32 class_index;
    s32 level;
    struct CharacterDefinition *growth;
    u8 unknown_0c[0x20];
};

u32 Random16(void);

s32 OwnerAction_Add(s32 state_index, s32 value)
{
    struct BtlUnit *state = (struct BtlUnit *)UnitGet(state_index);
    s32 key = value & OWNER_ACTION_ID_MASK;
    s32 found = -1;
    s32 index;

    for (index = 0; index <= 30; index++) {
        s32 masked = state->action_slots[index].encoded_action & OWNER_ACTION_ID_MASK;

        if ((masked ^ key) == 0) {
            state->action_slots[index].encoded_action = masked;
            found = index;
            break;
        }
    }

    if (found < 0) {
        /* FAKEMATCH: the scalar member-address walk preserves halfword operand order; typed indexing shortens the loop by four bytes. */
        for (index = 0; index <= 30; index++) {
            s32 offset = index * sizeof(state->action_slots[0]) +
                (s32)&((struct BtlUnit *)0)->action_slots;

            if (*(u16 *)((u8 *)state + offset) == 0) {
                *(u16 *)((u8 *)state + offset) = key;
                found = index;
                break;
            }
        }
        if (found < 0) {
            return -1;
        }
    }

    Owner_RefreshClassActions(state_index);
    for (index = 0; index <= 31; index++) {
        if (state->action_slots[index].encoded_action == key) {
            break;
        }
    }
    return index;
}

struct LevelUpResult *OwnerAction_CheckLevelThreshold(s32 owner, struct LevelUpResult *value)
{
    return Owner_GetValueIfLevelThresholdReached(owner, value);
}

struct CharacterDefinition *Character_GetDefinition(s32 index)
{
    struct CharacterDefinition *base;

    base = Character_DefinitionTable;
    return &base[index];
}

void Owner_InitRecords(void)
{
    struct BtlUnit *state;
    struct CharacterDefinition *tmpl;
    u16 name_buf[16];
    s32 owner;
    s32 *remote = Character_StartingEquipOwnerIds;
    s32 i;
    s32 slot;
    u8 *name;

    for (owner = 0; owner <= 7; owner++) {
        state = (struct BtlUnit *)UnitGet(owner);
        Ui_AdjustValueWithoutLimitFar(owner + (s32)MsgCharacterName, name_buf);
        name = state->name;
        name[0] = name_buf[0];
        i = 0;
        if (name_buf[0] != 0) {
            do {
                i++;
                if (i > 13)
                    break;
                name[i] = name_buf[i];
            } while (name_buf[i] != 0);
        }
        state->name[14] = 0;
    }

    if (*remote != -1) {
        do {
            state = (struct BtlUnit *)UnitGet(*remote);
            if (state != 0) {
                state->class_id = *remote;
                tmpl = Character_GetDefinition(state->class_id);

                for (i = 14; i >= 0; i--)
                    state->inventory[i] = 0;

                for (i = 0; i < sizeof(tmpl->items) / sizeof(tmpl->items[0]); i++) {
                    slot = Inventory_AddItem(*remote, tmpl->items[i] & 0x1ff);
                    Inventory_Equip(*remote, slot);
                }

                Owner_RefreshDerivedData(*remote);
                state->ep_gauge = 0x4000;
                state->hp_gauge = 0x4000;
                Party_AdvanceOwnerCountToTarget(*remote, tmpl->starting_level);
                UnitCalc(*remote);
            }
            remote++;
        } while (*remote != -1);
    }
}

void Owner_LevelNoOp(void)
{
}

u32 Owner_GetLevelThreshold(s32 owner, s32 level)
{
    struct BtlUnit *state = (struct BtlUnit *)UnitGet(owner);

    if (state->class_index != 0) {
        if (level <= 0) {
            return 0;
        }
        if (level <= 99 && state->class_id <= 7) {
            return Character_LevelExpTable[state->class_id * 99 + level - 1];
        }
    }
    return -1;
}

struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *res)
{
    struct BtlUnit *st;
    struct LevelUpWork *work;
    u32 threshold;
    s16 band;
    s32 diff;
    s32 level;

    st = (struct BtlUnit *)UnitGet(owner);
    work = (struct LevelUpWork *)Runtime_BumpAllocateAlternatePool(sizeof(struct LevelUpWork));
    work->class_index = st->class_index;
    level = st->level;
    work->level = level;
    res->level = level;
    res->ability = 0xffff;
    res->hp = 0;
    res->ep = 0;
    res->atk = 0;
    res->def = 0;
    res->agi = 0;
    res->luck = 0;
    if (level < 99) {
        st->level++;
        res->level = level + 1;
        threshold = Owner_GetLevelThreshold(owner, st->level);
        if (threshold != -1 && st->experience < threshold)
            st->experience = threshold;
        work->growth = Character_GetDefinition(owner);
        if (res->level == 1) {
            res->hp += work->growth->hp[0];
            res->ep += work->growth->ep[0];
            res->atk += work->growth->atk[0];
            res->def += work->growth->def[0];
            res->agi += work->growth->agi[0];
            res->luck += work->growth->luck[0];
        }
        band = res->level / 20;
        if (band < 0)
            band = 0;
        if (band > 4)
            band = 4;
        diff = work->growth->hp[band + 1] - work->growth->hp[band];
        res->hp += ((Random16() * 20 >> 16) + diff) / 20;
        diff = work->growth->ep[band + 1] - work->growth->ep[band];
        res->ep += ((Random16() * 20 >> 16) + diff) / 20;
        diff = work->growth->atk[band + 1] - work->growth->atk[band];
        res->atk += ((Random16() * 20 >> 16) + diff) / 20;
        diff = work->growth->def[band + 1] - work->growth->def[band];
        res->def += ((Random16() * 20 >> 16) + diff) / 20;
        diff = work->growth->agi[band + 1] - work->growth->agi[band];
        res->agi += ((Random16() * 20 >> 16) + diff) / 20;
        diff = work->growth->luck[band + 1] - work->growth->luck[band];
        res->luck += ((Random16() * 20 >> 16) + diff) / 20;
        st->base_hp += res->hp;
        st->base_ep += res->ep;
        st->base_attack += res->atk;
        st->base_defense += res->def;
        st->base_agility += res->agi;
        st->base_luck += res->luck;
        st->base_action_count = 1;
        st->base_hp_regen = 0;
        st->base_ep_regen = 0;
        Owner_RefreshClassActions(owner);
        UnitCalc(owner);
    }
    MemFree(work);
    return res;
}

struct LevelUpResult *Owner_GetValueIfLevelThresholdReached(s32 owner_no, struct LevelUpResult *value)
{
    struct BtlUnit *owner;

    owner = (struct BtlUnit *)UnitGet(owner_no);
    if ((owner->experience >= Owner_GetLevelThreshold(owner_no, owner->level + 1)) &&
        (Owner_LevelUp(owner_no, value) != NULL)) {
        return value;
    }
    return 0;
}
