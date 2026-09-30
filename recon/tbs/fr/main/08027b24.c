/* 2026-09-30: the French Menu_DrawModeIndicator, the one routine that keeps
   games/THE BROKEN SEAL/SRC/MENU/SELECTION.C out of the French build. It
   clears its list with RenderOutput_ClearList in each branch, and redraws
   the cable help only when it follows the password help. The logic and
   every call match; the remaining difference is one hoisted load: this
   build loads *current as a halfword at the top for the final store on the
   path without calls (and shifts the registers by one), where the
   reference reloads it once after both branches. */
#include "TYPES.H"

extern void *gMenuSelectWork;
extern u8 MsgPasswordTransferHelp;
extern u8 MsgCableTransferHelp;

void RenderOutput_ClearList(void *);
void UiText_DrawResource(s32 no, s32 work, s32 x, s32 y);

void Menu_DrawModeIndicator(void)
{
    u8 *state = (u8 *)gMenuSelectWork;
    s16 *shown = (s16 *)(state + 150);
    s16 *current = (s16 *)(state + 140);

    if (*shown != *current) {
        if (*current == 0) {
            RenderOutput_ClearList(*(void **)(state + 124));
            UiText_DrawResource((s32)&MsgPasswordTransferHelp,
                *(s32 *)(state + 124), 16, 4);
            UiText_DrawResource((s32)&MsgPasswordTransferHelp + 1,
                *(s32 *)(state + 124), 16, 16);
        } else if (*shown == 0) {
            RenderOutput_ClearList(*(void **)(state + 124));
            UiText_DrawResource((s32)&MsgCableTransferHelp,
                *(s32 *)(state + 124), 0, 4);
            UiText_DrawResource((s32)&MsgCableTransferHelp + 1,
                *(s32 *)(state + 124), 0, 16);
            UiText_DrawResource((s32)&MsgCableTransferHelp + 2,
                *(s32 *)(state + 124), 0, 28);
        }
        *shown = *current;
    }
}
