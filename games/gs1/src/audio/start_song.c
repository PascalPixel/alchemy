#include "audio_engine_symbols.h"
#include "types.h"

struct SongHeader {
    u8 track_count;
    u8 block_count;
    u8 priority;
    u8 flags;
    s32 voice_group;
    s32 track_data[1];
};

struct SongStartTrack {
    u8 status;
    u8 padding01[0x1f];
    s32 channel_head;
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

void Func_080f9ef8(
    struct SongStartPlayer *player,
    struct SongStartTrack *track);
void AudioEngine_SetMode(u8 flags);

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
               || (player->tracks->status & 0x40) == 0)
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
        Func_080f9ef8(player, track);
        track->status = 0xc0;
        track->channel_head = 0;
        track->command = song->track_data[track_index];
        track_index++;
        track++;
    }
    while (track_index < player->track_capacity) {
        Func_080f9ef8(player, track);
        track->status = 0;
        track_index++;
        track++;
    }
    if ((song->flags & 0x80) != 0)
        AudioEngine_SetMode(song->flags);
    player->ident = 0x68736d53;
}
