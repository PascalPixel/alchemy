#ifndef ALCHEMY_BATTLE_EFFECT_H
#define ALCHEMY_BATTLE_EFFECT_H

/* @file */
/* The battle effects: the effect kinds an ability plays, and the runners in */
/* src/battle_effect/ that draw them over the battle, part by part. */

#include "types.h"
#include "fixed_math.h"
#include "battle/battle_effect_state.h"
#include "battle/blitter.h"
#include "graphics/render_placement.h"

struct BATTLE_EFFECT_PARM;
struct SCR_POINT;
struct PARTICLE;

/* The battle-effect module's functions in the order of its stub table */
/* (src/battle_effect/stubs.s). */
s32 btEffectBuildBlitter(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
						 u32 mode);
void btEffectRunByIdRange(BATTLE_EFFECT_PARM *args);
void btEffectRunSummon(BATTLE_EFFECT_PARM *args);
void btEffectRunFromTable(BATTLE_EFFECT_PARM *args);
void btEffectRunElementPillar(BATTLE_EFFECT_PARM *args);
void btEffectRunShatteringOpening(BATTLE_EFFECT_PARM *args);
void btEffectRunElementBurst(BATTLE_EFFECT_PARM *args);
void btEffectStartElementStreaks(s32 element);
void btEffectEndElementStreaks(void);

/* The large effects btEffectRunSummon runs, by kind: the twelve */
/* summons. A summon's row of AbilityBattleEffects has EFFECT_SUMMON plus its */
/* kind. */
typedef enum SUMMON_EFFECT_ID {
	SUMMON_EFFECT_NONE,		/* no row uses it; runs Meteor's effect too */
	SUMMON_EFFECT_RAMSES,	/* btEffectRunRamses (battle_effect_ramses.c) */
	SUMMON_EFFECT_NEREID,	/* btEffectRunNereid (battle_effect_nereid.c) */
	SUMMON_EFFECT_KIRIN,	/* btEffectRunKirin (battle_effect_kirin.c) */
	SUMMON_EFFECT_ATALANTA, /* btEffectRunAtalanta */
							/* (battle_effect_atalanta.c) */
	SUMMON_EFFECT_CYBELE,	/* btEffectRunCybele (battle_effect_cybele.c) */
	SUMMON_EFFECT_NEPTUNE,	/* btEffectRunNeptune (battle_effect_neptune.c) */
	SUMMON_EFFECT_TIAMAT,	/* btEffectRunTiamat (battle_effect_tiamat.c) */
	SUMMON_EFFECT_PROCNE,	/* btEffectRunProcne (battle_effect_procne.c) */
	SUMMON_EFFECT_JUDGMENT, /* btEffectRunJudgment */
							/* (battle_effect_judgment.c) */
	SUMMON_EFFECT_BOREAS,	/* btEffectRunBoreas (battle_effect_boreas.c) */
	SUMMON_EFFECT_METEOR,	/* btEffectRunMeteor */
							/* (battle_effect_starfield_fall.c) */
	SUMMON_EFFECT_THOR		/* btEffectRunThor (battle_effect_thor.c) */
} SUMMON_EFFECT_ID;

/* An effect handler: plays one effect over the battle, frame by frame, */
/* and returns when it is over. */
typedef void (*BATTLE_EFFECT_HANDLER)(BATTLE_EFFECT_PARM *args);
/* Effect kinds: the kind an ability's AbilityBattleEffects entry selects, by */
/* the handler that plays it; a later kind that shares a handler is named for */
/* the ability that uses it (the Djinn from 200, the monsters' abilities from */
/* 300), or Unused when none does. Kinds without a handler have no name. */
typedef enum BATTLE_EFFECT_ID {
	BATTLE_EFFECT_NONE =
		0, /* no effect: the dispatcher only clears the variant */
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
	BATTLE_EFFECT_UNUSED_ORBITING_SPIRALS = 34, /* no ability uses it */
	BATTLE_EFFECT_UNUSED_HEARTRENDER = 35,		/* no ability uses it */
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
	BATTLE_EFFECT_UNUSED_IMPACT = 46, /* no ability uses it */
	BATTLE_EFFECT_UNUSED_IMPAIR = 47, /* no ability uses it */
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
	BATTLE_EFFECT_UNUSED_TRIANGLE_STRIKE = 67, /* no ability uses it */
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
	BATTLE_EFFECT_UNUSED_SEVERE_BLOW_LEAP =
		387, /* no ability uses it; The Lost */
			 /* Age's Severe Blow does */
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
	BATTLE_EFFECT_COUNT = 408 /* one more than the last kind */
} BATTLE_EFFECT_ID;

