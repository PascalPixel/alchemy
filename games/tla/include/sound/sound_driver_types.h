#ifndef ALCHEMY_SOUND_DRIVER_TYPES_H
#define ALCHEMY_SOUND_DRIVER_TYPES_H

#include "types.h"
#include "../lib/static_assert.h"

#define SOUND_LOCK 0x68736D53

#define SOUND_WORK (*(struct SoundWork **)0x03007FF0)

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

struct SoundNote;
struct CgbNote;
struct SoundPlayer;
struct SoundTrack;

typedef void (*SoundCommand)(struct SoundPlayer *player,
                             struct SoundTrack *track);

/// Entries of gSound_CommandTable, in gSound_CommandTableTemplate's order. A
/// song byte from 0xB1 to 0xCE runs entry (byte - 0xB1) for its track; bytes
/// from 0xCF up start notes through the work's noteOn instead, so entries 30
/// to 35 are the driver's helper tail, reached only by its own calls. Unused
/// entries hold ply_fine.
///@{
/// Byte 0xB1: ends the track, releasing and unlinking its notes.
#define SOUND_COMMAND_FINISH 0
#define SOUND_COMMAND_GOTO 1 ///< Byte 0xB2: jumps to the address after it.
/// Byte 0xB9, installed by MPlayExtender: an operation, a work byte and
/// an operand; sets, adds to or compares a byte of the player's workBytes
/// (SoundTrackWorkOperation), jumping like GOTO when a comparison holds.
#define SOUND_COMMAND_WORK_BYTE 8
/// Byte 0xC2: sets lfoSpeed from the next byte; 0 clears the modulation.
#define SOUND_COMMAND_LFO_SPEED 17
/// Byte 0xC4: sets modDepth from the next byte; 0 clears the modulation.
#define SOUND_COMMAND_MODULATION 19
/// Byte 0xCD, installed by MPlayExtender: runs the entry of
/// gSound_ExtendedCommandTable the next byte names (voice fields, tail level
/// and length).
#define SOUND_COMMAND_EXTENDED 28
/// Byte 0xCE: releases the track's sounding note of the key given (or of its
/// last key when no key byte follows).
#define SOUND_COMMAND_RELEASE_KEY 29
#define SOUND_COMMAND_PCM_RATE 30 ///< SampleFreqSet.
/// TrackStop: silences the track's notes and empties its list.
#define SOUND_COMMAND_STOP_TRACK 31
#define SOUND_COMMAND_FADE 32 ///< FadeOutBody: a fade's step a VBlank.
#define SOUND_COMMAND_CALCULATE_OUTPUT 33 ///< TrkVolPitSet.
/// SoundNote_Detach: unlinks a note from its track's list
/// (ClearChain).
#define SOUND_COMMAND_DETACH_NOTE 34
/// SoundDriver_ClearBuffer: zeroes the first 64 bytes of a player or track
/// (Clear64byte).
#define SOUND_COMMAND_CLEAR_WORK 35
///@}

enum SoundDriverLimit {
  /// The PCM notes in SoundWork.pcmNotes; mode changes and
  /// SoundClear clear all 12, though only pcmNoteCount play.
  SOUND_PCM_NOTE_CAPACITY = 12,
  SOUND_CGB_CHANNEL_COUNT = 4, ///< CGB channels, numbered 1 to 4.
  /// Most tracks a player takes; MPlayOpen caps its count here,
  /// and the track masks of the MusicPlayer_Set routines are 16 bits.
  SOUND_PLAYER_TRACK_LIMIT = 16
};

enum SoundModulationTarget {
  SOUND_MODULATION_PITCH,
  SOUND_MODULATION_VOLUME,
  SOUND_MODULATION_PAN
};

enum SoundFadeFlag { SOUND_FADE_PAUSE = 1, SOUND_FADE_IN = 2 };

