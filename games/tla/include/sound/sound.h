#ifndef SOUND_H
#define SOUND_H

/// @file
/// SOUND

#include "types.h"
#include "edition.h"
#include "sound_driver_types.h"

struct SoundPlayer;
struct SoundTrack;
struct SoundWork;

extern const u8 gSound_Cgb3LevelCodes[];
extern const s16 gSound_CgbFrequencySteps[];
extern const u8 gSound_CgbPitchCodes[];
extern const u16 gSound_PcmSamplesPerVBlank[];
extern const u8 gSound_NoisePitchCodes[];
extern u32 gSound_PcmFrequencySteps[];
extern u8 gSound_PcmPitchCodes[];
extern struct SoundWork gSound_Work;
extern u8 gSound_WorkBytes[];
void DummyFunc(void);
void m4aMPlayAllContinue(void);
void MPlayContinue(struct SoundPlayer *player);
void m4aMPlayAllStop(void);
u32 umul3232H32(u32 left, u32 right);
void SoundInit(struct SoundWork *work);
void m4aSoundVSyncOn(void);
void SoundMain(void);
void m4aSoundMode(u32 mode);
void SampleFreqSet(u32 mode);
void m4aSoundVSyncOff(void);
s32 MidiKeyToCgbFreq(u8 kind, u8 key, u8 fine);
void CgbSound(void);
void CgbOscOff(u8 channel);
void MPlayFadeOut(struct SoundPlayer *player, u16 speed);
void MPlayOpen(struct SoundPlayer *player, struct SoundTrack *tracks, u8 count);
void FadeOutBody(struct SoundPlayer *player);
void m4aMPlayStop(struct SoundPlayer *player);
void MPlayMain(struct SoundPlayer *player);
void TrkVolPitSet(struct SoundPlayer *player, struct SoundTrack *track);
void ply_note(u32 command, struct SoundPlayer *player,
              struct SoundTrack *track);
void ply_memacc(struct SoundPlayer *player, struct SoundTrack *track);
void ply_endtie(struct SoundPlayer *player, struct SoundTrack *track);
void ply_lfos(struct SoundPlayer *player, struct SoundTrack *track);
void ply_mod(struct SoundPlayer *player, struct SoundTrack *track);
void TrackStop(struct SoundPlayer *player, struct SoundTrack *track);
void Sound_Mixer(void);

extern u16 gData_02005830;
extern u8 gSoundFlags[];
s32 Sound_GetBgmRestoreDelayFar(void);
void Sound_WaitForJingle(void);

#endif
