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
    u8 padding01[0x1F];
    s32 field20;
    u8 padding24[0x1C];
    s32 source;
    u8 padding44[0x0C];
};

void Func_080f9ef8(void *, struct MusicTrack_080faa58 *);
void Func_080fa83c(u8);

/*
 * Replace the active song when the player is idle or the new song has enough
 * priority. Reset and populate the used tracks, clear every remaining track,
 * apply the song flag, and release the player's update lock.
 */
void Func_080faa58(void *player_pointer, const struct SongHeader_080faa58 *song) {
    u8 *player;
    struct MusicTrack_080faa58 *track;
    s32 player_state;
    s32 track_index;
    u8 maximum_tracks;
    u8 current_priority;
    u8 unlocked;
    u32 magic;

    player = player_pointer;
    magic = *(u32 *)(player + 0x34);
    if (magic != 0x68736D53) {
        return;
    }

    player_state = *(s32 *)(player + 4);
    maximum_tracks = player[8];
    current_priority = player[9];
    unlocked = player[0xB];
    track = *(struct MusicTrack_080faa58 **)(player + 0x2C);
    if (unlocked != 0 &&
        !(((*(void **)player == 0 || (track->status & 0x40) == 0) &&
           ((u16)player_state == 0 || player_state < 0)) ||
          current_priority <= song->priority)) {
        return;
    }

    *(u32 *)(player + 0x34) = magic + 1;
    *(s32 *)(player + 4) = 0;
    *(const struct SongHeader_080faa58 **)player = song;
    *(s32 *)(player + 0x30) = song->field04;
    player[9] = song->priority;
    *(s32 *)(player + 0x0C) = 0;
    *(s16 *)(player + 0x1C) = 0x96;
    *(s16 *)(player + 0x20) = 0x96;
    *(s16 *)(player + 0x1E) = 0x100;
    *(s16 *)(player + 0x22) = 0;
    *(s16 *)(player + 0x24) = 0;

    track_index = 0;
    while (track_index < song->track_count &&
           track_index < maximum_tracks) {
        Func_080f9ef8(player, track);
        track->status = 0xC0;
        track->field20 = 0;
        track->source = song->track_data[track_index];
        track_index++;
        track++;
    }
    while (track_index < maximum_tracks) {
        Func_080f9ef8(player, track);
        track->status = 0;
        track_index++;
        track++;
    }

    if (song->flags & 0x80) {
        Func_080fa83c(song->flags);
    }
    *(u32 *)(player + 0x34) = 0x68736D53;
}
