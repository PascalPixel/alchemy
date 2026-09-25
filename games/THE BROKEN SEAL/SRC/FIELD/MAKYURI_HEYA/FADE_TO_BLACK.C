#include "TYPES.H"

s32 Engine_TaskWait();


extern u8 Data_0000001f[];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* Fade palette entries 97 to 103 to black one step every five frames. */
void MakyuriHeya_FadePaletteToBlack(void)
{
    volatile u16 *pal;
    u32 i;
    s32 done;
    s32 mask;
    s32 r;
    s32 g;
    s32 b;

    /* FAKEMATCH: the 31 mask is a HImode pool constant through the linker's
     * address-31 symbol, which places the literal pool mid-loop. */
    mask = (u16)(u32)Data_0000001f;
    do {
        done = 0;
        pal = (volatile u16 *)0x050000c2;
        for (i = 0; i <= 6; i++) {
            r = *pal & 31;
            g = (*pal >> 5) & mask;
            b = (*pal >> 10) & mask;
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
        Engine_TaskWait(5);
    } while (done != 7);
}
