#include "TYPES.H"

s32 Engine_TaskWait();


extern u8 Data_0000001f[];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* Fade palette entries 40 to 47 to white one step every two frames. */
void MakyuriHeya_FadePaletteToWhite(void)
{
    volatile u16 *pal;
    u32 i;
    u32 done;
    s32 mask;
    s32 r;
    s32 g;
    s32 b;

    do {
        /* FAKEMATCH: the 31 mask is a HImode pool constant through the linker's
         * address-31 symbol, which places the literal pool at the top. */
        mask = (u16)(u32)Data_0000001f;
        pal = (volatile u16 *)0x05000050;
        done = 0;
        for (i = 0; i <= 7; i++) {
            r = *pal & 31;
            g = (*pal >> 5) & mask;
            b = (*pal >> 10) & mask;
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
        Engine_TaskWait(2);
    } while (done <= 7);
}
