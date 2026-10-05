/* UiText_BuildRenderEntries, Spanish 0x08016fe0.
 * Native extent 0x660, frame 136, matched by this draft.
 * Remaining 16 instruction lines rotate the final possessive-loop registers:
 * draft text r0 / char r1 / counter r4; native text r4 / char r0 / counter r1.
 * Shared counter: 30 lines differ; separate counter: 16. Signed counter: 17;
 * while loop: 150 and 12 bytes short; const pointer: 30. Fixed counter: 160
 * and 4 bytes long; fixed pointer: 159 and 8 bytes long; pointer barrier: 30.
 * Normalized scaffold comparison: 720 native / 720 draft, differ 32.
 * Reordered declarations, early counter init and combined loop tests: unchanged.
 * Pointer r0 clobber: differ 22; loop counter barrier: differ 199.
 * Those failed steering trials are not retained.
 */
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

#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
#include "RUNTIME_MEM.H"
#include "TEXT_READER.H"
#include "TYPES.H"
#include "DMA.H"
#include "IO_REG.H"
#include "WINDOW.H"
#include "HEAP_STATE.H"
#include "GAME_STATE.H"
#include "BATTLE_RUNTIME.H"
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
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
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
#if !defined(TBS_EDITION_DE)
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
#if defined(TBS_EDITION_DE)
    s32 running = 1;
#endif
    s8 *word;
    s8 c8;
    u32 n;
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
    s32 mag;
    u8 *num;
    u8 *src;
    u16 *dst;
    u16 *p;
    u16 name[24];
    u8 numbuf[16];
    struct TextReader st;
#if defined(TBS_EDITION_DE)
    struct ItemPossessiveTable possessives;
    s32 kind;
#endif

    if (script == -1) {
        start = work->message_start;
    } else {
        {
            u32 size;
            void *code;

            /* FAKEMATCH: the one-pass block loads the size before the slot number, as in SYMBOL.C. */
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
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
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
                        src = Owner_GetStateFar(gGameState.selected_actor)->name;
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
                        src = Owner_GetStateFar(
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
                        src = Owner_GetStateFar(tmp)->name;
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
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
                        entry[pos] = ' ';
#else
                        entry[pos] = 143;
#endif
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        entry[pos] = '-';
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
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
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
                    case 13:
#elif defined(TBS_EDITION_EN)
                    case 25:
#endif
#if !defined(TBS_EDITION_DE)
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
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
                    case 25:
#if defined(TBS_EDITION_DE)
                        tmp = UiRender_LookupNamedValue(2, 0);
                        tmp &= RENDER_ENTRY_MASK;
                        possessives = UiText_ItemPossessives;
                        kind = UiText_ItemGenders[tmp];
                        word = possessives.text[kind & 3];
#elif defined(TBS_EDITION_ES)
                        UiRender_LookupNamedValue(2, 0);
                        possessives = UiText_ItemPossessives;
                        word = possessives.text[plural];
#else
                        UiRender_LookupNamedValue(2, 0);
                        word = UiText_ItemPronoun;
#endif
                        for (n = 0; n < 16; n++) {
                            if ((c8 = *word++) == 0)
                                break;
                            if (n == 0 && suffix == 2)
                                c8 -= 0x20;
                            entry[pos] = c8;
                            pos = (pos + 1) & RENDER_ENTRY_MASK;
                        }
#if !defined(TBS_EDITION_ES)
                        suffix = 0;
#endif
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
