#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "palbuf.h"
#include "types.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "heap_state.h"
#include "mem.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "global_cells.h"

void TitlePalette_UpdateFade(void)
{
    /* FAKEMATCH: needed to match: the byte-slot read avoids an extra add #128. */
    struct TitlePaletteWork *work = *(struct TitlePaletteWork **)((u8 *)gWorkSlot + 32 * sizeof(void *));
    u16 *delta = work->delta;
    u16 *current;
    u16 *packed;
    s32 i;
    u16 *bank;
    s32 blue;

    if (work->duration == 0)
        return;
    if ((s8)++work->step < work->duration) {
        current = work->current;
        for (i = 0; i <= 0x5ff; i++)
            *current++ += *delta++;
    } else {
        Dma_Set(work->target, work->current, DMA_ENABLE32 | DMA_32BIT | 0x300, REG_DMA3);
        work->duration = 0;
    }
    packed = work->packed[work->page ^ 1];
    blue = 0x7c00;
    i = 512;
    current = work->current;
    for (; i != 0; i--) {
        *packed++ = (current[0] & blue) | (((s16)current[1] >> 5) & 0x3e0) | (((s16)current[2] >> 10) & 0x1f);
        current += 3;
    }
    work->page ^= 1;
    bank = work->packed[work->page];
    {
        u16 *front = bank;

        IO_WRITE_QUEUE_PUSH(0x05000000, (u32)front, 0x84000080);
    }
    IO_WRITE_QUEUE_PUSH(0x05000200, (u32)(bank + 256), 0x84000080);
}

