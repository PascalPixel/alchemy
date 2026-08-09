#include "types.h"

struct SongHeader_080faa58 {
    u8 track_count;
    u8 unknown01;
    u8 priority;
    u8 flags;
    s32 field04;
    s32 track_data[1];
};

struct MusicTrack_080faa58 {
    u8 status;
    u8 padding01[0x1f];
    s32 field20;
    u8 padding24[0x1c];
    s32 source;
    u8 padding44[0x0c];
};

struct MusicPlayer_080faa58 {
    struct SongHeader_080faa58 *song;
    s32 state;
    u8 maximum_tracks;
    u8 priority;
    u8 padding0a;
    u8 unlocked;
    s32 field0c;
    u8 padding10[0x1c];
    struct MusicTrack_080faa58 *tracks;
    s32 field30;
    u32 magic;
};

void Func_080f9ef8(struct MusicPlayer_080faa58 *player,
                   struct MusicTrack_080faa58 *track);
void Func_080fa83c(u8 flags);

void Func_080faa58(struct MusicPlayer_080faa58 *player,
                   struct SongHeader_080faa58 *song)
{
    struct MusicTrack_080faa58 *track;
    s32 track_index;
    s32 state;
    u8 unlocked;
    u32 priority;
    u32 reset;

    if (player->magic != 0x68736d53)
        return;

    unlocked = player->unlocked;
    priority = song->priority;
    if (unlocked != 0
        && !(((player->song == 0
               || (player->tracks->status & 0x40) == 0)
              && ((state = player->state),
                  (*(volatile u16 *)&player->state == 0 || state < 0)))
             || player->priority
                    <= (priority = *(volatile u8 *)&song->priority)))
        return;

    player->magic++;
    player->state = 0;
    player->song = song;
    player->field30 = song->field04;
    player->priority = priority;
    player->field0c = 0;

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
           && track_index < player->maximum_tracks) {
        Func_080f9ef8(player, track);
        track->status = 0xc0;
        track->field20 = 0;
        track->source = song->track_data[track_index];
        track_index++;
        track++;
    }
    while (track_index < player->maximum_tracks) {
        Func_080f9ef8(player, track);
        track->status = 0;
        track_index++;
        track++;
    }
    if ((song->flags & 0x80) != 0)
        Func_080fa83c(song->flags);
    player->magic = 0x68736d53;
}
