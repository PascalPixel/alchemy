#ifndef SOUND_H
#define SOUND_H

/// @file
/// The sound tables (CGB codes, frequency steps, samples a VBlank) and the
/// sound routines.

#include "types.h"
#include "edition.h"
#include "sound_driver_types.h"

struct MusicPlayerInfo;
struct MusicPlayerTrack;
struct SoundInfo;

extern const u8 gCgb3Vol[];
extern const s16 gCgbFreqTable[];
extern const u8 gCgbScaleTable[];
extern const u16 gPcmSamplesPerVBlankTable[];
extern const u8 gNoiseTable[];
extern u32 gFreqTable[];
extern u8 gScaleTable[];
/// The sound driver's work, which m4aSoundInit sets up through
/// SoundInit.
extern struct SoundInfo gSoundInfo;
/// The 16 work bytes every music player shares (MusicPlayerInfo.workBytes), so
/// one sound can test what another has set (ply_memacc).
extern u8 gMPlayMemAccArea[];
void m4aMPlayAllContinue(void);
void MPlayContinue(struct MusicPlayerInfo *player);
void m4aMPlayAllStop(void);
u32 umul3232H32(u32 left, u32 right);
void SoundInit(struct SoundInfo *work);
void m4aSoundVSyncOn(void);
void SoundMain(void);
void m4aSoundMode(u32 mode);
void SampleFreqSet(u32 mode);
void m4aSoundVSyncOff(void);
s32 MidiKeyToCgbFreq(u8 kind, u8 key, u8 fine);
void CgbSound(void);
void CgbOscOff(u8 channel);
void MPlayFadeOut(struct MusicPlayerInfo *player, u16 speed);
void MPlayOpen(struct MusicPlayerInfo *player, struct MusicPlayerTrack *tracks,
               u8 count);
void FadeOutBody(struct MusicPlayerInfo *player);
void m4aMPlayStop(struct MusicPlayerInfo *player);
void MPlayMain(struct MusicPlayerInfo *player);
void TrkVolPitSet(struct MusicPlayerInfo *player,
                  struct MusicPlayerTrack *track);
void ply_note(u32 command, struct MusicPlayerInfo *player,
              struct MusicPlayerTrack *track);
void ply_memacc(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_endtie(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_lfos(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_mod(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void TrackStop(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void SoundMainRAM_Rom(void);

/// The build stamp System_CopyBuildStamp copies to IWRAM at power-on: twelve
/// digits and a zero word, at the bottom of the frame-wait stack.
extern s32 gBuildStamp[];
/// Nonzero once SOUND_FADE_OUT_BGM has started the music's fade; further fade
/// requests are ignored until the next music clears it.
extern u8 gBgmFadeRequested;
/// Set by the resident mixer whenever it clips a sample: when a sum overflows,
/// IwramSoundNormalizeChannels saturates the samples and stores the clip mask
/// (0x80008000) here, through pc. Cleared by the sound test, SoundTest_Run,
/// which restarts a 20-frame countdown when it finds it set.
extern s32 gSoundMixerClipped;
/// An EWRAM byte Sound_Initialize clears and nothing else
/// names.
extern u8 gSound_UnreadByte;
void m4aSoundInit(void);
void m4aSongNumStart(u16 id);
void m4aSoundVSync(void);

#endif
