#include "edition.h"
#include "types.h"
#include "window.h"
#include "glyph.h"
#include "tbs_edition.h"
#include "text_render_runtime.h"
#include "menu_list.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "fixed_math.h"
#include "runtime_interfaces.h"
#include "global_cells.h"
#include "resource.h"
#include "btlwork.h"
#include "resource_ids.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"
#include "text_font.h"
#include "affine.h"
#include "btlpty.h"
#include "callback_scheduler.h"

void UiWork_ResetCounters(void);

void UiText_RenderWideStringAtOffset(u16 *text, struct UiWindow *window, s32 x, s32 y)
{
    u8 *base;
    struct UiRenderWork *work;
    u32 c;
    u32 next;
    s16 start;

    base = gWindowWork[0];
    work = (struct UiRenderWork *)base;
    c = 0;
    start = x;
    if (text == NULL) {
        text = work->entries;
        work->entries[work->count] = c;
        work->count = (work->count + 1) & RENDER_ENTRY_MASK;
    }
#if EDITION_INTERNATIONAL
    for (;;)
#else
    while ((c = *text++) != 0)
#endif
    {
#if EDITION_INTERNATIONAL
        c = *text++;
        if (c > 0xff)
            c = 0x40;
        if (c == 0)
            break;
#endif
        if (c <= 30) {
            switch (c) {
            case 8:
                work->colour = *text;
                text++;
                break;
            case 9:
                work->outline = *text;
                text++;
                break;
            case 10:
                work->line_spacing = *text;
                text++;
                break;
            case 7:
                UiWork_ResetCounters();
                break;
            case 3:
                x = start;
                y += 15;
                break;
            case 14:
            case 15:
            case 28:
                text++;
            case 11:
            case 12:
            case 17:
#if EDITION_INTERNATIONAL
            case 29:
#endif
                text++;
                break;
            }
        } else {
#if EDITION_INTERNATIONAL
            if ((((struct UiWindow *)window)->flags & 8) == 0) {
                next = *text;
#if defined(TBS_EDITION_ES)
                if (c > 32 && next > 32) {
                    s16 width = UiText_Glyphs[c - 32].width + UiText_Glyphs[next - 32].width;

                    if (work->outline == 1) {
                        if ((u16)width <= 14) {
                            c |= next << 8;
                            text++;
                        }
                    } else if ((u16)width <= 15) {
                        c |= next << 8;
                        text++;
                    }
                }
#else
                if (c > 32 && next > 32
                    && (u16)(UiText_Glyphs[c - 32].width + UiText_Glyphs[next - 32].width) <= 15) {
                    c |= next << 8;
                    text++;
                }
#endif
            }
#else
            if (*text == 0xde) {
                c |= 0x4000;
                text++;
            } else if (*text == 0xdf) {
                c |= 0x8000;
                text++;
            }
#endif
            x += UiText_DrawGlyph(window, c, x, y, 0);
        }
    }
}

void UiText_RenderGlyphTileAtWorkOffset(
    u16 *buffer,
    const struct RenderInput *work,
    s32 offset_x,
    s32 offset_y)
{
    struct UiRenderWork *canvas = (struct UiRenderWork *)gWindowWork[0];
    u32 cell;

    if (buffer == NULL) {
        u16 *counter = &canvas->count;

        buffer = canvas->entries;
        canvas->entries[*counter] = 0;
        *counter = (*counter + 1) & RENDER_ENTRY_MASK;
    }

    cell = ((work->y + offset_y + 1) << 5)
        + (work->x + offset_x) + 1;
    if (cell < 0x280) {
        s32 src;
        s32 dst;

        cell *= 2;
        dst = 0x06002000 + cell;
        src = (s32)canvas->tilemap + cell;

        UiText_RenderStringTiles(
            buffer,
            (u16 *)src,
            (u16 *)dst,
            0x06002000);
    }
}

void UiText_RenderWideStringInWindow(s16 *text, struct UiWindow *window, s32 x, s32 y)
{
    struct UiRenderWork *work;
    u16 c;
    s16 start;

    work = (struct UiRenderWork *)gWindowWork[0];
    start = x;
    if (text == NULL) {
        text = (s16 *)work->entries;
        work->entries[work->count] = 0;
        work->count = (work->count + 1) & RENDER_ENTRY_MASK;
    }
    c = *text++;
    if (c != 0) do {
        if (c <= 30) {
            switch (c) {
            case 3:
                x = start;
                y++;
                break;
            case 14:
            case 15:
            case 28:
                text++;
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 17:
#if EDITION_INTERNATIONAL
            case 29:
#endif
                text++;
                break;
            }
        } else {
            UiWindow_PutGlyph(window, c, x, y, 0);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            x++;
#else
            if ((u16)(c - 0xde) > 1)
                x++;
#endif
        }
        c = *text++;
    } while (c != 0);
    work->dirty = 1;
}

u8 *UiText_FormatNumber(u8 *buffer, s32 input, s32 width)
{
    s32 offset;
    s32 value;
    s32 negative;
    s32 space;
    s32 minus;
    u8 *leading;
    u8 *trim;

    value = input;
    negative = 0;
    if (value < 0) {
        if (width == 0)
            negative = 1;
        value = 0U - value;
    }

    buffer[0] = ' ';
    for (offset = 12; offset != 0; offset--) {
        buffer[offset] = value % 10 + '0';
        value = value / 10;
    }

    offset = 0;
    buffer[13] = offset;
    space = ' ';
    offset = 1;
    minus = '-';
    for (leading = buffer; offset != 13; leading++, offset++) {
        if (leading[1] == '0') {
            if (offset != 12)
                leading[1] = space;
        } else {
            if (negative != 0)
                leading[0] = minus;
            break;
        }
    }

    if (width == 0) {
        offset = 0;
        if (buffer[0] == ' ') {
            trim = buffer;
            do {
                offset++;
                if (offset == 12)
                    break;
                trim++;
            } while (*trim == ' ');
        }
        return buffer + offset;
    }

    if (width < 0 || width > 12)
        width = 12;
    return buffer + 13 - width;
}

#if EDITION_INTERNATIONAL
#if defined(TBS_EDITION_IT)
#define ARTICLE_KINDS     11
#define ARTICLE_KIND_MASK 15
#elif defined(TBS_EDITION_ES)
#define ARTICLE_KINDS     5
#define ARTICLE_KIND_MASK 15
#else
#define ARTICLE_KINDS     8
#define ARTICLE_KIND_MASK 7
#endif

struct ArticleTable {
    s8 *text[ARTICLE_KINDS];
};

