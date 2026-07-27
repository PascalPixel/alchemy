typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

struct MusicPlayerState;

typedef void (*PlayerMainCallback)(struct MusicPlayerState *);

struct MusicTrackState {
    s8 status;
    u8 unknown01[0x4f];
};

struct MusicPlayerState {
    u32 unknown00;
    u32 status;
    u8 track_count;
    u8 priority;
    u8 unknown0a;
    u8 config;
    u8 unknown0c[0x0c];
    u8 *memory_area;
    u8 unknown1c[0x10];
    struct MusicTrackState *tracks;
    void *unknown30;
    u32 ident;
    PlayerMainCallback next_callback;
    struct MusicPlayerState *next_player;
};

struct AudioEngineState {
    u32 ident;
    u8 unknown04[0x1c];
    PlayerMainCallback player_main;
    struct MusicPlayerState *player_head;
};

void Func_080fa68c(struct MusicPlayerState *player);

void Func_080fa9e0(
    struct MusicPlayerState *player,
    struct MusicTrackState *tracks_arg,
    u32 count_arg)
{
    register struct MusicTrackState *tracks = tracks_arg;
    u8 count = count_arg;
    struct AudioEngineState *audio_state;

    if (count == 0)
        return;
    if (count > 16)
        count = 16;

    audio_state = *(struct AudioEngineState **)0x03007ff0;
    if (audio_state->ident != 0x68736d53)
        return;

    audio_state->ident++;
    Func_080fa68c(player);
    player->tracks = tracks;
    player->track_count = count;
    player->status = 0x80000000;

    if (count != 0) {
        do {
            u32 next;
            tracks->status = 0;
            next = count - 1;
            count = next;
            tracks++;
        } while (count != 0);
    }

    if (audio_state->player_main != 0) {
        player->next_callback = audio_state->player_main;
        player->next_player = audio_state->player_head;
        audio_state->player_main = 0;
    }

    audio_state->player_head = player;
    audio_state->player_main = (PlayerMainCallback)0x080f9c91;
    audio_state->ident = 0x68736d53;
    player->ident = 0x68736d53;
}
