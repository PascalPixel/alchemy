/// @file
/// The battle sprite rows, by unit id: the eight characters', then the
/// enemies'. Each enemy's name is the English release's.

#include "enemy_ids.h"
#include "sprite_ids.h"
#include "battle/battle_summon.h"
#include "game/character_ids.h"

// clang-format off
struct BattleSpriteDefinition gBattleSprites[] = {
    // The characters' rows.
    [ISAAC] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .attackEffect = 1,
        .attackHop = 5,
    },
    [GARET] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .attackEffect = 1,
        .attackHop = 4,
    },
    [IVAN] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .attackEffect = 4,
        .attackHop = 2,
    },
    [MIA] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .attackEffect = 4,
        .attackHop = 2,
    },
    [FELIX] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .attackEffect = 1,
        .attackHop = 5,
    },
    [JENNA] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .attackEffect = 1,
        .attackHop = 5,
    },
    [SHEBA] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .attackEffect = 4,
        .attackHop = 5,
    },
    [PIERS] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .attackEffect = 1,
        .attackHop = 5,
    },
    // ???
    [ENEMY_NONE] = {
        .animation = 0,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_UNUSED_QUARTZ] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_UNUSED_VINE] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_UNUSED_BANE] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_UNUSED_ECHO] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_IRON] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_STEEL] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_MUD] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_FLOWER] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_MELD] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_PETRA] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_UNUSED_SALT] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_UNUSED_GEODE] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_UNUSED_MOLD] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_CRYSTAL] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_UNUSED_SLEET] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_UNUSED_SPRITZ] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_UNUSED_HAIL] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_UNUSED_DEW] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_FOG] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_SOUR] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_UNUSED_SPRING] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_UNUSED_SHADE] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_CHILL] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_STEAM] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_UNUSED_RIME] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_UNUSED_GEL] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_EDDY] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_BALM] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_SERAC] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_UNUSED_FORGE] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_UNUSED_CORONA] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_UNUSED_FLASH] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_CANNON] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_UNUSED_SPARK] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_KINDLE] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_UNUSED_CHAR] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_UNUSED_COAL] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_REFLUX] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_CORE] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_UNUSED_TINDER] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_UNUSED_SHINE] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_FURY] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_FUGUE] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_UNUSED_BREEZE] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_UNUSED_ZEPHYR] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_UNUSED_SQUALL] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_UNUSED_LUFF] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_BREATH] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_BLITZ] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_UNUSED_ETHER] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_WAFT] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_UNUSED_HAZE] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_WHEEZE] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_UNUSED_AROMA] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_WHORL] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_GASP] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_UNUSED_LULL] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_GALE] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Unused: "?"
    [ENEMY_ID_FIRST + 59] = {
        .animation = 0,
    },
    // Ruffian
    [ENEMY_RUFFIAN] = {
        .animation = SPRITE_BATTLE_RUFFIAN,
        .attackEffect = 1,
        .attackHop = 4,
        .defeatSound = 1,
    },
    // Ruffian 2
    [ENEMY_UNUSED_RUFFIAN_A] = {
        .animation = SPRITE_BATTLE_RUFFIAN,
        .attackEffect = 1,
        .attackHop = 4,
        .defeatSound = 1,
    },
    // Ruffian 3
    [ENEMY_UNUSED_RUFFIAN_B] = {
        .animation = SPRITE_BATTLE_RUFFIAN,
        .attackEffect = 1,
        .attackHop = 4,
        .defeatSound = 1,
    },
    // Punch Ant
    [ENEMY_PUNCH_ANT] = {
        .animation = SPRITE_BATTLE_PUNCH_ANT,
        .isSmall = 1,
        .attackEffect = 6,
        .defeatSound = 1,
    },
    // Flash Ant
    [ENEMY_FLASH_ANT] = {
        .animation = SPRITE_BATTLE_PUNCH_ANT,
        .isSmall = 1,
        .attackEffect = 6,
        .palette = 3,
        .defeatSound = 1,
    },
    // Numb Ant
    [ENEMY_NUMB_ANT] = {
        .animation = SPRITE_BATTLE_PUNCH_ANT,
        .isSmall = 1,
        .attackEffect = 6,
        .palette = 2,
        .defeatSound = 1,
    },
    // Chestbeater
    [ENEMY_CHESTBEATER] = {
        .animation = SPRITE_BATTLE_CHESTBEATER,
        .attackEffect = 1,
        .attackHop = 6,
        .defeatSound = 2,
    },
    // Wild Gorilla
    [ENEMY_WILD_GORILLA] = {
        .animation = SPRITE_BATTLE_CHESTBEATER,
        .attackEffect = 1,
        .attackHop = 6,
        .palette = 3,
        .defeatSound = 2,
    },
    // Crazy Gorilla
    [ENEMY_CRAZY_GORILLA] = {
        .animation = SPRITE_BATTLE_CHESTBEATER,
        .attackEffect = 1,
        .attackHop = 6,
        .palette = 5,
        .defeatSound = 2,
    },
    // King Scorpion
    [ENEMY_KING_SCORPION] = {
        .animation = SPRITE_BATTLE_KING_SCORPION,
        .attackEffect = 4,
        .defeatSound = 2,
    },
    // Devil Scorpion
    [ENEMY_DEVIL_SCORPION] = {
        .animation = SPRITE_BATTLE_KING_SCORPION,
        .attackEffect = 4,
        .palette = 3,
        .defeatSound = 2,
    },
    // Sand Scorpion
    [ENEMY_SAND_SCORPION] = {
        .animation = SPRITE_BATTLE_KING_SCORPION,
        .attackEffect = 4,
        .palette = 5,
        .defeatSound = 2,
    },
    // Briggs
    [ENEMY_BRIGGS] = {
        .animation = SPRITE_BATTLE_BRIGGS,
        .attackEffect = 5,
        .defeatSound = 1,
    },
    // Sea Fighter
    [ENEMY_SEA_FIGHTER] = {
        .animation = SPRITE_BATTLE_SEA_FIGHTER,
        .attackEffect = 5,
        .defeatSound = 1,
    },
    // Champa 2
    [ENEMY_UNUSED_CHAMPA_A] = {
        .animation = SPRITE_BATTLE_SEA_FIGHTER,
        .attackEffect = 5,
        .defeatSound = 1,
    },
    // Champa 3
    [ENEMY_UNUSED_CHAMPA_B] = {
        .animation = SPRITE_BATTLE_SEA_FIGHTER,
        .attackEffect = 5,
        .defeatSound = 1,
    },
    // Cuttle
    [ENEMY_CUTTLE] = {
        .animation = SPRITE_BATTLE_CUTTLE,
        .isSmall = 1,
        .attackEffect = 3,
    },
    // Calamar
    [ENEMY_CALAMAR] = {
        .animation = SPRITE_BATTLE_CUTTLE,
        .isSmall = 1,
        .attackEffect = 3,
        .palette = 5,
    },
    // Man o' War
    [ENEMY_MAN_O_WAR] = {
        .animation = SPRITE_BATTLE_CUTTLE,
        .isSmall = 1,
        .attackEffect = 3,
        .palette = 3,
    },
    // Aqua Jelly
    [ENEMY_AQUA_JELLY] = {
        .animation = SPRITE_BATTLE_CUTTLE,
        .isSmall = 1,
        .attackEffect = 3,
    },
    // Aqua Hydra
    [ENEMY_AQUA_HYDRA] = {
        .animation = SPRITE_BATTLE_AQUA_HYDRA,
        .attackEffect = 1,
        .attackHop = 4,
        .palette = 1,
        .defeatSound = 1,
    },
    // Hydra
    [ENEMY_HYDRA] = {
        .animation = SPRITE_BATTLE_AQUA_HYDRA,
        .attackEffect = 1,
        .attackHop = 4,
        .defeatSound = 1,
    },
    // Pyrodra
    [ENEMY_PYRODRA] = {
        .animation = SPRITE_BATTLE_AQUA_HYDRA,
        .attackEffect = 1,
        .attackHop = 4,
        .palette = 3,
        .defeatSound = 1,
    },
    // Serpent
    [ENEMY_SERPENT] = {
        .animation = SPRITE_BATTLE_SERPENT,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = 3,
    },
    // Serpent
    [ENEMY_SERPENT_ONE_BEAM] = {
        .animation = SPRITE_BATTLE_SERPENT,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = 3,
    },
    // Serpent
    [ENEMY_SERPENT_TWO_BEAMS] = {
        .animation = SPRITE_BATTLE_SERPENT,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = 3,
    },
    // Serpent
    [ENEMY_SERPENT_THREE_BEAMS] = {
        .animation = SPRITE_BATTLE_SERPENT,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = 3,
    },
    // Serpent
    [ENEMY_SERPENT_FOUR_BEAMS] = {
        .animation = SPRITE_BATTLE_SERPENT,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = 3,
    },
    // Serpent 6
    [ENEMY_UNUSED_SERPENT_A] = {
        .animation = SPRITE_BATTLE_SERPENT,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = 3,
    },
    // Serpent 7
    [ENEMY_UNUSED_SERPENT_B] = {
        .animation = SPRITE_BATTLE_SERPENT,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = 3,
    },
    // Avimander
    [ENEMY_AVIMANDER] = {
        .animation = SPRITE_BATTLE_AVIMANDER,
        .attackEffect = 5,
        .heavyImpact = 1,
        .defeatSound = 1,
    },
    // Macetail
    [ENEMY_MACETAIL] = {
        .animation = SPRITE_BATTLE_AVIMANDER,
        .attackEffect = 5,
        .heavyImpact = 1,
        .palette = 1,
        .defeatSound = 1,
    },
    // Bombander
    [ENEMY_BOMBANDER] = {
        .animation = SPRITE_BATTLE_AVIMANDER,
        .attackEffect = 5,
        .heavyImpact = 1,
        .palette = 4,
        .defeatSound = 1,
    },
    // Poseidon
    [ENEMY_POSEIDON] = {
        .animation = SPRITE_BATTLE_POSEIDON,
        .attackEffect = 7,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Poseidon
    [ENEMY_POSEIDON_SECOND_ACTION] = {
        .animation = SPRITE_BATTLE_POSEIDON,
        .attackEffect = 7,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Poseidon 3
    [ENEMY_UNUSED_POSEIDON] = {
        .animation = SPRITE_BATTLE_POSEIDON,
        .attackEffect = 7,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Moapa
    [ENEMY_MOAPA] = {
        .animation = SPRITE_BATTLE_MOAPA,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = 1,
    },
    // Moapa
    [ENEMY_MOAPA_B] = {
        .animation = SPRITE_BATTLE_MOAPA,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = 1,
    },
    // Moapa
    [ENEMY_MOAPA_C] = {
        .animation = SPRITE_BATTLE_MOAPA,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = 1,
    },
    // Knight
    [ENEMY_KNIGHT] = {
        .animation = SPRITE_BATTLE_KNIGHT,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = 1,
    },
    // Knight
    [ENEMY_KNIGHT_B] = {
        .animation = SPRITE_BATTLE_KNIGHT,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = 1,
    },
    // Knight 3
    [ENEMY_UNUSED_KNIGHT] = {
        .animation = SPRITE_BATTLE_KNIGHT,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = 1,
    },
    // Agatio
    [ENEMY_AGATIO] = {
        .animation = SPRITE_BATTLE_AGATIO,
        .attackEffect = 7,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Agatio
    [ENEMY_AGATIO_B] = {
        .animation = SPRITE_BATTLE_AGATIO,
        .attackEffect = 7,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Agatio
    [ENEMY_AGATIO_VS_ISAAC] = {
        .animation = SPRITE_BATTLE_AGATIO,
        .attackEffect = 7,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Karst
    [ENEMY_KARST] = {
        .animation = SPRITE_BATTLE_KARST,
        .attackEffect = 5,
        .defeatSound = 1,
    },
    // Karst
    [ENEMY_KARST_B] = {
        .animation = SPRITE_BATTLE_KARST,
        .attackEffect = 5,
        .defeatSound = 1,
    },
    // Karst
    [ENEMY_KARST_VS_ISAAC] = {
        .animation = SPRITE_BATTLE_KARST,
        .attackEffect = 5,
        .defeatSound = 1,
    },
    // Unused: "?"
    [ENEMY_ID_FIRST + 108] = {
        .animation = 0,
    },
    // Wild Wolf
    [ENEMY_WILD_WOLF] = {
        .animation = SPRITE_BATTLE_WILD_WOLF,
        .attackEffect = 1,
        .attackHop = 4,
        .defeatSound = 1,
        .effectHeight = 42,
    },
    // Dire Wolf
    [ENEMY_DIRE_WOLF] = {
        .animation = SPRITE_BATTLE_WILD_WOLF,
        .attackEffect = 1,
        .attackHop = 4,
        .palette = 3,
        .defeatSound = 1,
        .effectHeight = 42,
    },
    // White Wolf
    [ENEMY_WHITE_WOLF] = {
        .animation = SPRITE_BATTLE_WILD_WOLF,
        .attackEffect = 1,
        .attackHop = 4,
        .palette = 4,
        .defeatSound = 1,
        .effectHeight = 42,
    },
    // Angle Worm
    [ENEMY_ANGLE_WORM] = {
        .animation = SPRITE_BATTLE_ANGLE_WORM,
        .isSmall = 1,
        .attackEffect = 3,
        .attackHop = 4,
    },
    // Fire Worm
    [ENEMY_FIRE_WORM] = {
        .animation = SPRITE_BATTLE_ANGLE_WORM,
        .isSmall = 1,
        .attackEffect = 3,
        .attackHop = 4,
        .palette = 5,
    },
    // Chimera Worm
    [ENEMY_CHIMERA_WORM] = {
        .animation = SPRITE_BATTLE_ANGLE_WORM,
        .isSmall = 1,
        .attackEffect = 3,
        .attackHop = 4,
        .palette = 3,
    },
    // Mini-Goblin
    [ENEMY_MINI_GOBLIN] = {
        .animation = SPRITE_BATTLE_MINI_GOBLIN,
        .attackEffect = 1,
        .attackHop = 5,
        .defeatSound = 1,
    },
    // Alec Goblin
    [ENEMY_ALEC_GOBLIN] = {
        .animation = SPRITE_BATTLE_MINI_GOBLIN,
        .attackEffect = 1,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = 1,
    },
    // Baboon Goblin
    [ENEMY_BABOON_GOBLIN] = {
        .animation = SPRITE_BATTLE_MINI_GOBLIN,
        .attackEffect = 1,
        .attackHop = 5,
        .palette = 5,
        .defeatSound = 1,
    },
    // Momonga
    [ENEMY_MOMONGA] = {
        .animation = SPRITE_BATTLE_MOMONGA,
        .isSmall = 1,
        .attackEffect = 2,
        .attackHop = 7,
    },
    // Squirrelfang
    [ENEMY_SQUIRRELFANG] = {
        .animation = SPRITE_BATTLE_MOMONGA,
        .isSmall = 1,
        .attackEffect = 2,
        .attackHop = 7,
        .palette = 3,
    },
    // Momangler
    [ENEMY_MOMANGLER] = {
        .animation = SPRITE_BATTLE_MOMONGA,
        .isSmall = 1,
        .attackEffect = 2,
        .attackHop = 7,
        .palette = 2,
    },
    // Kobold
    [ENEMY_KOBOLD] = {
        .animation = SPRITE_BATTLE_KOBOLD,
        .attackEffect = 1,
        .attackHop = 5,
        .defeatSound = 1,
    },
    // Wargold
    [ENEMY_WARGOLD] = {
        .animation = SPRITE_BATTLE_KOBOLD,
        .attackEffect = 1,
        .attackHop = 5,
        .palette = 3,
        .defeatSound = 1,
    },
    // Kobold King
    [ENEMY_KOBOLD_KING] = {
        .animation = SPRITE_BATTLE_KOBOLD,
        .attackEffect = 1,
        .attackHop = 5,
        .palette = 2,
        .defeatSound = 1,
    },
    // Mad Plant
    [ENEMY_MAD_PLANT_GABOMBA_CATACOMBS] = {
        .animation = SPRITE_BATTLE_MAD_PLANT,
        .attackEffect = 9,
        .defeatSound = 1,
    },
    // Mad Plant
    [ENEMY_MAD_PLANT_TAOPO_SWAMP] = {
        .animation = SPRITE_BATTLE_MAD_PLANT,
        .attackEffect = 9,
        .defeatSound = 1,
    },
    // Mad Plant
    [ENEMY_MAD_PLANT_GAIA_ROCK] = {
        .animation = SPRITE_BATTLE_MAD_PLANT,
        .attackEffect = 9,
        .defeatSound = 1,
    },
    // Mad Plant
    [ENEMY_MAD_PLANT_JUPITER_LIGHTHOUSE] = {
        .animation = SPRITE_BATTLE_MAD_PLANT,
        .attackEffect = 9,
        .defeatSound = 1,
    },
    // Mad Plant
    [ENEMY_MAD_PLANT_TRIAL_ROAD] = {
        .animation = SPRITE_BATTLE_MAD_PLANT,
        .attackEffect = 9,
        .defeatSound = 1,
    },
    // Emu
    [ENEMY_EMU] = {
        .animation = SPRITE_BATTLE_EMU,
        .attackEffect = 8,
        .defeatSound = 1,
    },
    // Talon Runner
    [ENEMY_TALON_RUNNER] = {
        .animation = SPRITE_BATTLE_EMU,
        .attackEffect = 8,
        .palette = 3,
        .defeatSound = 1,
    },
    // Winged Runner
    [ENEMY_WINGED_RUNNER] = {
        .animation = SPRITE_BATTLE_EMU,
        .attackEffect = 8,
        .palette = 1,
        .defeatSound = 1,
    },
    // Mummy
    [ENEMY_MUMMY] = {
        .animation = SPRITE_BATTLE_MUMMY,
        .attackEffect = 1,
        .attackHop = 4,
        .palette = 1,
        .defeatSound = 1,
    },
    // Foul Mummy
    [ENEMY_FOUL_MUMMY] = {
        .animation = SPRITE_BATTLE_MUMMY,
        .attackEffect = 1,
        .attackHop = 4,
        .defeatSound = 1,
    },
    // Grave Wight
    [ENEMY_GRAVE_WIGHT] = {
        .animation = SPRITE_BATTLE_MUMMY,
        .attackEffect = 1,
        .attackHop = 4,
        .palette = 4,
        .defeatSound = 1,
    },
    // Wyvern Chick
    [ENEMY_WYVERN_CHICK] = {
        .animation = SPRITE_BATTLE_WYVERN_CHICK,
        .attackEffect = 1,
        .attackHop = 5,
        .palette = 3,
        .defeatSound = 1,
    },
    // Pteranodon
    [ENEMY_PTERANODON] = {
        .animation = SPRITE_BATTLE_WYVERN_CHICK,
        .attackEffect = 1,
        .attackHop = 5,
        .defeatSound = 1,
    },
    // Winged Lizard
    [ENEMY_WINGED_LIZARD] = {
        .animation = SPRITE_BATTLE_WYVERN_CHICK,
        .attackEffect = 1,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = 1,
    },
    // Wolfkin Cub
    [ENEMY_WOLFKIN_CUB] = {
        .animation = SPRITE_BATTLE_WOLFKIN_CUB,
        .attackEffect = 2,
        .attackHop = 4,
        .palette = 3,
        .defeatSound = 1,
    },
    // Wolfkin
    [ENEMY_WOLFKIN] = {
        .animation = SPRITE_BATTLE_WOLFKIN_CUB,
        .attackEffect = 2,
        .attackHop = 4,
        .palette = 1,
        .defeatSound = 1,
    },
    // Skinwalker
    [ENEMY_SKINWALKER] = {
        .animation = SPRITE_BATTLE_WOLFKIN_CUB,
        .attackEffect = 2,
        .attackHop = 4,
        .palette = 4,
        .defeatSound = 1,
    },
    // Dino
    [ENEMY_DINO] = {
        .animation = SPRITE_BATTLE_DINO,
        .attackEffect = 1,
        .attackHop = 7,
        .palette = 1,
        .defeatSound = 1,
    },
    // Dinox
    [ENEMY_DINOX] = {
        .animation = SPRITE_BATTLE_DINO,
        .attackEffect = 1,
        .attackHop = 7,
        .defeatSound = 1,
    },
    // Dinosaurus
    [ENEMY_DINOSAURUS] = {
        .animation = SPRITE_BATTLE_DINO,
        .attackEffect = 1,
        .attackHop = 7,
        .palette = 4,
        .defeatSound = 1,
    },
    // Assassin
    [ENEMY_ASSASSIN] = {
        .animation = SPRITE_BATTLE_ASSASSIN,
        .attackEffect = 3,
        .palette = 3,
        .defeatSound = 1,
    },
    // Slayer
    [ENEMY_SLAYER] = {
        .animation = SPRITE_BATTLE_ASSASSIN,
        .attackEffect = 3,
        .palette = 2,
        .defeatSound = 1,
    },
    // Dark Murder
    [ENEMY_DARK_MURDER] = {
        .animation = SPRITE_BATTLE_ASSASSIN,
        .attackEffect = 3,
        .palette = 4,
        .defeatSound = 1,
    },
    // Doomsayer
    [ENEMY_DOOMSAYER] = {
        .animation = SPRITE_BATTLE_DOOMSAYER,
        .attackEffect = 1,
        .attackHop = 4,
        .palette = 2,
        .defeatSound = 1,
    },
    // Lich
    [ENEMY_LICH] = {
        .animation = SPRITE_BATTLE_DOOMSAYER,
        .attackEffect = 1,
        .attackHop = 4,
        .defeatSound = 1,
    },
    // Bane Wight
    [ENEMY_BANE_WIGHT] = {
        .animation = SPRITE_BATTLE_DOOMSAYER,
        .attackEffect = 1,
        .attackHop = 4,
        .palette = 3,
        .defeatSound = 1,
    },
    // Pixie
    [ENEMY_PIXIE] = {
        .animation = SPRITE_BATTLE_PIXIE,
        .isSmall = 1,
        .attackEffect = 9,
        .attackHop = 5,
        .palette = 3,
    },
    // Faery
    [ENEMY_FAERY] = {
        .animation = SPRITE_BATTLE_PIXIE,
        .isSmall = 1,
        .attackEffect = 9,
        .attackHop = 5,
    },
    // Weird Nypmh
    [ENEMY_WEIRD_NYPMH] = {
        .animation = SPRITE_BATTLE_PIXIE,
        .isSmall = 1,
        .attackEffect = 9,
        .attackHop = 5,
        .palette = 5,
    },
    // Wood Walker
    [ENEMY_WOOD_WALKER] = {
        .animation = SPRITE_BATTLE_WOOD_WALKER,
        .attackEffect = 8,
        .defeatSound = 1,
    },
    // Elder Wood
    [ENEMY_ELDER_WOOD] = {
        .animation = SPRITE_BATTLE_WOOD_WALKER,
        .attackEffect = 8,
        .palette = 1,
        .defeatSound = 1,
    },
    // Estre Wood
    [ENEMY_ESTRE_WOOD] = {
        .animation = SPRITE_BATTLE_WOOD_WALKER,
        .attackEffect = 8,
        .palette = 4,
        .defeatSound = 1,
    },
    // Star Magician
    [ENEMY_STAR_MAGICIAN] = {
        .animation = SPRITE_BATTLE_STAR_MAGICIAN,
        .attackEffect = 10,
        .defeatSound = 1,
    },
    // Dark Wizard
    [ENEMY_DARK_WIZARD] = {
        .animation = SPRITE_BATTLE_STAR_MAGICIAN,
        .attackEffect = 10,
        .defeatSound = 1,
    },
    // Evil Shaman
    [ENEMY_EVIL_SHAMAN] = {
        .animation = SPRITE_BATTLE_STAR_MAGICIAN,
        .attackEffect = 10,
        .defeatSound = 1,
    },
    // Urchin Beast
    [ENEMY_URCHIN_BEAST] = {
        .animation = SPRITE_BATTLE_URCHIN_BEAST,
        .isSmall = 1,
        .attackEffect = 9,
    },
    // Needle Egg
    [ENEMY_NEEDLE_EGG] = {
        .animation = SPRITE_BATTLE_URCHIN_BEAST,
        .isSmall = 1,
        .attackEffect = 9,
        .palette = 2,
    },
    // Sea Hedgehog
    [ENEMY_SEA_HEDGEHOG] = {
        .animation = SPRITE_BATTLE_URCHIN_BEAST,
        .isSmall = 1,
        .attackEffect = 9,
        .palette = 3,
    },
    // Conch Shell
    [ENEMY_CONCH_SHELL] = {
        .animation = SPRITE_BATTLE_CONCH_SHELL,
        .isSmall = 1,
        .attackEffect = 5,
    },
    // Spiral Shell
    [ENEMY_SPIRAL_SHELL] = {
        .animation = SPRITE_BATTLE_CONCH_SHELL,
        .isSmall = 1,
        .attackEffect = 5,
        .palette = 3,
    },
    // Poison Shell
    [ENEMY_POISON_SHELL] = {
        .animation = SPRITE_BATTLE_CONCH_SHELL,
        .isSmall = 1,
        .attackEffect = 5,
        .palette = 5,
    },
    // Merman
    [ENEMY_MERMAN] = {
        .animation = SPRITE_BATTLE_MERMAN,
        .attackEffect = 3,
        .palette = 4,
        .defeatSound = 1,
    },
    // Gillman
    [ENEMY_GILLMAN] = {
        .animation = SPRITE_BATTLE_MERMAN,
        .attackEffect = 3,
        .defeatSound = 1,
    },
    // Gillman Lord
    [ENEMY_GILLMAN_LORD] = {
        .animation = SPRITE_BATTLE_MERMAN,
        .attackEffect = 3,
        .palette = 3,
        .defeatSound = 1,
    },
    // Sea Dragon
    [ENEMY_SEA_DRAGON] = {
        .animation = SPRITE_BATTLE_SEA_DRAGON,
        .attackEffect = 8,
        .attackHop = 7,
        .defeatSound = 2,
        .effectHeight = 60,
    },
    // Turtle Dragon
    [ENEMY_TURTLE_DRAGON] = {
        .animation = SPRITE_BATTLE_SEA_DRAGON,
        .attackEffect = 8,
        .attackHop = 7,
        .palette = 3,
        .defeatSound = 2,
        .effectHeight = 60,
    },
    // Ocean Dragon
    [ENEMY_OCEAN_DRAGON] = {
        .animation = SPRITE_BATTLE_SEA_DRAGON,
        .attackEffect = 8,
        .attackHop = 7,
        .palette = 2,
        .defeatSound = 2,
        .effectHeight = 60,
    },
    // Seabird
    [ENEMY_SEABIRD] = {
        .animation = SPRITE_BATTLE_SEABIRD,
        .attackEffect = 9,
        .defeatSound = 1,
    },
    // Seafowl
    [ENEMY_SEAFOWL] = {
        .animation = SPRITE_BATTLE_SEABIRD,
        .attackEffect = 9,
        .palette = 2,
        .defeatSound = 1,
    },
    // Great Seagull
    [ENEMY_GREAT_SEAGULL] = {
        .animation = SPRITE_BATTLE_SEABIRD,
        .attackEffect = 9,
        .palette = 5,
        .defeatSound = 1,
    },
    // Roc
    [ENEMY_ROC] = {
        .animation = SPRITE_BATTLE_ROC,
        .attackEffect = 1,
        .attackHop = 6,
        .defeatSound = 2,
    },
    // Raptor
    [ENEMY_RAPTOR] = {
        .animation = SPRITE_BATTLE_ROC,
        .attackEffect = 1,
        .attackHop = 6,
        .palette = 2,
        .defeatSound = 2,
    },
    // Fell Raptor
    [ENEMY_FELL_RAPTOR] = {
        .animation = SPRITE_BATTLE_ROC,
        .attackEffect = 1,
        .attackHop = 6,
        .palette = 5,
        .defeatSound = 2,
    },
    // Wyvern
    [ENEMY_WYVERN] = {
        .animation = SPRITE_BATTLE_WYVERN,
        .attackEffect = 1,
        .attackHop = 6,
        .defeatSound = 2,
    },
    // Sky Dragon
    [ENEMY_SKY_DRAGON] = {
        .animation = SPRITE_BATTLE_WYVERN,
        .attackEffect = 1,
        .attackHop = 6,
        .palette = 2,
        .defeatSound = 2,
    },
    // Winged Dragon
    [ENEMY_WINGED_DRAGON] = {
        .animation = SPRITE_BATTLE_WYVERN,
        .attackEffect = 1,
        .attackHop = 6,
        .palette = 5,
        .defeatSound = 2,
    },
    // Phoenix
    [ENEMY_PHOENIX] = {
        .animation = SPRITE_BATTLE_PHOENIX,
        .attackEffect = 9,
        .attackHop = 6,
        .defeatSound = 1,
    },
    // Fire Bird
    [ENEMY_FIRE_BIRD] = {
        .animation = SPRITE_BATTLE_PHOENIX,
        .attackEffect = 9,
        .attackHop = 6,
        .palette = 2,
        .defeatSound = 1,
    },
    // Wonder Bird
    [ENEMY_WONDER_BIRD] = {
        .animation = SPRITE_BATTLE_PHOENIX,
        .attackEffect = 9,
        .attackHop = 6,
        .palette = 5,
        .defeatSound = 1,
    },
    // Blue Dragon
    [ENEMY_BLUE_DRAGON] = {
        .animation = SPRITE_BATTLE_BLUE_DRAGON,
        .attackEffect = 2,
        .attackHop = 5,
        .defeatSound = 2,
    },
    // Cruel Dragon
    [ENEMY_CRUEL_DRAGON] = {
        .animation = SPRITE_BATTLE_BLUE_DRAGON,
        .attackEffect = 2,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = 2,
    },
    // Dragon
    [ENEMY_DRAGON] = {
        .animation = SPRITE_BATTLE_BLUE_DRAGON,
        .attackEffect = 2,
        .attackHop = 5,
        .palette = 4,
        .defeatSound = 2,
    },
    // Flame Dragon
    [ENEMY_FLAME_DRAGON_AGATIO] = {
        .animation = SPRITE_BATTLE_FLAME_DRAGON,
        .attackEffect = 2,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Flame Dragon
    [ENEMY_FLAME_DRAGON_KARST] = {
        .animation = SPRITE_BATTLE_FLAME_DRAGON,
        .attackEffect = 2,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Fire Dragon
    [ENEMY_FIRE_DRAGON] = {
        .animation = SPRITE_BATTLE_FLAME_DRAGON,
        .attackEffect = 2,
        .attackHop = 5,
        .heavyImpact = 1,
        .palette = 4,
        .defeatSound = 2,
    },
    // Minotaurus
    [ENEMY_MINOTAURUS] = {
        .animation = SPRITE_BATTLE_MINOTAURUS,
        .attackEffect = 1,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Minos Warrior
    [ENEMY_MINOS_WARRIOR] = {
        .animation = SPRITE_BATTLE_MINOTAURUS,
        .attackEffect = 1,
        .attackHop = 5,
        .heavyImpact = 1,
        .palette = 4,
        .defeatSound = 2,
    },
    // Minos Knight
    [ENEMY_MINOS_KNIGHT] = {
        .animation = SPRITE_BATTLE_MINOTAURUS,
        .attackEffect = 1,
        .attackHop = 5,
        .heavyImpact = 1,
        .palette = 1,
        .defeatSound = 2,
    },
    // Gressil
    [ENEMY_GRESSIL] = {
        .animation = SPRITE_BATTLE_GRESSIL,
        .attackEffect = 1,
        .attackHop = 5,
        .defeatSound = 1,
    },
    // Little Death
    [ENEMY_LITTLE_DEATH] = {
        .animation = SPRITE_BATTLE_GRESSIL,
        .attackEffect = 1,
        .attackHop = 5,
        .palette = 3,
        .defeatSound = 1,
    },
    // Mini-Death
    [ENEMY_MINI_DEATH] = {
        .animation = SPRITE_BATTLE_GRESSIL,
        .attackEffect = 1,
        .attackHop = 5,
        .palette = 4,
        .defeatSound = 1,
    },
    // Red Demon
    [ENEMY_RED_DEMON] = {
        .animation = SPRITE_BATTLE_RED_DEMON,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = 1,
    },
    // Lesser Demon
    [ENEMY_LESSER_DEMON] = {
        .animation = SPRITE_BATTLE_RED_DEMON,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .palette = 1,
        .defeatSound = 1,
    },
    // Mad Demon
    [ENEMY_MAD_DEMON] = {
        .animation = SPRITE_BATTLE_RED_DEMON,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .palette = 4,
        .defeatSound = 1,
    },
    // Aka Manah
    [ENEMY_AKA_MANAH] = {
        .animation = SPRITE_BATTLE_AKA_MANAH,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Druj
    [ENEMY_DRUJ] = {
        .animation = SPRITE_BATTLE_AKA_MANAH,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .palette = 3,
        .defeatSound = 2,
    },
    // Aeshma
    [ENEMY_AESHMA] = {
        .animation = SPRITE_BATTLE_AKA_MANAH,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .palette = 4,
        .defeatSound = 2,
    },
    // Valukar
    [ENEMY_VALUKAR] = {
        .animation = SPRITE_BATTLE_VALUKAR,
        .attackEffect = 1,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Valukar
    [ENEMY_UNUSED_VALUKAR_A] = {
        .animation = SPRITE_BATTLE_VALUKAR,
        .attackEffect = 1,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Valukar
    [ENEMY_UNUSED_VALUKAR_B] = {
        .animation = SPRITE_BATTLE_VALUKAR,
        .attackEffect = 1,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Living Armor
    [ENEMY_LIVING_ARMOR] = {
        .animation = SPRITE_BATTLE_LIVING_ARMOR,
        .attackEffect = 1,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Puppet Warrior
    [ENEMY_PUPPET_WARRIOR] = {
        .animation = SPRITE_BATTLE_LIVING_ARMOR,
        .attackEffect = 1,
        .attackHop = 5,
        .heavyImpact = 1,
        .palette = 3,
        .defeatSound = 2,
    },
    // Estre Baron
    [ENEMY_ESTRE_BARON] = {
        .animation = SPRITE_BATTLE_LIVING_ARMOR,
        .attackEffect = 1,
        .attackHop = 5,
        .heavyImpact = 1,
        .palette = 1,
        .defeatSound = 2,
    },
    // Ghost Army
    [ENEMY_GHOST_ARMY] = {
        .animation = SPRITE_BATTLE_GHOST_ARMY,
        .attackEffect = 8,
        .heavyImpact = 1,
        .defeatSound = 1,
    },
    // Soul Army
    [ENEMY_SOUL_ARMY] = {
        .animation = SPRITE_BATTLE_GHOST_ARMY,
        .attackEffect = 8,
        .heavyImpact = 1,
        .palette = 3,
        .defeatSound = 1,
    },
    // Spirit Army
    [ENEMY_SPIRIT_ARMY] = {
        .animation = SPRITE_BATTLE_GHOST_ARMY,
        .attackEffect = 8,
        .heavyImpact = 1,
        .palette = 2,
        .defeatSound = 1,
    },
    // Dullahan
    [ENEMY_DULLAHAN] = {
        .animation = SPRITE_BATTLE_DULLAHAN,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Dullahan
    [ENEMY_DULLAHAN_B] = {
        .animation = SPRITE_BATTLE_DULLAHAN,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Dullahan
    [ENEMY_DULLAHAN_C] = {
        .animation = SPRITE_BATTLE_DULLAHAN,
        .attackEffect = 1,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Sentinel
    [ENEMY_SENTINEL] = {
        .animation = SPRITE_BATTLE_SENTINEL,
        .attackEffect = 1,
        .attackHop = 7,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Sentinel
    [ENEMY_UNUSED_SENTINEL_A] = {
        .animation = SPRITE_BATTLE_SENTINEL,
        .attackEffect = 1,
        .attackHop = 7,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Sentinel
    [ENEMY_UNUSED_SENTINEL_B] = {
        .animation = SPRITE_BATTLE_SENTINEL,
        .attackEffect = 1,
        .attackHop = 7,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Vermin
    [ENEMY_VERMIN] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .isSmall = 1,
        .attackEffect = 3,
        .attackHop = 2,
    },
    // Vermin
    [ENEMY_VERMIN_WORLD_MAP] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .isSmall = 1,
        .attackEffect = 3,
        .attackHop = 2,
    },
    // Mad Vermin
    [ENEMY_MAD_VERMIN] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .isSmall = 1,
        .attackEffect = 3,
        .attackHop = 2,
        .palette = 5,
    },
    // Bat
    [ENEMY_BAT] = {
        .animation = SPRITE_BATTLE_BAT,
        .isSmall = 1,
        .attackEffect = 4,
        .attackHop = 6,
    },
    // Bat
    [ENEMY_BAT_WORLD_MAP] = {
        .animation = SPRITE_BATTLE_BAT,
        .isSmall = 1,
        .attackEffect = 4,
        .attackHop = 6,
    },
    // Rabid Bat
    [ENEMY_RABID_BAT] = {
        .animation = SPRITE_BATTLE_BAT,
        .isSmall = 1,
        .attackEffect = 4,
        .attackHop = 6,
        .palette = 4,
    },
    // Giant Bat
    [ENEMY_GIANT_BAT] = {
        .animation = SPRITE_BATTLE_BAT,
        .isSmall = 1,
        .attackEffect = 4,
        .attackHop = 6,
        .palette = 4,
    },
    // Wild Mushroom
    [ENEMY_WILD_MUSHROOM] = {
        .animation = SPRITE_BATTLE_WILD_MUSHROOM,
        .isSmall = 1,
        .attackEffect = 4,
    },
    // Wild Mushroom
    [ENEMY_WILD_MUSHROOM_WORLD_MAP] = {
        .animation = SPRITE_BATTLE_WILD_MUSHROOM,
        .isSmall = 1,
        .attackEffect = 4,
    },
    // Death Cap
    [ENEMY_DEATH_CAP] = {
        .animation = SPRITE_BATTLE_WILD_MUSHROOM,
        .isSmall = 1,
        .attackEffect = 4,
        .palette = 1,
    },
    // Slime
    [ENEMY_SLIME_SOL_SANCTUM] = {
        .animation = SPRITE_BATTLE_SLIME,
        .isSmall = 1,
        .attackEffect = 1,
        .palette = 1,
    },
    // Slime
    [ENEMY_SLIME_WORLD_MAP] = {
        .animation = SPRITE_BATTLE_SLIME,
        .isSmall = 1,
        .attackEffect = 1,
        .palette = 1,
    },
    // Ooze
    [ENEMY_OOZE] = {
        .animation = SPRITE_BATTLE_SLIME,
        .isSmall = 1,
        .attackEffect = 1,
        .palette = 3,
    },
    // Slime Beast
    [ENEMY_SLIME_BEAST] = {
        .animation = SPRITE_BATTLE_SLIME,
        .isSmall = 1,
        .attackEffect = 1,
    },
    // Ghost
    [ENEMY_GHOST] = {
        .animation = SPRITE_BATTLE_GHOST,
        .attackHop = 4,
    },
    // Ghost Mage
    [ENEMY_GHOST_MAGE] = {
        .animation = SPRITE_BATTLE_GHOST,
        .attackHop = 4,
        .palette = 3,
    },
    // Horned Ghost
    [ENEMY_HORNED_GHOST] = {
        .animation = SPRITE_BATTLE_GHOST,
        .attackHop = 4,
        .palette = 4,
    },
    // Lich
    [ENEMY_UNUSED_LICH] = {
        .animation = SPRITE_BATTLE_GHOST,
        .palette = 2,
    },
    // Zombie
    [ENEMY_ZOMBIE] = {
        .animation = SPRITE_BATTLE_ZOMBIE,
        .attackEffect = 1,
        .defeatSound = 1,
    },
    // Undead
    [ENEMY_UNDEAD] = {
        .animation = SPRITE_BATTLE_ZOMBIE,
        .attackEffect = 1,
        .palette = 1,
        .defeatSound = 1,
    },
    // Wight
    [ENEMY_WIGHT] = {
        .animation = SPRITE_BATTLE_ZOMBIE,
        .attackEffect = 1,
        .palette = 4,
        .defeatSound = 1,
    },
    // Bandit
    [ENEMY_BANDIT] = {
        .animation = SPRITE_BATTLE_BANDIT,
        .attackEffect = 1,
        .attackHop = 4,
        .defeatSound = 1,
    },
    // Thief
    [ENEMY_THIEF] = {
        .animation = SPRITE_BATTLE_BANDIT,
        .attackEffect = 1,
        .palette = 4,
        .defeatSound = 1,
    },
    // Brigand
    [ENEMY_BRIGAND] = {
        .animation = SPRITE_BATTLE_BANDIT,
        .attackEffect = 1,
        .defeatSound = 1,
    },
    // Skeleton
    [ENEMY_SKELETON] = {
        .animation = SPRITE_BATTLE_SKELETON,
        .attackEffect = 1,
        .defeatSound = 1,
    },
    // Bone Fighter
    [ENEMY_BONE_FIGHTER] = {
        .animation = SPRITE_BATTLE_SKELETON,
        .attackEffect = 1,
        .attackHop = 4,
        .palette = 3,
        .defeatSound = 1,
    },
    // Skull Warrior
    [ENEMY_SKULL_WARRIOR] = {
        .animation = SPRITE_BATTLE_SKELETON,
        .attackEffect = 1,
        .attackHop = 7,
        .palette = 2,
        .defeatSound = 1,
    },
    // Will Head
    [ENEMY_WILL_HEAD] = {
        .animation = SPRITE_BATTLE_WILL_HEAD,
        .isSmall = 1,
        .palette = 1,
    },
    // Death Head
    [ENEMY_DEATH_HEAD] = {
        .animation = SPRITE_BATTLE_WILL_HEAD,
        .isSmall = 1,
        .palette = 2,
    },
    // Willowisp
    [ENEMY_WILLOWISP] = {
        .animation = SPRITE_BATTLE_WILL_HEAD,
        .isSmall = 1,
        .attackHop = 1,
    },
    // Rat
    [ENEMY_RAT] = {
        .animation = SPRITE_BATTLE_RAT,
        .isSmall = 1,
        .attackEffect = 5,
        .attackHop = 5,
        .effectHeight = 8,
    },
    // Armored Rat
    [ENEMY_ARMORED_RAT] = {
        .animation = SPRITE_BATTLE_RAT,
        .isSmall = 1,
        .attackEffect = 5,
        .attackHop = 5,
        .palette = 1,
        .effectHeight = 8,
    },
    // Plated Rat
    [ENEMY_PLATED_RAT] = {
        .animation = SPRITE_BATTLE_RAT,
        .isSmall = 1,
        .attackEffect = 5,
        .attackHop = 5,
        .palette = 4,
        .effectHeight = 8,
    },
    // Rat Soldier
    [ENEMY_RAT_SOLDIER] = {
        .animation = SPRITE_BATTLE_RAT_SOLDIER,
        .attackEffect = 3,
        .attackHop = 4,
    },
    // Rat Fighter
    [ENEMY_RAT_FIGHTER] = {
        .animation = SPRITE_BATTLE_RAT_SOLDIER,
        .attackEffect = 3,
        .attackHop = 4,
        .palette = 4,
    },
    // Rat Warrior
    [ENEMY_RAT_WARRIOR] = {
        .animation = SPRITE_BATTLE_RAT_SOLDIER,
        .attackEffect = 3,
        .attackHop = 4,
        .palette = 2,
    },
    // Drone Bee
    [ENEMY_DRONE_BEE] = {
        .animation = SPRITE_BATTLE_DRONE_BEE,
        .isSmall = 1,
        .attackEffect = 3,
        .attackHop = 6,
    },
    // Fighter Bee
    [ENEMY_FIGHTER_BEE] = {
        .animation = SPRITE_BATTLE_DRONE_BEE,
        .isSmall = 1,
        .attackEffect = 3,
        .attackHop = 6,
        .palette = 1,
    },
    // Warrior Bee
    [ENEMY_WARRIOR_BEE] = {
        .animation = SPRITE_BATTLE_DRONE_BEE,
        .isSmall = 1,
        .attackEffect = 3,
        .attackHop = 6,
        .palette = 3,
    },
    // Troll
    [ENEMY_TROLL] = {
        .animation = SPRITE_BATTLE_TROLL,
        .defeatSound = 1,
    },
    // Cave Troll
    [ENEMY_CAVE_TROLL] = {
        .animation = SPRITE_BATTLE_TROLL,
        .palette = 5,
        .defeatSound = 1,
    },
    // Brutal Troll
    [ENEMY_BRUTAL_TROLL] = {
        .animation = SPRITE_BATTLE_TROLL,
        .palette = 2,
        .defeatSound = 1,
    },
    // Spider
    [ENEMY_SPIDER] = {
        .animation = SPRITE_BATTLE_SPIDER,
        .isSmall = 1,
        .effectHeight = 16,
    },
    // Tarantula
    [ENEMY_TARANTULA] = {
        .animation = SPRITE_BATTLE_SPIDER,
        .isSmall = 1,
        .palette = 1,
        .effectHeight = 16,
    },
    // Recluse
    [ENEMY_RECLUSE] = {
        .animation = SPRITE_BATTLE_SPIDER,
        .isSmall = 1,
        .palette = 4,
        .effectHeight = 16,
    },
    // Gnome
    [ENEMY_GNOME] = {
        .animation = SPRITE_BATTLE_GNOME,
        .isSmall = 1,
        .attackHop = 5,
    },
    // Gnome Mage
    [ENEMY_GNOME_MAGE] = {
        .animation = SPRITE_BATTLE_GNOME,
        .isSmall = 1,
        .attackHop = 5,
        .palette = 4,
    },
    // Gnome Wizard
    [ENEMY_GNOME_WIZARD] = {
        .animation = SPRITE_BATTLE_GNOME,
        .isSmall = 1,
        .attackHop = 5,
        .palette = 3,
    },
    // Ghoul
    [ENEMY_GHOUL] = {
        .animation = SPRITE_BATTLE_GHOUL,
        .defeatSound = 1,
    },
    // Fiendish Ghoul
    [ENEMY_FIENDISH_GHOUL] = {
        .animation = SPRITE_BATTLE_GHOUL,
        .palette = 2,
        .defeatSound = 1,
    },
    // Cannibal Ghoul
    [ENEMY_CANNIBAL_GHOUL] = {
        .animation = SPRITE_BATTLE_GHOUL,
        .palette = 4,
        .defeatSound = 1,
    },
    // Mauler
    [ENEMY_MAULER] = {
        .animation = SPRITE_BATTLE_MAULER,
        .attackEffect = 2,
        .attackHop = 5,
        .defeatSound = 1,
    },
    // Ravager
    [ENEMY_RAVAGER] = {
        .animation = SPRITE_BATTLE_MAULER,
        .attackEffect = 2,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = 1,
    },
    // Grisly
    [ENEMY_GRISLY] = {
        .animation = SPRITE_BATTLE_MAULER,
        .attackEffect = 2,
        .attackHop = 5,
        .palette = 3,
        .defeatSound = 1,
    },
    // Harpy
    [ENEMY_HARPY] = {
        .animation = SPRITE_BATTLE_HARPY,
        .attackEffect = 2,
        .attackHop = 5,
        .defeatSound = 1,
    },
    // Virago
    [ENEMY_VIRAGO] = {
        .animation = SPRITE_BATTLE_HARPY,
        .attackEffect = 2,
        .attackHop = 5,
        .palette = 2,
        .defeatSound = 1,
    },
    // Harridan
    [ENEMY_HARRIDAN] = {
        .animation = SPRITE_BATTLE_HARPY,
        .attackEffect = 2,
        .attackHop = 5,
        .palette = 3,
        .defeatSound = 1,
    },
    // Siren
    [ENEMY_SIREN] = {
        .animation = SPRITE_BATTLE_SIREN,
        .attackHop = 6,
    },
    // Succubus
    [ENEMY_SUCCUBUS] = {
        .animation = SPRITE_BATTLE_SIREN,
        .attackHop = 6,
        .palette = 3,
    },
    // Nightmare
    [ENEMY_NIGHTMARE] = {
        .animation = SPRITE_BATTLE_SIREN,
        .attackHop = 6,
        .palette = 2,
    },
    // Mole
    [ENEMY_MOLE] = {
        .animation = SPRITE_BATTLE_MOLE,
        .isSmall = 1,
        .attackEffect = 2,
        .effectHeight = 16,
    },
    // Mad Mole
    [ENEMY_MAD_MOLE] = {
        .animation = SPRITE_BATTLE_MOLE,
        .isSmall = 1,
        .attackEffect = 2,
        .palette = 1,
        .effectHeight = 16,
    },
    // Mole Mage
    [ENEMY_MOLE_MAGE] = {
        .animation = SPRITE_BATTLE_MOLE,
        .isSmall = 1,
        .attackEffect = 2,
        .palette = 2,
        .effectHeight = 16,
    },
    // Dirge
    [ENEMY_DIRGE] = {
        .animation = SPRITE_BATTLE_DIRGE,
        .isSmall = 1,
        .attackEffect = 2,
        .attackHop = 6,
    },
    // Foul Dirge
    [ENEMY_FOUL_DIRGE] = {
        .animation = SPRITE_BATTLE_DIRGE,
        .isSmall = 1,
        .attackEffect = 2,
        .attackHop = 6,
        .palette = 4,
    },
    // Vile Dirge
    [ENEMY_VILE_DIRGE] = {
        .animation = SPRITE_BATTLE_DIRGE,
        .isSmall = 1,
        .attackEffect = 2,
        .attackHop = 6,
        .palette = 3,
    },
    // Ape
    [ENEMY_APE] = {
        .animation = SPRITE_BATTLE_APE,
        .attackHop = 5,
        .defeatSound = 1,
    },
    // Killer Ape
    [ENEMY_KILLER_APE] = {
        .animation = SPRITE_BATTLE_APE,
        .attackHop = 5,
        .palette = 3,
        .defeatSound = 1,
    },
    // Dirty Ape
    [ENEMY_DIRTY_APE] = {
        .animation = SPRITE_BATTLE_APE,
        .attackHop = 5,
        .palette = 2,
        .defeatSound = 1,
    },
    // Grub
    [ENEMY_GRUB] = {
        .animation = SPRITE_BATTLE_GRUB,
        .isSmall = 1,
        .attackEffect = 3,
    },
    // Worm
    [ENEMY_WORM] = {
        .animation = SPRITE_BATTLE_GRUB,
        .isSmall = 1,
        .attackEffect = 3,
        .palette = 3,
    },
    // Acid Maggot
    [ENEMY_ACID_MAGGOT] = {
        .animation = SPRITE_BATTLE_GRUB,
        .isSmall = 1,
        .attackEffect = 3,
        .palette = 1,
    },
    // Orc
    [ENEMY_ORC] = {
        .animation = SPRITE_BATTLE_ORC,
        .attackHop = 3,
        .defeatSound = 1,
    },
    // Orc Captain
    [ENEMY_ORC_CAPTAIN] = {
        .animation = SPRITE_BATTLE_ORC,
        .attackHop = 3,
        .palette = 2,
        .defeatSound = 1,
    },
    // Orc Lord
    [ENEMY_ORC_LORD] = {
        .animation = SPRITE_BATTLE_ORC,
        .attackHop = 3,
        .palette = 3,
        .defeatSound = 1,
    },
    // Salamander
    [ENEMY_SALAMANDER] = {
        .animation = SPRITE_BATTLE_SALAMANDER,
        .defeatSound = 1,
        .effectHeight = 32,
    },
    // Earth Lizard
    [ENEMY_EARTH_LIZARD] = {
        .animation = SPRITE_BATTLE_SALAMANDER,
        .palette = 5,
        .defeatSound = 1,
        .effectHeight = 32,
    },
    // Thunder Lizard
    [ENEMY_THUNDER_LIZARD] = {
        .animation = SPRITE_BATTLE_SALAMANDER,
        .palette = 3,
        .defeatSound = 1,
        .effectHeight = 32,
    },
    // Manticore
    [ENEMY_MANTICORE] = {
        .animation = SPRITE_BATTLE_MANTICORE,
        .attackEffect = 2,
        .attackHop = 7,
        .defeatSound = 1,
    },
    // Magicore
    [ENEMY_MAGICORE] = {
        .animation = SPRITE_BATTLE_MANTICORE,
        .attackEffect = 2,
        .attackHop = 7,
        .palette = 1,
        .defeatSound = 1,
    },
    // Manticore King
    [ENEMY_MANTICORE_KING] = {
        .animation = SPRITE_BATTLE_MANTICORE,
        .attackEffect = 2,
        .attackHop = 7,
        .palette = 4,
        .defeatSound = 1,
    },
    // Kobold
    [ENEMY_UNUSED_KOBOLD] = {
        .animation = SPRITE_BATTLE_GOBLIN,
        .defeatSound = 1,
    },
    // Goblin
    [ENEMY_GOBLIN] = {
        .animation = SPRITE_BATTLE_GOBLIN,
        .palette = 1,
        .defeatSound = 1,
    },
    // Hobgoblin
    [ENEMY_HOBGOBLIN] = {
        .animation = SPRITE_BATTLE_GOBLIN,
        .palette = 2,
        .defeatSound = 1,
    },
    // Gargoyle
    [ENEMY_GARGOYLE] = {
        .animation = SPRITE_BATTLE_GARGOYLE,
        .attackHop = 5,
        .defeatSound = 1,
        .effectHeight = 64,
    },
    // Clay Gargoyle
    [ENEMY_CLAY_GARGOYLE] = {
        .animation = SPRITE_BATTLE_GARGOYLE,
        .attackHop = 5,
        .palette = 3,
        .defeatSound = 1,
        .effectHeight = 64,
    },
    // Ice Gargoyle
    [ENEMY_ICE_GARGOYLE] = {
        .animation = SPRITE_BATTLE_GARGOYLE,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = 1,
        .effectHeight = 64,
    },
    // Gryphon
    [ENEMY_GRYPHON] = {
        .animation = SPRITE_BATTLE_GRYPHON,
        .attackEffect = 5,
        .defeatSound = 1,
        .effectHeight = 64,
    },
    // Wild Gryphon
    [ENEMY_WILD_GRYPHON] = {
        .animation = SPRITE_BATTLE_GRYPHON,
        .attackEffect = 5,
        .palette = 1,
        .defeatSound = 1,
        .effectHeight = 64,
    },
    // Wise Gryphon
    [ENEMY_WISE_GRYPHON] = {
        .animation = SPRITE_BATTLE_GRYPHON,
        .attackEffect = 5,
        .palette = 4,
        .defeatSound = 1,
        .effectHeight = 64,
    },
    // Golem
    [ENEMY_GOLEM] = {
        .animation = SPRITE_BATTLE_GOLEM,
        .defeatSound = 1,
    },
    // Earth Golem
    [ENEMY_EARTH_GOLEM] = {
        .animation = SPRITE_BATTLE_GOLEM,
        .palette = 1,
        .defeatSound = 1,
    },
    // Grand Golem
    [ENEMY_GRAND_GOLEM] = {
        .animation = SPRITE_BATTLE_GOLEM,
        .palette = 2,
        .defeatSound = 1,
    },
    // Dread Hound
    [ENEMY_DREAD_HOUND] = {
        .animation = SPRITE_BATTLE_DREAD_HOUND,
        .attackHop = 6,
        .palette = 1,
        .defeatSound = 1,
    },
    // Cerberus
    [ENEMY_CERBERUS] = {
        .animation = SPRITE_BATTLE_DREAD_HOUND,
        .attackHop = 6,
        .defeatSound = 1,
    },
    // Fenrir
    [ENEMY_FENRIR] = {
        .animation = SPRITE_BATTLE_DREAD_HOUND,
        .attackHop = 6,
        .palette = 5,
        .defeatSound = 1,
    },
    // Stone Soldier
    [ENEMY_STONE_SOLDIER] = {
        .animation = SPRITE_BATTLE_STONE_SOLDIER,
        .isSmall = 1,
        .attackHop = 7,
        .effectHeight = 32,
    },
    // Boulder Beast
    [ENEMY_BOULDER_BEAST] = {
        .animation = SPRITE_BATTLE_STONE_SOLDIER,
        .isSmall = 1,
        .attackHop = 7,
        .palette = 1,
        .effectHeight = 32,
    },
    // Raging Rock
    [ENEMY_RAGING_ROCK] = {
        .animation = SPRITE_BATTLE_STONE_SOLDIER,
        .isSmall = 1,
        .attackHop = 7,
        .palette = 4,
        .effectHeight = 32,
    },
    // Chimera
    [ENEMY_CHIMERA] = {
        .animation = SPRITE_BATTLE_CHIMERA,
        .attackEffect = 2,
        .attackHop = 5,
        .defeatSound = 1,
    },
    // Chimera Mage
    [ENEMY_CHIMERA_MAGE] = {
        .animation = SPRITE_BATTLE_CHIMERA,
        .attackEffect = 2,
        .attackHop = 5,
        .palette = 2,
        .defeatSound = 1,
    },
    // Grand Chimera
    [ENEMY_GRAND_CHIMERA] = {
        .animation = SPRITE_BATTLE_CHIMERA,
        .attackEffect = 2,
        .attackHop = 5,
        .palette = 3,
        .defeatSound = 1,
    },
    // Amaze
    [ENEMY_AMAZE_SOL_SANCTUM] = {
        .animation = SPRITE_BATTLE_AMAZE,
    },
    // Amaze
    [ENEMY_AMAZE_KANDOREAN_TEMPLE] = {
        .animation = SPRITE_BATTLE_AMAZE,
    },
    // Creeper
    [ENEMY_CREEPER] = {
        .animation = SPRITE_BATTLE_AMAZE,
        .palette = 1,
    },
    // Spirit
    [ENEMY_SPIRIT] = {
        .animation = SPRITE_BATTLE_AMAZE,
        .palette = 3,
    },
    // Lizard Man
    [ENEMY_LIZARD_MAN] = {
        .animation = SPRITE_BATTLE_LIZARD_MAN,
        .attackHop = 5,
        .defeatSound = 1,
    },
    // Lizard Fighter
    [ENEMY_LIZARD_FIGHTER] = {
        .animation = SPRITE_BATTLE_LIZARD_MAN,
        .attackHop = 5,
        .palette = 5,
        .defeatSound = 1,
    },
    // Lizard King
    [ENEMY_LIZARD_KING] = {
        .animation = SPRITE_BATTLE_LIZARD_MAN,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = 1,
    },
    // Ant Lion
    [ENEMY_ANT_LION] = {
        .animation = SPRITE_BATTLE_ANT_LION,
        .attackEffect = 5,
        .defeatSound = 1,
        .effectHeight = 32,
    },
    // Roach
    [ENEMY_ROACH] = {
        .animation = SPRITE_BATTLE_ANT_LION,
        .attackEffect = 5,
        .palette = 3,
        .defeatSound = 1,
        .effectHeight = 32,
    },
    // Doodle Bug
    [ENEMY_DOODLE_BUG] = {
        .animation = SPRITE_BATTLE_ANT_LION,
        .attackEffect = 5,
        .palette = 2,
        .defeatSound = 1,
        .effectHeight = 32,
    },
    // Toadonpa
    [ENEMY_TOADONPA] = {
        .animation = SPRITE_BATTLE_TOADONPA,
        .attackHop = 5,
        .defeatSound = 1,
        .effectHeight = 42,
    },
    // Poison Toad
    [ENEMY_POISON_TOAD] = {
        .animation = SPRITE_BATTLE_TOADONPA,
        .attackHop = 5,
        .palette = 3,
        .defeatSound = 1,
        .effectHeight = 42,
    },
    // Devil Frog
    [ENEMY_DEVIL_FROG] = {
        .animation = SPRITE_BATTLE_TOADONPA,
        .attackHop = 5,
        .palette = 5,
        .defeatSound = 1,
        .effectHeight = 42,
    },
    // Living Statue
    [ENEMY_LIVING_STATUE] = {
        .animation = SPRITE_BATTLE_LIVING_STATUE,
        .attackEffect = 5,
        .palette = 2,
        .defeatSound = 1,
        .effectHeight = 48,
    },
    // Hydros Statue
    [ENEMY_HYDROS_STATUE] = {
        .animation = SPRITE_BATTLE_LIVING_STATUE,
        .attackEffect = 5,
        .defeatSound = 1,
        .effectHeight = 48,
    },
    // Azart
    [ENEMY_AZART_IRON_SHIELD] = {
        .animation = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .defeatSound = 1,
    },
    // Azart
    [ENEMY_AZART_PADDED_GLOVES] = {
        .animation = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .defeatSound = 1,
    },
    // Satrage
    [ENEMY_SATRAGE_CHAIN_MAIL] = {
        .animation = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = 1,
    },
    // Satrage
    [ENEMY_SATRAGE_COTTON_SHIRT] = {
        .animation = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = 1,
    },
    // Navampa
    [ENEMY_NAVAMPA_CLAYMORE] = {
        .animation = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .palette = 2,
        .defeatSound = 1,
    },
    // Navampa
    [ENEMY_NAVAMPA_HUNTERS_SWORD] = {
        .animation = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .palette = 2,
        .defeatSound = 1,
    },
    // Tret
    [ENEMY_TRET] = {
        .animation = SPRITE_BATTLE_TRET,
        .attackEffect = 5,
        .defeatSound = 1,
    },
    // Kraken
    [ENEMY_KRAKEN] = {
        .animation = SPRITE_BATTLE_KRAKEN,
        .defeatSound = 2,
    },
    // Tornado Lizard
    [ENEMY_TORNADO_LIZARD] = {
        .animation = SPRITE_BATTLE_TORNADO_LIZARD,
        .attackEffect = 2,
        .attackHop = 5,
        .defeatSound = 1,
    },
    // Storm Lizard
    [ENEMY_STORM_LIZARD] = {
        .animation = SPRITE_BATTLE_TORNADO_LIZARD,
        .attackEffect = 2,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = 1,
    },
    // Tempest Lizard
    [ENEMY_TEMPEST_LIZARD] = {
        .animation = SPRITE_BATTLE_TORNADO_LIZARD,
        .attackEffect = 2,
        .attackHop = 5,
        .palette = 4,
        .defeatSound = 1,
    },
    // Mystery Man
    [ENEMY_MYSTERY_MAN] = {
        .animation = SPRITE_BATTLE_SATUROS,
        .attackHop = 5,
    },
    // Saturos
    [ENEMY_SATUROS_MERCURY_LIGHTHOUSE] = {
        .animation = SPRITE_BATTLE_SATUROS,
        .attackHop = 5,
    },
    // Saturos
    [ENEMY_SATUROS_VENUS_LIGHTHOUSE] = {
        .animation = SPRITE_BATTLE_SATUROS,
        .attackHop = 5,
        .heavyImpact = 1,
    },
    // Mystery Woman
    [ENEMY_MYSTERY_WOMAN] = {
        .animation = SPRITE_BATTLE_MENARDI,
        .attackEffect = 5,
    },
    // Menardi
    [ENEMY_MENARDI] = {
        .animation = SPRITE_BATTLE_MENARDI,
        .attackEffect = 5,
        .heavyImpact = 1,
    },
    // Fusion Dragon
    [ENEMY_FUSION_DRAGON] = {
        .animation = SPRITE_BATTLE_FUSION_DRAGON,
        .attackHop = 6,
        .heavyImpact = 1,
        .defeatSound = 3,
    },
    // Deadbeard
    [ENEMY_DEADBEARD] = {
        .animation = 0,
    },
    // Mimic
    [ENEMY_MIMIC_KANDOREAN_TEMPLE] = {
        .animation = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effectHeight = 32,
    },
    // Mimic
    [ENEMY_MIMIC_AIRS_ROCK] = {
        .animation = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effectHeight = 32,
    },
    // Mimic
    [ENEMY_MIMIC_GABOMBA_STATUE] = {
        .animation = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effectHeight = 32,
    },
    // Mimic
    [ENEMY_MIMIC_AQUA_ROCK] = {
        .animation = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effectHeight = 32,
    },
    // Mimic
    [ENEMY_MIMIC_GAIA_ROCK] = {
        .animation = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effectHeight = 32,
    },
    // Mimic
    [ENEMY_MIMIC_TREASURE_ISLE] = {
        .animation = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effectHeight = 32,
    },
    // Mimic
    [ENEMY_MIMIC_JUPITER_LIGHTHOUSE] = {
        .animation = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effectHeight = 32,
    },
    // Mimic
    [ENEMY_MIMIC_MAGMA_ROCK] = {
        .animation = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effectHeight = 32,
    },
    // Mimic
    [ENEMY_MIMIC_MARS_LIGHTHOUSE] = {
        .animation = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effectHeight = 32,
    },
    // Zombie
    [ENEMY_ZOMBIE_DJINN_LESSON] = {
        .animation = SPRITE_BATTLE_ZOMBIE,
        .attackEffect = 1,
        .defeatSound = 1,
    },
    // Doom Dragon
    [ENEMY_DOOM_DRAGON_A] = {
        .animation = SPRITE_BATTLE_DOOM_DRAGON_A,
        .attackHop = 6,
        .heavyImpact = 1,
        .defeatSound = 2,
    },
    // Doom Dragon
    [ENEMY_DOOM_DRAGON_B] = {
        .animation = 0,
    },
    // Doom Dragon
    [ENEMY_DOOM_DRAGON_C] = {
        .animation = 0,
    },
    // Doom Dragon
    [ENEMY_DOOM_DRAGON_D] = {
        .animation = 0,
    },
    // Doom Dragon
    [ENEMY_DOOM_DRAGON_E] = {
        .animation = SPRITE_BATTLE_DOOM_DRAGON_E,
        .attackHop = 6,
        .heavyImpact = 1,
        .defeatSound = 3,
    },
    // Doom Dragon
    [ENEMY_DOOM_DRAGON_F] = {
        .animation = 0,
    },
    // Doom Dragon
    [ENEMY_DOOM_DRAGON_G] = {
        .animation = 0,
    },
    // Doom Dragon
    [ENEMY_DOOM_DRAGON_H] = {
        .animation = SPRITE_BATTLE_DOOM_DRAGON_H,
        .attackHop = 6,
        .heavyImpact = 1,
        .defeatSound = 3,
    },
    // Doom Dragon
    [ENEMY_DOOM_DRAGON_I] = {
        .animation = 0,
    },
    // Refresh Ball
    [ENEMY_REFRESH_BALL] = {
        .animation = SPRITE_BATTLE_REFRESH_BALL,
        .isSmall = 1,
        .attackEffect = 9,
    },
    // Thunder Ball
    [ENEMY_THUNDER_BALL] = {
        .animation = SPRITE_BATTLE_REFRESH_BALL,
        .isSmall = 1,
        .attackEffect = 9,
        .palette = 1,
    },
    // Anger Ball
    [ENEMY_ANGER_BALL] = {
        .animation = SPRITE_BATTLE_REFRESH_BALL,
        .isSmall = 1,
        .attackEffect = 9,
        .palette = 5,
    },
    // Guardian Ball
    [ENEMY_GUARDIAN_BALL] = {
        .animation = SPRITE_BATTLE_REFRESH_BALL,
        .isSmall = 1,
        .attackEffect = 9,
        .palette = 4,
    },
    // Azart
    [ENEMY_AZART_SHAMAN_VILLAGE_CAVE] = {
        .animation = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .defeatSound = 1,
    },
    // Satrage
    [ENEMY_SATRAGE_SHAMAN_VILLAGE_CAVE] = {
        .animation = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = 1,
    },
    // Navampa
    [ENEMY_NAVAMPA_SHAMAN_VILLAGE_CAVE] = {
        .animation = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .palette = 2,
        .defeatSound = 1,
    },
    // Bandit
    [ENEMY_BANDIT_WORLD_MAP] = {
        .animation = SPRITE_BATTLE_BANDIT,
        .attackEffect = 1,
        .attackHop = 4,
        .defeatSound = 1,
    },
    // Thief
    [ENEMY_THIEF_WORLD_MAP] = {
        .animation = SPRITE_BATTLE_BANDIT,
        .attackEffect = 1,
        .palette = 4,
        .defeatSound = 1,
    },
};
// clang-format on
