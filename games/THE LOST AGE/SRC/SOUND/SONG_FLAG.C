#include "TYPES.H"

extern u16 Data_02005830;
/* One bit for each of 1024 song numbers, kept as GameFlagBytes keeps the
   game's flags. */
extern u8 Data_02002f20[];

u16 Func_081c117c(void)
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
    bytes = Data_02002f20;
    value = bytes[song] & mask;
    return (s32)((u32)((0 - value) | value) >> 31);
}
