/* @file */
/* How each ability is staged in battle: the outcome staging */
/* btActionRunBuild gives it and the effect it plays, by ability id, and */
/* the effect chance it rolls ahead. The ability names in the comments are the
 */
/* English release's; `?` is the name MsgAbilityName gives those ids. An */
/* effect's kind, its low twelve bits, is an BATTLE_EFFECT_ID, an enum */
/* SUMMON_EFFECT_ID with EFFECT_SUMMON, and a number of */
/* btEffectRunByIdRange's own with EFFECT_BY_ID_RANGE. */

#include "battle/ability_staging.h"
#include "battle_effect/battle_effect.h"

const u8 ActionRun_EffectChance[] = {100, 60, 30, 30, 30, 30};

/* clang-format off */
/* By ability id, ABILITY_NOTHING to ABILITY_PRESSURE; 0 leaves the choice to */
/* btActionRunBuild. */
const u8 AbilityStaging[] = {
    0, /* Nothing */
    0, /* Attack */
    0, /* Defend */
    0, /* Quake */
    0, /* Earthquake */
    0, /* Quake Sphere */
    0, /* Spire */
    0, /* Clay Spire */
    0, /* Stone Spire */
    STAGING_FULL_SCREEN, /* Gaia */
    STAGING_FULL_SCREEN, /* Mother Gaia */
    STAGING_FULL_SCREEN, /* Grand Gaia */
    0, /* Growth */
    0, /* Mad Growth */
    0, /* Wild Growth */
    0, /* Thorn */
    0, /* Briar */
    0, /* Nettle */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* Frost */
    0, /* Tundra */
    0, /* Glacier */
    0, /* Ice */
    0, /* Ice Horn */
    0, /* Ice Missile */
    0, /* Prism */
    0, /* Hail Prism */
    0, /* Freeze Prism */
    0, /* Douse */
    0, /* Drench */
    0, /* Deluge */
    0, /* Froth */
    0, /* Froth Sphere */
    0, /* Froth Spiral */
    0, /* Cool */
    0, /* Supercool */
    0, /* Megacool */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* Flare */
    0, /* Flare Wall */
    0, /* Flare Storm */
    0, /* Fire */
    0, /* Fireball */
    0, /* Inferno */
    0, /* Volcano */
    0, /* Eruption */
    0, /* Pyroclasm */
    0, /* Blast */
    0, /* Mad Blast */
    0, /* Fiery Blast */
    0, /* Blast */
    0, /* Nova */
    0, /* Supernova */
    0, /* Fume */
    0, /* Serpent Fume */
    0, /* Dragon Fume */
    0, /* Beam */
    0, /* Cycle Beam */
    0, /* Searing Beam */
    0, /* Bolt */
    0, /* Flash Bolt */
    0, /* Blue Bolt */
    0, /* Ray */
    0, /* Storm Ray */
    0, /* Destruct Ray */
    0, /* Plasma */
    0, /* Shine Plasma */
    0, /* Spark Plasma */
    0, /* Slash */
    0, /* Wind Slash */
    0, /* Sonic Slash */
    0, /* Whirlwind */
    0, /* Tornado */
    0, /* Tempest */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* Aura */
    0, /* Healing Aura */
    0, /* Cool Aura */
    0, /* Cure */
    0, /* Cure Well */
    0, /* Potent Cure */
    0, /* Ply */
    0, /* Ply Well */
    0, /* Pure Ply */
    0, /* Wish */
    0, /* Wish Well */
    0, /* Pure Wish */
    0, /* Cure Poison */
    0, /* Restore */
    0, /* Revive */
    0, /* Impact */
    0, /* High Impact */
    0, /* Dull */
    0, /* Blunt */
    0, /* Guard */
    0, /* Protect */
    0, /* Impair */
    0, /* Debilitate */
    0, /* Ward */
    0, /* Resist */
    0, /* Weaken */
    0, /* Enfeeble */
    0, /* Taint */
    0, /* Poison */
    0, /* Delude */
    0, /* Confuse */
    0, /* Charm */
    0, /* Paralyze */
    0, /* Sleep */
    0, /* Bind */
    0, /* Haunt */
    0, /* Curse */
    0, /* Condemn */
    0, /* Drain */
    0, /* Psy Drain */
    0, /* Break */
    0, /* Regenerate */
    0, /* Reflect */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* Lash */
    0, /* Pound */
    0, /* Tremor */
    0, /* Scoop */
    0, /* Cyclone */
    0, /* Parch */
    0, /* Sand */
    0, /* Move */
    0, /* Mind Read */
    0, /* Force */
    0, /* Lift */
    0, /* Reveal */
    0, /* Halt */
    0, /* Cloak */
    0, /* Carry */
    0, /* Catch */
    0, /* Retreat */
    0, /* Avoid */
    0, /* Burst */
    0, /* Grind */
    0, /* Hover */
    0, /* Blaze */
    0, /* Ma???? */
    0, /* Teleport */
    0, /* A?? */
    0, /* ? */
    0, /* ? */
    0, /* Dragon Cloud */
    0, /* Demon Night */
    0, /* Helm Splitter */
    0, /* Quick Strike */
    STAGING_FULL_SCREEN, /* Rockfall */
    STAGING_FULL_SCREEN, /* Rockslide */
    STAGING_FULL_SCREEN, /* Avalanche */
    0, /* Lava Shower */
    0, /* Molten Bath */
    0, /* Magma Storm */
    0, /* Demon Spear */
    0, /* Angel Spear */
    0, /* Guardian */
    0, /* Protector */
    0, /* Magic Shell */
    0, /* Magic Shield */
    0, /* Death Plunge */
    0, /* Shuriken */
    0, /* Annihilation */
    0, /* Punji */
    0, /* Punji Trap */
    0, /* Punji Strike */
    0, /* Fire Bomb */
    0, /* Cluster Bomb */
    0, /* Carpet Bomb */
    0, /* Gale */
    0, /* Typhoon */
    0, /* Hurricane */
    0, /* Thunderclap */
    0, /* Thunderbolt */
    0, /* Thunderhead */
    0, /* Mist */
    0, /* Ragnarok */
    0, /* Cutting Edge */
    0, /* Heat Wave */
    0, /* Astral Blast */
    0, /* Planet Diver */
    0, /* Diamond Dust */
    0, /* Odyssey */
    0, /* Liquifier */
    0, /* Plume Edge */
    0, /* Thunder Mine */
    0, /* Planetary */
    0, /* Diamond Berg */
    0, /* Death Leap */
    0, /* Epicenter */
    0, /* Thorny Grave */
    0, /* Skull Splitter */
    0, /* ? */
    0, /* ? */
    STAGING_ATTACK, /* Terra Strike */
    STAGING_ATTACK, /* Poison Cloud */
    STAGING_ATTACK, /* Poison Death */
    STAGING_ATTACK, /* Mortal Danger */
    STAGING_ATTACK, /* Bad Omen */
    STAGING_ATTACK, /* Life Nourish */
    STAGING_ATTACK, /* Aqua Sock */
    STAGING_ENEMY_CAST, /* Blizzard */
    STAGING_ATTACK, /* Frost Bite */
    STAGING_ENEMY_CAST, /* Drown */
    STAGING_ATTACK, /* Life Leech */
    STAGING_ATTACK, /* Psy Leech */
    STAGING_ATTACK, /* Broil */
    STAGING_ATTACK, /* Meltdown */
    STAGING_ATTACK, /* Heat Mirage */
    STAGING_ATTACK, /* Barrage */
    STAGING_ENEMY_CAST, /* Demonfire */
    STAGING_ATTACK, /* Acid Bath */
    STAGING_ENEMY_CAST, /* Vorpal Slash */
    STAGING_ATTACK, /* Stun Voltage */
    STAGING_ATTACK, /* Blinding Smog */
    STAGING_ATTACK, /* Murk */
    STAGING_ATTACK, /* Cyclone Slash */
    STAGING_ATTACK, /* Psyphon Seal */
    STAGING_ENEMY_CAST, /* Rapid Smash */
    STAGING_ENEMY_CAST, /* Sonic Smash */
    STAGING_ATTACK, /* Asura */
    STAGING_ENEMY_CAST, /* Titan Blade */
    0, /* Shining Star */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* Herb */
    0, /* Nut */
    0, /* Vial */
    0, /* Potion */
    0, /* Soothing Water */
    0, /* Psy Crystal */
    0, /* Antidote */
    0, /* Elixir */
    0, /* Water of Life */
    0, /* Mist Potion */
    0, /* Power Bread */
    0, /* Cookie */
    0, /* Apple */
    0, /* Hard Nut */
    0, /* Mint */
    0, /* Lucky Pepper */
    0, /* ? */
    0, /* Smoke Bomb */
    0, /* Sleep Bomb */
    0, /* Adept Ring */
    0, /* Corn */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* Aurora Field */
    STAGING_DJINNI, /* Djinn Counter */
    STAGING_ATTACK, /* Flint */
    STAGING_DJINNI, /* Granite */
    STAGING_DJINNI, /* Quartz */
    STAGING_DJINNI, /* Vine */
    STAGING_ATTACK, /* Sap */
    STAGING_DJINNI, /* Ground */
    STAGING_ATTACK, /* Bane */
    STAGING_ATTACK, /* Echo */
    STAGING_DJINNI, /* Iron */
    STAGING_ATTACK, /* Steel */
    STAGING_DJINNI, /* Mud */
    STAGING_DJINNI, /* Flower */
    STAGING_TEAM_STRIKE, /* Meld */
    STAGING_DJINNI, /* Petra */
    STAGING_DJINNI, /* Salt */
    STAGING_DJINNI, /* Geode */
    STAGING_TEAM_STRIKE, /* Mold */
    STAGING_DJINNI, /* Crystal */
    0, /* Earth18 */
    0, /* Earth19 */
    STAGING_DJINNI, /* Fizz */
    STAGING_DJINNI, /* Sleet */
    STAGING_ATTACK, /* Mist */
    STAGING_DJINNI, /* Spritz */
    STAGING_DJINNI, /* Hail */
    STAGING_DJINNI, /* Tonic */
    STAGING_DJINNI, /* Dew */
    STAGING_ATTACK, /* Fog */
    STAGING_ATTACK, /* Sour */
    STAGING_DJINNI, /* Spring */
    STAGING_DJINNI, /* Shade */
    STAGING_ATTACK, /* Chill */
    STAGING_DJINNI, /* Steam */
    STAGING_DJINNI, /* Rime */
    STAGING_ATTACK, /* Gel */
    STAGING_DJINN_RECOVERY, /* Eddy */
    STAGING_DJINNI, /* Balm */
    STAGING_DJINNI, /* Serac */
    0, /* Water18 */
    0, /* Water19 */
    STAGING_DJINNI, /* Forge */
    STAGING_ATTACK, /* Fever */
    STAGING_DJINNI, /* Corona */
    STAGING_ATTACK, /* Scorch */
    STAGING_DJINNI, /* Ember */
    STAGING_DJINNI, /* Flash */
    STAGING_DJINNI, /* Torch */
    STAGING_DJINNI, /* Cannon */
    STAGING_DJINNI, /* Spark */
    STAGING_DJINNI, /* Kindle */
    STAGING_DJINNI, /* Char */
    STAGING_DJINNI, /* Coal */
    STAGING_DJINNI, /* Reflux */
    STAGING_DJINNI, /* Core */
    STAGING_DJINNI, /* Tinder */
    STAGING_DJINNI, /* Shine */
    STAGING_DJINNI, /* Fury */
    STAGING_DJINNI, /* Fugue */
    0, /* Fire18 */
    0, /* Fire19 */
    STAGING_DJINNI, /* Gust */
    STAGING_DJINNI, /* Breeze */
    STAGING_DJINNI, /* Zephyr */
    STAGING_ATTACK, /* Smog */
    STAGING_DJINNI, /* Kite */
    STAGING_ATTACK, /* Squall */
    STAGING_DJINNI, /* Luff */
    STAGING_DJINNI, /* Breath */
    STAGING_ATTACK, /* Blitz */
    STAGING_DJINNI, /* Ether */
    STAGING_DJINNI, /* Waft */
    STAGING_DJINNI, /* Haze */
    STAGING_ATTACK, /* Wheeze */
    STAGING_DJINNI, /* Aroma */
    STAGING_DJINNI, /* Whorl */
    STAGING_DJINNI, /* Gasp */
    STAGING_DJINNI, /* Lull */
    STAGING_DJINNI, /* Gale */
    0, /* Wind18 */
    0, /* Wind19 */
    STAGING_CAST, /* Venus */
    STAGING_SUMMON, /* Ramses */
    STAGING_SUMMON, /* Cybele */
    STAGING_SUMMON, /* Judgment */
    STAGING_FULL_SCREEN, /* Zagan */
    STAGING_FULL_SCREEN, /* Haures */
    STAGING_FULL_SCREEN, /* Charon */
    0, /* ? */
    STAGING_CAST, /* Mercury */
    STAGING_SUMMON, /* Nereid */
    STAGING_SUMMON, /* Neptune */
    STAGING_SUMMON, /* Boreas */
    STAGING_SUMMON, /* Moloch */
    STAGING_FULL_SCREEN, /* Coatlicue */
    STAGING_SUMMON, /* Azul */
    0, /* ? */
    STAGING_CAST, /* Mars */
    STAGING_SUMMON, /* Kirin */
    STAGING_SUMMON, /* Tiamat */
    STAGING_SUMMON, /* Meteor */
    STAGING_MEGAERA, /* Megaera */
    STAGING_SUMMON, /* Ulysses */
    STAGING_FULL_SCREEN, /* Daedalus */
    STAGING_SUMMON, /* Daedalus */
    STAGING_IRIS, /* Iris */
    0, /* ? */
    STAGING_CAST, /* Jupiter */
    STAGING_SUMMON, /* Atalanta */
    STAGING_FULL_SCREEN, /* Procne */
    STAGING_SUMMON, /* Thor */
    STAGING_FULL_SCREEN, /* Flora */
    STAGING_FULL_SCREEN, /* Eclipse */
    STAGING_FULL_SCREEN, /* Catastrophe */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* Fire Breath */
    0, /* Fire Breath */
    0, /* Fire Breath */
    0, /* Water Breath */
    0, /* Water Breath */
    0, /* Water Breath */
    0, /* Ice Breath */
    0, /* Ice Breath */
    0, /* Ice Breath */
    0, /* Dark Breath */
    0, /* Dark Breath */
    0, /* Acid Breath */
    0, /* Acid Breath */
    0, /* Storm Breath */
    0, /* Storm Breath */
    0, /* Sonic Wave */
    0, /* Sonic Wave */
    0, /* Shriek */
    0, /* Banshee Howl */
    0, /* Crazy Voice */
    0, /* War Cry */
    0, /* Wicked Howl */
    0, /* Wing Beat */
    0, /* Wing Flutter */
    0, /* Wing Stroke */
    0, /* Evil Blessing */
    0, /* Deadly Gas */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* Rumble */
    0, /* Bone Chiller */
    STAGING_ENEMY_CAST, /* Slice */
    0, /* Bone Charge */
    0, /* Mystic Flame */
    0, /* Numbing Sting */
    0, /* Brute Force */
    0, /* Sticky Goo */
    0, /* Cannibal Fang */
    0, /* Bear Claw */
    0, /* Poisonous Bite */
    0, /* Flying Attack */
    0, /* Undead Sword */
    0, /* Ransack */
    0, /* Sticky Poison */
    0, /* Poison Fang */
    0, /* Electric Bite */
    0, /* Poison Tail */
    0, /* Onslaught */
    0, /* Vampiric Fang */
    0, /* Bacteria Rush */
    0, /* Swift Strike */
    0, /* Rotten Blood */
    STAGING_ATTACK, /* Forcible Arm */
    0, /* Double Fang */
    STAGING_ATTACK, /* Mortal Blow */
    0, /* Freebite Rush */
    0, /* Twin Beaks */
    0, /* Rabid Fang */
    0, /* Acid Bite */
    STAGING_ATTACK, /* Dynamite */
    0, /* Headbutt */
    0, /* Poison Ink */
    STAGING_ATTACK, /* Truncheon Fist */
    0, /* Counterstrike */
    0, /* Mad Dash */
    0, /* Soothing Star */
    0, /* Spider Web */
    0, /* Heartrender */
    0, /* Mad Spatter */
    0, /* Spasm */
    0, /* Sleep Star */
    0, /* Decompose */
    0, /* Haunting */
    0, /* Worms */
    0, /* Berserk */
    0, /* Lucid Prophecy */
    0, /* Recovery */
    0, /* Flee */
    0, /* Contain */
    0, /* Threaten */
    0, /* Tremble */
    0, /* Fortify */
    0, /* Speed Surge */
    0, /* Ally Search */
    0, /* Sidestep */
    0, /* Total Defense */
    0, /* Stand Ready */
    0, /* Battle Cry */
    0, /* Can't Use */
    0, /* Poison Beat */
    0, /* Spinning Beat */
    STAGING_ATTACK, /* Heat Flash */
    STAGING_ATTACK, /* Death Scythe */
    0, /* Outer Space */
    0, /* Dragon Driver */
    0, /* Drain Fang */
    0, /* Severe Blow */
    0, /* Thrash */
    0, /* ? */
    STAGING_ENEMY_CAST, /* Stone Justice */
    STAGING_ATTACK, /* Sarcophagus */
    STAGING_ATTACK, /* Evil Eye */
    STAGING_ATTACK, /* Vein Tap */
    STAGING_ENEMY_CAST, /* Heavy Divide */
    STAGING_ATTACK, /* Hammersphere */
    STAGING_ATTACK, /* Mother Earth */
    STAGING_ENEMY_CAST, /* Wyrd Curse */
    STAGING_ENEMY_CAST, /* Heartbreak */
    STAGING_ENEMY_CAST, /* Vengeance */
    STAGING_ENEMY_CAST, /* Acheron's Grief */
    STAGING_DJINNI, /* Megiddo */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    STAGING_ENEMY_CAST, /* Hurricane */
    STAGING_ENEMY_CAST, /* Dreamtide */
    STAGING_ATTACK, /* Stun Cloud */
    STAGING_ATTACK, /* Searing Fog */
    STAGING_ATTACK, /* Ice Crush */
    STAGING_ATTACK, /* Flash Force */
    STAGING_ATTACK, /* Sargasso */
    STAGING_ENEMY_CAST, /* Lethe Albion */
    STAGING_ATTACK, /* Reverse Star */
    STAGING_ENEMY_CAST, /* Rising Dragon */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    STAGING_ATTACK, /* Power Drive */
    STAGING_ENEMY_CAST, /* Boost Hack */
    STAGING_ATTACK, /* Heat Smash */
    STAGING_ENEMY_CAST, /* Fire Dance */
    STAGING_ENEMY_CAST, /* Blaze Rush */
    STAGING_ATTACK, /* Flare Burst */
    STAGING_ENEMY_CAST, /* Shred */
    STAGING_ATTACK, /* Crucible Fire */
    STAGING_ATTACK, /* Scorpionfish */
    STAGING_ENEMY_CAST, /* Purgatory */
    STAGING_ENEMY_CAST, /* Soul Shatter */
    STAGING_ENEMY_CAST, /* Radiant Fire */
    STAGING_ENEMY_CAST, /* Life Shear */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    STAGING_ATTACK, /* Moon Air */
    STAGING_ATTACK, /* Flash Edge */
    STAGING_ENEMY_CAST, /* Aging Gas */
    STAGING_ENEMY_CAST, /* Mad Zephyr */
    STAGING_ENEMY_CAST, /* Lunar Slash */
    STAGING_ENEMY_CAST, /* Nirvana */
    STAGING_ATTACK, /* High Vitals */
    STAGING_ATTACK, /* Stun Bolt */
    STAGING_FULL_SCREEN, /* Trident */
    STAGING_ENEMY_CAST, /* Light Surge */
    STAGING_ATTACK, /* Raiden's Wrath */
    STAGING_ENEMY_CAST, /* Void Beam */
    STAGING_ATTACK, /* Supernova */
    STAGING_ATTACK, /* Apocalypse */
    STAGING_ENEMY_CAST, /* Legend */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* Sabre Dance */
    0, /* Backstab */
    0, /* Fire Breath */
    0, /* Juggle */
    0, /* Heat Juggle */
    0, /* Fiery Juggle */
    0, /* Flame Card */
    0, /* Thunder Card */
    0, /* Bramble Card */
    0, /* Frost Card */
    0, /* Baffle Card */
    0, /* Sword Card */
    0, /* Sleep Card */
    0, /* Death Card */
    0, /* ? */
    0, /* Whiplash */
    STAGING_BEAST_ATTACK, /* Wild Wolf */
    STAGING_BEAST_CAST, /* Emu */
    STAGING_BEAST_ATTACK, /* Roc */
    STAGING_BEAST_CAST, /* Salamander */
    STAGING_BEAST_ATTACK, /* Orc */
    STAGING_BEAST_CAST, /* Harpy */
    STAGING_BEAST_ATTACK, /* Grand Golem */
    STAGING_BEAST_CAST, /* Cerberus */
    STAGING_BEAST_CAST, /* Wyvern */
    STAGING_BEAST_CAST, /* Pixie */
    STAGING_BEAST_ATTACK, /* Dinox */
    STAGING_BEAST_CAST, /* Gryphon */
    STAGING_BEAST_CAST, /* Living Armor */
    STAGING_BEAST_CAST, /* Chimera */
    STAGING_BEAST_CAST, /* Blue Dragon */
    STAGING_BEAST_CAST, /* Faery */
    STAGING_BEAST_CAST, /* Elder Wood */
    STAGING_BEAST_CAST, /* Lich */
    STAGING_BEAST_CAST, /* Troll */
    STAGING_BEAST_CAST, /* Minotaur */
    STAGING_BEAST_CAST, /* Ghost Soldier */
    STAGING_BEAST_CAST, /* Macetail */
    STAGING_BEAST_CAST, /* Fire Dragon */
    STAGING_BEAST_CAST, /* Weird Nymph */
    STAGING_BEAST_CAST, /* Succubus */
    STAGING_BEAST_CAST, /* Estre Wood */
    STAGING_BEAST_CAST, /* Manticore */
    STAGING_BEAST_CAST, /* Phoenix */
    0, /* ? */
    STAGING_BEAST_ATTACK, /* Call Zombie */
    STAGING_BEAST_CAST, /* Call Demon */
    STAGING_BEAST_CAST, /* Call Dullahan */
    0, /* Raging Heat */
    0, /* Fiery Abyss */
    0, /* Dire Inferno */
    0, /* Poison Flow */
    0, /* Fire Puppet */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* Sand Breath */
    0, /* Desert Gasp */
    0, /* Black Ice */
    0, /* Toxic Breath */
    0, /* Typhoon Blow */
    0, /* Blast Breath */
    0, /* Gravel Blow */
    0, /* Darksol Gasp */
    0, /* Flame Breath */
    0, /* Aqua Breath */
    0, /* Fire Breath */
    0, /* Chill Breath */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    STAGING_ATTACK, /* Flash Punch */
    0, /* Formic Acid */
    0, /* Claw Attack */
    0, /* Beat Dance */
    0, /* Twin Shear */
    0, /* Paralytail */
    0, /* Echo Cut */
    STAGING_DJINNI, /* Clarion Cry */
    0, /* Triple Chomp */
    0, /* Raging Flood */
    0, /* Rising Venom */
    0, /* Slaver */
    0, /* Mighty Press */
    0, /* Star Mine */
    0, /* Heat Stun */
    0, /* Ocean Fist */
    0, /* Watery Grave */
    0, /* Counter-rush */
    0, /* Counter */
    STAGING_ATTACK, /* Bosca Hit */
    0, /* Strong Hit */
    0, /* Rolling Flame */
    0, /* Rising Dragon */
    STAGING_FULL_SCREEN, /* Meteor Blow */
    0, /* Cage */
    0, /* Stun Muscle */
    0, /* Djinnfest */
    0, /* Heat Kiss */
    STAGING_ATTACK, /* Fatal Fang */
    0, /* Angle Spike */
    STAGING_ATTACK, /* Quick Slash */
    0, /* Doublestep */
    0, /* Maneater */
    0, /* Claw Slash */
    0, /* Mega Slash */
    0, /* Vanish Claw */
    0, /* Crusher Grip */
    0, /* Ur Flash */
    STAGING_ATTACK, /* Power Bite */
    STAGING_ATTACK, /* Terrible Bite */
    0, /* Poison Sting */
    0, /* Kill Sting */
    0, /* Beast Needle */
    0, /* Poison Sting */
    0, /* Stun Sting */
    0, /* Mucous Gel */
    0, /* Poison Gel */
    0, /* Hydro Slash */
    0, /* Regen Dance */
    0, /* Fire Dance */
    0, /* Power Crush */
    0, /* Sack */
    0, /* Vital Moon */
    0, /* Human Hunt */
    0, /* Demon Eye */
    STAGING_ATTACK, /* Stun Jip */
    0, /* Pike Assault */
    0, /* Formina Sage */
    0, /* True Collide */
    STAGING_ATTACK, /* Armor Crush */
    0, /* Djinn Blast */
    0, /* Djinn Storm */
    0, /* Earth Force */
    0, /* Guard Aura */
    STAGING_FULL_SCREEN, /* Cruel Ruin */
    STAGING_DJINNI, /* Mystic Call */
    0, /* Mine Ball */
    STAGING_ATTACK, /* Angry Mine */
    STAGING_DJINNI, /* Djinn Stun */
    0, /* Crucible */
    STAGING_DJINNI, /* Element Swap */
    0, /* Earnest Ply */
    STAGING_DJINNI, /* Psy Boost */
    0, /* Pressure */
};