void Graphics_TransformPaletteBuffer(u32 mode, u16 *src, u16 *dst, s32 half)
{
    u32 i;
    u32 cnt = 512;
    s32 r;
    s32 b;
    u32 c;
    s32 g;

    if (mode == PALETTE_TRANSFORM_FROM_SOURCE)
        mode = *src;
    if (half == 1) {
        cnt = 256;
    } else if (half == 2) {
        dst += 768;
        src += 256;
        cnt = 256;
    }

    if (mode < PALETTE_TRANSFORM_FROM_SOURCE) {
        *dst++ = mode & 0x7c00;
        *dst++ = (mode & 0x03e0) << 5;
        *dst++ = (mode & 0x001f) << 10;
        Dma_Set(dst - 3, dst, (((cnt - 1) * 6) >> 1) | DMA_ENABLE32, REG_DMA3);
    } else if (mode < PALETTE_TRANSFORM_FLAGS) {
        switch (mode) {
        case PALETTE_TRANSFORM_GREY:
            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = ((c << 11) & 0xf800);
                g = ((c << 7) & 0x1f000);
                b = (c & 0x7c00);
                c = Iwram_SignedDivide(r + g + b, 7);
                *dst++ = c;
                *dst++ = c;
                *dst++ = c;
            }
            break;
        case PALETTE_TRANSFORM_SEPIA:
            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                c = Iwram_SignedDivide(r + ((c >> 5) & 31) + ((c >> 10) & 31), 10);
                r = c * 4 + 5;
                b = c * 3 + 5;
                /* FAKEMATCH: needed to match: this reversible XOR initialization
                 * folds late, preserving the blue-first add/copy and independent
                 * lower comparison. Copying g = b before two XOR assignments
                 * exchanges the add/copy registers (four aligned differences). */
                g = b ^ r;
                g ^= r;
                if (r < 8)
                    r = 8;
                if (b < 8) {
                    g = 8;
                    /* FAKEMATCH: needed to match: g is 8, so this pair changes no bits. Its late fold retains the separate blue lower test; an ordinary copy threads past that test and loses two instructions. */
                    b ^= g;
                    b ^= 8;
                }
                if (b < 8)
                    b = 8;
                if (r > 28)
                    r = 28;
                if (g > 28)
                    g = 28;
                if (b > 28)
                    b = 28;
                *dst++ = Palette_ChannelCurveLight[b];
                *dst++ = Palette_ChannelCurveLight[g];
                *dst++ = Palette_ChannelCurveLight[r];
            }
            break;
        case PALETTE_TRANSFORM_WARM:
            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                r -= (u32)r >> 1;
                g -= g / 3;
                r += 6;
                g += 4;
                b -= 6;
                r = Graphics_ClampRgb555Channel(r);
                g = Graphics_ClampRgb555Channel(g);
                b = Graphics_ClampRgb555Channel(b);
                dst[0] = Palette_ChannelCurveContrast[b];
                dst[1] = Palette_ChannelCurveLight[g];
                dst[2] = Palette_ChannelCurve[r];
                dst += 3;
            }
            break;
        case PALETTE_TRANSFORM_MUTED:
            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                if (r < 10)
                    r = 10;
                if (g < 16)
                    g = 16;
                if (b < 16)
                    b = 16;
                if (r > 28)
                    r = 28;
                if (g > 24)
                    g = 24;
                if (b > 26)
                    b = 26;
                g += 2;
                b += 2;
                r = Graphics_ClampRgb555Channel(r);
                g = Graphics_ClampRgb555Channel(g);
                b = Graphics_ClampRgb555Channel(b);
                *dst++ = Palette_ChannelCurve[b];
                *dst++ = Palette_ChannelCurve[g];
                *dst++ = Palette_ChannelCurve[r];
            }
            break;
        case PALETTE_TRANSFORM_PALE:
            for (i = 0; i < cnt; i++) {
                s32 mean;

                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                mean = Graphics_ClampRgb555Channel((r + g + b) / 3);
                r = (r >> 1) + mean;
                g = (g >> 1) + mean;
                b = (b >> 1) + mean;
                r = Graphics_ClampRgb555Channel(r);
                g = Graphics_ClampRgb555Channel(g);
                b = Graphics_ClampRgb555Channel(b);
                dst[0] = Palette_ChannelCurveContrast[b];
                dst[1] = Palette_ChannelCurveContrast[g];
                dst[2] = Palette_ChannelCurveContrast[r];
                dst += 3;
            }
            break;
        case PALETTE_TRANSFORM_RED:
            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                r = r + ((g >> 3) + (b >> 3));
                r = Graphics_ClampRgb555Channel(r);
                g = g - g / 3;
                b = b - b / 3;
                dst[0] = Palette_ChannelCurve[b];
                dst[1] = Palette_ChannelCurve[g];
                dst[2] = Palette_ChannelCurveLight[r];
                dst += 3;
            }
            break;
        case PALETTE_TRANSFORM_WARM2:
            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                r -= (u32)r >> 1;
                g -= g / 3;
                r += 6;
                g += 4;
                b -= 6;
                r = Graphics_ClampRgb555Channel(r);
                g = Graphics_ClampRgb555Channel(g);
                b = Graphics_ClampRgb555Channel(b);
                dst[0] = Palette_ChannelCurveContrast[b];
                dst[1] = Palette_ChannelCurveLight[g];
                dst[2] = Palette_ChannelCurve[r];
                dst += 3;
            }
            break;
        default:
            for (i = 0; i < cnt; i++) {
                c = *src++;
                dst[i * 3 + 0] = c & 0x7c00;
                dst[i * 3 + 1] = (c & 0x03e0) << 5;
                dst[i * 3 + 2] = (c & 0x001f) << 10;
            }
            break;
        }
    } else if (mode & PALETTE_TRANSFORM_TINT) {
        u32 tr;
        u32 tg;
        u32 tb;

        tr = mode & 31;
        tg = (mode >> 5) & 31;
        tb = (mode >> 10) & 31;
        for (i = 0; i < cnt; i++) {
            c = *src++;
            r = ((c << 11) & 0xf800);
            g = ((c << 7) & 0x1f000);
            b = (c & 0x7c00);
            c = Iwram_SignedDivide(r + g + b, 96);
            r = c * tr;
            g = c * tg;
            b = c * tb;
            r = Graphics_ClampRgb555Component(r);
            g = Graphics_ClampRgb555Component(g);
            b = Graphics_ClampRgb555Component(b);
            dst[0] = b;
            dst[1] = g;
            dst[2] = r;
            dst += 3;
        }
    } else if (mode & PALETTE_TRANSFORM_BLEND) {
        u32 tr;
        u32 tg;
        u32 tb;

        tr = mode & 31;
        tg = (mode >> 5) & 31;
        tb = (mode >> 10) & 31;
        for (i = 0; i < cnt; i++) {
            c = *src++;
            r = c & 31;
            c = Iwram_SignedDivide((r + ((c >> 5) & 31) + ((c >> 10) & 31)) << 4, tr + tg + tb);
            r = Iwram_MulQ16(((c * tr) >> 4) << 16, (s32)(tr << 16) >> 4);
            g = Iwram_MulQ16(((c * tg) >> 4) << 16, (s32)(tg << 16) >> 4);
            b = Iwram_MulQ16(((c * tb) >> 4) << 16, (s32)(tb << 16) >> 4);
            r = Graphics_ClampRgb555Channel((u32)r >> 16);
            g = Graphics_ClampRgb555Channel((u32)g >> 16);
            b = Graphics_ClampRgb555Channel((u32)b >> 16);
            *dst++ = Palette_ChannelCurve[b];
            *dst++ = Palette_ChannelCurve[g];
            *dst++ = Palette_ChannelCurve[r];
        }
    } else if (mode & PALETTE_TRANSFORM_SPLIT) {
        for (i = 0; i < cnt; i++) {
            c = *src++;
            dst[i * 3 + 0] = c & 0x7c00;
            dst[i * 3 + 1] = (c & 0x03e0) << 5;
            dst[i * 3 + 2] = (c & 0x001f) << 10;
        }
    } else {
        if (half == 2)
            mode += 0x600;
        Dma_Set((const void *)mode, dst, ((cnt * 6) >> 2) | DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
    }
}

