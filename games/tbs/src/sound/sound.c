/* The game's own sound code over the sound driver: sySndPlay, which takes every
   sound number, the music's volume and tempo ramps, the music's return after a
   jingle, and a sound test nothing in the game names. */

#include "sound/sound.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "sound/sound_driver_symbols.h"
#include "sdk/sound_driver.h"

enum {
	/* Full volume, and the song's own tempo and pitch: 1.0 in the ramps' 8.8
	   fixed point. */
	BGM_UNITY_SCALE = 256,
	/* A ramp's step a frame: a full fade in or out takes 64 frames. */
	BGM_DEFAULT_RAMP_STEP = 4,
	/* The track mask of every music command here. */
	TRACKS_ALL = 0xff,
	MUSIC_PLAYER_COUNT = 8, /* MusicPlayerLastSound' entries, one a player. */
	/* Frames after a jingle starts before the music waits for it to end. */
	BGM_RESTORE_DELAY_FRAMES = 10,
	/* sySndWaitForBGMRestore gives up after this many ticks, about five
	   seconds. */
	BGM_WAIT_TIMEOUT_TICKS = 300,
	/* CurrentBgm before any music: an id no music has, so the first music
	   always plays. */
	BGM_NONE = 255
};

/* The semitones of an octave. */
#define SEMITONES_PER_OCTAVE 12

/* Plays a sound number, the game's one way of playing a sound: a fade control,
   a sound effect, a jingle or music, as the README's "What a sound number
   means" describes. */
void sySndPlay(Sint32 id)
{
	Sint32 flg;
	Sint32 player;

	flg = id & SOUND_FLAGS_MASK;
	id &= SOUND_NUM_MASK;

	if(id == SOUND_FADE_OUT_BGM) {
		/* Fade the music out, once until the next music starts: 16 steps 7
		   Frames apart, 112 frames. */
		if(BgmFadeRequested != 0)
			return;
		m4aMPlayFadeOut(&MPlayInfo_BGM, 7);
		BgmFadeRequested++;
		CurrentBgm = MUS_SILENCE;
	} else if(id == SOUND_FADE_OUT_ACTION_SE) {
		/* The control is also the number of the song table's row 289, so it
		   fades MUSIC_PLAYER_ACTION_SE out over 48 frames (16 steps of 3) and
		   that row never plays. */
		MusicPlayerLastSound[MUSIC_PLAYER_ACTION_SE] = 0;
		m4aMPlayFadeOut(&MPlayInfo_ActionSE, 3);
	} else if(id >= SOUND_SE_FIRST) {
		/* A sound on MUSIC_PLAYER_SE_LAST borrows a free effect player,
		   searching back to MUSIC_PLAYER_SE_FIRST. */
		player = SongTable[id].ms;
		if(player == MUSIC_PLAYER_SE_LAST) {
			while((Uint8)MPlayTable[player].info->status != 0) {
				player--;
				if(player < MUSIC_PLAYER_SE_FIRST) {
					player = MUSIC_PLAYER_SE_LAST;
					break;
				}
			}
		}
		MPlayStart(MPlayTable[player].info, SongTable[id].header);
		MusicPlayerLastSound[player] = id;
	} else if(id >= SOUND_JINGLE_FIRST) {
		/* A jingle, on its own player (MUSIC_PLAYER_JINGLE): the music's tracks
		   0 to 7 go silent at once but keep playing, and fade back in once the
		   jingle has ended (sySndUpdateBGMRamps). */
		m4aMPlayVolumeControl(&MPlayInfo_BGM, TRACKS_ALL, 0);
		BgmVolumeTarget = 0;
		BgmVolume = 0;
		m4aSongNumStart(id);
		BgmRestoreDelay = BGM_RESTORE_DELAY_FRAMES;
	} else {
		/* Music: the keep control, or the music last chosen (CurrentBgm),
		   changes nothing, so a scene may name its music on every entry; music
		   that has ended is not started again. */
		if(id == SOUND_KEEP_BGM)
			return;
		if(id == CurrentBgm)
			return;
		CurrentBgm = id;
		sySndLoadReverbCode(
			(id == MUS_MAIN_THEME_TITLE || id == MUS_TO_BE_CONTINUED ||
				id == MUS_THE_BROKEN_SEAL_ENDING)
				? SOUND_REVERB_LONG
				: SOUND_REVERB_NORMAL);
		m4aSongNumStart(id);
		/* A silent start fades the new music in over 64 frames. */
		if(flg & SOUND_FLAG_FADE_IN)
			BgmVolume = 0;
		else
			BgmVolume = BGM_UNITY_SCALE;
		BgmVolumeTarget = BGM_UNITY_SCALE;
		BgmVolumeStep = BGM_DEFAULT_RAMP_STEP;
		BgmFadeRequested = 0;
	}
}

/* Runs once a VBlank: brings the music back after a jingle and steps its volume
   and its tempo and pitch toward their targets, stopping on each. */
