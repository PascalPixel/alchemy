#ifndef ALCHEMY_AUDIO_ENGINE_TYPES_H
#define ALCHEMY_AUDIO_ENGINE_TYPES_H

#include "TYPES.H"
#include "../LIB/LAYOUT_GUARD.H"

/*
 * Sound driver work areas, laid out from the reads and writes of the driver
 * routines in the ROM. A field is named only where a reconstructed routine
 * uses it; the rest keep their offsets.
 */

/* Written into every work area while it is idle; routines add to it while busy. */
#define SOUND_LOCK 0x68736D53

/* The driver keeps its work area address in the BIOS sound pointer. */
#define SOUND_WORK (*(struct SoundWork **)0x03007FF0)

struct SoundNote;
struct CgbNote;
struct SoundPlayer;
struct SoundTrack;

typedef void (*SoundCommand)(struct SoundPlayer *player, struct SoundTrack *track);

/* The twelve bytes a sequence selects from a voice bank. */
struct SoundVoice {
    u8 kind;                        /* 0x00 */
    u8 base_key;                    /* 0x01 */
    u8 length;                      /* 0x02 */
    u8 pan_or_sweep;                /* 0x03 */
    const void *target;             /* 0x04 */
    u8 attack;                      /* 0x08 */
    u8 decay;                       /* 0x09 */
    u8 sustain;                     /* 0x0a */
    u8 release;                     /* 0x0b */
};

/* Shared note prefix and track-chain links used by the DirectSound mixer. */
struct SoundNote {
    u8 state;                       /* 0x00 */
    u8 kind;                        /* 0x01 */
    u8 out_right;                   /* 0x02 */
    u8 out_left;                    /* 0x03 */
    u8 unk04[2];
    u8 sustain;                     /* 0x06 */
    u8 unk07[3];
    u8 peak;                        /* 0x0a */
    u8 unk0b[0x0e];
    u8 sustain_level;               /* 0x19 */
    u8 unk1a;
    u8 pan_bits;                    /* 0x1b */
    u8 channel_bits;                /* 0x1c */
    u8 unk1d[0x0f];
    struct SoundTrack *track;       /* 0x2c */
    struct SoundNote *prev;         /* 0x30 */
    struct SoundNote *next;         /* 0x34 */
    u8 unk38[8];
};

/* A note on one of the four CGB channels, as the channel update reads it. */
struct CgbNote {
    u8 state;                       /* 0x00 */
    u8 kind;                        /* 0x01 */
    u8 out_right;                   /* 0x02 */
    u8 out_left;                    /* 0x03 */
    u8 attack;                      /* 0x04 */
    u8 decay;                       /* 0x05 */
    u8 sustain;                     /* 0x06 */
    u8 release;                     /* 0x07 */
    u8 key;                         /* 0x08 */
    u8 level;                       /* 0x09 */
    u8 peak;                        /* 0x0a */
    u8 counter;                     /* 0x0b */
    u8 echo_level;                  /* 0x0c */
    u8 echo_length;                 /* 0x0d */
    u8 unk0e[0x0b];
    u8 sustain_level;               /* 0x19 */
    u8 trigger;                     /* 0x1a */
    u8 pan_bits;                    /* 0x1b */
    u8 channel_bits;                /* 0x1c */
    u8 modified;                    /* 0x1d */
    u8 length;                      /* 0x1e */
    u8 sweep;                       /* 0x1f */
    u32 frequency;                  /* 0x20 */
    u32 *wave;                      /* 0x24 */
    u32 *loaded_wave;               /* 0x28 */
    u8 unk2c[0x14];
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
    u8 flags;                       /* 0x00 */
    u8 unk01[7];
    u8 pitch_hi;                    /* 0x08 */
    u8 pitch_lo;                    /* 0x09 */
    s8 key_offset_a;                /* 0x0a */
    s8 key_offset_b;                /* 0x0b */
    s8 tune;                        /* 0x0c */
    u8 fine;                        /* 0x0d */
    s8 bend;                        /* 0x0e */
    u8 bend_range;                  /* 0x0f */
    u8 out_volume_a;                /* 0x10 */
    u8 out_volume_b;                /* 0x11 */
    u8 volume;                      /* 0x12 */
    u8 volume_scale;                /* 0x13 */
    s8 pan;                         /* 0x14 */
    s8 pan_offset;                  /* 0x15 */
    s8 mod_amount;                  /* 0x16 */
    u8 mod_depth;                   /* 0x17 */
    u8 mod_target;                  /* 0x18 */
    u8 lfo_speed;                   /* 0x19 */
    u8 lfo_phase;                   /* 0x1a */
    u8 unk1b[3];
    u8 tail_level;                  /* 0x1e */
    u8 tail_length;                 /* 0x1f */
    struct SoundNote *notes;        /* 0x20 */
    struct SoundVoice voice;        /* 0x24 */
    u8 unk30[0x10];
    const u8 *cursor;               /* 0x40 */
    u8 unk44[0x0c];
};

