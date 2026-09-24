#include "TYPES.H"

s32 Engine_RandomNext();

/* Copy the background scroll offsets for this frame, occasionally from the
 * shaken set while the display is outside the visible lines. */
void TakaraHashira_JitterBackgroundScroll(void)
{
    u32 line = *(volatile u16 *)0x04000006;
    u32 *src = (u32 *)0x03001ad4;
    volatile u32 *reg = (volatile u32 *)0x04000014;

    if (line == 227 || line <= 46) {
        if ((u32)(Engine_RandomNext() * 100) >> 16 < *(u32 *)0x0200b738) {
            src = (u32 *)0x0200b72c;
        }
    }
    *reg = *src++;
    reg = (volatile u32 *)0x04000018;
    *reg++ = *src++;
    *reg = *src;
}
