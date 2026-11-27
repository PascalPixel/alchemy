/* @file */
/* The sound driver's entry points: its start-up (m4aSoundInit, */
/* MPlayExtender), the work it does each VBlank, PCM pitch, and the commands */
/* that start, stop, resume and fade a music player's song. */

#include "types.h"
#include "sound/sound.h"
#include "sdk/sound_driver.h"
#include "system/globals.h"

/* The PCM keys ScaleTable has a code for, 0 to 179. */
#define SCALE_KEY_COUNT 180
/* Moves a pitch in 1/256 semitone to the top byte of a 32-bit fraction, the */
/* share of the step to the next key that umul3232H32 takes. */
#define PITCH_FRACTION_SHIFT 24

/* Interpolates adjacent key frequencies and scales them by the wave sample */
/* frequency. */
s32 MidiKeyToFreq(struct WaveData *wave, u8 key, u8 pitch) {
	u32 keyIndex;
	u32 pitchScale;
	u32 lowerFrequency;
	u32 upperFrequency;

	/* The highest key plays as the one below it at the finest pitch under the
	 */
	/* next, so that a key above it is always in the table. */
	keyIndex = key;
	pitchScale = (u32)pitch << PITCH_FRACTION_SHIFT;
	if(keyIndex > SCALE_KEY_COUNT - 2) {
		keyIndex = SCALE_KEY_COUNT - 2;
		pitchScale = (SOUND_PITCH_SEMITONE - 1U) << PITCH_FRACTION_SHIFT;
	}

	/* A key's code: the low four bits pick the semitone's step (2^31 times */
	/* 2^(n / 12)), the high four bits the octaves to shift it down, 9 for
	 * key 60. */
	lowerFrequency = ScaleTable[keyIndex];
	lowerFrequency = FreqTable[lowerFrequency & 0xf] >> (lowerFrequency >> 4);
	upperFrequency = ScaleTable[keyIndex + 1];
	upperFrequency = FreqTable[upperFrequency & 0xf] >> (upperFrequency >> 4);

	return umul3232H32(
		wave->freq,
		lowerFrequency +
			umul3232H32(upperFrequency - lowerFrequency, pitchScale));
}

/* Does nothing; nothing in the game refers to it. The driver's empty callback
 */
/* is DummyFunc (empty_callback.c). */
void UnusedDummyFunc(void) {}

/* The music player count and the mixer's line limit, as the music player */
/* table supplies them. */
#define NUM_MUSIC_PLAYERS ((u16)(u32)NumMusicPlayers)
#define MAX_LINES		  ((u32)MaxLines)

/* Clears the music player's paused flag, raising its ident while it works. */
void MPlayContinue(struct MusicPlayerInfo *player) {
	if(player->ident == ID_NUMBER) {
		player->ident++;
		player->status &= ~MUSICPLAYER_STATUS_PAUSE;
		player->ident = ID_NUMBER;
	}
}

/* Starts a fade with the music player's ident raised, resuming the music */
/* player for fade-ins: FadeOutBody moves the level 16 of 256 every `period` */
/* VBlanks, so a whole fade takes 16 * period VBlanks. */
static __inline__ void MPlayStartFade(struct MusicPlayerInfo *player,
									  u16 period, u16 volume) {
	if(player->ident == ID_NUMBER) {
		player->ident++;
		player->fadeCounter = period;
		player->fadePeriod = period;
		player->fadeVolume = volume;
		if(volume & FADE_IN)
			player->status &= ~MUSICPLAYER_STATUS_PAUSE;
		player->ident = ID_NUMBER;
	}
}

/* Begins a fade that stops and clears tracks when its volume reaches zero. */
/* Called by m4aMPlayFadeOut. */
void MPlayFadeOut(struct MusicPlayerInfo *player, u16 speed) {
	MPlayStartFade(player, speed, SOUND_FADE_FULL_VOLUME);
}

/* The bytes of mixer code m4aSoundInit copies to IWRAM. */
#define SOUND_MIXER_CODE_SIZE 1024

/* Copies the RAM mixer to IWRAM, initializes the PCM and CGB paths and */
/* configures every music player from MPlayTable. */
void m4aSoundInit(void) {
	s32 playerIndex;

	/* sySndMainRAMRom is a Thumb symbol: its address has bit 0 set */
	Bios_CpuSet((void *)((u32)sySndMainRAMRom & ~1), Ram_SoundMixer,
				CPU_SET_32BIT | (SOUND_MIXER_CODE_SIZE / 4));
	SoundInit(&SoundInfoWork);
	MPlayExtender(CgbChans);
	m4aSoundMode(AUDIO_INITIAL_MODE);
	/* Each music player owns its tracks but shares the memacc area. */
	for(playerIndex = 0; playerIndex < NUM_MUSIC_PLAYERS; playerIndex++) {
		struct MusicPlayerInfo *player = MPlayTable[playerIndex].info;

		MPlayOpen(player, MPlayTable[playerIndex].track,
				  MPlayTable[playerIndex].numTracks);
		player->checkPriority = MPlayTable[playerIndex].checkPriority;
		player->workBytes = MPlayMemAccArea;
	}
}