/* An effect's variant: the strength of the ability that plays it, from bits */
/* 12 and 13 of its row of AbilityBattleEffects. Most abilities come in threes
 */
/* that share an effect, such as Quake, Earthquake and Quake Sphere, and a */
/* double or triple blow adds one or two. A runner that sets its own variants */
/* names them beside it. */
typedef enum EFFECT_VARIANT {
	EFFECT_VARIANT_BASIC,	 /* Quake, Douse, Ply */
	EFFECT_VARIANT_STRONGER, /* Earthquake, Drench, Ply Well */
	EFFECT_VARIANT_STRONGEST /* Quake Sphere, Deluge, Pure Ply */
} EFFECT_VARIANT;

/* The handlers by kind less one (battle_effect_table.c); */
/* btEffectRunFromTable calls the one an effect's kind selects. */
extern BATTLE_EFFECT_HANDLER BattleEffect_Handlers[];

/* What btEffectBuildCenteredIndexList (unused) reads: a value to find in */
/* a list, and how many indices either side of it to keep. */
typedef struct {
	s32 unused[3];			 /* no code names it */
	s32 curValue;			 /* the value looked for in values */
	s32 idxRadius;			 /* indices kept either side of where it sits */
	s32 entryLimit;			 /* indices from this up are dropped */
	s32 unusedAfterLimit[3]; /* no code names it */
	s16 values[1];			 /* idxRadius * 2 + 1 values, searched in order */
} BATTLE_EFFECT_INDEX_WINDOW;

/* The map cell buffer (MapGridCellBuffer), which the effects borrow, has */
/* two views here: Ram_MapGridCellBuffer as bytes, at its fixed address, and */
/* PARTICLES as particles. The effects use these two, not MapGridCellBuffer */
/* itself. */
#define PARTICLES ((PARTICLE *)Ram_MapGridCellBuffer)
/* The particles PARTICLES holds. */
#define PARTICLE_POOL_COUNT 1024

/* Clips overflow on all four sides. */
#define EFFECT_CLIP_BOUNDS (BLITTER_CLIP_X | BLITTER_CLIP_Y)

/* The whole pixels of a stored 16.16 value, read as its upper halfword, as */
/* the impact burst, Neptune and the spark groups' ring flashes read them. */
/* UB: the s32 is read through an s16. */
#ifdef UBFIX
#define FIXED_INTEGER_PART(v) ((s32)(v) >> 16)
#else
#define FIXED_INTEGER_PART(v) (((s16 *)&(v))[1])
#endif

/* Two-dimensional {x, y} pair for position or velocity, a local of the */
/* summon effects (Nereid, Procne, Tiamat and Ramses). */
typedef struct POINT2_D {
	FIXED x; /* 16.16 pixels in every effect that uses it */
	FIXED y; /* 16.16 pixels */
} POINT2_D;

/* A blitter pair: [0] built in HEAP_SLOT_RAM_CODE and [1] in */
/* HEAP_SLOT_RAM_CODE_PAIRED (btEffectBuildCanvasBlitters fills one). */
/* Many effects keep this pair in their stack frame. */
typedef DRAW_RECTANGLE BLITTER_PAIR[2];

/* The heap slots read as blitters: EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE] and */
/* EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED]. */
#define EFFECT_BLITTERS ((DRAW_RECTANGLE *)HeapSlots)

/* The functions the parts of battle_effect.c define, part by part in include */
/* order. */

/* battle_effect_display.c */
void btEffectConfigureEffectDisp(void);
void btEffectAdvanceScrollOnInterval(void);
void btEffectAdvanceBg2OnInterval(void);
void btEffectArmWin0HBlankDma(void);

/* battle_effect_falling.c */
void btEffectRunIce(BATTLE_EFFECT_PARM *args);
void btEffectRunDouse(BATTLE_EFFECT_PARM *args);

