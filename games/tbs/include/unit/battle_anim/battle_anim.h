#ifndef ALCHEMY_UNIT_BATTLE_ANIM_BATTLE_ANIM_H
#define ALCHEMY_UNIT_BATTLE_ANIM_BATTLE_ANIM_H

#include "types.h"
#include "render_placement.h"
#include "battle_anim_work.h"
#include "blitter.h"

struct BattleAnimArgs;
struct ScreenPoint;
struct Particle;

/// The large animations BattleAnim_RunSummon runs, by kind: the twelve
/// summons. A summon's row of gAbilityAnimations has ANIM_SUMMON plus its
/// kind.
enum SummonAnimId {
  SUMMON_ANIM_NONE,     ///< no row uses it; runs Meteor's animation too
  SUMMON_ANIM_RAMSES,   ///< BattleAnim_RunRamses (ramses.c)
  SUMMON_ANIM_NEREID,   ///< BattleAnim_RunNereid (nereid.c)
  SUMMON_ANIM_KIRIN,    ///< BattleAnim_RunKirin (kirin.c)
  SUMMON_ANIM_ATALANTA, ///< BattleAnim_RunAtalanta (atalanta.c)
  SUMMON_ANIM_CYBELE,   ///< BattleAnim_RunCybele (cybele.c)
  SUMMON_ANIM_NEPTUNE,  ///< BattleAnim_RunNeptune (neptune.c)
  SUMMON_ANIM_TIAMAT,   ///< BattleAnim_RunTiamat (tiamat.c)
  SUMMON_ANIM_PROCNE,   ///< BattleAnim_RunProcne (procne.c)
  SUMMON_ANIM_JUDGMENT, ///< BattleAnim_RunJudgment (judgment.c)
  SUMMON_ANIM_BOREAS,   ///< BattleAnim_RunBoreas (boreas.c)
  SUMMON_ANIM_METEOR,   ///< BattleAnim_RunMeteor (particle_streams.c)
  SUMMON_ANIM_THOR      ///< BattleAnim_RunThor (thor.c)
};

