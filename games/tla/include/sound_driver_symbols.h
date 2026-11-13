#ifndef ALCHEMY_SOUND_DRIVER_SYMBOLS_H
#define ALCHEMY_SOUND_DRIVER_SYMBOLS_H

#include "types.h"

struct SoundPlayer;
struct LzPackWork;
extern struct LzPackWork *gFlash_Handler3;
extern struct SoundPlayer gMPlayInfo_BGM;

enum AudioCommandMask {
  AUDIO_COMMAND_TOGGLE = 0x80,
  AUDIO_COMMAND_MASK = 0x7f
};

#define AUDIO_INITIAL_MODE 0x0099FA00

/// A sound for Sound_Play, as The Broken Seal's: bits 11..0 the sound number,
/// 15..12 flags, with this game's ranges.
enum SoundControl {
  SOUND_NUM_MASK = 0xfff,
  SOUND_FLAGS_MASK = 0xf000,
  SOUND_FLAG_FADE_IN = 0x1000, ///< music fades in over 64 VBlanks
  SOUND_FADE_OUT_BGM = 78,     ///< fades the music out
  SOUND_KEEP_BGM = 79,         ///< changes nothing
  /// Sounds from here to SOUND_SE_FIRST are jingles; those below are music,
  /// but for SOUND_FADE_OUT_BGM and SOUND_KEEP_BGM.
  SOUND_JINGLE_FIRST = 80,
  /// Sounds from here to SOUND_SE_END, but for the two fades, each play on the
  /// player gSongTable names for them.
  SOUND_SE_FIRST = 100,
  /// Fades MUSIC_PLAYER_ACTION_SE and MUSIC_PLAYER_SE5 out; the song table's
  /// row of this number never plays.
  SOUND_FADE_OUT_ACTION_SE = 390,
  SOUND_FADE_OUT_SE5 = 395, ///< fades MUSIC_PLAYER_SE5 out
  SOUND_SE_END = 700        ///< sounds from here on are music again
};

/// The music players by their row in gSound_PlayerSlots (sound/players.s), as
/// a song table row names them (SongEntry.slot).
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
  /// SoundWork.transferCountdown as the update last left it: the VBlank
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
extern u8 gBgmRestoreDelay;
extern u8 gSoundCommandMask;

extern u16 gBgmTempoPitchTarget;
extern u16 gBgmTempoPitch;
extern u16 gBgmTempoPitchStep;

/// The BIOS CpuSet service (bios.s).
void Bios_CpuSet(const void *source, void *destination, u32 control);

#endif
