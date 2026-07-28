#include "types.h"

struct State_08028aa8 {
    u8 pad_000[124];
    void *window_07c;
    u8 pad_080[12];
    s16 mode_08c;
    u8 pad_08e[8];
    s16 previous_096;
};

extern struct State_08028aa8 *Data_03001f38;

extern void Func_080164d4(void *, s32, s32, s32, s32);
extern void Func_0801e7c0(const u8 *, void *, s32, s32);

void Func_08028aa8(void)
{
    struct State_08028aa8 *state = Data_03001f38;

    if (state->previous_096 != state->mode_08c) {
        state->previous_096 = state->mode_08c;
        Func_080164d4(state->window_07c, 8, 40, 144, 80);

        if (state->mode_08c != 1) {
            if (state->mode_08c > 1)
                goto mode_other;
            if (state->mode_08c != 0)
                goto mode_other;

            {
                const u8 *text = (const u8 *)0xc7b;

                Func_0801e7c0(text, state->window_07c, 18, 40);
                Func_0801e7c0(text + 1, state->window_07c, 18, 48);
                Func_0801e7c0(text + 2, state->window_07c, 18, 56);
                Func_0801e7c0(text + 3, state->window_07c, 18, 64);
                text += 4;
                Func_0801e7c0(text, state->window_07c, 18, 72);
                goto done;
            }
        }

        {
            const u8 *text = (const u8 *)0xc7b;

            Func_0801e7c0(text, state->window_07c, 18, 40);
            Func_0801e7c0(text + 1, state->window_07c, 18, 48);
            text += 2;
            Func_0801e7c0(text, state->window_07c, 18, 56);
            goto done;
        }

mode_other:
        {
            const u8 *text = (const u8 *)0xc7b;

            Func_0801e7c0(text++, state->window_07c, 18, 40);
            Func_0801e7c0(text, state->window_07c, 18, 48);
        }
done:
        ;
    }
}
