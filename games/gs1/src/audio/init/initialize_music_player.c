#include "audio_engine_symbols.h"
#include "types.h"

struct MusicPlayerState;

typedef void (*PlayerMainCallback)(struct MusicPlayerState *);

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

struct AudioEngineState {
    u32 ident;
    u8 unknown04[0x1c];
    PlayerMainCallback mplay_main_head;
    struct MusicPlayerState *music_player_head;
};

extern u8 Data_080f9c91;

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
    audio->mplay_main_head = (PlayerMainCallback)&Data_080f9c91;
    audio->ident = 0x68736d53;
    player->ident = 0x68736d53;
}