/* battle_effect_streaks.c */
void btEffectRunSonicWave(BATTLE_EFFECT_PARM *args);
void btEffectRunShriek(BATTLE_EFFECT_PARM *args);
void btEffectRunBansheeHowl(BATTLE_EFFECT_PARM *args);
void btEffectRunWarCry(BATTLE_EFFECT_PARM *args);
void btEffectRunWickedHowl(BATTLE_EFFECT_PARM *args);
void btEffectRunLimeStreaks(BATTLE_EFFECT_PARM *args);
void btEffectRunStreakVolley(BATTLE_EFFECT_PARM *args, s32 mode);
void btEffectRunSleepStar(BATTLE_EFFECT_PARM *args);
void btEffectRunSoothingStar(BATTLE_EFFECT_PARM *args);
void btEffectRunMoteFountain(BATTLE_EFFECT_PARM *args, s32 mode);

/* battle_effect_target_strike.c */
void btEffectRunNumbingSting(BATTLE_EFFECT_PARM *args);
void btEffectRunCannibalFang(BATTLE_EFFECT_PARM *args);
void btEffectRunPoisonousBite(BATTLE_EFFECT_PARM *args);
void btEffectRunRabidFang(BATTLE_EFFECT_PARM *args);
void btEffectRunElectricBite(BATTLE_EFFECT_PARM *args);
void btEffectRunPoisonTail(BATTLE_EFFECT_PARM *args);
void btEffectRunVampiricFang(BATTLE_EFFECT_PARM *args);
void btEffectRunDoubleFang(BATTLE_EFFECT_PARM *args);
void btEffectRunMortalBlow(BATTLE_EFFECT_PARM *args);
void btEffectRunTwinBeaks(BATTLE_EFFECT_PARM *args);
void btEffectRunAcidBite(BATTLE_EFFECT_PARM *args);
void btEffectRunDrainFang(BATTLE_EFFECT_PARM *args);
void btEffectRunTargetStrike(BATTLE_EFFECT_PARM *args, s32 mode);

/* battle_effect_death_plunge.c */
void btEffectRunDeathPlunge(BATTLE_EFFECT_PARM *args);
void btEffectRunBladeRain(BATTLE_EFFECT_PARM *args);
void btEffectDrawStreaks(void);

/* battle_effect_spider_web.c */
void btEffectRunPoisonBeat(BATTLE_EFFECT_PARM *args);
void btEffectRunSpinningBeat(BATTLE_EFFECT_PARM *args);
void btEffectRunSwirlSwipe(BATTLE_EFFECT_PARM *args, s32 mode);
void btEffectRunSpiderWeb(BATTLE_EFFECT_PARM *args);

/* battle_effect_canvas.c */
void btEffectWipeCanvas(s32 direction, s32 fill);
void btEffectTransferCanvas(void);
void btEffectTransferFullScrImage(void);
void btEffectApplyWindowBlendRegisters(void);
void btEffectApplyBg2Reference(void);
void btEffectDarkenBackgroundStep(void);
void btEffectClearTintTimers(void);
void btEffectCountDownBattlerTints(void);
void btEffectStartCanvasLayer(s32 bgControl);
void btEffectOpenCanvasLayer(s32 bgControl);
void btEffectStartTiledCanvas(s32 bgControl);
void btEffectEndCanvasLayer(void);
void btEffectHideBg1AndRestoreBackdrop(void);
void btEffectSetupCanvasTileMap(void);
void btEffectDrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);

/* battle_effect_planet_diver.c */
void btEffectRunPlanetDiver(BATTLE_EFFECT_PARM *args);
void btEffectRunHaunt(BATTLE_EFFECT_PARM *args);
void btEffectRunUnusedOrbitingSpirals(BATTLE_EFFECT_PARM *args);

/* battle_effect_gathering_lights.c */
void btEffectRunDemonNight(BATTLE_EFFECT_PARM *args);
void btEffectRunRumble(BATTLE_EFFECT_PARM *args);
void btEffectRunHaunting(BATTLE_EFFECT_PARM *args);
void btEffectRunGatheringLights(BATTLE_EFFECT_PARM *args, s32 mode);

/* battle_effect_mirrored_wall.c */
void btEffectBuildCanvasBlitters(s32 flip, BLITTER_PAIR out);
void btEffectRunBerserk(BATTLE_EFFECT_PARM *args);
void btEffectRunSpasm(BATTLE_EFFECT_PARM *args);
void btEffectRunRecovery(BATTLE_EFFECT_PARM *args);
void btEffectRunMirroredWall(BATTLE_EFFECT_PARM *args, s32 mode);

