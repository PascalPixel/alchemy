#ifndef ALCHEMY_BATTLE_EFFECT_BATTLE_EFFECT_H
#define ALCHEMY_BATTLE_EFFECT_BATTLE_EFFECT_H

/// @file
/// What the battle_effect object defines for the code outside it.

#include "fixed_math.h"
#include "types.h"
#include "resource_ids.h"
#include "field/particle.h"
#include "battle/battle_effect.h"
#include "battle/battle_unit.h"

struct BattleEffectParams;
struct ScreenPoint;
struct Particle;
struct MeshTriangle;
struct MeshVertex;
struct PoolEntry;
struct StripQuad;

/// The summons' sprite formations (BattleEffect_PlaceFormationObjects and
/// BattleEffect_RunImpactBurst), each named for the summon that keeps it.
enum SummonFormation {
  SUMMON_FORMATION_THOR,    ///< nine in a 3x3 grid at 32-pixel spacing
  SUMMON_FORMATION_NEREID,  ///< twelve in rows of 1, 3, 4 and 4
  SUMMON_FORMATION_CYBELE,  ///< eight in rows of 3, 2 and 3
  SUMMON_FORMATION_TIAMAT,  ///< eight drawn at 1.25 times their size
  SUMMON_FORMATION_MEGAERA, ///< eleven from signed columns
  SUMMON_FORMATION_MOLOCH,  ///< twelve in a 4x3 grid
  /// Tiamat's eight when the first target's animation kind is 2.
  SUMMON_FORMATION_TIAMAT_ALTERNATE
};

/// The large effects BattleEffect_RunSummon runs, by kind: the summons, Ramses
/// to Iris, and the scenes of Daedalus's second strike, Trident, Cruel Ruin and
/// Meteor Blow.
enum SummonEffectId {
  SUMMON_EFFECT_NONE,        ///< runs Meteor's effect
  SUMMON_EFFECT_RAMSES,      ///< BattleEffect_RunRamses (ramses.c)
  SUMMON_EFFECT_NEREID,      ///< BattleEffect_RunNereid (nereid.c)
  SUMMON_EFFECT_KIRIN,       ///< BattleEffect_RunKirin (kirin.c)
  SUMMON_EFFECT_ATALANTA,    ///< BattleEffect_RunAtalanta (atalanta.c)
  SUMMON_EFFECT_CYBELE,      ///< BattleEffect_RunCybele (cybele.c)
  SUMMON_EFFECT_NEPTUNE,     ///< BattleEffect_RunNeptune (neptune.c)
  SUMMON_EFFECT_TIAMAT,      ///< BattleEffect_RunTiamat (procne.c)
  SUMMON_EFFECT_PROCNE,      ///< BattleEffect_RunProcne (procne.c)
  SUMMON_EFFECT_JUDGMENT,    ///< BattleEffect_RunJudgment (judgment.c)
  SUMMON_EFFECT_BOREAS,      ///< BattleEffect_RunBoreas (boreas.c)
  SUMMON_EFFECT_METEOR,      ///< BattleEffect_RunMeteor (particle_streams.c)
  SUMMON_EFFECT_THOR,        ///< BattleEffect_RunThor (thor.c)
  SUMMON_EFFECT_ZAGAN,       ///< BattleEffect_RunZagan (djinn_heal.c)
  SUMMON_EFFECT_MEGAERA,     ///< BattleEffect_RunMegaera (thunder_mine.c)
  SUMMON_EFFECT_FLORA,       ///< BattleEffect_RunFlora (flora.c)
  SUMMON_EFFECT_MOLOCH,      ///< BattleEffect_RunMoloch (moloch.c)
  SUMMON_EFFECT_ULYSSES,     ///< BattleEffect_RunUlysses (spider_web.c)
  SUMMON_EFFECT_HAURES,      ///< BattleEffect_RunHaures (haures.c)
  SUMMON_EFFECT_ECLIPSE,     ///< BattleEffect_RunEclipse (canvas.c)
  SUMMON_EFFECT_COATLICUE,   ///< BattleEffect_RunCoatlicue (vengeance.c)
  SUMMON_EFFECT_DAEDALUS,    ///< BattleEffect_RunDaedalus (daedalus.c)
  SUMMON_EFFECT_AZUL,        ///< BattleEffect_RunAzul (azul.c)
  SUMMON_EFFECT_CATASTROPHE, ///< BattleEffect_RunCatastrophe (catastrophe.c)
  SUMMON_EFFECT_CHARON,      ///< BattleEffect_RunCharon (charon.c)
  SUMMON_EFFECT_IRIS,        ///< BattleEffect_RunIris (moloch.c)
  SUMMON_EFFECT_DAEDALUS_STRIKE, ///< BattleEffect_RunDaedalusStrike
                                 ///< (daedalus.c)
  SUMMON_EFFECT_TRIDENT,         ///< BattleEffect_RunTrident (trident.c)
  SUMMON_EFFECT_CRUEL_RUIN,      ///< BattleEffect_RunCruelRuin (cruel_ruin.c)
  SUMMON_EFFECT_METEOR_BLOW      ///< BattleEffect_RunMeteorBlow (meteor_blow.c)
};

