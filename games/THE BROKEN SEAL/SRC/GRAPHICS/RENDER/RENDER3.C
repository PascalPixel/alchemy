#include "TYPES.H"
#include "TBS_EDITION.H"
#include "IWRAM_CALL.H"
#include "GLOBAL_CELLS.H"

/* One of the three queued text renders in the glyph work area. */
struct TextRender {
    void *entries;
    s16 x;
    s16 y;
    u16 colours[4];
    s16 unknown_10;
    s16 window;
    s16 unknown_14;
    s16 unknown_16;
    s16 unknown_18;
    s16 unknown_1a;
    s16 unknown_1c;
    s16 start_x;
    s16 unknown_20;
    s16 unknown_22;
    s16 flags;
    s16 unknown_26;
};

extern u8 *gWindowWork;

extern u8 Data_03001e8c[];

struct UiChannelWork {
    u8 padding00[0x14];
    u16 state;
};

struct UiChannelSlot {
    struct UiChannelWork *work;
    u16 field04;
    u16 field06;
    u16 values[4];
    u16 field10;
    u16 field12;
    u16 field14;
    u16 field16;
    u16 field18;
    u16 field1a;
    u16 field1c;
    u16 field1e;
    u16 field20;
    u16 field22;
    u16 field24;
    u16 field26;
};

void UiWork_ResetChannelTransition(void *);
s32 Ui_ClearVramBlock(void);
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

/* Claims the first of the window work's three text render slots for a
   built entry list, placing it at x, y (whole pixels) in window with the
   given colours (or colour 0) and flags. Returns the slot, or NULL. */
struct TextRender *UiText_QueueRenderEntries(void *entries, s32 window, s32 x, s32 y, u16 *colours, s32 flags)
{
    struct TextRender *render = (struct TextRender *)(gWindowWork + RENDER_CHANNEL_OFS);
    struct TextRender *found = NULL;
    u32 i;

    for (i = 0; i != 3; i++, render++) {
        if (render->entries == NULL) {
            found = render;
            break;
        }
    }
    if (found != NULL) {
        found->entries = entries;
        found->start_x = x << 8;
        found->x = x << 8;
        found->y = y << 8;
        found->window = window;
        found->unknown_16 = 15;
        found->unknown_1a = 10;
        found->unknown_14 = 0;
        found->unknown_18 = 0;
        found->unknown_20 = 0;
        found->flags = flags;
        if (colours != NULL) {
            for (i = 0; i < 4; i++)
                found->colours[i] = *colours++;
        } else {
            for (i = 0; i < 4; i++)
                found->colours[i] = 0;
        }
        found->unknown_10 = 0;
    }
    return found;
}

struct UiChannelSlot *UiWork_ActivateChannel(struct UiChannelWork *work, s32 value, s32 preserve)
{
    struct UiChannelSlot *slot;
    struct UiChannelSlot *selected;
    u16 *destination;
    u16 val04;
    u16 zero;
    u32 index;

    slot = (struct UiChannelSlot *)(gWindowWork + RENDER_CHANNEL_OFS);
    selected = 0;
    for (index = 0; index != 3; slot++, index++) {
        if (slot->work == 0 || slot->work->state != 0) {
            selected = slot;
            break;
        }
    }

    if (selected != 0) {
        if (selected->work == 0) {
            selected->field06 = 0xA00;
            val04 = 0x300;
            selected->work = work;
            goto reset_field04;
        }
        if (preserve == 0) {
            if (selected->field06 == 0) {
                selected->field06 = 0xA00;
            } else if ((u32)selected->field06 < 0xD00) {
                selected->field06 += 0xD00;
            } else {
                UiWork_ResetChannelTransition(selected);
            }
            val04 = 0x300;
reset_field04:
            selected->field04 = val04;
        }

        zero = 0;
        selected->field1e = 0x300;
        selected->work->state = zero;
        selected->field16 = 15;
        selected->field1a = 10;
        selected->field12 = value;
        selected->field14 = zero;
        selected->field18 = zero;
        selected->field10 = zero;
        selected->field20 = zero;
        index = 0;
        destination = selected->values;
        do {
            index++;
            *destination++ = zero;
        } while (index <= 3);
    }
    return selected;
}

s32 Ui_ClearVramBlock(void)
{
    s32 (*fill)(void *, s32, u32) = Iwram_FillWords;

    return fill((void *)0x06002500, 0xF00, 0);
}

s32 Ui_FillVramBlockPattern(void)
{
    s32 (*fill)(void *, s32, u32) = Iwram_FillWords;

    return fill((void *)0x06002500, 0xF00, 0x44444444);
}

void UiWork_ResetFreeChannel(void)
{
    struct UiChannelSlot *slot =
        (struct UiChannelSlot *)(gWindowWork + RENDER_CHANNEL_OFS);
    struct UiChannelSlot *sel = 0;
    s32 i;

    for (i = 0; i != 3; slot++, i++) {
        if (slot->work == 0 || slot->work->state != 0) {
            sel = slot;
            break;
        }
    }
    if (sel != 0) {
        if (sel->work != 0) {
            Ui_ClearVramBlock();
            sel->field06 = 0;
        }
        sel->field04 = 0;
        sel->field14 = 0;
        sel->field16 = 0xF;
        sel->field18 = 0;
        sel->field1a = 0xA;
    }
}

void UiWork_CopyParamsToRenderWork(void *work)
{
    u32 p2;
    s32 p1;
    s32 z1;
    u32 p0;
    u32 z0;
    void *render;

    render = *(void **)((u32)&Data_03001e8c);
    p0 = (s32)(FIELD_AT_OFFSET(work, u16 *, 0x16)); FIELD_AT_OFFSET(render, u16 *, RENDER_PARAM_OFS) = (u16)p0;
    p1 = (s32)(FIELD_AT_OFFSET(work, u16 *, 0x18)); (s32)z0 = 0; FIELD_AT_OFFSET(render, u16 *, RENDER_WORD_OFS) = (u16)p1;
    p2 = (FIELD_AT_OFFSET(work, u16 *, 0x1A)); (s32)z1 = 0; FIELD_AT_OFFSET(render, u16 *, RENDER_WORD2_OFS) = (u16)p2;
}

void UiWork_ResetChannelTransition(void *work)
{
  int mode;
  int val;
  u8 *p;
  p = ((u8 *)work) + 0x1C;
  val = 2;
  mode = val;
  *((s16 *)p) = mode;
}
