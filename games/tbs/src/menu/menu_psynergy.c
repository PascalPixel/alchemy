// The Psynergy menu: the field menu's Psynergy screen, the party member and Psynergy lists,
// targets, and the L and R shortcuts. Part of menu.c.

#include "menu_internal.h"

// A Psynergy shortcut, and the field's Psynergy request packed the same way: its party member above
// bit 10, the Psynergy's id below.
#define PSYNERGY_SHORTCUT_MEMBER_SHIFT 10
#define PSYNERGY_SHORTCUT_ID_MASK      0x3ff

// The bits of a listed Psynergy the info window's description is found by: nine, where the list's
// other reads take PSYNERGY_ABILITY_MASK's fourteen.
#define ABILITY_DESCRIPTION_MASK       0x1ff

// PSYNERGY_MENU_STATE.flags: set on going back to the list after a use or a cancelled target, which
// nothing reads; and while Select shows the shortcut help in the list.
#define PSYNERGY_MENU_BACK_TO_LIST     1
#define PSYNERGY_MENU_SHORTCUT_HELP    2

// The German, Spanish, French and Italian Psynergy menus save the window tiles they draw over and
// restore them after.
#if PSYNERGY_MENU_EXTRA_TEXT_TILES
#    define PSYNERGY_PROMPT_TILES      BG_CHAR_BLOCK(1)
#    define PSYNERGY_PROMPT_TILES_SIZE 8192
#endif

// The highlight over the Psynergy list's rows.
#define PSYNERGY_DETAIL_ROW_X     JAPANESE_OR(1, 0)
#define PSYNERGY_DETAIL_ROW_WIDTH JAPANESE_OR(14, 15)

// The Psynergy list's page icons, "PP" label, names and hand cursor.
#define PSYNERGY_PAGE_ICONS_X     JAPANESE_OR(120, 112)
#define PSYNERGY_PP_LABEL_X       JAPANESE_OR(72, 96)
#define PSYNERGY_NAME_X           JAPANESE_OR(32, 16)
#define PSYNERGY_LIST_CURSOR_X    JAPANESE_OR(98, 88)

// The field menu's Psynergy screen (mnFieldMenuRun, FIELD_MENU_PSYNERGY), over the field, which
// stops meanwhile.
Sint32 mnPsynergyMenuRun(void)
{
#if PSYNERGY_MENU_EXTRA_TEXT_TILES
    void *saved = syHeapAllocateEWRAM(PSYNERGY_PROMPT_TILES_SIZE);
#endif
    PSYNERGY_MENU_STATE *menu = syHeapSlotAllocate(HEAP_SLOT_MENU, MENU_STATE_SIZE);
    Sint32               member;
    Sint32               unused;
    Sint32               psynergy;
    Sint32               result;

    ActorSystemStateWork->suspended = 1;
    mnWindowDrawBorderStub(0, 0, 30, 20);
    sySynch(1);
    mnInitState(0);
    menu->memberCount = gmPartyListMembersStub(menu->memberTable);
    mnItemMenuInit(0, 3, 0, 7);
    menu->msgWindow = mnWindowCreateStub(13, 0, 17, 3, WINDOW_FLAG_INSTANT);
    mnPaletteLightenHighlight(HIGHLIGHT_PALETTE);
    mnPageArrowsDrawStub(PAGE_ARROW_VRAM);
#if PSYNERGY_MENU_EXTRA_TEXT_TILES
    mnSuspendFieldEvents();
    syWordsCopy(saved, PSYNERGY_PROMPT_TILES, PSYNERGY_PROMPT_TILES_SIZE);
    syWordsFill(PSYNERGY_PROMPT_TILES, PSYNERGY_PROMPT_TILES_SIZE, PIXELS_4BPP(3));
    mnTextUseExtraTilesStub(1);
#endif
    // Let the player choose.
    mnStartWatchingStart();
    result = mnPsynergyMenuRunSteps(&member, &unused, &psynergy);
    mnStopWatchingStart();
    if (result == 1)
    {
        FIELD_STATE *target = FieldStateWork;
        Uint16       request;
        // The action looked up here is not used.
        gmAbilityGetDirectStub(PSYNERGY_ABILITY_MASK & menu->sideAction[0]);
        request          = psynergy | (member << PSYNERGY_SHORTCUT_MEMBER_SHIFT);
        target->psynergy = request;
    }
    // Close the menu and give the screen back to the field.
    mnWindowSpriteClearListStub((void *)menu->statusWindow);
    WINDOW_STATE->holdTilemapUpload = 1;
    mnItemMenuClose();
    mnWindowDrawBorderStub(0, 0, 30, 20);
    syHeapSlotRelease(HEAP_SLOT_MENU);
    ActorSystemStateWork->suspended = 0;
#if PSYNERGY_MENU_EXTRA_TEXT_TILES
    mnWindowMarkVisibleTextTilesStub();
    mnTextUseExtraTilesStub(0);
    syWordsCopy(PSYNERGY_PROMPT_TILES, saved, PSYNERGY_PROMPT_TILES_SIZE);
    WINDOW_STATE->holdTilemapUpload = 0;
    syHeapRelease(saved);
    sySynch(1);
    mnResumeFieldEvents();
#endif
    sySynch(1);
    mnWindowEraseBorderRectStub(0, 0, 30, 20);
    WINDOW_STATE->holdTilemapUpload = 0;
    fdPartyClearUnusableShortcutsStub();
    return result;
}

// The steps of the Psynergy menu (mnPsynergyMenuRunSteps).
typedef enum PSYNERGY_MENU_STEP
{
    PSYNERGY_STEP_MEMBER,
    PSYNERGY_STEP_LIST,
    // how the Psynergy is used (mnPsynergyMenuClassifySelectedPsynergy)
    PSYNERGY_STEP_CLASSIFY,
    PSYNERGY_STEP_TARGET,
    PSYNERGY_STEP_APPLY,
} PSYNERGY_MENU_STEP;

