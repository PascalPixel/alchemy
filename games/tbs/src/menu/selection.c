#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resmenu.h"
#include "edition.h"
#include "types.h"
#include "save_state.h"

void Menu_LayoutResourceEntries(s32 x, s32 y, s32 width, s32 resource_base)
{
    struct ResourceMenuWork *work;
    s32 i;
    u8 *entries;

    work = gMenuSelectWork;
    work->width = width + 2;
    work->resource_base = resource_base;
    work->row = y;
    /* FAKEMATCH: the byte span keeps the sprite base at +0; struct coordinates hoist +12 into the loop and add four bytes. */
    entries = (u8 *)work;
    for (i = 0; i < work->count; i++) {
        *(u16 *)(entries + i * sizeof(struct ResourceMenuEntry) + 14) = y << 3;
        *(u16 *)(entries + i * sizeof(struct ResourceMenuEntry) + 12) = x << 3;
        x = x + 3;
    }
    work->window = UiWindow_Create(x, y, work->width, 3, 2);
}

#include "tbs_edition.h"
#include "text/msg_ids.h"

#define UiText_DrawCharacterAtOffset ((void (*)(s32, void *, s32, s32))UiText_DrawCharacterAtOffset)

#define UiWindow_ClearInteriorTiles ((void (*)(void*,s32,s32,s32,s32))UiWindow_ClearInteriorTiles)

void Menu_CenterResourceEntries(s32, s32, s32);

static __inline__ s32 TblGet(s8 *tbl, s32 index)
{
    return tbl[index];
}

s32 Menu_SelectTopEntry(s32 sel)
{
    s32 group = 0;
    s32 ofs;
    s32 triple;
    s32 ret;
    s8 *tbl;

    if (Party_SumDjinnCountsFar(-1) == 0) {
        group = 1;
    }

    triple = group * 3;
    tbl = Menu_TopEntryPositionByCommand;
    ofs = triple << 1;
    sel = TblGet(tbl, sel + ofs) - 1;
    if (sel < 0) {
        sel = 0;
    }

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(1);
    if (group == 0) {
        Menu_AppendResourceEntry(15);
    }
    Menu_AppendResourceEntry(2);
    Menu_AppendResourceEntry(7);
    Menu_CenterResourceEntries(17, SELECT_MENU_WIDTH, 0);
    ret = Menu_RunResourceSelectionLoop(sel);
    Menu_EndResourceSelection();

    if (ret >= 0) {
        ret = Menu_TopEntryCommandByPosition[ret + ofs + 1];
    }

    return ret;
}

#if EDITION_INTERNATIONAL
#define ANIMATE_ENTRIES_ROW 7
#else
#define ANIMATE_ENTRIES_ROW 5
#endif

s32 Menu_AnimateSelectionToEntry(s32 arg0, s32 arg1)
{
    UiWindow_OpenMode1AndWaitFrame();
    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(1);
    Menu_AppendResourceEntry(0xF);
    Menu_AppendResourceEntry(2);
    Menu_AppendResourceEntry(7);
    Menu_CenterResourceEntries(0x11, ANIMATE_ENTRIES_ROW, 0);
    arg1 = Menu_SelectResource(arg0, arg1 - 1);
    Menu_EndResourceSelection();
    UiWork_CloseAndRelease();
    return arg1;
}

s32 Menu_SelectSaveSlotAction(void)
{
    s32 type;
    s32 ret;
    s32 initial;
    u32 group;

    group = 0;
    initial = 0;
    type = SaveState_ScanRecordFlags();
    if (type < 0) {
        return -1;
    }
    if (type == 0) {
        return 0;
    }
    if (type == 3) {
        group = 1;
    } else if (type == 0x67) {
        group = 2;
    } else if (type > 0x64) {
        group = 3;
    } else {
        initial = 1;
    }
    AffineEffect_InitializeWork();
    if ((group == 0) || (group == 3)) {
        Menu_AppendResourceEntry(0x15);
    }
    if (group <= 1U) {
        Menu_AppendResourceEntry(0x16);
    }
    if ((group == 0) || (group == 3)) {
        Menu_AppendResourceEntry(0x17);
    }
    Menu_AppendResourceEntry(0x18);
    if ((*(s16 *)gTitleExtraOptionEnabled) != 0) {
        Menu_AppendResourceEntry(0x1D);
    }
    if (gTitleSendOptionEnabled != 0) {
        Menu_AppendResourceEntry(0x1E);
    }
    Menu_CenterResourceEntries(0x11, TYPE_MENU_WIDTH, 0);
    ret = Menu_RunResourceSelectionLoop(initial);
    Menu_EndResourceSelection();
    if (ret >= 0) {
        ret = Menu_SaveSlotActionByPosition[ret + (group * 6)];
    }
    return ret;
}

