/* differ 6: the native stores the entry before building the line spacing; only ARM9TDMI store scheduling differs */
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
#include "resource.h"
#include "iwram_call.h"

#if EDITION_INTERNATIONAL
#define UI_TEXT_MODE_OFFSET 0x138c
#define UI_PALETTE_OWNER_OFFSET 0x1360
#else
#define UI_TEXT_MODE_OFFSET 0x118c
#define UI_PALETTE_OWNER_OFFSET 0x1160
#endif

struct UiChannelSlot *Func_080395fc(struct UiWindow *work, s32 value, s32 preserve)
{
    struct UiChannelSlot *slot;
    struct UiChannelSlot *selected;
    u16 *destination;
    u16 val04;
    u16 zero;
    u32 index;

    slot = ((struct UiRenderWork *)Ram_HeapSlots->window_tiles)->channels;
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
