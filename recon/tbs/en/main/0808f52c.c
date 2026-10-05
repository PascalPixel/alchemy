#include "TYPES.H"
#include "GAME_STATE.H"
#include "IWRAM_CALL.H"
#include "GLOBAL_CELLS.H"
#include "IO_REG.H"
#include "FRAME.H"
#include "DISPTRAN.H"
#include "CALLBACK_SCHEDULER.H"

/*
 * Per-frame scanline table builder for the screen transition effect that
 * DisplayTransition_InitializeState (recon/tbs/en/main/0808fe38.c)
 * arms.  That function allocates the 0x540-byte work block, seeds
 * mode (+0x528), phase (+0x52a), +0x534 and +0x536, and schedules this
 * routine at 0x0808f52d together with its companion at 0x0808f499.
 *
 * Every frame the routine
 *   1. advances the phase counter, or tears the effect down when the
 *      configured step count has been reached,
 *   2. picks the back buffer of the two 644-byte scanline buffers,
 *   3. fills 160 scanline entries according to the selected mode, and
 *   4. flips the buffer index so the DMA reads the finished buffer.
 *
 * Each buffer holds two leading control halfwords followed by 160 pairs of
 * halfwords; only the first halfword of each pair is written here, packed as
 * (left << 8) | right in the GBA window-register layout.  UiWindow_FillFromSceneFar is
 * handed the line array before the mode fill runs.
 *
 * Uncertain, and left as raw offsets or neutral names:
 *   - the roles of the +0x539..+0x53e control bytes beyond what the code
 *     shows (a step counter, a step limit, a follow-on mode and a variant
 *     selector),
 *   - the three ROM tables, which are only indexed here.
 *
 * 2026-10-05 deep finisher 12: no address constants left. The two IWRAM
 * routines are IWRAM_CALL.H's fixed entries Iwram_SignedDivide and
 * Iwram_Sqrt, the work cells are gMapWork[0] and [23] (GLOBAL_CELLS.H; the
 * RAM_BUFFER.H constants Ram_MapWork and Ram_DisplayWork lose the reference's
 * ldr r3, [r3, #92] and score 948), the frame word is gFrameCount, the two
 * callbacks are removed by name and the game state cell is selected_actor.
 * alchemy diff 939 (885 listing instructions, 870 here), unchanged.
 *
 * 2026-10-05 deep finisher 20: alchemy diff 30 (885 and 885 instructions).
 * Each step, with the score after it:
 *   - the DMA stop is the channel idiom the other DMA users have,
 *     dma0[5] twice and a bare read of dma0[5]: 929;
 *   - the step is one expression, (end - start) * ++step / count, so the
 *     two ramp ends load before the step byte is stored: 712;
 *   - the mode 1 ramp is a counted loop, start + (end - start) * k / cnt,
 *     which loop.c strength-reduces to two running sums and reverses to the
 *     native count-down (an explicit running sum cannot): structure 84;
 *   - v also carries the radius and the line offset (v = v * 5,
 *     v = i - 80): 646; lo also carries mode 1's start y, which gives it
 *     a call-saved register and puts lo in r6 and v in r5: 431;
 *   - mode 8 has its own camera pair (bx, by in r2 and r1): 405;
 *   - span = &shape->spans[i] at the top of each pass: 355;
 *     start + (...), not (...) + start, for both ramp ends: 353;
 *   - mode 8's running index is k, the mode 1 counter (k in r9, which also
 *     frees r3 and r2 for the running sums): 256;
 *   - mode 1's start x is rr, the radius square of the other modes
 *     (rr in r11, cy in r10): 218;
 *   - mode 0's random value is its own w: 208; its width stays inline
 *     (241 - v, hoisted after i = 0): 206;
 *   - mode 2 is v &= 31 and (v << 4) inside the loop, its value in h,
 *     made u32 (the square roots shift it arithmetically before the
 *     conversion, mode 2 compares it unsigned): 194;
 *   - mode 4's left edge is v, (u32)(240 * (v & 31)) >> 5, and its right
 *     edge an int computed inside the loop (a u16 cast there narrows the
 *     arithmetic and loads 240 with ldrh): 169;
 *   - mode 8's camera pointer is its own (cam2 in r1): 153;
 *   - mode 1's span value and end y are hi, its end x d: 119;
 *   - mode 8's index is -cy * 2 + gFrameCount, so cy * 2 is formed
 *     before the frame load: 30.  (gFrameCount - cy * 2 and its other
 *     spellings stay at 119-123; -2 * cy + gFrameCount scores 36.)
 * Tried and worse: v and lo as the camera pair in modes 7/9/77 (204),
 * one separate counter for the ramp loop (no change), the camera read
 * through sys without a pointer (215).
 *
 * Remaining (30): modes 7, 9 and 77 keep the camera pointer in r0, the
 * reference in r2.  cam is global there and inherits sys's preferences
 * (r0, and r1 from mode 8's local cam2).  A pointer per mode, or one
 * pinned to r2, does get r2 (46), but then ax takes r2 as a preference
 * from the load through it and hands it to the cx temporary, which the
 * reference keeps in r3.
 */

