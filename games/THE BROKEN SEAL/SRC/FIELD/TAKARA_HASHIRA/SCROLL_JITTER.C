#include "TYPES.H"

s32 Engine_RandomNext();
extern u32 gBgScroll[];
extern u32 TakaraHashira_ShakeChance;
extern u32 TakaraHashira_ShakenScroll[];

/* Copy the background scroll offsets for this frame, occasionally from the
 * shaken set while the display is outside the visible lines. */
void TakaraHashira_JitterBackgroundScroll(void)
{
    u32 line = *(volatile u16 *)0x04000006;
    u32 *src = &gBgScroll[1];
    volatile u32 *reg = (volatile u32 *)0x04000014;

    if (line == 227 || line <= 46) {
        if ((u32)(Engine_RandomNext() * 100) >> 16 < TakaraHashira_ShakeChance) {
            src = TakaraHashira_ShakenScroll;
        }
    }
    *reg = *src++;
    reg = (volatile u32 *)0x04000018;
    *reg++ = *src++;
    *reg = *src;
}
