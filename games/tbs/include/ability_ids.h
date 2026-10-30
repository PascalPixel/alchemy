#ifndef ALCHEMY_ABILITY_IDS_H
#define ALCHEMY_ABILITY_IDS_H

/// @file
/// Ability numbers, named from the English release.

/// Ability numbers, one for each row of gAbilityDefinitions (ability_table.c)
/// in order. Each ability's name is the message MsgAbilityName plus its
/// number. A row the code doesn't name yet keeps its number as its
/// placeholder.
enum AbilityId {
  ABILITY_000,
  /// ★ Named from the game: "Attack" (MsgAbilityName)
  ABILITY_ATTACK,
  /// ★ Named from the game: "Defend" (MsgAbilityName)
  ABILITY_DEFEND,
  /// ★ Named from the game: "Quake" (MsgAbilityName)
  ABILITY_QUAKE,
  /// ★ Named from the game: "Earthquake" (MsgAbilityName)
  ABILITY_EARTHQUAKE,
  /// ★ Named from the game: "Quake Sphere" (MsgAbilityName)
  ABILITY_QUAKE_SPHERE,
  /// ★ Named from the game: "Spire" (MsgAbilityName)
  ABILITY_SPIRE,
  /// ★ Named from the game: "Clay Spire" (MsgAbilityName)
  ABILITY_CLAY_SPIRE,
  /// ★ Named from the game: "Stone Spire" (MsgAbilityName)
  ABILITY_STONE_SPIRE,
  /// ★ Named from the game: "Gaia" (MsgAbilityName)
  ABILITY_GAIA,
  /// ★ Named from the game: "Mother Gaia" (MsgAbilityName)
  ABILITY_MOTHER_GAIA,
  /// ★ Named from the game: "Grand Gaia" (MsgAbilityName)
  ABILITY_GRAND_GAIA,
  /// ★ Named from the game: "Growth" (MsgAbilityName)
  ABILITY_GROWTH,
  /// ★ Named from the game: "Mad Growth" (MsgAbilityName)
  ABILITY_MAD_GROWTH,
  /// ★ Named from the game: "Wild Growth" (MsgAbilityName)
  ABILITY_WILD_GROWTH,
  /// ★ Named from the game: "Thorn" (MsgAbilityName)
  ABILITY_THORN,
  /// ★ Named from the game: "Briar" (MsgAbilityName)
  ABILITY_BRIAR,
  /// ★ Named from the game: "Nettle" (MsgAbilityName)
  ABILITY_NETTLE,
  ABILITY_018,
  ABILITY_019,
  ABILITY_020,
  ABILITY_021,
  ABILITY_022,
  ABILITY_023,
  /// ★ Named from the game: "Frost" (MsgAbilityName)
  ABILITY_FROST,
  /// ★ Named from the game: "Tundra" (MsgAbilityName)
  ABILITY_TUNDRA,
  /// ★ Named from the game: "Glacier" (MsgAbilityName)
  ABILITY_GLACIER,
  /// ★ Named from the game: "Ice" (MsgAbilityName)
  ABILITY_ICE,
  /// ★ Named from the game: "Ice Horn" (MsgAbilityName)
  ABILITY_ICE_HORN,
  /// ★ Named from the game: "Ice Missile" (MsgAbilityName)
  ABILITY_ICE_MISSILE,
  /// ★ Named from the game: "Prism" (MsgAbilityName)
  ABILITY_PRISM,
  /// ★ Named from the game: "Hail Prism" (MsgAbilityName)
  ABILITY_HAIL_PRISM,
  /// ★ Named from the game: "Freeze Prism" (MsgAbilityName)
  ABILITY_FREEZE_PRISM,
  /// ★ Named from the game: "Douse" (MsgAbilityName)
  ABILITY_DOUSE,
  /// ★ Named from the game: "Drench" (MsgAbilityName)
  ABILITY_DRENCH,
  /// ★ Named from the game: "Deluge" (MsgAbilityName)
  ABILITY_DELUGE,
  /// ★ Named from the game: "Froth" (MsgAbilityName)
  ABILITY_FROTH,
  /// ★ Named from the game: "Froth Sphere" (MsgAbilityName)
  ABILITY_FROTH_SPHERE,
  /// ★ Named from the game: "Froth Spiral" (MsgAbilityName)
  ABILITY_FROTH_SPIRAL,
  ABILITY_039,
  ABILITY_040,
  ABILITY_041,
  ABILITY_042,
  ABILITY_043,
  ABILITY_044,
  /// ★ Named from the game: "Flare" (MsgAbilityName)
  ABILITY_FLARE,
  /// ★ Named from the game: "Flare Wall" (MsgAbilityName)
  ABILITY_FLARE_WALL,
  /// ★ Named from the game: "Flare Storm" (MsgAbilityName)
  ABILITY_FLARE_STORM,
  /// ★ Named from the game: "Fire" (MsgAbilityName)
  ABILITY_FIRE,
  /// ★ Named from the game: "Fireball" (MsgAbilityName)
  ABILITY_FIREBALL,
  /// ★ Named from the game: "Inferno" (MsgAbilityName)
  ABILITY_INFERNO,
  /// ★ Named from the game: "Volcano" (MsgAbilityName)
  ABILITY_VOLCANO,
  /// ★ Named from the game: "Eruption" (MsgAbilityName)
  ABILITY_ERUPTION,
  /// ★ Named from the game: "Pyroclasm" (MsgAbilityName)
  ABILITY_PYROCLASM,
  /// Power 25, against ABILITY_BLAST_STRONG's 40.
  /// ★ Named from the game: "Blast" (MsgAbilityName)
  ABILITY_BLAST,
  /// ★ Named from the game: "Mad Blast" (MsgAbilityName)
  ABILITY_MAD_BLAST,
  /// ★ Named from the game: "Fiery Blast" (MsgAbilityName)
  ABILITY_FIERY_BLAST,
  /// Power 40, against ABILITY_BLAST's 25: the row before Nova and Supernova.
  /// ★ Named from the game: "Blast" (MsgAbilityName)
  ABILITY_BLAST_STRONG,
  /// ★ Named from the game: "Nova" (MsgAbilityName)
  ABILITY_NOVA,
  /// ★ Named from the game: "Supernova" (MsgAbilityName)
  ABILITY_SUPERNOVA,
  ABILITY_060,
  ABILITY_061,
  ABILITY_062,
  ABILITY_063,
  ABILITY_064,
  ABILITY_065,
  /// ★ Named from the game: "Bolt" (MsgAbilityName)
  ABILITY_BOLT,
  /// ★ Named from the game: "Flash Bolt" (MsgAbilityName)
  ABILITY_FLASH_BOLT,
  /// ★ Named from the game: "Blue Bolt" (MsgAbilityName)
  ABILITY_BLUE_BOLT,
  /// ★ Named from the game: "Ray" (MsgAbilityName)
  ABILITY_RAY,
  /// ★ Named from the game: "Storm Ray" (MsgAbilityName)
  ABILITY_STORM_RAY,
  /// ★ Named from the game: "Destruct Ray" (MsgAbilityName)
  ABILITY_DESTRUCT_RAY,
  /// ★ Named from the game: "Plasma" (MsgAbilityName)
  ABILITY_PLASMA,
  /// ★ Named from the game: "Shine Plasma" (MsgAbilityName)
  ABILITY_SHINE_PLASMA,
  /// ★ Named from the game: "Spark Plasma" (MsgAbilityName)
  ABILITY_SPARK_PLASMA,
  /// ★ Named from the game: "Slash" (MsgAbilityName)
  ABILITY_SLASH,
  /// ★ Named from the game: "Wind Slash" (MsgAbilityName)
  ABILITY_WIND_SLASH,
  /// ★ Named from the game: "Sonic Slash" (MsgAbilityName)
  ABILITY_SONIC_SLASH,
  /// ★ Named from the game: "Whirlwind" (MsgAbilityName)
  ABILITY_WHIRLWIND,
  /// ★ Named from the game: "Tornado" (MsgAbilityName)
  ABILITY_TORNADO,
  /// ★ Named from the game: "Tempest" (MsgAbilityName)
  ABILITY_TEMPEST,
  ABILITY_081,
  ABILITY_082,
  ABILITY_083,
  ABILITY_084,
  ABILITY_085,
  ABILITY_086,
  ABILITY_087,
  ABILITY_088,
  ABILITY_089,
  /// ★ Named from the game: "Cure" (MsgAbilityName)
  ABILITY_CURE,
  /// ★ Named from the game: "Cure Well" (MsgAbilityName)
  ABILITY_CURE_WELL,
  /// ★ Named from the game: "Potent Cure" (MsgAbilityName)
  ABILITY_POTENT_CURE,
  /// ★ Named from the game: "Ply" (MsgAbilityName)
  ABILITY_PLY,
  /// ★ Named from the game: "Ply Well" (MsgAbilityName)
  ABILITY_PLY_WELL,
  /// ★ Named from the game: "Pure Ply" (MsgAbilityName)
  ABILITY_PURE_PLY,
  /// ★ Named from the game: "Wish" (MsgAbilityName)
  ABILITY_WISH,
  /// ★ Named from the game: "Wish Well" (MsgAbilityName)
  ABILITY_WISH_WELL,
  /// ★ Named from the game: "Pure Wish" (MsgAbilityName)
  ABILITY_PURE_WISH,
  /// ★ Named from the game: "Cure Poison" (MsgAbilityName)
  ABILITY_CURE_POISON,
  /// ★ Named from the game: "Restore" (MsgAbilityName)
  ABILITY_RESTORE,
  /// ★ Named from the game: "Revive" (MsgAbilityName)
  ABILITY_REVIVE,
  /// ★ Named from the game: "Impact" (MsgAbilityName)
  ABILITY_IMPACT,
  /// ★ Named from the game: "High Impact" (MsgAbilityName)
  ABILITY_HIGH_IMPACT,
  /// ★ Named from the game: "Dull" (MsgAbilityName)
  ABILITY_DULL,
  /// ★ Named from the game: "Blunt" (MsgAbilityName)
  ABILITY_BLUNT,
  /// ★ Named from the game: "Guard" (MsgAbilityName)
  ABILITY_GUARD,
  /// ★ Named from the game: "Protect" (MsgAbilityName)
  ABILITY_PROTECT,
  /// ★ Named from the game: "Impair" (MsgAbilityName)
  ABILITY_IMPAIR,
  /// ★ Named from the game: "Debilitate" (MsgAbilityName)
  ABILITY_DEBILITATE,
  /// ★ Named from the game: "Ward" (MsgAbilityName)
  ABILITY_WARD,
  /// ★ Named from the game: "Resist" (MsgAbilityName)
  ABILITY_RESIST,
  /// ★ Named from the game: "Weaken" (MsgAbilityName)
  ABILITY_WEAKEN,
  /// ★ Named from the game: "Enfeeble" (MsgAbilityName)
  ABILITY_ENFEEBLE,
  /// ★ Named from the game: "Taint" (MsgAbilityName)
  ABILITY_TAINT,
  /// ★ Named from the game: "Poison" (MsgAbilityName)
  ABILITY_POISON,
  /// ★ Named from the game: "Delude" (MsgAbilityName)
  ABILITY_DELUDE,
  /// ★ Named from the game: "Confuse" (MsgAbilityName)
  ABILITY_CONFUSE,
  /// ★ Named from the game: "Charm" (MsgAbilityName)
  ABILITY_CHARM,
  /// ★ Named from the game: "Paralyze" (MsgAbilityName)
  ABILITY_PARALYZE,
  /// ★ Named from the game: "Sleep" (MsgAbilityName)
  ABILITY_SLEEP,
  /// ★ Named from the game: "Bind" (MsgAbilityName)
  ABILITY_BIND,
  /// ★ Named from the game: "Haunt" (MsgAbilityName)
  ABILITY_HAUNT,
  /// ★ Named from the game: "Curse" (MsgAbilityName)
  ABILITY_CURSE,
  /// ★ Named from the game: "Condemn" (MsgAbilityName)
  ABILITY_CONDEMN,
  /// ★ Named from the game: "Drain" (MsgAbilityName)
  ABILITY_DRAIN,
  /// ★ Named from the game: "Psy Drain" (MsgAbilityName)
  ABILITY_PSY_DRAIN, ///< Psynergy that costs no PP
  /// ★ Named from the game: "Break" (MsgAbilityName)
  ABILITY_BREAK,
  /// ★ Named from the game: "Regenerate" (MsgAbilityName)
  ABILITY_REGENERATE,
  /// ★ Named from the game: "Reflect" (MsgAbilityName)
  ABILITY_REFLECT,
  ABILITY_130,
  ABILITY_131,
  ABILITY_132,
  ABILITY_133,
  ABILITY_134,
  ABILITY_135,
  ABILITY_136,
  ABILITY_137,
  ABILITY_138,
  ABILITY_139,
  /// ★ Named from the game: "Move" (MsgAbilityName)
  ABILITY_MOVE,
  /// ★ Named from the game: "Mind Read" (MsgAbilityName)
  ABILITY_MIND_READ,
  /// ★ Named from the game: "Force" (MsgAbilityName)
  ABILITY_FORCE,
  /// ★ Named from the game: "Lift" (MsgAbilityName)
  ABILITY_LIFT,
  /// ★ Named from the game: "Reveal" (MsgAbilityName)
  ABILITY_REVEAL,
  /// ★ Named from the game: "Halt" (MsgAbilityName)
  ABILITY_HALT,
  /// ★ Named from the game: "Cloak" (MsgAbilityName)
  ABILITY_CLOAK,
  /// ★ Named from the game: "Carry" (MsgAbilityName)
  ABILITY_CARRY,
  /// ★ Named from the game: "Catch" (MsgAbilityName)
  ABILITY_CATCH,
  /// ★ Named from the game: "Retreat" (MsgAbilityName)
  ABILITY_RETREAT,
  /// ★ Named from the game: "Avoid" (MsgAbilityName)
  ABILITY_AVOID,
  ABILITY_151,
  ABILITY_152,
  ABILITY_153,
  ABILITY_154,
  ABILITY_155,
  ABILITY_156,
  ABILITY_157,
  ABILITY_158,
  ABILITY_159,
  /// ★ Named from the game: "Dragon Cloud" (MsgAbilityName)
  ABILITY_DRAGON_CLOUD,
  /// ★ Named from the game: "Demon Night" (MsgAbilityName)
  ABILITY_DEMON_NIGHT,
  /// ★ Named from the game: "Helm Splitter" (MsgAbilityName)
  ABILITY_HELM_SPLITTER,
  /// ★ Named from the game: "Quick Strike" (MsgAbilityName)
  ABILITY_QUICK_STRIKE,
  /// ★ Named from the game: "Rockfall" (MsgAbilityName)
  ABILITY_ROCKFALL,
  /// ★ Named from the game: "Rockslide" (MsgAbilityName)
  ABILITY_ROCKSLIDE,
  /// ★ Named from the game: "Avalanche" (MsgAbilityName)
  ABILITY_AVALANCHE,
  /// ★ Named from the game: "Lava Shower" (MsgAbilityName)
  ABILITY_LAVA_SHOWER,
  /// ★ Named from the game: "Molten Bath" (MsgAbilityName)
  ABILITY_MOLTEN_BATH,
  /// ★ Named from the game: "Magma Storm" (MsgAbilityName)
  ABILITY_MAGMA_STORM,
  /// ★ Named from the game: "Demon Spear" (MsgAbilityName)
  ABILITY_DEMON_SPEAR,
  /// ★ Named from the game: "Angel Spear" (MsgAbilityName)
  ABILITY_ANGEL_SPEAR,
  /// ★ Named from the game: "Guardian" (MsgAbilityName)
  ABILITY_GUARDIAN,
  /// ★ Named from the game: "Protector" (MsgAbilityName)
  ABILITY_PROTECTOR,
  /// ★ Named from the game: "Magic Shell" (MsgAbilityName)
  ABILITY_MAGIC_SHELL,
  /// ★ Named from the game: "Magic Shield" (MsgAbilityName)
  ABILITY_MAGIC_SHIELD,
  /// ★ Named from the game: "Death Plunge" (MsgAbilityName)
  ABILITY_DEATH_PLUNGE,
  /// ★ Named from the game: "Shuriken" (MsgAbilityName)
  ABILITY_SHURIKEN,
  /// Rolls its hit as it is planned.
  /// ★ Named from the game: "Annihilation" (MsgAbilityName)
  ABILITY_ANNIHILATION,
  /// ★ Named from the game: "Punji" (MsgAbilityName)
  ABILITY_PUNJI,
  /// ★ Named from the game: "Punji Trap" (MsgAbilityName)
  ABILITY_PUNJI_TRAP,
  /// ★ Named from the game: "Punji Strike" (MsgAbilityName)
  ABILITY_PUNJI_STRIKE,
  /// ★ Named from the game: "Fire Bomb" (MsgAbilityName)
  ABILITY_FIRE_BOMB,
  /// ★ Named from the game: "Cluster Bomb" (MsgAbilityName)
  ABILITY_CLUSTER_BOMB,
  /// ★ Named from the game: "Carpet Bomb" (MsgAbilityName)
  ABILITY_CARPET_BOMB,
  /// ★ Named from the game: "Gale" (MsgAbilityName)
  ABILITY_GALE,
  /// ★ Named from the game: "Typhoon" (MsgAbilityName)
  ABILITY_TYPHOON,
  /// ★ Named from the game: "Hurricane" (MsgAbilityName)
  ABILITY_HURRICANE,
  /// ★ Named from the game: "Thunderclap" (MsgAbilityName)
  ABILITY_THUNDERCLAP,
  /// ★ Named from the game: "Thunderbolt" (MsgAbilityName)
  ABILITY_THUNDERBOLT,
  /// ★ Named from the game: "Thunderstorm" (MsgAbilityName)
  ABILITY_THUNDERSTORM,
  /// Jupiter Psynergy that deludes its targets.
  /// ★ Named from the game: "Mist" (MsgAbilityName)
  ABILITY_MIST,
  /// ★ Named from the game: "Ragnarok" (MsgAbilityName)
  ABILITY_RAGNAROK,
  /// ★ Named from the game: "Cutting Edge" (MsgAbilityName)
  ABILITY_CUTTING_EDGE,
  /// ★ Named from the game: "Heat Wave" (MsgAbilityName)
  ABILITY_HEAT_WAVE,
  /// ★ Named from the game: "Astral Blast" (MsgAbilityName)
  ABILITY_ASTRAL_BLAST,
  /// ★ Named from the game: "Planet Diver" (MsgAbilityName)
  ABILITY_PLANET_DIVER,
  ABILITY_197,
  ABILITY_198,
  ABILITY_199,
  ABILITY_200,
  ABILITY_201,
  ABILITY_202,
  ABILITY_203,
  ABILITY_204,
  ABILITY_205,
  ABILITY_206,
  ABILITY_207,
  ABILITY_208,
  ABILITY_209,
  /// ★ Named from the game: "Terra Strike" (MsgAbilityName)
  ABILITY_TERRA_STRIKE,
  /// ★ Named from the game: "Poison Cloud" (MsgAbilityName)
  ABILITY_POISON_CLOUD,
  /// ★ Named from the game: "Deadly Poison" (MsgAbilityName)
  ABILITY_DEADLY_POISON,
  /// ★ Named from the game: "Mortal Danger" (MsgAbilityName)
  ABILITY_MORTAL_DANGER,
  /// ★ Named from the game: "Bad Omen" (MsgAbilityName)
  ABILITY_BAD_OMEN,
  /// ★ Named from the game: "Life Nourish" (MsgAbilityName)
  ABILITY_LIFE_NOURISH,
  /// ★ Named from the game: "Aqua Sock" (MsgAbilityName)
  ABILITY_AQUA_SOCK,
  /// ★ Named from the game: "Blizzard" (MsgAbilityName)
  ABILITY_BLIZZARD,
  /// ★ Named from the game: "Frost Bite" (MsgAbilityName)
  ABILITY_FROST_BITE,
  /// ★ Named from the game: "Drown" (MsgAbilityName)
  ABILITY_DROWN,
  /// ★ Named from the game: "Life Leech" (MsgAbilityName)
  ABILITY_LIFE_LEECH,
  /// ★ Named from the game: "Psy Leech" (MsgAbilityName)
  ABILITY_PSY_LEECH,
  /// ★ Named from the game: "Broil" (MsgAbilityName)
  ABILITY_BROIL,
  /// ★ Named from the game: "Meltdown" (MsgAbilityName)
  ABILITY_MELTDOWN,
  /// The first of the enemy abilities BattleActionRun_Build announces in
  /// their own words; the others run from ABILITY_SONIC_WAVE to
  /// ABILITY_SEARCH_FOR_HELP.
  /// ★ Named from the game: "Heat Mirage" (MsgAbilityName)
  ABILITY_HEAT_MIRAGE,
  /// ★ Named from the game: "Barrage" (MsgAbilityName)
  ABILITY_BARRAGE,
  /// ★ Named from the game: "Demon Fire" (MsgAbilityName)
  ABILITY_DEMON_FIRE,
  /// ★ Named from the game: "Acid Bath" (MsgAbilityName)
  ABILITY_ACID_BATH,
  /// ★ Named from the game: "Vorpal Slash" (MsgAbilityName)
  ABILITY_VORPAL_SLASH,
  /// ★ Named from the game: "Stun Voltage" (MsgAbilityName)
  ABILITY_STUN_VOLTAGE,
  /// ★ Named from the game: "Blinding Smog" (MsgAbilityName)
  ABILITY_BLINDING_SMOG,
  /// ★ Named from the game: "Murk" (MsgAbilityName)
  ABILITY_MURK,
  /// ★ Named from the game: "Cyclone Slash" (MsgAbilityName)
  ABILITY_CYCLONE_SLASH,
  /// ★ Named from the game: "Psyphon Seal" (MsgAbilityName)
  ABILITY_PSYPHON_SEAL,
  /// ★ Named from the game: "Rapid Smash" (MsgAbilityName)
  ABILITY_RAPID_SMASH,
  /// ★ Named from the game: "Sonic Smash" (MsgAbilityName)
  ABILITY_SONIC_SMASH,
  /// ★ Named from the game: "Asura" (MsgAbilityName)
  ABILITY_ASURA,
  /// ★ Named from the game: "Titan Blade" (MsgAbilityName)
  ABILITY_TITAN_BLADE,
  /// ★ Named from the game: "Shining Star" (MsgAbilityName)
  ABILITY_SHINING_STAR,
  ABILITY_239,
  ABILITY_240,
  ABILITY_241,
  ABILITY_242,
  ABILITY_243,
  ABILITY_244,
  ABILITY_245,
  ABILITY_246,
  ABILITY_247,
  ABILITY_248,
  ABILITY_249,
  /// ★ Named from the game: "Herb" (MsgAbilityName)
  ABILITY_HERB,
  /// ★ Named from the game: "Nut" (MsgAbilityName)
  ABILITY_NUT,
  /// ★ Named from the game: "Vial" (MsgAbilityName)
  ABILITY_VIAL,
  /// ★ Named from the game: "Potion" (MsgAbilityName)
  ABILITY_POTION,
  /// ★ Named from the game: "Soothing Water" (MsgAbilityName)
  ABILITY_SOOTHING_WATER,
  /// ★ Named from the game: "Psy Crystal" (MsgAbilityName)
  ABILITY_PSY_CRYSTAL,
  /// ★ Named from the game: "Antidote" (MsgAbilityName)
  ABILITY_ANTIDOTE,
  /// ★ Named from the game: "Elixir" (MsgAbilityName)
  ABILITY_ELIXIR,
  /// ★ Named from the game: "Water of Life" (MsgAbilityName)
  ABILITY_WATER_OF_LIFE,
  ABILITY_259,
  /// The stat items' abilities, each raising one base stat for good (the item
  /// menu's use).
  /// ★ Named from the game: "Power Bread" (MsgAbilityName)
  ABILITY_POWER_BREAD,
  /// ★ Named from the game: "Cookie" (MsgAbilityName)
  ABILITY_COOKIE,
  /// ★ Named from the game: "Apple" (MsgAbilityName)
  ABILITY_APPLE,
  /// ★ Named from the game: "Hard Nut" (MsgAbilityName)
  ABILITY_HARD_NUT,
  /// ★ Named from the game: "Mint" (MsgAbilityName)
  ABILITY_MINT,
  /// ★ Named from the game: "Lucky Pepper" (MsgAbilityName)
  ABILITY_LUCKY_PEPPER,
  ABILITY_266,
  /// ★ Named from the game: "Smoke Bomb" (MsgAbilityName)
  ABILITY_SMOKE_BOMB,
  /// ★ Named from the game: "Sleep Bomb" (MsgAbilityName)
  ABILITY_SLEEP_BOMB,
  /// ★ Named from the game: "Adept Ring" (MsgAbilityName)
  ABILITY_ADEPT_RING,
  /// ★ Named from the game: "Corn" (MsgAbilityName)
  ABILITY_CORN,
  ABILITY_271,
  ABILITY_272,
  ABILITY_273,
  ABILITY_274,
  ABILITY_275,
  ABILITY_276,
  ABILITY_277,
  ABILITY_278,
  ABILITY_279,
  ABILITY_280,
  ABILITY_281,
  ABILITY_282,
  ABILITY_283,
  ABILITY_284,
  ABILITY_285,
  ABILITY_286,
  ABILITY_287,
  ABILITY_288,
  ABILITY_289,
  ABILITY_290,
  ABILITY_291,
  ABILITY_292,
  ABILITY_293,
  ABILITY_294,
  ABILITY_295,
  ABILITY_296,
  ABILITY_297,
  ABILITY_298,
  ABILITY_299,
  /// The first Djinni's ability: Djinni n of element e has ability
  /// ABILITY_DJINNI_FIRST + e * DJINN_PER_ELEMENT + n, djinn_table.c's ability
  /// column, whose name is the Djinni's.
  ABILITY_DJINNI_FIRST,
  /// ★ Named from the game: "Granite" (MsgDjinnName)
  ABILITY_GRANITE,
  /// ★ Named from the game: "Quartz" (MsgDjinnName)
  ABILITY_QUARTZ,
  /// ★ Named from the game: "Vine" (MsgDjinnName)
  ABILITY_VINE,
  /// ★ Named from the game: "Sap" (MsgDjinnName)
  ABILITY_SAP,
  /// ★ Named from the game: "Ground" (MsgDjinnName)
  ABILITY_GROUND,
  /// ★ Named from the game: "Bane" (MsgDjinnName)
  ABILITY_BANE,
  ABILITY_307,
  ABILITY_308,
  ABILITY_309,
  ABILITY_310,
  ABILITY_311,
  ABILITY_312,
  ABILITY_313,
  ABILITY_314,
  ABILITY_315,
  ABILITY_316,
  ABILITY_317,
  ABILITY_318,
  ABILITY_319,
  /// ★ Named from the game: "Fizz" (MsgDjinnName)
  ABILITY_FIZZ,
  /// ★ Named from the game: "Sleet" (MsgDjinnName)
  ABILITY_SLEET,
  /// The Mercury Djinni Mist's (djinn_table.c): an attack that may put its
  /// target to sleep.
  /// ★ Named from the game: "Mist" (MsgDjinnName)
  ABILITY_MIST_DJINNI,
  /// ★ Named from the game: "Spritz" (MsgDjinnName)
  ABILITY_SPRITZ,
  /// ★ Named from the game: "Hail" (MsgDjinnName)
  ABILITY_HAIL,
  /// ★ Named from the game: "Tonic" (MsgDjinnName)
  ABILITY_TONIC,
  /// ★ Named from the game: "Dew" (MsgDjinnName)
  ABILITY_DEW,
  ABILITY_327,
  ABILITY_328,
  ABILITY_329,
  ABILITY_330,
  ABILITY_331,
  ABILITY_332,
  ABILITY_333,
  ABILITY_334,
  ABILITY_335,
  ABILITY_336,
  ABILITY_337,
  ABILITY_338,
  ABILITY_339,
  /// ★ Named from the game: "Forge" (MsgDjinnName)
  ABILITY_FORGE,
  /// ★ Named from the game: "Fever" (MsgDjinnName)
  ABILITY_FEVER,
  /// ★ Named from the game: "Corona" (MsgDjinnName)
  ABILITY_CORONA,
  /// ★ Named from the game: "Scorch" (MsgDjinnName)
  ABILITY_SCORCH,
  /// ★ Named from the game: "Ember" (MsgDjinnName)
  ABILITY_EMBER,
  /// ★ Named from the game: "Flash" (MsgDjinnName)
  ABILITY_FLASH,
  /// ★ Named from the game: "Torch" (MsgDjinnName)
  ABILITY_TORCH,
  ABILITY_347,
  ABILITY_348,
  ABILITY_349,
  ABILITY_350,
  ABILITY_351,
  ABILITY_352,
  ABILITY_353,
  ABILITY_354,
  ABILITY_355,
  ABILITY_356,
  ABILITY_357,
  ABILITY_358,
  ABILITY_359,
  /// ★ Named from the game: "Gust" (MsgDjinnName)
  ABILITY_GUST,
  /// ★ Named from the game: "Breeze" (MsgDjinnName)
  ABILITY_BREEZE,
  /// ★ Named from the game: "Zephyr" (MsgDjinnName)
  ABILITY_ZEPHYR,
  /// ★ Named from the game: "Smog" (MsgDjinnName)
  ABILITY_SMOG,
  /// ★ Named from the game: "Kite" (MsgDjinnName)
  ABILITY_KITE,
  /// ★ Named from the game: "Squall" (MsgDjinnName)
  ABILITY_SQUALL,
  /// ★ Named from the game: "Luff" (MsgDjinnName)
  ABILITY_LUFF,
  ABILITY_367,
  ABILITY_368,
  ABILITY_369,
  ABILITY_370,
  ABILITY_371,
  ABILITY_372,
  ABILITY_373,
  ABILITY_374,
  ABILITY_375,
  ABILITY_376,
  ABILITY_377,
  ABILITY_378,
  ABILITY_379,
  /// The summons: four to an element, in the order of their Djinn costs, each
  /// element's six ids apart.
  /// ★ Named from the game: "Venus" (MsgDjinnName)
  ABILITY_VENUS,
  /// ★ Named from the game: "Ramses" (MsgDjinnName)
  ABILITY_RAMSES,
  /// ★ Named from the game: "Cybele" (MsgDjinnName)
  ABILITY_CYBELE,
  /// ★ Named from the game: "Judgment" (MsgDjinnName)
  ABILITY_JUDGMENT,
  ABILITY_384,
  ABILITY_385,
  /// ★ Named from the game: "Mercury" (MsgDjinnName)
  ABILITY_MERCURY,
  /// ★ Named from the game: "Nereid" (MsgDjinnName)
  ABILITY_NEREID,
  /// ★ Named from the game: "Neptune" (MsgDjinnName)
  ABILITY_NEPTUNE,
  /// ★ Named from the game: "Boreas" (MsgDjinnName)
  ABILITY_BOREAS,
  ABILITY_390,
  ABILITY_391,
  /// ★ Named from the game: "Mars" (MsgDjinnName)
  ABILITY_MARS,
  /// ★ Named from the game: "Kirin" (MsgDjinnName)
  ABILITY_KIRIN,
  /// ★ Named from the game: "Tiamat" (MsgDjinnName)
  ABILITY_TIAMAT,
  /// ★ Named from the game: "Meteor" (MsgDjinnName)
  ABILITY_METEOR,
  ABILITY_396,
  ABILITY_397,
  /// ★ Named from the game: "Jupiter" (MsgDjinnName)
  ABILITY_JUPITER,
  /// ★ Named from the game: "Atalanta" (MsgDjinnName)
  ABILITY_ATALANTA,
  /// ★ Named from the game: "Procne" (MsgDjinnName)
  ABILITY_PROCNE,
  /// ★ Named from the game: "Thor" (MsgDjinnName)
  ABILITY_THOR,
  ABILITY_402,
  ABILITY_403,
  ABILITY_404,
  ABILITY_405,
  ABILITY_406,
  ABILITY_407,
  ABILITY_408,
  ABILITY_409,
  ABILITY_410,
  ABILITY_411,
  ABILITY_412,
  ABILITY_413,
  ABILITY_414,
  ABILITY_415,
  ABILITY_416,
  ABILITY_417,
  ABILITY_418,
  ABILITY_419,
  /// Power 60: the Salamander's.
  /// ★ Named from the game: "Fire Blessing" (MsgDjinnName)
  ABILITY_FIRE_BLESSING,
  /// Power 90: the Dread Hound's, the Chimera's and the Chimera Mage's.
  /// ★ Named from the game: "Fire Blessing" (MsgDjinnName)
  ABILITY_FIRE_BLESSING_STRONG,
  /// Power 130: ENEMY_104's.
  /// ★ Named from the game: "Fire Blessing" (MsgDjinnName)
  ABILITY_FIRE_BLESSING_STRONGEST,
  /// Power 25: the Lizard Man's and the Living Statue's.
  /// ★ Named from the game: "Water Blessing" (MsgDjinnName)
  ABILITY_WATER_BLESSING,
  /// Power 50: the Lizard Fighter's and the Hydros Statue's.
  /// ★ Named from the game: "Water Blessing" (MsgDjinnName)
  ABILITY_WATER_BLESSING_STRONG,
  /// Power 100: the Kraken's.
  /// ★ Named from the game: "Water Blessing" (MsgDjinnName)
  ABILITY_WATER_BLESSING_STRONGEST,
  /// Power 50: the Gargoyle's.
  /// ★ Named from the game: "Ice Blessing" (MsgDjinnName)
  ABILITY_ICE_BLESSING,
  /// Power 90: the Ice Gargoyle's.
  /// ★ Named from the game: "Ice Blessing" (MsgDjinnName)
  ABILITY_ICE_BLESSING_STRONG,
  /// Power 180: the Cerebus's, the Fenrir's and the Lizard King's.
  /// ★ Named from the game: "Ice Blessing" (MsgDjinnName)
  ABILITY_ICE_BLESSING_STRONGEST,
  /// Venus, power 70: the Kraken's.
  /// ★ Named from the game: "Dark Blessing" (MsgDjinnName)
  ABILITY_DARK_BLESSING,
  /// Venus, power 110; no enemy's abilities hold it.
  /// ★ Named from the game: "Dark Blessing" (MsgDjinnName)
  ABILITY_DARK_BLESSING_STRONG,
  /// Mars, power 170, lowering defense: the Toadonpa's.
  /// ★ Named from the game: "Dark Blessing" (MsgDjinnName)
  ABILITY_DARK_BLESSING_MARS,
  /// ★ Named from the game: "Acid Blessing" (MsgDjinnName)
  ABILITY_ACID_BLESSING,
  /// Power 190: the Thunder Lizard's.
  /// ★ Named from the game: "Storm Blessing" (MsgDjinnName)
  ABILITY_STORM_BLESSING,
  /// Power 220; no enemy's abilities hold it.
  /// ★ Named from the game: "Storm Blessing" (MsgDjinnName)
  ABILITY_STORM_BLESSING_STRONG,
  /// Power 5: ENEMY_BAT_2's.
  /// ★ Named from the game: "Sonic Wave" (MsgDjinnName)
  ABILITY_SONIC_WAVE,
  /// Power 45 against ENEMY_BAT_2's 5: the Rabid Bat's, the Clay Gargoyle's,
  /// the Ant Lion's and the Roach's.
  /// ★ Named from the game: "Sonic Wave" (MsgDjinnName)
  ABILITY_SONIC_WAVE_STRONG,
  /// ★ Named from the game: "Shriek" (MsgDjinnName)
  ABILITY_SHRIEK,
  /// ★ Named from the game: "Banshee Howl" (MsgDjinnName)
  ABILITY_BANSHEE_HOWL,
  /// ★ Named from the game: "Crazy Voice" (MsgDjinnName)
  ABILITY_CRAZY_VOICE,
  /// ★ Named from the game: "War Cry" (MsgDjinnName)
  ABILITY_WAR_CRY,
  /// ★ Named from the game: "Wicked Howl" (MsgDjinnName)
  ABILITY_WICKED_HOWL,
  /// ★ Named from the game: "Wing Beat" (MsgDjinnName)
  ABILITY_WING_BEAT,
  /// ★ Named from the game: "Wing Flutter" (MsgDjinnName)
  ABILITY_WING_FLUTTER,
  /// ★ Named from the game: "Wing Stroke" (MsgDjinnName)
  ABILITY_WING_STROKE,
  /// ★ Named from the game: "Evil Blessing" (MsgDjinnName)
  ABILITY_EVIL_BLESSING,
  /// ★ Named from the game: "Deadly Gas" (MsgDjinnName)
  ABILITY_DEADLY_GAS,
  ABILITY_447,
  ABILITY_448,
  ABILITY_449,
  /// ★ Named from the game: "Rumble" (MsgDjinnName)
  ABILITY_RUMBLE,
  /// ★ Named from the game: "Bone Chiller" (MsgDjinnName)
  ABILITY_BONE_CHILLER,
  /// ★ Named from the game: "Slice" (MsgDjinnName)
  ABILITY_SLICE,
  /// ★ Named from the game: "Bone Charge" (MsgDjinnName)
  ABILITY_BONE_CHARGE,
  /// ★ Named from the game: "Mystic Flame" (MsgDjinnName)
  ABILITY_MYSTIC_FLAME,
  /// ★ Named from the game: "Numbing Sting" (MsgDjinnName)
  ABILITY_NUMBING_STING,
  /// ★ Named from the game: "Brute Force" (MsgDjinnName)
  ABILITY_BRUTE_FORCE,
  /// ★ Named from the game: "Sticky Goo" (MsgDjinnName)
  ABILITY_STICKY_GOO,
  /// ★ Named from the game: "Cannibal Fang" (MsgDjinnName)
  ABILITY_CANNIBAL_FANG,
  /// ★ Named from the game: "Bear Claw" (MsgDjinnName)
  ABILITY_BEAR_CLAW,
  /// ★ Named from the game: "Poisonous Bite" (MsgDjinnName)
  ABILITY_POISONOUS_BITE,
  /// ★ Named from the game: "Flying Attack" (MsgDjinnName)
  ABILITY_FLYING_ATTACK,
  /// ★ Named from the game: "Undead Sword" (MsgDjinnName)
  ABILITY_UNDEAD_SWORD,
  /// ★ Named from the game: "Ransack" (MsgDjinnName)
  ABILITY_RANSACK,
  /// ★ Named from the game: "Sticky Poison" (MsgDjinnName)
  ABILITY_STICKY_POISON,
  /// ★ Named from the game: "Poison Fang" (MsgDjinnName)
  ABILITY_POISON_FANG,
  /// ★ Named from the game: "Electric Bite" (MsgDjinnName)
  ABILITY_ELECTRIC_BITE,
  /// ★ Named from the game: "Poison Tail" (MsgDjinnName)
  ABILITY_POISON_TAIL,
  /// ★ Named from the game: "Onslaught" (MsgDjinnName)
  ABILITY_ONSLAUGHT,
  /// ★ Named from the game: "Vampiric Fang" (MsgDjinnName)
  ABILITY_VAMPIRIC_FANG,
  /// ★ Named from the game: "Bacteria Rush" (MsgDjinnName)
  ABILITY_BACTERIA_RUSH,
  /// ★ Named from the game: "Swift Strike" (MsgDjinnName)
  ABILITY_SWIFT_STRIKE,
  /// ★ Named from the game: "Rotten Blood" (MsgDjinnName)
  ABILITY_ROTTEN_BLOOD,
  /// ★ Named from the game: "Forcible Arm" (MsgDjinnName)
  ABILITY_FORCIBLE_ARM,
  /// ★ Named from the game: "Double Fang" (MsgDjinnName)
  ABILITY_DOUBLE_FANG,
  /// ★ Named from the game: "Mortal Blow" (MsgDjinnName)
  ABILITY_MORTAL_BLOW,
  /// ★ Named from the game: "Freebite Rush" (MsgDjinnName)
  ABILITY_FREEBITE_RUSH,
  /// ★ Named from the game: "Twin Beaks" (MsgDjinnName)
  ABILITY_TWIN_BEAKS,
  /// ★ Named from the game: "Rabid Fang" (MsgDjinnName)
  ABILITY_RABID_FANG,
  /// ★ Named from the game: "Acid Bite" (MsgDjinnName)
  ABILITY_ACID_BITE,
  /// ★ Named from the game: "Dynamite" (MsgDjinnName)
  ABILITY_DYNAMITE,
  /// ★ Named from the game: "Headbutt" (MsgDjinnName)
  ABILITY_HEADBUTT,
  /// ★ Named from the game: "Poison Ink" (MsgDjinnName)
  ABILITY_POISON_INK,
  /// ★ Named from the game: "Truncheon Fist" (MsgDjinnName)
  ABILITY_TRUNCHEON_FIST,
  /// ★ Named from the game: "Counterstrike" (MsgDjinnName)
  ABILITY_COUNTERSTRIKE,
  /// ★ Named from the game: "Mad Dash" (MsgDjinnName)
  ABILITY_MAD_DASH,
  /// ★ Named from the game: "Soothing Star" (MsgDjinnName)
  ABILITY_SOOTHING_STAR,
  /// ★ Named from the game: "Spider Web" (MsgDjinnName)
  ABILITY_SPIDER_WEB,
  /// ★ Named from the game: "Heartrender" (MsgDjinnName)
  ABILITY_HEARTRENDER,
  /// ★ Named from the game: "Mad Spatter" (MsgDjinnName)
  ABILITY_MAD_SPATTER,
  /// ★ Named from the game: "Spasm" (MsgDjinnName)
  ABILITY_SPASM,
  /// ★ Named from the game: "Sleep Star" (MsgDjinnName)
  ABILITY_SLEEP_STAR,
  /// ★ Named from the game: "Decompose" (MsgDjinnName)
  ABILITY_DECOMPOSE,
  /// ★ Named from the game: "Haunting" (MsgDjinnName)
  ABILITY_HAUNTING,
  /// ★ Named from the game: "Worms" (MsgDjinnName)
  ABILITY_WORMS,
  /// ★ Named from the game: "Berserk" (MsgDjinnName)
  ABILITY_BERSERK,
  /// ★ Named from the game: "Lucid Prophecy" (MsgDjinnName)
  ABILITY_LUCID_PROPHECY,
  /// ★ Named from the game: "Recovery" (MsgDjinnName)
  ABILITY_RECOVERY,
  /// ★ Named from the game: "Flee" (MsgDjinnName)
  ABILITY_FLEE,
  /// ★ Named from the game: "Contain" (MsgDjinnName)
  ABILITY_CONTAIN,
  /// ★ Named from the game: "Threaten" (MsgDjinnName)
  ABILITY_THREATEN,
  /// ★ Named from the game: "Tremble" (MsgDjinnName)
  ABILITY_TREMBLE,
  /// ★ Named from the game: "Fortify" (MsgDjinnName)
  ABILITY_FORTIFY,
  /// An enemy that splits itself in two. Its message: "attempts to divide".
  /// ★ Named from the game: "Speed Surge" (MsgDjinnName)
  ABILITY_SPEED_SURGE,
  /// ★ Named from the game: "Ally Search" (MsgDjinnName)
  ABILITY_ALLY_SEARCH,
  /// ★ Named from the game: "Sidestep" (MsgDjinnName)
  ABILITY_SIDESTEP,
  /// ★ Named from the game: "Total Defense" (MsgDjinnName)
  ABILITY_TOTAL_DEFENSE,
  /// ★ Named from the game: "Stand Ready" (MsgDjinnName)
  ABILITY_STAND_READY,
  /// ★ Named from the game: "Search for Help" (MsgDjinnName)
  ABILITY_SEARCH_FOR_HELP,
  /// What an enemy with nothing it can use commands.
  /// ★ Named from the game: "Can't Use" (MsgDjinnName)
  ABILITY_CANT_USE,
  /// ★ Named from the game: "Poison Beat" (MsgDjinnName)
  ABILITY_POISON_BEAT,
  /// ★ Named from the game: "Spinning Beat" (MsgDjinnName)
  ABILITY_SPINNING_BEAT,
  /// ★ Named from the game: "Heat Flash" (MsgDjinnName)
  ABILITY_HEAT_FLASH,
  /// ★ Named from the game: "Death Size" (MsgDjinnName)
  ABILITY_DEATH_SIZE,
  /// ★ Named from the game: "Outer Space" (MsgDjinnName)
  ABILITY_OUTER_SPACE,
  /// ★ Named from the game: "Dragon Driver" (MsgDjinnName)
  ABILITY_DRAGON_DRIVER,
  /// ★ Named from the game: "Drain Fang" (MsgDjinnName)
  ABILITY_DRAIN_FANG,
  /// The last row of gAbilityStaging.
  /// ★ Named from the game: "Severe Blow" (MsgDjinnName)
  ABILITY_SEVERE_BLOW,
  /// The last row of ability_table.c and gAbilityBattleEffects.
  /// ★ Named from the game: "Thrash" (MsgDjinnName)
  ABILITY_THRASH
};

#endif
