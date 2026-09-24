/* Draft, not exact (2026-09-24): candidate=48 reference=46, 18 differing
   halfwords under the agbcc route. The reference tests the note state with
   `movs r0, #0xc7; tst r0, r1` and keeps the byte in r1 for the OR; neither
   agscc nor old_agbcc emits tst for any spelling tried (u8/u32/s32 local,
   != 0, compound |=). The body and its neighbour SoundNote_Detach
   (0x080f9a30) read like hand-written engine assembly rather than compiler
   output. */
#include "AUDIO_ENGINE.H"

void SoundNote_Detach(struct SoundNote *note);

void Func_080f9a50(struct SoundPlayer *player, struct SoundTrack *track)
{
    struct SoundNote *note = track->notes;

    if (note != NULL) {
        do {
            u8 state = note->state;

            if (state & 0xc7)
                note->state = state | 0x40;
            SoundNote_Detach(note);
            note = note->next;
        } while (note != NULL);
    }
    track->flags = 0;
}
