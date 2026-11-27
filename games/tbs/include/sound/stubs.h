#ifndef ALCHEMY_SOUND_STUBS_H
#define ALCHEMY_SOUND_STUBS_H

/* The sound module's stub calls: playing sounds and music, and the driver's
   update each frame. */

void   m4aSoundMainStub(void);
void   sySndInitStub(void);
void   sySndPlayStub(Sint32 id);
void   sySndUpdateBGMRampsStub(void);
void   sySndSetBGMTempoStub(Uint16 tempoScale);
Sint32 sySndGetBGMRestoreDelayStub(void);
Sint32 sySndGetCurrentBGMStub(void);
void   sySndWaitForBGMRestoreStub(void);

#endif
