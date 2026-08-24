#ifndef ALCHEMY_AUDIO_ENGINE_H
#define ALCHEMY_AUDIO_ENGINE_H

#include "audio_engine_symbols.h"
#include "types.h"

/*
 * Shared layouts for the reconstructed game audio engine.
 *
 * The named fields below are backed by accesses in Alchemy's exact C or local
 * assembly.  Offset-named storage is intentional where the field's role is not
 * yet established.  Exact sources compiled through the raw old_agbcc route
 * mirror these declarations locally because that route does not preprocess
 * #include directives.
 */

struct CgbChannel;
struct MusicPlayerState;
struct MusicTrackState;

typedef void (*PlayerMainCallback)(struct MusicPlayerState *);
typedef void (*CgbUpdateCallback)(void);
typedef void (*CgbDisableCallback)(u8);
typedef s32 (*KeyToFrequencyCallback)(s32, s32, s32);
typedef void (*NoteHandler)(
    u32,
    struct MusicPlayerState *,
    struct MusicTrackState *);

union CgbDisableCallbackSlot {
    CgbUpdateCallback placeholder;
    CgbDisableCallback handler;
};

union KeyToFrequencyCallbackSlot {
    CgbUpdateCallback placeholder;
    KeyToFrequencyCallback handler;
};

typedef void (*PlayerTrackCommand)(
    struct MusicPlayerState *,
    struct MusicTrackState *);
typedef void (*WordAudioCommand)(u32);
typedef void (*PlayerCommand)(struct MusicPlayerState *);

union AudioCommandSlot {
    PlayerTrackCommand player_track;
    WordAudioCommand word;
    PlayerCommand player;
};

/* 楽曲トラックは0x50バイト。+0x40は次のコマンド位置。 */
struct MusicTrackState {
    u8 flags;                     /* 0x00 */
    u8 unknown01[0x1d];          /* 0x01 */
    u8 pseudo_echo_volume;       /* 0x1e */
    u8 pseudo_echo_length;       /* 0x1f */
    void *channel;                /* 0x20 */
    u8 tone_type;                 /* 0x24 */
    u8 tone_key;                  /* 0x25 */
    u8 tone_length;               /* 0x26 */
    u8 tone_pan_sweep;            /* 0x27 */
    u32 wave_pointer;              /* 0x28 */
    u8 tone_attack;               /* 0x2c */
    u8 tone_decay;                /* 0x2d */
    u8 tone_sustain;              /* 0x2e */
    u8 tone_release;              /* 0x2f */
    u8 track_gap[0x10];
    const u8 *command;            /* 0x40 */
    u8 pattern_stack_bytes[0x0c]; /* 0x44 */
};

/* One music-player state is 0x40 bytes. */
struct MusicPlayerState {
    u32 song_header_word;                  /* 0x00 */
    u32 status;                           /* 0x04 */
    u8 track_count;                       /* 0x08 */
    u8 priority;                          /* 0x09 */
    u8 command;                           /* 0x0a */
    u8 config;                            /* 0x0b */
    u8 clock_and_gap[0x0c];               /* 0x0c */
    u8 *memory_area;                      /* 0x18 */
    u8 tempo_and_fade_bytes[0x10];        /* 0x1c */
    struct MusicTrackState *tracks;       /* 0x2c */
    void *voice_group;                    /* 0x30 */
    u32 ident;                            /* 0x34 */
    PlayerMainCallback next_callback;             /* 0x38 */
    struct MusicPlayerState *next_player;         /* 0x3c */
};

/* One Game Boy sound channel is 0x40 bytes. */
struct CgbChannel {
    u8 status_flags;                        /* 0x00 */
    u8 type;                                /* 0x01 */
    u8 right_volume;                        /* 0x02 */
    u8 left_volume;                         /* 0x03 */
    u8 attack;                              /* 0x04 */
    u8 decay;                               /* 0x05 */
    u8 sustain;                             /* 0x06 */
    u8 release;                             /* 0x07 */
    u8 key;                                 /* 0x08 */
    u8 envelope_volume;                     /* 0x09 */
    u8 envelope_goal;                       /* 0x0a */
    u8 envelope_counter;                    /* 0x0b */
    u8 pseudo_echo_volume;                  /* 0x0c */
    u8 pseudo_echo_length;                  /* 0x0d */
    u8 dummy1[2];                           /* 0x0e */
    u8 gate_time;                           /* 0x10 */
    u8 midi_key;                            /* 0x11 */
    u8 velocity;                            /* 0x12 */
    u8 priority;                            /* 0x13 */
    u8 rhythm_pan;                          /* 0x14 */
    u8 dummy3[3];                           /* 0x15 */
    u8 dummy5;                              /* 0x18 */
    s8 sustain_goal;                        /* 0x19 */
    u8 n4;                                  /* 0x1a */
    u8 pan;                                 /* 0x1b */
    u8 pan_mask;                            /* 0x1c */
    u8 modify;                              /* 0x1d */
    u8 length;                              /* 0x1e */
    u8 sweep;                               /* 0x1f */
    u32 frequency;                          /* 0x20 */
    const u8 *wave_pointer;                 /* 0x24 */
    const u8 *current_pointer;              /* 0x28 */
    struct MusicTrackState *track;           /* 0x2c */
    struct CgbChannel *previous_channel;    /* 0x30 */
    struct CgbChannel *next_channel;        /* 0x34 */
    u8 dummy4[8];                            /* 0x38 */
};

