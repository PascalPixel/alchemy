#include "types.h"
#include "global_cells.h"
#include "sound_ids.h"

extern void WaitFrames(u32);
extern void Audio_PlayCue(u32);
extern void Func_0801b664(void *);
extern void Func_0801b810(void *);

u32 Menu_WaitForSelectionInput(u32 value)
{
    u8 *state = *(u8 **)ADDR_03001E98;
    volatile u32 *input;
    u32 result;

again:
    WaitFrames(1);
    if (*(u16 *)(state + 0x3a0) != 0)
        goto again;

    if (value != 999) {
        input = (u32 *)ADDR_03001B04;
        if (*input & 0x10) {
            Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
            Func_0801b664(state);
        } else if (*input & 0x20) {
            Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
            Func_0801b810(state);
        }

        if (*(u32 *)ADDR_03001C94 & 1) {
            result = *(u16 *)(state + 0x39c)
                   + *(u16 *)(state + 0x39e);
            if (*(u16 *)(*(u8 **)(state + 0x348) + 10) == 6) {
                if (result == 0)
                    Audio_PlayCue(SOUND_MENU_CONFIRM);
                else
                    Audio_PlayCue(SOUND_MENU_CANCEL);
            } else {
                Audio_PlayCue(SOUND_MENU_CONFIRM);
            }
            return result;
        }
    }

    if (value != 0 && (*(u32 *)ADDR_03001C94 & 2)) {
        Audio_PlayCue(SOUND_MENU_CANCEL);
        return -1;
    }
    goto again;
}
