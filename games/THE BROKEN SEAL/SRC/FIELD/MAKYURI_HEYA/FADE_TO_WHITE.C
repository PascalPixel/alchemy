#include "PROBE.H"

/* Fade palette entries 40 to 47 to white one step every two frames. */
void MakyuriHeya_FadePaletteToWhite(void)
{
    volatile u16 *pal;
    u32 i;
    u32 done;
    s32 r;
    s32 g;
    s32 b;

    do {
        pal = (volatile u16 *)0x05000050;
        done = 0;
        for (i = 0; i <= 7; i++) {
            r = *pal & 31;
            g = (u16)(*pal >> 5) & 31;
            b = (u16)(*pal >> 10) & 31;
            if (r == 31 && g == 31 && b == 31) {
                done++;
            } else {
                if (r <= 30)
                    r++;
                if (g <= 30)
                    g++;
                if (b <= 30)
                    b++;
                *pal = (b << 10) | (g << 5) | r;
            }
            pal++;
        }
        Task_Wait(2);
    } while (done <= 7);
}
