typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned short u16;
typedef unsigned int u32;

struct MusicPlayerState;
struct MusicTrackState;

typedef void (*PlayerMainCallback)(struct MusicPlayerState *);
typedef void (*CgbUpdateCallback)(void);
typedef void (*CgbDisableCallback)(u8);
typedef s32 (*KeyToFrequencyCallback)(s32, s32, s32);
typedef void (*PlayerTrackCommand)(
    struct MusicPlayerState *,
    struct MusicTrackState *);
typedef void (*WordAudioCommand)(u32);
typedef void (*PlayerCommand)(struct MusicPlayerState *);

union CgbDisableCallbackSlot {
    CgbUpdateCallback placeholder;
    CgbDisableCallback handler;
};

union KeyToFrequencyCallbackSlot {
    CgbUpdateCallback placeholder;
    KeyToFrequencyCallback handler;
};

union AudioCommandSlot {
    PlayerTrackCommand player_track;
    WordAudioCommand word;
    PlayerCommand player;
};

struct CgbChannel {
    u8 status;
    u8 type;
    u8 right_volume;
    u8 left_volume;
    u8 attack;
    u8 decay;
    u8 sustain;
    u8 release;
    u8 key;
    u8 envelope_volume;
    u8 envelope_goal;
    u8 envelope_counter;
    u8 pseudo_echo_volume;
    u8 pseudo_echo_length;
    u8 unknown0e[2];
    u8 gate_time;
    u8 midi_key;
    u8 velocity;
    u8 priority;
    u8 rhythm_pan;
    u8 unknown15[3];
    u8 unknown18;
    s8 sustain_goal;
    u8 unknown1a;
    u8 pan;
    u8 pan_mask;
    u8 modify;
    u8 length;
    u8 sweep;
    u32 frequency;
    const u8 *wave;
    const u8 *current_wave;
    struct MusicTrackState *track;
    struct CgbChannel *previous;
    struct CgbChannel *next;
    u8 unknown38[8];
};

struct AudioEngineState {
    u32 ident;
    u8 pcm_dma_counter;
    u8 reverb;
    u8 max_pcm_channels;
    u8 master_volume;
    u8 pcm_rate;
    u8 mode;
    u8 counter_0a;
    u8 pcm_dma_period;
    u8 max_lines;
    u8 unknown0d[3];
    u32 samples_per_vblank;
    u32 pcm_frequency;
    u32 frequency_scale;
    struct CgbChannel *cgb_channels;
    PlayerMainCallback player_main;
    struct MusicPlayerState *player_head;
    CgbUpdateCallback cgb_update;
    union CgbDisableCallbackSlot cgb_disable;
    union KeyToFrequencyCallbackSlot key_to_freq;
};

void Func_08006864(const void *source, void *destination, u32 control);
void Func_080fb518(struct MusicPlayerState *, struct MusicTrackState *);
void Func_080fa1d4(struct MusicPlayerState *, struct MusicTrackState *);
void Func_080fa1e8(struct MusicPlayerState *, struct MusicTrackState *);
void Func_080fb670(struct MusicPlayerState *, struct MusicTrackState *);
void Func_080fa16c(struct MusicPlayerState *, struct MusicTrackState *);
void Func_080fa798(u32);
void Func_080f9ef8(struct MusicPlayerState *, struct MusicTrackState *);
void Func_080fab7c(struct MusicPlayerState *);
void Func_080fac44(struct MusicPlayerState *, struct MusicTrackState *);
void Func_080fae58(void);
void Func_080fada0(u8);
s32 Func_080facf8(s32, s32, s32);
extern u8 Value_00000000;

void Func_080fa55c(struct CgbChannel *channels)
{
    u32 zero;
    struct AudioEngineState *state;
    union AudioCommandSlot *command_table;
    u32 ident;

    *(volatile u16 *)0x04000084 = 143;
    *(volatile u16 *)0x04000080 = 0;
    *(volatile u8 *)0x04000063 = 8;
    *(volatile u8 *)0x04000069 = 8;
    *(volatile u8 *)0x04000079 = 8;
    *(volatile u8 *)0x04000065 = 128;
    *(volatile u8 *)0x0400006d = 128;
    *(volatile u8 *)0x0400007d = 128;
    *(volatile u8 *)0x04000070 = 0;
    *(volatile u8 *)0x04000080 = 119;

    state = *(struct AudioEngineState * volatile *)0x03007ff0;
    ident = state->ident;
    if (ident != 0x68736d53)
        return;

    state->ident = ident + 1;

    command_table = (union AudioCommandSlot *)0x02004000;
    command_table[8].player_track = Func_080fb518;
    command_table[17].player_track = Func_080fa1d4;
    command_table[19].player_track = Func_080fa1e8;
    command_table[28].player_track = Func_080fb670;
    command_table[29].player_track = Func_080fa16c;
    command_table[30].word = Func_080fa798;
    command_table[31].player_track = Func_080f9ef8;
    command_table[32].player = Func_080fab7c;
    command_table[33].player_track = Func_080fac44;

    state->cgb_channels = channels;
    state->cgb_update = Func_080fae58;
    state->cgb_disable.handler = Func_080fada0;
    state->key_to_freq.handler = Func_080facf8;
    state->max_lines = (u32)&Value_00000000;

    zero = 0;
    Func_08006864(
        &zero,
        channels,
        0x01000000 | 0x04000000 |
            (sizeof(struct CgbChannel) * 4 / sizeof(u32)));

    channels[0].type = 1;
    channels[0].pan_mask = 0x11;
    channels[1].type = 2;
    channels[1].pan_mask = 0x22;
    channels[2].type = 3;
    channels[2].pan_mask = 0x44;
    channels[3].type = 4;
    channels[3].pan_mask = 0x88;

    state->ident = ident;
}
