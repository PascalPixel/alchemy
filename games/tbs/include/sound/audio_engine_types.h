#ifndef ALCHEMY_AUDIO_ENGINE_TYPES_H
#define ALCHEMY_AUDIO_ENGINE_TYPES_H

/// @file
/// The sound driver's types, registers and work: MusicPlayer2000, the library
/// pret calls m4a, under the names pret gives it (include/gba/m4a_internal.h).

#include "types.h"
#include "../lib/static_assert.h"

/// SoundInfo.ident and MusicPlayerInfo.ident when free; its bytes spell `Smsh`
/// in memory order. A call raises the word while it changes the driver or a
/// music player, which turns away the mixer and other calls (the README's "The
/// driver's ident").
#define ID_NUMBER 0x68736D53

#define SOUND_INFO_PTR (*(struct SoundInfo **)0x03007FF0)

#define REG_NR10 (*(volatile u8 *)0x04000060)
#define REG_NR11 (*(volatile u8 *)0x04000062)
#define REG_NR12 (*(volatile u8 *)0x04000063)
#define REG_NR13 (*(volatile u8 *)0x04000064)
#define REG_NR14 (*(volatile u8 *)0x04000065)
#define REG_NR21 (*(volatile u8 *)0x04000068)
#define REG_NR22 (*(volatile u8 *)0x04000069)
#define REG_NR23 (*(volatile u8 *)0x0400006c)
#define REG_NR24 (*(volatile u8 *)0x0400006d)
#define REG_NR30 (*(volatile u8 *)0x04000070)
#define REG_NR31 (*(volatile u8 *)0x04000072)
#define REG_NR32 (*(volatile u8 *)0x04000073)
#define REG_NR33 (*(volatile u8 *)0x04000074)
#define REG_NR34 (*(volatile u8 *)0x04000075)
#define REG_NR41 (*(volatile u8 *)0x04000078)
#define REG_NR42 (*(volatile u8 *)0x04000079)
#define REG_NR43 (*(volatile u8 *)0x0400007c)
#define REG_NR44 (*(volatile u8 *)0x0400007d)
#define REG_NR51 (*(volatile u8 *)0x04000081)

#define REG_NR50 (*(volatile u8 *)0x04000080)
#define REG_SOUNDCNT_L (*(volatile u16 *)0x04000080)
#define REG_SOUNDCNT_X (*(volatile u16 *)0x04000084)
#define SOUNDCNT_X_ENABLE 0x80
#define SOUNDCNT_X_ALL_TONES_ON 0x0f
#define REG_SOUNDBIAS_H (*(volatile u8 *)0x04000089)
#define REG_WAVE_RAM ((volatile u32 *)0x04000090)

struct SoundChannel;
struct CgbChannel;
struct MusicPlayerInfo;
struct MusicPlayerTrack;

/// A jump table entry: the driver calls it with the music player and the
/// track.
typedef void (*MPlayFunc)(struct MusicPlayerInfo *player,
                          struct MusicPlayerTrack *track);
/// Converts between MPlayFunc and a table entry that takes other
/// parameters, through the generic function type so neither cast warns.
#define MPLAY_FUNC_CAST(type, routine) ((type)(void (*)(void))(routine))

