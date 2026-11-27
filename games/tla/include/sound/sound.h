#ifndef ALCHEMY_SOUND_H
#define ALCHEMY_SOUND_H

/* @file */
/* The game's sound: sySndPlay and the music's volume and tempo ramps */
/* (sound_play.c), the sound test and the heard-sound flags (sound_test.c), */
/* and the sound driver's functions and pitch tables beneath them. */

#include "types.h"
#include "edition.h"
#include "sdk/sound_driver_types.h"
#include "system.h"

/* The sound module's functions in the order of its stub table. */
/* Runs the sound driver for one VBlank through SoundMain. */
void m4aSoundMain(void);
/* Starts the sound driver at power-on: no music, the volume and tempo ramps at
 */
/* rest, no player's last sound, and the fixed 31536 Hz output rate. As The */
/* Broken Seal's, but here no music is MUS_SILENCE, sound 0. */
void sySndInit(void);
/* Plays a sound number, the game's one way of playing a sound: a fade control,
 */
/* a sound effect, a jingle or music, as The Broken Seal's with this game's */
/* ranges (enum SoundControl). Every sound is first marked as heard */
/* (sySndFlagSet), and none plays while the frame wait runs the driver's */
/* update. */
void sySndPlay(s32 id);
/* Runs the sound driver's VBlank work: the DMA restart count The Broken */
/* Seal's sySndUpdateBgmRamps ran at its end. Reached through the stub call */
/* slot that function had. */
void sySndVSync(void);
/* Sets the BGM tempo scale directly; BGM_UNITY_SCALE preserves its native */
/* tempo. */
void sySndSetBgmTempo(u16 tempoScale);
/* Applies a pitch offset, in 1/256 semitone, to the music's tracks 0 to 7. */
void sySndSetBgmPitch(u16 pitch);
/* Sets the target and per-VBlank step of the combined BGM tempo/pitch ramp */
/* (BGM_UNITY_SCALE the music's own). */
void sySndSetBgmTempoPitchTarget(u16 targetScale, u16 step);
/* Sets BGM volume (BGM_UNITY_SCALE full) immediately and cancels any */
/* outstanding volume ramp. */
void sySndSetBgmVolume(s16 volume);
/* Sets the BGM volume target (BGM_UNITY_SCALE full) and its per-VBlank step. */
void sySndSetBgmVolumeTarget(u16 targetVolume, u16 step);
/* Returns nonzero while a jingle holds the music: from its start until its */
/* player falls silent and the music begins to fade back in. */
u8 sySndGetBgmRestoreDelay(void);
/* Unused: the sound test, which never returns. Up and Down choose a selector */
/* (music, jingles, sound effects), Left and Right step its sound by 1 and L */
/* and R by 10, A plays it, B silences the music and Start fades it, and Select
 */
/* fades the action sounds and steps the reverb code, from SOUND_REVERB_LONG, */
/* which this game's sySndLoadReverbCode ignores. */
void sySndTestRun(void);
/* Stops every sound player while preserving its state for a later resume. */
void sySndStopAllMusicPlayers(void);
/* Resumes every player, each from where m4aMPlayAllStop stopped it. */
void sySndResumeAllMusicPlayers(void);
/* Unused: replaces SoundCommandMask, or flips the bits named with */
/* AUDIO_COMMAND_TOGGLE. Nothing calls it and nothing reads the mask. */
void sySndUpdateCommandMask(u32 command);
/* Returns the music sySndPlay last started: MUS_SILENCE before any and after */
/* a fade. */
u16 sySndGetCurrentBgm(void);
/* Waits until a jingle has ended and the music has begun to come back, giving
 */
/* up after BGM_WAIT_TIMEOUT_FRAMES. */
void sySndWaitForBgmRestore(void);
/* Stops the sound DMA and its timer; the soft reset calls it first. */
void sySndSuspendDirectSnd(void);
/* Restores BGM after a jingle and steps its volume and combined tempo/pitch */
/* ramps once per VBlank, clamping each ramp when it crosses its target, as */
/* The Broken Seal's without the DMA restart count (sySndVSync here). */
void sySndUpdateBgmRamps(void);
/* Returns sound `sound`'s bit in SoundFlags, one bit for each of the */
/* SOUND_FLAG_COUNT sound ids. */
s32 sySndFlagTest(s32 sound);
/* Sets sound `sound`'s bit in SoundFlags. */
void sySndFlagSet(s32 sound);
/* Clears sound `sound`'s bit in SoundFlags. */
void sySndFlagClear(s32 sound);
/* Unused: does nothing; the stub table exports it. */
void sySndNoOp(void);

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
void m4aMPlayAllContinue(void);
void MPlayContinue(struct MusicPlayerInfo *player);
void m4aMPlayAllStop(void);
/* The high word of the 64-bit product of two words: a 32-bit fraction's share
 */