/* battle_effect_djinn_beams.c */
void btEffectRunRevive(BATTLE_EFFECT_PARM *args);
void btEffectRunDew(BATTLE_EFFECT_PARM *args);
void btEffectRunDjinnBeams(BATTLE_EFFECT_PARM *args, s32 element);

/* battle_effect_lobbed_shells.c */
void btEffectRunMadSpatter(BATTLE_EFFECT_PARM *args);
void btEffectRunDecompose(BATTLE_EFFECT_PARM *args);
void btEffectRunBoneChiller(BATTLE_EFFECT_PARM *args);
void btEffectRunStickyGoo(BATTLE_EFFECT_PARM *args);
void btEffectRunPoisonInk(BATTLE_EFFECT_PARM *args);
void btEffectRunStickyPoison(BATTLE_EFFECT_PARM *args);
void btEffectRunRottenBlood(BATTLE_EFFECT_PARM *args);
void btEffectRunLobbedShells(BATTLE_EFFECT_PARM *args, s32 mode);

/* battle_effect_condemn.c */
void btEffectRunCondemn(BATTLE_EFFECT_PARM *args);

/* battle_effect_bind.c */
void btEffectConvertBackgroundToBlueRamp(void);
void btEffectRunBind(BATTLE_EFFECT_PARM *args);
void btEffectRunUnusedTriangleStrike(BATTLE_EFFECT_PARM *args);

/* battle_effect_stars.c */
void btEffectRunAstralBlast(BATTLE_EFFECT_PARM *args);
void btEffectRunShiningStar(BATTLE_EFFECT_PARM *args);

/* battle_effect_procne.c */
void btEffectRunProcne(BATTLE_EFFECT_PARM *args);

/* battle_effect_tiamat.c */
void btEffectRunDragonDriver(BATTLE_EFFECT_PARM *args);
void btEffectRunTiamat(BATTLE_EFFECT_PARM *args);
void btEffectRunDragonFire(BATTLE_EFFECT_PARM *args, s32 mode);

/* battle_effect_nereid.c */
void btEffectRunNereid(BATTLE_EFFECT_PARM *args);

/* battle_effect_vortices.c */
void btEffectRunFroth(BATTLE_EFFECT_PARM *args);
void btEffectRunWhirlwind(BATTLE_EFFECT_PARM *args);

/* battle_effect_prism.c */
void btEffectRunPrism(BATTLE_EFFECT_PARM *args);
void btEffectUpdatePhasePalette(s32 frame, s32 redPhase, s32 greenPhase,
								s32 bluePhase);

/* battle_effect_plasma.c */
void btEffectRunPlasma(BATTLE_EFFECT_PARM *args);
void btEffectRunNova(BATTLE_EFFECT_PARM *args);
void btEffectRunDynamite(BATTLE_EFFECT_PARM *args);
void btEffectRunSparkGroups(BATTLE_EFFECT_PARM *args, s32 mode);

/* battle_effect_volcano.c */
void btEffectRunVolcano(BATTLE_EFFECT_PARM *args);

/* battle_effect_target_motes.c */
void btEffectRunTaint(BATTLE_EFFECT_PARM *args);
void btEffectRunUnusedPinkSmoke(BATTLE_EFFECT_PARM *args);
void btEffectRunDelude(BATTLE_EFFECT_PARM *args);
void btEffectRunParalyze(BATTLE_EFFECT_PARM *args);
void btEffectRunHeartrender(BATTLE_EFFECT_PARM *args);
void btEffectRunMysticFlame(BATTLE_EFFECT_PARM *args);
void btEffectRunDemonFire(BATTLE_EFFECT_PARM *args);
void btEffectRunDrown(BATTLE_EFFECT_PARM *args);
void btEffectRunTargetMotes(BATTLE_EFFECT_PARM *args, u32 mode);
void btEffectRunSleep(BATTLE_EFFECT_PARM *args);
void btEffectRunCurse(BATTLE_EFFECT_PARM *args);

/* battle_effect_djinn_heal.c */
void btEffectRunDjinnHeal(BATTLE_EFFECT_PARM *args);
void btCameraApplyPhasedDelta(void);

