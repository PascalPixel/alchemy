#ifndef ALCHEMY_SOUND_H
#define ALCHEMY_SOUND_H

/* The game's sound over the development kit's sound driver
   (sdk/sound_driver.h): its state and the sound module's functions. */

#include "types.h"
#include "edition.h"
#include "sdk/sound_driver.h"

/* Nonzero once SOUND_FADE_OUT_BGM has started the music's fade; further fade
   requests are ignored until the next music clears it. */
extern Uint8 BgmFadeRequested;
/* Set by the resident mixer whenever it clips a sample: when a sum overflows,
   IwramSoundNormalizeChannels saturates the samples and stores the clip mask
   (0x80008000) here. Only the unused sound test reads it. */
extern Sint32 SoundMixerClipped;
/* An EWRAM byte sySoundInit clears and nothing else names. */
extern Uint8 SoundUnreadByte;

/* The sound module's functions in the order of its stub table
   (src/sound/stubs.s). */
void   m4aSoundMain(void);
void   sySoundInit(void);
void   sySoundPlay(Sint32 id);
void   sySoundUpdateBGMRamps(void);
void   sySoundSetBGMTempo(Uint16 tempoScale);
void   sySoundSetBGMPitch(Uint16 pitch);
void   sySoundSetBGMTempoPitchTarget(Uint16 targetScale, Uint16 step);
void   sySoundSetBGMVolume(Sint16 volume);
void   sySoundSetBGMVolumeTarget(Uint16 targetVolume, Uint16 step);
Sint32 sySoundGetBGMRestoreDelay(void);
void   sySoundTestRun(void);
void   sySoundStopAllMusicPlayers(void);
void   sySoundResumeAllMusicPlayers(void);
void   sySoundUpdateCommandMask(Uint32 command);
Sint32 sySoundGetCurrentBGM(void);
void   sySoundWaitForBGMRestore(void);

Sint32 sySoundGetReverbCode(Sint32 command);

#endif