/// An animation handler: plays one animation over the battle, frame by frame,
/// and returns when it is over.
typedef void (*BattleAnimHandler)(struct BattleAnimArgs *args);
/// Animation kinds: the kind an ability's gAbilityAnimations entry selects, by
/// the handler that plays it; a later kind that shares a handler is named for
/// the ability that uses it (the Djinn from 200, the monsters' abilities from
/// 300), or Unused when none does. Kinds without a handler have no name.
enum BattleAnimId {
  BATTLE_ANIM_NONE =
      0, ///< no animation: the dispatcher only clears the variant
  BATTLE_ANIM_QUAKE = 1,
  BATTLE_ANIM_SPIRE = 2,
  BATTLE_ANIM_GAIA = 3,
  BATTLE_ANIM_GROWTH = 4,
  BATTLE_ANIM_THORN = 5,
  BATTLE_ANIM_FROST = 6,
  BATTLE_ANIM_ICE = 7,
  BATTLE_ANIM_PRISM = 8,
  BATTLE_ANIM_DOUSE = 9,
  BATTLE_ANIM_FROTH = 10,
  BATTLE_ANIM_FLARE = 11,
  BATTLE_ANIM_FIRE = 12,
  BATTLE_ANIM_VOLCANO = 13,
  BATTLE_ANIM_BLAST = 14,
  BATTLE_ANIM_NOVA = 15,
  BATTLE_ANIM_BOLT = 16,
  BATTLE_ANIM_RAY = 17,
  BATTLE_ANIM_PLASMA = 18,
  BATTLE_ANIM_SLASH = 19,
  BATTLE_ANIM_WHIRLWIND = 20,
  BATTLE_ANIM_CURE = 21,
  BATTLE_ANIM_PLY = 22,
  BATTLE_ANIM_WISH = 23,
  BATTLE_ANIM_CURE_POISON = 24,
  BATTLE_ANIM_REVIVE = 25,
  BATTLE_ANIM_IMPACT = 26,
  BATTLE_ANIM_DULL = 27,
  BATTLE_ANIM_GUARD = 28,
  BATTLE_ANIM_IMPAIR = 29,
  BATTLE_ANIM_WARD = 30,
  BATTLE_ANIM_WEAKEN = 31,
  BATTLE_ANIM_TAINT = 32,
  BATTLE_ANIM_DELUDE = 33,
  BATTLE_ANIM_UNUSED_ORBITING_SPIRALS = 34, ///< no ability uses it
  BATTLE_ANIM_UNUSED_HEARTRENDER = 35,      ///< no ability uses it
  BATTLE_ANIM_PARALYZE = 36,
  BATTLE_ANIM_SLEEP = 37,
  BATTLE_ANIM_BIND = 38,
  BATTLE_ANIM_HAUNT = 39,
  BATTLE_ANIM_CONDEMN = 40,
  BATTLE_ANIM_CURSE = 41,
  BATTLE_ANIM_DRAIN = 42,
  BATTLE_ANIM_BREAK = 43,
  BATTLE_ANIM_REGENERATE = 44,
  BATTLE_ANIM_REFLECT = 45,
  BATTLE_ANIM_UNUSED_IMPACT = 46, ///< no ability uses it
  BATTLE_ANIM_UNUSED_IMPAIR = 47, ///< no ability uses it
  BATTLE_ANIM_DRAGON_CLOUD = 49,
  BATTLE_ANIM_DEMON_NIGHT = 50,
  BATTLE_ANIM_HELM_SPLITTER = 51,
  BATTLE_ANIM_QUICK_STRIKE = 52,
  BATTLE_ANIM_DEATH_PLUNGE = 53,
  BATTLE_ANIM_SHURIKEN = 54,
  BATTLE_ANIM_ANNIHILATION = 55,
  BATTLE_ANIM_PUNJI = 56,
  BATTLE_ANIM_CUTTING_EDGE = 57,
  BATTLE_ANIM_RAGNAROK = 58,
  BATTLE_ANIM_HEAT_WAVE = 59,
  BATTLE_ANIM_PLANET_DIVER = 60,
  BATTLE_ANIM_TITAN_BLADE = 61,
  BATTLE_ANIM_SHINING_STAR = 62,
  BATTLE_ANIM_BLIZZARD = 63,
  BATTLE_ANIM_DEMON_FIRE = 64,
  BATTLE_ANIM_VORPAL_SLASH = 65,
  BATTLE_ANIM_DROWN = 66,
  BATTLE_ANIM_UNUSED_TRIANGLE_STRIKE = 67, ///< no ability uses it
  BATTLE_ANIM_ASTRAL_BLAST = 68,
  BATTLE_ANIM_GRANITE = 200,
  BATTLE_ANIM_QUARTZ = 201,
  BATTLE_ANIM_VINE = 202,
  BATTLE_ANIM_GROUND = 203,
  BATTLE_ANIM_FIZZ = 204,
  BATTLE_ANIM_SLEET = 205,
  BATTLE_ANIM_SPRITZ = 206,
  BATTLE_ANIM_HAIL = 207,
  BATTLE_ANIM_TONIC = 208,
  BATTLE_ANIM_DEW = 209,
  BATTLE_ANIM_FORGE = 210,
  BATTLE_ANIM_CORONA = 211,
  BATTLE_ANIM_PSY_CRYSTAL = 212,
  BATTLE_ANIM_FLASH = 213,
  BATTLE_ANIM_TORCH = 214,
  BATTLE_ANIM_BREEZE = 215,
  BATTLE_ANIM_ZEPHYR = 216,
  BATTLE_ANIM_KITE = 217,
  BATTLE_ANIM_LUFF = 218,
  BATTLE_ANIM_GUST = 219,
  BATTLE_ANIM_VENUS = 250,
  BATTLE_ANIM_MERCURY = 251,
  BATTLE_ANIM_MARS = 252,
  BATTLE_ANIM_JUPITER = 253,
  BATTLE_ANIM_SONIC_WAVE = 300,
  BATTLE_ANIM_SHRIEK = 301,
  BATTLE_ANIM_BANSHEE_HOWL = 302,
  BATTLE_ANIM_CRAZY_VOICE = 303,
  BATTLE_ANIM_WAR_CRY = 304,
  BATTLE_ANIM_WICKED_HOWL = 305,
  BATTLE_ANIM_WING_BEAT = 306,
  BATTLE_ANIM_WING_FLUTTER = 307,
  BATTLE_ANIM_RUMBLE = 308,
  BATTLE_ANIM_BONE_CHILLER = 309,
  BATTLE_ANIM_RAPID_SMASH = 310,
  BATTLE_ANIM_BONE_CHARGE = 311,
  BATTLE_ANIM_MYSTIC_FLAME = 312,
  BATTLE_ANIM_NUMBING_STING = 313,
  BATTLE_ANIM_BRUTE_FORCE = 314,
  BATTLE_ANIM_STICKY_GOO = 315,
  BATTLE_ANIM_CANNIBAL_FANG = 316,
  BATTLE_ANIM_BEAR_CLAW = 317,
  BATTLE_ANIM_POISONOUS_BITE = 318,
  BATTLE_ANIM_FLYING_ATTACK = 319,
  BATTLE_ANIM_UNDEAD_SWORD = 320,
  BATTLE_ANIM_RANSACK = 321,
  BATTLE_ANIM_STICKY_POISON = 322,
  BATTLE_ANIM_POISON_FANG = 323,
  BATTLE_ANIM_ELECTRIC_BITE = 324,
  BATTLE_ANIM_POISON_TAIL = 325,
  BATTLE_ANIM_ONSLAUGHT = 326,
  BATTLE_ANIM_VAMPIRIC_FANG = 327,
  BATTLE_ANIM_BACTERIA_RUSH = 328,
  BATTLE_ANIM_SWIFT_STRIKE = 329,
  BATTLE_ANIM_ROTTEN_BLOOD = 330,
  BATTLE_ANIM_DOUBLE_FANG = 332,
  BATTLE_ANIM_MORTAL_BLOW = 333,
  BATTLE_ANIM_FREEBITE_RUSH = 334,
  BATTLE_ANIM_TWIN_BEAKS = 335,
  BATTLE_ANIM_RABID_FANG = 336,
  BATTLE_ANIM_ACID_BITE = 337,
  BATTLE_ANIM_DYNAMITE = 338,
  BATTLE_ANIM_HEADBUTT = 339,
  BATTLE_ANIM_POISON_INK = 340,
  BATTLE_ANIM_COUNTERSTRIKE = 342,
  BATTLE_ANIM_MAD_DASH = 343,
  BATTLE_ANIM_SOOTHING_STAR = 344,
  BATTLE_ANIM_SPIDER_WEB = 345,
  BATTLE_ANIM_HEARTRENDER = 346,
  BATTLE_ANIM_MAD_SPATTER = 347,
  BATTLE_ANIM_SPASM = 348,
  BATTLE_ANIM_SLEEP_STAR = 349,
  BATTLE_ANIM_DECOMPOSE = 350,
  BATTLE_ANIM_HAUNTING = 351,
  BATTLE_ANIM_WORMS = 352,
  BATTLE_ANIM_BERSERK = 353,
  BATTLE_ANIM_LUCID_PROPHECY = 354,
  BATTLE_ANIM_SMOKE_BOMB = 380,
  BATTLE_ANIM_WING_STROKE = 381,
  BATTLE_ANIM_POISON_BEAT = 382,
  BATTLE_ANIM_SPINNING_BEAT = 383,
  BATTLE_ANIM_OUTER_SPACE = 384,
  BATTLE_ANIM_DRAGON_DRIVER = 385,
  BATTLE_ANIM_DRAIN_FANG = 386,
  BATTLE_ANIM_UNUSED_SEVERE_BLOW_LEAP = 387, ///< no ability uses it; The Lost
                                             ///< Age's Severe Blow does
  BATTLE_ANIM_THRASH = 388,
  BATTLE_ANIM_RECOVERY = 389,
  BATTLE_ANIM_FIRE_BLESSING = 400,
  BATTLE_ANIM_WATER_BLESSING = 401,
  BATTLE_ANIM_ICE_BLESSING = 402,
  BATTLE_ANIM_DARK_BLESSING = 403,
  BATTLE_ANIM_ACID_BLESSING = 404,
  BATTLE_ANIM_STORM_BLESSING = 405,
  BATTLE_ANIM_EVIL_BLESSING = 406,
  BATTLE_ANIM_DEADLY_GAS = 407,
  BATTLE_ANIM_COUNT = 408 ///< one more than the last kind
};

