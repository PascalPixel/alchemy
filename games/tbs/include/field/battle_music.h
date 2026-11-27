#ifndef ALCHEMY_BATTLE_MUSIC_H
#define ALCHEMY_BATTLE_MUSIC_H

/* The battles that play music of their own (battle_music.c). */

#include "types.h"

/* The bit that marks a song in EncounterBattleMusic; encounter keys have it
   clear. */
#define ENCOUNTER_BATTLE_MUSIC_MARKER 0x8000
/* The bits of a song marker that hold the song. */
#define ENCOUNTER_BATTLE_MUSIC_MASK	  0xfff
/* A song marker in EncounterBattleMusic: the song (a MUS_ id) with
   ENCOUNTER_BATTLE_MUSIC_MARKER set. */
#define ENCOUNTER_BATTLE_MUSIC(song)  (ENCOUNTER_BATTLE_MUSIC_MARKER | (song))
/* The key of one battle: slot of zone (an ENCOUNTER_ZONE_ID). */
#define ENCOUNTER_KEY(zone, slot)	  (((zone) << 4) + (slot))

extern const Uint16 EncounterBattleMusic[35];

#endif
