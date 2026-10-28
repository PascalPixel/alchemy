#ifndef ALCHEMY_MODULE_BATTLE_EFFECT_BATTLE_EFFECT_H
#define ALCHEMY_MODULE_BATTLE_EFFECT_BATTLE_EFFECT_H

#include "types.h"
#include "render_placement.h"
#include "battle_effect_work.h"
#include "blitter.h"

struct BattleEffectParams;
struct ScreenPoint;
struct Particle;

/// The large effects BattleEffect_RunSummon runs, by kind: the twelve
/// summons. A summon's row of gAbilityBattleEffects has EFFECT_SUMMON plus its
/// kind.
enum SummonEffectId {
  SUMMON_ANIM_NONE,     ///< no row uses it; runs Meteor's effect too
  SUMMON_ANIM_RAMSES,   ///< BattleEffect_RunRamses (ramses.c)
  SUMMON_ANIM_NEREID,   ///< BattleEffect_RunNereid (nereid.c)
  SUMMON_ANIM_KIRIN,    ///< BattleEffect_RunKirin (kirin.c)
  SUMMON_ANIM_ATALANTA, ///< BattleEffect_RunAtalanta (atalanta.c)
  SUMMON_ANIM_CYBELE,   ///< BattleEffect_RunCybele (cybele.c)
  SUMMON_ANIM_NEPTUNE,  ///< BattleEffect_RunNeptune (neptune.c)
  SUMMON_ANIM_TIAMAT,   ///< BattleEffect_RunTiamat (tiamat.c)
  SUMMON_ANIM_PROCNE,   ///< BattleEffect_RunProcne (procne.c)
  SUMMON_ANIM_JUDGMENT, ///< BattleEffect_RunJudgment (judgment.c)
  SUMMON_ANIM_BOREAS,   ///< BattleEffect_RunBoreas (boreas.c)
  SUMMON_ANIM_METEOR,   ///< BattleEffect_RunMeteor (particle_streams.c)
  SUMMON_ANIM_THOR      ///< BattleEffect_RunThor (thor.c)
};

