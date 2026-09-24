/* Draft, not exact (2026-09-24): with the owner routed to the agbcc family
   this is candidate=26 reference=26 (owner extent 26, then an alignment
   halfword) with 8 differing halfwords, all register choice: the reference
   reuses the zero register r2 for the modulation-type load and the flag
   bits, the candidate takes r0. Under the game route it is 4 bytes longer. */
#include "AUDIO_ENGINE.H"

void MusicTrack_ClearModulationState(struct SoundPlayer *player, struct SoundTrack *track)
{
    u8 *bytes = (u8 *)track;

    bytes[0x16] = 0;
    bytes[0x1a] = 0;
    if (bytes[0x18] == 0)
        track->flags |= 12;
    else
        track->flags |= 3;
}
