/* 08090a5c: differ 180 of 812, draft 812; the native and draft use the same 40-byte frame. Remaining: red and green registers rotate through the channel transforms. Rebuilt from the current verified Graphics_TransformPaletteBuffer body, using the battle native 448-color count, half spans, palette-memory read and warm transform. Natural twin form differed 291; nonvolatile palette memory read 280; three localized clamp constraints 180 (without the first or second: 182, without the third: 284; third input-only remains 180). */
/* Trials: legacy draft 1052; current IWRAM and division vocabulary 1049; Dma_Set 1062; shared palette source then resolves the global frame/register differences above. Ordinary color declaration orders stay 280 and register hints stay 180. Scoped warm/sepia/tint/blend locals give 186/658/180/252. A fixed sepia-red bridge adds an instruction (209); fixed-color variants 315-873 and extra tie operands do not help, so none remain. */

#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "IO_REG.H"
#include "DMA.H"
#include "PALBUF.H"

s32 BattleFx_ClampRgb555Channel(s32 value);
s32 BattleFx_ClampRgb555Component(s32 value);

extern u16 Data_0809e92e[];
extern u16 Data_0809e96e[];
extern u16 Data_0809e9ae[];

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
                *dst++ = Data_0809e96e[b];
                *dst++ = Data_0809e96e[g];
                *dst++ = Data_0809e96e[r];
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
                *dst++ = Data_0809e9ae[b];
                *dst++ = Data_0809e96e[g];
                *dst++ = Data_0809e92e[r];
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
                *dst++ = Data_0809e92e[b];
                *dst++ = Data_0809e92e[g];
                *dst++ = Data_0809e92e[r];
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
                dst[0] = Data_0809e9ae[b];
                dst[1] = Data_0809e9ae[g];
                dst[2] = Data_0809e9ae[r];
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
                r = BattleFx_ClampRgb555Channel(r);
                g = g - g / 3;
                b = b - b / 3;
                dst[0] = Data_0809e92e[b];
                dst[1] = Data_0809e92e[g];
                dst[2] = Data_0809e96e[r];
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
                dst[0] = Data_0809e9ae[b];
                dst[1] = Data_0809e96e[g];
                dst[2] = Data_0809e92e[r];
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
            r = BattleFx_ClampRgb555Channel((u32)r >> 16);
            g = BattleFx_ClampRgb555Channel((u32)g >> 16);
            b = BattleFx_ClampRgb555Channel((u32)b >> 16);
            *dst++ = Data_0809e92e[b];
            *dst++ = Data_0809e92e[g];
            *dst++ = Data_0809e92e[r];
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
