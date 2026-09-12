#include "types.h"
#include "scene.h"

/* battle/effects/misc/wait_then_set_field18.c */
/* battle/effects/misc/wait_then_set_field18.c */
void WaitFrames(s32);

void BattleFx_WaitThenSetField18To4(void *arg0)
{
  unsigned int ofs;
  Battle_SetMode(*((s32 *)(arg0 + 8)), *((s16 *)(((u8 *)arg0) + 0x24)), 0x18, 0xC3333);
  WaitFrames(0x1D);
  ofs = 0x18;
  *((s32 *)(((u8 *)arg0) + ofs)) = 4;
  Battle_Apply(arg0, 2);
}

/* object/wait_twelve_frames_then_set_field18.c */

struct Object080eceac {
  u8 padding_00[8];
  s32 field_08;
  u8 padding_0c[0xc];
  s32 field_18;
  u8 padding_1c[8];
  s16 field_24;
};

void Object_WaitTwelveFramesThenSetField18(struct Object080eceac *obj)
{
  Obj_SetMode(obj->field_08, obj->field_24, 0x18, 0x73333);
 do { WaitFrames(0xC); } while (0);
  obj->field_18 = 3;
  Obj_Apply(obj, 2);
}

/* battle/effects/reveal_column/fx_run_reveal_column_mode1.c */
/* battle/effects/reveal_column/run_mode1.c */
void BattleFx_RunRevealColumnMode1(s32 arg0)
{
    Battle_ApplyRevealColumnMode1(arg0, 1);
}

/* battle/effects/reveal_column/run_mode2.c */
void BattleFx_RunRevealColumnMode2(s32 arg0)
{
    Battle_ApplyRevealColumnMode1(arg0, 2);
}

/* display/scroll/scroll_build_hblank_word_table.c */
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