// Runs the Psynergy menu: choose a party member and one of their Psynergy, then use it on a party
// member (or the whole party), or, in the shortcut modes, set it as the L or R shortcut.
Sint32 mnPsynergyMenuRunSteps(Sint32 *outMember, Sint32 *unused, Sint32 *outAction)
{
    PSYNERGY_MENU_STATE *menu;
    Sint32               useResult;
    Sint32               step;
    Sint32               done;
    Sint32               chosenAction;
    Sint32               targetMode;
    Sint32               actionEntry;
    Sint32               chosenTarget;
    ABILITY_DEFINITION  *action;

    step      = PSYNERGY_STEP_MEMBER;
    done      = 0;
    menu      = MenuState;
    useResult = 0;

    while (done == 0 && gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0)
    {
        switch (step)
        {
            // Choose whose Psynergy to look at.
            case PSYNERGY_STEP_MEMBER:
            {
                Sint32 chosen;

                menu->sideRow[0] = 0;
                mnItemMenuDrawMessage(0, MsgPsynergyChooseOwner);
                chosen = mnPsynergyMenuSelectPartySlot(0);
                if (chosen == -1)
                {
                    // FAKEMATCH(dead): chosen cleared though the menu ends before
                    // * reading it, as the Item menu's member step clears its own
                    // * (there the ROM keeps the store; here it leaves none); as the
                    // * arm without it.
                    chosen    = 0;
                    useResult = -1;
                    done      = 1;
                }
                mnWindowSpriteRedrawSavedRectStub(menu->infoWindow);
                step = PSYNERGY_STEP_LIST;
                break;
            }

            // Choose one of their Psynergy; in the shortcut modes, set it as the L or R shortcut
            // and start again.
            case PSYNERGY_STEP_LIST:
                sySynch(1);
                gmUnitGetStub(menu->memberIds[0]);
                if (menu->psynergyCount == 0)
                {
                    step = PSYNERGY_STEP_MEMBER;
                }
                else
                {
                    switch (menu->mode)
                    {
                        case PSYNERGY_MODE_USE:
                            mnItemMenuDrawMessage(0, MsgPsynergyChooseAbility);
                            break;
                        case PSYNERGY_MODE_SET_L:
                            mnItemMenuDrawMessage(0, MsgPsynergyChooseForL);
                            break;
                        case PSYNERGY_MODE_SET_R:
                            mnItemMenuDrawMessage(0, MsgPsynergyChooseForR);
                            break;
                    }
                    mnHideEntryIcons();
                    mnStatusPanelDraw(
                        menu->statusWindow, menu->memberIds[0], 0, STATUS_PANEL_EXPERIENCE);
                    chosenAction = mnPsynergyMenuRunList(0);
                    step         = PSYNERGY_STEP_MEMBER;
                    if (chosenAction != -1)
                    {
                        step = PSYNERGY_STEP_CLASSIFY;
                        if (menu->mode != PSYNERGY_MODE_USE)
                        {
                            if (menu->mode == PSYNERGY_MODE_SET_L)
                            {
                                mnPsynergyMenuSetShortcut(
                                    menu->memberIds[0], chosenAction, PSYNERGY_SHORTCUT_L);
                                mnWindowSpriteClearListStub((void *)menu->infoWindow);
                                mnItemMenuShowMessage(MsgShortcutSetL, -1, -1);
#if LANGUAGE == LANGUAGE_JAPANESE
                                mnWindowSpriteRedrawSavedRectStub(menu->infoWindow);
#endif
                            }
                            else
                            {
                                mnPsynergyMenuSetShortcut(
                                    menu->memberIds[0], chosenAction, PSYNERGY_SHORTCUT_R);
                                mnWindowSpriteClearListStub((void *)menu->infoWindow);
                                mnItemMenuShowMessage(MsgShortcutSetR, -1, -1);
#if LANGUAGE == LANGUAGE_JAPANESE
                                mnWindowSpriteRedrawSavedRectStub(menu->infoWindow);
#endif
                            }
                            step = PSYNERGY_STEP_MEMBER;
                        }
                    }
                }
                break;

            // Choose the party member to use it on.
            case PSYNERGY_STEP_TARGET:
                mnItemMenuDrawMessage(0, MsgPsynergyChooseTarget);
                chosenTarget = mnPsynergyMenuSelectTarget(0);
                step         = PSYNERGY_STEP_APPLY;
                if (chosenTarget == -1)
                {
                    menu->flags |= PSYNERGY_MENU_BACK_TO_LIST;
                    step         = PSYNERGY_STEP_LIST;
                }
                break;

            // How is it used? On one party member, on the whole party, or on the field once the
            // menu closes.
            case PSYNERGY_STEP_CLASSIFY:
                targetMode = mnPsynergyMenuClassifySelectedPsynergy();
                if (targetMode == ITEM_USE_MODE_CHOOSE_MEMBER)
                {
                    step = PSYNERGY_STEP_TARGET;
                    break;
                }
                if (targetMode == ITEM_USE_MODE_WHOLE_PARTY)
                {
                    menu->memberIds[1] = MENU_TARGET_WHOLE_PARTY;
                    step               = PSYNERGY_STEP_APPLY;
                    break;
                }
                done       = 1;
                useResult  = 1;
                *outMember = menu->memberIds[0];
                *outAction = menu->sideAction[0] & PSYNERGY_ABILITY_MASK;
                break;

            // Use it: pay its PP, show the result and go back to the list, or to the target when it
            // failed on one party member.
            case PSYNERGY_STEP_APPLY:
                chosenTarget = 0;
                actionEntry  = menu->sideAction[0];
                useResult =
                    mnAbilityApplyToTargets(actionEntry, menu->memberIds[0], menu->memberIds[1], 0);
                if (menu->memberIds[1] == MENU_TARGET_WHOLE_PARTY)
                {
                    menu->memberIds[1] = menu->memberIds[0];
                    chosenTarget       = MENU_TARGET_WHOLE_PARTY;
                }
                if (useResult != -1)
                {
                    action = gmAbilityGetDirectStub(menu->sideAction[0] & PSYNERGY_ABILITY_MASK);
                    gmUnitAddPpStub(menu->memberIds[0], -action->ppCost);
                }
                gmUnitCalculateStub(menu->memberIds[0]);
                if (useResult != -1)
                {
                    mnStatusPanelDraw(
                        menu->statusWindow, menu->memberIds[1], 0, STATUS_PANEL_EXPERIENCE);
                    mnAbilityPlayUseSound(menu->sideAction[0] & PSYNERGY_ABILITY_MASK);
                    mnWindowSpriteClearListStub((void *)menu->infoWindow);
                    mnItemMenuShowMessage(menu->useResult + MsgItemUseResult, 0, -1);
                }
                else
                {
                    sySoundPlayStub(SE_MENU_ERROR);
                    mnWindowSpriteClearListStub((void *)menu->infoWindow);
                    mnItemMenuShowMessage(menu->useResult + MsgItemUseResult, useResult, useResult);
                }
                if (useResult != -1)
                {
                    useResult    = 1;
                    menu->flags |= PSYNERGY_MENU_BACK_TO_LIST;
                    step         = PSYNERGY_STEP_LIST;
                }
                else
                {
                    menu->skipSlide = 1;
                    if (chosenTarget == MENU_TARGET_WHOLE_PARTY)
                    {
                        menu->flags |= PSYNERGY_MENU_BACK_TO_LIST;
                        step         = PSYNERGY_STEP_LIST;
                    }
                    else
                    {
                        step = PSYNERGY_STEP_TARGET;
                    }
                }
                break;

            default: done = 1; break;
        }
    }

    if (gmFlagGetStub(FLAG_MENU_START_PRESSED) != 0)
    {
        useResult = -1;
    }
    return useResult;
}

// Says how the selected Psynergy is used from the menu, as an item is (enum ITEM_USE_MODE):
// ITEM_USE_MODE_FROM_FIELD once the menu closes when the scene has a script for its field Psynergy
// (fieldPsynergy) where the leader faces, ITEM_USE_MODE_WHOLE_PARTY for ABILITY_RANGE_ALL,
// ITEM_USE_MODE_CHOOSE_MEMBER when it targets ABILITY_TARGET_ALLIES, and ITEM_USE_MODE_FROM_FIELD
// otherwise.
Sint32 mnPsynergyMenuClassifySelectedPsynergy(void)
{
    ABILITY_DEFINITION *action;
    Sint32              classification;

    action = gmAbilityGetDirectStub(PSYNERGY_ABILITY_MASK
                                    & ((PSYNERGY_MENU_STATE *)MenuState)->sideAction[0]);
    if (fdHasPsynergyCueStub(action->fieldPsynergy) != 0)
    {
        return ITEM_USE_MODE_FROM_FIELD;
    }
    if (action->range == ABILITY_RANGE_ALL)
    {
        return ITEM_USE_MODE_WHOLE_PARTY;
    }
    if (action->targetMode != ABILITY_TARGET_ALLIES)
    {
        classification = ITEM_USE_MODE_FROM_FIELD;
    }
    else
    {
        classification = ITEM_USE_MODE_CHOOSE_MEMBER;
    }
    return classification;
}