/*
 * The complete sound state is 0xfb0 bytes: a 0x50-byte header, twelve
 * 0x40-byte direct-sound channel records, and two 0x630-byte PCM buffers.
 */
struct AudioEngineState {
    u32 ident;                                        /* 0x000 */
    u8 pcm_dma_counter;                               /* 0x004 */
    u8 reverb;                                        /* 0x005 */
    u8 max_pcm_channels;                              /* 0x006 */
    u8 master_volume;                                 /* 0x007 */
    u8 pcm_rate;                                      /* 0x008 */
    u8 mode;                                          /* 0x009 */
    u8 c15_counter;                                   /* 0x00a */
    u8 pcm_dma_period;                                /* 0x00b */
    u8 max_lines;                                     /* 0x00c */
    u8 gap[3];                                        /* 0x00d */
    u32 pcm_samples_per_vblank;                       /* 0x010 */
    u32 pcm_freq;                                     /* 0x014 */
    u32 div_freq;                                     /* 0x018 */
    struct CgbChannel *cgb_channels;                  /* 0x01c */
    PlayerMainCallback mplay_main_head;               /* 0x020 */
    struct MusicPlayerState *music_player_head;       /* 0x024 */
    CgbUpdateCallback cgb_sound;                      /* 0x028 */
    union CgbDisableCallbackSlot cgb_osc_off;         /* 0x02c */
    union KeyToFrequencyCallbackSlot midi_key_to_cgb_freq; /* 0x030 */
    union AudioCommandSlot *mplay_jump_table;         /* 0x034 */
    NoteHandler ply_note;                             /* 0x038 */
    CgbUpdateCallback ext_volume_pitch;               /* 0x03c */
    u8 gap2[0x10];                                    /* 0x040 */
    u8 direct_channels[12][0x40];                     /* 0x050 */
    s8 pcm_buffers[2][0x630];                         /* 0x350 */
};

/* One entry in the eight-record table at 0x080fc624. */
struct PlayerBootstrapRecord {
    struct MusicPlayerState *player;   /* 0x00 */
    struct MusicTrackState *tracks;    /* 0x04 */
    u8 max_tracks;                     /* 0x08 */
    u8 unknown09;                      /* 0x09 */
    u16 config;                        /* 0x0a */
};

#define AUDIO_ENGINE_OFFSET(type, field) \
    ((u32)&(((type *)0)->field))

typedef char MusicTrackState_size[
    sizeof(struct MusicTrackState) == 0x50 ? 1 : -1
];
typedef char MusicTrackState_command_offset[
    AUDIO_ENGINE_OFFSET(struct MusicTrackState, command) == 0x40 ? 1 : -1
];
typedef char MusicPlayerState_size[
    sizeof(struct MusicPlayerState) == 0x40 ? 1 : -1
];
typedef char MusicPlayerState_tracks_offset[
    AUDIO_ENGINE_OFFSET(struct MusicPlayerState, tracks) == 0x2c ? 1 : -1
];
typedef char MusicPlayerState_ident_offset[
    AUDIO_ENGINE_OFFSET(struct MusicPlayerState, ident) == 0x34 ? 1 : -1
];
typedef char CgbChannel_size[
    sizeof(struct CgbChannel) == 0x40 ? 1 : -1
];
typedef char AudioEngineState_size[
    sizeof(struct AudioEngineState) == 0xfb0 ? 1 : -1
];
typedef char AudioEngineState_direct_channels_offset[
    AUDIO_ENGINE_OFFSET(struct AudioEngineState, direct_channels) == 0x50
        ? 1 : -1
];

#undef AUDIO_ENGINE_OFFSET

void Audio_Initialize(void);
void CgbAudio_Initialize(struct CgbChannel *channels);
void MusicPlayer_ExecuteMemoryAccessCommand(
    struct MusicPlayerState *player,
    struct MusicTrackState *track);
void Func_080fa6a0(struct AudioEngineState *audio_state);
void Func_080fa83c(u32 value);
void Func_080fa9e0(
    struct MusicPlayerState *player,
    struct MusicTrackState *tracks,
    u32 count);
void Func_080fadf0(struct CgbChannel *channel);

#endif
