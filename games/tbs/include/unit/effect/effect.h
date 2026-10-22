#ifndef ALCHEMY_UNIT_EFFECT_EFFECT_H
#define ALCHEMY_UNIT_EFFECT_EFFECT_H

#include "types.h"
#include "render_placement.h"
#include "battle_effect_work.h"
#include "battle_effect.h"

struct BattleEffectArgument;
struct EffectPosition;
struct EffectStep;

/// The large effects BattleEffect_RunSceneEffect runs, by kind: the twelve
/// summons. A summon's row of actflags.tsv has effect 0x8000 plus its kind.
enum SceneEffectKind {
  SCENE_EFFECT_NONE,     ///< no row uses it; runs Meteor's scene too
  SCENE_EFFECT_RAMSES,   ///< BattleEffect_RunRamses (ramses.c)
  SCENE_EFFECT_NEREID,   ///< BattleEffect_RunNereid (nereid.c)
  SCENE_EFFECT_KIRIN,    ///< BattleEffect_RunKirin (kirin.c)
  SCENE_EFFECT_ATALANTA, ///< BattleEffect_RunAtalanta (atalanta.c)
  SCENE_EFFECT_CYBELE,   ///< BattleEffect_RunCybele (cybele.c)
  SCENE_EFFECT_NEPTUNE,  ///< BattleEffect_RunNeptune (neptune.c)
  SCENE_EFFECT_TIAMAT,   ///< BattleEffect_RunTiamat (tiamat.c)
  SCENE_EFFECT_PROCNE,   ///< BattleEffect_RunProcne (procne.c)
  SCENE_EFFECT_JUDGMENT, ///< BattleEffect_RunJudgment (judgment.c)
  SCENE_EFFECT_BOREAS,   ///< BattleEffect_RunBoreas (boreas.c)
  SCENE_EFFECT_METEOR,   ///< BattleEffect_RunMeteor (particle_streams.c)
  SCENE_EFFECT_THOR      ///< BattleEffect_RunThor (thor.c)
};

/// An effect handler: plays one effect over the battle, frame by frame, and
/// returns when it is over.
typedef void (*BattleFxHandler)(struct BattleEffectArgument *effect);
/// Effect kinds: the kind an ability's gBattle_ActionFlags entry selects, by
/// the handler that plays it; a later kind that shares a handler adds its
/// number. Kinds without a handler have no name.
enum BattleEffectKind {
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
  BATTLE_EFFECT_HEARTRENDER = 35,
  BATTLE_EFFECT_PARALYZE = 36,
  BATTLE_EFFECT_SLEEP = 37,
  BATTLE_EFFECT_BIND = 38,
  BATTLE_EFFECT_HAUNT = 39,
  BATTLE_EFFECT_CONDEMN = 40,
  BATTLE_EFFECT_CURSE = 41,
  BATTLE_EFFECT_DRAIN = 42,
  BATTLE_EFFECT_BREAK = 43,
  BATTLE_EFFECT_WISH_44 = 44,
  BATTLE_EFFECT_WISH_45 = 45,
  BATTLE_EFFECT_IMPACT_46 = 46,
  BATTLE_EFFECT_IMPAIR_47 = 47,
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
  BATTLE_EFFECT_REVIVE_201 = 201,
  BATTLE_EFFECT_VINE = 202,
  BATTLE_EFFECT_GROUND = 203,
  BATTLE_EFFECT_PLY_204 = 204,
  BATTLE_EFFECT_SLEET = 205,
  BATTLE_EFFECT_WISH_206 = 206,
  BATTLE_EFFECT_HAIL = 207,
  BATTLE_EFFECT_TONIC = 208,
  BATTLE_EFFECT_DEW = 209,
  BATTLE_EFFECT_IMPACT_210 = 210,
  BATTLE_EFFECT_GUARD_211 = 211,
  BATTLE_EFFECT_PSY_CRYSTAL = 212,
  BATTLE_EFFECT_FLASH = 213,
  BATTLE_EFFECT_TORCH = 214,
  BATTLE_EFFECT_WARD_215 = 215,
  BATTLE_EFFECT_ZEPHYR = 216,
  BATTLE_EFFECT_KITE = 217,
  BATTLE_EFFECT_BIND_218 = 218,
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
  BATTLE_EFFECT_GUST_310 = 310,
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
  BATTLE_EFFECT_POISONOUS_BITE_323 = 323,
  BATTLE_EFFECT_ELECTRIC_BITE = 324,
  BATTLE_EFFECT_POISON_TAIL = 325,
  BATTLE_EFFECT_ONSLAUGHT = 326,
  BATTLE_EFFECT_VAMPIRIC_FANG = 327,
  BATTLE_EFFECT_BACTERIA_RUSH = 328,
  BATTLE_EFFECT_FLYING_ATTACK_329 = 329,
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
  BATTLE_EFFECT_BEAR_CLAW_342 = 342,
  BATTLE_EFFECT_RANSACK_343 = 343,
  BATTLE_EFFECT_SOOTHING_STAR = 344,
  BATTLE_EFFECT_SPIDER_WEB = 345,
  BATTLE_EFFECT_HEARTRENDER_346 = 346,
  BATTLE_EFFECT_MAD_SPATTER = 347,
  BATTLE_EFFECT_SPASM = 348,
  BATTLE_EFFECT_SLEEP_STAR = 349,
  BATTLE_EFFECT_DECOMPOSE = 350,
  BATTLE_EFFECT_HAUNTING = 351,
  BATTLE_EFFECT_CURE_352 = 352,
  BATTLE_EFFECT_BERSERK = 353,
  BATTLE_EFFECT_CURSE_354 = 354,
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
  BATTLE_EFFECT_KIND_COUNT = 408 ///< one more than the last kind
};

