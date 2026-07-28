#ifndef ALCHEMY_AUDIO_ENGINE_H
#define ALCHEMY_AUDIO_ENGINE_H

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
    u8 status;                    /* 0x00 */
    u8 unknown01[0x1d];          /* 0x01 */
    u8 byte_1e;
    u8 unknown1f;
    void *channel_head;           /* 0x20 */
    u8 byte_24;
    u8 unknown25[3];
    u32 word_28;
    u8 byte_2c;
    u8 byte_2d;
    u8 byte_2e;
    u8 byte_2f;
    u8 unknown30[0x10];
    const u8 *command;            /* 0x40 */
    u8 unknown44[0x0c];          /* 0x44 */
};

/* One music-player state is 0x40 bytes. */
struct MusicPlayerState {
    u32 unknown00;                        /* 0x00 */
    u32 status;                           /* 0x04 */
    u8 track_count;                       /* 0x08 */
    u8 priority;                          /* 0x09 */
    u8 unknown0a;                         /* 0x0a */
    u8 config;                            /* 0x0b */
    u8 unknown0c[0x0c];                   /* 0x0c */
    u8 *memory_area;                      /* 0x18 */
    u8 unknown1c[0x10];                   /* 0x1c */
    struct MusicTrackState *tracks;       /* 0x2c */
    void *unknown30;                      /* 0x30 */
    u32 ident;                            /* 0x34 */
    PlayerMainCallback next_callback;             /* 0x38 */
    struct MusicPlayerState *next_player;         /* 0x3c */
};

/* One Game Boy sound channel is 0x40 bytes. */
struct CgbChannel {
    u8 status;                              /* 0x00 */
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
    u8 unknown0e[2];                        /* 0x0e */
    u8 gate_time;                           /* 0x10 */
    u8 midi_key;                            /* 0x11 */
    u8 velocity;                            /* 0x12 */
    u8 priority;                            /* 0x13 */
    u8 rhythm_pan;                          /* 0x14 */
    u8 unknown15[3];                        /* 0x15 */
    u8 unknown18;                           /* 0x18 */
    s8 sustain_goal;                        /* 0x19 */
    u8 unknown1a;                           /* 0x1a */
    u8 pan;                                 /* 0x1b */
    u8 pan_mask;                            /* 0x1c */
    u8 modify;                              /* 0x1d */
    u8 length;                              /* 0x1e */
    u8 sweep;                               /* 0x1f */
    u32 frequency;                          /* 0x20 */
    const u8 *wave;                         /* 0x24 */
    const u8 *current_wave;                 /* 0x28 */
    struct MusicTrackState *track;           /* 0x2c */
    struct CgbChannel *previous;             /* 0x30 */
    struct CgbChannel *next;                 /* 0x34 */
    u8 unknown38[8];                        /* 0x38 */
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
    u8 counter_0a;                                    /* 0x00a */
    u8 pcm_dma_period;                                /* 0x00b */
    u8 max_lines;                                     /* 0x00c */
    u8 unknown0d[3];                                  /* 0x00d */
    u32 samples_per_vblank;                           /* 0x010 */
    u32 pcm_frequency;                                /* 0x014 */
    u32 frequency_scale;                              /* 0x018 */
    struct CgbChannel *cgb_channels;                  /* 0x01c */
    PlayerMainCallback player_main;                   /* 0x020 */
    struct MusicPlayerState *player_head;             /* 0x024 */
    CgbUpdateCallback cgb_update;                     /* 0x028 */
    union CgbDisableCallbackSlot cgb_disable;         /* 0x02c */
    union KeyToFrequencyCallbackSlot key_to_freq;     /* 0x030 */
    union AudioCommandSlot *command_table;            /* 0x034 */
    NoteHandler note_handler;                         /* 0x038 */
    CgbUpdateCallback ext_volume_pitch;               /* 0x03c */
    u8 unknown40[0x10];                               /* 0x040 */
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

void Func_080fa2a0(void);
void Func_080fa55c(struct CgbChannel *channels);
void Func_080fa6a0(struct AudioEngineState *audio_state);
void Func_080fa83c(u32 value);
void Func_080fa9e0(
    struct MusicPlayerState *player,
    struct MusicTrackState *tracks,
    u32 count);
void Func_080fadf0(struct CgbChannel *channel);

#endif
