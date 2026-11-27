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
/* An EWRAM byte sySndInit clears and nothing else names. */
extern Uint8 SoundUnreadByte;

/* The sound module's functions in the order of its stub table
   (src/sound/stubs.s). */
void   m4aSoundMain(void);
void   sySndInit(void);
void   sySndPlay(Sint32 id);
void   sySndUpdateBGMRamps(void);
void   sySndSetBGMTempo(Uint16 tempoScale);
void   sySndSetBGMPitch(Uint16 pitch);
void   sySndSetBGMTempoPitchTarget(Uint16 tgtScale, Uint16 step);
void   sySndSetBGMVolume(Sint16 volume);
void   sySndSetBGMVolumeTarget(Uint16 tgtVolume, Uint16 step);
Sint32 sySndGetBGMRestoreDelay(void);
void   sySndTestRun(void);
void   sySndStopAllMusicPlayers(void);
void   sySndResumeAllMusicPlayers(void);
void   sySndUpdateCommandMask(Uint32 command);
Sint32 sySndGetCurrentBGM(void);
void   sySndWaitForBGMRestore(void);

Sint32 sySndGetReverbCode(Sint32 command);

#endif