/// An effect's variant: the strength of the ability that plays it, from bits
/// 12 and 13 of its row of actflags.tsv. Most abilities come in threes that
/// share an effect, such as Quake, Earthquake and Quake Sphere, and a double
/// or triple blow adds one or two. A runner that sets its own variants names
/// them beside it.
enum EffectVariant {
  EFFECT_VARIANT_BASIC,    ///< Quake, Douse, Ply
  EFFECT_VARIANT_STRONGER, ///< Earthquake, Drench, Ply Well
  EFFECT_VARIANT_STRONGEST ///< Quake Sphere, Deluge, Pure Ply
};

/// The handlers by kind less one (effect_table.c); BattleEffect_RunTableEffect
/// calls the one an effect's kind selects.
extern BattleFxHandler gBattleEffect_EffectHandlers[];

/// What BattleEffect_BuildCenteredIndexList (unused) reads: a value to find in
/// a list, and how many indices either side of it to keep.
typedef struct {
  s32 unk_00[3];    ///< not named by TBS code
  s32 currentValue; ///< the value looked for in values
  s32 indexRadius;  ///< indices kept either side of where it sits
  s32 entryLimit;   ///< indices from this up are dropped
  s32 unk_18[3];    ///< not named by TBS code
  s16 values[1];    ///< indexRadius * 2 + 1 values, searched in order
} BattleEffectIndexWindow;

/// The map cell buffer (gMapCellBuffer), which the effects borrow, has two
/// views here: Ram_MapCellBuffer as bytes, at its fixed address, and PARTICLES
/// as particles. The effects use these two, not gMapCellBuffer itself.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

/// Clips overflow on all four sides.
#define EFFECT_CLIP_BOUNDS (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

// Extract signed 16.16 pixels without reading an s32 through an s16 alias.
// The plain shift adds four bytes to native English effect.text overall.
#ifdef UBFIX
#define Q_16_16_TO_INT(v) ((s32)(v) >> 16)
#else
#define Q_16_16_TO_INT(v) (((s16 *)&(v))[1])
#endif

/// Two-dimensional {x, y} pair for position or velocity, a local of the
/// summon scenes (Nereid, Procne, Tiamat and Ramses).
struct Point2D {
  s32 x; ///< 16.16 pixels in every scene that uses it
  s32 y; ///< 16.16 pixels
};

/// A blitter pair: [0] built in HEAP_SLOT_BLITTER and [1] in
/// HEAP_SLOT_BLITTER_ALTERNATE (BattleEffect_FetchRectangleBlitters fills one).
/// Many effects keep this pair in their stack frame.
typedef DrawRectangle BlitterPair[2];

/// The work slots read as blitters: EFFECT_BLITTERS[HEAP_SLOT_BLITTER] and
/// EFFECT_BLITTERS[HEAP_SLOT_BLITTER_ALTERNATE].
#define EFFECT_BLITTERS ((DrawRectangle *)gWorkSlot)

