#include "types.h"

struct MenuState {
    u8 padding_000[20];
    s32 slots[4];
    u8 padding_024[414 - 36];
    s16 mode;
};

s32 Func_080770c0(s32);
void Func_080770c8(void *);
s32 Func_08077288(s32, void *);
void Func_080916b0(void);
void Func_0808c44c(void);
void Func_0808b8e8(void);
void Func_08096140(s32);
void Func_080965a8(s32);
void Func_08095dd0(s32);
void Func_08095a44(s32);
void Func_0808b98c(void);
void Func_08015358(s32, s32, void *);
void Func_0808c4c0(void);
void Func_08091750(void);

void Func_080955b0(s32 slot, s32 page, void *entries)
{
    struct MenuState *state;
    s32 selection;

    state = *(struct MenuState **)0x03001ebc;
    if (Func_080770c0(366) != 0) {
        selection = 0;
        Func_080770c8((u8 *)entries + page * 20 + 48);
    } else {
        selection = Func_08077288(page, entries);
    }

    if (selection >= 0) {
        Func_080916b0();
        Func_0808c44c();
        if (slot != -1) {
            if (state->mode == 3)
                Func_0808b8e8();

            if (page == 0)
                Func_08096140(slot);
            else if (page == 1)
                Func_080965a8(slot);
            else if (page == 2)
                Func_08095dd0(slot);
            else if (page == 3)
                Func_08095a44(slot);

            state->slots[slot] = 0;
            if (state->mode == 3)
                Func_0808b98c();
        }

        Func_08015358(selection, page, entries);
        Func_0808c4c0();
        Func_08091750();
    }
}
