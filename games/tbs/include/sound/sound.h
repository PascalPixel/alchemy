#ifndef ALCHEMY_SOUND_H
#define ALCHEMY_SOUND_H

/// @file
/// The sound tables (CGB codes, frequency steps, samples a VBlank) and the
/// sound routines.

#include "types.h"
#include "edition.h"
#include "sound/sound_driver_types.h"

struct MusicPlayerInfo;
struct MusicPlayerTrack;
struct SoundInfo;

extern const u8 gCgb3Vol[];
extern const s16 gCgbFreqTable[];
extern const u8 gCgbScaleTable[];
extern const u16 gPcmSamplesPerVBlankTable[];
extern const u8 gNoiseTable[];
extern const u32 gFreqTable[];
extern const u8 gScaleTable[];
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
void DummyFunc(void);
void MPlayJumpTableCopy(MPlayFunc *table);
void RealClearChain(struct SoundChannel *note);
void SoundMainBTM(void *block);
/// The CGB channels' notes, which SoundInit hands the driver
/// (SoundInfo.cgbNotes).
extern struct CgbChannel gCgbChans[4];

// The music player table (sound/music_player_table.s) supplies the music
// player count and the mixer's line limit as the addresses of these symbols.
extern char gNumMusicPlayers[];
extern char gMaxLines[];

// The song commands (gMPlayJumpTableTemplate) and the extended commands
// (gXcmdTable), each called with the music player and the track.
void ply_fine(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_goto(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_patt(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_pend(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_rept(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_prio(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_tempo(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_keysh(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_voice(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_vol(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_pan(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_bend(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_bendr(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_lfodl(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_modt(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_tune(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_port(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xxx(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xtype(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xatta(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xdeca(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xsust(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xrele(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xiecv(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xiecl(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xleng(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xswee(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);

/// The build stamp System_CopyBuildStamp copies to IWRAM at power-on: twelve
/// digits and a zero word, at the bottom of the frame-wait stack.
extern s32 gBuildStamp[];
/// Nonzero once SOUND_FADE_OUT_BGM has started the music's fade; further fade
/// requests are ignored until the next music clears it.
extern u8 gBgmFadeRequested;
/// Set by the resident mixer whenever it clips a sample: when a sum overflows,
/// IwramSoundNormalizeChannels saturates the samples and stores the clip mask
/// (0x80008000) here, through pc.
extern s32 gSoundMixerClipped;
/// An EWRAM byte Sound_Initialize clears and nothing else
/// names.
extern u8 gSound_UnreadByte;
void m4aSoundInit(void);
void m4aSongNumStart(u16 id);
void m4aSoundVSync(void);

void Sound_Play(s32 id);
void Sound_UpdateBgmRamps(void);
void SoundTest_Run(void);
void Sound_Initialize(void);
void Sound_SetBgmTempo(u16 tempoScale);
void Sound_SetBgmPitch(u16 pitch);
void Sound_SetBgmTempoPitchTarget(u16 targetScale, u16 step);
void Sound_SetBgmVolume(s16 volume);
void Sound_SetBgmVolumeTarget(u16 targetVolume, u16 step);
s32 Sound_GetBgmRestoreDelay(void);
void Sound_StopAllMusicPlayers(void);
void Sound_ResumeAllMusicPlayers(void);
void Sound_UpdateCommandMask(u32 command);
s32 Sound_GetCurrentBgm(void);
void Sound_WaitForBgmRestore(void);
s32 Sound_GetReverbCode(s32 command);

#endif
