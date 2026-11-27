#ifndef ALCHEMY_SOUND_DRIVER_SYMBOLS_H
#define ALCHEMY_SOUND_DRIVER_SYMBOLS_H

/* What the game sets over the sound driver: the music's player, the sound
   numbers sySndPlay takes and their ranges, the music players by row, the
   mixer's reverb codes, and the music's volume and tempo ramps. */

#include "sounds.h"
#include "system.h"
#include "types.h"

struct MusicPlayerInfo;

/* sySndUpdateCommandMask's command: the new SoundCommandMask, or with
   AUDIO_COMMAND_TOGGLE the bits to flip in it. Nothing sends one. */
enum AudioCommandMask {
	AUDIO_COMMAND_TOGGLE = 0x80,
	AUDIO_COMMAND_MASK = 0x7f
};

/* Rate 9, 31536 samples a second, as the rate bits of a sound mode word
   (SOUND_MODE_FREQ): the rate sySndInit fixes for the game (pret's
   SOUND_MODE_FREQ_31536). */
#define SOUND_MODE_FREQ_GAME 0x00090000

/* A sound for sySndPlay: bits 11..0 the sound number, 15..12 flags.
   SOUND_FADE_OUT_BGM, SOUND_KEEP_BGM, SOUND_FADE_OUT_ACTION_SE and
   SOUND_FADE_OUT_EXIT_SE are rows of the song table, in sounds.h. */
enum SoundControl {
	SOUND_NUM_MASK = 0xfff,
	SOUND_FLAGS_MASK = 0xf000,
	SOUND_FLAG_FADE_IN = 0x1000, /* music fades in over 64 frames */
	/* Sounds from here to SOUND_SE_FIRST are jingles; those below are music,
	   but for SOUND_FADE_OUT_BGM and SOUND_KEEP_BGM. */
	SOUND_JINGLE_FIRST = MUS_PUZZLE_SOLVED,
	/* Sounds from here to SOUND_SE_END, but for the two fades, each play on the
	   player SongTable names for them. */
	SOUND_SE_FIRST = SE_BELL,
	SOUND_SE_END = MUS_VILLAGE_THEME /* sounds from here on are music again */
};

/* The music players by their row in MPlayTable (sound/music_player_table.s), as
   a song table row names them (Song.slot). */
enum MusicPlayerId {
	MUSIC_PLAYER_JINGLE = 0,	/* MPlayInfo_Jingle */
	MUSIC_PLAYER_BGM = 1,		/* MPlayInfo_BGM */
	MUSIC_PLAYER_MENU_SE = 2,	/* MPlayInfo_MenuSE */
	MUSIC_PLAYER_ACTION_SE = 3, /* MPlayInfo_ActionSE */
	/* The lowest music player a sound on MUSIC_PLAYER_SE_LAST may borrow. */
	MUSIC_PLAYER_SE_FIRST = 4,
	MUSIC_PLAYER_SE5 = 5, /* MPlayInfo_SE5 */
	/* A sound on it takes the first idle music player from here down to
	   MUSIC_PLAYER_SE_FIRST, else this one (sySndPlay). */
	MUSIC_PLAYER_SE_LAST = 7
};

/* The mixer's reverb codes. sySndLoadReverbCode loads none of them, and
   sySndGetReverbCode gives every music SOUND_REVERB_LONG. */
enum SoundReverbCode {
	SOUND_REVERB_MONO,
	SOUND_REVERB_CROSSED,
	SOUND_REVERB_NORMAL,
	SOUND_REVERB_LONG,
	SOUND_REVERB_NONE
};

/* How the tick wait and the VBlank handler share the sound driver's update, so
   that it runs once a VBlank, from whichever reaches it first. */
struct SoundVBlankState {
	/* The low byte of FrameCount when the update last ran. */
	Uint8 lastFrame;
	/* SoundInfo.transferCountdown as the update last left it: the VBlank
	   handler copies it, the tick wait counts it a frame ahead. */
	Uint8 transferCountdown;
	/* Set while the sound test runs: the tick wait then leaves the update to
	   the VBlank handler. */
	Uint8 soundTestActive;
	/* Set while the tick wait runs the update: the VBlank handler skips its
	   own, and sySndPlay ignores every sound. */
	Uint8 updating;
};

#endif
