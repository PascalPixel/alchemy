// The battle sprite rows, by unit id: the eight characters', then the enemies'. Each enemy's name
// is the English release's.

#include "enemy_ids.h"
#include "sprite_ids.h"
#include "battle/battle_summon.h"
#include "game/character_ids.h"

// clang-format off
BATTLE_SPR_DEFINITION BattleSprites[] = {
    // The characters' rows: their attack and hop are read, but a party member's sprite comes from btObjectMake by character, so each row's Vermin sprite is never shown.
    [ISAAC] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
    },
    [GARET] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
    },
    [IVAN] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    [MIA] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    [FELIX] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
    },
    [JENNA] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
    },
    [SHEBA] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    [PIERS] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
    },
    // ???
    [ENEMY_NONE] = {
        .animation = 0,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_UNUSED_QUARTZ] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_UNUSED_VINE] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_UNUSED_BANE] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_UNUSED_ECHO] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_IRON] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_STEEL] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_MUD] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_FLOWER] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_MELD] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_PETRA] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_UNUSED_SALT] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_UNUSED_GEODE] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_UNUSED_MOLD] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_CRYSTAL] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_UNUSED_SLEET] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_UNUSED_SPRITZ] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_UNUSED_HAIL] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_UNUSED_DEW] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_FOG] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_SOUR] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_UNUSED_SPRING] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_UNUSED_SHADE] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_CHILL] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_STEAM] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_UNUSED_RIME] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_UNUSED_GEL] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_EDDY] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_BALM] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_SERAC] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_UNUSED_FORGE] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_UNUSED_CORONA] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_UNUSED_FLASH] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_CANNON] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_UNUSED_SPARK] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_KINDLE] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_UNUSED_CHAR] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_UNUSED_COAL] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_REFLUX] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_CORE] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_UNUSED_TINDER] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_UNUSED_SHINE] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_FURY] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_FUGUE] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_UNUSED_BREEZE] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_UNUSED_ZEPHYR] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_UNUSED_SQUALL] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_UNUSED_LUFF] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_BREATH] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_BLITZ] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_UNUSED_ETHER] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_WAFT] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_UNUSED_HAZE] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_WHEEZE] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_UNUSED_AROMA] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_WHORL] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_GASP] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_UNUSED_LULL] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_GALE] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    // Unused: "?"
    [ENEMY_ID_FIRST + 59] = {
        .animation = 0,
    },
    // Ruffian
    [ENEMY_RUFFIAN] = {
        .animation = SPRITE_BATTLE_RUFFIAN,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Ruffian 2
    [ENEMY_RUFFIAN_2] = {
        .animation = SPRITE_BATTLE_RUFFIAN,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Ruffian 3
    [ENEMY_RUFFIAN_3] = {
        .animation = SPRITE_BATTLE_RUFFIAN,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Punch Ant
    [ENEMY_PUNCH_ANT] = {
        .animation = SPRITE_BATTLE_PUNCH_ANT,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH_LOW,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Flash Ant
    [ENEMY_FLASH_ANT] = {
        .animation = SPRITE_BATTLE_PUNCH_ANT,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH_LOW,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Numb Ant
    [ENEMY_NUMB_ANT] = {
        .animation = SPRITE_BATTLE_PUNCH_ANT,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH_LOW,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Chestbeater
    [ENEMY_CHESTBEATER] = {
        .animation = SPRITE_BATTLE_CHESTBEATER,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 6,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Wild Gorilla
    [ENEMY_WILD_GORILLA] = {
        .animation = SPRITE_BATTLE_CHESTBEATER,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 6,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Crazy Gorilla
    [ENEMY_CRAZY_GORILLA] = {
        .animation = SPRITE_BATTLE_CHESTBEATER,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 6,
        .palette = 5,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // King Scorpion
    [ENEMY_KING_SCORPION] = {
        .animation = SPRITE_BATTLE_KING_SCORPION,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Devil Scorpion
    [ENEMY_DEVIL_SCORPION] = {
        .animation = SPRITE_BATTLE_KING_SCORPION,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Sand Scorpion
    [ENEMY_SAND_SCORPION] = {
        .animation = SPRITE_BATTLE_KING_SCORPION,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .palette = 5,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Briggs
    [ENEMY_BRIGGS] = {
        .animation = SPRITE_BATTLE_BRIGGS,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Sea Fighter
    [ENEMY_SEA_FIGHTER] = {
        .animation = SPRITE_BATTLE_SEA_FIGHTER,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Champa 2
    [ENEMY_CHAMPA_2] = {
        .animation = SPRITE_BATTLE_SEA_FIGHTER,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Champa 3
    [ENEMY_CHAMPA_3] = {
        .animation = SPRITE_BATTLE_SEA_FIGHTER,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Cuttle
    [ENEMY_CUTTLE] = {
        .animation = SPRITE_BATTLE_CUTTLE,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_THRUST,
    },
    // Calamar
    [ENEMY_CALAMAR] = {
        .animation = SPRITE_BATTLE_CUTTLE,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .palette = 5,
    },
    // Man o' War
    [ENEMY_MAN_O_WAR] = {
        .animation = SPRITE_BATTLE_CUTTLE,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .palette = 3,
    },
    // Aqua Jelly
    [ENEMY_AQUA_JELLY] = {
        .animation = SPRITE_BATTLE_CUTTLE,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_THRUST,
    },
    // Aqua Hydra
    [ENEMY_AQUA_HYDRA] = {
        .animation = SPRITE_BATTLE_AQUA_HYDRA,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Hydra
    [ENEMY_HYDRA] = {
        .animation = SPRITE_BATTLE_AQUA_HYDRA,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Pyrodra
    [ENEMY_PYRODRA] = {
        .animation = SPRITE_BATTLE_AQUA_HYDRA,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Serpent
    [ENEMY_SERPENT] = {
        .animation = SPRITE_BATTLE_SERPENT,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_MONSTER_ROAR,
    },
    // Serpent
    [ENEMY_SERPENT_ONE_BEAM] = {
        .animation = SPRITE_BATTLE_SERPENT,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_MONSTER_ROAR,
    },
    // Serpent
    [ENEMY_SERPENT_TWO_BEAMS] = {
        .animation = SPRITE_BATTLE_SERPENT,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_MONSTER_ROAR,
    },
    // Serpent
    [ENEMY_SERPENT_THREE_BEAMS] = {
        .animation = SPRITE_BATTLE_SERPENT,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_MONSTER_ROAR,
    },
    // Serpent
    [ENEMY_SERPENT_FOUR_BEAMS] = {
        .animation = SPRITE_BATTLE_SERPENT,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_MONSTER_ROAR,
    },
    // Serpent 6
    [ENEMY_SERPENT_6] = {
        .animation = SPRITE_BATTLE_SERPENT,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_MONSTER_ROAR,
    },
    // Serpent 7
    [ENEMY_SERPENT_7] = {
        .animation = SPRITE_BATTLE_SERPENT,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_MONSTER_ROAR,
    },
    // Avimander
    [ENEMY_AVIMANDER] = {
        .animation = SPRITE_BATTLE_AVIMANDER,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Macetail
    [ENEMY_MACETAIL] = {
        .animation = SPRITE_BATTLE_AVIMANDER,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .heavyImpact = 1,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Bombander
    [ENEMY_BOMBANDER] = {
        .animation = SPRITE_BATTLE_AVIMANDER,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .heavyImpact = 1,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Poseidon
    [ENEMY_POSEIDON] = {
        .animation = SPRITE_BATTLE_POSEIDON,
        .attackEffect = ATTACK_EFFECT_HURL,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Poseidon
    [ENEMY_POSEIDON_SECOND_ACTION] = {
        .animation = SPRITE_BATTLE_POSEIDON,
        .attackEffect = ATTACK_EFFECT_HURL,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Poseidon 3
    [ENEMY_POSEIDON_3] = {
        .animation = SPRITE_BATTLE_POSEIDON,
        .attackEffect = ATTACK_EFFECT_HURL,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Moapa
    [ENEMY_MOAPA] = {
        .animation = SPRITE_BATTLE_MOAPA,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Moapa
    [ENEMY_MOAPA_B] = {
        .animation = SPRITE_BATTLE_MOAPA,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Moapa
    [ENEMY_MOAPA_C] = {
        .animation = SPRITE_BATTLE_MOAPA,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Knight
    [ENEMY_KNIGHT] = {
        .animation = SPRITE_BATTLE_KNIGHT,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Knight
    [ENEMY_KNIGHT_B] = {
        .animation = SPRITE_BATTLE_KNIGHT,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Knight 3
    [ENEMY_KNIGHT_3] = {
        .animation = SPRITE_BATTLE_KNIGHT,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Agatio
    [ENEMY_AGATIO] = {
        .animation = SPRITE_BATTLE_AGATIO,
        .attackEffect = ATTACK_EFFECT_HURL,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Agatio
    [ENEMY_AGATIO_B] = {
        .animation = SPRITE_BATTLE_AGATIO,
        .attackEffect = ATTACK_EFFECT_HURL,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Agatio
    [ENEMY_AGATIO_VS_ISAAC] = {
        .animation = SPRITE_BATTLE_AGATIO,
        .attackEffect = ATTACK_EFFECT_HURL,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Karst
    [ENEMY_KARST] = {
        .animation = SPRITE_BATTLE_KARST,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Karst
    [ENEMY_KARST_B] = {
        .animation = SPRITE_BATTLE_KARST,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Karst
    [ENEMY_KARST_VS_ISAAC] = {
        .animation = SPRITE_BATTLE_KARST,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Unused: "?"
    [ENEMY_ID_FIRST + 108] = {
        .animation = 0,
    },
    // Wild Wolf
    [ENEMY_WILD_WOLF] = {
        .animation = SPRITE_BATTLE_WILD_WOLF,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 42,
    },
    // Dire Wolf
    [ENEMY_DIRE_WOLF] = {
        .animation = SPRITE_BATTLE_WILD_WOLF,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 42,
    },
    // White Wolf
    [ENEMY_WHITE_WOLF] = {
        .animation = SPRITE_BATTLE_WILD_WOLF,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 42,
    },
    // Angle Worm
    [ENEMY_ANGLE_WORM] = {
        .animation = SPRITE_BATTLE_ANGLE_WORM,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .attackHop = 4,
    },
    // Fire Worm
    [ENEMY_FIRE_WORM] = {
        .animation = SPRITE_BATTLE_ANGLE_WORM,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .attackHop = 4,
        .palette = 5,
    },
    // Chimera Worm
    [ENEMY_CHIMERA_WORM] = {
        .animation = SPRITE_BATTLE_ANGLE_WORM,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .attackHop = 4,
        .palette = 3,
    },
    // Mini-Goblin
    [ENEMY_MINI_GOBLIN] = {
        .animation = SPRITE_BATTLE_MINI_GOBLIN,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Alec Goblin
    [ENEMY_ALEC_GOBLIN] = {
        .animation = SPRITE_BATTLE_MINI_GOBLIN,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Baboon Goblin
    [ENEMY_BABOON_GOBLIN] = {
        .animation = SPRITE_BATTLE_MINI_GOBLIN,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .palette = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Momonga
    [ENEMY_MOMONGA] = {
        .animation = SPRITE_BATTLE_MOMONGA,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 7,
    },
    // Squirrelfang
    [ENEMY_SQUIRRELFANG] = {
        .animation = SPRITE_BATTLE_MOMONGA,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 7,
        .palette = 3,
    },
    // Momangler
    [ENEMY_MOMANGLER] = {
        .animation = SPRITE_BATTLE_MOMONGA,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 7,
        .palette = 2,
    },
    // Kobold
    [ENEMY_KOBOLD] = {
        .animation = SPRITE_BATTLE_KOBOLD,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Wargold
    [ENEMY_WARGOLD] = {
        .animation = SPRITE_BATTLE_KOBOLD,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Kobold King
    [ENEMY_KOBOLD_KING] = {
        .animation = SPRITE_BATTLE_KOBOLD,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Mad Plant
    [ENEMY_MAD_PLANT_GABOMBA_CATACOMBS] = {
        .animation = SPRITE_BATTLE_MAD_PLANT,
        .attackEffect = ATTACK_EFFECT_BLOW,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Mad Plant
    [ENEMY_MAD_PLANT_TAOPO_SWAMP] = {
        .animation = SPRITE_BATTLE_MAD_PLANT,
        .attackEffect = ATTACK_EFFECT_BLOW,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Mad Plant
    [ENEMY_MAD_PLANT_GAIA_ROCK] = {
        .animation = SPRITE_BATTLE_MAD_PLANT,
        .attackEffect = ATTACK_EFFECT_BLOW,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Mad Plant
    [ENEMY_MAD_PLANT_JUPITER_LIGHTHOUSE] = {
        .animation = SPRITE_BATTLE_MAD_PLANT,
        .attackEffect = ATTACK_EFFECT_BLOW,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Mad Plant
    [ENEMY_MAD_PLANT_TRIAL_ROAD] = {
        .animation = SPRITE_BATTLE_MAD_PLANT,
        .attackEffect = ATTACK_EFFECT_BLOW,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Emu
    [ENEMY_EMU] = {
        .animation = SPRITE_BATTLE_EMU,
        .attackEffect = ATTACK_EFFECT_SHOVE,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Talon Runner
    [ENEMY_TALON_RUNNER] = {
        .animation = SPRITE_BATTLE_EMU,
        .attackEffect = ATTACK_EFFECT_SHOVE,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Winged Runner
    [ENEMY_WINGED_RUNNER] = {
        .animation = SPRITE_BATTLE_EMU,
        .attackEffect = ATTACK_EFFECT_SHOVE,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Mummy
    [ENEMY_MUMMY] = {
        .animation = SPRITE_BATTLE_MUMMY,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Foul Mummy
    [ENEMY_FOUL_MUMMY] = {
        .animation = SPRITE_BATTLE_MUMMY,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Grave Wight
    [ENEMY_GRAVE_WIGHT] = {
        .animation = SPRITE_BATTLE_MUMMY,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Wyvern Chick
    [ENEMY_WYVERN_CHICK] = {
        .animation = SPRITE_BATTLE_WYVERN_CHICK,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Pteranodon
    [ENEMY_PTERANODON] = {
        .animation = SPRITE_BATTLE_WYVERN_CHICK,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Winged Lizard
    [ENEMY_WINGED_LIZARD] = {
        .animation = SPRITE_BATTLE_WYVERN_CHICK,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Wolfkin Cub
    [ENEMY_WOLFKIN_CUB] = {
        .animation = SPRITE_BATTLE_WOLFKIN_CUB,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 4,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Wolfkin
    [ENEMY_WOLFKIN] = {
        .animation = SPRITE_BATTLE_WOLFKIN_CUB,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 4,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Skinwalker
    [ENEMY_SKINWALKER] = {
        .animation = SPRITE_BATTLE_WOLFKIN_CUB,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 4,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Dino
    [ENEMY_DINO] = {
        .animation = SPRITE_BATTLE_DINO,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 7,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Dinox
    [ENEMY_DINOX] = {
        .animation = SPRITE_BATTLE_DINO,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 7,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Dinosaurus
    [ENEMY_DINOSAURUS] = {
        .animation = SPRITE_BATTLE_DINO,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 7,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Assassin
    [ENEMY_ASSASSIN] = {
        .animation = SPRITE_BATTLE_ASSASSIN,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Slayer
    [ENEMY_SLAYER] = {
        .animation = SPRITE_BATTLE_ASSASSIN,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Dark Murder
    [ENEMY_DARK_MURDER] = {
        .animation = SPRITE_BATTLE_ASSASSIN,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Doomsayer
    [ENEMY_DOOMSAYER] = {
        .animation = SPRITE_BATTLE_DOOMSAYER,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Lich
    [ENEMY_LICH] = {
        .animation = SPRITE_BATTLE_DOOMSAYER,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Bane Wight
    [ENEMY_BANE_WIGHT] = {
        .animation = SPRITE_BATTLE_DOOMSAYER,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Pixie
    [ENEMY_PIXIE] = {
        .animation = SPRITE_BATTLE_PIXIE,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_BLOW,
        .attackHop = 5,
        .palette = 3,
    },
    // Faery
    [ENEMY_FAERY] = {
        .animation = SPRITE_BATTLE_PIXIE,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_BLOW,
        .attackHop = 5,
    },
    // Weird Nypmh
    [ENEMY_WEIRD_NYPMH] = {
        .animation = SPRITE_BATTLE_PIXIE,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_BLOW,
        .attackHop = 5,
        .palette = 5,
    },
    // Wood Walker
    [ENEMY_WOOD_WALKER] = {
        .animation = SPRITE_BATTLE_WOOD_WALKER,
        .attackEffect = ATTACK_EFFECT_SHOVE,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Elder Wood
    [ENEMY_ELDER_WOOD] = {
        .animation = SPRITE_BATTLE_WOOD_WALKER,
        .attackEffect = ATTACK_EFFECT_SHOVE,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Estre Wood
    [ENEMY_ESTRE_WOOD] = {
        .animation = SPRITE_BATTLE_WOOD_WALKER,
        .attackEffect = ATTACK_EFFECT_SHOVE,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Star Magician
    [ENEMY_STAR_MAGICIAN] = {
        .animation = SPRITE_BATTLE_STAR_MAGICIAN,
        .attackEffect = ATTACK_EFFECT_BURST,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Dark Wizard
    [ENEMY_DARK_WIZARD] = {
        .animation = SPRITE_BATTLE_STAR_MAGICIAN,
        .attackEffect = ATTACK_EFFECT_BURST,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Evil Shaman
    [ENEMY_EVIL_SHAMAN] = {
        .animation = SPRITE_BATTLE_STAR_MAGICIAN,
        .attackEffect = ATTACK_EFFECT_BURST,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Urchin Beast
    [ENEMY_URCHIN_BEAST] = {
        .animation = SPRITE_BATTLE_URCHIN_BEAST,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_BLOW,
    },
    // Needle Egg
    [ENEMY_NEEDLE_EGG] = {
        .animation = SPRITE_BATTLE_URCHIN_BEAST,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_BLOW,
        .palette = 2,
    },
    // Sea Hedgehog
    [ENEMY_SEA_HEDGEHOG] = {
        .animation = SPRITE_BATTLE_URCHIN_BEAST,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_BLOW,
        .palette = 3,
    },
    // Conch Shell
    [ENEMY_CONCH_SHELL] = {
        .animation = SPRITE_BATTLE_CONCH_SHELL,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
    },
    // Spiral Shell
    [ENEMY_SPIRAL_SHELL] = {
        .animation = SPRITE_BATTLE_CONCH_SHELL,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .palette = 3,
    },
    // Poison Shell
    [ENEMY_POISON_SHELL] = {
        .animation = SPRITE_BATTLE_CONCH_SHELL,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .palette = 5,
    },
    // Merman
    [ENEMY_MERMAN] = {
        .animation = SPRITE_BATTLE_MERMAN,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Gillman
    [ENEMY_GILLMAN] = {
        .animation = SPRITE_BATTLE_MERMAN,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Gillman Lord
    [ENEMY_GILLMAN_LORD] = {
        .animation = SPRITE_BATTLE_MERMAN,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Sea Dragon
    [ENEMY_SEA_DRAGON] = {
        .animation = SPRITE_BATTLE_SEA_DRAGON,
        .attackEffect = ATTACK_EFFECT_SHOVE,
        .attackHop = 7,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
        .effectHeight = 60,
    },
    // Turtle Dragon
    [ENEMY_TURTLE_DRAGON] = {
        .animation = SPRITE_BATTLE_SEA_DRAGON,
        .attackEffect = ATTACK_EFFECT_SHOVE,
        .attackHop = 7,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
        .effectHeight = 60,
    },
    // Ocean Dragon
    [ENEMY_OCEAN_DRAGON] = {
        .animation = SPRITE_BATTLE_SEA_DRAGON,
        .attackEffect = ATTACK_EFFECT_SHOVE,
        .attackHop = 7,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
        .effectHeight = 60,
    },
    // Seabird
    [ENEMY_SEABIRD] = {
        .animation = SPRITE_BATTLE_SEABIRD,
        .attackEffect = ATTACK_EFFECT_BLOW,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Seafowl
    [ENEMY_SEAFOWL] = {
        .animation = SPRITE_BATTLE_SEABIRD,
        .attackEffect = ATTACK_EFFECT_BLOW,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Great Seagull
    [ENEMY_GREAT_SEAGULL] = {
        .animation = SPRITE_BATTLE_SEABIRD,
        .attackEffect = ATTACK_EFFECT_BLOW,
        .palette = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Roc
    [ENEMY_ROC] = {
        .animation = SPRITE_BATTLE_ROC,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 6,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Raptor
    [ENEMY_RAPTOR] = {
        .animation = SPRITE_BATTLE_ROC,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 6,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Fell Raptor
    [ENEMY_FELL_RAPTOR] = {
        .animation = SPRITE_BATTLE_ROC,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 6,
        .palette = 5,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Wyvern
    [ENEMY_WYVERN] = {
        .animation = SPRITE_BATTLE_WYVERN,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 6,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Sky Dragon
    [ENEMY_SKY_DRAGON] = {
        .animation = SPRITE_BATTLE_WYVERN,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 6,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Winged Dragon
    [ENEMY_WINGED_DRAGON] = {
        .animation = SPRITE_BATTLE_WYVERN,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 6,
        .palette = 5,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Phoenix
    [ENEMY_PHOENIX] = {
        .animation = SPRITE_BATTLE_PHOENIX,
        .attackEffect = ATTACK_EFFECT_BLOW,
        .attackHop = 6,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Fire Bird
    [ENEMY_FIRE_BIRD] = {
        .animation = SPRITE_BATTLE_PHOENIX,
        .attackEffect = ATTACK_EFFECT_BLOW,
        .attackHop = 6,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Wonder Bird
    [ENEMY_WONDER_BIRD] = {
        .animation = SPRITE_BATTLE_PHOENIX,
        .attackEffect = ATTACK_EFFECT_BLOW,
        .attackHop = 6,
        .palette = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Blue Dragon
    [ENEMY_BLUE_DRAGON] = {
        .animation = SPRITE_BATTLE_BLUE_DRAGON,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Cruel Dragon
    [ENEMY_CRUEL_DRAGON] = {
        .animation = SPRITE_BATTLE_BLUE_DRAGON,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Dragon
    [ENEMY_DRAGON] = {
        .animation = SPRITE_BATTLE_BLUE_DRAGON,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Flame Dragon
    [ENEMY_FLAME_DRAGON_AGATIO] = {
        .animation = SPRITE_BATTLE_FLAME_DRAGON,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Flame Dragon
    [ENEMY_FLAME_DRAGON_KARST] = {
        .animation = SPRITE_BATTLE_FLAME_DRAGON,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Fire Dragon
    [ENEMY_FIRE_DRAGON] = {
        .animation = SPRITE_BATTLE_FLAME_DRAGON,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .heavyImpact = 1,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Minotaurus
    [ENEMY_MINOTAURUS] = {
        .animation = SPRITE_BATTLE_MINOTAURUS,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Minos Warrior
    [ENEMY_MINOS_WARRIOR] = {
        .animation = SPRITE_BATTLE_MINOTAURUS,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .heavyImpact = 1,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Minos Knight
    [ENEMY_MINOS_KNIGHT] = {
        .animation = SPRITE_BATTLE_MINOTAURUS,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .heavyImpact = 1,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Gressil
    [ENEMY_GRESSIL] = {
        .animation = SPRITE_BATTLE_GRESSIL,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Little Death
    [ENEMY_LITTLE_DEATH] = {
        .animation = SPRITE_BATTLE_GRESSIL,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Mini-Death
    [ENEMY_MINI_DEATH] = {
        .animation = SPRITE_BATTLE_GRESSIL,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Red Demon
    [ENEMY_RED_DEMON] = {
        .animation = SPRITE_BATTLE_RED_DEMON,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Lesser Demon
    [ENEMY_LESSER_DEMON] = {
        .animation = SPRITE_BATTLE_RED_DEMON,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Mad Demon
    [ENEMY_MAD_DEMON] = {
        .animation = SPRITE_BATTLE_RED_DEMON,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Aka Manah
    [ENEMY_AKA_MANAH] = {
        .animation = SPRITE_BATTLE_AKA_MANAH,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Druj
    [ENEMY_DRUJ] = {
        .animation = SPRITE_BATTLE_AKA_MANAH,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Aeshma
    [ENEMY_AESHMA] = {
        .animation = SPRITE_BATTLE_AKA_MANAH,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Valukar
    [ENEMY_VALUKAR] = {
        .animation = SPRITE_BATTLE_VALUKAR,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Valukar
    [ENEMY_VALUKAR_UNUSED_A] = {
        .animation = SPRITE_BATTLE_VALUKAR,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Valukar
    [ENEMY_VALUKAR_UNUSED_B] = {
        .animation = SPRITE_BATTLE_VALUKAR,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Living Armor
    [ENEMY_LIVING_ARMOR] = {
        .animation = SPRITE_BATTLE_LIVING_ARMOR,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Puppet Warrior
    [ENEMY_PUPPET_WARRIOR] = {
        .animation = SPRITE_BATTLE_LIVING_ARMOR,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .heavyImpact = 1,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Estre Baron
    [ENEMY_ESTRE_BARON] = {
        .animation = SPRITE_BATTLE_LIVING_ARMOR,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .heavyImpact = 1,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Ghost Army
    [ENEMY_GHOST_ARMY] = {
        .animation = SPRITE_BATTLE_GHOST_ARMY,
        .attackEffect = ATTACK_EFFECT_SHOVE,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Soul Army
    [ENEMY_SOUL_ARMY] = {
        .animation = SPRITE_BATTLE_GHOST_ARMY,
        .attackEffect = ATTACK_EFFECT_SHOVE,
        .heavyImpact = 1,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Spirit Army
    [ENEMY_SPIRIT_ARMY] = {
        .animation = SPRITE_BATTLE_GHOST_ARMY,
        .attackEffect = ATTACK_EFFECT_SHOVE,
        .heavyImpact = 1,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Dullahan
    [ENEMY_DULLAHAN] = {
        .animation = SPRITE_BATTLE_DULLAHAN,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Dullahan
    [ENEMY_DULLAHAN_B] = {
        .animation = SPRITE_BATTLE_DULLAHAN,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Dullahan
    [ENEMY_DULLAHAN_C] = {
        .animation = SPRITE_BATTLE_DULLAHAN,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Sentinel
    [ENEMY_SENTINEL] = {
        .animation = SPRITE_BATTLE_SENTINEL,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 7,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Sentinel
    [ENEMY_SENTINEL_UNUSED_A] = {
        .animation = SPRITE_BATTLE_SENTINEL,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 7,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Sentinel
    [ENEMY_SENTINEL_UNUSED_B] = {
        .animation = SPRITE_BATTLE_SENTINEL,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 7,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Vermin
    [ENEMY_VERMIN] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .attackHop = 2,
    },
    // Vermin
    [ENEMY_VERMIN_WORLD_MAP] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .attackHop = 2,
    },
    // Mad Vermin
    [ENEMY_MAD_VERMIN] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .attackHop = 2,
        .palette = 5,
    },
    // Bat
    [ENEMY_BAT] = {
        .animation = SPRITE_BATTLE_BAT,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 6,
    },
    // Bat
    [ENEMY_BAT_WORLD_MAP] = {
        .animation = SPRITE_BATTLE_BAT,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 6,
    },
    // Rabid Bat
    [ENEMY_RABID_BAT] = {
        .animation = SPRITE_BATTLE_BAT,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 6,
        .palette = 4,
    },
    // Giant Bat
    [ENEMY_GIANT_BAT] = {
        .animation = SPRITE_BATTLE_BAT,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .attackHop = 6,
        .palette = 4,
    },
    // Wild Mushroom
    [ENEMY_WILD_MUSHROOM] = {
        .animation = SPRITE_BATTLE_WILD_MUSHROOM,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_WAVE,
    },
    // Wild Mushroom
    [ENEMY_WILD_MUSHROOM_WORLD_MAP] = {
        .animation = SPRITE_BATTLE_WILD_MUSHROOM,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_WAVE,
    },
    // Death Cap
    [ENEMY_DEATH_CAP] = {
        .animation = SPRITE_BATTLE_WILD_MUSHROOM,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_WAVE,
        .palette = 1,
    },
    // Slime
    [ENEMY_SLIME_SOL_SANCTUM] = {
        .animation = SPRITE_BATTLE_SLIME,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .palette = 1,
    },
    // Slime
    [ENEMY_SLIME_WORLD_MAP] = {
        .animation = SPRITE_BATTLE_SLIME,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .palette = 1,
    },
    // Ooze
    [ENEMY_OOZE] = {
        .animation = SPRITE_BATTLE_SLIME,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .palette = 3,
    },
    // Slime Beast
    [ENEMY_SLIME_BEAST] = {
        .animation = SPRITE_BATTLE_SLIME,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_SLASH,
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
    [ENEMY_LICH_UNUSED] = {
        .animation = SPRITE_BATTLE_GHOST,
        .palette = 2,
    },
    // Zombie
    [ENEMY_ZOMBIE] = {
        .animation = SPRITE_BATTLE_ZOMBIE,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Undead
    [ENEMY_UNDEAD] = {
        .animation = SPRITE_BATTLE_ZOMBIE,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Wight
    [ENEMY_WIGHT] = {
        .animation = SPRITE_BATTLE_ZOMBIE,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Bandit
    [ENEMY_BANDIT] = {
        .animation = SPRITE_BATTLE_BANDIT,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Thief
    [ENEMY_THIEF] = {
        .animation = SPRITE_BATTLE_BANDIT,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Brigand
    [ENEMY_BRIGAND] = {
        .animation = SPRITE_BATTLE_BANDIT,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Skeleton
    [ENEMY_SKELETON] = {
        .animation = SPRITE_BATTLE_SKELETON,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Bone Fighter
    [ENEMY_BONE_FIGHTER] = {
        .animation = SPRITE_BATTLE_SKELETON,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Skull Warrior
    [ENEMY_SKULL_WARRIOR] = {
        .animation = SPRITE_BATTLE_SKELETON,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 7,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
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
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .attackHop = 5,
        .effectHeight = 8,
    },
    // Armored Rat
    [ENEMY_ARMORED_RAT] = {
        .animation = SPRITE_BATTLE_RAT,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .attackHop = 5,
        .palette = 1,
        .effectHeight = 8,
    },
    // Plated Rat
    [ENEMY_PLATED_RAT] = {
        .animation = SPRITE_BATTLE_RAT,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .attackHop = 5,
        .palette = 4,
        .effectHeight = 8,
    },
    // Rat Soldier
    [ENEMY_RAT_SOLDIER] = {
        .animation = SPRITE_BATTLE_RAT_SOLDIER,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .attackHop = 4,
    },
    // Rat Fighter
    [ENEMY_RAT_FIGHTER] = {
        .animation = SPRITE_BATTLE_RAT_SOLDIER,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .attackHop = 4,
        .palette = 4,
    },
    // Rat Warrior
    [ENEMY_RAT_WARRIOR] = {
        .animation = SPRITE_BATTLE_RAT_SOLDIER,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .attackHop = 4,
        .palette = 2,
    },
    // Drone Bee
    [ENEMY_DRONE_BEE] = {
        .animation = SPRITE_BATTLE_DRONE_BEE,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .attackHop = 6,
    },
    // Fighter Bee
    [ENEMY_FIGHTER_BEE] = {
        .animation = SPRITE_BATTLE_DRONE_BEE,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .attackHop = 6,
        .palette = 1,
    },
    // Warrior Bee
    [ENEMY_WARRIOR_BEE] = {
        .animation = SPRITE_BATTLE_DRONE_BEE,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .attackHop = 6,
        .palette = 3,
    },
    // Troll
    [ENEMY_TROLL] = {
        .animation = SPRITE_BATTLE_TROLL,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Cave Troll
    [ENEMY_CAVE_TROLL] = {
        .animation = SPRITE_BATTLE_TROLL,
        .palette = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Brutal Troll
    [ENEMY_BRUTAL_TROLL] = {
        .animation = SPRITE_BATTLE_TROLL,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
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
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Fiendish Ghoul
    [ENEMY_FIENDISH_GHOUL] = {
        .animation = SPRITE_BATTLE_GHOUL,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Cannibal Ghoul
    [ENEMY_CANNIBAL_GHOUL] = {
        .animation = SPRITE_BATTLE_GHOUL,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Mauler
    [ENEMY_MAULER] = {
        .animation = SPRITE_BATTLE_MAULER,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Ravager
    [ENEMY_RAVAGER] = {
        .animation = SPRITE_BATTLE_MAULER,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Grisly
    [ENEMY_GRISLY] = {
        .animation = SPRITE_BATTLE_MAULER,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Harpy
    [ENEMY_HARPY] = {
        .animation = SPRITE_BATTLE_HARPY,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Virago
    [ENEMY_VIRAGO] = {
        .animation = SPRITE_BATTLE_HARPY,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Harridan
    [ENEMY_HARRIDAN] = {
        .animation = SPRITE_BATTLE_HARPY,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
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
        .attackEffect = ATTACK_EFFECT_CLAW,
        .effectHeight = 16,
    },
    // Mad Mole
    [ENEMY_MAD_MOLE] = {
        .animation = SPRITE_BATTLE_MOLE,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .palette = 1,
        .effectHeight = 16,
    },
    // Mole Mage
    [ENEMY_MOLE_MAGE] = {
        .animation = SPRITE_BATTLE_MOLE,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .palette = 2,
        .effectHeight = 16,
    },
    // Dirge
    [ENEMY_DIRGE] = {
        .animation = SPRITE_BATTLE_DIRGE,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 6,
    },
    // Foul Dirge
    [ENEMY_FOUL_DIRGE] = {
        .animation = SPRITE_BATTLE_DIRGE,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 6,
        .palette = 4,
    },
    // Vile Dirge
    [ENEMY_VILE_DIRGE] = {
        .animation = SPRITE_BATTLE_DIRGE,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 6,
        .palette = 3,
    },
    // Ape
    [ENEMY_APE] = {
        .animation = SPRITE_BATTLE_APE,
        .attackHop = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Killer Ape
    [ENEMY_KILLER_APE] = {
        .animation = SPRITE_BATTLE_APE,
        .attackHop = 5,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Dirty Ape
    [ENEMY_DIRTY_APE] = {
        .animation = SPRITE_BATTLE_APE,
        .attackHop = 5,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Grub
    [ENEMY_GRUB] = {
        .animation = SPRITE_BATTLE_GRUB,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_THRUST,
    },
    // Worm
    [ENEMY_WORM] = {
        .animation = SPRITE_BATTLE_GRUB,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .palette = 3,
    },
    // Acid Maggot
    [ENEMY_ACID_MAGGOT] = {
        .animation = SPRITE_BATTLE_GRUB,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_THRUST,
        .palette = 1,
    },
    // Orc
    [ENEMY_ORC] = {
        .animation = SPRITE_BATTLE_ORC,
        .attackHop = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Orc Captain
    [ENEMY_ORC_CAPTAIN] = {
        .animation = SPRITE_BATTLE_ORC,
        .attackHop = 3,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Orc Lord
    [ENEMY_ORC_LORD] = {
        .animation = SPRITE_BATTLE_ORC,
        .attackHop = 3,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Salamander
    [ENEMY_SALAMANDER] = {
        .animation = SPRITE_BATTLE_SALAMANDER,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 32,
    },
    // Earth Lizard
    [ENEMY_EARTH_LIZARD] = {
        .animation = SPRITE_BATTLE_SALAMANDER,
        .palette = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 32,
    },
    // Thunder Lizard
    [ENEMY_THUNDER_LIZARD] = {
        .animation = SPRITE_BATTLE_SALAMANDER,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 32,
    },
    // Manticore
    [ENEMY_MANTICORE] = {
        .animation = SPRITE_BATTLE_MANTICORE,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 7,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Magicore
    [ENEMY_MAGICORE] = {
        .animation = SPRITE_BATTLE_MANTICORE,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 7,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Manticore King
    [ENEMY_MANTICORE_KING] = {
        .animation = SPRITE_BATTLE_MANTICORE,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 7,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Kobold
    [ENEMY_KOBOLD_UNUSED] = {
        .animation = SPRITE_BATTLE_GOBLIN,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Goblin
    [ENEMY_GOBLIN] = {
        .animation = SPRITE_BATTLE_GOBLIN,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Hobgoblin
    [ENEMY_HOBGOBLIN] = {
        .animation = SPRITE_BATTLE_GOBLIN,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Gargoyle
    [ENEMY_GARGOYLE] = {
        .animation = SPRITE_BATTLE_GARGOYLE,
        .attackHop = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 64,
    },
    // Clay Gargoyle
    [ENEMY_CLAY_GARGOYLE] = {
        .animation = SPRITE_BATTLE_GARGOYLE,
        .attackHop = 5,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 64,
    },
    // Ice Gargoyle
    [ENEMY_ICE_GARGOYLE] = {
        .animation = SPRITE_BATTLE_GARGOYLE,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 64,
    },
    // Gryphon
    [ENEMY_GRYPHON] = {
        .animation = SPRITE_BATTLE_GRYPHON,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 64,
    },
    // Wild Gryphon
    [ENEMY_WILD_GRYPHON] = {
        .animation = SPRITE_BATTLE_GRYPHON,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 64,
    },
    // Wise Gryphon
    [ENEMY_WISE_GRYPHON] = {
        .animation = SPRITE_BATTLE_GRYPHON,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 64,
    },
    // Golem
    [ENEMY_GOLEM] = {
        .animation = SPRITE_BATTLE_GOLEM,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Earth Golem
    [ENEMY_EARTH_GOLEM] = {
        .animation = SPRITE_BATTLE_GOLEM,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Grand Golem
    [ENEMY_GRAND_GOLEM] = {
        .animation = SPRITE_BATTLE_GOLEM,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Dread Hound
    [ENEMY_DREAD_HOUND] = {
        .animation = SPRITE_BATTLE_DREAD_HOUND,
        .attackHop = 6,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Cerberus
    [ENEMY_CERBERUS] = {
        .animation = SPRITE_BATTLE_DREAD_HOUND,
        .attackHop = 6,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Fenrir
    [ENEMY_FENRIR] = {
        .animation = SPRITE_BATTLE_DREAD_HOUND,
        .attackHop = 6,
        .palette = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
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
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Chimera Mage
    [ENEMY_CHIMERA_MAGE] = {
        .animation = SPRITE_BATTLE_CHIMERA,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Grand Chimera
    [ENEMY_GRAND_CHIMERA] = {
        .animation = SPRITE_BATTLE_CHIMERA,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
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
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Lizard Fighter
    [ENEMY_LIZARD_FIGHTER] = {
        .animation = SPRITE_BATTLE_LIZARD_MAN,
        .attackHop = 5,
        .palette = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Lizard King
    [ENEMY_LIZARD_KING] = {
        .animation = SPRITE_BATTLE_LIZARD_MAN,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Ant Lion
    [ENEMY_ANT_LION] = {
        .animation = SPRITE_BATTLE_ANT_LION,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 32,
    },
    // Roach
    [ENEMY_ROACH] = {
        .animation = SPRITE_BATTLE_ANT_LION,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 32,
    },
    // Doodle Bug
    [ENEMY_DOODLE_BUG] = {
        .animation = SPRITE_BATTLE_ANT_LION,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 32,
    },
    // Toadonpa
    [ENEMY_TOADONPA] = {
        .animation = SPRITE_BATTLE_TOADONPA,
        .attackHop = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 42,
    },
    // Poison Toad
    [ENEMY_POISON_TOAD] = {
        .animation = SPRITE_BATTLE_TOADONPA,
        .attackHop = 5,
        .palette = 3,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 42,
    },
    // Devil Frog
    [ENEMY_DEVIL_FROG] = {
        .animation = SPRITE_BATTLE_TOADONPA,
        .attackHop = 5,
        .palette = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 42,
    },
    // Living Statue
    [ENEMY_LIVING_STATUE] = {
        .animation = SPRITE_BATTLE_LIVING_STATUE,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 48,
    },
    // Hydros Statue
    [ENEMY_HYDROS_STATUE] = {
        .animation = SPRITE_BATTLE_LIVING_STATUE,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
        .effectHeight = 48,
    },
    // Azart
    [ENEMY_AZART_IRON_SHIELD] = {
        .animation = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Azart
    [ENEMY_AZART_PADDED_GLOVES] = {
        .animation = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Satrage
    [ENEMY_SATRAGE_CHAIN_MAIL] = {
        .animation = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Satrage
    [ENEMY_SATRAGE_COTTON_SHIRT] = {
        .animation = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Navampa
    [ENEMY_NAVAMPA_CLAYMORE] = {
        .animation = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Navampa
    [ENEMY_NAVAMPA_HUNTERS_SWORD] = {
        .animation = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Tret
    [ENEMY_TRET] = {
        .animation = SPRITE_BATTLE_TRET,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Kraken
    [ENEMY_KRAKEN] = {
        .animation = SPRITE_BATTLE_KRAKEN,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
    },
    // Tornado Lizard
    [ENEMY_TORNADO_LIZARD] = {
        .animation = SPRITE_BATTLE_TORNADO_LIZARD,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Storm Lizard
    [ENEMY_STORM_LIZARD] = {
        .animation = SPRITE_BATTLE_TORNADO_LIZARD,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Tempest Lizard
    [ENEMY_TEMPEST_LIZARD] = {
        .animation = SPRITE_BATTLE_TORNADO_LIZARD,
        .attackEffect = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
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
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
    },
    // Menardi
    [ENEMY_MENARDI] = {
        .animation = SPRITE_BATTLE_MENARDI,
        .attackEffect = ATTACK_EFFECT_RISING_SLASH,
        .heavyImpact = 1,
    },
    // Fusion Dragon
    [ENEMY_FUSION_DRAGON] = {
        .animation = SPRITE_BATTLE_FUSION_DRAGON,
        .attackHop = 6,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_MONSTER_ROAR,
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
        .attackEffect = ATTACK_EFFECT_SLASH,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Doom Dragon
    [ENEMY_DOOM_DRAGON_A] = {
        .animation = SPRITE_BATTLE_DOOM_DRAGON_A,
        .attackHop = 6,
        .heavyImpact = 1,
        .defeatSound = DEFEAT_SOUND_BOSS_ROAR,
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
        .defeatSound = DEFEAT_SOUND_MONSTER_ROAR,
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
        .defeatSound = DEFEAT_SOUND_MONSTER_ROAR,
    },
    // Doom Dragon
    [ENEMY_DOOM_DRAGON_I] = {
        .animation = 0,
    },
    // Refresh Ball
    [ENEMY_REFRESH_BALL] = {
        .animation = SPRITE_BATTLE_REFRESH_BALL,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_BLOW,
    },
    // Thunder Ball
    [ENEMY_THUNDER_BALL] = {
        .animation = SPRITE_BATTLE_REFRESH_BALL,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_BLOW,
        .palette = 1,
    },
    // Anger Ball
    [ENEMY_ANGER_BALL] = {
        .animation = SPRITE_BATTLE_REFRESH_BALL,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_BLOW,
        .palette = 5,
    },
    // Guardian Ball
    [ENEMY_GUARDIAN_BALL] = {
        .animation = SPRITE_BATTLE_REFRESH_BALL,
        .isSmall = 1,
        .attackEffect = ATTACK_EFFECT_BLOW,
        .palette = 4,
    },
    // Azart
    [ENEMY_AZART_SHAMAN_VILLAGE_CAVE] = {
        .animation = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Satrage
    [ENEMY_SATRAGE_SHAMAN_VILLAGE_CAVE] = {
        .animation = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .palette = 1,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Navampa
    [ENEMY_NAVAMPA_SHAMAN_VILLAGE_CAVE] = {
        .animation = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .palette = 2,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Bandit
    [ENEMY_BANDIT_WORLD_MAP] = {
        .animation = SPRITE_BATTLE_BANDIT,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
    // Thief
    [ENEMY_THIEF_WORLD_MAP] = {
        .animation = SPRITE_BATTLE_BANDIT,
        .attackEffect = ATTACK_EFFECT_SLASH,
        .palette = 4,
        .defeatSound = DEFEAT_SOUND_BEAST_CRY,
    },
};
// clang-format on
