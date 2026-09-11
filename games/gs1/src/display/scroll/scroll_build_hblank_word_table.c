#include "types.h"
#include "scene.h"

/* display/scroll/scroll_build_hblank_word_table.c */
/* display/scroll/build_hblank_word_table.c */
extern u8 Value_00020002;

void DisplayScroll_BuildHblankWordTable(u32 *arg0)
{
    s32 count;
    u32 value = 0x01FF01FF;
    u32 step = 0x10000;

    count = 31;
    do {
        count--;
        *arg0++ = value;
    } while (count >= 0);
    {
        u32 increment = (u32)&Value_00020002;
        count = 239;
        do {
            count--;
            *arg0++ = step;
            step += increment;
        } while (count >= 0);
    }
    count = 47;
    do {
        count--;
        *arg0++ = value;
    } while (count >= 0);
    step = 0;
    count = 191;
    do {
        count--;
        *arg0++ = step;
    } while (count >= 0);
}

/* display/scroll/step_position_every_four_frames.c */
typedef struct {
    u16 unused[4];
    u16 first;
    u16 padding;
    u16 second;
} State;

extern u32 gIw;
extern State gIw2;
extern u8 Value_0000ffff;

void DisplayScroll_StepPositionEveryFourFrames(void)
{
    if ((gIw & 3) == 0) {
        State *state = &gIw2;
        u32 decrement = (u32)&Value_0000ffff;
        state->first += decrement;
        state->second += decrement;
    }
}

/* graphics/color/no_op_color.c */
/* graphics/color/reserved_no_op.c */
void Graphics_NoOpColor(void)
{
}

/* graphics/color/interpolate_palette_buffers.c */
void Graphics_InterpolatePaletteBuffers(s16 *a, s16 *b, s16 *dst, s32 n)
{
    s32 index;
    s32 first;
    s32 second;
    s32 (*divide)(s32, s32);

    if (n > 0) {
        divide = (s32 (*)(s32, s32))0x03000380;
        index = 0x5FF;
        do {
            first = *a;
            second = *b;
            *dst = divide(second - first, n);
            index--;
            a++;
            b++;
            dst++;
        } while (index >= 0);
    }
}