/// An animation's variant: the strength of the ability that plays it, from bits
/// 12 and 13 of its row of gAbilityAnimations. Most abilities come in threes
/// that share an animation, such as Quake, Earthquake and Quake Sphere, and a
/// double or triple blow adds one or two. A runner that sets its own variants
/// names them beside it.
enum AnimVariant {
  ANIM_VARIANT_BASIC,    ///< Quake, Douse, Ply
  ANIM_VARIANT_STRONGER, ///< Earthquake, Drench, Ply Well
  ANIM_VARIANT_STRONGEST ///< Quake Sphere, Deluge, Pure Ply
};

/// The handlers by kind less one (battle_anim_table.c);
/// BattleAnim_RunFromTable calls the one an animation's kind selects.
extern BattleAnimHandler gBattleAnim_Handlers[];

/// What BattleAnim_BuildCenteredIndexList (unused) reads: a value to find in
/// a list, and how many indices either side of it to keep.
typedef struct {
  s32 unused[3];           ///< no code names it
  s32 currentValue;        ///< the value looked for in values
  s32 indexRadius;         ///< indices kept either side of where it sits
  s32 entryLimit;          ///< indices from this up are dropped
  s32 unusedAfterLimit[3]; ///< no code names it
  s16 values[1];           ///< indexRadius * 2 + 1 values, searched in order
} BattleAnimIndexWindow;

