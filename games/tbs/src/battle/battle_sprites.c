/* @file */
/* The battle sprite rows, by unit id: the eight characters', then the */
/* enemies'. Each enemy's name is the English release's. */

#include "battle/battle_sprites.h"
#include "enemy_ids.h"
#include "sprite_ids.h"
#include "game/character_ids.h"

/* clang-format off */
const BATTLE_SPR_DEFINITION BattleSprites[] = {
    /* The characters' rows; their sprite, palette and attack effect go unused. */
    [ISAAC] = {
        .anm = SPRITE_BATTLE_VERMIN,
        .attackEff = 1,
        .attackHop = 5,
    },
    [GARET] = {
        .anm = SPRITE_BATTLE_VERMIN,
        .attackEff = 1,
        .attackHop = 4,
    },
    [IVAN] = {
        .anm = SPRITE_BATTLE_VERMIN,
        .attackEff = 4,
        .attackHop = 2,
    },
    [MIA] = {
        .anm = SPRITE_BATTLE_VERMIN,
        .attackEff = 4,
        .attackHop = 2,
    },
    [FELIX] = {
        .anm = 0,
    },
    [JENNA] = {
        .anm = SPRITE_BATTLE_VERMIN,
        .attackEff = 1,
        .attackHop = 5,
    },
    [SHEBA] = {
        .anm = 0,
    },
    [PIERS] = {
        .anm = 0,
    },
    /* Unused: "???", in no formation. */
    [ENEMY_NONE] = {
        .anm = 0,
    },
    /* Vermin */
    [ENEMY_VERMIN] = {
        .anm = SPRITE_BATTLE_VERMIN,
        .isSmall = 1,
        .attackEff = 3,
        .attackHop = 2,
    },
    /* Vermin */
    [ENEMY_VERMIN_WORLD_MAP] = {
        .anm = SPRITE_BATTLE_VERMIN,
        .isSmall = 1,
        .attackEff = 3,
        .attackHop = 2,
    },
    /* Mad Vermin */
    [ENEMY_MAD_VERMIN] = {
        .anm = SPRITE_BATTLE_VERMIN,
        .isSmall = 1,
        .attackEff = 3,
        .attackHop = 2,
        .pal = 5,
    },
    /* Bat */
    [ENEMY_BAT] = {
        .anm = SPRITE_BATTLE_BAT,
        .isSmall = 1,
        .attackEff = 4,
        .attackHop = 6,
    },
    /* Bat */
    [ENEMY_BAT_WORLD_MAP] = {
        .anm = SPRITE_BATTLE_BAT,
        .isSmall = 1,
        .attackEff = 4,
        .attackHop = 6,
    },
    /* Rabid Bat */
    [ENEMY_RABID_BAT] = {
        .anm = SPRITE_BATTLE_BAT,
        .isSmall = 1,
        .attackEff = 4,
        .attackHop = 6,
        .pal = 4,
    },
    /* Wild Mushroom */
    [ENEMY_WILD_MUSHROOM] = {
        .anm = SPRITE_BATTLE_WILD_MUSHROOM,
        .isSmall = 1,
        .attackEff = 4,
    },
    /* Wild Mushroom */
    [ENEMY_WILD_MUSHROOM_WORLD_MAP] = {
        .anm = SPRITE_BATTLE_WILD_MUSHROOM,
        .isSmall = 1,
        .attackEff = 4,
    },
    /* Death Cap */
    [ENEMY_DEATH_CAP] = {
        .anm = SPRITE_BATTLE_WILD_MUSHROOM,
        .isSmall = 1,
        .attackEff = 4,
        .pal = 1,
    },
    /* Slime */
    [ENEMY_SLIME_SOL_SANCTUM] = {
        .anm = SPRITE_BATTLE_SLIME,
        .isSmall = 1,
        .attackEff = 1,
        .pal = 1,
    },
    /* Slime */
    [ENEMY_SLIME_WORLD_MAP] = {
        .anm = SPRITE_BATTLE_SLIME,
        .isSmall = 1,
        .attackEff = 1,
        .pal = 1,
    },
    /* Ooze */
    [ENEMY_OOZE] = {
        .anm = SPRITE_BATTLE_SLIME,
        .isSmall = 1,
        .attackEff = 1,
        .pal = 3,
    },
    /* Slime Beast */
    [ENEMY_SLIME_BEAST] = {
        .anm = SPRITE_BATTLE_SLIME,
        .isSmall = 1,
        .attackEff = 1,
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
    [ENEMY_LICH] = {
        .anm = SPRITE_BATTLE_GHOST,
        .pal = 2,
    },
    /* Zombie */
    [ENEMY_ZOMBIE] = {
        .anm = SPRITE_BATTLE_ZOMBIE,
        .attackEff = 1,
        .defeatSnd = 1,
    },
    /* Undead */
    [ENEMY_UNDEAD] = {
        .anm = SPRITE_BATTLE_ZOMBIE,
        .attackEff = 1,
        .pal = 1,
        .defeatSnd = 1,
    },
    /* Wight */
    [ENEMY_WIGHT] = {
        .anm = SPRITE_BATTLE_ZOMBIE,
        .attackEff = 1,
        .pal = 4,
        .defeatSnd = 1,
    },
    /* Bandit */
    [ENEMY_BANDIT] = {
        .anm = SPRITE_BATTLE_BANDIT,
        .attackEff = 1,
        .attackHop = 4,
        .defeatSnd = 1,
    },
    /* Thief */
    [ENEMY_THIEF] = {
        .anm = SPRITE_BATTLE_BANDIT,
        .attackEff = 1,
        .pal = 4,
        .defeatSnd = 1,
    },
    /* Brigand */
    [ENEMY_BRIGAND] = {
        .anm = SPRITE_BATTLE_BANDIT,
        .attackEff = 1,
        .defeatSnd = 1,
    },
    /* Skeleton */
    [ENEMY_SKELETON] = {
        .anm = SPRITE_BATTLE_SKELETON,
        .attackEff = 1,
        .defeatSnd = 1,
    },
    /* Bone Fighter */
    [ENEMY_BONE_FIGHTER] = {
        .anm = SPRITE_BATTLE_SKELETON,
        .attackEff = 1,
        .attackHop = 4,
        .pal = 3,
        .defeatSnd = 1,
    },
    /* Skull Warrior */
    [ENEMY_SKULL_WARRIOR] = {
        .anm = SPRITE_BATTLE_SKELETON,
        .attackEff = 1,
        .attackHop = 7,
        .pal = 2,
        .defeatSnd = 1,
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
        .attackEff = 5,
        .attackHop = 5,
        .effHeight = 8,
    },
    /* Armored Rat */
    [ENEMY_ARMORED_RAT] = {
        .anm = SPRITE_BATTLE_RAT,
        .isSmall = 1,
        .attackEff = 5,
        .attackHop = 5,
        .pal = 1,
        .effHeight = 8,
    },
    /* Plated Rat */
    [ENEMY_PLATED_RAT] = {
        .anm = SPRITE_BATTLE_RAT,
        .isSmall = 1,
        .attackEff = 5,
        .attackHop = 5,
        .pal = 4,
        .effHeight = 8,
    },
    /* Rat Soldier */
    [ENEMY_RAT_SOLDIER] = {
        .anm = SPRITE_BATTLE_RAT_SOLDIER,
        .attackEff = 3,
        .attackHop = 4,
    },
    /* Rat Fighter */
    [ENEMY_RAT_FIGHTER] = {
        .anm = SPRITE_BATTLE_RAT_SOLDIER,
        .attackEff = 3,
        .attackHop = 4,
        .pal = 4,
    },
    /* Rat Warrior */
    [ENEMY_RAT_WARRIOR] = {
        .anm = SPRITE_BATTLE_RAT_SOLDIER,
        .attackEff = 3,
        .attackHop = 4,
        .pal = 2,
    },
    /* Drone Bee */
    [ENEMY_DRONE_BEE] = {
        .anm = SPRITE_BATTLE_DRONE_BEE,
        .isSmall = 1,
        .attackEff = 3,
        .attackHop = 6,
    },
    /* Fighter Bee */
    [ENEMY_FIGHTER_BEE] = {
        .anm = SPRITE_BATTLE_DRONE_BEE,
        .isSmall = 1,
        .attackEff = 3,
        .attackHop = 6,
        .pal = 1,
    },
    /* Warrior Bee */
    [ENEMY_WARRIOR_BEE] = {
        .anm = SPRITE_BATTLE_DRONE_BEE,
        .isSmall = 1,
        .attackEff = 3,
        .attackHop = 6,
        .pal = 3,
    },
    /* Troll */
    [ENEMY_TROLL] = {
        .anm = SPRITE_BATTLE_TROLL,
        .defeatSnd = 1,
    },
    /* Cave Troll */
    [ENEMY_CAVE_TROLL] = {
        .anm = SPRITE_BATTLE_TROLL,
        .pal = 5,
        .defeatSnd = 1,
    },
    /* Brutal Troll */
    [ENEMY_BRUTAL_TROLL] = {
        .anm = SPRITE_BATTLE_TROLL,
        .pal = 2,
        .defeatSnd = 1,
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
        .defeatSnd = 1,
    },
    /* Fiendish Ghoul */
    [ENEMY_FIENDISH_GHOUL] = {
        .anm = SPRITE_BATTLE_GHOUL,
        .pal = 2,
        .defeatSnd = 1,
    },
    /* Cannibal Ghoul */
    [ENEMY_CANNIBAL_GHOUL] = {
        .anm = SPRITE_BATTLE_GHOUL,
        .pal = 4,
        .defeatSnd = 1,
    },
    /* Mauler */
    [ENEMY_MAULER] = {
        .anm = SPRITE_BATTLE_MAULER,
        .attackEff = 2,
        .attackHop = 5,
        .defeatSnd = 1,
    },
    /* Ravager */
    [ENEMY_RAVAGER] = {
        .anm = SPRITE_BATTLE_MAULER,
        .attackEff = 2,
        .attackHop = 5,
        .pal = 1,
        .defeatSnd = 1,
    },
    /* Grisly */
    [ENEMY_GRISLY] = {
        .anm = SPRITE_BATTLE_MAULER,
        .attackEff = 2,
        .attackHop = 5,
        .pal = 3,
        .defeatSnd = 1,
    },
    /* Cuttle */
    [ENEMY_CUTTLE] = {
        .anm = SPRITE_BATTLE_CUTTLE,
        .isSmall = 1,
        .attackEff = 3,
    },
    /* Calamar */
    [ENEMY_CALAMAR] = {
        .anm = SPRITE_BATTLE_CUTTLE,
        .isSmall = 1,
        .attackEff = 3,
        .pal = 5,
    },
    /* Man o' War */
    [ENEMY_MAN_O_WAR] = {
        .anm = SPRITE_BATTLE_CUTTLE,
        .isSmall = 1,
        .attackEff = 3,
        .pal = 3,
    },
    /* Harpy */
    [ENEMY_HARPY] = {
        .anm = SPRITE_BATTLE_HARPY,
        .attackEff = 2,
        .attackHop = 5,
        .defeatSnd = 1,
    },
    /* Virago */
    [ENEMY_VIRAGO] = {
        .anm = SPRITE_BATTLE_HARPY,
        .attackEff = 2,
        .attackHop = 5,
        .pal = 2,
        .defeatSnd = 1,
    },
    /* Harridan */
    [ENEMY_HARRIDAN] = {
        .anm = SPRITE_BATTLE_HARPY,
        .attackEff = 2,
        .attackHop = 5,
        .pal = 3,
        .defeatSnd = 1,
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
        .attackEff = 2,
        .effHeight = 16,
    },
    /* Mad Mole */
    [ENEMY_MAD_MOLE] = {
        .anm = SPRITE_BATTLE_MOLE,
        .isSmall = 1,
        .attackEff = 2,
        .pal = 1,
        .effHeight = 16,
    },
    /* Mole Mage */
    [ENEMY_MOLE_MAGE] = {
        .anm = SPRITE_BATTLE_MOLE,
        .isSmall = 1,
        .attackEff = 2,
        .pal = 2,
        .effHeight = 16,
    },
    /* Dirge */
    [ENEMY_DIRGE] = {
        .anm = SPRITE_BATTLE_DIRGE,
        .isSmall = 1,
        .attackEff = 2,
        .attackHop = 6,
    },
    /* Foul Dirge */
    [ENEMY_FOUL_DIRGE] = {
        .anm = SPRITE_BATTLE_DIRGE,
        .isSmall = 1,
        .attackEff = 2,
        .attackHop = 6,
        .pal = 4,
    },
    /* Vile Dirge */
    [ENEMY_VILE_DIRGE] = {
        .anm = SPRITE_BATTLE_DIRGE,
        .isSmall = 1,
        .attackEff = 2,
        .attackHop = 6,
        .pal = 3,
    },
    /* Ape */
    [ENEMY_APE] = {
        .anm = SPRITE_BATTLE_APE,
        .attackHop = 5,
        .defeatSnd = 1,
    },
    /* Killer Ape */
    [ENEMY_KILLER_APE] = {
        .anm = SPRITE_BATTLE_APE,
        .attackHop = 5,
        .pal = 3,
        .defeatSnd = 1,
    },
    /* Dirty Ape */
    [ENEMY_DIRTY_APE] = {
        .anm = SPRITE_BATTLE_APE,
        .attackHop = 5,
        .pal = 2,
        .defeatSnd = 1,
    },
    /* Grub */
    [ENEMY_GRUB] = {
        .anm = SPRITE_BATTLE_GRUB,
        .isSmall = 1,
        .attackEff = 3,
    },
    /* Worm */
    [ENEMY_WORM] = {
        .anm = SPRITE_BATTLE_GRUB,
        .isSmall = 1,
        .attackEff = 3,
        .pal = 3,
    },
    /* Acid Maggot */
    [ENEMY_ACID_MAGGOT] = {
        .anm = SPRITE_BATTLE_GRUB,
        .isSmall = 1,
        .attackEff = 3,
        .pal = 1,
    },
    /* Orc */
    [ENEMY_ORC] = {
        .anm = SPRITE_BATTLE_ORC,
        .attackHop = 3,
        .defeatSnd = 1,
    },
    /* Orc Captain */
    [ENEMY_ORC_CAPTAIN] = {
        .anm = SPRITE_BATTLE_ORC,
        .attackHop = 3,
        .pal = 2,
        .defeatSnd = 1,
    },
    /* Orc Lord */
    [ENEMY_ORC_LORD] = {
        .anm = SPRITE_BATTLE_ORC,
        .attackHop = 3,
        .pal = 3,
        .defeatSnd = 1,
    },
    /* Salamander */
    [ENEMY_SALAMANDER] = {
        .anm = SPRITE_BATTLE_SALAMANDER,
        .defeatSnd = 1,
        .effHeight = 32,
    },
    /* Earth Lizard */
    [ENEMY_EARTH_LIZARD] = {
        .anm = SPRITE_BATTLE_SALAMANDER,
        .pal = 5,
        .defeatSnd = 1,
        .effHeight = 32,
    },
    /* Thunder Lizard */
    [ENEMY_THUNDER_LIZARD] = {
        .anm = SPRITE_BATTLE_SALAMANDER,
        .pal = 3,
        .defeatSnd = 1,
        .effHeight = 32,
    },
    /* Manticore */
    [ENEMY_MANTICORE] = {
        .anm = SPRITE_BATTLE_MANTICORE,
        .attackEff = 2,
        .attackHop = 7,
        .defeatSnd = 1,
    },
    /* Magicore */
    [ENEMY_MAGICORE] = {
        .anm = SPRITE_BATTLE_MANTICORE,
        .attackEff = 2,
        .attackHop = 7,
        .pal = 1,
        .defeatSnd = 1,
    },
    /* Manticore King */
    [ENEMY_MANTICORE_KING] = {
        .anm = SPRITE_BATTLE_MANTICORE,
        .attackEff = 2,
        .attackHop = 7,
        .pal = 4,
        .defeatSnd = 1,
    },
    /* Kobold */
    [ENEMY_KOBOLD] = {
        .anm = SPRITE_BATTLE_KOBOLD,
        .defeatSnd = 1,
    },
    /* Goblin */
    [ENEMY_GOBLIN] = {
        .anm = SPRITE_BATTLE_KOBOLD,
        .pal = 1,
        .defeatSnd = 1,
    },
    /* Hobgoblin */
    [ENEMY_HOBGOBLIN] = {
        .anm = SPRITE_BATTLE_KOBOLD,
        .pal = 2,
        .defeatSnd = 1,
    },
    /* Gargoyle */
    [ENEMY_GARGOYLE] = {
        .anm = SPRITE_BATTLE_GARGOYLE,
        .attackHop = 5,
        .defeatSnd = 1,
        .effHeight = 64,
    },
    /* Clay Gargoyle */
    [ENEMY_CLAY_GARGOYLE] = {
        .anm = SPRITE_BATTLE_GARGOYLE,
        .attackHop = 5,
        .pal = 3,
        .defeatSnd = 1,
        .effHeight = 64,
    },
    /* Ice Gargoyle */
    [ENEMY_ICE_GARGOYLE] = {
        .anm = SPRITE_BATTLE_GARGOYLE,
        .attackHop = 5,
        .pal = 1,
        .defeatSnd = 1,
        .effHeight = 64,
    },
    /* Gryphon */
    [ENEMY_GRYPHON] = {
        .anm = SPRITE_BATTLE_GRYPHON,
        .attackEff = 5,
        .defeatSnd = 1,
        .effHeight = 64,
    },
    /* Wild Gryphon */
    [ENEMY_WILD_GRYPHON] = {
        .anm = SPRITE_BATTLE_GRYPHON,
        .attackEff = 5,
        .pal = 1,
        .defeatSnd = 1,
        .effHeight = 64,
    },
    /* Wise Gryphon */
    [ENEMY_WISE_GRYPHON] = {
        .anm = SPRITE_BATTLE_GRYPHON,
        .attackEff = 5,
        .pal = 4,
        .defeatSnd = 1,
        .effHeight = 64,
    },
    /* Golem */
    [ENEMY_GOLEM] = {
        .anm = SPRITE_BATTLE_GOLEM,
        .defeatSnd = 1,
    },
    /* Earth Golem */
    [ENEMY_EARTH_GOLEM] = {
        .anm = SPRITE_BATTLE_GOLEM,
        .pal = 1,
        .defeatSnd = 1,
    },
    /* Grand Golem */
    [ENEMY_GRAND_GOLEM] = {
        .anm = SPRITE_BATTLE_GOLEM,
        .pal = 2,
        .defeatSnd = 1,
    },
    /* Dread Hound */
    [ENEMY_DREAD_HOUND] = {
        .anm = SPRITE_BATTLE_DREAD_HOUND,
        .attackHop = 6,
        .pal = 1,
        .defeatSnd = 1,
    },
    /* Cerebus */
    [ENEMY_CEREBUS] = {
        .anm = SPRITE_BATTLE_DREAD_HOUND,
        .attackHop = 6,
        .defeatSnd = 1,
    },
    /* Fenrir */
    [ENEMY_FENRIR] = {
        .anm = SPRITE_BATTLE_DREAD_HOUND,
        .attackHop = 6,
        .pal = 5,
        .defeatSnd = 1,
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
        .attackEff = 2,
        .attackHop = 5,
        .defeatSnd = 1,
    },
    /* Chimera Mage */
    [ENEMY_CHIMERA_MAGE] = {
        .anm = SPRITE_BATTLE_CHIMERA,
        .attackEff = 2,
        .attackHop = 5,
        .pal = 2,
        .defeatSnd = 1,
    },
    [ENEMY_UNUSED_CHIMERA] = {
        .anm = SPRITE_BATTLE_CHIMERA,
        .attackEff = 2,
        .attackHop = 5,
        .pal = 3,
        .defeatSnd = 1,
    },
    /* Amaze */
    [ENEMY_AMAZE_SOL_SANCTUM] = {
        .anm = SPRITE_BATTLE_AMAZE,
    },
    /* Amaze */
    [ENEMY_AMAZE_WORLD_MAP] = {
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
    /* Zombie */
    [ENEMY_ZOMBIE_DJINN_LESSON] = {
        .anm = SPRITE_BATTLE_ZOMBIE,
        .attackEff = 1,
        .defeatSnd = 1,
    },
    /* Kraken */
    [ENEMY_KRAKEN] = {
        .anm = SPRITE_BATTLE_KRAKEN,
        .defeatSnd = 2,
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
        .attackEff = 5,
    },
    /* Menardi */
    [ENEMY_MENARDI] = {
        .anm = SPRITE_BATTLE_MENARDI,
        .attackEff = 5,
        .heavyImpact = 1,
    },
    /* Lizard Man */
    [ENEMY_LIZARD_MAN] = {
        .anm = SPRITE_BATTLE_LIZARD_MAN,
        .attackHop = 5,
        .defeatSnd = 1,
    },
    /* Lizard Fighter */
    [ENEMY_LIZARD_FIGHTER] = {
        .anm = SPRITE_BATTLE_LIZARD_MAN,
        .attackHop = 5,
        .pal = 5,
        .defeatSnd = 1,
    },
    /* Lizard King */
    [ENEMY_LIZARD_KING] = {
        .anm = SPRITE_BATTLE_LIZARD_MAN,
        .attackHop = 5,
        .pal = 1,
        .defeatSnd = 1,
    },
    /* Living Statue */
    [ENEMY_LIVING_STATUE] = {
        .anm = SPRITE_BATTLE_LIVING_STATUE,
        .attackEff = 5,
        .pal = 2,
        .defeatSnd = 1,
        .effHeight = 48,
    },
    /* Hydros Statue */
    [ENEMY_HYDROS_STATUE] = {
        .anm = SPRITE_BATTLE_LIVING_STATUE,
        .attackEff = 5,
        .defeatSnd = 1,
        .effHeight = 48,
    },
    /* Azart */
    [ENEMY_AZART_IRON_SHIELD] = {
        .anm = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .defeatSnd = 1,
    },
    /* Azart */
    [ENEMY_AZART_PADDED_GLOVES] = {
        .anm = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .defeatSnd = 1,
    },
    /* Satrage */
    [ENEMY_SATRAGE_CHAIN_MAIL] = {
        .anm = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .pal = 1,
        .defeatSnd = 1,
    },
    /* Satrage */
    [ENEMY_SATRAGE_COTTON_SHIRT] = {
        .anm = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .pal = 1,
        .defeatSnd = 1,
    },
    /* Navampa */
    [ENEMY_NAVAMPA_CLAYMORE] = {
        .anm = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .pal = 2,
        .defeatSnd = 1,
    },
    /* Navampa */
    [ENEMY_NAVAMPA_HUNTERS_SWORD] = {
        .anm = SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET,
        .attackHop = 5,
        .pal = 2,
        .defeatSnd = 1,
    },
    /* Ant Lion */
    [ENEMY_ANT_LION] = {
        .anm = SPRITE_BATTLE_ANT_LION,
        .attackEff = 5,
        .defeatSnd = 1,
        .effHeight = 32,
    },
    /* Roach */
    [ENEMY_ROACH] = {
        .anm = SPRITE_BATTLE_ANT_LION,
        .attackEff = 5,
        .pal = 3,
        .defeatSnd = 1,
        .effHeight = 32,
    },
    /* Doodle Bug */
    [ENEMY_DOODLE_BUG] = {
        .anm = SPRITE_BATTLE_ANT_LION,
        .attackEff = 5,
        .pal = 2,
        .defeatSnd = 1,
        .effHeight = 32,
    },
    /* Toadonpa */
    [ENEMY_TOADONPA] = {
        .anm = SPRITE_BATTLE_TOADONPA,
        .attackHop = 5,
        .defeatSnd = 1,
        .effHeight = 42,
    },
    /* Poison Toad */
    [ENEMY_POISON_TOAD] = {
        .anm = SPRITE_BATTLE_TOADONPA,
        .attackHop = 5,
        .pal = 3,
        .defeatSnd = 1,
        .effHeight = 42,
    },
    [ENEMY_UNUSED_TOADONPA] = {
        .anm = SPRITE_BATTLE_TOADONPA,
        .attackHop = 5,
        .pal = 5,
        .defeatSnd = 1,
        .effHeight = 42,
    },
    /* Tornado Lizard */
    [ENEMY_TORNADO_LIZARD] = {
        .anm = SPRITE_BATTLE_TORNADO_LIZARD,
        .attackEff = 2,
        .attackHop = 5,
        .defeatSnd = 1,
    },
    /* Storm Lizard */
    [ENEMY_STORM_LIZARD] = {
        .anm = SPRITE_BATTLE_TORNADO_LIZARD,
        .attackEff = 2,
        .attackHop = 5,
        .pal = 1,
        .defeatSnd = 1,
    },
    /* Tempest Lizard */
    [ENEMY_TEMPEST_LIZARD] = {
        .anm = SPRITE_BATTLE_TORNADO_LIZARD,
        .attackEff = 2,
        .attackHop = 5,
        .pal = 4,
        .defeatSnd = 1,
    },
    /* Tret */
    [ENEMY_TRET] = {
        .anm = SPRITE_BATTLE_TRET,
        .attackEff = 5,
        .defeatSnd = 1,
    },
    /* Fusion Dragon */
    [ENEMY_FUSION_DRAGON] = {
        .anm = SPRITE_BATTLE_FUSION_DRAGON,
        .attackHop = 6,
        .heavyImpact = 1,
        .defeatSnd = 3,
    },
    /* Deadbeard */
    [ENEMY_DEADBEARD] = {
        .anm = SPRITE_BATTLE_DEADBEARD,
        .attackHop = 5,
        .defeatSnd = 1,
    },
    /* Mimic */
    [ENEMY_MIMIC_MERCURY_LIGHTHOUSE] = {
        .anm = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effHeight = 32,
    },
    /* Mimic */
    [ENEMY_MIMIC_FUCHIN_FALLS_CAVE] = {
        .anm = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effHeight = 32,
    },
    /* Mimic */
    [ENEMY_MIMIC_ALTIN_PEAK] = {
        .anm = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effHeight = 32,
    },
    /* Mimic */
    [ENEMY_MIMIC_VAULT_CAVE] = {
        .anm = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effHeight = 32,
    },
    /* Mimic */
    [ENEMY_MIMIC_CROSSBONE_ISLE] = {
        .anm = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effHeight = 32,
    },
    /* Mimic */
    [ENEMY_MIMIC_ALTMILLER_CAVE] = {
        .anm = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effHeight = 32,
    },
    /* Mimic */
    [ENEMY_MIMIC_SUHALLA_DESERT] = {
        .anm = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effHeight = 32,
    },
    /* Mimic */
    [ENEMY_MIMIC_PIRATE_SHIP] = {
        .anm = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effHeight = 32,
    },
    /* Venus Djinni */
    [ENEMY_VENUS_DJINNI_QUARTZ] = {
        .anm = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEff = 4,
        .attackHop = 2,
        .effHeight = 32,
    },
    /* Venus Djinni */
    [ENEMY_VENUS_DJINNI_VINE] = {
        .anm = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEff = 4,
        .attackHop = 2,
        .effHeight = 32,
    },
    /* Venus Djinni */
    [ENEMY_VENUS_DJINNI_UNUSED_BANE] = {
        .anm = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEff = 4,
        .attackHop = 2,
        .effHeight = 32,
    },
    /* Mercury Djinni */
    [ENEMY_MERCURY_DJINNI_SLEET] = {
        .anm = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEff = 4,
        .attackHop = 5,
    },
    /* Mercury Djinni */
    [ENEMY_MERCURY_DJINNI_SPRITZ] = {
        .anm = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEff = 4,
        .attackHop = 5,
    },
    /* Mercury Djinni */
    [ENEMY_MERCURY_DJINNI_HAIL] = {
        .anm = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEff = 4,
        .attackHop = 5,
    },
    /* Mercury Djinni */
    [ENEMY_MERCURY_DJINNI_DEW] = {
        .anm = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEff = 4,
        .attackHop = 5,
    },
    /* Mars Djinni */
    [ENEMY_MARS_DJINNI_FORGE] = {
        .anm = SPRITE_BATTLE_MARS_DJINNI,
        .attackEff = 4,
        .attackHop = 2,
    },
    /* Mars Djinni */
    [ENEMY_MARS_DJINNI_CORONA] = {
        .anm = SPRITE_BATTLE_MARS_DJINNI,
        .attackEff = 4,
        .attackHop = 2,
    },
    /* Mars Djinni */
    [ENEMY_MARS_DJINNI_FLASH] = {
        .anm = SPRITE_BATTLE_MARS_DJINNI,
        .attackEff = 4,
        .attackHop = 2,
    },
    /* Jupiter Djinni */
    [ENEMY_JUPITER_DJINNI_BREEZE] = {
        .anm = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEff = 4,
        .attackHop = 5,
    },
    /* Jupiter Djinni */
    [ENEMY_JUPITER_DJINNI_ZEPHYR] = {
        .anm = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEff = 4,
        .attackHop = 5,
    },
    /* Jupiter Djinni */
    [ENEMY_JUPITER_DJINNI_SQUALL] = {
        .anm = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEff = 4,
        .attackHop = 5,
    },
    /* Jupiter Djinni */
    [ENEMY_JUPITER_DJINNI_LUFF] = {
        .anm = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEff = 4,
        .attackHop = 5,
    },
};
/* clang-format on */
