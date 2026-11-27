// Previewing a Djinn change before it is made, the chosen party member's status, and the checks
// that keep the party members' Djinn counts even. Part of menu.c.

#include "menu_internal.h"

// The Djinni figures of a give or trade preview hop across between the two party members once a
// second, a few ticks into it: a hop takes DJINN_PREVIEW_HOP_TICKS ticks along half a sine wave of
// DJINN_PREVIEW_HOP_SINE_HEIGHT pixels, cut flat at DJINN_PREVIEW_HOP_HEIGHT. It would dip no
// deeper than DJINN_PREVIEW_HOP_DEPTH below its line, and its slide would start over after
// DJINN_PREVIEW_SLIDE_TICKS ticks; a hop reaches neither.
#define DJINN_PREVIEW_HOP_TICKS       30
#define DJINN_PREVIEW_HOP_DELAY       5
#define DJINN_PREVIEW_HOP_SINE_HEIGHT 16
#define DJINN_PREVIEW_HOP_HEIGHT      6
#define DJINN_PREVIEW_HOP_DEPTH       12
#define DJINN_PREVIEW_SLIDE_TICKS     35

// The tilemap rows at the top of the screen that a tutorial message covers, saved before it opens
// and put back after it closes.
#define DJINN_TUTORIAL_COVERED_ROWS   8

// Shows or hides the menu's cursor around a line of the Djinn tutorial.
#define DJINN_TUTORIAL_SET_CURSOR(menu, value)                                                     \
    do                                                                                             \
    {                                                                                              \
        (menu)->cursor->motion = (value);                                                          \
    } while (0)