#if defined(TBS_EDITION_ES)
#endif
#endif

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
u32 UiText_AppendArticleName(s32 mode, u16 *name, u32 pos, u16 *entry,
                             s32 no, s32 plural, s32 *suffix)
{
    struct ArticleTable tbl;
#if defined(TBS_EDITION_ES)
    struct ArticleTable indefinite_tbl;
    u16 count;
    s16 indefinite;
#endif
    s32 kind;
    u16 head;
    u16 c;
    s8 *p;
    s8 c8;
    s32 cnt;

#if defined(TBS_EDITION_ES)
    count = no & 7;
    indefinite = no & 8;
#endif
    if (mode != 0) {
        entry[pos] = ' ';
        pos = (pos + 1) & 0x1ff;
        entry[pos] = 0x0a;
        pos = (pos + 1) & 0x1ff;
        entry[pos] = 0x0a;
        pos = (pos + 1) & 0x1ff;
    }
#if defined(TBS_EDITION_ES)
    if (count == 0 || count == 4) {
#else
    if (no == 0 || no == 4) {
#endif
        head = name[0];
        if (head == 29)
            name += 2;
    } else {
        kind = 0;
        tbl = UiText_ArticleTable;
#if defined(TBS_EDITION_ES)
        indefinite_tbl = UiText_IndefiniteArticles;
#endif
        if (name[0] == 29) {
            kind = name[1];
            name += 2;
        }
#if defined(TBS_EDITION_ES)
        if (indefinite == 0)
            p = tbl.text[kind & ARTICLE_KIND_MASK];
        else
            p = indefinite_tbl.text[kind & ARTICLE_KIND_MASK];
#else
        p = tbl.text[kind & ARTICLE_KIND_MASK];
#endif
        for (cnt = 0; cnt < 8; cnt++) {
            if ((c8 = *p++) == 0)
                break;
            if (cnt == 0 && *suffix == 2)
                c8 -= 0x20;
            entry[pos] = c8;
            pos = (pos + 1) & 0x1ff;
        }
    }
    while (*name != 0) {
        c = *name++;
        entry[pos] = c;
        pos = (pos + 1) & 0x1ff;
#if defined(TBS_EDITION_DE)
        if (c == 's' || c == 'S' || c == 'z' || c == 'Z')
            *suffix = 1;
        else
            *suffix = 0;
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
        if (c == 's' || c == 'S')
            *suffix = 1;
        else
            *suffix = 0;
#endif
    }
    if (mode != 0) {
        entry[pos] = 0x0a;
        pos = (pos + 1) & 0x1ff;
        entry[pos] = 0x08;
        pos = (pos + 1) & 0x1ff;
        entry[pos] = ' ';
        pos = (pos + 1) & 0x1ff;
    }
    return pos;
}
#endif

#if defined(TBS_EDITION_EN)
u32 UiText_AppendArticleName(s32 mode, u16 *name, u32 pos, u16 *entry,
                             s32 no, s32 plural, s32 *suffix)
{
    struct ArticleTable tbl;
    s32 kind;
    u16 head;
    u16 c;
    u8 *p;
    s8 c8;
    s32 cnt;

    if (mode != 0) {
        entry[pos] = ' ';
        pos = (pos + 1) & 0x1ff;
        entry[pos] = 0x0a;
        pos = (pos + 1) & 0x1ff;
        entry[pos] = 0x0a;
        pos = (pos + 1) & 0x1ff;
    }
    if (no == 1 || (no == 3 && plural == 0)) {
        kind = 0;
        tbl = UiText_ArticleTable;
        head = name[0];
        if (head == 29) {
            kind = name[1] - 1;
            name += 2;
        }
        if (kind == 0) {
            if (head == 'A' || head == 'I' || head == 'U' || head == 'E' || head == 'O')
                kind = 2;
            else
                kind = 1;
        }
        p = tbl.text[kind & 7];
        for (cnt = 0; cnt < 8; cnt++) {
            if ((c8 = *p++) == 0)
                break;
            entry[pos] = c8;
            pos = (pos + 1) & 0x1ff;
        }
    } else {
        head = name[0];
        if (head == 29)
            name += 2;
    }
    while (*name != 0) {
        c = *name++;
        entry[pos] = c;
        pos = (pos + 1) & 0x1ff;
        if (c == 'S' || c == 's')
            *suffix = 1;
        else
            *suffix = 0;
    }
    if (no == 2 || (no == 3 && plural != 0)) {
        if (*suffix != 0) {
            entry[pos] = 'e';
            pos = (pos + 1) & 0x1ff;
        }
        entry[pos] = 's';
        pos = (pos + 1) & 0x1ff;
    }
    if (mode != 0) {
        entry[pos] = 0x0a;
        pos = (pos + 1) & 0x1ff;
        entry[pos] = 0x08;
        pos = (pos + 1) & 0x1ff;
        entry[pos] = ' ';
        pos = (pos + 1) & 0x1ff;
    }
    return pos;
}
#endif

#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || \
    defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#include "mem.h"
#include "text_reader.h"
#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "window.h"
#include "heap_state.h"
#include "game_state.h"
#include "btlsys.h"
#include "scene.h"

#define ENTRY_NARROW_GAP 5
#define ENTRY_WIDE_GAP   222

#define TEXT_WORK_BLOCK 50

extern const u8 Text_DecodeSymbol[];
extern u8 Text_DecodeSymbolCodeSize[];

extern char MsgItemName;
extern char MsgAbilityName;
extern char MsgClassName;
extern char MsgDebugEntryName;
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || \
    defined(TBS_EDITION_IT)
extern char MsgItemPluralName;
#if defined(TBS_EDITION_FR)
extern s8 UiText_ItemPronoun[];
#else
struct ItemPossessiveTable {
#if defined(TBS_EDITION_ES)
    s8 *text[2];
#else
    s8 *text[4];
#endif
};
extern const struct ItemPossessiveTable UiText_ItemPossessives;
extern const s8 UiText_ItemGenders[];
extern u8 *UiText_NumberWords[];
#endif
#endif

s32 UiText_BuildRenderEntries(s32 script, s32 clear)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    u32 ch = 0;
#if defined(TBS_EDITION_EN)
    s32 head = 1;
#endif
    s32 quote = 0;
    u32 start = work->count;
    u32 cnt = 0;
    s32 plural = 0;
#if !defined(TBS_EDITION_DE) && !defined(TBS_EDITION_IT)
    s32 running = 1;
#endif
#if defined(TBS_EDITION_EN)
    u32 pos = start;
    u32 prev;
    s32 suffix = 0;
#else
#if defined(TBS_EDITION_ES)
    struct ItemPossessiveTable possessives;
    s32 form;
#endif
    s32 suffix = 2;
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_IT)
    s32 running = 1;
#endif
    s8 *word;
    s8 c8;
    u32 pos = start;
#endif
    u16 *entry = work->entries;
    s32 overflow = 0;
    s32 (*decode)(struct TextReader *);
    u32 next;
    s32 no;
    u32 j;
    u32 i;
    s32 value;
    s32 tmp;
#if defined(TBS_EDITION_IT)
    s32 ending;
#endif
    s32 mag;
    u8 *num;
    u8 *src;
    u16 *dst;
    u16 *p;
    u16 name[24];
    u8 numbuf[16];
    struct TextReader st;
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_IT)
    struct ItemPossessiveTable possessives;
    s32 kind;
