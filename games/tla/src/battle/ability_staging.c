/// @file
/// How each ability is staged in battle: the outcome staging
/// BattleActionRun_Build gives it and the effect it plays, by ability id, and
/// the effect chance it rolls ahead. The ability names in the comments are the
/// English release's; `?` is the name MsgAbilityName gives those ids. An
/// effect's kind, its low twelve bits, stays a number.

#include "battle/ability_staging.h"

const u8 gActionRun_EffectChance[] = {100, 60, 30, 30, 30, 30};

// clang-format off
/// By ability id, ABILITY_NOTHING to ABILITY_PRESSURE; 0 leaves the choice to
/// BattleActionRun_Build.
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
    0, // Cool
    0, // Supercool
    0, // Megacool
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
    0, // Fume
    0, // Serpent Fume
    0, // Dragon Fume
    0, // Beam
    0, // Cycle Beam
    0, // Searing Beam
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
    0, // Aura
    0, // Healing Aura
    0, // Cool Aura
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
    0, // Lash
    0, // Pound
    0, // Tremor
    0, // Scoop
    0, // Cyclone
    0, // Parch
    0, // Sand
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
    0, // Burst
    0, // Grind
    0, // Hover
    0, // Blaze
    0, // Ma????
    0, // Teleport
    0, // A??
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
    0, // Thunderhead
    0, // Mist
    0, // Ragnarok
    0, // Cutting Edge
    0, // Heat Wave
    0, // Astral Blast
    0, // Planet Diver
    0, // Diamond Dust
    0, // Odyssey
    0, // Liquifier
    0, // Plume Edge
    0, // Thunder Mine
    0, // Planetary
    0, // Diamond Berg
    0, // Death Leap
    0, // Epicenter
    0, // Thorny Grave
    0, // Skull Splitter
    0, // ?
    0, // ?
    STAGING_ATTACK, // Terra Strike
    STAGING_ATTACK, // Poison Cloud
    STAGING_ATTACK, // Poison Death
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
    STAGING_ENEMY_CAST, // Demonfire
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
    0, // {article_class:3}Herb
    0, // {article_class:2}Nut
    0, // {article_class:2}Vial
    0, // {article_class:2}Potion
    0, // {article_class:5}Soothing Water
    0, // {article_class:2}Psy Crystal
    0, // {article_class:2}Antidote
    0, // {article_class:3}Elixir
    0, // {article_class:4}Water of Life
    0, // {article_class:2}Mist Potion
    0, // {article_class:4}Power Bread
    0, // {article_class:2}Cookie
    0, // {article_class:3}Apple
    0, // {article_class:2}Hard Nut
    0, // {article_class:2}Mint
    0, // {article_class:4}Lucky Pepper
    0, // ?
    0, // {article_class:2}Smoke Bomb
    0, // {article_class:2}Sleep Bomb
    0, // {article_class:3}Adept Ring
    0, // {article_class:4}Corn
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
    0, // Aurora Field
    STAGING_DJINNI, // Djinn Counter
    STAGING_ATTACK, // Flint
    STAGING_DJINNI, // Granite
    STAGING_DJINNI, // Quartz
    STAGING_DJINNI, // Vine
    STAGING_ATTACK, // Sap
    STAGING_DJINNI, // Ground
    STAGING_ATTACK, // Bane
    STAGING_ATTACK, // Echo
    STAGING_DJINNI, // Iron
    STAGING_ATTACK, // Steel
    STAGING_DJINNI, // Mud
    STAGING_DJINNI, // Flower
    STAGING_TEAM_STRIKE, // Meld
    STAGING_DJINNI, // Petra
    STAGING_DJINNI, // Salt
    STAGING_DJINNI, // Geode
    STAGING_TEAM_STRIKE, // Mold
    STAGING_DJINNI, // Crystal
    0, // Earth18
    0, // Earth19
    STAGING_DJINNI, // Fizz
    STAGING_DJINNI, // Sleet
    STAGING_ATTACK, // Mist
    STAGING_DJINNI, // Spritz
    STAGING_DJINNI, // Hail
    STAGING_DJINNI, // Tonic
    STAGING_DJINNI, // Dew
    STAGING_ATTACK, // Fog
    STAGING_ATTACK, // Sour
    STAGING_DJINNI, // Spring
    STAGING_DJINNI, // Shade
    STAGING_ATTACK, // Chill
    STAGING_DJINNI, // Steam
    STAGING_DJINNI, // Rime
    STAGING_ATTACK, // Gel
    STAGING_DJINN_RECOVERY, // Eddy
    STAGING_DJINNI, // Balm
    STAGING_DJINNI, // Serac
    0, // Water18
    0, // Water19
    STAGING_DJINNI, // Forge
    STAGING_ATTACK, // Fever
    STAGING_DJINNI, // Corona
    STAGING_ATTACK, // Scorch
    STAGING_DJINNI, // Ember
    STAGING_DJINNI, // Flash
    STAGING_DJINNI, // Torch
    STAGING_DJINNI, // Cannon
    STAGING_DJINNI, // Spark
    STAGING_DJINNI, // Kindle
    STAGING_DJINNI, // Char
    STAGING_DJINNI, // Coal
    STAGING_DJINNI, // Reflux
    STAGING_DJINNI, // Core
    STAGING_DJINNI, // Tinder
    STAGING_DJINNI, // Shine
    STAGING_DJINNI, // Fury
    STAGING_DJINNI, // Fugue
    0, // Fire18
    0, // Fire19
    STAGING_DJINNI, // Gust
    STAGING_DJINNI, // Breeze
    STAGING_DJINNI, // Zephyr
    STAGING_ATTACK, // Smog
    STAGING_DJINNI, // Kite
    STAGING_ATTACK, // Squall
    STAGING_DJINNI, // Luff
    STAGING_DJINNI, // Breath
    STAGING_ATTACK, // Blitz
    STAGING_DJINNI, // Ether
    STAGING_DJINNI, // Waft
    STAGING_DJINNI, // Haze
    STAGING_ATTACK, // Wheeze
    STAGING_DJINNI, // Aroma
    STAGING_DJINNI, // Whorl
    STAGING_DJINNI, // Gasp
    STAGING_DJINNI, // Lull
    STAGING_DJINNI, // Gale
    0, // Wind18
    0, // Wind19
    STAGING_CAST, // Venus
    STAGING_SUMMON, // Ramses
    STAGING_SUMMON, // Cybele
    STAGING_SUMMON, // Judgment
    STAGING_FULL_SCREEN, // Zagan
    STAGING_FULL_SCREEN, // Haures
    STAGING_FULL_SCREEN, // Charon
    0, // ?
    STAGING_CAST, // Mercury
    STAGING_SUMMON, // Nereid
    STAGING_SUMMON, // Neptune
    STAGING_SUMMON, // Boreas
    STAGING_SUMMON, // Moloch
    STAGING_FULL_SCREEN, // Coatlicue
    STAGING_SUMMON, // Azul
    0, // ?
    STAGING_CAST, // Mars
    STAGING_SUMMON, // Kirin
    STAGING_SUMMON, // Tiamat
    STAGING_SUMMON, // Meteor
    STAGING_MEGAERA, // Megaera
    STAGING_SUMMON, // Ulysses
    STAGING_FULL_SCREEN, // Daedalus
    STAGING_SUMMON, // Daedalus
    STAGING_IRIS, // Iris
    0, // ?
    STAGING_CAST, // Jupiter
    STAGING_SUMMON, // Atalanta
    STAGING_FULL_SCREEN, // Procne
    STAGING_SUMMON, // Thor
    STAGING_FULL_SCREEN, // Flora
    STAGING_FULL_SCREEN, // Eclipse
    STAGING_FULL_SCREEN, // Catastrophe
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // Fire Breath
    0, // Fire Breath
    0, // Fire Breath
    0, // Water Breath
    0, // Water Breath
    0, // Water Breath
    0, // Ice Breath
    0, // Ice Breath
    0, // Ice Breath
    0, // Dark Breath
    0, // Dark Breath
    0, // Acid Breath
    0, // Acid Breath
    0, // Storm Breath
    0, // Storm Breath
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
    0, // Battle Cry
    0, // Can't Use
    0, // Poison Beat
    0, // Spinning Beat
    STAGING_ATTACK, // Heat Flash
    STAGING_ATTACK, // Death Scythe
    0, // Outer Space
    0, // Dragon Driver
    0, // Drain Fang
    0, // Severe Blow
    0, // Thrash
    0, // ?
    STAGING_ENEMY_CAST, // Stone Justice
    STAGING_ATTACK, // Sarcophagus
    STAGING_ATTACK, // Evil Eye
    STAGING_ATTACK, // Vein Tap
    STAGING_ENEMY_CAST, // Heavy Divide
    STAGING_ATTACK, // Hammersphere
    STAGING_ATTACK, // Mother Earth
    STAGING_ENEMY_CAST, // Wyrd Curse
    STAGING_ENEMY_CAST, // Heartbreak
    STAGING_ENEMY_CAST, // Vengeance
    STAGING_ENEMY_CAST, // Acheron's Grief
    STAGING_DJINNI, // Megiddo
    0, // ?
    0, // ?
    0, // ?
    STAGING_ENEMY_CAST, // Hurricane
    STAGING_ENEMY_CAST, // Dreamtide
    STAGING_ATTACK, // Stun Cloud
    STAGING_ATTACK, // Searing Fog
    STAGING_ATTACK, // Ice Crush
    STAGING_ATTACK, // Flash Force
    STAGING_ATTACK, // Sargasso
    STAGING_ENEMY_CAST, // Lethe Albion
    STAGING_ATTACK, // Reverse Star
    STAGING_ENEMY_CAST, // Rising Dragon
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    STAGING_ATTACK, // Power Drive
    STAGING_ENEMY_CAST, // Boost Hack
    STAGING_ATTACK, // Heat Smash
    STAGING_ENEMY_CAST, // Fire Dance
    STAGING_ENEMY_CAST, // Blaze Rush
    STAGING_ATTACK, // Flare Burst
    STAGING_ENEMY_CAST, // Shred
    STAGING_ATTACK, // Crucible Fire
    STAGING_ATTACK, // Scorpionfish
    STAGING_ENEMY_CAST, // Purgatory
    STAGING_ENEMY_CAST, // Soul Shatter
    STAGING_ENEMY_CAST, // Radiant Fire
    STAGING_ENEMY_CAST, // Life Shear
    0, // ?
    0, // ?
    0, // ?
    STAGING_ATTACK, // Moon Air
    STAGING_ATTACK, // Flash Edge
    STAGING_ENEMY_CAST, // Aging Gas
    STAGING_ENEMY_CAST, // Mad Zephyr
    STAGING_ENEMY_CAST, // Lunar Slash
    STAGING_ENEMY_CAST, // Nirvana
    STAGING_ATTACK, // High Vitals
    STAGING_ATTACK, // Stun Bolt
    STAGING_FULL_SCREEN, // Trident
    STAGING_ENEMY_CAST, // Light Surge
    STAGING_ATTACK, // Raiden's Wrath
    STAGING_ENEMY_CAST, // Void Beam
    STAGING_ATTACK, // Supernova
    STAGING_ATTACK, // Apocalypse
    STAGING_ENEMY_CAST, // Legend
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // Sabre Dance
    0, // Backstab
    0, // Fire Breath
    0, // Juggle
    0, // Heat Juggle
    0, // Fiery Juggle
    0, // Flame Card
    0, // Thunder Card
    0, // Bramble Card
    0, // Frost Card
    0, // Baffle Card
    0, // Sword Card
    0, // Sleep Card
    0, // Death Card
    0, // ?
    0, // Whiplash
    STAGING_BEAST_ATTACK, // Wild Wolf
    STAGING_BEAST_CAST, // Emu
    STAGING_BEAST_ATTACK, // Roc
    STAGING_BEAST_CAST, // Salamander
    STAGING_BEAST_ATTACK, // Orc
    STAGING_BEAST_CAST, // Harpy
    STAGING_BEAST_ATTACK, // Grand Golem
    STAGING_BEAST_CAST, // Cerberus
    STAGING_BEAST_CAST, // Wyvern
    STAGING_BEAST_CAST, // Pixie
    STAGING_BEAST_ATTACK, // Dinox
    STAGING_BEAST_CAST, // Gryphon
    STAGING_BEAST_CAST, // Living Armor
    STAGING_BEAST_CAST, // Chimera
    STAGING_BEAST_CAST, // Blue Dragon
    STAGING_BEAST_CAST, // Faery
    STAGING_BEAST_CAST, // Elder Wood
    STAGING_BEAST_CAST, // Lich
    STAGING_BEAST_CAST, // Troll
    STAGING_BEAST_CAST, // Minotaur
    STAGING_BEAST_CAST, // Ghost Soldier
    STAGING_BEAST_CAST, // Macetail
    STAGING_BEAST_CAST, // Fire Dragon
    STAGING_BEAST_CAST, // Weird Nymph
    STAGING_BEAST_CAST, // Succubus
    STAGING_BEAST_CAST, // Estre Wood
    STAGING_BEAST_CAST, // Manticore
    STAGING_BEAST_CAST, // Phoenix
    0, // ?
    STAGING_BEAST_ATTACK, // Call Zombie
    STAGING_BEAST_CAST, // Call Demon
    STAGING_BEAST_CAST, // Call Dullahan
    0, // Raging Heat
    0, // Fiery Abyss
    0, // Dire Inferno
    0, // Poison Flow
    0, // Fire Puppet
    0, // ?
    0, // ?
    0, // ?
    0, // Sand Breath
    0, // Desert Gasp
    0, // Black Ice
    0, // Toxic Breath
    0, // Typhoon Blow
    0, // Blast Breath
    0, // Gravel Blow
    0, // Darksol Gasp
    0, // Flame Breath
    0, // Aqua Breath
    0, // Fire Breath
    0, // Chill Breath
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    STAGING_ATTACK, // Flash Punch
    0, // Formic Acid
    0, // Claw Attack
    0, // Beat Dance
    0, // Twin Shear
    0, // Paralytail
    0, // Echo Cut
    STAGING_DJINNI, // Clarion Cry
    0, // Triple Chomp
    0, // Raging Flood
    0, // Rising Venom
    0, // Slaver
    0, // Mighty Press
    0, // Star Mine
    0, // Heat Stun
    0, // Ocean Fist
    0, // Watery Grave
    0, // Counter-rush
    0, // Counter
    STAGING_ATTACK, // Bosca Hit
    0, // Strong Hit
    0, // Rolling Flame
    0, // Rising Dragon
    STAGING_FULL_SCREEN, // Meteor Blow
    0, // Cage
    0, // Stun Muscle
    0, // Djinnfest
    0, // Heat Kiss
    STAGING_ATTACK, // Fatal Fang
    0, // Angle Spike
    STAGING_ATTACK, // Quick Slash
    0, // Doublestep
    0, // Maneater
    0, // Claw Slash
    0, // Mega Slash
    0, // Vanish Claw
    0, // Crusher Grip
    0, // Ur Flash
    STAGING_ATTACK, // Power Bite
    STAGING_ATTACK, // Terrible Bite
    0, // Poison Sting
    0, // Kill Sting
    0, // Beast Needle
    0, // Poison Sting
    0, // Stun Sting
    0, // Mucous Gel
    0, // Poison Gel
    0, // Hydro Slash
    0, // Regen Dance
    0, // Fire Dance
    0, // Power Crush
    0, // Sack
    0, // Vital Moon
    0, // Human Hunt
    0, // Demon Eye
    STAGING_ATTACK, // Stun Jip
    0, // Pike Assault
    0, // Formina Sage
    0, // True Collide
    STAGING_ATTACK, // Armor Crush
    0, // Djinn Blast
    0, // Djinn Storm
    0, // Earth Force
    0, // Guard Aura
    STAGING_FULL_SCREEN, // Cruel Ruin
    STAGING_DJINNI, // Mystic Call
    0, // Mine Ball
    STAGING_ATTACK, // Angry Mine
    STAGING_DJINNI, // Djinn Stun
    0, // Crucible
    STAGING_DJINNI, // Element Swap
    0, // Earnest Ply
    STAGING_DJINNI, // Psy Boost
    0, // Pressure
};

