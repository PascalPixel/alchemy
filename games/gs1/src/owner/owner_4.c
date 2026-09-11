#include "scene.h"
#include "owner_state.h"
#include "party_state.h"
#include "game_flags.h"
#include "types.h"

/* owner/refresh_active_ratios.c */
extern s32 FixedPoint_Ratio(s32, s32);

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
        obj = OwnerState_Get(gCell.active_owners[n]);

        do {
            *(u16 *)(obj + 0x38) = *(u16 *)(obj + 0x34);
            *(u16 *)(obj + 0x3A) = *(u16 *)(obj + 0x36);
        } while (0);

        v34 = *(s16 *)(obj + 0x34);
        t = FixedPoint_Ratio(v34 << 14, v34);
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

        t = FixedPoint_Ratio(*(s16 *)(obj + 0x3A) << 14, *(s16 *)(obj + 0x36));
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

/* owner/refresh_ratios_on_flag.c */
extern s32 FixedPoint_Ratio(s32, s32);
extern const u8 gRom[];

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
        ownerId = gCell.active_owners[n];
        group = gRom[ownerId];
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

        obj = OwnerState_Get(ownerId);
        do {
            *(u16 *)(obj + 0x3A) = *(u16 *)(obj + 0x36);
        } while (0);

        v38 = *(s16 *)(obj + 0x38);
        v34 = *(s16 *)(obj + 0x34);
        t = FixedPoint_Ratio(v38 << 14, v34);
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
        t = FixedPoint_Ratio(v3A << 14, v36);
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

/* owner/reserved_no_op_a.c */
void Owner_ReservedNoOp78228(void)
{
}

/* owner/recalculate_ratios.c */
s32 FixedPoint_Ratio(s32, s32);

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
    first = FixedPoint_Ratio(
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
    second = FixedPoint_Ratio(
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

/* owner/update_ratio_pair.c */
struct OwnerRatioPairState {
    u8 padding0[20];
    s16 outputX;
    s16 outputY;
    u8 padding1[28];
    s16 limitX;
    s16 limitY;
    s16 inputX;
    s16 inputY;
};

s32 FixedPoint_Ratio(s32, s32);

void Owner_UpdateRatioPair(struct OwnerRatioPairState *state, s32 input)
{
    s32 value;

    if (input > state->limitX) {
        value = state->limitX;
    } else {
        value = 0;
        if (input >= 0) {
            value = input;
        }
    }
    state->inputX = value;
    value = FixedPoint_Ratio((value << 16) >> 2, state->limitX);

    {
        s32 output = 0x4000;

        if (value <= output) {
            output = 0;
            if (value >= 0) {
                output = value;
            }
        }
        state->outputX = output;
        if ((output << 16) == 0 && state->inputX != 0) {
            state->outputX = 1;
        }
    }

    value = FixedPoint_Ratio(state->inputY << 14, state->limitY);
    {
        s32 output = 0x4000;

        if (value <= output) {
            output = 0;
            if (value >= 0) {
                output = value;
            }
        }
        state->outputY = output;
        if ((output << 16) == 0 && state->inputY != 0) {
            state->outputY = 1;
        }
    }
}

/* owner/adjust_first_value.c */
struct OwnerFirstValueState { u8 padding[0x34]; s16 max; s16 unk; s16 pos; };
struct OwnerFirstValueState *Owner_GetState(s32);
void Owner_RecalculateRatios(s32 arg0);
s16 Owner_AdjustFirstValue(s32 o, s32 d)
{
    struct OwnerFirstValueState *s = Owner_GetState(o);
    s32 cur = s->pos;
    s32 mx = s->max;
    s32 p = cur + d;
    s32 r;
    if (p > mx) r = mx;
    else { r = 0; if (p >= 0) r = p; }
    s->pos = (s16)r;
    Owner_RecalculateRatios(o);
    return s->pos;
}

/* owner/adjust_second_value.c */
struct OwnerSecondValueState { u8 padding[0x36]; s16 max; s16 pad2; s16 pos; };
struct OwnerSecondValueState *Owner_GetState(s32);
void Owner_RecalculateRatios(s32 arg0);
s16 Owner_AdjustSecondValue(s32 o, s32 d){ struct OwnerSecondValueState *s=Owner_GetState(o);
 s32 cur=s->pos; s32 mx=s->max; s32 p=cur+d;
 s32 r; if (p>mx) r=mx; else { r=0; if (p>=0) r=p; }
 s->pos=r; Owner_RecalculateRatios(o); return s->pos; }