#define SOUND_FADE_FULL_VOLUME 0x100
#define SOUND_FADE_STEP 16

/// A voice a track plays its notes with: its kind and base key, the sample or
/// wave it plays, and its attack, decay, sustain and release.
struct SoundVoice {
  /// bits: 7 rhythm | 6 key split | 3 fixed rate for PCM, bias rounding for
  /// CGB | 2..0 CGB channel (0 PCM).
  /// A rhythm or split voice plays an entry of the voice table at target,
  /// chosen by the key (rhythm) or through the key map that attack's four
  /// bytes point to (split).
  u8 kind;
  u8 baseKey; ///< Key a rhythm entry plays every note at.
  u8 length;  ///< CGB sound length (NRx1); 0 plays until released.
  /// CGB channel 1's sweep (NR10) when bit 7 is clear and bits 6..4 are not;
  /// else no sweep. In a rhythm entry with bit 7 set, the note's pan is
  /// (value - 192) * 2.
  u8 panOrSweep;
  /// The PCM sample, the CGB wave pattern, duty or noise width, or for a
  /// rhythm or split voice its voice table.
  const void *target;
  /// PCM: added to the envelope each VBlank until 255. CGB: VBlanks a step.
  /// In a split voice, these four bytes are a pointer to its key map.
  u8 attack;
  u8 decay;   ///< PCM: envelope * decay / 256 a VBlank. CGB: VBlanks a step.
  u8 sustain; ///< Level decay stops at: 0 to 255 for PCM, 0 to 15 for CGB.
  u8 release; ///< PCM: envelope * release / 256 a VBlank. CGB: VBlanks a step.
};

/// A PCM note, one of 12 the mixer renders: its state and envelope, its
/// output levels, and its links to the other notes of its track. A track's
/// note list holds CGB notes through this type too; the names from 0x0a to
/// 0x1c come from the CGB layout (CgbNote).
struct SoundNote {
  /// 0 when free. bits: 7 start | 6 released | 4 looping | 2 echo |
  /// 1..0 phase (3 attack, 2 decay, 1 sustain, 0 release).
  u8 state;
  u8 kind;     ///< The voice's kind; bits 2..0 nonzero for a CGB note.
  u8 outRight; ///< Velocity and track volume to the right, 0 to 255.
  u8 outLeft;  ///< Velocity and track volume to the left, 0 to 255.
  u8 unk04[2]; ///< The voice's attack (0x04) and decay (0x05).
  u8 sustain;  ///< The voice's sustain level.
  /// The voice's release (0x07), the key played (0x08) and the envelope level,
  /// 0 to 255 (0x09).
  u8 unk07[3];
  /// For a PCM note, the mixer writes the envelope's right level here each
  /// VBlank; the name comes from CgbNote.peak.
  u8 peak;
  /// 0x0b left level, 0x0c echo level, 0x0d echo VBlanks, 0x10 gate in ticks,
  /// 0x11 the song's key, 0x12 velocity, 0x13 priority, 0x14 rhythm pan;
  /// the mixer keeps a PCM note's samples left in the word at 0x18.
  u8 unk0b[0x0e];
  /// CgbNote.sustainLevel; inside the samples-left word for a PCM note.
  u8 sustainLevel;
  u8 unk1a;   ///< Inside the samples-left word for a PCM note.
  u8 panBits; ///< CgbNote.panBits; inside the samples-left word for a PCM note.
  /// CgbNote.channelBits; for a PCM note, the mixer's fractional sample
  /// position is the word at 0x1c.
  u8 channelBits;
  /// 0x1d CGB update bits, 0x1e CGB length, 0x1f CGB sweep, 0x20 the word
  /// frequency, 0x24 the voice's target (the sample), 0x28 the next sample
  /// byte (mixer).
  u8 unk1d[0x0f];
  /// The track whose list holds this note; NULL once detached.
  struct SoundTrack *track;
  /// The note started after this one in the track's list (nearer its head);
  /// NULL at the head.
  struct SoundNote *prev;
  /// The note started before this one in the track's list; NULL at the end.
  struct SoundNote *next;
  u8 unk38[8]; ///< Not named by TLA code.
};