// Lets the player choose a party member in side partySlot (0 or 1), starting from the one chosen
// last, with each party member's Psynergy shown as the cursor passes.
Sint32 mnPsynergyMenuSelectPartySlot(Sint32 partySlot)
{
    PSYNERGY_MENU_STATE *menu        = MenuState;
    Sint32               memberIndex = menu->tabIndex[partySlot];
    WINDOW_SPR          *icon;
    UNIT                *unit;
    Uint16              *actions;
    Sint32               result;

    result                     = 0;
    icon                       = menu->sideIcon[partySlot];
    icon->motion               = ICON_SHOWN;
    icon->animationStep        = result;
    menu->cursorIcon->motion   = ICON_HIDDEN;
    menu->tabCounts[partySlot] = menu->memberCount;

    if (memberIndex == -1)
    {
        menu->tabIndex[partySlot] = 0;
        memberIndex               = 0;
    }
    else
    {
        mnCursorSlide(MENU_MEMBER_CURSOR_X(memberIndex), 16);
    }

    unit                = gmUnitGetStub(menu->memberTable[memberIndex]);
    actions             = menu->psynergies;
    menu->psynergyCount = mnPsynergyMenuCollectActions(unit, actions, PSYNERGY_LIST_FIELD);
    result              = mnPsynergyMenuSelectMember(menu->memberTable, actions);
    icon                = menu->sideIcon[partySlot];
    mnIconShow(icon);
    sySynch(1);
    return result;
}

// The party member row of the Psynergy menu.
Sint32 mnPsynergyMenuSelectMember(Uint16 *memberIds, Uint16 *unused)
{
    PSYNERGY_MENU_STATE *menu;
    Sint32               selection;
    Sint32               memberCount;
    Bool                 redraw;
    Sint32               result;
    UNIT                *unit;
    WINDOW              *window;
    Bool                 shown;
    Sint32               i;
    Uint16              *actions;
    Sint32               shortcutCount;
    Sint8                fieldCount;

    menu        = MenuState;
    selection   = menu->tabIndex[0];
    memberCount = menu->tabCounts[0];
    redraw      = TRUE;
    result      = 0;
    shown       = FALSE;
    menu->mode  = result;
    unit        = gmUnitGetStub(memberIds[selection]);
    if (mnWindowUpdateOrCreate(&menu->psynergyWindow, 13, 3, 17, 10, WINDOW_FLAG_INSTANT))
        mnSpawnIconEntries((ITEM_MENU_STATE *)menu, menu->psynergyWindow);
    if (mnWindowUpdateOrCreate(&menu->shortcutWindow, 13, 13, 17, 4, WINDOW_FLAG_INSTANT))
    {
        menu->cursorIcon = mnWindowSpriteCreateFromResourceStub(
            ICON_KIND_ITEM, 0, menu->shortcutWindow, 0, result);
        menu->cursorIcon->motion = ICON_HIDDEN;
    }
    while (!gmFlagGetStub(FLAG_MENU_START_PRESSED))
    {
        if (redraw)
        {
            redraw    = FALSE;
            selection = (memberCount + selection) % memberCount;
            window    = menu->statusWindow;
            unit      = gmUnitGetStub(memberIds[selection]);
            mnPsynergyMenuRefreshMemberPsynergy(memberIds[selection]);
            mnStatusPanelDraw(window, memberIds[selection], 0, STATUS_PANEL_EXPERIENCE);
            mnPsynergyMenuDrawShortcuts(menu->shortcutWindow, memberIds[selection]);
            mnReloadMemberPortrait(menu, memberIds[selection]);
            for (i = 0; i < 4; i++)
                menu->rowPositions[i] = MENU_MEMBER_Y;
            // Raise the chosen party member.
            menu->rowPositions[selection] = MENU_MEMBER_RAISED_Y;
            if (!gmFlagGetStub(FLAG_MENU_INFO_KEPT) && !shown)
            {
                mnWindowSpriteClearListStub((void *)menu->infoWindow);
#if LANGUAGE != LANGUAGE_JAPANESE
                mnWindowSpriteRedrawSavedRectStub((void *)menu->infoWindow);
#endif
                mnDrawCoins(menu->infoWindow);
                shown = TRUE;
            }
            else
            {
                gmFlagClearStub(FLAG_MENU_INFO_KEPT);
            }
        }
        mnCursorPos(MENU_MEMBER_CURSOR_X(selection), 16);
        sySynch(1);
        if (PerPush & KEY_A)
        {
            if (menu->psynergyCount)
            {
                sySoundPlayStub(SE_MENU_CONFIRM);
                result = memberIds[selection];
                break;
            }
            sySoundPlayStub(SE_MENU_ERROR);
        }
        // L or R: set that shortcut, if the party member has field Psynergy for it.
        if ((PerPush & KEY_L) || (PerPush & KEY_R))
        {
            result = memberIds[selection];
            if (PerPush & KEY_L)
                menu->mode = PSYNERGY_MODE_SET_L;
            else
                menu->mode = PSYNERGY_MODE_SET_R;
            actions = syHeapAllocate(32 * sizeof(Uint16));
            fieldCount =
                mnPsynergyMenuCollectActions((UNIT *)unit, actions, PSYNERGY_LIST_SHORTCUTS);
            syHeapRelease(actions);
            shortcutCount = fieldCount;
            if (shortcutCount == 0)
            {
                menu->mode = shortcutCount;
                sySoundPlayStub(SE_MENU_ERROR);
            }
            else
            {
                sySoundPlayStub(SE_MENU_CONFIRM);
                break;
            }
        }
        if (PerPush & KEY_B)
        {
            sySoundPlayStub(SE_MENU_CANCEL);
            result = -1;
            break;
        }
        if (PerRepeat & KEY_LEFT)
        {
            sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            selection--;
            redraw = TRUE;
        }
        if (PerRepeat & KEY_RIGHT)
        {
            sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            selection++;
            redraw = TRUE;
        }
    }
    menu->tabIndex[0]    = selection;
    menu->selectedMember = memberIds[selection];
    menu->memberIds[0]   = memberIds[selection];
    return result;
}