void TitlePalette_InitializeBuffers(void)
{
    volatile u32 zero;
    struct TitlePaletteWork *buffer;
    s32 operation;

    buffer = Runtime_AllocateBlock(32, sizeof(*buffer));
    zero = 0;
    Dma_Set(&zero, buffer, 0x85000c01, REG_DMA3);
    Dma_Set((void *)0x05000000, buffer, 0x84000080, REG_DMA3);
    Dma_Set((void *)0x05000200, &buffer->palette[256], 0x84000080, REG_DMA3);
    Graphics_TransformPaletteBuffer(0x10000, buffer->palette, buffer->target, 0);
    operation = 3200;
    Scheduler_AddOrUpdateCallback(TitlePalette_UpdateFade, operation);
}

void Runtime_ScheduleCallbackAndReleaseBlock32A(void)
{
    Scheduler_RemoveCallback(&TitlePalette_UpdateFade);
    Runtime_ReleaseHeapBlock(0x20);
}

void Graphics_TransformLargePalette(s32 index, s32 transform)
{
    struct TitlePaletteWork *target = gPaletteBufferWork;

    if (target != NULL)
        Graphics_TransformPaletteBuffer(index, target->palette, target->target, transform);
}

void Graphics_TransformSmallPalette(s32 index, s32 transform)
{
    struct TitlePaletteWork *target = gPaletteBufferWork;

    if (target != NULL)
        Graphics_TransformPaletteBuffer(index, target, target->current, transform);
}

void Graphics_SetPaletteTransformValue(s32 value)
{
    struct TitlePaletteWork *target = gPaletteBufferWork;

    if (target != NULL)
        target->palette[0] = value;
}

void Graphics_UpdatePaletteInterpolation(s32 value)
{
    struct TitlePaletteWork *state = gPaletteBufferWork;

    if (state != NULL) {
        state->duration = value;
        state->step = 0;
        Graphics_InterpolatePaletteBuffers((s16 *)state->current, (s16 *)state->target, (s16 *)state->delta, value);
    }
}
