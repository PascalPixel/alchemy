// The party status window. Part of screens.c.

// UB: the standby counts pass four of mnWindowPutGlyph's five arguments, so its mode is whatever
// the stack slot of the fifth holds; UBFIX passes 0, as the summon lists do.
#if FIX_FUNCTION_SIGNATURES
#    define PARTY_STATUS_PUT_DIGIT(window, glyph, x, y)                                            \
        mnWindowPutGlyph(window, glyph, x, y, GLYPH_DRAW_CHARACTER)
#else
#    define PARTY_STATUS_PUT_DIGIT(window, glyph, x, y) mnWindowPutGlyph(window, glyph, x, y)
#endif

// The party status window's columns, in tiles: one for each party member, and the Djinn column on
// their left (PARTY_STAT_FLAG, battle_status.h).
#define PARTY_STATUS_COLUMN_WIDTH 6
#define PARTY_STATUS_DJINN_WIDTH  5

// The address of pixel row 7 - `up` of BG tile `tile` (4bpp, 32 bytes a tile, 4 bytes a row): the
// status bars are drawn from a tile's bottom row up.
#define TILE_ROW_FROM_BOTTOM(tile, up)                                                             \
    ((Uint32 *)((tile) * 32 - (up) * 4 + ((Sint32)BG_CHAR_BLOCK(0) + 7 * 4)))

// Works out where the party status window goes for `flags`: one column per party member (those in
// battle when a battle is on), plus the Djinn column, right-aligned on the 30-tile screen row at
// the top.
void mnPartyStatusBuildLayoutBounds(Sint32 flags)
{
    PARTY_STAT_WINDOW *layout = HEAP_SLOT_BLOCK(HEAP_SLOT_PARTY_STATUS);
    WIN_STATE         *state  = WINDOW_STATE;
    Sint32             height = 4;
    Sint32             members;
    Sint32             width;
    Sint32             x;

    if (state->battleMode != WINDOW_FIELD)
    {
        members = btPartyPrepareMembersStub(0);
        height  = 3;
    }
    else
    {
        members = gmPartyCountMembersStub();
    }
    if (flags & PARTY_STATUS_SHOW_PP)
        height++;
    else
        flags &= ~PARTY_STATUS_SHOW_DJINN;

    members *= PARTY_STATUS_COLUMN_WIDTH;
    width    = members + 1;
    if (flags & PARTY_STATUS_SHOW_DJINN)
        width += PARTY_STATUS_DJINN_WIDTH;

    x               = DISPLAY_TILE_WIDTH;
    x              -= width;
    layout->x       = x;
    layout->y       = 0;
    layout->width   = width;
    layout->height  = height;
    layout->flags   = flags;
}

// Opens the party status window with `flags` (see PARTY_STATUS_*) in a new heap block,
// HEAP_SLOT_PARTY_STATUS, which mnPartyStatusClose frees, and draws it; the tilemap upload is held
// back (holdTilemapUpload) while it is drawn.
void mnPartyStatusCreateWindow(Sint32 flags)
{
    PARTY_STAT_WINDOW *layout;
    Uint8             *hold;
    Sint32             released;

    layout   = syHeapSlotAllocateEWRAM(HEAP_SLOT_PARTY_STATUS, sizeof(*layout));
    hold     = &WINDOW_STATE->holdTilemapUpload;
    released = 0;
    *hold    = 1;
    mnPartyStatusBuildLayoutBounds(flags);
    layout->window =
        mnWindowCreate(layout->x, layout->y, layout->width, layout->height, MENU_WINDOW_FLAGS);
    mnPartyStatusDraw(flags);
    *hold = released;
}