/// A note on one of the four CGB channels of the hardware: its state and
/// envelope, its levels and pan, its frequency and its wave.
struct CgbNote {
  u8 state; ///< CgbNoteState and CgbNotePhase; 0 when the channel is free.
  /// Channel number 1 to 4 at setup, then the voice's kind; bit 3 rounds
  /// the frequency to the sound bias's resolution on channels 1 to 3.
  u8 kind;
  u8 outRight; ///< Velocity and track volume to the right, 0 to 255.
  u8 outLeft;  ///< Velocity and track volume to the left, 0 to 255.
  u8 attack;   ///< VBlanks per attack step.
  u8 decay;    ///< VBlanks per decay step.
  u8 sustain;  ///< Sustain level 0 to 15, scaled by peak.
  u8 release;  ///< VBlanks per release step.
  u8 key;      ///< The key played, before the track's pitch.
  u8 level;    ///< Envelope level 0 to 15 written to the channel.
  /// Top of the envelope: (outRight + outLeft) / 16, at most 15 when one
  /// side plays.
  u8 peak;
  u8 counter;    ///< VBlanks left in the current envelope step.
  u8 echoLevel;  ///< After release, the level held is peak * echoLevel / 256.
  u8 echoLength; ///< VBlanks the echo lasts.
  /// 0x10 gate in ticks, 0x11 the song's key, 0x12 velocity, 0x13 priority
  /// (a new note of higher priority takes the channel), 0x14 rhythm pan.
  u8 unk0e[0x0b];
  u8 sustainLevel; ///< (peak * sustain + 15) / 16.
  u8 trigger;      ///< Value for NRx4: 7 restart, 6 length on, 2..0 frequency.
  u8 panBits;      ///< NR51 bits this note sets: one side or both.
  u8 channelBits;  ///< This channel's NR51 bits: 0x11, 0x22, 0x44, 0x88.
  u8 modified;     ///< CgbNoteModified: what to write to the channel.
  u8 length;       ///< The voice's length.
  u8 sweep;        ///< Channel 1's NR10 sweep; 8 when none.
  /// Tone frequency from MidiKeyToCgbFreq (2048 added), or channel 4's
  /// NR43 value.
  u32 frequency;
  /// Channel 3: the 16-byte wave pattern. Channels 1 and 2: the duty, into
  /// NRx1's bits 7..6. Channel 4: the width, into NR43's bit 3.
  u32 *wave;
  u32 *loadedWave; ///< Channel 3: the pattern last copied into wave RAM.
  /// 0x2c track, 0x30 prev and 0x34 next, as in SoundNote.
  u8 unk2c[0x14];
};

enum CgbNoteState {
  CGB_NOTE_PHASE = 0x03,
  CGB_NOTE_ECHO = 0x04,
  CGB_NOTE_STOP = 0x40,
  CGB_NOTE_START = 0x80,
  CGB_NOTE_ON = CGB_NOTE_START | CGB_NOTE_STOP | CGB_NOTE_ECHO | CGB_NOTE_PHASE
};

enum CgbNotePhase {
  CGB_PHASE_RELEASE,
  CGB_PHASE_SUSTAIN,
  CGB_PHASE_DECAY,
  CGB_PHASE_ATTACK
};

enum CgbNoteModified { CGB_NOTE_VOLUME_DIRTY = 1, CGB_NOTE_PITCH_DIRTY = 2 };

enum SoundTrackFlag {
  SOUND_TRACK_VOLUME_DIRTY = 1,
  SOUND_TRACK_UPDATE_VOLUME = 3,
  SOUND_TRACK_PITCH_DIRTY = 4,
  SOUND_TRACK_UPDATE_PITCH = 12,
  SOUND_TRACK_RESET = 0x40,
  SOUND_TRACK_ACTIVE = 0x80
};