/// Entries of gMPlayJumpTable, in gMPlayJumpTableTemplate's order. A
/// song byte from 0xB1 to 0xCE runs entry (byte - 0xB1) for its track; bytes
/// from 0xCF up start notes through the work's noteOn instead, so entries 30
/// to 35 are the driver's helper tail, reached only by its own calls. Unused
/// entries hold ply_fine.
///@{
/// Byte 0xB1: ends the track, releasing and unlinking its notes.
#define MPLAY_JUMP_FINE 0
#define MPLAY_JUMP_GOTO 1 ///< Byte 0xB2: jumps to the address after it.
/// Byte 0xB9, installed by MPlayExtender: an operation, a work byte and
/// an operand; sets, adds to or compares a byte of the player's workBytes
/// (MemAccOp), jumping like GOTO when a comparison holds.
#define MPLAY_JUMP_MEMACC 8
/// Byte 0xC2: sets lfoSpeed from the next byte; 0 clears the modulation.
#define MPLAY_JUMP_LFOS 17
/// Byte 0xC4: sets modDepth from the next byte; 0 clears the modulation.
#define MPLAY_JUMP_MOD 19
/// Byte 0xCD, installed by MPlayExtender: runs the entry of
/// gXcmdTable the next byte names (voice fields, tail level
/// and length).
#define MPLAY_JUMP_XCMD 28
/// Byte 0xCE: releases the track's sounding note of the key given (or of its
/// last key when no key byte follows).
#define MPLAY_JUMP_EOT 29
#define MPLAY_JUMP_SAMPLE_FREQ_SET 30 ///< SampleFreqSet.
/// TrackStop: silences the track's notes and empties its list.
#define MPLAY_JUMP_TRACK_STOP 31
#define MPLAY_JUMP_FADE_OUT_BODY 32   ///< FadeOutBody: a frame of a fade.
#define MPLAY_JUMP_TRK_VOL_PIT_SET 33 ///< TrkVolPitSet.
/// RealClearChain: unlinks a note from its track's list
/// (ClearChain).
#define MPLAY_JUMP_CLEAR_CHAIN 34
/// SoundMainBTM: zeroes the first 64 bytes of a player or track
/// (Clear64byte).
#define MPLAY_JUMP_CLEAR_64BYTE 35
///@}

enum SoundDriverLimit {
  /// The PCM notes in SoundInfo.pcmNotes; mode changes and
  /// SoundClear clear all 12, though only pcmNoteCount play.
  MAX_DIRECTSOUND_CHANNELS = 12,
  SOUND_CGB_CHANNEL_COUNT = 4, ///< CGB channels, one per enum CgbChannelId
  /// Most tracks a player takes; MPlayOpen caps its count here,
  /// and the track masks of the m4aMPlay control routines are 16 bits.
  MAX_MUSICPLAYER_TRACKS = 16
};

/// The four CGB channels, numbered as the hardware's NR registers are.
enum CgbChannelId {
  CGB_CHANNEL_SQUARE_1 = 1, ///< a square wave with a sweep (NR10 to NR14)
  CGB_CHANNEL_SQUARE_2 = 2, ///< a square wave (NR21 to NR24)
  CGB_CHANNEL_WAVE = 3,     ///< a pattern from wave RAM (NR30 to NR34)
  CGB_CHANNEL_NOISE = 4     ///< noise (NR41 to NR44)
};

enum SoundModulationTarget {
  SOUND_MODULATION_PITCH,
  SOUND_MODULATION_VOLUME,
  SOUND_MODULATION_PAN
};

enum MusicPlayerFadeFlag { TEMPORARY_FADE = 1, FADE_IN = 2 };

#define SOUND_FADE_FULL_VOLUME 0x100
#define SOUND_FADE_STEP 16

/// A voice a track plays its notes with: its kind and base key, the sample or
/// wave it plays, and its attack, decay, sustain and release.
struct ToneData {
  /// bits: 7 rhythm | 6 key split | 3 fixed rate for PCM, bias rounding for
  /// CGB | 2..0 CGB channel (0 PCM).
  /// A rhythm or split voice plays an entry of the voice table at target,
  /// chosen by the key (rhythm) or through the key map that attack's four
  /// bytes point to (split).
  u8 kind;
  u8 baseKey; ///< Key a rhythm entry plays every note at.
  u8 length;  ///< CGB sound length (NRx1); 0 plays until released.
  /// CGB_CHANNEL_SQUARE_1's sweep (NR10) when bit 7 is clear and bits 6..4 are
  /// not; else no sweep. In a rhythm entry with bit 7 set, the note's pan is
  /// (value - 192) * 2.
  u8 panOrSweep;
  /// The PCM sample, the CGB wave pattern, duty or noise width, or for a
  /// rhythm or split voice its voice table.
  const void *target;
  /// PCM: added to the envelope each frame until 255. CGB: frames a step.
  /// In a split voice, these four bytes are a pointer to its key map.
  u8 attack;
  u8 decay;   ///< PCM: envelope * decay / 256 a frame. CGB: frames a step.
  u8 sustain; ///< Level decay stops at: 0 to 255 for PCM, 0 to 15 for CGB.
  u8 release; ///< PCM: envelope * release / 256 a frame. CGB: frames a step.
};