#endif

    if (script == -1) {
        start = work->message_start;
    } else {
        {
            u32 size;
            void *code;

            /* FAKEMATCH: needed to match: the one-pass block loads the size before the slot number, as in symbol.c. */
            do {
                size = (u32)Text_DecodeSymbolCodeSize;
            } while (0);
            code = Runtime_AllocateHeapBlock(50, size);
            Dma_Set((const void *)Text_DecodeSymbol, code,
                0x84000000 | (size >> 2), REG_DMA3);
        }
        decode = (s32 (*)(struct TextReader *))
            gWindowWork[TEXT_WORK_BLOCK - HEAP_SLOT_WINDOW];
        UiText_LookupMessage(&st, script);

        do {
#if defined(TBS_EDITION_EN)
            prev = ch;
#endif
            ch = (u32)decode(&st);
            if (ch > 255)
                ch = 64;

            if (overflow != 0) {
                if (ch < 32) {
                    switch (ch) {
                    case 19:
                        decode(&st);
                        UiRender_LookupNamedValue(3, clear);
                        break;
                    case 0:
                    case 2:
                    case 30:
                        running = 0;
                        break;
                    case 22:
                        UiRender_LookupNamedValue(5, clear);
                        break;
                    case 20:
                        decode(&st);
                        UiRender_LookupNamedValue(2, clear);
                        break;
                    case 21:
                        UiRender_LookupNamedValue(4, clear);
                        break;
                    case 23:
                        UiRender_LookupNamedValue(6, clear);
                        break;
                    case 8:
                    case 9:
                        decode(&st);
                        break;
                    case 17:
                        decode(&st);
                        break;
                    case 18:
                        decode(&st);
                        break;
                    case 29:
                        decode(&st);
                        break;
                    case 1:
                        running = 0;
                        ch = 2;
                        break;
                    case 16:
                        break;
                    case -1:
                        break;
                    default:
                        break;
                    }
                }
            } else {
                if (work->spacing_before != 0
#if defined(TBS_EDITION_EN)
                    && head == 0 && ch != 222 && ch != 223
#endif
                ) {
                    entry[pos] = ENTRY_NARROW_GAP;
                    pos = (pos + 1) & RENDER_ENTRY_MASK;
                }

#if defined(TBS_EDITION_EN)
                if (work->spacing_after != 0 && head == 0 &&
                    ch != 222 && ch != 223 &&
                    prev <= 256 && prev > 127 &&
                    prev != 222 && prev != 223 && prev != 32 &&
                    prev != 165 && prev != 161 && prev != 164) {
                    entry[pos] = ENTRY_WIDE_GAP;
                    pos = (pos + 1) & RENDER_ENTRY_MASK;
                }

#endif
                if (ch > 31) {
                    if (work->spacing_before != 0 &&
                        (ch == 32 || cnt > 10)) {
                        entry[pos] = '.';
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        entry[pos] = '.';
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        entry[pos] = '.';
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        overflow = 1;
                        if (cnt > 10)
                            ch = 32;
                    }
                    if (ch == 34) {
                        quote ^= 1;
                        if (quote != 0)
                            ch = 142;
                    }
                    entry[pos] = (u16)ch;
                    pos = (pos + 1) & RENDER_ENTRY_MASK;
#if defined(TBS_EDITION_EN)
                    head = 0;
#else
                    if (ch != ' '
#if defined(TBS_EDITION_ES)
                        && ch != 161 && ch != 191
#endif
                    ) {
                        suffix = 0;
                        if (ch == '.' || ch == '!' || ch == '?')
                            suffix = 2;
                        if (ch == 's' || ch == 'S'
#if defined(TBS_EDITION_DE)
                            || ch == 'z' || ch == 'Z'
#endif
                        )
                            suffix = 1;
                    }
#endif
                } else {
                    switch (ch) {
                    case 0:
                    case 2:
                    case 30:
                        running = 0;
                        break;
                    case 8:
                    case 9:
                    case 29:
                        entry[pos] = (u16)ch;
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        entry[pos] = decode(&st) + 0xFFFFU;
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        break;
                    case 22:
                        value = (s32)UiRender_LookupNamedValue(5, clear);
                        mag = value;
                        if (value < 0)
                            mag = -value;
                        plural = 1;
                        if (mag <= 1)
                            plural = 0;
                        num = UiText_FormatNumber(numbuf, value, 0);
                        i = num - numbuf;
                        while (i != 16 && numbuf[i] != 0) {
                            entry[pos] = numbuf[i];
                            pos = (pos + 1) & RENDER_ENTRY_MASK;
                            i++;
                        }
                        break;
                    case 19:
                        no = decode(&st) - 1;
                        tmp = UiRender_LookupNamedValue(3, clear);
                        UiText_DecodeMessage(tmp + (s32)&MsgClassName, name, 24);
                        pos = UiText_AppendArticleName(0, name, pos, entry, no, plural,
                                            &suffix);
                        break;
                    case 20:
                        no = decode(&st) - 1;
#if defined(TBS_EDITION_ES)
                        form = no & 7;
#endif
                        tmp = UiRender_LookupNamedValue(2, clear);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || \
    defined(TBS_EDITION_IT)
                        tmp &= RENDER_ENTRY_MASK;
#if defined(TBS_EDITION_ES)
                        if (form == 2 || (form == 3 && plural != 0) || form == 4)
#else
                        if (no == 2 || (no == 3 && plural != 0) || no == 4)
#endif
                            UiText_DecodeMessage(tmp + (s32)&MsgItemPluralName, name, 24);
                        else
                            UiText_DecodeMessage(tmp + (s32)&MsgItemName, name, 24);
#else
                        UiText_DecodeMessage((tmp & RENDER_ENTRY_MASK) + (s32)&MsgItemName,
                                             name, 24);
#endif
                        pos = UiText_AppendArticleName(0, name, pos, entry, no, plural,
                                            &suffix);
                        break;
                    case 21:
                        tmp = UiRender_LookupNamedValue(4, clear);
                        UiText_DecodeMessage(tmp + (s32)&MsgAbilityName, name, 24);
                        p = name;
                        next = pos;
                        while (*p != 0) {
                            entry[next] = *p;
                            p++;
                            next = (next + 1) & RENDER_ENTRY_MASK;
                        }
                        pos = next;
                        break;
                    case 23:
                        UiText_DecodeMessage(
                            Scene_GetAreaNameIndexFar(
                                UiRender_LookupNamedValue(6, clear), 1)
                            + (s32)&MsgDebugEntryName, name, 24);
                        p = name;
                        next = pos;
                        while (*p != 0) {
                            entry[next] = *p;
                            p++;
                            next = (next + 1) & RENDER_ENTRY_MASK;
                        }
                        pos = next;
                        break;
                    case 16:
                        src = UnitGetFar(gGameState.selected_actor)->name;
                        dst = name;
                        i = 0;
                        do {
                            *dst = *src;
                            i++;
                            src++;
                            dst++;
                        } while (i <= 14);
                        pos = UiText_AppendArticleName(0, name, pos, entry, 0, 0,
                                            &suffix);
                        break;
                    case 18:
                        no = decode(&st) - 1;
                        src = UnitGetFar(
                            UiRender_LookupNamedValue(1, clear))->name;
                        dst = name;
                        j = 0;
                        do {
                            *dst = *src;
                            j++;
                            src++;
                            dst++;
                        } while (j <= 14);
                        pos = UiText_AppendArticleName(0, name, pos, entry, no, plural,
                                            &suffix);
                        break;
                    case 17:
                        tmp = decode(&st) - 1;
                        src = UnitGetFar(tmp)->name;
                        dst = name;
                        j = 0;
                        do {
                            *dst = *src;
                            j++;
                            src++;
                            dst++;
                        } while (j <= 14);
                        pos = UiText_AppendArticleName(0, name, pos, entry, 0, 0,
                                            &suffix);
                        break;
                    case 26:
                        tmp = ((decode(&st) - 1) << 1) + 128;
                        entry[pos] = tmp;
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        entry[pos] = tmp + 1;
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        break;
                    case 24:
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || \
    defined(TBS_EDITION_IT)
                        entry[pos] = ' ';
#else
                        entry[pos] = 143;
#endif
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        entry[pos] = '-';
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || \
    defined(TBS_EDITION_IT)
                        entry[pos] = ' ';
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
#endif
                        break;
#if defined(TBS_EDITION_DE)
                    case 13:
                        no = decode(&st) - 1;
                        src = UiText_NumberWords[(no & 7) * 2 + plural];
                        while (*src != 0) {
                            entry[pos] = *src++;
                            pos = (pos + 1) & RENDER_ENTRY_MASK;
                        }
                        break;
#endif
#if defined(TBS_EDITION_IT)
                    case 13:
                        ending = decode(&st) - 1;
                        if (ending == 0) {
                            if (plural == 0)
                                entry[pos] = 'o';
                            else
                                entry[pos] = 'i';
                        } else if (plural == 0)
                            entry[pos] = 'a';
                        else
                            entry[pos] = 'e';
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        break;
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
                    case 13:
#elif defined(TBS_EDITION_EN)
                    case 25:
#endif
#if !defined(TBS_EDITION_DE) && !defined(TBS_EDITION_IT)
                        if (plural != 0) {
                            if (suffix != 0) {
                                entry[pos] = 'e';
                                pos = (pos + 1) & RENDER_ENTRY_MASK;
                            }
                            entry[pos] = 's';
                            pos = (pos + 1) & RENDER_ENTRY_MASK;
                        }
                        break;
#endif
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                    case 25:
#if defined(TBS_EDITION_ES)
                        UiRender_LookupNamedValue(2, 0);
                        possessives = UiText_ItemPossessives;
                        word = possessives.text[plural];
#else
                        tmp = UiRender_LookupNamedValue(2, 0);
                        tmp &= RENDER_ENTRY_MASK;
                        possessives = UiText_ItemPossessives;
                        kind = UiText_ItemGenders[tmp];
                        word = possessives.text[kind & 3];
#endif
                        /* FAKEMATCH: needed to match: value counts the letters, compared unsigned; a counter of its own loses r1 in Spanish and Italian. */
                        for (value = 0; (u32)value < 16; value++) {
                            if ((c8 = *word++) == 0)
                                break;
                            if (value == 0 && suffix == 2)
                                c8 -= 0x20;
                            entry[pos] = c8;
                            pos = (pos + 1) & RENDER_ENTRY_MASK;
                        }
                        break;
#endif
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
                    case 25:
#if defined(TBS_EDITION_DE)
                        tmp = UiRender_LookupNamedValue(2, 0);
                        tmp &= RENDER_ENTRY_MASK;
                        possessives = UiText_ItemPossessives;
                        kind = UiText_ItemGenders[tmp];
                        word = possessives.text[kind & 3];
#else
                        UiRender_LookupNamedValue(2, 0);
                        word = UiText_ItemPronoun;
#endif
                        for (i = 0; i < 16; i++) {
                            if ((c8 = *word++) == 0)
                                break;
                            if (i == 0 && suffix == 2)
                                c8 -= 0x20;
                            entry[pos] = c8;
                            pos = (pos + 1) & RENDER_ENTRY_MASK;
                        }
                        suffix = 0;
                        break;
#endif
                    case 27:
#if defined(TBS_EDITION_DE)
                        if (suffix == 1)
                            entry[pos] = '\'';
                        else
                            entry[pos] = 's';
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
#elif defined(TBS_EDITION_EN)
                        entry[pos] = '\'';
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        if (suffix == 0) {
                            entry[pos] = 's';
                            pos = (pos + 1) & RENDER_ENTRY_MASK;
                        }
#endif
                        break;
                    case -1:
                        break;
                    case 1:
                    case 3:
#if defined(TBS_EDITION_EN)
                        head = 1;
#endif
                    default:
                        entry[pos] = (u16)ch;
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
#if defined(TBS_EDITION_EN)
                        if (ch == 's' || ch == 'S')
                            suffix = 1;
                        else
                            suffix = 0;
#endif
                        break;
                    }
                }
            }
            cnt++;
        } while (running != 0 && cnt <= RENDER_ENTRY_MASK);

        entry[pos] = (u16)ch;
        pos = (pos + 1) & RENDER_ENTRY_MASK;
        entry[pos] = 0;
        next = (pos + 1) & RENDER_ENTRY_MASK;
        work->count = (u16)next;
        Runtime_ReleaseHeapBlock(TEXT_WORK_BLOCK);
        work->message_start = (u16)start;
    }

    if (clear != 0)
        UiWork_ClearValueNameTables();

    return start;
}

