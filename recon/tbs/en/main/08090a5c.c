#include "TYPES.H"
#include "IWRAM_CALL.H"

typedef s32 (*SignedDivide)(s32 numerator, s32 denominator);

s32 __divsi3(s32 numerator, s32 denominator);
s32 BattleFx_ClampRgb555Channel(s32 value);
s32 BattleFx_ClampRgb555Component(s32 value);

extern u16 Data_0809e92e[];
extern u16 Data_0809e96e[];
extern u16 Data_0809e9ae[];

void BattleFx_BuildBuffer(u32 mode, u16 *src, u16 *dst, s32 part)
{
    u32 *reg;
    u32 i;
    u32 cnt;
    u32 c;

    cnt = 448;
    if (mode == 0x8000)
        mode = *(u16 *)0x05000000;

    if (part == 1) {
        cnt = 224;
    } else if (part == 2) {
        dst += 672;
        cnt = 224;
        src += 224;
    }

    if (mode < 0x8000) {
        *dst++ = mode & 0x7c00;
        *dst++ = (mode & 0x3e0) << 5;
        *dst++ = (mode & 0x1f) << 10;

        reg = (u32 *)0x040000d4;
        reg[0] = (u32)(dst - 3);
        reg[1] = (u32)dst;
        reg[2] = (((cnt - 1) * 3 * 2) >> 1) | 0x80000000;
        return;
    }

    if (mode < 0x100000) {
        switch (mode) {
        case 0x10001: {
            SignedDivide divide;
            u16 *out;
            s32 v;

            divide = (SignedDivide)0x03000380;
            out = dst;
            for (i = 0; i < cnt; i++) {
                c = *src++;
                v = divide(((c << 11) & 0xf800) + ((c << 7) & 0x1f000) +
                               (c & 0x7c00),
                           7);
                out[0] = (u16)v;
                out[1] = (u16)v;
                out[2] = (u16)v;
                out += 3;
            }
            return;
        }

        case 0x10002: {
            SignedDivide divide;
            s32 v;
            s32 r;
            s32 g;
            s32 b;

            for (i = 0; i < cnt; i++) {
                c = *src++;
                divide = (SignedDivide)0x03000380;
                v = divide((s32)((c & 31) + ((c >> 5) & 31) + ((c >> 10) & 31)),
                           10);
                r = v * 4 + 5;
                g = v * 3 + 5;
                b = v * 3 + 5;
                if (r < 8)
                    r = 8;
                if (g < 8)
                    g = 8;
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
            return;
        }

        case 0x10003: {
            u32 r;
            u32 g;
            u32 b;

            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                if ((r | g | b) != 0) {
                    r = r - (r >> 1) + 10;
                    g = (u32)((s32)g - __divsi3((s32)g, 3) + 8);
                    b = b - 7;
                    r = (u32)BattleFx_ClampRgb555Channel((s32)r);
                    g = (u32)BattleFx_ClampRgb555Channel((s32)g);
                    b = (u32)BattleFx_ClampRgb555Channel((s32)b);
                }
                *dst++ = Data_0809e9ae[b];
                *dst++ = Data_0809e96e[g];
                *dst++ = Data_0809e92e[r];
            }
            return;
        }

        case 0x10004: {
            s32 r;
            s32 g;
            s32 b;

            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = (s32)(c & 31);
                g = (s32)((c >> 5) & 31);
                b = (s32)((c >> 10) & 31);
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
                g = g + 2;
                b = b + 2;
                r = BattleFx_ClampRgb555Channel(r);
                g = BattleFx_ClampRgb555Channel(g);
                b = BattleFx_ClampRgb555Channel(b);
                *dst++ = Data_0809e92e[b];
                *dst++ = Data_0809e92e[g];
                *dst++ = Data_0809e92e[r];
            }
            return;
        }

        case 0x10005: {
            s32 r;
            s32 g;
            s32 b;
            s32 v;

            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = (s32)(c & 31);
                g = (s32)((c >> 5) & 31);
                b = (s32)((c >> 10) & 31);
                v = BattleFx_ClampRgb555Channel(
                    __divsi3(r + g + b, 3));
                r = (r >> 1) + v;
                g = (g >> 1) + v;
                b = (b >> 1) + v;
                r = BattleFx_ClampRgb555Channel(r);
                g = BattleFx_ClampRgb555Channel(g);
                b = BattleFx_ClampRgb555Channel(b);
                dst[0] = Data_0809e9ae[b];
                dst[1] = Data_0809e9ae[g];
                dst[2] = Data_0809e9ae[r];
                dst += 3;
            }
            return;
        }

        case 0x10006: {
            s32 r;
            s32 g;
            s32 b;

            for (i = 0; i < cnt; i++) {
                c = *src++;
                g = (s32)((c >> 5) & 31);
                b = (s32)((c >> 10) & 31);
                r = (s32)(c & 31);
                r = BattleFx_ClampRgb555Channel(r + ((g >> 3) + (b >> 3)));
                g = g - __divsi3(g, 3);
                b = b - __divsi3(b, 3);
                dst[0] = Data_0809e92e[b];
                dst[1] = Data_0809e92e[g];
                dst[2] = Data_0809e96e[r];
                dst += 3;
            }
            return;
        }

        case 0x10007: {
            u32 r;
            u32 g;
            u32 b;

            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                r = r - (r >> 1) + 6;
                g = (u32)((s32)g - __divsi3((s32)g, 3) + 4);
                b = b - 6;
                r = (u32)BattleFx_ClampRgb555Channel((s32)r);
                g = (u32)BattleFx_ClampRgb555Channel((s32)g);
                b = (u32)BattleFx_ClampRgb555Channel((s32)b);
                dst[0] = Data_0809e9ae[b];
                dst[1] = Data_0809e96e[g];
                dst[2] = Data_0809e92e[r];
                dst += 3;
            }
            return;
        }

        default: {
            u16 *out;

            out = dst;
            for (i = 0; i < cnt; i++) {
                c = *src++;
                out[0] = c & 0x7c00;
                out[1] = (c & 0x3e0) << 5;
                out[2] = (c & 0x1f) << 10;
                out += 3;
            }
            return;
        }
        }
    }

    if ((mode & 0x200000) != 0) {
        SignedDivide divide;
        s32 red;
        s32 green;
        s32 blue;
        s32 k;
        s32 rr;
        s32 gg;
        s32 bb;

        red = (s32)(mode & 31);
        green = (s32)((mode >> 5) & 31);
        blue = (s32)((mode >> 10) & 31);
        for (i = 0; i < cnt; i++) {
            c = *src++;
            divide = (SignedDivide)0x03000380;
            k = divide(((c << 11) & 0xf800) + ((c << 7) & 0x1f000) +
                           (c & 0x7c00),
                       96);
            rr = red * k;
            gg = green * k;
            bb = blue * k;
            rr = BattleFx_ClampRgb555Component(rr);
            gg = BattleFx_ClampRgb555Component(gg);
            bb = BattleFx_ClampRgb555Component(bb);
            dst[0] = (u16)bb;
            dst[1] = (u16)gg;
            dst[2] = (u16)rr;
            dst += 3;
        }
        return;
    }

    if ((mode & 0x400000) != 0) {
        SignedDivide divide;
        u32 red;
        u32 green;
        u32 blue;
        s32 k;
        u32 rr;
        u32 gg;
        u32 bb;

        red = mode & 31;
        green = (mode >> 5) & 31;
        blue = (mode >> 10) & 31;
        for (i = 0; i < cnt; i++) {
            c = *src++;
            divide = (SignedDivide)0x03000380;
            k = divide((s32)(((c & 31) + ((c >> 5) & 31) + ((c >> 10) & 31))
                             << 4),
                       (s32)(red + green + blue));
            rr = (u32)Iwram_MulQ16((s32)(((red * (u32)k) >> 4) << 16),
                            (s32)(red << 16) >> 4);
            gg = (u32)Iwram_MulQ16((s32)(((green * (u32)k) >> 4) << 16),
                            (s32)(green << 16) >> 4);
            bb = (u32)Iwram_MulQ16((s32)(((blue * (u32)k) >> 4) << 16),
                            (s32)(blue << 16) >> 4);
            rr = (u32)BattleFx_ClampRgb555Channel((s32)(rr >> 16));
            gg = (u32)BattleFx_ClampRgb555Channel((s32)(gg >> 16));
            bb = (u32)BattleFx_ClampRgb555Channel((s32)(bb >> 16));
            *dst++ = Data_0809e92e[bb];
            *dst++ = Data_0809e92e[gg];
            *dst++ = Data_0809e92e[rr];
        }
        return;
    }

    if ((mode & 0x800000) != 0) {
        u16 *out;

        out = dst;
        for (i = 0; i < cnt; i++) {
            c = *src++;
            out[0] = c & 0x7c00;
            out[1] = (c & 0x3e0) << 5;
            out[2] = (c & 0x1f) << 10;
            out += 3;
        }
        return;
    }

    if (part == 2)
        mode += 1344;

    reg = (u32 *)0x040000d4;
    reg[0] = mode;
    reg[1] = (u32)dst;
    reg[2] = ((cnt * 3) >> 1) | 0x84000000;
}
