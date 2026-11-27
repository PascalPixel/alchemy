#ifndef ALCHEMY_BATTLE_EFFECT_TABLE_H
#define ALCHEMY_BATTLE_EFFECT_TABLE_H

// The tables the battle effects read (battle_effect_table.c), in its order: picture offsets, sizes,
// timings and counts by variant. A table read by columns is a two-dimensional array, its columns
// named by the enum above it.

#include "types.h"
#include "graphics/render_placement.h"
#include "system.h"

// The two kinds of target strike (TargetStrikeStyles).
#define STRIKE_KIND_SLASH      0 // sideways slashes
// rows of fangs above and below that close on it
#define STRIKE_KIND_BITE       1

// A target strike style's extras. each hit raises smoke puffs from the target's feet
#define STRIKE_SMOKE           1
// motes fly out of the target and back into the user
#define STRIKE_ABSORB          2
#define STRIKE_GLINTS          4 // glints sparkle around the target
#define STRIKE_SPARKS          8 // each hit scatters sparks
// one heavy hit: a big shake and the target recoils
#define STRIKE_HEAVY           16

// The palettes a target strike's background takes (TARGET_STRIKE_COLOR).
#define STRIKE_COLOR_BLUE      0 // BlueBeamSheet's
#define STRIKE_COLOR_LIME      1 // LimePalette
#define STRIKE_COLOR_PINK      2 // PinkPalette
#define STRIKE_COLOR_EMBER     3 // EmberStreakSheet's
#define STRIKE_COLOR_MARS      4 // MarsDjinnSheet's
#define STRIKE_COLOR_LIGHTNING 5 // LightningBoltSheet's

// The twelve target strike styles, rows of TargetStrikeStyles.
typedef enum TARGET_STRIKE_STYLE
{
	TARGET_STRIKE_STYLE_BLUE_SPARK_BITES,
	TARGET_STRIKE_STYLE_LIME_SMOKE_BITE,
	TARGET_STRIKE_STYLE_PINK_SMOKE_BITES,
	TARGET_STRIKE_STYLE_EMBER_DRAIN_BITES,
	TARGET_STRIKE_STYLE_MARS_DRAIN_BITES,
	TARGET_STRIKE_STYLE_LIGHTNING_GLINT_BITES,
	TARGET_STRIKE_STYLE_LIGHTNING_GLINT_SLASH,
	TARGET_STRIKE_STYLE_LIGHTNING_SPARK_SLASHES,
	TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH,
	TARGET_STRIKE_STYLE_LIME_SMOKE_SLASH,
	TARGET_STRIKE_STYLE_MARS_SMOKE_BITE,
	TARGET_STRIKE_STYLE_MARS_ADDITIVE_DRAIN_BITES
} TARGET_STRIKE_STYLE;

// Where each picture begins in the sprite sheets many effects share: the ten motes and twenty
// flares of a glow sheet, the nine sparkles, the six flame puffs (size, drop and offset) and the
// six glints (offset and size).
extern Uint16 BattleEffectMotePictures[];
extern Uint16 BattleEffectFlarePictures[];
extern Uint16 BattleEffectSparklePictures[];
extern Uint8  BattleEffectSparkleSizes[];
extern Uint8  BattleEffectPuffArcPictureWidths[];
extern Uint8  BattleEffectPuffArcPictureHeights[];
extern Uint8  BattleEffectPuffArcPictureBiasY[];
extern Uint16 BattleEffectPuffArcPictures[];
extern Uint16 BattleEffectGlintPictures[];
extern Uint8  BattleEffectGlintPictureWidths[];
extern Uint8  BattleEffectGlintPictureHeights[];

// Each Ice variant: its shards and how many ticks it runs.
enum
{
	ICE_SHARD_COUNT, // shards that fall
	ICE_TICKS        // the effect's ticks; the damage shows 16 before
};

extern Uint8 IceCounts[][2];

// Each Douse variant: its drops, the droplets each splash throws, the spacing and the ticks.
enum
{
	DOUSE_DROP_COUNT,        // drops that fall, of the 64 placed
	DOUSE_DROPLETS_PER_DROP, // droplets each landing drop throws up
	DOUSE_DROP_SPACING,      // pixels between drops along the slant
	DOUSE_TICKS              // the effect's ticks; the damage shows 64 before
};