/* of a value, as MidiKeyToFreq takes the step between two keys. */
u32 umul3232H32(u32 left, u32 right);
void SoundInit(struct SoundInfo *work);
void m4aSoundVSyncOn(void);
/* The sound driver's work each VBlank: steps the music players (the MPlayMain
 */
/* chain), then mixes the next buffer through SoundMainRAM in IWRAM. */
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
/* Advances one music player by its tempo's ticks for this VBlank, running the
 */
/* commands of each track that falls due; MPlayOpen links it into the chain. */
void MPlayMain(struct MusicPlayerInfo *player);
void TrkVolPitSet(struct MusicPlayerInfo *player,
				  struct MusicPlayerTrack *track);
/* A note command: keys a note on the track's voice for the command's length */
/* (SoundInfo.noteOn). */
void ply_note(u32 command, struct MusicPlayerInfo *player,
			  struct MusicPlayerTrack *track);
void ply_memacc(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* EOT, the version MPlayExtender installs: releases the track's tied note of */
/* the key the command names, or of its last key. */
void ply_endtie(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* LFOS and MOD, the versions MPlayExtender installs: set the track's vibrato */
/* speed and depth, a 0 stopping the vibrato at once. */
void ply_lfos(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_mod(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* Silences a track: every note still on its chain stops at once, a CGB */
/* channel switched off with it. */
void TrackStop(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* The mixer's code as the ROM holds it, which m4aSoundInit copies to IWRAM */
/* (Ram_SoundMixer) to run from there. */
void sySndMainRAMRom(void);
void DummyFunc(void);
/* Copies the driver's jump table, MPlayJumpTableTemplate, into table. */
void MPlayJumpTableCopy(MPlayFunc *table);
/* Takes a note off its track's chain (ClearChain, through the jump table). */
void RealClearChain(struct SoundChannel *note);
/* Clears 64 bytes (Clear64byte, through the jump table). */
void SoundMainBTM(void *block);
/* The CGB channels' notes, which SoundInit hands the driver */
/* (SoundInfo.cgbNotes). */
extern struct CgbChannel CgbChans[4];

/* The music player count, MPlayTable's rows, as this symbol's address */
/* (sound/music_player_table.s). */
extern char NumMusicPlayers[];
/* The mixer's line limit, 0 for none, as this symbol's address */
/* (sound/music_player_table.s). */
extern char MaxLines[];

/* The song commands (MPlayJumpTableTemplate) and the extended commands */
/* (XcmdTable), each called with the music player and the track. */
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

void m4aSoundInit(void);
void m4aSongNumStart(u16 id);
/* The driver's VBlank step (sySndVSync): counts down the PCM buffer's */
/* VBlanks and, as it wraps, restarts the two sound DMAs, which then read from
 */
/* the buffer's start again. */
void m4aSoundVSync(void);

s32 sySndGetReverbCode(s32 sound);

/* Returns nonzero while a jingle holds the music. */
s32 sySndGetBgmRestoreDelayStub(void);

/* The three sounds the sound test's selectors start on. */
typedef struct SND_TEST_CUES {
	s32 cue[3];
} SND_TEST_CUES;

enum {
	/* Full volume, and the song's own tempo and pitch: 1.0 in the ramps' 8.8 */
	/* fixed point. */
	BGM_UNITY_SCALE = 256,
	/* A ramp's step a VBlank: a full fade in or out takes 64 VBlanks. */
	BGM_DEFAULT_RAMP_STEP = 4,
	/* The track mask of every music command here: tracks 0 to 7. */
	TRACKS_ALL = 0xff,
	MUSIC_PLAYER_COUNT = 8, /* MusicPlayerLastSound's entries, one a player. */
	/* VBlanks after a jingle starts before the music waits for it to end. */
	BGM_RESTORE_DELAY_VBLANKS = 10,
	/* sySndWaitForBgmRestore gives up after this many frames, about */
	/* five seconds. */
	BGM_WAIT_TIMEOUT_FRAMES = 300
};

void sySndSetFixedSampleFreq(u32 mode);

/* The byte `member` of the sound driver's state `info` (struct SoundInfo), */
/* read through SoundInfoPointer's bytes without the struct's volatile. */
#define SOUND_INFO_BYTE(info, member)                                          \
	((info)[offsetof(struct SoundInfo, member)])
/* SoundMain as the frame wait runs it: steps the music players and the CGB */
/* channels and starts the IWRAM mixer on this VBlank's PCM, returning 0; it */
/* returns 1 with nothing done when the stack is too deep or the driver busy. */
s32 SoundDriver_EnterVBlankUpdate(void);
/* The VBlank handler's sound step, as m4aSoundVSync: counts down the PCM */
/* buffer's VBlanks and, as it wraps, restarts the two sound DMAs, which then */
/* read from the buffer's start again. */
void SoundDriver_VSyncRefresh(void);

#endif
