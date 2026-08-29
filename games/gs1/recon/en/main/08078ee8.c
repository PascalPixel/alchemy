#include "types.h"
#include "owner_state.h"
#include "inventory.h"

/*
 * Field names/offsets for name/name_flags/hp_ratio/pp_ratio come from the
 * sibling draft games/gs1/recon/en/main/08079460.c (BattleUnit_Assign),
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

/* Per-class starting-equipment template returned by Func_08078ed8. */
struct OwnerEquipTemplate {
    u8 unknown_000[0x96];
    u8 unknown_096; /* 0x96 (150) */
    u16 items[13]; /* 0x98 (152) */
};

extern s32 Data_0807b690[];

void Func_08015020(s32, u16 *);
void Func_080792fc(s32, u8);
void Func_08077428(s32);
void Owner_RefreshDerivedData(s32);

#define Owner_InitRecords Func_08078ee8
void Owner_InitRecords(void)
{
    struct OwnerRecordState *state;
    struct OwnerEquipTemplate *tmpl;
    volatile u16 name_buf[16];
    s32 owner;
    s32 *remote = Data_0807b690;
    s32 i;
    s32 slot;

    for (owner = 0; owner <= 7; owner++) {
        state = (struct OwnerRecordState *)OwnerState_Get(owner);
        Func_08015020(owner + 102, (u16 *)name_buf);
        state->name[0] = name_buf[0];
        if (name_buf[0] != 0) {
            i = 0;
            do {
                i++;
                if (i > 13)
                    break;
                state->name[i] = (u8)name_buf[i];
            } while (name_buf[i] != 0);
        }
        state->name_flags = 0;
    }

    if (*remote != -1) {
        do {
            state = (struct OwnerRecordState *)OwnerState_Get(*remote);
            if (state != 0) {
                state->class_id = (u8)*remote;
                tmpl = (struct OwnerEquipTemplate *)Func_08078ed8(state->class_id);

                for (i = 14; i != -1; i--)
                    state->inventory[i] = 0;

                for (i = 0; i != 13; i++) {
                    slot = Inventory_Add(*remote, tmpl->items[i] & 0x1ff);
                    Inventory_Equip(*remote, slot);
                }

                Owner_RefreshDerivedData(*remote);
                state->pp_ratio = 0x4000;
                state->hp_ratio = 0x4000;
                Func_080792fc(*remote, tmpl->unknown_096);
                Func_08077428(*remote);
            }
            remote++;
        } while (*remote != -1);
    }
}
