#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "window.h"
#include "tbs_edition.h"
#include "iwram_call.h"
#include "global_cells.h"


struct UiChannelSlot *UiText_QueueRenderEntries(struct UiWindow *window, s32 entry, s32 x, s32 y, u16 *spacing, s32 flags)
{
    struct UiChannelSlot *render = ((struct UiRenderWork *)gWindowWork[0])->channels;
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
        found->colour = 15;
        found->line_spacing = 10;
        found->countdown = 0;
        found->outline = 0;
        found->ended = 0;
        found->flags = flags;
        if (spacing != NULL) {
            for (i = 0; i < 4; i++)
                found->spacing[i] = *spacing++;
        } else {
            for (i = 0; i < 4; i++)
                found->spacing[i] = 0;
        }
        found->line = 0;
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

    slot = ((struct UiRenderWork *)gWindowWork[0])->channels;
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
            } else if (selected->y < 0xD00) {
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
        selected->colour = 15;
        selected->line_spacing = 10;
        selected->entry = value;
        selected->countdown = zero;
        selected->outline = zero;
        selected->line = zero;
        selected->ended = zero;
        index = 0;
        destination = selected->spacing;
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
        ((struct UiRenderWork *)gWindowWork[0])->channels;
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
        sel->colour = 0xF;
        sel->outline = 0;
        sel->line_spacing = 0xA;
    }
}

void UiWork_CopyParamsToRenderWork(struct UiChannelSlot *channel)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];

    work->colour = channel->colour;
    work->outline = channel->outline;
    work->line_spacing = channel->line_spacing;
}

void UiWork_ResetChannelTransition(struct UiChannelSlot *channel)
{

    channel->transition = 2;
}
