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

extern s32 Data_0807b690[];
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
    s32 *remote = Data_0807b690;
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
                    slot = Inventory_Add(*remote, tmpl->items[i] & 0x1ff);
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
