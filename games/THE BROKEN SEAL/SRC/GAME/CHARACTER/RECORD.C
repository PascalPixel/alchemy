#include "OWNER_STATE.H"

void *Owner_GetState(s32);
void Owner_RefreshClassActions(s32);
s32 Owner_GetValueIfLevelThresholdReached(s32, s32);

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

#include "TYPES.H"

typedef struct {
    u8 bytes[0xB4];
} Data_080844ec_Record;

extern Data_080844ec_Record Character_DefinitionTable[];

Data_080844ec_Record *Owner_GetRecordStride180(s32 index)
{
    Data_080844ec_Record *base;

    base = Character_DefinitionTable;
    return &base[index];
}

#include "TYPES.H"
#include "OWNER_STATE.H"
#include "INVENTORY.H"

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
extern u8 Value_00000066;

void Ui_AdjustValueWithoutLimitFar(s32, u16 *);
void Party_AdvanceOwnerCountToTarget(s32, u8);
void Owner_RecalculateStats(s32);
void Owner_RefreshDerivedData(s32);

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
        state = (struct OwnerRecordState *)OwnerState_Get(owner);
        Ui_AdjustValueWithoutLimitFar(owner + (s32)&Value_00000066, name_buf);
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
            state = (struct OwnerRecordState *)OwnerState_Get(*remote);
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

#include "TYPES.H"

struct OwnerLevelState {
    u8 unknown[0x128];
    u8 type;
    u8 enabled;
};

void *Owner_GetState(s32);
extern u32 Character_LevelExpTable[];

void Owner_LevelNoOp(void)
{
}

u32 Owner_GetLevelThreshold(s32 owner, s32 level)
{
    struct OwnerLevelState *state = (struct OwnerLevelState *)Owner_GetState(owner);

    if (state->enabled != 0) {
        if (level <= 0) {
            return 0;
        }
        if (level <= 99 && state->type <= 7) {
            return Character_LevelExpTable[state->type * 99 + level - 1];
        }
    }
    return (u32)-1;
}
