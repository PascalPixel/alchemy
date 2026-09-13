#include "types.h"
#include "global_cells.h"
#include "global_progress.h"
#include "gs1_edition.h"

/*
 * Message-script step for one render channel.
 *
 * UiWork_ProcessRenderChannels (main:08016868) calls this once per frame for
 * every open channel whose work has no pending transition. The channel holds
 * a program counter into the shared halfword entry table at
 * RENDER_ENTRY_TBL_OFS; codes 0..30 are control opcodes and everything above
 * 30 is a glyph to draw. One call consumes several entries: the repeat count
 * comes from the per-speed byte table at 0x0807380B, or from the clamped
 * 0x03001CD0 setting while the menu state byte is set. A control opcode that
 * produces visible output shortens the remaining budget to one step.
 *
 * Return value is the caller's switch selector: 8 = script finished,
 * 9 = cancelled through UiWork_CheckCancelByModeInput, 0 = still running.
 *
 * Uncertain: the three parallel byte tables at 0x08073808 / +3 / +6 are all
 * indexed by the settings byte at Data_02000240[0x20C] (message speed), but
 * only their roles here are evidenced - repeat budget, inter-character delay
 * and sound-cue cooldown. Three render-state offsets used here have no name
 * in gs1_edition.h yet - the halfwords at 0x12B6 and 0x12F6 and the byte at
 * 0x12F8 - so they are spelled locally below; they are English offsets and
 * belong in that header once their Japanese counterparts are measured.
 *
 * Residual: this is a draft, not a match. Measured against the reference it
 * is 1592 candidate bytes against 1592, 627 instructions against 628, with
 * 717 differing halfwords and 700 wrong instructions. Control flow, the
 * 31-entry jump table, every call and every store line up; erasing register
 * numbers leaves about 145 of 628 instruction lines disagreeing, so most of
 * the residual is allocation: the reference keeps the channel argument in r6
 * and the work pointer in r5, this candidate keeps them in r7 and r6, and
 * every dependent scratch register shifts with them.
 *
 * Four smaller differences are measured but unexplained:
 *   - The reference splits (Data_02000240 + 0x20C) into a pooled symbol plus
 *     a runtime 0x20C (movs #131, lsls #2) at all three use sites; GCC folds
 *     the equivalent here into one pooled address.
 *   - The reference reloads *ADDR_03001AE8 just after the prologue and
 *     discards the value, keeping the cell address live in r1 for the real
 *     read further down. No ordinary spelling reproduced the dead load.
 *   - In case 3 the reference compares the pen y against a pooled 0xCFF and
 *     branches bls; GCC canonicalises the same comparison here to 0xD00 with
 *     bcc, costing one instruction.
 *   - At the six (pc + 1) & RENDER_ENTRY_MASK sites the reference loads the
 *     mask as a word and this candidate as a halfword. Routing the masked
 *     index through an s32 temporary was tried and made the whole function
 *     worse (720 differing halfwords, 724 wrong instructions), so the
 *     halfword form is kept.
 *
 * The short-lived zero temporaries below are not decoration: writing the
 * literal directly makes GCC narrow the store to HImode and load the
 * constant from the pool, where the reference emits movs. The same idiom is
 * already used in games/gs1/src/ui/text/draw/draw_resource.c.
 */

/* Not yet in gs1_edition.h; English offsets only. */
#define RENDER_CUE_ID_OFS 0x12B6
#define RENDER_CUE_WAIT_OFS 0x12F6
#define RENDER_PROMPT_OFS 0x12F8

/* Per-message-speed byte tables. */
#define SPEED_DELAY_TBL 0x08073808
#define SPEED_STEPS_TBL 0x0807380B
#define SPEED_CUE_WAIT_TBL 0x0807380E

/* Glyph metric records, 0x20 bytes each, first record is character 0x20. */
#define GLYPH_METRIC_TBL 0x08032224

/* Interpreted as the window work in main:08016f2c. */
struct Work {
    s32 unknown00;      /* 0x00 */
    s32 unknown04;      /* 0x04 */
    u16 width;          /* 0x08 */
    u16 height;         /* 0x0a */
    u16 x;              /* 0x0c */
    u16 y;              /* 0x0e */
    u16 unknown10;      /* 0x10 */
    u16 unknown12;      /* 0x12 */
    u16 state;          /* 0x14 */
    u16 flags;          /* 0x16 */
};

