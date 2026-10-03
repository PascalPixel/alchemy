#include "TEXT_READER.H"
#include "TYPES.H"
#include "DMA.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
extern u8 gWindowWork[];

/*
 * UiText_BuildRenderEntries (UiText_BuildRenderEntries)
 *
 * 会話用スクリプトを描画待ち行列へ展開する。
 *
 * Reads a message script one byte at a time through the stream reader opened
 * by UiText_LookupMessage and appends render entries to the 512-halfword ring at
 * work + RENDER_ENTRY_TBL_OFS.  Codes 0x20 and above are glyphs and are stored
 * directly; codes below 0x20 are control codes that either stop the scan,
 * splice in a looked-up name or number, or emit a fixed pair of entries.
 *
 * The write cursor lives at RENDER_ENTRY_COUNT_OFS and the position where the
 * current message started at the halfword after it, so a call with
 * script == -1 reports the previous start without expanding anything.
 *
 * Returns the ring index at which this message begins.
 *
 * Uncertain: the roles of the two flag bytes after RENDER_BUSY_OFS (here
 * spelled "spaced" and "wide gap") and of the fixed table bases 0x741, 0x182
 * and 0x333 are inferred from their use only.  Codes 222 and 223 are excluded
 * by every spacing guard below; 222 is the wide spacing entry this routine
 * emits itself, while what 223 stands for is not established here.
 *
 * Both dispatches carry a `case -1:` with an empty body.  That is not a
 * spelling device: the reference tests the value explicitly (movs r2, #1 /
 * negs r2, r2 / cmp r7, r2 at 0x0801830e) before falling through to the
 * default, so the source it was compiled from named -1 as a case of its own.
 * The likeliest reading is an end-of-stream sentinel from _call_via_r9 that
 * the code below deliberately ignores, but that is inference.
 *
 * Known divergence from the reference: at the stream-open call the reference
 * loads *(u32 *)(ADDR_03001E8C + 140) and keeps it in a callee-saved register
 * for the rest of the body without ever reading it back.  No ordinary C
 * spelling reproduces a load that is dead yet retained, so it is not written
 * here; because of it the reference has one more live value than this draft,
 * spills the script argument, and reloads the ring mask from the literal pool
 * at every use instead of holding it in a register.
 * 2026-09-29 slice 4: the draft does not compile against the current
 * headers (undeclared identifiers in UiText_BuildRenderEntries), so alchemy
 * permute could not score it.
 */

/* Companion of the ring write cursor: where the current message started. */
#define RENDER_ENTRY_START_OFS (RENDER_ENTRY_COUNT_OFS + 2)
/* Two flag bytes follow the busy byte.  The first asks for a narrow spacing
   entry before every glyph, the second for a wide one after a wide glyph. */
#define RENDER_SPACED_OFS   (RENDER_BUSY_OFS + 1)
#define RENDER_WIDE_GAP_OFS (RENDER_BUSY_OFS + 2)

/* The two spacing entries themselves. */
#define ENTRY_NARROW_GAP 5
#define ENTRY_WIDE_GAP   222

/* DMA3 source/destination/control triple, and the 0x140-byte table the
   expander needs resident while it runs. */
#define DMA3_REGS   0x040000D4
#define DMA_ENABLE  0x84000000
#define TEXT_WORK_BLOCK 50
#define TEXT_WORK_SIZE  0x140
#define TEXT_TABLE_SRC  0x08015430

/* alchemy inspect names this callee Runtime_ReleaseHeapBlock; the project has
   no header alias for it yet, so declare the alias beside the prototype. */

s32 Runtime_AllocateHeapBlock(s32 kind, s32 size);
void Runtime_ReleaseHeapBlock(s32 kind);
s32 _call_via_r9(struct TextReader *st);
u8 *Text_FormatNumber(u8 *buf, s32 input, s32 width);
u32 UiText_AppendArticleName(s32 mode, u16 *name, u32 pos, u16 *entry, s32 no, s32 plural,
                  s32 *suffix);
void UiText_DecodeMessage(s32 res, u16 *dst, s32 cnt);
s32 UiRender_LookupNamedValue(s32 kind, s32 clear);
void UiWork_ClearValueNameTables(void);
u8 *Runtime_GetObject(s32 no);
s32 BattleFx_FindConditionResourceFar(s32 no, s32 kind);