/// An effect handler: plays one effect over the battle, frame by frame,
/// and returns when it is over.
typedef void (*BattleEffectHandler)(struct BattleEffectParams *args);
/// Effect kinds: the kind an ability's gAbilityBattleEffects entry selects, by
/// the handler that plays it; a later kind that shares a handler is named for
/// the ability that uses it (the Djinn from 200, the monsters' abilities from
/// 300), or Unused when none does. Kinds without a handler have no name.
enum BattleEffectId {
  BATTLE_EFFECT_NONE = 0, ///< no effect: the dispatcher only clears the variant
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
  BATTLE_EFFECT_REGENERATE = 44,
  BATTLE_EFFECT_REFLECT = 45,
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
  BATTLE_EFFECT_GRANITE = 200,
  BATTLE_EFFECT_QUARTZ = 201,
  BATTLE_EFFECT_VINE = 202,
  BATTLE_EFFECT_GROUND = 203,
  BATTLE_EFFECT_FIZZ = 204,
  BATTLE_EFFECT_SLEET = 205,
  BATTLE_EFFECT_SPRITZ = 206,
  BATTLE_EFFECT_HAIL = 207,
  BATTLE_EFFECT_TONIC = 208,
  BATTLE_EFFECT_DEW = 209,
  BATTLE_EFFECT_FORGE = 210,
  BATTLE_EFFECT_CORONA = 211,
  BATTLE_EFFECT_PSY_CRYSTAL = 212,
  BATTLE_EFFECT_FLASH = 213,
  BATTLE_EFFECT_TORCH = 214,
  BATTLE_EFFECT_BREEZE = 215,
  BATTLE_EFFECT_ZEPHYR = 216,
  BATTLE_EFFECT_KITE = 217,
  BATTLE_EFFECT_LUFF = 218,
  BATTLE_EFFECT_GUST = 219,
  BATTLE_EFFECT_VENUS = 250,
  BATTLE_EFFECT_MERCURY = 251,
  BATTLE_EFFECT_MARS = 252,
  BATTLE_EFFECT_JUPITER = 253,
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
  BATTLE_EFFECT_RAPID_SMASH = 310,
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
  BATTLE_EFFECT_POISON_FANG = 323,
  BATTLE_EFFECT_ELECTRIC_BITE = 324,
  BATTLE_EFFECT_POISON_TAIL = 325,
  BATTLE_EFFECT_ONSLAUGHT = 326,
  BATTLE_EFFECT_VAMPIRIC_FANG = 327,
  BATTLE_EFFECT_BACTERIA_RUSH = 328,
  BATTLE_EFFECT_SWIFT_STRIKE = 329,
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
  BATTLE_EFFECT_COUNTERSTRIKE = 342,
  BATTLE_EFFECT_MAD_DASH = 343,
  BATTLE_EFFECT_SOOTHING_STAR = 344,
  BATTLE_EFFECT_SPIDER_WEB = 345,
  BATTLE_EFFECT_HEARTRENDER = 346,
  BATTLE_EFFECT_MAD_SPATTER = 347,
  BATTLE_EFFECT_SPASM = 348,
  BATTLE_EFFECT_SLEEP_STAR = 349,
  BATTLE_EFFECT_DECOMPOSE = 350,
  BATTLE_EFFECT_HAUNTING = 351,
  BATTLE_EFFECT_WORMS = 352,
  BATTLE_EFFECT_BERSERK = 353,
  BATTLE_EFFECT_LUCID_PROPHECY = 354,
  BATTLE_EFFECT_SMOKE_BOMB = 380,
  BATTLE_EFFECT_WING_STROKE = 381,
  BATTLE_EFFECT_POISON_BEAT = 382,
  BATTLE_EFFECT_SPINNING_BEAT = 383,
  BATTLE_EFFECT_OUTER_SPACE = 384,
  BATTLE_EFFECT_DRAGON_DRIVER = 385,
  BATTLE_EFFECT_DRAIN_FANG = 386,
  BATTLE_EFFECT_UNUSED_SEVERE_BLOW_LEAP = 387, ///< no ability uses it; The Lost
                                               ///< Age's Severe Blow does
  BATTLE_EFFECT_THRASH = 388,
  BATTLE_EFFECT_RECOVERY = 389,
  BATTLE_EFFECT_FIRE_BLESSING = 400,
  BATTLE_EFFECT_WATER_BLESSING = 401,
  BATTLE_EFFECT_ICE_BLESSING = 402,
  BATTLE_EFFECT_DARK_BLESSING = 403,
  BATTLE_EFFECT_ACID_BLESSING = 404,
  BATTLE_EFFECT_STORM_BLESSING = 405,
  BATTLE_EFFECT_EVIL_BLESSING = 406,
  BATTLE_EFFECT_DEADLY_GAS = 407,
  BATTLE_EFFECT_COUNT = 408 ///< one more than the last kind
};

/// An effect's variant: the strength of the ability that plays it, from bits
/// 12 and 13 of its row of gAbilityBattleEffects. Most abilities come in threes
/// that share an effect, such as Quake, Earthquake and Quake Sphere, and a
/// double or triple blow adds one or two. A runner that sets its own variants
/// names them beside it.
enum EffectVariant {
  EFFECT_VARIANT_BASIC,    ///< Quake, Douse, Ply
  EFFECT_VARIANT_STRONGER, ///< Earthquake, Drench, Ply Well
  EFFECT_VARIANT_STRONGEST ///< Quake Sphere, Deluge, Pure Ply
};

/// The handlers by kind less one (battle_effect_table.c);
/// BattleEffect_RunFromTable calls the one an effect's kind selects.
extern BattleEffectHandler gBattleEffect_Handlers[];

/// What BattleEffect_BuildCenteredIndexList (unused) reads: a value to find in
/// a list, and how many indices either side of it to keep.
typedef struct {
  s32 unused[3];           ///< no code names it
  s32 currentValue;        ///< the value looked for in values
  s32 indexRadius;         ///< indices kept either side of where it sits
  s32 entryLimit;          ///< indices from this up are dropped
  s32 unusedAfterLimit[3]; ///< no code names it
  s16 values[1];           ///< indexRadius * 2 + 1 values, searched in order
} BattleEffectIndexWindow;

