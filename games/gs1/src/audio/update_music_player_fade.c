// Approved reference provenance: adapted from pret/pokeemerald
// src/m4a.c:FadeOutBody at commit 83df84e40623b79281f2397faa611cbf044170bd.
// Names and layout were checked against this ROM region and exact compiler output.
#include "audio_engine_symbols.h"
#include "types.h"

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

void Func_080f9ef8(
    struct FadePlayerState *player,
    struct FadeTrackState *track);

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

                Func_080f9ef8(player, track);

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