/* battle_effect_dispatch.c */
void btEffectCallDoNothing(s32 count);
void btEffectBuildWindowEdgeTable(void);
void btEffectShowLivingTargets(BATTLE_EFFECT_PARM *args);
void btEffectSetupBlendedDisp(void);
/* Passed to btEffectTintBattler as its palette, animation or timerSlot: */
/* leave that one as it is. */
#define TINT_KEEP (-1)
void btEffectTintBattler(s32 actorId, s32 palette, s32 animation, s32 timerSlot,
						 s32 timerFrames);
void btEffectOpenAndCloseTiledCanvas(void);

/* battle_effect_cybele.c */
void btEffectRunCybele(BATTLE_EFFECT_PARM *args);

/* battle_effect_boreas.c */
void btEffectRunBoreas(BATTLE_EFFECT_PARM *args);

/* battle_effect_drain.c */
void btEffectRunBreak(BATTLE_EFFECT_PARM *args);
void btEffectRunDrain(BATTLE_EFFECT_PARM *args);

/* battle_effect_healing.c */
void btEffectRunPly(BATTLE_EFFECT_PARM *args);
void btEffectRunWish(BATTLE_EFFECT_PARM *args);
void btEffectRunCure(BATTLE_EFFECT_PARM *args);
void btEffectRunCurePoison(BATTLE_EFFECT_PARM *args);
void btEffectRunUnusedRestoreMotes(BATTLE_EFFECT_PARM *args);
void btEffectRunTonic(BATTLE_EFFECT_PARM *args);
void btEffectRunPsyCrystal(BATTLE_EFFECT_PARM *args);
void btEffectRunHealingMotes(BATTLE_EFFECT_PARM *args, u32 mode);

/* battle_effect_stat_change.c */
void btEffectRunImpact(BATTLE_EFFECT_PARM *args);
void btEffectRunGuard(BATTLE_EFFECT_PARM *args);
void btEffectRunWard(BATTLE_EFFECT_PARM *args);
void btEffectRunGranite(BATTLE_EFFECT_PARM *args);
void btEffectRunFlash(BATTLE_EFFECT_PARM *args);
void btEffectRunZephyr(BATTLE_EFFECT_PARM *args);
void btEffectRunStatBoost(BATTLE_EFFECT_PARM *args, u32 mode);
void btEffectRunDull(BATTLE_EFFECT_PARM *args);
void btEffectRunImpair(BATTLE_EFFECT_PARM *args);
void btEffectRunWeaken(BATTLE_EFFECT_PARM *args);
void btEffectRunUnusedStatReduction(BATTLE_EFFECT_PARM *args);
void btEffectRunStatReduction(BATTLE_EFFECT_PARM *args, s32 mode);

/* battle_effect_flare.c */
void btEffectRunFlare(BATTLE_EFFECT_PARM *args);
s32 btEffectBuildCenteredIndexList(BATTLE_EFFECT_INDEX_WINDOW *window,
								   s16 *output);

/* battle_effect_quake.c */
void btEffectRunQuake(BATTLE_EFFECT_PARM *args);

/* battle_effect_fire.c */
void btEffectRunFire(BATTLE_EFFECT_PARM *args);

/* battle_effect_frost.c */
void btEffectRunFrost(BATTLE_EFFECT_PARM *args);

/* battle_effect_ray.c */
void btEffectRunRay(BATTLE_EFFECT_PARM *args);

/* battle_effect_eruption.c */
void btEffectRunSmokeBomb(BATTLE_EFFECT_PARM *args);
void btEffectRunBlast(BATTLE_EFFECT_PARM *args);
void btEffectRunEruption(BATTLE_EFFECT_PARM *args, s32 mode);
void btEffectCreateSprs(s32 count, s32 spriteId, u32 priority);
void btEffectDoNothing(void);

/* battle_effect_burst_shower.c */
void btEffectArmBg2AffineHBlankDma(void);
void btEffectRunFireBlessing(BATTLE_EFFECT_PARM *args);
void btEffectRunWaterBlessing(BATTLE_EFFECT_PARM *args);
void btEffectRunDarkBlessing(BATTLE_EFFECT_PARM *args);
void btEffectRunAcidBlessing(BATTLE_EFFECT_PARM *args);
void btEffectRunStormBlessing(BATTLE_EFFECT_PARM *args);
void btEffectRunEvilBlessing(BATTLE_EFFECT_PARM *args);
void btEffectRunDeadlyGas(BATTLE_EFFECT_PARM *args);
void btEffectRunBurstShower(BATTLE_EFFECT_PARM *args, s32 mode);