// The functions the parts of effect.c define, part by part in include order.

// display.c
void BattleEffect_ConfigureEffectDisplay(void);
void BattleEffect_AdvanceScrollOnInterval(void);
void BattleEffect_AdvanceBg2OnInterval(void);
void BattleEffect_ArmWin0HBlankDma(void);

// falling.c
void BattleEffect_RunIce(struct BattleEffectArgument *effect);
void BattleEffect_RunDouse(struct BattleEffectArgument *effect);

// streaks.c
void BattleEffect_RunSonicWave(struct BattleEffectArgument *effect);
void BattleEffect_RunShriek(struct BattleEffectArgument *effect);
void BattleEffect_RunBansheeHowl(struct BattleEffectArgument *effect);
void BattleEffect_RunWarCry(struct BattleEffectArgument *effect);
void BattleEffect_RunWickedHowl(struct BattleEffectArgument *effect);
void BattleEffect_RunLimeStreaks(struct BattleEffectArgument *effect);
void BattleEffect_RunStreakVolley(struct BattleEffectArgument *effect,
                                  s32 mode);
void BattleEffect_RunSleepStar(struct BattleEffectArgument *effect);
void BattleEffect_RunSoothingStar(struct BattleEffectArgument *effect);
void BattleEffect_RunMoteFountain(struct BattleEffectArgument *effect,
                                  s32 mode);

// target_strike.c
void BattleEffect_RunNumbingSting(struct BattleEffectArgument *effect);
void BattleEffect_RunCannibalFang(struct BattleEffectArgument *effect);
void BattleEffect_RunPoisonousBite(struct BattleEffectArgument *effect);
void BattleEffect_RunRabidFang(struct BattleEffectArgument *effect);
void BattleEffect_RunElectricBite(struct BattleEffectArgument *effect);
void BattleEffect_RunPoisonTail(struct BattleEffectArgument *effect);
void BattleEffect_RunVampiricFang(struct BattleEffectArgument *effect);
void BattleEffect_RunDoubleFang(struct BattleEffectArgument *effect);
void BattleEffect_RunMortalBlow(struct BattleEffectArgument *effect);
void BattleEffect_RunTwinBeaks(struct BattleEffectArgument *effect);
void BattleEffect_RunAcidBite(struct BattleEffectArgument *effect);
void BattleEffect_RunDrainFang(struct BattleEffectArgument *effect);
void BattleEffect_RunTargetStrike(struct BattleEffectArgument *effect,
                                  s32 mode);

// death_plunge.c
void BattleEffect_RunDeathPlunge(struct BattleEffectArgument *effect);
void BattleEffect_RunBladeRain(struct BattleEffectArgument *effect);
void BattleEffect_RunElementPillar(struct BattleEffectArgument *effect);
void BattleEffect_RunShatteringOpening(struct BattleEffectArgument *effect);
void BattleEffect_RunElementBurst(struct BattleEffectArgument *effect);
void BattleEffect_DrawStreaks(void);
void BattleEffect_StartElementStreaks(s32 element);
void BattleEffect_EndElementStreaks(void);

// spider_web.c
void BattleEffect_RunPoisonBeat(struct BattleEffectArgument *effect);
void BattleEffect_RunSpinningBeat(struct BattleEffectArgument *effect);
void BattleEffect_RunSwirlSwipe(struct BattleEffectArgument *effect, s32 mode);
void BattleEffect_RunSpiderWeb(struct BattleEffectArgument *effect);

// canvas.c
void BattleEffect_WipeCanvas(s32 direction, s32 fill);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_FlushPendingGraphicsTransfer(void);
void Graphics_ApplyWindowBlendRegisters(void);
void Display_ApplyBg2Reference(void);
void Palette_StepFadeTransfer(void);
void BattleEffect_ClearActorTimers(void);
void ObjectGroup_TickMemberTimers(void);
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_OpenCanvasLayer(s32 bgControl);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_HideBg1AndRestoreBackdrop(void);
void BattleEffect_SetupCanvasTileMap(void);
void BattleEffect_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1,
                                        s32 color);