/// The map cell buffer (gMapGridCellBuffer), which the animations borrow, has
/// two views here: Ram_MapGridCellBuffer as bytes, at its fixed address, and
/// PARTICLES as particles. The animations use these two, not gMapGridCellBuffer
/// itself.
#define PARTICLES ((struct Particle *)Ram_MapGridCellBuffer)

/// Clips overflow on all four sides.
#define ANIM_CLIP_BOUNDS (BLITTER_CLIP_X | BLITTER_CLIP_Y)

// Extract signed 16.16 pixels without reading an s32 through an s16 alias.
// The plain shift adds four bytes to native English battle_anim.text overall.
#ifdef UBFIX
#define Q_16_16_TO_INT(v) ((s32)(v) >> 16)
#else
#define Q_16_16_TO_INT(v) (((s16 *)&(v))[1])
#endif

/// Two-dimensional {x, y} pair for position or velocity, a local of the
/// summon animations (Nereid, Procne, Tiamat and Ramses).
struct Point2D {
  s32 x; ///< 16.16 pixels in every animation that uses it
  s32 y; ///< 16.16 pixels
};

/// A blitter pair: [0] built in HEAP_SLOT_RAM_CODE and [1] in
/// HEAP_SLOT_RAM_CODE_2 (BattleAnim_FetchRectangleBlitters fills one).
/// Many animations keep this pair in their stack frame.
typedef DrawRectangle BlitterPair[2];

/// The heap slots read as blitters: ANIM_BLITTERS[HEAP_SLOT_RAM_CODE] and
/// ANIM_BLITTERS[HEAP_SLOT_RAM_CODE_2].
#define ANIM_BLITTERS ((DrawRectangle *)gHeapSlots)

// The functions the parts of battle_anim.c define, part by part in include
// order.

// display.c
void BattleAnim_ConfigureEffectDisplay(void);
void BattleAnim_AdvanceScrollOnInterval(void);
void BattleAnim_AdvanceBg2OnInterval(void);
void BattleAnim_ArmWin0HBlankDma(void);

// falling.c
void BattleAnim_RunIce(struct BattleAnimArgs *args);
void BattleAnim_RunDouse(struct BattleAnimArgs *args);

// streaks.c
void BattleAnim_RunSonicWave(struct BattleAnimArgs *args);
void BattleAnim_RunShriek(struct BattleAnimArgs *args);
void BattleAnim_RunBansheeHowl(struct BattleAnimArgs *args);
void BattleAnim_RunWarCry(struct BattleAnimArgs *args);
void BattleAnim_RunWickedHowl(struct BattleAnimArgs *args);
void BattleAnim_RunLimeStreaks(struct BattleAnimArgs *args);
void BattleAnim_RunStreakVolley(struct BattleAnimArgs *args, s32 mode);
void BattleAnim_RunSleepStar(struct BattleAnimArgs *args);
void BattleAnim_RunSoothingStar(struct BattleAnimArgs *args);
void BattleAnim_RunMoteFountain(struct BattleAnimArgs *args, s32 mode);

// target_strike.c
void BattleAnim_RunNumbingSting(struct BattleAnimArgs *args);
void BattleAnim_RunCannibalFang(struct BattleAnimArgs *args);
void BattleAnim_RunPoisonousBite(struct BattleAnimArgs *args);
void BattleAnim_RunRabidFang(struct BattleAnimArgs *args);
void BattleAnim_RunElectricBite(struct BattleAnimArgs *args);
void BattleAnim_RunPoisonTail(struct BattleAnimArgs *args);
void BattleAnim_RunVampiricFang(struct BattleAnimArgs *args);
void BattleAnim_RunDoubleFang(struct BattleAnimArgs *args);
void BattleAnim_RunMortalBlow(struct BattleAnimArgs *args);
void BattleAnim_RunTwinBeaks(struct BattleAnimArgs *args);
void BattleAnim_RunAcidBite(struct BattleAnimArgs *args);
void BattleAnim_RunDrainFang(struct BattleAnimArgs *args);
void BattleAnim_RunTargetStrike(struct BattleAnimArgs *args, s32 mode);

