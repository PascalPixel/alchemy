#include "PROBE.H"

/* Fade palette entries 97 to 103 to black one step every five frames. */
void MakyuriHeya_FadePaletteToBlack(void)
{
    volatile u16 *pal;
    u32 i;
    s32 done;
    s32 r;
    s32 g;
    s32 b;

    do {
        pal = (volatile u16 *)0x050000c2;
        done = 0;
        for (i = 0; i <= 6; i++) {
            r = *pal & 31;
            g = (u16)(*pal >> 5) & 31;
            b = (u16)(*pal >> 10) & 31;
            if (r > 0)
                r--;
            if (g > 0)
                g--;
            if (b > 0)
                b--;
            *pal = (b << 10) | (g << 5) | r;
            if (*pal == 0)
                done++;
            pal++;
        }
        Task_Wait(5);
    } while (done != 7);
}