// The Djinn menu's preview of a give, trade, set or standby (mode, enum DJINN_COMPARE_OPERATION):
// both party members' stats and Psynergy as the change would leave them, paged with R, the Djinni
// figures hopping across, and "Is this OK?". Each unit is changed, measured and put back; nothing
// is applied.
Sint32 mnDjinnMenuRunPreview(Uint32 mode)
{
    DJINN_MENU_STATE    *menu;
    DJINN_MENU_BACKDROP *backdrop;
    WIN_STATE           *state;
    WINDOW              *list;
    Sint32               pageCounts[2];
    Sint32               selectedPages[2];
    Sint32               confirmationChoice;
    Uint32               redraw;
    Sint32               frame;
    Sint32               page;
    Sint32               previewMode;
    Bool                 addPairSlot;
    Sint32               step;
    Sint32               row;
    Sint32               result;
    Sint32               index;
    Sint32               memberIndex;

    menu                 = MenuState;
    confirmationChoice   = YES;
    result               = 0;
    redraw               = 1;
    frame                = 0;
    selectedPages[0]     = 0;
    page                 = DJINN_COMPARE_PAGE_BOTH;
    menu->cursor->motion = ICON_HIDDEN;
    selectedPages[1]     = 0;

    // The party figures go below the screen, except the two party members concerned: the first over
    // the left window, the second over the right.
    for (memberIndex = 0; memberIndex < 4; memberIndex++)
    {
        menu->memberY[memberIndex] = MENU_FIGURE_HIDDEN_Y;
    }
    menu->memberX[menu->selectedColumn[1]] = 120;
    menu->memberY[menu->selectedColumn[1]] = 32;
    menu->memberX[menu->selectedColumn[0]] = 16;
    menu->memberY[menu->selectedColumn[0]] = 32;

    mnWindowSpriteClearListStub(menu->listWindow);
    sySynch(1);
    mnWindowCloseIfOpen(&menu->helpWindow, 1);
    mnWindowUpdateOrCreate(&menu->helpWindow, 0, 0, 30, 5, WINDOW_FLAG_INSTANT);

    pageCounts[0] = 1;
    pageCounts[1] = 1;

    // Count the Psynergy lists' pages: make the change on each party member's unit, work the unit
    // out again, compare its Psynergy with a copy taken before, and put the unit back from the
    // copy.
    if (mode <= DJINN_COMPARE_GIVE)
    {
        Uint16 *differenceState;
        UNIT   *originalUnit;
        UNIT   *unit;
        Sint32  memberId;
        Sint32  firstElement;
        Sint32  firstIndex;
        Sint32  secondElement;
        Sint32  secondIndex;
        Sint32  firstWasSet;
        Sint32  secondWasSet;
        Sint32  lostCount;
        Sint32  gainedCount;
        Sint32  size;

        differenceState = syHeapAllocateEWRAM(96);
        size            = sizeof(*originalUnit);
        originalUnit    = syHeapAllocateEWRAM(size);
        secondElement   = 0;
        secondIndex     = 0;
        memberId        = menu->shownMember[0];
        firstElement    = menu->pairElement[0];
        firstIndex      = menu->pairIndex[0] & DJINN_ENTRY_INDEX;
        firstWasSet     = (Uint16)(menu->pairEntries[0] & DJINN_ENTRY_IS_SET);

        unit            = gmUnitGetStub(memberId);
        syWordsCopy(originalUnit, unit, size);
        gmDjinnPutOnStandbyStub(memberId, firstElement, firstIndex);
        if (mode == DJINN_COMPARE_TRADE)
        {
            secondElement = menu->pairElement[1];
            secondIndex   = menu->pairIndex[1] & DJINN_ENTRY_INDEX;
            secondWasSet  = (Uint16)(menu->pairEntries[1] & DJINN_ENTRY_IS_SET);
            gmDjinnGiveStub(memberId, secondElement, secondIndex);
            if (secondWasSet != 0)
            {
                gmDjinnSetStub(memberId, secondElement, secondIndex);
            }
        }
        gmUnitCalculateStub(memberId);
        pageCounts[0] = mnPsynergyListDifference(
            originalUnit->psynergy, unit->psynergy, differenceState, &gainedCount, &lostCount);
        syWordsCopy(unit, originalUnit, size);

        memberId = menu->shownMember[1];
        unit     = gmUnitGetStub(memberId);
        syWordsCopy(originalUnit, unit, size);
        if (mode == DJINN_COMPARE_TRADE)
        {
            gmDjinnPutOnStandbyStub(memberId, secondElement, secondIndex);
        }
        gmDjinnGiveStub(memberId, firstElement, firstIndex);
        if (firstWasSet != 0)
        {
            gmDjinnSetStub(memberId, firstElement, firstIndex);
        }
        gmUnitCalculateStub(memberId);
        pageCounts[1] = mnPsynergyListDifference(
            originalUnit->psynergy, unit->psynergy, differenceState, &gainedCount, &lostCount);
        syWordsCopy(unit, originalUnit, size);
        syHeapRelease(originalUnit);
        syHeapRelease(differenceState);
    }
    else if (mode == DJINN_COMPARE_STANDBY || mode == DJINN_COMPARE_SET)
    {
        Uint16 *differenceState;
        UNIT   *originalUnit;
        UNIT   *unit;
        Sint32  firstMemberId;
        Sint32  firstElement;
        Sint32  firstIndex;
        Sint32  applySecond;
        Sint32  lostCount;
        Sint32  gainedCount;

        differenceState = syHeapAllocateEWRAM(96);
        originalUnit    = syHeapAllocateEWRAM(sizeof(*originalUnit));
        firstMemberId   = menu->shownMember[0];
        firstElement    = menu->pairElement[0];
        firstIndex      = menu->pairIndex[0] & DJINN_ENTRY_INDEX;
        applySecond     = (mode == DJINN_COMPARE_SET);

        unit            = gmUnitGetStub(firstMemberId);
        syWordsCopy(originalUnit, unit, sizeof(*originalUnit));
        gmDjinnPutOnStandbyStub(firstMemberId, firstElement, firstIndex);
        if (applySecond != 0)
        {
            gmDjinnSetStub(firstMemberId, firstElement, firstIndex);
        }
        gmUnitCalculateStub(firstMemberId);
        pageCounts[0] = mnPsynergyListDifference(
            originalUnit->psynergy, unit->psynergy, differenceState, &gainedCount, &lostCount);
        syWordsCopy(unit, originalUnit, sizeof(*originalUnit));
        pageCounts[1] = pageCounts[0];
        page          = DJINN_COMPARE_PAGE_GIVER;
        syHeapRelease(originalUnit);
        syHeapRelease(differenceState);
    }

    // Psynergy entries to pages of five.
    pageCounts[0] = ((pageCounts[0] - 1) / 5) + 1;
    if (pageCounts[0] == 0)
    {
        pageCounts[0] = 1;
    }
    pageCounts[1] = ((pageCounts[1] - 1) / 5) + 1;
    if (pageCounts[1] == 0)
    {
        pageCounts[1] = 1;
    }

    // Open the paired sides only after both units have been restored.
    mnWindowUpdateOrCreate(&menu->window, 0, 5, 15, 15, WINDOW_FLAG_INSTANT);
    mnWindowUpdateOrCreate(&menu->secondWindow, 15, 5, 15, 15, WINDOW_FLAG_INSTANT);
    mnWindowSpriteRedrawSavedRectStub(menu->helpWindow);

    if (mode == DJINN_COMPARE_STANDBY)
    {
        mnTextDrawMessageAsTilesStub(MsgDjinnCompareStandby, menu->helpWindow, 96, 0);
        mnTextDrawMessageAsTilesStub(MsgDjinnCompareConfirm, menu->helpWindow, 96, 16);
    }
    else if (mode == DJINN_COMPARE_SET)
    {
        mnTextDrawMessageAsTilesStub(MsgDjinnCompareSet, menu->helpWindow, 96, 0);
        mnTextDrawMessageAsTilesStub(MsgDjinnCompareConfirm, menu->helpWindow, 96, 16);
    }
    else if (mode == DJINN_COMPARE_TRADE)
    {
        mnTextDrawMessageAsTilesStub(MsgDjinnCompareTrade, menu->helpWindow, 128, 0);
        mnTextDrawMessageAsTilesStub(MsgDjinnCompareConfirm, menu->helpWindow, 128, 8);
    }
    else
    {
        mnTextDrawMessageAsTilesStub(MsgDjinnCompareGive, menu->helpWindow, 128, 0);
        mnTextDrawMessageAsTilesStub(MsgDjinnCompareConfirm, menu->helpWindow, 128, 8);
    }

    // The Djinni concerned, after its element icon (BASIC_COLORS_PALETTE): in TEXT_COLOR_RED when
    // it is not set.
    row = (mode == DJINN_COMPARE_GIVE);
    if ((menu->pairEntries[0] & DJINN_ENTRY_IS_SET) == 0)
    {
        mnTextSetColorStub(TEXT_COLOR_RED);
    }
    mnTextDrawMessageAsTilesStub(DJINN_ELEMENT(menu->pairEntries[0]) * DJINN_PER_ELEMENT
                                     + DJINN_NUMBER(menu->pairEntries[0]) + MsgDjinnName,
                                 menu->helpWindow,
                                 48,
                                 row * 8);
    mnWindowSetTilemapEntryStub(menu->helpWindow,
                                DJINN_ELEMENT(menu->pairEntries[0]) + DJINN_ELEMENT_TILE,
                                5,
                                row,
                                TILEMAP_ENTRY_PLAIN);
    mnTextSetColorStub(TEXT_COLOR_NORMAL);

    if (mode == DJINN_COMPARE_TRADE)
    {
        if ((menu->pairEntries[1] & DJINN_ENTRY_IS_SET) == 0)
        {
            mnTextSetColorStub(TEXT_COLOR_RED);
        }
        mnTextDrawMessageAsTilesStub(DJINN_ELEMENT(menu->pairEntries[1]) * DJINN_PER_ELEMENT
                                         + DJINN_NUMBER(menu->pairEntries[1]) + MsgDjinnName,
                                     menu->helpWindow,
                                     48,
                                     16);
        mnWindowSetTilemapEntryStub(
            menu->helpWindow, DJINN_ELEMENT(menu->pairEntries[1]) + DJINN_ELEMENT_TILE, 5, 2, mode);
        mnTextSetColorStub(TEXT_COLOR_NORMAL);
    }
    else if (mode == DJINN_COMPARE_STANDBY || mode == DJINN_COMPARE_SET)
    {
        if ((menu->pairEntries[0] & DJINN_ENTRY_IS_SET) != 0)
        {
            mnTextSetColorStub(TEXT_COLOR_RED);
        }
        mnTextDrawMessageAsTilesStub(DJINN_ELEMENT(menu->pairEntries[0]) * DJINN_PER_ELEMENT
                                         + DJINN_NUMBER(menu->pairEntries[0]) + MsgDjinnName,
                                     menu->helpWindow,
                                     48,
                                     16);
        mnWindowSetTilemapEntryStub(menu->helpWindow,
                                    DJINN_ELEMENT(menu->pairEntries[0]) + DJINN_ELEMENT_TILE,
                                    5,
                                    2,
                                    TILEMAP_ENTRY_PLAIN);
        mnTextSetColorStub(TEXT_COLOR_NORMAL);
        // The blinking down arrow between the two, a tile of BG character block 1, the block after
        // the windows' own.
        mnWindowSetTilemapEntryStub(
            menu->helpWindow,
            WINDOW_TILE(BG_CHAR_SIZE / TILE_SIZE_4BPP + DJINN_CHANGE_ARROW_TILE),
            7,
            1,
            TILEMAP_ENTRY_PLAIN);
    }

    for (;;)
    {
        Sint32 pressedKeys;
        Sint32 repeatedKeys;

        state        = WINDOW_STATE;
        backdrop     = menu->backdrop;
        pressedKeys  = PerPush;
        repeatedKeys = PerRepeat;

        // The tutorial presses A for the player after 100 ticks, or shows its messages; the
        // windows' tilemap rows a message covers are saved before it opens and put back after it
        // closes.
        if (backdrop->tutorialStep != DJINN_TUTORIAL_OFF)
        {
            Sint16 *saved;
            Sint32  size;

            size         = DJINN_TUTORIAL_COVERED_ROWS * BG_SCREEN_WIDTH * sizeof(Uint16);
            saved        = syHeapAllocateEWRAM(size);
            pressedKeys  = 0;
            repeatedKeys = 0;
            backdrop->tutorialTimer += 1;

            switch (backdrop->tutorialStep)
            {
                case DJINN_TUTORIAL_GIVE_PREVIEW:
                case DJINN_TUTORIAL_SET_PREVIEW:
                case DJINN_TUTORIAL_POWERS_PREVIEW:
                    if (backdrop->tutorialTimer == 100)
                    {
                        repeatedKeys             = KEY_A;
                        pressedKeys              = KEY_A;
                        backdrop->tutorialTimer  = 0;
                        backdrop->tutorialStep  += 1;
                    }
                    break;
                case DJINN_TUTORIAL_SET_CHANGES:
                    if (backdrop->tutorialTimer == 60)
                    {
                        WINDOW *window;

                        syWordsCopy(saved, state->tilemap, size);
                        // "Setting a Djinni changes your attributes and maybe even your class."
                        window = mnMessageOpenWindowStub(
                            MsgDjinnTutorialSetChanges, 8, 0, MESSAGE_WINDOW_ZOOM);
                        DJINN_TUTORIAL_SET_CURSOR(menu, ICON_SHOWN);
                        mnCursorSlide(2, 96);
                        MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_NORMAL;
                        while (mnMessageAllTypedStub() == 0)
                        {
                            sySynch(1);
                        }
                        mnWindowCloseStub(window, 1);
                        syWordsCopy(state->tilemap, saved, size);
                        mnWindowMarkVisibleTextTilesStub();
                        state->dirty            = 1;
                        redraw                  = 1;
                        backdrop->tutorialTimer = 0;
                        backdrop->tutorialStep  = DJINN_TUTORIAL_CLASS_PSYNERGY;
                        DJINN_TUTORIAL_SET_CURSOR(menu, ICON_HIDDEN);
                    }
                    break;
                case DJINN_TUTORIAL_CLASS_PSYNERGY:
                    if (backdrop->tutorialTimer == 60)
                    {
                        WINDOW *window;

                        syWordsCopy(saved, state->tilemap, size);
                        // "When your class changes, the Psynergy you can use may also change."
                        window = mnMessageOpenWindowStub(
                            MsgDjinnTutorialClassPsynergy, 8, 0, MESSAGE_WINDOW_ZOOM);
                        DJINN_TUTORIAL_SET_CURSOR(menu, ICON_SHOWN);
                        mnCursorSlide(106, 56);
                        MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_NORMAL;
                        while (mnMessageAllTypedStub() == 0)
                        {
                            sySynch(1);
                        }
                        mnWindowCloseStub(window, 1);
                        syWordsCopy(state->tilemap, saved, size);
                        mnWindowMarkVisibleTextTilesStub();
                        state->dirty = 1;
                        sySynch(1);
                        redraw                  = 1;
                        backdrop->tutorialTimer = 0;
                        backdrop->tutorialStep  = DJINN_TUTORIAL_SET_PREVIEW;
                        DJINN_TUTORIAL_SET_CURSOR(menu, ICON_HIDDEN);
                    }
                    break;
                default: break;
            }
            syHeapRelease(saved);
        }

        // The help names what R shows next: `R: <giver>'s Psy` on DJINN_COMPARE_PAGE_BOTH, the
        // receiver's on DJINN_COMPARE_PAGE_GIVER, "R: Status" on DJINN_COMPARE_PAGE_RECEIVER.
        if (redraw != 0)
        {
            addPairSlot = TRUE;
            if (mode <= DJINN_COMPARE_GIVE)
            {
                mnWindowClearInteriorTilesStub(menu->helpWindow, 128, 16, 224, 24);
                if (page == DJINN_COMPARE_PAGE_RECEIVER)
                {
                    mnTextDrawMessageAsTilesStub(MsgDjinnStatusHelp, menu->helpWindow, 128, 16);
                }
                else
                {
                    mnMessageArgumentPushStub(menu->shownMember[page != DJINN_COMPARE_PAGE_BOTH],
                                              MESSAGE_ARGUMENT_ACTOR);
                    mnTextDrawMessageAsTilesStub(MsgDjinnPsyHelp, menu->helpWindow, 128, 16);
                }
            }
            state->holdTilemapUpload = 1;
            mnWindowSpritePrepareForRedrawStub(menu->window);
            mnWindowSpritePrepareForRedrawStub(menu->secondWindow);
            if ((redraw >> 1) != 0)
            {
                sySynch(1);
            }

            if (mode == DJINN_COMPARE_SET)
            {
                previewMode = DJINN_PREVIEW_ADD_AND_SET;
                addPairSlot = FALSE;
            }
            else if (mode == DJINN_COMPARE_STANDBY)
            {
                previewMode = DJINN_PREVIEW_UNSET;
                addPairSlot = FALSE;
            }
            else
            {
                previewMode = DJINN_PREVIEW_TRADE;
            }

            switch (page)
            {
                default: mnDjinnMenuDrawPairPreview(mode, 0); break;
                case DJINN_COMPARE_PAGE_RECEIVER:
                    if (mode == DJINN_COMPARE_GIVE)
                    {
                        previewMode = DJINN_PREVIEW_ADD;
                    }
                    mnDjinnMenuDrawStatPreview(
                        menu->window, 0, 0, menu->shownMember[1], page, 0, previewMode, 0, 0);
                    mnDjinnMenuDrawStatPreview(menu->secondWindow,
                                               0,
                                               0,
                                               menu->shownMember[1],
                                               page,
                                               0,
                                               previewMode,
                                               selectedPages[1] + 1,
                                               0);
                    break;
                case DJINN_COMPARE_PAGE_GIVER:
                    if (mode == DJINN_COMPARE_GIVE)
                    {
                        previewMode = DJINN_PREVIEW_UNSET;
                        addPairSlot = FALSE;
                    }
                    mnDjinnMenuDrawStatPreview(menu->window,
                                               0,
                                               0,
                                               menu->pairMember[0],
                                               page,
                                               addPairSlot,
                                               previewMode,
                                               page,
                                               1);
                    mnDjinnMenuDrawStatPreview(menu->secondWindow,
                                               0,
                                               0,
                                               menu->pairMember[0],
                                               page,
                                               addPairSlot,
                                               previewMode,
                                               selectedPages[0] + 1,
                                               1);
                    break;
            }
            state->holdTilemapUpload = 0;
        }

        // The Psynergy list's page numbers on the right window's top border: WINDOW_TILE('1') + n
        // is the window font's digit n + 1 in WINDOW_PALETTE ('0' past the tenth page), in
        // HIGHLIGHT_PALETTE for the page shown.
        if (page <= DJINN_COMPARE_PAGE_RECEIVER)
        {
            list = menu->secondWindow;
            if (pageCounts[page] > 1)
            {

                for (index = 0; index < pageCounts[page]; index++)
                {
                    Sint32 tile;

                    tile = WINDOW_TILE('1') + index;
                    if (index > 9)
                    {
                        tile = WINDOW_TILE('0');
                    }
                    if (index == selectedPages[page])
                    {
                        tile -= BG_TILE_PALETTE(WINDOW_PALETTE - HIGHLIGHT_PALETTE);
                    }
                    mnWindowSetTilemapEntryStub(list,
                                                tile,
                                                list->width - pageCounts[page] + index - 2,
                                                -1,
                                                TILEMAP_ENTRY_PLAIN);
                }
                mnWindowSetTilemapEntryStub(list,
                                            PAGE_ARROW_LEFT,
                                            list->width - pageCounts[page] - 3,
                                            -1,
                                            TILEMAP_ENTRY_PLAIN);
                mnWindowSetTilemapEntryStub(
                    list, PAGE_ARROW_RIGHT, list->width - 2, -1, TILEMAP_ENTRY_PLAIN);
                mnWindowMarkTilemapRow(state, list->y);
            }
        }

        // The Djinni figures, at a fixed 3/4 size (a negative phase), hop once a second: step runs
        // through the hop's ticks and holds at its end.
        frame++;
        step = (frame % 60) - DJINN_PREVIEW_HOP_DELAY;
        if (step < 0)
        {
            step = 0;
        }
        if (step > DJINN_PREVIEW_HOP_TICKS - 1)
        {
            step = DJINN_PREVIEW_HOP_TICKS - 1;
        }
        mnDjinnFiguresSetSlotPhase(0, -(toFIXED(3) / 4));
        mnDjinnFiguresSetSlotPhase(1, -(toFIXED(3) / 4));

        // The given Djinni hops from the giver's side to the receiver's, two pixels a tick; in a
        // trade the other one passes it the other way, dipping as far as the first one rises.
        if (mode <= DJINN_COMPARE_GIVE)
        {
            Sint32 rise;
            Sint32 slide;

            // DEAD: step never reaches DJINN_PREVIEW_HOP_TICKS, so the modulo leaves it as it is;
            // the ROM keeps its division.
            rise = -syMulFX(DJINN_PREVIEW_HOP_SINE_HEIGHT,
                            sySin((step % DJINN_PREVIEW_HOP_TICKS)
                                  * (DEGtoANG(180) / DJINN_PREVIEW_HOP_TICKS)));
            if (rise < -DJINN_PREVIEW_HOP_HEIGHT)
            {
                rise = -DJINN_PREVIEW_HOP_HEIGHT;
            }
            // DEAD: rise is never positive, the hop's half sine wave staying above the line, so
            // DJINN_PREVIEW_HOP_DEPTH never applies; the ROM keeps its test.
            if (rise > DJINN_PREVIEW_HOP_DEPTH)
            {
                rise = DJINN_PREVIEW_HOP_DEPTH;
            }
            // DEAD: step never reaches DJINN_PREVIEW_SLIDE_TICKS, so the modulo leaves it as it is
            // and the slide never wraps; the ROM keeps its division.
            slide = (step % DJINN_PREVIEW_SLIDE_TICKS) * 2;
            mnDjinnFiguresSetSlotPos(0, slide + 34, rise + 20, 0);
            if (mode == DJINN_COMPARE_TRADE)
            {
                mnDjinnFiguresSetSlotPos(1, 99 - slide, 36 - rise, 0);
            }
        }
        else
        {
            mnDjinnFiguresSetSlotPos(0, 32, 30, 0);
        }

        // No key moves confirmationChoice: the cursor stays on the first answer, so A always
        // confirms and B cancels.
        if (redraw != 0)
        {
            redraw             = 0;
            confirmationChoice = mnWrapIndex(confirmationChoice, 2);
        }
        mnCursorPos(confirmationChoice * 56 + 80, 16);

        // Blink the down arrow between a thing and what the change makes of it
        // (DJINN_CHANGE_ARROW_TILE): drawn and blanked in turn every 4 ticks.
        if ((frame & 3) == 0)
        {
            void  *tile;
            Uint32 fill = TEXT_CANVAS_BACKGROUND_WORD;

            tile        = (void *)DJINN_CHANGE_ARROW_VRAM;
            if ((frame & 4) != 0)
            {
                syWordsCopy(tile, MenuChangeArrowTile, 32);
            }
            else
            {
                syWordsFill(tile, 32, fill);
            }
        }

        if ((pressedKeys & KEY_A) != 0)
        {
            if (confirmationChoice == YES)
            {
                result = DJINN_RESULT_CONFIRMED;
                break;
            }
            sySoundPlayStub(SE_MENU_CANCEL);
            result = DJINN_RESULT_BACK;
            break;
        }
        if ((pressedKeys & KEY_START) != 0)
        {
            sySoundPlayStub(SE_MENU_CANCEL);
            result = DJINN_RESULT_START;
            break;
        }
        else if ((pressedKeys & KEY_B) != 0)
        {
            sySoundPlayStub(SE_MENU_CANCEL);
            result = DJINN_RESULT_BACK;
            break;
        }
        else if ((pressedKeys & KEY_R) != 0)
        {
            if (mode <= DJINN_COMPARE_GIVE)
            {
                page   = ((page + 1) % 3);
                redraw = 2;
                sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            }
        }
        else if ((repeatedKeys & KEY_LEFT) != 0)
        {
            if (page <= DJINN_COMPARE_PAGE_RECEIVER)
            {
                selectedPages[page]--;
                selectedPages[page] = mnWrapIndex(selectedPages[page], pageCounts[page]);
                sySoundPlayStub(SE_MENU_CURSOR_MOVE);
                syInputResetKeyRepeat();
                redraw = 1;
            }
        }
        else if ((repeatedKeys & KEY_RIGHT) != 0)
        {
            if (page <= DJINN_COMPARE_PAGE_RECEIVER)
            {
                sySoundPlayStub(SE_MENU_CURSOR_MOVE);
                syInputResetKeyRepeat();
                selectedPages[page]++;
                redraw              = 1;
                selectedPages[page] = mnWrapIndex(selectedPages[page], pageCounts[page]);
            }
        }
        sySynch(1);
    }

    // Restore the ordinary list drawing events and close the preview sides; the figures grow in
    // again from phase 0.
    mnDjinnFiguresSetSlotPhase(0, 0);
    mnDjinnFiguresSetSlotPhase(1, 0);
    sySetEvent(mnPlacePartyFigures, EVENT_PASS_TICK);

    WINDOW_STATE->holdTilemapUpload = 1;
    mnWindowCloseIfOpen(&menu->helpWindow, 1);
    sySynch(1);
    mnWindowUpdateOrCreate(&menu->helpWindow, 13, 0, 17, 5, WINDOW_FLAG_INSTANT);
    mnWindowCloseIfOpen((WINDOW **)&menu->window, 1);
    mnWindowCloseIfOpen(&menu->secondWindow, 1);
    mnWindowSpriteRedrawSavedRectStub(menu->listWindow);
    // BUG: no window is ever made in unopenedWindow (DJINN_MENU_STATE), so this redraws through
    // NULL; see the field. BUGFIX skips it.
#if !FIX_UNOPENED_WINDOW_REDRAW
    mnWindowSpriteRedrawSavedRectStub((WINDOW *)menu->unopenedWindow);
#endif
    mnWindowSpriteRedrawSavedRectStub(menu->statusWindow);
    WINDOW_STATE->holdTilemapUpload = 0;
    sySynch(1);
    return result;
}