// planet_diver.c
void BattleEffect_RunPlanetDiver(struct BattleEffectArgument *effect);
void BattleEffect_RunHaunt(struct BattleEffectArgument *effect);
void BattleEffect_RunUnusedOrbitingSpirals(struct BattleEffectArgument *effect);

// gathering_lights.c
void BattleEffect_RunDemonNight(struct BattleEffectArgument *effect);
void BattleEffect_RunRumble(struct BattleEffectArgument *effect);
void BattleEffect_RunHaunting(struct BattleEffectArgument *effect);
void BattleEffect_RunGatheringLights(struct BattleEffectArgument *effect,
                                     s32 mode);

// mirrored_wall.c
void BattleEffect_FetchRectangleBlitters(s32 flip, BlitterPair out);
void BattleEffect_RunBerserk(struct BattleEffectArgument *effect);
void BattleEffect_RunSpasm(struct BattleEffectArgument *effect);
void BattleEffect_RunRecovery(struct BattleEffectArgument *effect);
void BattleEffect_RunMirroredWall(struct BattleEffectArgument *effect,
                                  s32 mode);

// djinn_beams.c
void BattleEffect_RunRevive(struct BattleEffectArgument *effect);
void BattleEffect_RunDew(struct BattleEffectArgument *effect);
void BattleEffect_RunDjinnBeams(struct BattleEffectArgument *effect,
                                s32 element);

// lobbed_shells.c
void BattleEffect_RunMadSpatter(struct BattleEffectArgument *effect);
void BattleEffect_RunDecompose(struct BattleEffectArgument *effect);
void BattleEffect_RunBoneChiller(struct BattleEffectArgument *effect);
void BattleEffect_RunStickyGoo(struct BattleEffectArgument *effect);
void BattleEffect_RunPoisonInk(struct BattleEffectArgument *effect);
void BattleEffect_RunStickyPoison(struct BattleEffectArgument *effect);
void BattleEffect_RunRottenBlood(struct BattleEffectArgument *effect);
void BattleEffect_RunLobbedShells(struct BattleEffectArgument *effect,
                                  s32 mode);

// condemn.c
void BattleEffect_RunCondemn(struct BattleEffectArgument *effect);

// bind.c
void Graphics_ConvertBackgroundToBlueRamp(void);
void BattleEffect_RunBind(struct BattleEffectArgument *effect);
void BattleEffect_RunUnusedTriangleStrike(struct BattleEffectArgument *effect);

// stars.c
void BattleEffect_RunAstralBlast(struct BattleEffectArgument *effect);
void BattleEffect_RunShiningStar(struct BattleEffectArgument *effect);

// procne.c
void BattleEffect_RunProcne(struct BattleEffectArgument *effect);

// tiamat.c
void BattleEffect_RunDragonDriver(struct BattleEffectArgument *effect);
void BattleEffect_RunTiamat(struct BattleEffectArgument *effect);
void BattleEffect_RunPaletteParticles(struct BattleEffectArgument *effect,
                                      s32 mode);

// nereid.c
void BattleEffect_RunNereid(struct BattleEffectArgument *effect);

// vortices.c
void BattleEffect_RunFroth(struct BattleEffectArgument *effect);
void BattleEffect_RunWhirlwind(struct BattleEffectArgument *effect);

// prism.c
void BattleEffect_RunPrism(struct BattleEffectArgument *effect);
void Graphics_UpdatePhasePalette(s32 frame, s32 redPhase, s32 greenPhase,
                                 s32 bluePhase);

// plasma.c
void BattleEffect_RunPlasma(struct BattleEffectArgument *effect);
void BattleEffect_RunNova(struct BattleEffectArgument *effect);
void BattleEffect_RunDynamite(struct BattleEffectArgument *effect);
void BattleEffect_RunSparkGroups(struct BattleEffectArgument *effect, s32 mode);

// volcano.c
void BattleEffect_RunVolcano(struct BattleEffectArgument *effect);

// target_motes.c
void BattleEffect_RunTaint(struct BattleEffectArgument *effect);
void BattleEffect_RunUnusedPinkSmoke(struct BattleEffectArgument *effect);
void BattleEffect_RunDelude(struct BattleEffectArgument *effect);
void BattleEffect_RunParalyze(struct BattleEffectArgument *effect);
void BattleEffect_RunHeartrender(struct BattleEffectArgument *effect);
void BattleEffect_RunMysticFlame(struct BattleEffectArgument *effect);
void BattleEffect_RunDemonFire(struct BattleEffectArgument *effect);
void BattleEffect_RunDrown(struct BattleEffectArgument *effect);
void BattleEffect_RunTargetMotes(struct BattleEffectArgument *effect, u32 mode);
void BattleEffect_RunSleep(struct BattleEffectArgument *effect);
void BattleEffect_RunCurse(struct BattleEffectArgument *effect);