// death_plunge.c
void BattleAnim_RunDeathPlunge(struct BattleAnimArgs *args);
void BattleAnim_RunBladeRain(struct BattleAnimArgs *args);
void BattleAnim_RunElementPillar(struct BattleAnimArgs *args);
void BattleAnim_RunShatteringOpening(struct BattleAnimArgs *args);
void BattleAnim_RunElementBurst(struct BattleAnimArgs *args);
void BattleAnim_DrawStreaks(void);
void BattleAnim_StartElementStreaks(s32 element);
void BattleAnim_EndElementStreaks(void);

// spider_web.c
void BattleAnim_RunPoisonBeat(struct BattleAnimArgs *args);
void BattleAnim_RunSpinningBeat(struct BattleAnimArgs *args);
void BattleAnim_RunSwirlSwipe(struct BattleAnimArgs *args, s32 mode);
void BattleAnim_RunSpiderWeb(struct BattleAnimArgs *args);

// canvas.c
void BattleAnim_WipeCanvas(s32 direction, s32 fill);
void BattleAnim_ProcessPendingGraphicsTransfer(void);
void BattleAnim_FlushPendingGraphicsTransfer(void);
void BattleAnim_ApplyWindowBlendRegisters(void);
void Display_ApplyBg2Reference(void);
void Palette_StepFadeTransfer(void);
void BattleAnim_ClearActorTimers(void);
void BattleAnim_CountDownBattlerTints(void);
void BattleAnim_StartCanvasLayer(s32 bgControl);
void BattleAnim_OpenCanvasLayer(s32 bgControl);
void BattleAnim_StartTiledCanvas(s32 bgControl);
void BattleAnim_EndCanvasLayer(void);
void BattleAnim_HideBg1AndRestoreBackdrop(void);
void BattleAnim_SetupCanvasTileMap(void);
void BattleAnim_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1,
                                      s32 color);

// planet_diver.c
void BattleAnim_RunPlanetDiver(struct BattleAnimArgs *args);
void BattleAnim_RunHaunt(struct BattleAnimArgs *args);
void BattleAnim_RunUnusedOrbitingSpirals(struct BattleAnimArgs *args);

// gathering_lights.c
void BattleAnim_RunDemonNight(struct BattleAnimArgs *args);
void BattleAnim_RunRumble(struct BattleAnimArgs *args);
void BattleAnim_RunHaunting(struct BattleAnimArgs *args);
void BattleAnim_RunGatheringLights(struct BattleAnimArgs *args, s32 mode);

// mirrored_wall.c
void BattleAnim_FetchRectangleBlitters(s32 flip, BlitterPair out);
void BattleAnim_RunBerserk(struct BattleAnimArgs *args);
void BattleAnim_RunSpasm(struct BattleAnimArgs *args);
void BattleAnim_RunRecovery(struct BattleAnimArgs *args);
void BattleAnim_RunMirroredWall(struct BattleAnimArgs *args, s32 mode);

// djinn_beams.c
void BattleAnim_RunRevive(struct BattleAnimArgs *args);
void BattleAnim_RunDew(struct BattleAnimArgs *args);
void BattleAnim_RunDjinnBeams(struct BattleAnimArgs *args, s32 element);

// lobbed_shells.c
void BattleAnim_RunMadSpatter(struct BattleAnimArgs *args);
void BattleAnim_RunDecompose(struct BattleAnimArgs *args);
void BattleAnim_RunBoneChiller(struct BattleAnimArgs *args);
void BattleAnim_RunStickyGoo(struct BattleAnimArgs *args);
void BattleAnim_RunPoisonInk(struct BattleAnimArgs *args);
void BattleAnim_RunStickyPoison(struct BattleAnimArgs *args);
void BattleAnim_RunRottenBlood(struct BattleAnimArgs *args);
void BattleAnim_RunLobbedShells(struct BattleAnimArgs *args, s32 mode);

// condemn.c
void BattleAnim_RunCondemn(struct BattleAnimArgs *args);

// bind.c
void BattleAnim_ConvertBackgroundToBlueRamp(void);
void BattleAnim_RunBind(struct BattleAnimArgs *args);
void BattleAnim_RunUnusedTriangleStrike(struct BattleAnimArgs *args);

// stars.c
void BattleAnim_RunAstralBlast(struct BattleAnimArgs *args);
void BattleAnim_RunShiningStar(struct BattleAnimArgs *args);

// procne.c
void BattleAnim_RunProcne(struct BattleAnimArgs *args);

// tiamat.c
void BattleAnim_RunDragonDriver(struct BattleAnimArgs *args);
void BattleAnim_RunTiamat(struct BattleAnimArgs *args);
void BattleAnim_RunPaletteParticles(struct BattleAnimArgs *args, s32 mode);

