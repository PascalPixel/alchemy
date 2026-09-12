#include "audio_engine_symbols.h"
#include "types.h"
#include "scene.h"

/* audio/init/initialize.c */
struct CgbChannel;
struct MusicPlayerState;
struct MusicTrackState;
struct AudioEngineState;

typedef void (*PlayerMainCallback)(struct MusicPlayerState *);

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

struct PlayerBootstrapRecord {
    struct MusicPlayerState *player;
    struct MusicTrackState *tracks;
    u8 max_tracks;
    u8 unknown09;
    u16 config;
};

void CgbAudio_Initialize(struct CgbChannel *);
void AudioEngine_Initialize(struct AudioEngineState *);
void AudioEngine_SetMode(u32 value);
void MusicPlayer_Initialize(
    struct MusicPlayerState *,
    struct MusicTrackState *,
    u32);
extern u8 gRom;
extern u32 Value_00000008;
extern u8 gOv;
extern struct PlayerBootstrapRecord gRom2[];

void Audio_Initialize(void)
{
    u16 count;

    Audio_Place((s32)&gRom & ~1, 0x03007000, 0x04000100);
    AudioEngine_Initialize((struct AudioEngineState *)0x02003050);
    CgbAudio_Initialize((struct CgbChannel *)0x02004090);
    AudioEngine_SetMode(0x0097F800);

    count = (u32)&Value_00000008;
    if (count != 0) {
        struct PlayerBootstrapRecord *record = gRom2;
        u32 remaining = count;
        do {
            struct MusicPlayerState *player = record->player;
            MusicPlayer_Initialize(
                player,
                record->tracks,
                record->max_tracks);
            player->config = record->config;
            player->memory_area = &gOv;
            record++;
            remaining--;
        } while (remaining != 0);
    }
}

/* audio/mix/run_mixer.c */
s32 AudioEngine_RunMixerTick(void);

void AudioEngine_RunMixer(void)
{
    AudioEngine_RunMixerTick();
}

/* audio/play/play_sound.c */
typedef struct {
    s32 header;
    u16 player;
} SoundTableEntry;

extern s32 gRom[];
extern SoundTableEntry gRom2[];

void MusicPlayer_StartSong(s32, s32);

void Audio_PlaySound(u16 audio_cue_id)
{
    s32 *player_records = gRom;
    SoundTableEntry *audio_cue_table = gRom2;
    SoundTableEntry *audio_cue = &audio_cue_table[audio_cue_id];
    s32 *player_record = &player_records[audio_cue->player * 3];

    MusicPlayer_StartSong(player_record[0], audio_cue->header);
}

/* audio/play/play_sound_if_inactive.c */
struct SoundTableEntry {
    u32 header;
    u16 player;
    u16 padding;
};

struct MusicPlayerView {
    u32 song_header;
    s32 status;
    u32 unknown08;
};

void MusicPlayer_StartSong(struct MusicPlayerView *, u32);
extern struct MusicPlayerView *gRom[];
extern struct SoundTableEntry gRom2[];

void Audio_PlaySoundIfInactive(u16 audio_cue_id)
{
    struct MusicPlayerView **players = gRom;
    struct SoundTableEntry *audio_cue_table = gRom2;
    struct SoundTableEntry *audio_cue = &audio_cue_table[audio_cue_id];
    struct MusicPlayerView *player = players[audio_cue->player * 3];

    if (player->song_header != audio_cue->header) {
        MusicPlayer_StartSong(player, audio_cue->header);
    } else {
        s32 status = player->status;
        u16 low_status = *(volatile u16 *)&player->status;

        if (low_status == 0 || status < 0)
            MusicPlayer_StartSong(player, player->song_header);
    }
}

/* audio/play/play_or_resume_sound.c */
typedef struct {
    s32 header;
    u16 player;
} SoundTableEntry;

extern s32 *gRom[];
extern SoundTableEntry gRom2[];

void MusicPlayer_StartSong(void *, s32);
void Audio_ResumePlayer(void *);

void Audio_PlayOrResumeSound(u16 audio_cue_id)
{
    s32 **players = gRom;
    SoundTableEntry *audio_cue_table = gRom2;
    SoundTableEntry *audio_cue = &audio_cue_table[audio_cue_id];
    s32 *player = players[audio_cue->player * 3];
    s32 current_header = player[0];
    s32 target_header = audio_cue->header;

    if (current_header != target_header) {
        MusicPlayer_StartSong(player, target_header);
    } else {
        s32 status = player[1];

        if ((u16)status == 0)
            MusicPlayer_StartSong(player, current_header);
        else if (status < 0)
            Audio_ResumePlayer(player);
    }
}

/* audio/ctrl/stop_sound.c */
typedef struct {
    u32 header;
    u16 player;
} SoundTableEntry;

void MusicPlayer_Stop(void *player);
extern void *gRom[];
extern SoundTableEntry gRom2[];

void Audio_StopSound(u16 audio_cue_id)
{
    void **players = gRom;
    SoundTableEntry *audio_cue_table = gRom2;
    SoundTableEntry *audio_cue = &audio_cue_table[audio_cue_id];
    void *player = players[audio_cue->player * 3];

    if (*(u32 *)player == audio_cue->header)
        MusicPlayer_Stop(player);
}

