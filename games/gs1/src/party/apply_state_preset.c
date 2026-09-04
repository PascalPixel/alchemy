#include "game_flags.h"
#include "inventory.h"
#include "party_state.h"

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
#define Owner_RecalculateStats Func_08077428

void Owner_RefreshDerivedData(s32 id);
void Owner_RecalculateStats(s32 id);
s32 FixedPoint_Ratio(s32, s32);
s32 OwnerAction_Add(s32 id, s32 value);

#define Party_ApplyStatePreset Func_08077f70

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