// nereid.c
void BattleAnim_RunNereid(struct BattleAnimArgs *args);

// vortices.c
void BattleAnim_RunFroth(struct BattleAnimArgs *args);
void BattleAnim_RunWhirlwind(struct BattleAnimArgs *args);

// prism.c
void BattleAnim_RunPrism(struct BattleAnimArgs *args);
void BattleAnim_UpdatePhasePalette(s32 frame, s32 redPhase, s32 greenPhase,
                                   s32 bluePhase);

// plasma.c
void BattleAnim_RunPlasma(struct BattleAnimArgs *args);
void BattleAnim_RunNova(struct BattleAnimArgs *args);
void BattleAnim_RunDynamite(struct BattleAnimArgs *args);
void BattleAnim_RunSparkGroups(struct BattleAnimArgs *args, s32 mode);

// volcano.c
void BattleAnim_RunVolcano(struct BattleAnimArgs *args);

// target_motes.c
void BattleAnim_RunTaint(struct BattleAnimArgs *args);
void BattleAnim_RunUnusedPinkSmoke(struct BattleAnimArgs *args);
void BattleAnim_RunDelude(struct BattleAnimArgs *args);
void BattleAnim_RunParalyze(struct BattleAnimArgs *args);
void BattleAnim_RunHeartrender(struct BattleAnimArgs *args);
void BattleAnim_RunMysticFlame(struct BattleAnimArgs *args);
void BattleAnim_RunDemonFire(struct BattleAnimArgs *args);
void BattleAnim_RunDrown(struct BattleAnimArgs *args);
void BattleAnim_RunTargetMotes(struct BattleAnimArgs *args, u32 mode);
void BattleAnim_RunSleep(struct BattleAnimArgs *args);
void BattleAnim_RunCurse(struct BattleAnimArgs *args);

// djinn_heal.c
void BattleAnim_RunDjinnHeal(struct BattleAnimArgs *args);
void BattleCamera_ApplyPhasedDelta(void);

// dispatch.c
void BattleAnim_CallDoNothing(s32 count);
void BattleAnim_RunSummon(struct BattleAnimArgs *args);
void BattleAnim_RunFromTable(struct BattleAnimArgs *args);
void BattleAnim_BuildWindowEdgeTable(void);
void BattleAnim_SelectLivingTargets(struct BattleAnimArgs *args);
void BattleAnim_SetupBlendedDisplay(void);
void BattleAnim_TintBattler(s32 actorId, s32 palette, s32 animation,
                            s32 timerSlot, s32 timerFrames);
void BattleAnim_InitializeTilemapAndFlushQueue(void);

// cybele.c
void BattleAnim_RunCybele(struct BattleAnimArgs *args);

// boreas.c
void BattleAnim_RunBoreas(struct BattleAnimArgs *args);

// drain.c
void BattleAnim_RunBreak(struct BattleAnimArgs *args);
void BattleAnim_RunDrain(struct BattleAnimArgs *args);

// healing.c
void BattleAnim_RunPly(struct BattleAnimArgs *args);
void BattleAnim_RunWish(struct BattleAnimArgs *args);
void BattleAnim_RunCure(struct BattleAnimArgs *args);
void BattleAnim_RunCurePoison(struct BattleAnimArgs *args);
void BattleAnim_RunUnusedRestoreMotes(struct BattleAnimArgs *args);
void BattleAnim_RunTonic(struct BattleAnimArgs *args);
void BattleAnim_RunPsyCrystal(struct BattleAnimArgs *args);
void BattleAnim_RunHealingMotes(struct BattleAnimArgs *args, u32 mode);

// stat_change.c
void BattleAnim_RunImpact(struct BattleAnimArgs *args);
void BattleAnim_RunGuard(struct BattleAnimArgs *args);
void BattleAnim_RunWard(struct BattleAnimArgs *args);
void BattleAnim_RunGranite(struct BattleAnimArgs *args);
void BattleAnim_RunFlash(struct BattleAnimArgs *args);
void BattleAnim_RunZephyr(struct BattleAnimArgs *args);
void BattleAnim_RunStatBoost(struct BattleAnimArgs *args, u32 mode);
void BattleAnim_RunDull(struct BattleAnimArgs *args);
void BattleAnim_RunImpair(struct BattleAnimArgs *args);
void BattleAnim_RunWeaken(struct BattleAnimArgs *args);
void BattleAnim_RunUnusedStatReduction(struct BattleAnimArgs *args);
void BattleAnim_RunStatReduction(struct BattleAnimArgs *args, s32 mode);

