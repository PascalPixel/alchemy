#ifndef ALCHEMY_SOUND_DRIVER_SYMBOLS_H
#define ALCHEMY_SOUND_DRIVER_SYMBOLS_H

#include "sounds.h"
#include "types.h"

struct MusicPlayerInfo;
/// Plays music; SOUND_FADE_OUT_BGM fades it and a jingle silences it until it
/// ends.
extern struct MusicPlayerInfo gMPlayInfo_BGM;

/// Sound_UpdateCommandMask's command: the new gSoundCommandMask, or
/// with AUDIO_COMMAND_TOGGLE the bits to flip in it.
enum AudioCommandMask {
  AUDIO_COMMAND_TOGGLE = 0x80,
  AUDIO_COMMAND_MASK = 0x7f
};

/// The sound driver's mode at start (m4aSoundInit, m4aSoundMode): 8 PCM notes
/// (bits 11..8), volume 15 (15..12), rate 7 (19..16), 352 samples a VBlank or
/// 21024 a second, and sound bias resolution 1 (from bits 23..20).
#define AUDIO_INITIAL_MODE 0x0099FA00

/// A sound for Sound_Play, as The Broken Seal's: bits 11..0 the sound number,
/// 15..12 flags, with this game's ranges. SOUND_FADE_OUT_BGM, SOUND_KEEP_BGM,
/// SOUND_FADE_OUT_ACTION_SE and SOUND_FADE_OUT_SE5 are rows of the song table,
/// in sounds.h.
enum SoundControl {
  SOUND_NUM_MASK = 0xfff,
  SOUND_FLAGS_MASK = 0xf000,
  SOUND_FLAG_FADE_IN = 0x1000, ///< music fades in over 64 VBlanks
  /// Sounds from here to SOUND_SE_FIRST are jingles; those below are music,
  /// but for SOUND_FADE_OUT_BGM and SOUND_KEEP_BGM.
  SOUND_JINGLE_FIRST = MUS_PUZZLE_SOLVED,
  /// Sounds from here to SOUND_SE_END, but for the two fades, each play on the
  /// player gSongTable names for them.
  SOUND_SE_FIRST = SE_BELL,
  SOUND_SE_END = MUS_VALE ///< sounds from here on are music again
};

/// The music players by their row in gMPlayTable
/// (sound/music_player_table.s), as a song table row names them
/// (Song.slot).
enum MusicPlayerId {
  MUSIC_PLAYER_JINGLE = 0,    ///< gMPlayInfo_Jingle
  MUSIC_PLAYER_BGM = 1,       ///< gMPlayInfo_BGM
  MUSIC_PLAYER_MENU_SE = 2,   ///< gMPlayInfo_MenuSE
  MUSIC_PLAYER_ACTION_SE = 3, ///< gMPlayInfo_ActionSE
  /// The first music player a sound on MUSIC_PLAYER_SE_LAST may borrow.
  MUSIC_PLAYER_SE_FIRST = 4,
  MUSIC_PLAYER_SE5 = 5, ///< gMPlayInfo_SE5
  /// A sound on it takes the first idle music player from here down to
  /// MUSIC_PLAYER_SE_FIRST, else this one (Sound_Play).
  MUSIC_PLAYER_SE_LAST = 7
};

/// The reverb codes of The Broken Seal's mixer. This game's
/// Sound_LoadReverbCode loads none of them, and Sound_GetReverbCode gives every
/// music SOUND_REVERB_LONG.
enum SoundReverbCode {
  SOUND_REVERB_MONO,
  SOUND_REVERB_CROSSED,
  SOUND_REVERB_NORMAL,
  SOUND_REVERB_LONG,
  SOUND_REVERB_NONE
};

/// How the frame wait and the VBlank handler share the sound driver's update,
/// so that it runs once a VBlank, from whichever reaches it first.
struct SoundVBlankState {
  u8 lastVBlank; ///< The low byte of gVBlankCount when the update last ran.
  /// SoundInfo.transferCountdown as the update last left it: the VBlank
  /// handler copies it, the frame wait counts it a VBlank ahead.
  u8 transferCountdown;
  /// Set while the sound test runs: the frame wait then leaves the update to
  /// the VBlank handler.
  u8 soundTestActive;
  /// Set while the frame wait runs the update: the VBlank handler skips its
  /// own, and Sound_Play ignores every sound.
  u8 updating;
};

// volatile: the VBlank handler and the frame wait share it.
extern volatile struct SoundVBlankState gSoundVBlankState;

extern u16 gBgmVolume;
extern u16 gBgmVolumeTarget;
extern u16 gBgmVolumeStep;
/// VBlanks after a jingle starts before the music waits for its end; held
/// at 1 until the jingle's player stops, then 0.
extern u8 gBgmRestoreDelay;
/// Set by Sound_UpdateCommandMask; nothing reads it.
extern u8 gSoundCommandMask;

extern u16 gBgmTempoPitchTarget;
extern u16 gBgmTempoPitch;
extern u16 gBgmTempoPitchStep;

/// The BIOS CpuSet service (bios.s).
void Bios_CpuSet(const void *source, void *destination, u32 control);

#endif
