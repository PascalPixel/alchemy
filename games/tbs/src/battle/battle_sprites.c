/// @file
/// The battle sprite rows, by unit id: the eight characters', then the
/// enemies'. Each enemy's name is the English release's.

#include "battle_sprites.h"
#include "sprite_ids.h"
#include "character_ids.h"
#include "enemy_ids.h"

// clang-format off
const struct BattleSpriteDefinition gBattleSprites[] = {
    // The characters' rows; their sprite, palette and attack effect go unused.
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
        .animation = 0,
    },
    [JENNA] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .attackEffect = 1,
        .attackHop = 5,
    },
    [SHEBA] = {
        .animation = 0,
    },
    [PIERS] = {
        .animation = 0,
    },
    // Unused: "???", in no formation.
    [ENEMY_000] = {
        .animation = 0,
    },
    // Vermin
    [ENEMY_VERMIN] = {
        .animation = SPRITE_BATTLE_VERMIN,
        .isSmall = 1,
        .attackEffect = 3,
        .attackHop = 2,
    },
    // Vermin
    [ENEMY_VERMIN_2] = {
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
    [ENEMY_BAT_2] = {
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
    // Wild Mushroom
    [ENEMY_WILD_MUSHROOM] = {
        .animation = SPRITE_BATTLE_WILD_MUSHROOM,
        .isSmall = 1,
        .attackEffect = 4,
    },
    // Wild Mushroom
    [ENEMY_WILD_MUSHROOM_2] = {
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
    [ENEMY_SLIME_2] = {
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
    [ENEMY_LICH] = {
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
    [ENEMY_KOBOLD] = {
        .animation = SPRITE_BATTLE_KOBOLD,
        .defeatSound = 1,
    },
    // Goblin
    [ENEMY_GOBLIN] = {
        .animation = SPRITE_BATTLE_KOBOLD,
        .palette = 1,
        .defeatSound = 1,
    },
    // Hobgoblin
    [ENEMY_HOBGOBLIN] = {
        .animation = SPRITE_BATTLE_KOBOLD,
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
    // Cerebus
    [ENEMY_CEREBUS] = {
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
    [ENEMY_104] = {
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
    [ENEMY_AMAZE_WORLD_MAP] = {
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
    // Zombie
    [ENEMY_ZOMBIE_DJINN_LESSON] = {
        .animation = SPRITE_BATTLE_ZOMBIE,
        .attackEffect = 1,
        .defeatSound = 1,
    },
    // Unused: "?", in no formation.
    [ENEMY_110] = {
        .animation = 0,
    },
    // Unused: "?", in no formation.
    [ENEMY_111] = {
        .animation = 0,
    },
    // Unused: "?", in no formation.
    [ENEMY_112] = {
        .animation = 0,
    },
    // Kraken
    [ENEMY_KRAKEN] = {
        .animation = SPRITE_BATTLE_KRAKEN,
        .defeatSound = 2,
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
    [ENEMY_135] = {
        .animation = SPRITE_BATTLE_TOADONPA,
        .attackHop = 5,
        .palette = 5,
        .defeatSound = 1,
        .effectHeight = 42,
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
    // Tret
    [ENEMY_TRET] = {
        .animation = SPRITE_BATTLE_TRET,
        .attackEffect = 5,
        .defeatSound = 1,
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
        .animation = SPRITE_BATTLE_DEADBEARD,
        .attackHop = 5,
        .defeatSound = 1,
    },
    // Mimic
    [ENEMY_MIMIC_MERCURY_LIGHTHOUSE] = {
        .animation = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effectHeight = 32,
    },
    // Mimic
    [ENEMY_MIMIC_FUCHIN_FALLS_CAVE] = {
        .animation = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effectHeight = 32,
    },
    // Mimic
    [ENEMY_MIMIC_ALTIN_PEAK] = {
        .animation = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effectHeight = 32,
    },
    // Mimic
    [ENEMY_MIMIC_VAULT_CAVE] = {
        .animation = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effectHeight = 32,
    },
    // Mimic
    [ENEMY_MIMIC_CROSSBONE_ISLE] = {
        .animation = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effectHeight = 32,
    },
    // Mimic
    [ENEMY_MIMIC_ALTMILLER_CAVE] = {
        .animation = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effectHeight = 32,
    },
    // Mimic
    [ENEMY_MIMIC_SUHALLA_DESERT] = {
        .animation = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effectHeight = 32,
    },
    // Mimic
    [ENEMY_MIMIC_PIRATE_SHIP] = {
        .animation = SPRITE_BATTLE_MIMIC,
        .attackHop = 6,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_QUARTZ] = {
        .animation = SPRITE_BATTLE_VENUS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
        .effectHeight = 32,
    },
    // Venus Djinni
    [ENEMY_VENUS_DJINNI_VINE] = {
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
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_SLEET] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_SPRITZ] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_HAIL] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Mercury Djinni
    [ENEMY_MERCURY_DJINNI_DEW] = {
        .animation = SPRITE_BATTLE_MERCURY_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_FORGE] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_CORONA] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
    },
    // Mars Djinni
    [ENEMY_MARS_DJINNI_FLASH] = {
        .animation = SPRITE_BATTLE_MARS_DJINNI,
        .attackEffect = 4,
        .attackHop = 2,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_BREEZE] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_ZEPHYR] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_SQUALL] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
    // Jupiter Djinni
    [ENEMY_JUPITER_DJINNI_LUFF] = {
        .animation = SPRITE_BATTLE_JUPITER_DJINNI,
        .attackEffect = 4,
        .attackHop = 5,
    },
};
// clang-format on