// Shows party member memberId's Psynergy icons in the Psynergy window, eight to a row, or "no
// Psynergy" when they have none.
void mnPsynergyMenuRefreshMemberPsynergy(Sint32 memberId)
{
    Uint16              *psynergies;
    PSYNERGY_MENU_STATE *menu;
    UNIT                *member;

    menu                = MenuState;
    member              = gmUnitGetStub(memberId);
    psynergies          = menu->psynergies;
    menu->psynergyCount = mnPsynergyMenuCollectActions(member, psynergies, PSYNERGY_LIST_FIELD);
    mnWindowSpriteRedrawSavedRectStub((void *)menu->psynergyWindow);
    mnPsynergyMenuPlaceEntryIcons(108, 32, 8);
    mnPsynergyMenuDrawPsynergyIcons(psynergies);
    if (menu->psynergyCount == 0)
    {
        mnTextDrawMessageAsTilesStub(MsgPsynergyMenuEmpty, menu->psynergyWindow, 0, 24);
    }
}

UNUSED Sint32 mnPsynergyMenuReturnTrue(void)
{
    return 1;
}

UNUSED void mnPsynergyMenuNoOperation(void)
{
}

// Chooses the party member to use the selected Psynergy on.
Sint32 mnPsynergyMenuSelectTarget(Sint32 targetMode)
{

    PSYNERGY_MENU_STATE *menu;
    Sint32               selection;
    Sint32               count;
    Bool                 redraw;
    Sint32               result;
    Bool                 shown;
    WINDOW_SPR          *marker;

    menu      = MenuState;
    selection = menu->tabIndex[1];
    count     = menu->memberCount;
    redraw    = TRUE;
    result    = 0;
    shown     = FALSE;
    gmUnitGetStub(menu->memberTable[menu->tabIndex[0]]);
    mnCursorSlide(MENU_MEMBER_CURSOR_X(selection), 16);
    while (!gmFlagGetStub(FLAG_MENU_START_PRESSED))
    {
        if (redraw)
        {
            redraw    = FALSE;
            selection = (selection + count) % count;
            gmUnitGetStub(menu->memberTable[selection]);
            // The marker over the party member, three tiles a party member; no screen makes it, so
            // it is NULL (mnCursorCreate).
            marker               = menu->sideIcon[1];
            marker->sprite.oam.x = marker->x =
                (((menu->auxiliaryWindow)->x + selection * 3) << 3) - 2;
            if (targetMode == 0)
            {
                mnStatusPanelDraw(
                    menu->statusWindow, menu->memberTable[selection], 0, STATUS_PANEL_EXPERIENCE);
                mnReloadMemberPortrait(menu, menu->memberTable[selection]);
                if (!gmFlagGetStub(FLAG_MENU_INFO_KEPT) && !shown)
                {
                    MENU_RESET_DESCRIPTION_WINDOW((void *)menu->infoWindow);
                    MENU_DRAW_DESCRIPTION((menu->sideAction[0] & PSYNERGY_ABILITY_MASK)
                                              + MsgAbilityDescription,
                                          menu->infoWindow,
                                          0,
                                          0);
                    shown = TRUE;
                }
                else
                {
                    gmFlagClearStub(FLAG_MENU_INFO_KEPT);
                }
            }
        }
        mnCursorPos(MENU_MEMBER_CURSOR_X(selection), 16);
        sySynch(1);
        if (PerPush & KEY_A)
        {
            sySoundPlayStub(SE_MENU_CONFIRM);
            result = menu->memberTable[selection];
            break;
        }
        if (PerPush & KEY_B)
        {
            sySoundPlayStub(SE_MENU_CANCEL);
            result = -1;
            break;
        }
        if (PerRepeat & KEY_LEFT)
        {
            sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            selection--;
            redraw = TRUE;
        }
        if (PerRepeat & KEY_RIGHT)
        {
            sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            selection++;
            redraw = TRUE;
        }
    }
    // The marker, still NULL.
    marker            = menu->sideIcon[1];
    menu->tabIndex[1] = selection;
    mnIconShow(marker);
    marker->motion = ICON_HIDDEN;
    sySynch(1);
    menu->tabIndex[1]    = selection;
    menu->selectedMember = menu->memberTable[selection];
    menu->memberIds[1]   = menu->memberTable[selection];
    return result;
}

// Sets the L or R Psynergy shortcut (PSYNERGY_SHORTCUT) to abilityId as memberId uses it, packed as
// the field's Psynergy request is. Returns 1.
Sint32 mnPsynergyMenuSetShortcut(Sint32 memberId, Sint32 abilityId, Sint32 shortcutIndex)
{
    Sint32 id   = abilityId & PSYNERGY_ABILITY_MASK;
    Sint32 code = (memberId << PSYNERGY_SHORTCUT_MEMBER_SHIFT) | id;

    if (shortcutIndex == PSYNERGY_SHORTCUT_L)
    {
        MainRoutineStateWork.firstShortcut = code;
    }
    else
    {
        MainRoutineStateWork.secondShortcut = code;
    }
    return 1;
}

// Writes the L and R Psynergy shortcuts into window: each one's Psynergy and party member, or that
// it is empty, under a heading that says how to change them. A name too wide for the line (measured
// as a window, over 10 tiles; 11 in Japanese) hides the party member (international) or drops the
// colon after "L" and "R" (Japanese). Returns 1.
#if LANGUAGE != LANGUAGE_JAPANESE

Sint32 mnPsynergyMenuDrawShortcuts(WINDOW *window, Sint32 memberId)
{
    Sint32 left;
    Sint32 top;
    Sint32 width;
    Sint32 height;
    Bool   wide;

    if (MainRoutineStateWork.firstShortcut != 0 && MainRoutineStateWork.secondShortcut != 0)
        mnTextDrawMessageAsTilesStub(MsgShortcutChangeHelp, window, 0, -8);
    else
        mnTextDrawMessageAsTilesStub(MsgShortcutLabel, window, 0, -8);

    mnTextGetMessageSizeStub((MainRoutineStateWork.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK)
                                 + MsgAbilityName,
                             &left,
                             &top,
                             &width,
                             &height);
    if (width < 0 || width > 10)
        wide = TRUE;
    else
        wide = FALSE;
    if (MainRoutineStateWork.firstShortcut != 0)
    {
        mnMessageArgumentPushStub(MainRoutineStateWork.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK,
                                  MESSAGE_ARGUMENT_ABILITY);
        mnTextDrawMessageAsTilesStub(MsgShortcutNameL, window, 0, 0);
        if (wide == 0)
            mnTextDrawStringAtOffsetStub(
                gmUnitGetStub(MainRoutineStateWork.firstShortcut >> PSYNERGY_SHORTCUT_MEMBER_SHIFT)
                    ->name,
                window,
                80,
                0);
    }
    else
    {
        mnTextDrawMessageAsTilesStub(MsgShortcutEmptyL, window, 0, 0);
    }

    mnTextGetMessageSizeStub((MainRoutineStateWork.secondShortcut & PSYNERGY_SHORTCUT_ID_MASK)
                                 + MsgAbilityName,
                             &left,
                             &top,
                             &width,
                             &height);
    if (width < 0 || width > 10)
        wide = TRUE;
    else
        wide = FALSE;
    if (MainRoutineStateWork.secondShortcut != 0)
    {
        mnMessageArgumentPushStub(MainRoutineStateWork.secondShortcut & PSYNERGY_SHORTCUT_ID_MASK,
                                  MESSAGE_ARGUMENT_ABILITY);
        mnTextDrawMessageAsTilesStub(MsgShortcutNameR, window, 0, 8);
        if (wide == 0)
            mnTextDrawStringAtOffsetStub(
                gmUnitGetStub(MainRoutineStateWork.secondShortcut >> PSYNERGY_SHORTCUT_MEMBER_SHIFT)
                    ->name,
                window,
                80,
                8);
        mnTextSetColorStub(TEXT_COLOR_NORMAL);
    }
    else
    {
        mnTextDrawMessageAsTilesStub(MsgShortcutEmptyR, window, 0, 8);
    }
    return 1;
}
#else

