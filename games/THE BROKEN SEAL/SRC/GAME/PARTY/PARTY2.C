#include "SCENE.H"
#include "GAME_FLAGS.H"
#include "INVENTORY.H"
#include "PARTY_STATE.H"
#include "TYPES.H"
#include "OWNER_STATE.H"
#include "FIXED_MATH.H"

/* party/set_flag32_and_refresh_members.c */
void Owner_RefreshDerivedData(s32 arg0);
s32 Owner_RecalculateStats(s32);
s32 GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);

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

s32 OwnerAction_Add(s32 id, s32 value);

/* owner/Owner_RefreshActiveRatios.c */
s32 Party_CountActiveOwners();

extern const u8 Character_ElementGroupTable[];
void *Owner_GetState(s32);

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

void Party_SetFlag32AndRefreshMembers(void)
{
    GameFlag_SetBit(0x20);
    Owner_RefreshDerivedData(0);
    Owner_RefreshDerivedData(1);
    Owner_RefreshDerivedData(5);
    Owner_RecalculateStats(0);
    Owner_RecalculateStats(1);
    Owner_RecalculateStats(5);
}

void Party_ApplyStatePreset(void)
{
    s32 id;

    GameFlag_ClearBit(32);
    GameFlag_ClearBit(33);
    GameFlag_SetBit(0x901);
    Owner_RefreshDerivedData(5);
    Owner_RecalculateStats(5);
    GameFlag_ClearBit(0x11b);
    GameFlag_SetBit(282);

    for (id = 0; id <= 1; id++) {
        struct OwnerWork *unit = (struct OwnerWork *)Owner_GetState(id);
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

            ratio = (max_hp << 14) / max_hp;
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

        ratio = (unit->pp << 14) / unit->max_pp;
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

    gGameState.coins += 300;
}

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
        obj = Owner_GetState(gGameState.active_owners[n]);

        do {
            *(u16 *)(obj + 0x38) = *(u16 *)(obj + 0x34);
            *(u16 *)(obj + 0x3A) = *(u16 *)(obj + 0x36);
        } while (0);

        v34 = *(s16 *)(obj + 0x34);
        t = (v34 << 14) / v34;
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

        t = (*(s16 *)(obj + 0x3A) << 14) / (*(s16 *)(obj + 0x36));
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
        ownerId = gGameState.active_owners[n];
        group = Character_ElementGroupTable[ownerId];
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

        obj = Owner_GetState(ownerId);
        do {
            *(u16 *)(obj + 0x3A) = *(u16 *)(obj + 0x36);
        } while (0);

        v38 = *(s16 *)(obj + 0x38);
        v34 = *(s16 *)(obj + 0x34);
        t = (v38 << 14) / v34;
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
        t = (v3A << 14) / v36;
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

void Owner_RatioNoOp(void)
{
}

void Owner_RecalculateRatios(s32 owner_no)
{
    s32 first;
    s32 second;
    s32 first_value;
    s32 second_value;
    struct OwnerRatioState *owner;

    owner = Owner_GetState(owner_no);
    first = (s32)((u32)(s32)owner->value_38 << 14) / owner->divisor_34;
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
    second = (s32)((u32)(s32)owner->value_3a << 14) / owner->divisor_36;
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

void Owner_UpdateRatioPair(struct OwnerRatioState *state, s32 input)
{
    s32 value;

    if (input > state->divisor_34) {
        value = state->divisor_34;
    } else {
        value = 0;
        if (input >= 0) {
            value = input;
        }
    }
    state->value_38 = value;
    value = ((value << 16) >> 2) / state->divisor_34;

    {
        s32 output = 0x4000;

        if (value <= output) {
            output = 0;
            if (value >= 0) {
                output = value;
            }
        }
        state->value_14 = output;
        if ((output << 16) == 0 && state->value_38 != 0) {
            state->value_14 = 1;
        }
    }

    value = (state->value_3a << 14) / state->divisor_36;
    {
        s32 output = 0x4000;

        if (value <= output) {
            output = 0;
            if (value >= 0) {
                output = value;
            }
        }
        state->value_16 = output;
        if ((output << 16) == 0 && state->value_3a != 0) {
            state->value_16 = 1;
        }
    }
}