/// By ability id, ABILITY_NOTHING to ABILITY_PSY_BOOST.
const u32 gAbilityBattleEffects[] = {
    0, // Nothing
    0, // Attack
    0, // Defend
    1, // Quake
    1 | (1 << EFFECT_VARIANT_SHIFT), // Earthquake
    1 | (2 << EFFECT_VARIANT_SHIFT), // Quake Sphere
    2, // Spire
    2 | (1 << EFFECT_VARIANT_SHIFT), // Clay Spire
    2 | (2 << EFFECT_VARIANT_SHIFT), // Stone Spire
    3, // Gaia
    3 | (1 << EFFECT_VARIANT_SHIFT), // Mother Gaia
    3 | (2 << EFFECT_VARIANT_SHIFT), // Grand Gaia
    4, // Growth
    4 | (1 << EFFECT_VARIANT_SHIFT), // Mad Growth
    4 | (2 << EFFECT_VARIANT_SHIFT), // Wild Growth
    5, // Thorn
    5 | (1 << EFFECT_VARIANT_SHIFT), // Briar
    5 | (2 << EFFECT_VARIANT_SHIFT), // Nettle
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    6, // Frost
    6 | (1 << EFFECT_VARIANT_SHIFT), // Tundra
    6 | (2 << EFFECT_VARIANT_SHIFT), // Glacier
    7, // Ice
    7 | (1 << EFFECT_VARIANT_SHIFT), // Ice Horn
    7 | (2 << EFFECT_VARIANT_SHIFT), // Ice Missile
    8, // Prism
    8 | (1 << EFFECT_VARIANT_SHIFT), // Hail Prism
    8 | (2 << EFFECT_VARIANT_SHIFT), // Freeze Prism
    9, // Douse
    9 | (1 << EFFECT_VARIANT_SHIFT), // Drench
    9 | (2 << EFFECT_VARIANT_SHIFT), // Deluge
    10, // Froth
    10 | (1 << EFFECT_VARIANT_SHIFT), // Froth Sphere
    10 | (2 << EFFECT_VARIANT_SHIFT), // Froth Spiral
    75, // Cool
    75 | (1 << EFFECT_VARIANT_SHIFT), // Supercool
    75 | (2 << EFFECT_VARIANT_SHIFT), // Megacool
    0, // ?
    0, // ?
    0, // ?
    11, // Flare
    11 | (1 << EFFECT_VARIANT_SHIFT), // Flare Wall
    11 | (2 << EFFECT_VARIANT_SHIFT), // Flare Storm
    12, // Fire
    12 | (1 << EFFECT_VARIANT_SHIFT), // Fireball
    12 | (2 << EFFECT_VARIANT_SHIFT), // Inferno
    13, // Volcano
    13 | (1 << EFFECT_VARIANT_SHIFT), // Eruption
    13 | (2 << EFFECT_VARIANT_SHIFT), // Pyroclasm
    14, // Blast
    14 | (1 << EFFECT_VARIANT_SHIFT), // Mad Blast
    14 | (2 << EFFECT_VARIANT_SHIFT), // Fiery Blast
    15, // Blast
    15 | (1 << EFFECT_VARIANT_SHIFT), // Nova
    15 | (2 << EFFECT_VARIANT_SHIFT), // Supernova
    72, // Fume
    72 | (1 << EFFECT_VARIANT_SHIFT), // Serpent Fume
    72 | (2 << EFFECT_VARIANT_SHIFT), // Dragon Fume
    73, // Beam
    73 | (1 << EFFECT_VARIANT_SHIFT), // Cycle Beam
    73 | (2 << EFFECT_VARIANT_SHIFT), // Searing Beam
    16, // Bolt
    16 | (1 << EFFECT_VARIANT_SHIFT), // Flash Bolt
    16 | (2 << EFFECT_VARIANT_SHIFT), // Blue Bolt
    17, // Ray
    17 | (1 << EFFECT_VARIANT_SHIFT), // Storm Ray
    17 | (2 << EFFECT_VARIANT_SHIFT), // Destruct Ray
    18, // Plasma
    18 | (1 << EFFECT_VARIANT_SHIFT), // Shine Plasma
    18 | (2 << EFFECT_VARIANT_SHIFT), // Spark Plasma
    19, // Slash
    19 | (1 << EFFECT_VARIANT_SHIFT), // Wind Slash
    19 | (2 << EFFECT_VARIANT_SHIFT), // Sonic Slash
    20, // Whirlwind
    20 | (1 << EFFECT_VARIANT_SHIFT), // Tornado
    20 | (2 << EFFECT_VARIANT_SHIFT), // Tempest
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    71, // Aura
    71 | (1 << EFFECT_VARIANT_SHIFT), // Healing Aura
    71 | (2 << EFFECT_VARIANT_SHIFT), // Cool Aura
    21, // Cure
    21 | (1 << EFFECT_VARIANT_SHIFT), // Cure Well
    21 | (2 << EFFECT_VARIANT_SHIFT), // Potent Cure
    22, // Ply
    22 | (1 << EFFECT_VARIANT_SHIFT), // Ply Well
    22 | (2 << EFFECT_VARIANT_SHIFT), // Pure Ply
    23, // Wish
    23 | (1 << EFFECT_VARIANT_SHIFT), // Wish Well
    23 | (2 << EFFECT_VARIANT_SHIFT), // Pure Wish
    24, // Cure Poison
    24 | (1 << EFFECT_VARIANT_SHIFT), // Restore
    25, // Revive
    26, // Impact
    26 | (1 << EFFECT_VARIANT_SHIFT), // High Impact
    27, // Dull
    27 | (1 << EFFECT_VARIANT_SHIFT), // Blunt
    28, // Guard
    28 | (1 << EFFECT_VARIANT_SHIFT), // Protect
    29, // Impair
    29 | (1 << EFFECT_VARIANT_SHIFT), // Debilitate
    30, // Ward
    30 | (1 << EFFECT_VARIANT_SHIFT), // Resist
    31, // Weaken
    31 | (1 << EFFECT_VARIANT_SHIFT), // Enfeeble
    32, // Taint
    32, // Poison
    33, // Delude
    33 | (1 << EFFECT_VARIANT_SHIFT), // Confuse
    0, // Charm
    36, // Paralyze
    37, // Sleep
    38, // Bind
    39, // Haunt
    41, // Curse
    40, // Condemn
    42, // Drain
    42 | (1 << EFFECT_VARIANT_SHIFT), // Psy Drain
    43, // Break
    44, // Regenerate
    45, // Reflect
    0, // ?
    0, // ?
    0, // ?
    0, // Lash
    0, // Pound
    0, // Tremor
    0, // Scoop
    0, // Cyclone
    0, // Parch
    0, // Sand
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
    0, // Burst
    0, // Grind
    0, // Hover
    0, // Blaze
    0, // Ma????
    0, // Teleport
    0, // A??
    0, // ?
    0, // ?
    49, // Dragon Cloud
    50, // Demon Night
    51, // Helm Splitter
    52, // Quick Strike
    3, // Rockfall
    3 | (1 << EFFECT_VARIANT_SHIFT), // Rockslide
    3 | (2 << EFFECT_VARIANT_SHIFT), // Avalanche
    13, // Lava Shower
    13 | (1 << EFFECT_VARIANT_SHIFT), // Molten Bath
    13 | (2 << EFFECT_VARIANT_SHIFT), // Magma Storm
    26, // Demon Spear
    26 | (1 << EFFECT_VARIANT_SHIFT), // Angel Spear
    28, // Guardian
    28, // Protector
    30, // Magic Shell
    30 | (1 << EFFECT_VARIANT_SHIFT), // Magic Shield
    53, // Death Plunge
    54, // Shuriken
    55, // Annihilation
    56, // Punji
    56 | (1 << EFFECT_VARIANT_SHIFT), // Punji Trap
    56 | (1 << EFFECT_VARIANT_SHIFT), // Punji Strike
    15, // Fire Bomb
    15 | (1 << EFFECT_VARIANT_SHIFT), // Cluster Bomb
    15 | (2 << EFFECT_VARIANT_SHIFT), // Carpet Bomb
    20, // Gale
    20 | (1 << EFFECT_VARIANT_SHIFT), // Typhoon
    20 | (2 << EFFECT_VARIANT_SHIFT), // Hurricane
    18, // Thunderclap
    18 | (1 << EFFECT_VARIANT_SHIFT), // Thunderbolt
    18 | (2 << EFFECT_VARIANT_SHIFT), // Thunderhead
    33, // Mist
    58, // Ragnarok
    57, // Cutting Edge
    59, // Heat Wave
    68 | (1 << EFFECT_VARIANT_SHIFT) | EFFECT_VIEW_REVERSED, // Astral Blast
    60, // Planet Diver
    83, // Diamond Dust
    125 | EFFECT_VIEW_REVERSED, // Odyssey
    123 | EFFECT_VIEW_REVERSED, // Liquifier
    85, // Plume Edge
    86, // Thunder Mine
    128, // Planetary
    84, // Diamond Berg
    53 | (1 << EFFECT_VARIANT_SHIFT), // Death Leap
    49 | (1 << EFFECT_VARIANT_SHIFT), // Epicenter
    127, // Thorny Grave
    51 | (1 << EFFECT_VARIANT_SHIFT), // Skull Splitter
    0, // ?
    0, // ?
    138 | EFFECT_BY_ID_RANGE, // Terra Strike
    139 | EFFECT_BY_ID_RANGE, // Poison Cloud
    140 | EFFECT_BY_ID_RANGE, // Poison Death
    141 | EFFECT_BY_ID_RANGE, // Mortal Danger
    142 | EFFECT_BY_ID_RANGE, // Bad Omen
    143 | EFFECT_BY_ID_RANGE, // Life Nourish
    144 | EFFECT_BY_ID_RANGE, // Aqua Sock
    63, // Blizzard
    146 | EFFECT_BY_ID_RANGE, // Frost Bite
    66, // Drown
    148 | EFFECT_BY_ID_RANGE, // Life Leech
    149 | EFFECT_BY_ID_RANGE, // Psy Leech
    150 | EFFECT_BY_ID_RANGE, // Broil
    151 | EFFECT_BY_ID_RANGE, // Meltdown
    152 | EFFECT_BY_ID_RANGE, // Heat Mirage
    153 | EFFECT_BY_ID_RANGE, // Barrage
    64, // Demonfire
    155 | EFFECT_BY_ID_RANGE, // Acid Bath
    65, // Vorpal Slash
    157 | EFFECT_BY_ID_RANGE, // Stun Voltage
    158 | EFFECT_BY_ID_RANGE, // Blinding Smog
    159 | EFFECT_BY_ID_RANGE, // Murk
    160 | EFFECT_BY_ID_RANGE, // Cyclone Slash
    161 | EFFECT_BY_ID_RANGE, // Psyphon Seal
    310, // Rapid Smash
    310, // Sonic Smash
    162 | EFFECT_BY_ID_RANGE, // Asura
    61, // Titan Blade
    62, // Shining Star
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
    22, // {article_class:3}Herb
    22, // {article_class:2}Nut
    22, // {article_class:2}Vial
    22, // {article_class:2}Potion
    22, // {article_class:5}Soothing Water
    212, // {article_class:2}Psy Crystal
    24, // {article_class:2}Antidote
    24 | (1 << EFFECT_VARIANT_SHIFT), // {article_class:3}Elixir
    25, // {article_class:4}Water of Life
    23 | (2 << EFFECT_VARIANT_SHIFT), // {article_class:2}Mist Potion
    0, // {article_class:4}Power Bread
    0, // {article_class:2}Cookie
    0, // {article_class:3}Apple
    0, // {article_class:2}Hard Nut
    0, // {article_class:2}Mint
    0, // {article_class:4}Lucky Pepper
    0, // ?
    380, // {article_class:2}Smoke Bomb
    380, // {article_class:2}Sleep Bomb
    212, // {article_class:3}Adept Ring
    21, // {article_class:4}Corn
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
    0, // Aurora Field
    249, // Djinn Counter
    104 | EFFECT_BY_ID_RANGE, // Flint
    200, // Granite
    201, // Quartz
    202, // Vine
    107 | EFFECT_BY_ID_RANGE, // Sap
    203, // Ground
    100 | EFFECT_BY_ID_RANGE, // Bane
    108 | EFFECT_BY_ID_RANGE, // Echo
    220, // Iron
    112 | EFFECT_BY_ID_RANGE, // Steel
    237, // Mud
    225, // Flower
    251, // Meld
    239, // Petra
    226, // Salt
    253, // Geode
    250, // Mold
    244, // Crystal
    0, // Earth18
    0, // Earth19
    204, // Fizz
    205, // Sleet
    102 | EFFECT_BY_ID_RANGE, // Mist
    206, // Spritz
    207, // Hail
    208, // Tonic
    209, // Dew
    113 | EFFECT_BY_ID_RANGE, // Fog
    109 | EFFECT_BY_ID_RANGE, // Sour
    227, // Spring
    222, // Shade
    110 | EFFECT_BY_ID_RANGE, // Chill
    221, // Steam
    240, // Rime
    111 | EFFECT_BY_ID_RANGE, // Gel
    245, // Eddy
    230, // Balm
    254, // Serac
    0, // Water18
    0, // Water19
    210, // Forge
    103 | EFFECT_BY_ID_RANGE, // Fever
    211, // Corona
    105 | EFFECT_BY_ID_RANGE, // Scorch
    212, // Ember
    213, // Flash
    214, // Torch
    246, // Cannon
    255, // Spark
    223, // Kindle
    257, // Char
    224, // Coal
    248, // Reflux
    236, // Core
    231, // Tinder
    252, // Shine
    256, // Fury
    243, // Fugue
    0, // Fire18
    0, // Fire19
    219, // Gust
    215, // Breeze
    216, // Zephyr
    101 | EFFECT_BY_ID_RANGE, // Smog
    217, // Kite
    106 | EFFECT_BY_ID_RANGE, // Squall
    218, // Luff
    228, // Breath
    117 | EFFECT_BY_ID_RANGE, // Blitz
    229, // Ether
    234, // Waft
    238, // Haze
    116 | EFFECT_BY_ID_RANGE, // Wheeze
    247, // Aroma
    232, // Whorl
    242, // Gasp
    241, // Lull
    233, // Gale
    0, // Wind18
    0, // Wind19
    280 | EFFECT_VIEW_ACROSS, // Venus
    1 | EFFECT_SUMMON, // Ramses
    5 | EFFECT_SUMMON, // Cybele
    9 | EFFECT_SUMMON, // Judgment
    13 | EFFECT_SUMMON, // Zagan
    18 | EFFECT_SUMMON, // Haures
    24 | EFFECT_SUMMON, // Charon
    0, // ?
    281 | EFFECT_VIEW_ACROSS, // Mercury
    2 | EFFECT_SUMMON, // Nereid
    6 | EFFECT_SUMMON, // Neptune
    10 | EFFECT_SUMMON, // Boreas
    16 | EFFECT_SUMMON, // Moloch
    20 | EFFECT_SUMMON, // Coatlicue
    22 | EFFECT_SUMMON, // Azul
    0, // ?
    282 | EFFECT_VIEW_ACROSS, // Mars
    3 | EFFECT_SUMMON, // Kirin
    7 | EFFECT_SUMMON, // Tiamat
    11 | EFFECT_SUMMON, // Meteor
    14 | EFFECT_SUMMON, // Megaera
    17 | EFFECT_SUMMON, // Ulysses
    21 | EFFECT_SUMMON, // Daedalus
    26 | EFFECT_SUMMON, // Daedalus
    25 | EFFECT_SUMMON, // Iris
    0, // ?
    283 | EFFECT_VIEW_ACROSS, // Jupiter
    4 | EFFECT_SUMMON, // Atalanta
    8 | EFFECT_SUMMON, // Procne
    12 | EFFECT_SUMMON, // Thor
    15 | EFFECT_SUMMON, // Flora
    19 | EFFECT_SUMMON, // Eclipse
    23 | EFFECT_SUMMON, // Catastrophe
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    400 | EFFECT_VIEW_REVERSED, // Fire Breath
    400 | EFFECT_VIEW_REVERSED, // Fire Breath
    400 | EFFECT_VIEW_REVERSED, // Fire Breath
    401 | EFFECT_VIEW_REVERSED, // Water Breath
    401 | EFFECT_VIEW_REVERSED, // Water Breath
    401 | EFFECT_VIEW_REVERSED, // Water Breath
    402, // Ice Breath
    402, // Ice Breath
    402, // Ice Breath
    403 | EFFECT_VIEW_REVERSED, // Dark Breath
    403 | EFFECT_VIEW_REVERSED, // Dark Breath
    404 | EFFECT_VIEW_REVERSED, // Acid Breath
    404 | EFFECT_VIEW_REVERSED, // Acid Breath
    405 | EFFECT_VIEW_REVERSED, // Storm Breath
    471 | EFFECT_VIEW_REVERSED, // Storm Breath
    300, // Sonic Wave
    300, // Sonic Wave
    301, // Shriek
    302, // Banshee Howl
    303, // Crazy Voice
    304, // War Cry
    305, // Wicked Howl
    306, // Wing Beat
    307, // Wing Flutter
    381, // Wing Stroke
    406 | EFFECT_VIEW_REVERSED, // Evil Blessing
    407 | EFFECT_VIEW_REVERSED, // Deadly Gas
    0, // ?
    0, // ?
    0, // ?
    308, // Rumble
    309, // Bone Chiller
    310, // Slice
    311, // Bone Charge
    312, // Mystic Flame
    313, // Numbing Sting
    314, // Brute Force
    315, // Sticky Goo
    316, // Cannibal Fang
    317, // Bear Claw
    318, // Poisonous Bite
    319, // Flying Attack
    320, // Undead Sword
    321, // Ransack
    322, // Sticky Poison
    323, // Poison Fang
    324, // Electric Bite
    325, // Poison Tail
    326, // Onslaught
    327, // Vampiric Fang
    328, // Bacteria Rush
    329, // Swift Strike
    330, // Rotten Blood
    163 | EFFECT_BY_ID_RANGE, // Forcible Arm
    332, // Double Fang
    333, // Mortal Blow
    334, // Freebite Rush
    335, // Twin Beaks
    336, // Rabid Fang
    337, // Acid Bite
    338, // Dynamite
    339, // Headbutt
    340, // Poison Ink
    164 | EFFECT_BY_ID_RANGE, // Truncheon Fist
    342, // Counterstrike
    343, // Mad Dash
    344, // Soothing Star
    345, // Spider Web
    346, // Heartrender
    347, // Mad Spatter
    348, // Spasm
    349, // Sleep Star
    350, // Decompose
    351, // Haunting
    352, // Worms
    353, // Berserk
    354, // Lucid Prophecy
    389, // Recovery
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
    0, // Battle Cry
    0, // Can't Use
    382, // Poison Beat
    383, // Spinning Beat
    152 | EFFECT_BY_ID_RANGE, // Heat Flash
    141 | EFFECT_BY_ID_RANGE, // Death Scythe
    384 | EFFECT_VIEW_REVERSED, // Outer Space
    385 | EFFECT_VIEW_REVERSED, // Dragon Driver
    386, // Drain Fang
    387, // Severe Blow
    388, // Thrash
    0, // ?
    100, // Stone Justice
    185 | EFFECT_BY_ID_RANGE, // Sarcophagus
    172 | EFFECT_BY_ID_RANGE, // Evil Eye
    173 | EFFECT_BY_ID_RANGE, // Vein Tap
    101, // Heavy Divide
    179 | EFFECT_BY_ID_RANGE, // Hammersphere
    194 | EFFECT_BY_ID_RANGE, // Mother Earth
    103, // Wyrd Curse
    69, // Heartbreak
    126, // Vengeance
    104, // Acheron's Grief
    124, // Megiddo
    0, // ?
    0, // ?
    0, // ?
    105, // Hurricane
    106, // Dreamtide
    168 | EFFECT_BY_ID_RANGE, // Stun Cloud
    169 | EFFECT_BY_ID_RANGE, // Searing Fog
    170 | EFFECT_BY_ID_RANGE, // Ice Crush
    180 | EFFECT_BY_ID_RANGE, // Flash Force
    171 | EFFECT_BY_ID_RANGE, // Sargasso
    107, // Lethe Albion
    182 | EFFECT_BY_ID_RANGE, // Reverse Star
    108, // Rising Dragon
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    166 | EFFECT_BY_ID_RANGE, // Power Drive
    109, // Boost Hack
    167 | EFFECT_BY_ID_RANGE, // Heat Smash
    111, // Fire Dance
    112, // Blaze Rush
    178 | EFFECT_BY_ID_RANGE, // Flare Burst
    113, // Shred
    181 | EFFECT_BY_ID_RANGE, // Crucible Fire
    174 | EFFECT_BY_ID_RANGE, // Scorpionfish
    114, // Purgatory
    115, // Soul Shatter
    110, // Radiant Fire
    129, // Life Shear
    0, // ?
    0, // ?
    0, // ?
    175 | EFFECT_BY_ID_RANGE, // Moon Air
    176 | EFFECT_BY_ID_RANGE, // Flash Edge
    116, // Aging Gas
    117, // Mad Zephyr
    118, // Lunar Slash
    119, // Nirvana
    177 | EFFECT_BY_ID_RANGE, // High Vitals
    184 | EFFECT_BY_ID_RANGE, // Stun Bolt
    27 | EFFECT_SUMMON, // Trident
    120, // Light Surge
    186 | EFFECT_BY_ID_RANGE, // Raiden's Wrath
    121, // Void Beam
    183 | EFFECT_BY_ID_RANGE, // Supernova
    188 | EFFECT_BY_ID_RANGE, // Apocalypse
    122, // Legend
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    76, // Sabre Dance
    77, // Backstab
    400, // Fire Breath
    78, // Juggle
    78 | (1 << EFFECT_VARIANT_SHIFT), // Heat Juggle
    78 | (2 << EFFECT_VARIANT_SHIFT), // Fiery Juggle
    79, // Flame Card
    89, // Thunder Card
    88, // Bramble Card
    87, // Frost Card
    90, // Baffle Card
    91, // Sword Card
    92, // Sleep Card
    93, // Death Card
    0, // ?
    80, // Whiplash
    196 | EFFECT_BY_ID_RANGE, // Wild Wolf
    446 | EFFECT_VIEW_REVERSED, // Emu
    195 | EFFECT_BY_ID_RANGE, // Roc
    400 | EFFECT_VIEW_REVERSED, // Salamander
    195 | EFFECT_BY_ID_RANGE, // Orc
    301, // Harpy
    163 | EFFECT_BY_ID_RANGE, // Grand Golem
    400 | EFFECT_VIEW_REVERSED, // Cerberus
    400 | EFFECT_VIEW_REVERSED, // Wyvern
    22, // Pixie
    197 | EFFECT_BY_ID_RANGE, // Dinox
    335, // Gryphon
    457, // Living Armor
    400 | EFFECT_VIEW_REVERSED, // Chimera
    402 | EFFECT_VIEW_REVERSED, // Blue Dragon
    22, // Faery
    23 | (1 << EFFECT_VARIANT_SHIFT), // Elder Wood
    25, // Lich
    314, // Troll
    457, // Minotaur
    449 | EFFECT_VIEW_REVERSED, // Ghost Soldier
    400 | EFFECT_VIEW_REVERSED, // Macetail
    400 | EFFECT_VIEW_REVERSED, // Fire Dragon
    22, // Weird Nymph
    22 | (1 << EFFECT_VARIANT_SHIFT), // Succubus
    23 | (1 << EFFECT_VARIANT_SHIFT), // Estre Wood
    71 | (2 << EFFECT_VARIANT_SHIFT), // Manticore
    436, // Phoenix
    0, // ?
    199 | EFFECT_BY_ID_RANGE, // Call Zombie
    446 | (2 << EFFECT_VARIANT_SHIFT) | EFFECT_VIEW_REVERSED, // Call Demon
    448 | EFFECT_VIEW_REVERSED, // Call Dullahan
    74, // Raging Heat
    74 | (1 << EFFECT_VARIANT_SHIFT), // Fiery Abyss
    74 | (2 << EFFECT_VARIANT_SHIFT), // Dire Inferno
    81, // Poison Flow
    82, // Fire Puppet
    0, // ?
    0, // ?
    0, // ?
    408 | EFFECT_VIEW_REVERSED, // Sand Breath
    409 | EFFECT_VIEW_REVERSED, // Desert Gasp
    410 | EFFECT_VIEW_REVERSED, // Black Ice
    412 | EFFECT_VIEW_REVERSED, // Toxic Breath
    411 | EFFECT_VIEW_REVERSED, // Typhoon Blow
    414 | EFFECT_VIEW_REVERSED, // Blast Breath
    413 | EFFECT_VIEW_REVERSED, // Gravel Blow
    415 | EFFECT_VIEW_REVERSED, // Darksol Gasp
    416 | EFFECT_VIEW_REVERSED, // Flame Breath
    401 | EFFECT_VIEW_REVERSED, // Aqua Breath
    416 | EFFECT_VIEW_REVERSED, // Fire Breath
    470, // Chill Breath
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    0, // ?
    189 | EFFECT_BY_ID_RANGE, // Flash Punch
    453, // Formic Acid
    454, // Claw Attack
    455, // Beat Dance
    425, // Twin Shear
    313, // Paralytail
    390 | EFFECT_VIEW_REVERSED, // Echo Cut
    467, // Clarion Cry
    426, // Triple Chomp
    392, // Raging Flood
    430, // Rising Venom
    395, // Slaver
    394, // Mighty Press
    452, // Star Mine
    458, // Heat Stun
    447, // Ocean Fist
    391, // Watery Grave
    396, // Counter-rush
    0, // Counter
    193 | EFFECT_BY_ID_RANGE, // Bosca Hit
    469, // Strong Hit
    434 | EFFECT_VIEW_REVERSED, // Rolling Flame
    450, // Rising Dragon
    29 | EFFECT_SUMMON, // Meteor Blow
    435, // Cage
    433, // Stun Muscle
    397, // Djinnfest
    398, // Heat Kiss
    196 | EFFECT_BY_ID_RANGE, // Fatal Fang
    399, // Angle Spike
    192 | EFFECT_BY_ID_RANGE, // Quick Slash
    427, // Doublestep
    421, // Maneater
    446 | EFFECT_VIEW_REVERSED, // Claw Slash
    446 | (1 << EFFECT_VARIANT_SHIFT) | EFFECT_VIEW_REVERSED, // Mega Slash
    424, // Vanish Claw
    423, // Crusher Grip
    422, // Ur Flash
    197 | EFFECT_BY_ID_RANGE, // Power Bite
    198 | EFFECT_BY_ID_RANGE, // Terrible Bite
    440, // Poison Sting
    441, // Kill Sting
    442, // Beast Needle
    443, // Poison Sting
    444, // Stun Sting
    429, // Mucous Gel
    428, // Poison Gel
    393, // Hydro Slash
    436, // Regen Dance
    432 | EFFECT_VIEW_REVERSED, // Fire Dance
    457, // Power Crush
    431, // Sack
    446 | (2 << EFFECT_VARIANT_SHIFT) | EFFECT_VIEW_REVERSED, // Vital Moon
    437, // Human Hunt
    445 | EFFECT_VIEW_REVERSED, // Demon Eye
    191 | EFFECT_BY_ID_RANGE, // Stun Jip
    449 | EFFECT_VIEW_REVERSED, // Pike Assault
    448 | EFFECT_VIEW_REVERSED, // Formina Sage
    451, // True Collide
    190 | EFFECT_BY_ID_RANGE, // Armor Crush
    461, // Djinn Blast
    462, // Djinn Storm
    459 | EFFECT_VIEW_REVERSED, // Earth Force
    460, // Guard Aura
    28 | EFFECT_SUMMON, // Cruel Ruin
    466, // Mystic Call
    463, // Mine Ball
    338, // Angry Mine
    465, // Djinn Stun
    0, // Crucible
    464, // Element Swap
    22 | (2 << EFFECT_VARIANT_SHIFT), // Earnest Ply
    468, // Psy Boost
};
// clang-format on
