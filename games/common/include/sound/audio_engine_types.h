#ifndef ALCHEMY_AUDIO_ENGINE_TYPES_H
#define ALCHEMY_AUDIO_ENGINE_TYPES_H

#include "types.h"
#include "../lib/layout_guard.h"

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

typedef void (*SoundCommand)(struct SoundPlayer *player, struct SoundTrack *track);

struct SoundVoice {
    u8 kind;
    u8 base_key;
    u8 length;
    u8 pan_or_sweep;
    const void *target;
    u8 attack;
    u8 decay;
    u8 sustain;
    u8 release;
};

struct SoundNote {
    u8 state;
    u8 kind;
    u8 out_right;
    u8 out_left;
    u8 unk04[2];
    u8 sustain;
    u8 unk07[3];
    u8 peak;
    u8 unk0b[0x0e];
    u8 sustain_level;
    u8 unk1a;
    u8 pan_bits;
    u8 channel_bits;
    u8 unk1d[0x0f];
    struct SoundTrack *track;
    struct SoundNote *prev;
    struct SoundNote *next;
    u8 unk38[8];
};

struct CgbNote {
    u8 state;
    u8 kind;
    u8 out_right;
    u8 out_left;
    u8 attack;
    u8 decay;
    u8 sustain;
    u8 release;
    u8 key;
    u8 level;
    u8 peak;
    u8 counter;
    u8 echo_level;
    u8 echo_length;
    u8 unk0e[0x0b];
    u8 sustain_level;
    u8 trigger;
    u8 pan_bits;
    u8 channel_bits;
    u8 modified;
    u8 length;
    u8 sweep;
    u32 frequency;
    u32 *wave;
    u32 *loaded_wave;
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

enum CgbNoteModified {
    CGB_NOTE_VOLUME_DIRTY = 1,
    CGB_NOTE_PITCH_DIRTY = 2
};

enum SoundTrackFlag {
    SOUND_TRACK_VOLUME_DIRTY = 1,
    SOUND_TRACK_UPDATE_VOLUME = 3,
    SOUND_TRACK_PITCH_DIRTY = 4,
    SOUND_TRACK_UPDATE_PITCH = 12,
    SOUND_TRACK_RESET = 0x40,
    SOUND_TRACK_ACTIVE = 0x80
};

enum SoundPlayerFlag {
    SOUND_PLAYER_PAUSED = 0x80000000
};

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
    u8 pitch_hi;
    u8 pitch_lo;
    s8 key_offset_a;
    s8 key_offset_b;
    s8 tune;
    u8 fine;
    s8 bend;
    u8 bend_range;
    u8 out_volume_a;
    u8 out_volume_b;
    u8 volume;
    u8 volume_scale;
    s8 pan;
    s8 pan_offset;
    s8 mod_amount;
    u8 mod_depth;
    u8 mod_target;
    u8 lfo_speed;
    u8 lfo_phase;
    u8 unk1b[3];
    u8 tail_level;
    u8 tail_length;
    struct SoundNote *notes;
    struct SoundVoice voice;
    u8 unk30[0x10];
    const u8 *cursor;
    u8 unk44[0x0c];
};

struct SequenceHeader {
    u8 track_count;
    u8 unk01;
    u8 priority;
    u8 mode;
    const struct SoundVoice *voices;
    const u8 *tracks[1];
};

struct SoundPlayer {
    const struct SequenceHeader *header;
    u32 status;
    u8 track_count;
    u8 priority;
    u8 unk0a;
    u8 check_priority;
    u32 ticks;
    u8 unk10[8];
    u8 *work_bytes;
    u16 tempo;
    u16 tempo_scale;
    u16 tempo_step;
    u16 tempo_count;
    u16 fade_period;
    u16 fade_counter;
    u16 fade_volume;
    u8 unk2a[2];
    struct SoundTrack *tracks;
    const struct SoundVoice *voices;
    u32 lock;
    void (*next_tick)(struct SoundPlayer *player);
    struct SoundPlayer *next_tick_player;
};

