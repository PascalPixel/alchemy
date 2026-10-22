#ifndef ALCHEMY_UNIT_EFFECT_EFFECT_H
#define ALCHEMY_UNIT_EFFECT_EFFECT_H

#include "types.h"
#include "render_placement.h"
#include "battle_effect_work.h"
#include "battle_effect.h"

struct BattleEffectArgument;
struct EffectPosition;
struct EffectStep;

/// The large effects BattleFx_RunSceneEffect runs, by kind: the twelve
/// summons. A summon's row of actflags.tsv has effect 0x8000 plus its kind.
enum SceneEffectKind {
  SCENE_EFFECT_NONE,     ///< no row uses it; runs Meteor's scene too
  SCENE_EFFECT_RAMSES,   ///< BattleFx_RunRamses (ramses.c)
  SCENE_EFFECT_NEREID,   ///< BattleFx_RunNereid (nereid.c)
  SCENE_EFFECT_KIRIN,    ///< BattleFx_RunKirin (kirin.c)
  SCENE_EFFECT_ATALANTA, ///< BattleFx_RunAtalanta (atalanta.c)
  SCENE_EFFECT_CYBELE,   ///< BattleFx_RunCybele (cybele.c)
  SCENE_EFFECT_NEPTUNE,  ///< BattleFx_RunNeptune (neptune.c)
  SCENE_EFFECT_TIAMAT,   ///< BattleFx_RunTiamat (tiamat.c)
  SCENE_EFFECT_PROCNE,   ///< BattleFx_RunProcne (procne.c)
  SCENE_EFFECT_JUDGMENT, ///< BattleFx_RunJudgment (judgment.c)
  SCENE_EFFECT_BOREAS,   ///< BattleFx_RunBoreas (boreas.c)
  SCENE_EFFECT_METEOR,   ///< BattleFx_RunMeteor (particle_streams.c)
  SCENE_EFFECT_THOR      ///< BattleFx_RunThor (thor.c)
};