Sint32 mnPsynergyMenuDrawShortcuts(WINDOW *window, Sint32 member)
{
    Sint32 left;
    Sint32 top;
    Sint32 width;
    Sint32 height;
    Uint16 wide;

    if (MainRoutineStateWork.firstShortcut != 0 && MainRoutineStateWork.secondShortcut != 0)
        mnTextDrawMessageAsTilesStub(MsgShortcutChangeHelp, window, 0, -8);
    else
        mnTextDrawMessageAsTilesStub(MsgShortcutLabel, window, 0, -8);

    mnTextGetMessageSizeStub((MainRoutineStateWork.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK)
                                 + MsgAbilityName,
                             &left,
                             &top,
                             &width,
                             &height);
    if (width < 0 || width > 11)
        wide = 1;
    else
        wide = 0;
    if (wide)
        mnTextDrawStringInWindowStub((Uint8 *)MenuShortcutLString, window, 0, 0);
    else
        mnTextDrawStringInWindowStub((Uint8 *)MenuShortcutLColonString, window, 0, 0);
    if (MainRoutineStateWork.firstShortcut != 0)
    {
        if (wide)
            mnTextDrawMessageAsTilesStub(
                (MainRoutineStateWork.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK) + MsgAbilityName,
                window,
                8,
                0);
        else
            mnTextDrawMessageAsTilesStub(
                (MainRoutineStateWork.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK) + MsgAbilityName,
                window,
                16,
                0);
        mnTextDrawStringAtOffsetStub(
            gmUnitGetStub(MainRoutineStateWork.firstShortcut >> PSYNERGY_SHORTCUT_MEMBER_SHIFT)
                ->name,
            window,
            80,
            0);
    }
    else
    {
        mnTextDrawMessageAsTilesStub(MsgShortcutEmptyL, window, 24, 0);
    }

    mnTextGetMessageSizeStub((MainRoutineStateWork.secondShortcut & PSYNERGY_SHORTCUT_ID_MASK)
                                 + MsgAbilityName,
                             &left,
                             &top,
                             &width,
                             &height);
    if (width < 0 || width > 11)
        wide = 1;
    else
        wide = 0;
    if (wide)
        mnTextDrawStringInWindowStub((Uint8 *)MenuShortcutRString, window, 0, 8);
    else
        mnTextDrawStringInWindowStub((Uint8 *)MenuShortcutRColonString, window, 0, 8);
    if (MainRoutineStateWork.secondShortcut != 0)
    {
        if (wide)
            mnTextDrawMessageAsTilesStub(
                (MainRoutineStateWork.secondShortcut & PSYNERGY_SHORTCUT_ID_MASK) + MsgAbilityName,
                window,
                8,
                8);
        else
            mnTextDrawMessageAsTilesStub(
                (MainRoutineStateWork.secondShortcut & PSYNERGY_SHORTCUT_ID_MASK) + MsgAbilityName,
                window,
                16,
                8);
        mnTextDrawStringAtOffsetStub(
            gmUnitGetStub(MainRoutineStateWork.secondShortcut >> PSYNERGY_SHORTCUT_MEMBER_SHIFT)
                ->name,
            window,
            80,
            8);
        mnTextSetColorStub(TEXT_COLOR_NORMAL);
    }
    else
    {
        mnTextDrawMessageAsTilesStub(MsgShortcutEmptyR, window, 24, 8);
    }
    return 1;
}
#endif

// Sets up a Psynergy grid screen: the cursor window with the party's figures, a 30 by 15 window
// below with a hidden grid cursor and a portrait, and 16 Psynergy icon sprites in two rows of eight
// from (96, 8), on an 8 by 2 grid. Nothing calls it.
void mnPsynergyMenuCreateEntryGrid(void)
{
    WINDOW              *window;
    PSYNERGY_MENU_STATE *menu;
    WINDOW_SPR          *cursor;
    Sint32               index;
    Sint32               x;
    Sint32               y;
    WINDOW_SPR         **sprite;

    menu   = MenuState;
    window = mnCursorCreate(menu);
    mnCreatePartyFigures((void *)window, 2, 2, 8, 0);

    window                = mnWindowCreateStub(0, 5, 30, 15, WINDOW_FLAG_INSTANT);
    menu->psynergyWindow  = window;
    menu->selectedColumn  = 0;
    menu->selectedRow     = 0;
    menu->columnCount     = ICON_GRID_ROW;
    menu->rowCount        = 2;

    cursor                = (WINDOW_SPR *)mnIconCreateHandCursor(window, 0, 4);
    cursor->motion        = ICON_HIDDEN;
    menu->entryGridCursor = cursor;
    mnPortraitCreateStub(ISAAC, 0, 0, window, 0, 0);

    y      = 8;
    index  = 0;
    sprite = &menu->entryIcons[0];
    x      = 96;
    do
    {
        *sprite++ = mnWindowSpriteCreateFromResourceStub(ICON_KIND_ABILITY, index, window, x, y);
        index++;
        x += 16;
    } while (index < ICON_GRID_ROW);

    index  = ICON_GRID_ROW;
    y      = 24;
    sprite = &menu->entryIcons[ICON_GRID_ROW];
    x      = 96;
    do
    {
        *sprite++ = mnWindowSpriteCreateFromResourceStub(ICON_KIND_ABILITY, index, window, x, y);
        index++;
        x += 16;
    } while (index < 2 * ICON_GRID_ROW);
}

// Releases the party figures and closes the Psynergy grid screen's windows. Nothing calls it.
void mnPsynergyMenuCloseWindows(void)
{
    PSYNERGY_MENU_STATE *menu;

    menu = MenuState;
    mnReleasePartyFigures();
    mnWindowCloseStub((void *)menu->auxiliaryWindow, 1);
    mnWindowCloseStub((void *)menu->psynergyWindow, 1);
    mnWindowCloseStub((void *)menu->msgWindow, 1);
}

// Loads each listed Psynergy's icon into its icon sprite (32 entries) and hides the icons of empty
// entries.
void mnPsynergyMenuDrawPsynergyIcons(Uint16 *psynergies)
{
    Sint32       i;
    WINDOW_SPR **icons;
    Uint16      *p;
    Sint32       psynergyId;

    icons = ((PSYNERGY_MENU_STATE *)MenuState)->entryIcons;
    p     = psynergies;
    for (i = 0; i < PSYNERGY_SLOTS; i++)
    {
        psynergyId = *p++;
        if (psynergyId != 0)
        {
            mnSelectionIconLoadIntoSlotStub(
                ICON_KIND_ABILITY, psynergyId, (Uint8)(*icons)->index, 0);
        }
        icons++;
    }
    mnHideEmptyEntryIcons(psynergies);
}

