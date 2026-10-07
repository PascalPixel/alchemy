/* ported from games/THE BROKEN SEAL/SRC/SYSTEM/IO.C */
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
#include "SYSTEM.H"
#include "FRAME.H"
#include "DMA.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"


void Input_HandleKeyInterrupt(void)
{
    if (gSleepActive == 0) {
        REG_KEYCNT = 0xc3ff;
        gResetRequested = 1;
    }
}