/* The 0x28-byte channel record; main:08019854 reads the same layout. */
struct RenderChannel {
    struct Work *work;  /* 0x00 */
    u16 pos_x;          /* 0x04  pen x, 8.8 fixed point */
    u16 pos_y;          /* 0x06  pen y, 8.8 fixed point */
    u16 space_pad[4];   /* 0x08  per-line extra space width */
    u16 line;           /* 0x10 */
    u16 pc;             /* 0x12 */
    u16 wait;           /* 0x14 */
    u16 param0;         /* 0x16 */
    u16 param1;         /* 0x18 */
    u16 param2;         /* 0x1a */
    u16 pause;          /* 0x1c */
    u16 origin_x;       /* 0x1e */
    u16 done;           /* 0x20 */
    u16 delay;          /* 0x22 */
    u16 skip;           /* 0x24 */
};

#define UiWork_ResetChannelTransition Func_080167d8
#define RenderOutput_PrepareForRedraw Func_08016478
#define UiWindow_EraseBorderRect Func_08016178
#define UiWindow_FitOnScreen Func_0801868c
#define UiWork_CopyParamsToRenderWork Func_080167ac
#define UiWork_CheckCancelByInput Func_0801999c
#define UiWork_CheckCancelByModeInput Func_080199ec
#define UiWork_RunMessageScript Func_080168f4

void Func_080167e0(s32 mode);
void UiWork_ResetChannelTransition(void *channel);
void RenderOutput_PrepareForRedraw(void *work);
void UiWindow_EraseBorderRect(s32 x, s32 y, s32 width, s32 height);
void Func_080170f8(s32 x, s32 y, s32 width, s32 height);
void UiWindow_FitOnScreen(
    s32 no,
    s32 *px,
    s32 *py,
    u32 *pw,
    u32 *ph,
    u16 *pad,
    s32 mode);
void UiWork_CopyParamsToRenderWork(void *channel);
s32 UiWork_CheckCancelByInput(void *channel);
s32 UiWork_CheckCancelByModeInput(void *channel);
void Func_08003f3c(s32 cue);
void Audio_PlayCue(s32 cue);
s32 Func_08018cac(void *work, s32 code, s32 x, s32 y, s32 mode);