/// Effect kinds: the kind an ability's gAbilityBattleEffects row selects for
/// BattleEffect_RunFromTable, by the handler of gBattleEffect_Handlers that
/// plays it; a later kind that shares a handler is named for the ability that
/// uses it, or Unused when none does. Kinds without a handler have no name. A
/// row with EFFECT_SUMMON holds an enum SummonEffectId instead, and one with
/// EFFECT_BY_ID_RANGE a number of BattleEffect_RunByIdRange's own.
enum BattleEffectId {
  BATTLE_EFFECT_NONE = 0, ///< no effect
  BATTLE_EFFECT_QUAKE = 1,
  BATTLE_EFFECT_SPIRE = 2,
  BATTLE_EFFECT_GAIA = 3,
  BATTLE_EFFECT_GROWTH = 4,
  BATTLE_EFFECT_THORN = 5,
  BATTLE_EFFECT_FROST = 6,
  BATTLE_EFFECT_ICE = 7,
  BATTLE_EFFECT_PRISM = 8,
  BATTLE_EFFECT_DOUSE = 9,
  BATTLE_EFFECT_FROTH = 10,
  BATTLE_EFFECT_FLARE = 11,
  BATTLE_EFFECT_FIRE = 12,
  BATTLE_EFFECT_VOLCANO = 13,
  BATTLE_EFFECT_BLAST = 14,
  BATTLE_EFFECT_NOVA = 15,
  BATTLE_EFFECT_BOLT = 16,
  BATTLE_EFFECT_RAY = 17,
  BATTLE_EFFECT_PLASMA = 18,
  BATTLE_EFFECT_SLASH = 19,
  BATTLE_EFFECT_WHIRLWIND = 20,
  BATTLE_EFFECT_CURE = 21,
  BATTLE_EFFECT_PLY = 22,
  BATTLE_EFFECT_WISH = 23,
  BATTLE_EFFECT_CURE_POISON = 24,
  BATTLE_EFFECT_REVIVE = 25,
  BATTLE_EFFECT_IMPACT = 26,
  BATTLE_EFFECT_DULL = 27,
  BATTLE_EFFECT_GUARD = 28,
  BATTLE_EFFECT_IMPAIR = 29,
  BATTLE_EFFECT_WARD = 30,
  BATTLE_EFFECT_WEAKEN = 31,
  BATTLE_EFFECT_TAINT = 32,
  BATTLE_EFFECT_DELUDE = 33,
  BATTLE_EFFECT_UNUSED_ORBITING_SPIRALS = 34, ///< no ability uses it
  BATTLE_EFFECT_UNUSED_HEARTRENDER = 35,      ///< no ability uses it
  BATTLE_EFFECT_PARALYZE = 36,
  BATTLE_EFFECT_SLEEP = 37,
  BATTLE_EFFECT_BIND = 38,
  BATTLE_EFFECT_HAUNT = 39,
  BATTLE_EFFECT_CONDEMN = 40,
  BATTLE_EFFECT_CURSE = 41,
  BATTLE_EFFECT_DRAIN = 42,
  BATTLE_EFFECT_BREAK = 43,
  BATTLE_EFFECT_REGENERATE = 44,    ///< played by BattleEffect_RunWish
  BATTLE_EFFECT_REFLECT = 45,       ///< played by BattleEffect_RunWish
  BATTLE_EFFECT_UNUSED_IMPACT = 46, ///< no ability uses it
  BATTLE_EFFECT_UNUSED_IMPAIR = 47, ///< no ability uses it
  BATTLE_EFFECT_DRAGON_CLOUD = 49,
  BATTLE_EFFECT_DEMON_NIGHT = 50,
  BATTLE_EFFECT_HELM_SPLITTER = 51,
  BATTLE_EFFECT_QUICK_STRIKE = 52,
  BATTLE_EFFECT_DEATH_PLUNGE = 53,
  BATTLE_EFFECT_SHURIKEN = 54,
  BATTLE_EFFECT_ANNIHILATION = 55,
  BATTLE_EFFECT_PUNJI = 56,
  BATTLE_EFFECT_CUTTING_EDGE = 57,
  BATTLE_EFFECT_RAGNAROK = 58,
  BATTLE_EFFECT_HEAT_WAVE = 59,
  BATTLE_EFFECT_PLANET_DIVER = 60,
  BATTLE_EFFECT_TITAN_BLADE = 61,
  BATTLE_EFFECT_SHINING_STAR = 62,
  BATTLE_EFFECT_BLIZZARD = 63,
  BATTLE_EFFECT_DEMON_FIRE = 64,
  BATTLE_EFFECT_VORPAL_SLASH = 65,
  BATTLE_EFFECT_DROWN = 66,
  BATTLE_EFFECT_UNUSED_TRIANGLE_STRIKE = 67, ///< no ability uses it
  BATTLE_EFFECT_ASTRAL_BLAST = 68,
  BATTLE_EFFECT_HEARTBREAK = 69,
  BATTLE_EFFECT_AURA = 71,
  BATTLE_EFFECT_FUME = 72,
  BATTLE_EFFECT_BEAM = 73,
  BATTLE_EFFECT_RAGING_HEAT = 74,
  BATTLE_EFFECT_COOL = 75,
  BATTLE_EFFECT_SABRE_DANCE = 76,
  BATTLE_EFFECT_BACKSTAB = 77,
  BATTLE_EFFECT_JUGGLE = 78,
  BATTLE_EFFECT_FLAME_CARD = 79,
  BATTLE_EFFECT_WHIPLASH = 80,
  BATTLE_EFFECT_POISON_FLOW = 81,
  BATTLE_EFFECT_FIRE_PUPPET = 82,
  BATTLE_EFFECT_DIAMOND_DUST = 83,
  BATTLE_EFFECT_DIAMOND_BERG = 84,
  BATTLE_EFFECT_PLUME_EDGE = 85,
  BATTLE_EFFECT_THUNDER_MINE = 86,
  BATTLE_EFFECT_FROST_CARD = 87,
  BATTLE_EFFECT_BRAMBLE_CARD = 88,
  BATTLE_EFFECT_THUNDER_CARD = 89,
  BATTLE_EFFECT_BAFFLE_CARD = 90,
  BATTLE_EFFECT_SWORD_CARD = 91,
  BATTLE_EFFECT_SLEEP_CARD = 92,
  BATTLE_EFFECT_DEATH_CARD = 93,
  BATTLE_EFFECT_STONE_JUSTICE = 100,
  BATTLE_EFFECT_HEAVY_DIVIDE = 101,
  BATTLE_EFFECT_WYRD_CURSE = 103,
  BATTLE_EFFECT_ACHERONS_GRIEF = 104,
  BATTLE_EFFECT_HURRICANE = 105,
  BATTLE_EFFECT_DREAMTIDE = 106,
  BATTLE_EFFECT_LETHE_ALBION = 107,
  BATTLE_EFFECT_RISING_DRAGON = 108,
  BATTLE_EFFECT_BOOST_HACK = 109,
  BATTLE_EFFECT_RADIANT_FIRE = 110,
  BATTLE_EFFECT_FIRE_DANCE = 111,
  BATTLE_EFFECT_BLAZE_RUSH = 112,
  BATTLE_EFFECT_SHRED = 113,
  BATTLE_EFFECT_PURGATORY = 114,
  BATTLE_EFFECT_SOUL_SHATTER = 115,
  BATTLE_EFFECT_AGING_GAS = 116,
  BATTLE_EFFECT_MAD_ZEPHYR = 117,
  BATTLE_EFFECT_LUNAR_SLASH = 118,
  BATTLE_EFFECT_NIRVANA = 119,
  BATTLE_EFFECT_LIGHT_SURGE = 120,
  BATTLE_EFFECT_VOID_BEAM = 121,
  BATTLE_EFFECT_LEGEND = 122,
  BATTLE_EFFECT_LIQUIFIER = 123,
  BATTLE_EFFECT_MEGIDDO = 124,
  BATTLE_EFFECT_ODYSSEY = 125,
  BATTLE_EFFECT_VENGEANCE = 126,
  BATTLE_EFFECT_THORNY_GRAVE = 127,
  BATTLE_EFFECT_PLANETARY = 128,
  BATTLE_EFFECT_LIFE_SHEAR = 129,
  /// High Impact, which BattleStaging_PlanHighImpact plans.
  BATTLE_EFFECT_CRUCIBLE_FIRE = 181,
  /// Aurora Field, which BattleStaging_PlanAuroraField plans.
  BATTLE_EFFECT_REVERSE_STAR = 182,
  BATTLE_EFFECT_GRANITE = 200,
  BATTLE_EFFECT_QUARTZ = 201, ///< played by BattleEffect_RunRevive
  BATTLE_EFFECT_VINE = 202,
  BATTLE_EFFECT_GROUND = 203,
  BATTLE_EFFECT_FIZZ = 204, ///< played by BattleEffect_RunPly
  BATTLE_EFFECT_SLEET = 205,
  BATTLE_EFFECT_SPRITZ = 206, ///< played by BattleEffect_RunWish
  BATTLE_EFFECT_HAIL = 207,
  BATTLE_EFFECT_TONIC = 208,
  BATTLE_EFFECT_DEW = 209,
  BATTLE_EFFECT_FORGE = 210,  ///< played by BattleEffect_RunImpact
  BATTLE_EFFECT_CORONA = 211, ///< played by BattleEffect_RunGuard
  BATTLE_EFFECT_PSY_CRYSTAL = 212,
  BATTLE_EFFECT_FLASH = 213,
  BATTLE_EFFECT_TORCH = 214,
  BATTLE_EFFECT_BREEZE = 215, ///< played by BattleEffect_RunWard
  BATTLE_EFFECT_ZEPHYR = 216,
  BATTLE_EFFECT_KITE = 217,
  BATTLE_EFFECT_LUFF = 218, ///< played by BattleEffect_RunBind
  BATTLE_EFFECT_GUST = 219,
  BATTLE_EFFECT_IRON = 220,
  BATTLE_EFFECT_STEAM = 221,
  BATTLE_EFFECT_SHADE = 222,
  BATTLE_EFFECT_KINDLE = 223, ///< played by BattleEffect_RunImpact
  BATTLE_EFFECT_COAL = 224,
  BATTLE_EFFECT_FLOWER = 225, ///< played by BattleEffect_RunCure
  BATTLE_EFFECT_SALT = 226,
  BATTLE_EFFECT_SPRING = 227,
  BATTLE_EFFECT_BREATH = 228,
  BATTLE_EFFECT_ETHER = 229,
  BATTLE_EFFECT_BALM = 230,
  BATTLE_EFFECT_TINDER = 231,
  BATTLE_EFFECT_WHORL = 232,
  BATTLE_EFFECT_GALE = 233,
  BATTLE_EFFECT_WAFT = 234,
  BATTLE_EFFECT_CORE = 236,
  BATTLE_EFFECT_MUD = 237,
  BATTLE_EFFECT_HAZE = 238,
  BATTLE_EFFECT_PETRA = 239,
  BATTLE_EFFECT_RIME = 240,
  BATTLE_EFFECT_LULL = 241,
  BATTLE_EFFECT_GASP = 242,
  BATTLE_EFFECT_FUGUE = 243,
  BATTLE_EFFECT_CRYSTAL = 244,
  BATTLE_EFFECT_EDDY = 245,
  BATTLE_EFFECT_CANNON = 246,
  BATTLE_EFFECT_AROMA = 247,
  BATTLE_EFFECT_REFLUX = 248,
  BATTLE_EFFECT_DJINN_COUNTER = 249,
  BATTLE_EFFECT_MOLD = 250,
  BATTLE_EFFECT_MELD = 251,
  BATTLE_EFFECT_SHINE = 252,
  BATTLE_EFFECT_GEODE = 253,
  BATTLE_EFFECT_SERAC = 254,
  BATTLE_EFFECT_SPARK = 255,
  BATTLE_EFFECT_FURY = 256,
  BATTLE_EFFECT_CHAR = 257,
  BATTLE_EFFECT_VENUS = 280,
  BATTLE_EFFECT_MERCURY = 281,
  BATTLE_EFFECT_MARS = 282,
  BATTLE_EFFECT_JUPITER = 283,
  BATTLE_EFFECT_SONIC_WAVE = 300,
  BATTLE_EFFECT_SHRIEK = 301,
  BATTLE_EFFECT_BANSHEE_HOWL = 302,
  BATTLE_EFFECT_CRAZY_VOICE = 303,
  BATTLE_EFFECT_WAR_CRY = 304,
  BATTLE_EFFECT_WICKED_HOWL = 305,
  BATTLE_EFFECT_WING_BEAT = 306,
  BATTLE_EFFECT_WING_FLUTTER = 307,
  BATTLE_EFFECT_RUMBLE = 308,
  BATTLE_EFFECT_BONE_CHILLER = 309,
  BATTLE_EFFECT_RAPID_SMASH = 310, ///< played by BattleEffect_RunGust
  BATTLE_EFFECT_BONE_CHARGE = 311,
  BATTLE_EFFECT_MYSTIC_FLAME = 312,
  BATTLE_EFFECT_NUMBING_STING = 313,
  BATTLE_EFFECT_BRUTE_FORCE = 314,
  BATTLE_EFFECT_STICKY_GOO = 315,
  BATTLE_EFFECT_CANNIBAL_FANG = 316,
  BATTLE_EFFECT_BEAR_CLAW = 317,
  BATTLE_EFFECT_POISONOUS_BITE = 318,
  BATTLE_EFFECT_FLYING_ATTACK = 319,
  BATTLE_EFFECT_UNDEAD_SWORD = 320,
  BATTLE_EFFECT_RANSACK = 321,
  BATTLE_EFFECT_STICKY_POISON = 322,
  BATTLE_EFFECT_POISON_FANG = 323, ///< played by BattleEffect_RunPoisonousBite
  BATTLE_EFFECT_ELECTRIC_BITE = 324,
  BATTLE_EFFECT_POISON_TAIL = 325,
  BATTLE_EFFECT_ONSLAUGHT = 326,
  BATTLE_EFFECT_VAMPIRIC_FANG = 327,
  BATTLE_EFFECT_BACTERIA_RUSH = 328,
  BATTLE_EFFECT_SWIFT_STRIKE = 329, ///< played by BattleEffect_RunFlyingAttack
  BATTLE_EFFECT_ROTTEN_BLOOD = 330,
  BATTLE_EFFECT_DOUBLE_FANG = 332,
  BATTLE_EFFECT_MORTAL_BLOW = 333,
  BATTLE_EFFECT_FREEBITE_RUSH = 334,
  BATTLE_EFFECT_TWIN_BEAKS = 335,
  BATTLE_EFFECT_RABID_FANG = 336,
  BATTLE_EFFECT_ACID_BITE = 337,
  BATTLE_EFFECT_DYNAMITE = 338,
  BATTLE_EFFECT_HEADBUTT = 339,
  BATTLE_EFFECT_POISON_INK = 340,
  BATTLE_EFFECT_COUNTERSTRIKE = 342, ///< played by BattleEffect_RunBearClaw
  BATTLE_EFFECT_MAD_DASH = 343,      ///< played by BattleEffect_RunRansack
  BATTLE_EFFECT_SOOTHING_STAR = 344,
  BATTLE_EFFECT_SPIDER_WEB = 345,
  BATTLE_EFFECT_HEARTRENDER = 346, ///< played by BattleEffect_RunHeartrender
  BATTLE_EFFECT_MAD_SPATTER = 347,
  BATTLE_EFFECT_SPASM = 348,
  BATTLE_EFFECT_SLEEP_STAR = 349,
  BATTLE_EFFECT_DECOMPOSE = 350,
  BATTLE_EFFECT_HAUNTING = 351,
  BATTLE_EFFECT_WORMS = 352, ///< played by BattleEffect_RunCure
  BATTLE_EFFECT_BERSERK = 353,
  BATTLE_EFFECT_LUCID_PROPHECY = 354, ///< played by BattleEffect_RunCurse
  BATTLE_EFFECT_SMOKE_BOMB = 380,
  BATTLE_EFFECT_WING_STROKE = 381,
  BATTLE_EFFECT_POISON_BEAT = 382,
  BATTLE_EFFECT_SPINNING_BEAT = 383,
  BATTLE_EFFECT_OUTER_SPACE = 384,
  BATTLE_EFFECT_DRAGON_DRIVER = 385,
  BATTLE_EFFECT_DRAIN_FANG = 386,
  BATTLE_EFFECT_SEVERE_BLOW = 387,
  BATTLE_EFFECT_THRASH = 388,
  BATTLE_EFFECT_RECOVERY = 389,
  BATTLE_EFFECT_ECHO_CUT = 390,
  BATTLE_EFFECT_WATERY_GRAVE = 391,
  BATTLE_EFFECT_RAGING_FLOOD = 392,
  BATTLE_EFFECT_HYDRO_SLASH = 393,
  BATTLE_EFFECT_MIGHTY_PRESS = 394,
  BATTLE_EFFECT_SLAVER = 395,
  BATTLE_EFFECT_COUNTER_RUSH = 396,
  BATTLE_EFFECT_DJINNFEST = 397,
  BATTLE_EFFECT_HEAT_KISS = 398,
  BATTLE_EFFECT_ANGLE_SPIKE = 399,
  BATTLE_EFFECT_FIRE_BREATH = 400,
  BATTLE_EFFECT_WATER_BREATH = 401,
  BATTLE_EFFECT_ICE_BLESSING = 402,
  BATTLE_EFFECT_DARK_BREATH = 403,
  BATTLE_EFFECT_ACID_BREATH = 404,
  BATTLE_EFFECT_STORM_BREATH = 405,
  BATTLE_EFFECT_EVIL_BLESSING = 406,
  BATTLE_EFFECT_DEADLY_GAS = 407,
  BATTLE_EFFECT_SAND_BREATH = 408,
  BATTLE_EFFECT_DESERT_GASP = 409,
  BATTLE_EFFECT_BLACK_ICE = 410,
  BATTLE_EFFECT_TYPHOON_BLOW = 411,
  BATTLE_EFFECT_TOXIC_BREATH = 412,
  BATTLE_EFFECT_GRAVEL_BLOW = 413,
  BATTLE_EFFECT_BLAST_BREATH = 414,
  BATTLE_EFFECT_DARKSOL_GASP = 415,
  BATTLE_EFFECT_FLAME_BREATH = 416, ///< played by BattleEffect_RunFireBreath
  BATTLE_EFFECT_MANEATER = 421,
  BATTLE_EFFECT_UR_FLASH = 422,
  BATTLE_EFFECT_CRUSHER_GRIP = 423,
  BATTLE_EFFECT_VANISH_CLAW = 424,
  BATTLE_EFFECT_TWIN_SHEAR = 425,
  BATTLE_EFFECT_TRIPLE_CHOMP = 426,
  BATTLE_EFFECT_DOUBLESTEP = 427,
  BATTLE_EFFECT_POISON_GEL = 428,
  BATTLE_EFFECT_MUCOUS_GEL = 429, ///< played by BattleEffect_RunDelude
  BATTLE_EFFECT_RISING_VENOM = 430,
  BATTLE_EFFECT_SACK = 431,
  BATTLE_EFFECT_FIRE_DANCE_UNLEASH = 432,
  BATTLE_EFFECT_STUN_MUSCLE = 433,
  BATTLE_EFFECT_ROLLING_FLAME = 434,
  BATTLE_EFFECT_CAGE = 435,
  BATTLE_EFFECT_PHOENIX = 436,
  BATTLE_EFFECT_HUMAN_HUNT = 437,
  BATTLE_EFFECT_UNUSED_HEAVY_BITE = 438, ///< no ability uses it
  BATTLE_EFFECT_UNUSED_FLASH_BITE = 439, ///< no ability uses it
  /// ABILITY_POISON_STING, played by BattleEffect_RunPoisonTail.
  BATTLE_EFFECT_POISON_STING_TAIL = 440,
  BATTLE_EFFECT_KILL_STING = 441,
  BATTLE_EFFECT_BEAST_NEEDLE = 442,
  BATTLE_EFFECT_POISON_STING = 443,
  BATTLE_EFFECT_STUN_STING = 444,
  BATTLE_EFFECT_DEMON_EYE = 445,
  BATTLE_EFFECT_EMU = 446,
  BATTLE_EFFECT_OCEAN_FIST = 447,
  BATTLE_EFFECT_CALL_DULLAHAN = 448,
  BATTLE_EFFECT_GHOST_SOLDIER = 449,
  BATTLE_EFFECT_RISING_DRAGON_ENEMY = 450,
  BATTLE_EFFECT_TRUE_COLLIDE = 451,
  BATTLE_EFFECT_STAR_MINE = 452,
  BATTLE_EFFECT_FORMIC_ACID = 453,
  BATTLE_EFFECT_CLAW_ATTACK = 454,
  BATTLE_EFFECT_BEAT_DANCE = 455,
  BATTLE_EFFECT_UNUSED_SPARK_BITE = 456, ///< no ability uses it
  BATTLE_EFFECT_LIVING_ARMOR = 457,
  BATTLE_EFFECT_HEAT_STUN = 458,
  BATTLE_EFFECT_EARTH_FORCE = 459,
  BATTLE_EFFECT_GUARD_AURA = 460,
  BATTLE_EFFECT_DJINN_BLAST = 461,
  BATTLE_EFFECT_DJINN_STORM = 462,
  BATTLE_EFFECT_MINE_BALL = 463,
  BATTLE_EFFECT_ELEMENT_SWAP = 464,
  BATTLE_EFFECT_DJINN_STUN = 465,
  BATTLE_EFFECT_MYSTIC_CALL = 466,
  BATTLE_EFFECT_CLARION_CRY = 467,
  BATTLE_EFFECT_PSY_BOOST = 468,
  BATTLE_EFFECT_STRONG_HIT = 469,
  BATTLE_EFFECT_CHILL_BREATH = 470,
  BATTLE_EFFECT_STORM_BREATH_STRONG = 471,
  BATTLE_EFFECT_COUNT = 472 ///< one more than the last kind
};