#endif

#if defined(TBS_EDITION_JA)
#include "mem.h"
#include "text_reader.h"
#include "dma.h"
#include "io_reg.h"
#include "window.h"
#include "heap_state.h"
#include "game_state.h"
#include "btlsys.h"
#include "scene.h"

extern const u8 Text_DecodeSymbol[];
extern u8 Text_DecodeSymbolCodeSize[];
extern char MsgItemName;
extern char MsgAbilityName;
extern char MsgClassName;
extern char MsgDebugEntryName;
extern char MsgMysteryPersonName;

extern __inline__ u32 UiText_AppendArticleName(s32 mode, u16 *name,
    u32 pos, u16 *entry, s32 no, s32 plural, s32 *suffix)
{
    u16 c;

    if (mode != 0) {
        entry[pos] = ' ';
        pos = (pos + 1) & RENDER_ENTRY_MASK;
        entry[pos] = 10;
        pos = (pos + 1) & RENDER_ENTRY_MASK;
        entry[pos] = 10;
        pos = (pos + 1) & RENDER_ENTRY_MASK;
    }
    while (*name != 0) {
        c = *name++;
        entry[pos] = c;
        pos = (pos + 1) & RENDER_ENTRY_MASK;
    }
    if (mode != 0) {
        entry[pos] = 10;
        pos = (pos + 1) & RENDER_ENTRY_MASK;
        entry[pos] = 8;
        pos = (pos + 1) & RENDER_ENTRY_MASK;
        entry[pos] = ' ';
        pos = (pos + 1) & RENDER_ENTRY_MASK;
    }
    return pos;
}

s32 UiText_BuildRenderEntries(s32 script, s32 clear)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    u32 ch = 0;
    s32 head = 1;
    u32 start = work->count;
    u32 cnt = 0;
    s32 running = 1;
    u32 pos = start;
    u16 *entry = work->entries;
    u8 mode = work->menu_state & 2;
    s32 (*decode)(struct TextReader *);
    s32 overflow = 0;
    u32 prev;
    u32 next;
    u32 i;
    u32 j;
    s32 tmp;
    s32 message;
    s32 value;
    u8 *num;
    u8 *src;
    u16 *dst;
    struct BtlUnit *actor;
    u16 name[24];
    u8 numbuf[16];
    struct TextReader st;

    if (script == -1) {
        start = work->message_start;
    } else {
        {
            u32 size;
            void *code;

            /* FAKEMATCH: needed to match: plain assignment and const initialization reverse the two slot/size argument moves; the one-pass block preserves their native order. */
            do {
                size = (u32)Text_DecodeSymbolCodeSize;
            } while (0);
            code = Runtime_AllocateHeapBlock(50, size);
            Dma_Set((const void *)Text_DecodeSymbol, code,
                0x84000000 | (size >> 2), REG_DMA3);
        }
        decode = (s32 (*)(struct TextReader *))gWindowWork[50 - HEAP_SLOT_WINDOW];
        UiText_LookupMessage(&st, script);
        do {
            prev = ch;
            ch = (u32)decode(&st);
            if (overflow != 0) {
                if (ch < 32) {
                    switch (ch) {
                    case 19:
                        UiRender_LookupNamedValue(3, clear);
                        break;
                    case 0:
                    case 2:
                    case 30:
                        running = 0;
                        break;
                    case 8:
                    case 9:
                        decode(&st);
                        break;
                    case 22:
                        UiRender_LookupNamedValue(5, clear);
                        break;
                    case 20:
                        UiRender_LookupNamedValue(2, clear);
                        break;
                    case 21:
                        UiRender_LookupNamedValue(4, clear);
                        break;
                    case 23:
                        UiRender_LookupNamedValue(6, clear);
                        break;
                    case 17:
                        decode(&st);
                        break;
                    case 18:
                        break;
                    case 1:
                        running = 0;
                        ch = 2;
                        break;
                    case 16:
                        break;
                    case -1:
                        break;
                    default:
                        break;
                    }
                }
            } else {
                if (work->spacing_before != 0 && head == 0 &&
                    ch != 222 && ch != 223) {
                    entry[pos] = 5;
                    pos = (pos + 1) & RENDER_ENTRY_MASK;
                }
                if (work->spacing_after != 0 && head == 0 &&
                    ch != 222 && ch != 223 &&
                    prev <= 256 && prev > 127 &&
                    prev != 222 && prev != 223 && prev != 32 &&
                    prev != 165 && prev != 161 && prev != 164) {
                    entry[pos] = 222;
                    pos = (pos + 1) & RENDER_ENTRY_MASK;
                }
                if (ch > 31) {
                    if (mode != 0 && ch == ' ') {
                        entry[pos] = ch;
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                    }
                    if (work->spacing_before != 0 &&
                        (ch == 32 || cnt > 10)) {
                        entry[pos] = 165;
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        entry[pos] = 165;
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        entry[pos] = 165;
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        entry[pos] = 161;
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        overflow = 1;
                        if (cnt > 10)
                            ch = 32;
                    }
                    entry[pos] = (u16)ch;
                    pos = (pos + 1) & RENDER_ENTRY_MASK;
                    head = 0;
                } else {
                    switch (ch) {
                    case 0:
                    case 2:
                    case 30:
                        running = 0;
                        break;
                    case 8:
                    case 9:
                        entry[pos] = (u16)ch;
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        entry[pos] = decode(&st) + 0xffffU;
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        break;
                    case 22:
                        value = UiRender_LookupNamedValue(5, clear);
                        num = UiText_FormatNumber(numbuf, value, 0);
                        i = num - numbuf;
                        while (i != 16 && numbuf[i] != 0) {
                            entry[pos] = numbuf[i];
                            pos = (pos + 1) & RENDER_ENTRY_MASK;
                            i++;
                        }
                        break;
                    case 19:
                        tmp = UiRender_LookupNamedValue(3, clear);
                        UiText_DecodeMessage(tmp + (s32)&MsgClassName, name, 24);
                        pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
                        break;
                    case 20:
                        tmp = UiRender_LookupNamedValue(2, clear);
                        UiText_DecodeMessage((tmp & 511) + (s32)&MsgItemName, name, 24);
                        pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
                        break;
                    case 21:
                        tmp = UiRender_LookupNamedValue(4, clear);
                        UiText_DecodeMessage(tmp + (s32)&MsgAbilityName, name, 24);
                        pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
                        break;
                    case 23:
                        UiText_DecodeMessage(
                            Scene_GetAreaNameIndexFar(UiRender_LookupNamedValue(6, clear), 1)
                                + (s32)&MsgDebugEntryName, name, 24);
                        pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
                        break;
                    case 16:
                        src = UnitGetFar(gGameState.selected_actor)->name;
                        dst = name;
                        i = 0;
                        do {
                            *dst = *src;
                            i++;
                            src++;
                            dst++;
                        } while (i <= 14);
                        pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
                        break;
                    case 18:
                        actor = UnitGetFar(UiRender_LookupNamedValue(1, clear));
                        if (work->menu_state != 0 &&
                            (actor->class_id == 122 || actor->class_id == 125)) {
                            message = (s32)&MsgMysteryPersonName;
                            if (actor->class_id == 125)
                                message++;
                            UiText_DecodeMessage(message, name, 10);
                            pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
                        } else {
                            src = actor->name;
                            dst = name;
                            j = 0;
                            do {
                                *dst = *src;
                                j++;
                                src++;
                                dst++;
                            } while (j <= 14);
                            pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
                        }
                        break;
                    case 17:
                        tmp = decode(&st) - 1;
                        src = UnitGetFar(tmp)->name;
                        dst = name;
                        j = 0;
                        do {
                            *dst = *src;
                            j++;
                            src++;
                            dst++;
                        } while (j <= 14);
                        pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
                        break;
                    case -1:
                        break;
                    case 1:
                    case 3:
                        head = 1;
                    default:
                        entry[pos] = (u16)ch;
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        break;
                    }
                }
            }
            cnt++;
        } while (running != 0 && cnt <= RENDER_ENTRY_MASK);
        entry[pos] = (u16)ch;
        pos = (pos + 1) & RENDER_ENTRY_MASK;
        entry[pos] = 0;
        next = (pos + 1) & RENDER_ENTRY_MASK;
        work->count = (u16)next;
        Runtime_ReleaseHeapBlock(50);
        work->message_start = (u16)start;
    }
    if (clear != 0)
        UiWork_ClearValueNameTables();
    return start;
}

