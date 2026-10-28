#ifndef ALCHEMY_MODULE_OVERWORLD_BATTLE_CUE_H
#define ALCHEMY_MODULE_OVERWORLD_BATTLE_CUE_H

#include "types.h"

/// The bit that marks a song in gEncounter_BattleMusic; encounter keys have it
/// clear.
#define ENCOUNTER_BATTLE_MUSIC_MARKER 0x8000
/// The bits of a song marker that hold the song.
#define ENCOUNTER_BATTLE_MUSIC_SONG_MASK 0xfff
/// A song marker in gEncounter_BattleMusic: the song (a MUS_ id) with
/// ENCOUNTER_BATTLE_MUSIC_MARKER set.
#define ENCOUNTER_BATTLE_MUSIC(song) (ENCOUNTER_BATTLE_MUSIC_MARKER | (song))
/// The key of one battle: slot of zone (an enum EncounterZoneId).
#define ENCOUNTER_KEY(zone, slot) (((zone) << 4) + (slot))

extern const u16 gEncounter_BattleMusic[35];

#endif
