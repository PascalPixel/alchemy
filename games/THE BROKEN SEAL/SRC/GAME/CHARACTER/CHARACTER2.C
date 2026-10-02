#include "OWNER_STATE.H"
#include "CHARACTER.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "INVENTORY.H"

s32 Inventory_AddItem(s32 owner, s32 item);
void *Owner_GetState(s32);
s32 Owner_RefreshClassActions(s32 owner);

extern s32 Character_StartingEquipOwnerIds[];
extern u8 MsgCharacterName;
void Ui_AdjustValueWithoutLimitFar(s32, u16 *);
void Party_AdvanceOwnerCountToTarget(s32, u8);
void Owner_RecalculateStats(s32);
void Owner_RefreshDerivedData(s32);

extern u32 Character_LevelExpTable[];

struct LevelUpWork {
    s32 class_id;
    s32 level;
    struct CharacterDefinition *growth;
    u8 unused_0c[0x20];
};

void Runtime_BumpFree(void *buffer);
u32 Owner_GetLevelThreshold(s32 owner, s32 level);
void Owner_RecalculateStats(s32 owner);
u32 Random16(void);

s32 OwnerAction_Add(s32 state_index, s32 value)
{
    struct OwnerActionState *state = (struct OwnerActionState *)Owner_GetState(state_index);
    s32 key = value & 0x3fff;
    s32 found = -1;
    s32 index;

    for (index = 0; index <= 30; index++) {
        s32 masked = state->action_slots[index].encoded_action & 0x3fff;

        if ((masked ^ key) == 0) {
            state->action_slots[index].encoded_action = masked;
            found = index;
            break;
        }
    }

    if (found < 0) {
        for (index = 0; index <= 30; index++) {
            s32 offset = (index * 4) + 0x58;
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

struct CharacterDefinition *Owner_GetRecordStride180(s32 index)
{
    struct CharacterDefinition *base;

    base = Character_DefinitionTable;
    return &base[index];
}

void Owner_InitRecords(void)
{
    struct BattleUnit *state;
    struct CharacterDefinition *tmpl;
    u16 name_buf[16];
    s32 owner;
    s32 *remote = Character_StartingEquipOwnerIds;
    s32 i;
    s32 slot;
    u8 *name;

    for (owner = 0; owner <= 7; owner++) {
        state = (struct BattleUnit *)Owner_GetState(owner);
        Ui_AdjustValueWithoutLimitFar(owner + (s32)&MsgCharacterName, name_buf);
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
            state = (struct BattleUnit *)Owner_GetState(*remote);
            if (state != 0) {
                state->class_id = (u8)*remote;
                tmpl = (struct CharacterDefinition *)Owner_GetRecordStride180(state->class_id);

                for (i = 14; i >= 0; i--)
                    state->inventory[i] = 0;

                for (i = 0; i < sizeof(tmpl->items) / sizeof(tmpl->items[0]); i++) {
                    slot = Inventory_AddItem(*remote, tmpl->items[i] & 0x1ff);
                    Inventory_Equip(*remote, slot);
                }

                Owner_RefreshDerivedData(*remote);
                state->pp_gauge = 0x4000;
                state->hp_gauge = 0x4000;
                Party_AdvanceOwnerCountToTarget(*remote, tmpl->starting_level);
                Owner_RecalculateStats(*remote);
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
    struct BattleUnit *state = (struct BattleUnit *)Owner_GetState(owner);

    if (state->class_index != 0) {
        if (level <= 0) {
            return 0;
        }
        if (level <= 99 && state->class_id <= 7) {
            return Character_LevelExpTable[state->class_id * 99 + level - 1];
        }
    }
    return (u32)-1;
}

/* Raises a party member one level: the gain of each statistic is the
   growth between the two surrounding twenty-level marks, spread over twenty
   levels with a random remainder. Level 99 is the cap. */
struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *res)
{
    struct BattleUnit *st;
    struct LevelUpWork *work;
    u32 threshold;
    s16 band;
    s32 diff;
    s32 level;

    st = (struct BattleUnit *)Owner_GetState(owner);
    work = (struct LevelUpWork *)Runtime_BumpAllocateAlternatePool(sizeof(struct LevelUpWork));
    work->class_id = st->class_index;
    level = st->level;
    work->level = level;
    res->level = level;
    res->ability = 0xffff;
    res->hp = 0;
    res->pp = 0;
    res->attack = 0;
    res->defense = 0;
    res->agility = 0;
    res->luck = 0;
    if (level < 99) {
        st->level++;
        res->level = level + 1;
        threshold = Owner_GetLevelThreshold(owner, st->level);
        if (threshold != (u32)-1 && st->experience < threshold)
            st->experience = threshold;
        work->growth = Owner_GetRecordStride180(owner);
        if (res->level == 1) {
            res->hp += work->growth->hp[0];
            res->pp += work->growth->pp[0];
            res->attack += work->growth->attack[0];
            res->defense += work->growth->defense[0];
            res->agility += work->growth->agility[0];
            res->luck += work->growth->luck[0];
        }
        band = res->level / 20;
        if (band < 0)
            band = 0;
        if (band > 4)
            band = 4;
        diff = work->growth->hp[band + 1] - work->growth->hp[band];
        res->hp += ((Random16() * 20 >> 16) + diff) / 20;
        diff = work->growth->pp[band + 1] - work->growth->pp[band];
        res->pp += ((Random16() * 20 >> 16) + diff) / 20;
        diff = work->growth->attack[band + 1] - work->growth->attack[band];
        res->attack += ((Random16() * 20 >> 16) + diff) / 20;
        diff = work->growth->defense[band + 1] - work->growth->defense[band];
        res->defense += ((Random16() * 20 >> 16) + diff) / 20;
        diff = work->growth->agility[band + 1] - work->growth->agility[band];
        res->agility += ((Random16() * 20 >> 16) + diff) / 20;
        diff = work->growth->luck[band + 1] - work->growth->luck[band];
        res->luck += ((Random16() * 20 >> 16) + diff) / 20;
        st->base_hp += res->hp;
        st->base_pp += res->pp;
        st->base_attack += res->attack;
        st->base_defense += res->defense;
        st->base_agility += res->agility;
        st->base_luck += res->luck;
        st->base_action_count = 1;
        st->base_hp_regen = 0;
        st->base_pp_regen = 0;
        Owner_RefreshClassActions(owner);
        Owner_RecalculateStats(owner);
    }
    Runtime_BumpFree(work);
    return res;
}

struct LevelUpResult *Owner_GetValueIfLevelThresholdReached(s32 owner_no, struct LevelUpResult *value)
{
    struct BattleUnit *owner;

    owner = (struct BattleUnit *)Owner_GetState(owner_no);
    if ((owner->experience >= Owner_GetLevelThreshold(owner_no, owner->level + 1)) &&
        (Owner_LevelUp(owner_no, value) != NULL)) {
        return value;
    }
    return 0;
}
