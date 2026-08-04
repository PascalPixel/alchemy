typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct CgbChannel;
struct MusicPlayerState;
struct MusicTrackState;
union AudioCommandSlot;

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
    union AudioCommandSlot *command_table;
    NoteHandler note_handler;
    CgbUpdateCallback ext_volume_pitch;
    u8 unknown40[0x10];
    u8 direct_channels[12][0x40];
    u8 pcm_buffers[2][0x630];
};

void Func_08006864(const void *source, void *destination, u32 control);
void Func_080f9a80(union AudioCommandSlot *destination);
void Func_080f9f6c(
    u32,
    struct MusicPlayerState *,
    struct MusicTrackState *);
void Func_080fb790(void);
void Func_080fa798(u32 value);

void Func_080fa6a0(struct AudioEngineState *state)
{
    u32 zero;
    union AudioCommandSlot *command_table;

    state->ident = 0;

    if ((*(volatile u32 *)0x040000c4 & 0x02000000) != 0)
        *(volatile u32 *)0x040000c4 = 0x84400004;
    if ((*(volatile u32 *)0x040000d0 & 0x02000000) != 0)
        *(volatile u32 *)0x040000d0 = 0x84400004;

    *(volatile u16 *)0x040000c6 = 0x400;
    *(volatile u16 *)0x040000d2 = 0x400;
    *(volatile u16 *)0x04000084 = 143;
    *(volatile u16 *)0x04000082 = 0xa90e;
    *(volatile u8 *)0x04000089 =
        (*(volatile u8 *)0x04000089 & 0x3f) | 0x40;

    *(volatile u32 *)0x040000bc = (u32)state->pcm_buffers[0];
    *(volatile u32 *)0x040000c0 = 0x040000a0;
    *(volatile u32 *)0x040000c8 = (u32)state->pcm_buffers[1];
    *(volatile u32 *)0x040000cc = 0x040000a4;

    *(struct AudioEngineState * volatile *)0x03007ff0 = state;
    zero = 0;
    Func_08006864(
        &zero,
        state,
        0x01000000 | 0x04000000 | (sizeof(*state) / sizeof(u32)));

    state->max_pcm_channels = 8;
    state->master_volume = 15;
    state->note_handler = Func_080f9f6c;
    state->cgb_update = Func_080fb790;
    state->cgb_disable.placeholder = Func_080fb790;
    state->key_to_freq.placeholder = Func_080fb790;
    state->ext_volume_pitch = Func_080fb790;

    command_table = (union AudioCommandSlot *)0x02004000;
    Func_080f9a80(command_table);
    state->command_table = command_table;

    Func_080fa798(0x40000);
    state->ident = 0x68736d53;
}
