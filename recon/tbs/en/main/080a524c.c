/* 2026-09-29: the two confirmation messages are now named in the PO files
   (MsgConfirmDrop and MsgYes, in every edition), and the callees and key
   words carry their build names: 60 on the permuter scorer before a
   rebuild names the messages, which leaves only the flag register (r3
   where the reference takes r2 for the first changed = 1 and the test).
   Four minutes of permutation from here found nothing lower. */
/* NONMATCHING: 316 bytes, candidate 316, 4 differing halfwords, 4 halfword
 * edits (2026-09-25). The changed flag uses r3 where the reference uses r2.
 * Narrowing the flag leaves the output unchanged; an increment or a scoped
 * assignment regresses.
 * WALL: Temporary register choice for the changed flag's assignment and test. */
#include "TYPES.H"

extern volatile s32 gKeysRepeat;
extern volatile s32 gKeyState;
extern u8 MsgItemName[];
extern u8 MsgConfirmDrop[];
extern u8 MsgYes[];

s32 Func_080022fc(s32, s32);
void WaitFrames(s32 frames);
s32 UiWindow_CreateFar(s32, s32, s32, s32, s32);
void UiWork_FinalizeFar(s32, s32);
void UiText_DrawAt(s32, s32, s32, s32);
void Item_Get(s32);
s32 GameFlag_IsSet(s32);
void UiMenu_PositionCursor(s32, s32);
void UiMenu_SlideCursor(s32, s32);
void Audio_PlayCue(s32 cue);

s32 Func_080a524c(s32 a0)
{
    volatile s32 *pad;
    s32 win;
    s32 slot;
    s32 text;
    s32 label;
    s32 sel;
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
    sel = 1;
    changed = 1;
    UiMenu_SlideCursor(104, 86);
    for (;;) {
        if (GameFlag_IsSet(0x150) != 0) {
            break;
        }
        if (changed) {
            changed = 0;
            sel = Func_080022fc(sel + 2, 2);
        }
        if (gKeyState & 1) {
            Audio_PlayCue(112);
            break;
        }
        if (gKeyState & 2) {
            Audio_PlayCue(113);
            sel = 1;
            break;
        }
        UiMenu_PositionCursor(104, (sel << 4) + 70);
        pad = &gKeysRepeat;
        if (*pad & 64) {
            sel -= 1;
            changed = 1;
            Audio_PlayCue(111);
        }
        if (*pad & 128) {
            sel += 1;
            changed = 1;
            Audio_PlayCue(111);
        }
        WaitFrames(1);
    }
    if (GameFlag_IsSet(0x150) != 0) {
        sel = 1;
    }
    UiWork_FinalizeFar(win, 1);
    return sel;
}
