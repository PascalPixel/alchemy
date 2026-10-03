/* Trial 2026-10-03: removed the duplicate Owner_GetState declaration.
 * Still unscored: owner/template views are incomplete and setup symbols
 * lack declarations in this draft. The body is unchanged.
 */
#include "OWNER_STATE.H"
s32 Inventory_AddItem(s32 owner, s32 item);

void Owner_RefreshClassActions(s32);
s32 Owner_GetValueIfLevelThresholdReached(s32, s32);

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
