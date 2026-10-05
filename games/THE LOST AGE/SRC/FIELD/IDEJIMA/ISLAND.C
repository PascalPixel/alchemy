#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"


void ScanlineWave_Reset(void)
{
    gScanlineWavePage = 0;
    gScanlineWaveCount = 0;
}
