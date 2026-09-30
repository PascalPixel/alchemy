#include "TYPES.H"
#include "SAVE_STATE.H"
extern u8 gTitleExtraOptionEnabled[];

struct MenuModeLabelState;
extern struct MenuModeLabelState *gMenuSelectWork;
extern u8 MsgPasswordLevel[];

s32 UiWindow_Create(s32, s32, s32, s32, s32);

void Menu_LayoutResourceEntries(s32 x, s32 y, s32 w, s32 h)
{
    u8 *state;
    s32 i;

    state = (u8 *)gMenuSelectWork;

    *(u16 *)(state + 144) = (u16)((u32)w + 2);
    *(u16 *)(state + 146) = (u16)h;
    *(u16 *)(state + 148) = (u16)y;

    for (i = 0; i < *(s16 *)(state + 142); i++) {
        *(u16 *)(state + i * 20 + 14) = (u16)((u32)y << 3);
        *(u16 *)(state + i * 20 + 12) = (u16)((u32)x << 3);
        x = (s32)((u32)x + 3);
    }

    *(s32 *)(state + 120) =
        UiWindow_Create(x, y, *(s16 *)(state + 144), 3, 2);
}

#include "TBS_EDITION.H"

extern s8 Menu_TopEntryPositionByCommand[];
extern s8 Menu_TopEntryCommandByPosition[];

s32 Party_SumDjinnCountsFar(s32);
void *AffineEffect_InitializeWork(void);
void Menu_AppendResourceEntry(s32 arg0);
void Menu_CenterResourceEntries(s32, s32, s32);
s32 Menu_RunResourceSelectionLoop(s32);
void Menu_EndResourceSelection(void);

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

void UiWindow_OpenMode1AndWaitFrame(void);
s32 Menu_SelectResource(s32, s32);
void UiWork_CloseAndRelease(void);

/* The Japanese list of four entries is centred two rows higher. */
#if defined(TBS_EDITION_JA)
#define ANIMATE_ENTRIES_ROW 5
#else
#define ANIMATE_ENTRIES_ROW 7
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

extern s32 SaveState_ScanRecordFlags(void);
extern s8 Menu_SaveSlotActionByPosition[];

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

struct MenuModeLabelState {
    u8 unknown_000[124];
    void *window;
    u8 unknown_080[12];
    s16 mode;
    u8 unknown_08e[8];
    s16 previous_mode;
};

extern void UiWindow_ClearInteriorTiles(void *, s32, s32, s32, s32);
extern void UiText_DrawCharacterAtOffset(s32, void *, s32, s32);

/* The Japanese mode labels start further left in a narrower cleared box. */
#if defined(TBS_EDITION_JA)
#define MODE_LABEL_X     8
#define MODE_LABEL_RIGHT 80
#else
#define MODE_LABEL_X     18
#define MODE_LABEL_RIGHT 144
#endif

void Menu_DrawModeLabel(void)
{
    struct MenuModeLabelState *state = gMenuSelectWork;

    if (state->previous_mode != state->mode) {
        state->previous_mode = state->mode;
        UiWindow_ClearInteriorTiles(state->window, 8, 40, MODE_LABEL_RIGHT, 80);

        if (state->mode != 1) {
            if (state->mode > 1)
                goto mode_other;
            if (state->mode != 0)
                goto mode_other;

            {
                s32 text = (s32)MsgPasswordLevel;

                UiText_DrawCharacterAtOffset(text, state->window, MODE_LABEL_X, 40);
                UiText_DrawCharacterAtOffset(text + 1, state->window, MODE_LABEL_X, 48);
                UiText_DrawCharacterAtOffset(text + 2, state->window, MODE_LABEL_X, 56);
                UiText_DrawCharacterAtOffset(text + 3, state->window, MODE_LABEL_X, 64);
                text += 4;
                UiText_DrawCharacterAtOffset(text, state->window, MODE_LABEL_X, 72);
                goto done;
            }
        }

        {
            s32 text = (s32)MsgPasswordLevel;

            UiText_DrawCharacterAtOffset(text, state->window, MODE_LABEL_X, 40);
            UiText_DrawCharacterAtOffset(text + 1, state->window, MODE_LABEL_X, 48);
            text += 2;
            UiText_DrawCharacterAtOffset(text, state->window, MODE_LABEL_X, 56);
            goto done;
        }

mode_other:
        {
            s32 text = (s32)MsgPasswordLevel;

            UiText_DrawCharacterAtOffset(text++, state->window, MODE_LABEL_X, 40);
            UiText_DrawCharacterAtOffset(text, state->window, MODE_LABEL_X, 48);
        }
done:
;
    }
}