/// The map cell buffer (gMapGridCellBuffer), which the effects borrow, has
/// two views here: Ram_MapGridCellBuffer as bytes, at its fixed address, and
/// PARTICLES as particles. The effects use these two, not gMapGridCellBuffer
/// itself.
#define PARTICLES ((struct Particle *)Ram_MapGridCellBuffer)

/// Clips overflow on all four sides.
#define EFFECT_CLIP_BOUNDS (BLITTER_CLIP_X | BLITTER_CLIP_Y)

// Extract signed 16.16 pixels without reading an s32 through an s16 alias.
// The plain shift adds four bytes to native English battle_effect.text overall.
#ifdef UBFIX
#define Q_16_16_TO_INT(v) ((s32)(v) >> 16)
#else
#define Q_16_16_TO_INT(v) (((s16 *)&(v))[1])
#endif

/// Two-dimensional {x, y} pair for position or velocity, a local of the
/// summon effects (Nereid, Procne, Tiamat and Ramses).
struct Point2D {
  s32 x; ///< 16.16 pixels in every effect that uses it
  s32 y; ///< 16.16 pixels
};

/// A blitter pair: [0] built in HEAP_SLOT_RAM_CODE and [1] in
/// HEAP_SLOT_RAM_CODE_2 (BattleEffect_FetchRectangleBlitters fills one).
/// Many effects keep this pair in their stack frame.
typedef DrawRectangle BlitterPair[2];

/// The heap slots read as blitters: EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE] and
/// EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2].
#define EFFECT_BLITTERS ((DrawRectangle *)gHeapSlots)

// The functions the parts of battle_effect.c define, part by part in include
// order.

// display.c
void BattleEffect_ConfigureEffectDisplay(void);
void BattleEffect_AdvanceScrollOnInterval(void);
void BattleEffect_AdvanceBg2OnInterval(void);
void BattleEffect_ArmWin0HBlankDma(void);

// falling.c
void BattleEffect_RunIce(struct BattleEffectParams *args);
void BattleEffect_RunDouse(struct BattleEffectParams *args);

// streaks.c
void BattleEffect_RunSonicWave(struct BattleEffectParams *args);
void BattleEffect_RunShriek(struct BattleEffectParams *args);
void BattleEffect_RunBansheeHowl(struct BattleEffectParams *args);
void BattleEffect_RunWarCry(struct BattleEffectParams *args);
void BattleEffect_RunWickedHowl(struct BattleEffectParams *args);
void BattleEffect_RunLimeStreaks(struct BattleEffectParams *args);
void BattleEffect_RunStreakVolley(struct BattleEffectParams *args, s32 mode);
void BattleEffect_RunSleepStar(struct BattleEffectParams *args);
void BattleEffect_RunSoothingStar(struct BattleEffectParams *args);
void BattleEffect_RunMoteFountain(struct BattleEffectParams *args, s32 mode);

// target_strike.c
void BattleEffect_RunNumbingSting(struct BattleEffectParams *args);
void BattleEffect_RunCannibalFang(struct BattleEffectParams *args);
void BattleEffect_RunPoisonousBite(struct BattleEffectParams *args);
void BattleEffect_RunRabidFang(struct BattleEffectParams *args);
void BattleEffect_RunElectricBite(struct BattleEffectParams *args);
void BattleEffect_RunPoisonTail(struct BattleEffectParams *args);
void BattleEffect_RunVampiricFang(struct BattleEffectParams *args);
void BattleEffect_RunDoubleFang(struct BattleEffectParams *args);
void BattleEffect_RunMortalBlow(struct BattleEffectParams *args);
void BattleEffect_RunTwinBeaks(struct BattleEffectParams *args);
void BattleEffect_RunAcidBite(struct BattleEffectParams *args);
void BattleEffect_RunDrainFang(struct BattleEffectParams *args);
void BattleEffect_RunTargetStrike(struct BattleEffectParams *args, s32 mode);

// death_plunge.c
void BattleEffect_RunDeathPlunge(struct BattleEffectParams *args);
void BattleEffect_RunBladeRain(struct BattleEffectParams *args);
void BattleEffect_RunElementPillar(struct BattleEffectParams *args);
void BattleEffect_RunShatteringOpening(struct BattleEffectParams *args);
void BattleEffect_RunElementBurst(struct BattleEffectParams *args);
void BattleEffect_DrawStreaks(void);
void BattleEffect_StartElementStreaks(s32 element);
void BattleEffect_EndElementStreaks(void);