typedef DrawRectangle BlitterPair[2];

/// A texture as the render jobs take it: 1 << logWidth by 1 << logHeight.
struct Texture {
  u8 logWidth;
  u8 logHeight;
  u16 padding;
  void *pixels;
};

/// A point or a velocity, 16.16 pixels.
struct Point2D {
  FIXED x;
  FIXED y;
};

/// A sprite's scale across and down, 16.16, as an effect keeps it in a local
/// (struct SpriteScale is the same pair as a union with an array).
struct Scale {
  FIXED x;
  FIXED y;
};

/// An effect's variant (BattleEffectParams.variant): abilities of one family
/// that share an effect, such as Quake, Earthquake and Quake Sphere, and a
/// double or triple blow adds one or two. A runner that sets its own variants
/// names them beside it.
enum EffectVariant {
  EFFECT_VARIANT_BASIC,    ///< Quake, Douse, Bolt
  EFFECT_VARIANT_STRONGER, ///< Earthquake, Drench, Flash Bolt
  EFFECT_VARIANT_STRONGEST ///< Quake Sphere, Deluge, Blue Bolt
};

/// BattleMotion_ApplyKnockback's knockbacks, the columns of the
/// gBattleKnockback_* tables. Each is named by how far and how high it sends
/// the unit, as percent of its spot's distance from the centre and its hop in
/// pixels a frame; all but KNOCKBACK_HURL and KNOCKBACK_HURL_FAR accelerate by
/// 1 pixel a frame to at most 4.
enum BattleKnockback {
  KNOCKBACK_NUDGE,  ///< 140 percent, a hop of 3
  KNOCKBACK_HOP,    ///< 140 percent, a hop of 6
  KNOCKBACK_LEAP,   ///< 150 percent, a hop of 7
  KNOCKBACK_LAUNCH, ///< 200 percent, a hop of 10
  /// The hardest: 200 percent at 8 pixels a frame more each frame, to at
  /// most 20, with a hop of 5.
  KNOCKBACK_HURL,
  /// 150 percent, a hop of 2. From here on a unit already in the air takes
  /// the new hop too.
  KNOCKBACK_SKID,
  KNOCKBACK_BOUNCE, ///< the spot itself, a hop of 3
  KNOCKBACK_JOLT,   ///< the spot itself, a hop of 5.5
  /// Unused: the spot itself, a hop of 10; no knockback names it.
  KNOCKBACK_LIFT,
  /// Unused: 300 percent, as KNOCKBACK_HURL otherwise; no knockback names it.
  KNOCKBACK_HURL_FAR
};

