#include "GLOBAL.H"
#include "TYPES.H"


u16 Audio_GetBackgroundCue(void)
{
    return Data_02005830;
}

s32 SongFlag_Test(s32 song)
{
    s32 mask;
    u8 *bytes;
    s32 value;

    mask = 1 << (7 & song);
    song = (song & 0x3ff) >> 3;
    bytes = gSongFlags;
    value = bytes[song] & mask;
    return (u32)(-value | value) >> 31;
}
