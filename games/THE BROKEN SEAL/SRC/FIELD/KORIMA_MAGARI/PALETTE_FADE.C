#include "TYPES.H"
#include "FIELD_EVENT.H"

void KorimaPalette_SaveFirst(void);
void KorimaPalette_SaveSecond(void);
void KorimaPalette_Capture(void);
u16 Effect_AdjustColorChannels(u16 color, s32 adj);

/* Adjust every palette colour but for colours 17 to 23 and 193 to 200 by
   the amount given, keeping the palette before and after, then blend
   towards it. */
void Effect_AdjustPaletteColors(s32 amount)
{
    u32 x;

    KorimaPalette_SaveFirst();
    x = 0;
    do {
        u32 idx = x >> 16;
        if (x + 0xffef0000 > 0x60000 && (idx + 0xff3f) << 16 > 0x70000) {
            u16 *pal = (u16 *)(0x5000000 + idx * 2);
            *pal = Effect_AdjustColorChannels(*pal, amount);
        }
        /* FAKEMATCH: forced temporary; the step is kept in its own
           register and tested before it replaces x, which the plain
           loop condition does not do. */
        {
            u32 nx = x + 0x10000;
            x = nx;
            if (nx > 0xdf0000) {
                break;
            }
        }
    } while (1);
    KorimaPalette_Capture();
    KorimaPalette_SaveSecond();
    Engine_ColorBufferApplyTarget(0x10000, 0);
}
