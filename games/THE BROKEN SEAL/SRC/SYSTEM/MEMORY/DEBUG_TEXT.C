#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "IO_REG.H"


void Runtime_WriteDebugTextTiles(const u8 *src)
{
    if (gDebugTextEnabled != 0) {
        u32 addr = ((u32)&gDebugTextCursor);
        u32 c = *src;
        u16 *dst = *(u16 **)addr;
        u32 cnt = 0;
        src++;

        if (c != 0) {
            u32 mask = 0xf000;
            addr = (u32)(BG_SCREEN_BLOCK(4) + 0x500);
            do {
                *dst++ = c | mask;
                if (dst == (u16 *)addr)
                    dst = (u16 *)BG_SCREEN_BLOCK(4);
                cnt++;
                if (cnt > 31)
                    break;
                c = *src++;
            } while (c != 0);
            addr = ((u32)&gDebugTextCursor);
        }
        *(u16 **)addr = dst;
    }
}
