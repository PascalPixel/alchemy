#ifndef ALCHEMY_SOUND_STUBS_H
#define ALCHEMY_SOUND_STUBS_H

/* The sound module's stub calls: playing sounds and music, and the driver's
   update each frame. */

void   m4aSoundMainStub(void);
void   sySoundInitStub(void);
void   sySoundPlayStub(Sint32 id);
void   sySoundUpdateBGMRampsStub(void);
void   sySoundSetBGMTempoStub(Uint16 tempoScale);
Sint32 sySoundGetBGMRestoreDelayStub(void);
Sint32 sySoundGetCurrentBGMStub(void);
void   sySoundWaitForBGMRestoreStub(void);

#endif