// Shows the chosen party member's current stats and paged Psynergy list.
Sint32 mnDjinnMenuShowMemberStatus(void)
{
    Sint32            selectedPages[2];
    Sint32            pageCounts[2];
    Sint32            gained;
    Sint32            lost;
    Sint32            phase;
    Bool              redraw;
    Sint32            frame;
    WINDOW          **leftWindowSlot;
    WINDOW          **rightWindowSlot;
    WIN_STATE        *state;
    Uint32            pressedKeys;
    Uint32            repeatedKeys;
    DJINN_MENU_STATE *menu;
    UNIT             *unit;
    WINDOW           *window;
    Uint16           *listedActions;
    void             *saved;
    Sint32            result;
    Sint32            tile;
    Sint32            pageIndex;
    Sint32            side;
    Sint32            blink;

    menu                 = MenuState;
    phase                = 0;
    redraw               = TRUE;
    result               = 0;
    frame                = 0;
    menu->cursor->motion = ICON_HIDDEN;
    selectedPages[1] = selectedPages[0] = result;
    for (side = 0; side < 4; side++)
        menu->memberY[side] = MENU_FIGURE_HIDDEN_Y;
    mnWindowSpriteClearListStub(menu->listWindow);
    sySynch(1);
    // Count the party member's Psynergy by diffing their list with itself; saved, room for a copy
    // of the unit, is taken and given back unused.
    pageCounts[1] = pageCounts[0] = 1;
    listedActions                 = syHeapAllocateEWRAM(96);
    saved                         = syHeapAllocateEWRAM(sizeof(UNIT));
    unit                          = gmUnitGetStub(menu->shownMember[0]);
    pageCounts[1]                 = pageCounts[0] =
        mnPsynergyListDifference(unit->psynergy, unit->psynergy, listedActions, &gained, &lost);
    syHeapRelease(saved);
    syHeapRelease(listedActions);
    pageCounts[0] = (pageCounts[0] - 1) / 6 + 1;
    if (pageCounts[0] == 0)
        pageCounts[0] = 1;
    pageCounts[1] = (pageCounts[1] - 1) / 6 + 1;
    if (pageCounts[1] == 0)
        pageCounts[1] = 1;
    leftWindowSlot = (WINDOW **)&menu->window;
    mnWindowUpdateOrCreate(leftWindowSlot, 0, 5, 15, 15, WINDOW_FLAG_INSTANT);
    rightWindowSlot = &menu->secondWindow;
    mnWindowUpdateOrCreate(rightWindowSlot, 15, 5, 15, 15, WINDOW_FLAG_INSTANT);
    mnWindowSpriteRedrawSavedRectStub(menu->helpWindow);
    mnWindowSpriteRedrawSavedRectStub(menu->statusWindow);
    mnTextDrawMessageAsTilesStub(MsgCurrentStatus, menu->statusWindow, 0, 0);
    mnTextDrawMessageAsTilesStub(MsgCurrentStatus + 2, menu->statusWindow, 0, 16);
    side = 0;
    while (1)
    {
        state        = WINDOW_STATE;
        pressedKeys  = PerPush;
        repeatedKeys = PerRepeat;
        if (redraw)
        {
            state->holdTilemapUpload = 1;
            mnWindowSpritePrepareForRedrawStub(menu->window);
            mnWindowSpritePrepareForRedrawStub(menu->secondWindow);
            mnDjinnMenuDrawStatPreview(
                menu->window, 0, 0, menu->pairMember[0], 0, 0, DJINN_PREVIEW_NO_CHANGE, 0, 1);
            mnDjinnMenuDrawStatPreview(menu->secondWindow,
                                       0,
                                       0,
                                       menu->pairMember[0],
                                       0,
                                       0,
                                       DJINN_PREVIEW_NO_CHANGE,
                                       selectedPages[0] + 1,
                                       1);
            state->holdTilemapUpload = 0;
        }
        if (pageCounts[side] > 1)
        {
            window = menu->secondWindow;
            for (pageIndex = 0; pageIndex < pageCounts[side]; pageIndex++)
            {
                tile = WINDOW_TILE('1') + pageIndex;
                if (pageIndex > 9)
                    tile = WINDOW_TILE('0');
                if (pageIndex == selectedPages[side])
                    tile -= BG_TILE_PALETTE(WINDOW_PALETTE - HIGHLIGHT_PALETTE);
                mnWindowSetTilemapEntryStub(window,
                                            tile,
                                            window->width - pageCounts[side] + pageIndex - 2,
                                            -1,
                                            TILEMAP_ENTRY_PLAIN);
            }
            mnWindowSetTilemapEntryStub(window,
                                        PAGE_ARROW_LEFT,
                                        window->width - pageCounts[side] - 3,
                                        -1,
                                        TILEMAP_ENTRY_PLAIN);
            mnWindowSetTilemapEntryStub(
                window, PAGE_ARROW_RIGHT, window->width - 2, -1, TILEMAP_ENTRY_PLAIN);
            mnWindowMarkTilemapRow(state, window->y);
        }
        // blink is worked out as the preview's hop step is, and not used; the Djinni figure stays
        // below the screen.
        frame++;
        blink = frame % 60 - DJINN_PREVIEW_HOP_DELAY;
        CLAMP(blink, 0, DJINN_PREVIEW_HOP_TICKS - 1);
        mnDjinnFiguresSetSlotPos(0, 32, MENU_FIGURE_HIDDEN_Y, 0);
        if (redraw)
        {
            redraw = FALSE;
            phase  = mnWrapIndex(phase, 2);
        }
        if ((frame & 3) == 0)
        {
            if (frame & 4)
                syWordsCopy((void *)DJINN_CHANGE_ARROW_VRAM, MenuChangeArrowTile, 32);
            else
                syWordsFill((void *)DJINN_CHANGE_ARROW_VRAM, 32, TEXT_CANVAS_BACKGROUND_WORD);
        }
        if (pressedKeys & KEY_START)
        {
            sySoundPlayStub(SE_MENU_CANCEL);
            result = DJINN_RESULT_START;
            break;
        }
        if (pressedKeys & (KEY_A | KEY_B | KEY_R | KEY_L))
        {
            sySoundPlayStub(SE_MENU_CANCEL);
            result = DJINN_RESULT_BACK;
            break;
        }
        if (repeatedKeys & KEY_LEFT)
        {
            selectedPages[side]--;
            selectedPages[side] = mnWrapIndex(selectedPages[side], pageCounts[side]);
            sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            syInputResetKeyRepeat();
            redraw = TRUE;
        }
        else if (repeatedKeys & KEY_RIGHT)
        {
            selectedPages[side]++;
            sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            syInputResetKeyRepeat();
            redraw              = TRUE;
            selectedPages[side] = mnWrapIndex(selectedPages[side], pageCounts[side]);
        }
        sySynch(1);
    }
    sySetEvent(mnPlacePartyFigures, EVENT_PASS_TICK);
    WINDOW_STATE->holdTilemapUpload = 1;
    mnWindowCloseIfOpen(&menu->helpWindow, 1);
    sySynch(1);
    mnWindowUpdateOrCreate(&menu->helpWindow, 13, 0, 17, 5, WINDOW_FLAG_INSTANT);
    mnWindowCloseIfOpen(leftWindowSlot, 1);
    mnWindowCloseIfOpen(rightWindowSlot, 1);
    mnWindowSpriteRedrawSavedRectStub(menu->listWindow);
    // BUG: no window is ever made in unopenedWindow (DJINN_MENU_STATE), so this redraws through
    // NULL; see the field. BUGFIX skips it.
#if !FIX_UNOPENED_WINDOW_REDRAW
    mnWindowSpriteRedrawSavedRectStub((WINDOW *)menu->unopenedWindow);
#endif
    mnWindowSpriteRedrawSavedRectStub(menu->statusWindow);
    WINDOW_STATE->holdTilemapUpload = 0;
    sySynch(1);
    return result;
}

