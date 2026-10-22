#include "unit/object/battle_cue.h"
#include "types.h"

/// Battle music: each song as 0x8000 | song, followed by the encounters that
/// use it as (zone << 4) + slot, ended by 0 (FieldFx_SelectBattleCue).
/// The songs are MUS_054, MUS_053, MUS_052, MUS_076, MUS_051 and MUS_BATTLE.
/// MUS_BATTLE is listed last with no encounters: a key the list does not hold,
/// such as the Djinni battles' 0, ends the search with it, so it is every
/// other battle's music. Link battles skip the list and take
/// AUDIO_CUE_KEEP_BGM.
const u16 gEncounter_BattleMusic[35] = {
    0x8036, 0x0620, 0x8035, 0x0621, 0x0545, 0x8034, 0x0240, 0x0351, 0x0353,
    0x0355, 0x03e3, 0x0624, 0x0626, 0x0627, 0x03f0, 0x03f1, 0x03f2, 0x03f3,
    0x0540, 0x0541, 0x0542, 0x0543, 0x0544, 0x804c, 0x0480, 0x0481, 0x0482,
    0x0483, 0x0484, 0x0485, 0x8033, 0x00c4, 0x0242, 0x8032, 0x0000,
};
