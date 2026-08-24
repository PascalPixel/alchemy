#include "types.h"

struct MenuModeLabelState {
    u8 pad_000[124];
    void *window;
    u8 pad_080[12];
    s16 mode;
    u8 pad_08e[8];
    s16 previous_mode;
};

extern struct State_08028aa8 *Data_03001f38;

extern void Func_080164d4(void *, s32, s32, s32, s32);
extern void UiText_DrawCharacter(const u8 *, void *, s32, s32);

void Menu_DrawModeLabel(void)
{
    struct MenuModeLabelState *state = Data_03001f38;

    if (state->previous_mode != state->mode) {
        state->previous_mode = state->mode;
        Func_080164d4(state->window, 8, 40, 144, 80);

        if (state->mode != 1) {
            if (state->mode > 1)
                goto mode_other;
            if (state->mode != 0)
                goto mode_other;

            {
                const u8 *text = (const u8 *)0xc7b;

                UiText_DrawCharacter(text, state->window, 18, 40);
                UiText_DrawCharacter(text + 1, state->window, 18, 48);
                UiText_DrawCharacter(text + 2, state->window, 18, 56);
                UiText_DrawCharacter(text + 3, state->window, 18, 64);
                text += 4;
                UiText_DrawCharacter(text, state->window, 18, 72);
                goto done;
            }
        }

        {
            const u8 *text = (const u8 *)0xc7b;

            UiText_DrawCharacter(text, state->window, 18, 40);
            UiText_DrawCharacter(text + 1, state->window, 18, 48);
            text += 2;
            UiText_DrawCharacter(text, state->window, 18, 56);
            goto done;
        }

mode_other:
        {
            const u8 *text = (const u8 *)0xc7b;

            UiText_DrawCharacter(text++, state->window, 18, 40);
            UiText_DrawCharacter(text, state->window, 18, 48);
        }
done:
        ;
    }
}
