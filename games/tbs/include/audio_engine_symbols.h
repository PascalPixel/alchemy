#ifndef ALCHEMY_AUDIO_ENGINE_SYMBOLS_H
#define ALCHEMY_AUDIO_ENGINE_SYMBOLS_H

/// @file
/// The sound driver's command masks, song controls and the routines and globals
/// it shares with the engine.

#include "types.h"

/// Sound_UpdateCommandMask's command: the new gSoundCommandMask, or
/// with SOUND_COMMAND_MASK_TOGGLE the bits to flip in it.
enum AudioCommandMask {
  SOUND_COMMAND_MASK_TOGGLE = 0x80,
  SOUND_COMMAND_MASK_BITS = 0x7f
};

/// The sound driver's mode at start (m4aSoundInit, m4aSoundMode):
/// 8 PCM notes (bits 11..8), volume 15 (15..12), rate 7 (19..16),
/// 352 samples a VBlank or 21024 a second, and sound bias resolution 1 (from
/// bits 23..20). Its reverb field, 0, leaves the reverb as it is.
#define AUDIO_INITIAL_MODE 0x0097F800

/// A song for Sound_PlaySong: bits 11..0 the song number, 15..12 flags. The
/// sound README's "What a song number means" gives the ranges.
enum SongControl {
  SONG_NUM_MASK = 0xfff,
  SONG_FLAGS_MASK = 0xf000,
  SONG_FLAG_FADE_IN = 0x1000, ///< music fades in over 64 VBlanks
  SONG_FADE_OUT_BGM = 17,     ///< fades the music out over 112 VBlanks, once
  SONG_KEEP_BGM = 18,         ///< keeps the music that is playing
  /// Fades player 3 out over 48 VBlanks; also the number of the song table's
  /// row 289, so that row never plays.
  SONG_FADE_OUT_ACTION_SE = 0x121,
  /// Songs from here to SONG_SE_FIRST are fanfares; those below are music,
  /// but for SONG_FADE_OUT_BGM and SONG_KEEP_BGM.
  SONG_FANFARE_FIRST = 80,
  /// Songs from here but SONG_FADE_OUT_ACTION_SE each play on the player
  /// gSongTable names for them.
  SONG_SE_FIRST = 100
};

/// The music players by their row in gMPlayTable (sound/music_player_table.s),
/// as a song table row names them (Song.ms).
enum MusicPlayerId {
  MUSIC_PLAYER_FANFARE = 0,   ///< gMPlayInfo_Fanfare
  MUSIC_PLAYER_BGM = 1,       ///< gMPlayInfo_BGM
  MUSIC_PLAYER_MENU_SE = 2,   ///< gMPlayInfo_MenuSE: the interface's sounds
  MUSIC_PLAYER_ACTION_SE = 3, ///< gMPlayInfo_ActionSE: field and battle actions
  /// The first music player a song on MUSIC_PLAYER_SE_LAST may borrow.
  MUSIC_PLAYER_SE_FIRST = 4,
  /// A song on it takes the first idle music player from here down to
  /// MUSIC_PLAYER_SE_FIRST, else this one (Sound_PlaySong).
  MUSIC_PLAYER_SE_LAST = 7
};

/// The five versions of the IWRAM mixer's last step Sound_LoadReverbCode
/// copies into place, each carrying a different share of a VBlank's sound into
/// the next: the song's reverb, as MusicPlayer2000 calls it. Each is the last
/// step of IwramSoundRenderVBlank's loop, which reads back what the two
/// hardware buffers held the time before, and works out what the mix buffer
/// starts the next VBlank with.
enum SoundReverbCode {
  /// One value for both channels, from the two buffers together; only the
  /// audio test loads it.
  SOUND_REVERB_MONO,
  /// Each channel's old samples crossed with the other's; only the audio test
  /// loads it.
  SOUND_REVERB_CROSSED,
  /// A quarter of the mix and an eighth of each buffer's old samples carry
  /// over: most music and the Battle Arena.
  SOUND_REVERB_NORMAL,
  /// An eighth of the mix and a quarter of the old samples carry over:
  /// MUS_TITLE, MUS_ENDING_LOGO and MUS_ENDING.
  SOUND_REVERB_LONG,
  /// Nothing carries over, the mix buffer is cleared: a link battle.
  SOUND_REVERB_NONE
};

s32 Sound_LoadReverbCode(s32 code);

void Bios_CpuSet(const void *source, void *destination, u32 control);

struct MusicPlayerInfo;
struct LzPackWork;
/// A flash library cell the LZ packer borrows as its work pointer (lzpack.h);
/// IdentifyFlash sets it again before the flash code reads it.
extern struct LzPackWork *gWaitForFlashWrite;
/// Plays music; song 17 fades it and a fanfare silences it until it ends.
extern struct MusicPlayerInfo gMPlayInfo_BGM;
/// Plays fanfares; the music returns once it stops.
extern struct MusicPlayerInfo gMPlayInfo_Fanfare;
extern struct MusicPlayerInfo
    gMPlayInfo_ActionSE; ///< the music player SONG_FADE_OUT_ACTION_SE fades
/// The song from SONG_SE_FIRST up each player last started, by player, not
/// a volume: songs below it are not recorded, and SONG_FADE_OUT_ACTION_SE
/// clears entry 3. Nothing in TBS reads it.
extern u16 gMusicPlayerLastSong[8];
/// VBlanks after a fanfare starts before the music waits for its end; held
/// at 1 until the fanfare's player stops, then 0.
extern u8 gBgmRestoreDelay;

// The music's volume and its combined tempo and pitch, each ramped toward its
// target by its step each VBlank, in 8.8 fixed point (BGM_UNITY_SCALE 1.0).
extern u16 gBgmVolume;
extern u16 gBgmVolumeTarget;
extern u16 gBgmVolumeStep;
extern u16 gBgmTempoPitch;
extern u16 gBgmTempoPitchTarget;
extern u16 gBgmTempoPitchStep;
/// The music Sound_PlaySong last chose: 0xff before any, MUS_SILENCE
/// after a fade; asking for it again changes nothing.
extern u8 gCurrentBgm;
/// Set by Sound_UpdateCommandMask; nothing in TBS reads it.
extern u8 gSoundCommandMask;

#endif