extern u8 gGameState[];

u32 UiText_BuildRenderEntries(s32 script, s32 clear)
{
    u8 *work;
    u16 *entry;
    s32 buf;
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
    struct TextReader st;
    u8 numbuf[16];
    u16 name[24];

    work = *(u8 **)gWindowWork;
    start = *(u16 *)(work + RENDER_ENTRY_COUNT_OFS);
    entry = (u16 *)(work + RENDER_ENTRY_TBL_OFS);
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
        start = *(u16 *)(work + RENDER_ENTRY_START_OFS);
    } else {
        buf = Runtime_AllocateHeapBlock(TEXT_WORK_BLOCK, TEXT_WORK_SIZE);
        Dma_Set((void *)TEXT_TABLE_SRC, buf,
                DMA_ENABLE | (TEXT_WORK_SIZE >> 2),
                (volatile u32 *)DMA3_REGS);

        UiText_LookupMessage(&st, script);

        do {
            prev = ch;
            ch = (u32)_call_via_r9(&st);
            if (ch > 255)
                ch = 64;

            if (overflow != 0) {
                /* 行が溢れた後は制御コードだけを読み飛ばす。 */
                if (ch < 32) {
                    switch (ch) {
                    case 19:
                        _call_via_r9(&st);
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
                        _call_via_r9(&st);
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
                        _call_via_r9(&st);
                        break;
                    case 17:
                        _call_via_r9(&st);
                        break;
                    case 18:
                        _call_via_r9(&st);
                        break;
                    case 29:
                        _call_via_r9(&st);
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
                if (work[RENDER_SPACED_OFS] != 0 && head == 0 &&
                    ch != 222 && ch != 223) {
                    entry[pos] = ENTRY_NARROW_GAP;
                    pos = (pos + 1) & RENDER_ENTRY_MASK;
                }

                if (work[RENDER_WIDE_GAP_OFS] != 0 && head == 0 &&
                    ch != 222 && ch != 223 &&
                    prev <= 256 && prev > 127 &&
                    prev != 222 && prev != 223 && prev != 32 &&
                    prev != 165 && prev != 161 && prev != 164) {
                    entry[pos] = ENTRY_WIDE_GAP;
                    pos = (pos + 1) & RENDER_ENTRY_MASK;
                }

                if (ch > 31) {
                    if (work[RENDER_SPACED_OFS] != 0 &&
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
                        entry[pos] = (u16)(_call_via_r9(&st) + 0xFFFF);
                        pos = (pos + 1) & RENDER_ENTRY_MASK;
                        break;
                    case 22:
                        value = UiRender_LookupNamedValue(5, clear);
                        mag = value;
                        if (value < 0)
                            mag = -value;
                        plural = 1;
                        if (mag <= 1)
                            plural = 0;
                        num = Text_FormatNumber(numbuf, value, 0);
                        off = num - numbuf;
                        while (off != 16 && numbuf[off] != 0) {
                            entry[pos] = numbuf[off];
                            pos = (pos + 1) & RENDER_ENTRY_MASK;
                            off++;
                        }
                        break;
                    case 19:
                        no = _call_via_r9(&st) - 1;
                        UiText_DecodeMessage(
                            UiRender_LookupNamedValue(3, clear) + 0x741,
                            name, 24);
                        pos = UiText_AppendArticleName(0, name, pos, entry, no, plural,
                                            &suffix);
                        break;
                    case 20:
                        no = _call_via_r9(&st) - 1;
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
                            BattleFx_FindConditionResourceFar(
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
                        src = Runtime_GetObject(
                            *(s32 *)(gGameState + 500));
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
                        no = _call_via_r9(&st) - 1;
                        src = Runtime_GetObject(
                            UiRender_LookupNamedValue(1, clear));
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
                        no = _call_via_r9(&st) - 1;
                        src = Runtime_GetObject(no);
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
                        no = (_call_via_r9(&st) - 1) * 2;
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
        *(u16 *)(work + RENDER_ENTRY_COUNT_OFS) = (u16)next;
        Runtime_ReleaseHeapBlock(TEXT_WORK_BLOCK);
        *(u16 *)(work + RENDER_ENTRY_START_OFS) = (u16)start;
    }

    if (clear != 0)
        UiWork_ClearValueNameTables();

    return start;
}