/* Resolved project symbols. */
void UiWindow_FillFromSceneFar(void *lines);
u32 Random16(void);             /* Random16 */
void *ObjectTable_Get(s32 id);         /* ObjectTable_Get */

extern u8 Data_0809e8ac[];  /* 33 unsigned radii, indexed by the phase step */
extern s8 Data_0809e8ce[];  /* 32 signed widths, indexed by a running counter */
extern u8 Data_0809f840[];  /* 32 records of 28 bytes, see TransitionSpan */



/* One span of the mode 1 table: either `cnt` copies of `val`, or, when `val`
   is zero, `cnt` scanlines interpolating x0->x1 and y0->y1. */
struct TransitionSpan {
    u16 cnt;
    u16 val;
    u8 x0;
    u8 x1;
    u8 y0;
    u8 y1;
};

/* One record of the mode 1 table: the two control halfwords, then three
   spans. */
struct TransitionShape {
    u16 first;
    u16 second;
    struct TransitionSpan spans[3];
};

struct DisplayTransitionWork {
    u16 buf[2][322];   /* two 644-byte scanline buffers */
    u8 unk_508[0x20];
    u16 mode;          /* 0x528 */
    u16 phase;         /* 0x52a */
    s32 field_52c;
    s32 field_530;
    u16 field_534;
    u16 field_536;
    u8 field_538;
    u8 page;           /* 0x539: index of the buffer the DMA is reading */
    s8 field_53a;      /* start value of the phase ramp */
    s8 field_53b;      /* end value of the phase ramp */
    s8 field_53c;      /* step count, zero while idle */
    s8 field_53d;      /* steps taken so far */
    s8 field_53e;      /* mode to continue with, zero to tear down */
};