// Lists unit's Psynergy into actions (PSYNERGY_SLOTS entries, cleared first) and returns how many
// there are, as mode (PSYNERGY_LIST_MODE) chooses; any mode other than PSYNERGY_LIST_SHORTCUTS and
// PSYNERGY_LIST_FIELD lists as PSYNERGY_LIST_ALL.
Sint32 mnPsynergyMenuCollectActions(UNIT *unit, Uint16 *actions, Sint32 mode)
{
    Sint32 clearIndex;
    Sint32 passCount;
    Sint32 actionCount;
    Sint32 slotIndex;
    Sint32 passIndex;

    passCount = (mode != PSYNERGY_LIST_FIELD) ? 4 : 3;
    for (clearIndex = 0; clearIndex < PSYNERGY_SLOTS; clearIndex++)
        actions[clearIndex] = 0;
    actionCount = 0;

    // fieldPsynergy is the Psynergy's field effect. Passes 1 and 2 add nothing.
    if (mode == PSYNERGY_LIST_SHORTCUTS)
    {
        for (slotIndex = 0; slotIndex < PSYNERGY_SLOTS; slotIndex++)
        {
            if (unit->psynergy[slotIndex].encodedAction != 0)
            {
                if (gmAbilityGetDirectStub(unit->psynergy[slotIndex].encodedAction
                                           & PSYNERGY_ABILITY_MASK)
                        ->fieldPsynergy
                    != 0)
                {
                    actions[actionCount] = unit->psynergy[slotIndex].encodedAction;
                    actionCount++;
                }
            }
        }
    }
    else
    {
        for (passIndex = 0; passIndex < passCount; passIndex++)
        {
            for (slotIndex = 0; slotIndex < PSYNERGY_SLOTS; slotIndex++)
            {
                if (unit->psynergy[slotIndex].encodedAction != 0)
                {
                    ABILITY_DEFINITION *ability = gmAbilityGetDirectStub(
                        unit->psynergy[slotIndex].encodedAction & PSYNERGY_ABILITY_MASK);

                    if (passIndex == 0
                        && (ability->fieldPsynergy != 0
                            || (ability->flags & ABILITY_USABLE_IN_FIELD) != 0))
                    {
                        actions[actionCount] = unit->psynergy[slotIndex].encodedAction;
                        actionCount++;
                    }
                    else if (passIndex == 1)
                    {
                    }
                    else if (passIndex == 2)
                    {
                    }
                    else if (passIndex == 3 && ability->fieldPsynergy == 0
                             && (ability->flags & ABILITY_USABLE_IN_FIELD) == 0)
                    {
                        actions[actionCount] = unit->psynergy[slotIndex].encodedAction;
                        actionCount++;
                    }
                }
            }
        }
    }

    return actionCount;
}

// Fills result with the Psynergy list of the party member in side index: their unit, how many
// Psynergy are listed, and the page and row of the one chosen last for them (kept within the list),
// five to a page. Returns 1.
Sint32 mnPsynergyMenuBuildPagedList(PAGED_LIST *result, Sint32 index)
{
    Sint32               unitBits;
    PSYNERGY_MENU_STATE *menu = MenuState;
    Sint32               entryCount;
    Sint32               row;
    Sint32               page;
    Sint32               pageCount;
    Sint32               selectedIndex;
    Sint32               memberId;

    unitBits      = (Sint32)gmUnitGetStub(menu->memberIds[index]);
    entryCount    = menu->psynergyCount;
    memberId      = menu->memberIds[index];
    selectedIndex = menu->selectedIndexByMember[memberId];
    if ((selectedIndex + 1) > entryCount)
    {
        selectedIndex = entryCount - 1;
    }
    page      = selectedIndex / LIST_PAGE_SIZE;
    row       = selectedIndex % LIST_PAGE_SIZE;
    pageCount = entryCount / LIST_PAGE_SIZE;
    if (entryCount % LIST_PAGE_SIZE != 0)
    {
        pageCount++;
    }
    result->memberState   = unitBits;
    result->page          = page;
    result->pageCount     = pageCount;
    result->row           = row;
    result->entryCount    = entryCount;
    result->selectedIndex = selectedIndex;
    return 1;
}

// Updates the Psynergy list for the row under the cursor: puts the Psynergy's description in the
// info window, unless a kept message is there, and highlights its row of the five. window and
// memberState are not used.
Sint32 mnPsynergyMenuShowSelectedRow(WINDOW *window, Sint32 *memberState, PAGED_LIST *state)
{
    PSYNERGY_MENU_STATE *menu;
    Sint32               actionIndex;
    Sint32               rowIndex;

    menu                  = MenuState;
    actionIndex           = state->page * LIST_PAGE_SIZE;
    actionIndex          += state->row;
    state->selectedIndex  = actionIndex;

    if (gmFlagGetStub(FLAG_MENU_INFO_KEPT) == 0)
    {
        MENU_RESET_DESCRIPTION_WINDOW(menu->infoWindow);
        sySynch(1);

        actionIndex = state->selectedIndex;
        if (menu->psynergies[actionIndex] != 0)
        {
            Sint32 description =
                (menu->psynergies[actionIndex] & ABILITY_DESCRIPTION_MASK) + MsgAbilityDescription;
            MENU_DRAW_DESCRIPTION(description, menu->infoWindow, 0, 0);
        }
    }
    else
    {
        // BUG: clears FLAG_SCENE_TEMP_FF instead of FLAG_MENU_INFO_KEPT, so a kept message keeps
        // the descriptions away until another screen clears it.
#if FIX_KEPT_MESSAGE_FLAG_CLEAR
        gmFlagClearStub(FLAG_MENU_INFO_KEPT);
#else
        gmFlagClearStub(FLAG_SCENE_TEMP_FF);
#endif
    }

    for (rowIndex = 0; rowIndex < LIST_PAGE_SIZE; rowIndex++)
    {
        if (rowIndex == state->row)
        {
            mnWindowSetHighlightRect((void *)menu->psynergyWindow,
                                     PSYNERGY_DETAIL_ROW_X,
                                     rowIndex * 2 + 1,
                                     PSYNERGY_DETAIL_ROW_WIDTH,
                                     1,
                                     HIGHLIGHT_PALETTE);
        }
        else
        {
            mnWindowSetHighlightRect((void *)menu->psynergyWindow,
                                     PSYNERGY_DETAIL_ROW_X,
                                     rowIndex * 2 + 1,
                                     PSYNERGY_DETAIL_ROW_WIDTH,
                                     1,
                                     WINDOW_PALETTE);
        }
    }

    sySynch(1);
    return 1;
}