enum SoundPlayerFlag { SOUND_PLAYER_PAUSED = 0x80000000 };

enum SoundTrackWorkOperation {
  SOUND_WORK_SET = 0,
  SOUND_WORK_ADD,
  SOUND_WORK_SUBTRACT,
  SOUND_WORK_COPY,
  SOUND_WORK_ADD_BYTE,
  SOUND_WORK_SUBTRACT_BYTE,
  SOUND_WORK_EQUAL,
  SOUND_WORK_NOT_EQUAL,
  SOUND_WORK_GREATER,
  SOUND_WORK_GREATER_EQUAL,
  SOUND_WORK_LESS_EQUAL,
  SOUND_WORK_LESS,
  SOUND_WORK_EQUAL_BYTE,
  SOUND_WORK_NOT_EQUAL_BYTE,
  SOUND_WORK_GREATER_BYTE,
  SOUND_WORK_GREATER_EQUAL_BYTE,
  SOUND_WORK_LESS_EQUAL_BYTE,
  SOUND_WORK_LESS_BYTE
};

/// A track of a playing song: the song commands it runs, its voice and notes,
/// and the pitch, volume, pan and modulation it gives them.
struct SoundTrack {
  /// SoundTrackFlag. bits: 7 active | 6 reset | 3..2 pitch to update |
  /// 1..0 volume to update.
  u8 flags;
  /// 0x01 ticks until the next command, 0x02 pattern depth (0 to 3), 0x03
  /// repeat count, 0x04 gate of the last note in ticks, 0x05 its key, 0x06
  /// its velocity, 0x07 the last command byte from 0xBD up, which a byte
  /// below 0x80 repeats.
  u8 unk01[7];
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
  /// 0x1b LFO delay in ticks (song command), 0x1c its countdown, reloaded by
  /// each note, 0x1d track priority added to the player's for its notes.
  u8 unk1b[3];
  u8 tailLevel;  ///< Each new note's echo level.
  u8 tailLength; ///< Each new note's echo length in VBlanks.
  /// The notes this track started, newest first, linked through
  /// SoundNote.next; PCM notes from SoundWork.pcmNotes and CGB notes. NULL
  /// ends the list or leaves it empty.
  struct SoundNote *notes;
  /// Copied from the player's voice table by the voice command, or set a field
  /// at a time by extended commands.
  struct SoundVoice voice;
  /// Not named by TLA code; zeroed with the first 64 bytes at reset.
  u8 unk30[0x10];
  /// The next command byte in the song's data, from the header's track
  /// pointer; a track's commands end at a finish command (0xB1).
  const u8 *cursor;
  /// Cursors to return to after each of up to three nested patterns.
  u8 unk44[0x0c];
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
  const struct SoundVoice *voices; ///< The voice table its tracks choose from.
  const u8 *tracks[1]; ///< Each track's first command, trackCount of them.
};