/* Runs the sound driver for one VBlank through SoundMain. */
void m4aSoundMain(void) { SoundMain(); }

/* Starts the song on the music player its song-table entry names. */
void m4aSongNumStart(u16 songId) {
	const struct MusicPlayer *mplayTable = MPlayTable;
	const struct Song *songTable = SongTable;
	const struct Song *song = &songTable[songId];

	MPlayStart(mplayTable[song->ms].info, song->header);
}

/* Unused: starts the song unless its music player is already playing it. */
UNUSED void m4aSongNumStartOrChange(u16 songId) {
	const struct MusicPlayer *mplayTable = MPlayTable;
	const struct Song *songTable = SongTable;
	const struct Song *song = &songTable[songId];
	struct MusicPlayerInfo *player = mplayTable[song->ms].info;

	if(player->header != song->header)
		MPlayStart(player, song->header);
	else if((player->status & MUSICPLAYER_STATUS_TRACK) == 0 ||
			(player->status & MUSICPLAYER_STATUS_PAUSE))
		MPlayStart(player, song->header);
}

/* Unused: starts a different or finished song, or resumes the same paused */
/* song. */
UNUSED void m4aSongNumStartOrContinue(u16 songId) {
	const struct MusicPlayer *mplayTable = MPlayTable;
	const struct Song *songTable = SongTable;
	const struct Song *song = &songTable[songId];
	struct MusicPlayerInfo *player = mplayTable[song->ms].info;

	if(player->header != song->header)
		MPlayStart(player, song->header);
	else if((player->status & MUSICPLAYER_STATUS_TRACK) == 0)
		MPlayStart(player, song->header);
	else if(player->status & MUSICPLAYER_STATUS_PAUSE)
		MPlayContinue(player);
}

/* Unused: stops the song's music player only when it holds this song. */
UNUSED void m4aSongNumStop(u16 songId) {
	const struct MusicPlayer *mplayTable = MPlayTable;
	const struct Song *songTable = SongTable;
	const struct Song *song = &songTable[songId];
	struct MusicPlayerInfo *player = mplayTable[song->ms].info;

	if(player->header == song->header)
		m4aMPlayStop(player);
}

/* Unused: resumes the song's music player only when it holds this song. */
UNUSED void m4aSongNumContinue(u16 songId) {
	const struct MusicPlayer *mplayTable = MPlayTable;
	const struct Song *songTable = SongTable;
	const struct Song *song = &songTable[songId];
	struct MusicPlayerInfo *player = mplayTable[song->ms].info;

	if(player->header == song->header)
		MPlayContinue(player);
}

/* Unused: stops every music player. Only sySndStopAllMusicPlayers calls it, */
/* and only a bare stub, which no code in the game names, leads */
/* there. */
UNUSED void m4aMPlayAllStop(void) {
	s32 playerIndex;

	for(playerIndex = 0; playerIndex < NUM_MUSIC_PLAYERS; playerIndex++)
		m4aMPlayStop(MPlayTable[playerIndex].info);
}

/* Unused: resumes `player`. */
UNUSED void m4aMPlayContinue(struct MusicPlayerInfo *player) {
	MPlayContinue(player);
}

/* Unused: resumes every music player. Only sySndResumeAllMusicPlayers calls */
/* it, and only a bare stub, which no code in the game names, leads */
/* there. */
UNUSED void m4aMPlayAllContinue(void) {
	s32 playerIndex;

	for(playerIndex = 0; playerIndex < NUM_MUSIC_PLAYERS; playerIndex++)
		MPlayContinue(MPlayTable[playerIndex].info);
}

/* Begins the standard stop-and-clear fade. */
void m4aMPlayFadeOut(struct MusicPlayerInfo *player, u16 speed) {
	MPlayFadeOut(player, speed);
}

/* Unused: begins a fade that pauses the music player at zero volume, keeping */
/* its tracks' state for a later resume. */
UNUSED void m4aMPlayFadeOutTemporarily(struct MusicPlayerInfo *player,
									   u16 speed) {
	MPlayStartFade(player, speed, SOUND_FADE_FULL_VOLUME | TEMPORARY_FADE);
}

/* Unused: starts a fade from zero volume and resumes the music player. */
UNUSED void m4aMPlayFadeIn(struct MusicPlayerInfo *player, u16 speed) {
	MPlayStartFade(player, speed, FADE_IN);
}