void sySndUpdateBGMRamps(void)
{
	Sint32 delta;

	/* After a jingle starts, count down to 1, then hold there until the
	   jingle's player has no track left and let the music fade back in. */
	if(BgmRestoreDelay != 0) {
		if(BgmRestoreDelay == 1) {
			if((Uint8)MPlayInfo_Jingle.status == 0) {
				BgmRestoreDelay = 0;
				BgmVolumeTarget = BGM_UNITY_SCALE;
			}
		} else {
			BgmRestoreDelay -= 1;
		}
	}
	/* A step that changes the sign of target - value has crossed the target,
	   and the value stops on it. */
	if((Sint16)BgmVolumeTarget != (Sint16)BgmVolume) {
		delta = (Sint16)BgmVolumeTarget - (Sint16)BgmVolume;
		if(delta > 0) {
			BgmVolume += BgmVolumeStep;
		} else {
			BgmVolume -= BgmVolumeStep;
		}
		if((((Sint16)BgmVolumeTarget - (Sint16)BgmVolume) ^ delta) < 0) {
			BgmVolume = BgmVolumeTarget;
		}
		m4aMPlayVolumeControl(&MPlayInfo_BGM, TRACKS_ALL, BgmVolume);
	}
	if((Sint16)BgmTempoPitchTarget != (Sint16)BgmTempoPitch) {
		delta = (Sint16)BgmTempoPitchTarget - (Sint16)BgmTempoPitch;
		if(delta > 0) {
			BgmTempoPitch += BgmTempoPitchStep;
		} else {
			BgmTempoPitch -= BgmTempoPitchStep;
		}
		if((((Sint16)BgmTempoPitchTarget - (Sint16)BgmTempoPitch) ^ delta) <
			0) {
			BgmTempoPitch = BgmTempoPitchTarget;
		}
		/* The tempo scale and the pitch move together, like a record played at
		   another speed, but in a straight line: an octave for each
		   BGM_UNITY_SCALE of scale (pitch is in 1/256 semitone), so twice
		   BGM_UNITY_SCALE plays an octave up and half of it half an octave
		   down. */
		m4aMPlayTempoControl(&MPlayInfo_BGM, BgmTempoPitch);
		m4aMPlayPitchControl(&MPlayInfo_BGM, TRACKS_ALL,
			(Sint16)BgmTempoPitch * SEMITONES_PER_OCTAVE -
				SEMITONES_PER_OCTAVE * BGM_UNITY_SCALE);
	}
	/* Count down the frames left in the PCM buffer and restart the FIFO DMAs
	   from its start when they run out (SoundInfo.transferCountdown). */
	m4aSoundVSync();
}

/* Unused: a sound test that never returns. Up and Down choose a row (music,
   jingles, sound effects), Left and Right step its number by 1 and L and R by
   10, A plays it, B silences the music, Start fades it out, and Select fades
   the action sounds and moves to the next reverb code. Only a bare stub call
   stub, which no code names, leads here. */
void sySndTestRun(void)
{
	/* One row each for music, jingles and sound effects. */
	Sint32 snds[3] = {0, SOUND_JINGLE_FIRST, SOUND_SE_FIRST};
	Sint32 reverbCode;
	Sint32 row;
	Sint32 clipTmr;

	row = 0;
	reverbCode = SOUND_REVERB_NORMAL;
	IO_WRITE32(0, &BuildStampWork[0]);
	clipTmr = 20;

	for(;;) {
		/* A 20-tick countdown, restarted whenever the mixer has clipped a
		   sample since the last tick; nothing reads it. */
		if(clipTmr != 0)
			clipTmr--;

		if(SoundMixerClipped != 0) {
			SoundMixerClipped = 0;
			clipTmr = 20;
		}

		if(PerRepeat & KEY_SELECT) {
			reverbCode = (reverbCode + 1) % (SOUND_REVERB_NONE + 1);
			sySndLoadReverbCode(reverbCode);
		}
		if(PerRepeat & KEY_R)
			snds[row] += 10;
		if(PerRepeat & KEY_L)
			snds[row] -= 10;
		if(PerRepeat & KEY_RIGHT)
			snds[row] += 1;
		if(PerRepeat & KEY_LEFT)
			snds[row] -= 1;
		if(PerRepeat & KEY_UP) {
			if(row > 0)
				row--;
		}
		if(PerRepeat & KEY_DOWN) {
			if(row <= 1)
				row++;
		}
		if(PerRepeat & KEY_A)
			sySndPlay(snds[row]);
		if(PerRepeat & KEY_B)
			sySndPlay(MUS_SILENCE);
		if(PerRepeat & KEY_START)
			sySndPlay(SOUND_FADE_OUT_BGM);
		if(PerRepeat & KEY_SELECT)
			sySndPlay(SOUND_FADE_OUT_ACTION_SE);

		sySynch(1);
	}
}

/* Starts the sound driver at power-on, with the music at full volume and its
   own tempo, and no music chosen yet. */
