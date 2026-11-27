// The game's own sound code over the sound driver, as The Broken Seal's: sySoundPlay, which takes
// every sound number, the music's volume and tempo ramps, and the music's return after a jingle.

#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "sdk/sound_driver.h"

// The semitones of an octave.
#define SEMITONES_PER_OCTAVE 12

// Unused: does nothing; the stub table exports it.
void sySoundNoOperation(void)
{
}

// Starts the sound driver at power-on: no music, the volume and tempo ramps at rest, no player's
// last sound, and the fixed 31536 Hz output rate. As The Broken Seal's, but here no music is
// MUS_SILENCE, sound 0.
void sySoundInit(void)
{
	Uint16 *lastSound;
	Sint32  remaining;

	m4aSoundInit();
	CurrentBgm          = MUS_SILENCE;
	BgmRestoreDelay     = 0;
	BgmVolumeTarget     = BGM_UNITY_SCALE;
	BgmVolume           = BGM_UNITY_SCALE;
	BgmVolumeStep       = BGM_DEFAULT_RAMP_STEP;
	BgmTempoPitchTarget = BGM_UNITY_SCALE;
	BgmTempoPitch       = BGM_UNITY_SCALE;
	BgmTempoPitchStep   = BGM_DEFAULT_RAMP_STEP;
	BgmFadeRequested    = 0;
	SoundCommandMask    = 0;
	SoundUnreadByte     = 0;
	lastSound           = MusicPlayerLastSound;
	for (remaining = 0; remaining < MUSIC_PLAYER_COUNT; remaining++)
		*lastSound++ = 0;
	sySoundSetFixedSampleFrequency(SOUND_MODE_FREQ_GAME);
}

// Returns the reverb code sySoundPlay gives music `sound`: SOUND_REVERB_LONG for every one, where
// The Broken Seal's gives it to three.
Sint32 sySoundGetReverbCode(Sint32 sound)
{
	return SOUND_REVERB_LONG;
}

// Plays a sound number, the game's one way of playing a sound: a fade control, a sound effect, a
// jingle or music, as The Broken Seal's with this game's ranges (enum SoundControl). Every sound is
// first marked as heard (sySoundFlagSet), and none plays while the tick wait runs the driver's
// update.
void sySoundPlay(Sint32 id)
{
	Sint32 flags;
	Sint32 player;

	sySoundFlagSet(id);
	flags = id & SOUND_FLAGS_MASK;
	if (SoundVBlankStateWork.updating != 0)
		return;
	id &= SOUND_NUM_MASK;

	if (id == SOUND_FADE_OUT_BGM)
	{
		// Fade the music out, once until the next music starts: 16 steps 7 Frames apart, 112
		// frames.
		if (BgmFadeRequested != 0)
			return;
		m4aMPlayFadeOut(&MPlayInfoBGM, 7);
		BgmFadeRequested++;
		CurrentBgm = MUS_SILENCE;
	}
	else if (id == SOUND_FADE_OUT_ACTION_SE)
	{
		MusicPlayerLastSound[MUSIC_PLAYER_ACTION_SE] = 0;
		m4aMPlayFadeOut(&MPlayInfoActionSE, 3);
		MusicPlayerLastSound[MUSIC_PLAYER_SE5] = 0;
		m4aMPlayFadeOut(&MPlayInfoSE5, 3);
	}
	else if (id == SOUND_FADE_OUT_EXIT_SE)
	{
		MusicPlayerLastSound[MUSIC_PLAYER_SE5] = 0;
		m4aMPlayFadeOut(&MPlayInfoSE5, 3);
	}
	else if (id >= SOUND_SE_END)
	{
		goto background;
	}
	else if (id >= SOUND_SE_FIRST)
	{
		// A sound on MUSIC_PLAYER_SE_LAST borrows a free effect player, searching back to
		// MUSIC_PLAYER_SE_FIRST; with all four busy, MUSIC_PLAYER_SE_LAST's sound is cut off.
		player = SongTable[id].ms;
		if (player == MUSIC_PLAYER_SE_LAST)
		{
			while ((Uint8)MPlayTable[player].info->status != 0)
			{
				player--;
				if (player < MUSIC_PLAYER_SE_FIRST)
				{
					player = MUSIC_PLAYER_SE_LAST;
					break;
				}
			}
		}
		MPlayStart(MPlayTable[player].info, SongTable[id].header);
		MusicPlayerLastSound[player] = id;
	}
	else if (id >= SOUND_JINGLE_FIRST)
	{
		// A jingle, on its own player: the music's tracks 0 to 7 go silent at once but keep
		// playing, and fade back in once the jingle has ended (sySoundUpdateBGMRamps).
		m4aMPlayVolumeControl(&MPlayInfoBGM, TRACKS_ALL, 0);
		BgmVolumeTarget = 0;
		BgmVolume       = 0;
		m4aSongNumStart(id);
		BgmRestoreDelay = BGM_RESTORE_DELAY_FRAMES;
	}
	else
	{
		// Music: the keep control, or the music last chosen, changes nothing.
		if (id == SOUND_KEEP_BGM)
			return;
	background:
		if (id == CurrentBgm)
			return;
		CurrentBgm = id;
		sySoundLoadReverbCode(sySoundGetReverbCode(id));
		m4aSongNumStart(id);
		// A silent start fades the new music in over 64 frames.
		if (flags & SOUND_FLAG_FADE_IN)
			BgmVolume = 0;
		else
			BgmVolume = BGM_UNITY_SCALE;
		BgmVolumeTarget  = BGM_UNITY_SCALE;
		BgmVolumeStep    = BGM_DEFAULT_RAMP_STEP;
		BgmFadeRequested = 0;
	}
}

