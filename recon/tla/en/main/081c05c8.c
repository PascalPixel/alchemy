// Blocked, 5 November 2026: the cut-off head of a work copy of the song start, as
// ☀️'s m4aSongNumStart (games/tbs/src/sound/audio.c): the copy stops after the
// player slot's offset and runs straight into the ARM 32x32 high multiply's Thumb
// entry (bx pc, umull, bx lr). No compiler emits a function cut off like that.
#include "types.h"
#include "sound_driver.h"

void MPlayStart(struct SoundPlayer *player,
                           const struct SongHeader *header);
extern const struct PlayerSlot gSound_PlayerSlots[];
extern const struct SongEntry gSongTable[];

void Func_081c05c8(u16 id) {
  const struct PlayerSlot *slots = gSound_PlayerSlots;
  const struct SongEntry *songs = gSongTable;
  const struct SongEntry *song = &songs[id];

  MPlayStart(slots[song->slot].player, song->header);
}
