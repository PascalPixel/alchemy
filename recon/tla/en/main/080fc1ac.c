/* Pin-free trial (tla-en): listing 127 instructions, draft 130, common 93, differ 71.
 * Earlier register-allocation notes describe the prior pinned form.
 */
#include "types.h"
#include "io_reg.h"

extern volatile s32 gKeysRepeat;
extern volatile s32 gKeyState;
extern u8 MsgItemName[];
extern u8 MsgConfirmDrop[];
extern u8 MsgYes[];

s32 __modsi3(s32, s32);
void WaitFrames(s32 frames);
s32 UiWindow_CreateFar(s32, s32, s32, s32, s32);
void UiWork_FinalizeFar(s32, s32);
void UiText_DrawAt(s32, s32, s32, s32);
void Item_Get(s32);
s32 GameFlag_IsSet(s32);
void UiMenu_PositionCursor(s32, s32);
void UiMenu_SlideCursor(s32, s32);
void Audio_PlayCue(s32 cue);

s32 ItemMenu_ConfirmDrop(s32 a0)
{
    volatile s32 *pad;
    s32 win;
    s32 slot;
    s32 text;
    s32 label;
    s32 selection; /* Earlier pinned trial: keeps the row in r6 */
    s32 changed;

    win = UiWindow_CreateFar(13, 3, 17, 10, 2);
    slot = a0 & 0x1ff;
    Item_Get(slot);
    UiText_DrawAt(slot + (s32)MsgItemName, win, 24, 0);
    text = (s32)MsgConfirmDrop;
    UiText_DrawAt(text, win, 0, 16);
    text++;
    UiText_DrawAt(text, win, 0, 24);
    label = (s32)MsgYes;
    UiText_DrawAt(label, win, 24, 40);
    label++;
    UiText_DrawAt(label, win, 24, 56);
    selection = 1;
    changed = 1;
    UiMenu_SlideCursor(104, 86);
    for (;;) {
        if (GameFlag_IsSet(0x150) != 0) {
            break;
        }
        {
        s32 c; /* Earlier pinned trial: tests the flag through r2 */
        asm("mov %0, %1" : "=l"(c) : "h"(changed)); /* FAKEMATCH: tests the flag through r2 */
        if (c) {
            changed = 0;
            selection = __modsi3(selection + 2, 2);
        }}
        if (gKeyState & KEY_A) {
            Audio_PlayCue(112);
            break;
        }
        if (gKeyState & KEY_B) {
            Audio_PlayCue(113);
            selection = 1;
            break;
        }
        UiMenu_PositionCursor(104, (selection << 4) + 70);
        pad = &gKeysRepeat;
        if (*pad & 64) {
            {
            s32 one = 1; /* Earlier pinned trial: sets the flag through r2 */
            s32 cue = 111; /* Earlier pinned trial: loads the cue before the step */
            selection -= 1;
            asm volatile("mov %0, %1" : "=h"(changed) : "l"(one)); /* FAKEMATCH: sets the flag through r2 */
            Audio_PlayCue(cue);
            }
        }
        if (*pad & 128) {
            selection += 1;
            changed = 1;
            Audio_PlayCue(111);
        }
        WaitFrames(1);
    }
    if (GameFlag_IsSet(0x150) != 0) {
        selection = 1;
    }
    UiWork_FinalizeFar(win, 1);
    return selection;
}
