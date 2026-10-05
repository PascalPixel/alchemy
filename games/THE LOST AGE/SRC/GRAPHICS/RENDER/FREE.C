#include "WINDOW.H"
#include "RAM_BUFFER.H"

s32 Ui_ClearVramBlock(void);

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