// spider_web.c
void BattleEffect_RunPoisonBeat(struct BattleEffectParams *args);
void BattleEffect_RunSpinningBeat(struct BattleEffectParams *args);
void BattleEffect_RunSwirlSwipe(struct BattleEffectParams *args, s32 mode);
void BattleEffect_RunSpiderWeb(struct BattleEffectParams *args);

// canvas.c
void BattleEffect_WipeCanvas(s32 direction, s32 fill);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_FlushPendingGraphicsTransfer(void);
void BattleEffect_ApplyWindowBlendRegisters(void);
void Display_ApplyBg2Reference(void);
void Palette_StepFadeTransfer(void);
void BattleEffect_ClearActorTimers(void);
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_OpenCanvasLayer(s32 bgControl);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_HideBg1AndRestoreBackdrop(void);
void BattleEffect_SetupCanvasTileMap(void);
void BattleEffect_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1,
                                        s32 color);

// planet_diver.c
void BattleEffect_RunPlanetDiver(struct BattleEffectParams *args);
void BattleEffect_RunHaunt(struct BattleEffectParams *args);
void BattleEffect_RunUnusedOrbitingSpirals(struct BattleEffectParams *args);

// gathering_lights.c
void BattleEffect_RunDemonNight(struct BattleEffectParams *args);
void BattleEffect_RunRumble(struct BattleEffectParams *args);
void BattleEffect_RunHaunting(struct BattleEffectParams *args);
void BattleEffect_RunGatheringLights(struct BattleEffectParams *args, s32 mode);

// mirrored_wall.c
void BattleEffect_FetchRectangleBlitters(s32 flip, BlitterPair out);
void BattleEffect_RunBerserk(struct BattleEffectParams *args);
void BattleEffect_RunSpasm(struct BattleEffectParams *args);
void BattleEffect_RunRecovery(struct BattleEffectParams *args);
void BattleEffect_RunMirroredWall(struct BattleEffectParams *args, s32 mode);

// djinn_beams.c
void BattleEffect_RunRevive(struct BattleEffectParams *args);
void BattleEffect_RunDew(struct BattleEffectParams *args);
void BattleEffect_RunDjinnBeams(struct BattleEffectParams *args, s32 element);

// lobbed_shells.c
void BattleEffect_RunMadSpatter(struct BattleEffectParams *args);
void BattleEffect_RunDecompose(struct BattleEffectParams *args);
void BattleEffect_RunBoneChiller(struct BattleEffectParams *args);
void BattleEffect_RunStickyGoo(struct BattleEffectParams *args);
void BattleEffect_RunPoisonInk(struct BattleEffectParams *args);
void BattleEffect_RunStickyPoison(struct BattleEffectParams *args);
void BattleEffect_RunRottenBlood(struct BattleEffectParams *args);
void BattleEffect_RunLobbedShells(struct BattleEffectParams *args, s32 mode);

// condemn.c
void BattleEffect_RunCondemn(struct BattleEffectParams *args);

// bind.c
void BattleEffect_ConvertBackgroundToBlueRamp(void);
void BattleEffect_RunBind(struct BattleEffectParams *args);
void BattleEffect_RunUnusedTriangleStrike(struct BattleEffectParams *args);

// stars.c
void BattleEffect_RunAstralBlast(struct BattleEffectParams *args);
void BattleEffect_RunShiningStar(struct BattleEffectParams *args);

// procne.c
void BattleEffect_RunProcne(struct BattleEffectParams *args);

// tiamat.c
void BattleEffect_RunDragonDriver(struct BattleEffectParams *args);
void BattleEffect_RunTiamat(struct BattleEffectParams *args);
void BattleEffect_RunPaletteParticles(struct BattleEffectParams *args,
                                      s32 mode);

// nereid.c
void BattleEffect_RunNereid(struct BattleEffectParams *args);

// vortices.c
void BattleEffect_RunFroth(struct BattleEffectParams *args);
void BattleEffect_RunWhirlwind(struct BattleEffectParams *args);

