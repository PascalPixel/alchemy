#include "TYPES.H"

extern s16 gScanlineWavePage;
extern s32 gScanlineWaveCount;

void ScanlineWave_Reset(void)
{
    gScanlineWavePage = 0;
    gScanlineWaveCount = 0;
}