/* By ability id, ABILITY_NOTHING to ABILITY_PSY_BOOST. */
const u32 AbilityBattleEffects[] = {
    0, /* Nothing */
    0, /* Attack */
    0, /* Defend */
    BATTLE_EFFECT_QUAKE, /* Quake */
    BATTLE_EFFECT_QUAKE | (1 << EFFECT_VARIANT_SHIFT), /* Earthquake */
    BATTLE_EFFECT_QUAKE | (2 << EFFECT_VARIANT_SHIFT), /* Quake Sphere */
    BATTLE_EFFECT_SPIRE, /* Spire */
    BATTLE_EFFECT_SPIRE | (1 << EFFECT_VARIANT_SHIFT), /* Clay Spire */
    BATTLE_EFFECT_SPIRE | (2 << EFFECT_VARIANT_SHIFT), /* Stone Spire */
    BATTLE_EFFECT_GAIA, /* Gaia */
    BATTLE_EFFECT_GAIA | (1 << EFFECT_VARIANT_SHIFT), /* Mother Gaia */
    BATTLE_EFFECT_GAIA | (2 << EFFECT_VARIANT_SHIFT), /* Grand Gaia */
    BATTLE_EFFECT_GROWTH, /* Growth */
    BATTLE_EFFECT_GROWTH | (1 << EFFECT_VARIANT_SHIFT), /* Mad Growth */
    BATTLE_EFFECT_GROWTH | (2 << EFFECT_VARIANT_SHIFT), /* Wild Growth */
    BATTLE_EFFECT_THORN, /* Thorn */
    BATTLE_EFFECT_THORN | (1 << EFFECT_VARIANT_SHIFT), /* Briar */
    BATTLE_EFFECT_THORN | (2 << EFFECT_VARIANT_SHIFT), /* Nettle */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    BATTLE_EFFECT_FROST, /* Frost */
    BATTLE_EFFECT_FROST | (1 << EFFECT_VARIANT_SHIFT), /* Tundra */
    BATTLE_EFFECT_FROST | (2 << EFFECT_VARIANT_SHIFT), /* Glacier */
    BATTLE_EFFECT_ICE, /* Ice */
    BATTLE_EFFECT_ICE | (1 << EFFECT_VARIANT_SHIFT), /* Ice Horn */
    BATTLE_EFFECT_ICE | (2 << EFFECT_VARIANT_SHIFT), /* Ice Missile */
    BATTLE_EFFECT_PRISM, /* Prism */
    BATTLE_EFFECT_PRISM | (1 << EFFECT_VARIANT_SHIFT), /* Hail Prism */
    BATTLE_EFFECT_PRISM | (2 << EFFECT_VARIANT_SHIFT), /* Freeze Prism */
    BATTLE_EFFECT_DOUSE, /* Douse */
    BATTLE_EFFECT_DOUSE | (1 << EFFECT_VARIANT_SHIFT), /* Drench */
    BATTLE_EFFECT_DOUSE | (2 << EFFECT_VARIANT_SHIFT), /* Deluge */
    BATTLE_EFFECT_FROTH, /* Froth */
    BATTLE_EFFECT_FROTH | (1 << EFFECT_VARIANT_SHIFT), /* Froth Sphere */
    BATTLE_EFFECT_FROTH | (2 << EFFECT_VARIANT_SHIFT), /* Froth Spiral */
    BATTLE_EFFECT_COOL, /* Cool */
    BATTLE_EFFECT_COOL | (1 << EFFECT_VARIANT_SHIFT), /* Supercool */
    BATTLE_EFFECT_COOL | (2 << EFFECT_VARIANT_SHIFT), /* Megacool */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    BATTLE_EFFECT_FLARE, /* Flare */
    BATTLE_EFFECT_FLARE | (1 << EFFECT_VARIANT_SHIFT), /* Flare Wall */
    BATTLE_EFFECT_FLARE | (2 << EFFECT_VARIANT_SHIFT), /* Flare Storm */
    BATTLE_EFFECT_FIRE, /* Fire */
    BATTLE_EFFECT_FIRE | (1 << EFFECT_VARIANT_SHIFT), /* Fireball */
    BATTLE_EFFECT_FIRE | (2 << EFFECT_VARIANT_SHIFT), /* Inferno */
    BATTLE_EFFECT_VOLCANO, /* Volcano */
    BATTLE_EFFECT_VOLCANO | (1 << EFFECT_VARIANT_SHIFT), /* Eruption */
    BATTLE_EFFECT_VOLCANO | (2 << EFFECT_VARIANT_SHIFT), /* Pyroclasm */
    BATTLE_EFFECT_BLAST, /* Blast */
    BATTLE_EFFECT_BLAST | (1 << EFFECT_VARIANT_SHIFT), /* Mad Blast */
    BATTLE_EFFECT_BLAST | (2 << EFFECT_VARIANT_SHIFT), /* Fiery Blast */
    BATTLE_EFFECT_NOVA, /* Blast */
    BATTLE_EFFECT_NOVA | (1 << EFFECT_VARIANT_SHIFT), /* Nova */
    BATTLE_EFFECT_NOVA | (2 << EFFECT_VARIANT_SHIFT), /* Supernova */
    BATTLE_EFFECT_FUME, /* Fume */
    BATTLE_EFFECT_FUME | (1 << EFFECT_VARIANT_SHIFT), /* Serpent Fume */
    BATTLE_EFFECT_FUME | (2 << EFFECT_VARIANT_SHIFT), /* Dragon Fume */
    BATTLE_EFFECT_BEAM, /* Beam */
    BATTLE_EFFECT_BEAM | (1 << EFFECT_VARIANT_SHIFT), /* Cycle Beam */
    BATTLE_EFFECT_BEAM | (2 << EFFECT_VARIANT_SHIFT), /* Searing Beam */
    BATTLE_EFFECT_BOLT, /* Bolt */
    BATTLE_EFFECT_BOLT | (1 << EFFECT_VARIANT_SHIFT), /* Flash Bolt */
    BATTLE_EFFECT_BOLT | (2 << EFFECT_VARIANT_SHIFT), /* Blue Bolt */
    BATTLE_EFFECT_RAY, /* Ray */
    BATTLE_EFFECT_RAY | (1 << EFFECT_VARIANT_SHIFT), /* Storm Ray */
    BATTLE_EFFECT_RAY | (2 << EFFECT_VARIANT_SHIFT), /* Destruct Ray */
    BATTLE_EFFECT_PLASMA, /* Plasma */
    BATTLE_EFFECT_PLASMA | (1 << EFFECT_VARIANT_SHIFT), /* Shine Plasma */
    BATTLE_EFFECT_PLASMA | (2 << EFFECT_VARIANT_SHIFT), /* Spark Plasma */
    BATTLE_EFFECT_SLASH, /* Slash */
    BATTLE_EFFECT_SLASH | (1 << EFFECT_VARIANT_SHIFT), /* Wind Slash */
    BATTLE_EFFECT_SLASH | (2 << EFFECT_VARIANT_SHIFT), /* Sonic Slash */
    BATTLE_EFFECT_WHIRLWIND, /* Whirlwind */
    BATTLE_EFFECT_WHIRLWIND | (1 << EFFECT_VARIANT_SHIFT), /* Tornado */
    BATTLE_EFFECT_WHIRLWIND | (2 << EFFECT_VARIANT_SHIFT), /* Tempest */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    BATTLE_EFFECT_AURA, /* Aura */
    BATTLE_EFFECT_AURA | (1 << EFFECT_VARIANT_SHIFT), /* Healing Aura */
    BATTLE_EFFECT_AURA | (2 << EFFECT_VARIANT_SHIFT), /* Cool Aura */
    BATTLE_EFFECT_CURE, /* Cure */
    BATTLE_EFFECT_CURE | (1 << EFFECT_VARIANT_SHIFT), /* Cure Well */
    BATTLE_EFFECT_CURE | (2 << EFFECT_VARIANT_SHIFT), /* Potent Cure */
    BATTLE_EFFECT_PLY, /* Ply */
    BATTLE_EFFECT_PLY | (1 << EFFECT_VARIANT_SHIFT), /* Ply Well */
    BATTLE_EFFECT_PLY | (2 << EFFECT_VARIANT_SHIFT), /* Pure Ply */
    BATTLE_EFFECT_WISH, /* Wish */
    BATTLE_EFFECT_WISH | (1 << EFFECT_VARIANT_SHIFT), /* Wish Well */
    BATTLE_EFFECT_WISH | (2 << EFFECT_VARIANT_SHIFT), /* Pure Wish */
    BATTLE_EFFECT_CURE_POISON, /* Cure Poison */
    BATTLE_EFFECT_CURE_POISON | (1 << EFFECT_VARIANT_SHIFT), /* Restore */
    BATTLE_EFFECT_REVIVE, /* Revive */
    BATTLE_EFFECT_IMPACT, /* Impact */
    BATTLE_EFFECT_IMPACT | (1 << EFFECT_VARIANT_SHIFT), /* High Impact */
    BATTLE_EFFECT_DULL, /* Dull */
    BATTLE_EFFECT_DULL | (1 << EFFECT_VARIANT_SHIFT), /* Blunt */
    BATTLE_EFFECT_GUARD, /* Guard */
    BATTLE_EFFECT_GUARD | (1 << EFFECT_VARIANT_SHIFT), /* Protect */
    BATTLE_EFFECT_IMPAIR, /* Impair */
    BATTLE_EFFECT_IMPAIR | (1 << EFFECT_VARIANT_SHIFT), /* Debilitate */
    BATTLE_EFFECT_WARD, /* Ward */
    BATTLE_EFFECT_WARD | (1 << EFFECT_VARIANT_SHIFT), /* Resist */
    BATTLE_EFFECT_WEAKEN, /* Weaken */
    BATTLE_EFFECT_WEAKEN | (1 << EFFECT_VARIANT_SHIFT), /* Enfeeble */
    BATTLE_EFFECT_TAINT, /* Taint */
    BATTLE_EFFECT_TAINT, /* Poison */
    BATTLE_EFFECT_DELUDE, /* Delude */
    BATTLE_EFFECT_DELUDE | (1 << EFFECT_VARIANT_SHIFT), /* Confuse */
    0, /* Charm */
    BATTLE_EFFECT_PARALYZE, /* Paralyze */
    BATTLE_EFFECT_SLEEP, /* Sleep */
    BATTLE_EFFECT_BIND, /* Bind */
    BATTLE_EFFECT_HAUNT, /* Haunt */
    BATTLE_EFFECT_CURSE, /* Curse */
    BATTLE_EFFECT_CONDEMN, /* Condemn */
    BATTLE_EFFECT_DRAIN, /* Drain */
    BATTLE_EFFECT_DRAIN | (1 << EFFECT_VARIANT_SHIFT), /* Psy Drain */
    BATTLE_EFFECT_BREAK, /* Break */
    BATTLE_EFFECT_REGENERATE, /* Regenerate */
    BATTLE_EFFECT_REFLECT, /* Reflect */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* Lash */
    0, /* Pound */
    0, /* Tremor */
    0, /* Scoop */
    0, /* Cyclone */
    0, /* Parch */
    0, /* Sand */
    0, /* Move */
    0, /* Mind Read */
    0, /* Force */
    0, /* Lift */
    0, /* Reveal */
    0, /* Halt */
    0, /* Cloak */
    0, /* Carry */
    0, /* Catch */
    0, /* Retreat */
    0, /* Avoid */
    0, /* Burst */
    0, /* Grind */
    0, /* Hover */
    0, /* Blaze */
    0, /* Ma???? */
    0, /* Teleport */
    0, /* A?? */
    0, /* ? */
    0, /* ? */
    BATTLE_EFFECT_DRAGON_CLOUD, /* Dragon Cloud */
    BATTLE_EFFECT_DEMON_NIGHT, /* Demon Night */
    BATTLE_EFFECT_HELM_SPLITTER, /* Helm Splitter */
    BATTLE_EFFECT_QUICK_STRIKE, /* Quick Strike */
    BATTLE_EFFECT_GAIA, /* Rockfall */
    BATTLE_EFFECT_GAIA | (1 << EFFECT_VARIANT_SHIFT), /* Rockslide */
    BATTLE_EFFECT_GAIA | (2 << EFFECT_VARIANT_SHIFT), /* Avalanche */
    BATTLE_EFFECT_VOLCANO, /* Lava Shower */
    BATTLE_EFFECT_VOLCANO | (1 << EFFECT_VARIANT_SHIFT), /* Molten Bath */
    BATTLE_EFFECT_VOLCANO | (2 << EFFECT_VARIANT_SHIFT), /* Magma Storm */
    BATTLE_EFFECT_IMPACT, /* Demon Spear */
    BATTLE_EFFECT_IMPACT | (1 << EFFECT_VARIANT_SHIFT), /* Angel Spear */
    BATTLE_EFFECT_GUARD, /* Guardian */
    BATTLE_EFFECT_GUARD, /* Protector */
    BATTLE_EFFECT_WARD, /* Magic Shell */
    BATTLE_EFFECT_WARD | (1 << EFFECT_VARIANT_SHIFT), /* Magic Shield */
    BATTLE_EFFECT_DEATH_PLUNGE, /* Death Plunge */
    BATTLE_EFFECT_SHURIKEN, /* Shuriken */
    BATTLE_EFFECT_ANNIHILATION, /* Annihilation */
    BATTLE_EFFECT_PUNJI, /* Punji */
    BATTLE_EFFECT_PUNJI | (1 << EFFECT_VARIANT_SHIFT), /* Punji Trap */
    BATTLE_EFFECT_PUNJI | (1 << EFFECT_VARIANT_SHIFT), /* Punji Strike */
    BATTLE_EFFECT_NOVA, /* Fire Bomb */
    BATTLE_EFFECT_NOVA | (1 << EFFECT_VARIANT_SHIFT), /* Cluster Bomb */
    BATTLE_EFFECT_NOVA | (2 << EFFECT_VARIANT_SHIFT), /* Carpet Bomb */
    BATTLE_EFFECT_WHIRLWIND, /* Gale */
    BATTLE_EFFECT_WHIRLWIND | (1 << EFFECT_VARIANT_SHIFT), /* Typhoon */
    BATTLE_EFFECT_WHIRLWIND | (2 << EFFECT_VARIANT_SHIFT), /* Hurricane */
    BATTLE_EFFECT_PLASMA, /* Thunderclap */
    BATTLE_EFFECT_PLASMA | (1 << EFFECT_VARIANT_SHIFT), /* Thunderbolt */
    BATTLE_EFFECT_PLASMA | (2 << EFFECT_VARIANT_SHIFT), /* Thunderhead */
    BATTLE_EFFECT_DELUDE, /* Mist */
    BATTLE_EFFECT_RAGNAROK, /* Ragnarok */
    BATTLE_EFFECT_CUTTING_EDGE, /* Cutting Edge */
    BATTLE_EFFECT_HEAT_WAVE, /* Heat Wave */
    BATTLE_EFFECT_ASTRAL_BLAST | (1 << EFFECT_VARIANT_SHIFT) | EFFECT_VIEW_REVERSED, /* Astral Blast */
    BATTLE_EFFECT_PLANET_DIVER, /* Planet Diver */
    BATTLE_EFFECT_DIAMOND_DUST, /* Diamond Dust */
    BATTLE_EFFECT_ODYSSEY | EFFECT_VIEW_REVERSED, /* Odyssey */
    BATTLE_EFFECT_LIQUIFIER | EFFECT_VIEW_REVERSED, /* Liquifier */
    BATTLE_EFFECT_PLUME_EDGE, /* Plume Edge */
    BATTLE_EFFECT_THUNDER_MINE, /* Thunder Mine */
    BATTLE_EFFECT_PLANETARY, /* Planetary */
    BATTLE_EFFECT_DIAMOND_BERG, /* Diamond Berg */
    BATTLE_EFFECT_DEATH_PLUNGE | (1 << EFFECT_VARIANT_SHIFT), /* Death Leap */
    BATTLE_EFFECT_DRAGON_CLOUD | (1 << EFFECT_VARIANT_SHIFT), /* Epicenter */
    BATTLE_EFFECT_THORNY_GRAVE, /* Thorny Grave */
    BATTLE_EFFECT_HELM_SPLITTER | (1 << EFFECT_VARIANT_SHIFT), /* Skull Splitter */
    0, /* ? */
    0, /* ? */
    138 | EFFECT_BY_ID_RANGE, /* Terra Strike */
    139 | EFFECT_BY_ID_RANGE, /* Poison Cloud */
    140 | EFFECT_BY_ID_RANGE, /* Poison Death */
    141 | EFFECT_BY_ID_RANGE, /* Mortal Danger */
    142 | EFFECT_BY_ID_RANGE, /* Bad Omen */
    143 | EFFECT_BY_ID_RANGE, /* Life Nourish */
    144 | EFFECT_BY_ID_RANGE, /* Aqua Sock */
    BATTLE_EFFECT_BLIZZARD, /* Blizzard */
    146 | EFFECT_BY_ID_RANGE, /* Frost Bite */
    BATTLE_EFFECT_DROWN, /* Drown */
    148 | EFFECT_BY_ID_RANGE, /* Life Leech */
    149 | EFFECT_BY_ID_RANGE, /* Psy Leech */
    150 | EFFECT_BY_ID_RANGE, /* Broil */
    151 | EFFECT_BY_ID_RANGE, /* Meltdown */
    152 | EFFECT_BY_ID_RANGE, /* Heat Mirage */
    153 | EFFECT_BY_ID_RANGE, /* Barrage */
    BATTLE_EFFECT_DEMON_FIRE, /* Demonfire */
    155 | EFFECT_BY_ID_RANGE, /* Acid Bath */
    BATTLE_EFFECT_VORPAL_SLASH, /* Vorpal Slash */
    157 | EFFECT_BY_ID_RANGE, /* Stun Voltage */
    158 | EFFECT_BY_ID_RANGE, /* Blinding Smog */
    159 | EFFECT_BY_ID_RANGE, /* Murk */
    160 | EFFECT_BY_ID_RANGE, /* Cyclone Slash */
    161 | EFFECT_BY_ID_RANGE, /* Psyphon Seal */
    BATTLE_EFFECT_RAPID_SMASH, /* Rapid Smash */
    BATTLE_EFFECT_RAPID_SMASH, /* Sonic Smash */
    162 | EFFECT_BY_ID_RANGE, /* Asura */
    BATTLE_EFFECT_TITAN_BLADE, /* Titan Blade */
    BATTLE_EFFECT_SHINING_STAR, /* Shining Star */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    BATTLE_EFFECT_PLY, /* Herb */
    BATTLE_EFFECT_PLY, /* Nut */
    BATTLE_EFFECT_PLY, /* Vial */
    BATTLE_EFFECT_PLY, /* Potion */
    BATTLE_EFFECT_PLY, /* Soothing Water */
    BATTLE_EFFECT_PSY_CRYSTAL, /* Psy Crystal */
    BATTLE_EFFECT_CURE_POISON, /* Antidote */
    BATTLE_EFFECT_CURE_POISON | (1 << EFFECT_VARIANT_SHIFT), /* Elixir */
    BATTLE_EFFECT_REVIVE, /* Water of Life */
    BATTLE_EFFECT_WISH | (2 << EFFECT_VARIANT_SHIFT), /* Mist Potion */
    0, /* Power Bread */
    0, /* Cookie */
    0, /* Apple */
    0, /* Hard Nut */
    0, /* Mint */
    0, /* Lucky Pepper */
    0, /* ? */
    BATTLE_EFFECT_SMOKE_BOMB, /* Smoke Bomb */
    BATTLE_EFFECT_SMOKE_BOMB, /* Sleep Bomb */
    BATTLE_EFFECT_PSY_CRYSTAL, /* Adept Ring */
    BATTLE_EFFECT_CURE, /* Corn */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* Aurora Field */
    BATTLE_EFFECT_DJINN_COUNTER, /* Djinn Counter */
    104 | EFFECT_BY_ID_RANGE, /* Flint */
    BATTLE_EFFECT_GRANITE, /* Granite */
    BATTLE_EFFECT_QUARTZ, /* Quartz */
    BATTLE_EFFECT_VINE, /* Vine */
    107 | EFFECT_BY_ID_RANGE, /* Sap */
    BATTLE_EFFECT_GROUND, /* Ground */
    100 | EFFECT_BY_ID_RANGE, /* Bane */
    108 | EFFECT_BY_ID_RANGE, /* Echo */
    BATTLE_EFFECT_IRON, /* Iron */
    112 | EFFECT_BY_ID_RANGE, /* Steel */
    BATTLE_EFFECT_MUD, /* Mud */
    BATTLE_EFFECT_FLOWER, /* Flower */
    BATTLE_EFFECT_MELD, /* Meld */
    BATTLE_EFFECT_PETRA, /* Petra */
    BATTLE_EFFECT_SALT, /* Salt */
    BATTLE_EFFECT_GEODE, /* Geode */
    BATTLE_EFFECT_MOLD, /* Mold */
    BATTLE_EFFECT_CRYSTAL, /* Crystal */
    0, /* Earth18 */
    0, /* Earth19 */
    BATTLE_EFFECT_FIZZ, /* Fizz */
    BATTLE_EFFECT_SLEET, /* Sleet */
    102 | EFFECT_BY_ID_RANGE, /* Mist */
    BATTLE_EFFECT_SPRITZ, /* Spritz */
    BATTLE_EFFECT_HAIL, /* Hail */
    BATTLE_EFFECT_TONIC, /* Tonic */
    BATTLE_EFFECT_DEW, /* Dew */
    113 | EFFECT_BY_ID_RANGE, /* Fog */
    109 | EFFECT_BY_ID_RANGE, /* Sour */
    BATTLE_EFFECT_SPRING, /* Spring */
    BATTLE_EFFECT_SHADE, /* Shade */
    110 | EFFECT_BY_ID_RANGE, /* Chill */
    BATTLE_EFFECT_STEAM, /* Steam */
    BATTLE_EFFECT_RIME, /* Rime */
    111 | EFFECT_BY_ID_RANGE, /* Gel */
    BATTLE_EFFECT_EDDY, /* Eddy */
    BATTLE_EFFECT_BALM, /* Balm */
    BATTLE_EFFECT_SERAC, /* Serac */
    0, /* Water18 */
    0, /* Water19 */
    BATTLE_EFFECT_FORGE, /* Forge */
    103 | EFFECT_BY_ID_RANGE, /* Fever */
    BATTLE_EFFECT_CORONA, /* Corona */
    105 | EFFECT_BY_ID_RANGE, /* Scorch */
    BATTLE_EFFECT_PSY_CRYSTAL, /* Ember */
    BATTLE_EFFECT_FLASH, /* Flash */
    BATTLE_EFFECT_TORCH, /* Torch */
    BATTLE_EFFECT_CANNON, /* Cannon */
    BATTLE_EFFECT_SPARK, /* Spark */
    BATTLE_EFFECT_KINDLE, /* Kindle */
    BATTLE_EFFECT_CHAR, /* Char */
    BATTLE_EFFECT_COAL, /* Coal */
    BATTLE_EFFECT_REFLUX, /* Reflux */
    BATTLE_EFFECT_CORE, /* Core */
    BATTLE_EFFECT_TINDER, /* Tinder */
    BATTLE_EFFECT_SHINE, /* Shine */
    BATTLE_EFFECT_FURY, /* Fury */
    BATTLE_EFFECT_FUGUE, /* Fugue */
    0, /* Fire18 */
    0, /* Fire19 */
    BATTLE_EFFECT_GUST, /* Gust */
    BATTLE_EFFECT_BREEZE, /* Breeze */
    BATTLE_EFFECT_ZEPHYR, /* Zephyr */
    101 | EFFECT_BY_ID_RANGE, /* Smog */
    BATTLE_EFFECT_KITE, /* Kite */
    106 | EFFECT_BY_ID_RANGE, /* Squall */
    BATTLE_EFFECT_LUFF, /* Luff */
    BATTLE_EFFECT_BREATH, /* Breath */
    117 | EFFECT_BY_ID_RANGE, /* Blitz */
    BATTLE_EFFECT_ETHER, /* Ether */
    BATTLE_EFFECT_WAFT, /* Waft */
    BATTLE_EFFECT_HAZE, /* Haze */
    116 | EFFECT_BY_ID_RANGE, /* Wheeze */
    BATTLE_EFFECT_AROMA, /* Aroma */
    BATTLE_EFFECT_WHORL, /* Whorl */
    BATTLE_EFFECT_GASP, /* Gasp */
    BATTLE_EFFECT_LULL, /* Lull */
    BATTLE_EFFECT_GALE, /* Gale */
    0, /* Wind18 */
    0, /* Wind19 */
    BATTLE_EFFECT_VENUS | EFFECT_VIEW_ACROSS, /* Venus */
    SUMMON_EFFECT_RAMSES | EFFECT_SUMMON, /* Ramses */
    SUMMON_EFFECT_CYBELE | EFFECT_SUMMON, /* Cybele */
    SUMMON_EFFECT_JUDGMENT | EFFECT_SUMMON, /* Judgment */
    SUMMON_EFFECT_ZAGAN | EFFECT_SUMMON, /* Zagan */
    SUMMON_EFFECT_HAURES | EFFECT_SUMMON, /* Haures */
    SUMMON_EFFECT_CHARON | EFFECT_SUMMON, /* Charon */
    0, /* ? */
    BATTLE_EFFECT_MERCURY | EFFECT_VIEW_ACROSS, /* Mercury */
    SUMMON_EFFECT_NEREID | EFFECT_SUMMON, /* Nereid */
    SUMMON_EFFECT_NEPTUNE | EFFECT_SUMMON, /* Neptune */
    SUMMON_EFFECT_BOREAS | EFFECT_SUMMON, /* Boreas */
    SUMMON_EFFECT_MOLOCH | EFFECT_SUMMON, /* Moloch */
    SUMMON_EFFECT_COATLICUE | EFFECT_SUMMON, /* Coatlicue */
    SUMMON_EFFECT_AZUL | EFFECT_SUMMON, /* Azul */
    0, /* ? */
    BATTLE_EFFECT_MARS | EFFECT_VIEW_ACROSS, /* Mars */
    SUMMON_EFFECT_KIRIN | EFFECT_SUMMON, /* Kirin */
    SUMMON_EFFECT_TIAMAT | EFFECT_SUMMON, /* Tiamat */
    SUMMON_EFFECT_METEOR | EFFECT_SUMMON, /* Meteor */
    SUMMON_EFFECT_MEGAERA | EFFECT_SUMMON, /* Megaera */
    SUMMON_EFFECT_ULYSSES | EFFECT_SUMMON, /* Ulysses */
    SUMMON_EFFECT_DAEDALUS | EFFECT_SUMMON, /* Daedalus */
    SUMMON_EFFECT_DAEDALUS_STRIKE | EFFECT_SUMMON, /* Daedalus */
    SUMMON_EFFECT_IRIS | EFFECT_SUMMON, /* Iris */
    0, /* ? */
    BATTLE_EFFECT_JUPITER | EFFECT_VIEW_ACROSS, /* Jupiter */
    SUMMON_EFFECT_ATALANTA | EFFECT_SUMMON, /* Atalanta */
    SUMMON_EFFECT_PROCNE | EFFECT_SUMMON, /* Procne */
    SUMMON_EFFECT_THOR | EFFECT_SUMMON, /* Thor */
    SUMMON_EFFECT_FLORA | EFFECT_SUMMON, /* Flora */
    SUMMON_EFFECT_ECLIPSE | EFFECT_SUMMON, /* Eclipse */
    SUMMON_EFFECT_CATASTROPHE | EFFECT_SUMMON, /* Catastrophe */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    BATTLE_EFFECT_FIRE_BREATH | EFFECT_VIEW_REVERSED, /* Fire Breath */
    BATTLE_EFFECT_FIRE_BREATH | EFFECT_VIEW_REVERSED, /* Fire Breath */
    BATTLE_EFFECT_FIRE_BREATH | EFFECT_VIEW_REVERSED, /* Fire Breath */
    BATTLE_EFFECT_WATER_BREATH | EFFECT_VIEW_REVERSED, /* Water Breath */
    BATTLE_EFFECT_WATER_BREATH | EFFECT_VIEW_REVERSED, /* Water Breath */
    BATTLE_EFFECT_WATER_BREATH | EFFECT_VIEW_REVERSED, /* Water Breath */
    BATTLE_EFFECT_ICE_BLESSING, /* Ice Breath */
    BATTLE_EFFECT_ICE_BLESSING, /* Ice Breath */
    BATTLE_EFFECT_ICE_BLESSING, /* Ice Breath */
    BATTLE_EFFECT_DARK_BREATH | EFFECT_VIEW_REVERSED, /* Dark Breath */
    BATTLE_EFFECT_DARK_BREATH | EFFECT_VIEW_REVERSED, /* Dark Breath */
    BATTLE_EFFECT_ACID_BREATH | EFFECT_VIEW_REVERSED, /* Acid Breath */
    BATTLE_EFFECT_ACID_BREATH | EFFECT_VIEW_REVERSED, /* Acid Breath */
    BATTLE_EFFECT_STORM_BREATH | EFFECT_VIEW_REVERSED, /* Storm Breath */
    BATTLE_EFFECT_STORM_BREATH_STRONG | EFFECT_VIEW_REVERSED, /* Storm Breath */
    BATTLE_EFFECT_SONIC_WAVE, /* Sonic Wave */
    BATTLE_EFFECT_SONIC_WAVE, /* Sonic Wave */
    BATTLE_EFFECT_SHRIEK, /* Shriek */
    BATTLE_EFFECT_BANSHEE_HOWL, /* Banshee Howl */
    BATTLE_EFFECT_CRAZY_VOICE, /* Crazy Voice */
    BATTLE_EFFECT_WAR_CRY, /* War Cry */
    BATTLE_EFFECT_WICKED_HOWL, /* Wicked Howl */
    BATTLE_EFFECT_WING_BEAT, /* Wing Beat */
    BATTLE_EFFECT_WING_FLUTTER, /* Wing Flutter */
    BATTLE_EFFECT_WING_STROKE, /* Wing Stroke */
    BATTLE_EFFECT_EVIL_BLESSING | EFFECT_VIEW_REVERSED, /* Evil Blessing */
    BATTLE_EFFECT_DEADLY_GAS | EFFECT_VIEW_REVERSED, /* Deadly Gas */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    BATTLE_EFFECT_RUMBLE, /* Rumble */
    BATTLE_EFFECT_BONE_CHILLER, /* Bone Chiller */
    BATTLE_EFFECT_RAPID_SMASH, /* Slice */
    BATTLE_EFFECT_BONE_CHARGE, /* Bone Charge */
    BATTLE_EFFECT_MYSTIC_FLAME, /* Mystic Flame */
    BATTLE_EFFECT_NUMBING_STING, /* Numbing Sting */
    BATTLE_EFFECT_BRUTE_FORCE, /* Brute Force */
    BATTLE_EFFECT_STICKY_GOO, /* Sticky Goo */
    BATTLE_EFFECT_CANNIBAL_FANG, /* Cannibal Fang */
    BATTLE_EFFECT_BEAR_CLAW, /* Bear Claw */
    BATTLE_EFFECT_POISONOUS_BITE, /* Poisonous Bite */
    BATTLE_EFFECT_FLYING_ATTACK, /* Flying Attack */
    BATTLE_EFFECT_UNDEAD_SWORD, /* Undead Sword */
    BATTLE_EFFECT_RANSACK, /* Ransack */
    BATTLE_EFFECT_STICKY_POISON, /* Sticky Poison */
    BATTLE_EFFECT_POISON_FANG, /* Poison Fang */
    BATTLE_EFFECT_ELECTRIC_BITE, /* Electric Bite */
    BATTLE_EFFECT_POISON_TAIL, /* Poison Tail */
    BATTLE_EFFECT_ONSLAUGHT, /* Onslaught */
    BATTLE_EFFECT_VAMPIRIC_FANG, /* Vampiric Fang */
    BATTLE_EFFECT_BACTERIA_RUSH, /* Bacteria Rush */
    BATTLE_EFFECT_SWIFT_STRIKE, /* Swift Strike */
    BATTLE_EFFECT_ROTTEN_BLOOD, /* Rotten Blood */
    163 | EFFECT_BY_ID_RANGE, /* Forcible Arm */
    BATTLE_EFFECT_DOUBLE_FANG, /* Double Fang */
    BATTLE_EFFECT_MORTAL_BLOW, /* Mortal Blow */
    BATTLE_EFFECT_FREEBITE_RUSH, /* Freebite Rush */
    BATTLE_EFFECT_TWIN_BEAKS, /* Twin Beaks */
    BATTLE_EFFECT_RABID_FANG, /* Rabid Fang */
    BATTLE_EFFECT_ACID_BITE, /* Acid Bite */
    BATTLE_EFFECT_DYNAMITE, /* Dynamite */
    BATTLE_EFFECT_HEADBUTT, /* Headbutt */
    BATTLE_EFFECT_POISON_INK, /* Poison Ink */
    164 | EFFECT_BY_ID_RANGE, /* Truncheon Fist */
    BATTLE_EFFECT_COUNTERSTRIKE, /* Counterstrike */
    BATTLE_EFFECT_MAD_DASH, /* Mad Dash */
    BATTLE_EFFECT_SOOTHING_STAR, /* Soothing Star */
    BATTLE_EFFECT_SPIDER_WEB, /* Spider Web */
    BATTLE_EFFECT_HEARTRENDER, /* Heartrender */
    BATTLE_EFFECT_MAD_SPATTER, /* Mad Spatter */
    BATTLE_EFFECT_SPASM, /* Spasm */
    BATTLE_EFFECT_SLEEP_STAR, /* Sleep Star */
    BATTLE_EFFECT_DECOMPOSE, /* Decompose */
    BATTLE_EFFECT_HAUNTING, /* Haunting */
    BATTLE_EFFECT_WORMS, /* Worms */
    BATTLE_EFFECT_BERSERK, /* Berserk */
    BATTLE_EFFECT_LUCID_PROPHECY, /* Lucid Prophecy */
    BATTLE_EFFECT_RECOVERY, /* Recovery */
    0, /* Flee */
    0, /* Contain */
    0, /* Threaten */
    0, /* Tremble */
    0, /* Fortify */
    0, /* Speed Surge */
    0, /* Ally Search */
    0, /* Sidestep */
    0, /* Total Defense */
    0, /* Stand Ready */
    0, /* Battle Cry */
    0, /* Can't Use */
    BATTLE_EFFECT_POISON_BEAT, /* Poison Beat */
    BATTLE_EFFECT_SPINNING_BEAT, /* Spinning Beat */
    152 | EFFECT_BY_ID_RANGE, /* Heat Flash */
    141 | EFFECT_BY_ID_RANGE, /* Death Scythe */
    BATTLE_EFFECT_OUTER_SPACE | EFFECT_VIEW_REVERSED, /* Outer Space */
    BATTLE_EFFECT_DRAGON_DRIVER | EFFECT_VIEW_REVERSED, /* Dragon Driver */
    BATTLE_EFFECT_DRAIN_FANG, /* Drain Fang */
    BATTLE_EFFECT_SEVERE_BLOW, /* Severe Blow */
    BATTLE_EFFECT_THRASH, /* Thrash */
    0, /* ? */
    BATTLE_EFFECT_STONE_JUSTICE, /* Stone Justice */
    185 | EFFECT_BY_ID_RANGE, /* Sarcophagus */
    172 | EFFECT_BY_ID_RANGE, /* Evil Eye */
    173 | EFFECT_BY_ID_RANGE, /* Vein Tap */
    BATTLE_EFFECT_HEAVY_DIVIDE, /* Heavy Divide */
    179 | EFFECT_BY_ID_RANGE, /* Hammersphere */
    194 | EFFECT_BY_ID_RANGE, /* Mother Earth */
    BATTLE_EFFECT_WYRD_CURSE, /* Wyrd Curse */
    BATTLE_EFFECT_HEARTBREAK, /* Heartbreak */
    BATTLE_EFFECT_VENGEANCE, /* Vengeance */
    BATTLE_EFFECT_ACHERONS_GRIEF, /* Acheron's Grief */
    BATTLE_EFFECT_MEGIDDO, /* Megiddo */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    BATTLE_EFFECT_HURRICANE, /* Hurricane */
    BATTLE_EFFECT_DREAMTIDE, /* Dreamtide */
    168 | EFFECT_BY_ID_RANGE, /* Stun Cloud */
    169 | EFFECT_BY_ID_RANGE, /* Searing Fog */
    170 | EFFECT_BY_ID_RANGE, /* Ice Crush */
    180 | EFFECT_BY_ID_RANGE, /* Flash Force */
    171 | EFFECT_BY_ID_RANGE, /* Sargasso */
    BATTLE_EFFECT_LETHE_ALBION, /* Lethe Albion */
    182 | EFFECT_BY_ID_RANGE, /* Reverse Star */
    BATTLE_EFFECT_RISING_DRAGON, /* Rising Dragon */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    166 | EFFECT_BY_ID_RANGE, /* Power Drive */
    BATTLE_EFFECT_BOOST_HACK, /* Boost Hack */
    167 | EFFECT_BY_ID_RANGE, /* Heat Smash */
    BATTLE_EFFECT_FIRE_DANCE, /* Fire Dance */
    BATTLE_EFFECT_BLAZE_RUSH, /* Blaze Rush */
    178 | EFFECT_BY_ID_RANGE, /* Flare Burst */
    BATTLE_EFFECT_SHRED, /* Shred */
    181 | EFFECT_BY_ID_RANGE, /* Crucible Fire */
    174 | EFFECT_BY_ID_RANGE, /* Scorpionfish */
    BATTLE_EFFECT_PURGATORY, /* Purgatory */
    BATTLE_EFFECT_SOUL_SHATTER, /* Soul Shatter */
    BATTLE_EFFECT_RADIANT_FIRE, /* Radiant Fire */
    BATTLE_EFFECT_LIFE_SHEAR, /* Life Shear */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    175 | EFFECT_BY_ID_RANGE, /* Moon Air */
    176 | EFFECT_BY_ID_RANGE, /* Flash Edge */
    BATTLE_EFFECT_AGING_GAS, /* Aging Gas */
    BATTLE_EFFECT_MAD_ZEPHYR, /* Mad Zephyr */
    BATTLE_EFFECT_LUNAR_SLASH, /* Lunar Slash */
    BATTLE_EFFECT_NIRVANA, /* Nirvana */
    177 | EFFECT_BY_ID_RANGE, /* High Vitals */
    184 | EFFECT_BY_ID_RANGE, /* Stun Bolt */
    SUMMON_EFFECT_TRIDENT | EFFECT_SUMMON, /* Trident */
    BATTLE_EFFECT_LIGHT_SURGE, /* Light Surge */
    186 | EFFECT_BY_ID_RANGE, /* Raiden's Wrath */
    BATTLE_EFFECT_VOID_BEAM, /* Void Beam */
    183 | EFFECT_BY_ID_RANGE, /* Supernova */
    188 | EFFECT_BY_ID_RANGE, /* Apocalypse */
    BATTLE_EFFECT_LEGEND, /* Legend */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    BATTLE_EFFECT_SABRE_DANCE, /* Sabre Dance */
    BATTLE_EFFECT_BACKSTAB, /* Backstab */
    BATTLE_EFFECT_FIRE_BREATH, /* Fire Breath */
    BATTLE_EFFECT_JUGGLE, /* Juggle */
    BATTLE_EFFECT_JUGGLE | (1 << EFFECT_VARIANT_SHIFT), /* Heat Juggle */
    BATTLE_EFFECT_JUGGLE | (2 << EFFECT_VARIANT_SHIFT), /* Fiery Juggle */
    BATTLE_EFFECT_FLAME_CARD, /* Flame Card */
    BATTLE_EFFECT_THUNDER_CARD, /* Thunder Card */
    BATTLE_EFFECT_BRAMBLE_CARD, /* Bramble Card */
    BATTLE_EFFECT_FROST_CARD, /* Frost Card */
    BATTLE_EFFECT_BAFFLE_CARD, /* Baffle Card */
    BATTLE_EFFECT_SWORD_CARD, /* Sword Card */
    BATTLE_EFFECT_SLEEP_CARD, /* Sleep Card */
    BATTLE_EFFECT_DEATH_CARD, /* Death Card */
    0, /* ? */
    BATTLE_EFFECT_WHIPLASH, /* Whiplash */
    196 | EFFECT_BY_ID_RANGE, /* Wild Wolf */
    BATTLE_EFFECT_EMU | EFFECT_VIEW_REVERSED, /* Emu */
    195 | EFFECT_BY_ID_RANGE, /* Roc */
    BATTLE_EFFECT_FIRE_BREATH | EFFECT_VIEW_REVERSED, /* Salamander */
    195 | EFFECT_BY_ID_RANGE, /* Orc */
    BATTLE_EFFECT_SHRIEK, /* Harpy */
    163 | EFFECT_BY_ID_RANGE, /* Grand Golem */
    BATTLE_EFFECT_FIRE_BREATH | EFFECT_VIEW_REVERSED, /* Cerberus */
    BATTLE_EFFECT_FIRE_BREATH | EFFECT_VIEW_REVERSED, /* Wyvern */
    BATTLE_EFFECT_PLY, /* Pixie */
    197 | EFFECT_BY_ID_RANGE, /* Dinox */
    BATTLE_EFFECT_TWIN_BEAKS, /* Gryphon */
    BATTLE_EFFECT_LIVING_ARMOR, /* Living Armor */
    BATTLE_EFFECT_FIRE_BREATH | EFFECT_VIEW_REVERSED, /* Chimera */
    BATTLE_EFFECT_ICE_BLESSING | EFFECT_VIEW_REVERSED, /* Blue Dragon */
    BATTLE_EFFECT_PLY, /* Faery */
    BATTLE_EFFECT_WISH | (1 << EFFECT_VARIANT_SHIFT), /* Elder Wood */
    BATTLE_EFFECT_REVIVE, /* Lich */
    BATTLE_EFFECT_BRUTE_FORCE, /* Troll */
    BATTLE_EFFECT_LIVING_ARMOR, /* Minotaur */
    BATTLE_EFFECT_GHOST_SOLDIER | EFFECT_VIEW_REVERSED, /* Ghost Soldier */
    BATTLE_EFFECT_FIRE_BREATH | EFFECT_VIEW_REVERSED, /* Macetail */
    BATTLE_EFFECT_FIRE_BREATH | EFFECT_VIEW_REVERSED, /* Fire Dragon */
    BATTLE_EFFECT_PLY, /* Weird Nymph */
    BATTLE_EFFECT_PLY | (1 << EFFECT_VARIANT_SHIFT), /* Succubus */
    BATTLE_EFFECT_WISH | (1 << EFFECT_VARIANT_SHIFT), /* Estre Wood */
    BATTLE_EFFECT_AURA | (2 << EFFECT_VARIANT_SHIFT), /* Manticore */
    BATTLE_EFFECT_PHOENIX, /* Phoenix */
    0, /* ? */
    199 | EFFECT_BY_ID_RANGE, /* Call Zombie */
    BATTLE_EFFECT_EMU | (2 << EFFECT_VARIANT_SHIFT) | EFFECT_VIEW_REVERSED, /* Call Demon */
    BATTLE_EFFECT_CALL_DULLAHAN | EFFECT_VIEW_REVERSED, /* Call Dullahan */
    BATTLE_EFFECT_RAGING_HEAT, /* Raging Heat */
    BATTLE_EFFECT_RAGING_HEAT | (1 << EFFECT_VARIANT_SHIFT), /* Fiery Abyss */
    BATTLE_EFFECT_RAGING_HEAT | (2 << EFFECT_VARIANT_SHIFT), /* Dire Inferno */
    BATTLE_EFFECT_POISON_FLOW, /* Poison Flow */
    BATTLE_EFFECT_FIRE_PUPPET, /* Fire Puppet */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    BATTLE_EFFECT_SAND_BREATH | EFFECT_VIEW_REVERSED, /* Sand Breath */
    BATTLE_EFFECT_DESERT_GASP | EFFECT_VIEW_REVERSED, /* Desert Gasp */
    BATTLE_EFFECT_BLACK_ICE | EFFECT_VIEW_REVERSED, /* Black Ice */
    BATTLE_EFFECT_TOXIC_BREATH | EFFECT_VIEW_REVERSED, /* Toxic Breath */
    BATTLE_EFFECT_TYPHOON_BLOW | EFFECT_VIEW_REVERSED, /* Typhoon Blow */
    BATTLE_EFFECT_BLAST_BREATH | EFFECT_VIEW_REVERSED, /* Blast Breath */
    BATTLE_EFFECT_GRAVEL_BLOW | EFFECT_VIEW_REVERSED, /* Gravel Blow */
    BATTLE_EFFECT_DARKSOL_GASP | EFFECT_VIEW_REVERSED, /* Darksol Gasp */
    BATTLE_EFFECT_FLAME_BREATH | EFFECT_VIEW_REVERSED, /* Flame Breath */
    BATTLE_EFFECT_WATER_BREATH | EFFECT_VIEW_REVERSED, /* Aqua Breath */
    BATTLE_EFFECT_FLAME_BREATH | EFFECT_VIEW_REVERSED, /* Fire Breath */
    BATTLE_EFFECT_CHILL_BREATH, /* Chill Breath */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    0, /* ? */
    189 | EFFECT_BY_ID_RANGE, /* Flash Punch */
    BATTLE_EFFECT_FORMIC_ACID, /* Formic Acid */
    BATTLE_EFFECT_CLAW_ATTACK, /* Claw Attack */
    BATTLE_EFFECT_BEAT_DANCE, /* Beat Dance */
    BATTLE_EFFECT_TWIN_SHEAR, /* Twin Shear */
    BATTLE_EFFECT_NUMBING_STING, /* Paralytail */
    BATTLE_EFFECT_ECHO_CUT | EFFECT_VIEW_REVERSED, /* Echo Cut */
    BATTLE_EFFECT_CLARION_CRY, /* Clarion Cry */
    BATTLE_EFFECT_TRIPLE_CHOMP, /* Triple Chomp */
    BATTLE_EFFECT_RAGING_FLOOD, /* Raging Flood */
    BATTLE_EFFECT_RISING_VENOM, /* Rising Venom */
    BATTLE_EFFECT_SLAVER, /* Slaver */
    BATTLE_EFFECT_MIGHTY_PRESS, /* Mighty Press */
    BATTLE_EFFECT_STAR_MINE, /* Star Mine */
    BATTLE_EFFECT_HEAT_STUN, /* Heat Stun */
    BATTLE_EFFECT_OCEAN_FIST, /* Ocean Fist */
    BATTLE_EFFECT_WATERY_GRAVE, /* Watery Grave */
    BATTLE_EFFECT_COUNTER_RUSH, /* Counter-rush */
    0, /* Counter */
    193 | EFFECT_BY_ID_RANGE, /* Bosca Hit */
    BATTLE_EFFECT_STRONG_HIT, /* Strong Hit */
    BATTLE_EFFECT_ROLLING_FLAME | EFFECT_VIEW_REVERSED, /* Rolling Flame */
    BATTLE_EFFECT_RISING_DRAGON_ENEMY, /* Rising Dragon */
    SUMMON_EFFECT_METEOR_BLOW | EFFECT_SUMMON, /* Meteor Blow */
    BATTLE_EFFECT_CAGE, /* Cage */
    BATTLE_EFFECT_STUN_MUSCLE, /* Stun Muscle */
    BATTLE_EFFECT_DJINNFEST, /* Djinnfest */
    BATTLE_EFFECT_HEAT_KISS, /* Heat Kiss */
    196 | EFFECT_BY_ID_RANGE, /* Fatal Fang */
    BATTLE_EFFECT_ANGLE_SPIKE, /* Angle Spike */
    192 | EFFECT_BY_ID_RANGE, /* Quick Slash */
    BATTLE_EFFECT_DOUBLESTEP, /* Doublestep */
    BATTLE_EFFECT_MANEATER, /* Maneater */
    BATTLE_EFFECT_EMU | EFFECT_VIEW_REVERSED, /* Claw Slash */
    BATTLE_EFFECT_EMU | (1 << EFFECT_VARIANT_SHIFT) | EFFECT_VIEW_REVERSED, /* Mega Slash */
    BATTLE_EFFECT_VANISH_CLAW, /* Vanish Claw */
    BATTLE_EFFECT_CRUSHER_GRIP, /* Crusher Grip */
    BATTLE_EFFECT_UR_FLASH, /* Ur Flash */
    197 | EFFECT_BY_ID_RANGE, /* Power Bite */
    198 | EFFECT_BY_ID_RANGE, /* Terrible Bite */
    BATTLE_EFFECT_POISON_STING_TAIL, /* Poison Sting */
    BATTLE_EFFECT_KILL_STING, /* Kill Sting */
    BATTLE_EFFECT_BEAST_NEEDLE, /* Beast Needle */
    BATTLE_EFFECT_POISON_STING, /* Poison Sting */
    BATTLE_EFFECT_STUN_STING, /* Stun Sting */
    BATTLE_EFFECT_MUCOUS_GEL, /* Mucous Gel */
    BATTLE_EFFECT_POISON_GEL, /* Poison Gel */
    BATTLE_EFFECT_HYDRO_SLASH, /* Hydro Slash */
    BATTLE_EFFECT_PHOENIX, /* Regen Dance */
    BATTLE_EFFECT_FIRE_DANCE_UNLEASH | EFFECT_VIEW_REVERSED, /* Fire Dance */
    BATTLE_EFFECT_LIVING_ARMOR, /* Power Crush */
    BATTLE_EFFECT_SACK, /* Sack */
    BATTLE_EFFECT_EMU | (2 << EFFECT_VARIANT_SHIFT) | EFFECT_VIEW_REVERSED, /* Vital Moon */
    BATTLE_EFFECT_HUMAN_HUNT, /* Human Hunt */
    BATTLE_EFFECT_DEMON_EYE | EFFECT_VIEW_REVERSED, /* Demon Eye */
    191 | EFFECT_BY_ID_RANGE, /* Stun Jip */
    BATTLE_EFFECT_GHOST_SOLDIER | EFFECT_VIEW_REVERSED, /* Pike Assault */
    BATTLE_EFFECT_CALL_DULLAHAN | EFFECT_VIEW_REVERSED, /* Formina Sage */
    BATTLE_EFFECT_TRUE_COLLIDE, /* True Collide */
    190 | EFFECT_BY_ID_RANGE, /* Armor Crush */
    BATTLE_EFFECT_DJINN_BLAST, /* Djinn Blast */
    BATTLE_EFFECT_DJINN_STORM, /* Djinn Storm */
    BATTLE_EFFECT_EARTH_FORCE | EFFECT_VIEW_REVERSED, /* Earth Force */
    BATTLE_EFFECT_GUARD_AURA, /* Guard Aura */
    SUMMON_EFFECT_CRUEL_RUIN | EFFECT_SUMMON, /* Cruel Ruin */
    BATTLE_EFFECT_MYSTIC_CALL, /* Mystic Call */
    BATTLE_EFFECT_MINE_BALL, /* Mine Ball */
    BATTLE_EFFECT_DYNAMITE, /* Angry Mine */
    BATTLE_EFFECT_DJINN_STUN, /* Djinn Stun */
    0, /* Crucible */
    BATTLE_EFFECT_ELEMENT_SWAP, /* Element Swap */
    BATTLE_EFFECT_PLY | (2 << EFFECT_VARIANT_SHIFT), /* Earnest Ply */
    BATTLE_EFFECT_PSY_BOOST, /* Psy Boost */
};
/* clang-format on */
