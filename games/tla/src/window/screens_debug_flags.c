// The debug menus' keys, the flag browser, and hexadecimal numbers written as text. Part of
// screens.c.

// The debug destination menu's range: its scene runs 0 to DEBUG_SCENE_MAX, its entrance 0 to
// DEBUG_ENTRANCE_MAX.
#define DEBUG_SCENE_MAX     324
#define DEBUG_ENTRANCE_MAX  99

// The flag browser's pages of DEBUG_FLAG_ROWS rows of DEBUG_FLAGS_PER_ROW flags, 256 a page: every
// game flag.
#define DEBUG_FLAG_PAGES    16
#define DEBUG_FLAG_ROWS     16
#define DEBUG_FLAGS_PER_ROW 16

// Edits the debug scene or entrance, returning -1 to exit and -2 to cancel, else the scene after
// this tick's keys. Up and Down switch the field (`field`: 0 scene, 1 entrance), Left and Right
// step by one and wrap at either end. R and L step a scene by DEBUG_LARGE_STEP, setting the
// entrance to 0, and past either end go to the other; they step an entrance as far, wrapping by
// DEBUG_ENTRANCE_MAX (95 + 10 gives 6).
Sint32
mnDebugSceneSelectHandleRowInput(Sint32 window, Sint16 scene, Sint16 *entrance, Sint16 *field)
{
    if (PerWork.repeat & KEY_A)
        return -1;
    if (PerWork.repeat & KEY_B)
        return -2;
    if (PerWork.repeat & KEY_DOWN)
    {
        *field ^= 1;
    }
    else if (PerWork.repeat & KEY_UP)
    {
        *field ^= 1;
    }
    else if (PerWork.repeat & KEY_RIGHT)
    {
        if (*field == 0)
        {
            scene++;
        }
        else
        {
            (*entrance)++;
            if (*entrance > DEBUG_ENTRANCE_MAX)
                *entrance = 0;
        }
        if (scene > DEBUG_SCENE_MAX)
            scene = 0;
        mnDebugSceneSelectDrawRow(window, scene, entrance);
    }
    else if (PerWork.repeat & KEY_LEFT)
    {
        if (*field == 0)
        {
            scene--;
        }
        else
        {
            (*entrance)--;
            if (*entrance < 0)
                *entrance = DEBUG_ENTRANCE_MAX;
        }
        if (scene < 0)
            scene = DEBUG_SCENE_MAX;
        mnDebugSceneSelectDrawRow(window, scene, entrance);
    }
    else if (PerWork.repeat & KEY_R)
    {
        if (*field == 0)
        {
            *entrance  = 0;
            scene     += DEBUG_LARGE_STEP;
        }
        else
        {
            *entrance += DEBUG_LARGE_STEP;
            if (*entrance > DEBUG_ENTRANCE_MAX)
                *entrance -= DEBUG_ENTRANCE_MAX;
        }
        if (scene > DEBUG_SCENE_MAX)
            scene = 0;
        mnDebugSceneSelectDrawRow(window, scene, entrance);
    }
    else if (PerWork.repeat & KEY_L)
    {
        if (*field == 0)
        {
            *entrance  = 0;
            scene     -= DEBUG_LARGE_STEP;
        }
        else
        {
            *entrance -= DEBUG_LARGE_STEP;
            if (*entrance < 0)
                *entrance += DEBUG_ENTRANCE_MAX;
        }
        if (scene < 0)
            scene = DEBUG_SCENE_MAX;
        mnDebugSceneSelectDrawRow(window, scene, entrance);
    }
    return scene;
}

// The flag browser: shows a page of flags and takes input until closed, starting on page 8. Returns
// 0.
Sint32 mnDebugFlagEditorRun(void)
{
    MENU_SPR cursorSprite;
    Sint32   cursor[2]; // column and row
    Sint32   cursorSlot;
    Sint32   page;
    WINDOW  *window;
    Sint32   change;

    page      = 8;
    window    = 0;
    cursor[0] = 0;
    cursor[1] = 0;
    window    = mnWindowCreate(1, 0, 28, 20, WINDOW_FLAG_INSTANT);
    mnDebugFlagEditorDrawTable(window, page);
    mnArrowCreate(&cursorSprite, &cursorSlot);
    for (;;)
    {
        sySynch(1);
        change = mnDebugFlagEditorHandleInput(window, &page, cursor);
        // B closes the table; a change of flag or page redraws it.
        if (change == -1)
            break;
        if (change == 1)
            mnDebugFlagEditorDrawTable(window, page);
        mnArrowSetPos(&cursorSprite, cursor[0] * 8 + 58, cursor[1] * 8 + 20);
    }
    mnArrowRelease(cursorSlot);
    mnWindowClose(window, 2);
    return 0;
}

// Writes the lowest count digits of value (up to five), most significant first, to out as uppercase
// hexadecimal, without a terminating zero.
void mnStringFormatHexDigits(Uint32 value, Uint32 count, Uint8 *out)
{
    Uint8  digits[8];
    Uint32 i;

    if (count > 5)
        count = 5;

    for (i = 0; i != count; i++)
    {
        Uint32 digit = value & 0xf;
        if (digit <= 9)
            digits[i] = digit + '0';
        else
            digits[i] = digit + 'A' - 10;
        value >>= 4;
    }

    for (i = count - 1; (Sint32)i >= 0; i--)
        *out++ = digits[i];
}

