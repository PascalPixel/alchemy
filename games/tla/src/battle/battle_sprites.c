/* @file */
/* The battle sprite rows, by unit id: the eight characters', then the */
/* enemies'. Each enemy's name is the English release's. */

#include "enemy_ids.h"
#include "sprite_ids.h"
#include "battle/battle_summon.h"
#include "game/character_ids.h"

/* clang-format off */
BATTLE_SPR_DEFINITION BattleSprites[] = {
    /* The characters' rows: their attack and hop are read, but a party */
    /* member's sprite comes from btObjectMake by character, so each row's */
    /* Vermin sprite is never shown. */
    [ISAAC] = {
        .anm = SPRITE_BATTLE_VERMIN,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
    },
    [GARET] = {
        .anm = SPRITE_BATTLE_VERMIN,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
    },
    [IVAN] = {
        .anm = SPRITE_BATTLE_VERMIN,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    [MIA] = {
        .anm = SPRITE_BATTLE_VERMIN,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    [FELIX] = {
        .anm = SPRITE_BATTLE_VERMIN,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
    },
    [JENNA] = {
        .anm = SPRITE_BATTLE_VERMIN,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
    },
    [SHEBA] = {
        .anm = SPRITE_BATTLE_VERMIN,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    [PIERS] = {
        .anm = SPRITE_BATTLE_VERMIN,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
    },
    /* ??? */
    [ENEMY_NONE] = {
        .anm = 0,
    },
    /* Venus Djinni */
    [ENEMY_VENUS_DJINNI_UNUSED_QUARTZ] = {
        .anm = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effHeight = 32,
    },
    /* Venus Djinni */
    [ENEMY_VENUS_DJINNI_UNUSED_VINE] = {
        .anm = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effHeight = 32,
    },
    /* Venus Djinni */
    [ENEMY_VENUS_DJINNI_UNUSED_BANE] = {
        .anm = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effHeight = 32,
    },
    /* Venus Djinni */
    [ENEMY_VENUS_DJINNI_UNUSED_ECHO] = {
        .anm = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effHeight = 32,
    },
    /* Venus Djinni */
    [ENEMY_VENUS_DJINNI_IRON] = {
        .anm = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effHeight = 32,
    },
    /* Venus Djinni */
    [ENEMY_VENUS_DJINNI_STEEL] = {
        .anm = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effHeight = 32,
    },
    /* Venus Djinni */
    [ENEMY_VENUS_DJINNI_MUD] = {
        .anm = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effHeight = 32,
    },
    /* Venus Djinni */
    [ENEMY_VENUS_DJINNI_FLOWER] = {
        .anm = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effHeight = 32,
    },
    /* Venus Djinni */
    [ENEMY_VENUS_DJINNI_MELD] = {
        .anm = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effHeight = 32,
    },
    /* Venus Djinni */
    [ENEMY_VENUS_DJINNI_PETRA] = {
        .anm = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effHeight = 32,
    },
    /* Venus Djinni */
    [ENEMY_VENUS_DJINNI_UNUSED_SALT] = {
        .anm = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effHeight = 32,
    },
    /* Venus Djinni */
    [ENEMY_VENUS_DJINNI_UNUSED_GEODE] = {
        .anm = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effHeight = 32,
    },
    /* Venus Djinni */
    [ENEMY_VENUS_DJINNI_UNUSED_MOLD] = {
        .anm = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effHeight = 32,
    },
    /* Venus Djinni */
    [ENEMY_VENUS_DJINNI_CRYSTAL] = {
        .anm = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
        .effHeight = 32,
    },
    /* Mercury Djinni */
    [ENEMY_MERCURY_DJINNI_UNUSED_SLEET] = {
        .anm = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Mercury Djinni */
    [ENEMY_MERCURY_DJINNI_UNUSED_SPRITZ] = {
        .anm = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Mercury Djinni */
    [ENEMY_MERCURY_DJINNI_UNUSED_HAIL] = {
        .anm = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Mercury Djinni */
    [ENEMY_MERCURY_DJINNI_UNUSED_DEW] = {
        .anm = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Mercury Djinni */
    [ENEMY_MERCURY_DJINNI_FOG] = {
        .anm = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Mercury Djinni */
    [ENEMY_MERCURY_DJINNI_SOUR] = {
        .anm = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Mercury Djinni */
    [ENEMY_MERCURY_DJINNI_UNUSED_SPRING] = {
        .anm = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Mercury Djinni */
    [ENEMY_MERCURY_DJINNI_UNUSED_SHADE] = {
        .anm = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Mercury Djinni */
    [ENEMY_MERCURY_DJINNI_CHILL] = {
        .anm = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Mercury Djinni */
    [ENEMY_MERCURY_DJINNI_STEAM] = {
        .anm = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Mercury Djinni */
    [ENEMY_MERCURY_DJINNI_UNUSED_RIME] = {
        .anm = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Mercury Djinni */
    [ENEMY_MERCURY_DJINNI_UNUSED_GEL] = {
        .anm = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Mercury Djinni */
    [ENEMY_MERCURY_DJINNI_EDDY] = {
        .anm = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Mercury Djinni */
    [ENEMY_MERCURY_DJINNI_BALM] = {
        .anm = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Mercury Djinni */
    [ENEMY_MERCURY_DJINNI_SERAC] = {
        .anm = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Mars Djinni */
    [ENEMY_MARS_DJINNI_UNUSED_FORGE] = {
        .anm = SPRITE_BATTLE_MARS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    /* Mars Djinni */
    [ENEMY_MARS_DJINNI_UNUSED_CORONA] = {
        .anm = SPRITE_BATTLE_MARS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    /* Mars Djinni */
    [ENEMY_MARS_DJINNI_UNUSED_FLASH] = {
        .anm = SPRITE_BATTLE_MARS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    /* Mars Djinni */
    [ENEMY_MARS_DJINNI_CANNON] = {
        .anm = SPRITE_BATTLE_MARS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    /* Mars Djinni */
    [ENEMY_MARS_DJINNI_UNUSED_SPARK] = {
        .anm = SPRITE_BATTLE_MARS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    /* Mars Djinni */
    [ENEMY_MARS_DJINNI_KINDLE] = {
        .anm = SPRITE_BATTLE_MARS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    /* Mars Djinni */
    [ENEMY_MARS_DJINNI_UNUSED_CHAR] = {
        .anm = SPRITE_BATTLE_MARS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    /* Mars Djinni */
    [ENEMY_MARS_DJINNI_UNUSED_COAL] = {
        .anm = SPRITE_BATTLE_MARS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    /* Mars Djinni */
    [ENEMY_MARS_DJINNI_REFLUX] = {
        .anm = SPRITE_BATTLE_MARS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    /* Mars Djinni */
    [ENEMY_MARS_DJINNI_CORE] = {
        .anm = SPRITE_BATTLE_MARS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    /* Mars Djinni */
    [ENEMY_MARS_DJINNI_UNUSED_TINDER] = {
        .anm = SPRITE_BATTLE_MARS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    /* Mars Djinni */
    [ENEMY_MARS_DJINNI_UNUSED_SHINE] = {
        .anm = SPRITE_BATTLE_MARS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    /* Mars Djinni */
    [ENEMY_MARS_DJINNI_FURY] = {
        .anm = SPRITE_BATTLE_MARS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    /* Mars Djinni */
    [ENEMY_MARS_DJINNI_FUGUE] = {
        .anm = SPRITE_BATTLE_MARS_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 2,
    },
    /* Jupiter Djinni */
    [ENEMY_JUPITER_DJINNI_UNUSED_BREEZE] = {
        .anm = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Jupiter Djinni */
    [ENEMY_JUPITER_DJINNI_UNUSED_ZEPHYR] = {
        .anm = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Jupiter Djinni */
    [ENEMY_JUPITER_DJINNI_UNUSED_SQUALL] = {
        .anm = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Jupiter Djinni */
    [ENEMY_JUPITER_DJINNI_UNUSED_LUFF] = {
        .anm = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Jupiter Djinni */
    [ENEMY_JUPITER_DJINNI_BREATH] = {
        .anm = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Jupiter Djinni */
    [ENEMY_JUPITER_DJINNI_BLITZ] = {
        .anm = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Jupiter Djinni */
    [ENEMY_JUPITER_DJINNI_UNUSED_ETHER] = {
        .anm = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Jupiter Djinni */
    [ENEMY_JUPITER_DJINNI_WAFT] = {
        .anm = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Jupiter Djinni */
    [ENEMY_JUPITER_DJINNI_UNUSED_HAZE] = {
        .anm = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Jupiter Djinni */
    [ENEMY_JUPITER_DJINNI_WHEEZE] = {
        .anm = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Jupiter Djinni */
    [ENEMY_JUPITER_DJINNI_UNUSED_AROMA] = {
        .anm = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Jupiter Djinni */
    [ENEMY_JUPITER_DJINNI_WHORL] = {
        .anm = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Jupiter Djinni */
    [ENEMY_JUPITER_DJINNI_GASP] = {
        .anm = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Jupiter Djinni */
    [ENEMY_JUPITER_DJINNI_UNUSED_LULL] = {
        .anm = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Jupiter Djinni */
    [ENEMY_JUPITER_DJINNI_GALE] = {
        .anm = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 5,
    },
    /* Unused: "?" */
    [ENEMY_ID_FIRST + 59] = {
        .anm = 0,
    },
    /* Ruffian */
    [ENEMY_RUFFIAN] = {
        .anm = SPRITE_BATTLE_RUFFIAN,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Ruffian 2 */
    [ENEMY_RUFFIAN_2] = {
        .anm = SPRITE_BATTLE_RUFFIAN,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Ruffian 3 */
    [ENEMY_RUFFIAN_3] = {
        .anm = SPRITE_BATTLE_RUFFIAN,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Punch Ant */
    [ENEMY_PUNCH_ANT] = {
        .anm = SPRITE_BATTLE_PUNCH_ANT,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_RISING_SLASH_LOW,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Flash Ant */
    [ENEMY_FLASH_ANT] = {
        .anm = SPRITE_BATTLE_PUNCH_ANT,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_RISING_SLASH_LOW,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Numb Ant */
    [ENEMY_NUMB_ANT] = {
        .anm = SPRITE_BATTLE_PUNCH_ANT,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_RISING_SLASH_LOW,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Chestbeater */
    [ENEMY_CHESTBEATER] = {
        .anm = SPRITE_BATTLE_CHESTBEATER,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 6,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Wild Gorilla */
    [ENEMY_WILD_GORILLA] = {
        .anm = SPRITE_BATTLE_CHESTBEATER,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 6,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Crazy Gorilla */
    [ENEMY_CRAZY_GORILLA] = {
        .anm = SPRITE_BATTLE_CHESTBEATER,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 6,
        .pal = 5,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* King Scorpion */
    [ENEMY_KING_SCORPION] = {
        .anm = SPRITE_BATTLE_KING_SCORPION,
        .attackEff = ATTACK_EFFECT_WAVE,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Devil Scorpion */
    [ENEMY_DEVIL_SCORPION] = {
        .anm = SPRITE_BATTLE_KING_SCORPION,
        .attackEff = ATTACK_EFFECT_WAVE,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Sand Scorpion */
    [ENEMY_SAND_SCORPION] = {
        .anm = SPRITE_BATTLE_KING_SCORPION,
        .attackEff = ATTACK_EFFECT_WAVE,
        .pal = 5,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Briggs */
    [ENEMY_BRIGGS] = {
        .anm = SPRITE_BATTLE_BRIGGS,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Sea Fighter */
    [ENEMY_SEA_FIGHTER] = {
        .anm = SPRITE_BATTLE_SEA_FIGHTER,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Champa 2 */
    [ENEMY_CHAMPA_2] = {
        .anm = SPRITE_BATTLE_SEA_FIGHTER,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Champa 3 */
    [ENEMY_CHAMPA_3] = {
        .anm = SPRITE_BATTLE_SEA_FIGHTER,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Cuttle */
    [ENEMY_CUTTLE] = {
        .anm = SPRITE_BATTLE_CUTTLE,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_THRUST,
    },
    /* Calamar */
    [ENEMY_CALAMAR] = {
        .anm = SPRITE_BATTLE_CUTTLE,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_THRUST,
        .pal = 5,
    },
    /* Man o' War */
    [ENEMY_MAN_O_WAR] = {
        .anm = SPRITE_BATTLE_CUTTLE,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_THRUST,
        .pal = 3,
    },
    /* Aqua Jelly */
    [ENEMY_AQUA_JELLY] = {
        .anm = SPRITE_BATTLE_CUTTLE,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_THRUST,
    },
    /* Aqua Hydra */
    [ENEMY_AQUA_HYDRA] = {
        .anm = SPRITE_BATTLE_AQUA_HYDRA,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Hydra */
    [ENEMY_HYDRA] = {
        .anm = SPRITE_BATTLE_AQUA_HYDRA,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Pyrodra */
    [ENEMY_PYRODRA] = {
        .anm = SPRITE_BATTLE_AQUA_HYDRA,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Serpent */
    [ENEMY_SERPENT] = {
        .anm = SPRITE_BATTLE_SERPENT,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_MONSTER_ROAR,
    },
    /* Serpent */
    [ENEMY_SERPENT_ONE_BEAM] = {
        .anm = SPRITE_BATTLE_SERPENT,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_MONSTER_ROAR,
    },
    /* Serpent */
    [ENEMY_SERPENT_TWO_BEAMS] = {
        .anm = SPRITE_BATTLE_SERPENT,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_MONSTER_ROAR,
    },
    /* Serpent */
    [ENEMY_SERPENT_THREE_BEAMS] = {
        .anm = SPRITE_BATTLE_SERPENT,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_MONSTER_ROAR,
    },
    /* Serpent */
    [ENEMY_SERPENT_FOUR_BEAMS] = {
        .anm = SPRITE_BATTLE_SERPENT,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_MONSTER_ROAR,
    },
    /* Serpent 6 */
    [ENEMY_SERPENT_6] = {
        .anm = SPRITE_BATTLE_SERPENT,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_MONSTER_ROAR,
    },
    /* Serpent 7 */
    [ENEMY_SERPENT_7] = {
        .anm = SPRITE_BATTLE_SERPENT,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_MONSTER_ROAR,
    },
    /* Avimander */
    [ENEMY_AVIMANDER] = {
        .anm = SPRITE_BATTLE_AVIMANDER,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Macetail */
    [ENEMY_MACETAIL] = {
        .anm = SPRITE_BATTLE_AVIMANDER,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .heavyImpact = 1,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Bombander */
    [ENEMY_BOMBANDER] = {
        .anm = SPRITE_BATTLE_AVIMANDER,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .heavyImpact = 1,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Poseidon */
    [ENEMY_POSEIDON] = {
        .anm = SPRITE_BATTLE_POSEIDON,
        .attackEff = ATTACK_EFFECT_HURL,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Poseidon */
    [ENEMY_POSEIDON_SECOND_ACTION] = {
        .anm = SPRITE_BATTLE_POSEIDON,
        .attackEff = ATTACK_EFFECT_HURL,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Poseidon 3 */
    [ENEMY_POSEIDON_3] = {
        .anm = SPRITE_BATTLE_POSEIDON,
        .attackEff = ATTACK_EFFECT_HURL,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Moapa */
    [ENEMY_MOAPA] = {
        .anm = SPRITE_BATTLE_MOAPA,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Moapa */
    [ENEMY_MOAPA_B] = {
        .anm = SPRITE_BATTLE_MOAPA,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Moapa */
    [ENEMY_MOAPA_C] = {
        .anm = SPRITE_BATTLE_MOAPA,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Knight */
    [ENEMY_KNIGHT] = {
        .anm = SPRITE_BATTLE_KNIGHT,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Knight */
    [ENEMY_KNIGHT_B] = {
        .anm = SPRITE_BATTLE_KNIGHT,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Knight 3 */
    [ENEMY_KNIGHT_3] = {
        .anm = SPRITE_BATTLE_KNIGHT,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Agatio */
    [ENEMY_AGATIO] = {
        .anm = SPRITE_BATTLE_AGATIO,
        .attackEff = ATTACK_EFFECT_HURL,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Agatio */
    [ENEMY_AGATIO_B] = {
        .anm = SPRITE_BATTLE_AGATIO,
        .attackEff = ATTACK_EFFECT_HURL,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Agatio */
    [ENEMY_AGATIO_VS_ISAAC] = {
        .anm = SPRITE_BATTLE_AGATIO,
        .attackEff = ATTACK_EFFECT_HURL,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Karst */
    [ENEMY_KARST] = {
        .anm = SPRITE_BATTLE_KARST,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Karst */
    [ENEMY_KARST_B] = {
        .anm = SPRITE_BATTLE_KARST,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Karst */
    [ENEMY_KARST_VS_ISAAC] = {
        .anm = SPRITE_BATTLE_KARST,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Unused: "?" */
    [ENEMY_ID_FIRST + 108] = {
        .anm = 0,
    },
    /* Wild Wolf */
    [ENEMY_WILD_WOLF] = {
        .anm = SPRITE_BATTLE_WILD_WOLF,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 42,
    },
    /* Dire Wolf */
    [ENEMY_DIRE_WOLF] = {
        .anm = SPRITE_BATTLE_WILD_WOLF,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 42,
    },
    /* White Wolf */
    [ENEMY_WHITE_WOLF] = {
        .anm = SPRITE_BATTLE_WILD_WOLF,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 42,
    },
    /* Angle Worm */
    [ENEMY_ANGLE_WORM] = {
        .anm = SPRITE_BATTLE_ANGLE_WORM,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_THRUST,
        .attackHop = 4,
    },
    /* Fire Worm */
    [ENEMY_FIRE_WORM] = {
        .anm = SPRITE_BATTLE_ANGLE_WORM,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_THRUST,
        .attackHop = 4,
        .pal = 5,
    },
    /* Chimera Worm */
    [ENEMY_CHIMERA_WORM] = {
        .anm = SPRITE_BATTLE_ANGLE_WORM,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_THRUST,
        .attackHop = 4,
        .pal = 3,
    },
    /* Mini-Goblin */
    [ENEMY_MINI_GOBLIN] = {
        .anm = SPRITE_BATTLE_MINI_GOBLIN,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Alec Goblin */
    [ENEMY_ALEC_GOBLIN] = {
        .anm = SPRITE_BATTLE_MINI_GOBLIN,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Baboon Goblin */
    [ENEMY_BABOON_GOBLIN] = {
        .anm = SPRITE_BATTLE_MINI_GOBLIN,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .pal = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Momonga */
    [ENEMY_MOMONGA] = {
        .anm = SPRITE_BATTLE_MOMONGA,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 7,
    },
    /* Squirrelfang */
    [ENEMY_SQUIRRELFANG] = {
        .anm = SPRITE_BATTLE_MOMONGA,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 7,
        .pal = 3,
    },
    /* Momangler */
    [ENEMY_MOMANGLER] = {
        .anm = SPRITE_BATTLE_MOMONGA,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 7,
        .pal = 2,
    },
    /* Kobold */
    [ENEMY_KOBOLD] = {
        .anm = SPRITE_BATTLE_KOBOLD,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Wargold */
    [ENEMY_WARGOLD] = {
        .anm = SPRITE_BATTLE_KOBOLD,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Kobold King */
    [ENEMY_KOBOLD_KING] = {
        .anm = SPRITE_BATTLE_KOBOLD,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Mad Plant */
    [ENEMY_MAD_PLANT_GABOMBA_CATACOMBS] = {
        .anm = SPRITE_BATTLE_MAD_PLANT,
        .attackEff = ATTACK_EFFECT_BLOW,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Mad Plant */
    [ENEMY_MAD_PLANT_TAOPO_SWAMP] = {
        .anm = SPRITE_BATTLE_MAD_PLANT,
        .attackEff = ATTACK_EFFECT_BLOW,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Mad Plant */
    [ENEMY_MAD_PLANT_GAIA_ROCK] = {
        .anm = SPRITE_BATTLE_MAD_PLANT,
        .attackEff = ATTACK_EFFECT_BLOW,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Mad Plant */
    [ENEMY_MAD_PLANT_JUPITER_LIGHTHOUSE] = {
        .anm = SPRITE_BATTLE_MAD_PLANT,
        .attackEff = ATTACK_EFFECT_BLOW,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Mad Plant */
    [ENEMY_MAD_PLANT_TRIAL_ROAD] = {
        .anm = SPRITE_BATTLE_MAD_PLANT,
        .attackEff = ATTACK_EFFECT_BLOW,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Emu */
    [ENEMY_EMU] = {
        .anm = SPRITE_BATTLE_EMU,
        .attackEff = ATTACK_EFFECT_SHOVE,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Talon Runner */
    [ENEMY_TALON_RUNNER] = {
        .anm = SPRITE_BATTLE_EMU,
        .attackEff = ATTACK_EFFECT_SHOVE,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Winged Runner */
    [ENEMY_WINGED_RUNNER] = {
        .anm = SPRITE_BATTLE_EMU,
        .attackEff = ATTACK_EFFECT_SHOVE,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Mummy */
    [ENEMY_MUMMY] = {
        .anm = SPRITE_BATTLE_MUMMY,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Foul Mummy */
    [ENEMY_FOUL_MUMMY] = {
        .anm = SPRITE_BATTLE_MUMMY,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Grave Wight */
    [ENEMY_GRAVE_WIGHT] = {
        .anm = SPRITE_BATTLE_MUMMY,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Wyvern Chick */
    [ENEMY_WYVERN_CHICK] = {
        .anm = SPRITE_BATTLE_WYVERN_CHICK,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Pteranodon */
    [ENEMY_PTERANODON] = {
        .anm = SPRITE_BATTLE_WYVERN_CHICK,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Winged Lizard */
    [ENEMY_WINGED_LIZARD] = {
        .anm = SPRITE_BATTLE_WYVERN_CHICK,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Wolfkin Cub */
    [ENEMY_WOLFKIN_CUB] = {
        .anm = SPRITE_BATTLE_WOLFKIN_CUB,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 4,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Wolfkin */
    [ENEMY_WOLFKIN] = {
        .anm = SPRITE_BATTLE_WOLFKIN_CUB,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 4,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Skinwalker */
    [ENEMY_SKINWALKER] = {
        .anm = SPRITE_BATTLE_WOLFKIN_CUB,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 4,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Dino */
    [ENEMY_DINO] = {
        .anm = SPRITE_BATTLE_DINO,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 7,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Dinox */
    [ENEMY_DINOX] = {
        .anm = SPRITE_BATTLE_DINO,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 7,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Dinosaurus */
    [ENEMY_DINOSAURUS] = {
        .anm = SPRITE_BATTLE_DINO,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 7,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Assassin */
    [ENEMY_ASSASSIN] = {
        .anm = SPRITE_BATTLE_ASSASSIN,
        .attackEff = ATTACK_EFFECT_THRUST,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Slayer */
    [ENEMY_SLAYER] = {
        .anm = SPRITE_BATTLE_ASSASSIN,
        .attackEff = ATTACK_EFFECT_THRUST,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Dark Murder */
    [ENEMY_DARK_MURDER] = {
        .anm = SPRITE_BATTLE_ASSASSIN,
        .attackEff = ATTACK_EFFECT_THRUST,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Doomsayer */
    [ENEMY_DOOMSAYER] = {
        .anm = SPRITE_BATTLE_DOOMSAYER,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Lich */
    [ENEMY_LICH] = {
        .anm = SPRITE_BATTLE_DOOMSAYER,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Bane Wight */
    [ENEMY_BANE_WIGHT] = {
        .anm = SPRITE_BATTLE_DOOMSAYER,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Pixie */
    [ENEMY_PIXIE] = {
        .anm = SPRITE_BATTLE_PIXIE,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_BLOW,
        .attackHop = 5,
        .pal = 3,
    },
    /* Faery */
    [ENEMY_FAERY] = {
        .anm = SPRITE_BATTLE_PIXIE,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_BLOW,
        .attackHop = 5,
    },
    /* Weird Nypmh */
    [ENEMY_WEIRD_NYPMH] = {
        .anm = SPRITE_BATTLE_PIXIE,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_BLOW,
        .attackHop = 5,
        .pal = 5,
    },
    /* Wood Walker */
    [ENEMY_WOOD_WALKER] = {
        .anm = SPRITE_BATTLE_WOOD_WALKER,
        .attackEff = ATTACK_EFFECT_SHOVE,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Elder Wood */
    [ENEMY_ELDER_WOOD] = {
        .anm = SPRITE_BATTLE_WOOD_WALKER,
        .attackEff = ATTACK_EFFECT_SHOVE,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Estre Wood */
    [ENEMY_ESTRE_WOOD] = {
        .anm = SPRITE_BATTLE_WOOD_WALKER,
        .attackEff = ATTACK_EFFECT_SHOVE,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Star Magician */
    [ENEMY_STAR_MAGICIAN] = {
        .anm = SPRITE_BATTLE_STAR_MAGICIAN,
        .attackEff = ATTACK_EFFECT_BURST,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Dark Wizard */
    [ENEMY_DARK_WIZARD] = {
        .anm = SPRITE_BATTLE_STAR_MAGICIAN,
        .attackEff = ATTACK_EFFECT_BURST,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Evil Shaman */
    [ENEMY_EVIL_SHAMAN] = {
        .anm = SPRITE_BATTLE_STAR_MAGICIAN,
        .attackEff = ATTACK_EFFECT_BURST,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Urchin Beast */
    [ENEMY_URCHIN_BEAST] = {
        .anm = SPRITE_BATTLE_URCHIN_BEAST,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_BLOW,
    },
    /* Needle Egg */
    [ENEMY_NEEDLE_EGG] = {
        .anm = SPRITE_BATTLE_URCHIN_BEAST,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_BLOW,
        .pal = 2,
    },
    /* Sea Hedgehog */
    [ENEMY_SEA_HEDGEHOG] = {
        .anm = SPRITE_BATTLE_URCHIN_BEAST,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_BLOW,
        .pal = 3,
    },
    /* Conch Shell */
    [ENEMY_CONCH_SHELL] = {
        .anm = SPRITE_BATTLE_CONCH_SHELL,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
    },
    /* Spiral Shell */
    [ENEMY_SPIRAL_SHELL] = {
        .anm = SPRITE_BATTLE_CONCH_SHELL,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .pal = 3,
    },
    /* Poison Shell */
    [ENEMY_POISON_SHELL] = {
        .anm = SPRITE_BATTLE_CONCH_SHELL,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .pal = 5,
    },
    /* Merman */
    [ENEMY_MERMAN] = {
        .anm = SPRITE_BATTLE_MERMAN,
        .attackEff = ATTACK_EFFECT_THRUST,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Gillman */
    [ENEMY_GILLMAN] = {
        .anm = SPRITE_BATTLE_MERMAN,
        .attackEff = ATTACK_EFFECT_THRUST,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Gillman Lord */
    [ENEMY_GILLMAN_LORD] = {
        .anm = SPRITE_BATTLE_MERMAN,
        .attackEff = ATTACK_EFFECT_THRUST,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Sea Dragon */
    [ENEMY_SEA_DRAGON] = {
        .anm = SPRITE_BATTLE_SEA_DRAGON,
        .attackEff = ATTACK_EFFECT_SHOVE,
        .attackHop = 7,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
        .effHeight = 60,
    },
    /* Turtle Dragon */
    [ENEMY_TURTLE_DRAGON] = {
        .anm = SPRITE_BATTLE_SEA_DRAGON,
        .attackEff = ATTACK_EFFECT_SHOVE,
        .attackHop = 7,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
        .effHeight = 60,
    },
    /* Ocean Dragon */
    [ENEMY_OCEAN_DRAGON] = {
        .anm = SPRITE_BATTLE_SEA_DRAGON,
        .attackEff = ATTACK_EFFECT_SHOVE,
        .attackHop = 7,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
        .effHeight = 60,
    },
    /* Seabird */
    [ENEMY_SEABIRD] = {
        .anm = SPRITE_BATTLE_SEABIRD,
        .attackEff = ATTACK_EFFECT_BLOW,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Seafowl */
    [ENEMY_SEAFOWL] = {
        .anm = SPRITE_BATTLE_SEABIRD,
        .attackEff = ATTACK_EFFECT_BLOW,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Great Seagull */
    [ENEMY_GREAT_SEAGULL] = {
        .anm = SPRITE_BATTLE_SEABIRD,
        .attackEff = ATTACK_EFFECT_BLOW,
        .pal = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Roc */
    [ENEMY_ROC] = {
        .anm = SPRITE_BATTLE_ROC,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 6,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Raptor */
    [ENEMY_RAPTOR] = {
        .anm = SPRITE_BATTLE_ROC,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 6,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Fell Raptor */
    [ENEMY_FELL_RAPTOR] = {
        .anm = SPRITE_BATTLE_ROC,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 6,
        .pal = 5,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Wyvern */
    [ENEMY_WYVERN] = {
        .anm = SPRITE_BATTLE_WYVERN,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 6,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Sky Dragon */
    [ENEMY_SKY_DRAGON] = {
        .anm = SPRITE_BATTLE_WYVERN,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 6,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Winged Dragon */
    [ENEMY_WINGED_DRAGON] = {
        .anm = SPRITE_BATTLE_WYVERN,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 6,
        .pal = 5,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Phoenix */
    [ENEMY_PHOENIX] = {
        .anm = SPRITE_BATTLE_PHOENIX,
        .attackEff = ATTACK_EFFECT_BLOW,
        .attackHop = 6,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Fire Bird */
    [ENEMY_FIRE_BIRD] = {
        .anm = SPRITE_BATTLE_PHOENIX,
        .attackEff = ATTACK_EFFECT_BLOW,
        .attackHop = 6,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Wonder Bird */
    [ENEMY_WONDER_BIRD] = {
        .anm = SPRITE_BATTLE_PHOENIX,
        .attackEff = ATTACK_EFFECT_BLOW,
        .attackHop = 6,
        .pal = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Blue Dragon */
    [ENEMY_BLUE_DRAGON] = {
        .anm = SPRITE_BATTLE_BLUE_DRAGON,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Cruel Dragon */
    [ENEMY_CRUEL_DRAGON] = {
        .anm = SPRITE_BATTLE_BLUE_DRAGON,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Dragon */
    [ENEMY_DRAGON] = {
        .anm = SPRITE_BATTLE_BLUE_DRAGON,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Flame Dragon */
    [ENEMY_FLAME_DRAGON_AGATIO] = {
        .anm = SPRITE_BATTLE_FLAME_DRAGON,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Flame Dragon */
    [ENEMY_FLAME_DRAGON_KARST] = {
        .anm = SPRITE_BATTLE_FLAME_DRAGON,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Fire Dragon */
    [ENEMY_FIRE_DRAGON] = {
        .anm = SPRITE_BATTLE_FLAME_DRAGON,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .heavyImpact = 1,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Minotaurus */
    [ENEMY_MINOTAURUS] = {
        .anm = SPRITE_BATTLE_MINOTAURUS,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Minos Warrior */
    [ENEMY_MINOS_WARRIOR] = {
        .anm = SPRITE_BATTLE_MINOTAURUS,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .heavyImpact = 1,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Minos Knight */
    [ENEMY_MINOS_KNIGHT] = {
        .anm = SPRITE_BATTLE_MINOTAURUS,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .heavyImpact = 1,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Gressil */
    [ENEMY_GRESSIL] = {
        .anm = SPRITE_BATTLE_GRESSIL,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Little Death */
    [ENEMY_LITTLE_DEATH] = {
        .anm = SPRITE_BATTLE_GRESSIL,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Mini-Death */
    [ENEMY_MINI_DEATH] = {
        .anm = SPRITE_BATTLE_GRESSIL,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Red Demon */
    [ENEMY_RED_DEMON] = {
        .anm = SPRITE_BATTLE_RED_DEMON,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Lesser Demon */
    [ENEMY_LESSER_DEMON] = {
        .anm = SPRITE_BATTLE_RED_DEMON,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Mad Demon */
    [ENEMY_MAD_DEMON] = {
        .anm = SPRITE_BATTLE_RED_DEMON,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Aka Manah */
    [ENEMY_AKA_MANAH] = {
        .anm = SPRITE_BATTLE_AKA_MANAH,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Druj */
    [ENEMY_DRUJ] = {
        .anm = SPRITE_BATTLE_AKA_MANAH,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Aeshma */
    [ENEMY_AESHMA] = {
        .anm = SPRITE_BATTLE_AKA_MANAH,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Valukar */
    [ENEMY_VALUKAR] = {
        .anm = SPRITE_BATTLE_VALUKAR,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Valukar */
    [ENEMY_VALUKAR_UNUSED_A] = {
        .anm = SPRITE_BATTLE_VALUKAR,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Valukar */
    [ENEMY_VALUKAR_UNUSED_B] = {
        .anm = SPRITE_BATTLE_VALUKAR,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Living Armor */
    [ENEMY_LIVING_ARMOR] = {
        .anm = SPRITE_BATTLE_LIVING_ARMOR,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Puppet Warrior */
    [ENEMY_PUPPET_WARRIOR] = {
        .anm = SPRITE_BATTLE_LIVING_ARMOR,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .heavyImpact = 1,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Estre Baron */
    [ENEMY_ESTRE_BARON] = {
        .anm = SPRITE_BATTLE_LIVING_ARMOR,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 5,
        .heavyImpact = 1,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Ghost Army */
    [ENEMY_GHOST_ARMY] = {
        .anm = SPRITE_BATTLE_GHOST_ARMY,
        .attackEff = ATTACK_EFFECT_SHOVE,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Soul Army */
    [ENEMY_SOUL_ARMY] = {
        .anm = SPRITE_BATTLE_GHOST_ARMY,
        .attackEff = ATTACK_EFFECT_SHOVE,
        .heavyImpact = 1,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Spirit Army */
    [ENEMY_SPIRIT_ARMY] = {
        .anm = SPRITE_BATTLE_GHOST_ARMY,
        .attackEff = ATTACK_EFFECT_SHOVE,
        .heavyImpact = 1,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Dullahan */
    [ENEMY_DULLAHAN] = {
        .anm = SPRITE_BATTLE_DULLAHAN,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Dullahan */
    [ENEMY_DULLAHAN_B] = {
        .anm = SPRITE_BATTLE_DULLAHAN,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Dullahan */
    [ENEMY_DULLAHAN_C] = {
        .anm = SPRITE_BATTLE_DULLAHAN,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Sentinel */
    [ENEMY_SENTINEL] = {
        .anm = SPRITE_BATTLE_SENTINEL,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 7,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Sentinel */
    [ENEMY_SENTINEL_UNUSED_A] = {
        .anm = SPRITE_BATTLE_SENTINEL,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 7,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Sentinel */
    [ENEMY_SENTINEL_UNUSED_B] = {
        .anm = SPRITE_BATTLE_SENTINEL,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 7,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Vermin */
    [ENEMY_VERMIN] = {
        .anm = SPRITE_BATTLE_VERMIN,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_THRUST,
        .attackHop = 2,
    },
    /* Vermin */
    [ENEMY_VERMIN_WORLD_MAP] = {
        .anm = SPRITE_BATTLE_VERMIN,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_THRUST,
        .attackHop = 2,
    },
    /* Mad Vermin */
    [ENEMY_MAD_VERMIN] = {
        .anm = SPRITE_BATTLE_VERMIN,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_THRUST,
        .attackHop = 2,
        .pal = 5,
    },
    /* Bat */
    [ENEMY_BAT] = {
        .anm = SPRITE_BATTLE_BAT,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 6,
    },
    /* Bat */
    [ENEMY_BAT_WORLD_MAP] = {
        .anm = SPRITE_BATTLE_BAT,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 6,
    },
    /* Rabid Bat */
    [ENEMY_RABID_BAT] = {
        .anm = SPRITE_BATTLE_BAT,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 6,
        .pal = 4,
    },
    /* Giant Bat */
    [ENEMY_GIANT_BAT] = {
        .anm = SPRITE_BATTLE_BAT,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_WAVE,
        .attackHop = 6,
        .pal = 4,
    },
    /* Wild Mushroom */
    [ENEMY_WILD_MUSHROOM] = {
        .anm = SPRITE_BATTLE_WILD_MUSHROOM,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_WAVE,
    },
    /* Wild Mushroom */
    [ENEMY_WILD_MUSHROOM_WORLD_MAP] = {
        .anm = SPRITE_BATTLE_WILD_MUSHROOM,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_WAVE,
    },
    /* Death Cap */
    [ENEMY_DEATH_CAP] = {
        .anm = SPRITE_BATTLE_WILD_MUSHROOM,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_WAVE,
        .pal = 1,
    },
    /* Slime */
    [ENEMY_SLIME_SOL_SANCTUM] = {
        .anm = SPRITE_BATTLE_SLIME,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_SLASH,
        .pal = 1,
    },
    /* Slime */
    [ENEMY_SLIME_WORLD_MAP] = {
        .anm = SPRITE_BATTLE_SLIME,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_SLASH,
        .pal = 1,
    },
    /* Ooze */
    [ENEMY_OOZE] = {
        .anm = SPRITE_BATTLE_SLIME,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_SLASH,
        .pal = 3,
    },
    /* Slime Beast */
    [ENEMY_SLIME_BEAST] = {
        .anm = SPRITE_BATTLE_SLIME,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_SLASH,
    },
    /* Ghost */
    [ENEMY_GHOST] = {
        .anm = SPRITE_BATTLE_GHOST,
        .attackHop = 4,
    },
    /* Ghost Mage */
    [ENEMY_GHOST_MAGE] = {
        .anm = SPRITE_BATTLE_GHOST,
        .attackHop = 4,
        .pal = 3,
    },
    /* Horned Ghost */
    [ENEMY_HORNED_GHOST] = {
        .anm = SPRITE_BATTLE_GHOST,
        .attackHop = 4,
        .pal = 4,
    },
    /* Lich */
    [ENEMY_LICH_UNUSED] = {
        .anm = SPRITE_BATTLE_GHOST,
        .pal = 2,
    },
    /* Zombie */
    [ENEMY_ZOMBIE] = {
        .anm = SPRITE_BATTLE_ZOMBIE,
        .attackEff = ATTACK_EFFECT_SLASH,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Undead */
    [ENEMY_UNDEAD] = {
        .anm = SPRITE_BATTLE_ZOMBIE,
        .attackEff = ATTACK_EFFECT_SLASH,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Wight */
    [ENEMY_WIGHT] = {
        .anm = SPRITE_BATTLE_ZOMBIE,
        .attackEff = ATTACK_EFFECT_SLASH,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Bandit */
    [ENEMY_BANDIT] = {
        .anm = SPRITE_BATTLE_BANDIT,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Thief */
    [ENEMY_THIEF] = {
        .anm = SPRITE_BATTLE_BANDIT,
        .attackEff = ATTACK_EFFECT_SLASH,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Brigand */
    [ENEMY_BRIGAND] = {
        .anm = SPRITE_BATTLE_BANDIT,
        .attackEff = ATTACK_EFFECT_SLASH,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Skeleton */
    [ENEMY_SKELETON] = {
        .anm = SPRITE_BATTLE_SKELETON,
        .attackEff = ATTACK_EFFECT_SLASH,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Bone Fighter */
    [ENEMY_BONE_FIGHTER] = {
        .anm = SPRITE_BATTLE_SKELETON,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Skull Warrior */
    [ENEMY_SKULL_WARRIOR] = {
        .anm = SPRITE_BATTLE_SKELETON,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 7,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Will Head */
    [ENEMY_WILL_HEAD] = {
        .anm = SPRITE_BATTLE_WILL_HEAD,
        .isSmall = 1,
        .pal = 1,
    },
    /* Death Head */
    [ENEMY_DEATH_HEAD] = {
        .anm = SPRITE_BATTLE_WILL_HEAD,
        .isSmall = 1,
        .pal = 2,
    },
    /* Willowisp */
    [ENEMY_WILLOWISP] = {
        .anm = SPRITE_BATTLE_WILL_HEAD,
        .isSmall = 1,
        .attackHop = 1,
    },
    /* Rat */
    [ENEMY_RAT] = {
        .anm = SPRITE_BATTLE_RAT,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .attackHop = 5,
        .effHeight = 8,
    },
    /* Armored Rat */
    [ENEMY_ARMORED_RAT] = {
        .anm = SPRITE_BATTLE_RAT,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .attackHop = 5,
        .pal = 1,
        .effHeight = 8,
    },
    /* Plated Rat */
    [ENEMY_PLATED_RAT] = {
        .anm = SPRITE_BATTLE_RAT,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .attackHop = 5,
        .pal = 4,
        .effHeight = 8,
    },
    /* Rat Soldier */
    [ENEMY_RAT_SOLDIER] = {
        .anm = SPRITE_BATTLE_RAT_SOLDIER,
        .attackEff = ATTACK_EFFECT_THRUST,
        .attackHop = 4,
    },
    /* Rat Fighter */
    [ENEMY_RAT_FIGHTER] = {
        .anm = SPRITE_BATTLE_RAT_SOLDIER,
        .attackEff = ATTACK_EFFECT_THRUST,
        .attackHop = 4,
        .pal = 4,
    },
    /* Rat Warrior */
    [ENEMY_RAT_WARRIOR] = {
        .anm = SPRITE_BATTLE_RAT_SOLDIER,
        .attackEff = ATTACK_EFFECT_THRUST,
        .attackHop = 4,
        .pal = 2,
    },
    /* Drone Bee */
    [ENEMY_DRONE_BEE] = {
        .anm = SPRITE_BATTLE_DRONE_BEE,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_THRUST,
        .attackHop = 6,
    },
    /* Fighter Bee */
    [ENEMY_FIGHTER_BEE] = {
        .anm = SPRITE_BATTLE_DRONE_BEE,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_THRUST,
        .attackHop = 6,
        .pal = 1,
    },
    /* Warrior Bee */
    [ENEMY_WARRIOR_BEE] = {
        .anm = SPRITE_BATTLE_DRONE_BEE,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_THRUST,
        .attackHop = 6,
        .pal = 3,
    },
    /* Troll */
    [ENEMY_TROLL] = {
        .anm = SPRITE_BATTLE_TROLL,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Cave Troll */
    [ENEMY_CAVE_TROLL] = {
        .anm = SPRITE_BATTLE_TROLL,
        .pal = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Brutal Troll */
    [ENEMY_BRUTAL_TROLL] = {
        .anm = SPRITE_BATTLE_TROLL,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Spider */
    [ENEMY_SPIDER] = {
        .anm = SPRITE_BATTLE_SPIDER,
        .isSmall = 1,
        .effHeight = 16,
    },
    /* Tarantula */
    [ENEMY_TARANTULA] = {
        .anm = SPRITE_BATTLE_SPIDER,
        .isSmall = 1,
        .pal = 1,
        .effHeight = 16,
    },
    /* Recluse */
    [ENEMY_RECLUSE] = {
        .anm = SPRITE_BATTLE_SPIDER,
        .isSmall = 1,
        .pal = 4,
        .effHeight = 16,
    },
    /* Gnome */
    [ENEMY_GNOME] = {
        .anm = SPRITE_BATTLE_GNOME,
        .isSmall = 1,
        .attackHop = 5,
    },
    /* Gnome Mage */
    [ENEMY_GNOME_MAGE] = {
        .anm = SPRITE_BATTLE_GNOME,
        .isSmall = 1,
        .attackHop = 5,
        .pal = 4,
    },
    /* Gnome Wizard */
    [ENEMY_GNOME_WIZARD] = {
        .anm = SPRITE_BATTLE_GNOME,
        .isSmall = 1,
        .attackHop = 5,
        .pal = 3,
    },
    /* Ghoul */
    [ENEMY_GHOUL] = {
        .anm = SPRITE_BATTLE_GHOUL,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Fiendish Ghoul */
    [ENEMY_FIENDISH_GHOUL] = {
        .anm = SPRITE_BATTLE_GHOUL,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Cannibal Ghoul */
    [ENEMY_CANNIBAL_GHOUL] = {
        .anm = SPRITE_BATTLE_GHOUL,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Mauler */
    [ENEMY_MAULER] = {
        .anm = SPRITE_BATTLE_MAULER,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Ravager */
    [ENEMY_RAVAGER] = {
        .anm = SPRITE_BATTLE_MAULER,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Grisly */
    [ENEMY_GRISLY] = {
        .anm = SPRITE_BATTLE_MAULER,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Harpy */
    [ENEMY_HARPY] = {
        .anm = SPRITE_BATTLE_HARPY,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Virago */
    [ENEMY_VIRAGO] = {
        .anm = SPRITE_BATTLE_HARPY,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Harridan */
    [ENEMY_HARRIDAN] = {
        .anm = SPRITE_BATTLE_HARPY,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Siren */
    [ENEMY_SIREN] = {
        .anm = SPRITE_BATTLE_SIREN,
        .attackHop = 6,
    },
    /* Succubus */
    [ENEMY_SUCCUBUS] = {
        .anm = SPRITE_BATTLE_SIREN,
        .attackHop = 6,
        .pal = 3,
    },
    /* Nightmare */
    [ENEMY_NIGHTMARE] = {
        .anm = SPRITE_BATTLE_SIREN,
        .attackHop = 6,
        .pal = 2,
    },
    /* Mole */
    [ENEMY_MOLE] = {
        .anm = SPRITE_BATTLE_MOLE,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_CLAW,
        .effHeight = 16,
    },
    /* Mad Mole */
    [ENEMY_MAD_MOLE] = {
        .anm = SPRITE_BATTLE_MOLE,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_CLAW,
        .pal = 1,
        .effHeight = 16,
    },
    /* Mole Mage */
    [ENEMY_MOLE_MAGE] = {
        .anm = SPRITE_BATTLE_MOLE,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_CLAW,
        .pal = 2,
        .effHeight = 16,
    },
    /* Dirge */
    [ENEMY_DIRGE] = {
        .anm = SPRITE_BATTLE_DIRGE,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 6,
    },
    /* Foul Dirge */
    [ENEMY_FOUL_DIRGE] = {
        .anm = SPRITE_BATTLE_DIRGE,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 6,
        .pal = 4,
    },
    /* Vile Dirge */
    [ENEMY_VILE_DIRGE] = {
        .anm = SPRITE_BATTLE_DIRGE,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 6,
        .pal = 3,
    },
    /* Ape */
    [ENEMY_APE] = {
        .anm = SPRITE_BATTLE_APE,
        .attackHop = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Killer Ape */
    [ENEMY_KILLER_APE] = {
        .anm = SPRITE_BATTLE_APE,
        .attackHop = 5,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Dirty Ape */
    [ENEMY_DIRTY_APE] = {
        .anm = SPRITE_BATTLE_APE,
        .attackHop = 5,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Grub */
    [ENEMY_GRUB] = {
        .anm = SPRITE_BATTLE_GRUB,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_THRUST,
    },
    /* Worm */
    [ENEMY_WORM] = {
        .anm = SPRITE_BATTLE_GRUB,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_THRUST,
        .pal = 3,
    },
    /* Acid Maggot */
    [ENEMY_ACID_MAGGOT] = {
        .anm = SPRITE_BATTLE_GRUB,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_THRUST,
        .pal = 1,
    },
    /* Orc */
    [ENEMY_ORC] = {
        .anm = SPRITE_BATTLE_ORC,
        .attackHop = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Orc Captain */
    [ENEMY_ORC_CAPTAIN] = {
        .anm = SPRITE_BATTLE_ORC,
        .attackHop = 3,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Orc Lord */
    [ENEMY_ORC_LORD] = {
        .anm = SPRITE_BATTLE_ORC,
        .attackHop = 3,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Salamander */
    [ENEMY_SALAMANDER] = {
        .anm = SPRITE_BATTLE_SALAMANDER,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 32,
    },
    /* Earth Lizard */
    [ENEMY_EARTH_LIZARD] = {
        .anm = SPRITE_BATTLE_SALAMANDER,
        .pal = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 32,
    },
    /* Thunder Lizard */
    [ENEMY_THUNDER_LIZARD] = {
        .anm = SPRITE_BATTLE_SALAMANDER,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 32,
    },
    /* Manticore */
    [ENEMY_MANTICORE] = {
        .anm = SPRITE_BATTLE_MANTICORE,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 7,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Magicore */
    [ENEMY_MAGICORE] = {
        .anm = SPRITE_BATTLE_MANTICORE,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 7,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Manticore King */
    [ENEMY_MANTICORE_KING] = {
        .anm = SPRITE_BATTLE_MANTICORE,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 7,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Kobold */
    [ENEMY_KOBOLD_UNUSED] = {
        .anm = SPRITE_BATTLE_GOBLIN,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Goblin */
    [ENEMY_GOBLIN] = {
        .anm = SPRITE_BATTLE_GOBLIN,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Hobgoblin */
    [ENEMY_HOBGOBLIN] = {
        .anm = SPRITE_BATTLE_GOBLIN,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Gargoyle */
    [ENEMY_GARGOYLE] = {
        .anm = SPRITE_BATTLE_GARGOYLE,
        .attackHop = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 64,
    },
    /* Clay Gargoyle */
    [ENEMY_CLAY_GARGOYLE] = {
        .anm = SPRITE_BATTLE_GARGOYLE,
        .attackHop = 5,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 64,
    },
    /* Ice Gargoyle */
    [ENEMY_ICE_GARGOYLE] = {
        .anm = SPRITE_BATTLE_GARGOYLE,
        .attackHop = 5,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 64,
    },
    /* Gryphon */
    [ENEMY_GRYPHON] = {
        .anm = SPRITE_BATTLE_GRYPHON,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 64,
    },
    /* Wild Gryphon */
    [ENEMY_WILD_GRYPHON] = {
        .anm = SPRITE_BATTLE_GRYPHON,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 64,
    },
    /* Wise Gryphon */
    [ENEMY_WISE_GRYPHON] = {
        .anm = SPRITE_BATTLE_GRYPHON,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 64,
    },
    /* Golem */
    [ENEMY_GOLEM] = {
        .anm = SPRITE_BATTLE_GOLEM,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Earth Golem */
    [ENEMY_EARTH_GOLEM] = {
        .anm = SPRITE_BATTLE_GOLEM,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Grand Golem */
    [ENEMY_GRAND_GOLEM] = {
        .anm = SPRITE_BATTLE_GOLEM,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Dread Hound */
    [ENEMY_DREAD_HOUND] = {
        .anm = SPRITE_BATTLE_DREAD_HOUND,
        .attackHop = 6,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Cerberus */
    [ENEMY_CERBERUS] = {
        .anm = SPRITE_BATTLE_DREAD_HOUND,
        .attackHop = 6,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Fenrir */
    [ENEMY_FENRIR] = {
        .anm = SPRITE_BATTLE_DREAD_HOUND,
        .attackHop = 6,
        .pal = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Stone Soldier */
    [ENEMY_STONE_SOLDIER] = {
        .anm = SPRITE_BATTLE_STONE_SOLDIER,
        .isSmall = 1,
        .attackHop = 7,
        .effHeight = 32,
    },
    /* Boulder Beast */
    [ENEMY_BOULDER_BEAST] = {
        .anm = SPRITE_BATTLE_STONE_SOLDIER,
        .isSmall = 1,
        .attackHop = 7,
        .pal = 1,
        .effHeight = 32,
    },
    /* Raging Rock */
    [ENEMY_RAGING_ROCK] = {
        .anm = SPRITE_BATTLE_STONE_SOLDIER,
        .isSmall = 1,
        .attackHop = 7,
        .pal = 4,
        .effHeight = 32,
    },
    /* Chimera */
    [ENEMY_CHIMERA] = {
        .anm = SPRITE_BATTLE_CHIMERA,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Chimera Mage */
    [ENEMY_CHIMERA_MAGE] = {
        .anm = SPRITE_BATTLE_CHIMERA,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Grand Chimera */
    [ENEMY_GRAND_CHIMERA] = {
        .anm = SPRITE_BATTLE_CHIMERA,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Amaze */
    [ENEMY_AMAZE_SOL_SANCTUM] = {
        .anm = SPRITE_BATTLE_AMAZE,
    },
    /* Amaze */
    [ENEMY_AMAZE_KANDOREAN_TEMPLE] = {
        .anm = SPRITE_BATTLE_AMAZE,
    },
    /* Creeper */
    [ENEMY_CREEPER] = {
        .anm = SPRITE_BATTLE_AMAZE,
        .pal = 1,
    },
    /* Spirit */
    [ENEMY_SPIRIT] = {
        .anm = SPRITE_BATTLE_AMAZE,
        .pal = 3,
    },
    /* Lizard Man */
    [ENEMY_LIZARD_MAN] = {
        .anm = SPRITE_BATTLE_LIZARD_MAN,
        .attackHop = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Lizard Fighter */
    [ENEMY_LIZARD_FIGHTER] = {
        .anm = SPRITE_BATTLE_LIZARD_MAN,
        .attackHop = 5,
        .pal = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Lizard King */
    [ENEMY_LIZARD_KING] = {
        .anm = SPRITE_BATTLE_LIZARD_MAN,
        .attackHop = 5,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Ant Lion */
    [ENEMY_ANT_LION] = {
        .anm = SPRITE_BATTLE_ANT_LION,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 32,
    },
    /* Roach */
    [ENEMY_ROACH] = {
        .anm = SPRITE_BATTLE_ANT_LION,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 32,
    },
    /* Doodle Bug */
    [ENEMY_DOODLE_BUG] = {
        .anm = SPRITE_BATTLE_ANT_LION,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 32,
    },
    /* Toadonpa */
    [ENEMY_TOADONPA] = {
        .anm = SPRITE_BATTLE_TOADONPA,
        .attackHop = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 42,
    },
    /* Poison Toad */
    [ENEMY_POISON_TOAD] = {
        .anm = SPRITE_BATTLE_TOADONPA,
        .attackHop = 5,
        .pal = 3,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 42,
    },
    /* Devil Frog */
    [ENEMY_DEVIL_FROG] = {
        .anm = SPRITE_BATTLE_TOADONPA,
        .attackHop = 5,
        .pal = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 42,
    },
    /* Living Statue */
    [ENEMY_LIVING_STATUE] = {
        .anm = SPRITE_BATTLE_LIVING_STATUE,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 48,
    },
    /* Hydros Statue */
    [ENEMY_HYDROS_STATUE] = {
        .anm = SPRITE_BATTLE_LIVING_STATUE,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
        .effHeight = 48,
    },
    /* Azart */
    [ENEMY_AZART_IRON_SHIELD] = {
        .anm = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Azart */
    [ENEMY_AZART_PADDED_GLOVES] = {
        .anm = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Satrage */
    [ENEMY_SATRAGE_CHAIN_MAIL] = {
        .anm = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Satrage */
    [ENEMY_SATRAGE_COTTON_SHIRT] = {
        .anm = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Navampa */
    [ENEMY_NAVAMPA_CLAYMORE] = {
        .anm = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Navampa */
    [ENEMY_NAVAMPA_HUNTERS_SWORD] = {
        .anm = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Tret */
    [ENEMY_TRET] = {
        .anm = SPRITE_BATTLE_TRET,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Kraken */
    [ENEMY_KRAKEN] = {
        .anm = SPRITE_BATTLE_KRAKEN,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Tornado Lizard */
    [ENEMY_TORNADO_LIZARD] = {
        .anm = SPRITE_BATTLE_TORNADO_LIZARD,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Storm Lizard */
    [ENEMY_STORM_LIZARD] = {
        .anm = SPRITE_BATTLE_TORNADO_LIZARD,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Tempest Lizard */
    [ENEMY_TEMPEST_LIZARD] = {
        .anm = SPRITE_BATTLE_TORNADO_LIZARD,
        .attackEff = ATTACK_EFFECT_CLAW,
        .attackHop = 5,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Mystery Man */
    [ENEMY_MYSTERY_MAN] = {
        .anm = SPRITE_BATTLE_SATUROS,
        .attackHop = 5,
    },
    /* Saturos */
    [ENEMY_SATUROS_MERCURY_LIGHTHOUSE] = {
        .anm = SPRITE_BATTLE_SATUROS,
        .attackHop = 5,
    },
    /* Saturos */
    [ENEMY_SATUROS_VENUS_LIGHTHOUSE] = {
        .anm = SPRITE_BATTLE_SATUROS,
        .attackHop = 5,
        .heavyImpact = 1,
    },
    /* Mystery Woman */
    [ENEMY_MYSTERY_WOMAN] = {
        .anm = SPRITE_BATTLE_MENARDI,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
    },
    /* Menardi */
    [ENEMY_MENARDI] = {
        .anm = SPRITE_BATTLE_MENARDI,
        .attackEff = ATTACK_EFFECT_RISING_SLASH,
        .heavyImpact = 1,
    },
    /* Fusion Dragon */
    [ENEMY_FUSION_DRAGON] = {
        .anm = SPRITE_BATTLE_FUSION_DRAGON,
        .attackHop = 6,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_MONSTER_ROAR,
    },
    /* Deadbeard */
    [ENEMY_DEADBEARD] = {
        .anm = 0,
    },
    /* Mimic */
    [ENEMY_MIMIC_KANDOREAN_TEMPLE] = {
        .anm = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effHeight = 32,
    },
    /* Mimic */
    [ENEMY_MIMIC_AIRS_ROCK] = {
        .anm = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effHeight = 32,
    },
    /* Mimic */
    [ENEMY_MIMIC_GABOMBA_STATUE] = {
        .anm = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effHeight = 32,
    },
    /* Mimic */
    [ENEMY_MIMIC_AQUA_ROCK] = {
        .anm = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effHeight = 32,
    },
    /* Mimic */
    [ENEMY_MIMIC_GAIA_ROCK] = {
        .anm = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effHeight = 32,
    },
    /* Mimic */
    [ENEMY_MIMIC_TREASURE_ISLE] = {
        .anm = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effHeight = 32,
    },
    /* Mimic */
    [ENEMY_MIMIC_JUPITER_LIGHTHOUSE] = {
        .anm = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effHeight = 32,
    },
    /* Mimic */
    [ENEMY_MIMIC_MAGMA_ROCK] = {
        .anm = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effHeight = 32,
    },
    /* Mimic */
    [ENEMY_MIMIC_MARS_LIGHTHOUSE] = {
        .anm = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effHeight = 32,
    },
    /* Zombie */
    [ENEMY_ZOMBIE_DJINN_LESSON] = {
        .anm = SPRITE_BATTLE_ZOMBIE,
        .attackEff = ATTACK_EFFECT_SLASH,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Doom Dragon */
    [ENEMY_DOOM_DRAGON_A] = {
        .anm = SPRITE_BATTLE_DOOM_DRAGON_A,
        .attackHop = 6,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_BOSS_ROAR,
    },
    /* Doom Dragon */
    [ENEMY_DOOM_DRAGON_B] = {
        .anm = 0,
    },
    /* Doom Dragon */
    [ENEMY_DOOM_DRAGON_C] = {
        .anm = 0,
    },
    /* Doom Dragon */
    [ENEMY_DOOM_DRAGON_D] = {
        .anm = 0,
    },
    /* Doom Dragon */
    [ENEMY_DOOM_DRAGON_E] = {
        .anm = SPRITE_BATTLE_DOOM_DRAGON_E,
        .attackHop = 6,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_MONSTER_ROAR,
    },
    /* Doom Dragon */
    [ENEMY_DOOM_DRAGON_F] = {
        .anm = 0,
    },
    /* Doom Dragon */
    [ENEMY_DOOM_DRAGON_G] = {
        .anm = 0,
    },
    /* Doom Dragon */
    [ENEMY_DOOM_DRAGON_H] = {
        .anm = SPRITE_BATTLE_DOOM_DRAGON_H,
        .attackHop = 6,
        .heavyImpact = 1,
        .defeatSnd = DEFEAT_SOUND_MONSTER_ROAR,
    },
    /* Doom Dragon */
    [ENEMY_DOOM_DRAGON_I] = {
        .anm = 0,
    },
    /* Refresh Ball */
    [ENEMY_REFRESH_BALL] = {
        .anm = SPRITE_BATTLE_REFRESH_BALL,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_BLOW,
    },
    /* Thunder Ball */
    [ENEMY_THUNDER_BALL] = {
        .anm = SPRITE_BATTLE_REFRESH_BALL,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_BLOW,
        .pal = 1,
    },
    /* Anger Ball */
    [ENEMY_ANGER_BALL] = {
        .anm = SPRITE_BATTLE_REFRESH_BALL,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_BLOW,
        .pal = 5,
    },
    /* Guardian Ball */
    [ENEMY_GUARDIAN_BALL] = {
        .anm = SPRITE_BATTLE_REFRESH_BALL,
        .isSmall = 1,
        .attackEff = ATTACK_EFFECT_BLOW,
        .pal = 4,
    },
    /* Azart */
    [ENEMY_AZART_SHAMAN_VILLAGE_CAVE] = {
        .anm = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Satrage */
    [ENEMY_SATRAGE_SHAMAN_VILLAGE_CAVE] = {
        .anm = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .pal = 1,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Navampa */
    [ENEMY_NAVAMPA_SHAMAN_VILLAGE_CAVE] = {
        .anm = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .pal = 2,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Bandit */
    [ENEMY_BANDIT_WORLD_MAP] = {
        .anm = SPRITE_BATTLE_BANDIT,
        .attackEff = ATTACK_EFFECT_SLASH,
        .attackHop = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
    /* Thief */
    [ENEMY_THIEF_WORLD_MAP] = {
        .anm = SPRITE_BATTLE_BANDIT,
        .attackEff = ATTACK_EFFECT_SLASH,
        .pal = 4,
        .defeatSnd = DEFEAT_SOUND_BEAST_CRY,
    },
};
/* clang-format on */
