#include "audio_engine_symbols.h"
#include "types.h"

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
void Audio_DummyCallback(void);
void AudioEngine_SetPcmRate(u32 value);

void AudioEngine_Initialize(struct AudioEngineState *audio)
{
    u32 zero;
    union AudioCommandSlot *command_table;

    audio->ident = 0;

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

    *(volatile u32 *)0x040000bc = (u32)audio->pcm_buffers[0];
    *(volatile u32 *)0x040000c0 = 0x040000a0;
    *(volatile u32 *)0x040000c8 = (u32)audio->pcm_buffers[1];
    *(volatile u32 *)0x040000cc = 0x040000a4;

    *(struct AudioEngineState * volatile *)0x03007ff0 = audio;
    zero = 0;
    Func_08006864(
        &zero,
        audio,
        0x01000000 | 0x04000000 | (sizeof(*audio) / sizeof(u32)));

    audio->max_pcm_channels = 8;
    audio->master_volume = 15;
    audio->note_handler = Func_080f9f6c;
    audio->cgb_update = Audio_DummyCallback;
    audio->cgb_disable.placeholder = Audio_DummyCallback;
    audio->key_to_freq.placeholder = Audio_DummyCallback;
    audio->ext_volume_pitch = Audio_DummyCallback;

    command_table = (union AudioCommandSlot *)0x02004000;
    Func_080f9a80(command_table);
    audio->command_table = command_table;

    AudioEngine_SetPcmRate(0x40000);
    audio->ident = 0x68736d53;
}
