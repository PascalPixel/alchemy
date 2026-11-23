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
/// The high word of the 64-bit product of two words: a 32-bit fraction's share
/// of a value, as MidiKeyToFreq takes the step between two keys.
u32 umul3232H32(u32 left, u32 right);
void SoundInit(struct SoundInfo *work);
void m4aSoundVSyncOn(void);
/// The sound driver's work each VBlank: steps the music players (the MPlayMain
/// chain), then mixes the next buffer through SoundMainRAM in IWRAM.
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
/// Advances one music player by its tempo's ticks for this VBlank, running the
/// commands of each track that falls due; MPlayOpen links it into the chain.
void MPlayMain(struct MusicPlayerInfo *player);
void TrkVolPitSet(struct MusicPlayerInfo *player,
                  struct MusicPlayerTrack *track);
/// A note command: keys a note on the track's voice for the command's length
/// (SoundInfo.noteOn).
void ply_note(u32 command, struct MusicPlayerInfo *player,
              struct MusicPlayerTrack *track);
void ply_memacc(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/// EOT, the version MPlayExtender installs: releases the track's tied note of
/// the key the command names, or of its last key.
void ply_endtie(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/// LFOS and MOD, the versions MPlayExtender installs: set the track's vibrato
/// speed and depth, a 0 stopping the vibrato at once.
void ply_lfos(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_mod(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/// Silences a track: every note still on its chain stops at once, a CGB
/// channel switched off with it.
void TrackStop(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/// The mixer's code as the ROM holds it, which m4aSoundInit copies to IWRAM
/// (Ram_SoundMixer) to run from there.
void SoundMainRAM_Rom(void);
void DummyFunc(void);
/// Copies the driver's jump table, gMPlayJumpTableTemplate, into table.
void MPlayJumpTableCopy(MPlayFunc *table);
/// Takes a note off its track's chain (ClearChain, through the jump table).
void RealClearChain(struct SoundChannel *note);
/// Clears 64 bytes (Clear64byte, through the jump table).
void SoundMainBTM(void *block);
/// The CGB channels' notes, which SoundInit hands the driver
/// (SoundInfo.cgbNotes).
extern struct CgbChannel gCgbChans[4];

/// The music player count, gMPlayTable's rows, as this symbol's address
/// (sound/music_player_table.s).
extern char gNumMusicPlayers[];
/// The mixer's line limit, 0 for none, as this symbol's address
/// (sound/music_player_table.s).
extern char gMaxLines[];

/// The song commands (gMPlayJumpTableTemplate) and the extended commands
/// (gXcmdTable), each called with the music player and the track.
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
/// The driver's VBlank step (Sound_VSync): counts down the PCM buffer's
/// VBlanks and, as it wraps, restarts the two sound DMAs, which then read from
/// the buffer's start again.
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
u8 Sound_GetBgmRestoreDelay(void);
void Sound_StopAllMusicPlayers(void);
void Sound_ResumeAllMusicPlayers(void);
void Sound_UpdateCommandMask(u32 command);
u16 Sound_GetCurrentBgm(void);
void Sound_WaitForBgmRestore(void);
s32 Sound_GetReverbCode(s32 command);

/// The music playing, which Sound_Play sets from each music id it starts.
extern u16 gCurrentBgm;
/// One bit for each sound id, set once the sound has played (SoundFlag_Set);
/// the link lobby offers only the tunes heard.
extern u8 gSoundFlags[];
/// Returns nonzero while a jingle holds the music.
s32 Sound_GetBgmRestoreDelayFar(void);
void Sound_WaitForJingle(void);

/// The three sounds the sound test's selectors start on.
struct SoundTestCues {
  s32 cue[3];
};

void Sound_WaitForBgmRestore(void);

/// The mixer's PCM output: SoundDriver_EnterVBlankUpdate hands it to
/// IwramSoundMixEntry, and Sound_SetFixedSampleFreq clears it and points DMA 1
/// at its start and DMA 2 0xc60 bytes in.
extern u8 gMixerPcmBuffer[];

enum {
  /// Full volume, and the song's own tempo and pitch: 1.0 in the ramps' 8.8
  /// fixed point.
  BGM_UNITY_SCALE = 256,
  /// A ramp's step a VBlank: a full fade in or out takes 64 VBlanks.
  BGM_DEFAULT_RAMP_STEP = 4,
  /// The track mask of every music command here: tracks 0 to 7.
  TRACKS_ALL = 0xff,
  MUSIC_PLAYER_COUNT = 8, ///< gMusicPlayerLastSound's entries, one a player.
  /// VBlanks after a jingle starts before the music waits for it to end.
  BGM_RESTORE_DELAY_VBLANKS = 10,
  /// Sound_WaitForBgmRestore gives up after this many frames, about
  /// five seconds.
  BGM_WAIT_TIMEOUT_FRAMES = 300
};

/// The sound effect from SOUND_SE_FIRST up each player last started, by player;
/// SOUND_FADE_OUT_ACTION_SE and SOUND_FADE_OUT_EXIT_SE clear the entries they
/// fade.
extern u16 gMusicPlayerLastSound[MUSIC_PLAYER_COUNT];

void Sound_SetFixedSampleFreq(u32 mode);

/// Plays the field's and the battle's action sounds (MUSIC_PLAYER_ACTION_SE);
/// SOUND_FADE_OUT_ACTION_SE fades it.
extern struct MusicPlayerInfo gMPlayInfo_ActionSE;
/// One of the sound effects' shared players (MUSIC_PLAYER_SE5).
extern struct MusicPlayerInfo gMPlayInfo_SE5;
/// Plays jingles; the music returns once it stops.
extern struct MusicPlayerInfo gMPlayInfo_Jingle;

void SoundFlag_Set(s32 sound);

#endif
