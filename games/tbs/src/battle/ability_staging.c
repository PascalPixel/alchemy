/// @file
/// How each ability is staged in battle: the outcome staging
/// BattleActionRun_Build gives it and the effect it plays, by ability id.
/// The ability names in the comments are the English release's.

#include "battle_command.h"
#include "module/battle_effect/battle_effect.h"

// clang-format off
/// Up to ABILITY_SEVERE_BLOW; 0 leaves the choice to BattleActionRun_Build.
const u8 gAbilityStaging[] = {
    0, // Nothing
    0, // Attack
    0, // Defend
    0, // Quake
    0, // Earthquake
    0, // Quake Sphere
    0, // Spire
    0, // Clay Spire
    0, // Stone Spire
    STAGING_FULL_SCREEN, // Gaia
    STAGING_FULL_SCREEN, // Mother Gaia
    STAGING_FULL_SCREEN, // Grand Gaia
    0, // Growth
    0, // Mad Growth
    0, // Wild Growth
    0, // Thorn
    0, // Briar
    0, // Nettle
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // Frost
    0, // Tundra
    0, // Glacier
    0, // Ice
    0, // Ice Horn
    0, // Ice Missile
    0, // Prism
    0, // Hail Prism
    0, // Freeze Prism
    0, // Douse
    0, // Drench
    0, // Deluge
    0, // Froth
    0, // Froth Sphere
    0, // Froth Spiral
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // Flare
    0, // Flare Wall
    0, // Flare Storm
    0, // Fire
    0, // Fireball
    0, // Inferno
    0, // Volcano
    0, // Eruption
    0, // Pyroclasm
    0, // Blast
    0, // Mad Blast
    0, // Fiery Blast
    0, // Blast
    0, // Nova
    0, // Supernova
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // Bolt
    0, // Flash Bolt
    0, // Blue Bolt
    0, // Ray
    0, // Storm Ray
    0, // Destruct Ray
    0, // Plasma
    0, // Shine Plasma
    0, // Spark Plasma
    0, // Slash
    0, // Wind Slash
    0, // Sonic Slash
    0, // Whirlwind
    0, // Tornado
    0, // Tempest
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // Cure
    0, // Cure Well
    0, // Potent Cure
    0, // Ply
    0, // Ply Well
    0, // Pure Ply
    0, // Wish
    0, // Wish Well
    0, // Pure Wish
    0, // Cure Poison
    0, // Restore
    0, // Revive
    0, // Impact
    0, // High Impact
    0, // Dull
    0, // Blunt
    0, // Guard
    0, // Protect
    0, // Impair
    0, // Debilitate
    0, // Ward
    0, // Resist
    0, // Weaken
    0, // Enfeeble
    0, // Taint
    0, // Poison
    0, // Delude
    0, // Confuse
    0, // Charm
    0, // Paralyze
    0, // Sleep
    0, // Bind
    0, // Haunt
    0, // Curse
    0, // Condemn
    0, // Drain
    0, // Psy Drain
    0, // Break
    0, // Regenerate
    0, // Reflect
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // Move
    0, // Mind Read
    0, // Force
    0, // Lift
    0, // Reveal
    0, // Halt
    0, // Cloak
    0, // Carry
    0, // Catch
    0, // Retreat
    0, // Avoid
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // Dragon Cloud
    0, // Demon Night
    0, // Helm Splitter
    0, // Quick Strike
    STAGING_FULL_SCREEN, // Rockfall
    STAGING_FULL_SCREEN, // Rockslide
    STAGING_FULL_SCREEN, // Avalanche
    0, // Lava Shower
    0, // Molten Bath
    0, // Magma Storm
    0, // Demon Spear
    0, // Angel Spear
    0, // Guardian
    0, // Protector
    0, // Magic Shell
    0, // Magic Shield
    0, // Death Plunge
    0, // Shuriken
    0, // Annihilation
    0, // Punji
    0, // Punji Trap
    0, // Punji Strike
    0, // Fire Bomb
    0, // Cluster Bomb
    0, // Carpet Bomb
    0, // Gale
    0, // Typhoon
    0, // Hurricane
    0, // Thunderclap
    0, // Thunderbolt
    0, // Thunderstorm
    0, // Mist
    0, // Ragnarok
    0, // Cutting Edge
    0, // Heat Wave
    0, // Astral Blast
    0, // Planet Diver
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    STAGING_ATTACK, // Terra Strike
    STAGING_ATTACK, // Poison Cloud
    STAGING_ATTACK, // Deadly Poison
    STAGING_ATTACK, // Mortal Danger
    STAGING_ATTACK, // Bad Omen
    STAGING_ATTACK, // Life Nourish
    STAGING_ATTACK, // Aqua Sock
    STAGING_ENEMY_CAST, // Blizzard
    STAGING_ATTACK, // Frost Bite
    STAGING_ENEMY_CAST, // Drown
    STAGING_ATTACK, // Life Leech
    STAGING_ATTACK, // Psy Leech
    STAGING_ATTACK, // Broil
    STAGING_ATTACK, // Meltdown
    STAGING_ATTACK, // Heat Mirage
    STAGING_ATTACK, // Barrage
    STAGING_ENEMY_CAST, // Demon Fire
    STAGING_ATTACK, // Acid Bath
    STAGING_ENEMY_CAST, // Vorpal Slash
    STAGING_ATTACK, // Stun Voltage
    STAGING_ATTACK, // Blinding Smog
    STAGING_ATTACK, // Murk
    STAGING_ATTACK, // Cyclone Slash
    STAGING_ATTACK, // Psyphon Seal
    STAGING_ENEMY_CAST, // Rapid Smash
    STAGING_ENEMY_CAST, // Sonic Smash
    STAGING_ATTACK, // Asura
    STAGING_ENEMY_CAST, // Titan Blade
    0, // Shining Star
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // Herb
    0, // Nut
    0, // Vial
    0, // Potion
    0, // Soothing Water
    0, // Psy Crystal
    0, // Antidote
    0, // Elixir
    0, // Water of Life
    0, // ?
    0, // Power Bread
    0, // Cookie
    0, // Apple
    0, // Hard Nut
    0, // Mint
    0, // Lucky Pepper
    0, // ?
    0, // Smoke Bomb
    0, // Sleep Bomb
    0, // Adept Ring
    0, // Corn
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    STAGING_ATTACK, // Flint
    STAGING_DJINNI, // Granite
    STAGING_DJINNI, // Quartz
    STAGING_DJINNI, // Vine
    STAGING_ATTACK, // Sap
    STAGING_DJINNI, // Ground
    STAGING_ATTACK, // Bane
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    STAGING_DJINNI, // Fizz
    STAGING_DJINNI, // Sleet
    STAGING_ATTACK, // Mist
    STAGING_DJINNI, // Spritz
    STAGING_DJINNI, // Hail
    STAGING_DJINNI, // Tonic
    STAGING_DJINNI, // Dew
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    STAGING_DJINNI, // Forge
    STAGING_ATTACK, // Fever
    STAGING_DJINNI, // Corona
    STAGING_ATTACK, // Scorch
    STAGING_DJINNI, // Ember
    STAGING_DJINNI, // Flash
    STAGING_DJINNI, // Torch
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    STAGING_DJINNI, // Gust
    STAGING_DJINNI, // Breeze
    STAGING_DJINNI, // Zephyr
    STAGING_ATTACK, // Smog
    STAGING_DJINNI, // Kite
    STAGING_ATTACK, // Squall
    STAGING_DJINNI, // Luff
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    STAGING_CAST, // Venus
    STAGING_SUMMON, // Ramses
    STAGING_SUMMON, // Cybele
    STAGING_SUMMON, // Judgment
    0, // ?
    0, // ?
    STAGING_CAST, // Mercury
    STAGING_SUMMON, // Nereid
    STAGING_SUMMON, // Neptune
    STAGING_SUMMON, // Boreas
    0, // ?
    0, // ?
    STAGING_CAST, // Mars
    STAGING_SUMMON, // Kirin
    STAGING_SUMMON, // Tiamat
    STAGING_SUMMON, // Meteor
    0, // ?
    0, // ?
    STAGING_CAST, // Jupiter
    STAGING_SUMMON, // Atalanta
    STAGING_FULL_SCREEN, // Procne
    STAGING_SUMMON, // Thor
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // Fire Blessing
    0, // Fire Blessing
    0, // Fire Blessing
    0, // Water Blessing
    0, // Water Blessing
    0, // Water Blessing
    0, // Ice Blessing
    0, // Ice Blessing
    0, // Ice Blessing
    0, // Dark Blessing
    0, // Dark Blessing
    0, // Dark Blessing
    0, // Acid Blessing
    0, // Storm Blessing
    0, // Storm Blessing
    0, // Sonic Wave
    0, // Sonic Wave
    0, // Shriek
    0, // Banshee Howl
    0, // Crazy Voice
    0, // War Cry
    0, // Wicked Howl
    0, // Wing Beat
    0, // Wing Flutter
    0, // Wing Stroke
    0, // Evil Blessing
    0, // Deadly Gas
    0, // ?
    0, // ?
    0, // ?
    0, // Rumble
    0, // Bone Chiller
    STAGING_ENEMY_CAST, // Slice
    0, // Bone Charge
    0, // Mystic Flame
    0, // Numbing Sting
    0, // Brute Force
    0, // Sticky Goo
    0, // Cannibal Fang
    0, // Bear Claw
    0, // Poisonous Bite
    0, // Flying Attack
    0, // Undead Sword
    0, // Ransack
    0, // Sticky Poison
    0, // Poison Fang
    0, // Electric Bite
    0, // Poison Tail
    0, // Onslaught
    0, // Vampiric Fang
    0, // Bacteria Rush
    0, // Swift Strike
    0, // Rotten Blood
    STAGING_ATTACK, // Forcible Arm
    0, // Double Fang
    STAGING_ATTACK, // Mortal Blow
    0, // Freebite Rush
    0, // Twin Beaks
    0, // Rabid Fang
    0, // Acid Bite
    STAGING_ATTACK, // Dynamite
    0, // Headbutt
    0, // Poison Ink
    STAGING_ATTACK, // Truncheon Fist
    0, // Counterstrike
    0, // Mad Dash
    0, // Soothing Star
    0, // Spider Web
    0, // Heartrender
    0, // Mad Spatter
    0, // Spasm
    0, // Sleep Star
    0, // Decompose
    0, // Haunting
    0, // Worms
    0, // Berserk
    0, // Lucid Prophecy
    0, // Recovery
    0, // Flee
    0, // Contain
    0, // Threaten
    0, // Tremble
    0, // Fortify
    0, // Speed Surge
    0, // Ally Search
    0, // Sidestep
    0, // Total Defense
    0, // Stand Ready
    0, // Search for Help
    0, // Can't Use
    0, // Poison Beat
    0, // Spinning Beat
    STAGING_ATTACK, // Heat Flash
    STAGING_ATTACK, // Death Size
    0, // Outer Space
    0, // Dragon Driver
    0, // Drain Fang
    STAGING_ATTACK, // Severe Blow
};

