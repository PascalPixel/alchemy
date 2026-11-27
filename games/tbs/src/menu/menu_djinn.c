// The Djinn menu: its screen, the Djinn list, help, and choosing the Djinn to set, put on standby,
// give or trade. Part of menu.c.

#include "menu_internal.h"

// The editions outside Japanese widen the Djinn list's highlight and lay the stat preview out for
// fewer digits; in every edition but the Japanese a trade preview shows its second side in another
// mode.
#define DJINN_CURSOR_WIDTH        JAPANESE_OR(5, 6)

// DJINN_MENU_STATE.pairEntries before a Djinni is chosen: every element and number bit set, which
// no list entry has.
#define DJINN_ENTRY_NONE          (DJINN_ENTRY_ELEMENT | DJINN_ENTRY_INDEX)

// The tile of BG character block 1 the backdrop's digits are copied to.
#define DJINN_BACKDROP_DIGIT_TILE 128

// The palettes the Djinn menu takes over, kept in DJINN_MENU_BACKDROP.palette while it is open:
// from DJINN_BACKDROP_PALETTE, a highlighted row's element icons (the highlight's palette bit turns
// BASIC_COLORS_PALETTE into it, as it turns WINDOW_PALETTE into HIGHLIGHT_PALETTE), the element
// icons' own, the dimmed window colours a chooser puts behind it and the copy of the window colours
// they are darkened from.
#define DJINN_BACKDROP_PALETTE    4
#define DJINN_DIM_PALETTE         6
#define DJINN_WINDOW_COPY_PALETTE 7

// The mode of a trade preview's second side.
#define DJINN_SWAP_PREVIEW_MODE                                                                    \
    WHEN(DJINN_GIVE_PREVIEW_RECEIVER_AS_TRADE, DJINN_PREVIEW_TRADE, DJINN_PREVIEW_ADD)

// The stat preview's columns; an arrow stands at x (in tiles) or, in the international editions, at
// the pixel column iconX the preview sets.
#if LANGUAGE != LANGUAGE_JAPANESE
#    define DJINN_PREVIEW_DIGITS            3
#    define DJINN_PREVIEW_CURRENT_X         48
#    define DJINN_PREVIEW_MAX_X             80
#    define DJINN_PREVIEW_SLASH_X           72
#    define DJINN_PREVIEW_LUCK_X            56
#    define DJINN_PREVIEW_ARROW_X(x, iconX) (iconX)
#    define DJINN_PREVIEW_PP_X              88
#else
#    define DJINN_PREVIEW_DIGITS            4
#    define DJINN_PREVIEW_CURRENT_X         32
#    define DJINN_PREVIEW_MAX_X             72
#    define DJINN_PREVIEW_SLASH_X           64
#    define DJINN_PREVIEW_LUCK_X            48
#    define DJINN_PREVIEW_ARROW_X(x, iconX) ((x) * 8 + 64)
#    define DJINN_PREVIEW_PP_X              56
#endif

// The y of the preview's stat row `row` (0 to 5: HP, PP, Attack, Defense, Agility and Luck) in a
// window whose top is tile row y.
#define DJINN_PREVIEW_STAT_Y(y, row) ((y) * 8 + 56 + (row) * 8)
// The x of a changed stat's new value on that row, for a window whose left is tile column x.
#define DJINN_PREVIEW_VALUE_X(x)     ((x) * 8 + 72)

// The party's Djinn columns, one a member, 7 tiles wide, and the x of a column's figure.
#define DJINN_COLUMN_TILES           7
#define DJINN_COLUMN_WIDTH           (DJINN_COLUMN_TILES * 8)
#define DJINN_FIGURE_X(column)       ((column) * DJINN_COLUMN_WIDTH + 48)

// The cursor packed in DJINN_MENU_STATE.selection: column + row * 10.
#define DJINN_CURSOR_ROW_STRIDE      10

// Wraps a cursor index that has stepped one past either end of modulus entries: -1 becomes modulus
// - 1 and modulus becomes 0.
Sint32 mnWrapIndex(Sint32 value, Sint32 modulus)
{
    return (value + modulus) % modulus;
}

// Where the first party figure stands, over its Djinn column.
#define DJINN_MEMBER_FIGURE_X 32
#define DJINN_MEMBER_FIGURE_Y 61

// Places the four party figures over their Djinn columns, at screen y DJINN_MEMBER_FIGURE_Y plus
// yOffset. Returns that y.
Sint32 mnDjinnMenuPlacePartyFigures(Sint32 yOffset)
{
    DJINN_MENU_STATE *menu;
    Sint32            memberX;
    Sint32            memberIndex;
    menu     = MenuState;
    yOffset += DJINN_MEMBER_FIGURE_Y;
    memberX  = DJINN_MEMBER_FIGURE_X;
    for (memberIndex = 0; memberIndex < 4; memberIndex++)
    {
        menu->memberX[memberIndex]  = memberX;
        menu->memberY[memberIndex]  = yOffset;
        memberX                    += DJINN_COLUMN_WIDTH;
    }
    return yOffset;
}

// Runs the Djinn menu over the field: the field menu's Djinn screen (mnFieldMenuRun,
// FIELD_MENU_DJINN), and the Djinn tutorial, which the world map opens when Flint joins.
Sint32 mnDjinnMenuRun(void)
{
    DJINN_MENU_STATE    *menu;
    Uint32               savedMsgSpeed;
    DJINN_MENU_BACKDROP *backdrop;

    menu                            = syHeapSlotAllocate(HEAP_SLOT_MENU, MENU_STATE_SIZE);
    savedMsgSpeed                   = MainRoutineStateWork.msgSpeed;
    MainRoutineStateWork.msgSpeed   = MESSAGE_SPEED_FAST;
    ActorSystemStateWork->suspended = 1;
    mnWindowDrawBorderStub(0, 0, DISPLAY_TILE_WIDTH, DISPLAY_TILE_HEIGHT);
    sySynch(1);
    mnInitState(0);
    backdrop                = syHeapAllocateEWRAM(sizeof(*backdrop));
    menu->backdrop          = backdrop;
    backdrop->tutorialTimer = 0;
    backdrop->tutorialStep  = DJINN_TUTORIAL_OFF;
    // The tutorial's lesson, from the flags the world map sets for each.
    if (gmFlagGetStub(FLAG_DJINN_LESSON))
    {
        if (!gmFlagGetStub(FLAG_DJINN_LESSON_PART_A))
        {
            if (!gmFlagGetStub(FLAG_DJINN_LESSON_PART_B))
                backdrop->tutorialStep = DJINN_TUTORIAL_GIVE;
            else
                backdrop->tutorialStep = DJINN_TUTORIAL_POWERS;
        }
        else
        {
            if (!gmFlagGetStub(FLAG_DJINN_LESSON_PART_B))
                backdrop->tutorialStep = DJINN_TUTORIAL_RECOVERY;
            else
                backdrop->tutorialStep = DJINN_TUTORIAL_CLOSE;
        }
    }
    mnSuspendFieldEvents();
    mnTextUseExtraTilesStub(1);
    mnPageArrowsDrawStub(PAGE_ARROW_VRAM);
    menu->memberCount = gmPartyListMembersStub(menu->members);
    mnLoadDjinnArrows();
    mnItemMenuInit(0, 3, 0, 7);
    mnDjinnMenuPlacePartyFigures(0);
    mnPaletteLightenHighlight(HIGHLIGHT_PALETTE);
    menu->helpWindow        = mnWindowCreateStub(13, 0, 17, 5, WINDOW_FLAG_INSTANT);
    menu->pairEntries[0]    = DJINN_ENTRY_NONE;
    menu->selectedColumn[0] = 0;
    menu->selectedColumn[1] = 0;
    menu->selection[0]      = 0;
    menu->selection[1]      = 0;
    mnDjinnFiguresInitBottomRow((Sint32)menu->helpWindow, 0);
    mnDjinnMenuRunScreen();
    mnDjinnFiguresReleaseBottomRow();
    mnIconMenuReleaseArrows();
    sySynch(1);
    mnItemMenuClose();
    mnWindowDrawBorderStub(0, 0, DISPLAY_TILE_WIDTH, DISPLAY_TILE_HEIGHT);
    ActorSystemStateWork->suspended = 0;
    mnWindowMarkVisibleTextTilesStub();
    mnTextUseExtraTilesStub(0);
    syWordsCopy(BG_CHAR_BLOCK(1), backdrop->tiles, sizeof(backdrop->tiles));
    syWordsCopy((void *)&BG_PLTT_COLOR(DJINN_BACKDROP_PALETTE, 0),
                backdrop->palette,
                sizeof(backdrop->palette));
    sySynch(1);
    mnResumeFieldEvents();
    mnWindowEraseBorderRectStub(0, 0, DISPLAY_TILE_WIDTH, DISPLAY_TILE_HEIGHT);
    syHeapRelease(menu->backdrop);
    syHeapSlotRelease(HEAP_SLOT_MENU);
    fdPartyClearUnusableShortcutsStub();
    MainRoutineStateWork.msgSpeed = savedMsgSpeed;
    return 1;
}

// Finds the moved Djinni in the list and selects its new position.
#define DJINN_RESELECT_PAIR_ENTRY(menu, i)                                                         \
    do                                                                                             \
    {                                                                                              \
        Sint32 column;                                                                             \
        Sint32 found;                                                                              \
        Uint8  entry;                                                                              \
                                                                                                   \
        column = (Uint16)((Uint32)(menu)->selection[1] % DJINN_LIST_ROWS);                         \
        found  = 0;                                                                                \
        entry  = (menu)->pairEntries[0];                                                           \
        for ((i) = 0; (i) < (menu)->backdrop->list.counts[column]; (i)++)                          \
        {                                                                                          \
            if (entry == (Uint8)(menu)->backdrop->list.ids[column][(i)])                           \
            {                                                                                      \
                found = (i);                                                                       \
                break;                                                                             \
            }                                                                                      \
        }                                                                                          \
        (menu)->selection[0] = column + found * DJINN_LIST_ROWS;                                   \
    } while (0)

// The screen chooses an operation, optionally previews it, then commits it.
typedef enum DJINN_MENU_STEP
{
    // After a chooser: a negative result closes the screen, any other goes back to choosing a
    // Djinni.
    DJINN_STEP_HANDLE_RESULT      = 0,
    DJINN_STEP_RESERVED           = 1, // nothing sets it
    DJINN_STEP_SELECT_DJINN       = 2,
    DJINN_STEP_SELECT_ACTION      = 3, // what to do with the chosen Djinni
                                  // After a second choice: back to the Djinn, or on to a trade's
                                  // second Djinni or a gift's confirmation.
    DJINN_STEP_HANDLE_PAIR_RESULT = 4,
    DJINN_STEP_CONFIRM_SET        = 5,
    DJINN_STEP_CONFIRM_STANDBY    = 6,
    DJINN_STEP_CONFIRM_GIVE       = 7,
    DJINN_STEP_SELECT_SECOND      = 8, // the other Djinni of a trade
    DJINN_STEP_CONFIRM_TRADE      = 9,
    // the status of the chosen column's party member
    DJINN_STEP_SHOW_STATUS        = 10,
    DJINN_STEP_APPLY_SET          = 11,
    DJINN_STEP_APPLY_STANDBY      = 12,
    DJINN_STEP_APPLY_GIVE         = 13,
    DJINN_STEP_APPLY_TRADE        = 14,
    DJINN_STEP_SHOW_HELP          = 15
} DJINN_MENU_STEP;