// flare.c
void BattleAnim_RunFlare(struct BattleAnimArgs *args);
s32 BattleAnim_BuildCenteredIndexList(BattleAnimIndexWindow *window,
                                      s16 *output);

// quake.c
void BattleAnim_RunQuake(struct BattleAnimArgs *args);

// fire.c
void BattleAnim_RunFire(struct BattleAnimArgs *args);

// frost.c
void BattleAnim_RunFrost(struct BattleAnimArgs *args);

// ray.c
void BattleAnim_RunRay(struct BattleAnimArgs *args);

// eruption.c
void BattleAnim_RunSmokeBomb(struct BattleAnimArgs *args);
void BattleAnim_RunBlast(struct BattleAnimArgs *args);
void BattleAnim_RunEruption(struct BattleAnimArgs *args, s32 mode);
void BattleAnim_SpawnObjects(s32 entryCount, s32 source, u32 priority);
void BattleAnim_DoNothing(void);

// burst_shower.c
void BattleAnim_ArmBg2AffineHBlankDma(void);
void BattleAnim_RunFireBlessing(struct BattleAnimArgs *args);
void BattleAnim_RunWaterBlessing(struct BattleAnimArgs *args);
void BattleAnim_RunDarkBlessing(struct BattleAnimArgs *args);
void BattleAnim_RunAcidBlessing(struct BattleAnimArgs *args);
void BattleAnim_RunStormBlessing(struct BattleAnimArgs *args);
void BattleAnim_RunEvilBlessing(struct BattleAnimArgs *args);
void BattleAnim_RunDeadlyGas(struct BattleAnimArgs *args);
void BattleAnim_RunBurstShower(struct BattleAnimArgs *args, s32 mode);

// orbiting.c
void BattleAnim_RunSwirlingStars(struct BattleAnimArgs *args);
void BattleAnim_RunOrbitingSkulls(void);
void BattleAnim_RunTargetBursts(struct BattleAnimArgs *args);

// atalanta.c
void BattleAnim_RunAtalanta(struct BattleAnimArgs *args);

// growth.c
void BattleAnim_RunGrowth(struct BattleAnimArgs *args);
void BattleAnim_RunPunji(struct BattleAnimArgs *args);
void BattleAnim_RunGrowingColumns(struct BattleAnimArgs *args, s32 mode);
void BattleAnim_RunVine(struct BattleAnimArgs *args);
void BattleAnim_RunThorn(struct BattleAnimArgs *args);

// bolt.c
void BattleAnim_RunBolt(struct BattleAnimArgs *args);

// projectiles.c
void BattleAnim_RunDjinniEntrance(struct BattleAnimArgs *args, s32 element,
                                  s32 side, s32 anchor, s32 *outX, s32 *outY);
void BattleAnim_RunSlash(struct BattleAnimArgs *args);
void BattleAnim_RunShuriken(struct BattleAnimArgs *args);
void BattleAnim_RunCrazyVoice(struct BattleAnimArgs *args);
void BattleAnim_RunWingFlutter(struct BattleAnimArgs *args);
void BattleAnim_RunWingStroke(struct BattleAnimArgs *args);
void BattleAnim_RunWingBeat(struct BattleAnimArgs *args);
void BattleAnim_RunCuttingEdge(struct BattleAnimArgs *args);
void BattleAnim_RunHeatWave(struct BattleAnimArgs *args);
void BattleAnim_RunSleet(struct BattleAnimArgs *args);
void BattleAnim_RunIceBlessing(struct BattleAnimArgs *args);
void BattleAnim_RunBlizzard(struct BattleAnimArgs *args);
void BattleAnim_RunProjectileVolley(struct BattleAnimArgs *args, s32 mode);
void BattleAnim_InitializeRisingObject(s32 id);
void BattleAnim_SetApproachMotion(s32 id, s32 targetId, s32 frames);

// rushing_strike.c
void BattleAnim_PackTileRows(u8 *src, u8 *dst, s32 stride, s32 rowCount);
void BattleAnim_RunBoneCharge(struct BattleAnimArgs *args);
void BattleAnim_RunBruteForce(struct BattleAnimArgs *args);
void BattleAnim_RunOnslaught(struct BattleAnimArgs *args);
void BattleAnim_RunHeadbutt(struct BattleAnimArgs *args);
void BattleAnim_RunRushingStrike(struct BattleAnimArgs *args, s32 mode);

// djinn_attacks.c
void BattleAnim_TransposeCopy(const u8 *src, u8 *dst, s32 w, s32 h);
void BattleAnim_RunMercury(struct BattleAnimArgs *args);
void BattleAnim_RunJupiter(struct BattleAnimArgs *args);
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette);
void BattleAnim_RunVenus(struct BattleAnimArgs *args);
void BattleAnim_RunMars(struct BattleAnimArgs *args);