s32 UiWork_RunMessageScript(struct RenderChannel *ch)
{
    u8 *base;
    struct Work *pane;
    s32 cnt;
    s32 code;
    s32 next;
    s32 speed;
    s32 idx;
    s32 ofs;
    s32 tmp;
    u32 uval;
    s32 gx;
    s32 gy;
    s32 cue_wait;
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    s32 old_w;
    s32 old_h;
    s32 zero;

    base = *(u8 **)ADDR_03001E8C;
    cnt = ((u8 *)SPEED_STEPS_TBL)[Data_02000240_asBytes[0x20C]];
    if (base[RENDER_MENU_STATE_OFS] != 0) {
        speed = *(u16 *)0x03001CD0;
        if (speed < 0)
            speed = 0;
        if (speed > 2)
            speed = 2;
        cnt = speed * 5 + 3;
    }

    if (ch->pause != 0) {
        Func_080167e0(1);
        ch->pause--;
        return 0;
    }
    if (*(s32 *)ADDR_03001AE8 == 0) {
        if (ch->delay != 0) {
            ch->delay--;
            return 0;
        }
    }

    do {
        code = 0;
        if (ch->done == 0) {
            ofs = ch->pc * 2 + RENDER_ENTRY_TBL_OFS;
            code = *(u16 *)(base + ofs);
        }

        switch (code) {
        case 3:
            /* Line break. */
            ch->pos_x = ch->origin_x;
            if ((ch->work->flags & 8) != 0) {
                uval = ch->pos_y;
                if (uval > 0xCFFU) {
                    UiWork_ResetChannelTransition(ch);
                    cnt = 1;
                } else {
                    ch->pos_y = uval + 0xD00;
                }
            } else {
                ch->pos_y += 0xF00;
                uval = ch->line;
                if (uval <= 2U)
                    ch->line = uval + 1;
            }
            break;

        case 1:
            /* Page break: wait for input, then refit the window. */
            if (base[RENDER_MODE_OFS] != 0 && ch->wait < 0x384)
                *(s32 *)ADDR_03001AF8 = 0;
            ch->wait = 0x397;
            if (UiWork_CheckCancelByModeInput(ch) == 0) {
                pane = ch->work;
                if (pane->width == 0)
                    break;
                if (pane->height == 0)
                    break;
                if (base[RENDER_PROMPT_OFS] != 0)
                    break;
                Func_08018cac(
                    pane,
                    1,
                    pane->width * 4 - 8,
                    pane->height * 8 - 16,
                    1);
                base[RENDER_PROMPT_OFS] = 1;
                break;
            }
            pane = ch->work;
            x = pane->x;
            y = pane->y;
            old_w = pane->width;
            old_h = pane->height;
            idx = ch->pc;
            zero = 0;
            base[RENDER_PROMPT_OFS] = zero;
            RenderOutput_PrepareForRedraw(pane);
            if (ch->skip == 0 && (pane->width | pane->height) != 0)
                UiWindow_EraseBorderRect(
                    pane->x, pane->y, pane->width, pane->height);
            idx = (idx + 1) & RENDER_ENTRY_MASK;
            ofs = idx * 2 + RENDER_ENTRY_TBL_OFS;
            if (*(u16 *)(base + ofs) != 0
                && (pane->width | pane->height) != 0) {
                if (ch->skip != 0) {
                    UiWindow_EraseBorderRect(
                        pane->x, pane->y, pane->width, pane->height);
                } else {
                    UiWindow_FitOnScreen(
                        idx, &x, &y, (u32 *)&w, (u32 *)&h,
                        ch->space_pad, 0);
                    if ((pane->flags & 0x80) != 0) {
                        if (h != old_h)
                            y -= h - old_h;
                        if (y < 0)
                            y = 0;
                    }
                    if ((pane->flags & 0x100) == 0) {
                        x += (old_w - w) / 4;
                        UiWindow_FitOnScreen(
                            idx, &x, &y, (u32 *)&w, (u32 *)&h,
                            ch->space_pad, 2);
                    }
                    pane->x = x;
                    pane->y = y;
                    pane->width = w;
                    pane->height = h;
                }
                Func_080170f8(pane->x, pane->y, pane->width, pane->height);
            }
            ch->pos_x = ch->origin_x;
            ch->pos_y = 0;
            ch->line = 0;
            Func_08003f3c(*(u16 *)(base + RENDER_CUE_ID_OFS));
            *(u16 *)(base + RENDER_CUE_ID_OFS) = 99;
            break;

        case 2:
            /* Wait for input, report a cancel to the caller. */
            if (base[RENDER_MODE_OFS] != 0 && ch->wait < 0x384)
                *(s32 *)ADDR_03001AF8 = 0;
            if (UiWork_CheckCancelByModeInput(ch) != 0)
                return 9;
            ch->wait = 0x397;
            break;

        case 5:
            if (ch->wait == 0)
                ch->wait = 20;
            zero = 0;
            *(u16 *)(base + RENDER_CUE_WAIT_OFS) = zero;
            UiWork_CheckCancelByInput(ch);
            break;

        case 6:
            if (ch->wait == 0)
                ch->wait = 120;
            zero = 0;
            *(u16 *)(base + RENDER_CUE_WAIT_OFS) = zero;
            UiWork_CheckCancelByInput(ch);
            break;

        case 4:
            if (ch->wait == 0)
                ch->wait = 60;
            zero = 0;
            *(u16 *)(base + RENDER_CUE_WAIT_OFS) = zero;
            break;

        case 8:
            ch->pc = (ch->pc + 1) & RENDER_ENTRY_MASK;
            ofs = ch->pc * 2 + RENDER_ENTRY_TBL_OFS;
            ch->param0 = *(u16 *)(base + ofs);
            UiWork_CopyParamsToRenderWork(ch);
            break;

        case 9:
            ch->pc = (ch->pc + 1) & RENDER_ENTRY_MASK;
            ofs = ch->pc * 2 + RENDER_ENTRY_TBL_OFS;
            ch->param1 = *(u16 *)(base + ofs);
            UiWork_CopyParamsToRenderWork(ch);
            break;

        case 10:
            ch->pc = (ch->pc + 1) & RENDER_ENTRY_MASK;
            ofs = ch->pc * 2 + RENDER_ENTRY_TBL_OFS;
            ch->param2 = *(u16 *)(base + ofs);
            UiWork_CopyParamsToRenderWork(ch);
            break;

        case 7:
            ch->param1 = 0;
            ch->param0 = 15;
            ch->param2 = 10;
            UiWork_CopyParamsToRenderWork(ch);
            break;

        case 15:
            ch->pc = (ch->pc + 1) & RENDER_ENTRY_MASK;
            idx = ch->pc;
            ofs = idx * 2 + RENDER_ENTRY_TBL_OFS;
            ch->work->unknown12 = *(u16 *)(base + ofs);
            ch->wait = 10;
            ch->pc = (idx + 1) & RENDER_ENTRY_MASK;
            break;

        case 0:
        case 30:
            ch->done = 1;
            return 8;

        case 11:
        case 12:
        case 13:
        case 14:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
            break;

        default:
            /* Printable character. */
            gx = (ch->pos_x + 128) / 256;
            gy = (ch->pos_y + 128) / 256;
            cue_wait =
                ((u8 *)SPEED_CUE_WAIT_TBL)[Data_02000240_asBytes[0x20C]];
            if (base[RENDER_MODE_OFS] != 0)
                gx += 8;
            ofs = ((ch->pc + 1) & RENDER_ENTRY_MASK) * 2
                + RENDER_ENTRY_TBL_OFS;
            next = *(u16 *)(base + ofs);
            if (next == 222) {
                code |= 0x4000;
                ch->pc = (ch->pc + 1) & RENDER_ENTRY_MASK;
            } else if (next == 223) {
                code |= 0x8000;
                ch->pc = (ch->pc + 1) & RENDER_ENTRY_MASK;
            }
            pane = ch->work;
            if ((pane->flags & 8) == 0 && code > 32 && next > 32) {
                /* Pack a narrow pair into one cell when both fit. */
                if ((u16)(*(u16 *)(GLYPH_METRIC_TBL + ((code - 32) << 5))
                          + *(u16 *)(GLYPH_METRIC_TBL + ((next - 32) << 5)))
                    <= 15) {
                    code |= next << 8;
                    ch->pc = (ch->pc + 1) & RENDER_ENTRY_MASK;
                }
            }
            tmp = Func_08018cac(pane, code, gx, gy, 0);
            ch->delay =
                ((u8 *)SPEED_DELAY_TBL)[Data_02000240_asBytes[0x20C]];
            if (tmp != 0) {
                if (*(u16 *)(base + RENDER_RESULT_OFS) != 0) {
                    if (*(u16 *)(base + RENDER_CUE_WAIT_OFS) != 0) {
                        *(u16 *)(base + RENDER_CUE_WAIT_OFS) -= 1;
                    } else if (code != 32) {
                        Audio_PlayCue(
                            *(u16 *)(base + RENDER_RESULT_OFS) + (code & 3));
                        *(u16 *)(base + RENDER_CUE_WAIT_OFS) = cue_wait;
                    }
                }
                tmp = tmp << 8;
                if (code == 32)
                    tmp += ch->space_pad[ch->line];
                ch->pos_x += tmp;
            }
            if (code == 32 && base[RENDER_MENU_STATE_OFS] == 0)
                cnt = 1;
            goto advance;
        }

        if (base[RENDER_MENU_STATE_OFS] == 0)
            cnt = 1;

    advance:
        if (ch->wait != 0) {
            ch->wait--;
            if (ch->wait != 0)
                continue;
        }
        ch->pc = (ch->pc + 1) & RENDER_ENTRY_MASK;
    } while (--cnt != 0);

    return 0;
}