/// A PCM note, one of 12 the mixer renders: its state and envelope, its
/// output levels, and its links to the other notes of its track. A track's
/// note list holds CGB notes through this type too; the names from 0x0a to
/// 0x1c come from the CGB layout (CgbChannel).
struct SoundChannel {
  /// 0 when free. bits: 7 start | 6 released | 4 looping | 2 echo |
  /// 1..0 phase (3 attack, 2 decay, 1 sustain, 0 release).
  u8 state;
  u8 kind;     ///< The voice's kind; bits 2..0 nonzero for a CGB note.
  u8 outRight; ///< Velocity and track volume to the right, 0 to 255.
  u8 outLeft;  ///< Velocity and track volume to the left, 0 to 255.
  u8 attack;   ///< The voice's attack.
  u8 decay;    ///< The voice's decay.
  u8 sustain;  ///< The voice's sustain level.
  u8 release;  ///< The voice's release.
  u8 key;      ///< The key played.
  u8 level;    ///< The envelope level, 0 to 255.
  /// For a PCM note, the mixer writes the envelope's right level here each
  /// frame; the name comes from CgbChannel.peak.
  u8 peak;
  u8 levelLeft;  ///< The envelope's left level, written by the mixer.
  u8 echoLevel;  ///< After release, the level held.
  u8 echoLength; ///< Frames the echo lasts.
  u8 unk0e[2];   ///< XXX: not named by TBS code (see the README).
  u8 gate;       ///< Ticks until the note is released.
  u8 songKey;    ///< The key as the song wrote it.
  u8 velocity;   ///< 0 to 127, from the note command.
  u8 priority;   ///< A new note of higher priority takes this one's place.
  u8 rhythmPan;  ///< A rhythm voice's pan.
  u8 unk15[3];   ///< XXX: not named by TBS code (see the README).
  /// The low byte of the word at 0x18 in which the mixer keeps a PCM note's
  /// samples left.
  u8 samplesLeftLow;
  /// CgbChannel.sustainLevel; inside the samples-left word for a PCM note.
  u8 sustainLevel;
  u8 trigger; ///< CgbChannel.trigger; inside the samples-left word for a PCM
              ///< note.
  u8 panBits; ///< CgbChannel.panBits; inside the samples-left word for a PCM
              ///< note.
  /// CgbChannel.channelBits; for a PCM note, the mixer's fractional sample
  /// position is the word at 0x1c.
  u8 channelBits;
  u8 modified;          ///< CgbChannel.modified.
  u8 length;            ///< CgbChannel.length.
  u8 sweep;             ///< CgbChannel.sweep.
  u32 frequency;        ///< The note's frequency, as in CgbChannel.
  const void *sample;   ///< The voice's target: the sample it plays.
  const s8 *nextSample; ///< The next sample byte the mixer reads.
  /// The track whose list holds this note; NULL once detached.
  struct MusicPlayerTrack *track;
  /// The note started after this one in the track's list (nearer its head);
  /// NULL at the head.
  struct SoundChannel *previous;
  /// The note started before this one in the track's list; NULL at the end.
  struct SoundChannel *next;
  u8 unk38[8]; ///< XXX: not named by TBS code (see the README).
};

