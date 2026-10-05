#include "PROTO.H"
#include "GLOBAL.H"
#include "TYPES.H"
#include "text/MSG_IDS.H"



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
