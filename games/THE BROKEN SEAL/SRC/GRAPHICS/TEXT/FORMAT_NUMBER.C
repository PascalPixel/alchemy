#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "EDITION.H"
#include "TYPES.H"
#include "FIXED_MATH.H"

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
#include "MEM.H"
#include "TEXT_READER.H"
#include "TYPES.H"
#include "DMA.H"
#include "IO_REG.H"
#include "WINDOW.H"
#include "HEAP_STATE.H"
#include "GAME_STATE.H"
#include "BTLSYS.H"
#include "SCENE.H"

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

            /* FAKEMATCH: needed to match: the one-pass block loads the size before the slot number, as in SYMBOL.C. */
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
                        /* FAKEMATCH: needed to match: value counts the letters, compared unsigned; a counter of its own loses r1 (Italian differs throughout). */
                        value = 0;
                        for (;;) {
                            if ((c8 = *word++) == 0)
                                break;
                            if (value == 0 && suffix == 2)
                                c8 -= 0x20;
                            entry[pos] = c8;
                            pos = (pos + 1) & RENDER_ENTRY_MASK;
                            if ((u32)++value >= 16)
                                break;
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
#include "MEM.H"
#include "TEXT_READER.H"
#include "DMA.H"
#include "IO_REG.H"
#include "WINDOW.H"
#include "HEAP_STATE.H"
#include "GAME_STATE.H"
#include "BTLSYS.H"
#include "SCENE.H"

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
