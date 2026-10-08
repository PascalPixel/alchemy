#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "text/msg_ids.h"



void DisplayScroll_RenderEnteringLine(void)
{
    s32 first = (s32)MsgIntroCrawl;
    u32 current;
    s32 line;

    if (DisplayScroll_Busy == 0) {
        current = DisplayScroll_Position;
        if ((s16)DisplayScroll_Position / 16 != DisplayScroll_LastPosition / 16) {
            DisplayScroll_LastPosition = current;
            line = (s16)((s16)DisplayScroll_Position / 8);
            DisplayScroll_Busy = DisplayScroll_DrawMessageLine(line / 2 + first - 1, ((line + 16) & 31) * 24, 1);
        }
    }
}