extern Uint8  DouseCounts[][4];

// Each streak style's effect, hit, motion and shake, and the pixels a struck target trembles by,
// tick by tick.
extern Sint8  StreakStyles[][4];
extern Sint8  StreakTremble[];
// The target strike: its twelve styles by the TARGET_STRIKE_ columns, the six slash pictures (size,
// offset and reach), the fang stage of each picture of a bite, and the flips a glint may take.
extern Uint8  TargetStrikeStyles[][7];
extern Uint8  TargetStrikeWidths[];
extern Uint8  TargetStrikeHeights[];
extern Uint16 TargetStrikePictures[];
extern Uint8  TargetStrikeReach[];
extern Uint8  TargetStrikeBiteStages[];
extern Uint8  TargetStrikeGlintDrawFlags[];
// The blade rain's four sword pictures, by how steeply the sword falls.
extern Uint8  BladeRainSwordWidths[];
extern Uint8  BladeRainSwordHeights[];
extern Uint16 BladeRainSwordPictures[];
// The 33 pieces the shattering opening's picture breaks into: size, offset in the picture and where
// each starts on the canvas.
extern Uint8  ShatteringOpeningPieceWidths[];
extern Uint8  ShatteringOpeningPieceHeights[];
extern Uint16 ShatteringOpeningPiecePictures[];
extern Uint8  ShatteringOpeningPieceStartX[];
extern Uint8  ShatteringOpeningPieceStartY[];
// The element burst's four corner sparks: offsets and flips.
extern Sint8  ElementBurstSparkX[];
extern Sint8  ElementBurstSparkY[];
extern Uint8  ElementBurstSparkFlips[];
// The swirl swipe's six pictures (Spider Web): size, offset, an x for each side and a y offset.
extern Uint8  SwirlSwipePictureWidths[];
extern Uint8  SwirlSwipePictureHeights[];
extern Uint16 SwirlSwipePictures[];
extern Uint8  SwirlSwipePictureX[];
extern Sint8  SwirlSwipePictureBiasY[];

// Each gathering-lights mode: the lights per target and the ticks before the targets' turns.
enum
{
	GATHERING_LIGHTS_COUNT, // lights for each target
	GATHERING_LIGHTS_TICKS  // ticks before the targets' turns
};

// The gathering lights' modes (btEffectRunGatheringLights), each named for the effect that plays
// it, and its row of GatheringLightsCounts.
typedef enum GATHERING_LIGHTS_MODE
{
	GATHERING_LIGHTS_DEMON_NIGHT, // pink
	GATHERING_LIGHTS_RUMBLE,      // Mars Djinn colours
	GATHERING_LIGHTS_HAUNTING     // Mercury Djinn colours
} GATHERING_LIGHTS_MODE;

extern Uint8  GatheringLightsCounts[][2];

// The earth wall's three pictures, as the mirrored wall reads them.
extern Uint16 MirroredWallPictures[];
extern Uint8  MirroredWallPictureWidths[];
extern Uint8  MirroredWallPictureHeights[];
// Revive's beams: the mote picture of each step of its eight-step cycle, and the three pictures of
// the glow at a target's feet.
extern Uint8  DjinnBeamsMotePictures[];
extern Uint16 DjinnBeamsHeadPictures[];
extern Uint8  DjinnBeamsHeadWidths[];
extern Uint8  DjinnBeamsHeadHeights[];

// Each lobbed shells mode's style (LOBBED_SHELLS_MODE, battle_effect_lobbed_shells.c).
enum
{
	// nonzero: the shells burst into shards, not smoke
	LOBBED_SHELLS_STYLE_SHATTER,
	LOBBED_SHELLS_STYLE_UNUSED // 0 in every row; nothing reads it
};