// prism.c
void BattleEffect_RunPrism(struct BattleEffectParams *args);
void BattleEffect_UpdatePhasePalette(s32 frame, s32 redPhase, s32 greenPhase,
                                     s32 bluePhase);

// plasma.c
void BattleEffect_RunPlasma(struct BattleEffectParams *args);
void BattleEffect_RunNova(struct BattleEffectParams *args);
void BattleEffect_RunDynamite(struct BattleEffectParams *args);
void BattleEffect_RunSparkGroups(struct BattleEffectParams *args, s32 mode);

// volcano.c
void BattleEffect_RunVolcano(struct BattleEffectParams *args);

// target_motes.c
void BattleEffect_RunTaint(struct BattleEffectParams *args);
void BattleEffect_RunUnusedPinkSmoke(struct BattleEffectParams *args);
void BattleEffect_RunDelude(struct BattleEffectParams *args);
void BattleEffect_RunParalyze(struct BattleEffectParams *args);
void BattleEffect_RunHeartrender(struct BattleEffectParams *args);
void BattleEffect_RunMysticFlame(struct BattleEffectParams *args);
void BattleEffect_RunDemonFire(struct BattleEffectParams *args);
void BattleEffect_RunDrown(struct BattleEffectParams *args);
void BattleEffect_RunTargetMotes(struct BattleEffectParams *args, u32 mode);
void BattleEffect_RunSleep(struct BattleEffectParams *args);
void BattleEffect_RunCurse(struct BattleEffectParams *args);

// djinn_heal.c
void BattleEffect_RunDjinnHeal(struct BattleEffectParams *args);
void BattleCamera_ApplyPhasedDelta(void);

// dispatch.c
void BattleEffect_CallDoNothing(s32 count);
void BattleEffect_RunSummon(struct BattleEffectParams *args);
void BattleEffect_RunFromTable(struct BattleEffectParams *args);
void BattleEffect_BuildWindowEdgeTable(void);
void BattleEffect_SelectLivingTargets(struct BattleEffectParams *args);
void BattleEffect_SetupBlendedDisplay(void);
void BattleEffect_TintBattler(s32 actorId, s32 palette, s32 animation,
                              s32 timerSlot, s32 timerFrames);
void BattleEffect_InitializeTilemapAndFlushQueue(void);

// cybele.c
void BattleEffect_RunCybele(struct BattleEffectParams *args);

// boreas.c
void BattleEffect_RunBoreas(struct BattleEffectParams *args);

// drain.c
void BattleEffect_RunBreak(struct BattleEffectParams *args);
void BattleEffect_RunDrain(struct BattleEffectParams *args);

// healing.c
void BattleEffect_RunPly(struct BattleEffectParams *args);
void BattleEffect_RunWish(struct BattleEffectParams *args);
void BattleEffect_RunCure(struct BattleEffectParams *args);
void BattleEffect_RunCurePoison(struct BattleEffectParams *args);
void BattleEffect_RunUnusedRestoreMotes(struct BattleEffectParams *args);
void BattleEffect_RunTonic(struct BattleEffectParams *args);
void BattleEffect_RunPsyCrystal(struct BattleEffectParams *args);
void BattleEffect_RunHealingMotes(struct BattleEffectParams *args, u32 mode);

// stat_change.c
void BattleEffect_RunImpact(struct BattleEffectParams *args);
void BattleEffect_RunGuard(struct BattleEffectParams *args);
void BattleEffect_RunWard(struct BattleEffectParams *args);
void BattleEffect_RunGranite(struct BattleEffectParams *args);
void BattleEffect_RunFlash(struct BattleEffectParams *args);
void BattleEffect_RunZephyr(struct BattleEffectParams *args);
void BattleEffect_RunStatBoost(struct BattleEffectParams *args, u32 mode);
void BattleEffect_RunDull(struct BattleEffectParams *args);
void BattleEffect_RunImpair(struct BattleEffectParams *args);
void BattleEffect_RunWeaken(struct BattleEffectParams *args);
void BattleEffect_RunUnusedStatReduction(struct BattleEffectParams *args);
void BattleEffect_RunStatReduction(struct BattleEffectParams *args, s32 mode);

// flare.c
void BattleEffect_RunFlare(struct BattleEffectParams *args);
s32 BattleEffect_BuildCenteredIndexList(BattleEffectIndexWindow *window,
                                        s16 *output);