#endif

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)

#define FIT_FIXED_LIMIT 1
#endif

void UiWindow_FitOnScreen(s32 no, s32 *px, s32 *py, u32 *pw, u32 *ph, u16 *spacing, u32 flags)
{
#if !defined(FIT_FIXED_LIMIT)
    u8 *base;
#endif
    s32 x;
    s32 y;
    s32 limit;
    s32 right;
    s32 bottom;
    s32 over;
    s32 pos;

#if !defined(FIT_FIXED_LIMIT)
    base = gWindowWork[0];
#endif
    x = *px;
    y = *py;
    limit = 30;

    if (!(flags & 2)) {
        if (flags & 1)
            UiText_MeasureStringVariant(no, pw, ph, spacing);
        else
            UiText_MeasureEntryDimensions(no, pw, ph, spacing);
    }

    if (*pw == 0 && *ph == 0)
        return;

    if (!(flags & 2)) {
        *pw = (*pw + 19) >> 3;
        *ph = (*ph + 15) >> 3;
#if !defined(FIT_FIXED_LIMIT)
        if (((struct UiRenderWork *)base)->mode != 0) {
            *pw += 2;
            limit = 29;
        }
#endif
    }

    right = x + *pw;
    if (right > limit) {
        over = right - limit;
        pos = x - over;
        if (pos >= 0)
            x = pos;
        else
            x = 0;
    }
    bottom = y + *ph;
    if (bottom > 20) {
        over = bottom - 20;
        pos = y - over;
        if (pos >= 0)
            y = pos;
        else
            y = 0;
    }
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (x > limit - *pw)
        x = limit - *pw;
    if (y > 20 - *ph)
        y = 20 - *ph;
    *px = x;
    *py = y;
}

void UiText_MeasureResourceEntries(s32 no, s32 *x, s32 *y)
{
    UiText_MeasureEntryDimensions(UiText_BuildRenderEntries(no, 0), (u32 *)x, (u32 *)y, 0);
}

s32 UiText_GetResourceDimensions(s32 no, s32 *x, s32 *y, u32 *width, u32 *height)
{
    struct UiRenderWork *work;
    s32 temp;

    work = (struct UiRenderWork *)gWindowWork[0];
    temp = UiText_BuildRenderEntries(no, 0);
    if (work->entries[temp] == 0)
    {
        return 0;
    }
    UiWindow_FitOnScreen(temp, x, y, width, height, 0, 0);
    return 1;
}

s32 UiText_GetResourceDimensionsAlt(s32 no, s32 *x, s32 *y, u32 *width, u32 *height)
{
    struct UiRenderWork *work;
    s32 idx;

    work = (struct UiRenderWork *)gWindowWork[0];
    idx = UiText_BuildRenderEntries(no, 0);
    if (work->entries[idx] == 0)
    {
        return 0;
    }
    UiWindow_FitOnScreen(idx, x, y, width, height, 0, 1);
    return 1;
}

s32 __divsi3(s32 numerator, s32 denominator);

void UiText_MeasureEntryDimensions(s32 pos, u32 *out_width, u32 *out_height, u16 *spacing)
{
    struct UiRenderWork *work;
    u32 lines;
    u32 width;
    u32 height;
    u32 line_width;
    u32 count;
    u32 c;
    u32 glyph;
    u32 i;
    u16 counts[4];
    u16 widths[4];
    s32 gap;
    u32 style;
#if !EDITION_INTERNATIONAL
    s32 trailing;
#endif

    work = (struct UiRenderWork *)gWindowWork[0];
#if !EDITION_INTERNATIONAL
    trailing = 0;
#endif
    width = 0;
    height = 15;
    lines = 0;
    line_width = 0;
    count = 0;
    for (;;) {
        c = work->entries[pos];
        pos = (pos + 1) & RENDER_ENTRY_MASK;
        if (c > 31) {
#if EDITION_INTERNATIONAL
            if (c == 32) {
                line_width += 5;
                count++;
            } else {
                glyph = UiText_Glyphs[c - 32].width;
                style = work->outline;
                if (style == 1 || style == 5)
                    glyph++;
                line_width += glyph;
            }
#else
            if (c != 0xde && c != 0xdf) {
                if (c == 32) {
                    line_width += 7;
                } else {
                    if (c == 0xa5)
                        trailing = -1;
                    else if (c == 0x21)
                        trailing = -3;
                    else if (c == 0xa1 || c == 0xa4)
                        trailing = -6;
                    else
                        trailing = 0;
                    glyph = 10;
                    if (c <= 0xff)
                        glyph = UiText_Glyphs[c - 32].width;
                    style = work->outline;
                    if (style == 1 || style == 5)
                        glyph++;
                    line_width += glyph;
                }
            }
            count++;
#endif
            continue;
        }
        switch (c) {
        case 0:
#if !defined(TBS_EDITION_ES) && !defined(TBS_EDITION_FR) && !defined(TBS_EDITION_IT)
        case 1:
#endif
            goto done;
        case 3:
#if EDITION_INTERNATIONAL
            count++;
#else
            line_width += trailing;
#endif
            counts[lines] = count;
            widths[lines] = line_width;
            if (width < line_width)
                width = line_width;
            if (lines < 3)
                lines++;
            count = 0;
            line_width = 0;
            height += 15;
            break;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        case 1:
            work->outline = 0;
            goto done;
#endif
        case 14:
        case 28:
            pos = (pos + 1) & RENDER_ENTRY_MASK;
        case 8:
        case 10:
        case 15:
        case 17:
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            break;
        case 9:
            work->outline = work->entries[pos];
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            break;
        }
    }
done:
#if EDITION_INTERNATIONAL
    count++;
#else
    line_width += trailing;
#endif
    counts[lines] = count;
    widths[lines] = line_width;
    if (width < line_width)
        width = line_width;
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
    if (work->mode)
        width = width + 2;
#endif
    *out_width = width;
    *out_height = height;
    width = (width + 19) >> 3;
    width = (width << 3) - 16;
    if (spacing != NULL) {
        i = 0;
        while (i <= lines) {
            if (counts[i] <= 1) {
                *spacing++ = 0;
            } else {
                gap = width - widths[i];
                gap -= 4;
                if (gap < 0)
                    gap = 0;
                gap = __divsi3(gap << 8, counts[i] - 1);
#if EDITION_INTERNATIONAL
                if (gap < 0 || gap > 0xc00)
                    gap = 0x200;
#else
                if (gap < 0 || gap > 0x800)
                    gap = 0;
                if (gap < 0 || gap > 0x100)
                    gap = 0x100;
#endif
                *spacing++ = gap;
            }
            i++;
        }
    }
}