extern Sint8   LobbedShellsStyles[][2];
// The eighteen pictures of the lobbed shells' tumbling shards, a large set of nine and a small one.
extern Uint8   LobbedShellsShardWidths[];
extern Uint8   LobbedShellsShardHeights[];
extern Uint16  LobbedShellsShardPictures[];
// Where Condemn draws the reaper's seven pictures: an x for each casting side, and a y.
extern Uint8   CondemnPictureX[][7];
extern Uint8   CondemnPictureY[][7];
// The first vertex of Bind's spinning and striking triangles, 24 pixels up from the centre.
extern VECTOR3 BindVertex[];
extern VECTOR3 BindTriangleStrikeVertex[];
// The stars' two points, 48 and 28 pixels up, and their radii.
extern VECTOR3 StarPoints[];
extern Uint8   StarRadii[];
// Procne's nine sprites on a 3x3 grid, the ticks of its four strikes, and its three comet pictures
// (size, lift and offset).
extern Uint8   ProcneGridX[];
extern Uint8   ProcneGridY[];
extern Uint16  ProcneTicks[];
extern Uint8   ProcnePictureWidths[];
extern Uint8   ProcnePictureHeights[];
extern Uint8   ProcnePictureBiasY[];
extern Uint16  ProcnePictures[];
// Tiamat's embers (speed and size by ember index & 3), its sparks' 1x2 picture, and the six flame
// pictures (size, drop and offset).
extern Uint8   TiamatEmberSpeeds[];
extern Uint8   TiamatSparkPixels[];
extern Uint8   TiamatEmberSizes[];
extern Uint8   TiamatFlameWidths[];
extern Uint8   TiamatFlameHeights[];
extern Uint8   TiamatFlameDrops[];
extern Uint16  TiamatFlamePictures[];

// The foot of each of Nereid's columns of fire.
enum
{
	NEREID_COLUMN_X, // canvas x of the column's middle
	NEREID_COLUMN_Y  // canvas y of its foot
};

extern Uint8  NereidColumns[][2];

// A falling drop's gravity in 16.16 pixels a tick each tick, by drop index & 3.
extern Sint32 NereidGravity[];

// Each Froth variant: its motes and ticks.
enum
{
	FROTH_BUBBLE_COUNT, // bubbles, at most 30 (battle_effect_vortices.c)
	FROTH_TICKS         // the effect's ticks; the damage shows 16 before
};

extern Uint8 VorticesCounts[][2];

// Each Whirlwind variant: its tornadoes, their sway and the screen shake.
enum
{
	WHIRLWIND_TORNADOES, // tornadoes drawn
	WHIRLWIND_SWAY,      // how far each sways across, in pixels
	WHIRLWIND_SHAKE      // btCameraShake's range
};

extern Uint8 VorticesShapes[][3];

// Where each Prism shard starts in the crystal.
enum
{
	PRISM_SHARD_START_X, // pixels from the crystal's left
	PRISM_SHARD_START_Y  // pixels from its top
};

extern Uint8 PrismShardStarts[][2];

// Each Prism variant: its crystals and ticks.
enum
{
	PRISM_CRYSTAL_COUNT, // crystals drawn
	PRISM_TICKS          // the effect's ticks
};

extern Uint8  PrismCounts[][2];

// The twelve pictures of Prism's shards.
extern Uint8  PrismShardWidths[];
extern Uint8  PrismShardHeights[];
extern Sint32 PrismShardPictures[];

// Each Plasma variant: the sparks per pillar and the ticks of shake.
enum
{
	PLASMA_SPARKS,     // sparks each lightning pillar throws
	PLASMA_SHAKE_TICKS // ticks the screen shakes at each strike
};

extern Uint16 PlasmaSparks[][2];

// Each Plasma pillar's canvas x, seven for each side, and how many strike by variant.
extern Uint8  PlasmaColumns[][7];
extern Uint8  PlasmaCounts[];

// Each spark group variant: sparks per burst, bursts, and from SPARK_GROUPS_OFFSET each burst's
// offset across.
enum
{
	SPARK_GROUPS_SPARKS, // sparks each burst throws
	SPARK_GROUPS_BURSTS, // bursts, 8 ticks apart
	SPARK_GROUPS_OFFSET  // burst i's offset across, in pixels, at [2 + i]
};

// The spark groups' variants past the three strengths of Blast and Dynamite (EFFECT_VARIANT), which
// their callers set before the run.
enum
{
	SPARK_GROUPS_SEVERE_BLOW = 3, // 340 sparks and a flash: Severe Blow's
	// impact and btEffectRunUnusedSevereBlowLeap
	SPARK_GROUPS_THRASH // 32 sparks: Thrash
};