struct SoundWork {
    u32 lock;
    volatile u8 transfer_countdown;
    u8 reverb;
    u8 pcm_note_count;
    u8 volume;
    u8 rate;
    u8 unk09;
    u8 cgb_tick;
    u8 transfer_period;
    u8 max_lines;
    u8 unk0d[3];
    u32 frame_samples;
    u32 output_rate;
    u32 rate_step;
    struct CgbNote *cgb_notes;
    void (*tick)(struct SoundPlayer *player);
    struct SoundPlayer *tick_player;
    void (*cgb_update)(void);
    void (*cgb_mute)(u8 channel);
    s32 (*cgb_frequency)(u8 kind, u8 key, u8 fine);
    SoundCommand *commands;
    void (*note_on)(u32 command, struct SoundPlayer *player, struct SoundTrack *track);
    void (*unk3c)(void);
    u8 unk40[0x10];
    struct SoundNote pcm_notes[12];
    s8 buffers[2][0x630];
};

struct PlayerSlot {
    struct SoundPlayer *player;
    struct SoundTrack *tracks;
    u8 track_count;
    u8 unk09;
    u16 check_priority;
};

struct SongEntry {
    const struct SequenceHeader *header;
    u16 slot;
    u16 unk06;
};

LAYOUT_SIZE_GUARD(SoundVoice_Size, struct SoundVoice, 12);
LAYOUT_SIZE_GUARD(SoundNote_Size, struct SoundNote, 64);
LAYOUT_SIZE_GUARD(CgbNote_Size, struct CgbNote, 64);
LAYOUT_OFFSET_GUARD(CgbNote_Frequency, struct CgbNote, frequency, 0x20);
LAYOUT_OFFSET_GUARD(CgbNote_Wave, struct CgbNote, wave, 0x24);
LAYOUT_SIZE_GUARD(SoundTrack_Size, struct SoundTrack, 80);
LAYOUT_OFFSET_GUARD(SoundTrack_Voice, struct SoundTrack, voice, 0x24);
LAYOUT_OFFSET_GUARD(SoundTrack_Cursor, struct SoundTrack, cursor, 0x40);
LAYOUT_SIZE_GUARD(SoundPlayer_Size, struct SoundPlayer, 64);
LAYOUT_OFFSET_GUARD(SoundWork_CgbNotes, struct SoundWork, cgb_notes, 0x1c);
LAYOUT_SIZE_GUARD(SoundWork_Size, struct SoundWork, 0xfb0);

void CgbAudio_Initialize(struct CgbNote *notes);
void CgbNote_UpdatePanEnvelope(struct CgbNote *note);
void MusicTrack_ReadVoiceTarget(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_DispatchStreamCommand(struct SoundPlayer *player, struct SoundTrack *track);
void AudioCommand_InvokeSlot34(struct SoundNote *note);
void AudioCommand_InvokeSlot35(void *block);

void MusicPlayer_StartSong(struct SoundPlayer *player, const struct SequenceHeader *header);
void MusicPlayer_FadeOut(struct SoundPlayer *player, u16 speed);
void MusicPlayer_SetPitchAndUpdateFrequency(struct SoundPlayer *player, u16 scale);
void MusicPlayer_SetVolume(struct SoundPlayer *player, u16 mask, u16 volume);
void MusicPlayer_SetPitch(struct SoundPlayer *player, u16 mask, s16 pitch);
void MusicPlayer_SetPan(struct SoundPlayer *player, u16 mask, s8 pan);
void MusicPlayer_SetModulationDepth(struct SoundPlayer *player, s32 mask, s32 value);
void MusicPlayer_SetLfoSpeed(struct SoundPlayer *player, u32 mask, u32 value);
void MusicTrack_ClearModulation(struct SoundTrack *track);

extern const struct PlayerSlot Sound_PlayerSlots[];
extern const struct SongEntry Sound_SongTable[];
extern SoundCommand Sound_CommandTable[36];
extern const SoundCommand Sound_ExtendedCommandTable[];

#endif
