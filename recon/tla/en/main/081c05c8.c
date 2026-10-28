// Draft, differ 20 of 11: the cut-off head of a work copy of the song start,
// as ☀️'s m4aSongNumStart (games/tbs/src/sound/audio.c): the copy stops
// after the player slot's offset, and the listing's last four words are the
// ARM 32x32 high multiply's Thumb entry (bx pc, umull, bx lr) that follows
// it. A sound-library function: the draft compiles with the game profile,
// where the build routes the library through old_agbcc. Other: the listing
// loads both table addresses first (r2, r1) and keeps the products in r1 and
// r3; the game compiler loads each table where it is used.
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