extern Uint16 SparkGroupsShapes[][5];

// The flash picture for each three ticks at the end of a flash's life.
extern Uint8  PlasmaFlashPictures[];
// Volcano's fire pillars by variant: how many stand, the depth of each and the tick each starts.
extern Uint8  VolcanoCounts[];
extern Sint8  VolcanoDepths[][4];
extern Uint8  VolcanoStartTicks[];
// The flips a glint around a target may take.
extern Uint8  TargetMotesGlintFlips[];

// Cybele's face as she spits the thorns: the animations of sprites 3 and 4, the left and right
// halves of the middle row of SUMMON_FORMATION_CYBELE, as the cast's recording shows them. Her
// mouth, in the left half, opens and shuts; her far eye, in the right, stays as it is.
enum
{
	CYBELE_POSE_FACE_LEFT, // sprite 3's animation: the half with her mouth
	CYBELE_POSE_FACE_RIGHT // sprite 4's animation
};

extern Uint8  CybeleObjectPoses[][2];

// Cybele's pictures, each by offset and size: the five thorns, the three flowers, the five chips
// (the thorns again), the five of a spark and the seven of the blast sheet, of which she draws only
// the last.
extern Uint16 CybeleThornPictures[];
extern Uint8  CybeleThornWidths[];
extern Uint8  CybeleThornHeights[];
extern Uint16 CybeleFlowerPictures[];
extern Uint8  CybeleFlowerSizes[];
extern Uint16 CybeleChipPictures[];
extern Uint8  CybeleChipWidths[];
extern Uint8  CybeleChipHeights[];
extern Uint16 CybeleSparkPictures[];
extern Uint8  CybeleSparkSizes[];
extern Uint16 CybeleBlastPictures[];
extern Uint16 CybeleBlastSizes[];
// Boreas: where the crystals stand, the background's jolt as the ice block lands, its third
// sprite's animations as the block sinks, and the pictures of the crystals, the shards thrown as
// they crumble, the spray, the crystals that rise among the targets and those that fall.
extern Uint8  BoreasPoints[][2];
extern Sint8  BoreasShakeOffsets[];
extern Uint16 BoreasCrystalPictures[];
extern Uint8  BoreasCrystalWidths[];
extern Uint8  BoreasCrystalHeights[];
extern Uint8  BoreasAnimations[];
extern Uint8  BoreasDebrisWidths[];
extern Uint8  BoreasDebrisHeights[];
extern Uint16 BoreasDebrisPictures[];
extern Uint8  BoreasSprayWidths[];
extern Uint8  BoreasSprayHeights[];
extern Uint16 BoreasSprayPictures[];
extern Uint16 BoreasRisingCrystalPictures[];
extern Uint8  BoreasRisingCrystalWidths[];
extern Uint8  BoreasRisingCrystalHeights[];
extern Uint8  BoreasFallWidths[];
extern Uint8  BoreasFallHeights[];
extern Uint16 BoreasFallPictures[];
// The gravity of Drain's ice shards, by shard index & 3.
extern Sint32 DrainGravities[];
// The twelve pictures of the stat changes' shards: four shapes, three ticks each.
extern Uint16 StatChangeShardPictures[];
extern Uint8  StatChangeShardWidths[];
extern Uint8  StatChangeShardHeights[];
// Each Quake variant: the effect's ticks, each pillar's and the shake.
extern Uint8  QuakeTimings[][3];
// Fire's embers by variant, and the six pictures of its impact flare.
extern Uint8  FireCounts[];
extern Uint8  FireFlareWidths[];
extern Uint8  FireFlareHeights[];
extern Uint8  FireFlareBiasY[];
extern Uint16 FireFlarePictures[];

// Where each ice mound of Frost stands: x and the y of its foot.
enum
{
	FROST_MOUND_X,   // canvas x of the mound's left edge
	FROST_MOUND_FOOT // canvas y of its foot
};

extern Uint8     FrostMoundSpots[][2];

