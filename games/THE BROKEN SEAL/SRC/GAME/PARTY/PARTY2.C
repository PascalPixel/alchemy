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
s32 OwnerAction_Add(s32 id, s32 value);

/* owner/Owner_RefreshActiveRatios.c */
s32 Party_CountActiveOwners();

extern const u8 Character_ElementGroupTable[];
void *Owner_GetState(s32);

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
        struct BattleUnit *unit = (struct BattleUnit *)Owner_GetState(id);
        s32 ratio;
        s32 rate;
        s32 slot;

        /* Keep both halfwords live through both stores for GCC's copy shape. */
        /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
        do {
            *(u16 *)&unit->hp = *(u16 *)&unit->max_hp;
            *(u16 *)&unit->pp = *(u16 *)&unit->max_pp;
        } while (0);
        {
            s16 max_hp = unit->max_hp;

            ratio = (max_hp << 14) / max_hp;
        }
        rate = 0x4000;
        if (ratio <= rate) {
            rate = 0;
            if (ratio >= 0) {
                rate = ratio;
            }
        }
        unit->hp_gauge = rate;
        if ((rate << 16) == 0 && unit->hp != 0) {
            unit->hp_gauge = 1;
        }

        ratio = (unit->pp << 14) / unit->max_pp;
        rate = 0x4000;
        if (ratio <= rate) {
            rate = 0;
            if (ratio >= 0) {
                rate = ratio;
            }
        }
        unit->pp_gauge = rate;
        if ((rate << 16) == 0 && unit->pp != 0) {
            unit->pp_gauge = 1;
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
    struct BattleUnit *obj;
    s32 t;
    s32 v14;
    s32 v16;
    s32 one;
    s16 v34;

    count = Party_CountActiveOwners();
    for (n = 0; n < count; n++) {
        obj = Owner_GetState(gGameState.active_owners[n]);

        /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
        do {
            *(u16 *)&obj->hp = *(u16 *)&obj->max_hp;
            *(u16 *)&obj->pp = *(u16 *)&obj->max_pp;
        } while (0);

        v34 = obj->max_hp;
        t = (v34 << 14) / v34;
        v14 = 0x4000;
        if (t <= 0x4000) {
            v14 = 0;
            if (t >= 0) {
                v14 = t;
            }
        }
        obj->hp_gauge = (s16)v14;
        if (((v14 << 16) == 0) && (obj->hp != 0)) {
            one = 1;
            obj->hp_gauge = (s16)one;
        }

        t = (obj->pp << 14) / (obj->max_pp);
        v16 = 0x4000;
        if (t <= 0x4000) {
            v16 = 0;
            if (t >= 0) {
                v16 = t;
            }
        }
        obj->pp_gauge = (s16)v16;
        if (((v16 << 16) == 0) && (obj->pp != 0)) {
            one = 1;
            obj->pp_gauge = (s16)one;
        }

        if (arg0 == 1) {
            obj->poison = 0;
            obj->evil_spirit = 0;
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
    struct BattleUnit *obj;
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
        /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
        do {
            *(u16 *)&obj->pp = *(u16 *)&obj->max_pp;
        } while (0);

        v38 = obj->hp;
        v34 = obj->max_hp;
        t = (v38 << 14) / v34;
        v14 = 0x4000;
        if (t <= 0x4000) {
            v14 = 0;
            if (t >= 0) {
                v14 = t;
            }
        }
        obj->hp_gauge = (s16)v14;
        if (((v14 << 16) == 0) && (obj->hp != 0)) {
            one = 1;
            obj->hp_gauge = (s16)one;
        }

        v3A = obj->pp;
        v36 = obj->max_pp;
        t = (v3A << 14) / v36;
        v16 = 0x4000;
        if (t <= 0x4000) {
            v16 = 0;
            if (t >= 0) {
                v16 = t;
            }
        }
        obj->pp_gauge = (s16)v16;
        if (((v16 << 16) == 0) && (obj->pp != 0)) {
            one = 1;
            obj->pp_gauge = (s16)one;
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
    struct BattleUnit *owner;

    owner = Owner_GetState(owner_no);
    first = (s32)((u32)(s32)owner->hp << 14) / owner->max_hp;
    first_value = 0x4000;
    if (first <= 0x4000) {
        first_value = 0;
        if (first >= 0) {
            first_value = first;
        }
    }
    owner->hp_gauge = first_value;
    if ((((u32)first_value << 16) == 0) && (owner->hp != 0)) {
        first_value = 1;
        owner->hp_gauge = first_value;
    }
    second = (s32)((u32)(s32)owner->pp << 14) / owner->max_pp;
    second_value = 0x4000;
    if (second <= 0x4000) {
        second_value = 0;
        if (second >= 0) {
            second_value = second;
        }
    }
    owner->pp_gauge = second_value;
    if ((((u32)second_value << 16) == 0) && (owner->pp != 0)) {
        second_value = 1;
        owner->pp_gauge = second_value;
    }
}

void Owner_UpdateRatioPair(struct BattleUnit *state, s32 input)
{
    s32 value;

    if (input > state->max_hp) {
        value = state->max_hp;
    } else {
        value = 0;
        if (input >= 0) {
            value = input;
        }
    }
    state->hp = value;
    value = ((value << 16) >> 2) / state->max_hp;

    {
        s32 output = 0x4000;

        if (value <= output) {
            output = 0;
            if (value >= 0) {
                output = value;
            }
        }
        state->hp_gauge = output;
        if ((output << 16) == 0 && state->hp != 0) {
            state->hp_gauge = 1;
        }
    }

    value = (state->pp << 14) / state->max_pp;
    {
        s32 output = 0x4000;

        if (value <= output) {
            output = 0;
            if (value >= 0) {
                output = value;
            }
        }
        state->pp_gauge = output;
        if ((output << 16) == 0 && state->pp != 0) {
            state->pp_gauge = 1;
        }
    }
}
