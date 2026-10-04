/* SongFlag_Clear: differ 2, the address load SongFlag_Set's draft (081c11ac)
   also misplaces. */
#include "TYPES.H"

extern u8 Data_02002f20[];

void SongFlag_Clear(s32 song)
{
    s32 mask;
    u8 *bytes;

    mask = 1 << (7 & song);
    bytes = Data_02002f20;
    song = (song & 0x3ff) >> 3;
    bytes[song] = (u8)(bytes[song] & ~mask);
}
