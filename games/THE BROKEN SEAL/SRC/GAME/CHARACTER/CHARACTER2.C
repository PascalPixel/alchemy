#include "OWNER_STATE.H"
#include "TYPES.H"
#include "INVENTORY.H"

s32 Inventory_AddItem(s32 owner, s32 item);
void *Owner_GetState(s32);
s32 Owner_RefreshClassActions(s32 owner);
s32 Owner_GetValueIfLevelThresholdReached(s32, s32);

typedef struct {
    u8 bytes[0xB4];
} Data_080844ec_Record;

extern Data_080844ec_Record Character_DefinitionTable[];

/*
 * Field names/offsets for name/name_flags/hp_ratio/pp_ratio come from the
 * sibling draft recon/tbs/en/main/08079460.c (BattleUnit_Assign),
 * which zero-inits the same OWNER_STATE_SIZE (332-byte) record through the
 * same Owner_GetState allocator and sets the same two fields to the same
 * 0x4000 constant at these exact offsets. inventory[15]/class_id reuse the
 * already-guarded offsets from owner_state.h.
 */
struct OwnerRecordState {
    u8 name[14]; /* 0x00 */
    u8 name_flags; /* 0x0e */
    u8 unknown_00f[0x14 - 0x0f];
    s16 hp_ratio; /* 0x14 */
    s16 pp_ratio; /* 0x16 */
    u8 unknown_018[0xd8 - 0x18];
    u16 inventory[15]; /* 0xd8, guarded OwnerInventoryState_Inventory */
    u8 unknown_0f6[0x128 - 0xf6];
    u8 class_id; /* 0x128, guarded OwnerInventoryState_ClassId */
};

/* Per-class starting-equipment template returned by Owner_GetRecordStride180. */
struct OwnerEquipTemplate {
    u8 unknown_000[0x96];
    u8 unknown_096; /* 0x96 (150) */
    u16 items[13]; /* 0x98 (152) */
};

extern s32 Character_StartingEquipOwnerIds[];
extern u8 MsgCharacterName;
void Ui_AdjustValueWithoutLimitFar(s32, u16 *);
void Party_AdvanceOwnerCountToTarget(s32, u8);
void Owner_RecalculateStats(s32);
void Owner_RefreshDerivedData(s32);

struct OwnerLevelState {
    u8 unknown_000[0x0f];
    u8 level;                   /* 0x0f */
    u16 base_hp;                /* 0x10 */
    u16 base_pp;                /* 0x12 */
    u8 unknown_014[4];
    u16 base_attack;            /* 0x18 */
    u16 base_defense;           /* 0x1a */
    u16 base_agility;           /* 0x1c */
    u8 base_luck;               /* 0x1e */
    u8 base_turns;              /* 0x1f */
    u8 base_20;                 /* 0x20 */
    u8 base_21;                 /* 0x21 */
    u8 unknown_022[0x102];
    u32 experience;             /* 0x124 */
    u8 character;               /* 0x128 */
    u8 class_id;                /* 0x129 */
};

extern u32 Character_LevelExpTable[];

/* Growth record: each statistic at levels 0, 20, 40, 60, 80 and 100. */
struct OwnerGrowth {
    u8 unknown_00[0x50];
    s16 hp[6];                  /* 0x50 */
    s16 pp[6];                  /* 0x5c */
    u16 attack[6];              /* 0x68 */
    u16 defense[6];             /* 0x74 */
    u16 agility[6];             /* 0x80 */
    u8 luck[6];                 /* 0x8c */
};

/* What a level gained, for the level-up message. */
struct LevelUpResult {
    s16 level;
    u16 ability;
    u16 hp;
    u16 pp;
    u16 attack;
    u16 defense;
    u16 agility;
    u16 luck;
};

struct LevelUpWork {
    s32 class_id;
    s32 level;
    struct OwnerGrowth *growth;
    u8 unused_0c[0x20];
};

void *Runtime_BumpAllocateAlternatePool(s32 size);
void Runtime_BumpFree(void *buffer);
u32 Owner_GetLevelThreshold(s32 owner, s32 level);
void Owner_RecalculateStats(s32 owner);
u32 Random16(void);

struct Owner_080792c4 {
    u8 unknown_000[0x0f];
    u8 level;
    u8 unknown_010[0x114];
    u32 value_124;
};

struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *res);

Data_080844ec_Record *Owner_GetRecordStride180(s32 index);

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

s32 OwnerAction_CheckLevelThreshold(s32 owner, s32 value)
{
    return Owner_GetValueIfLevelThresholdReached(owner, value);
}

Data_080844ec_Record *Owner_GetRecordStride180(s32 index)
{
    Data_080844ec_Record *base;

    base = Character_DefinitionTable;
    return &base[index];
}

void Owner_InitRecords(void)
{
    struct OwnerRecordState *state;
    struct OwnerEquipTemplate *tmpl;
    u16 name_buf[16];
    s32 owner;
    s32 *remote = Character_StartingEquipOwnerIds;
    s32 i;
    s32 slot;
    u8 *name;

    for (owner = 0; owner <= 7; owner++) {
        state = (struct OwnerRecordState *)Owner_GetState(owner);
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
        state->name_flags = 0;
    }

    if (*remote != -1) {
        do {
            state = (struct OwnerRecordState *)Owner_GetState(*remote);
            if (state != 0) {
                state->class_id = (u8)*remote;
                tmpl = (struct OwnerEquipTemplate *)Owner_GetRecordStride180(state->class_id);

                for (i = 14; i >= 0; i--)
                    state->inventory[i] = 0;

                for (i = 0; i < sizeof(tmpl->items) / sizeof(tmpl->items[0]); i++) {
                    slot = Inventory_AddItem(*remote, tmpl->items[i] & 0x1ff);
                    Inventory_Equip(*remote, slot);
                }

                Owner_RefreshDerivedData(*remote);
                state->pp_ratio = 0x4000;
                state->hp_ratio = 0x4000;
                Party_AdvanceOwnerCountToTarget(*remote, tmpl->unknown_096);
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
    struct OwnerLevelState *state = (struct OwnerLevelState *)Owner_GetState(owner);

    if (state->class_id != 0) {
        if (level <= 0) {
            return 0;
        }
        if (level <= 99 && state->character <= 7) {
            return Character_LevelExpTable[state->character * 99 + level - 1];
        }
    }
    return (u32)-1;
}

/* Raises a party member one level: the gain of each statistic is the
   growth between the two surrounding twenty-level marks, spread over twenty
   levels with a random remainder. Level 99 is the cap. */
struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *res)
{
    struct OwnerLevelState *st;
    struct LevelUpWork *work;
    u32 threshold;
    s16 band;
    s32 diff;
    s32 level;

    st = (struct OwnerLevelState *)Owner_GetState(owner);
    work = Runtime_BumpAllocateAlternatePool(sizeof(struct LevelUpWork));
    work->class_id = st->class_id;
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
        st->base_turns = 1;
        st->base_20 = 0;
        st->base_21 = 0;
        Owner_RefreshClassActions(owner);
        Owner_RecalculateStats(owner);
    }
    Runtime_BumpFree(work);
    return res;
}

s32 Owner_GetValueIfLevelThresholdReached(s32 owner_no, s32 value)
{
    struct Owner_080792c4 *owner;

    owner = (struct Owner_080792c4 *)Owner_GetState(owner_no);
    if ((owner->value_124 >= Owner_GetLevelThreshold(owner_no, owner->level + 1)) &&
        ((s32)Owner_LevelUp(owner_no, value) != 0)) {
        return value;
    }
    return 0;
}