// Draws the vertical lines between the party members' columns of the party status `window`, with
// their top and bottom joints, at the offsets listed in WindowPartyColumnOffsets (shifted right
// past the Djinn column, whose own line comes first in the list).
void mnWindowDrawColumnBorders(WINDOW *window, Uint32 flags)
{
    WIN_STATE *state      = WINDOW_STATE;
    Sint32     first      = 1;
    Sint32     shift      = 0;
    Uint32     lastColumn = window->width - 1;
    Sint32     i;
    Uint32     rows = window->height;
    Uint32     column;
    Uint32     row;
    Uint16    *cell;

    if ((flags & PARTY_STATUS_SHOW_PP) == 0)
        flags &= ~PARTY_STATUS_SHOW_DJINN;
    if (flags & PARTY_STATUS_SHOW_DJINN)
    {
        shift = PARTY_STATUS_DJINN_WIDTH;
        first = 0;
    }
    for (i = first; WindowPartyColumnOffsets[i] >= 0; i++)
    {
        column = WindowPartyColumnOffsets[i] + shift;
        if (column < lastColumn)
        {
            for (row = 0; row != rows; row++)
            {
                cell = (Uint16 *)(((window->y + row) * BG_SCREEN_WIDTH + (window->x + column)) * 2
                                  + (Uint32)state->tilemap);
                if (row == 0)
                    *cell = WINDOW_TILE_BORDER_TOP_TEE;
                else if (row == rows - 1)
                    *cell = WINDOW_TILE_BORDER_BOTTOM_TEE;
                else
                    *cell = WINDOW_TILE_DIVIDER_VERTICAL;
            }
        }
    }
    if (state->battleMode)
    {
        cell    = state->tilemap + (window->y + window->height - 1) * BG_SCREEN_WIDTH + window->x;
        *cell++ = WINDOW_TILE_BATTLE_BORDER_BOTTOM_LEFT;
        for (column = 1; column < lastColumn; column++)
        {
            *cell++ = WINDOW_TILE_BATTLE_BORDER_BOTTOM;
        }
        *cell = WINDOW_TILE_BATTLE_BORDER_BOTTOM_RIGHT;
    }
    state->dirty = 1;
}

// An HP or PP bar at full, in pixels: five tiles.
#define PARTY_STATUS_BAR_LENGTH 40
// The colours of the bar tiles that the bar recolours: its body and its shadow.
#define BAR_TILE_COLOR          14
#define BAR_TILE_SHADOW_COLOR   1
// The colours they become, in HIGHLIGHT_PALETTE: the filled part of the bar and its shadow, and the
// empty part and its shadow.
#define BAR_FILLED_COLOR        8
#define BAR_FILLED_SHADOW_COLOR 13
#define BAR_EMPTY_COLOR         2
#define BAR_EMPTY_SHADOW_COLOR  12
// A 4bpp tile row: eight pixels of four bits, one word.
#define TILE_ROW_BITS           32

// Recolours the five bar tiles at (x, y) in `window` to show a bar `length` pixels long (0 to
// PARTY_STATUS_BAR_LENGTH).
UNREAD_RESULT mnPartyStatusDrawBars(WINDOW *window, Sint32 x, Sint32 y, Sint32 length)
{
    Sint32     i;
    WIN_STATE *state      = WINDOW_STATE;
    Sint32     fullLength = length;
    Sint32     row;
    Sint32     column;
    Uint32     tile;
    Uint32     bar;
    Uint32     shadow;
    Uint32     pixels;
    Uint32     recoloured;

    if (state->battleMode == WINDOW_FIELD)
    {
        syDMAXCopy(syPaletteGetBasicSpriteColors(),
                   (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
                   DMA_COPY_HALFWORDS(16),
                   REG_DMA3);
        BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR) =
            BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
    }
    x += window->x;
    y += window->y;
    for (i = 0; i < PARTY_STATUS_BAR_LENGTH / 8; i++, x++)
    {
        // This tile's eight pixels: filled up to `length`, empty after.
        tile    = state->tilemap[y * BG_SCREEN_WIDTH + x];
        bar     = PIXELS_4BPP(BAR_EMPTY_COLOR);
        shadow  = PIXELS_4BPP(BAR_EMPTY_SHADOW_COLOR);
        tile   &= TILE_NUMBER_MASK;
        if (length > 7)
        {
            bar    = PIXELS_4BPP(BAR_FILLED_COLOR);
            shadow = PIXELS_4BPP(BAR_FILLED_SHADOW_COLOR);
            // UB: at a length of zero the shift below is by 32, which the GBA takes as zero.
#if FIX_UNDEFINED_SHIFT
        }
        else if (length > 0)
        {
#else
        }
        else if (length >= 0)
        {
#endif
            bar    <<= length * 4;
            bar     |= PIXELS_4BPP(BAR_FILLED_COLOR) >> (TILE_ROW_BITS - length * 4);
            shadow <<= length * 4;
            shadow  |= PIXELS_4BPP(BAR_FILLED_SHADOW_COLOR) >> (TILE_ROW_BITS - length * 4);
        }
        for (row = 0; row <= (fullLength != 0 ? 2 : 0); row++)
        {
            pixels     = *TILE_ROW_FROM_BOTTOM(tile, row);
            recoloured = 0;
            for (column = 0; column <= 7; column++)
            {
                Uint32 colour = pixels & 0xf;

                if (colour == BAR_TILE_COLOR)
                    recoloured |= bar & (0xfU << (column * 4));
                else if (colour == BAR_TILE_SHADOW_COLOR)
                    recoloured |= shadow & (0xfU << (column * 4));
                else
                    recoloured |= colour << (column * 4);
                pixels >>= 4;
            }
            *TILE_ROW_FROM_BOTTOM(tile, row) = recoloured;
        }
        length -= 8;
    }
}