// Restores BGM after a jingle and steps its volume and combined tempo/pitch ramps once per frame,
// clamping each ramp when it crosses its target, as The Broken Seal's without the DMA restart count
// (sySoundVSync here).
void sySoundUpdateBGMRamps(void)
{
	Sint32 delta;

	// After a jingle starts, count down to 1, then hold there until the jingle's player has no
	// track left and let the music fade back in.
	if (BgmRestoreDelay != 0)
	{
		if (BgmRestoreDelay == 1)
		{
			if ((Uint8)MPlayInfoJingle.status == 0)
			{
				BgmRestoreDelay = 0;
				BgmVolumeTarget = BGM_UNITY_SCALE;
			}
		}
		else
		{
			BgmRestoreDelay -= 1;
		}
	}
	// A step that changes the sign of target - value has crossed the target, and the value stops on
	// it.
	if ((Sint16)BgmVolumeTarget != (Sint16)BgmVolume)
	{
		delta = (Sint16)BgmVolumeTarget - (Sint16)BgmVolume;
		if (delta > 0)
			BgmVolume += BgmVolumeStep;
		else
			BgmVolume -= BgmVolumeStep;
		if ((((Sint16)BgmVolumeTarget - (Sint16)BgmVolume) ^ delta) < 0)
			BgmVolume = BgmVolumeTarget;
		m4aMPlayVolumeControl(&MPlayInfoBGM, TRACKS_ALL, BgmVolume);
	}
	if ((Sint16)BgmTempoPitchTarget != (Sint16)BgmTempoPitch)
	{
		delta = (Sint16)BgmTempoPitchTarget - (Sint16)BgmTempoPitch;
		if (delta > 0)
			BgmTempoPitch += BgmTempoPitchStep;
		else
			BgmTempoPitch -= BgmTempoPitchStep;
		if ((((Sint16)BgmTempoPitchTarget - (Sint16)BgmTempoPitch) ^ delta) < 0)
			BgmTempoPitch = BgmTempoPitchTarget;
		// The tempo scale and the pitch move together, like a record played at another speed, but
		// in a straight line: an octave for each BGM_UNITY_SCALE of scale (pitch is in 1/256
		// semitone), so twice the scale plays an octave up.
		m4aMPlayTempoControl(&MPlayInfoBGM, BgmTempoPitch);
		m4aMPlayPitchControl(&MPlayInfoBGM,
		                     TRACKS_ALL,
		                     (Sint16)BgmTempoPitch * SEMITONES_PER_OCTAVE
		                         - SEMITONES_PER_OCTAVE * BGM_UNITY_SCALE);
	}
}

// Runs the sound driver's VBlank work: the DMA restart count The Broken Seal's
// sySoundUpdateBGMRamps ran at its end. Reached through the stub call slot that function had.
void sySoundVSync(void)
{
	m4aSoundVSync();
}

// Sets the BGM tempo scale directly; BGM_UNITY_SCALE preserves its native tempo.
void sySoundSetBGMTempo(Uint16 tempoScale)
{
	m4aMPlayTempoControl(&MPlayInfoBGM, tempoScale);
}

// Applies a pitch offset, in 1/256 semitone, to the music's tracks 0 to 7.
void sySoundSetBGMPitch(Uint16 pitch)
{
	m4aMPlayPitchControl(&MPlayInfoBGM, TRACKS_ALL, pitch);
}

// Sets the target and per-VBlank step of the combined BGM tempo/pitch ramp (BGM_UNITY_SCALE the
// music's own).
void sySoundSetBGMTempoPitchTarget(Uint16 targetScale, Uint16 step)
{
	BgmTempoPitchTarget = targetScale;
	BgmTempoPitchStep   = step;
}

// Sets BGM volume (BGM_UNITY_SCALE full) immediately and cancels any outstanding volume ramp.
void sySoundSetBGMVolume(Sint16 volume)
{
	m4aMPlayVolumeControl(&MPlayInfoBGM, TRACKS_ALL, volume);
	BgmVolumeTarget = volume;
	BgmVolume       = volume;
}

// Sets the BGM volume target (BGM_UNITY_SCALE full) and its per-VBlank step.
void sySoundSetBGMVolumeTarget(Uint16 targetVolume, Uint16 step)
{
	BgmVolumeTarget = targetVolume;
	BgmVolumeStep   = step;
}

// Stops every sound player while preserving its state for a later resume.
void sySoundStopAllMusicPlayers(void)
{
	m4aMPlayAllStop();
}

// Resumes every player, each from where m4aMPlayAllStop stopped it.
void sySoundResumeAllMusicPlayers(void)
{
	m4aMPlayAllContinue();
}

// Returns nonzero while a jingle holds the music: from its start until its player falls silent and
// the music begins to fade back in.
Uint8 sySoundGetBGMRestoreDelay(void)
{
	return BgmRestoreDelay;
}

// Waits until a jingle has ended and the music has begun to come back, giving up after
// BGM_WAIT_TIMEOUT_TICKS.
void sySoundWaitForBGMRestore(void)
{
	Sint32 ticksWaited = 0;

	do
	{
		if (BgmRestoreDelay == 0)
			break;
		sySynch(1);
		ticksWaited++;
	} while (ticksWaited < BGM_WAIT_TIMEOUT_TICKS);
}

// Stops the sound DMA and its timer; the soft reset calls it first.
void sySoundSuspendDirectSound(void)
{
	m4aSoundVSyncOff();
}