void UiText_MeasureStringVariant(s32 start, u32 *out_width, u32 *out_height,
                                 u16 *spacing)
{
    u32 heights[16];
    u16 counts[4];
    u16 widths[4];
    struct UiRenderWork *work;
    u32 max_width;
    u32 line;
    u32 width;
    u32 count;
    u32 variant;
    u32 code;
    u32 glyph;
    u32 outline;
    u32 i;
    u32 j;
    s32 gap;
#if !EDITION_INTERNATIONAL
    s32 trailing;
#endif

#if !EDITION_INTERNATIONAL
    trailing = 0;
#endif
    work = (struct UiRenderWork *)gWindowWork[0];
    heights[0] = 15;
    heights[1] = 15;
    heights[2] = 15;
    heights[3] = 15;
    heights[4] = 15;
    heights[5] = 15;
    heights[6] = 15;
    heights[7] = 15;
    heights[8] = 15;
    heights[9] = 15;
    heights[10] = 15;
    heights[11] = 15;
    heights[12] = 15;
    max_width = 0;
    heights[13] = 15;
    heights[14] = 15;
    heights[15] = 15;
    line = 0;
    width = 0;
    count = 0;
    variant = 0;
    for (;;) {
        code = work->entries[start];
        start = (start + 1) & RENDER_ENTRY_MASK;
        if (code >= 32) {
#if EDITION_INTERNATIONAL
            if (code == 32) {
                width += 5;
                count++;
            } else {
                glyph = UiText_Glyphs[code - 32].width;
                outline = work->outline;
                if (outline == 1 || outline == 5)
                    glyph++;
                width += glyph;
            }
#else
            if (code != 0xde && code != 0xdf) {
                if (code == 32) {
                    width += 7;
                } else {
                    if (code == 0xa5)
                        trailing = -1;
                    else if (code == 0x21)
                        trailing = -3;
                    else if (code == 0xa1 || code == 0xa4)
                        trailing = -6;
                    else
                        trailing = 0;
                    glyph = 10;
                    if (code <= 0xff)
                        glyph = UiText_Glyphs[code - 32].width;
                    outline = work->outline;
                    if (outline == 1 || outline == 5)
                        glyph++;
                    width += glyph;
                }
            }
            count++;
#endif
            continue;
        }
        switch (code) {
        case 3:
#if EDITION_INTERNATIONAL
            counts[line] = ++count;
#else
            width += trailing;
            counts[line] = count;
#endif
            widths[line] = width;
            if (variant == 0 && max_width < width)
                max_width = width;
            if (line < 3)
                line++;
            count = 0;
            width = 0;
            heights[variant] += 15;
            break;
        case 0:
#if EDITION_INTERNATIONAL
            counts[line] = ++count;
#else
            width += trailing;
            counts[line] = count;
#endif
            widths[line] = width;
            if (variant == 0 && max_width < width)
                max_width = width;
            variant++;
            goto done;
        case 1:
#if EDITION_INTERNATIONAL
            counts[line] = ++count;
#else
            width += trailing;
            counts[line] = count;
#endif
            widths[line] = width;
            if (variant == 0 && max_width < width)
                max_width = width;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            work->outline = 0;
#endif
            variant++;
            break;
        case 14:
        case 28:
            start = (start + 1) & RENDER_ENTRY_MASK;
        case 8:
        case 10:
        case 15:
        case 17:
            start = (start + 1) & RENDER_ENTRY_MASK;
            break;
        case 9:
            work->outline = work->entries[start];
            start = (start + 1) & RENDER_ENTRY_MASK;
            break;
        }
    }
done:
#if !defined(TBS_EDITION_DE) && !defined(TBS_EDITION_FR)
    if (work->mode != 0)
        max_width += 2;
#endif
    for (i = 0; i < variant; i++) {
        if (i == 0)
            *out_height = heights[0];
        else if (*out_height < heights[i])
            *out_height = heights[i];
    }
    *out_width = max_width;
    max_width = (((max_width + 19) >> 3) << 3) - 16;
    if (spacing != NULL) {
        for (j = 0; j <= line; j++) {
            if (counts[j] <= 1) {
                *spacing++ = 0;
            } else {
                gap = max_width - widths[j];
                gap -= 4;
                if (gap < 0)
                    gap = 0;
                gap = (gap << 8) / (counts[j] - 1);
#if !EDITION_INTERNATIONAL
                if (gap < 0 || gap > 0x800)
                    gap = 0;
                if (gap < 0 || gap > 0x100)
                    gap = 0x100;
#endif
                *spacing++ = gap;
            }
        }
    }
}

typedef s32 (*GlyphRoutine)(struct UiWindow *, s32, s32, s32, u8 *);

struct GlyphSprite {
    u32 unknown_00;
    struct {
        u16 y : 8;
        u16 flags : 8;
        u16 x : 9;
        u16 unknown_attr1 : 7;
    } pos;
    u16 tile;
    u16 unknown_0a;
};

#undef OAM
#define OAM(output) ((struct GlyphSprite *)(output)->unknown_10)

s32 UiText_DrawGlyph(struct UiWindow *window, s32 character,
    s32 x, s32 y, s32 mode)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    u32 spacing_x = work->unknown_before_count;
    s32 width;
    u32 spacing_y = work->line_spacing;

    if (mode != 1 && (window->flags & 8) != 0) {
        u8 *font;
        GlyphRoutine draw;
        s32 result;
        u32 size;
#if !EDITION_INTERNATIONAL
        s32 mark;

        font = NULL;
        if (((struct BattleDisplayWork *)gWindowWork[22])->window == window) {
            font = Resource_GetTableEntry((s32)&ResourceId_SmallFont);
            width = 3;
            if (character == ' ')
                goto done;
        }
        mark = (s16)UiText_MarkGlyphCodes[(u32)character >> 14];
        character &= 0x3fff;
        width = 4;
        if (character == ' ')
            goto done;
#else
        if (((struct BattleDisplayWork *)gWindowWork[22])->window == window) {
            Resource_GetTableEntry((s32)&ResourceId_SmallFont);
            Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
            width = 3;
            if (character == ' ')
                goto done;
        }
        font = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
        width = 4;
        if (character == ' ')
            goto done;
#endif
        size = (u32)UiText_DrawGlyphCodeSize;
        draw = (GlyphRoutine)MemAlloc(size);
        Dma_Set(UiText_DrawGlyphArm, draw, 0x84000000 | (size >> 2), REG_DMA3);
        result = draw(window, character, x, y, font);
#if !EDITION_INTERNATIONAL
        if ((u16)mark != 0)
            result += draw(window, (u16)mark, x + result, y, font);
#endif
        MemFree(draw);
        return result;
    }

    if (character == ' ') {
#if !EDITION_INTERNATIONAL
        width = 7;
#else
        width = 5;
#endif
        goto done;
    }
    {
        u32 tiles[32];
        struct RenderOutput *output;
        struct GlyphSprite *sprite;
        s32 index;

        output = RenderOutput_AcquireFree();
        if (output == NULL)
            return 0;
        index = (output - work->outputs) * 4;
        output->active = 1;
        output->kind = 0;
        if (mode == 1) {
            width = 1;
            output->active = 2;
        } else {
            switch (work->outline) {
            case 3:
                output->active = 5;
                break;
            case 4:
                output->active = 6;
                output->anim_step = 8;
                break;
            case 5:
                output->active = 7;
                output->anim_step = 0;
                break;
            case 2:
                output->active = 4;
                output->anim_step = 0;
                break;
            }
#if !EDITION_INTERNATIONAL
            width = UiText_RenderGlyphPair(character, tiles);
#else
            {
                s32 drawn = UiText_RenderGlyphPair(character, tiles);

                if (drawn == 0)
                    drawn = 1;
                width = drawn;
            }
#endif
        }

        if ((u8)output->active == 2) {
            sprite = OAM(output);
            if (work->glyph_resource == 99)
                work->glyph_resource = Resource_FindFreeEntry();
            sprite->pos.x = (window->x << 3) + ((window->width - 2) << 3) + 4;
#if !EDITION_INTERNATIONAL
            sprite->pos.y = (window->y << 3) + ((window->height - 2) << 3) + 1;
#else
            sprite->pos.y = (window->y << 3) + ((window->height - 2) << 3) - 1;
#endif
        } else {
            Dma_Set(tiles, (void *)(0x06010000 + (work->glyph_tiles + index) * 32),
                0x84000020, REG_DMA3);
#if !EDITION_INTERNATIONAL
            ((u16 *)&output->packed)[0] = y + (spacing_y >> 1) + window->y * 8;
#else
            ((u16 *)&output->packed)[0] = y + (spacing_y >> 1) + window->y * 8 - 2;
#endif
            ((u16 *)&output->packed)[1] = (x + (spacing_x >> 1) + window->x * 8 + 2) | 0x4000;
            ((u16 *)&output->packed)[2] = work->glyph_tiles + index;
            sprite = OAM(output);
        }
        output->sentinel = 254;
        output->x = sprite->pos.x;
        output->y = sprite->pos.y;
        output->index = index;
        output->next = NULL;
        RenderOutput_AppendToList(&window->output, output);
    }