/// A note on one of the four CGB channels of the hardware: its state and
/// envelope, its levels and pan, its frequency and its wave.
struct CgbChannel {
  u8 state; ///< SoundChannelFlag and SoundChannelEnvelope; 0 when the channel
            ///< is free.
  /// Its enum CgbChannelId at setup, then the voice's kind; bit 3 rounds the
  /// frequency to the sound bias's resolution on every channel but
  /// CGB_CHANNEL_NOISE.
  u8 kind;
  u8 outRight; ///< Velocity and track volume to the right, 0 to 255.
  u8 outLeft;  ///< Velocity and track volume to the left, 0 to 255.
  u8 attack;   ///< Frames per attack step.
  u8 decay;    ///< Frames per decay step.
  u8 sustain;  ///< Sustain level 0 to 15, scaled by peak.
  u8 release;  ///< Frames per release step.
  u8 key;      ///< The key played, before the track's pitch.
  u8 level;    ///< Envelope level 0 to 15 written to the channel.
  /// Top of the envelope: (outRight + outLeft) / 16, at most 15 when one
  /// side plays.
  u8 peak;
  u8 counter;      ///< Frames left in the current envelope step.
  u8 echoLevel;    ///< After release, the level held is peak * echoLevel / 256.
  u8 echoLength;   ///< Frames the echo lasts.
  u8 unk0e[2];     ///< XXX: not named by TBS code (see the README).
  u8 gate;         ///< Ticks until the note is released.
  u8 songKey;      ///< The key as the song wrote it.
  u8 velocity;     ///< 0 to 127, from the note command.
  u8 priority;     ///< A new note of higher priority takes the channel.
  u8 rhythmPan;    ///< A rhythm voice's pan.
  u8 unk15[4];     ///< XXX: not named by TBS code (see the README).
  u8 sustainLevel; ///< (peak * sustain + 15) / 16.
  u8 trigger;      ///< Value for NRx4: 7 restart, 6 length on, 2..0 frequency.
  u8 panBits;      ///< NR51 bits this note sets: one side or both.
  u8 channelBits;  ///< This channel's NR51 bits: 0x11, 0x22, 0x44, 0x88.
  u8 modified;     ///< CgbChannelModify: what to write to the channel.
  u8 length;       ///< The voice's length.
  u8 sweep;        ///< CGB_CHANNEL_SQUARE_1's NR10 sweep; 8 when none.
  /// Tone frequency from MidiKeyToCgbFreq (2048 added), or
  /// CGB_CHANNEL_NOISE's NR43 value.
  u32 frequency;
  /// CGB_CHANNEL_WAVE: the 16-byte wave pattern. The square channels: the duty,
  /// into NRx1's bits 7..6. CGB_CHANNEL_NOISE: the width, into NR43's bit 3.
  u32 *wave;
  u32 *loadedWave; ///< CGB_CHANNEL_WAVE: the pattern last copied into wave RAM.
  struct MusicPlayerTrack *track; ///< As in SoundChannel.
  struct SoundChannel *previous;  ///< As in SoundChannel.
  struct SoundChannel *next;      ///< As in SoundChannel.
  u8 unk38[8]; ///< XXX: not named by TBS code (see the README).
};

enum SoundChannelFlag {
  SOUND_CHANNEL_SF_ENV = 0x03,
  SOUND_CHANNEL_SF_IEC = 0x04,
  SOUND_CHANNEL_SF_STOP = 0x40,
  SOUND_CHANNEL_SF_START = 0x80,
  SOUND_CHANNEL_SF_ON = SOUND_CHANNEL_SF_START | SOUND_CHANNEL_SF_STOP |
      SOUND_CHANNEL_SF_IEC | SOUND_CHANNEL_SF_ENV
};

enum SoundChannelEnvelope {
  SOUND_CHANNEL_SF_ENV_RELEASE,
  SOUND_CHANNEL_SF_ENV_SUSTAIN,
  SOUND_CHANNEL_SF_ENV_DECAY,
  SOUND_CHANNEL_SF_ENV_ATTACK
};

enum CgbChannelModify { CGB_CHANNEL_MO_VOL = 1, CGB_CHANNEL_MO_PIT = 2 };

enum MusicPlayerTrackFlag {
  MPT_FLG_VOLSET = 1,
  MPT_FLG_VOLCHG = 3,
  MPT_FLG_PITSET = 4,
  MPT_FLG_PITCHG = 12,
  MPT_FLG_START = 0x40,
  MPT_FLG_EXIST = 0x80
};

enum MusicPlayerStatus {
  MUSICPLAYER_STATUS_TRACK =
      0x0000ffff, ///< the tracks active after the last tick
  MUSICPLAYER_STATUS_PAUSE = 0x80000000
};

enum MemAccOp {
  MEMACC_SET = 0,
  MEMACC_ADD,
  MEMACC_SUB,
  MEMACC_MEM_SET,
  MEMACC_MEM_ADD,
  MEMACC_MEM_SUB,
  MEMACC_BEQ,
  MEMACC_BNE,
  MEMACC_BHI,
  MEMACC_BHS,
  MEMACC_BLS,
  MEMACC_BLO,
  MEMACC_MEM_BEQ,
  MEMACC_MEM_BNE,
  MEMACC_MEM_BHI,
  MEMACC_MEM_BHS,
  MEMACC_MEM_BLS,
  MEMACC_MEM_BLO
};

