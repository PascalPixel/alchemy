#ifndef ALCHEMY_AUDIO_ENGINE_SYMBOLS_H
#define ALCHEMY_AUDIO_ENGINE_SYMBOLS_H

/// @file
/// The sound driver's command masks, cue controls and the routines and globals
/// it shares with the engine.

#include "types.h"

/// AudioCommand_UpdateToggleMask's command: the new gAudio_CommandMask, or
/// with AUDIO_COMMAND_TOGGLE the bits to flip in it.
enum AudioCommandMask {
  AUDIO_COMMAND_TOGGLE = 0x80,
  AUDIO_COMMAND_MASK = 0x7f
};

/// The sound driver's mode at start (Audio_Initialize, AudioEngine_SetMode):
/// 8 PCM notes (bits 11..8), volume 15 (15..12), frame length 7 (19..16),
/// 352 samples a frame or 21024 a second, and sound bias resolution 1 (from
/// bits 23..20). Its reverb field, 0, leaves the reverb as it is.
#define AUDIO_INITIAL_MODE 0x0097F800

/// A cue for Sound_PlaySong: bits 11..0 the cue, 15..12 flags. The sound
/// README's "What a cue number means" gives the ranges.
enum AudioCueControl {
  AUDIO_CUE_ID_MASK = 0xfff,
  AUDIO_CUE_FLAGS_MASK = 0xf000,
  AUDIO_CUE_START_SILENT = 0x1000, ///< music fades in over 64 frames
  AUDIO_CUE_FADE_BGM = 17, ///< fades the music out over 112 frames, once
  AUDIO_CUE_KEEP_BGM = 18, ///< keeps the music that is playing
  /// Fades player 3 out over 48 frames; also the number of the song table's
  /// row 289, so that row never plays.
  AUDIO_CUE_FADE_SLOT3 = 0x121,
  /// Cues from here to AUDIO_SONG_FIRST are fanfares; those below are music,
  /// but for AUDIO_CUE_FADE_BGM and AUDIO_CUE_KEEP_BGM.
  AUDIO_JINGLE_FIRST = 80,
  /// Cues from here but AUDIO_CUE_FADE_SLOT3 each play on the player
  /// gSound_SongTable names for them.
  AUDIO_SONG_FIRST = 100
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
    gSound_Player3; ///< the player AUDIO_CUE_FADE_SLOT3 fades
/// The cue from AUDIO_SONG_FIRST up each player last started, by player, not
/// a volume: cues below it are not recorded, and AUDIO_CUE_FADE_SLOT3 clears
/// entry 3. Nothing in TBS reads it.
extern u16 gMusicPlayerVolumes[8];
/// VBlanks after a fanfare starts before the music waits for its end; held
/// at 1 until the fanfare's player stops, then 0.
extern u8 gMusicRestoreDelay;

// The music's volume and its combined tempo and pitch, each ramped toward its
// target by its step each VBlank, in 8.8 fixed point (MUSIC_UNITY_SCALE 1.0).
extern u16 gMusicVolume;
extern u16 gMusicVolumeTarget;
extern u16 gMusicVolumeStep;
extern u16 gMusicPitch;
extern u16 gMusicPitchTarget;
extern u16 gMusicPitchStep;
/// The music cue Sound_PlaySong last chose: 0xff before any, MUS_SILENCE
/// after a fade; asking for it again changes nothing.
extern u8 gCurrentBgm;
/// Set by AudioCommand_UpdateToggleMask; nothing in TBS reads it.
extern u8 gAudio_CommandMask;

#endif