done:
    return width;
}

struct SpriteAttr {
    u8 unk0[4];
    u8 y;
    u8 unk5;
    u16 x : 9;
    u16 unk6 : 7;
    u32 unk8;
};

struct GlyphSpriteOutput {
    s32 link_word;
    u8 kind;
    u8 active;
    s16 x;
    s16 y;
    u8 unknown_0a[4];
    s8 index;
    u8 sentinel;
    struct SpriteAttr attr;
};

LAYOUT_SIZE_GUARD(GlyphSpriteOutput_Size, struct GlyphSpriteOutput,
    sizeof(struct RenderOutput));
LAYOUT_OFFSET_GUARD(GlyphSpriteOutput_X, struct GlyphSpriteOutput, x,
    (u32)&((struct RenderOutput *)0)->x);
LAYOUT_OFFSET_GUARD(GlyphSpriteOutput_Y, struct GlyphSpriteOutput, y,
    (u32)&((struct RenderOutput *)0)->y);
LAYOUT_OFFSET_GUARD(GlyphSpriteOutput_Active, struct GlyphSpriteOutput, active,
    (u32)&((struct RenderOutput *)0)->active);
LAYOUT_OFFSET_GUARD(GlyphSpriteOutput_Index, struct GlyphSpriteOutput, index,
    (u32)&((struct RenderOutput *)0)->index);
LAYOUT_OFFSET_GUARD(GlyphSpriteOutput_Attr, struct GlyphSpriteOutput, attr,
    (u32)&((struct RenderOutput *)0)->unknown_10);
LAYOUT_OFFSET_GUARD(GlyphSpriteOutput_Oam, struct GlyphSpriteOutput, attr.y,
    (u32)&((struct RenderOutput *)0)->packed);
LAYOUT_OFFSET_GUARD(GlyphSpriteOutput_Table, struct GlyphSpriteOutput, attr.unk8,
    (u32)&((struct RenderOutput *)0)->table);

#if EDITION_INTERNATIONAL

void UiWindow_PutGlyph(struct UiWindow *win, u32 tile, u32 x, u32 y, s32 mode)
{
    void *work = gWindowWork[0];
    u8 *base = work;
    struct GlyphSpriteOutput *out;
    s32 idx;
    u16 *slot;
    struct SpriteAttr *attr;
    u32 pos;
    u16 row;

    if (y > (win->height - 2))
        return;
    if (x > (win->width - 2))
        return;
    if (mode == 1) {
        s32 column;
        work = RenderOutput_AcquireFree();
        if (work == NULL)
            return;
        out = work;
        idx = ((struct RenderOutput *)out -
            ((struct UiRenderWork *)base)->outputs) * 4;
        out->active = 2;
        attr = &out->attr;
        slot = &((struct UiRenderWork *)base)->glyph_resource;
        if (*slot == 99)
            *slot = Resource_FindFreeEntry();
        column = 0xfffe;
        attr->x = (win->x + (column + *(u16 *)&win->width)) * 8 + 4;
        row = (u8)win->y + (row = (u8)win->height + 254);
        attr->y = row * 8 - 1;
        /* FAKEMATCH: needed to match: the embedded OAM view preserves packed/logical position store order changed by byte-span and scalar views. */
        out->x = attr->x;
        out->y = attr->y;
        out->link_word = 0;
        out->index = idx;
        if (out->active == 0)
            out->active = mode;
        RenderOutput_AppendToList(&win->output, (struct RenderOutput *)out);
    } else if (tile <= 0xff) {
        x++;
        y++;
        pos = (win->y + y) * 32 + (win->x + x);
        if (pos < 640)
            ((struct UiRenderWork *)work)->tilemap[pos] = tile | 0xf000;
    }
}

#else

void UiWindow_PutGlyph(struct UiWindow *win, u32 tile, u32 x, u32 y, s32 mode)
{
    u8 *work = gWindowWork[0];
    /* FAKEMATCH: needed to match: this XOR pair changes no pointer bits and folds without instructions, preserving work and base as independent lifetimes. A direct copy merges both into r8, omits the native r8/r12 copy, and differs 17 aligned instructions (0x144 versus 0x148 bytes). */
    u8 *base = (u8 *)((unsigned long)work ^ tile);
    struct GlyphSpriteOutput *out;
    s32 idx;
    u16 *slot;
    struct SpriteAttr *attr;
    u32 pos;
    u16 row;

    base = (u8 *)((unsigned long)base ^ tile);

    if (y > (win->height - 2))
        return;
    if (x > (win->width - 2))
        return;
    if (mode == 1) {
        s32 column;
        u16 left;
        out = (struct GlyphSpriteOutput *)RenderOutput_AcquireFree();
        if (out == NULL)
            return;
        idx = ((struct RenderOutput *)out -
            ((struct UiRenderWork *)base)->outputs) * 4;
        out->active = 2;
        attr = &out->attr;
        slot = &((struct UiRenderWork *)base)->glyph_resource;
        if (*slot == 99)
            *slot = Resource_FindFreeEntry();
        column = 0xfffe;
        left = win->x;
        attr->x = (left + (column + *(u16 *)&win->width)) * 8 + 4;
        row = (u8)win->y + (row = (u8)win->height + 254);
        attr->y = row * 8 + 1;
        /* FAKEMATCH: needed to match: the embedded OAM view preserves packed/logical position store order changed by byte-span and scalar views. */
        out->x = attr->x;
        out->y = attr->y;
        out->link_word = 0;
        out->index = idx;
        if (out->active == 0)
            out->active = mode;
        RenderOutput_AppendToList(&win->output, (struct RenderOutput *)out);
    } else if (tile <= 0xff) {
        if (tile - 0xde <= 1) {
            u32 line;

            line = (win->y + y) * 32;
            switch (*(((struct UiRenderWork *)base)->tilemap + (line + (win->x + x)))) {
            case 0xf011:
                tile -= 0xc0;
                break;
            case 0xf00e:
                tile -= 0xd0;
                break;
            }
        } else {
            x++;
            y++;
        }
        pos = (win->y + y) * 32 + (win->x + x);
        if (pos < 640)
            ((struct UiRenderWork *)work)->tilemap[pos] = tile | 0xf000;
    }
}