/// A player, which plays one song: its header and status, its tracks, its
/// tempo and fade.
struct SoundPlayer {
  const struct SongHeader *header; ///< The song playing or last played.
  /// bits: 31 paused | 15..0 tracks active after the last tick. Set to
  /// SOUND_PLAYER_PAUSED alone when no track is left.
  u32 status;
  u8 trackCount; ///< Tracks this player owns, at most 16.
  u8 priority;   ///< The song's priority.
  u8 unk0a;      ///< The last command run: its byte - 0xB1.
  /// Nonzero: a song of lower priority does not replace a playing one. From
  /// gSound_PlayerSlots, 0 for every player.
  u8 checkPriority;
  u32 clock;      ///< Song ticks played since the song started.
  u8 unk10[8];    ///< Not named by TLA code.
  u8 *workBytes;  ///< gSound_WorkBytes, which the work-byte command uses.
  u16 tempo;      ///< Twice the tempo command's byte; 150 at start.
  u16 tempoScale; ///< 256 plays at the song's tempo.
  /// tempo * tempoScale / 256, added to tempoCount each VBlank.
  u16 tempoStep;
  /// Each 150 in it runs one tick, so a tempoStep of 150 is a tick a VBlank.
  u16 tempoCount;
  u16 fadePeriod;  ///< VBlanks between fade steps; 0 when not fading.
  u16 fadeCounter; ///< VBlanks left until the next fade step.
  /// bits: 8..4 level, 0 to 0x100 in steps of 16 | 1 fade in | 0 pause at the
  /// end (SoundFadeFlag).
  u16 fadeVolume;
  u8 unk2a[2];                     ///< Not named by TLA code.
  struct SoundTrack *tracks;       ///< trackCount tracks.
  const struct SoundVoice *voices; ///< The song's voice table.
  /// SOUND_LOCK when free; raised while the player is changed or run.
  u32 lock;
  /// The MPlayMain routine and player that were first in the chain before this
  /// player was added; this player's MPlayMain runs them first.
  void (*MPlayMainNext)(struct SoundPlayer *player);
  struct SoundPlayer *musicPlayerNext; ///< Passed to MPlayMainNext.
};

/// The sound driver's work: its lock, the mixer's rate and buffers, the PCM
/// and CGB notes, and the routines the driver calls.
struct SoundWork {
  u32 lock; ///< SOUND_LOCK when free; raised while the driver changes it.
  /// VBlanks until the DMA restart, which reloads it from
  /// transferPeriod; also picks which VBlank's part of the buffers is mixed.
  /* FAKEMATCH: volatile steers library C only: without it
   * m4aSoundVSyncOn: differ 9 in tbs-en, 22 instructions for 23.
   */
  volatile u8 transferCountdown;
  u8 reverb;       ///< 0 to 127 (mode bits 6..0); 0 skips the reverb pass.
  u8 pcmNoteCount; ///< PCM notes in use, 8 at start (mode bits 11..8).
  u8 volume;       ///< PCM volume 0 to 15, 15 at start (mode bits 15..12).
  u8 rate;  ///< 1 to 12, indexes gSound_PcmSamplesPerVBlank (mode bits 19..16).
  u8 unk09; ///< Not named by TLA code.
  /// Counts 14 down to 0 once a CGB update; at 0 the envelopes take an extra
  /// step.
  u8 c15;
  u8 transferPeriod; ///< VBlanks the buffers hold: 0x630 / pcmSamplesPerVBlank.
  /// Nonzero: the mixer stops taking notes once the scanline passes this
  /// many lines after its start. Sound_MaxLines, 0.
  u8 maxLines;
  u8 unk0d[3];             ///< Not named by TLA code.
  u32 pcmSamplesPerVBlank; ///< Samples mixed a VBlank, from
                           ///< gSound_PcmSamplesPerVBlank.
  u32 outputRate; ///< Samples a second: pcmSamplesPerVBlank * 59.7275, rounded.
  /// 2^23 / outputRate, rounded; a note's frequency times it is its step a
  /// sample.
  u32 rateStep;
  struct CgbNote *cgbNotes; ///< gSound_CgbNotes; NULL before CGB setup.
  /// First of the chain of MPlayMain routines run each VBlank; NULL for none.
  void (*MPlayMainHead)(struct SoundPlayer *player);
  struct SoundPlayer *musicPlayerHead;           ///< Passed to MPlayMainHead.
  void (*cgbUpdate)(void);                       ///< Run each VBlank: CgbSound.
  void (*cgbMute)(u8 channel);                   ///< CgbOscOff, channel 1 to 4.
  s32 (*cgbFrequency)(u8 kind, u8 key, u8 fine); ///< MidiKeyToCgbFreq.
  SoundCommand *commands;                        ///< gSound_CommandTable.
  /// ply_note, run for song bytes from 0xCF up with the byte
  /// - 0xCF.
  void (*noteOn)(u32 command, struct SoundPlayer *player,
                 struct SoundTrack *track);
  /// Set to DummyFunc by SoundInit; no TLA code reads
  /// it by name.
  void (*unk3c)(void);
  u8 unk40[0x10];                ///< Not named by TLA code.
  struct SoundNote pcmNotes[12]; ///< pcmNoteCount of them play.
  /// Right (0, DMA 1 to FIFO A) and left (1, DMA 2 to FIFO B) samples,
  /// transferPeriod VBlanks of pcmSamplesPerVBlank each.
  s8 buffers[2][0x630];
};

