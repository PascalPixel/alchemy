#include "TYPES.H"
#include "FIXED_MATH.H"
#include "UI.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFFECT_WORK.H"
#include "RAM_BUFFER.H"

/* The reel game's state block, the seventh heap-cache cell. */
struct ReelWork {
    u8 unknown_000[0x8c];
    s32 state;
    s32 cursor;
    s32 spins;
    u8 unknown_098[0x434];
    s32 sub_window;
    u8 unknown_4d0[8];
    u16 scanline_offsets[160];
};

extern u8 gBattleFxWork[];
extern char MsgSlotsBet;
extern const u8 ReelGame_TitleLetterWidths[];

/* Reel game: clear every particle's variant, line the eight title letters
   up above the screen (each 0x80000 higher than the last, spaced by their
   widths), build the scanline offset curve (zero outside a cosine bump
   mirrored about line 86), reset the state, cursor and spin count, and open
   the two-line coin window. */
void ReelGame_InitTitle(void)
{
    struct ReelWork *state;
    struct BattleEffectWork *work;
    struct EffectStep *letter;
    const u8 *width;
    s32 left;
    s32 step;
    s32 y;
    s32 i;
    s32 angle;
    s32 window;
    s32 message;

    state = ((struct ReelWork **)gBattleFxWork)[6];
    work = ((struct BattleEffectWork **)gBattleFxWork)[0];
    left = 0;
    for (i = 0; i != 0x800; i++)
        ((struct EffectStep *)Ram_MapCellBuffer)[i].variant = 0;

    letter = work->particles;
    y = -0x200000;
    width = ReelGame_TitleLetterWidths;
    for (i = 0; i != 8; i++) {
        letter[i].x = (left + 24) << 16;
        step = *width;
        width++;
        left += step;
        letter[i].y = y;
        letter[i].velocity_y = 0;
        letter[i].variant = 0;
        y += -0x80000;
    }

    for (i = 0; i != 160; i++)
        state->scanline_offsets[i] = 0;

    for (i = 0; i != 40; i++) {
        angle = i * 0x199;
        state->scanline_offsets[23 + i] = (u32)(Trig_Cos(angle) * 3) >> 15;
        state->scanline_offsets[110 - i] = (u32)(Trig_Cos(angle) * 3) >> 15;
    }

    state->spins = 0;
    state->state = 0;
    state->cursor = 0;
    work->transfer_mode = 1;
    work->transfer_value = 0;
    *(volatile u16 *)0x04000050 = 0;
    window = UiWindow_CreateFar(18, 0, 12, 4, 6);
    state->sub_window = window;
    message = (s32)&MsgSlotsBet;
    UiText_DrawCharacterAtOffsetFar(message, window, 0, 8);
    UiText_DrawCharacterAtOffsetFar(message - 1, state->sub_window, 0, 0);
}