/// An effect handler: plays one effect over the battle, frame by frame, and
/// returns when it is over.
typedef void (*BattleFxHandler)(struct BattleEffectArgument *effect);
/// Effect kinds: the kind an ability's gBattle_ActionFlags entry selects, by
/// the handler that plays it; a later kind that shares a handler adds its
/// number. Kinds without a handler have no name.
enum BattleFxKind {
  BATTLE_FX_NONE = 0, ///< no effect: the dispatcher only clears the variant
  BATTLE_FX_QUAKE = 1,
  BATTLE_FX_SPIRE = 2,
  BATTLE_FX_GAIA = 3,
  BATTLE_FX_GROWTH = 4,
  BATTLE_FX_THORN = 5,
  BATTLE_FX_FROST = 6,
  BATTLE_FX_ICE = 7,
  BATTLE_FX_PRISM = 8,
  BATTLE_FX_DOUSE = 9,
  BATTLE_FX_FROTH = 10,
  BATTLE_FX_FLARE = 11,
  BATTLE_FX_FIRE = 12,
  BATTLE_FX_VOLCANO = 13,
  BATTLE_FX_BLAST = 14,
  BATTLE_FX_NOVA = 15,
  BATTLE_FX_BOLT = 16,
  BATTLE_FX_RAY = 17,
  BATTLE_FX_PLASMA = 18,
  BATTLE_FX_SLASH = 19,
  BATTLE_FX_WHIRLWIND = 20,
  BATTLE_FX_CURE = 21,
  BATTLE_FX_PLY = 22,
  BATTLE_FX_WISH = 23,
  BATTLE_FX_CURE_POISON = 24,
  BATTLE_FX_REVIVE = 25,
  BATTLE_FX_IMPACT = 26,
  BATTLE_FX_DULL = 27,
  BATTLE_FX_GUARD = 28,
  BATTLE_FX_IMPAIR = 29,
  BATTLE_FX_WARD = 30,
  BATTLE_FX_WEAKEN = 31,
  BATTLE_FX_TAINT = 32,
  BATTLE_FX_DELUDE = 33,
  BATTLE_FX_UNUSED_ORBITING_SPIRALS = 34, ///< no ability uses it
  BATTLE_FX_HEARTRENDER = 35,
  BATTLE_FX_PARALYZE = 36,
  BATTLE_FX_SLEEP = 37,
  BATTLE_FX_BIND = 38,
  BATTLE_FX_HAUNT = 39,
  BATTLE_FX_CONDEMN = 40,
  BATTLE_FX_CURSE = 41,
  BATTLE_FX_DRAIN = 42,
  BATTLE_FX_BREAK = 43,
  BATTLE_FX_WISH_44 = 44,
  BATTLE_FX_WISH_45 = 45,
  BATTLE_FX_IMPACT_46 = 46,
  BATTLE_FX_IMPAIR_47 = 47,
  BATTLE_FX_DRAGON_CLOUD = 49,
  BATTLE_FX_DEMON_NIGHT = 50,
  BATTLE_FX_HELM_SPLITTER = 51,
  BATTLE_FX_QUICK_STRIKE = 52,
  BATTLE_FX_DEATH_PLUNGE = 53,
  BATTLE_FX_SHURIKEN = 54,
  BATTLE_FX_ANNIHILATION = 55,
  BATTLE_FX_PUNJI = 56,
  BATTLE_FX_CUTTING_EDGE = 57,
  BATTLE_FX_RAGNAROK = 58,
  BATTLE_FX_HEAT_WAVE = 59,
  BATTLE_FX_PLANET_DIVER = 60,
  BATTLE_FX_TITAN_BLADE = 61,
  BATTLE_FX_SHINING_STAR = 62,
  BATTLE_FX_BLIZZARD = 63,
  BATTLE_FX_DEMON_FIRE = 64,
  BATTLE_FX_VORPAL_SLASH = 65,
  BATTLE_FX_DROWN = 66,
  BATTLE_FX_UNUSED_TRIANGLE_STRIKE = 67, ///< no ability uses it
  BATTLE_FX_ASTRAL_BLAST = 68,
  BATTLE_FX_GRANITE = 200,
  BATTLE_FX_REVIVE_201 = 201,
  BATTLE_FX_VINE = 202,
  BATTLE_FX_GROUND = 203,
  BATTLE_FX_PLY_204 = 204,
  BATTLE_FX_SLEET = 205,
  BATTLE_FX_WISH_206 = 206,
  BATTLE_FX_HAIL = 207,
  BATTLE_FX_TONIC = 208,
  BATTLE_FX_DEW = 209,
  BATTLE_FX_IMPACT_210 = 210,
  BATTLE_FX_GUARD_211 = 211,
  BATTLE_FX_PSY_CRYSTAL = 212,
  BATTLE_FX_FLASH = 213,
  BATTLE_FX_TORCH = 214,
  BATTLE_FX_WARD_215 = 215,
  BATTLE_FX_ZEPHYR = 216,
  BATTLE_FX_KITE = 217,
  BATTLE_FX_BIND_218 = 218,
  BATTLE_FX_GUST = 219,
  BATTLE_FX_VENUS = 250,
  BATTLE_FX_MERCURY = 251,
  BATTLE_FX_MARS = 252,
  BATTLE_FX_JUPITER = 253,
  BATTLE_FX_SONIC_WAVE = 300,
  BATTLE_FX_SHRIEK = 301,
  BATTLE_FX_BANSHEE_HOWL = 302,
  BATTLE_FX_CRAZY_VOICE = 303,
  BATTLE_FX_WAR_CRY = 304,
  BATTLE_FX_WICKED_HOWL = 305,
  BATTLE_FX_WING_BEAT = 306,
  BATTLE_FX_WING_FLUTTER = 307,
  BATTLE_FX_RUMBLE = 308,
  BATTLE_FX_BONE_CHILLER = 309,
  BATTLE_FX_GUST_310 = 310,
  BATTLE_FX_BONE_CHARGE = 311,
  BATTLE_FX_MYSTIC_FLAME = 312,
  BATTLE_FX_NUMBING_STING = 313,
  BATTLE_FX_BRUTE_FORCE = 314,
  BATTLE_FX_STICKY_GOO = 315,
  BATTLE_FX_CANNIBAL_FANG = 316,
  BATTLE_FX_BEAR_CLAW = 317,
  BATTLE_FX_POISONOUS_BITE = 318,
  BATTLE_FX_FLYING_ATTACK = 319,
  BATTLE_FX_UNDEAD_SWORD = 320,
  BATTLE_FX_RANSACK = 321,
  BATTLE_FX_STICKY_POISON = 322,
  BATTLE_FX_POISONOUS_BITE_323 = 323,
  BATTLE_FX_ELECTRIC_BITE = 324,
  BATTLE_FX_POISON_TAIL = 325,
  BATTLE_FX_ONSLAUGHT = 326,
  BATTLE_FX_VAMPIRIC_FANG = 327,
  BATTLE_FX_BACTERIA_RUSH = 328,
  BATTLE_FX_FLYING_ATTACK_329 = 329,
  BATTLE_FX_ROTTEN_BLOOD = 330,
  BATTLE_FX_DOUBLE_FANG = 332,
  BATTLE_FX_MORTAL_BLOW = 333,
  BATTLE_FX_FREEBITE_RUSH = 334,
  BATTLE_FX_TWIN_BEAKS = 335,
  BATTLE_FX_RABID_FANG = 336,
  BATTLE_FX_ACID_BITE = 337,
  BATTLE_FX_DYNAMITE = 338,
  BATTLE_FX_HEADBUTT = 339,
  BATTLE_FX_POISON_INK = 340,
  BATTLE_FX_BEAR_CLAW_342 = 342,
  BATTLE_FX_RANSACK_343 = 343,
  BATTLE_FX_SOOTHING_STAR = 344,
  BATTLE_FX_SPIDER_WEB = 345,
  BATTLE_FX_HEARTRENDER_346 = 346,
  BATTLE_FX_MAD_SPATTER = 347,
  BATTLE_FX_SPASM = 348,
  BATTLE_FX_SLEEP_STAR = 349,
  BATTLE_FX_DECOMPOSE = 350,
  BATTLE_FX_HAUNTING = 351,
  BATTLE_FX_CURE_352 = 352,
  BATTLE_FX_BERSERK = 353,
  BATTLE_FX_CURSE_354 = 354,
  BATTLE_FX_SMOKE_BOMB = 380,
  BATTLE_FX_WING_STROKE = 381,
  BATTLE_FX_POISON_BEAT = 382,
  BATTLE_FX_SPINNING_BEAT = 383,
  BATTLE_FX_OUTER_SPACE = 384,
  BATTLE_FX_DRAGON_DRIVER = 385,
  BATTLE_FX_DRAIN_FANG = 386,
  BATTLE_FX_UNUSED_SEVERE_BLOW_LEAP = 387, ///< no ability uses it; The Lost
                                           ///< Age's Severe Blow does
  BATTLE_FX_THRASH = 388,
  BATTLE_FX_RECOVERY = 389,
  BATTLE_FX_FIRE_BLESSING = 400,
  BATTLE_FX_WATER_BLESSING = 401,
  BATTLE_FX_ICE_BLESSING = 402,
  BATTLE_FX_DARK_BLESSING = 403,
  BATTLE_FX_ACID_BLESSING = 404,
  BATTLE_FX_STORM_BLESSING = 405,
  BATTLE_FX_EVIL_BLESSING = 406,
  BATTLE_FX_DEADLY_GAS = 407,
  BATTLE_FX_KIND_COUNT = 408 ///< one more than the last kind
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

/// The handlers by kind less one (effect_table.c); BattleFx_RunTableEffect
/// calls the one an effect's kind selects.
extern BattleFxHandler gBattleFx_EffectHandlers[];

/// What BattleFx_BuildCenteredIndexList (unused) reads: a value to find in a
/// list, and how many indices either side of it to keep.
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
/// HEAP_SLOT_BLITTER_ALTERNATE (BattleFx_FetchRectangleBlitters fills one).
/// Many effects keep this pair in their stack frame.
typedef DrawRectangle BlitterPair[2];

/// The work slots read as blitters: EFFECT_BLITTERS[HEAP_SLOT_BLITTER] and
/// EFFECT_BLITTERS[HEAP_SLOT_BLITTER_ALTERNATE].
#define EFFECT_BLITTERS ((DrawRectangle *)gWorkSlot)

// The functions the parts of effect.c define, part by part in include order.

// display.c
void BattleFx_ConfigureEffectDisplay(void);
void BattleFx_AdvanceScrollOnInterval(void);
void BattleFx_AdvanceBg2OnInterval(void);
void BattleFx_ArmWin0HBlankDma(void);

// falling.c
void BattleFx_RunIce(struct BattleEffectArgument *effect);
void BattleFx_RunDouse(struct BattleEffectArgument *effect);

// streaks.c
void BattleFx_RunSonicWave(struct BattleEffectArgument *effect);
void BattleFx_RunShriek(struct BattleEffectArgument *effect);
void BattleFx_RunBansheeHowl(struct BattleEffectArgument *effect);
void BattleFx_RunWarCry(struct BattleEffectArgument *effect);
void BattleFx_RunWickedHowl(struct BattleEffectArgument *effect);
void BattleFx_RunLimeStreaks(struct BattleEffectArgument *effect);
void BattleFx_RunStreakVolley(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunSleepStar(struct BattleEffectArgument *effect);
void BattleFx_RunSoothingStar(struct BattleEffectArgument *effect);
void BattleFx_RunMoteFountain(struct BattleEffectArgument *effect, s32 mode);

// target_strike.c
void BattleFx_RunNumbingSting(struct BattleEffectArgument *effect);
void BattleFx_RunCannibalFang(struct BattleEffectArgument *effect);
void BattleFx_RunPoisonousBite(struct BattleEffectArgument *effect);
void BattleFx_RunRabidFang(struct BattleEffectArgument *effect);
void BattleFx_RunElectricBite(struct BattleEffectArgument *effect);
void BattleFx_RunPoisonTail(struct BattleEffectArgument *effect);
void BattleFx_RunVampiricFang(struct BattleEffectArgument *effect);
void BattleFx_RunDoubleFang(struct BattleEffectArgument *effect);
void BattleFx_RunMortalBlow(struct BattleEffectArgument *effect);
void BattleFx_RunTwinBeaks(struct BattleEffectArgument *effect);
void BattleFx_RunAcidBite(struct BattleEffectArgument *effect);
void BattleFx_RunDrainFang(struct BattleEffectArgument *effect);
void BattleFx_RunTargetStrike(struct BattleEffectArgument *effect, s32 mode);

// death_plunge.c
void BattleFx_RunDeathPlunge(struct BattleEffectArgument *effect);
void BattleFx_RunBladeRain(struct BattleEffectArgument *effect);
void BattleFx_RunElementPillar(struct BattleEffectArgument *effect);
void BattleFx_RunShatteringOpening(struct BattleEffectArgument *effect);
void BattleFx_RunElementBurst(struct BattleEffectArgument *effect);
void BattleFx_DrawStreaks(void);
void BattleFx_StartElementStreaks(s32 element);
void BattleFx_EndElementStreaks(void);

// spider_web.c
void BattleFx_RunPoisonBeat(struct BattleEffectArgument *effect);
void BattleFx_RunSpinningBeat(struct BattleEffectArgument *effect);
void BattleFx_RunSwirlSwipe(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunSpiderWeb(struct BattleEffectArgument *effect);

// canvas.c
void BattleFx_WipeCanvas(s32 direction, s32 fill);
void BattleFx_ProcessPendingGraphicsTransfer(void);
void BattleFx_FlushPendingGraphicsTransfer(void);
void Graphics_ApplyWindowBlendRegisters(void);
void Display_ApplyBg2Reference(void);
void Palette_StepFadeTransfer(void);
void BattleFx_ClearActorTimers(void);
void ObjectGroup_TickMemberTimers(void);
void BattleFx_BeginCanvasLayer(s32 bgControl);
void BattleFx_OpenCanvasLayer(s32 bgControl);
void BattleFx_BeginTiledCanvas(s32 bgControl);
void BattleFx_EndCanvasLayer(void);
void BattleFx_HideBg1AndRestoreBackdrop(void);
void BattleFx_SetupCanvasTileMap(void);
void BattleFx_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);

// planet_diver.c
void BattleFx_RunPlanetDiver(struct BattleEffectArgument *effect);
void BattleFx_RunHaunt(struct BattleEffectArgument *effect);
void BattleFx_RunUnusedOrbitingSpirals(struct BattleEffectArgument *effect);

// gathering_lights.c
void BattleFx_RunDemonNight(struct BattleEffectArgument *effect);
void BattleFx_RunRumble(struct BattleEffectArgument *effect);
void BattleFx_RunHaunting(struct BattleEffectArgument *effect);
void BattleFx_RunGatheringLights(struct BattleEffectArgument *effect, s32 mode);

// mirrored_wall.c
void BattleFx_FetchRectangleBlitters(s32 flip, BlitterPair out);
void BattleFx_RunBerserk(struct BattleEffectArgument *effect);
void BattleFx_RunSpasm(struct BattleEffectArgument *effect);
void BattleFx_RunRecovery(struct BattleEffectArgument *effect);
void BattleFx_RunMirroredWall(struct BattleEffectArgument *effect, s32 mode);

// djinn_beams.c
void BattleFx_RunRevive(struct BattleEffectArgument *effect);
void BattleFx_RunDew(struct BattleEffectArgument *effect);
void BattleFx_RunDjinnBeams(struct BattleEffectArgument *effect, s32 element);

// lobbed_shells.c
void BattleFx_RunMadSpatter(struct BattleEffectArgument *effect);
void BattleFx_RunDecompose(struct BattleEffectArgument *effect);
void BattleFx_RunBoneChiller(struct BattleEffectArgument *effect);
void BattleFx_RunStickyGoo(struct BattleEffectArgument *effect);
void BattleFx_RunPoisonInk(struct BattleEffectArgument *effect);
void BattleFx_RunStickyPoison(struct BattleEffectArgument *effect);
void BattleFx_RunRottenBlood(struct BattleEffectArgument *effect);
void BattleFx_RunLobbedShells(struct BattleEffectArgument *effect, s32 mode);

// condemn.c
void BattleFx_RunCondemn(struct BattleEffectArgument *effect);

// bind.c
void Graphics_ConvertBackgroundToBlueRamp(void);
void BattleFx_RunBind(struct BattleEffectArgument *effect);
void BattleFx_RunUnusedTriangleStrike(struct BattleEffectArgument *effect);

// stars.c
void BattleFx_RunAstralBlast(struct BattleEffectArgument *effect);
void BattleFx_RunShiningStar(struct BattleEffectArgument *effect);

// procne.c
void BattleFx_RunProcne(struct BattleEffectArgument *effect);

// tiamat.c
void BattleFx_RunDragonDriver(struct BattleEffectArgument *effect);
void BattleFx_RunTiamat(struct BattleEffectArgument *effect);
void BattleFx_RunPaletteParticles(struct BattleEffectArgument *effect,
                                  s32 mode);

// nereid.c
void BattleFx_RunNereid(struct BattleEffectArgument *effect);

// vortices.c
void BattleFx_RunFroth(struct BattleEffectArgument *effect);
void BattleFx_RunWhirlwind(struct BattleEffectArgument *effect);

// prism.c
void BattleFx_RunPrism(struct BattleEffectArgument *effect);
void Graphics_UpdatePhasePalette(s32 frame, s32 redPhase, s32 greenPhase,
                                 s32 bluePhase);

// plasma.c
void BattleFx_RunPlasma(struct BattleEffectArgument *effect);
void BattleFx_RunNova(struct BattleEffectArgument *effect);
void BattleFx_RunDynamite(struct BattleEffectArgument *effect);
void BattleFx_RunSparkGroups(struct BattleEffectArgument *effect, s32 mode);

// volcano.c
void BattleFx_RunVolcano(struct BattleEffectArgument *effect);

// target_motes.c
void BattleFx_RunTaint(struct BattleEffectArgument *effect);
void BattleFx_RunUnusedPinkSmoke(struct BattleEffectArgument *effect);
void BattleFx_RunDelude(struct BattleEffectArgument *effect);
void BattleFx_RunParalyze(struct BattleEffectArgument *effect);
void BattleFx_RunHeartrender(struct BattleEffectArgument *effect);
void BattleFx_RunMysticFlame(struct BattleEffectArgument *effect);
void BattleFx_RunDemonFire(struct BattleEffectArgument *effect);
void BattleFx_RunDrown(struct BattleEffectArgument *effect);
void BattleFx_RunTargetMotes(struct BattleEffectArgument *effect, u32 mode);
void BattleFx_RunSleep(struct BattleEffectArgument *effect);
void BattleFx_RunCurse(struct BattleEffectArgument *effect);

// djinn_heal.c
void BattleFx_RunDjinnHeal(struct BattleEffectArgument *effect);
void Camera_ApplyPhasedDelta(void);

// dispatch.c
void BattleFx_CallDoNothing(s32 count);
void BattleFx_RunSceneEffect(struct BattleEffectArgument *effect);
void BattleFx_RunTableEffect(struct BattleEffectArgument *effect);
void BattleFx_BuildWindowEdgeTable(void);
void BattleFx_SelectLivingTargets(struct BattleEffectArgument *effect);
void BattleFx_SetupBlendedDisplay(void);
void ObjectGroup_UpdateMembers(s32 actorId, s32 palette, s32 animation,
                               s32 timerSlot, s32 timerFrames);
void BattleFx_InitTilemapAndFlushQueue(void);

// cybele.c
void BattleFx_RunCybele(struct BattleEffectArgument *effect);

// boreas.c
void BattleFx_RunBoreas(struct BattleEffectArgument *effect);

// drain.c
void BattleFx_RunBreak(struct BattleEffectArgument *effect);
void BattleFx_RunDrain(struct BattleEffectArgument *effect);

// healing.c
void BattleFx_RunPly(struct BattleEffectArgument *effect);
void BattleFx_RunWish(struct BattleEffectArgument *effect);
void BattleFx_RunCure(struct BattleEffectArgument *effect);
void BattleFx_RunCurePoison(struct BattleEffectArgument *effect);
void BattleFx_RunUnusedRestoreMotes(struct BattleEffectArgument *effect);
void BattleFx_RunTonic(struct BattleEffectArgument *effect);
void BattleFx_RunPsyCrystal(struct BattleEffectArgument *effect);
void BattleFx_RunHealingMotes(struct BattleEffectArgument *effect, u32 mode);

// stat_change.c
void BattleFx_RunImpact(struct BattleEffectArgument *effect);
void BattleFx_RunGuard(struct BattleEffectArgument *effect);
void BattleFx_RunWard(struct BattleEffectArgument *effect);
void BattleFx_RunGranite(struct BattleEffectArgument *effect);
void BattleFx_RunFlash(struct BattleEffectArgument *effect);
void BattleFx_RunZephyr(struct BattleEffectArgument *effect);
void BattleFx_RunStatBoost(struct BattleEffectArgument *effect, u32 mode);
void BattleFx_RunDull(struct BattleEffectArgument *effect);
void BattleFx_RunImpair(struct BattleEffectArgument *effect);
void BattleFx_RunWeaken(struct BattleEffectArgument *effect);
void BattleFx_RunUnusedStatReduction(struct BattleEffectArgument *effect);
void BattleFx_RunStatReduction(struct BattleEffectArgument *effect, s32 mode);

// flare.c
void BattleFx_RunFlare(struct BattleEffectArgument *effect);
s32 BattleFx_BuildCenteredIndexList(BattleEffectIndexWindow *window,
                                    s16 *output);

// quake.c
void BattleFx_RunQuake(struct BattleEffectArgument *effect);

// fire.c
void BattleFx_RunFire(struct BattleEffectArgument *effect);

// frost.c
void BattleFx_RunFrost(struct BattleEffectArgument *effect);

// ray.c
void BattleFx_RunRay(struct BattleEffectArgument *effect);

// eruption.c
void BattleFx_RunSmokeBomb(struct BattleEffectArgument *effect);
void BattleFx_RunBlast(struct BattleEffectArgument *effect);
void BattleFx_RunEruption(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_SpawnObjects(s32 entryCount, s32 source, u32 priority);
void BattleFx_DoNothing(void);

// burst_shower.c
void BattleFx_ArmBg2AffineHBlankDma(void);
void BattleFx_RunFireBlessing(struct BattleEffectArgument *effect);
void BattleFx_RunWaterBlessing(struct BattleEffectArgument *effect);
void BattleFx_RunDarkBlessing(struct BattleEffectArgument *effect);
void BattleFx_RunAcidBlessing(struct BattleEffectArgument *effect);
void BattleFx_RunStormBlessing(struct BattleEffectArgument *effect);
void BattleFx_RunEvilBlessing(struct BattleEffectArgument *effect);
void BattleFx_RunDeadlyGas(struct BattleEffectArgument *effect);
void BattleFx_RunBurstShower(struct BattleEffectArgument *effect, s32 mode);

// orbiting.c
void BattleFx_RunSwirlingStars(struct BattleEffectArgument *effect);
void BattleFx_RunOrbitingSkulls(void);
void BattleFx_RunTargetBursts(struct BattleEffectArgument *effect);

// atalanta.c
void BattleFx_RunAtalanta(struct BattleEffectArgument *effect);

// growth.c
void BattleFx_RunGrowth(struct BattleEffectArgument *effect);
void BattleFx_RunPunji(struct BattleEffectArgument *effect);
void BattleFx_RunGrowingColumns(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunVine(struct BattleEffectArgument *effect);
void BattleFx_RunThorn(struct BattleEffectArgument *effect);

// bolt.c
void BattleFx_RunBolt(struct BattleEffectArgument *effect);

// projectiles.c
void BattleFx_RunDjinniEntrance(struct BattleEffectArgument *effect,
                                s32 element, s32 side, s32 anchor, s32 *outX,
                                s32 *outY);
void BattleFx_RunSlash(struct BattleEffectArgument *effect);
void BattleFx_RunShuriken(struct BattleEffectArgument *effect);
void BattleFx_RunCrazyVoice(struct BattleEffectArgument *effect);
void BattleFx_RunWingFlutter(struct BattleEffectArgument *effect);
void BattleFx_RunWingStroke(struct BattleEffectArgument *effect);
void BattleFx_RunWingBeat(struct BattleEffectArgument *effect);
void BattleFx_RunCuttingEdge(struct BattleEffectArgument *effect);
void BattleFx_RunHeatWave(struct BattleEffectArgument *effect);
void BattleFx_RunSleet(struct BattleEffectArgument *effect);
void BattleFx_RunIceBlessing(struct BattleEffectArgument *effect);
void BattleFx_RunBlizzard(struct BattleEffectArgument *effect);
void BattleFx_RunProjectileVolley(struct BattleEffectArgument *effect,
                                  s32 mode);
void BattleFx_InitializeRisingObject(s32 id);
void BattleFx_SetApproachMotion(s32 id, s32 targetId, s32 frames);

// rushing_strike.c
void Graphics_PackTileRows(u8 *src, u8 *dst, s32 stride, s32 rowCount);
void BattleFx_RunBoneCharge(struct BattleEffectArgument *effect);
void BattleFx_RunBruteForce(struct BattleEffectArgument *effect);
void BattleFx_RunOnslaught(struct BattleEffectArgument *effect);
void BattleFx_RunHeadbutt(struct BattleEffectArgument *effect);
void BattleFx_RunRushingStrike(struct BattleEffectArgument *effect, s32 mode);

// djinn_attacks.c
void Graphics_TransposeCopy(const u8 *src, u8 *dst, s32 w, s32 h);
void BattleFx_RunMercury(struct BattleEffectArgument *effect);
void BattleFx_RunJupiter(struct BattleEffectArgument *effect);
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette);
void BattleFx_RunVenus(struct BattleEffectArgument *effect);
void BattleFx_RunMars(struct BattleEffectArgument *effect);

// hail.c
void BattleFx_RunHail(struct BattleEffectArgument *effect);
void BattleFx_RunGround(struct BattleEffectArgument *effect);
void Camera_ApplyShake(s32 rangeX, u32 rangeY);

// thor.c
void BattleFx_RunThor(struct BattleEffectArgument *effect);

// spire.c
void BattleFx_RunSpire(struct BattleEffectArgument *effect);

// burst_scene.c
void BattleFx_RunGust(struct BattleEffectArgument *effect);
void BattleFx_RunBearClaw(struct BattleEffectArgument *effect);
void BattleFx_RunRansack(struct BattleEffectArgument *effect);
void BattleFx_RunBacteriaRush(struct BattleEffectArgument *effect);
void BattleFx_RunFreebiteRush(struct BattleEffectArgument *effect);
void BattleFx_RunFlyingAttack(struct BattleEffectArgument *effect);
void BattleFx_RunBurstScene(struct BattleEffectArgument *effect, s32 mode);

// gaia.c
void BattleFx_RunGaia(struct BattleEffectArgument *effect);

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
void BattleFx_ProbeKeysForExtendedActor(void);
void BattleFx_DispatchByIdRange(struct BattleEffectArgument *effect);

// beams.c
void BattleFx_RunBeamSequence(struct BattleEffectArgument *effect);
void BattleFx_RunRingAndSparkScene(struct BattleEffectArgument *effect);

// casting_impact.c
void BattleFx_StepPaletteToResource(s32 resourceId);
void BattleFx_RunCastingImpact(struct BattleEffectArgument *effect, s32 mode);

// torch.c
void BattleFx_RunTorch(struct BattleEffectArgument *effect);
void BattleFx_RunKite(struct BattleEffectArgument *effect);

// helm_splitter.c
void BattleFx_RunHelmSplitter(struct BattleEffectArgument *effect);
void BattleFx_PlaceFormationObjects(s32 formation, s32 x, s32 z);
void BattleFx_RunImpactBurst(s32 formation, s32 x, s32 y);

// particle_streams.c
void Palette_BrightenBgEntries(s32 blueStep, s32 greenStep, s32 redStep);
void BattleFx_ArmPaletteHBlankDma(void);
void BattleFx_RunMeteor(struct BattleEffectArgument *effect);
void BattleFx_RunOuterSpace(struct BattleEffectArgument *effect);
void BattleFx_RunParticleStreams(struct BattleEffectArgument *effect, s32 mode);

// ramses.c
void BattleFx_RunRamses(struct BattleEffectArgument *effect);

// dragon_cloud.c
void BattleFx_RunDragonCloud(struct BattleEffectArgument *effect);

// annihilation.c
void BattleFx_RunAnnihilation(struct BattleEffectArgument *effect);

// ragnarok.c
void BattleFx_RunRagnarok(struct BattleEffectArgument *effect);

// titan_blade.c
void BattleFx_RunTitanBlade(struct BattleEffectArgument *effect);

// judgment.c
void BattleFx_RunJudgment(struct BattleEffectArgument *effect);

// kirin.c
void BattleFx_RunKirin(struct BattleEffectArgument *effect);

// neptune.c
void Graphics_ResetBg2Pa(void);
void Graphics_SetBg2AffineScaleHalf(void);
void BattleFx_RunNeptune(struct BattleEffectArgument *effect);

// thrash.c
void BattleFx_RunThrash(struct BattleEffectArgument *effect);
void BattleFx_RunUnusedSevereBlowLeap(struct BattleEffectArgument *effect);
void BattleFx_RunQuickStrike(struct BattleEffectArgument *effect);
void BattleFx_RunVorpalSlash(struct BattleEffectArgument *effect);
void BattleFx_RunFlashingStrike(struct BattleEffectArgument *effect, s32 mode);

// undead_sword.c
void BattleFx_RunUndeadSword(struct BattleEffectArgument *effect);

// blitter.c
s32 BattleFx_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                          u32 mode);

#endif
