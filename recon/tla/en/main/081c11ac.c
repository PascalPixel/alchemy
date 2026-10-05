/* Near miss: differ 2, the split-constant scheduling gap. Written as
   SongFlag_Test is. The listing loads the flag bytes' address (ldr r1)
   between the two halves of 0x3ff (after lsls r3, r3, #2, before adds r3,
   #255); this build finishes the constant first. Inline index expressions
   (28) or a mask local alone (16) are further off. Goes after
   SongFlag_Test in SONG_FLAG.C. */
#include "TYPES.H"

extern u8 Data_02002f20[];

void SongFlag_Set(s32 song)
{
    s32 mask;
    u8 *bytes;

    mask = 1 << (7 & song);
    song = (song & 0x3ff) >> 3;
    bytes = Data_02002f20;
    bytes[song] |= mask;
}