// djinn_heal.c
void BattleEffect_RunDjinnHeal(struct BattleEffectArgument *effect);
void Camera_ApplyPhasedDelta(void);

// dispatch.c
void BattleEffect_CallDoNothing(s32 count);
void BattleEffect_RunSceneEffect(struct BattleEffectArgument *effect);
void BattleEffect_RunTableEffect(struct BattleEffectArgument *effect);
void BattleEffect_BuildWindowEdgeTable(void);
void BattleEffect_SelectLivingTargets(struct BattleEffectArgument *effect);
void BattleEffect_SetupBlendedDisplay(void);
void ObjectGroup_UpdateMembers(s32 actorId, s32 palette, s32 animation,
                               s32 timerSlot, s32 timerFrames);
void BattleEffect_InitializeTilemapAndFlushQueue(void);

// cybele.c
void BattleEffect_RunCybele(struct BattleEffectArgument *effect);

// boreas.c
void BattleEffect_RunBoreas(struct BattleEffectArgument *effect);

// drain.c
void BattleEffect_RunBreak(struct BattleEffectArgument *effect);
void BattleEffect_RunDrain(struct BattleEffectArgument *effect);

// healing.c
void BattleEffect_RunPly(struct BattleEffectArgument *effect);
void BattleEffect_RunWish(struct BattleEffectArgument *effect);
void BattleEffect_RunCure(struct BattleEffectArgument *effect);
void BattleEffect_RunCurePoison(struct BattleEffectArgument *effect);
void BattleEffect_RunUnusedRestoreMotes(struct BattleEffectArgument *effect);
void BattleEffect_RunTonic(struct BattleEffectArgument *effect);
void BattleEffect_RunPsyCrystal(struct BattleEffectArgument *effect);
void BattleEffect_RunHealingMotes(struct BattleEffectArgument *effect,
                                  u32 mode);

// stat_change.c
void BattleEffect_RunImpact(struct BattleEffectArgument *effect);
void BattleEffect_RunGuard(struct BattleEffectArgument *effect);
void BattleEffect_RunWard(struct BattleEffectArgument *effect);
void BattleEffect_RunGranite(struct BattleEffectArgument *effect);
void BattleEffect_RunFlash(struct BattleEffectArgument *effect);
void BattleEffect_RunZephyr(struct BattleEffectArgument *effect);
void BattleEffect_RunStatBoost(struct BattleEffectArgument *effect, u32 mode);
void BattleEffect_RunDull(struct BattleEffectArgument *effect);
void BattleEffect_RunImpair(struct BattleEffectArgument *effect);
void BattleEffect_RunWeaken(struct BattleEffectArgument *effect);
void BattleEffect_RunUnusedStatReduction(struct BattleEffectArgument *effect);
void BattleEffect_RunStatReduction(struct BattleEffectArgument *effect,
                                   s32 mode);

// flare.c
void BattleEffect_RunFlare(struct BattleEffectArgument *effect);
s32 BattleEffect_BuildCenteredIndexList(BattleEffectIndexWindow *window,
                                        s16 *output);

// quake.c
void BattleEffect_RunQuake(struct BattleEffectArgument *effect);

// fire.c
void BattleEffect_RunFire(struct BattleEffectArgument *effect);

// frost.c
void BattleEffect_RunFrost(struct BattleEffectArgument *effect);

// ray.c
void BattleEffect_RunRay(struct BattleEffectArgument *effect);

// eruption.c
void BattleEffect_RunSmokeBomb(struct BattleEffectArgument *effect);
void BattleEffect_RunBlast(struct BattleEffectArgument *effect);
void BattleEffect_RunEruption(struct BattleEffectArgument *effect, s32 mode);
void BattleEffect_SpawnObjects(s32 entryCount, s32 source, u32 priority);
void BattleEffect_DoNothing(void);