/// Up to ABILITY_THRASH.
const u32 gAbilityBattleEffects[] = {
    0, // Nothing
    0, // Attack
    0, // Defend
    BATTLE_EFFECT_QUAKE, // Quake
    BATTLE_EFFECT_QUAKE | (1 << EFFECT_VARIANT_SHIFT), // Earthquake
    BATTLE_EFFECT_QUAKE | (2 << EFFECT_VARIANT_SHIFT), // Quake Sphere
    BATTLE_EFFECT_SPIRE, // Spire
    BATTLE_EFFECT_SPIRE | (1 << EFFECT_VARIANT_SHIFT), // Clay Spire
    BATTLE_EFFECT_SPIRE | (2 << EFFECT_VARIANT_SHIFT), // Stone Spire
    BATTLE_EFFECT_GAIA, // Gaia
    BATTLE_EFFECT_GAIA | (1 << EFFECT_VARIANT_SHIFT), // Mother Gaia
    BATTLE_EFFECT_GAIA | (2 << EFFECT_VARIANT_SHIFT), // Grand Gaia
    BATTLE_EFFECT_GROWTH, // Growth
    BATTLE_EFFECT_GROWTH | (1 << EFFECT_VARIANT_SHIFT), // Mad Growth
    BATTLE_EFFECT_GROWTH | (2 << EFFECT_VARIANT_SHIFT), // Wild Growth
    BATTLE_EFFECT_THORN, // Thorn
    BATTLE_EFFECT_THORN | (1 << EFFECT_VARIANT_SHIFT), // Briar
    BATTLE_EFFECT_THORN | (2 << EFFECT_VARIANT_SHIFT), // Nettle
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    BATTLE_EFFECT_FROST, // Frost
    BATTLE_EFFECT_FROST | (1 << EFFECT_VARIANT_SHIFT), // Tundra
    BATTLE_EFFECT_FROST | (2 << EFFECT_VARIANT_SHIFT), // Glacier
    BATTLE_EFFECT_ICE, // Ice
    BATTLE_EFFECT_ICE | (1 << EFFECT_VARIANT_SHIFT), // Ice Horn
    BATTLE_EFFECT_ICE | (2 << EFFECT_VARIANT_SHIFT), // Ice Missile
    BATTLE_EFFECT_PRISM, // Prism
    BATTLE_EFFECT_PRISM | (1 << EFFECT_VARIANT_SHIFT), // Hail Prism
    BATTLE_EFFECT_PRISM | (2 << EFFECT_VARIANT_SHIFT), // Freeze Prism
    BATTLE_EFFECT_DOUSE, // Douse
    BATTLE_EFFECT_DOUSE | (1 << EFFECT_VARIANT_SHIFT), // Drench
    BATTLE_EFFECT_DOUSE | (2 << EFFECT_VARIANT_SHIFT), // Deluge
    BATTLE_EFFECT_FROTH, // Froth
    BATTLE_EFFECT_FROTH | (1 << EFFECT_VARIANT_SHIFT), // Froth Sphere
    BATTLE_EFFECT_FROTH | (2 << EFFECT_VARIANT_SHIFT), // Froth Spiral
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    BATTLE_EFFECT_FLARE, // Flare
    BATTLE_EFFECT_FLARE | (1 << EFFECT_VARIANT_SHIFT), // Flare Wall
    BATTLE_EFFECT_FLARE | (2 << EFFECT_VARIANT_SHIFT), // Flare Storm
    BATTLE_EFFECT_FIRE, // Fire
    BATTLE_EFFECT_FIRE | (1 << EFFECT_VARIANT_SHIFT), // Fireball
    BATTLE_EFFECT_FIRE | (2 << EFFECT_VARIANT_SHIFT), // Inferno
    BATTLE_EFFECT_VOLCANO, // Volcano
    BATTLE_EFFECT_VOLCANO | (1 << EFFECT_VARIANT_SHIFT), // Eruption
    BATTLE_EFFECT_VOLCANO | (2 << EFFECT_VARIANT_SHIFT), // Pyroclasm
    BATTLE_EFFECT_BLAST, // Blast
    BATTLE_EFFECT_BLAST | (1 << EFFECT_VARIANT_SHIFT), // Mad Blast
    BATTLE_EFFECT_BLAST | (2 << EFFECT_VARIANT_SHIFT), // Fiery Blast
    BATTLE_EFFECT_NOVA, // Blast
    BATTLE_EFFECT_NOVA | (1 << EFFECT_VARIANT_SHIFT), // Nova
    BATTLE_EFFECT_NOVA | (2 << EFFECT_VARIANT_SHIFT), // Supernova
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    BATTLE_EFFECT_BOLT, // Bolt
    BATTLE_EFFECT_BOLT | (1 << EFFECT_VARIANT_SHIFT), // Flash Bolt
    BATTLE_EFFECT_BOLT | (2 << EFFECT_VARIANT_SHIFT), // Blue Bolt
    BATTLE_EFFECT_RAY, // Ray
    BATTLE_EFFECT_RAY | (1 << EFFECT_VARIANT_SHIFT), // Storm Ray
    BATTLE_EFFECT_RAY | (2 << EFFECT_VARIANT_SHIFT), // Destruct Ray
    BATTLE_EFFECT_PLASMA, // Plasma
    BATTLE_EFFECT_PLASMA | (1 << EFFECT_VARIANT_SHIFT), // Shine Plasma
    BATTLE_EFFECT_PLASMA | (2 << EFFECT_VARIANT_SHIFT), // Spark Plasma
    BATTLE_EFFECT_SLASH, // Slash
    BATTLE_EFFECT_SLASH | (1 << EFFECT_VARIANT_SHIFT), // Wind Slash
    BATTLE_EFFECT_SLASH | (2 << EFFECT_VARIANT_SHIFT), // Sonic Slash
    BATTLE_EFFECT_WHIRLWIND, // Whirlwind
    BATTLE_EFFECT_WHIRLWIND | (1 << EFFECT_VARIANT_SHIFT), // Tornado
    BATTLE_EFFECT_WHIRLWIND | (2 << EFFECT_VARIANT_SHIFT), // Tempest
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    BATTLE_EFFECT_CURE, // Cure
    BATTLE_EFFECT_CURE | (1 << EFFECT_VARIANT_SHIFT), // Cure Well
    BATTLE_EFFECT_CURE | (2 << EFFECT_VARIANT_SHIFT), // Potent Cure
    BATTLE_EFFECT_PLY, // Ply
    BATTLE_EFFECT_PLY | (1 << EFFECT_VARIANT_SHIFT), // Ply Well
    BATTLE_EFFECT_PLY | (2 << EFFECT_VARIANT_SHIFT), // Pure Ply
    BATTLE_EFFECT_WISH, // Wish
    BATTLE_EFFECT_WISH | (1 << EFFECT_VARIANT_SHIFT), // Wish Well
    BATTLE_EFFECT_WISH | (2 << EFFECT_VARIANT_SHIFT), // Pure Wish
    BATTLE_EFFECT_CURE_POISON, // Cure Poison
    BATTLE_EFFECT_CURE_POISON | (1 << EFFECT_VARIANT_SHIFT), // Restore
    BATTLE_EFFECT_REVIVE, // Revive
    BATTLE_EFFECT_IMPACT, // Impact
    BATTLE_EFFECT_IMPACT | (1 << EFFECT_VARIANT_SHIFT), // High Impact
    BATTLE_EFFECT_DULL, // Dull
    BATTLE_EFFECT_DULL | (1 << EFFECT_VARIANT_SHIFT), // Blunt
    BATTLE_EFFECT_GUARD, // Guard
    BATTLE_EFFECT_GUARD | (1 << EFFECT_VARIANT_SHIFT), // Protect
    BATTLE_EFFECT_IMPAIR, // Impair
    BATTLE_EFFECT_IMPAIR | (1 << EFFECT_VARIANT_SHIFT), // Debilitate
    BATTLE_EFFECT_WARD, // Ward
    BATTLE_EFFECT_WARD | (1 << EFFECT_VARIANT_SHIFT), // Resist
    BATTLE_EFFECT_WEAKEN, // Weaken
    BATTLE_EFFECT_WEAKEN | (1 << EFFECT_VARIANT_SHIFT), // Enfeeble
    BATTLE_EFFECT_TAINT, // Taint
    BATTLE_EFFECT_TAINT, // Poison
    BATTLE_EFFECT_DELUDE, // Delude
    BATTLE_EFFECT_DELUDE | (1 << EFFECT_VARIANT_SHIFT), // Confuse
    0, // Charm
    BATTLE_EFFECT_PARALYZE, // Paralyze
    BATTLE_EFFECT_SLEEP, // Sleep
    BATTLE_EFFECT_BIND, // Bind
    BATTLE_EFFECT_HAUNT, // Haunt
    BATTLE_EFFECT_CURSE, // Curse
    BATTLE_EFFECT_CONDEMN, // Condemn
    BATTLE_EFFECT_DRAIN, // Drain
    BATTLE_EFFECT_DRAIN | (1 << EFFECT_VARIANT_SHIFT), // Psy Drain
    BATTLE_EFFECT_BREAK, // Break
    BATTLE_EFFECT_REGENERATE, // Regenerate
    BATTLE_EFFECT_REFLECT, // Reflect
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // Move
    0, // Mind Read
    0, // Force
    0, // Lift
    0, // Reveal
    0, // Halt
    0, // Cloak
    0, // Carry
    0, // Catch
    0, // Retreat
    0, // Avoid
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    BATTLE_EFFECT_DRAGON_CLOUD, // Dragon Cloud
    BATTLE_EFFECT_DEMON_NIGHT, // Demon Night
    BATTLE_EFFECT_HELM_SPLITTER, // Helm Splitter
    BATTLE_EFFECT_QUICK_STRIKE, // Quick Strike
    BATTLE_EFFECT_GAIA, // Rockfall
    BATTLE_EFFECT_GAIA | (1 << EFFECT_VARIANT_SHIFT), // Rockslide
    BATTLE_EFFECT_GAIA | (2 << EFFECT_VARIANT_SHIFT), // Avalanche
    BATTLE_EFFECT_VOLCANO, // Lava Shower
    BATTLE_EFFECT_VOLCANO | (1 << EFFECT_VARIANT_SHIFT), // Molten Bath
    BATTLE_EFFECT_VOLCANO | (2 << EFFECT_VARIANT_SHIFT), // Magma Storm
    BATTLE_EFFECT_IMPACT, // Demon Spear
    BATTLE_EFFECT_IMPACT | (1 << EFFECT_VARIANT_SHIFT), // Angel Spear
    BATTLE_EFFECT_GUARD, // Guardian
    BATTLE_EFFECT_GUARD, // Protector
    BATTLE_EFFECT_WARD, // Magic Shell
    BATTLE_EFFECT_WARD | (1 << EFFECT_VARIANT_SHIFT), // Magic Shield
    BATTLE_EFFECT_DEATH_PLUNGE, // Death Plunge
    BATTLE_EFFECT_SHURIKEN, // Shuriken
    BATTLE_EFFECT_ANNIHILATION, // Annihilation
    BATTLE_EFFECT_PUNJI, // Punji
    BATTLE_EFFECT_PUNJI | (1 << EFFECT_VARIANT_SHIFT), // Punji Trap
    BATTLE_EFFECT_PUNJI | (1 << EFFECT_VARIANT_SHIFT), // Punji Strike
    BATTLE_EFFECT_NOVA, // Fire Bomb
    BATTLE_EFFECT_NOVA | (1 << EFFECT_VARIANT_SHIFT), // Cluster Bomb
    BATTLE_EFFECT_NOVA | (2 << EFFECT_VARIANT_SHIFT), // Carpet Bomb
    BATTLE_EFFECT_WHIRLWIND, // Gale
    BATTLE_EFFECT_WHIRLWIND | (1 << EFFECT_VARIANT_SHIFT), // Typhoon
    BATTLE_EFFECT_WHIRLWIND | (2 << EFFECT_VARIANT_SHIFT), // Hurricane
    BATTLE_EFFECT_PLASMA, // Thunderclap
    BATTLE_EFFECT_PLASMA | (1 << EFFECT_VARIANT_SHIFT), // Thunderbolt
    BATTLE_EFFECT_PLASMA | (2 << EFFECT_VARIANT_SHIFT), // Thunderstorm
    BATTLE_EFFECT_DELUDE, // Mist
    BATTLE_EFFECT_RAGNAROK, // Ragnarok
    BATTLE_EFFECT_CUTTING_EDGE, // Cutting Edge
    BATTLE_EFFECT_HEAT_WAVE, // Heat Wave
    BATTLE_EFFECT_ASTRAL_BLAST | (1 << EFFECT_VARIANT_SHIFT) | EFFECT_VIEW_REVERSED, // Astral Blast
    BATTLE_EFFECT_PLANET_DIVER, // Planet Diver
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0x6c | EFFECT_BY_ID_RANGE, // Terra Strike
    0x6d | EFFECT_BY_ID_RANGE, // Poison Cloud
    0x6e | EFFECT_BY_ID_RANGE, // Deadly Poison
    0x6f | EFFECT_BY_ID_RANGE, // Mortal Danger
    0x70 | EFFECT_BY_ID_RANGE, // Bad Omen
    0x71 | EFFECT_BY_ID_RANGE, // Life Nourish
    0x72 | EFFECT_BY_ID_RANGE, // Aqua Sock
    BATTLE_EFFECT_BLIZZARD, // Blizzard
    0x74 | EFFECT_BY_ID_RANGE, // Frost Bite
    BATTLE_EFFECT_DROWN, // Drown
    0x76 | EFFECT_BY_ID_RANGE, // Life Leech
    0x77 | EFFECT_BY_ID_RANGE, // Psy Leech
    0x78 | EFFECT_BY_ID_RANGE, // Broil
    0x79 | EFFECT_BY_ID_RANGE, // Meltdown
    0x7a | EFFECT_BY_ID_RANGE, // Heat Mirage
    0x7b | EFFECT_BY_ID_RANGE, // Barrage
    BATTLE_EFFECT_DEMON_FIRE, // Demon Fire
    0x7d | EFFECT_BY_ID_RANGE, // Acid Bath
    BATTLE_EFFECT_VORPAL_SLASH, // Vorpal Slash
    0x7f | EFFECT_BY_ID_RANGE, // Stun Voltage
    0x80 | EFFECT_BY_ID_RANGE, // Blinding Smog
    0x81 | EFFECT_BY_ID_RANGE, // Murk
    0x82 | EFFECT_BY_ID_RANGE, // Cyclone Slash
    0x83 | EFFECT_BY_ID_RANGE, // Psyphon Seal
    BATTLE_EFFECT_RAPID_SMASH, // Rapid Smash
    BATTLE_EFFECT_RAPID_SMASH, // Sonic Smash
    0x84 | EFFECT_BY_ID_RANGE, // Asura
    BATTLE_EFFECT_TITAN_BLADE, // Titan Blade
    BATTLE_EFFECT_SHINING_STAR, // Shining Star
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    BATTLE_EFFECT_PLY, // Herb
    BATTLE_EFFECT_PLY, // Nut
    BATTLE_EFFECT_PLY, // Vial
    BATTLE_EFFECT_PLY, // Potion
    BATTLE_EFFECT_PLY, // Soothing Water
    BATTLE_EFFECT_PSY_CRYSTAL, // Psy Crystal
    BATTLE_EFFECT_CURE_POISON, // Antidote
    BATTLE_EFFECT_CURE_POISON | (1 << EFFECT_VARIANT_SHIFT), // Elixir
    BATTLE_EFFECT_REVIVE, // Water of Life
    0, // ?
    0, // Power Bread
    0, // Cookie
    0, // Apple
    0, // Hard Nut
    0, // Mint
    0, // Lucky Pepper
    0, // ?
    BATTLE_EFFECT_SMOKE_BOMB, // Smoke Bomb
    BATTLE_EFFECT_SMOKE_BOMB, // Sleep Bomb
    BATTLE_EFFECT_PSY_CRYSTAL, // Adept Ring
    BATTLE_EFFECT_CURE, // Corn
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0x68 | EFFECT_BY_ID_RANGE, // Flint
    BATTLE_EFFECT_GRANITE, // Granite
    BATTLE_EFFECT_QUARTZ, // Quartz
    BATTLE_EFFECT_VINE, // Vine
    0x6b | EFFECT_BY_ID_RANGE, // Sap
    BATTLE_EFFECT_GROUND, // Ground
    0x64 | EFFECT_BY_ID_RANGE, // Bane
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    BATTLE_EFFECT_FIZZ, // Fizz
    BATTLE_EFFECT_SLEET, // Sleet
    0x66 | EFFECT_BY_ID_RANGE, // Mist
    BATTLE_EFFECT_SPRITZ, // Spritz
    BATTLE_EFFECT_HAIL, // Hail
    BATTLE_EFFECT_TONIC, // Tonic
    BATTLE_EFFECT_DEW, // Dew
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    BATTLE_EFFECT_FORGE, // Forge
    0x67 | EFFECT_BY_ID_RANGE, // Fever
    BATTLE_EFFECT_CORONA, // Corona
    0x69 | EFFECT_BY_ID_RANGE, // Scorch
    BATTLE_EFFECT_PSY_CRYSTAL, // Ember
    BATTLE_EFFECT_FLASH, // Flash
    BATTLE_EFFECT_TORCH, // Torch
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    BATTLE_EFFECT_GUST, // Gust
    BATTLE_EFFECT_BREEZE, // Breeze
    BATTLE_EFFECT_ZEPHYR, // Zephyr
    0x65 | EFFECT_BY_ID_RANGE, // Smog
    BATTLE_EFFECT_KITE, // Kite
    0x6a | EFFECT_BY_ID_RANGE, // Squall
    BATTLE_EFFECT_LUFF, // Luff
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    0, // =
    BATTLE_EFFECT_VENUS | EFFECT_VIEW_ACROSS, // Venus
    BATTLE_EFFECT_QUAKE | EFFECT_SUMMON, // Ramses
    BATTLE_EFFECT_THORN | EFFECT_SUMMON, // Cybele
    BATTLE_EFFECT_DOUSE | EFFECT_SUMMON, // Judgment
    0, // ?
    0, // ?
    BATTLE_EFFECT_MERCURY | EFFECT_VIEW_ACROSS, // Mercury
    BATTLE_EFFECT_SPIRE | EFFECT_SUMMON, // Nereid
    BATTLE_EFFECT_FROST | EFFECT_SUMMON, // Neptune
    BATTLE_EFFECT_FROTH | EFFECT_SUMMON, // Boreas
    0, // ?
    0, // ?
    BATTLE_EFFECT_MARS | EFFECT_VIEW_ACROSS, // Mars
    BATTLE_EFFECT_GAIA | EFFECT_SUMMON, // Kirin
    BATTLE_EFFECT_ICE | EFFECT_SUMMON, // Tiamat
    BATTLE_EFFECT_FLARE | EFFECT_SUMMON, // Meteor
    0, // ?
    0, // ?
    BATTLE_EFFECT_JUPITER | EFFECT_VIEW_ACROSS, // Jupiter
    BATTLE_EFFECT_GROWTH | EFFECT_SUMMON, // Atalanta
    BATTLE_EFFECT_PRISM | EFFECT_SUMMON, // Procne
    BATTLE_EFFECT_FIRE | EFFECT_SUMMON, // Thor
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    BATTLE_EFFECT_FIRE_BLESSING | EFFECT_VIEW_REVERSED, // Fire Blessing
    BATTLE_EFFECT_FIRE_BLESSING | EFFECT_VIEW_REVERSED, // Fire Blessing
    BATTLE_EFFECT_FIRE_BLESSING | EFFECT_VIEW_REVERSED, // Fire Blessing
    BATTLE_EFFECT_WATER_BLESSING | EFFECT_VIEW_REVERSED, // Water Blessing
    BATTLE_EFFECT_WATER_BLESSING | EFFECT_VIEW_REVERSED, // Water Blessing
    BATTLE_EFFECT_WATER_BLESSING | EFFECT_VIEW_REVERSED, // Water Blessing
    BATTLE_EFFECT_ICE_BLESSING | EFFECT_VIEW_REVERSED, // Ice Blessing
    BATTLE_EFFECT_ICE_BLESSING | EFFECT_VIEW_REVERSED, // Ice Blessing
    BATTLE_EFFECT_ICE_BLESSING | EFFECT_VIEW_REVERSED, // Ice Blessing
    BATTLE_EFFECT_DARK_BLESSING | EFFECT_VIEW_REVERSED, // Dark Blessing
    BATTLE_EFFECT_DARK_BLESSING | EFFECT_VIEW_REVERSED, // Dark Blessing
    BATTLE_EFFECT_ACID_BLESSING | EFFECT_VIEW_REVERSED, // Dark Blessing
    BATTLE_EFFECT_ACID_BLESSING | EFFECT_VIEW_REVERSED, // Acid Blessing
    BATTLE_EFFECT_STORM_BLESSING | EFFECT_VIEW_REVERSED, // Storm Blessing
    BATTLE_EFFECT_STORM_BLESSING | EFFECT_VIEW_REVERSED, // Storm Blessing
    BATTLE_EFFECT_SONIC_WAVE, // Sonic Wave
    BATTLE_EFFECT_SONIC_WAVE, // Sonic Wave
    BATTLE_EFFECT_SHRIEK, // Shriek
    BATTLE_EFFECT_BANSHEE_HOWL, // Banshee Howl
    BATTLE_EFFECT_CRAZY_VOICE, // Crazy Voice
    BATTLE_EFFECT_WAR_CRY, // War Cry
    BATTLE_EFFECT_WICKED_HOWL, // Wicked Howl
    BATTLE_EFFECT_WING_BEAT, // Wing Beat
    BATTLE_EFFECT_WING_FLUTTER, // Wing Flutter
    BATTLE_EFFECT_WING_STROKE, // Wing Stroke
    BATTLE_EFFECT_EVIL_BLESSING | EFFECT_VIEW_REVERSED, // Evil Blessing
    BATTLE_EFFECT_DEADLY_GAS | EFFECT_VIEW_REVERSED, // Deadly Gas
    0, // ?
    0, // ?
    0, // ?
    BATTLE_EFFECT_RUMBLE, // Rumble
    BATTLE_EFFECT_BONE_CHILLER, // Bone Chiller
    BATTLE_EFFECT_RAPID_SMASH, // Slice
    BATTLE_EFFECT_BONE_CHARGE, // Bone Charge
    BATTLE_EFFECT_MYSTIC_FLAME, // Mystic Flame
    BATTLE_EFFECT_NUMBING_STING, // Numbing Sting
    BATTLE_EFFECT_BRUTE_FORCE, // Brute Force
    BATTLE_EFFECT_STICKY_GOO, // Sticky Goo
    BATTLE_EFFECT_CANNIBAL_FANG, // Cannibal Fang
    BATTLE_EFFECT_BEAR_CLAW, // Bear Claw
    BATTLE_EFFECT_POISONOUS_BITE, // Poisonous Bite
    BATTLE_EFFECT_FLYING_ATTACK, // Flying Attack
    BATTLE_EFFECT_UNDEAD_SWORD, // Undead Sword
    BATTLE_EFFECT_RANSACK, // Ransack
    BATTLE_EFFECT_STICKY_POISON, // Sticky Poison
    BATTLE_EFFECT_POISON_FANG, // Poison Fang
    BATTLE_EFFECT_ELECTRIC_BITE, // Electric Bite
    BATTLE_EFFECT_POISON_TAIL, // Poison Tail
    BATTLE_EFFECT_ONSLAUGHT, // Onslaught
    BATTLE_EFFECT_VAMPIRIC_FANG, // Vampiric Fang
    BATTLE_EFFECT_BACTERIA_RUSH, // Bacteria Rush
    BATTLE_EFFECT_SWIFT_STRIKE, // Swift Strike
    BATTLE_EFFECT_ROTTEN_BLOOD, // Rotten Blood
    0x85 | EFFECT_BY_ID_RANGE, // Forcible Arm
    BATTLE_EFFECT_DOUBLE_FANG, // Double Fang
    BATTLE_EFFECT_MORTAL_BLOW, // Mortal Blow
    BATTLE_EFFECT_FREEBITE_RUSH, // Freebite Rush
    BATTLE_EFFECT_TWIN_BEAKS, // Twin Beaks
    BATTLE_EFFECT_RABID_FANG, // Rabid Fang
    BATTLE_EFFECT_ACID_BITE, // Acid Bite
    BATTLE_EFFECT_DYNAMITE, // Dynamite
    BATTLE_EFFECT_HEADBUTT, // Headbutt
    BATTLE_EFFECT_POISON_INK, // Poison Ink
    0x86 | EFFECT_BY_ID_RANGE, // Truncheon Fist
    BATTLE_EFFECT_COUNTERSTRIKE, // Counterstrike
    BATTLE_EFFECT_MAD_DASH, // Mad Dash
    BATTLE_EFFECT_SOOTHING_STAR, // Soothing Star
    BATTLE_EFFECT_SPIDER_WEB, // Spider Web
    BATTLE_EFFECT_HEARTRENDER, // Heartrender
    BATTLE_EFFECT_MAD_SPATTER, // Mad Spatter
    BATTLE_EFFECT_SPASM, // Spasm
    BATTLE_EFFECT_SLEEP_STAR, // Sleep Star
    BATTLE_EFFECT_DECOMPOSE, // Decompose
    BATTLE_EFFECT_HAUNTING, // Haunting
    BATTLE_EFFECT_WORMS, // Worms
    BATTLE_EFFECT_BERSERK, // Berserk
    BATTLE_EFFECT_LUCID_PROPHECY, // Lucid Prophecy
    BATTLE_EFFECT_RECOVERY, // Recovery
    0, // Flee
    0, // Contain
    0, // Threaten
    0, // Tremble
    0, // Fortify
    0, // Speed Surge
    0, // Ally Search
    0, // Sidestep
    0, // Total Defense
    0, // Stand Ready
    0, // Search for Help
    0, // Can't Use
    BATTLE_EFFECT_POISON_BEAT, // Poison Beat
    BATTLE_EFFECT_SPINNING_BEAT, // Spinning Beat
    0x7a | EFFECT_BY_ID_RANGE, // Heat Flash
    0x6f | EFFECT_BY_ID_RANGE, // Death Size
    BATTLE_EFFECT_OUTER_SPACE | EFFECT_VIEW_REVERSED, // Outer Space
    BATTLE_EFFECT_DRAGON_DRIVER | EFFECT_VIEW_REVERSED, // Dragon Driver
    BATTLE_EFFECT_DRAIN_FANG, // Drain Fang
    EFFECT_ID_SEVERE_BLOW | EFFECT_BY_ID_RANGE, // Severe Blow
    BATTLE_EFFECT_THRASH, // Thrash
};
// clang-format on
