#include "audio_engine.h"

struct SoundTableEntry {
    u32 header;
    u16 player;
    u16 unknown06;
};

extern struct PlayerBootstrapRecord Data_080fc624[];
extern struct SoundTableEntry Data_080fc684[];

void MusicPlayer_StartSong(struct MusicPlayerState *player, u32 header);
void MusicPlayer_Stop(struct MusicPlayerState *player);
void Audio_ResumePlayer(struct MusicPlayerState *player);

extern u8 RomBytes_080f9675[];
extern u32 Value_00000008;
extern u8 gOv;
extern u8 Data_00000008[];
s32 AudioEngine_RunMixerTick(void);
s32 Func_080fa280(s32 player, u16 interval);

void Audio_Initialize(void)
{
    u16 count;

    Audio_Place((const void *)((u32)&RomBytes_080f9675 & ~1u),
        (void *)0x03007000, 0x04000100);
    AudioEngine_Initialize((struct AudioEngineState *)0x02003050);
    CgbAudio_Initialize((struct CgbChannel *)0x02004090);
    AudioEngine_SetMode(0x0097F800);

    count = (u32)&Value_00000008;
    if (count != 0) {
        struct PlayerBootstrapRecord *record = Data_080fc624;
        u32 remaining = count;
        do {
            struct MusicPlayerState *player = record->player;
            MusicPlayer_Initialize(player, record->tracks, record->max_tracks);
            player->config = record->config;
            player->memory_area = &gOv;
            record++;
            remaining--;
        } while (remaining != 0);
    }
}

void AudioEngine_RunMixer(void)
{
    AudioEngine_RunMixerTick();
}

void Audio_PlaySound(u16 audio_cue_id)
{
    struct PlayerBootstrapRecord *player_records = Data_080fc624;
    struct SoundTableEntry *audio_cue_table = Data_080fc684;
    struct SoundTableEntry *audio_cue = &audio_cue_table[audio_cue_id];
    struct PlayerBootstrapRecord *player_record = &player_records[audio_cue->player];

    MusicPlayer_StartSong(player_record->player, audio_cue->header);
}

void Audio_PlaySoundIfInactive(u16 audio_cue_id)
{
    struct PlayerBootstrapRecord *players = Data_080fc624;
    struct SoundTableEntry *audio_cue_table = Data_080fc684;
    struct SoundTableEntry *audio_cue = &audio_cue_table[audio_cue_id];
    struct MusicPlayerState *player = players[audio_cue->player].player;

    if (player->song_header_word != audio_cue->header) {
        MusicPlayer_StartSong(player, audio_cue->header);
    } else {
        s32 status = player->status;
        u16 low_status = *(volatile u16 *)&player->status;

        if (low_status == 0 || status < 0)
            MusicPlayer_StartSong(player, player->song_header_word);
    }
}

void Audio_PlayOrResumeSound(u16 audio_cue_id)
{
    struct PlayerBootstrapRecord *players = Data_080fc624;
    struct SoundTableEntry *audio_cue_table = Data_080fc684;
    struct SoundTableEntry *audio_cue = &audio_cue_table[audio_cue_id];
    struct MusicPlayerState *player = players[audio_cue->player].player;
    u32 current_header = player->song_header_word;
    u32 target_header = audio_cue->header;

    if (current_header != target_header) {
        MusicPlayer_StartSong(player, target_header);
    } else {
        s32 status = player->status;

        if ((u16)status == 0)
            MusicPlayer_StartSong(player, current_header);
        else if (status < 0)
            Audio_ResumePlayer(player);
    }
}

void Audio_StopSound(u16 audio_cue_id)
{
    struct PlayerBootstrapRecord *players = Data_080fc624;
    struct SoundTableEntry *audio_cue_table = Data_080fc684;
    struct SoundTableEntry *audio_cue = &audio_cue_table[audio_cue_id];
    struct MusicPlayerState *player = players[audio_cue->player].player;

    if (player->song_header_word == audio_cue->header)
        MusicPlayer_Stop(player);
}

void Audio_ResumeSound(u16 audio_cue_id)
{
    u32 table_offset = audio_cue_id;
    volatile struct PlayerBootstrapRecord *players;
    struct SoundTableEntry *audio_cue_table;
    struct SoundTableEntry *audio_cue;
    struct MusicPlayerState *player;
    u32 current_header;
    u32 target_header;
    u32 player_id;

    table_offset <<= 16;
    players = Data_080fc624;
    audio_cue_table = Data_080fc684;
    table_offset >>= 13;
    audio_cue = (struct SoundTableEntry *)((u8 *)audio_cue_table + table_offset);
    player_id = audio_cue->player;
    player = players[player_id].player;
    current_header = *(volatile u32 *)&player->song_header_word;
    target_header = audio_cue->header;

    if (current_header == target_header)
        Audio_ResumePlayer(player);
}

void Audio_StopAllPlayers(void)
{
    u32 player_count = (u16)(u32)Data_00000008;

    if (player_count != 0) {
        struct PlayerBootstrapRecord *record = Data_080fc624;
        u32 remaining = player_count;

        do {
            MusicPlayer_Stop(record->player);
            record++;
            remaining--;
        } while (remaining != 0);
    }
}

void MusicPlayer_Resume(struct MusicPlayerState *player)
{
    Audio_ResumePlayer(player);
}

void Audio_ResumeAllPlayers(void)
{
    u16 player_count = (u32)Data_00000008;

    if (player_count != 0) {
        struct PlayerBootstrapRecord *record = Data_080fc624;
        u32 remaining = player_count;

        do {
            Audio_ResumePlayer(record->player);
            record++;
            remaining--;
        } while (remaining != 0);
    }
}

void MusicPlayer_FadeOut(s32 player, u16 interval)
{
    Func_080fa280(player, interval);
}