// Draws one page of 256 flags as sixteen rows of hexadecimal labels and bits. Each row: its first
// flag's number in three hex digits, then its 16 flags as 0 or 1, lowest first, under a heading of
// hex digits.
void mnDebugFlagEditorDrawTable(Sint32 window, Sint32 page)
{
    Sint32 row;
    Sint32 y;
    Sint32 flag;
    Uint8  label[5];
    char   bits[17];

    // UB: called with no argument; the window it redraws is this one, left where the argument would
    // be. The ROM's code makes the call with no declaration in scope; the cast to an unprototyped
    // function keeps it so.
#if FIX_FUNCTION_SIGNATURES
    mnWindowSpritePrepareForRedraw(window);
#else
    ((void (*)())mnWindowSpritePrepareForRedraw)();
#endif
    mnTextDrawStringInWindow((Sint32)MenuHexDigitsString, window, 48, 0);

    flag = page << 8;
    for (row = 0; row != DEBUG_FLAG_ROWS; row++)
    {
        Sint32 i;

        y = row * 8 + 16;

        for (i = 0; i != 5; i++)
        {
            label[i] = 0;
        }
        mnStringFormatHexDigits(flag, 3, label);
        mnTextDrawStringInWindow((Sint32)label, window, 0, y);
        mnTextDrawStringInWindow((Sint32)MenuColonString, window, 32, y);

        for (i = 0; i < DEBUG_FLAGS_PER_ROW; i++)
        {
            Sint32 set = syGameFlagGet(flag);
            bits[i]    = (set != 0) + '0';
            flag++;
        }
        bits[i] = 0;
        mnTextDrawStringInWindow((Sint32)bits, window, 48, y);
    }
}

// Moves within the flag grid, changes pages, or toggles the selected flag. Returns 1 when
// flags/page need redrawing, -1 to close, and 0 otherwise. A toggles flag page * 256 + row * 16 +
// column; B or Select closes. L and R turn a page, wrapping at either end; with Start held they
// jump DEBUG_LARGE_STEP, to the other end when that passes it (5 - 10 gives 15).
Sint32 mnDebugFlagEditorHandleInput(Sint32 window, Sint32 *page, Sint32 *cursor)
{
    Sint32 *row = &cursor[1];

    if (PerWork.repeat & KEY_A)
    {
        Sint32 flag = ((*page << 4) + *row << 4) + cursor[0];

        if (syGameFlagGet(flag))
            syGameFlagClear(flag);
        else
            syGameFlagSet(flag);
        return 1;
    }
    if ((PerWork.push & KEY_B) || (PerWork.repeat & KEY_SELECT))
        return -1;
    if (PerWork.repeat & KEY_UP)
    {
        if (--*row < 0)
            *row = DEBUG_FLAG_ROWS - 1;
    }
    else if (PerWork.repeat & KEY_DOWN)
    {
        if (++*row > DEBUG_FLAG_ROWS - 1)
            *row = 0;
    }
    else if (PerWork.repeat & KEY_LEFT)
    {
        if (--cursor[0] < 0)
            cursor[0] = DEBUG_FLAGS_PER_ROW - 1;
    }
    else if (PerWork.repeat & KEY_RIGHT)
    {
        if (++cursor[0] > DEBUG_FLAGS_PER_ROW - 1)
            cursor[0] = 0;
    }
    else if ((PerWork.repeat & KEY_L) && (PerWork.repeat & KEY_START))
    {
        *page -= DEBUG_LARGE_STEP;
        if (*page < 0)
            *page = DEBUG_FLAG_PAGES - 1;
        return 1;
    }
    else if ((PerWork.repeat & KEY_R) && (PerWork.repeat & KEY_START))
    {
        *page += DEBUG_LARGE_STEP;
        if (*page > DEBUG_FLAG_PAGES - 1)
            *page = 0;
        return 1;
    }
    else if (PerWork.repeat & KEY_L)
    {
        if (--*page < 0)
            *page = DEBUG_FLAG_PAGES - 1;
        return 1;
    }
    else if (PerWork.repeat & KEY_R)
    {
        if (++*page > DEBUG_FLAG_PAGES - 1)
            *page = 0;
        return 1;
    }
    return 0;
}

// Unused: an empty function, the twin of the window object's mnSelectionMenuReservedNoOperation.
// Nothing calls it.
UNUSED void mnReservedNoOperation(void)
{
}

// Sets the Djinni's collected flag, then adds it to the specified party member. The flag is
// FLAG_DJINN_FIRST + DJINN_PER_ELEMENT * element + index.
void mnDjinnSetFoundFlagAndAdd(Sint32 member, Sint32 element, Sint32 index)
{
    syGameFlagSet((element * DJINN_PER_ELEMENT) + index + FLAG_DJINN_FIRST);
    gmDjinnGiveStub(member, element, index);
}
