#include "TYPES.H"
#include "WINDOW.H"
#include "TBS_EDITION.H"
#include "IWRAM_CALL.H"
#include "GLOBAL_CELLS.H"

void UiWork_ResetChannelTransition(void *);
s32 Ui_ClearVramBlock(void);
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

/* Claims the first of the window work's three text render slots for a
   built entry list, placing it at x, y (whole pixels) in window with the
   given colours (or colour 0) and flags. Returns the slot, or NULL. */
struct UiChannelSlot *UiText_QueueRenderEntries(struct UiWindow *window, s32 entry, s32 x, s32 y, u16 *colours, s32 flags)
{
    struct UiChannelSlot *render = (struct UiChannelSlot *)(gWindowWork[0] + RENDER_CHANNEL_OFS);
    struct UiChannelSlot *found = NULL;
    u32 i;

    for (i = 0; i != 3; i++, render++) {
        if (render->work == NULL) {
            found = render;
            break;
        }
    }
    if (found != NULL) {
        found->work = window;
        found->start_x = x << 8;
        found->x = x << 8;
        found->y = y << 8;
        found->entry = entry;
        found->unknown_16 = 15;
        found->unknown_1a = 10;
        found->countdown = 0;
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

struct UiChannelSlot *UiWork_ActivateChannel(struct UiWindow *work, s32 value, s32 preserve)
{
    struct UiChannelSlot *slot;
    struct UiChannelSlot *selected;
    u16 *destination;
    u16 val04;
    u16 zero;
    u32 index;

    slot = (struct UiChannelSlot *)(gWindowWork[0] + RENDER_CHANNEL_OFS);
    selected = 0;
    for (index = 0; index != 3; slot++, index++) {
        if (slot->work == 0 || slot->work->state != 0) {
            selected = slot;
            break;
        }
    }

    if (selected != 0) {
        if (selected->work == 0) {
            selected->y = 0xA00;
            val04 = 0x300;
            selected->work = work;
            goto reset_x;
        }
        if (preserve == 0) {
            if (selected->y == 0) {
                selected->y = 0xA00;
            } else if ((u32)selected->y < 0xD00) {
                selected->y += 0xD00;
            } else {
                UiWork_ResetChannelTransition(selected);
            }
            val04 = 0x300;
reset_x:
            selected->x = val04;
        }

        zero = 0;
        selected->start_x = 0x300;
        selected->work->state = zero;
        selected->unknown_16 = 15;
        selected->unknown_1a = 10;
        selected->entry = value;
        selected->countdown = zero;
        selected->unknown_18 = zero;
        selected->unknown_10 = zero;
        selected->unknown_20 = zero;
        index = 0;
        destination = selected->colours;
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
        (struct UiChannelSlot *)(gWindowWork[0] + RENDER_CHANNEL_OFS);
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
            sel->y = 0;
        }
        sel->x = 0;
        sel->countdown = 0;
        sel->unknown_16 = 0xF;
        sel->unknown_18 = 0;
        sel->unknown_1a = 0xA;
    }
}

void UiWork_CopyParamsToRenderWork(struct UiChannelSlot *channel)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];

    work->colour = channel->unknown_16;
    work->outline = channel->unknown_18;
    work->line_spacing = channel->unknown_1a;
}

void UiWork_ResetChannelTransition(void *work)
{
    struct UiChannelSlot *channel = work;

    channel->transition = 2;
}
