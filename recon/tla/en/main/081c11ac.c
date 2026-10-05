/* SongFlag_Set, after SONG_FLAG.C's SongFlag_Test: differ 2. The native
   loads the flag bytes' address between the shift and the add that build
   0x3ff; this draft loads it after the add. Statement orders, a returned
   index, a division by 8 and 150 device-search steps did not move it.
   SongFlag_Clear (081c11cc) has the same difference. */
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
