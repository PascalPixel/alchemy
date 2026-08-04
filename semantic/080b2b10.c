#include "layout_guard.h"
#include "types.h"

enum { PURCHASE_CANDIDATE_CAPACITY_080B2B10 = 9 };

typedef struct DisplayObject_080b2b10 {
    u8 padding00[5];
    s8 mode;
} DisplayObject_080b2b10;

typedef struct PurchaseMenu_080b2b10 {
    u8 padding000[0x36e];
    s16 candidates[PURCHASE_CANDIDATE_CAPACITY_080B2B10];
    DisplayObject_080b2b10 *focus;
    u8 padding384[0x23];
    s8 candidate_count;
    u8 display_mode;
    u8 padding3a9;
    s8 category;
} PurchaseMenu_080b2b10;

typedef struct EconomyState_080b2b10 {
    u8 padding00[0x10];
    u32 currency;
} EconomyState_080b2b10;

LAYOUT_OFFSET_GUARD(
    PurchaseMenu080b2b10_Candidates,
    PurchaseMenu_080b2b10,
    candidates,
    0x36e);
LAYOUT_OFFSET_GUARD(
    PurchaseMenu080b2b10_Focus,
    PurchaseMenu_080b2b10,
    focus,
    0x380);
LAYOUT_OFFSET_GUARD(
    PurchaseMenu080b2b10_CandidateCount,
    PurchaseMenu_080b2b10,
    candidate_count,
    0x3a7);
LAYOUT_OFFSET_GUARD(
    PurchaseMenu080b2b10_Category,
    PurchaseMenu_080b2b10,
    category,
    0x3aa);
LAYOUT_OFFSET_GUARD(
    EconomyState080b2b10_Currency,
    EconomyState_080b2b10,
    currency,
    0x10);

extern PurchaseMenu_080b2b10 *Data_03001f2c;
extern volatile u32 Data_03001c94;
extern volatile u32 Data_03001b04;

s32 Func_080022fc(s32, s32);
void Func_080030f8(s32);
void *Func_08015010(s32, s32, s32, s32, s32);
void Func_08015018(void *, s32);
void Func_08015120(s32, s32);
void Func_08015140(void);
void Func_08077230(s32);
void Func_080a1028(void *, s32, s32, s32, s32);
void Func_080a1030(void);
s32 Func_080b0664(s32);
void Func_080b0a6c(void *, s32, s32);
void Func_080b10cc(void);
s16 Func_080b2778(s16, s32);
s32 Func_080b27b0(s16, s32);
s32 Func_080b280c(void);
void Func_080b28d4(s32);
void Func_080b2928(s32);
void Func_080b2da8(s16, s32);
void Func_080b2e30(void *, s32);
void Func_080b2ed8(void *, s16);
void Func_080b3050(s32);
void Func_080f9010(s32);

static s32 FirstValidCandidate_080b2b10(
    const PurchaseMenu_080b2b10 *state,
    s32 category,
    s16 *candidate)
{
    s32 selection = 0;

    while (selection < state->candidate_count) {
        *candidate = state->candidates[selection];
        if (Func_080b27b0(*candidate, category) != 0)
            break;
        selection++;
    }
    return selection;
}

/* Run the candidate purchase/application menu until cancel or completion. */
s32 Func_080b2b10(void)
{
    PurchaseMenu_080b2b10 *state = Data_03001f2c;
    EconomyState_080b2b10 *economy =
        (EconomyState_080b2b10 *)0x02000240;
    s32 count = state->candidate_count;
    s32 category = state->category;
    void *message_window;
    void *list_window;
    s32 selection;
    s32 redraw = 1;
    s32 rebuild = 0;
    s16 candidate = 0;

    Func_080b28d4(0x0d26);
    message_window = Func_08015010(1, 0x0c, 0x0d, 3, 2);
    state->focus->mode = 4;
    state->display_mode = 1;
    Func_080a1028(message_window, 2, 0, 8, 0);
    list_window = Func_08015010(1, 0x10, 0x17, 3, 2);
    selection = FirstValidCandidate_080b2b10(
        state, category, &candidate);

    for (;;) {
        if (rebuild != 0) {
            rebuild = 0;
            Func_080b28d4(0x0d26);
            selection = FirstValidCandidate_080b2b10(
                state, category, &candidate);
            redraw = 1;
        }

        if (redraw != 0) {
            redraw = 0;
            selection = Func_080022fc(selection + count, count);
            candidate = state->candidates[selection];
            Func_080b0a6c(
                message_window, selection * 0x18 - 0x0c, 0);
            state->display_mode = 3;
            Func_080b2e30(message_window, selection);
            Func_080b2ed8(list_window, candidate);
        }

        if ((Data_03001c94 & 1) != 0) {
            s16 price;

            Func_080030f8(1);
            price = Func_080b2778(candidate, category);
            if (Func_080b27b0(candidate, category) == 0) {
                Func_080f9010(0x71);
                continue;
            }

            Func_08015120(candidate, 1);
            Func_08015120(price, 5);
            Func_080b28d4(0x0d27);
            if (Func_080b0664(0) != 0) {
                Func_080b2928(0x0d29);
                rebuild = 1;
                continue;
            }
            if ((u32)(s32)price > economy->currency) {
                Func_080f9010(0x71);
                Func_080b2928(0x0d28);
                rebuild = 1;
                continue;
            }

            Func_08015120(candidate, 1);
            Func_080b28d4(0x0d2a);
            Func_08015140();
            Func_080b2da8(candidate, category);
            Func_080b3050(selection);
            Func_08077230(-price);
            Func_080b10cc();
            Func_08015120(candidate, 1);
            Func_080b28d4(0x0d2b);
            if (Func_080b280c() != 0) {
                rebuild = 1;
                continue;
            }
            break;
        }

        if ((Data_03001c94 & 2) != 0) {
            Func_080f9010(0x71);
            break;
        }
        if ((Data_03001b04 & 0x20) != 0) {
            Func_080f9010(0x6f);
            redraw = 1;
            selection--;
        }
        if ((Data_03001b04 & 0x10) != 0) {
            Func_080f9010(0x6f);
            redraw = 1;
            selection++;
        }
        Func_080030f8(1);
    }

    Func_080a1030();
    Func_08015018(list_window, 2);
    Func_08015018(message_window, 2);
    Func_080030f8(1);
    return 0;
}