/* The LFO speed a track starts with: its phase steps 22 of 256 a tick. */
#define LFO_SPEED_RESET 22

/* Unused: initializes active tracks still carrying their reset flag: clears */
/* their settings and gives them the default bend range, volume and LFO speed.
 */
UNUSED void m4aMPlayImmInit(struct MusicPlayerInfo *player) {
	s32 count = player->trackCount;
	struct MusicPlayerTrack *track = player->tracks;

	while(count > 0) {
		if(track->flags & MPT_FLG_EXIST) {
			if(track->flags & MPT_FLG_START) {
				/* Clear the 64-byte settings prefix, retaining the sequence
				 * cursor. */
				Clear64byte(track);
				track->flags = MPT_FLG_EXIST;
				track->bendRange = 2;
				track->volumeScale = SOUND_FADE_FULL_VOLUME / 4;
				track->lfoSpeed = LFO_SPEED_RESET;
				track->voice.kind = CGB_CHANNEL_SWEEP_SQUARE;
			}
		}
		count--;
		track++;
	}
}

/* Resets the CGB sound hardware, installs the CGB-aware command handlers and */
/* callbacks in the sound work, and clears the four channels' `notes`. */
void MPlayExtender(struct CgbChannel *notes) {
	struct SoundInfo *work;
	u32 ident;

	REG_SOUNDCNT_X = SOUNDCNT_X_ENABLE | SOUNDCNT_X_ALL_TONES_ON;
	REG_SOUNDCNT_L = 0;
	/* The square and noise channels silent with a rising envelope, restarted;
	 */
	/* the wave channel off; the master volume full on both sides */
	REG_NR12 = NRX2_ENVELOPE_UP;
	REG_NR22 = NRX2_ENVELOPE_UP;
	REG_NR42 = NRX2_ENVELOPE_UP;
	REG_NR14 = NRX4_RESTART;
	REG_NR24 = NRX4_RESTART;
	REG_NR44 = NRX4_RESTART;
	REG_NR30 = 0;
	REG_NR50 = NR50_VOLUME_FULL;

	work = SOUND_INFO_PTR;
	ident = work->ident;
	if(ident != ID_NUMBER)
		return;
	work->ident = ident + 1;

	/* Replace generic commands with the CGB-capable handlers. */
	MPlayJumpTable[MPLAY_JUMP_MEMACC] = ply_memacc;
	MPlayJumpTable[MPLAY_JUMP_LFOS] = ply_lfos;
	MPlayJumpTable[MPLAY_JUMP_MOD] = ply_mod;
	MPlayJumpTable[MPLAY_JUMP_XCMD] = ply_xcmd;
	MPlayJumpTable[MPLAY_JUMP_EOT] = ply_endtie;
	MPlayJumpTable[MPLAY_JUMP_SAMPLE_FREQ_SET] =
		MPLAY_FUNC_CAST(MPlayFunc, SampleFreqSet);
	MPlayJumpTable[MPLAY_JUMP_TRACK_STOP] = TrackStop;
	MPlayJumpTable[MPLAY_JUMP_FADE_OUT_BODY] =
		MPLAY_FUNC_CAST(MPlayFunc, FadeOutBody);
	MPlayJumpTable[MPLAY_JUMP_TRK_VOL_PIT_SET] = TrkVolPitSet;

	work->cgbNotes = notes;
	work->cgbUpdate = CgbSound;
	work->cgbMute = CgbOscOff;
	work->cgbFrequency = MidiKeyToCgbFreq;
	work->lineLimit = MAX_LINES;

	/* Clear note state, then give each channel its NR51 bits. */
	{
		u32 zero = 0;

		Bios_CpuSet(
			(void *)&zero, notes,
			CPU_SET_SRC_FIXED | CPU_SET_32BIT |
				(sizeof(struct CgbChannel) * SOUND_CGB_CHANNEL_COUNT / 4));
	}
	notes[0].kind = CGB_CHANNEL_SWEEP_SQUARE;
	notes[0].channelBits = NR51_CHANNEL_BOTH(CGB_CHANNEL_SWEEP_SQUARE);
	notes[1].kind = CGB_CHANNEL_SQUARE;
	notes[1].channelBits = NR51_CHANNEL_BOTH(CGB_CHANNEL_SQUARE);
	notes[2].kind = CGB_CHANNEL_WAVE;
	notes[2].channelBits = NR51_CHANNEL_BOTH(CGB_CHANNEL_WAVE);
	notes[3].kind = CGB_CHANNEL_NOISE;
	notes[3].channelBits = NR51_CHANNEL_BOTH(CGB_CHANNEL_NOISE);

	work->ident = ident;
}
