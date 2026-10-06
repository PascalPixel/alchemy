#include "TYPES.H"

extern u8 gSongFlags[];

void SongFlag_Set(s32 song)
{
    s32 mask;
    u8 *bytes;

    mask = 1 << (7 & song);
    bytes = gSongFlags;
    song = (song & 0x3ff) >> 3;
    bytes[song] = (u8)(bytes[song] | mask);
}