void RenderOutput_PrepareForRedraw(void *);
void UiText_DrawResource(s32 no, s32 work, s32 x, s32 y);

extern u8 MsgPasswordTransferHelp;
extern u8 MsgCableTransferHelp;

/* The Japanese password help starts at the window edge. */
#if defined(TBS_EDITION_JA)
#define PASSWORD_HELP_X 0
#else
#define PASSWORD_HELP_X 16
#endif

#if defined(TBS_EDITION_FR)
void RenderOutput_ClearList(void *);

/* The French help clears its list in each branch and draws the cable help
   only when it follows the password help. */
void Menu_DrawModeIndicator(void)
{
    struct MenuModeLabelState *state = gMenuSelectWork;

    if (state->previous_mode != state->mode) {
        if (state->mode == 0) {
            RenderOutput_ClearList(state->window);
            UiText_DrawResource((s32)&MsgPasswordTransferHelp, (s32)state->window, 16, 4);
            UiText_DrawResource((s32)&MsgPasswordTransferHelp + 1, (s32)state->window, 16, 16);
        } else if (state->previous_mode == 0) {
            RenderOutput_ClearList(state->window);
            UiText_DrawResource((s32)&MsgCableTransferHelp, (s32)state->window, 0, 4);
            UiText_DrawResource((s32)&MsgCableTransferHelp + 1, (s32)state->window, 0, 16);
            UiText_DrawResource((s32)&MsgCableTransferHelp + 2, (s32)state->window, 0, 28);
        }
        state->previous_mode = state->mode;
    }
}
#else
void Menu_DrawModeIndicator(void)
{
    u8 *state = (u8 *)gMenuSelectWork;
    s16 *shown = (s16 *)(state + 150);
    s16 *current = (s16 *)(state + 140);

    /* 値が変わった時だけ表示を更新する。 */
    if (*shown != *current) {
        *shown = (u16)*current;
        RenderOutput_PrepareForRedraw(*(void **)(state + 124));
        /* The German help sits in whole character cells, a row apart. */
        if (*current == 0) {
#if defined(TBS_EDITION_DE)
            UiText_DrawCharacterAtOffset((s32)&MsgPasswordTransferHelp,
                *(void **)(state + 124), 32, 8);
            UiText_DrawCharacterAtOffset((s32)&MsgPasswordTransferHelp + 1,
                *(void **)(state + 124), 32, 24);
#else
            UiText_DrawResource((s32)&MsgPasswordTransferHelp,
                *(void **)(state + 124), PASSWORD_HELP_X, 4);
            UiText_DrawResource((s32)&MsgPasswordTransferHelp + 1,
                *(void **)(state + 124), PASSWORD_HELP_X, 16);
#endif
        } else {
#if defined(TBS_EDITION_DE)
            UiText_DrawCharacterAtOffset((s32)&MsgCableTransferHelp,
                *(void **)(state + 124), 0, 0);
            UiText_DrawCharacterAtOffset((s32)&MsgCableTransferHelp + 1,
                *(void **)(state + 124), 0, 16);
            UiText_DrawCharacterAtOffset((s32)&MsgCableTransferHelp + 2,
                *(void **)(state + 124), 0, 32);
#else
            UiText_DrawResource((s32)&MsgCableTransferHelp,
                *(void **)(state + 124), 0, 4);
            UiText_DrawResource((s32)&MsgCableTransferHelp + 1,
                *(void **)(state + 124), 0, 16);
            UiText_DrawResource((s32)&MsgCableTransferHelp + 2,
                *(void **)(state + 124), 0, 28);
#endif
        }
    }
}
#endif