// Draws the party status window with `flags` (see PARTY_STATUS_*): each party member's name, HP and
// optionally PP with their bars, and in battle the standby Djinn of each element.
void mnPartyStatusDraw(Sint32 flags)
{
    PARTY_STAT_WINDOW *layout = HEAP_SLOT_BLOCK(HEAP_SLOT_PARTY_STATUS);
    BATTLE_STATE      *battle = BattleStateWork;
    WINDOW            *window = layout->window;
    WIN_STATE         *state  = WINDOW_STATE;
    Sint32             x      = 0;
    Uint32             count;
    Sint32             y = 0;
    Uint32             i;
    Uint16             members[5];
    Uint8              standbyDjinn[4];

    // List the party members shown: those fighting in battle, the party otherwise.
    if (state->battleMode != WINDOW_FIELD)
    {
        count = btPartyPrepareMembersStub(0);
        y     = -1;
        for (i = 0; i < count; i++)
        {
            members[i] = battle->partyUnits[i];
            if (members[i] == BATTLE_UNIT_LIST_END)
                break;
        }
    }
    else
    {
        count = gmPartyCountMembersStub();
        for (i = 0; i < count; i++)
            members[i] = MainRoutineStateWork.party[i];
        members[i] = BATTLE_UNIT_LIST_END;
    }
    count = i;

    if (flags == PARTY_STATUS_KEEP_FLAGS)
        flags = layout->flags;
    if (!(flags & PARTY_STATUS_SHOW_PP))
        flags &= ~PARTY_STATUS_SHOW_DJINN;
    if (state->battleMode == WINDOW_FIELD || btDjinnCountStandbyStub(0, 0) == 0)
        flags &= ~PARTY_STATUS_SHOW_DJINN;
    if (flags == PARTY_STATUS_CLOSED)
    {
        mnWindowEraseBorderRect(layout->x, layout->y, layout->width, layout->height);
        return;
    }

    // Redraw the tick, laying it out again when the flags changed.
    state->holdTilemapUpload = 1;
    if (layout->flags == flags)
    {
        mnWindowSpriteRedrawSavedRect(window);
        mnWindowDrawColumnBorders(window, flags);
    }
    else
    {
        mnWindowEraseBorderRect(layout->x, layout->y, layout->width, layout->height);
        mnPartyStatusBuildLayoutBounds(flags);
        window->width  = layout->width;
        window->height = layout->height;
        window->x      = layout->x;
        mnWindowDrawBorder(layout->x, layout->y, layout->width, layout->height);
        mnWindowDrawColumnBorders(window, flags);
    }

    // Each party member's column: name, HP and PP, and their bars.
    if (flags & PARTY_STATUS_SHOW_DJINN)
        x = PARTY_STATUS_DJINN_WIDTH;
    for (i = 0; i != count; i++)
    {
        UNIT  *unit  = gmUnitGetStub(members[i]);
        Sint32 hp    = unit->hp;
        Sint32 hpMax = unit->hpMax;
        Sint32 barLength;
        Sint32 current;

        if (hp == 0)
            mnTextSetColor(TEXT_COLOR_RED);
        else if (hp <= hpMax / 4)
            mnTextSetColor(TEXT_COLOR_YELLOW);
        else
            mnTextSetColor(TEXT_COLOR_WHITE);
        state->textPalette = HIGHLIGHT_PALETTE;
        if (state->battleMode != WINDOW_FIELD)
            state->textPalette = BASIC_COLORS_PALETTE;
        mnTextDrawPrefixedNumberAtOffset(
            hp, window, (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8 + 8, 0);
        state->textPalette = WINDOW_PALETTE;
        mnTextDrawStringAtOffset(
            unit->name, window, (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8);
        mnTextSetColor(TEXT_COLOR_WHITE);
        if (unit->hpMax != 0)
        {
            // A party member with any HP left shows at least one pixel of bar.
            current   = unit->hp;
            barLength = current * PARTY_STATUS_BAR_LENGTH / unit->hpMax;
            if (barLength == 0 && current != 0)
                barLength = 1;
            mnPartyStatusDrawBars(window, x + i * PARTY_STATUS_COLUMN_WIDTH + 1, y + 2, barLength);
        }
        if (flags & PARTY_STATUS_SHOW_PP)
        {
            state->textPalette = HIGHLIGHT_PALETTE;
            if (state->battleMode != WINDOW_FIELD)
                state->textPalette = BASIC_COLORS_PALETTE;
            mnTextDrawPrefixedNumberAtOffset(
                unit->pp, window, (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8 + 16, 1);
            if (unit->ppMax != 0)
            {
                current   = unit->pp;
                barLength = current * PARTY_STATUS_BAR_LENGTH / unit->ppMax;
                if (barLength == 0 && current != 0)
                    barLength = 1;
                mnPartyStatusDrawBars(
                    window, x + i * PARTY_STATUS_COLUMN_WIDTH + 1, y + 3, barLength);
            }
        }
    }
    state->textPalette = WINDOW_PALETTE;

    // The Djinn column: each element's icon and its count of standby Djinn.
    if (state->battleMode != WINDOW_FIELD && (flags & PARTY_STATUS_SHOW_DJINN))
    {
        Sint32 row = y;

        if (flags & PARTY_STATUS_SHOW_PP)
            row++;
        btDjinnCountStandbyStub(0, standbyDjinn);
        mnWindowSetTilemapEntry(window, LIST_TILE_SUMMON_ELEMENT + ELEMENT_VENUS, 0, row, 0);
        mnWindowSetTilemapEntry(window, LIST_TILE_SUMMON_ELEMENT + ELEMENT_MERCURY, 2, row, 0);
        mnWindowSetTilemapEntry(window, LIST_TILE_SUMMON_ELEMENT + ELEMENT_MARS, 0, row + 1, 0);
        mnWindowSetTilemapEntry(window, LIST_TILE_SUMMON_ELEMENT + ELEMENT_JUPITER, 2, row + 1, 0);
        PARTY_STATUS_PUT_DIGIT(window, standbyDjinn[0] + '0', 1, row);
        PARTY_STATUS_PUT_DIGIT(window, standbyDjinn[1] + '0', 3, row);
        PARTY_STATUS_PUT_DIGIT(window, standbyDjinn[2] + '0', 1, row + 1);
        PARTY_STATUS_PUT_DIGIT(window, standbyDjinn[3] + '0', 3, row + 1);
    }
    state->holdTilemapUpload = 0;
}

// Closes the party status window and frees its heap block.
void mnPartyStatusClose(void)
{
    mnWindowClose(((PARTY_STAT_WINDOW *)WindowStateWork[1])->window, 1);
    syHeapSlotRelease(HEAP_SLOT_PARTY_STATUS);
}

// Unused: sets (`set` 1) or clears (0) the lowest bit of the palette number of the tilemap entries
// of the `width` x `height` tile rectangle at (x, y) inside `window`, clipped to the screen, so
// switching the rectangle between two palettes, such as WINDOW_PALETTE and HIGHLIGHT_PALETTE.
UNUSED void mnWindowSetInteriorPaletteBit(
    const WINDOW *window, Sint32 x, Sint32 y, Sint32 width, Sint32 height, Uint32 set)
{
    WIN_STATE *state  = WINDOW_STATE;

    x                += window->x + 1;
    y                += window->y + 1;
    set              &= 1;
    set               = BG_TILE_PALETTE(set);
    if (x < 0)
    {
        width += x;
        x      = 0;
    }
    if (x + width > DISPLAY_TILE_WIDTH - 1)
    {
        width = DISPLAY_TILE_WIDTH - x;
    }
    if (y < 0)
    {
        height += y;
        y       = 0;
    }
    // BUG: tests the bottom against the screen's width, so a window reaching below the screen draws
    // into the map's unseen rows.
#if FIX_RECT_BOTTOM_CLIP
    if (y + height > DISPLAY_TILE_HEIGHT - 1)
    {
#else
    if (y + height > DISPLAY_TILE_WIDTH - 1)
    {
#endif
        height = DISPLAY_TILE_HEIGHT - y;
    }
    if (width > 0 && height > 0)
    {
        // The byte offset of the first cell: 32 entries of 2 bytes a row.
        y <<= 6;
        x   = y + (x << 1);
        do
        {
            Uint16 *cell      = (Uint16 *)(x + (Uint32)state->tilemap);
            Sint32  remaining = width;
            while (remaining != 0)
            {
                Uint32 entry  = *cell;
                entry        &= ~BG_TILE_PALETTE(1);
                entry        |= set;
                remaining--;
                *cell = entry;
                cell++;
            }
            height--;
            x += 64;
        } while (height != 0);
        state->dirty = 1;
    }
}

#undef PARTY_STATUS_PUT_DIGIT