// burst_shower.c
void BattleEffect_ArmBg2AffineHBlankDma(void);
void BattleEffect_RunFireBlessing(struct BattleEffectArgument *effect);
void BattleEffect_RunWaterBlessing(struct BattleEffectArgument *effect);
void BattleEffect_RunDarkBlessing(struct BattleEffectArgument *effect);
void BattleEffect_RunAcidBlessing(struct BattleEffectArgument *effect);
void BattleEffect_RunStormBlessing(struct BattleEffectArgument *effect);
void BattleEffect_RunEvilBlessing(struct BattleEffectArgument *effect);
void BattleEffect_RunDeadlyGas(struct BattleEffectArgument *effect);
void BattleEffect_RunBurstShower(struct BattleEffectArgument *effect, s32 mode);

// orbiting.c
void BattleEffect_RunSwirlingStars(struct BattleEffectArgument *effect);
void BattleEffect_RunOrbitingSkulls(void);
void BattleEffect_RunTargetBursts(struct BattleEffectArgument *effect);

// atalanta.c
void BattleEffect_RunAtalanta(struct BattleEffectArgument *effect);

// growth.c
void BattleEffect_RunGrowth(struct BattleEffectArgument *effect);
void BattleEffect_RunPunji(struct BattleEffectArgument *effect);
void BattleEffect_RunGrowingColumns(struct BattleEffectArgument *effect,
                                    s32 mode);
void BattleEffect_RunVine(struct BattleEffectArgument *effect);
void BattleEffect_RunThorn(struct BattleEffectArgument *effect);

// bolt.c
void BattleEffect_RunBolt(struct BattleEffectArgument *effect);

// projectiles.c
void BattleEffect_RunDjinniEntrance(struct BattleEffectArgument *effect,
                                    s32 element, s32 side, s32 anchor,
                                    s32 *outX, s32 *outY);
void BattleEffect_RunSlash(struct BattleEffectArgument *effect);
void BattleEffect_RunShuriken(struct BattleEffectArgument *effect);
void BattleEffect_RunCrazyVoice(struct BattleEffectArgument *effect);
void BattleEffect_RunWingFlutter(struct BattleEffectArgument *effect);
void BattleEffect_RunWingStroke(struct BattleEffectArgument *effect);
void BattleEffect_RunWingBeat(struct BattleEffectArgument *effect);
void BattleEffect_RunCuttingEdge(struct BattleEffectArgument *effect);
void BattleEffect_RunHeatWave(struct BattleEffectArgument *effect);
void BattleEffect_RunSleet(struct BattleEffectArgument *effect);
void BattleEffect_RunIceBlessing(struct BattleEffectArgument *effect);
void BattleEffect_RunBlizzard(struct BattleEffectArgument *effect);
void BattleEffect_RunProjectileVolley(struct BattleEffectArgument *effect,
                                      s32 mode);
void BattleEffect_InitializeRisingObject(s32 id);
void BattleEffect_SetApproachMotion(s32 id, s32 targetId, s32 frames);

// rushing_strike.c
void Graphics_PackTileRows(u8 *src, u8 *dst, s32 stride, s32 rowCount);
void BattleEffect_RunBoneCharge(struct BattleEffectArgument *effect);
void BattleEffect_RunBruteForce(struct BattleEffectArgument *effect);
void BattleEffect_RunOnslaught(struct BattleEffectArgument *effect);
void BattleEffect_RunHeadbutt(struct BattleEffectArgument *effect);
void BattleEffect_RunRushingStrike(struct BattleEffectArgument *effect,
                                   s32 mode);

// djinn_attacks.c
void Graphics_TransposeCopy(const u8 *src, u8 *dst, s32 w, s32 h);
void BattleEffect_RunMercury(struct BattleEffectArgument *effect);
void BattleEffect_RunJupiter(struct BattleEffectArgument *effect);
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette);
void BattleEffect_RunVenus(struct BattleEffectArgument *effect);
void BattleEffect_RunMars(struct BattleEffectArgument *effect);

// hail.c
void BattleEffect_RunHail(struct BattleEffectArgument *effect);
void BattleEffect_RunGround(struct BattleEffectArgument *effect);
void Camera_ApplyShake(s32 rangeX, u32 rangeY);

// thor.c
void BattleEffect_RunThor(struct BattleEffectArgument *effect);