/// A track of a playing song: the song commands it runs, its voice and notes,
/// and the pitch, volume, pan and modulation it gives them.
struct MusicPlayerTrack {
  /// MusicPlayerTrackFlag. bits: 7 active | 6 reset | 3..2 pitch to update |
  /// 1..0 volume to update.
  u8 flags;
  u8 wait;         ///< Ticks until the next command.
  u8 patternDepth; ///< Patterns entered and not yet left, 0 to 3.
  u8 repeatCount;  ///< Times the current repeat has run.
  u8 gate;         ///< The last note's gate in ticks.
  u8 key;          ///< The last note's key.
  u8 velocity;     ///< The last note's velocity.
  /// The last command byte from 0xBD up, which a byte below 0x80 repeats.
  u8 lastCommand;
  /// Whole semitones of the track's pitch, read as signed and added to each
  /// note's key (a sum below 0 plays key 0).
  u8 pitchHi;
  /// The pitch's fraction, 0 to 255 in 1/256 semitone, passed as the fine
  /// pitch to the key-to-frequency step.
  u8 pitchLo;
  s8 keyOffsetA; ///< Semitones, from the song's key-shift command.
  s8 keyOffsetB; ///< Semitones, from m4aMPlayPitchControl.
  s8 tune;       ///< The tune command's byte - 64: 1/64 semitone.
  u8 fine;       ///< 1/256 semitone, from m4aMPlayPitchControl.
  s8 bend;       ///< The bend command's byte - 64: full bend is bendRange.
  u8 bendRange;  ///< Semitones a full bend reaches; 2 at reset.
  u8 outVolumeA; ///< Right volume: (pan + 128) * scaled volume / 256.
  u8 outVolumeB; ///< Left volume: (127 - pan) * scaled volume / 256.
  u8 volume;     ///< The song's volume command, scaled by volumeScale / 32.
  /// 0 to 64 (full, the reset value): a player volume or fade level of 0 to
  /// 0x100 divided by 4.
  u8 volumeScale;
  s8 pan;       ///< The pan command's byte - 64, -64 to 63; doubled for output.
  s8 panOffset; ///< Added to the doubled pan, from m4aMPlayPanpotControl.
  /// The LFO's output, modDepth * wave / 64: added to the pitch in 1/16
  /// semitone, to the pan, or scaling the volume by (amount + 128) / 128.
  s8 modAmount;
  /// 0 to 255, from the modulation command or m4aMPlayModDepthSet;
  /// 0 stops the LFO and clears modAmount.
  u8 modDepth;
  u8 modTarget; ///< SoundModulationTarget.
  /// LFO phase step a tick, 22 at reset; 0 stops the LFO and clears modAmount.
  u8 lfoSpeed;
  /// 0 to 255, wrapping; the wave is phase over 0 to 63, 128 - phase over 64
  /// to 191 and phase - 256 over 192 to 255, so -64 to 64.
  u8 lfoPhase;
  u8 lfoDelay;        ///< Ticks before the LFO starts, from the song.
  u8 lfoDelayCounter; ///< lfoDelay's countdown, reloaded by each note.
  u8 priority;        ///< Added to the player's priority for its notes.
  u8 tailLevel;       ///< Each new note's echo level.
  u8 tailLength;      ///< Each new note's echo length in frames.
  /// The notes this track started, newest first, linked through
  /// SoundChannel.next; PCM notes from SoundInfo.pcmNotes and CGB notes. NULL
  /// ends the list or leaves it empty.
  struct SoundChannel *notes;
  /// Copied from the player's voice table by the voice command, or set a field
  /// at a time by extended commands.
  struct ToneData voice;
  /// XXX: not named by TBS code; zeroed with the first 64 bytes at reset (see
  /// the README).
  u8 unk30[0x10];
  /// The next command byte in the song's data, from the header's track
  /// pointer; a track's commands end at a finish command (0xB1).
  const u8 *cursor;
  /// Cursors to return to after each of up to three nested patterns.
  const u8 *patternReturns[3];
};

/// A song's header: its track count, priority and mode, its voices, and where
/// each track's commands start.
struct SongHeader {
  u8 trackCount; ///< At most 16; 0 in the empty songs.
  u8 unk01;      ///< 0 in every song; ags-music refuses another block count.
  u8 priority;   ///< Becomes the player's priority.
  /// With bit 7 set, passed to m4aSoundMode (bits 6..0 the reverb);
  /// ags-music's reverb field.
  u8 mode;
  const struct ToneData *voices; ///< The voice table its tracks choose from.
  const u8 *tracks[1]; ///< Each track's first command, trackCount of them.
};

