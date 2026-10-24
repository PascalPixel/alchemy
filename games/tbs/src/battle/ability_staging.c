/// @file
/// How each ability is staged in battle: the outcome staging BattleTurn_Build
/// gives it and the animation it plays, by ability id. The ability names in
/// the comments are the English release's.

#include "battle_command.h"
#include "unit/battle_anim/battle_anim.h"

// clang-format off
/// Up to ABILITY_SEVERE_BLOW; 0 leaves the choice to BattleTurn_Build.
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
const u32 gAbilityAnimations[] = {
    0, // Nothing
    0, // Attack
    0, // Defend
    BATTLE_ANIM_QUAKE, // Quake
    BATTLE_ANIM_QUAKE | (1 << ANIM_VARIANT_SHIFT), // Earthquake
    BATTLE_ANIM_QUAKE | (2 << ANIM_VARIANT_SHIFT), // Quake Sphere
    BATTLE_ANIM_SPIRE, // Spire
    BATTLE_ANIM_SPIRE | (1 << ANIM_VARIANT_SHIFT), // Clay Spire
    BATTLE_ANIM_SPIRE | (2 << ANIM_VARIANT_SHIFT), // Stone Spire
    BATTLE_ANIM_GAIA, // Gaia
    BATTLE_ANIM_GAIA | (1 << ANIM_VARIANT_SHIFT), // Mother Gaia
    BATTLE_ANIM_GAIA | (2 << ANIM_VARIANT_SHIFT), // Grand Gaia
    BATTLE_ANIM_GROWTH, // Growth
    BATTLE_ANIM_GROWTH | (1 << ANIM_VARIANT_SHIFT), // Mad Growth
    BATTLE_ANIM_GROWTH | (2 << ANIM_VARIANT_SHIFT), // Wild Growth
    BATTLE_ANIM_THORN, // Thorn
    BATTLE_ANIM_THORN | (1 << ANIM_VARIANT_SHIFT), // Briar
    BATTLE_ANIM_THORN | (2 << ANIM_VARIANT_SHIFT), // Nettle
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    BATTLE_ANIM_FROST, // Frost
    BATTLE_ANIM_FROST | (1 << ANIM_VARIANT_SHIFT), // Tundra
    BATTLE_ANIM_FROST | (2 << ANIM_VARIANT_SHIFT), // Glacier
    BATTLE_ANIM_ICE, // Ice
    BATTLE_ANIM_ICE | (1 << ANIM_VARIANT_SHIFT), // Ice Horn
    BATTLE_ANIM_ICE | (2 << ANIM_VARIANT_SHIFT), // Ice Missile
    BATTLE_ANIM_PRISM, // Prism
    BATTLE_ANIM_PRISM | (1 << ANIM_VARIANT_SHIFT), // Hail Prism
    BATTLE_ANIM_PRISM | (2 << ANIM_VARIANT_SHIFT), // Freeze Prism
    BATTLE_ANIM_DOUSE, // Douse
    BATTLE_ANIM_DOUSE | (1 << ANIM_VARIANT_SHIFT), // Drench
    BATTLE_ANIM_DOUSE | (2 << ANIM_VARIANT_SHIFT), // Deluge
    BATTLE_ANIM_FROTH, // Froth
    BATTLE_ANIM_FROTH | (1 << ANIM_VARIANT_SHIFT), // Froth Sphere
    BATTLE_ANIM_FROTH | (2 << ANIM_VARIANT_SHIFT), // Froth Spiral
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    BATTLE_ANIM_FLARE, // Flare
    BATTLE_ANIM_FLARE | (1 << ANIM_VARIANT_SHIFT), // Flare Wall
    BATTLE_ANIM_FLARE | (2 << ANIM_VARIANT_SHIFT), // Flare Storm
    BATTLE_ANIM_FIRE, // Fire
    BATTLE_ANIM_FIRE | (1 << ANIM_VARIANT_SHIFT), // Fireball
    BATTLE_ANIM_FIRE | (2 << ANIM_VARIANT_SHIFT), // Inferno
    BATTLE_ANIM_VOLCANO, // Volcano
    BATTLE_ANIM_VOLCANO | (1 << ANIM_VARIANT_SHIFT), // Eruption
    BATTLE_ANIM_VOLCANO | (2 << ANIM_VARIANT_SHIFT), // Pyroclasm
    BATTLE_ANIM_BLAST, // Blast
    BATTLE_ANIM_BLAST | (1 << ANIM_VARIANT_SHIFT), // Mad Blast
    BATTLE_ANIM_BLAST | (2 << ANIM_VARIANT_SHIFT), // Fiery Blast
    BATTLE_ANIM_NOVA, // Blast
    BATTLE_ANIM_NOVA | (1 << ANIM_VARIANT_SHIFT), // Nova
    BATTLE_ANIM_NOVA | (2 << ANIM_VARIANT_SHIFT), // Supernova
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    BATTLE_ANIM_BOLT, // Bolt
    BATTLE_ANIM_BOLT | (1 << ANIM_VARIANT_SHIFT), // Flash Bolt
    BATTLE_ANIM_BOLT | (2 << ANIM_VARIANT_SHIFT), // Blue Bolt
    BATTLE_ANIM_RAY, // Ray
    BATTLE_ANIM_RAY | (1 << ANIM_VARIANT_SHIFT), // Storm Ray
    BATTLE_ANIM_RAY | (2 << ANIM_VARIANT_SHIFT), // Destruct Ray
    BATTLE_ANIM_PLASMA, // Plasma
    BATTLE_ANIM_PLASMA | (1 << ANIM_VARIANT_SHIFT), // Shine Plasma
    BATTLE_ANIM_PLASMA | (2 << ANIM_VARIANT_SHIFT), // Spark Plasma
    BATTLE_ANIM_SLASH, // Slash
    BATTLE_ANIM_SLASH | (1 << ANIM_VARIANT_SHIFT), // Wind Slash
    BATTLE_ANIM_SLASH | (2 << ANIM_VARIANT_SHIFT), // Sonic Slash
    BATTLE_ANIM_WHIRLWIND, // Whirlwind
    BATTLE_ANIM_WHIRLWIND | (1 << ANIM_VARIANT_SHIFT), // Tornado
    BATTLE_ANIM_WHIRLWIND | (2 << ANIM_VARIANT_SHIFT), // Tempest
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    BATTLE_ANIM_CURE, // Cure
    BATTLE_ANIM_CURE | (1 << ANIM_VARIANT_SHIFT), // Cure Well
    BATTLE_ANIM_CURE | (2 << ANIM_VARIANT_SHIFT), // Potent Cure
    BATTLE_ANIM_PLY, // Ply
    BATTLE_ANIM_PLY | (1 << ANIM_VARIANT_SHIFT), // Ply Well
    BATTLE_ANIM_PLY | (2 << ANIM_VARIANT_SHIFT), // Pure Ply
    BATTLE_ANIM_WISH, // Wish
    BATTLE_ANIM_WISH | (1 << ANIM_VARIANT_SHIFT), // Wish Well
    BATTLE_ANIM_WISH | (2 << ANIM_VARIANT_SHIFT), // Pure Wish
    BATTLE_ANIM_CURE_POISON, // Cure Poison
    BATTLE_ANIM_CURE_POISON | (1 << ANIM_VARIANT_SHIFT), // Restore
    BATTLE_ANIM_REVIVE, // Revive
    BATTLE_ANIM_IMPACT, // Impact
    BATTLE_ANIM_IMPACT | (1 << ANIM_VARIANT_SHIFT), // High Impact
    BATTLE_ANIM_DULL, // Dull
    BATTLE_ANIM_DULL | (1 << ANIM_VARIANT_SHIFT), // Blunt
    BATTLE_ANIM_GUARD, // Guard
    BATTLE_ANIM_GUARD | (1 << ANIM_VARIANT_SHIFT), // Protect
    BATTLE_ANIM_IMPAIR, // Impair
    BATTLE_ANIM_IMPAIR | (1 << ANIM_VARIANT_SHIFT), // Debilitate
    BATTLE_ANIM_WARD, // Ward
    BATTLE_ANIM_WARD | (1 << ANIM_VARIANT_SHIFT), // Resist
    BATTLE_ANIM_WEAKEN, // Weaken
    BATTLE_ANIM_WEAKEN | (1 << ANIM_VARIANT_SHIFT), // Enfeeble
    BATTLE_ANIM_TAINT, // Taint
    BATTLE_ANIM_TAINT, // Poison
    BATTLE_ANIM_DELUDE, // Delude
    BATTLE_ANIM_DELUDE | (1 << ANIM_VARIANT_SHIFT), // Confuse
    0, // Charm
    BATTLE_ANIM_PARALYZE, // Paralyze
    BATTLE_ANIM_SLEEP, // Sleep
    BATTLE_ANIM_BIND, // Bind
    BATTLE_ANIM_HAUNT, // Haunt
    BATTLE_ANIM_CURSE, // Curse
    BATTLE_ANIM_CONDEMN, // Condemn
    BATTLE_ANIM_DRAIN, // Drain
    BATTLE_ANIM_DRAIN | (1 << ANIM_VARIANT_SHIFT), // Psy Drain
    BATTLE_ANIM_BREAK, // Break
    BATTLE_ANIM_REGENERATE, // Regenerate
    BATTLE_ANIM_REFLECT, // Reflect
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
    BATTLE_ANIM_DRAGON_CLOUD, // Dragon Cloud
    BATTLE_ANIM_DEMON_NIGHT, // Demon Night
    BATTLE_ANIM_HELM_SPLITTER, // Helm Splitter
    BATTLE_ANIM_QUICK_STRIKE, // Quick Strike
    BATTLE_ANIM_GAIA, // Rockfall
    BATTLE_ANIM_GAIA | (1 << ANIM_VARIANT_SHIFT), // Rockslide
    BATTLE_ANIM_GAIA | (2 << ANIM_VARIANT_SHIFT), // Avalanche
    BATTLE_ANIM_VOLCANO, // Lava Shower
    BATTLE_ANIM_VOLCANO | (1 << ANIM_VARIANT_SHIFT), // Molten Bath
    BATTLE_ANIM_VOLCANO | (2 << ANIM_VARIANT_SHIFT), // Magma Storm
    BATTLE_ANIM_IMPACT, // Demon Spear
    BATTLE_ANIM_IMPACT | (1 << ANIM_VARIANT_SHIFT), // Angel Spear
    BATTLE_ANIM_GUARD, // Guardian
    BATTLE_ANIM_GUARD, // Protector
    BATTLE_ANIM_WARD, // Magic Shell
    BATTLE_ANIM_WARD | (1 << ANIM_VARIANT_SHIFT), // Magic Shield
    BATTLE_ANIM_DEATH_PLUNGE, // Death Plunge
    BATTLE_ANIM_SHURIKEN, // Shuriken
    BATTLE_ANIM_ANNIHILATION, // Annihilation
    BATTLE_ANIM_PUNJI, // Punji
    BATTLE_ANIM_PUNJI | (1 << ANIM_VARIANT_SHIFT), // Punji Trap
    BATTLE_ANIM_PUNJI | (1 << ANIM_VARIANT_SHIFT), // Punji Strike
    BATTLE_ANIM_NOVA, // Fire Bomb
    BATTLE_ANIM_NOVA | (1 << ANIM_VARIANT_SHIFT), // Cluster Bomb
    BATTLE_ANIM_NOVA | (2 << ANIM_VARIANT_SHIFT), // Carpet Bomb
    BATTLE_ANIM_WHIRLWIND, // Gale
    BATTLE_ANIM_WHIRLWIND | (1 << ANIM_VARIANT_SHIFT), // Typhoon
    BATTLE_ANIM_WHIRLWIND | (2 << ANIM_VARIANT_SHIFT), // Hurricane
    BATTLE_ANIM_PLASMA, // Thunderclap
    BATTLE_ANIM_PLASMA | (1 << ANIM_VARIANT_SHIFT), // Thunderbolt
    BATTLE_ANIM_PLASMA | (2 << ANIM_VARIANT_SHIFT), // Thunderstorm
    BATTLE_ANIM_DELUDE, // Mist
    BATTLE_ANIM_RAGNAROK, // Ragnarok
    BATTLE_ANIM_CUTTING_EDGE, // Cutting Edge
    BATTLE_ANIM_HEAT_WAVE, // Heat Wave
    BATTLE_ANIM_ASTRAL_BLAST | (1 << ANIM_VARIANT_SHIFT) | ANIM_VIEW_REVERSED, // Astral Blast
    BATTLE_ANIM_PLANET_DIVER, // Planet Diver
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
    0x6c | ANIM_BY_ID_RANGE, // Terra Strike
    0x6d | ANIM_BY_ID_RANGE, // Poison Cloud
    0x6e | ANIM_BY_ID_RANGE, // Deadly Poison
    0x6f | ANIM_BY_ID_RANGE, // Mortal Danger
    0x70 | ANIM_BY_ID_RANGE, // Bad Omen
    0x71 | ANIM_BY_ID_RANGE, // Life Nourish
    0x72 | ANIM_BY_ID_RANGE, // Aqua Sock
    BATTLE_ANIM_BLIZZARD, // Blizzard
    0x74 | ANIM_BY_ID_RANGE, // Frost Bite
    BATTLE_ANIM_DROWN, // Drown
    0x76 | ANIM_BY_ID_RANGE, // Life Leech
    0x77 | ANIM_BY_ID_RANGE, // Psy Leech
    0x78 | ANIM_BY_ID_RANGE, // Broil
    0x79 | ANIM_BY_ID_RANGE, // Meltdown
    0x7a | ANIM_BY_ID_RANGE, // Heat Mirage
    0x7b | ANIM_BY_ID_RANGE, // Barrage
    BATTLE_ANIM_DEMON_FIRE, // Demon Fire
    0x7d | ANIM_BY_ID_RANGE, // Acid Bath
    BATTLE_ANIM_VORPAL_SLASH, // Vorpal Slash
    0x7f | ANIM_BY_ID_RANGE, // Stun Voltage
    0x80 | ANIM_BY_ID_RANGE, // Blinding Smog
    0x81 | ANIM_BY_ID_RANGE, // Murk
    0x82 | ANIM_BY_ID_RANGE, // Cyclone Slash
    0x83 | ANIM_BY_ID_RANGE, // Psyphon Seal
    BATTLE_ANIM_RAPID_SMASH, // Rapid Smash
    BATTLE_ANIM_RAPID_SMASH, // Sonic Smash
    0x84 | ANIM_BY_ID_RANGE, // Asura
    BATTLE_ANIM_TITAN_BLADE, // Titan Blade
    BATTLE_ANIM_SHINING_STAR, // Shining Star
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
    BATTLE_ANIM_PLY, // Herb
    BATTLE_ANIM_PLY, // Nut
    BATTLE_ANIM_PLY, // Vial
    BATTLE_ANIM_PLY, // Potion
    BATTLE_ANIM_PLY, // Soothing Water
    BATTLE_ANIM_PSY_CRYSTAL, // Psy Crystal
    BATTLE_ANIM_CURE_POISON, // Antidote
    BATTLE_ANIM_CURE_POISON | (1 << ANIM_VARIANT_SHIFT), // Elixir
    BATTLE_ANIM_REVIVE, // Water of Life
    0, // ?
    0, // Power Bread
    0, // Cookie
    0, // Apple
    0, // Hard Nut
    0, // Mint
    0, // Lucky Pepper
    0, // ?
    BATTLE_ANIM_SMOKE_BOMB, // Smoke Bomb
    BATTLE_ANIM_SMOKE_BOMB, // Sleep Bomb
    BATTLE_ANIM_PSY_CRYSTAL, // Adept Ring
    BATTLE_ANIM_CURE, // Corn
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
    0x68 | ANIM_BY_ID_RANGE, // Flint
    BATTLE_ANIM_GRANITE, // Granite
    BATTLE_ANIM_QUARTZ, // Quartz
    BATTLE_ANIM_VINE, // Vine
    0x6b | ANIM_BY_ID_RANGE, // Sap
    BATTLE_ANIM_GROUND, // Ground
    0x64 | ANIM_BY_ID_RANGE, // Bane
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
    BATTLE_ANIM_FIZZ, // Fizz
    BATTLE_ANIM_SLEET, // Sleet
    0x66 | ANIM_BY_ID_RANGE, // Mist
    BATTLE_ANIM_SPRITZ, // Spritz
    BATTLE_ANIM_HAIL, // Hail
    BATTLE_ANIM_TONIC, // Tonic
    BATTLE_ANIM_DEW, // Dew
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
    BATTLE_ANIM_FORGE, // Forge
    0x67 | ANIM_BY_ID_RANGE, // Fever
    BATTLE_ANIM_CORONA, // Corona
    0x69 | ANIM_BY_ID_RANGE, // Scorch
    BATTLE_ANIM_PSY_CRYSTAL, // Ember
    BATTLE_ANIM_FLASH, // Flash
    BATTLE_ANIM_TORCH, // Torch
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
    BATTLE_ANIM_GUST, // Gust
    BATTLE_ANIM_BREEZE, // Breeze
    BATTLE_ANIM_ZEPHYR, // Zephyr
    0x65 | ANIM_BY_ID_RANGE, // Smog
    BATTLE_ANIM_KITE, // Kite
    0x6a | ANIM_BY_ID_RANGE, // Squall
    BATTLE_ANIM_LUFF, // Luff
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
    BATTLE_ANIM_VENUS | ANIM_VIEW_ACROSS, // Venus
    BATTLE_ANIM_QUAKE | ANIM_SUMMON, // Ramses
    BATTLE_ANIM_THORN | ANIM_SUMMON, // Cybele
    BATTLE_ANIM_DOUSE | ANIM_SUMMON, // Judgment
    0, // ?
    0, // ?
    BATTLE_ANIM_MERCURY | ANIM_VIEW_ACROSS, // Mercury
    BATTLE_ANIM_SPIRE | ANIM_SUMMON, // Nereid
    BATTLE_ANIM_FROST | ANIM_SUMMON, // Neptune
    BATTLE_ANIM_FROTH | ANIM_SUMMON, // Boreas
    0, // ?
    0, // ?
    BATTLE_ANIM_MARS | ANIM_VIEW_ACROSS, // Mars
    BATTLE_ANIM_GAIA | ANIM_SUMMON, // Kirin
    BATTLE_ANIM_ICE | ANIM_SUMMON, // Tiamat
    BATTLE_ANIM_FLARE | ANIM_SUMMON, // Meteor
    0, // ?
    0, // ?
    BATTLE_ANIM_JUPITER | ANIM_VIEW_ACROSS, // Jupiter
    BATTLE_ANIM_GROWTH | ANIM_SUMMON, // Atalanta
    BATTLE_ANIM_PRISM | ANIM_SUMMON, // Procne
    BATTLE_ANIM_FIRE | ANIM_SUMMON, // Thor
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
    BATTLE_ANIM_FIRE_BLESSING | ANIM_VIEW_REVERSED, // Fire Blessing
    BATTLE_ANIM_FIRE_BLESSING | ANIM_VIEW_REVERSED, // Fire Blessing
    BATTLE_ANIM_FIRE_BLESSING | ANIM_VIEW_REVERSED, // Fire Blessing
    BATTLE_ANIM_WATER_BLESSING | ANIM_VIEW_REVERSED, // Water Blessing
    BATTLE_ANIM_WATER_BLESSING | ANIM_VIEW_REVERSED, // Water Blessing
    BATTLE_ANIM_WATER_BLESSING | ANIM_VIEW_REVERSED, // Water Blessing
    BATTLE_ANIM_ICE_BLESSING | ANIM_VIEW_REVERSED, // Ice Blessing
    BATTLE_ANIM_ICE_BLESSING | ANIM_VIEW_REVERSED, // Ice Blessing
    BATTLE_ANIM_ICE_BLESSING | ANIM_VIEW_REVERSED, // Ice Blessing
    BATTLE_ANIM_DARK_BLESSING | ANIM_VIEW_REVERSED, // Dark Blessing
    BATTLE_ANIM_DARK_BLESSING | ANIM_VIEW_REVERSED, // Dark Blessing
    BATTLE_ANIM_ACID_BLESSING | ANIM_VIEW_REVERSED, // Dark Blessing
    BATTLE_ANIM_ACID_BLESSING | ANIM_VIEW_REVERSED, // Acid Blessing
    BATTLE_ANIM_STORM_BLESSING | ANIM_VIEW_REVERSED, // Storm Blessing
    BATTLE_ANIM_STORM_BLESSING | ANIM_VIEW_REVERSED, // Storm Blessing
    BATTLE_ANIM_SONIC_WAVE, // Sonic Wave
    BATTLE_ANIM_SONIC_WAVE, // Sonic Wave
    BATTLE_ANIM_SHRIEK, // Shriek
    BATTLE_ANIM_BANSHEE_HOWL, // Banshee Howl
    BATTLE_ANIM_CRAZY_VOICE, // Crazy Voice
    BATTLE_ANIM_WAR_CRY, // War Cry
    BATTLE_ANIM_WICKED_HOWL, // Wicked Howl
    BATTLE_ANIM_WING_BEAT, // Wing Beat
    BATTLE_ANIM_WING_FLUTTER, // Wing Flutter
    BATTLE_ANIM_WING_STROKE, // Wing Stroke
    BATTLE_ANIM_EVIL_BLESSING | ANIM_VIEW_REVERSED, // Evil Blessing
    BATTLE_ANIM_DEADLY_GAS | ANIM_VIEW_REVERSED, // Deadly Gas
    0, // ?
    0, // ?
    0, // ?
    BATTLE_ANIM_RUMBLE, // Rumble
    BATTLE_ANIM_BONE_CHILLER, // Bone Chiller
    BATTLE_ANIM_RAPID_SMASH, // Slice
    BATTLE_ANIM_BONE_CHARGE, // Bone Charge
    BATTLE_ANIM_MYSTIC_FLAME, // Mystic Flame
    BATTLE_ANIM_NUMBING_STING, // Numbing Sting
    BATTLE_ANIM_BRUTE_FORCE, // Brute Force
    BATTLE_ANIM_STICKY_GOO, // Sticky Goo
    BATTLE_ANIM_CANNIBAL_FANG, // Cannibal Fang
    BATTLE_ANIM_BEAR_CLAW, // Bear Claw
    BATTLE_ANIM_POISONOUS_BITE, // Poisonous Bite
    BATTLE_ANIM_FLYING_ATTACK, // Flying Attack
    BATTLE_ANIM_UNDEAD_SWORD, // Undead Sword
    BATTLE_ANIM_RANSACK, // Ransack
    BATTLE_ANIM_STICKY_POISON, // Sticky Poison
    BATTLE_ANIM_POISON_FANG, // Poison Fang
    BATTLE_ANIM_ELECTRIC_BITE, // Electric Bite
    BATTLE_ANIM_POISON_TAIL, // Poison Tail
    BATTLE_ANIM_ONSLAUGHT, // Onslaught
    BATTLE_ANIM_VAMPIRIC_FANG, // Vampiric Fang
    BATTLE_ANIM_BACTERIA_RUSH, // Bacteria Rush
    BATTLE_ANIM_SWIFT_STRIKE, // Swift Strike
    BATTLE_ANIM_ROTTEN_BLOOD, // Rotten Blood
    0x85 | ANIM_BY_ID_RANGE, // Forcible Arm
    BATTLE_ANIM_DOUBLE_FANG, // Double Fang
    BATTLE_ANIM_MORTAL_BLOW, // Mortal Blow
    BATTLE_ANIM_FREEBITE_RUSH, // Freebite Rush
    BATTLE_ANIM_TWIN_BEAKS, // Twin Beaks
    BATTLE_ANIM_RABID_FANG, // Rabid Fang
    BATTLE_ANIM_ACID_BITE, // Acid Bite
    BATTLE_ANIM_DYNAMITE, // Dynamite
    BATTLE_ANIM_HEADBUTT, // Headbutt
    BATTLE_ANIM_POISON_INK, // Poison Ink
    0x86 | ANIM_BY_ID_RANGE, // Truncheon Fist
    BATTLE_ANIM_COUNTERSTRIKE, // Counterstrike
    BATTLE_ANIM_MAD_DASH, // Mad Dash
    BATTLE_ANIM_SOOTHING_STAR, // Soothing Star
    BATTLE_ANIM_SPIDER_WEB, // Spider Web
    BATTLE_ANIM_HEARTRENDER, // Heartrender
    BATTLE_ANIM_MAD_SPATTER, // Mad Spatter
    BATTLE_ANIM_SPASM, // Spasm
    BATTLE_ANIM_SLEEP_STAR, // Sleep Star
    BATTLE_ANIM_DECOMPOSE, // Decompose
    BATTLE_ANIM_HAUNTING, // Haunting
    BATTLE_ANIM_WORMS, // Worms
    BATTLE_ANIM_BERSERK, // Berserk
    BATTLE_ANIM_LUCID_PROPHECY, // Lucid Prophecy
    BATTLE_ANIM_RECOVERY, // Recovery
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
    BATTLE_ANIM_POISON_BEAT, // Poison Beat
    BATTLE_ANIM_SPINNING_BEAT, // Spinning Beat
    0x7a | ANIM_BY_ID_RANGE, // Heat Flash
    0x6f | ANIM_BY_ID_RANGE, // Death Size
    BATTLE_ANIM_OUTER_SPACE | ANIM_VIEW_REVERSED, // Outer Space
    BATTLE_ANIM_DRAGON_DRIVER | ANIM_VIEW_REVERSED, // Dragon Driver
    BATTLE_ANIM_DRAIN_FANG, // Drain Fang
    ANIM_ID_SEVERE_BLOW | ANIM_BY_ID_RANGE, // Severe Blow
    BATTLE_ANIM_THRASH, // Thrash
};
// clang-format on