// Frost: its motes, mounds and burst tick by variant, and the pictures of its motes, its three ice
// mounds and its chips (the motes again).
extern Uint8     FrostTimings[][3];
extern Uint8     FrostMoteWidths[];
extern Uint8     FrostMoteHeights[];
extern Uint16    FrostMotePictures[];
extern Uint16    FrostMoundPictures[];
extern Uint8     FrostMoundHeights[];
extern Uint8     FrostChipWidths[];
extern Uint8     FrostChipHeights[];
extern Uint16    FrostChipPictures[];
// Ray's sparks by variant; [1] is how many each bolt throws a tick.
extern Uint8     RaySparks[][2];
// Eruption's motes, puffs and ticks by variant, and its seven puff pictures.
extern Uint8     EruptionCounts[][3];
extern Uint16    EruptionPuffPictures[];
extern Uint16    EruptionPuffSizes[];
// Unused: picture layouts nothing reads, from 6x6 to 20x20.
extern Uint16    UnusedSheetPicturesShort[];
extern Uint16    UnusedSheetPictures[];
extern Uint8     UnusedSheetWidths[];
extern Uint8     UnusedSheetHeights[];
// The scale Atalanta's sprites keep throughout, one to one.
extern SPR_SCALE AtalantaUnitScale;
// Growth's columns: the three shapes' vine and pillar pictures and height caps, the columns by
// variant, their roots and the pillars' heights, and the seven pictures of a puff of dust.
extern Uint8     GrowthVineWidths[];
extern Uint8     GrowthHeightLimits[];
extern Uint16    GrowthVinePictures[];
extern Uint8     GrowthPillarWidths[];
extern Uint16    GrowthPillarPictures[];
extern Uint8     GrowthColumnCounts[];
extern Uint8     GrowthColumnX[];
extern Sint8     GrowthPillarHeights[];
extern Sint8     GrowthPuffWidths[];
extern Uint8     GrowthPuffHeights[];
extern Uint16    GrowthPuffPictures[];
// Each of Vine's roots, across from the middle of the targets.
extern Sint8     VineColumnOffsets[];
// A thorn's kind in ThornKinds.
#define THORN_MIRRORED 1 // drawn mirrored across
#define THORN_SMALL    2 // 32 pixels wide, not 48
// Thorn: each thorn's kind and root, the thorns by variant, and the seven pictures of a puff of
// dust.
extern Uint8  ThornKinds[];
extern Sint8  ThornX[];
extern Uint8  ThornCounts[];
extern Sint8  ThornPuffWidths[];
extern Uint8  ThornPuffHeights[];
extern Uint16 ThornPuffPictures[];
// Bolt by variant (sparks, glints, their spread and bolts), the flips a glint may take, and the
// glints' blend mode by variant.
extern Uint8  BoltCounts[][4];
extern Uint8  BoltGlintFlips[];
extern Uint8  BoltGlintModes[];

// The volley's three comet sheets: Slash, Wind Slash and Sonic Slash fire the one their variant
// picks.
typedef enum PROJECTILE_VOLLEY_COMET
{
	PROJECTILE_VOLLEY_COMET_A, // SlashCometSheet, Slash's
	PROJECTILE_VOLLEY_COMET_B, // WindSlashCometSheet, Wind Slash's
	// SonicSlashCometSheet, Sonic Slash's and Cutting Edge's
	PROJECTILE_VOLLEY_COMET_C
} PROJECTILE_VOLLEY_COMET;

// Each comet's picture height.
extern Uint8 ProjectileVolleyCometHeights[];

// The projectile volley's modes (btEffectRunProjectileVolley), each named for the effect that fires
// it; the casting impacts fire two of them.
typedef enum PROJECTILE_VOLLEY_MODE
{
	PROJECTILE_VOLLEY_SLASH,        // comets, picked by the variant
	PROJECTILE_VOLLEY_SHURIKEN,     // spinning shurikens
	PROJECTILE_VOLLEY_CRAZY_VOICE,  // bouncing pellets
	PROJECTILE_VOLLEY_WING_FLUTTER, // yellow sparks
	PROJECTILE_VOLLEY_WING_BEAT,    // small yellow sparks
	PROJECTILE_VOLLEY_CUTTING_EDGE, // one large comet in ice colours
	PROJECTILE_VOLLEY_HEAT_WAVE,    // fireballs trailing embers
	PROJECTILE_VOLLEY_SLEET,        // ice chips falling from high above
	// comets, after CASTING_IMPACT_UNUSED_COMETS
	PROJECTILE_VOLLEY_IMPACT_COMETS,
	// scattered ice chips; also after CASTING_IMPACT_UNUSED_ICE_CHIPS
	PROJECTILE_VOLLEY_BLIZZARD,
	PROJECTILE_VOLLEY_ICE_BLESSING, // ice chips streaming one a tick
	PROJECTILE_VOLLEY_WING_STROKE   // large yellow sparks, spread up and down
} PROJECTILE_VOLLEY_MODE;