// quake.c
void BattleEffect_RunQuake(struct BattleEffectParams *args);

// fire.c
void BattleEffect_RunFire(struct BattleEffectParams *args);

// frost.c
void BattleEffect_RunFrost(struct BattleEffectParams *args);

// ray.c
void BattleEffect_RunRay(struct BattleEffectParams *args);

// eruption.c
void BattleEffect_RunSmokeBomb(struct BattleEffectParams *args);
void BattleEffect_RunBlast(struct BattleEffectParams *args);
void BattleEffect_RunEruption(struct BattleEffectParams *args, s32 mode);
void BattleEffect_SpawnObjects(s32 entryCount, s32 source, u32 priority);
void BattleEffect_DoNothing(void);

// burst_shower.c
void BattleEffect_ArmBg2AffineHBlankDma(void);
void BattleEffect_RunFireBlessing(struct BattleEffectParams *args);
void BattleEffect_RunWaterBlessing(struct BattleEffectParams *args);
void BattleEffect_RunDarkBlessing(struct BattleEffectParams *args);
void BattleEffect_RunAcidBlessing(struct BattleEffectParams *args);
void BattleEffect_RunStormBlessing(struct BattleEffectParams *args);
void BattleEffect_RunEvilBlessing(struct BattleEffectParams *args);
void BattleEffect_RunDeadlyGas(struct BattleEffectParams *args);
void BattleEffect_RunBurstShower(struct BattleEffectParams *args, s32 mode);

// orbiting.c
void BattleEffect_RunSwirlingStars(struct BattleEffectParams *args);
void BattleEffect_RunOrbitingSkulls(void);
void BattleEffect_RunTargetBursts(struct BattleEffectParams *args);

// atalanta.c
void BattleEffect_RunAtalanta(struct BattleEffectParams *args);

// growth.c
void BattleEffect_RunGrowth(struct BattleEffectParams *args);
void BattleEffect_RunPunji(struct BattleEffectParams *args);
void BattleEffect_RunGrowingColumns(struct BattleEffectParams *args, s32 mode);
void BattleEffect_RunVine(struct BattleEffectParams *args);
void BattleEffect_RunThorn(struct BattleEffectParams *args);

// bolt.c
void BattleEffect_RunBolt(struct BattleEffectParams *args);

// projectiles.c
void BattleEffect_RunDjinniEntrance(struct BattleEffectParams *args,
                                    s32 element, s32 side, s32 anchor,
                                    s32 *outX, s32 *outY);
void BattleEffect_RunSlash(struct BattleEffectParams *args);
void BattleEffect_RunShuriken(struct BattleEffectParams *args);
void BattleEffect_RunCrazyVoice(struct BattleEffectParams *args);
void BattleEffect_RunWingFlutter(struct BattleEffectParams *args);
void BattleEffect_RunWingStroke(struct BattleEffectParams *args);
void BattleEffect_RunWingBeat(struct BattleEffectParams *args);
void BattleEffect_RunCuttingEdge(struct BattleEffectParams *args);
void BattleEffect_RunHeatWave(struct BattleEffectParams *args);
void BattleEffect_RunSleet(struct BattleEffectParams *args);
void BattleEffect_RunIceBlessing(struct BattleEffectParams *args);
void BattleEffect_RunBlizzard(struct BattleEffectParams *args);
void BattleEffect_RunProjectileVolley(struct BattleEffectParams *args,
                                      s32 mode);
void BattleEffect_InitializeRisingObject(s32 id);
void BattleEffect_SetApproachMotion(s32 id, s32 targetId, s32 frames);

// rushing_strike.c
void BattleEffect_PackTileRows(u8 *src, u8 *dst, s32 stride, s32 rowCount);
void BattleEffect_RunBoneCharge(struct BattleEffectParams *args);
void BattleEffect_RunBruteForce(struct BattleEffectParams *args);
void BattleEffect_RunOnslaught(struct BattleEffectParams *args);
void BattleEffect_RunHeadbutt(struct BattleEffectParams *args);
void BattleEffect_RunRushingStrike(struct BattleEffectParams *args, s32 mode);

