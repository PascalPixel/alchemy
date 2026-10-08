#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "window.h"
#include "ram_buffer.h"
#include "iwram_call.h"

s32 Ui_ClearVramBlock(void)
{
    s32 (*fill)(void *destination, s32 size, u32 value) = Iwram_FillWords;

    return fill((void *)0x06002500, 0xf00, 0);
}

void Ui_FillVramBlockPattern(void)
{
    s32 (*fill)(void *destination, s32 size, u32 value) = Iwram_FillWords;

    fill((void *)0x06002500, 0xf00, 0x44444444);
}

void UiWork_ResetFreeChannel(void)
{
    struct UiChannelSlot *slot =
        ((struct UiRenderWork *)Ram_HeapSlots->window_tiles)->channels;
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