// Each projectile volley mode by its columns, the seven pictures of the hit flash (size, step, rise
// and offset), and the four ice chip pictures.
extern Uint8  ProjectileVolleyModes[][5];
extern Uint8  ProjectileVolleyFlashWidths[];
extern Uint8  ProjectileVolleyFlashHeights[];
extern Uint8  ProjectileVolleyFlashSteps[];
extern Uint8  ProjectileVolleyFlashRises[];
extern Uint16 ProjectileVolleyFlashPictures[];
extern Uint16 ProjectileVolleyPelletPictures[];
// The ticks on which the Mercury Djinni fires each of its five flames.
extern Uint8  DjinnAttacksLaunchTicks[];
// Hail's four 8x8 ice chip pictures.
extern Uint8  HailPictureWidths[];
extern Uint8  HailPictureHeights[];
extern Uint16 HailPictures[];
// Ground: the flips a crackle may take, and the portal's nine pictures (the portal, its white core
// and its crackles) by size, offset and place.
extern Uint8  GroundCrackleFlips[];
extern Uint8  GroundPortalWidths[];
extern Uint8  GroundPortalHeights[];
extern Uint16 GroundPortalPictures[];
extern Uint8  GroundPortalX[];
extern Uint8  GroundPortalY[];
// The colours a target flashes when Thor's bolt strikes it, and the flips of the crackles around
// Thor.
extern Uint8  ThorTargetPalettes[];
extern Uint8  ThorCrackleFlips[];

// Where each Spire shard starts, before its rock's offset across.
enum
{
	SPIRE_SHARD_X, // canvas x
	SPIRE_SHARD_Y  // canvas y
};

extern Uint8  SpireShardOffsets[][2];

// Spire: each rock's offset across and the tick it appears, the rocks by variant, and its shards'
// fifteen pictures.
extern Sint8  SpireRockX[];
extern Uint8  SpireDropTicks[];
extern Uint8  SpireRockCounts[];
extern Uint8  SpireShardWidths[];
extern Uint8  SpireShardHeights[];
extern Uint16 SpireShardPictures[];

// The flurry's modes (btEffectRunFlurry), its rows of FlurryStyles: Gust and Flying Attack take
// one, two or three strikes as a double or triple blow raises their variant.
typedef enum FLURRY_MODE
{
	FLURRY_BEAR_CLAW,            // five strikes; also Ransack
	FLURRY_BACTERIA_RUSH,        // seven strikes, in lime
	FLURRY_FREEBITE_RUSH,        // nine strikes, in blue
	FLURRY_FLYING_ATTACK,        // one strike, in embers
	FLURRY_FLYING_ATTACK_DOUBLE, // two
	FLURRY_FLYING_ATTACK_TRIPLE, // three
	FLURRY_GUST,                 // one strike, each filling the canvas
	FLURRY_GUST_DOUBLE,          // two
	FLURRY_GUST_TRIPLE           // three
} FLURRY_MODE;

// A flurry's arcs, its FLURRY_ARC_SHEET.
typedef enum FLURRY_ARC
{
	// blue arc A, with a wave for the third and fourth of every four strikes
	FLURRY_ARCS_AND_WAVES,
	FLURRY_ARCS // blue arc B for every strike
} FLURRY_ARC;

// Whose palette a flurry takes, its FLURRY_PALETTE.
typedef enum FLURRY_PAL
{
	FLURRY_PALETTE_MARS_DJINNI, // the Mars Djinni sprite sheet's
	FLURRY_PALETTE_LIME,
	FLURRY_PALETTE_BLUE_ARC,
	FLURRY_PALETTE_EMBER        // the ember streak sprite sheet's
} FLURRY_PAL;

