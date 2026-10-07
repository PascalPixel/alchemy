#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RUNTIME_MEM.H"
#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_EFFECT_RUNTIME.H"
#include "IO_REG.H"
#include "IWRAM_CALL.H"
#include "PALBUF.H"

s32 BattleFx_ClampRgb555Channel(s32 value);
s32 BattleFx_ClampRgb555Component(s32 value);

extern u16 BattlePalette_ChannelCurve[];
extern u16 BattlePalette_ChannelCurveLight[];
extern u16 BattlePalette_ChannelCurveContrast[];

void BattleFx_BuildBuffer(u32 mode, u16 *src, u16 *dst, s32 half)
{
    u32 i;
    u32 cnt = 448;
    s32 r;
    s32 b;
    u32 c;
    s32 g;

    if (mode == PALETTE_TRANSFORM_FROM_SOURCE)
        mode = *(u16 *)BG_PLTT;
    if (half == 1) {
        cnt = 224;
    } else if (half == 2) {
        dst += 672;
        src += 224;
        cnt = 224;
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
                /* FAKEMATCH: keep the native channel copy order after the ordinary shared-color form rotates r5-r7. */
                __asm__("" : "+r" (r));
                g = b;
                /* FAKEMATCH: preserve separate native channel lower-clamp lifetimes; ordinary assignments merge their tests. */
                __asm__("" : "+r" (g));
                if (r < 8)
                    r = 8;
                if (b < 8) {
                    g = 8;
                    /* FAKEMATCH: retain the second native blue test on the clamped path after ordinary tests merge. */
                    __asm__("" : : "r" (b));
                }
                if (b < 8)
                    b = 8;
                if (r > 28)
                    r = 28;
                if (g > 28)
                    g = 28;
                if (b > 28)
                    b = 28;
                *dst++ = BattlePalette_ChannelCurveLight[b];
                *dst++ = BattlePalette_ChannelCurveLight[g];
                *dst++ = BattlePalette_ChannelCurveLight[r];
            }
            break;
        case PALETTE_TRANSFORM_WARM:
            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                if ((r | g | b) != 0) {
                    r -= (u32)r >> 1;
                    g -= g / 3;
                    r += 10;
                    g += 8;
                    b -= 7;
                    r = BattleFx_ClampRgb555Channel(r);
                    g = BattleFx_ClampRgb555Channel(g);
                    b = BattleFx_ClampRgb555Channel(b);
                }
                *dst++ = BattlePalette_ChannelCurveContrast[b];
                *dst++ = BattlePalette_ChannelCurveLight[g];
                *dst++ = BattlePalette_ChannelCurve[r];
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
                r = BattleFx_ClampRgb555Channel(r);
                g = BattleFx_ClampRgb555Channel(g);
                b = BattleFx_ClampRgb555Channel(b);
                *dst++ = BattlePalette_ChannelCurve[b];
                *dst++ = BattlePalette_ChannelCurve[g];
                *dst++ = BattlePalette_ChannelCurve[r];
            }
            break;
        case PALETTE_TRANSFORM_PALE:
            for (i = 0; i < cnt; i++) {
                s32 mean;

                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                mean = BattleFx_ClampRgb555Channel((r + g + b) / 3);
                r = (r >> 1) + mean;
                g = (g >> 1) + mean;
                b = (b >> 1) + mean;
                r = BattleFx_ClampRgb555Channel(r);
                g = BattleFx_ClampRgb555Channel(g);
                b = BattleFx_ClampRgb555Channel(b);
                dst[0] = BattlePalette_ChannelCurveContrast[b];
                dst[1] = BattlePalette_ChannelCurveContrast[g];
                dst[2] = BattlePalette_ChannelCurveContrast[r];
                dst += 3;
            }
            break;
        case PALETTE_TRANSFORM_RED:
            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                r += ((g >> 3) + (b >> 3));
                r = BattleFx_ClampRgb555Channel(r);
                g -= g / 3;
                b -= b / 3;
                dst[0] = BattlePalette_ChannelCurve[b];
                dst[1] = BattlePalette_ChannelCurve[g];
                dst[2] = BattlePalette_ChannelCurveLight[r];
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
                r = BattleFx_ClampRgb555Channel(r);
                g = BattleFx_ClampRgb555Channel(g);
                b = BattleFx_ClampRgb555Channel(b);
                dst[0] = BattlePalette_ChannelCurveContrast[b];
                dst[1] = BattlePalette_ChannelCurveLight[g];
                dst[2] = BattlePalette_ChannelCurve[r];
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
        s32 r;
        s32 g;
        s32 b;

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
            r = BattleFx_ClampRgb555Component(r);
            g = BattleFx_ClampRgb555Component(g);
            b = BattleFx_ClampRgb555Component(b);
            dst[0] = b;
            dst[1] = g;
            dst[2] = r;
            dst += 3;
        }
    } else if (mode & PALETTE_TRANSFORM_BLEND) {
        u32 tr;
        u32 tg;
        u32 tb;
        s32 nr;
        s32 ng;
        s32 nb;

        tr = mode & 31;
        tg = (mode >> 5) & 31;
        tb = (mode >> 10) & 31;
        for (i = 0; i < cnt; i++) {
            c = *src++;
            r = c & 31;
            c = Iwram_SignedDivide((r + ((c >> 5) & 31) + ((c >> 10) & 31)) << 4, tr + tg + tb);
            nr = Iwram_MulQ16(((c * tr) >> 4) << 16, (s32)(tr << 16) >> 4);
            ng = Iwram_MulQ16(((c * tg) >> 4) << 16, (s32)(tg << 16) >> 4);
            nb = Iwram_MulQ16(((c * tb) >> 4) << 16, (s32)(tb << 16) >> 4);
            nr = BattleFx_ClampRgb555Channel((u32)nr >> 16);
            ng = BattleFx_ClampRgb555Channel((u32)ng >> 16);
            nb = BattleFx_ClampRgb555Channel((u32)nb >> 16);
            *dst++ = BattlePalette_ChannelCurve[nb];
            *dst++ = BattlePalette_ChannelCurve[ng];
            *dst++ = BattlePalette_ChannelCurve[nr];
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
            mode += 1344;
        Dma_Set((const void *)mode, dst, ((cnt * 6) >> 2) | DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
    }
}

void BattleEffect_InitializeBuffers(void)
{
    volatile s32 zero;
    struct BattleEffectBuffers *buffer;

    buffer = Runtime_AllocateBlock(0x20, sizeof(*buffer));
    zero = 0;
    Dma_Set(&zero, buffer, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | (sizeof(*buffer) >> 2), REG_DMA3);
    Dma_Set((void *)BG_PLTT, &buffer->palette[0], DMA_ENABLE32 | DMA_32BIT | 0x70, REG_DMA3);
    Dma_Set((void *)OBJ_PLTT, &buffer->palette[0xe0], DMA_ENABLE32 | DMA_32BIT | 0x70, REG_DMA3);
    BattleFx_BuildBuffer(PALETTE_TRANSFORM_PLAIN, buffer->palette, buffer->target, 0);
    Scheduler_AddOrUpdateCallback((void (*)(void))BattlePalette_UpdateBlend, 0xc8f);
}

void Runtime_ScheduleCallbackAndReleaseBlock32B(void)
{
    Scheduler_RemoveCallback(&BattlePalette_UpdateBlend);
    Runtime_ReleaseHeapBlock(0x20);
}

void BattleFx_ApplyColorToTargetBuffer(s32 value, s32 mode)
{
    struct BattleEffectBuffers *buffers = Data_03001ed0;

    if (buffers != NULL) {
        BattleFx_BuildBuffer(value, buffers->palette, buffers->target, mode);
    }
}

void BattleFx_ApplyColorToSourceBuffer(s32 value, s32 mode)
{
    struct BattleEffectBuffers *buffers = Data_03001ed0;

    if (buffers != NULL) {
        BattleFx_BuildBuffer(value, buffers->palette, buffers->current, mode);
    }
}

void BattleFx_SetPrimaryBufferValue(u32 value)
{
    struct BattleEffectBuffers *buffers = Data_03001ed0;

    if (buffers != NULL) {
        buffers->palette[0] = value;
    }
}

void BattleFx_StartBufferInterpolation(s32 mode)
{
    struct BattleEffectBuffers *buffers = Data_03001ed0;

    if (buffers != NULL) {
        buffers->duration = mode;
        buffers->step = 0;
        BattleFx_InterpolateBuffers((s16 *)buffers->current,
                                        (s16 *)buffers->target,
                                        (s16 *)buffers->delta,
                                        mode);
    }
}

s32 BattleFx_ClampRgb555Channel(s32 value)
{
    if (value > 31)
        return 31;
    if (value < 0)
        value = 0;
    return value;
}

s32 BattleFx_ClampRgb555Component(s32 value)
{
    if (value > 31744)
        value = 31744;
    return value;
}