/// A battle background's id, as the summons read it: the word and the bytes
/// give the read the alias sets of the stores around it.
union BattleBackgroundId {
  u16 id;
  s32 word;
  u8 bytes[6];
};

void BattleEffect_UpdatePhasePalette(s32 frame, s32 redPhase, s32 greenPhase,
                                     s32 bluePhase);
/// The canvas layer and its helpers.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_CountDownBattlerTints(void);
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags);
void BattleEffect_TintBattler(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                              s32 timerFrames);
void BattleEffect_DrawShadedCircles(u8 *map, s32 size, FIXED scale);
void Picture_MirrorIntoQuadrants(u8 *src, u8 *dst, s32 width, s32 height);
void BattleEffect_RunDjinniEntrance(struct BattleEffectParams *effect,
                                    u32 packed, s32 *screen, s32 *world);
void Canvas_BlitAtFoot(void *source, s32 x, s32 y, s32 size);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void EffectPosition_ProjectBattler(s32 id, struct ScreenPoint *position);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind, u32 variant);
void BattleEffect_SelectLivingTargets(struct BattleEffectParams *party);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
/// Moves a particle one frame on the screen: its velocity scaled by `damping`
/// sixty-fourths, then `gravity` added to its y speed.
void Particle_AdvanceWithGravity2D(struct Particle *step, s32 damping,
                                   s32 gravity);
void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);
/// Moves a particle one frame in three dimensions: its velocity scaled by
/// `damping` sixty-fourths, then `gravity` added to its y speed.
void Particle_AdvanceWithGravity3D(struct Particle *step, s32 damping,
                                   s32 gravity);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void RagingHeat_FillGradientCanvas(void);
void BattleEffect_AdvanceScrollOnInterval(void);
s32 BattleEffect_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift,
                              s32 flags, u32 mode);
void Palette_BrightenBattleBg(s32 redStep, s32 greenStep, s32 blueStep);
void Palette_BrightenSavedBattleBg(s32 redStep, s32 greenStep, s32 blueStep);
void Mesh_BuildCylinderBand(struct MeshVertex *vertices,
                            struct MeshTriangle *triangles, s32 segments);
