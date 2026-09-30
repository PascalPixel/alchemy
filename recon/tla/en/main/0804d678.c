#include "TYPES.H"
extern u8 gTitleExtraOptionEnabled[];

struct MenuModeLabelState;
extern struct MenuModeLabelState *gMenuSelectWork;
extern u8 MsgPasswordLevel[];

s32 UiWindow_Create(s32, s32, s32, s32, s32);

void Menu_DrawModeLabel(void)
{
    struct MenuModeLabelState *state = gMenuSelectWork;

    if (state->previous_mode != state->mode) {
        state->previous_mode = state->mode;
        UiWindow_ClearInteriorTiles(state->window, 8, 40, 144, 80);

        if (state->mode != 1) {
            if (state->mode > 1)
                goto mode_other;
            if (state->mode != 0)
                goto mode_other;

            {
                s32 text = (s32)MsgPasswordLevel;

                UiText_DrawCharacterAtOffset(text, state->window, 18, 40);
                UiText_DrawCharacterAtOffset(text + 1, state->window, 18, 48);
                UiText_DrawCharacterAtOffset(text + 2, state->window, 18, 56);
                UiText_DrawCharacterAtOffset(text + 3, state->window, 18, 64);
                text += 4;
                UiText_DrawCharacterAtOffset(text, state->window, 18, 72);
                goto done;
            }
        }

        {
            s32 text = (s32)MsgPasswordLevel;

            UiText_DrawCharacterAtOffset(text, state->window, 18, 40);
            UiText_DrawCharacterAtOffset(text + 1, state->window, 18, 48);
            text += 2;
            UiText_DrawCharacterAtOffset(text, state->window, 18, 56);
            goto done;
        }

mode_other:
        {
            s32 text = (s32)MsgPasswordLevel;

            UiText_DrawCharacterAtOffset(text++, state->window, 18, 40);
            UiText_DrawCharacterAtOffset(text, state->window, 18, 48);
        }
done:
;
    }
}