void sySndInit(void)
{
	Sint32 i;

	m4aSoundInit();
	CurrentBgm = BGM_NONE;
	BgmRestoreDelay = 0;
	BgmVolumeTarget = BGM_UNITY_SCALE;
	BgmVolume = BGM_UNITY_SCALE;
	BgmVolumeStep = BGM_DEFAULT_RAMP_STEP;
	BgmTempoPitchTarget = BGM_UNITY_SCALE;
	BgmTempoPitch = BGM_UNITY_SCALE;
	BgmTempoPitchStep = BGM_DEFAULT_RAMP_STEP;
	BgmFadeRequested = 0;
	SoundCommandMaskWork = 0;
	Sound_UnreadByte = 0;
	for(i = 0; i < MUSIC_PLAYER_COUNT; i++)
		MusicPlayerLastSound[i] = 0;
}

/* Plays the music at a tempo scale at once, BGM_UNITY_SCALE its own; its pitch
   stays. The Lemurian ship slows it to fifteen sixteenths. */
void sySndSetBGMTempo(Uint16 tempoScale)
{
	m4aMPlayTempoControl(&MPlayInfo_BGM, tempoScale);
}

/* Unused: transposes the music's tracks 0 to 7 by `pitch`, in 1/256 semitone.
   Only a bare stub, which no code names, leads here. */
void sySndSetBGMPitch(Uint16 pitch)
{
	m4aMPlayPitchControl(&MPlayInfo_BGM, TRACKS_ALL, pitch);
}

/* Unused: ramps the music's tempo and pitch together toward `tgtScale`
   (BGM_UNITY_SCALE its own) by `step` a frame. Only a bare stub, which no code
   names, leads here. */
void sySndSetBGMTempoPitchTarget(Uint16 tgtScale, Uint16 step)
{
	BgmTempoPitchTarget = tgtScale;
	BgmTempoPitchStep = step;
}

/* Unused: sets the music's volume at once (BGM_UNITY_SCALE full), ending any
   fade toward another. Only a bare stub, which no code names, leads here. */
void sySndSetBGMVolume(Sint16 volume)
{
	m4aMPlayVolumeControl(&MPlayInfo_BGM, TRACKS_ALL, volume);
	BgmVolumeTarget = volume;
	BgmVolume = volume;
}

/* Unused: fades the music toward `tgtVolume` (BGM_UNITY_SCALE full) by
   `step` a frame. Only a bare stub, which no code names, leads here. */
void sySndSetBGMVolumeTarget(Uint16 tgtVolume, Uint16 step)
{
	BgmVolumeTarget = tgtVolume;
	BgmVolumeStep = step;
}

/* Returns nonzero while a jingle holds the music: from its start until its
   player falls silent and the music begins to fade back in. */
Sint32 sySndGetBGMRestoreDelay(void)
{
	return BgmRestoreDelay;
}

/* Unused: stops every music player where it is, for sySndResumeAllMusicPlayers
   to go on from. Only a bare stub, which no code names, leads here. */
void sySndStopAllMusicPlayers(void)
{
	m4aMPlayAllStop();
}

/* Unused: resumes every music player from where sySndStopAllMusicPlayers
   stopped it. Only a bare stub, which no code names, leads here. */
void sySndResumeAllMusicPlayers(void)
{
	m4aMPlayAllContinue();
}

/* Unused: sets SoundCommandMaskWork, or with SOUND_COMMAND_MASK_TOGGLE flips
   the bits given; nothing reads the mask. Only a bare stub, which no code
   names, leads here. */
void sySndUpdateCommandMask(Uint32 command)
{
	Uint32 toggle = command & SOUND_COMMAND_MASK_TOGGLE;

	command &= SOUND_COMMAND_MASK_BITS;
	if(toggle != 0)
		SoundCommandMaskWork ^= command;
	else
		SoundCommandMaskWork = command;
}

/* Returns the music sySndPlay last chose: BGM_NONE before any, MUS_SILENCE
   after a fade. */
Sint32 sySndGetCurrentBGM(void)
{
	return CurrentBgm;
}

/* Waits until the music has begun to come back after a jingle, giving up after
   BGM_WAIT_TIMEOUT_TICKS. */
void sySndWaitForBGMRestore(void)
{
	Sint32 ticksWaited = 0;

	do {
		if(BgmRestoreDelay == 0)
			break;
		sySynch(1);
		ticksWaited++;
	} while(ticksWaited < BGM_WAIT_TIMEOUT_TICKS);
}

/* Unused: returns the reverb code sySndPlay gives music `command`:
   SOUND_REVERB_LONG for MUS_THE_BROKEN_SEAL_ENDING, MUS_MAIN_THEME_TITLE and
   MUS_TO_BE_CONTINUED, SOUND_REVERB_NORMAL for the rest. */
UNUSED Sint32 sySndGetReverbCode(Sint32 command)
{
	if(command == MUS_MAIN_THEME_TITLE || command == MUS_TO_BE_CONTINUED ||
		command == MUS_THE_BROKEN_SEAL_ENDING)
		return SOUND_REVERB_LONG;
	return SOUND_REVERB_NORMAL;
}
