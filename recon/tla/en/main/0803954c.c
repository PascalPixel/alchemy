/* differ 8: the native stores the channel fields before it builds the text-mode offset, and loads the zero before addressing the spacing; only ARM9TDMI scheduling differs */
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

struct UiChannelSlot *UiText_QueueRenderEntries(struct UiWindow *window, s32 entry, s32 x, s32 y, u16 *spacing, s32 flags)
{
    u8 *work = (u8 *)Ram_HeapSlots->window_tiles;
    struct UiChannelSlot *render = ((struct UiRenderWork *)work)->channels;
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
        found->unknown_26 = 0;
        if (*(s8 *)(work + UI_TEXT_MODE_OFFSET) != 0 && spacing != NULL)
            found->unknown_26 = spacing[3];
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
