#include "types.h"

/*
 * Per-frame scanline table builder for the screen transition effect that
 * DisplayTransition_InitializeState (games/gs1/recon/en/main/0808fe38.c)
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
 * (left << 8) | right in the GBA window-register layout.  Func_08015268 is
 * handed the line array before the mode fill runs.
 *
 * Uncertain, and left as raw offsets or neutral names:
 *   - the roles of the +0x539..+0x53e control bytes beyond what the code
 *     shows (a step counter, a step limit, a follow-on mode and a variant
 *     selector),
 *   - the identity of the two IWRAM routines called through the compiler's
 *     __call_via_rN veneer bank at 0x080072e4.  The argument shapes and the
 *     way their results are used make one an integer ratio and the other an
 *     integer square root, but neither has been read,
 *   - the three ROM tables, which are only indexed here,
 *   - the third, discarded read of DMA0CNT_H in the teardown path.  The
 *     reference loads DMA0CNT_H once more after the second read-modify-write
 *     and drops the value; the bare volatile read below reproduces it, but
 *     the statement it came from is a guess.
 *
 * Residual, measured with `alchemy score --owner 0808f52c`: 2268 candidate
 * bytes against 2316 reference bytes.  Every branch, loop, call and store is
 * present and the emitted blocks line up case for case; what is left is
 * register allocation.  The reference keeps each scanline loop counter, and
 * the loop-invariant square-root address, in the high registers r8..fp, which
 * costs it an extra `mov` on entry and two more per iteration.  This source
 * puts those counters in r5..r7, so each of the eight fill loops comes out
 * four to six bytes shorter.  No source spelling tried here moved the
 * allocator, and contriving one would not be evidence.
 */

#define DisplayTransition_UpdateScanlineTable Func_0808f52c

/* Resolved project symbols. */
void Func_08004278(void *callback);  /* Scheduler_RemoveCallback */
void Func_08015268(void *lines);
u32 Func_08004458(void);             /* Random16 */
s32 Func_080022ec(s32 num, s32 den); /* Math_Div */
void *Func_0808ba1c(s32 id);         /* ObjectTable_Get */

struct BattleSelectionWork {
    u8 unk_000[500];
    s32 object_id;
};
extern struct BattleSelectionWork Data_02000240;
extern u8 *Data_03001e70_a[]; /* IWRAM cell table; [0] and [23] are used here */
extern u8 Data_0809e8ac[];  /* 33 unsigned radii, indexed by the phase step */
extern s8 Data_0809e8ce[];  /* 32 signed widths, indexed by a running counter */
extern u8 Data_0809f840[];  /* 32 records of 28 bytes, see TransitionSpan */

/* Called through the __call_via_rN veneer bank, so the target address is a
   plain constant in the compiler's eyes rather than a linked symbol. */
typedef s32 (*RatioProc)(s32 num, s32 den);
typedef s32 (*SqrtProc)(s32 value);

#define Runtime_Ratio ((RatioProc)0x03000380)
#define Runtime_Sqrt ((SqrtProc)0x030001d8)