/* audio/ctrl/resume_sound.c */
typedef struct {
    u32 header;
    u16 player;
} SoundTableEntry;

void Audio_ResumePlayer(void *player);
extern void *gRom[];
extern SoundTableEntry gRom2[];

void Audio_ResumeSound(u16 audio_cue_id)
{
    u32 table_offset = audio_cue_id;
    void *volatile *players;
    SoundTableEntry *audio_cue_table;
    SoundTableEntry *audio_cue;
    void *player;
    u32 current_header;
    u32 target_header;
    u32 player_id;
    u32 player_slot;

    table_offset <<= 16;
    players = gRom;
    audio_cue_table = gRom2;
    table_offset >>= 13;
    audio_cue = (SoundTableEntry *)((unsigned char *)audio_cue_table + table_offset);
    player_id = audio_cue->player;
    player_slot = player_id << 1;
    player_slot += player_id;
    player = players[player_slot];
    current_header = *(volatile u32 *)player;
    target_header = audio_cue->header;

    if (current_header == target_header)
        Audio_ResumePlayer(player);
}

/* audio/ctrl/stop_all_players.c */
extern u8 gVal[];
extern u8 gRom[];
void MusicPlayer_Stop(u8 *player);

void Audio_StopAllPlayers(void)
{
    u32 player_count = (u16)(u32)gVal;

    if (player_count != 0) {
        u8 *record = gRom;
        u32 remaining = player_count;

        do {
            MusicPlayer_Stop(*(void **)record);
            record += 12;
            remaining--;
        } while (remaining != 0);
    }
}

/* audio/ctrl/resume_music_player.c */
void Audio_ResumePlayer(void);

void MusicPlayer_Resume(void)
{
    Audio_ResumePlayer();
}

/* audio/ctrl/resume_all_players.c */
extern u8 gVal[];
extern s32 gRom[];

void Audio_ResumePlayer(s32);

void Audio_ResumeAllPlayers(void)
{
    u16 player_count = (u32)gVal;

    if (player_count != 0) {
        s32 *record = gRom;
        u32 remaining = player_count;

        do {
            Audio_ResumePlayer(*record);
            record += 3;
            remaining--;
        } while (remaining != 0);
    }
}

/* audio/ctrl/fade_out_music_player.c */
s32 MusicPlayer_BeginFadeOut(s32, u16);

void MusicPlayer_FadeOut(s32 player, u16 interval)
{
    MusicPlayer_BeginFadeOut(player, interval);
}

/* audio/init/initialize_cgb.c */
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
};

void MusicPlayer_ExecuteMemoryAccessCommand(
    struct MusicPlayerState *,
    struct MusicTrackState *);
void MusicTrack_SetLfoSpeedFromCommand(struct MusicPlayerState *, struct MusicTrackState *);
void MusicTrack_SetModulationFromCommand(struct MusicPlayerState *, struct MusicTrackState *);
void MusicTrack_DispatchExtendedCommand(struct MusicPlayerState *, struct MusicTrackState *);
void MusicTrack_EndTie(struct MusicPlayerState *, struct MusicTrackState *);
void AudioEngine_SetPcmRate(u32);
void MusicTrack_Stop(struct MusicPlayerState *, struct MusicTrackState *);
void MusicPlayer_UpdateFade(struct MusicPlayerState *);
void MusicTrack_UpdateVolumePitch(
    struct MusicPlayerState *,
    struct MusicTrackState *);
void CgbAudio_Update(void);
void Cgb_StopOscillator(u8);
s32 Cgb_KeyToFrequency(s32, s32, s32);
extern u8 Value_00000000;

void CgbAudio_Initialize(struct CgbChannel *channels)
{
    u32 zero;
    struct AudioEngineState *state;
    union AudioCommandSlot *mplay_jump_table;
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

    state = *(struct AudioEngineState *volatile *)0x03007ff0;
    ident = state->ident;
    if (ident != 0x68736d53)
        return;

    state->ident = ident + 1;

    mplay_jump_table = (union AudioCommandSlot *)0x02004000;
    mplay_jump_table[8].player_track = MusicPlayer_ExecuteMemoryAccessCommand;
    mplay_jump_table[17].player_track = MusicTrack_SetLfoSpeedFromCommand;
    mplay_jump_table[19].player_track = MusicTrack_SetModulationFromCommand;
    mplay_jump_table[28].player_track = MusicTrack_DispatchExtendedCommand;
    mplay_jump_table[29].player_track = MusicTrack_EndTie;
    mplay_jump_table[30].word = AudioEngine_SetPcmRate;
    mplay_jump_table[31].player_track = MusicTrack_Stop;
    mplay_jump_table[32].player = MusicPlayer_UpdateFade;
    mplay_jump_table[33].player_track = MusicTrack_UpdateVolumePitch;

    state->cgb_channels = channels;
    state->cgb_sound = CgbAudio_Update;
    state->cgb_osc_off.handler = Cgb_StopOscillator;
    state->midi_key_to_cgb_freq.handler = Cgb_KeyToFrequency;
    state->max_lines = (u32)&Value_00000000;

    zero = 0;
    Audio_Place(
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