// Chooses a Djinni, previews changes, then sets, stands by, gives or trades it.
Sint32 mnDjinnMenuRunScreen(void)
{
    DJINN_MENU_STATE *menu;
    WINDOW_SPR       *cursor;
    Sint32            result;
    Sint32            step;
    Sint32            menuResult;
    Bool              done;
    Sint32            listIndex;

    done                  = FALSE;
    result                = 0;
    menuResult            = 0;
    menu                  = MenuState;
    cursor                = menu->cursor;
    cursor->motion        = ICON_HIDDEN;
    cursor->animationStep = 0;
    mnOpenBackdropScreen();
    sySynch(1);
    step = DJINN_STEP_SELECT_DJINN;
    do
    {
        switch (step)
        {
            case DJINN_STEP_HANDLE_RESULT:
                if (result < 0)
                {
                    menuResult = -1;
                    done       = TRUE;
                }
                step = DJINN_STEP_SELECT_DJINN;
                break;
            case DJINN_STEP_RESERVED: break;
            case DJINN_STEP_SELECT_DJINN:
                mnDjinnMenuPlacePartyFigures(0);
                mnDjinnFiguresSetSlotPos(1, 0, MENU_FIGURE_HIDDEN_Y, 0);
                result = mnDjinnMenuSelectDjinn(DJINN_SELECT_DJINNI);
                step   = DJINN_STEP_SHOW_HELP;
                if (result == DJINN_RESULT_HELP)
                    break;
                step = DJINN_STEP_HANDLE_RESULT;
                if (result < 0)
                    break;
                menu->selection[1] = menu->selectedColumn[0];
                step               = DJINN_STEP_SHOW_STATUS;
                if (result == DJINN_RESULT_STATUS)
                    break;
                step = DJINN_STEP_SELECT_ACTION;
                break;
            case DJINN_STEP_SHOW_STATUS:
                menu->member         = menu->members[menu->selectedColumn[0]];
                menu->shownMember[0] = menu->members[menu->selectedColumn[0]];
                result               = mnDjinnMenuShowMemberStatus();
                if (result == DJINN_RESULT_START)
                    done = TRUE;
                step = DJINN_STEP_SELECT_DJINN;
                break;
            case DJINN_STEP_SHOW_HELP:
                result = mnDjinnMenuShowHelp();
                if (result == DJINN_RESULT_START)
                    done = TRUE;
                step = DJINN_STEP_SELECT_DJINN;
                break;
            case DJINN_STEP_SELECT_SECOND:
                step = DJINN_STEP_HANDLE_RESULT;
                if (menu->pairCount == 0)
                    break;
                result = mnDjinnMenuSelectDjinn(DJINN_SELECT_ACTION);
                if (result == DJINN_RESULT_START)
                    done = TRUE;
                step = DJINN_STEP_HANDLE_PAIR_RESULT;
                if (result < 0)
                    break;
                step = DJINN_STEP_CONFIRM_TRADE;
                break;
            // Choose what to do with it. The figures go up 8 pixels while this chooser runs.
            case DJINN_STEP_SELECT_ACTION:
                mnDjinnMenuDrawElementList(&menu->backdrop->list);
                mnDjinnMenuPlacePartyFigures(-8);
                menu->member         = menu->members[menu->selectedColumn[0]];
                menu->shownMember[0] = menu->members[menu->selectedColumn[0]];
                mnDjinnFiguresSetSlotPos(0, DJINN_FIGURE_X(menu->selectedColumn[0]), 54, 0);
                result = mnDjinnMenuSelectDjinn(DJINN_SELECT_ACTION);
                for (listIndex = 0; listIndex < menu->memberCount; listIndex++)
                    menu->memberY[listIndex] += 8;
                if (result == DJINN_RESULT_START)
                    done = TRUE;
                if (result < 0)
                {
                    step = DJINN_STEP_SELECT_DJINN;
                    break;
                }
                if (result == DJINN_RESULT_GIVE || result == DJINN_RESULT_TRADE
                    || result == DJINN_RESULT_GIVE_NOW || result == DJINN_RESULT_TRADE_NOW)
                    menu->shownMember[1] = menu->members[menu->selectedColumn[1]];
                if (result < 0)
                    step = DJINN_STEP_SELECT_DJINN;
                else if (result == DJINN_RESULT_SET)
                    step = DJINN_STEP_CONFIRM_SET;
                else if (result == DJINN_RESULT_STANDBY)
                    step = DJINN_STEP_CONFIRM_STANDBY;
                else if (result == DJINN_RESULT_GIVE)
                {
                    menu->pending = DJINN_PENDING_CONFIRM;
                    step          = DJINN_STEP_CONFIRM_GIVE;
                }
                else if (result == DJINN_RESULT_TRADE)
                {
                    menu->pending = DJINN_PENDING_CONFIRM;
                    step          = DJINN_STEP_CONFIRM_TRADE;
                }
                else if (result == DJINN_RESULT_SET_NOW)
                    step = DJINN_STEP_APPLY_SET;
                else if (result == DJINN_RESULT_STANDBY_NOW)
                    step = DJINN_STEP_APPLY_STANDBY;
                else if (result == DJINN_RESULT_GIVE_NOW)
                {
                    menu->pending = DJINN_PENDING_CONFIRM;
                    step          = DJINN_STEP_APPLY_GIVE;
                }
                else if (result == DJINN_RESULT_TRADE_NOW)
                {
                    menu->pending = DJINN_PENDING_CONFIRM;
                    step          = DJINN_STEP_APPLY_TRADE;
                }
                break;
            case DJINN_STEP_CONFIRM_GIVE:
                result = mnDjinnMenuRunPreview(DJINN_COMPARE_GIVE);
                if (result == DJINN_RESULT_START)
                    done = TRUE;
                step = DJINN_STEP_SELECT_ACTION;
                if (result < 0)
                    break;
                // Confirmed: apply it at once. Fall through.
            case DJINN_STEP_APPLY_GIVE:
                sySoundPlayStub(SE_RECOVERY);
                result = gmDjinnTransferStub(menu->shownMember[0],
                                             menu->pairElement[0],
                                             menu->pairIndex[0],
                                             menu->shownMember[1]);
                gmUnitCalculateStub(menu->shownMember[0]);
                gmUnitCalculateStub(menu->shownMember[1]);
                menu->cursor->motion = ICON_HIDDEN;
                mnWindowSpriteClearListStub(menu->listWindow);
                mnDjinnMenuBuildLists(menu->backdrop);
                DJINN_RESELECT_PAIR_ENTRY(menu, listIndex);
                menu->cursor->motion = ICON_SHOWN;
                step                 = DJINN_STEP_HANDLE_RESULT;
                break;
            case DJINN_STEP_CONFIRM_STANDBY:
                result = mnDjinnMenuRunPreview(DJINN_COMPARE_STANDBY);
                if (result == DJINN_RESULT_START)
                    done = TRUE;
                step = DJINN_STEP_SELECT_ACTION;
                if (result < 0)
                    break;
                // Confirmed: apply it at once. Fall through.
            case DJINN_STEP_APPLY_STANDBY:
                sySoundPlayStub(SE_NOTE_HIGH);
                gmDjinnPutOnStandbyStub(
                    menu->shownMember[0], menu->pairElement[0], menu->pairIndex[0]);
                result = (Sint32)gmDjinnRecoveryAddStandbyStub(
                    menu->shownMember[0], menu->pairElement[0], menu->pairIndex[0]);
                gmUnitCalculateStub(menu->shownMember[0]);
                menu->cursor->motion = ICON_HIDDEN;
                mnWindowSpriteClearListStub(menu->listWindow);
                menu->cursor->motion = ICON_SHOWN;
                step                 = DJINN_STEP_SELECT_DJINN;
                break;
            case DJINN_STEP_CONFIRM_TRADE:
                result = mnDjinnMenuRunPreview(DJINN_COMPARE_TRADE);
                if (result == DJINN_RESULT_START)
                    done = TRUE;
                step = DJINN_STEP_SELECT_ACTION;
                if (result < 0)
                    break;
                // Confirmed: apply it at once. Fall through.
            case DJINN_STEP_APPLY_TRADE:
                sySoundPlayStub(SE_RECOVERY);
                gmDjinnTransferStub(menu->shownMember[0],
                                    menu->pairElement[0],
                                    menu->pairIndex[0],
                                    menu->shownMember[1]);
                result = gmDjinnTransferStub(menu->shownMember[1],
                                             menu->pairElement[1],
                                             menu->pairIndex[1],
                                             menu->shownMember[0]);
                gmUnitCalculateStub(menu->shownMember[0]);
                gmUnitCalculateStub(menu->shownMember[1]);
                mnDjinnMenuBuildLists(menu->backdrop);
                DJINN_RESELECT_PAIR_ENTRY(menu, listIndex);
                menu->cursor->motion = ICON_SHOWN;
                step                 = DJINN_STEP_SELECT_DJINN;
                break;
            case DJINN_STEP_HANDLE_PAIR_RESULT:
                if (result == DJINN_RESULT_BACK)
                {
                    menuResult = result;
                    step       = DJINN_STEP_SELECT_DJINN;
                }
                else if (menu->pending & DJINN_PENDING_SECOND)
                    step = DJINN_STEP_SELECT_SECOND;
                else if (menu->pending & DJINN_PENDING_CONFIRM)
                    step = DJINN_STEP_CONFIRM_GIVE;
                break;
            case DJINN_STEP_CONFIRM_SET:
                result = mnDjinnMenuRunPreview(DJINN_COMPARE_SET);
                if (result == DJINN_RESULT_START)
                    done = TRUE;
                step = DJINN_STEP_SELECT_ACTION;
                if (result < 0)
                    break;
                // Confirmed: apply it at once. Fall through.
            case DJINN_STEP_APPLY_SET:
                sySoundPlayStub(SE_DJINNI_SET);
                gmDjinnSetStub(menu->shownMember[0], menu->pairElement[0], menu->pairIndex[0]);
                result = gmDjinnRecoveryRemoveStub(
                    menu->shownMember[0], menu->pairElement[0], menu->pairIndex[0]);
                gmUnitCalculateStub(menu->shownMember[0]);
                menu->cursor->motion = ICON_HIDDEN;
                mnWindowSpriteClearListStub(menu->listWindow);
                menu->cursor->motion = ICON_SHOWN;
                step                 = DJINN_STEP_SELECT_DJINN;
                break;
            default: done = TRUE; break;
        }
    } while (done == 0);
    return menuResult;
}

// Writes copies of WINDOW_PALETTE, BASIC_COLORS_PALETTE and DJINN_WINDOW_COPY_PALETTE, each
// brightnessDelta lighter in every channel (clamped to 0 to RGB_LEVEL_MAX), into the palette before
// each: HIGHLIGHT_PALETTE, DJINN_BACKDROP_PALETTE and DJINN_DIM_PALETTE. The last copy is 12 levels
// darker instead.
void mnDjinnMenuAdjustPalettes(Sint32 brightnessDelta)
{
    Sint32 copyIndex;
    Sint32 paletteNumber;
    Sint32 colorIndex;
    Sint32 paletteIndex;

    paletteNumber = WINDOW_PALETTE;
    for (copyIndex = 0; copyIndex <= 2; copyIndex++)
    {
        for (colorIndex = 0; colorIndex < PALETTE_COLORS; colorIndex++)
        {
            Uint32 color;
            Sint32 blue;
            Sint32 green;
            Sint32 red;
            Sint32 adjustedColor;

            paletteIndex  = paletteNumber * PALETTE_COLORS + colorIndex;
            color         = ((Uint16 *)BG_PLTT)[paletteIndex];
            blue          = RGB_BLUE(color);
            green         = RGB_GREEN(color);
            red           = RGB_RED(color);
            blue         += brightnessDelta;
            green        += brightnessDelta;
            red          += brightnessDelta;
            if (blue > RGB_LEVEL_MAX)
                blue = RGB_LEVEL_MAX;
            if (green > RGB_LEVEL_MAX)
                green = RGB_LEVEL_MAX;
            if (red > RGB_LEVEL_MAX)
                red = RGB_LEVEL_MAX;
            if (blue < 0)
                blue = 0;
            if (green < 0)
                green = 0;
            if (red < 0)
                red = 0;
            adjustedColor                                      = RGB(red, green, blue);
            ((Sint16 *)BG_PLTT - PALETTE_COLORS)[paletteIndex] = adjustedColor;
        }
        if (copyIndex == 0)
        {
            paletteNumber = BASIC_COLORS_PALETTE;
        }
        else
        {
            paletteNumber   = DJINN_WINDOW_COPY_PALETTE;
            brightnessDelta = -12;
        }
    }
}

// mnPsynergyListDifference's marks on an action id: Psynergy the change gives, drawn in
// TEXT_COLOR_YELLOW, and Psynergy it takes away, in TEXT_COLOR_RED.
#define DJINN_PREVIEW_GAINED 0x8000
#define DJINN_PREVIEW_LOST   0x4000

