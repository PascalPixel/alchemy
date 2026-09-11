#include "types.h"
#include "scene.h"

/*
 * Blend control setup.  The eight-byte owner at 0x080c0ea8 includes its two
 * trailing pool words, 0x000000bf and 0x04000050.
 */

#define REG_BLDCNT (*(volatile u16 *)0x04000050)

/* Set the blend control bits to 0xbf.  No arguments, no result. */
void Graphics_SetBlendControl(void)
{
    REG_BLDCNT = 0xbf;
}