/// A music player, which plays one song: its header and status, its tracks,
/// its tempo and fade.
struct MusicPlayerInfo {
  const struct SongHeader *header; ///< The song playing or last played.
  /// bits: 31 paused | 15..0 tracks active after the last tick. Set to
  /// MUSICPLAYER_STATUS_PAUSE alone when no track is left.
  u32 status;
  u8 trackCount;  ///< Tracks this player owns, at most 16.
  u8 priority;    ///< The song's priority.
  u8 lastCommand; ///< The last command run: its byte - 0xB1.
  /// Nonzero: a song of lower priority does not replace a playing one. From
  /// gMPlayTable, 0 for every player.
  u8 checkPriority;
  u32 ticks;      ///< Song ticks played since the song started.
  u8 unk10[8];    ///< XXX: not named by TBS code (see the README).
  u8 *workBytes;  ///< gMPlayMemAccArea, which the work-byte command uses.
  u16 tempo;      ///< Twice the tempo command's byte; 150 at start.
  u16 tempoScale; ///< 256 plays at the song's tempo.
  /// tempo * tempoScale / 256, added to tempoCount each frame.
  u16 tempoStep;
  /// Each 150 in it runs one tick, so a tempoStep of 150 is a tick a frame.
  u16 tempoCount;
  u16 fadePeriod;  ///< Frames between fade steps; 0 when not fading.
  u16 fadeCounter; ///< Frames left until the next fade step.
  /// bits: 8..4 level, 0 to 0x100 in steps of 16 | 1 fade in | 0 pause at the
  /// end (MusicPlayerFadeFlag).
  u16 fadeVolume;
  u8 unk2a[2]; ///< XXX: not named by TBS code (see the README).
  struct MusicPlayerTrack *tracks; ///< trackCount tracks.
  const struct ToneData *voices;   ///< The song's voice table.
  /// ID_NUMBER when free; raised while the music player is changed or ticked.
  u32 ident;
  /// The tick routine and player that were first in the chain before this
  /// player was added; this player's tick runs them first.
  void (*nextTick)(struct MusicPlayerInfo *player);
  struct MusicPlayerInfo *nextTickPlayer; ///< Passed to nextTick.
};

/// The sound driver's work: its ident, the mixer's rate and buffers, the PCM
/// and CGB notes, and the routines the driver calls.
struct SoundInfo {
  u32 ident; ///< ID_NUMBER when free; raised while the driver changes it.
  /// Frames until the VBlank DMA restart, which reloads it from
  /// transferPeriod; also picks the frame of the buffers being mixed.
  /* FAKEMATCH(volatile): the byte is read before it is stored, as pret's
   * pcmDmaCounter is; as plain u8, 1 of 23 instructions differ in tbs-en
   * (m4aSoundVSyncOn). */
  volatile u8 transferCountdown;
  u8 reverb;       ///< 0 to 127 (mode bits 6..0); 0 skips the reverb pass.
  u8 pcmNoteCount; ///< PCM notes in use, 8 at start (mode bits 11..8).
  u8 volume;       ///< PCM volume 0 to 15, 15 at start (mode bits 15..12).
  u8 rate;  ///< 1 to 12, indexes gPcmSamplesPerVBlankTable (mode bits 19..16).
  u8 unk09; ///< XXX: not named by TBS code (see the README).
  /// Counts 14 down to 0 once a CGB update; at 0 the envelopes take an extra
  /// step.
  u8 cgbTick;
  u8 transferPeriod; ///< Frames the buffers hold: 0x630 / frameSamples.
  /// Nonzero: the mixer stops taking notes once the scanline passes this
  /// many lines after its start. gMaxLines, 0.
  u8 lineLimit;
  u8 unk0d[3];      ///< XXX: not named by TBS code (see the README).
  u32 frameSamples; ///< Samples mixed a frame, from gPcmSamplesPerVBlankTable.
  u32 outputRate;   ///< Samples a second: frameSamples * 59.7275, rounded.
  /// 2^23 / outputRate, rounded; a note's frequency times it is its step a
  /// sample.
  u32 rateStep;
  struct CgbChannel *cgbNotes; ///< gCgbChans; NULL before CGB setup.
  /// First of the chain of player ticks run each mixer frame; NULL for none.
  void (*tick)(struct MusicPlayerInfo *player);
  struct MusicPlayerInfo *tickPlayer; ///< Passed to tick.
  void (*cgbUpdate)(void);            ///< Run each mixer frame: CgbSound.
  void (*cgbMute)(u8 channel);        ///< CgbOscOff, by enum CgbChannelId.
  s32 (*cgbFrequency)(u8 kind, u8 key, u8 fine); ///< MidiKeyToCgbFreq.
  MPlayFunc *commands;                           ///< gMPlayJumpTable.
  /// ply_note, run for song bytes from 0xCF up with the byte
  /// - 0xCF.
  void (*noteOn)(u32 command, struct MusicPlayerInfo *player,
                 struct MusicPlayerTrack *track);
  /// The hook pret's MusicPlayer2000 calls ExtVolPit; set to
  /// DummyFunc by SoundInit, and no TBS code calls it.
  void (*externalVolumePitch)(void);
  u8 unk40[0x10]; ///< XXX: not named by TBS code (see the README).
  struct SoundChannel pcmNotes[12]; ///< pcmNoteCount of them play.
  /// Right (0, DMA 1 to FIFO A) and left (1, DMA 2 to FIFO B) samples,
  /// transferPeriod frames of frameSamples each.
  s8 buffers[2][0x630];
};