/// An entry of the player table: a player, its tracks and track count, and
/// its priority check.
struct PlayerSlot {
  struct SoundPlayer *player;
  struct SoundTrack *tracks; ///< trackCount tracks for the player.
  u8 trackCount;
  u8 unk09;          ///< 0 in every entry; not named by TLA code.
  u16 checkPriority; ///< Copied to the player's checkPriority; 0 everywhere.
};

/// An entry of the song table: a song's header and the player table entry
/// that plays it.
struct SongEntry {
  const struct SongHeader *header;
  /// Index into gSound_PlayerSlots; Sound_Play gives slot 7's songs
  /// the first player from 7 down to 4 with no active track, else 7.
  u16 slot;
  u16 unk06; ///< The same value as slot in every entry; not named by TLA code.
};

STATIC_ASSERT(sizeof(struct SoundVoice) == 12);
STATIC_ASSERT(sizeof(struct SoundNote) == 64);
STATIC_ASSERT(sizeof(struct CgbNote) == 64);
STATIC_ASSERT(offsetof(struct CgbNote, frequency) == 0x20);
STATIC_ASSERT(offsetof(struct CgbNote, wave) == 0x24);
STATIC_ASSERT(sizeof(struct SoundTrack) == 80);
STATIC_ASSERT(offsetof(struct SoundTrack, voice) == 0x24);
STATIC_ASSERT(offsetof(struct SoundTrack, cursor) == 0x40);
STATIC_ASSERT(sizeof(struct SoundPlayer) == 64);
STATIC_ASSERT(offsetof(struct SoundWork, cgbNotes) == 0x1c);
STATIC_ASSERT(sizeof(struct SoundWork) == 0xfb0);

void MPlayExtender(struct CgbNote *notes);
void CgbModVol(struct CgbNote *note);
void ply_xwave(struct SoundPlayer *player, struct SoundTrack *track);
void ply_xcmd(struct SoundPlayer *player, struct SoundTrack *track);
void ClearChain(struct SoundNote *note);
void Clear64byte(void *block);

void MPlayStart(struct SoundPlayer *player, const struct SongHeader *header);
void m4aMPlayFadeOut(struct SoundPlayer *player, u16 speed);
void m4aMPlayTempoControl(struct SoundPlayer *player, u16 scale);
void m4aMPlayVolumeControl(struct SoundPlayer *player, u16 mask, u16 volume);
void m4aMPlayPitchControl(struct SoundPlayer *player, u16 mask, s16 pitch);
void m4aMPlayPanpotControl(struct SoundPlayer *player, u16 mask, s8 pan);
void m4aMPlayModDepthSet(struct SoundPlayer *player, s32 mask, s32 value);
void m4aMPlayLFOSpeedSet(struct SoundPlayer *player, u32 mask, u32 value);
void ClearModM(struct SoundTrack *track);

extern const struct PlayerSlot gSound_PlayerSlots[];
extern const struct SongEntry gSongTable[];
extern SoundCommand gSound_CommandTable[36];
extern const SoundCommand gSound_ExtendedCommandTable[];

#endif