// Draws one page of the Psynergy list into window: up to five names with their icons and PP costs,
// Psynergy the party member cannot afford in the no-HP colour and one restricted here in the low-HP
// colour, the page numbers, and the help line (how to set a shortcut while Select is held, else how
// to change party member).
Sint32 mnPsynergyMenuDrawPage(WINDOW *window, Sint32 unused, const PAGED_LIST *pageState)
{
    Uint32               firstEntry;
    Uint32               visibleCount;
    Uint8                row;
    Sint32               cursor;
    UNIT                *member;
    ABILITY_DEFINITION  *ability;
    PSYNERGY_MENU_STATE *menu = MenuState;

    (void)unused;

    mnWindowSpriteRedrawSavedRectStub((void *)window);
    mnWindowDrawDividerLineStub(window, 0, 11, 16, 11);

    if (PSYNERGY_MENU_SHORTCUT_HELP & menu->flags)
    {
        mnTextDrawMessageAsTilesStub(MsgShortcutHelp, window, 0, 88);
    }
    else
    {
        mnTextDrawMessageAsTilesStub(MsgChangeCharacterHelp, window, CHANGE_CHARACTER_HELP_X, 88);
    }

    firstEntry   = pageState->page * LIST_PAGE_SIZE;
    visibleCount = (Uint8)(pageState->entryCount - firstEntry);
    if (visibleCount > LIST_PAGE_SIZE)
    {
        visibleCount = LIST_PAGE_SIZE;
    }

    mnSetPageIcons(LIST_PAGE_SIZE, firstEntry, window, PSYNERGY_PAGE_ICONS_X, 34);
    mnPagedListDrawPageNumbers(
        window, pageState->entryCount, LIST_PAGE_SIZE, pageState->page, PAGE_NUMBERS_RIGHT);
    mnTextDrawMessageAsTilesStub(MsgPsynergyPp, window, PSYNERGY_PP_LABEL_X, 0);

    row = 0;
    if (visibleCount > row)
    {
        cursor = firstEntry;
        do
        {
            member  = gmUnitGetStub(menu->memberIds[0]);
            ability = gmAbilityGetDirectStub(PSYNERGY_ABILITY_MASK & menu->psynergies[cursor]);

            if (ability->ppCost > member->pp)
            {
                mnTextSetColorStub(TEXT_COLOR_NO_HP);
            }
            else if (mnPsynergyMenuIsActionRestricted(PSYNERGY_ABILITY_MASK
                                                      & menu->psynergies[cursor])
                     != 0)
            {
                mnTextSetColorStub(TEXT_COLOR_LOW_HP);
            }
            else
            {
                mnTextSetColorStub(TEXT_COLOR_NORMAL);
            }

            mnTextDrawMessageAsTilesStub((PSYNERGY_ABILITY_MASK & menu->psynergies[cursor])
                                             + MsgAbilityName,
                                         window,
                                         PSYNERGY_NAME_X,
                                         row * 16 + 8);
            mnTextDrawNumberAtOffsetStub(ability->ppCost, 2, window, 104, row * 16 + 8);
            mnTextSetColorStub(TEXT_COLOR_NORMAL);

            row++;
            cursor++;
        } while (visibleCount > row);
    }

    return 1;
}

