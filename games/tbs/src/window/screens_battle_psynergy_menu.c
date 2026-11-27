// The battle Psynergy menu. Part of screens.c.

// The battle Psynergy list for unit `unitId`: `count` abilities of `actions` (ended by 0), five to
// a page, each with its icon, name, PP cost, element and range icon, in TEXT_COLOR_YELLOW when not
// usable in battle, TEXT_COLOR_RED when the unit's PP fall short and TEXT_COLOR_PINK while its
// Psynergy is sealed; the window above describes the one under the cursor.
Sint32 mnBattlePsynergyMenuRun(Sint32 unitId, Uint16 *actions, Sint32 count)
{
    WIN_STATE         *state;
    Sint32             drawnFirstEntry;
    Sint32             firstEntry;
    Sint32             drawnRow;
    Sint32             cursorHandle;
    UNIT              *unit;
    WINDOW            *msgWindow;
    Sint32             iconCount;
    Sint32             preferredRow;
    MENU_POINT         cursorPos;
    BATTLE_MENU_STATE *navigation;
    WINDOW            *window;
    Sint32             selectedRow;
    Sint32             encodedAction;
    Sint32             iconTile;
    Sint32             rangePattern;
    Sint32             result;
    Sint32             i;
    MENU_SPR           cursor;
    MENU_SPR          *iconSprite;
    Uint16             msgText[64];
    Sint32             iconSlots[BATTLE_LIST_ROWS];
    MENU_SPR           sprites[BATTLE_LIST_ROWS];
    Sint32             glyph;

    state           = WINDOW_STATE;
    drawnRow        = -1;
    drawnFirstEntry = -1;
    cursorHandle    = syVRAMCacheAllocateEntry(128);
    unit            = gmUnitGetStub(unitId);
    msgWindow       = mnWindowCreate(
        0, 5, 30, 4, WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS | WINDOW_FLAG_BORDERED_CANVAS);
    iconCount = BATTLE_LIST_ROWS;
    BATTLE_MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
    window = mnWindowCreate(9, 9, 21, BATTLE_LIST_ROWS * 2 + 1, MENU_WINDOW_FLAGS);

    for (i = 0; i < BATTLE_LIST_ROWS; i++)
    {
        Sint32 y   = i * 2;

        iconSprite = &sprites[i];
        MENU_SPR_INIT(sprites[i], OBJ_SIZE_16X16);
        iconSprite->oam.f.x = window->x * 8 + 8;
        iconSprite->oam.f.y = (y + window->y) * 8 + 4;
    }

    // Reserve one glyph slot per visible row; paging reloads these same slots.
    for (i = 0; i < BATTLE_LIST_ROWS; i++)
    {
        iconSlots[i]          = syVRAMCacheAllocateEntry(128);
        sprites[i].oam.f.tile = syVRAMCacheReload(iconSlots[i], -1);
    }

    // The border's tees go into tiles 0, 1, 16 and 17 of the second text range, which nothing shows
    // here: in battle they are BG1's 8bpp tiles COMMAND_ROW_ICON_TILE and COMMAND_ROW_ICON_TILE +
    // 8, the first command icon's top-left and bottom-left, and BG1 is off while a list is open.
    // The command row draws its icons there again when it opens next.
    mnVRAMCopyTile(WINDOW_TILE_BORDER_TOP_TEE, TEXT_EXTRA_TILE_FIRST);
    mnVRAMCopyTile(WINDOW_TILE_BORDER_TOP_TEE, TEXT_EXTRA_TILE_FIRST + 1);
    mnVRAMCopyTile(WINDOW_TILE_BORDER_BOTTOM_TEE, TEXT_EXTRA_TILE_FIRST + 16);
    mnVRAMCopyTile(WINDOW_TILE_BORDER_BOTTOM_TEE, TEXT_EXTRA_TILE_FIRST + 17);

    iconSprite = &cursor;
    for (;;)
    {
        if (firstEntry != drawnFirstEntry || selectedRow != drawnRow)
        {
            state->holdTilemapUpload = 1;
            mnWindowHighlightRect(
                window->x + 1, window->y + drawnRow * 2 + 1, window->width - 2, 1, WINDOW_PALETTE);
            mnTextFillCanvasBackground();
            if (count != 0)
            {
                BATTLE_MENU_MESSAGE_COPY((actions[firstEntry + selectedRow] & ACTION_ID_MASK)
                                             + MsgAbilityDescription,
                                         msgText,
                                         BATTLE_MENU_MESSAGE_CAPACITY);
            }
            else
            {
                BATTLE_MENU_MESSAGE_COPY(
                    MSGNO(MsgBattleNoPsynergy), msgText, BATTLE_MENU_MESSAGE_CAPACITY);
            }
            mnTextDrawWideStringAtOffset(msgText, msgWindow, 0, 4);
            drawnRow = selectedRow;

            // Cursor moves update help; only a page change reloads the row icons.
            if (firstEntry != drawnFirstEntry)
            {
                mnWindowSpriteRedrawSavedRect(window);
                for (i = 0; i < BATTLE_LIST_ROWS; i++)
                {
                    ABILITY_DEFINITION *action;

                    encodedAction = actions[firstEntry + i];
                    if (encodedAction == 0)
                    {
                        break;
                    }
                    action = gmAbilityGetDirectStub(encodedAction);
                    mnWindowSetTilemapEntry(window, PSYNERGY_COST_FIRST_TILE, 11, i * 2, 0);
                    mnWindowSetTilemapEntry(window, PSYNERGY_COST_SECOND_TILE, 12, i * 2, 0);
                    mnAbilityBuildIconTiles(
                        encodedAction & ACTION_ID_MASK, 0, &iconSlots[i], &glyph, 1);
                    sprites[i].oam.f.tile = glyph;

                    if ((action->flags & ABILITY_USABLE_IN_BATTLE) == 0)
                    {
                        mnTextSetColor(TEXT_COLOR_YELLOW);
                    }
                    else if (action->ppCost > unit->pp)
                    {
                        mnTextSetColor(TEXT_COLOR_RED);
                    }
                    else if (unit->psySeal != 0)
                    {
                        mnTextSetColor(TEXT_COLOR_PINK);
                    }

                    state->textPalette = BASIC_COLORS_PALETTE;
                    mnTextDrawMessageAsTiles(encodedAction + MsgAbilityName, window, 16, i * 16);
                    mnTextDrawNumberAtOffset(action->ppCost, 2, window, 104, i * 16);
                    mnTextSetColor(TEXT_COLOR_WHITE);
                    state->textPalette = WINDOW_PALETTE;

                    if (action->element != ELEMENT_NONE)
                    {
                        mnWindowSetTilemapEntry(
                            window, action->element + LIST_TILE_SUMMON_ELEMENT, 15, i * 2, 0);
                    }

                    rangePattern = action->range;
                    if (rangePattern == ABILITY_RANGE_ALL)
                    {
                        rangePattern = RANGE_ICON_ALL;
                    }
                    else
                    {
                        rangePattern--;
                    }
                    mnRangeIconDraw(window, 16, i * 2, rangePattern, 0);
                }
                iconCount       = i;
                drawnFirstEntry = firstEntry;
            }

            if (count > BATTLE_LIST_ROWS)
            {
                for (i = 0; i < (count + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS; i++)
                {
                    iconTile = i + PAGE_NUMBER_TILE;
                    if (i == firstEntry / BATTLE_LIST_ROWS)
                    {
                        iconTile = i + PAGE_NUMBER_LIT_TILE;
                    }
                    mnWindowSetTilemapEntry(
                        window,
                        iconTile,
                        window->width - (count + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS + i - 2,
                        -1,
                        0);
                }
            }

            mnWindowHighlightRect(window->x + 1,
                                  window->y + selectedRow * 2 + 1,
                                  window->width - 2,
                                  1,
                                  HIGHLIGHT_PALETTE);
            state->dirty             = 1;
            state->holdTilemapUpload = 0;
        }

        // Each tick: the current page's number lit 12 ticks in 16, and the page arrows on the
        // window's top edge (y -1).
        if (count > BATTLE_LIST_ROWS)
        {
            for (i = 0; i < (count + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS; i++)
            {
                iconTile = i + PAGE_NUMBER_TILE;
                if ((TickCount & 15) <= 11)
                {
                    if (i == firstEntry / BATTLE_LIST_ROWS)
                    {
                        iconTile = i + PAGE_NUMBER_LIT_TILE;
                    }
                }
                mnWindowSetTilemapEntry(
                    window,
                    iconTile,
                    window->width - (count + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS + i - 2,
                    -1,
                    0);
            }
            mnWindowSetTilemapEntry(window,
                                    LIST_TILE_PAGES_LEFT,
                                    window->width
                                        - (count + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS - 3,
                                    -1,
                                    0);
            mnWindowSetTilemapEntry(window, LIST_TILE_PAGES_RIGHT, window->width - 2, -1, 0);
            mnWindowMarkTilemapRow(state, window->y - 1);
        }

        for (i = 0; i < iconCount; i++)
        {
            syOAMAddSpriteAtDepth(&sprites[i], OAM_DEPTH_WINDOW);
        }

        MENU_PLACE_CURSOR(cursorPos, window, selectedRow, 4, iconSprite);
        iconSprite->oam.f.tile = syVRAMCacheReload(cursorHandle, (Sint32)PointingHandTiles);
        iconSprite->oam.f.x    = cursorPos.x + ((TickCount & 4) >> 1) - 4;
        iconSprite->oam.f.y    = cursorPos.y - ((TickCount & 4) >> 2) - 8;
        if (count != 0)
        {
            syOAMAddSpriteAtDepth(iconSprite, OAM_DEPTH_WINDOW + 2);
        }

        BATTLE_MENU_SET_CURSOR(firstEntry, selectedRow, preferredRow);
        navigation = BattleMenuStateWork;

        if ((PerPush & KEY_A) != 0)
        {
            if (count != 0)
            {
                result = firstEntry + selectedRow;
                if ((gmAbilityGetDirectStub(actions[result])->flags & ABILITY_USABLE_IN_BATTLE)
                    == 0)
                {
                    // Not usable in battle: the menu stays open.
                }
                else
                {
                    break;
                }
            }
            else
            {
                result = -1;
                break;
            }
        }
        else if (navigation->timer == 0 || (PerPush & KEY_B) != 0)
        {
            sySoundPlayStub(SE_MENU_CANCEL);
            result = -1;
            break;
        }

        if (count != 0)
        {
            if ((PerRepeat & KEY_DOWN) != 0)
            {
                sySoundPlayStub(SE_MENU_CURSOR_MOVE);
                selectedRow++;
                if (selectedRow == BATTLE_LIST_ROWS || firstEntry + selectedRow == count)
                {
                    selectedRow = 0;
                }
                preferredRow = selectedRow;
            }
            else if ((PerRepeat & KEY_UP) != 0)
            {
                sySoundPlayStub(SE_MENU_CURSOR_MOVE);
                selectedRow--;
                if (selectedRow < 0)
                {
                    if (firstEntry == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS)
                    {
                        selectedRow = count - firstEntry - 1;
                    }
                    else
                    {
                        selectedRow = BATTLE_LIST_ROWS - 1;
                    }
                }
                preferredRow = selectedRow;
            }
            else if ((PerRepeat & KEY_RIGHT) != 0)
            {
                sySoundPlayStub(SE_MENU_CURSOR_MOVE);
                syInputResetKeyRepeat();
                if (firstEntry + BATTLE_LIST_ROWS >= count)
                {
                    if (firstEntry != 0)
                    {
                        firstEntry  = 0;
                        selectedRow = preferredRow;
                    }
                }
                else
                {
                    firstEntry  += BATTLE_LIST_ROWS;
                    selectedRow  = preferredRow;
                    if (firstEntry == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS)
                    {
                        selectedRow = count - firstEntry - 1;
                        if (selectedRow > preferredRow)
                        {
                            selectedRow = preferredRow;
                        }
                    }
                }
            }
            else if ((PerRepeat & KEY_LEFT) != 0)
            {
                sySoundPlayStub(SE_MENU_CURSOR_MOVE);
                syInputResetKeyRepeat();
                if (firstEntry != 0)
                {
                    selectedRow  = preferredRow;
                    firstEntry  -= BATTLE_LIST_ROWS;
                }
                else
                {
                    firstEntry  = (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS;
                    selectedRow = preferredRow;
                    if (firstEntry != 0)
                    {
                        selectedRow = count - firstEntry - 1;
                        if (selectedRow > preferredRow)
                        {
                            selectedRow = preferredRow;
                        }
                    }
                }
            }
        }
        sySynch(1);
    }

    mnWindowClose(msgWindow, 1);
    mnWindowClose(window, 1);
    sySynch(1);
    for (i = 0; i < BATTLE_LIST_ROWS; i++)
    {
        syVRAMCacheFreeEntry(iconSlots[i]);
    }
    syVRAMCacheFreeEntry(cursorHandle);
    sySynch(1);
    return result;
}
