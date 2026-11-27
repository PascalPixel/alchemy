#ifndef ALCHEMY_SOUND_H
#define ALCHEMY_SOUND_H

/* @file */
/* The sound tables (CGB codes, frequency steps, samples a VBlank) and the */
/* sound functions. */

#include "types.h"
#include "edition.h"
#include "sdk/sound_driver_types.h"

struct MusicPlayerInfo;
struct MusicPlayerTrack;
struct SoundInfo;

extern const u8 Cgb3Vol[];
extern const s16 CgbFreqTable[];
extern const u8 CgbScaleTable[];
extern const u16 PcmSamplesPerVBlankTable[];
extern const u8 NoiseTable[];
extern const u32 FreqTable[];
extern const u8 ScaleTable[];
/* The sound driver's work, which m4aSoundInit sets up through */
/* SoundInit. */
extern struct SoundInfo SoundInfoWork;
/* The 16 work bytes every music player shares (MusicPlayerInfo.workBytes), so
 */
/* one sound can test what another has set (ply_memacc). */
extern u8 MPlayMemAccArea[];
void m4aMPlayAllContinue(void);
void MPlayContinue(struct MusicPlayerInfo *player);
void m4aMPlayAllStop(void);
/* The high 32 bits of the 64-bit product of left and right. */
u32 umul3232H32(u32 left, u32 right);
void SoundInit(struct SoundInfo *work);
void m4aSoundVSyncOn(void);
/* Runs one VBlank of the sound driver: the music players, then the mixer. */
void SoundMain(void);
void m4aSoundMode(u32 mode);
void SampleFreqSet(u32 mode);
void m4aSoundVSyncOff(void);
s32 MidiKeyToCgbFreq(u8 kind, u8 key, u8 fine);
void CgbSound(void);
void CgbOscOff(u8 channel);
void MPlayFadeOut(struct MusicPlayerInfo *player, u16 period);
void MPlayOpen(struct MusicPlayerInfo *player, struct MusicPlayerTrack *tracks,
			   u8 count);
void FadeOutBody(struct MusicPlayerInfo *player);
void m4aMPlayStop(struct MusicPlayerInfo *player);
/* Advances one music player by a VBlank: reads each track's commands as its */
/* tempo comes due and starts and ends its notes. */
void MPlayMain(struct MusicPlayerInfo *player);
void TrkVolPitSet(struct MusicPlayerInfo *player,
				  struct MusicPlayerTrack *track);
/* The note commands: plays a note for a length, or tied until EOT. */
void ply_note(u32 command, struct MusicPlayerInfo *player,
			  struct MusicPlayerTrack *track);
void ply_memacc(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* EOT: releases a tied note. */
void ply_endtie(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* LFOS: the track's vibrato speed. */
void ply_lfos(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* MOD: the track's vibrato depth. */
void ply_mod(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* Releases every note a track is playing. */
void TrackStop(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* The mixer's code as stored in ROM; m4aSoundInit copies it to IWRAM. */
void sySndMainRAMRom(void);
void DummyFunc(void);
/* Fills table with the driver's 36 song command and helper functions. */
void MPlayJumpTableCopy(MPlayFunc *table);
/* Takes a playing note off its track's list of notes. */
void RealClearChain(struct SoundChannel *note);
/* Clears the 64 bytes at block. */
void SoundMainBTM(void *block);
/* The CGB channels' notes, which SoundInit hands the driver */
/* (SoundInfo.cgbNotes). */
extern struct CgbChannel CgbChans[4];

/* The music player table (sound/music_player_table.s) supplies the music */
/* player count and the mixer's line limit as the addresses of these symbols. */
extern char NumMusicPlayers[];
extern char MaxLines[];

/* The song commands (MPlayJumpTableTemplate) and the extended commands */
/* (XcmdTable), each called with the music player and the track. */
/* FINE: ends the track. */
void ply_fine(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* GOTO: jumps to another place in the track. */
void ply_goto(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* PATT: plays a pattern, returning at its PEND. */
void ply_patt(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* PEND: returns from a pattern. */
void ply_pend(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* REPT: jumps back a number of times. */
void ply_rept(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* PRIO: the track's priority for a free channel. */
void ply_prio(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* TEMPO: the song's tempo; its byte is half the beats a minute. */
void ply_tempo(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* KEYSH: transposes the track, in semitones. */
void ply_keysh(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* VOICE: the track's instrument from the song's voice group. */
void ply_voice(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* VOL: the track's volume. */
void ply_vol(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* PAN: the track's place between the speakers. */
void ply_pan(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* BEND: the track's pitch bend. */
void ply_bend(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* BENDR: how far a full bend goes, in semitones. */
void ply_bendr(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* LFODL: how long a note plays before its vibrato starts. */
void ply_lfodl(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* MODT: what the vibrato moves: pitch, volume or pan. */
void ply_modt(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* TUNE: fine tuning, within a semitone. */
void ply_tune(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* PORT: writes a byte to a sound register. */
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

/* Nonzero once SOUND_FADE_OUT_BGM has started the music's fade; further fade */
/* requests are ignored until the next music clears it. */
extern u8 BgmFadeRequested;
/* Set by the resident mixer whenever it clips a sample: when a sum overflows,
 */
/* IwramSoundNormalizeChannels saturates the samples and stores the clip mask */
/* (0x80008000) here. Only the unused sound test reads it. */
extern s32 SoundMixerClipped;
/* An EWRAM byte sySndInit clears and nothing else */
/* names. */
extern u8 Sound_UnreadByte;
void m4aSoundInit(void);
void m4aSongNumStart(u16 id);
/* Restarts the two sound DMAs at the start of the mix buffer once it has */
/* played through; called each VBlank. */
void m4aSoundVSync(void);

/* The sound module's functions in the order of its stub table */
/* (src/sound/stubs.s). */
void m4aSoundMain(void);
void sySndInit(void);
void sySndPlay(s32 id);
void sySndUpdateBgmRamps(void);
void sySndSetBgmTempo(u16 tempoScale);
void sySndSetBgmPitch(u16 pitch);
void sySndSetBgmTempoPitchTarget(u16 targetScale, u16 step);
void sySndSetBgmVolume(s16 volume);
void sySndSetBgmVolumeTarget(u16 targetVolume, u16 step);
s32 sySndGetBgmRestoreDelay(void);
void sySndTestRun(void);
void sySndStopAllMusicPlayers(void);
void sySndResumeAllMusicPlayers(void);
void sySndUpdateCommandMask(u32 command);
s32 sySndGetCurrentBgm(void);
void sySndWaitForBgmRestore(void);

s32 sySndGetReverbCode(s32 command);

#endif
