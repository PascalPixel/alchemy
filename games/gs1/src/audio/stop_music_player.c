#include "audio_engine_symbols.h"
#include "types.h"

void MusicTrack_Stop(void *, void *);

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