/* battle_effect_orbiting.c */
void btEffectRunSwirlingStars(BATTLE_EFFECT_PARM *args);
void btEffectRunOrbitingSkulls(void);
void btEffectRunTargetBursts(BATTLE_EFFECT_PARM *args);

/* battle_effect_atalanta.c */
void btEffectRunAtalanta(BATTLE_EFFECT_PARM *args);

/* battle_effect_growth.c */
void btEffectRunGrowth(BATTLE_EFFECT_PARM *args);
void btEffectRunPunji(BATTLE_EFFECT_PARM *args);
void btEffectRunGrowingColumns(BATTLE_EFFECT_PARM *args, s32 mode);
void btEffectRunVine(BATTLE_EFFECT_PARM *args);
void btEffectRunThorn(BATTLE_EFFECT_PARM *args);

/* battle_effect_bolt.c */
void btEffectRunBolt(BATTLE_EFFECT_PARM *args);

/* battle_effect_projectiles.c */
void btEffectRunDjinniEntrance(BATTLE_EFFECT_PARM *args, s32 element, s32 side,
							   s32 anchor, s32 *outX, s32 *outY);
void btEffectRunSlash(BATTLE_EFFECT_PARM *args);
void btEffectRunShuriken(BATTLE_EFFECT_PARM *args);
void btEffectRunCrazyVoice(BATTLE_EFFECT_PARM *args);
void btEffectRunWingFlutter(BATTLE_EFFECT_PARM *args);
void btEffectRunWingStroke(BATTLE_EFFECT_PARM *args);
void btEffectRunWingBeat(BATTLE_EFFECT_PARM *args);
void btEffectRunCuttingEdge(BATTLE_EFFECT_PARM *args);
void btEffectRunHeatWave(BATTLE_EFFECT_PARM *args);
void btEffectRunSleet(BATTLE_EFFECT_PARM *args);
void btEffectRunIceBlessing(BATTLE_EFFECT_PARM *args);
void btEffectRunBlizzard(BATTLE_EFFECT_PARM *args);
void btEffectRunProjectileVolley(BATTLE_EFFECT_PARM *args, s32 mode);
void btEffectHopBackToSpot(s32 id);
void btEffectSetApproachMotion(s32 id, s32 targetId, s32 frames);

/* battle_effect_rushing_strike.c */
void btEffectPackYellowOrbs(u8 *src, u8 *dst, s32 stride, s32 rowCount);
void btEffectRunBoneCharge(BATTLE_EFFECT_PARM *args);
void btEffectRunBruteForce(BATTLE_EFFECT_PARM *args);
void btEffectRunOnslaught(BATTLE_EFFECT_PARM *args);
void btEffectRunHeadbutt(BATTLE_EFFECT_PARM *args);
void btEffectRunRushingStrike(BATTLE_EFFECT_PARM *args, s32 mode);

/* battle_effect_djinn_attacks.c */
void btEffectTransposeCopy(const u8 *src, u8 *dst, s32 w, s32 h);
void btEffectRunMercury(BATTLE_EFFECT_PARM *args);
void btEffectRunJupiter(BATTLE_EFFECT_PARM *args);
void btResourceLoadAndDecompress(s32 id, void *dst, s32 skipPalette,
								 s32 copyPalette);
void btEffectRunVenus(BATTLE_EFFECT_PARM *args);
void btEffectRunMars(BATTLE_EFFECT_PARM *args);

/* battle_effect_hail.c */
void btEffectRunHail(BATTLE_EFFECT_PARM *args);
void btEffectRunGround(BATTLE_EFFECT_PARM *args);
void btCameraShake(s32 rangeX, u32 rangeY);

/* battle_effect_thor.c */
void btEffectRunThor(BATTLE_EFFECT_PARM *args);

/* battle_effect_spire.c */
void btEffectRunSpire(BATTLE_EFFECT_PARM *args);

