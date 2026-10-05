#include "RUNTIME_MEM.H"
#include "TEXT_READER.H"
#include "TYPES.H"
#include "DMA.H"
#include "WINDOW.H"
#include "HEAP_STATE.H"
#include "GAME_STATE.H"
#include "BATTLE_RUNTIME.H"
#include "SCENE.H"

#define ENTRY_NARROW_GAP 5
#define ENTRY_WIDE_GAP   222

#define DMA3_REGS   0x040000D4
#define DMA_ENABLE  0x84000000
#define TEXT_WORK_BLOCK 50

extern const u8 Text_DecodeSymbol[];
extern u8 Text_DecodeSymbolCodeSize[];

extern char MsgItemName;
extern char MsgAbilityName;
extern char MsgClassName;
extern char MsgDebugEntryName;

s32 UiText_BuildRenderEntries(s32 script, s32 clear)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    s32 head = 1;
    s32 quote = 0;
    u32 start = work->count;
    u32 cnt = 0;
    s32 plural = 0;
    s32 running = 1;
    s32 overflow = 0;
    u32 pos = start;
    u16 *entry = work->entries;
    u32 ch = 0;
    u32 prev;
    s32 suffix = 0;
    void *buf;
    u32 size;
    s32 (*decode)(struct TextReader *);
    u32 next;
    s32 no;
    u32 off;
    u32 i;
    s32 value;
    s32 icon;
    u32 msg;
    s32 mag;
    u8 *num;
    u8 *src;
    u16 *dst;
    u16 *p;
    u16 name[24];
    u8 numbuf[16];
    struct TextReader st;

    if (script == -1) {
        start = work->message_start;
    } else {
        size = (u32)Text_DecodeSymbolCodeSize;
        buf = Runtime_AllocateHeapBlock(TEXT_WORK_BLOCK, size);
        Dma_Set((const void *)Text_DecodeSymbol, buf,
                DMA_ENABLE | (size >> 2),
                (volatile u32 *)DMA3_REGS);
        decode = (s32 (*)(struct TextReader *))
            gWindowWork[TEXT_WORK_BLOCK - HEAP_SLOT_WINDOW];
        UiText_LookupMessage(&st, script);

        do {
            prev = ch;
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
                if (work->spacing_before != 0 && head == 0 &&
                    ch != 222 && ch != 223) {
                    entry[pos] = ENTRY_NARROW_GAP;
                    pos = (pos + 1) & RENDER_ENTRY_MASK;
                }

                if (work->spacing_after != 0 && head == 0 &&
                    ch != 222 && ch != 223 &&
                    prev <= 256 && prev > 127 &&
                    prev != 222 && prev != 223 && prev != 32 &&
                    prev != 165 && prev != 161 && prev != 164) {
                    entry[pos] = ENTRY_WIDE_GAP;
                    pos = (pos + 1) & RENDER_ENTRY_MASK;
                }

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
                        msg = UiRender_LookupNamedValue(3, clear);
                        UiText_DecodeMessage(msg + (s32)&MsgClassName, name, 24);
                        pos = UiText_AppendArticleName(0, name, pos, entry, no, plural,
                                            &suffix);
                        break;
                    case 20:
                        no = decode(&st) - 1;
                        msg = UiRender_LookupNamedValue(2, clear);
                        UiText_DecodeMessage((msg & RENDER_ENTRY_MASK) + (s32)&MsgItemName,
                                             name, 24);
                        pos = UiText_AppendArticleName(0, name, pos, entry, no, plural,
                                            &suffix);
                        break;
                    case 21:
                        msg = UiRender_LookupNamedValue(4, clear);
                        UiText_DecodeMessage(msg + (s32)&MsgAbilityName, name, 24);
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
                        off = 0;
                        do {
                            *dst = *src;
                            off++;
                            src++;
                            dst++;
                        } while (off <= 14);
                        pos = UiText_AppendArticleName(0, name, pos, entry, 0, 0,
                                            &suffix);
                        break;
                    case 18:
                        no = decode(&st) - 1;
                        src = Owner_GetStateFar(
                            UiRender_LookupNamedValue(1, clear))->name;
                        dst = name;
                        off = 0;
                        do {
                            *dst = *src;
                            off++;
                            src++;
                            dst++;
                        } while (off <= 14);
                        pos = UiText_AppendArticleName(0, name, pos, entry, no, plural,
                                            &suffix);
                        break;
                    case 17:
                        msg = decode(&st) - 1;
                        src = Owner_GetStateFar(msg)->name;
                        dst = name;
                        off = 0;
                        do {
                            *dst = *src;
                            off++;
                            src++;
                            dst++;
                        } while (off <= 14);
                        pos = UiText_AppendArticleName(0, name, pos, entry, 0, 0,
                                            &suffix);
                        break;
                    case 26:
                        icon = ((decode(&st) - 1) << 1) + 128;
                        entry[pos] = icon;
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        entry[pos] = icon + 1;
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        break;
                    case 24:
                        entry[pos] = 143;
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        entry[pos] = '-';
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        break;
                    case 25:
                        if (plural != 0) {
                            if (suffix != 0) {
                                entry[pos] = 'e';
                                pos = (pos + 1) & RENDER_ENTRY_MASK;
                            }
                            entry[pos] = 's';
                            pos = (pos + 1) & RENDER_ENTRY_MASK;
                        }
                        break;
                    case 27:
                        entry[pos] = '\'';
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        if (suffix == 0) {
                            entry[pos] = 's';
                            pos = (pos + 1) & RENDER_ENTRY_MASK;
                        }
                        break;
                    case -1:
                        break;
                    case 1:
                    case 3:
                        head = 1;
                    default:
                        entry[pos] = (u16)ch;
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        if (ch == 's' || ch == 'S')
                            suffix = 1;
                        else
                            suffix = 0;
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