struct SequenceHeader {
    u8 track_count;                 /* 0x00 */
    u8 unk01;
    u8 priority;                    /* 0x02 */
    u8 mode;                        /* 0x03 */
    const struct SoundVoice *voices; /* 0x04 */
    const u8 *tracks[1];            /* 0x08 */
};

struct SoundPlayer {
    const struct SequenceHeader *header; /* 0x00 */
    u32 status;                     /* 0x04 */
    u8 track_count;                 /* 0x08 */
    u8 priority;                    /* 0x09 */
    u8 unk0a;
    u8 check_priority;              /* 0x0b */
    u32 ticks;                      /* 0x0c */
    u8 unk10[8];
    u8 *work_bytes;                 /* 0x18 */
    u16 tempo;                      /* 0x1c */
    u16 tempo_scale;                /* 0x1e */
    u16 tempo_step;                 /* 0x20 */
    u16 tempo_count;                /* 0x22 */
    u16 fade_period;                /* 0x24 */
    u16 fade_counter;               /* 0x26 */
    u16 fade_volume;                /* 0x28 */
    u8 unk2a[2];
    struct SoundTrack *tracks;      /* 0x2c */
    const struct SoundVoice *voices; /* 0x30 */
    u32 lock;                       /* 0x34 */
    void (*next_tick)(struct SoundPlayer *player);  /* 0x38 */
    struct SoundPlayer *next_tick_player;           /* 0x3c */
};

struct SoundWork {
    u32 lock;                       /* 0x000 */
    volatile u8 transfer_countdown; /* 0x004 */
    u8 reverb;                      /* 0x005 */
    u8 pcm_note_count;              /* 0x006 */
    u8 volume;                      /* 0x007 */
    u8 rate;                        /* 0x008 */
    u8 unk09;
    u8 cgb_tick;                    /* 0x00a */
    u8 transfer_period;             /* 0x00b */
    u8 max_lines;                   /* 0x00c */
    u8 unk0d[3];
    u32 frame_samples;              /* 0x010 */
    u32 output_rate;                /* 0x014 */
    u32 rate_step;                  /* 0x018 */
    struct CgbNote *cgb_notes;      /* 0x01c */
    void (*tick)(struct SoundPlayer *player);       /* 0x020 */
    struct SoundPlayer *tick_player;                /* 0x024 */
    void (*cgb_update)(void);                       /* 0x028 */
    void (*cgb_mute)(u8 channel);                   /* 0x02c */
    s32 (*cgb_frequency)(u8 kind, u8 key, u8 fine); /* 0x030 */
    SoundCommand *commands;         /* 0x034 */
    void (*note_on)(u32 command, struct SoundPlayer *player, struct SoundTrack *track); /* 0x038 */
    void (*unk3c)(void);
    u8 unk40[0x10];
    struct SoundNote pcm_notes[12]; /* 0x050 */
    s8 buffers[2][0x630];           /* 0x350 */
};

/* One of the players the driver opens at boot, with its track storage. */
struct PlayerSlot {
    struct SoundPlayer *player;     /* 0x00 */
    struct SoundTrack *tracks;      /* 0x04 */
    u8 track_count;                 /* 0x08 */
    u8 unk09;
    u16 check_priority;             /* 0x0a */
};

/* One song: its sequence and the player slot that plays it. */
struct SongEntry {
    const struct SequenceHeader *header; /* 0x00 */
    u16 slot;                       /* 0x04 */
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

/* Music-player controls use the same track records as the sequence driver. */
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