/// An entry of the music player table: a music player, its tracks and track
/// count, and its priority check.
struct MusicPlayer {
  struct MusicPlayerInfo *info;
  struct MusicPlayerTrack *track; ///< numTracks tracks for the music player.
  u8 numTracks;
  u8 padding;        ///< 0 in every entry, before the halfword.
  u16 checkPriority; ///< Copied to the player's checkPriority; 0 everywhere.
};

/// An entry of the song table: a song's header and the music player that
/// plays it.
struct Song {
  const struct SongHeader *header;
  /// The row of gMPlayTable that plays it (enum MusicPlayerId); Sound_PlaySong
  /// gives a song on MUSIC_PLAYER_SE_LAST the first music player from 7 down
  /// to 4 with no active track, else 7.
  u16 ms;
  u16 me; ///< The same value as ms in every entry; nothing reads it.
};

STATIC_ASSERT(sizeof(struct ToneData) == 12);
STATIC_ASSERT(sizeof(struct SoundChannel) == 64);
STATIC_ASSERT(sizeof(struct CgbChannel) == 64);
STATIC_ASSERT(offsetof(struct CgbChannel, frequency) == 0x20);
STATIC_ASSERT(offsetof(struct CgbChannel, wave) == 0x24);
STATIC_ASSERT(sizeof(struct MusicPlayerTrack) == 80);
STATIC_ASSERT(offsetof(struct MusicPlayerTrack, voice) == 0x24);
STATIC_ASSERT(offsetof(struct MusicPlayerTrack, cursor) == 0x40);
STATIC_ASSERT(sizeof(struct MusicPlayerInfo) == 64);
STATIC_ASSERT(offsetof(struct SoundInfo, cgbNotes) == 0x1c);
STATIC_ASSERT(sizeof(struct SoundInfo) == 0xfb0);

void MPlayExtender(struct CgbChannel *notes);
void CgbModVol(struct CgbChannel *note);
void ply_xwave(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xcmd(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ClearChain(struct SoundChannel *note);
void Clear64byte(void *block);

void MPlayStart(struct MusicPlayerInfo *player,
                const struct SongHeader *header);
void m4aMPlayFadeOut(struct MusicPlayerInfo *player, u16 speed);
void m4aMPlayTempoControl(struct MusicPlayerInfo *player, u16 scale);
void m4aMPlayVolumeControl(struct MusicPlayerInfo *player, u16 mask,
                           u16 volume);
void m4aMPlayPitchControl(struct MusicPlayerInfo *player, u16 mask, s16 pitch);
void m4aMPlayPanpotControl(struct MusicPlayerInfo *player, u16 mask, s8 pan);
void m4aMPlayModDepthSet(struct MusicPlayerInfo *player, s32 mask, s32 value);
void m4aMPlayLFOSpeedSet(struct MusicPlayerInfo *player, u32 mask, u32 value);
void ClearModM(struct MusicPlayerTrack *track);

extern const struct MusicPlayer gMPlayTable[];
extern const struct Song gSongTable[];
extern MPlayFunc gMPlayJumpTable[36];
extern const MPlayFunc gXcmdTable[];

#endif