extern Uint8  FlurryStyles[][7];
// Gaia's rising wall: its nine rows, the three flames at its foot, BG2's shift by side and variant,
// and the columns of the late rocks.
extern Uint16 GaiaRowPictures[];
extern Uint8  GaiaRowWidths[];
extern Uint16 GaiaFlamePictures[];
extern Uint8  GaiaFlameWidths[];
extern Uint8  GaiaFlameHeights[];
extern Sint8  GaiaBg2Shifts[][3];
extern Uint8  GaiaLateRockColumns[];
// The six pictures of the growing beam, and how far each reaches short of the user.
extern Uint8  BeamPictureWidths[];
extern Uint8  BeamPictureHeights[];
extern Uint16 BeamPictures[];
extern Uint8  BeamPictureReaches[];
// The casting impacts: the flips a glint may take, where Mortal Danger draws the reaper's pictures,
// and the four ice chip pictures.
extern Uint8  CastingImpactGlintDrawFlags[];
extern Uint8  ReaperPictureX[][7];
extern Uint8  ReaperPictureY[];
extern Uint16 CastingImpactIceChipPictures[];
// The seven pictures of Helm Splitter's fire swirl, and where each sits from the user.
extern Uint8  HelmSplitterPictureWidths[];
extern Uint8  HelmSplitterPictureHeights[];
extern Uint16 HelmSplitterPictures[];
extern Sint8  HelmSplitterPictureBiasX[];
extern Sint8  HelmSplitterPictureBiasY[];
// Where the sprites of a summon's formation stand, in pixels, one axis at a time
// (btEffectPlaceFormationSprites).
extern Uint8  SummonFormationNereidX[];
extern Uint8  SummonFormationNereidZ[];
extern Uint8  SummonFormationCybeleX[];
extern Uint8  SummonFormationCybeleZ[];
extern Uint8  SummonFormationTiamatX[];
extern Uint8  SummonFormationTiamatZ[];
// The eight pictures of the impact flash.
extern Uint8  ImpactBurstPictureWidths[];
extern Uint8  ImpactBurstPictureHeights[];
extern Uint16 ImpactBurstPictures[];
// The starfield fall: the seven sparks that orbit the falling sprite, where the seven fireballs
// fall, and the blast sheet's seven flash pictures.
extern Sint16 StarfieldFallOrbitPoints[][3];
extern Sint16 StarfieldFallFireballPoints[][2];
extern Uint16 StarfieldFallFlashPictures[];
extern Uint16 StarfieldFallFlashSizes[];
// Where Ramses's sprites stand from the statue's corner, and the blast sheet's puff pictures.
extern Uint8  RamsesStatueColumns[];
extern Uint8  RamsesStatueRows[];
extern Uint16 RamsesPuffPictures[];
extern Uint16 RamsesPuffSizes[];
// Ragnarok's strike columns, three for each side, and the flash picture for each three ticks at the
// end of a flash's life.
extern Uint8  RagnarokStrikeColumns[][3];
extern Uint8  RagnarokFlashPictures[];
// Titan Blade's fading flash pictures and its dust's gravity.
extern Uint8  TitanBladeFlashPictures[];
extern Sint32 TitanBladeDustGravity[];
// Judgment: its burst's seven quarter pictures, and the six shards thrown from it.
extern Uint8  JudgmentBurstHeights[];
extern Uint16 JudgmentShieldSprites[];
extern Uint16 JudgmentShardPictures[];
extern Uint8  JudgmentShardWidths[];
extern Uint8  JudgmentShardHeights[];
// Where Kirin's nine sprites stand, in pixels from its corner.
extern Uint8  KirinColumns[];
extern Uint8  KirinRows[];
// Neptune's eight sprites, offsets in pixels scaled by 1.5 as placed, and its spray's gravity by
// particle index & 3.
extern Uint8  NeptuneObjectX[];
extern Uint8  NeptuneObjectY[];
extern Sint32 NeptuneGravity[];
// The seven square pictures of Undead Sword's puffs.
extern Uint16 UndeadSwordPuffPictures[];
extern Uint16 UndeadSwordPuffSizes[];

#endif