// The Psynergy list of the party member in sideIndex, five a page.
Sint32 mnPsynergyMenuRunList(Sint32 sideIndex)
{
    PSYNERGY_MENU_STATE *menu;
    ABILITY_DEFINITION  *ability;
    WINDOW_SPR          *icon;
    WINDOW              *window;
    Bool                 changed;
    Sint32               moved;
    Sint8                listMode;
    Sint32               member;
    Uint8                i;
    Sint32               result;
    Bool                 redraw;
    Bool                 done;
    UNIT                *memberUnit;
    Sint32               prev;
    Sint32               shortcutHelp;
    Sint32               memberState[5];
    PAGED_LIST           state;

    menu                              = MenuState;
    result                            = 0;
    prev                              = 0;
    shortcutHelp                      = 0;
    menu->sideIcon[sideIndex]->motion = ICON_HIDDEN;
    mnWindowUpdateOrCreate(&menu->listWindow, 13, 3, 17, 14, WINDOW_FLAG_INSTANT);
    window = menu->listWindow;
    done   = FALSE;

    while (done == 0 && gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0)
    {
        memberUnit = gmUnitGetStub(menu->memberIds[sideIndex]);
        if (menu->mode != PSYNERGY_MODE_USE)
        {
            menu->psynergyCount =
                mnPsynergyMenuCollectActions(memberUnit, menu->psynergies, PSYNERGY_LIST_SHORTCUTS);
        }
        else
        {
            menu->psynergyCount =
                mnPsynergyMenuCollectActions(memberUnit, menu->psynergies, PSYNERGY_LIST_FIELD);
        }
        mnPsynergyMenuDrawPsynergyIcons(menu->psynergies);
        mnPsynergyMenuBuildPagedList(&state, sideIndex);
        redraw                            = TRUE;
        changed                           = TRUE;
        menu->sideIcon[sideIndex]->motion = ICON_SHOWN;

        while (gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0)
        {
            mnCursorPos(PSYNERGY_LIST_CURSOR_X, state.row * 16 + 36);

            if (changed != 0)
            {
                changed = FALSE;
                if (menu->psynergies[prev] != 0)
                {
                    icon = menu->entryIcons[prev];
                    mnIconShow(icon);
                }
                if (redraw != 0)
                {
                    redraw = FALSE;
                    sySynch(1);
                    mnPsynergyMenuDrawPage(window, 0, &state);
                }
                mnPsynergyMenuShowSelectedRow(window, memberState, &state);
                menu->sideAction[sideIndex] = menu->psynergies[state.selectedIndex];
                menu->cursorIcon->motion    = ICON_HIDDEN;
                if (menu->psynergies[state.selectedIndex] != 0)
                {
                    icon                = menu->entryIcons[state.selectedIndex];
                    // Swell the selected icon, in depth slot 250, as in mnItemMenuRunList.
                    icon->motion        = WINDOW_SPRITE_MOTION_SWELL;
                    icon->animationStep = 0;
                    icon->depth         = OAM_DEPTH_FRONT - 5;
                }
                for (i = 0; i < menu->memberCount; i++)
                {
                    fdSpriteStartAnimationStub(menu->tabSprites[i], ANIM_STAND);
                }
            }

            sySynch(1);
            prev = state.selectedIndex;

            if ((PerData & KEY_SELECT) == 0)
            {
                moved = mnPagedListHandleInput(
                    0, state.entryCount, LIST_PAGE_SIZE, &state.row, &state.page);
            }
            else
            {
                moved = -1;
            }
            if (moved == 1)
            {
                redraw  = TRUE;
                changed = TRUE;
            }
            if (moved == 0)
            {
                changed = TRUE;
            }
            if (moved == -1)
            {
                changed = FALSE;
            }

            // Select shows the shortcut help while held, for field Psynergy.
            if (menu->mode == PSYNERGY_MODE_USE)
            {
                if ((PerPush & KEY_SELECT) != 0 && shortcutHelp == 0)
                {
                    ability = gmAbilityGetDirectStub(menu->psynergies[state.selectedIndex]
                                                     & PSYNERGY_ABILITY_MASK);
                    if (ability->fieldPsynergy == 0)
                    {
                        sySoundPlayStub(SE_MENU_ERROR);
                    }
                    else
                    {
                        sySoundPlayStub(SE_NOTE_MID); // the Use sound
                        shortcutHelp  = 1;
                        menu->flags  |= PSYNERGY_MENU_SHORTCUT_HELP;
                        mnWindowClearInteriorTilesStub((void *)window, 0, 88, 120, 96);
                        mnTextDrawMessageAsTilesStub(MsgShortcutHelp, window, 0, 88);
                    }
                }
                if ((PerData & KEY_SELECT) == 0 && shortcutHelp == 1)
                {
                    shortcutHelp  = 0;
                    menu->flags  &= ~PSYNERGY_MENU_SHORTCUT_HELP;
                    mnWindowClearInteriorTilesStub((void *)window, 0, 88, 120, 96);
                    mnTextDrawMessageAsTilesStub(
                        MsgChangeCharacterHelp, window, CHANGE_CHARACTER_HELP_X, 88);
                }
            }

            // A: set the shortcut, or use the Psynergy if allowed and affordable.
            if ((PerPush & KEY_A) != 0)
            {
                if (menu->mode != PSYNERGY_MODE_USE)
                {
                    sySoundPlayStub(SE_PSYNERGY_START); // the shortcut sound
                    result = menu->psynergies[state.selectedIndex];
                    done   = TRUE;
                    break;
                }
                if (menu->psynergies[state.selectedIndex] != 0)
                {
                    if (mnPsynergyMenuIsActionRestricted(menu->psynergies[state.selectedIndex])
                        != 0)
                    {
                        sySoundPlayStub(SE_MENU_ERROR);
                        continue;
                    }
                    ability = gmAbilityGetDirectStub(menu->psynergies[state.selectedIndex]
                                                     & PSYNERGY_ABILITY_MASK);
                    if (ability->ppCost > memberUnit->pp)
                    {
                        sySoundPlayStub(SE_MENU_ERROR);
                    }
                    else
                    {
                        sySoundPlayStub(SE_NOTE_LOW); // the choice sound
                        result = menu->psynergies[state.selectedIndex];
                        done   = TRUE;
                        break;
                    }
                }
            }

            if ((PerPush & KEY_B) != 0)
            {
                sySoundPlayStub(SE_MENU_CANCEL);
                result = -1;
                done   = TRUE;
                break;
            }

            // L and R: the previous or next party member with Psynergy to list.
            if (((PerRepeat & KEY_R) != 0 || (PerRepeat & KEY_L) != 0)
                && (PerData & KEY_SELECT) == 0)
            {
                listMode =
                    menu->mode != PSYNERGY_MODE_USE ? PSYNERGY_LIST_SHORTCUTS : PSYNERGY_LIST_FIELD;
                sySoundPlayStub(SE_MENU_CURSOR_MOVE);
                menu->selectedIndexByMember[menu->memberIds[sideIndex]] = state.selectedIndex;
                member                                                  = menu->tabIndex[sideIndex];
                do
                {
                    if ((PerRepeat & KEY_R) != 0)
                    {
                        member++;
                    }
                    else
                    {
                        member--;
                    }
                    member               = (member + menu->memberCount) % menu->memberCount;
                    menu->selectedMember = menu->memberTable[member];
                    menu->memberIds[0]   = menu->memberTable[member];
                    menu->psynergyCount  = mnPsynergyMenuCollectActions(
                        gmUnitGetStub(menu->memberIds[0]), menu->psynergies, listMode);
                } while (menu->psynergyCount == 0);
                menu->tabIndex[sideIndex] = member;
                for (i = 0; i < 4; i++)
                {
                    menu->rowPositions[i] = MENU_MEMBER_Y;
                }
                menu->rowPositions[member] = MENU_MEMBER_RAISED_Y;
                mnStatusPanelDraw(
                    menu->statusWindow, menu->memberTable[member], 0, STATUS_PANEL_EXPERIENCE);
                mnReloadMemberPortrait(menu, menu->memberTable[member]);
                break;
            }

            // Select + L or R: make the field Psynergy that shortcut.
            if ((PerPush & KEY_L) != 0 && (PerData & KEY_SELECT) != 0)
            {
                ability = gmAbilityGetDirectStub(menu->psynergies[state.selectedIndex]
                                                 & PSYNERGY_ABILITY_MASK);
                if (ability->fieldPsynergy == 0)
                {
                    sySoundPlayStub(SE_MENU_ERROR);
                }
                else
                {
                    sySoundPlayStub(SE_PSYNERGY_START); // the shortcut sound
                    if (mnPsynergyMenuSetShortcut(menu->memberIds[sideIndex],
                                                  menu->psynergies[state.selectedIndex],
                                                  PSYNERGY_SHORTCUT_L)
                        != 0)
                    {
                        result     = menu->psynergies[state.selectedIndex];
                        menu->mode = PSYNERGY_MODE_SET_L;
                        done       = TRUE;
                        break;
                    }
                }
            }

            if ((PerPush & KEY_R) != 0 && (PerData & KEY_SELECT) != 0)
            {
                ability = gmAbilityGetDirectStub(menu->psynergies[state.selectedIndex]
                                                 & PSYNERGY_ABILITY_MASK);
                if (ability->fieldPsynergy == 0)
                {
                    sySoundPlayStub(SE_MENU_ERROR);
                }
                else
                {
                    if (mnPsynergyMenuSetShortcut(menu->memberIds[sideIndex],
                                                  menu->psynergies[state.selectedIndex],
                                                  PSYNERGY_SHORTCUT_R)
                        != 0)
                    {
                        // the shortcut sound
                        sySoundPlayStub(SE_PSYNERGY_START);
                        result     = menu->psynergies[state.selectedIndex];
                        menu->mode = PSYNERGY_MODE_SET_R;
                        done       = TRUE;
                        break;
                    }
                }
            }
        }
    }

    menu->flags &= ~PSYNERGY_MENU_SHORTCUT_HELP;
    mnIconShow(menu->entryGridCursor);
    menu->sideRow[sideIndex]                                = state.selectedIndex;
    menu->selectedIndexByMember[menu->memberIds[sideIndex]] = state.selectedIndex;
    menu->sideAction[sideIndex]                             = result;
    if (gmFlagGetStub(FLAG_MENU_START_PRESSED) != 0)
    {
        result = -1;
    }
    sySynch(1);
    return result;
}

// A Psynergy entry's action id, its PSYNERGY_ABILITY_MASK bits, taken out by shifts.
#define PSYNERGY_ABILITY_OF(entry) (((Uint32)(entry) << 18) >> 18)

// Returns 1 when actionEntry cannot be used from the menu: it has no field effect and is not usable
// both in the field and in battle.
Bool mnPsynergyMenuIsActionRestricted(Sint32 actionEntry)
{
    ABILITY_DEFINITION *action = gmAbilityGetDirectStub(PSYNERGY_ABILITY_OF(actionEntry));

    if (action->fieldPsynergy != 0
        || (action->flags & (ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE))
               == (ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE))
        return FALSE;
    return TRUE;
}

#undef PSYNERGY_PROMPT_TILES
#undef PSYNERGY_PROMPT_TILES_SIZE
#undef PSYNERGY_DETAIL_ROW_X
#undef PSYNERGY_DETAIL_ROW_WIDTH
#undef PSYNERGY_PAGE_ICONS_X
#undef PSYNERGY_PP_LABEL_X
#undef PSYNERGY_NAME_X
#undef PSYNERGY_LIST_CURSOR_X
#undef PSYNERGY_SHORTCUT_MEMBER_SHIFT
#undef PSYNERGY_SHORTCUT_ID_MASK
#undef ABILITY_DESCRIPTION_MASK
#undef PSYNERGY_ABILITY_OF
#undef PSYNERGY_MENU_BACK_TO_LIST
#undef PSYNERGY_MENU_SHORTCUT_HELP
