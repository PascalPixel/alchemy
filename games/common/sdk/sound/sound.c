/* The sound driver's entry points: its start-up (m4aSoundInit, MPlayExtender),
   the work it does each VBlank, PCM pitch, and the commands that start, stop,
   resume and fade a music player's song. */

#include "types.h"
#include "sdk/sound_driver.h"
#include "system/definitions.h"

/* The keys ScaleTable codes, 0 to SCALE_TABLE_KEYS - 1, and the largest pitch,
   in 1/256 of a semitone. MidiKeyToFreq moves a pitch to a word's top byte
   (PITCH_FRACTION_SHIFT), the fraction of the way to the next key that
   umul3232H32 takes. */
#define SCALE_TABLE_KEYS	 180
#define PITCH_MAX			 255
#define PITCH_FRACTION_SHIFT 24

/* The rate a PCM sample plays at for a key and a fine pitch: the sample's own
   rate times the key's step, found between the key's and the next key's. A key
   past the table's second-to-last plays as nearly its last, the second-to-last
   raised by PITCH_MAX. */
s32 MidiKeyToFreq(struct WaveData *wave, u8 key, u8 pitch)
{
	u32 keyIndex;
	u32 pitchScale;
	u32 lowerFrequency;
	u32 upperFrequency;

	keyIndex = key;
	pitchScale = (u32)pitch << PITCH_FRACTION_SHIFT;
	if(keyIndex > SCALE_TABLE_KEYS - 2) {
		keyIndex = SCALE_TABLE_KEYS - 2;
		pitchScale = (u32)PITCH_MAX << PITCH_FRACTION_SHIFT;
	}

	/* A key's code: the low four bits pick the semitone's step (2^31 times 2^(n
	   / 12)), the high four bits the octaves to shift it down, 9 for key 60. */
	lowerFrequency = ScaleTable[keyIndex];
	lowerFrequency = FreqTable[lowerFrequency & 0xf] >> (lowerFrequency >> 4);
	upperFrequency = ScaleTable[keyIndex + 1];
	upperFrequency = FreqTable[upperFrequency & 0xf] >> (upperFrequency >> 4);

	return umul3232H32(wave->freq,
		lowerFrequency +
			umul3232H32(upperFrequency - lowerFrequency, pitchScale));
}

/* Unused: does nothing. The driver's empty callback is DummyFunc
   (empty_callback.c). */
UNUSED void UnusedDummyFunc(void)
{
}

/* The music player count and the mixer's line limit, as the music player table
   supplies them. */
#define NUM_MUSIC_PLAYERS ((u16)(u32)NumMusicPlayers)
#define MAX_LINES		  ((u32)MaxLines)

/* Clears the music player's paused flag, raising its ident while it works. */
void MPlayContinue(struct MusicPlayerInfo *player)
{
	if(player->ident == ID_NUMBER) {
		player->ident++;
		player->status &= ~MUSICPLAYER_STATUS_PAUSE;
		player->ident = ID_NUMBER;
	}
}

/* Starts a fade with the music player's ident raised, resuming the music player
   for fade-ins: FadeOutBody moves the level 16 of 256 every `period` frames,
   so a whole fade takes 16 * period frames. */
