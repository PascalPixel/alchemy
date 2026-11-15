#ifndef ALCHEMY_MODULE_SOUND_SOUND_H
#define ALCHEMY_MODULE_SOUND_SOUND_H

/// @file
/// What games/tla/src/sound/ declares for the files that use it, written once.

#include "types.h"
#include "sound/sound_driver_types.h"

/// The three sounds the sound test's selectors start on.
struct SoundTestCues {
  s32 cue[3];
};

void Sound_WaitForBgmRestore(void);

extern u8 gData_02003a90[];

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
/// SOUND_FADE_OUT_ACTION_SE and SOUND_FADE_OUT_SE5 clear the entries they fade.
extern u16 gMusicPlayerLastSound[MUSIC_PLAYER_COUNT];

void Sound_SetFixedSampleFreq(u32 mode);

extern struct MusicPlayerInfo gMPlayInfo_ActionSE;
extern struct MusicPlayerInfo gMPlayInfo_SE5;
/// Plays jingles; the music returns once it stops.
extern struct MusicPlayerInfo gMPlayInfo_Jingle;

void SoundFlag_Set(s32 sound);

#endif
