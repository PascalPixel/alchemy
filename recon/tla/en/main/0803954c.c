/* differ 8: the native stores the channel fields before it builds the text-mode offset, and loads the zero before addressing the spacing; only ARM9TDMI scheduling differs */
#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "WINDOW.H"
#include "RAM_BUFFER.H"
#include "RESOURCE.H"
#include "IWRAM_CALL.H"

#if EDITION_INTERNATIONAL
#define UI_TEXT_MODE_OFS 0x138c
#define UI_PALETTE_OWNER_OFS 0x1360
#else
#define UI_TEXT_MODE_OFS 0x118c
#define UI_PALETTE_OWNER_OFS 0x1160
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
        if (*(s8 *)(work + UI_TEXT_MODE_OFS) != 0 && spacing != NULL)
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
