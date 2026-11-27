#ifndef ALCHEMY_SOUND_DRIVER_SYMBOLS_H
#define ALCHEMY_SOUND_DRIVER_SYMBOLS_H

/* The sound driver's command masks, sound controls and the functions and
   globals it shares with the rest of the game. */

#include "sounds.h"
#include "types.h"

/* sySndUpdateCommandMask's command: the new SoundCommandMaskWork, or with
   SOUND_COMMAND_MASK_TOGGLE the bits to flip in it. */
enum SoundCommandMask {
	SOUND_COMMAND_MASK_TOGGLE = 0x80,
	SOUND_COMMAND_MASK_BITS = 0x7f
};

/* A sound for sySndPlay: bits 11..0 the sound number, 15..12 flags. */
enum SoundControl {
	SOUND_NUM_MASK = 0xfff,
	SOUND_FLAGS_MASK = 0xf000,
	SOUND_FLAG_FADE_IN = 0x1000, /* music fades in over 64 frames */
	/* Sounds from here to SOUND_SE_FIRST are jingles; those below are music,
	   but for SOUND_FADE_OUT_BGM and SOUND_KEEP_BGM. */
	SOUND_JINGLE_FIRST = MUS_PUZZLE_SOLVED,
	/* Sounds from here but SOUND_FADE_OUT_ACTION_SE each play on the player
	   SongTable names for them. */
	SOUND_SE_FIRST = SE_BELL
};

/* The music players by their row in MPlayTable (sound/music_player_table.s), as
   a song table row names them (Song.ms). */
enum MusicPlayerId {
	MUSIC_PLAYER_JINGLE = 0,  /* MPlayInfo_Jingle */
	MUSIC_PLAYER_BGM = 1,	  /* MPlayInfo_BGM */
	MUSIC_PLAYER_MENU_SE = 2, /* MPlayInfo_MenuSE: the interface's sounds */
	/* MPlayInfo_ActionSE: field and battle actions */
	MUSIC_PLAYER_ACTION_SE = 3,
	/* The first music player a sound on MUSIC_PLAYER_SE_LAST may borrow. */
	MUSIC_PLAYER_SE_FIRST = 4,
	/* A sound on it takes the first idle music player from here down to
	   MUSIC_PLAYER_SE_FIRST, else this one (sySndPlay). */
	MUSIC_PLAYER_SE_LAST = 7
};

/* The five versions of the IWRAM mixer's last step sySndLoadReverbCode copies
   into place, each carrying a different share of a frame's sound into the
   next: the song's reverb, as MusicPlayer2000 calls it. */
enum SoundReverbCode {
	/* One value for both channels, from the two buffers together; only the
	   sound test loads it. */
	SOUND_REVERB_MONO,
	/* Each channel's old samples crossed with the other's; only the sound test
	   loads it. */
	SOUND_REVERB_CROSSED,
	/* A quarter of the mix and an eighth of each buffer's old samples carry
	   over: most music and the Battle Arena. */
	SOUND_REVERB_NORMAL,
	/* An eighth of the mix and a quarter of the old samples carry over:
	   MUS_MAIN_THEME_TITLE, MUS_TO_BE_CONTINUED and
	   MUS_THE_BROKEN_SEAL_ENDING. */
	SOUND_REVERB_LONG,
	/* Nothing carries over, the mix buffer is cleared: a link battle. */
	SOUND_REVERB_NONE
};

struct MusicPlayerInfo;
/* Plays music; SOUND_FADE_OUT_BGM fades it and a jingle silences it until it
   ends. */
extern struct MusicPlayerInfo MPlayInfo_BGM;
/* Plays jingles; the music returns once it stops. */
extern struct MusicPlayerInfo MPlayInfo_Jingle;
/* the music player SOUND_FADE_OUT_ACTION_SE fades */
extern struct MusicPlayerInfo MPlayInfo_ActionSE;
/* The sound effect each music player last started, by player: music and jingles
   are not recorded, and SOUND_FADE_OUT_ACTION_SE clears
   MUSIC_PLAYER_ACTION_SE's entry. Nothing reads it. */
extern Uint16 MusicPlayerLastSound[8];
/* Frames after a jingle starts before the music waits for its end; held at 1
   until the jingle's player stops, then 0. */
extern Uint8 BgmRestoreDelay;

/* The music's volume and its combined tempo and pitch, each ramped toward its
   target by its step each VBlank, in 8.8 fixed point (BGM_UNITY_SCALE 1.0). */
extern Uint16 BgmVolume;
extern Uint16 BgmVolumeTarget;
extern Uint16 BgmVolumeStep;
extern Uint16 BgmTempoPitch;
extern Uint16 BgmTempoPitchTarget;
extern Uint16 BgmTempoPitchStep;
/* The music sySndPlay last chose: 0xff before any, MUS_SILENCE after a fade;
   asking for it again changes nothing. */
extern Uint8 CurrentBgm;
/* Set by sySndUpdateCommandMask; nothing reads it. */
extern Uint8 SoundCommandMaskWork;

#endif