void DisplayTransition_UpdateScanlineTable(void)
{
    struct DisplayTransitionWork *p;
    struct TransitionSpan *span;
    struct TransitionShape *shape;
    u8 *sys;
    u16 *dst;
    s32 *cam;
    s32 *cam2;
    s32 *obj;
    u32 i;
    s32 k;
    s32 cnt;
    s32 v;
    s32 rr;
    s32 d;
    u32 h;
    s32 lo;
    s32 hi;
    s32 cx;
    s32 cy;
    s32 ax;
    s32 ay;
    s32 bx;
    s32 by;
    u32 w;
    s32 e;

    sys = (u8 *)gMapWork[0];
    p = (struct DisplayTransitionWork *)gMapWork[23];

    if (p->field_53c != 0) {
        if (p->field_53d >= p->field_53c) {
            p->field_53c = 0;
            if (p->field_53e == 0) {
                if (p->field_53b == 64) {
                    REG_DISPCNT &= 0x81ff;
                } else {
                    REG_DISPCNT &= 0x9fff;
                }
                Scheduler_RemoveCallback((u32)BattleFx_StartWindowHBlankDma);
                Scheduler_RemoveCallback((u32)DisplayTransition_UpdateScanlineTable);
                {
                    volatile u16 *dma0 = REG_DMA0;

                    dma0[5] &= 0xc5ff;
                    dma0[5] &= 0x7fff;
                    dma0[5];
                }
                return;
            }
            p->mode = p->field_53e;
        } else {
            p->phase = p->field_53a
                + Iwram_SignedDivide((p->field_53b - p->field_53a) * ++p->field_53d,
                                p->field_53c);
        }
    }

    dst = p->buf[1 ^ p->page];
    UiWindow_FillFromSceneFar(dst + 2);

    switch (p->mode) {
    case 0:
        /* Static noise: a random left edge per scanline, widening with the
           phase step through the radius table. */
        *dst++ = 0x7f7f;
        *dst++ = 1;
        v = p->phase;
        if (v & 32) {
            v = 32 - (v & 31);
        } else {
            v = v & 31;
        }
        v = Data_0809e8ac[v];
        for (i = 0; i < 160; i++) {
            w = (Random16() * (u32)(241 - v)) >> 16;
            dst[0] = (u16)((w << 8) | (w + v));
            dst += 2;
        }
        break;

    case 1:
        /* Table-driven shape: three spans per record, each either a run of a
           constant value or a linear ramp of both edges. */
        v = p->phase;
        shape = (struct TransitionShape *)(Data_0809f840 + (v & 31) * 28);
        if (v & 32) {
            *dst++ = shape->first;
            *dst++ = shape->second;
        } else {
            *dst++ = shape->second;
            *dst++ = shape->first;
        }
        for (i = 0; i < 3; i++) {
            span = &shape->spans[i];
            cnt = span->cnt;
            hi = span->val;
            if (cnt != 0) {
                if (hi != 0) {
                    for (k = 0; k < cnt; k++) {
                        dst[0] = (u16)hi;
                        dst += 2;
                    }
                } else {
                    rr = span->x0;
                    lo = span->y0;
                    d = span->x1;
                    hi = span->y1;
                    for (k = 0; k < cnt; k++) {
                        dst[0] = (u16)(((rr + (d - rr) * k / cnt) << 8)
                                       + (lo + (hi - lo) * k / cnt));
                        dst += 2;
                    }
                }
            }
        }
        break;

    case 2:
        /* Dissolve: a random amount added to a rising floor, clamped. */
        v = p->phase - 1;
        if (v & 32) {
            *dst++ = 1;
            *dst++ = 0x7f7f;
        } else {
            *dst++ = 0x7f7f;
            *dst++ = 1;
        }
        v &= 31;
        for (i = 0; i < 160; i++) {
            h = (v << 4) + ((16 * Random16()) >> 16);
            if (h > 255) {
                h = 255;
            }
            dst[0] = (u16)h;
            dst += 2;
        }
        break;

    case 3:
        /* Circle centred on the screen, radius five per phase step. */
        v = p->phase;
        *dst++ = 0x7f7f;
        *dst++ = 1;
        if ((u32)v > 32) {
            *dst++ = 1;
            *dst++ = 0x7f7f;
            v = 64 - v;
        }
        v = v * 5;
        rr = (v * v) << 16;
        for (i = 0; i < 160; i++) {
            v = i - 80;
            h = Iwram_Sqrt(rr - ((v * v) << 16)) >> 8;
            lo = 120 - h;
            hi = h + 120;
            if (lo < 0) {
                lo = 0;
            }
            if (hi < 0) {
                hi = 0;
            }
            if (lo > 240) {
                lo = 240;
            }
            if (hi > 240) {
                hi = 240;
            }
            dst[0] = (u16)((lo << 8) + hi);
            dst += 2;
        }
        break;

    case 4:
        /* Horizontal bars: even scanlines shrink from the left, odd ones
           from the right. */
        v = p->phase;
        if (v & 32) {
            *dst++ = 1;
            *dst++ = 0x7f7f;
        } else {
            *dst++ = 0x7f7f;
            *dst++ = 1;
        }
        v = (u32)(240 * (v & 31)) >> 5;
        for (i = 0; i < 160; i += 2) {
            e = ((240 - v) << 8) + 240;
            dst[0] = (u16)v;
            dst[2] = (u16)e;
            dst += 4;
        }
        break;

    case 7:
        /* Ellipse centred on the tracked object, suppressed on odd frames. */
        cam = (s32 *)(sys + 0xe4);
        ax = cam[0] & 0xffff0000;
        ay = cam[1] & 0xffff0000;
        obj = (s32 *)ObjectTable_Get(gGameState.selected_actor);
        cx = (obj[2] - ax) / 0x10000;
        cy = ((obj[4] - obj[3]) - ay) / 0x10000 - 16;
        gFrameCount;
        *dst++ = p->field_536;
        *dst++ = p->field_534;
        v = p->phase;
        if ((v & 32) == 0) {
            v = 32 - (v & 31);
        } else {
            v = v & 31;
        }
        if (gFrameCount & 1) {
            v = 0;
        }
        v = v * 5;
        rr = (v * v) << 16;
        for (i = 0; i < 160; i++) {
            v = i - cy;
            h = Iwram_Sqrt(rr - ((v * v * 3) << 15)) >> 8;
            lo = cx - h;
            hi = cx + h;
            if (lo < 0) {
                lo = 0;
            }
            if (hi < 0) {
                hi = 0;
            }
            if (lo > 240) {
                lo = 240;
            }
            if (hi > 240) {
                hi = 240;
            }
            dst[0] = (u16)((lo << 8) + hi);
            dst += 2;
        }
        break;

    case 8:
        /* Ellipse centred on the stored work position, with a per-scanline
           ragged edge taken from the signed width table. */
        cam2 = (s32 *)(sys + 0xe4);
        bx = cam2[0] & 0xffff0000;
        by = cam2[1] & 0xffff0000;
        cx = (p->field_52c - bx) / 0x10000;
        cy = (p->field_530 - by) / 0x10000 - 16;
        k = -cy * 2 + gFrameCount;
        *dst++ = p->field_534;
        *dst++ = p->field_536;
        v = p->phase;
        if (v & 32) {
            v = 32 - (v & 31);
        } else {
            v = v & 31;
        }
        v = v * 5;
        rr = (v * v) << 16;
        for (i = 0; i < 160; i++) {
            v = i - cy;
            h = Iwram_Sqrt(rr - ((v * v * 3) << 15)) >> 8;
            lo = cx - h;
            hi = cx + h;
            if (lo < hi) {
                d = Data_0809e8ce[k & 31];
                lo -= d;
                hi += d;
                if (lo >= hi) {
                    lo = 240;
                    hi = 240;
                }
            }
            if (lo < 0) {
                lo = 0;
            }
            if (hi < 0) {
                hi = 0;
            }
            if (lo > 240) {
                lo = 240;
            }
            if (hi > 240) {
                hi = 240;
            }
            dst[0] = (u16)((lo << 8) + hi);
            dst += 2;
            k += 2;
        }
        break;

    case 9:
        /* As mode 7, with the control halfwords in the other order and no
           odd-frame suppression. */
        cam = (s32 *)(sys + 0xe4);
        ax = cam[0] & 0xffff0000;
        ay = cam[1] & 0xffff0000;
        obj = (s32 *)ObjectTable_Get(gGameState.selected_actor);
        cx = (obj[2] - ax) / 0x10000;
        cy = ((obj[4] - obj[3]) - ay) / 0x10000 - 16;
        gFrameCount;
        *dst++ = p->field_534;
        *dst++ = p->field_536;
        v = p->phase;
        if (v & 32) {
            v = 32 - (v & 31);
        } else {
            v = v & 31;
        }
        v = v * 5;
        rr = (v * v) << 16;
        for (i = 0; i < 160; i++) {
            v = i - cy;
            h = Iwram_Sqrt(rr - ((v * v * 3) << 15)) >> 8;
            lo = cx - h;
            hi = cx + h;
            if (lo < 0) {
                lo = 0;
            }
            if (hi < 0) {
                hi = 0;
            }
            if (lo > 240) {
                lo = 240;
            }
            if (hi > 240) {
                hi = 240;
            }
            dst[0] = (u16)((lo << 8) + hi);
            dst += 2;
        }
        break;

    case 77:
        /* As mode 9, eight scanlines higher and half as tall. */
        cam = (s32 *)(sys + 0xe4);
        ax = cam[0] & 0xffff0000;
        ay = cam[1] & 0xffff0000;
        obj = (s32 *)ObjectTable_Get(gGameState.selected_actor);
        cx = (obj[2] - ax) / 0x10000;
        cy = ((obj[4] - obj[3]) - ay) / 0x10000 - 8;
        gFrameCount;
        *dst++ = p->field_534;
        *dst++ = p->field_536;
        v = p->phase;
        if (v & 32) {
            v = 32 - (v & 31);
        } else {
            v = v & 31;
        }
        v = v * 5;
        rr = (v * v) << 16;
        for (i = 0; i < 160; i++) {
            v = i - cy;
            h = Iwram_Sqrt(rr - ((v * v * 3) << 14)) >> 8;
            lo = cx - h;
            hi = cx + h;
            if (lo < 0) {
                lo = 0;
            }
            if (hi < 0) {
                hi = 0;
            }
            if (lo > 240) {
                lo = 240;
            }
            if (hi > 240) {
                hi = 240;
            }
            dst[0] = (u16)((lo << 8) + hi);
            dst += 2;
        }
        break;

    default:
        break;
    }

    p->page ^= 1;
}
