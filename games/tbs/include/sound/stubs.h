#ifndef ALCHEMY_SOUND_STUBS_H
#define ALCHEMY_SOUND_STUBS_H

/* @file */
/* The sound module's stub calls: playing sounds and music, and the driver's */
/* update each frame. */

void m4aSoundMainStub(void);
void sySndInitStub(void);
void sySndPlayStub(s32 id);
void sySndUpdateBgmRampsStub(void);
void sySndSetBgmTempoStub(u16 tempoScale);
s32 sySndGetBgmRestoreDelayStub(void);
s32 sySndGetCurrentBgmStub(void);
void sySndWaitForBgmRestoreStub(void);

#endif