// Draws the backdrop behind the Djinn menu's list: its window, tiles and palettes.
MENU_BACKDROP_RESULT mnOpenBackdropScreen(void)
{
    // UB: the implicit int call conflicts with DrawElementList's void definition. The sole caller
    // ignores this wrapper's result.
    DJINN_MENU_STATE    *menu     = MenuState;
    DJINN_MENU_BACKDROP *backdrop = menu->backdrop;

    mnWindowUpdateOrCreate(&menu->listWindow, 0, 5, 30, 15, WINDOW_FLAG_INSTANT);
    sySynch(1);
    syWordsCopy(backdrop->tiles, BG_CHAR_BLOCK(1), sizeof(backdrop->tiles));
    syWordsCopy(backdrop->palette,
                (void *)&BG_PLTT_COLOR(DJINN_BACKDROP_PALETTE, 0),
                sizeof(backdrop->palette));
    syWordsFill(BG_CHAR_BLOCK(1), sizeof(backdrop->tiles), PIXELS_4BPP(3));
    // Every colour of the backdrop's palettes RGB(21, 10, 21), two to a word.
    syWordsFill((void *)&BG_PLTT_COLOR(DJINN_BACKDROP_PALETTE, 0),
                sizeof(backdrop->palette),
                RGB(21, 10, 21) << 16 | RGB(21, 10, 21));
    mnGraphicsExpandVRAMTilesByColorTableStub(
        (Uint16 *)(BG_CHAR_BLOCK(1) + DJINN_BACKDROP_DIGIT_TILE * TILE_SIZE_4BPP));
    syWordsCopy(BG_CHAR_BLOCK(1) + DJINN_CHANGE_ARROW_TILE * TILE_SIZE_4BPP,
                MenuChangeArrowTile,
                TILE_SIZE_4BPP);
    syDMAXCopy(syPaletteGetBasicSpriteColors(),
               (void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
               DMA_COPY_HALFWORDS(PALETTE_COLORS),
               REG_DMA3);
    BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
        BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
    syDMAXCopy((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0),
               (void *)&BG_PLTT_COLOR(DJINN_WINDOW_COPY_PALETTE, 0),
               DMA_COPY_HALFWORDS(PALETTE_COLORS),
               REG_DMA3);
    mnDjinnMenuAdjustPalettes(8);
    BG_PLTT_COLOR(DJINN_WINDOW_COPY_PALETTE, WINDOW_BACKGROUND_COLOR) =
        BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
    BG_PLTT_COLOR(DJINN_DIM_PALETTE, WINDOW_BACKGROUND_COLOR) =
        BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
#if FIX_FUNCTION_SIGNATURES
    mnDjinnMenuDrawElementList(&backdrop->list);
#else
    return mnDjinnMenuDrawElementList(&backdrop->list);
#endif
}

// Lists the actions after a preview, marking gains, then appends the losses. The two output counts
// distinguish gained and lost actions.
Sint32 mnPsynergyListDifference(PSYNERGY_ENTRY *before,
                                PSYNERGY_ENTRY *after,
                                Uint16         *actionsOut,
                                Sint32         *gainedCountOut,
                                Sint32         *lostCountOut)
{
    Sint32 actionIndex;
    Sint32 otherIndex;
    Sint32 actionCount;
    Sint32 gainedCount;
    Sint32 lostCount;

    actionCount = 0;
    gainedCount = 0;
    lostCount   = 0;
    for (actionIndex = 0; actionIndex < PSYNERGY_SLOTS && after[actionIndex].encodedAction != 0;
         actionIndex++)
    {
        actionsOut[actionCount] = after[actionIndex].encodedAction & PSYNERGY_ABILITY_MASK;
        actionCount++;
        for (otherIndex = 0; otherIndex < PSYNERGY_SLOTS; otherIndex++)
        {
            if (((after[actionIndex].encodedAction ^ before[otherIndex].encodedAction)
                 & PSYNERGY_ABILITY_MASK)
                == 0)
                break;
        }
        if (otherIndex == PSYNERGY_SLOTS)
        {
            gainedCount++;
            actionsOut[actionCount - 1] |= DJINN_PREVIEW_GAINED;
        }
    }
    for (actionIndex = 0; actionIndex < PSYNERGY_SLOTS && before[actionIndex].encodedAction != 0;
         actionIndex++)
    {
        for (otherIndex = 0; otherIndex < PSYNERGY_SLOTS; otherIndex++)
        {
            if (((before[actionIndex].encodedAction ^ after[otherIndex].encodedAction)
                 & PSYNERGY_ABILITY_MASK)
                == 0)
                break;
        }
        if (otherIndex == PSYNERGY_SLOTS)
        {
            lostCount++;
            actionsOut[actionCount] =
                (before[actionIndex].encodedAction & PSYNERGY_ABILITY_MASK) | DJINN_PREVIEW_LOST;
            actionCount++;
        }
    }
    *gainedCountOut = gainedCount;
    *lostCountOut   = lostCount;
    return actionCount;
}

// Rebuilds the Djinn lists in table: each party member's packed Djinn entries and their count.
Sint32 mnDjinnMenuBuildLists(void *table)
{
    DJINN_MENU_STATE *menu;
    Sint32            memberIndex;
    Uint16            (*entries)[DJINN_LIST_ROWS];
    Uint16           *members;
    Sint8            *counts;
    Sint16            entryCount;
    Sint32            memberCount;

    menu        = MenuState;
    memberIndex = 0;
    if (memberIndex < menu->memberCount)
    {
        counts  = ((DJINN_LIST_TABLE *)table)->counts;
        members = menu->members;
        entries = ((DJINN_LIST_TABLE *)table)->ids;
        do
        {
            entryCount   = mnDjinnListUnitEntries(*entries, *members, -1);
            memberCount  = menu->memberCount;
            memberIndex += 1;
            *counts      = entryCount;
            members     += 1;
            counts      += 1;
            entries++;
        } while (memberIndex < memberCount);
    }
#if FIX_FUNCTION_SIGNATURES
    return 0;
#endif
}

// Draws each party member's Djinn column, including set and recovering entries. Rebuilds table's
// lists first.
void mnDjinnMenuDrawElementList(DJINN_LIST_TABLE *table)
{
    DJINN_MENU_STATE *menu;
    Sint32            listIndex;
    Sint32            row;
    Sint32            element;
    WIN_STATE        *state;
    Sint32            lineIndex;
    Sint32            canChangeState;
    Uint16           *entries;

    menu                     = MenuState;
    state                    = HEAP_SLOT_BLOCK(HEAP_SLOT_WINDOW);
    state->holdTilemapUpload = 1;
    listIndex                = 0;
    if (menu->memberCount != 0)
    {
        do
        {
            table->counts[listIndex] =
                mnDjinnListUnitEntries(table->ids[listIndex], menu->members[listIndex], -1);
            listIndex++;
        } while (listIndex < menu->memberCount);
    }
    mnWindowSpriteRedrawSavedRectStub(menu->listWindow);
    mnTextDrawMessageAsTilesStub(MsgUnleashEffect, menu->listWindow, 0, 80);
    for (row = 0; row < menu->memberCount; row++)
    {
        entries   = table->ids[row];
        lineIndex = 0;
        for (element = 0; element < DJINN_ELEMENTS; element++)
        {
            for (listIndex = 0; listIndex < table->counts[row]; listIndex++)
            {
                if (element != DJINN_ELEMENT(entries[listIndex]))
                    continue;
                // TEXT_COLOR_RED when not set; TEXT_COLOR_YELLOW over it when it is neither set nor
                // ready to set, that is recovering (or held by a summon).
                if ((entries[listIndex] & DJINN_ENTRY_IS_SET) == 0)
                    mnTextSetColorStub(TEXT_COLOR_RED);
                canChangeState = 0;
                if (gmDjinnCanSetStub(DJINN_MEMBER(entries[listIndex]),
                                      DJINN_ELEMENT(entries[listIndex]),
                                      DJINN_NUMBER(entries[listIndex]))
                    || gmDjinnIsSetStub(DJINN_MEMBER(entries[listIndex]),
                                        DJINN_ELEMENT(entries[listIndex]),
                                        DJINN_NUMBER(entries[listIndex])))
                    canChangeState = 1;
                if (!canChangeState)
                    mnTextSetColorStub(TEXT_COLOR_YELLOW);
                mnWindowSetTilemapEntryStub(menu->listWindow,
                                            DJINN_ELEMENT(entries[listIndex]) + DJINN_ELEMENT_TILE,
                                            row * DJINN_COLUMN_TILES + 1,
                                            lineIndex + 2,
                                            TILEMAP_ENTRY_PLAIN);
                mnTextDrawMessageAsTilesStub(
                    MsgDjinnName + DJINN_ELEMENT(entries[listIndex]) * DJINN_PER_ELEMENT
                        + DJINN_NUMBER(entries[listIndex]),
                    menu->listWindow,
                    row * DJINN_COLUMN_WIDTH + 16,
                    lineIndex * 8 + 16);
                lineIndex++;
                mnTextSetColorStub(TEXT_COLOR_NORMAL);
            }
        }
    }
    mnWindowDrawDividerLineStub(menu->listWindow, 0, 10, 28, 10);
    WINDOW_STATE->dirty      = 1;
    state->holdTilemapUpload = 0;
}

// Sets the palette of a rectangle of window's interior, (x, y) and its size in tiles from the
// interior's corner, through mnWindowHighlightRectStub.
Sint32 mnWindowSetInteriorPaletteBitRect(
    const WINDOW *window, Sint32 x, Sint32 y, Sint32 width, Sint32 height, Sint32 palette)
{
    mnWindowHighlightRectStub(window->x + x + 1, window->y + y + 1, width, height, palette);
#if FIX_FUNCTION_SIGNATURES
    return 0;
#endif
}

// Moves each tile of a screen rectangle (in tiles) that shows WINDOW_PALETTE to palette, and marks
// each row changed for upload.
void mnWindowSetRectPalette(Sint32 x, Sint32 y, Sint32 width, Sint32 height, Sint32 palette)
{
    WIN_STATE *state;
    Uint16    *tileCursor;
    Sint32     column;
    Uint32     tile;
    Uint32     masked;

    state   = WINDOW_STATE;
    palette = BG_TILE_PALETTE(palette);
    if (x < 0)
    {
        width += x;
        x      = 0;
    }
    if (x + width > DISPLAY_TILE_WIDTH - 1)
        width = DISPLAY_TILE_WIDTH - x;
    if (y < 0)
    {
        height += y;
        y       = 0;
    }
    // BUG: tests the bottom against the screen's width but clips to its rows, so a rectangle ending
    // in rows 20 to 28 is kept whole and recolours past the 32 by 20 tilemap.
#if FIX_RECT_BOTTOM_CLIP
    if (y + height > DISPLAY_TILE_HEIGHT - 1)
#else
    if (y + height > DISPLAY_TILE_WIDTH - 1)
#endif
        height = DISPLAY_TILE_HEIGHT - y;

    if (width > 0 && height > 0)
    {
        do
        {
            tileCursor = &((Uint16 (*)[BG_SCREEN_WIDTH])state->tilemap)[y][x];
            for (column = width; column != 0; column--)
            {
                tile = *tileCursor;
                if (BG_TILE_PALETTE_OF(tile) == WINDOW_PALETTE)
                {
                    masked      = tile & ~BG_TILE_PALETTE(0xf);
                    *tileCursor = masked | palette;
                }
                tileCursor++;
            }
            mnWindowMarkTilemapRow(state, y);
            height--;
            y++;
        } while (height != 0);
    }
}

// mnWindowSetRectPalette on a rectangle of window's interior, (x, y) and its size in tiles from the
// interior's corner.
void mnWindowSetInteriorRectPalette(
    void *window, Sint32 x, Sint32 y, Sint32 width, Sint32 height, Sint32 palette)
{
    mnWindowSetRectPalette(
        ((WINDOW *)window)->x + x + 1, ((WINDOW *)window)->y + y + 1, width, height, palette);
}

// The help's topics: Djinn, Set, Standby, Summon, Recovery, Class and Psynergy.
#define DJINN_HELP_TOPICS 7

// The Djinn menu's help, opened by Select on the first chooser: the list of topics with the chosen
// one's text typed beside it.
Sint32 mnDjinnMenuShowHelp(void)
{
    WINDOW           *helpWindow;
    WINDOW           *statusWindow;
    DJINN_MENU_STATE *menu;
    WIN_STATE        *windowState;
    Sint32            result;
    Sint32            prev;
    Sint32            selection;
    WINDOW           *list;
    MESSAGE          *typing;
    Sint32            msg;
    Sint32            helpIndex;
    Sint32            listMsg;

    menu        = MenuState;
    windowState = HEAP_SLOT_BLOCK(HEAP_SLOT_WINDOW);
    result      = 0;
    prev        = 0;
    selection   = 0;
    mnWindowSpriteClearListStub(menu->listWindow);
    sySynch(1);
    mnWindowSpritePrepareForRedrawStub(menu->helpWindow);
    msg = MsgDjinnInfoPrompt;
    mnTextDrawMessageAsTilesStub(msg, menu->helpWindow, 0, 0);
    msg++;
    mnTextDrawMessageAsTilesStub(msg, menu->helpWindow, 0, 16);
    // Dim the party window and the Djinn list behind the help.
    mnWindowSetRectPalette(1, 1, 11, 3, DJINN_DIM_PALETTE);
    mnWindowSetInteriorRectPalette((void *)menu->listWindow, 0, 0, 28, 10, DJINN_DIM_PALETTE);
    list         = mnWindowCreateStub(0, 9, 8, 10, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
    helpWindow   = mnWindowCreateStub(8, 12, 22, 7, WINDOW_FLAG_INSTANT);
    statusWindow = mnWindowCreateStub(8, 9, 22, 3, WINDOW_FLAG_INSTANT);
    mnWindowMarkVisibleTextTilesStub();
    for (helpIndex = 0, listMsg = MsgDjinnInfoTopic; helpIndex < DJINN_HELP_TOPICS; helpIndex++)
        mnTextDrawMessageAsTilesStub(helpIndex + listMsg, list, 0, helpIndex * 8);
    do
    {
        mnWindowSpritePrepareForRedrawStub(statusWindow);
        mnTextDrawMessageStub(selection + MsgDjinnInfoTopic, statusWindow, 0, 0);
        typing = (MESSAGE *)mnMessageOpenDecodedStub(helpWindow, selection + MsgDjinnInfoText);
        mnWindowSetInteriorPaletteBitRect(list, 0, prev, 6, 1, WINDOW_PALETTE);
        mnWindowSetInteriorPaletteBitRect(list, 0, selection, 6, 1, HIGHLIGHT_PALETTE);
        prev = selection;
        for (;;)
        {
            mnCursorPos(-12, (list->y + selection) * 8 + 8);
            sySynch(1);
            if (PerRepeat & (KEY_RIGHT | KEY_DOWN))
            {
                selection++;
                selection = mnWrapIndex(selection, DJINN_HELP_TOPICS);
                sySoundPlayStub(SE_MENU_CURSOR_MOVE);
                break;
            }
            if (PerRepeat & (KEY_LEFT | KEY_UP))
            {
                selection--;
                selection = mnWrapIndex(selection, DJINN_HELP_TOPICS);
                sySoundPlayStub(SE_MENU_CURSOR_MOVE);
                break;
            }
            else if (PerPush & KEY_START)
            {
                sySoundPlayStub(SE_MENU_CANCEL);
                result = DJINN_RESULT_START;
                break;
            }
            else if (PerPush & (KEY_B | KEY_SELECT))
            {
                sySoundPlayStub(SE_MENU_CANCEL);
                result = DJINN_RESULT_BACK;
                break;
            }
            else if (PerPush & KEY_A)
            {
                if (mnMessageAllTypedStub())
                {
                    selection++;
                    selection = mnWrapIndex(selection, DJINN_HELP_TOPICS);
                    sySoundPlayStub(SE_MENU_CONFIRM);
                    break;
                }
                sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            }
        }
        // Before the next topic: free the page arrow's VRAM slot the text took, and stop its
        // typing.
        if (windowState->glyphResource != NO_GLYPH_RESOURCE)
        {
            syVRAMCacheFreeEntry(windowState->glyphResource);
            windowState->glyphResource = NO_GLYPH_RESOURCE;
        }
        WINDOW_STATE->pageArrowShown = 0;
        mnWindowSpritePrepareForRedrawStub(helpWindow);
        {
            WINDOW *window    = typing->window;

            window->zoomStage = 0;
            window->duration  = 0;
            window->state     = 0;
        }
        typing->window = NULL;
    } while (result == 0);
    WINDOW_STATE->holdTilemapUpload = 1;
    mnWindowSpriteClearListStub(statusWindow);
    mnWindowSpriteClearListStub(helpWindow);
    sySynch(1);
    mnWindowCloseStub(statusWindow, 1);
    mnWindowCloseStub(list, 1);
    mnWindowCloseStub(helpWindow, 1);
    mnWindowMarkVisibleTextTilesStub();
    if (result == DJINN_RESULT_START)
    {
        mnWindowSpritePrepareForRedrawStub(menu->helpWindow);
        mnWindowSpritePrepareForRedrawStub(menu->listWindow);
        mnWindowSpritePrepareForRedrawStub(menu->statusWindow);
        WINDOW_STATE->holdTilemapUpload = 0;
    }
    sySetEvent(mnPlacePartyFigures, EVENT_PASS_TICK);
    return result;
}

// DJINN_ENTRY_IS_SET read as a signed halfword, for entries built in Sint32.
#define DJINN_ENTRY_SET ((Sint16)DJINN_ENTRY_IS_SET)

// The flags mnDjinnMenuSelectDjinn keeps for each party member's column.
typedef enum DJINN_COLUMN_FLAG
{
    DJINN_COLUMN_ON_HEADER        = 1, // the cursor is on the column's header
    DJINN_COLUMN_HEADER_CHOOSABLE = 2, // its header can be chosen
    DJINN_COLUMN_EMPTY            = 4  // no Djinni to choose
} DJINN_COLUMN_FLAG;

// What mnDjinnMenuSelectDjinn redraws: everything, or after a cursor move, which also makes the
// Djinni figure grow in again.
typedef enum DJINN_REDRAW
{
    DJINN_REDRAW_ALL   = 1,
    DJINN_REDRAW_MOVED = 2
} DJINN_REDRAW;

// The Djinn list's chooser. DJINN_SELECT_DJINNI picks a Djinni in any party member's column: R sets
// it or puts it on standby at once, R held with Select does so for every Djinni, L shows the
// member's status and Select the help. DJINN_SELECT_ACTION picks what to do with it, by the column
// chosen. While the tutorial runs, its steps press the buttons.
Sint32 mnDjinnMenuSelectDjinn(Sint32 mode)
{
    Sint8                columnFlags[DJINN_LIST_MEMBERS];
    Uint8                requiresTrade[16];
    DJINN_MENU_STATE    *menu     = MenuState;
    DJINN_MENU_BACKDROP *backdrop = menu->backdrop;
    UNIT                *unit;
    WINDOW              *window;
    Sint32               redraw = DJINN_REDRAW_ALL;
    Uint32               refresh;
    Sint32               i;
    Sint32               j;
    Sint32               selectedRow;
    Sint32               memberColumn = (Uint16)(menu->selection[mode] % DJINN_CURSOR_ROW_STRIDE);
    Sint32               djinnRow     = (Uint16)(menu->selection[mode] / DJINN_CURSOR_ROW_STRIDE);
    Sint32               savedRow     = 0;
    Uint32               djinn        = 0;
    Bool                 groupMode    = FALSE;
    Sint32               setAll       = 0;
    Sint32               result;
    Sint32               msg;
    Uint32               pressed;
    Sint32               count;
    Uint32               repeat;
    WINDOW              *msgWindow;
    Sint32               tutorialStep;
    Sint8               *status;

    // Each column's DJINN_COLUMN_ flags.
    selectedRow = -1;
    status      = columnFlags;
    for (i = 0; i < DJINN_LIST_MEMBERS; i++)
        status[i] = 0;
    if (mode == DJINN_SELECT_DJINNI)
    {
        mnDjinnMenuDrawElementList(&backdrop->list);
        for (i = 0; i < menu->memberCount; i++)
        {
            if (backdrop->list.counts[i] == 0)
                status[i] = DJINN_COLUMN_EMPTY;
        }
        savedRow = djinnRow;
        for (i = 0; i < menu->memberCount; i++)
        {
            if (status[memberColumn] == DJINN_COLUMN_EMPTY)
            {
                memberColumn++;
                memberColumn = mnWrapIndex(memberColumn, menu->memberCount);
            }
        }
    }
    else
    {
        mnDjinnMarkTradeOnlyRecipients((Sint8 *)requiresTrade, menu->selectedColumn[0]);
        for (i = 0; i < menu->memberCount; i++)
        {
            if (i == menu->selectedColumn[0])
            {
                columnFlags[i] =
                    DJINN_COLUMN_EMPTY | DJINN_COLUMN_HEADER_CHOOSABLE | DJINN_COLUMN_ON_HEADER;
            }
            else if (requiresTrade[i] != 0)
            {
                columnFlags[i] = 0;
            }
            else
            {
                columnFlags[i] = DJINN_COLUMN_HEADER_CHOOSABLE | DJINN_COLUMN_ON_HEADER;
                if (backdrop->list.counts[i] == 0)
                    status[i] = columnFlags[i] | DJINN_COLUMN_EMPTY;
            }
        }
    }
    // DJINN_SELECT_ACTION: keep the first Djinni highlighted, dim the rest of its column, and label
    // each column's header with what choosing it does.
    if (mode == DJINN_SELECT_ACTION)
    {
        Sint32 fromX;
        Sint32 fromY;
        Uint16 srcCursor;
        Sint32 n;
        srcCursor = menu->selection[0];
        fromX     = (Uint16)((Uint32)srcCursor % (Uint32)DJINN_CURSOR_ROW_STRIDE);
        fromY     = (Uint16)((Uint32)srcCursor / (Uint32)DJINN_CURSOR_ROW_STRIDE);
        mnWindowSetInteriorPaletteBitRect(menu->listWindow,
                                          fromX * DJINN_COLUMN_TILES + 1,
                                          fromY + 2,
                                          DJINN_CURSOR_WIDTH,
                                          1,
                                          HIGHLIGHT_PALETTE);
        mnWindowSetInteriorRectPalette(menu->listWindow,
                                       fromX * DJINN_COLUMN_TILES + 1,
                                       2,
                                       DJINN_CURSOR_WIDTH,
                                       7,
                                       DJINN_DIM_PALETTE);
        for (n = 0; n < menu->memberCount; n++)
        {
            if (n == menu->selectedColumn[0])
            {
                if (menu->pairEntries[0] & DJINN_ENTRY_IS_SET)
                    msg = MsgDjinnStandbyAction;
                else
                    msg = MsgDjinnSetAction;
            }
            else if (columnFlags[n] & DJINN_COLUMN_HEADER_CHOOSABLE)
            {
                msg = MsgDjinnGiveAction;
            }
            else
            {
                msg = MsgDjinnTradeAction;
            }
            mnTextDrawMessageAsTilesStub(msg, menu->listWindow, n * DJINN_COLUMN_WIDTH + 8, 8);
        }
    }
    mnWindowSpriteRedrawSavedRectStub(menu->helpWindow);
    menu->cursor->motion = ICON_SHOWN;
    for (;;)
    {
        if (redraw != 0)
        {
            refresh     = redraw;
            redraw      = 0;
            selectedRow = -1;
            if (!(columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER))
                selectedRow = djinnRow;
            window = menu->statusWindow;
            unit   = gmUnitGetStub(menu->members[memberColumn]);
            mnWindowSpriteRedrawSavedRectStub(window);
            mnTextDrawStringAtOffsetStub(unit->name, window, 0, 0);
            mnTextDrawMessageAsTilesStub(unit->classId + MsgClassName, window, 0, 8);
            mnTextDrawStringAtOffsetStub(DjinnMenuTextLevel, window, 48, 0);
            mnTextDrawNumberInWindowStub(unit->level, 2, window, 72, 0);
            if (mode == DJINN_SELECT_DJINNI)
                mnTextDrawMessageAsTilesStub(MsgDjinnCharacterStatus, window, 0, 16);
            if (selectedRow != -1)
                djinn = backdrop->list.ids[memberColumn][selectedRow];
            mnWindowSpriteRedrawSavedRectStub(menu->helpWindow);
            // DJINN_SELECT_ACTION's help: `<member>'s`, the element icon and the Djinni's name, an
            // ability name argument (its action, from ABILITY_DJINNI_FIRST), then "What will you
            // do?".
            if (mode == DJINN_SELECT_ACTION)
            {
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
                mnWindowSetTilemapEntryStub(menu->helpWindow,
                                            DJINN_ELEMENT(menu->pairEntries[0])
                                                + DJINN_ELEMENT_TILE,
                                            0,
                                            0,
                                            TILEMAP_ENTRY_PLAIN);
                mnMessageArgumentPushStub(menu->shownMember[0], MESSAGE_ARGUMENT_ACTOR);
                mnMessageArgumentPushStub(DJINN_ELEMENT(menu->pairEntries[0]) * DJINN_PER_ELEMENT
                                              + DJINN_NUMBER(menu->pairEntries[0])
                                              + ABILITY_DJINNI_FIRST,
                                          MESSAGE_ARGUMENT_ABILITY);
                mnTextDrawMessageAsTilesStub(MsgDjinnOwnerLabel, menu->helpWindow, 8, 0);
#else
                mnMessageArgumentPushStub(menu->shownMember[0], MESSAGE_ARGUMENT_ACTOR);
                mnTextDrawMessageAsTilesStub(MsgDjinnOwnerLabel, menu->helpWindow, 0, 0);
                mnMessageArgumentPushStub(DJINN_ELEMENT(menu->pairEntries[0]) * DJINN_PER_ELEMENT
                                              + DJINN_NUMBER(menu->pairEntries[0])
                                              + ABILITY_DJINNI_FIRST,
                                          MESSAGE_ARGUMENT_ABILITY);
                mnWindowSetTilemapEntryStub(menu->helpWindow,
                                            DJINN_ELEMENT(menu->pairEntries[0])
                                                + DJINN_ELEMENT_TILE,
                                            6,
                                            0,
                                            TILEMAP_ENTRY_PLAIN);
                mnTextDrawMessageAsTilesStub(MsgDjinnOwnerLabel + 1, menu->helpWindow, 56, 0);
#endif
                mnTextDrawMessageAsTilesStub(MsgDjinnOwnerLabel + 2, menu->helpWindow, 0, 8);
            }
            // The Djinni's figure, its element's: standing for one on standby or recovering,
            // walking for one that is set.
            if (selectedRow == -1)
            {
                mnDjinnFiguresSetSlotPos(mode, 0, MENU_FIGURE_HIDDEN_Y, 0);
            }
            else if (mode == DJINN_SELECT_DJINNI)
            {
                if (groupMode)
                {
                    if (setAll == 0)
                    {
                        mnTextDrawMessageAsTilesStub(MsgDjinnSetAll, menu->helpWindow, 0, 0);
                    }
                    else
                    {
                        mnTextDrawMessageAsTilesStub(MsgDjinnAllStandby, menu->helpWindow, 0, 0);
                    }
                    if (gmDjinnCanSetStub(
                            DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))
                        || gmDjinnIsSetStub(
                            DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                    {
                        if (gmDjinnCanSetStub(
                                DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                            mnDjinnFiguresReplaceSlot(mode, DJINN_ELEMENT(djinn), ANIM_STAND);
                        else
                            mnDjinnFiguresReplaceSlot(mode, DJINN_ELEMENT(djinn), ANIM_WALK);
                        mnDjinnFiguresSetSlotPos(mode, DJINN_FIGURE_X(memberColumn), 62, 0);
                    }
                    else
                    {
                        mnTextDrawMessageAsTilesStub(MsgDjinnRecovering, menu->helpWindow, 0, 16);
                        mnDjinnFiguresReplaceSlot(mode, DJINN_ELEMENT(djinn), ANIM_STAND);
                        mnDjinnFiguresSetSlotPos(mode, DJINN_FIGURE_X(memberColumn), 62, 1);
                    }
                    if (refresh & DJINN_REDRAW_MOVED)
                        mnDjinnFiguresSetSlotPhase(mode, 0);
                }
                else
                {
                    mnTextDrawMessageAsTilesStub(MsgDjinnChoose, menu->helpWindow, 0, 0);
                    if (gmDjinnCanSetStub(
                            DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))
                        || gmDjinnIsSetStub(
                            DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                    {
                        if (gmDjinnCanSetStub(
                                DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                        {
                            mnTextDrawMessageAsTilesStub(
                                MsgDjinnChoose + 3, menu->helpWindow, 0, 16);
                            mnDjinnFiguresReplaceSlot(mode, DJINN_ELEMENT(djinn), ANIM_STAND);
                        }
                        else
                        {
                            mnTextDrawMessageAsTilesStub(
                                MsgDjinnChoose + 2, menu->helpWindow, 0, 16);
                            mnDjinnFiguresReplaceSlot(mode, DJINN_ELEMENT(djinn), ANIM_WALK);
                        }
                        mnDjinnFiguresSetSlotPos(mode, DJINN_FIGURE_X(memberColumn), 62, 0);
                    }
                    else
                    {
                        mnTextDrawMessageAsTilesStub(MsgDjinnChoose + 4, menu->helpWindow, 0, 16);
                        mnDjinnFiguresReplaceSlot(mode, DJINN_ELEMENT(djinn), ANIM_STAND);
                        mnDjinnFiguresSetSlotPos(mode, DJINN_FIGURE_X(memberColumn), 62, 1);
                    }
                    if (refresh & DJINN_REDRAW_MOVED)
                        mnDjinnFiguresSetSlotPhase(mode, 0);
                }
            }
            else
            {
                if (gmDjinnCanSetStub(
                        DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))
                    || gmDjinnIsSetStub(
                        DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                {
                    if (gmDjinnCanSetStub(
                            DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                        mnDjinnFiguresReplaceSlot(mode, DJINN_ELEMENT(djinn), ANIM_STAND);
                    else
                        mnDjinnFiguresReplaceSlot(mode, DJINN_ELEMENT(djinn), ANIM_WALK);
                    mnDjinnFiguresSetSlotPos(mode, DJINN_FIGURE_X(memberColumn), 54, 0);
                }
                else
                {
                    mnDjinnFiguresReplaceSlot(mode, DJINN_ELEMENT(djinn), ANIM_STAND);
                    mnDjinnFiguresSetSlotPos(mode, DJINN_FIGURE_X(memberColumn), 54, 1);
                }
                if (refresh & DJINN_REDRAW_MOVED)
                    mnDjinnFiguresSetSlotPhase(mode, 0);
            }
            mnWindowSpriteClearListStub(menu->listWindow);
            if (selectedRow != -1)
            {
                mnTextDrawMessageAsTilesStub(MsgUnleashEffect, menu->listWindow, 0, 80);
#if LANGUAGE != LANGUAGE_JAPANESE
                mnWindowClearInteriorTilesStub(menu->listWindow, 0, 96, 224, 104);
                mnTextDrawMessageAsTilesStub(DJINN_ELEMENT(djinn) * DJINN_PER_ELEMENT
                                                 + DJINN_NUMBER(djinn) + MsgDjinnEffect,
                                             menu->listWindow,
                                             0,
                                             96);
#else
                mnTextDrawMessageStub(DJINN_ELEMENT(djinn) * DJINN_PER_ELEMENT + DJINN_NUMBER(djinn)
                                          + MsgDjinnEffect,
                                      menu->listWindow,
                                      0,
                                      96);
#endif
            }
            if (!(columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER))
                mnWindowSetInteriorPaletteBitRect(menu->listWindow,
                                                  memberColumn * DJINN_COLUMN_TILES + 1,
                                                  djinnRow + 2,
                                                  DJINN_CURSOR_WIDTH,
                                                  1,
                                                  HIGHLIGHT_PALETTE);
            WINDOW_STATE->dirty = 1;
        }
        if (columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER)
            mnCursorPos(memberColumn * DJINN_COLUMN_WIDTH - 8, 52);
        else
            mnCursorPos(memberColumn * DJINN_COLUMN_WIDTH - 8, djinnRow * 8 + 60);
        sySynch(1);
        // Holding R after R on a Djinni keeps the group commands up; they end when R is let go or
        // pressed again (the latch is cleared when they began).
        if (!(PerData & KEY_R) || (PerPushLatch & KEY_R))
        {
            if (groupMode)
                redraw = DJINN_REDRAW_ALL;
            groupMode = FALSE;
            setAll    = 0;
        }
        repeat       = PerRepeat;
        pressed      = PerPush;
        // The tutorial plays the menu itself: the steps press A, B, Right and Left, after waits
        // counted in ticks.
        tutorialStep = backdrop->tutorialStep;
        if (tutorialStep != DJINN_TUTORIAL_OFF)
        {
            pressed = 0;
            backdrop->tutorialTimer++;
            repeat = 0;
            switch (tutorialStep)
            {
                case DJINN_TUTORIAL_CLOSE:
                    while (!(PerPush & KEY_A))
                    {
                        mnCursorPos(150, 26);
                        sySynch(1);
                    }
                    pressed = KEY_B;
                    repeat  = KEY_B;
                    break;
                // The recovery lesson: after its two messages, three recovery countdowns and
                // Isaac's Flint goes back on standby.
                case DJINN_TUTORIAL_RECOVERY:
                    if (backdrop->tutorialTimer == 60)
                    {
                        // "After a Djinni has been used to summon, it must spend time in recovery."
                        msgWindow = mnMessageOpenWindowStub(
                            MsgDjinnTutorialRecovery, 9, 9, MESSAGE_WINDOW_ZOOM);
                        MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_NORMAL;
                        while (mnMessageAllTypedStub() == 0)
                            sySynch(1);
                        mnWindowCloseStub(msgWindow, 1);
                        mnDjinnMenuDrawElementList(&backdrop->list);
                        sySynch(1);
                        // "And after we recover, we will set ourselves again and be ready to
                        // attack."
                        msgWindow = mnMessageOpenWindowStub(
                            MsgDjinnTutorialSetAgain, 9, 9, MESSAGE_WINDOW_ZOOM);
                        MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_NORMAL;
                        while (mnMessageAllTypedStub() == 0)
                            sySynch(1);
                        mnWindowCloseStub(msgWindow, 1);
                        mnDjinnMenuDrawElementList(&backdrop->list);
                        backdrop->tutorialTimer = 0;
                        btDjinnRecoveryCountDownStub();
                        btDjinnRecoveryCountDownStub();
                        btDjinnRecoveryCountDownStub();
                        gmDjinnPutOnStandbyStub(ISAAC, ELEMENT_VENUS, DJINNI_FLINT);
                        gmDjinnRecoveryAddStandbyStub(ISAAC, ELEMENT_VENUS, DJINNI_FLINT);
                        gmUnitCalculateStub(ISAAC);
                        repeat  = KEY_B;
                        pressed = KEY_B;
                    }
                    break;
                case DJINN_TUTORIAL_GIVE:
                    if (backdrop->tutorialTimer == 60)
                    {
                        // "To give your Djinni to someone else, move it like this."
                        msgWindow = mnMessageOpenWindowStub(
                            MsgDjinnTutorialGive, 9, 9, MESSAGE_WINDOW_ZOOM);
                        MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_NORMAL;
                        while (mnMessageAllTypedStub() == 0)
                            sySynch(1);
                        mnWindowCloseStub(msgWindow, 1);
                        mnDjinnMenuDrawElementList(&backdrop->list);
                        backdrop->tutorialTimer = 0;
                        backdrop->tutorialStep  = DJINN_TUTORIAL_GIVE_PICK;
                    }
                    break;
                case DJINN_TUTORIAL_GIVE_PICK:
                case DJINN_TUTORIAL_GIVE_DROP:
                case DJINN_TUTORIAL_SET_PICK:
                case DJINN_TUTORIAL_SET_CHOOSE:
                case DJINN_TUTORIAL_POWERS_CHOOSE:
                    if (backdrop->tutorialTimer == 90)
                    {
                        repeat                  = KEY_A;
                        pressed                 = KEY_A;
                        backdrop->tutorialTimer = 0;
                        backdrop->tutorialStep++;
                    }
                    break;
                case DJINN_TUTORIAL_GIVE_MOVE:
                    if (backdrop->tutorialTimer == 90)
                    {
                        repeat                  = KEY_RIGHT;
                        pressed                 = KEY_RIGHT;
                        backdrop->tutorialTimer = 0;
                        backdrop->tutorialStep  = DJINN_TUTORIAL_GIVE_DROP;
                    }
                    break;
                case DJINN_TUTORIAL_SET_AFTER_GIVE:
                case DJINN_TUTORIAL_SET_SPARE:
                    if (backdrop->tutorialTimer == 60)
                    {
                        // "After choosing who will take the Djinni, you can set it."
                        msgWindow = mnMessageOpenWindowStub(
                            MsgDjinnTutorialSetAfterGive, 9, 9, MESSAGE_WINDOW_ZOOM);
                        MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_NORMAL;
                        while (mnMessageAllTypedStub() == 0)
                            sySynch(1);
                        mnWindowCloseStub(msgWindow, 1);
                        mnDjinnMenuDrawElementList(&backdrop->list);
                        backdrop->tutorialTimer = 0;
                        backdrop->tutorialStep  = DJINN_TUTORIAL_SET_PICK;
                    }
                    break;
                case DJINN_TUTORIAL_SET_DONE:
                    if (backdrop->tutorialTimer == 40)
                    {
                        backdrop->tutorialTimer = 0;
                        repeat                  = KEY_B;
                        pressed                 = KEY_B;
                        backdrop->tutorialStep++;
                    }
                    break;
                case DJINN_TUTORIAL_POWERS:
                case DJINN_TUTORIAL_POWERS_WAIT:
                case DJINN_TUTORIAL_POWERS_WAIT_MORE:
                    if (backdrop->tutorialTimer == 40)
                    {
                        backdrop->tutorialTimer = 0;
                        backdrop->tutorialStep++;
                    }
                    break;
                case DJINN_TUTORIAL_POWERS_TALK:
                    if (backdrop->tutorialTimer == 60)
                    {
                        MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_NORMAL;
                        // "Each Djinni has different powers."
                        msgWindow                     = mnMessageOpenWindowStub(
                            MsgDjinnTutorialPowers, 9, 9, MESSAGE_WINDOW_ZOOM);
                        mnCursorSlide(2, 146);
                        while (mnMessageAllTypedStub() == 0)
                            sySynch(1);
                        while (!(PerPush & KEY_A))
                        {
                            mnCursorPos(2, 146);
                            sySynch(1);
                        }
                        mnWindowCloseStub(msgWindow, 1);
                        mnDjinnMenuDrawElementList(&backdrop->list);
                        sySynch(1);
                        // "Once we are set, you can unleash us to use our powers."
                        msgWindow = mnMessageOpenWindowStub(
                            MsgDjinnTutorialUnleash, 9, 9, MESSAGE_WINDOW_ZOOM);
                        while (mnMessageAllTypedStub() == 0)
                            sySynch(1);
                        while (!(PerPush & KEY_A))
                        {
                            mnCursorPos(2, 146);
                            sySynch(1);
                        }
                        mnWindowCloseStub(msgWindow, 1);
                        mnDjinnMenuDrawElementList(&backdrop->list);
                        sySynch(1);
                        // "So, let me demonstrate the power of my attack, Isaac."
                        msgWindow = mnMessageOpenWindowStub(
                            MsgDjinnTutorialDemonstrate, 9, 9, MESSAGE_WINDOW_ZOOM);
                        while (mnMessageAllTypedStub() == 0)
                            sySynch(1);
                        while (!(PerPush & KEY_A))
                        {
                            mnCursorPos(2, 146);
                            sySynch(1);
                        }
                        mnWindowCloseStub(msgWindow, 1);
                        mnDjinnMenuDrawElementList(&backdrop->list);
                        sySynch(1);
                        backdrop->tutorialTimer = 0;
                        backdrop->tutorialStep  = DJINN_TUTORIAL_POWERS_WAIT;
                    }
                    break;
                case DJINN_TUTORIAL_POWERS_PICK:
                case DJINN_TUTORIAL_POWERS_PICK_SPARE:
                    if (backdrop->tutorialTimer == 90)
                    {
                        repeat                  = KEY_A;
                        pressed                 = KEY_A;
                        backdrop->tutorialTimer = 0;
                        backdrop->tutorialStep  = DJINN_TUTORIAL_POWERS_MOVE;
                    }
                    break;
                case DJINN_TUTORIAL_POWERS_MOVE:
                    if (backdrop->tutorialTimer == 90)
                    {
                        backdrop->tutorialTimer = 0;
                        repeat                  = KEY_LEFT;
                        pressed                 = KEY_LEFT;
                        backdrop->tutorialStep  = DJINN_TUTORIAL_POWERS_CHOOSE;
                    }
                    break;
                case DJINN_TUTORIAL_POWERS_DONE:
                    if (backdrop->tutorialTimer == 60)
                    {
                        backdrop->tutorialTimer = 0;
                        backdrop->tutorialStep  = DJINN_TUTORIAL_POWERS_END;
                        repeat                  = KEY_B;
                        pressed                 = KEY_B;
                    }
                    break;
            }
        }
        // DJINN_SELECT_DJINNI's R: put the Djinni on standby or set it at once, unless it is
        // recovering.
        if (mode == DJINN_SELECT_DJINNI && (pressed & KEY_R))
        {
            if (selectedRow == -1)
            {
                sySoundPlayStub(SE_MENU_ERROR);
                continue;
            }
            i = 0;
            if (gmDjinnCanSetStub(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))
                || gmDjinnIsSetStub(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                i = 1;
            groupMode    = TRUE;
            PerPushLatch = 0;
            if (i == 0)
            {
                sySoundPlayStub(SE_MENU_ERROR);
                mnWindowSpriteClearListStub(menu->listWindow);
                mnWindowClearInteriorTilesStub(menu->listWindow, 0, 80, 216, 104);
                mnTextDrawMessageStub(MsgDjinnNowRecovering, menu->listWindow, 0, 96);
                redraw = DJINN_REDRAW_ALL;
                continue;
            }
            if (djinn & DJINN_ENTRY_IS_SET)
            {
                sySoundPlayStub(SE_NOTE_HIGH);
                gmDjinnPutOnStandbyStub(
                    DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
                gmDjinnRecoveryAddStandbyStub(
                    DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
            }
            else
            {
                sySoundPlayStub(SE_DJINNI_SET);
                gmDjinnSetStub(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
                gmDjinnRecoveryRemoveStub(
                    DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
            }
            gmUnitCalculateStub(DJINN_MEMBER(djinn));
            mnDjinnMenuDrawElementList(&backdrop->list);
            redraw = DJINN_REDRAW_ALL;
        }
        if (mode == DJINN_SELECT_DJINNI && (pressed & KEY_L))
        {
            result = DJINN_RESULT_STATUS;
            sySoundPlayStub(SE_MENU_CONFIRM);
            break;
        }
        if ((pressed & KEY_A) || (mode == DJINN_SELECT_ACTION && (pressed & KEY_R)))
        {
            i = 1;
            if (!(columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER))
            {
                if (selectedRow == -1)
                {
                    sySoundPlayStub(SE_MENU_ERROR);
                    continue;
                }
                i = 0;
                if (gmDjinnCanSetStub(
                        DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))
                    || gmDjinnIsSetStub(
                        DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                    i = 1;
            }
            if (i == 0)
            {
                sySoundPlayStub(SE_MENU_ERROR);
                mnWindowSpriteClearListStub(menu->listWindow);
                mnWindowClearInteriorTilesStub(menu->listWindow, 0, 80, 216, 104);
                mnTextDrawMessageStub(MsgDjinnNowRecovering, menu->listWindow, 0, 96);
                continue;
            }
            if (mode == DJINN_SELECT_ACTION)
            {
                if (columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER)
                {
                    if (memberColumn == menu->selectedColumn[0])
                    {
                        if (menu->pairEntries[0] & DJINN_ENTRY_IS_SET)
                            result = DJINN_RESULT_STANDBY;
                        else
                            result = DJINN_RESULT_SET;
                    }
                    else
                    {
                        result = DJINN_RESULT_GIVE;
                    }
                }
                else
                {
                    result = DJINN_RESULT_TRADE;
                }
            }
            else
            {
                result = DJINN_RESULT_CHOSEN;
            }
            sySoundPlayStub(SE_MENU_CONFIRM);
            break;
        }
        if (pressed & KEY_START)
        {
            sySoundPlayStub(SE_MENU_CANCEL);
            result = DJINN_RESULT_START;
            break;
        }
        if (pressed & KEY_B)
        {
            sySoundPlayStub(SE_MENU_CANCEL);
            result = DJINN_RESULT_BACK;
            break;
        }
        // Select: with R held, set every Djinni that is not recovering, or, the next time, put
        // every one on standby; otherwise open the help.
        if (mode == DJINN_SELECT_DJINNI && (pressed & KEY_SELECT))
        {
            if (groupMode)
            {
                setAll ^= 1;
                if (setAll)
                    sySoundPlayStub(SE_DJINNI_SET);
                else
                    sySoundPlayStub(SE_NOTE_HIGH);
                for (i = 0; i < menu->memberCount; i++)
                {
                    for (j = 0; j < backdrop->list.counts[i]; j++)
                    {
                        Uint32 entry = backdrop->list.ids[i][j];
                        Bool   usable;
                        usable = FALSE;
                        if (gmDjinnCanSetStub(
                                DJINN_MEMBER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry))
                            || gmDjinnIsSetStub(
                                DJINN_MEMBER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry)))
                            usable = TRUE;
                        if (usable)
                        {
                            if (setAll)
                            {
                                if (!(entry & DJINN_ENTRY_IS_SET))
                                {
                                    gmDjinnSetStub(DJINN_MEMBER(entry),
                                                   DJINN_ELEMENT(entry),
                                                   DJINN_NUMBER(entry));
                                    gmDjinnRecoveryRemoveStub(DJINN_MEMBER(entry),
                                                              DJINN_ELEMENT(entry),
                                                              DJINN_NUMBER(entry));
                                    gmUnitCalculateStub(DJINN_MEMBER(entry));
                                }
                            }
                            else if (entry & DJINN_ENTRY_IS_SET)
                            {
                                gmDjinnPutOnStandbyStub(
                                    DJINN_MEMBER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
                                gmDjinnRecoveryAddStandbyStub(
                                    DJINN_MEMBER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
                                gmUnitCalculateStub(DJINN_MEMBER(entry));
                            }
                        }
                    }
                }
                mnDjinnMenuDrawElementList(&backdrop->list);
                redraw = DJINN_REDRAW_ALL;
            }
            else
            {
                result = DJINN_RESULT_HELP;
                mnWindowSetInteriorPaletteBitRect(menu->listWindow,
                                                  memberColumn * DJINN_COLUMN_TILES + 1,
                                                  djinnRow + 2,
                                                  DJINN_CURSOR_WIDTH,
                                                  1,
                                                  WINDOW_PALETTE);
                sySoundPlayStub(SE_MENU_CONFIRM);
                break;
            }
        }
        if (repeat & KEY_UP)
        {
            sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            if (columnFlags[memberColumn] & DJINN_COLUMN_EMPTY)
                continue;
            mnWindowSetInteriorPaletteBitRect(menu->listWindow,
                                              memberColumn * DJINN_COLUMN_TILES + 1,
                                              djinnRow + 2,
                                              DJINN_CURSOR_WIDTH,
                                              1,
                                              WINDOW_PALETTE);
            if (columnFlags[memberColumn] & DJINN_COLUMN_EMPTY)
                continue;
            if (columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER)
            {
                columnFlags[memberColumn] &= ~DJINN_COLUMN_ON_HEADER;
                djinnRow                   = 0;
            }
            else if (djinnRow == 0 && (columnFlags[memberColumn] & DJINN_COLUMN_HEADER_CHOOSABLE))
            {
                columnFlags[memberColumn] |= DJINN_COLUMN_ON_HEADER;
                redraw                     = DJINN_REDRAW_MOVED;
                continue;
            }
            djinnRow--;
            count = backdrop->list.counts[memberColumn];
            if (count == 0)
                count = 1;
            djinnRow = mnWrapIndex(djinnRow, count);
            savedRow = djinnRow;
            redraw   = DJINN_REDRAW_MOVED;
            continue;
        }
        if (repeat & KEY_DOWN)
        {
            sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            if (columnFlags[memberColumn] & DJINN_COLUMN_EMPTY)
                continue;
            mnWindowSetInteriorPaletteBitRect(menu->listWindow,
                                              memberColumn * DJINN_COLUMN_TILES + 1,
                                              djinnRow + 2,
                                              DJINN_CURSOR_WIDTH,
                                              1,
                                              WINDOW_PALETTE);
            djinnRow++;
            count = backdrop->list.counts[memberColumn];
            if (count == 0)
                count = 1;
            djinnRow = mnWrapIndex(djinnRow, count);
            if ((columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER)
                && !(columnFlags[memberColumn] & DJINN_COLUMN_EMPTY))
            {
                columnFlags[memberColumn] &= ~DJINN_COLUMN_ON_HEADER;
                djinnRow                   = 0;
            }
            else if (djinnRow == 0 && (columnFlags[memberColumn] & DJINN_COLUMN_HEADER_CHOOSABLE))
            {
                columnFlags[memberColumn] |= DJINN_COLUMN_ON_HEADER;
            }
            savedRow = djinnRow;
            redraw   = DJINN_REDRAW_MOVED;
            continue;
        }
        if (repeat & KEY_LEFT)
        {
            sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            if (!(columnFlags[memberColumn] & DJINN_COLUMN_EMPTY))
                mnWindowSetInteriorPaletteBitRect(menu->listWindow,
                                                  memberColumn * DJINN_COLUMN_TILES + 1,
                                                  djinnRow + 2,
                                                  DJINN_CURSOR_WIDTH,
                                                  1,
                                                  WINDOW_PALETTE);
            memberColumn--;
            memberColumn = mnWrapIndex(memberColumn, menu->memberCount);
            if (mode == DJINN_SELECT_DJINNI)
            {
                for (i = 0; i < menu->memberCount; i++)
                {
                    if (columnFlags[memberColumn] == DJINN_COLUMN_EMPTY)
                    {
                        memberColumn--;
                        memberColumn = mnWrapIndex(memberColumn, menu->memberCount);
                    }
                }
            }
            djinnRow = savedRow;
            count    = backdrop->list.counts[memberColumn];
            if (count == 0)
                count = 1;
            djinnRow = mnWrapIndex(djinnRow, count);
            redraw   = DJINN_REDRAW_MOVED;
            continue;
        }
        if (repeat & KEY_RIGHT)
        {
            sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            if (!(columnFlags[memberColumn] & DJINN_COLUMN_EMPTY))
                mnWindowSetInteriorPaletteBitRect(menu->listWindow,
                                                  memberColumn * DJINN_COLUMN_TILES + 1,
                                                  djinnRow + 2,
                                                  DJINN_CURSOR_WIDTH,
                                                  1,
                                                  WINDOW_PALETTE);
            memberColumn++;
            memberColumn = mnWrapIndex(memberColumn, menu->memberCount);
            if (mode == DJINN_SELECT_DJINNI)
            {
                for (i = 0; i < menu->memberCount; i++)
                {
                    if (columnFlags[memberColumn] == DJINN_COLUMN_EMPTY)
                    {
                        memberColumn++;
                        memberColumn = mnWrapIndex(memberColumn, menu->memberCount);
                    }
                }
            }
            djinnRow = savedRow;
            count    = backdrop->list.counts[memberColumn];
            if (count == 0)
                count = 1;
            djinnRow = mnWrapIndex(djinnRow, count);
            redraw   = DJINN_REDRAW_MOVED;
        }
    }
    menu->selectedColumn[mode] = memberColumn;
    if (selectedRow != -1)
    {
        Uint32 id;
        Uint32 entry;

        menu->pairEntries[mode] = id = backdrop->list.ids[memberColumn][selectedRow];
        entry                        = (Uint16)id;
        menu->pairIndex[mode]        = entry % (DJINN_ENTRY_INDEX + 1);
        menu->pairElement[mode]      = DJINN_ELEMENT(entry);
        menu->pairMember[mode]       = DJINN_MEMBER(entry);
    }
    menu->selection[mode] = memberColumn + djinnRow * DJINN_CURSOR_ROW_STRIDE;
    return result;
}

// Lists owned Djinn by element and index, marking those that are set. For element -1, entries also
// encode their party member.
Sint32 mnDjinnListUnitEntries(Uint16 *entriesOut, Sint32 memberId, Sint32 element)
{
    UNIT  *unit  = gmUnitGetStub(memberId);
    Sint32 count = 0;
    Sint32 elementIndex;
    Sint32 djinnIndex;
    Sint32 encodedDjinn;

    if (element == -1)
    {
        for (elementIndex = 0; elementIndex < DJINN_ELEMENTS; elementIndex++)
        {
            for (djinnIndex = 0; djinnIndex < DJINN_PER_ELEMENT; djinnIndex++)
            {
                if (unit->djinnSet[elementIndex] & (1 << djinnIndex))
                {
                    encodedDjinn         = (elementIndex << 5) | djinnIndex | DJINN_ENTRY_SET;
                    encodedDjinn        |= memberId << 8;
                    entriesOut[count++]  = encodedDjinn;
                }
                else if (unit->djinnHeld[elementIndex] & (1 << djinnIndex))
                {
                    entriesOut[count++] = (elementIndex << 5) | djinnIndex | (memberId << 8);
                }
            }
        }
    }
    else
    {
        for (djinnIndex = 0; djinnIndex < DJINN_PER_ELEMENT; djinnIndex++)
        {
            if (unit->djinnSet[element] & (1 << djinnIndex))
                entriesOut[count++] = (element << 5) | djinnIndex | DJINN_ENTRY_SET;
            else if (unit->djinnHeld[element] & (1 << djinnIndex))
                entriesOut[count++] = (element << 5) | djinnIndex;
        }
    }
    return count;
}

// Draws both sides of the give or trade preview for the selected page: the receiver in the second
// window and the giver in the first. mode is a DJINN_COMPARE_OPERATION; only DJINN_COMPARE_TRADE
// differs, swapping the two Djinn. Returns 1.
Sint32 mnDjinnMenuDrawPairPreview(Sint32 mode, Sint32 pageIndex)
{
    DJINN_MENU_STATE *menu;

    menu = MenuState;
    if (mode == DJINN_COMPARE_TRADE)
    {
        mnDjinnMenuDrawStatPreview(menu->secondWindow,
                                   0,
                                   0,
                                   menu->pairMember[1],
                                   1,
                                   mode,
                                   DJINN_PREVIEW_TRADE,
                                   pageIndex,
                                   1);
        mnDjinnMenuDrawStatPreview(
            menu->window, 0, 0, menu->pairMember[0], mode, 1, DJINN_PREVIEW_TRADE, pageIndex, mode);
    }
    else
    {
        mnDjinnMenuDrawStatPreview(menu->secondWindow,
                                   0,
                                   0,
                                   menu->shownMember[1],
                                   1,
                                   0,
                                   DJINN_SWAP_PREVIEW_MODE,
                                   pageIndex,
                                   1);
        mnDjinnMenuDrawStatPreview(
            menu->window, 0, 0, menu->shownMember[0], 0, 0, DJINN_PREVIEW_UNSET, pageIndex, 0);
    }
    return 1;
}

// Copies a full unit for a temporary Djinn-stat preview.
static __inline__ void mnUnitCopy(UNIT *dest, UNIT *src)
{
    syWordsCopy(dest, src, sizeof(UNIT));
}

// Saves the unit, applies a hypothetical Djinn change, draws its differences and restores the
// original unit before returning.
Sint32 mnDjinnMenuDrawStatPreview(WINDOW *window,
                                  Sint32  x,
                                  Sint32  y,
                                  Sint32  memberId,
                                  Sint32  removePairSlot,
                                  Sint32  addPairSlot,
                                  Sint32  previewMode,
                                  Sint32  page,
                                  Sint32  side)
{
    UNIT             *unit;
    UNIT             *originalUnit;
    Sint32            giveElement;
    Sint32            giveIndex;
    Sint32            giveSet;
    Sint32            takeElement;
    Sint32            takeIndex;
    Sint32            takeSet;
    Sint32            lost;
    Sint32            gained;
    // mnPsynergyListDifference can write up to 64 entries here in the worst case (32 kept or gained
    // and 32 lost).
    Uint16            actions[48];
    DJINN_MENU_STATE *menu;

    unit         = gmUnitGetStub(memberId);
    menu         = MenuState;
    giveElement  = menu->pairElement[removePairSlot];
    giveIndex    = menu->pairIndex[removePairSlot];
    giveSet      = menu->pairEntries[removePairSlot] & DJINN_ENTRY_IS_SET;
    takeElement  = menu->pairElement[addPairSlot];
    takeIndex    = menu->pairIndex[addPairSlot];
    takeSet      = menu->pairEntries[addPairSlot] & DJINN_ENTRY_IS_SET;
    originalUnit = syHeapAllocate(sizeof(UNIT));
    mnUnitCopy(originalUnit, unit);

    if (page == 0)
    {
        if (previewMode == DJINN_PREVIEW_NO_CHANGE)
        {
            mnTextDrawNumberInWindowStub(unit->hpMax,
                                         DJINN_PREVIEW_DIGITS,
                                         window,
                                         x * 8 + DJINN_PREVIEW_MAX_X,
                                         DJINN_PREVIEW_STAT_Y(y, 0));
            mnTextDrawNumberInWindowStub(unit->ppMax,
                                         DJINN_PREVIEW_DIGITS,
                                         window,
                                         x * 8 + DJINN_PREVIEW_MAX_X,
                                         DJINN_PREVIEW_STAT_Y(y, 1));
            mnTextDrawNumberInWindowStub(unit->hp,
                                         DJINN_PREVIEW_DIGITS,
                                         window,
                                         x * 8 + DJINN_PREVIEW_CURRENT_X,
                                         DJINN_PREVIEW_STAT_Y(y, 0));
            mnTextDrawNumberInWindowStub(unit->pp,
                                         DJINN_PREVIEW_DIGITS,
                                         window,
                                         x * 8 + DJINN_PREVIEW_CURRENT_X,
                                         DJINN_PREVIEW_STAT_Y(y, 1));
            mnTextDrawStringAtOffsetStub(DjinnMenuTextSlash,
                                         window,
                                         x * 8 + DJINN_PREVIEW_SLASH_X,
                                         DJINN_PREVIEW_STAT_Y(y, 0));
            mnTextDrawStringAtOffsetStub(DjinnMenuTextSlash,
                                         window,
                                         x * 8 + DJINN_PREVIEW_SLASH_X,
                                         DJINN_PREVIEW_STAT_Y(y, 1));
        }
        else
        {
            mnTextDrawNumberInWindowStub(unit->hp,
                                         DJINN_PREVIEW_DIGITS,
                                         window,
                                         x * 8 + DJINN_PREVIEW_CURRENT_X,
                                         DJINN_PREVIEW_STAT_Y(y, 0));
            mnTextDrawNumberInWindowStub(unit->pp,
                                         DJINN_PREVIEW_DIGITS,
                                         window,
                                         x * 8 + DJINN_PREVIEW_CURRENT_X,
                                         DJINN_PREVIEW_STAT_Y(y, 1));
        }
        mnTextDrawNumberInWindowStub(unit->attack,
                                     DJINN_PREVIEW_DIGITS,
                                     window,
                                     x * 8 + DJINN_PREVIEW_CURRENT_X,
                                     DJINN_PREVIEW_STAT_Y(y, 2));
        mnTextDrawNumberInWindowStub(unit->defense,
                                     DJINN_PREVIEW_DIGITS,
                                     window,
                                     x * 8 + DJINN_PREVIEW_CURRENT_X,
                                     DJINN_PREVIEW_STAT_Y(y, 3));
        mnTextDrawNumberInWindowStub(unit->agility,
                                     DJINN_PREVIEW_DIGITS,
                                     window,
                                     x * 8 + DJINN_PREVIEW_CURRENT_X,
                                     DJINN_PREVIEW_STAT_Y(y, 4));
        mnTextDrawNumberInWindowStub(
            unit->luck, 2, window, x * 8 + DJINN_PREVIEW_LUCK_X, DJINN_PREVIEW_STAT_Y(y, 5));
    }

    // Only the temporary unit changes; the saved record is restored below.
    switch (previewMode)
    {
        case DJINN_PREVIEW_ADD_AND_SET:
            gmDjinnGiveStub(memberId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
            gmDjinnSetStub(memberId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
            break;
        case DJINN_PREVIEW_UNSET:
            giveIndex &= DJINN_ENTRY_INDEX;
            gmDjinnPutOnStandbyStub(memberId, giveElement, giveIndex);
            break;
        case DJINN_PREVIEW_TRADE:
            if (giveSet)
            {
                giveIndex &= DJINN_ENTRY_INDEX;
                gmDjinnPutOnStandbyStub(memberId, giveElement, giveIndex);
            }
            gmDjinnGiveStub(memberId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
            if (takeSet)
                gmDjinnSetStub(memberId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
            break;
        case DJINN_PREVIEW_ADD:
            gmDjinnGiveStub(memberId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
            if (takeSet)
                gmDjinnSetStub(memberId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
            break;
    }

    gmUnitCalculateStub(memberId);
    unit = gmUnitGetStub(memberId);

    if (page == 0)
    {
        Sint32 label;

        mnTextDrawStringAtOffsetStub(unit->name, window, x * 8 + 40, y * 8);
        mnTextDrawStringAtOffsetStub(DjinnMenuTextLevel, window, x * 8 + 40, y * 8 + 16);
        mnTextDrawNumberInWindowStub(unit->level, 2, window, x * 8 + 88, y * 8 + 16);
        label = MsgPreviewStatLabel;
        mnTextDrawMessageAsTilesStub(label, window, x * 8, DJINN_PREVIEW_STAT_Y(y, 0));
        mnTextDrawMessageAsTilesStub(label + 1, window, x * 8, DJINN_PREVIEW_STAT_Y(y, 1));
        mnTextDrawMessageAsTilesStub(label + 2, window, x * 8, DJINN_PREVIEW_STAT_Y(y, 2));
        mnTextDrawMessageAsTilesStub(label + 3, window, x * 8, DJINN_PREVIEW_STAT_Y(y, 3));
        mnTextDrawMessageAsTilesStub(label + 4, window, x * 8, DJINN_PREVIEW_STAT_Y(y, 4));
        mnTextDrawMessageAsTilesStub(label + 5, window, x * 8, DJINN_PREVIEW_STAT_Y(y, 5));
        mnTextDrawMessageAsTilesStub(
            MsgClassName + originalUnit->classId, window, x * 8, y * 8 + 32);
    }

    {
        Sint32 i = 0;

        if (page == 0)
        {
            Sint32 column;
#if LANGUAGE != LANGUAGE_JAPANESE
            Sint32 iconX;
#endif
            Sint32 textX;

            if (originalUnit->classId != unit->classId)
            {
                textX = x * 8;
                mnTextDrawMessageAsTilesStub(
                    MsgClassName + unit->classId, window, textX, y * 8 + 48);
                // The down arrow from the class to the next, a tile of BG character block 1, the
                // block after the windows' own.
                mnWindowSetTilemapEntryStub(
                    window,
                    WINDOW_TILE(BG_CHAR_SIZE / TILE_SIZE_4BPP + DJINN_CHANGE_ARROW_TILE),
                    x + 2,
                    5,
                    TILEMAP_ENTRY_PLAIN);
            }
            column = x;
            if (originalUnit->classId != unit->classId)
                column += 5;
            // Each element's icon and its count of set Djinn: WINDOW_TILE('0')
            // + n is the window font's digit n in WINDOW_PALETTE.
            for (i = 0; i < DJINN_ELEMENTS; i++)
            {
                mnWindowSetTilemapEntryStub(
                    window, DJINN_ELEMENT_TILE + i, column + i * 2, y + 5, TILEMAP_ENTRY_PLAIN);
                mnWindowSetTilemapEntryStub(window,
                                            unit->djinnSetCounts[i] + WINDOW_TILE('0'),
                                            column + i * 2 + 1,
                                            y + 5,
                                            TILEMAP_ENTRY_PLAIN);
            }
#if LANGUAGE != LANGUAGE_JAPANESE
            iconX = x * 8 + 70;
#endif
            if (unit->hp != originalUnit->hp)
            {
                mnTextDrawNumberInWindowStub(
                    unit->hp, 4, window, DJINN_PREVIEW_VALUE_X(x), DJINN_PREVIEW_STAT_Y(y, 0));
                if (unit->hp > originalUnit->hp)
                    mnIconCreateDjinnArrow(window,
                                           DJINN_PREVIEW_ARROW_X(x, iconX),
                                           DJINN_PREVIEW_STAT_Y(y, 0),
                                           STAT_ARROW_UP);
                else
                    mnIconCreateDjinnArrow(window,
                                           DJINN_PREVIEW_ARROW_X(x, iconX),
                                           DJINN_PREVIEW_STAT_Y(y, 0),
                                           STAT_ARROW_DOWN);
            }
            if (unit->pp != originalUnit->pp)
            {
                mnTextDrawNumberInWindowStub(
                    unit->pp, 4, window, DJINN_PREVIEW_VALUE_X(x), DJINN_PREVIEW_STAT_Y(y, 1));
                if (unit->pp > originalUnit->pp)
                    mnIconCreateDjinnArrow(window,
                                           DJINN_PREVIEW_ARROW_X(x, iconX),
                                           DJINN_PREVIEW_STAT_Y(y, 1),
                                           STAT_ARROW_UP);
                else
                    mnIconCreateDjinnArrow(window,
                                           DJINN_PREVIEW_ARROW_X(x, iconX),
                                           DJINN_PREVIEW_STAT_Y(y, 1),
                                           STAT_ARROW_DOWN);
            }
            if (unit->attack != originalUnit->attack)
            {
                mnTextDrawNumberInWindowStub(
                    unit->attack, 4, window, DJINN_PREVIEW_VALUE_X(x), DJINN_PREVIEW_STAT_Y(y, 2));
                if (unit->attack > originalUnit->attack)
                    mnIconCreateDjinnArrow(window,
                                           DJINN_PREVIEW_ARROW_X(x, iconX),
                                           DJINN_PREVIEW_STAT_Y(y, 2),
                                           STAT_ARROW_UP);
                else
                    mnIconCreateDjinnArrow(window,
                                           DJINN_PREVIEW_ARROW_X(x, iconX),
                                           DJINN_PREVIEW_STAT_Y(y, 2),
                                           STAT_ARROW_DOWN);
            }
            if (unit->defense != originalUnit->defense)
            {
                mnTextDrawNumberInWindowStub(
                    unit->defense, 4, window, DJINN_PREVIEW_VALUE_X(x), DJINN_PREVIEW_STAT_Y(y, 3));
                if (unit->defense > originalUnit->defense)
                    mnIconCreateDjinnArrow(window,
                                           DJINN_PREVIEW_ARROW_X(x, iconX),
                                           DJINN_PREVIEW_STAT_Y(y, 3),
                                           STAT_ARROW_UP);
                else
                    mnIconCreateDjinnArrow(window,
                                           DJINN_PREVIEW_ARROW_X(x, iconX),
                                           DJINN_PREVIEW_STAT_Y(y, 3),
                                           STAT_ARROW_DOWN);
            }
            if (unit->agility != originalUnit->agility)
            {
                mnTextDrawNumberInWindowStub(
                    unit->agility, 4, window, DJINN_PREVIEW_VALUE_X(x), DJINN_PREVIEW_STAT_Y(y, 4));
                if (unit->agility > originalUnit->agility)
                    mnIconCreateDjinnArrow(window,
                                           DJINN_PREVIEW_ARROW_X(x, iconX),
                                           DJINN_PREVIEW_STAT_Y(y, 4),
                                           STAT_ARROW_UP);
                else
                    mnIconCreateDjinnArrow(window,
                                           DJINN_PREVIEW_ARROW_X(x, iconX),
                                           DJINN_PREVIEW_STAT_Y(y, 4),
                                           STAT_ARROW_DOWN);
            }
            if (unit->luck != originalUnit->luck)
            {
                mnTextDrawNumberInWindowStub(
                    unit->luck, 2, window, x * 8 + 88, DJINN_PREVIEW_STAT_Y(y, 5));
                if (unit->luck > originalUnit->luck)
                    mnIconCreateDjinnArrow(window,
                                           DJINN_PREVIEW_ARROW_X(x, iconX),
                                           DJINN_PREVIEW_STAT_Y(y, 5),
                                           STAT_ARROW_UP);
                else
                    mnIconCreateDjinnArrow(window,
                                           DJINN_PREVIEW_ARROW_X(x, iconX),
                                           DJINN_PREVIEW_STAT_Y(y, 5),
                                           STAT_ARROW_DOWN);
            }
        }
    }

    // Pages 1 on: the Psynergy after the change, five to a page (six with no change), then the key
    // below: "* Psynergy Gained" in TEXT_COLOR_YELLOW, "* Psynergy Lost" in TEXT_COLOR_RED, or "*
    // No change".
    if (page > 0)
    {
        Sint32 rows;
        Sint32 first;
        Sint8  count;
        Sint8  row;
        Sint8  line;

        rows  = 6 - (previewMode != DJINN_PREVIEW_NO_CHANGE);
        first = rows * (page - 1);
        count = mnPsynergyListDifference(
            originalUnit->psynergy, unit->psynergy, actions, &gained, &lost);
        line = 0;
        for (row = 0; first < count && row < rows; row++, first++)
        {
            mnIconCreatePsynergyIcon(
                window, x * 8, (y + line * 2) * 8 + 4, actions[first] & PSYNERGY_ABILITY_MASK);
            if (actions[first] & DJINN_PREVIEW_GAINED)
                mnTextSetColorStub(TEXT_COLOR_YELLOW);
            else if (actions[first] & DJINN_PREVIEW_LOST)
                mnTextSetColorStub(TEXT_COLOR_RED);
            else
                mnTextSetColorStub(TEXT_COLOR_NORMAL);
            mnTextDrawMessageAsTilesStub(MsgAbilityName + (actions[first] & PSYNERGY_ABILITY_MASK),
                                         window,
                                         x * 8 + 16,
                                         (y + line * 2) * 8 + 8);
            mnTextDrawNumberAtOffsetStub(gmAbilityGetDirectStub(actions[first])->ppCost,
                                         2,
                                         window,
                                         x * 8 + 88,
                                         (y + line * 2) * 8 + 8);
            line++;
        }
        mnTextSetColorStub(TEXT_COLOR_NORMAL);
        mnTextDrawMessageAsTilesStub(MsgPsynergyPp, window, x * 8 + DJINN_PREVIEW_PP_X, y * 8);
        if (previewMode != DJINN_PREVIEW_NO_CHANGE)
        {
            Sint32 lines;

            lines = 0;
            if (gained)
            {
                mnTextSetColorStub(TEXT_COLOR_YELLOW);
                mnTextDrawMessageAsTilesStub(MsgPsynergyGained, window, x * 8, y * 8 + 88);
                lines = 1;
            }
            if (lost)
            {
                mnTextSetColorStub(TEXT_COLOR_RED);
                mnTextDrawMessageAsTilesStub(MsgPsynergyLost, window, x * 8, (y + lines) * 8 + 88);
                lines++;
            }
            if (lines == 0)
                mnTextDrawMessageAsTilesStub(MsgPsynergyUnchanged, window, x * 8, y * 8 + 88);
            mnTextSetColorStub(TEXT_COLOR_NORMAL);
            mnWindowDrawDividerLineStub(window, 0, 11, 13, 11);
        }
        WINDOW_STATE->dirty = 1;
    }

    if (page == 0)
        mnPortraitCreateStub(memberId, 0, side, window, 0, 0);
    // None of the preview's class, stat or action changes escape this call.
    mnUnitCopy(unit, originalUnit);
    syHeapRelease(originalUnit);
    return 1;
}

#undef DJINN_ENTRY_NONE
#undef DJINN_BACKDROP_DIGIT_TILE
#undef DJINN_BACKDROP_PALETTE
#undef DJINN_DIM_PALETTE
#undef DJINN_WINDOW_COPY_PALETTE
#undef DJINN_HELP_TOPICS
#undef DJINN_MEMBER_FIGURE_X
#undef DJINN_MEMBER_FIGURE_Y
#undef DJINN_CURSOR_WIDTH
#undef DJINN_SWAP_PREVIEW_MODE
#undef DJINN_PREVIEW_DIGITS
#undef DJINN_PREVIEW_CURRENT_X
#undef DJINN_PREVIEW_MAX_X
#undef DJINN_PREVIEW_SLASH_X
#undef DJINN_PREVIEW_LUCK_X
#undef DJINN_PREVIEW_ARROW_X
#undef DJINN_PREVIEW_PP_X
#undef DJINN_RESELECT_PAIR_ENTRY
#undef DJINN_PREVIEW_GAINED
#undef DJINN_PREVIEW_LOST
#undef DJINN_ENTRY_SET
#undef DJINN_COLUMN_TILES
#undef DJINN_COLUMN_WIDTH
#undef DJINN_FIGURE_X
#undef DJINN_CURSOR_ROW_STRIDE
#undef DJINN_PREVIEW_VALUE_X
