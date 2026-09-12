#include "audio_engine_symbols.h"
#include "types.h"
#include "scene.h"

extern u8 RomBytes_080f9c91[];
extern u8 RomBytes_080fb92c[];

/* audio/init/initialize_engine.c */
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

    *(struct AudioEngineState *volatile *)0x03007ff0 = audio;
    zero = 0;
    Audio_Place(
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

/* audio/mix/set_pcm_rate.c */
struct PcmRateState {
    u8 padding00[8];
    u8 rate;
    u8 padding09[2];
    u8 dma_period;
    u8 padding0c[4];
    u32 pcm_samples_per_vblank;
    s32 frequency;
    s32 half_period;
};

extern u16 RomBytes_080fb914[];

s32 FixedPoint_Ratio(s32 numerator, s32 denominator);
void AudioEngine_EnablePcmDma(void);

void AudioEngine_SetPcmRate(u32 mode_bits)
{
    struct PcmRateState *audio =
        *(struct PcmRateState **)0x03007ff0;
    u16 pcm_samples_per_vblank;
    s32 frequency;
    s32 zero;
    volatile u16 *timer;

    mode_bits = (mode_bits & 0x000f0000) >> 16;
    zero = 0;
    audio->rate = mode_bits;
    pcm_samples_per_vblank = RomBytes_080fb914[mode_bits - 1];
    audio->pcm_samples_per_vblank = pcm_samples_per_vblank;
    audio->dma_period = FixedPoint_Ratio(0x630, pcm_samples_per_vblank);
    frequency = FixedPoint_Ratio(
        0x91d1b * pcm_samples_per_vblank + 0x1388,
        0x2710);
    audio->frequency = frequency;
    audio->half_period = (FixedPoint_Ratio(0x01000000, frequency) + 1) >> 1;
    *(volatile u16 *)0x04000102 = zero;
    timer = (volatile u16 *)0x04000100;
    *timer = -FixedPoint_Ratio(0x44940, pcm_samples_per_vblank);
    AudioEngine_EnablePcmDma();
    while (*(volatile u8 *)0x04000006 == 0x9f) {
    }
    while (*(volatile u8 *)0x04000006 != 0x9f) {
    }
    *(volatile u16 *)0x04000102 = 0x80;
}

/* audio/mix/set_mode.c */
void AudioEngine_DisablePcmDma(void);

void AudioEngine_SetMode(u32 mode)
{
    u8 *audio = *(u8 **)0x03007FF0;
    u32 ident = *(u32 *)audio;
    u32 mode_bits;

    if (ident != 0x68736D53) {
        return;
    }

    *(u32 *)audio = ident + 1;

    mode_bits = mode & 0xFF;
    if (mode_bits != 0) {
        audio[5] = mode_bits & 0x7F;
    }

    mode_bits = mode & 0xF00;
    if (mode_bits != 0) {
        u8 *channel;

        audio[6] = mode_bits >> 8;
        mode_bits = 12;
        channel = audio + 0x50;
        do {
            *channel = 0;
            mode_bits--;
            channel += 0x40;
        } while (mode_bits != 0);
    }

    mode_bits = mode & 0xF000;
    if (mode_bits != 0) {
        audio[7] = mode_bits >> 12;
    }

    mode_bits = mode & 0xB00000;
    if (mode_bits != 0) {
        mode_bits = (mode_bits & 0x300000) >> 14;
        *(volatile u8 *)0x04000089 =
            (*(volatile u8 *)0x04000089 & 0x3F) | mode_bits;
    }

    mode_bits = mode & 0xF0000;
    if (mode_bits != 0) {
        AudioEngine_DisablePcmDma();
        AudioEngine_SetPcmRate(mode_bits);
    }

    *(u32 *)audio = 0x68736D53;
}

/* audio/ctrl/stop_all_channels.c */
typedef void (*DisableFunction)(u8);

extern u8 *gIw;

void AudioEngine_StopAllChannels(void)
{
    u8 *audio = gIw;
    u32 ident = *(u32 *)audio;
    s32 remaining;
    u8 *channel;

    if (ident != 0x68736d53)
        return;

    *(u32 *)audio = ident + 1;

    remaining = 12;
    channel = audio + 80;
    do {
        *channel = 0;
        remaining--;
        channel += 64;
    } while (remaining > 0);

    channel = *(u8 **)(audio + 28);
    if (channel != 0) {
        remaining = 1;
        do {
            u8 channel_id = (u8)remaining;
            DisableFunction disable = *(DisableFunction *)(audio + 44);

            disable(channel_id);
            *channel = 0;
            remaining++;
            channel += 64;
        } while (remaining <= 4);
    }

    *(u32 *)audio = 0x68736d53;
}

/* audio/mix/disable_pcm_dma.c */
void AudioEngine_DisablePcmDma(void)
{
    s32 *audio;
    s32 ident;
    s32 zero;

    ident = *(audio = *(s32 **)0x03007ff0);
    if ((u32)(ident + 0x978c92ad) <= 1) {
        *audio = ident + 10;
        if (*(u32 *)0x040000c4 & 0x02000000)
            *(u32 *)0x040000c4 = 0x84400004;
        if (*(u32 *)0x040000d0 & 0x02000000)
            *(u32 *)0x040000d0 = 0x84400004;
        {
            volatile u16 *dma_control = (volatile u16 *)0x040000c6;
            *dma_control = 0x400;
            dma_control += 6;
            *dma_control = 0x400;
        }
        zero = 0;
        Audio_Place(&zero, (u8 *)audio + 848, (void *)0x05000318);
    }
}

/* audio/mix/enable_pcm_dma.c */
struct PcmDmaState {
    u32 ident;
    volatile u8 dma_counter;
};

void AudioEngine_EnablePcmDma(void)
{
    struct PcmDmaState *audio =
        *(struct PcmDmaState *volatile *)0x03007ff0;
    u32 ident = audio->ident;

    if (ident != 0x68736d53) {
        volatile u16 *dma_control = (volatile u16 *)0x040000c6;

        *dma_control = 0xb600;
        dma_control += 6;
        *dma_control = 0xb600;
        audio->dma_counter = 0;
        audio->ident = ident - 10;
    }
}

/* audio/init/initialize_music_player.c */

struct MusicTrackState {
    s8 flags;
    u8 unknown01[0x4f];
};

struct MusicPlayerState {
    u32 song_header_word;
    u32 status;
    u8 track_count;
    u8 priority;
    u8 command;
    u8 config;
    u8 clock_and_gap[0x0c];
    u8 *memory_area;
    u8 tempo_and_fade_bytes[0x10];
    struct MusicTrackState *tracks;
    void *voice_group;
    u32 ident;
    PlayerMainCallback next_callback;
    struct MusicPlayerState *next_player;
};

void MusicPlayer_Initialize(
    struct MusicPlayerState *player,
    struct MusicTrackState *track_storage_arg,
    u32 track_count_arg)
{
    register struct MusicTrackState *track_storage = track_storage_arg;
    u8 track_count = track_count_arg;
    struct AudioEngineState *audio;

    if (track_count == 0)
        return;
    if (track_count > 16)
        track_count = 16;

    audio = *(struct AudioEngineState **)0x03007ff0;
    if (audio->ident != 0x68736d53)
        return;

    audio->ident++;
    AudioCommand_InvokeSlot35(player);
    player->tracks = track_storage;
    player->track_count = track_count;
    player->status = 0x80000000;

    if (track_count != 0) {
        do {
            u32 next_count;
            track_storage->flags = 0;
            next_count = track_count - 1;
            track_count = next_count;
            track_storage++;
        } while (track_count != 0);
    }

    if (audio->mplay_main_head != 0) {
        player->next_callback = audio->mplay_main_head;
        player->next_player = audio->music_player_head;
        audio->mplay_main_head = 0;
    }

    audio->music_player_head = player;
    audio->mplay_main_head = (PlayerMainCallback)&RomBytes_080f9c91;
    audio->ident = 0x68736d53;
    player->ident = 0x68736d53;
}

/* audio/play/start_song.c */
struct SongHeader {
    u8 track_count;
    u8 block_count;
    u8 priority;
    u8 flags;
    s32 voice_group;
    s32 track_data[1];
};

struct SongStartTrack {
    u8 flags;
    u8 padding01[0x1f];
    s32 channel;
    u8 padding24[0x1c];
    s32 command;
    u8 padding44[0x0c];
};

struct SongStartPlayer {
    struct SongHeader *song;
    s32 status;
    u8 track_capacity;
    u8 priority;
    u8 command;
    u8 check_song_priority;
    s32 clock;
    u8 padding10[0x1c];
    struct SongStartTrack *tracks;
    s32 voice_group;
    u32 ident;
};

void MusicTrack_Stop();

void MusicPlayer_StartSong(
    struct SongStartPlayer *player,
    struct SongHeader *song)
{
    struct SongStartTrack *track;
    s32 track_index;
    s32 status;
    u8 check_song_priority;
    u32 priority;
    u32 reset;

    if (player->ident != 0x68736d53)
        return;

    check_song_priority = player->check_song_priority;
    priority = song->priority;
    if (check_song_priority != 0
        && !(((player->song == 0
               || (player->tracks->flags & 0x40) == 0)
              && ((status = player->status),
                  (*(volatile u16 *)&player->status == 0 || status < 0)))
             || player->priority
                    <= (priority = *(volatile u8 *)&song->priority)))
        return;

    player->ident++;
    player->status = 0;
    player->song = song;
    player->voice_group = song->voice_group;
    player->priority = priority;
    player->clock = 0;

    reset = 150;
    *(u16 *)((u8 *)player + 0x1c) = reset;
    *(u16 *)((u8 *)player + 0x20) = reset;
    reset += 106;
    *(u16 *)((u8 *)player + 0x1e) = reset;
    *(u16 *)((u8 *)player + 0x22) = 0;
    *(u16 *)((u8 *)player + 0x24) = 0;

    track_index = 0;
    track = player->tracks;
    while (track_index < song->track_count
           && track_index < player->track_capacity) {
        MusicTrack_Stop(player, track);
        track->flags = 0xc0;
        track->channel = 0;
        track->command = song->track_data[track_index];
        track_index++;
        track++;
    }
    while (track_index < player->track_capacity) {
        MusicTrack_Stop(player, track);
        track->flags = 0;
        track_index++;
        track++;
    }
    if ((song->flags & 0x80) != 0)
        AudioEngine_SetMode(song->flags);
    player->ident = 0x68736d53;
}

/* audio/ctrl/stop_music_player.c */
void MusicPlayer_Stop(u8 *player)
{
    u32 ident = *(u32 *)(player + 52);
    s32 track_count;
    u8 *track;

    if (ident != 0x68736d53)
        return;

    *(u32 *)(player + 52) = ident + 1;
    *(u32 *)(player + 4) |= 0x80000000;

    track_count = player[8];
    track = *(u8 **)(player + 44);
    if (track_count > 0) {
        do {
            MusicTrack_Stop(player, track);
            track_count--;
            track += 80;
        } while (track_count > 0);
    }

    *(u32 *)(player + 52) = 0x68736d53;
}

/* audio/misc/update_music_player_fade.c */
// Approved reference provenance: adapted from pret/pokeemerald
// src/m4a.c:FadeOutBody at commit 83df84e40623b79281f2397faa611cbf044170bd.
// Names and layout were checked against this ROM region and exact compiler output.

struct FadeTrackState {
    u8 flags;
    u8 padding01[18];
    u8 volume_scale;
    u8 padding14[60];
};

struct FadePlayerState {
    void *song_header;
    u32 status;
    u8 track_count;
    u8 priority;
    u8 command;
    u8 config;
    u32 clock;
    u8 gap10[8];
    u8 *memory;
    u16 tempo_down;
    u16 tempo_up;
    u16 tempo_interval;
    u16 tempo_counter;
    u16 fade_interval;
    u16 fade_counter;
    u16 fade_volume;
    struct FadeTrackState *tracks;
};

void MusicPlayer_UpdateFade(struct FadePlayerState *player)
{
    s32 track_count;
    struct FadeTrackState *track;
    u16 fade_volume;

    if (player->fade_interval == 0)
        return;
    if (--player->fade_counter != 0)
        return;

    player->fade_counter = player->fade_interval;

    if (player->fade_volume & 2) {
        if ((u16)(player->fade_volume += (4 << 2)) >= (64 << 2)) {
            player->fade_volume = (64 << 2);
            player->fade_interval = 0;
        }
    } else {
        if ((s16)(player->fade_volume -= (4 << 2)) <= 0) {
            track_count = player->track_count;
            track = player->tracks;

            while (track_count > 0) {
                u32 active;

                MusicTrack_Stop(player, track);

                active = 1;
                fade_volume = player->fade_volume;
                active &= fade_volume;

                if (!active)
                    track->flags = 0;

                track_count--;
                track++;
            }

            if (player->fade_volume & 1)
                player->status |= 0x80000000;
            else
                player->status = 0x80000000;

            player->fade_interval = 0;
            return;
        }
    }

    track_count = player->track_count;
    track = player->tracks;

    while (track_count > 0) {
        if (track->flags & 0x80) {
            fade_volume = player->fade_volume;

            track->volume_scale = (fade_volume >> 2);
            track->flags |= 3;
        }

        track_count--;
        track++;
    }
}

/* audio/misc/update_track_volume_pitch.c */
// Approved reference provenance: adapted from pret/pokeemerald
// src/m4a.c:TrkVolPitSet at commit 83df84e40623b79281f2397faa611cbf044170bd.
// Names and layout were checked against this ROM region and exact compiler output.


struct VolumePitchTrackState {
    u8 flags;
    u8 wait;
    u8 pattern_level;
    u8 repeat_count;
    u8 gate_time;
    u8 key;
    u8 velocity;
    u8 running_status;
    u8 key_mod;
    u8 pitch_mod;
    s8 key_shift;
    s8 key_shift_extra;
    s8 tune;
    u8 pitch_extra;
    s8 bend;
    u8 bend_range;
    u8 right_volume;
    u8 left_volume;
    u8 volume;
    u8 volume_scale;
    s8 pan;
    s8 pan_extra;
    s8 modulation_delta;
    u8 modulation;
    u8 modulation_type;
};

void MusicTrack_UpdateVolumePitch(
    struct MusicPlayerState *unused,
    struct VolumePitchTrackState *track)
{
    if (track->flags & 1) {
        s32 volume;
        s32 pan;

        volume = (u32)(track->volume *track->volume_scale) >> 5;

        if (track->modulation_type == 1)
            volume =
                (u32)(volume *(track->modulation_delta + 128)) >> 7;

        pan = 2 * track->pan + track->pan_extra;

        if (track->modulation_type == 2)
            pan += track->modulation_delta;

        if (pan < -128)
            pan = -128;
        else if (pan > 127)
            pan = 127;

        track->right_volume = (u32)((pan + 128) * volume) >> 8;
        track->left_volume = (u32)((127 - pan) * volume) >> 8;
    }

    if (track->flags & 4) {
        s32 bend = track->bend *track->bend_range;
        s32 pitch = (track->tune + bend)
                  * 4
                  + (track->key_shift << 8)
                  + (track->key_shift_extra << 8)
                  + track->pitch_extra;

        if (track->modulation_type == 0)
            pitch += 16 * track->modulation_delta;

        track->key_mod = pitch >> 8;
        track->pitch_mod = pitch;
    }

    track->flags &= ~(1 | 4);
}

/* audio/misc/cgb_key_to_frequency.c */
extern s16 gRom2[];
extern u8 gRom3[];

s32 Cgb_KeyToFrequency(
    s32 channel_type_arg,
    s32 key_arg,
    s32 pitch_arg)
{
    u8 channel_type = channel_type_arg;
    u8 key = key_arg;
    u8 pitch = pitch_arg;

    if (channel_type == 4) {
        if (key <= 20) {
            key = 0;
        } else {
            key -= 21;
            if (key > 59)
                key = 59;
        }
        return gRom3[key];
    }

    if (key <= 35) {
        pitch = 0;
        key = 0;
    } else {
        key -= 36;
        if (key > 130) {
            key = 130;
            pitch = 255;
        }
    }

    {
        s32 lower = RomBytes_080fb92c[key];
        s32 upper;
        lower = gRom2[lower & 15] >> (lower >> 4);
        upper = RomBytes_080fb92c[key + 1];
        upper = gRom2[upper & 15] >> (upper >> 4);
        return lower + ((pitch *(upper - lower)) >> 8) + 0x800;
    }
}

/* audio/ctrl/stop_cgb_oscillator.c */
// Approved reference provenance: adapted from pret/pokeemerald
// src/m4a.c:CgbOscOff at commit 83df84e40623b79281f2397faa611cbf044170bd.
// Register addresses and control flow were checked against this ROM region.

void Cgb_StopOscillator(u8 channel_id)
{
    volatile u8 *control;

    switch (channel_id) {
    case 1:
        control = (volatile u8 *)0x04000063;
        *control = 8;
        control += 2;
        break;
    case 2:
        control = (volatile u8 *)0x04000069;
        *control = 8;
        control += 4;
        break;
    case 3:
        *(volatile u8 *)0x04000070 = 0;
        return;
    default:
        control = (volatile u8 *)0x04000079;
        *control = 8;
        control += 4;
        break;
    }

    *control = 0x80;
}

/* audio/misc/update_cgb_pan_envelope.c */
struct CgbChannel
{
    u8 status_flags;
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
    u8 dummy1[2];
    u8 gate_time;
    u8 midi_key;
    u8 velocity;
    u8 priority;
    u8 rhythm_pan;
    u8 dummy3[3];
    u8 dummy5;
    s8 sustain_goal;
    u8 n4;
    u8 pan;
    u8 pan_mask;
    u8 modify;
    u8 length;
    u8 sweep;
    u32 frequency;
    const u8 *wave_pointer;
    const u8 *current_pointer;
    struct MusicTrackState *track;
    struct CgbChannel *previous_channel;
    struct CgbChannel *next_channel;
    u8 dummy4[8];
};

void CgbChannel_UpdatePanEnvelope(struct CgbChannel *channel)
{
    u32 right = channel->right_volume;
    u32 left = channel->left_volume;

    if ((u8)right >= (u8)left)
    {
        if (((u8)right >> 1) >= (u8)left)
        {
            channel->pan = 0x0F;
            goto clamp;
        }
    } else
    {
        if (((u8)left >> 1) >= (u8)right)
        {
            channel->pan = 0xF0;
            goto clamp;
        }
    }

    channel->pan = 0xFF;
    channel->envelope_goal =
        (u32)(channel->right_volume + channel->left_volume) >> 4;
    goto done;

clamp:
    channel->envelope_goal =
        (u32)(channel->right_volume + channel->left_volume) >> 4;
    if (channel->envelope_goal > 15)
        channel->envelope_goal = 15;

done:
    channel->sustain_goal =
        (channel->envelope_goal *channel->sustain + 15) >> 4;
    channel->pan &= channel->pan_mask;
}
