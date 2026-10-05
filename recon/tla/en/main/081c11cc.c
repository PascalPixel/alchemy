/* Near miss: differ 2, the same split-constant scheduling gap as
   SongFlag_Set: the listing loads the flag bytes' address between the two
   halves of 0x3ff. Goes after SongFlag_Set in SONG_FLAG.C. */
#include "TYPES.H"

extern u8 Data_02002f20[];

void SongFlag_Clear(s32 song)
{
    s32 mask;
    u8 *bytes;

    mask = 1 << (7 & song);
    song = (song & 0x3ff) >> 3;
    bytes = Data_02002f20;
    bytes[song] &= ~mask;
}