#endif

enum {
    TILEMAP_ENTRY_PLAIN,
    TILEMAP_ENTRY_NONE,
    TILEMAP_ENTRY_PALETTE_14,
    TILEMAP_ENTRY_PALETTE_15,
    TILEMAP_ENTRY_PALETTE_1
};

#define TILEMAP_WIDTH 32
#define TILEMAP_ENTRIES (TILEMAP_WIDTH * 20)

void UiWindow_SetTilemapEntry(
    struct UiWindow *window, s32 value, s32 x, s32 y, u32 mode)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    s32 palette;
    s32 index;

    y += 1;
    x += 1;
    if (y > (u32)(window->height - 1))
        return;
    if (x > (u32)(window->width - 1))
        return;

    switch (mode) {
    case TILEMAP_ENTRY_PALETTE_15:
        palette = 0xf000;
        break;
    case TILEMAP_ENTRY_PALETTE_14:
        palette = 0xe000;
        break;
    case TILEMAP_ENTRY_PALETTE_1:
        palette = 0x1000;
        break;
    default:
        palette = 0;
        break;
    }

    switch (mode) {
    case TILEMAP_ENTRY_NONE:
        return;
    case TILEMAP_ENTRY_PALETTE_14:
    case TILEMAP_ENTRY_PALETTE_15:
    case TILEMAP_ENTRY_PALETTE_1:
        index = (window->y + y) * TILEMAP_WIDTH + (window->x + x);
        if ((u32)index >= TILEMAP_ENTRIES)
            return;
        work->tilemap[index] = palette | value;
        return;
    case TILEMAP_ENTRY_PLAIN:
    default:
        index = (window->y + y) * TILEMAP_WIDTH + (window->x + x);
        if ((u32)index >= TILEMAP_ENTRIES)
            return;
        work->tilemap[index] = value;
        return;
    }
}

struct OutputSprite {
    u32 link;
    u8 y;
    u8 affine : 2;
    u8 mode : 2;
    u8 mosaic : 1;
    u8 colour : 1;
    u8 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
    union {
        u16 value;
        struct {
            u16 tile : 10;
            u16 priority : 2;
            u16 palette : 4;
        } f;
    } attr2;
};

void RenderOutput_UpdateScaleAnimation(struct RenderOutput *output)
{
    s32 scale = 256;
    struct OutputSprite *sprite = (struct OutputSprite *)output->unknown_10;
    struct AffineTransform effect;

    switch ((u8)output->active) {
    case 9:
        scale = Ui_BounceScales[output->anim_step++ & 31];
        break;
    case 10:
        scale = Ui_BounceScales[output->anim_step++ & 31] >> 1;
        break;
    case 11:
        if (output->anim_step <= 7)
            scale = Ui_BounceScales[output->anim_step++ * 2 + 16];
        break;
    case 12:
        if (output->anim_step <= 7)
            scale = Ui_BounceScales[output->anim_step++ * 2 + 16] >> 1;
        break;
    }
    if (scale == 256) {
        sprite->affine_index = 0;
        sprite->affine = 0;
        sprite->x = output->x;
        sprite->y = output->y;
    } else {
        effect.scale_x = scale;
        effect.scale_y = scale;
        effect.angle = 0;
        sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
        if (scale > 256) {
            sprite->affine = 3;
            /* FAKEMATCH: needed to match: the unsigned halfword read of the x cell; a cast of the signed member swaps the x load and the -8 pool load. */
            sprite->x = *(u16 *)&output->x - 8;
            sprite->y = (u16)output->y - 8;
        } else {
            sprite->affine = 1;
            sprite->x = output->x;
            sprite->y = output->y;
        }
    }
}

struct SpriteOutput {
    struct SpriteOutput *next;
    u8 unknown_04;
    u8 mode;
    u16 x;
    u16 y;
    u16 unknown_0a;
    u16 frame;
    u8 tile;
    u8 priority;
    struct OutputSprite sprite;
};

extern u32 gFrameTick;

u32 Random16(void);

void UiWork_AnimateSpriteSlots(void)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    struct UiWindow *window = work->windows;
    s32 no;
    struct AffineTransform effect;

    for (no = 0; no != WINDOW_COUNT; window++, no++) {
        struct SpriteOutput *output;
        u32 phase;

        if (!(window->flags & 1))
            continue;
        output = (struct SpriteOutput *)window->output.head;
        phase = (gFrameTick >> 2) & 7;
        while (output != NULL) {
            struct OutputSprite *sprite = &output->sprite;

            if (window->effect == 4) {
                output->frame = 2;
                output->mode = 8;
            }
            switch (output->mode) {
            case 2:
                if (work->glyph_resource != 0x60) {
                    const u8 *tiles = &UiSprite_CursorFrameTiles[phase * 128];

                    sprite->attr2.f.tile = VramBlock_LoadCached(
                        work->glyph_resource, 128, tiles);
                    output->tile = sprite->attr2.f.tile;
                    sprite->mode = 0;
                    sprite->mosaic = 0;
                    sprite->colour = 1;
                    sprite->size = 0;
                    sprite->shape = 2;
                    sprite->y = output->y + UiSprite_CursorBobOffsets[gFrameTick % 80] + 2;
                    sprite->affine = 0;
                    sprite->affine_index = 0;
                }
                break;
            case 5:
                if (gFrameTick & 1) {
                    sprite->x = output->x + (s32)(((Random16() * 3 >> 16) +
                        (Random16() * 3 >> 16)) >> 1) - 1;
                    sprite->y = output->y + (s32)(((Random16() * 3 >> 16) +
                        (Random16() * 3 >> 16)) >> 1) - 1;
                }
                break;
            case 6:
                if (output->frame == 0)
                    goto reset;
                effect.scale_x = 512;
                effect.scale_y = 512;
                effect.angle = 0;
                sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
                sprite->affine = 3;
                sprite->x = output->x - 5;
                sprite->y = output->y - 5;
                output->frame--;
                break;
            case 7:
                effect.scale_x = 256;
                effect.scale_y = 256;
                output->frame += 768;
                effect.angle = output->frame;
                sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
                sprite->affine = 1;
                sprite->x = output->x - (Trig_Sin(effect.angle + 0xe800) >> 14) - 2;
                sprite->y = output->y - (Trig_Cos(effect.angle + 0x6800) >> 14) - 2;
                break;
            case 4:
                if (gFrameTick & 1)
                    output->frame++;
                sprite->x = output->x + UiSprite_WobbleOffsets[output->frame % 20][0];
                sprite->y = output->y + UiSprite_WobbleOffsets[output->frame % 20][1] - 2;
                break;
            case 17:
                output->frame++;
                sprite->y = output->y - UiSprite_BounceOffsets[output->frame & 15];
                break;
            case 14:
            case 15:
            case 16:
                output->frame++;
                sprite->y = output->y + UiSprite_BounceOffsets[output->frame & 15];
                break;
            case 18:
                output->frame++;
                sprite->x = output->x - UiSprite_BounceOffsets[output->frame & 15];
                sprite->y = output->y + UiSprite_BounceOffsets[output->frame & 15];
                break;
            case 8:
                if (output->frame == 0)
                    goto reset;
                effect.scale_x = 320;
                effect.scale_y = 320;
                effect.angle = 0;
                sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
                sprite->affine = 3;
                sprite->x = output->x - 8;
                sprite->y = output->y - 8;
                output->frame--;
                break;
            reset:
                sprite->affine_index = 0;
                sprite->affine = 0;
                sprite->x = output->x;
                sprite->y = output->y;
                break;
            case 9:
            case 10:
            case 11:
            case 12:
                RenderOutput_UpdateScaleAnimation((struct RenderOutput *)output);
                break;
            }
            if (output->mode == 2) {
                if (work->glyph_resource != 0x60)
                    Runtime_PushSlotEntry(sprite, output->priority);
            } else if (output->mode != 13) {
                Runtime_PushSlotEntry(sprite, output->priority);
            }
            output = output->next;
            phase = (gFrameTick >> 2) & 7;
        }
    }
}
