#ifndef ALCHEMY_SOUND_H
#define ALCHEMY_SOUND_H

// The game's sound: sySoundPlay and the music's volume and tempo ramps (sound_play.c), the sound
// test and the heard-sound flags (sound_test.c), over the development kit's sound driver
// (sdk/sound_driver.h).

#include "types.h"
#include "edition.h"
#include "sdk/sound_driver.h"
#include "system.h"

// The sound module's functions in the order of its stub table. Runs the sound driver for one VBlank
// through SoundMain.
void   m4aSoundMain(void);
// Starts the sound driver at power-on: no music, the volume and tempo ramps at rest, no player's
// last sound, and the fixed 31536 Hz output rate. As The Broken Seal's, but here no music is
// MUS_SILENCE, sound 0.
void   sySoundInit(void);
// Plays a sound number, the game's one way of playing a sound: a fade control, a sound effect, a
// jingle or music, as The Broken Seal's with this game's ranges (enum SoundControl). Every sound is
// first marked as heard (sySoundFlagSet), and none plays while the tick wait runs the driver's
// update.
void   sySoundPlay(Sint32 id);
// Runs the sound driver's VBlank work: the DMA restart count The Broken Seal's
// sySoundUpdateBGMRamps ran at its end. Reached through the stub call slot that function had.
void   sySoundVSync(void);
// Sets the BGM tempo scale directly; BGM_UNITY_SCALE preserves its native tempo.
void   sySoundSetBGMTempo(Uint16 tempoScale);
// Applies a pitch offset, in 1/256 semitone, to the music's tracks 0 to 7.
void   sySoundSetBGMPitch(Uint16 pitch);
// Sets the target and per-VBlank step of the combined BGM tempo/pitch ramp (BGM_UNITY_SCALE the
// music's own).
void   sySoundSetBGMTempoPitchTarget(Uint16 targetScale, Uint16 step);
// Sets BGM volume (BGM_UNITY_SCALE full) immediately and cancels any outstanding volume ramp.
void   sySoundSetBGMVolume(Sint16 volume);
// Sets the BGM volume target (BGM_UNITY_SCALE full) and its per-VBlank step.
void   sySoundSetBGMVolumeTarget(Uint16 targetVolume, Uint16 step);
// Returns nonzero while a jingle holds the music: from its start until its player falls silent and
// the music begins to fade back in.
Uint8  sySoundGetBGMRestoreDelay(void);
// Unused: the sound test, which never returns. Up and Down choose a selector (music, jingles, sound
// effects), Left and Right step its sound by 1 and L and R by 10, A plays it, B silences the music
// and Start fades it, and Select fades the action sounds and steps the reverb code, from
// SOUND_REVERB_LONG, which this game's sySoundLoadReverbCode ignores.
void   sySoundTestRun(void);
// Stops every sound player while preserving its state for a later resume.
void   sySoundStopAllMusicPlayers(void);
// Resumes every player, each from where m4aMPlayAllStop stopped it.
void   sySoundResumeAllMusicPlayers(void);
// Unused: replaces SoundCommandMask, or flips the bits named with AUDIO_COMMAND_TOGGLE. Nothing
// calls it and nothing reads the mask.
void   sySoundUpdateCommandMask(Uint32 command);
// Returns the music sySoundPlay last started: MUS_SILENCE before any and after a fade.
Uint16 sySoundGetCurrentBGM(void);
// Waits until a jingle has ended and the music has begun to come back, giving up after
// BGM_WAIT_TIMEOUT_TICKS.
void   sySoundWaitForBGMRestore(void);
// Stops the sound DMA and its timer; the soft reset calls it first.
void   sySoundSuspendDirectSound(void);
// Restores BGM after a jingle and steps its volume and combined tempo/pitch ramps once per frame,
// clamping each ramp when it crosses its target, as The Broken Seal's without the DMA restart count
// (sySoundVSync here).
void   sySoundUpdateBGMRamps(void);
// Returns sound `sound`'s bit in SoundFlags, one bit for each of the SOUND_FLAG_COUNT sound ids.
Bool   sySoundFlagTest(Sint32 sound);
// Sets sound `sound`'s bit in SoundFlags.
void   sySoundFlagSet(Sint32 sound);
// Clears sound `sound`'s bit in SoundFlags.
void   sySoundFlagClear(Sint32 sound);
// Unused: does nothing; the stub table exports it.
void   sySoundNoOperation(void);

Sint32 sySoundGetReverbCode(Sint32 sound);

// Returns nonzero while a jingle holds the music.
Sint32 sySoundGetBGMRestoreDelayStub(void);

// The three sounds the sound test's selectors start on.
typedef struct SND_TEST_SNDS
{
	Sint32 sound[3];
} SND_TEST_SNDS;

enum
{
	// Full volume, and the song's own tempo and pitch: 1.0 in the ramps' 8.8 fixed point.
	BGM_UNITY_SCALE          = 256,
	// A ramp's step a frame: a full fade in or out takes 64 frames.
	BGM_DEFAULT_RAMP_STEP    = 4,
	// The track mask of every music command here: tracks 0 to 7.
	TRACKS_ALL               = 0xff,
	MUSIC_PLAYER_COUNT       = 8, // MusicPlayerLastSound's entries, one a player.
	                        // Frames after a jingle starts before the music waits for it to end.
	BGM_RESTORE_DELAY_FRAMES = 10,
	// sySoundWaitForBGMRestore gives up after this many ticks, about five seconds.
	BGM_WAIT_TIMEOUT_TICKS   = 300
};

void sySoundSetFixedSampleFrequency(Uint32 mode);

// The byte `member` of the sound driver's state `info` (struct SoundInfo), read through
// SoundInfoPointer's bytes without the struct's volatile.
#define SOUND_INFO_BYTE(info, member) ((info)[offsetof(struct SoundInfo, member)])
// SoundMain as the tick wait runs it: steps the music players and the CGB channels and starts the
// IWRAM mixer on this VBlank's PCM, returning 0; it returns 1 with nothing done when the stack is
// too deep or the driver busy.
Sint32 SoundDriverEnterVBlankUpdate(void);
// The VBlank handler's sound step, as m4aSoundVSync: counts down the PCM buffer's frames and, as it
// wraps, restarts the two sound DMAs, which then read from the buffer's start again.
void   SoundDriverVSyncRefresh(void);

#endif
