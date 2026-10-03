#include "RUNTIME_MEM.H"
#include "TEXT_READER.H"
#include "TYPES.H"
#include "DMA.H"
#include "WINDOW.H"
#include "HEAP_STATE.H"
#include "GAME_STATE.H"
#include "BATTLE_RUNTIME.H"

/*
 * Expands message glyphs, names, numbers and control codes into the render
 * ring. A message of -1 returns the previous message's starting position.
 * The cached decoder is an ARM routine called through a function pointer;
 * the compiler emits the interworking call. The native r9 load is that
 * callable target, not a dead value or a direct call to _call_via_r9.
 *
 * Earlier trial, 2026-09-29: did not compile against then-current headers.
 * Before this ownership repair, EN 2026-10-03 scored 10163 with 296 differing
 * instructions and unresolved Text_FormatNumber (symbol-name comparison).
 * The decoder/field repair scored 6975/254. One ordinary local-declaration
 * reorder scored 6959/253 and is retained. The complete draft is 1608 bytes;
 * the current native owner is 1620, including its literal pools. Stack
 * locals, initial stores, cached-decoder addressing and branch scheduling
 * still differ. No unresolved calls, new matching device or byte credit.
 * The control-code meanings and message-table bases are only partly known;
 * this remains an English draft, with no new linked source or byte credit.
 */

/* The two spacing entries themselves. */
#define ENTRY_NARROW_GAP 5
#define ENTRY_WIDE_GAP   222

/* The decoder is copied into its cached heap block by DMA3. */
#define DMA3_REGS   0x040000D4
#define DMA_ENABLE  0x84000000
#define TEXT_WORK_BLOCK 50

extern const u8 Func_08015430[];
extern u8 Text_DecodeSymbolCodeSize[];

/* The maintained callee exposes a pointer word; this consumer interprets
   that word as a message index. Its wider interface remains unresolved. */
void *BattleFx_FindConditionResourceFar(s32 no, s32 kind);

s32 UiText_BuildRenderEntries(s32 script, s32 clear)
{
    struct UiRenderWork *work;
    u16 *entry;
    void *buf;
    u32 size;
    s32 (*decode)(struct TextReader *);
    u32 start;
    u32 pos;
    u32 next;
    u32 ch;
    u32 prev;
    u32 cnt;
    s32 running;
    s32 overflow;
    s32 head;
    s32 quote;
    s32 plural;
    s32 suffix;
    s32 no;
    s32 off;
    s32 value;
    s32 mag;
    u8 *num;
    u8 *src;
    u16 *dst;
    u16 name[24];
    u8 numbuf[16];
    struct TextReader st;

    work = (struct UiRenderWork *)gWindowWork[0];
    start = work->count;
    entry = work->entries;
    pos = start;
    ch = 0;
    prev = 0;
    cnt = 0;
    running = 1;
    overflow = 0;
    head = 1;
    quote = 0;
    plural = 0;
    suffix = 0;

    if (script == -1) {
        start = work->message_start;
    } else {
        size = (u32)Text_DecodeSymbolCodeSize;
        buf = Runtime_AllocateHeapBlock(TEXT_WORK_BLOCK, size);
        Dma_Set((const void *)Func_08015430, buf,
                DMA_ENABLE | (size >> 2),
                (volatile u32 *)DMA3_REGS);
        decode = (s32 (*)(struct TextReader *))
            ((union HeapState *)&gWorkSlot)->slots[TEXT_WORK_BLOCK];
        UiText_LookupMessage(&st, script);

        do {
            prev = ch;
            ch = (u32)decode(&st);
            if (ch > 255)
                ch = 64;

            if (overflow != 0) {
                /* 行が溢れた後は制御コードだけを読み飛ばす。 */
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
                        /* 収まらない行は三点リーダで打ち切る。 */
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
                        entry[pos] = (u16)(decode(&st) + 0xFFFF);
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
                        off = num - numbuf;
                        while (off != 16 && numbuf[off] != 0) {
                            entry[pos] = numbuf[off];
                            pos = (pos + 1) & RENDER_ENTRY_MASK;
                            off++;
                        }
                        break;
                    case 19:
                        no = decode(&st) - 1;
                        UiText_DecodeMessage(
                            UiRender_LookupNamedValue(3, clear) + 0x741,
                            name, 24);
                        pos = UiText_AppendArticleName(0, name, pos, entry, no, plural,
                                            &suffix);
                        break;
                    case 20:
                        no = decode(&st) - 1;
                        UiText_DecodeMessage(
                            (UiRender_LookupNamedValue(2, clear) &
                             RENDER_ENTRY_MASK) + 0x182,
                            name, 24);
                        pos = UiText_AppendArticleName(0, name, pos, entry, no, plural,
                                            &suffix);
                        break;
                    case 21:
                        UiText_DecodeMessage(
                            UiRender_LookupNamedValue(4, clear) + 0x333,
                            name, 24);
                        dst = name;
                        while (*dst != 0) {
                            entry[pos] = *dst;
                            dst++;
                            pos = (pos + 1) & RENDER_ENTRY_MASK;
                        }
                        break;
                    case 23:
                        UiText_DecodeMessage(
                            (s32)BattleFx_FindConditionResourceFar(
                                UiRender_LookupNamedValue(6, clear), 1) +
                                RENDER_RESOURCE_BASE,
                            name, 24);
                        dst = name;
                        while (*dst != 0) {
                            entry[pos] = *dst;
                            dst++;
                            pos = (pos + 1) & RENDER_ENTRY_MASK;
                        }
                        break;
                    case 16:
                        src = Owner_GetStateFar(gGameState.selected_actor)->name;
                        dst = name;
                        no = 0;
                        do {
                            *dst = *src;
                            no++;
                            src++;
                            dst++;
                        } while (no <= 14);
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
                        no = decode(&st) - 1;
                        src = Owner_GetStateFar(no)->name;
                        dst = name;
                        no = 0;
                        do {
                            *dst = *src;
                            no++;
                            src++;
                            dst++;
                        } while (no <= 14);
                        pos = UiText_AppendArticleName(0, name, pos, entry, 0, 0,
                                            &suffix);
                        break;
                    case 26:
                        no = (decode(&st) - 1) * 2;
                        entry[pos] = (u16)(no + 128);
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        entry[pos] = (u16)(no + 129);
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
                        /* 続けて通常文字と同じ処理へ落ちる。 */
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
