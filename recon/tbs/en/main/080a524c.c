/* Pin-free ItemMenu_ConfirmDrop trial: 128 native instructions, 130 built,
 * 96 common, differ 66. Removing only the flag-test copy
 * binding differs 4 of 128 instructions (r3 replaces r2). Keeping its
 * empty barrier as a tied changed input leaves that residual; adding rows
 * as an input differs 27. Removing the one binding alone differs 62;
 * removing changed alone differs 48. A 3-minute search of the first trial
 * (seed 131, exact stack/branches) found no improvement. All three
 * bindings are removed here; the linked source keeps them. */
/* Further flag-copy-only trials: a low-register changed input retains
 * differ 4; making the copy early-clobber differs 21. A sel output differs
 * 38, and carrying the preceding flag-test result through text differs 43.
 * A sel input retains 4; a rows output differs 34 (130 built). Removing
 * only one's binding and adding sel as its barrier input differs 76
 * (130 built). An unsigned copy or generic register output retains 4;
 * sharing the zero assignment with the copy leaves 32, and keeping text
 * and label live at the copy leaves 68 (130 built). None closes the
 * r3/r2 residual. */
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "edition.h"
#include "btlsys.h"
#include "inventory_menu.h"
#include "types.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "item.h"
#include "text/msg_ids.h"
s32 GameFlag_TestFar(s32 flag);
struct ItemDefinition *Item_GetDirectFar(s32 item);

#define UiWindow_ClearInteriorTilesFar ((void (*)(s32,s32,s32,s32,s32))UiWindow_ClearInteriorTilesFar)

extern void UiText_DrawCharacterAtOffsetFar(s32, void *, s32, s32);

extern volatile s32 gKeysRepeat;
extern volatile s32 gKeyState;
s32 __modsi3(s32, s32);
void WaitFrames(s32 frames);
s32 UiWindow_CreateFar(s32, s32, s32, s32, s32);
void UiWork_FinalizeFar(s32, s32);
void AudioCommand_PlayFar(s32 cue);

s32 ItemMenu_ConfirmDrop(s32 a0)
{
    s32 win;
    s32 slot;
    s32 text;
    s32 label;
    s32 rows = 2;
    s32 sel;
    s32 changed;

    win = UiWindow_CreateFar(13, 3, 17, 10, 2);
    slot = a0 & ITEM_ID_MASK;
    Item_GetDirectFar(slot);
    UiText_DrawCharacterAtOffsetFar(slot + (s32)((u8 *)MsgItemName), win, 24, 0);
    text = (s32)MsgConfirmDrop;
#if EDITION_INTERNATIONAL
    UiText_DrawCharacterAtOffsetFar(text, win, 0, 16);
#else
    UiText_DrawCharacterAtOffsetFar(text, win, 8, 16);
#endif
    text++;
#if EDITION_INTERNATIONAL
    UiText_DrawCharacterAtOffsetFar(text, win, 0, 24);
#else
    UiText_DrawCharacterAtOffsetFar(text, win, 8, 24);
#endif
    label = (s32)MsgYes;
    UiText_DrawCharacterAtOffsetFar(label, win, 24, 40);
    label++;
    UiText_DrawCharacterAtOffsetFar(label, win, 24, 56);
    sel = 1;
    changed = 1;
    UiMenu_SlideCursor(104, 86);
    for (;;) {
        if (GameFlag_TestFar(0x150) != 0) {
            break;
        }
        {
        s32 c = changed;
        /* FAKEMATCH: retain the source's copy barrier during the pin-free trial. */
        asm("" : "+l"(c));
        if (c) {
            changed = 0;
            sel = (sel + rows) % rows;
        }}
        if (gKeyState & KEY_A) {
            AudioCommand_PlayFar(112);
            break;
        }
        if (gKeyState & KEY_B) {
            AudioCommand_PlayFar(113);
            sel = 1;
            break;
        }
        UiMenu_PositionCursor(104, (sel << 4) + 70);
        /* FAKEMATCH: needed to match: the key-repeat pointer shares text's variable; as a pointer of its own it ranks below sel and the two swap r5 and r6 (13 of 134 rows differ) */
        text = (s32)&gKeysRepeat;
        if (*(volatile s32 *)text & 64) {
            s32 one = 1;
            /* FAKEMATCH: retain the source's constant barrier during the pin-free trial. */
            asm("" : "+l"(one));
            sel -= 1;
            changed = one;
            AudioCommand_PlayFar(111);
        }
        if (*(volatile s32 *)text & 128) {
            sel += 1;
            changed = 1;
            AudioCommand_PlayFar(111);
        }
        WaitFrames(1);
    }
    if (GameFlag_TestFar(0x150) != 0) {
        sel = 1;
    }
    UiWork_FinalizeFar(win, 1);
    return sel;
}

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);

static __inline__ s32 CopyWords(WordCopyFn copy, void *dst, const void *src, s32 size)
{
    /* FAKEMATCH: a direct call loads the size before the copy routine and the two blocks, where the reference loads it last. */
    return copy(dst, src, size);
}
