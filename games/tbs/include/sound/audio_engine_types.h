#ifndef ALCHEMY_AUDIO_ENGINE_TYPES_H
#define ALCHEMY_AUDIO_ENGINE_TYPES_H

/// @file
/// The sound driver's types, registers and work.

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

// Driver entries follow gSound_CommandTableTemplate, including its helper
// tail.
#define SOUND_COMMAND_FINISH 0
#define SOUND_COMMAND_GOTO 1
#define SOUND_COMMAND_WORK_BYTE 8
#define SOUND_COMMAND_LFO_SPEED 17
#define SOUND_COMMAND_MODULATION 19
#define SOUND_COMMAND_EXTENDED 28
#define SOUND_COMMAND_RELEASE_KEY 29
#define SOUND_COMMAND_PCM_RATE 30
#define SOUND_COMMAND_STOP_TRACK 31
#define SOUND_COMMAND_FADE 32
#define SOUND_COMMAND_CALCULATE_OUTPUT 33
#define SOUND_COMMAND_DETACH_NOTE 34
#define SOUND_COMMAND_CLEAR_WORK 35

enum SoundDriverLimit {
  SOUND_PCM_NOTE_CAPACITY = 12,
  SOUND_CGB_CHANNEL_COUNT = 4,
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

struct SoundVoice {
  u8 kind;
  u8 baseKey;
  u8 length;
  u8 panOrSweep;
  const void *target;
  u8 attack;
  u8 decay;
  u8 sustain;
  u8 release;
};

struct SoundNote {
  u8 state;
  u8 kind;
  u8 outRight;
  u8 outLeft;
  u8 unk04[2];
  u8 sustain;
  u8 unk07[3];
  u8 peak;
  u8 unk0b[0x0e];
  u8 sustainLevel;
  u8 unk1a;
  u8 panBits;
  u8 channelBits;
  u8 unk1d[0x0f];
  struct SoundTrack *track;
  struct SoundNote *prev;
  struct SoundNote *next;
  u8 unk38[8];
};

struct CgbNote {
  u8 state;
  u8 kind;
  u8 outRight;
  u8 outLeft;
  u8 attack;
  u8 decay;
  u8 sustain;
  u8 release;
  u8 key;
  u8 level;
  u8 peak;
  u8 counter;
  u8 echoLevel;
  u8 echoLength;
  u8 unk0e[0x0b];
  u8 sustainLevel;
  u8 trigger;
  u8 panBits;
  u8 channelBits;
  u8 modified;
  u8 length;
  u8 sweep;
  u32 frequency;
  u32 *wave;
  u32 *loadedWave;
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

struct SoundTrack {
  u8 flags;
  u8 unk01[7];
  u8 pitchHi;
  u8 pitchLo;
  s8 keyOffsetA;
  s8 keyOffsetB;
  s8 tune;
  u8 fine;
  s8 bend;
  u8 bendRange;
  u8 outVolumeA;
  u8 outVolumeB;
  u8 volume;
  u8 volumeScale;
  s8 pan;
  s8 panOffset;
  s8 modAmount;
  u8 modDepth;
  u8 modTarget;
  u8 lfoSpeed;
  u8 lfoPhase;
  u8 unk1b[3];
  u8 tailLevel;
  u8 tailLength;
  struct SoundNote *notes;
  struct SoundVoice voice;
  u8 unk30[0x10];
  const u8 *cursor;
  u8 unk44[0x0c];
};

struct SequenceHeader {
  u8 trackCount;
  u8 unk01;
  u8 priority;
  u8 mode;
  const struct SoundVoice *voices;
  const u8 *tracks[1];
};

struct SoundPlayer {
  const struct SequenceHeader *header;
  u32 status;
  u8 trackCount;
  u8 priority;
  u8 unk0a;
  u8 checkPriority;
  u32 ticks;
  u8 unk10[8];
  u8 *workBytes;
  u16 tempo;
  u16 tempoScale;
  u16 tempoStep;
  u16 tempoCount;
  u16 fadePeriod;
  u16 fadeCounter;
  u16 fadeVolume;
  u8 unk2a[2];
  struct SoundTrack *tracks;
  const struct SoundVoice *voices;
  u32 lock;
  void (*nextTick)(struct SoundPlayer *player);
  struct SoundPlayer *nextTickPlayer;
};

struct SoundWork {
  u32 lock;
  /* FAKEMATCH: volatile steers library C only: without it
   * AudioEngine_ResumeDirectSound: differ 9 in tbs-en, 22 instructions for 23.
   */
  volatile u8 transferCountdown;
  u8 reverb;
  u8 pcmNoteCount;
  u8 volume;
  u8 rate;
  u8 unk09;
  u8 cgbTick;
  u8 transferPeriod;
  u8 maxLines;
  u8 unk0d[3];
  u32 frameSamples;
  u32 outputRate;
  u32 rateStep;
  struct CgbNote *cgbNotes;
  void (*tick)(struct SoundPlayer *player);
  struct SoundPlayer *tickPlayer;
  void (*cgbUpdate)(void);
  void (*cgbMute)(u8 channel);
  s32 (*cgbFrequency)(u8 kind, u8 key, u8 fine);
  SoundCommand *commands;
  void (*noteOn)(u32 command, struct SoundPlayer *player,
                 struct SoundTrack *track);
  void (*unk3c)(void);
  u8 unk40[0x10];
  struct SoundNote pcmNotes[12];
  s8 buffers[2][0x630];
};

struct PlayerSlot {
  struct SoundPlayer *player;
  struct SoundTrack *tracks;
  u8 trackCount;
  u8 unk09;
  u16 checkPriority;
};

struct SongEntry {
  const struct SequenceHeader *header;
  u16 slot;
  u16 unk06;
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

void CgbAudio_Initialize(struct CgbNote *notes);
void CgbNote_UpdatePanEnvelope(struct CgbNote *note);
void MusicTrack_ReadVoiceTarget(struct SoundPlayer *player,
                                struct SoundTrack *track);
void MusicTrack_DispatchStreamCommand(struct SoundPlayer *player,
                                      struct SoundTrack *track);
void AudioCommand_InvokeSlot34(struct SoundNote *note);
void AudioCommand_InvokeSlot35(void *block);

void MusicPlayer_StartSong(struct SoundPlayer *player,
                           const struct SequenceHeader *header);
void MusicPlayer_FadeOut(struct SoundPlayer *player, u16 speed);
void MusicPlayer_SetPitchAndUpdateFrequency(struct SoundPlayer *player,
                                            u16 scale);
void MusicPlayer_SetVolume(struct SoundPlayer *player, u16 mask, u16 volume);
void MusicPlayer_SetPitch(struct SoundPlayer *player, u16 mask, s16 pitch);
void MusicPlayer_SetPan(struct SoundPlayer *player, u16 mask, s8 pan);
void MusicPlayer_SetModulationDepth(struct SoundPlayer *player, s32 mask,
                                    s32 value);
void MusicPlayer_SetLfoSpeed(struct SoundPlayer *player, u32 mask, u32 value);
void MusicTrack_ClearModulation(struct SoundTrack *track);

extern const struct PlayerSlot gSound_PlayerSlots[];
extern const struct SongEntry gSound_SongTable[];
extern SoundCommand gSound_CommandTable[36];
extern const SoundCommand gSound_ExtendedCommandTable[];

#endif