// spire.c
void BattleEffect_RunSpire(struct BattleEffectArgument *effect);

// burst_scene.c
void BattleEffect_RunGust(struct BattleEffectArgument *effect);
void BattleEffect_RunBearClaw(struct BattleEffectArgument *effect);
void BattleEffect_RunRansack(struct BattleEffectArgument *effect);
void BattleEffect_RunBacteriaRush(struct BattleEffectArgument *effect);
void BattleEffect_RunFreebiteRush(struct BattleEffectArgument *effect);
void BattleEffect_RunFlyingAttack(struct BattleEffectArgument *effect);
void BattleEffect_RunBurstScene(struct BattleEffectArgument *effect, s32 mode);

// gaia.c
void BattleEffect_RunGaia(struct BattleEffectArgument *effect);

// positions.c
void EffectStep_AdvanceWithGravity3D(struct EffectStep *step, s32 damping,
                                     s32 gravity);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping,
                                     s32 gravity);
s32 EffectPosition_ProjectPoint(const void *point,
                                struct EffectPosition *position);
void EffectPosition_ProjectSprite(s32 id, struct EffectPosition *position);
void EffectPosition_ProjectScaledBattler(s32 id,
                                         struct EffectPosition *position);
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position);
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen);
void BattleEffect_ProbeKeysForExtendedActor(void);
void BattleEffect_DispatchByIdRange(struct BattleEffectArgument *effect);

// beams.c
void BattleEffect_RunBeamSequence(struct BattleEffectArgument *effect);
void BattleEffect_RunRingAndSparkScene(struct BattleEffectArgument *effect);

// casting_impact.c
void BattleEffect_StepPaletteToResource(s32 resourceId);
void BattleEffect_RunCastingImpact(struct BattleEffectArgument *effect,
                                   s32 mode);

// torch.c
void BattleEffect_RunTorch(struct BattleEffectArgument *effect);
void BattleEffect_RunKite(struct BattleEffectArgument *effect);

// helm_splitter.c
void BattleEffect_RunHelmSplitter(struct BattleEffectArgument *effect);
void BattleEffect_PlaceFormationObjects(s32 formation, s32 x, s32 z);
void BattleEffect_RunImpactBurst(s32 formation, s32 x, s32 y);

// particle_streams.c
void Palette_BrightenBgEntries(s32 blueStep, s32 greenStep, s32 redStep);
void BattleEffect_ArmPaletteHBlankDma(void);
void BattleEffect_RunMeteor(struct BattleEffectArgument *effect);
void BattleEffect_RunOuterSpace(struct BattleEffectArgument *effect);
void BattleEffect_RunParticleStreams(struct BattleEffectArgument *effect,
                                     s32 mode);

// ramses.c
void BattleEffect_RunRamses(struct BattleEffectArgument *effect);

// dragon_cloud.c
void BattleEffect_RunDragonCloud(struct BattleEffectArgument *effect);

// annihilation.c
void BattleEffect_RunAnnihilation(struct BattleEffectArgument *effect);

// ragnarok.c
void BattleEffect_RunRagnarok(struct BattleEffectArgument *effect);

// titan_blade.c
void BattleEffect_RunTitanBlade(struct BattleEffectArgument *effect);

// judgment.c
void BattleEffect_RunJudgment(struct BattleEffectArgument *effect);

// kirin.c
void BattleEffect_RunKirin(struct BattleEffectArgument *effect);

// neptune.c
void Graphics_ResetBg2Pa(void);
void Graphics_SetBg2AffineScaleHalf(void);
void BattleEffect_RunNeptune(struct BattleEffectArgument *effect);

// thrash.c
void BattleEffect_RunThrash(struct BattleEffectArgument *effect);
void BattleEffect_RunUnusedSevereBlowLeap(struct BattleEffectArgument *effect);
void BattleEffect_RunQuickStrike(struct BattleEffectArgument *effect);
void BattleEffect_RunVorpalSlash(struct BattleEffectArgument *effect);
void BattleEffect_RunFlashingStrike(struct BattleEffectArgument *effect,
                                    s32 mode);

// undead_sword.c
void BattleEffect_RunUndeadSword(struct BattleEffectArgument *effect);

// blitter.c
s32 BattleEffect_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift,
                              s32 flags, u32 mode);

#endif