// hail.c
void BattleAnim_RunHail(struct BattleAnimArgs *args);
void BattleAnim_RunGround(struct BattleAnimArgs *args);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);

// thor.c
void BattleAnim_RunThor(struct BattleAnimArgs *args);

// spire.c
void BattleAnim_RunSpire(struct BattleAnimArgs *args);

// flurry.c
void BattleAnim_RunGust(struct BattleAnimArgs *args);
void BattleAnim_RunBearClaw(struct BattleAnimArgs *args);
void BattleAnim_RunRansack(struct BattleAnimArgs *args);
void BattleAnim_RunBacteriaRush(struct BattleAnimArgs *args);
void BattleAnim_RunFreebiteRush(struct BattleAnimArgs *args);
void BattleAnim_RunFlyingAttack(struct BattleAnimArgs *args);
void BattleAnim_RunFlurry(struct BattleAnimArgs *args, s32 mode);

// gaia.c
void BattleAnim_RunGaia(struct BattleAnimArgs *args);

// positions.c
void Particle_AdvanceWithGravity3D(struct Particle *step, s32 damping,
                                   s32 gravity);
void Particle_AdvanceWithGravity2D(struct Particle *step, s32 damping,
                                   s32 gravity);
s32 Projection_Point(const void *point, struct ScreenPoint *position);
void Projection_Sprite(s32 id, struct ScreenPoint *position);
void Projection_ScaledUnit(s32 id, struct ScreenPoint *position);
void Projection_Unit(s32 id, struct ScreenPoint *position);
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen);
void BattleAnim_ProbeKeysForExtendedActor(void);
void BattleAnim_RunByIdRange(struct BattleAnimArgs *args);

// beams.c
void BattleAnim_RunBeam(struct BattleAnimArgs *args);
void BattleAnim_RunRingAndSpark(struct BattleAnimArgs *args);

// casting_impact.c
void BattleAnim_StepPaletteToResource(s32 resourceId);
void BattleAnim_RunCastingImpact(struct BattleAnimArgs *args, s32 mode);

// torch.c
void BattleAnim_RunTorch(struct BattleAnimArgs *args);
void BattleAnim_RunKite(struct BattleAnimArgs *args);

// helm_splitter.c
void BattleAnim_RunHelmSplitter(struct BattleAnimArgs *args);
void BattleAnim_PlaceFormationObjects(s32 formation, s32 x, s32 z);
void BattleAnim_RunImpactBurst(s32 formation, s32 x, s32 y);

// particle_streams.c
void Palette_BrightenBgEntries(s32 blueStep, s32 greenStep, s32 redStep);
void BattleAnim_ArmPaletteHBlankDma(void);
void BattleAnim_RunMeteor(struct BattleAnimArgs *args);
void BattleAnim_RunOuterSpace(struct BattleAnimArgs *args);
void BattleAnim_RunParticleStreams(struct BattleAnimArgs *args, s32 mode);

// ramses.c
void BattleAnim_RunRamses(struct BattleAnimArgs *args);

// dragon_cloud.c
void BattleAnim_RunDragonCloud(struct BattleAnimArgs *args);

// annihilation.c
void BattleAnim_RunAnnihilation(struct BattleAnimArgs *args);

// ragnarok.c
void BattleAnim_RunRagnarok(struct BattleAnimArgs *args);

// titan_blade.c
void BattleAnim_RunTitanBlade(struct BattleAnimArgs *args);

// judgment.c
void BattleAnim_RunJudgment(struct BattleAnimArgs *args);

// kirin.c
void BattleAnim_RunKirin(struct BattleAnimArgs *args);

// neptune.c
void BattleAnim_ResetBg2Pa(void);
void BattleAnim_SetBg2AffineScaleHalf(void);
void BattleAnim_RunNeptune(struct BattleAnimArgs *args);

// thrash.c
void BattleAnim_RunThrash(struct BattleAnimArgs *args);
void BattleAnim_RunUnusedSevereBlowLeap(struct BattleAnimArgs *args);
void BattleAnim_RunQuickStrike(struct BattleAnimArgs *args);
void BattleAnim_RunVorpalSlash(struct BattleAnimArgs *args);
void BattleAnim_RunFlashingStrike(struct BattleAnimArgs *args, s32 mode);

// undead_sword.c
void BattleAnim_RunUndeadSword(struct BattleAnimArgs *args);

// blitter.c
s32 BattleAnim_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift,
                            s32 flags, u32 mode);

#endif