#define REG_DISPCNT (*(volatile u16 *)0x04000000)
#define REG_DMA0CNT_H (*(volatile u16 *)0x040000ba)
#define FrameCounter (*(volatile u32 *)0x03001e40)

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
    u8 *sys;
    u16 *dst;
    s32 *cam;
    s32 *obj;
    u32 i;
    u32 j;
    s32 k;
    s32 n;
    s32 cnt;
    s32 val;
    s32 v;
    s32 r;
    s32 rr;
    s32 d;
    s32 h;
    s32 lo;
    s32 hi;
    s32 cx;
    s32 cy;
    s32 ax;
    s32 ay;
    s32 g;
    s32 nx;
    s32 ny;
    s32 dx;
    s32 dy;
    s32 x0;
    s32 y0;
    u32 t;

    sys = Data_03001e70_a[0];
    p = (struct DisplayTransitionWork *)Data_03001e70_a[23];

    if (p->field_53c != 0) {
        if (p->field_53d >= p->field_53c) {
            p->field_53c = 0;
            if (p->field_53e == 0) {
                if (p->field_53b == 64) {
                    REG_DISPCNT &= 0x81ff;
                } else {
                    REG_DISPCNT &= 0x9fff;
                }
                Func_08004278((void *)0x0808f499);
                Func_08004278((void *)0x0808f52d);
                REG_DMA0CNT_H &= 0xc5ff;
                REG_DMA0CNT_H &= 0x7fff;
                REG_DMA0CNT_H;
                return;
            }
            p->mode = p->field_53e;
        } else {
            p->field_53d = p->field_53d + 1;
            p->phase = p->field_53a
                + Runtime_Ratio(p->field_53d * (p->field_53b - p->field_53a),
                                p->field_53c);
        }
    }

    dst = p->buf[1 ^ p->page];
    Func_08015268(dst + 2);

    switch (p->mode) {
    case 0:
        /* Static noise: a random left edge per scanline, widening with the
           phase step through the radius table. */
        *dst++ = 0x7f7f;
        *dst++ = 1;
        v = p->phase;
        if (v & 32) {
            k = 32 - (v & 31);
        } else {
            k = v & 31;
        }
        lo = Data_0809e8ac[k];
        for (i = 0; i < 160; i++) {
            t = ((u32)(241 - lo) * Func_08004458()) >> 16;
            dst[0] = (u16)((t << 8) | (t + lo));
            dst += 2;
        }
        break;

    case 1:
        /* Table-driven shape: three spans per record, each either a run of a
           constant value or a linear ramp of both edges. */
        v = p->phase;
        span = (struct TransitionSpan *)(Data_0809f840 + (v & 31) * 28);
        if (v & 32) {
            *dst++ = ((u16 *)span)[0];
            *dst++ = ((u16 *)span)[1];
        } else {
            *dst++ = ((u16 *)span)[1];
            *dst++ = ((u16 *)span)[0];
        }
        span++;
        for (j = 0; j < 3; j++) {
            cnt = span->cnt;
            val = span->val;
            if (cnt != 0) {
                if (val != 0) {
                    for (k = 0; k < cnt; k++) {
                        dst[0] = (u16)val;
                        dst += 2;
                    }
                } else {
                    x0 = span->x0;
                    y0 = span->y0;
                    dx = span->x1 - x0;
                    dy = span->y1 - y0;
                    nx = 0;
                    ny = 0;
                    n = cnt;
                    while (n != 0) {
                        dst[0] = (u16)(((Func_080022ec(nx, cnt) + x0) << 8)
                                       + (Func_080022ec(ny, cnt) + y0));
                        ny += dy;
                        nx += dx;
                        dst += 2;
                        n--;
                    }
                }
            }
            span++;
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
        r = (v & 31) << 4;
        for (i = 0; i < 160; i++) {
            t = r + ((16 * Func_08004458()) >> 16);
            if (t > 255) {
                t = 255;
            }
            dst[0] = (u16)t;
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
        r = v * 5;
        rr = (r * r) << 16;
        for (i = 0; i < 160; i++) {
            d = i - 80;
            h = Runtime_Sqrt(rr - ((d * d) << 16)) >> 8;
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
        lo = (s32)((u32)(240 * (v & 31)) >> 5);
        hi = ((240 - lo) << 8) + 240;
        for (i = 0; i < 160; i += 2) {
            dst[0] = (u16)lo;
            dst[2] = (u16)hi;
            dst += 4;
        }
        break;

    case 7:
        /* Ellipse centred on the tracked object, suppressed on odd frames. */
        cam = (s32 *)(sys + 0xe4);
        ax = cam[0] & 0xffff0000;
        ay = cam[1] & 0xffff0000;
        obj = (s32 *)Func_0808ba1c(Data_02000240.object_id);
        cx = (obj[2] - ax) / 0x10000;
        cy = ((obj[4] - obj[3]) - ay) / 0x10000 - 16;
        FrameCounter;
        *dst++ = p->field_536;
        *dst++ = p->field_534;
        v = p->phase;
        if ((v & 32) == 0) {
            r = 32 - (v & 31);
        } else {
            r = v & 31;
        }
        if (FrameCounter & 1) {
            r = 0;
        }
        r = r * 5;
        rr = (r * r) << 16;
        for (i = 0; i < 160; i++) {
            d = i - cy;
            h = Runtime_Sqrt(rr - ((d * d * 3) << 15)) >> 8;
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
        cam = (s32 *)(sys + 0xe4);
        ax = cam[0] & 0xffff0000;
        ay = cam[1] & 0xffff0000;
        cx = (p->field_52c - ax) / 0x10000;
        cy = (p->field_530 - ay) / 0x10000 - 16;
        g = FrameCounter - cy * 2;
        *dst++ = p->field_534;
        *dst++ = p->field_536;
        v = p->phase;
        if (v & 32) {
            r = 32 - (v & 31);
        } else {
            r = v & 31;
        }
        r = r * 5;
        rr = (r * r) << 16;
        for (i = 0; i < 160; i++) {
            d = i - cy;
            h = Runtime_Sqrt(rr - ((d * d * 3) << 15)) >> 8;
            lo = cx - h;
            hi = cx + h;
            if (lo < hi) {
                d = Data_0809e8ce[g & 31];
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
            g += 2;
        }
        break;

    case 9:
        /* As mode 7, with the control halfwords in the other order and no
           odd-frame suppression. */
        cam = (s32 *)(sys + 0xe4);
        ax = cam[0] & 0xffff0000;
        ay = cam[1] & 0xffff0000;
        obj = (s32 *)Func_0808ba1c(Data_02000240.object_id);
        cx = (obj[2] - ax) / 0x10000;
        cy = ((obj[4] - obj[3]) - ay) / 0x10000 - 16;
        FrameCounter;
        *dst++ = p->field_534;
        *dst++ = p->field_536;
        v = p->phase;
        if (v & 32) {
            r = 32 - (v & 31);
        } else {
            r = v & 31;
        }
        r = r * 5;
        rr = (r * r) << 16;
        for (i = 0; i < 160; i++) {
            d = i - cy;
            h = Runtime_Sqrt(rr - ((d * d * 3) << 15)) >> 8;
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
        obj = (s32 *)Func_0808ba1c(Data_02000240.object_id);
        cx = (obj[2] - ax) / 0x10000;
        cy = ((obj[4] - obj[3]) - ay) / 0x10000 - 8;
        FrameCounter;
        *dst++ = p->field_534;
        *dst++ = p->field_536;
        v = p->phase;
        if (v & 32) {
            r = 32 - (v & 31);
        } else {
            r = v & 31;
        }
        r = r * 5;
        rr = (r * r) << 16;
        for (i = 0; i < 160; i++) {
            d = i - cy;
            h = Runtime_Sqrt(rr - ((d * d * 3) << 14)) >> 8;
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