static __inline__ void MPlayStartFade(
	struct MusicPlayerInfo *player, u16 period, u16 volume)
{
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

/* Fades a player's song out over 16 * `period` frames, then stops it for
   good. */
void MPlayFadeOut(struct MusicPlayerInfo *player, u16 period)
{
	MPlayStartFade(player, period, SOUND_FADE_FULL_VOLUME);
}

/* The bytes of mixer code m4aSoundInit copies to IWRAM. */
#define SOUND_MIXER_CODE_SIZE 1024

/* Copies the RAM mixer to IWRAM, initializes the PCM and CGB paths and
   configures every music player from MPlayTable. */
void m4aSoundInit(void)
{
	s32 playerIndex;

	/* sySoundMainRAMRom is a Thumb symbol: its address has bit 0 set */
	BiosCpuSet((void *)((u32)sySoundMainRAMRom & ~1), RamSoundMixer,
		CPU_SET_32BIT | (SOUND_MIXER_CODE_SIZE / 4));
	SoundInit(&SoundInfoWork);
	MPlayExtender(CgbChans);
	m4aSoundMode(SOUND_GAME_MODE);
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
void m4aSoundMain(void)
{
	SoundMain();
}

/* Starts the song on the music player its song-table entry names. */
void m4aSongNumStart(u16 songId)
{
	const struct MusicPlayer *mplayTable = MPlayTable;
	const struct Song		 *songTable = SongTable;
	const struct Song		 *song = &songTable[songId];

	MPlayStart(mplayTable[song->ms].info, song->header);
}

/* Unused: starts the song unless its music player is already playing it. */
UNUSED void m4aSongNumStartOrChange(u16 songId)
{
	const struct MusicPlayer *mplayTable = MPlayTable;
	const struct Song		 *songTable = SongTable;
	const struct Song		 *song = &songTable[songId];
	struct MusicPlayerInfo	 *player = mplayTable[song->ms].info;

	if(player->header != song->header)
		MPlayStart(player, song->header);
	else if((player->status & MUSICPLAYER_STATUS_TRACK) == 0 ||
		(player->status & MUSICPLAYER_STATUS_PAUSE))
		MPlayStart(player, song->header);
}

/* Unused: starts a different or finished song, or resumes the same paused
   song. */
UNUSED void m4aSongNumStartOrContinue(u16 songId)
{
	const struct MusicPlayer *mplayTable = MPlayTable;
	const struct Song		 *songTable = SongTable;
	const struct Song		 *song = &songTable[songId];
	struct MusicPlayerInfo	 *player = mplayTable[song->ms].info;

	if(player->header != song->header)
		MPlayStart(player, song->header);
	else if((player->status & MUSICPLAYER_STATUS_TRACK) == 0)
		MPlayStart(player, song->header);
	else if(player->status & MUSICPLAYER_STATUS_PAUSE)
		MPlayContinue(player);
}

/* Unused: stops the song's music player only when it holds this song. */
UNUSED void m4aSongNumStop(u16 songId)
{
	const struct MusicPlayer *mplayTable = MPlayTable;
	const struct Song		 *songTable = SongTable;
	const struct Song		 *song = &songTable[songId];
	struct MusicPlayerInfo	 *player = mplayTable[song->ms].info;

	if(player->header == song->header)
		m4aMPlayStop(player);
}

/* Unused: resumes the song's music player only when it holds this song. */
UNUSED void m4aSongNumContinue(u16 songId)
{
	const struct MusicPlayer *mplayTable = MPlayTable;
	const struct Song		 *songTable = SongTable;
	const struct Song		 *song = &songTable[songId];
	struct MusicPlayerInfo	 *player = mplayTable[song->ms].info;

	if(player->header == song->header)
		MPlayContinue(player);
}

/* Unused: stops every music player. Only sySoundStopAllMusicPlayers calls it,
   and only a bare stub, which no code in the game names, leads there. */
UNUSED void m4aMPlayAllStop(void)
{
	s32 playerIndex;

	for(playerIndex = 0; playerIndex < NUM_MUSIC_PLAYERS; playerIndex++)
		m4aMPlayStop(MPlayTable[playerIndex].info);
}

/* Unused: resumes `player`. */
UNUSED void m4aMPlayContinue(struct MusicPlayerInfo *player)
{
	MPlayContinue(player);
}

/* Unused: resumes every music player. Only sySoundResumeAllMusicPlayers calls
   it, and only a bare stub, which no code in the game names, leads there. */
UNUSED void m4aMPlayAllContinue(void)
{
	s32 playerIndex;

	for(playerIndex = 0; playerIndex < NUM_MUSIC_PLAYERS; playerIndex++)
		MPlayContinue(MPlayTable[playerIndex].info);
}

/* Fades a player's song out over 16 * `period` frames, then stops it: how
   sySoundPlay's fade controls, such as SOUND_FADE_OUT_BGM, end their sounds. */
void m4aMPlayFadeOut(struct MusicPlayerInfo *player, u16 period)
{
	MPlayFadeOut(player, period);
}

/* Unused: begins a fade that pauses the music player at zero volume, keeping
   its tracks' state for a later resume. */
UNUSED void m4aMPlayFadeOutTemporarily(
	struct MusicPlayerInfo *player, u16 period)
{
	MPlayStartFade(player, period, SOUND_FADE_FULL_VOLUME | TEMPORARY_FADE);
}

/* Unused: starts a fade from zero volume and resumes the music player. */
UNUSED void m4aMPlayFadeIn(struct MusicPlayerInfo *player, u16 period)
{
	MPlayStartFade(player, period, FADE_IN);
}

/* The bend range and LFO speed a track starts with, as MPlayMain gives a track
   at its start: a full bend reaches two semitones, and the LFO's phase steps 22
   a tick. */
enum { TRACK_START_BEND_RANGE = 2, TRACK_START_LFO_SPEED = 22 };

/* Unused: initializes active tracks still carrying their reset flag: clears
   their settings and gives them the bend range, full volume, LFO speed and
   voice kind a track starts with. */
UNUSED void m4aMPlayImmInit(struct MusicPlayerInfo *player)
{
	s32						 count = player->trackCount;
	struct MusicPlayerTrack *track = player->tracks;

	while(count > 0) {
		if(track->flags & MPT_FLG_EXIST) {
			if(track->flags & MPT_FLG_START) {
				/* Clear the 64-byte settings prefix, retaining the sequence
				   cursor. */
				Clear64byte(track);
				track->flags = MPT_FLG_EXIST;
				track->bendRange = TRACK_START_BEND_RANGE;
				track->volumeScale = SOUND_FADE_FULL_VOLUME / 4;
				track->lfoSpeed = TRACK_START_LFO_SPEED;
				track->voice.kind = CGB_CHANNEL_SWEEP_SQUARE;
			}
		}
		count--;
		track++;
	}
}

/* Resets the CGB sound hardware, installs the CGB-aware command handlers and
   callbacks in the sound work, and clears the four channels' `notes`. */
void MPlayExtender(struct CgbChannel *notes)
{
	struct SoundInfo *work;
	u32				  ident;

	REG_SOUNDCNT_X = SOUNDCNT_X_ENABLE | SOUNDCNT_X_ALL_TONES_ON;
	REG_SOUNDCNT_L = 0;
	/* The square and noise channels silent with a rising envelope, restarted;
	   the wave channel off; the master volume full on both sides */
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

		BiosCpuSet((void *)&zero, notes,
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