#if EDITION_INTERNATIONAL
#define MODE_LABEL_X     18
#define MODE_LABEL_RIGHT 144
#else
#define MODE_LABEL_X     8
#define MODE_LABEL_RIGHT 80
#endif

void Menu_DrawModeLabel(void)
{
    struct ResourceMenuWork *state = gMenuSelectWork;

    if (state->previous_mode != state->selection) {
        state->previous_mode = state->selection;
        UiWindow_ClearInteriorTiles(state->lower_window, 8, 40, MODE_LABEL_RIGHT, 80);

        if (state->selection != 1) {
            if (state->selection > 1)
                goto mode_other;
            if (state->selection != 0)
                goto mode_other;

            {
                s32 text = (s32)MsgPasswordLevel;

                UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 40);
                UiText_DrawCharacterAtOffset(text + 1, state->lower_window, MODE_LABEL_X, 48);
                UiText_DrawCharacterAtOffset(text + 2, state->lower_window, MODE_LABEL_X, 56);
                UiText_DrawCharacterAtOffset(text + 3, state->lower_window, MODE_LABEL_X, 64);
                text += 4;
                UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 72);
                goto done;
            }
        }

        {
            s32 text = (s32)MsgPasswordLevel;

            UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 40);
            UiText_DrawCharacterAtOffset(text + 1, state->lower_window, MODE_LABEL_X, 48);
            text += 2;
            UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 56);
            goto done;
        }

mode_other:
        {
            s32 text = (s32)MsgPasswordLevel;

            UiText_DrawCharacterAtOffset(text++, state->lower_window, MODE_LABEL_X, 40);
            UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 48);
        }
done:
;
    }
}

void RenderOutput_PrepareForRedraw(void *);
void UiText_DrawResource(s32 no, s32 work, s32 x, s32 y);

#if EDITION_INTERNATIONAL
#define PASSWORD_HELP_X 16
#else
#define PASSWORD_HELP_X 0
#endif

#if defined(TBS_EDITION_FR)

void Menu_DrawModeIndicator(void)
{
    struct ResourceMenuWork *state = gMenuSelectWork;

    if (state->previous_mode != state->selection) {
        if (state->selection == 0) {
            RenderOutput_ClearList(state->lower_window);
            UiText_DrawResource((s32)MsgPasswordTransferHelp, (s32)state->lower_window, 16, 4);
            UiText_DrawResource((s32)MsgPasswordTransferHelp + 1, (s32)state->lower_window, 16, 16);
        } else if (state->previous_mode == 0) {
            RenderOutput_ClearList(state->lower_window);
            UiText_DrawResource((s32)MsgCableTransferHelp, (s32)state->lower_window, 0, 4);
            UiText_DrawResource((s32)MsgCableTransferHelp + 1, (s32)state->lower_window, 0, 16);
            UiText_DrawResource((s32)MsgCableTransferHelp + 2, (s32)state->lower_window, 0, 28);
        }
        state->previous_mode = state->selection;
    }
}
#else
void Menu_DrawModeIndicator(void)
{
    struct ResourceMenuWork *state = gMenuSelectWork;
    s16 *shown = &state->previous_mode;
    s16 *current = &state->selection;

    if (*shown != *current) {
        *shown = *current;
        RenderOutput_PrepareForRedraw(state->lower_window);
        if (*current == 0) {
#if defined(TBS_EDITION_DE)
            UiText_DrawCharacterAtOffset((s32)MsgPasswordTransferHelp,
                state->lower_window, 32, 8);
            UiText_DrawCharacterAtOffset((s32)MsgPasswordTransferHelp + 1,
                state->lower_window, 32, 24);
#else
            UiText_DrawResource((s32)MsgPasswordTransferHelp,
                state->lower_window, PASSWORD_HELP_X, 4);
            UiText_DrawResource((s32)MsgPasswordTransferHelp + 1,
                state->lower_window, PASSWORD_HELP_X, 16);
#endif
        } else {
#if defined(TBS_EDITION_DE)
            UiText_DrawCharacterAtOffset((s32)MsgCableTransferHelp,
                state->lower_window, 0, 0);
            UiText_DrawCharacterAtOffset((s32)MsgCableTransferHelp + 1,
                state->lower_window, 0, 16);
            UiText_DrawCharacterAtOffset((s32)MsgCableTransferHelp + 2,
                state->lower_window, 0, 32);
#else
            UiText_DrawResource((s32)MsgCableTransferHelp,
                state->lower_window, 0, 4);
            UiText_DrawResource((s32)MsgCableTransferHelp + 1,
                state->lower_window, 0, 16);
            UiText_DrawResource((s32)MsgCableTransferHelp + 2,
                state->lower_window, 0, 28);
#endif
        }
    }
}
#endif