// djinn_attacks.c
void BattleEffect_TransposeCopy(const u8 *src, u8 *dst, s32 w, s32 h);
void BattleEffect_RunMercury(struct BattleEffectParams *args);
void BattleEffect_RunJupiter(struct BattleEffectParams *args);
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette);
void BattleEffect_RunVenus(struct BattleEffectParams *args);
void BattleEffect_RunMars(struct BattleEffectParams *args);

// hail.c
void BattleEffect_RunHail(struct BattleEffectParams *args);
void BattleEffect_RunGround(struct BattleEffectParams *args);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);

// thor.c
void BattleEffect_RunThor(struct BattleEffectParams *args);

// spire.c
void BattleEffect_RunSpire(struct BattleEffectParams *args);

// flurry.c
void BattleEffect_RunGust(struct BattleEffectParams *args);
void BattleEffect_RunBearClaw(struct BattleEffectParams *args);
void BattleEffect_RunRansack(struct BattleEffectParams *args);
void BattleEffect_RunBacteriaRush(struct BattleEffectParams *args);
void BattleEffect_RunFreebiteRush(struct BattleEffectParams *args);
void BattleEffect_RunFlyingAttack(struct BattleEffectParams *args);
void BattleEffect_RunFlurry(struct BattleEffectParams *args, s32 mode);

// gaia.c
void BattleEffect_RunGaia(struct BattleEffectParams *args);

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
void BattleEffect_ProbeKeysForExtendedActor(void);
void BattleEffect_RunByIdRange(struct BattleEffectParams *args);

// beams.c
void BattleEffect_RunBeam(struct BattleEffectParams *args);
void BattleEffect_RunRingAndSpark(struct BattleEffectParams *args);

// casting_impact.c
void BattleEffect_StepPaletteToResource(s32 resourceId);
void BattleEffect_RunCastingImpact(struct BattleEffectParams *args, s32 mode);

// torch.c
void BattleEffect_RunTorch(struct BattleEffectParams *args);
void BattleEffect_RunKite(struct BattleEffectParams *args);

// helm_splitter.c
void BattleEffect_RunHelmSplitter(struct BattleEffectParams *args);
void BattleEffect_PlaceFormationObjects(s32 formation, s32 x, s32 z);
void BattleEffect_RunImpactBurst(s32 formation, s32 x, s32 y);

// particle_streams.c
void Palette_BrightenBgEntries(s32 blueStep, s32 greenStep, s32 redStep);
void BattleEffect_ArmPaletteHBlankDma(void);
void BattleEffect_RunMeteor(struct BattleEffectParams *args);
void BattleEffect_RunOuterSpace(struct BattleEffectParams *args);
void BattleEffect_RunParticleStreams(struct BattleEffectParams *args, s32 mode);

// ramses.c
void BattleEffect_RunRamses(struct BattleEffectParams *args);

// dragon_cloud.c
void BattleEffect_RunDragonCloud(struct BattleEffectParams *args);

// annihilation.c
void BattleEffect_RunAnnihilation(struct BattleEffectParams *args);

// ragnarok.c
void BattleEffect_RunRagnarok(struct BattleEffectParams *args);

// titan_blade.c
void BattleEffect_RunTitanBlade(struct BattleEffectParams *args);

// judgment.c
void BattleEffect_RunJudgment(struct BattleEffectParams *args);

// kirin.c
void BattleEffect_RunKirin(struct BattleEffectParams *args);

// neptune.c
void BattleEffect_ResetBg2Pa(void);
void BattleEffect_SetBg2AffineScaleHalf(void);
void BattleEffect_RunNeptune(struct BattleEffectParams *args);

// thrash.c
void BattleEffect_RunThrash(struct BattleEffectParams *args);
void BattleEffect_RunUnusedSevereBlowLeap(struct BattleEffectParams *args);
void BattleEffect_RunQuickStrike(struct BattleEffectParams *args);
void BattleEffect_RunVorpalSlash(struct BattleEffectParams *args);
void BattleEffect_RunFlashingStrike(struct BattleEffectParams *args, s32 mode);

// undead_sword.c
void BattleEffect_RunUndeadSword(struct BattleEffectParams *args);

// blitter.c
s32 BattleEffect_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift,
                              s32 flags, u32 mode);

#endif
