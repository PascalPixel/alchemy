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
    u8 c15_counter;
    u8 pcm_dma_period;
    u8 max_lines;
    u8 gap[3];
    u32 pcm_samples_per_vblank;
    u32 pcm_freq;
    u32 div_freq;
    struct CgbChannel *cgb_channels;
    PlayerMainCallback mplay_main_head;
    struct MusicPlayerState *music_player_head;
    CgbUpdateCallback cgb_sound;
    union CgbDisableCallbackSlot cgb_osc_off;
    union KeyToFrequencyCallbackSlot midi_key_to_cgb_freq;
    union AudioCommandSlot *mplay_jump_table;
    NoteHandler ply_note;
    CgbUpdateCallback ext_volume_pitch;
    u8 gap2[0x10];
    u8 direct_channels[12][0x40];
    u8 pcm_buffers[2][0x630];
};

void Func_08006864(const void *source, void *destination, u32 control);
void MusicPlayer_CopyCommandTable(union AudioCommandSlot *destination);
void MusicTrack_HandleNote(
    u32,
    struct MusicPlayerState *,
    struct MusicTrackState *);
void Audio_DummyCallback(void);
void AudioEngine_SetPcmRate(u32 value);

void AudioEngine_Initialize(struct AudioEngineState *audio)
{
    u32 zero;
    union AudioCommandSlot *mplay_jump_table;

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
    audio->ply_note = MusicTrack_HandleNote;
    audio->cgb_sound = Audio_DummyCallback;
    audio->cgb_osc_off.placeholder = Audio_DummyCallback;
    audio->midi_key_to_cgb_freq.placeholder = Audio_DummyCallback;
    audio->ext_volume_pitch = Audio_DummyCallback;

    mplay_jump_table = (union AudioCommandSlot *)0x02004000;
    MusicPlayer_CopyCommandTable(mplay_jump_table);
    audio->mplay_jump_table = mplay_jump_table;

    AudioEngine_SetPcmRate(0x40000);
    audio->ident = 0x68736d53;
}