/* battle_effect_flurry.c */
void btEffectRunGust(BATTLE_EFFECT_PARM *args);
void btEffectRunBearClaw(BATTLE_EFFECT_PARM *args);
void btEffectRunRansack(BATTLE_EFFECT_PARM *args);
void btEffectRunBacteriaRush(BATTLE_EFFECT_PARM *args);
void btEffectRunFreebiteRush(BATTLE_EFFECT_PARM *args);
void btEffectRunFlyingAttack(BATTLE_EFFECT_PARM *args);
void btEffectRunFlurry(BATTLE_EFFECT_PARM *args, s32 mode);

/* battle_effect_gaia.c */
void btEffectRunGaia(BATTLE_EFFECT_PARM *args);

/* battle_effect_positions.c */
void btParticleAdvanceWithGravity3D(PARTICLE *step, s32 damping, s32 gravity);
void btParticleAdvanceWithGravity2D(PARTICLE *step, s32 damping, s32 gravity);
s32 btProjectionPoint(const void *point, SCR_POINT *position);
void btProjectionSpr(s32 id, SCR_POINT *position);
void btProjectionScaledUnit(s32 id, SCR_POINT *position);
void btProjectionUnit(s32 id, SCR_POINT *position);
s32 btUnitProjectToScr(s32 unit, s32 *screen);
void btEffectReadKeysForEnemyTarget(void);

/* battle_effect_beams.c */
void btEffectRunAttackSlash(BATTLE_EFFECT_PARM *args);
void btEffectRunCriticalHit(BATTLE_EFFECT_PARM *args);

/* battle_effect_casting_impact.c */
void btEffectStepPaletteToResource(s32 resourceId);
void btEffectRunCastingImpact(BATTLE_EFFECT_PARM *args, s32 mode);

/* battle_effect_torch.c */
void btEffectRunTorch(BATTLE_EFFECT_PARM *args);
void btEffectRunKite(BATTLE_EFFECT_PARM *args);

/* battle_effect_helm_splitter.c */
void btEffectRunHelmSplitter(BATTLE_EFFECT_PARM *args);
void btEffectPlaceFormationSprs(s32 formation, FIXED x, FIXED z);
void btEffectRunImpactBurst(s32 formation, s32 x, FIXED y);

/* battle_effect_starfield_fall.c */
void btPaletteBrightenBgEntries(s32 blueStep, s32 greenStep, s32 redStep);
void btEffectArmPaletteHBlankDma(void);
void btEffectRunMeteor(BATTLE_EFFECT_PARM *args);
void btEffectRunOuterSpace(BATTLE_EFFECT_PARM *args);
void btEffectRunStarfieldFall(BATTLE_EFFECT_PARM *args, s32 mode);

/* battle_effect_ramses.c */
void btEffectRunRamses(BATTLE_EFFECT_PARM *args);

/* battle_effect_dragon_cloud.c */
void btEffectRunDragonCloud(BATTLE_EFFECT_PARM *args);

/* battle_effect_annihilation.c */
void btEffectRunAnnihilation(BATTLE_EFFECT_PARM *args);

/* battle_effect_ragnarok.c */
void btEffectRunRagnarok(BATTLE_EFFECT_PARM *args);

/* battle_effect_titan_blade.c */
void btEffectRunTitanBlade(BATTLE_EFFECT_PARM *args);

/* battle_effect_judgment.c */
void btEffectRunJudgment(BATTLE_EFFECT_PARM *args);

/* battle_effect_kirin.c */
void btEffectRunKirin(BATTLE_EFFECT_PARM *args);

/* battle_effect_neptune.c */
void btEffectResetBg2Pa(void);
void btEffectSetBg2AffineScaleHalf(void);
void btEffectRunNeptune(BATTLE_EFFECT_PARM *args);

/* battle_effect_thrash.c */
void btEffectRunThrash(BATTLE_EFFECT_PARM *args);
void btEffectRunUnusedSevereBlowLeap(BATTLE_EFFECT_PARM *args);
void btEffectRunQuickStrike(BATTLE_EFFECT_PARM *args);
void btEffectRunVorpalSlash(BATTLE_EFFECT_PARM *args);
void btEffectRunFlashingStrike(BATTLE_EFFECT_PARM *args, s32 mode);

/* battle_effect_undead_sword.c */
void btEffectRunUndeadSword(BATTLE_EFFECT_PARM *args);

/* The template btEffectBuildBlitter walks word by word to write a blitter */
/* (blitter_fragments.s); none of it runs where it is. */
extern const u32 BlitterTemplate[];

#endif
