#include "OWNER_STATE.H"
#include "PARTY_STATE.H"
#include "GAME_FLAGS.H"
#include "FIXED_MATH.H"
#include "TYPES.H"

extern const u8 Character_ElementGroupTable[];

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
        t = Math_Div(v38 << 14, v34);
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
        t = Math_Div(v3A << 14, v36);
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

void *Owner_GetState(s32);

void Owner_RatioNoOp(void)
{
}

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

void Owner_RecalculateRatios(s32 owner_no)
{
    s32 first;
    s32 second;
    s32 first_value;
    s32 second_value;
    struct OwnerRatioState *owner;

    owner = Owner_GetState(owner_no);
    first = Math_Div(
        (s32)((u32)(s32)owner->value_38 << 14), owner->divisor_34);
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
    second = Math_Div(
        (s32)((u32)(s32)owner->value_3a << 14), owner->divisor_36);
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
    value = Math_Div((value << 16) >> 2, state->divisor_34);

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

    value = Math_Div(state->value_3a << 14, state->divisor_36);
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