void Graphics_LoadGreyRamp(void); // a grey ramp from color 1
void Mesh_BuildGrid(struct MeshTriangle *triangles, s32 columns, s32 rows,
                    s32 cellWidth, s32 cellHeight);
void Palette_BrightenBgEntries(s32 blueStep, s32 greenStep, s32 redStep);
void BattleEffect_ConfigureEffectDisplay(void);
void BattleEffect_ClearActorTimers(void);
void BattleEffect_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1,
                                        s32 color);
void BattleEffect_SetupCanvasWindows(void); // the BG2 canvas's registers
void BattleEffect_DrawShadedCirclesMirrored(u8 *map, s32 size, s32 scale);
void BattleEffect_RunRamses(struct BattleEffectParams *effect);
void BattleEffect_RunNereid(struct BattleEffectParams *effect);
void BattleEffect_RunKirin(struct BattleEffectParams *effect);
void BattleEffect_RunAtalanta(struct BattleEffectParams *effect);
void BattleEffect_RunNeptune(struct BattleEffectParams *effect);
void BattleEffect_RunProcne(struct BattleEffectParams *effect);
void BattleEffect_RunJudgment(struct BattleEffectParams *effect);
void BattleEffect_RunBoreas(struct BattleEffectParams *effect);
void BattleEffect_RunThor(struct BattleEffectParams *effect);
void BattleEffect_RunMegaera(struct BattleEffectParams *effect);
void BattleEffect_RunFlora(struct BattleEffectParams *effect);
void BattleEffect_RunMoloch(struct BattleEffectParams *effect);
void BattleEffect_RunUlysses(struct BattleEffectParams *effect);
void BattleEffect_RunHaures(struct BattleEffectParams *effect);
void BattleEffect_RunEclipse(struct BattleEffectParams *effect);
void BattleEffect_RunCoatlicue(struct BattleEffectParams *effect);
void BattleEffect_RunDaedalus(struct BattleEffectParams *args);
void BattleEffect_RunAzul(struct BattleEffectParams *effect);
void BattleEffect_RunCatastrophe(struct BattleEffectParams *effect);
void BattleEffect_RunCharon(struct BattleEffectParams *arg);
void BattleEffect_RunIris(struct BattleEffectParams *effect);
void BattleEffect_RunDaedalusStrike(struct BattleEffectParams *effect);
void BattleEffect_RunTrident(struct BattleEffectParams *effect);
void BattleEffect_RunCruelRuin(struct BattleEffectParams *effect);
void BattleEffect_RunMeteorBlow(struct BattleEffectParams *effect);
void BattleEffect_RunDjinnBeams(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunLobbedShells(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunImpair(struct BattleEffectParams *args);
void BattleEffect_RunDull(struct BattleEffectParams *args);
void BattleEffect_RunGatheringLights(struct BattleEffectParams *effect,
                                     s32 mode);
void BattleEffect_ArmBg1HBlankDma(void); // arms the BG1 scroll's HBlank DMA
void Palette_MirrorBgColors(void);       // mode 3 only
void BattleEffect_SetupBlendedDisplay(void);
void BattleEffect_RunDragonFire(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_PlaceFormationObjects(s32 formation, s32 x,
                                        s32 y); // ..PlaceFormationObjects
void BattleEffect_RunImpactBurst(s32 formation, s32 x,
                                 FIXED y); // ..RunImpactBurst
void BattleEffect_RunStreakVolley(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunMoteFountain(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_GetTargetsMidpoint(struct ScreenPoint *midpoint);
void BattleEffect_RunTumblingQuads(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunTargetStrike(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_SetApproachMotion(s32 actor, s32 target, s32 frames,
                                    s32 percent);
void BattleEffect_RunCloudSwirl(struct BattleEffectParams *effect);
void BattleCamera_ApplyPhasedDelta(void);
typedef s32 (*CopyWordsFn)(void *destination, const void *source, s32 size);
/// The window BattleEffect_BuildCenteredIndexList builds a list of indices
/// around: its value, radius and limit, and the values it lists.
typedef struct {
  s32 reservedA[3];
  s32 currentValue;
  s32 indexRadius;
  s32 entryLimit;
  s32 reservedB[3];
  s16 values[1];
} BattleEffectIndexWindow;
/// What BattleEffect_RunDjinniEntrance takes added to its element; the element
/// alone plays the 55-frame entrance.
enum DjinniEntranceLength {
  /// The long entrance: 84 frames instead of 55.
  DJINNI_ENTRANCE_LONG = ELEMENT_COUNT
};

/// Where BattleEffect_RunDjinniEntrance's Djinni appears: the first five 60
/// pixels up.
enum DjinniEntranceAnchor {
  DJINNI_ANCHOR_OVER_TARGET, ///< over the first target
  DJINNI_ANCHOR_TARGET_X,    ///< at the first target's x, at depth 0
  DJINNI_ANCHOR_OVER_USER,
  DJINNI_ANCHOR_USER_X,      ///< at the user's x, at depth 0
  DJINNI_ANCHOR_CENTRE,      ///< in the centre of the field
  DJINNI_ANCHOR_HIGH_USER_X, ///< at the user's x, 80 pixels up
  /// In the centre: the entrance takes 40 frames with no fade, and the
  /// backdrop brightens from frame 24.
  DJINNI_ANCHOR_CENTRE_BRIGHTENING,
  DJINNI_ANCHOR_LOW_CENTRE, ///< in the centre, 20 pixels up
  /// Over the first target: the entrance takes 54 frames.
  DJINNI_ANCHOR_OVER_TARGET_SHORT,
  DJINNI_ANCHOR_HIGH_TARGET_X, ///< at the first target's x, 120 pixels up
  DJINNI_ANCHOR_LOW_USER_X     ///< at the user's x, 30 pixels up
};

/// BattleEffect_RunDjinniEntrance's packed argument: which Djinni (an element,
/// plus DJINNI_ENTRANCE_LONG for the longer entrance), the side whose way it
/// faces, and where it appears (enum DjinniEntranceAnchor).
#define DJINNI_ENTRANCE(element, side, anchor)                                 \
  ((element) | (side) << 4 | (anchor) << 5)
#define DJINNI_ENTRANCE_ELEMENT(packed) ((packed) & 7)
#define DJINNI_ENTRANCE_SIDE(packed) (((packed) >> 4) & 1)
#define DJINNI_ENTRANCE_ANCHOR(packed) (((packed) >> 5) & 0xf)

/// A row of gQuake_Timings, one for each variant.
enum {
  TIMING_FRAMES,      ///< the whole effect
  TIMING_RISE_FRAMES, ///< how long each pillar stands
  TIMING_SHAKE        ///< the camera shake
};

/// The four rock pillars' heights, in pixels.
struct RockWallHeights {
  s32 column[4]; ///< each pillar's height in pixels
};

/// The five widths a rising pillar takes.
struct FiveWidths {
  s32 widths[5];
};

/// The seven widths a widening pillar takes.
struct SevenWidths {
  s32 widths[7];
};

/// Where a strip's petals start across, by quarter of its rise: the left edge
/// and the width they spread over.
struct FourSpans {
  struct {
    s32 left;
    s32 width;
  } spans[4];
};

/// A row of gFrost_Timings, one for each variant.
enum {
  RISE_MOTES,   ///< the rising motes
  RISE_COLUMNS, ///< the pillars
  RISE_FRAME    ///< the frame they burst
};

/// A row of gEruption_Counts, one for each variant.
enum { FIELD_MOTES, FIELD_PUFFS, FIELD_FRAMES };

/// Places in the sheet.
enum VolleyBSheet {
  VOLLEY_B_SHOTS = 0,      ///< the mode's own shot pictures
  VOLLEY_B_BEAM = 4096,    ///< the beam, doubled to 48 wide
  VOLLEY_B_GLOW = 5266,    ///< mode 20's glows
  VOLLEY_B_PICTURE = 3158, ///< the mode's picture
  VOLLEY_B_DJINNI = 26048  ///< the 20x40 Djinni
};

/// A growing column, laid over struct Particle in work->particles.
struct DualColumn {
  s32 x;                   ///< the root on the canvas
  s32 y;                   ///< the root on the canvas
  u8 unused[8];            ///< no code names it
  s32 height;              ///< the full length
  u8 unusedAfterHeight[4]; ///< no code names it
  s32 start;               ///< the frame it starts to grow
};

void BattleEffect_StepPaletteToResource(s32 resourceId);
void BattleEffect_WipeCanvas(s32 direction, s32 fill);
void EffectPosition_ProjectSprite(s32 id, struct ScreenPoint *position);
void BattleEffect_BuildWindowEdgeTable(void);
void BattleEffect_RunDrainMotes(struct BattleEffectParams *effect, s32 mode);
extern const struct Scale gBoreas_UnitScale;
void BattleEffect_RunStatBoost(struct BattleEffectParams *effect, u32 mode);
extern const struct RockWallHeights gRockWall_Heights;
void BattleEffect_RunEruption(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunGrowingColumns(struct BattleEffectParams *effect,
                                    s32 mode);
/// Copies `size` bytes of colour levels, halving each.
void ColorBuffer_Halve(void *source, void *destination, s32 size);
/// Copies `size` bytes of colour levels at three quarters of each.
void ColorBuffer_ScaleThreeQuarters(void *source, void *destination, s32 size);
/// Copies `size` bytes of colour levels, each lowered by `amount`.
void ColorBuffer_Darken(void *source, s32 amount, void *destination, s32 size);
/// Copies `size` bytes of colour levels, each raised by `amount`.
void ColorBuffer_Brighten(void *source, s32 amount, void *destination,
                          s32 size);
/// The high word of value times magic, plus value times multiplier: a division
/// by a constant.
s32 Math_UnsignedMulHighAdd(s32 value, u32 magic, s32 multiplier);

// The effect handlers, in the order the handler table (gBattleEffect_Handlers)
// first lists them.

void BattleEffect_RunQuake(struct BattleEffectParams *args);
void BattleEffect_RunSpire(struct BattleEffectParams *args);
void BattleEffect_RunGaia(struct BattleEffectParams *args);
void BattleEffect_RunGrowth(struct BattleEffectParams *args);
void BattleEffect_RunThorn(struct BattleEffectParams *args);
void BattleEffect_RunFrost(struct BattleEffectParams *args);
void BattleEffect_RunIce(struct BattleEffectParams *args);
void BattleEffect_RunPrism(struct BattleEffectParams *args);
void BattleEffect_RunDouse(struct BattleEffectParams *args);
void BattleEffect_RunFroth(struct BattleEffectParams *args);
void BattleEffect_RunFlare(struct BattleEffectParams *args);
void BattleEffect_RunFire(struct BattleEffectParams *args);
void BattleEffect_RunVolcano(struct BattleEffectParams *args);
void BattleEffect_RunBlast(struct BattleEffectParams *args);
void BattleEffect_RunNova(struct BattleEffectParams *args);
void BattleEffect_RunBolt(struct BattleEffectParams *args);
void BattleEffect_RunRay(struct BattleEffectParams *args);
void BattleEffect_RunPlasma(struct BattleEffectParams *args);
void BattleEffect_RunSlash(struct BattleEffectParams *args);
void BattleEffect_RunWhirlwind(struct BattleEffectParams *args);
void BattleEffect_RunCure(struct BattleEffectParams *args);
void BattleEffect_RunPly(struct BattleEffectParams *args);
void BattleEffect_RunWish(struct BattleEffectParams *args);
void BattleEffect_RunCurePoison(struct BattleEffectParams *args);
void BattleEffect_RunRevive(struct BattleEffectParams *args);
void BattleEffect_RunImpact(struct BattleEffectParams *args);
void BattleEffect_RunDull(struct BattleEffectParams *args);
void BattleEffect_RunGuard(struct BattleEffectParams *args);
void BattleEffect_RunImpair(struct BattleEffectParams *args);
void BattleEffect_RunWard(struct BattleEffectParams *args);
void BattleEffect_RunWeaken(struct BattleEffectParams *args);
void BattleEffect_RunTaint(struct BattleEffectParams *args);
void BattleEffect_RunDelude(struct BattleEffectParams *args);
void BattleEffect_RunUnusedOrbitingSpirals(struct BattleEffectParams *args);
void BattleEffect_RunHeartrender(struct BattleEffectParams *args);
void BattleEffect_RunParalyze(struct BattleEffectParams *args);
void BattleEffect_RunSleep(struct BattleEffectParams *args);
void BattleEffect_RunBind(struct BattleEffectParams *args);
void BattleEffect_RunHaunt(struct BattleEffectParams *args);
void BattleEffect_RunCondemn(struct BattleEffectParams *args);
void BattleEffect_RunCurse(struct BattleEffectParams *args);
void BattleEffect_RunDrain(struct BattleEffectParams *args);
void BattleEffect_RunBreak(struct BattleEffectParams *args);
void BattleEffect_RunDragonCloud(struct BattleEffectParams *args);
void BattleEffect_RunDemonNight(struct BattleEffectParams *args);
void BattleEffect_RunHelmSplitter(struct BattleEffectParams *args);
void BattleEffect_RunQuickStrike(struct BattleEffectParams *args);
void BattleEffect_RunDeathPlunge(struct BattleEffectParams *args);
void BattleEffect_RunShuriken(struct BattleEffectParams *args);
void BattleEffect_RunAnnihilation(struct BattleEffectParams *args);
void BattleEffect_RunPunji(struct BattleEffectParams *args);
void BattleEffect_RunCuttingEdge(struct BattleEffectParams *args);
void BattleEffect_RunRagnarok(struct BattleEffectParams *args);
void BattleEffect_RunHeatWave(struct BattleEffectParams *args);
void BattleEffect_RunPlanetDiver(struct BattleEffectParams *args);
void BattleEffect_RunTitanBlade(struct BattleEffectParams *args);
void BattleEffect_RunShiningStar(struct BattleEffectParams *args);
void BattleEffect_RunBlizzard(struct BattleEffectParams *args);
void BattleEffect_RunDemonFire(struct BattleEffectParams *args);
void BattleEffect_RunVorpalSlash(struct BattleEffectParams *args);
void BattleEffect_RunDrown(struct BattleEffectParams *args);
void BattleEffect_RunUnusedTriangleStrike(struct BattleEffectParams *args);
void BattleEffect_RunAstralBlast(struct BattleEffectParams *args);
void BattleEffect_RunHeartbreak(struct BattleEffectParams *args);
void BattleEffect_RunAura(struct BattleEffectParams *args);
void BattleEffect_RunFume(struct BattleEffectParams *args);
void BattleEffect_RunBeam(struct BattleEffectParams *args);
void BattleEffect_RunRagingHeat(struct BattleEffectParams *args);
void BattleEffect_RunCool(struct BattleEffectParams *args);
void BattleEffect_RunSabreDance(struct BattleEffectParams *args);
void BattleEffect_RunBackstab(struct BattleEffectParams *args);
void BattleEffect_RunJuggle(struct BattleEffectParams *args);
void BattleEffect_RunFlameCard(struct BattleEffectParams *args);
void BattleEffect_RunWhiplash(struct BattleEffectParams *args);
void BattleEffect_RunPoisonFlow(struct BattleEffectParams *args);
void BattleEffect_RunFirePuppet(struct BattleEffectParams *args);
void BattleEffect_RunDiamondDust(struct BattleEffectParams *args);
void BattleEffect_RunDiamondBerg(struct BattleEffectParams *args);
void BattleEffect_RunPlumeEdge(struct BattleEffectParams *args);
void BattleEffect_RunThunderMine(struct BattleEffectParams *args);
void BattleEffect_RunFrostCard(struct BattleEffectParams *args);
void BattleEffect_RunBrambleCard(struct BattleEffectParams *args);
void BattleEffect_RunThunderCard(struct BattleEffectParams *args);
void BattleEffect_RunBaffleCard(struct BattleEffectParams *args);
void BattleEffect_RunSwordCard(struct BattleEffectParams *args);
void BattleEffect_RunSleepCard(struct BattleEffectParams *args);
void BattleEffect_RunDeathCard(struct BattleEffectParams *args);
void BattleEffect_RunStoneJustice(struct BattleEffectParams *args);
void BattleEffect_RunHeavyDivide(struct BattleEffectParams *args);
void BattleEffect_RunWyrdCurse(struct BattleEffectParams *args);
void BattleEffect_RunAcheronsGrief(struct BattleEffectParams *args);
void BattleEffect_RunHurricane(struct BattleEffectParams *args);
void BattleEffect_RunDreamtide(struct BattleEffectParams *args);
void BattleEffect_RunLetheAlbion(struct BattleEffectParams *args);
void BattleEffect_RunRisingDragon(struct BattleEffectParams *args);
void BattleEffect_RunBoostHack(struct BattleEffectParams *args);
void BattleEffect_RunRadiantFire(struct BattleEffectParams *args);
void BattleEffect_RunFireDance(struct BattleEffectParams *args);
void BattleEffect_RunBlazeRush(struct BattleEffectParams *args);
void BattleEffect_RunShred(struct BattleEffectParams *args);
void BattleEffect_RunPurgatory(struct BattleEffectParams *args);
void BattleEffect_RunSoulShatter(struct BattleEffectParams *args);
void BattleEffect_RunAgingGas(struct BattleEffectParams *args);
void BattleEffect_RunMadZephyr(struct BattleEffectParams *args);
void BattleEffect_RunLunarSlash(struct BattleEffectParams *args);
void BattleEffect_RunNirvana(struct BattleEffectParams *args);
void BattleEffect_RunLightSurge(struct BattleEffectParams *args);
void BattleEffect_RunVoidBeam(struct BattleEffectParams *args);
void BattleEffect_RunLegend(struct BattleEffectParams *args);
void BattleEffect_RunLiquifier(struct BattleEffectParams *args);
void BattleEffect_RunMegiddo(struct BattleEffectParams *args);
void BattleEffect_RunOdyssey(struct BattleEffectParams *args);
void BattleEffect_RunVengeance(struct BattleEffectParams *args);
void BattleEffect_RunThornyGrave(struct BattleEffectParams *args);
void BattleEffect_RunPlanetary(struct BattleEffectParams *args);
void BattleEffect_RunLifeShear(struct BattleEffectParams *args);
void BattleEffect_RunCrucibleFire(struct BattleEffectParams *args);
void BattleEffect_RunReverseStar(struct BattleEffectParams *args);
void BattleEffect_RunGranite(struct BattleEffectParams *args);
void BattleEffect_RunVine(struct BattleEffectParams *args);
void BattleEffect_RunGround(struct BattleEffectParams *args);
void BattleEffect_RunSleet(struct BattleEffectParams *args);
void BattleEffect_RunHail(struct BattleEffectParams *args);
void BattleEffect_RunTonic(struct BattleEffectParams *args);
void BattleEffect_RunDew(struct BattleEffectParams *args);
void BattleEffect_RunPsyCrystal(struct BattleEffectParams *args);
void BattleEffect_RunFlash(struct BattleEffectParams *args);
void BattleEffect_RunTorch(struct BattleEffectParams *args);
void BattleEffect_RunZephyr(struct BattleEffectParams *args);
void BattleEffect_RunKite(struct BattleEffectParams *args);
void BattleEffect_RunGust(struct BattleEffectParams *args);
void BattleEffect_RunIron(struct BattleEffectParams *args);
void BattleEffect_RunSteam(struct BattleEffectParams *args);
void BattleEffect_RunShade(struct BattleEffectParams *args);
void BattleEffect_RunCoal(struct BattleEffectParams *args);
void BattleEffect_RunSalt(struct BattleEffectParams *args);
void BattleEffect_RunSpring(struct BattleEffectParams *args);
void BattleEffect_RunBreath(struct BattleEffectParams *args);
void BattleEffect_RunEther(struct BattleEffectParams *args);
void BattleEffect_RunBalm(struct BattleEffectParams *args);
void BattleEffect_RunTinder(struct BattleEffectParams *args);
void BattleEffect_RunWhorl(struct BattleEffectParams *args);
void BattleEffect_RunGale(struct BattleEffectParams *args);
void BattleEffect_RunWaft(struct BattleEffectParams *args);
void BattleEffect_RunCore(struct BattleEffectParams *args);
void BattleEffect_RunMud(struct BattleEffectParams *args);
void BattleEffect_RunHaze(struct BattleEffectParams *args);
void BattleEffect_RunPetra(struct BattleEffectParams *args);
void BattleEffect_RunRime(struct BattleEffectParams *args);
void BattleEffect_RunLull(struct BattleEffectParams *args);
void BattleEffect_RunGasp(struct BattleEffectParams *args);
void BattleEffect_RunFugue(struct BattleEffectParams *args);
void BattleEffect_RunCrystal(struct BattleEffectParams *args);
void BattleEffect_RunEddy(struct BattleEffectParams *args);
void BattleEffect_RunCannon(struct BattleEffectParams *args);
void BattleEffect_RunAroma(struct BattleEffectParams *args);
void BattleEffect_RunReflux(struct BattleEffectParams *args);
void BattleEffect_RunDjinnCounter(struct BattleEffectParams *args);
void BattleEffect_RunMold(struct BattleEffectParams *args);
void BattleEffect_RunMeld(struct BattleEffectParams *args);
void BattleEffect_RunShine(struct BattleEffectParams *args);
void BattleEffect_RunGeode(struct BattleEffectParams *args);
void BattleEffect_RunSerac(struct BattleEffectParams *args);
void BattleEffect_RunSpark(struct BattleEffectParams *args);
void BattleEffect_RunFury(struct BattleEffectParams *args);
void BattleEffect_RunChar(struct BattleEffectParams *args);
void BattleEffect_RunVenus(struct BattleEffectParams *args);
void BattleEffect_RunMercury(struct BattleEffectParams *args);
void BattleEffect_RunMars(struct BattleEffectParams *args);
void BattleEffect_RunJupiter(struct BattleEffectParams *args);
void BattleEffect_RunSonicWave(struct BattleEffectParams *args);
void BattleEffect_RunShriek(struct BattleEffectParams *args);
void BattleEffect_RunBansheeHowl(struct BattleEffectParams *args);
void BattleEffect_RunCrazyVoice(struct BattleEffectParams *args);
void BattleEffect_RunWarCry(struct BattleEffectParams *args);
void BattleEffect_RunWickedHowl(struct BattleEffectParams *args);
void BattleEffect_RunWingBeat(struct BattleEffectParams *args);
void BattleEffect_RunWingFlutter(struct BattleEffectParams *args);
void BattleEffect_RunRumble(struct BattleEffectParams *args);
void BattleEffect_RunBoneChiller(struct BattleEffectParams *args);
void BattleEffect_RunBoneCharge(struct BattleEffectParams *args);
void BattleEffect_RunMysticFlame(struct BattleEffectParams *args);
void BattleEffect_RunNumbingSting(struct BattleEffectParams *args);
void BattleEffect_RunBruteForce(struct BattleEffectParams *args);
void BattleEffect_RunStickyGoo(struct BattleEffectParams *args);
void BattleEffect_RunCannibalFang(struct BattleEffectParams *args);
void BattleEffect_RunBearClaw(struct BattleEffectParams *args);
void BattleEffect_RunPoisonousBite(struct BattleEffectParams *args);
void BattleEffect_RunFlyingAttack(struct BattleEffectParams *args);
void BattleEffect_RunUndeadSword(struct BattleEffectParams *args);
void BattleEffect_RunRansack(struct BattleEffectParams *args);
void BattleEffect_RunStickyPoison(struct BattleEffectParams *args);
void BattleEffect_RunElectricBite(struct BattleEffectParams *args);
void BattleEffect_RunPoisonTail(struct BattleEffectParams *args);
void BattleEffect_RunOnslaught(struct BattleEffectParams *args);
void BattleEffect_RunVampiricFang(struct BattleEffectParams *args);
void BattleEffect_RunBacteriaRush(struct BattleEffectParams *args);
void BattleEffect_RunRottenBlood(struct BattleEffectParams *args);
void BattleEffect_RunDoubleFang(struct BattleEffectParams *args);
void BattleEffect_RunMortalBlow(struct BattleEffectParams *args);
void BattleEffect_RunFreebiteRush(struct BattleEffectParams *args);
void BattleEffect_RunTwinBeaks(struct BattleEffectParams *args);
void BattleEffect_RunRabidFang(struct BattleEffectParams *args);
void BattleEffect_RunAcidBite(struct BattleEffectParams *args);
void BattleEffect_RunDynamite(struct BattleEffectParams *args);
void BattleEffect_RunHeadbutt(struct BattleEffectParams *args);
void BattleEffect_RunPoisonInk(struct BattleEffectParams *args);
void BattleEffect_RunSoothingStar(struct BattleEffectParams *args);
void BattleEffect_RunSpiderWeb(struct BattleEffectParams *args);
void BattleEffect_RunMadSpatter(struct BattleEffectParams *args);
void BattleEffect_RunSpasm(struct BattleEffectParams *args);
void BattleEffect_RunSleepStar(struct BattleEffectParams *args);
void BattleEffect_RunDecompose(struct BattleEffectParams *args);
void BattleEffect_RunHaunting(struct BattleEffectParams *args);
void BattleEffect_RunBerserk(struct BattleEffectParams *args);
void BattleEffect_RunSmokeBomb(struct BattleEffectParams *args);
void BattleEffect_RunWingStroke(struct BattleEffectParams *args);
void BattleEffect_RunPoisonBeat(struct BattleEffectParams *args);
void BattleEffect_RunSpinningBeat(struct BattleEffectParams *args);
void BattleEffect_RunOuterSpace(struct BattleEffectParams *args);
void BattleEffect_RunDragonDriver(struct BattleEffectParams *args);
void BattleEffect_RunDrainFang(struct BattleEffectParams *args);
void BattleEffect_RunSevereBlow(struct BattleEffectParams *args);
void BattleEffect_RunThrash(struct BattleEffectParams *args);
void BattleEffect_RunRecovery(struct BattleEffectParams *args);
void BattleEffect_RunEchoCut(struct BattleEffectParams *args);
void BattleEffect_RunWateryGrave(struct BattleEffectParams *args);
void BattleEffect_RunRagingFlood(struct BattleEffectParams *args);
void BattleEffect_RunHydroSlash(struct BattleEffectParams *args);
void BattleEffect_RunMightyPress(struct BattleEffectParams *args);
void BattleEffect_RunSlaver(struct BattleEffectParams *args);
void BattleEffect_RunCounterRush(struct BattleEffectParams *args);
void BattleEffect_RunDjinnfest(struct BattleEffectParams *args);
void BattleEffect_RunHeatKiss(struct BattleEffectParams *args);
void BattleEffect_RunAngleSpike(struct BattleEffectParams *args);
void BattleEffect_RunFireBreath(struct BattleEffectParams *args);
void BattleEffect_RunWaterBreath(struct BattleEffectParams *args);
void BattleEffect_RunIceBlessing(struct BattleEffectParams *args);
void BattleEffect_RunDarkBreath(struct BattleEffectParams *args);
void BattleEffect_RunAcidBreath(struct BattleEffectParams *args);
void BattleEffect_RunStormBreath(struct BattleEffectParams *args);
void BattleEffect_RunEvilBlessing(struct BattleEffectParams *args);
void BattleEffect_RunDeadlyGas(struct BattleEffectParams *args);
void BattleEffect_RunSandBreath(struct BattleEffectParams *args);
void BattleEffect_RunDesertGasp(struct BattleEffectParams *args);
void BattleEffect_RunBlackIce(struct BattleEffectParams *args);
void BattleEffect_RunTyphoonBlow(struct BattleEffectParams *args);
void BattleEffect_RunToxicBreath(struct BattleEffectParams *args);
void BattleEffect_RunGravelBlow(struct BattleEffectParams *args);
void BattleEffect_RunBlastBreath(struct BattleEffectParams *args);
void BattleEffect_RunDarksolGasp(struct BattleEffectParams *args);
void BattleEffect_RunManeater(struct BattleEffectParams *args);
void BattleEffect_RunUrFlash(struct BattleEffectParams *args);
void BattleEffect_RunCrusherGrip(struct BattleEffectParams *args);
void BattleEffect_RunVanishClaw(struct BattleEffectParams *args);
void BattleEffect_RunTwinShear(struct BattleEffectParams *args);
void BattleEffect_RunTripleChomp(struct BattleEffectParams *args);
void BattleEffect_RunDoublestep(struct BattleEffectParams *args);
void BattleEffect_RunPoisonGel(struct BattleEffectParams *args);
void BattleEffect_RunRisingVenom(struct BattleEffectParams *args);
void BattleEffect_RunSack(struct BattleEffectParams *args);
void BattleEffect_RunFireDanceUnleash(struct BattleEffectParams *args);
void BattleEffect_RunStunMuscle(struct BattleEffectParams *args);
void BattleEffect_RunRollingFlame(struct BattleEffectParams *args);
void BattleEffect_RunCage(struct BattleEffectParams *args);
void BattleEffect_RunPhoenix(struct BattleEffectParams *args);
void BattleEffect_RunHumanHunt(struct BattleEffectParams *args);
void BattleEffect_RunUnusedHeavyBite(struct BattleEffectParams *args);
void BattleEffect_RunUnusedFlashBite(struct BattleEffectParams *args);
void BattleEffect_RunKillSting(struct BattleEffectParams *args);
void BattleEffect_RunBeastNeedle(struct BattleEffectParams *args);
void BattleEffect_RunPoisonSting(struct BattleEffectParams *args);
void BattleEffect_RunStunSting(struct BattleEffectParams *args);
void BattleEffect_RunDemonEye(struct BattleEffectParams *args);
void BattleEffect_RunEmu(struct BattleEffectParams *args);
void BattleEffect_RunOceanFist(struct BattleEffectParams *args);
void BattleEffect_RunCallDullahan(struct BattleEffectParams *args);
void BattleEffect_RunGhostSoldier(struct BattleEffectParams *args);
void BattleEffect_RunRisingDragonEnemy(struct BattleEffectParams *args);
void BattleEffect_RunTrueCollide(struct BattleEffectParams *args);
void BattleEffect_RunStarMine(struct BattleEffectParams *args);
void BattleEffect_RunFormicAcid(struct BattleEffectParams *args);
void BattleEffect_RunClawAttack(struct BattleEffectParams *args);
void BattleEffect_RunBeatDance(struct BattleEffectParams *args);
void BattleEffect_RunUnusedSparkBite(struct BattleEffectParams *args);
void BattleEffect_RunLivingArmor(struct BattleEffectParams *args);
void BattleEffect_RunHeatStun(struct BattleEffectParams *args);
void BattleEffect_RunEarthForce(struct BattleEffectParams *args);
void BattleEffect_RunGuardAura(struct BattleEffectParams *args);
void BattleEffect_RunDjinnBlast(struct BattleEffectParams *args);
void BattleEffect_RunDjinnStorm(struct BattleEffectParams *args);
void BattleEffect_RunMineBall(struct BattleEffectParams *args);
void BattleEffect_RunElementSwap(struct BattleEffectParams *args);
void BattleEffect_RunDjinnStun(struct BattleEffectParams *args);
void BattleEffect_RunMysticCall(struct BattleEffectParams *args);
void BattleEffect_RunClarionCry(struct BattleEffectParams *args);
void BattleEffect_RunPsyBoost(struct BattleEffectParams *args);
void BattleEffect_RunStrongHit(struct BattleEffectParams *args);
void BattleEffect_RunChillBreath(struct BattleEffectParams *args);
void BattleEffect_RunStormBreathStrong(struct BattleEffectParams *args);

#endif
