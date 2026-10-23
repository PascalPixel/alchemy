#ifndef ALCHEMY_AUDIO_ENGINE_SYMBOLS_H
#define ALCHEMY_AUDIO_ENGINE_SYMBOLS_H

/// @file
/// The sound driver's command masks, cue controls and the routines and globals
/// it shares with the engine.

#include "types.h"

/// Sound_UpdateCommandMask's command: the new gSoundCommandMask, or
/// with SOUND_COMMAND_MASK_TOGGLE the bits to flip in it.
enum AudioCommandMask {
  SOUND_COMMAND_MASK_TOGGLE = 0x80,
  SOUND_COMMAND_MASK_BITS = 0x7f
};

/// The sound driver's mode at start (Audio_Initialize, AudioEngine_SetMode):
/// 8 PCM notes (bits 11..8), volume 15 (15..12), frame length 7 (19..16),
/// 352 samples a frame or 21024 a second, and sound bias resolution 1 (from
/// bits 23..20). Its reverb field, 0, leaves the reverb as it is.
#define AUDIO_INITIAL_MODE 0x0097F800

/// A cue for Sound_PlaySong: bits 11..0 the cue, 15..12 flags. The sound
/// README's "What a cue number means" gives the ranges.
enum SongControl {
  SONG_NUM_MASK = 0xfff,
  SONG_FLAGS_MASK = 0xf000,
  SONG_FLAG_FADE_IN = 0x1000, ///< music fades in over 64 frames
  SONG_FADE_OUT_BGM = 17,     ///< fades the music out over 112 frames, once
  SONG_KEEP_BGM = 18,         ///< keeps the music that is playing
  /// Fades player 3 out over 48 frames; also the number of the song table's
  /// row 289, so that row never plays.
  SONG_FADE_OUT_ACTION_SE = 0x121,
  /// Cues from here to SONG_SE_FIRST are fanfares; those below are music,
  /// but for SONG_FADE_OUT_BGM and SONG_KEEP_BGM.
  SONG_FANFARE_FIRST = 80,
  /// Cues from here but SONG_FADE_OUT_ACTION_SE each play on the player
  /// gSound_SongTable names for them.
  SONG_SE_FIRST = 100
};

/// The players by their place in gSound_PlayerSlots (sound/players.s).
enum SoundPlayerId {
  SOUND_PLAYER_FANFARE = 0, ///< gMusicPlayerFanfare
  SOUND_PLAYER_MUSIC = 1,   ///< gMusicPlayerBgm
  /// The first player a song on SOUND_PLAYER_EFFECT_LAST may borrow.
  SOUND_PLAYER_EFFECT_FIRST = 4,
  /// A song on it takes the first idle player from here down to
  /// SOUND_PLAYER_EFFECT_FIRST, else this one (Sound_PlaySong).
  SOUND_PLAYER_EFFECT_LAST = 7
};

s32 Sound_LoadPresetParameters(s32 preset);

void Bios_CpuSet(const void *source, void *destination, u32 control);

struct SoundPlayer;
struct LzPackWork;
/// A flash library cell the LZ packer borrows as its work pointer (lzpack.h);
/// IdentifyFlash sets it again before the flash code reads it.
extern struct LzPackWork *gFlash_Handler3;
/// Plays music; cue 17 fades it and a fanfare silences it until it ends.
extern struct SoundPlayer gMusicPlayerBgm;
/// Plays fanfares; the music returns once it stops.
extern struct SoundPlayer gMusicPlayerFanfare;
extern struct SoundPlayer
    gSound_Player3; ///< the player SONG_FADE_OUT_ACTION_SE fades
/// The cue from SONG_SE_FIRST up each player last started, by player, not
/// a volume: cues below it are not recorded, and SONG_FADE_OUT_ACTION_SE clears
/// entry 3. Nothing in TBS reads it.
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
/// The music cue Sound_PlaySong last chose: 0xff before any, MUS_SILENCE
/// after a fade; asking for it again changes nothing.
extern u8 gCurrentBgm;
/// Set by Sound_UpdateCommandMask; nothing in TBS reads it.
extern u8 gSoundCommandMask;

#endif