// Marks recipients that would leave unequal Djinn totals after a direct give. Returns how many
// recipients would need a trade instead.
Sint32 mnDjinnMarkTradeOnlyRecipients(Sint8 *requiresTrade, Sint32 srcMemberColumn)
{
    Sint32            tradeCount;
    Sint32            memberColumn;
    Sint8            *entry;
    DJINN_MENU_STATE *menu;

    menu         = MenuState;
    tradeCount   = 0;
    memberColumn = 0;
    if (tradeCount < menu->memberCount)
    {
        entry = requiresTrade;
        do
        {
            *entry = 0;
            if (memberColumn != srcMemberColumn)
            {
                if (mnDjinnCheckGiveBalance(srcMemberColumn, memberColumn) == 0)
                {
                    *entry      = 1;
                    tradeCount += 1;
                }
            }
            memberColumn += 1;
            entry        += 1;
        } while (memberColumn < menu->memberCount);
    }
    return tradeCount;
}

// Tests whether a hypothetical give leaves every pair of party members within one Djinn.
Sint32 mnDjinnCheckGiveBalance(Sint32 srcMemberColumn, Sint32 targetMemberColumn)
{
    DJINN_MENU_STATE *menu;
    Sint8             ownedCounts[16];
    Uint8             firstMember;
    Uint8             secondMember;
    Sint32            difference;
    Sint32            balanced;

    menu = MenuState;
    mnDjinnCountOwned((Uint8 *)ownedCounts);
    ownedCounts[srcMemberColumn]    -= 1;
    ownedCounts[targetMemberColumn] += 1;
    balanced                         = 1;
    for (firstMember = 0; firstMember < menu->memberCount; firstMember++)
    {
        secondMember = firstMember;
        if (firstMember < menu->memberCount)
        {
            while (1)
            {
                secondMember++;
                if (secondMember >= menu->memberCount)
                    break;
                difference = ownedCounts[firstMember] - ownedCounts[secondMember];
                if (difference < -1 || difference > 1)
                {
                    balanced = 0;
                    break;
                }
            }
        }
    }
    return balanced;
}

// Counts each party member's Djinn across its set and available bitmasks: writes every party
// member's total, set or not, into ownedCounts by list column.
void mnDjinnCountOwned(Uint8 *ownedCounts)
{
    DJINN_MENU_STATE *menu;
    UNIT             *unit;
    Sint32            memberIndex;
    Sint32            element;
    Sint32            djinnIndex;
    Sint32            ownedCount;

    menu = MenuState;
    for (memberIndex = 0; memberIndex < menu->memberCount; memberIndex++)
    {
        unit       = gmUnitGetStub(menu->members[memberIndex]);
        ownedCount = 0;
        for (element = 0; element <= 3; element++)
        {
            for (djinnIndex = 0; djinnIndex < DJINN_PER_ELEMENT; djinnIndex++)
            {
                if (unit->djinnSet[element] & (1 << djinnIndex))
                    ownedCount++;
                else if (unit->djinnHeld[element] & (1 << djinnIndex))
                    ownedCount++;
            }
        }
        ownedCounts[memberIndex] = ownedCount;
    }
}
