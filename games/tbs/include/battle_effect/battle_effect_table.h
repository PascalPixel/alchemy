#ifndef ALCHEMY_BATTLE_EFFECT_TABLE_H
#define ALCHEMY_BATTLE_EFFECT_TABLE_H

/// @file
/// The tables the battle effects read (battle_effect_table.c), in its order:
/// picture offsets, sizes, timings and counts by variant. A table read by
/// columns is a two-dimensional array, its columns named by the enum above it.

#include "types.h"
#include "graphics/render_placement.h"
#include "system/vector3.h"

// The two kinds of target strike (gTargetStrike_Styles).
#define STRIKE_KIND_SLASH 0 ///< sideways slashes
#define STRIKE_KIND_BITE 1  ///< rows of fangs above and below that close on it

// A target strike style's extras.
#define STRIKE_SMOKE 1  ///< each hit raises smoke puffs from the target's feet
#define STRIKE_ABSORB 2 ///< motes fly out of the target and back into the user
#define STRIKE_GLINTS 4 ///< glints sparkle around the target
#define STRIKE_SPARKS 8 ///< each hit scatters sparks
#define STRIKE_HEAVY 16 ///< one heavy hit: a big shake and the target recoils

// The palettes a target strike's background takes (TARGET_STRIKE_COLOR).
#define STRIKE_COLOR_BLUE 0      ///< BlueBeamSheet's
#define STRIKE_COLOR_LIME 1      ///< LimePalette
#define STRIKE_COLOR_PINK 2      ///< PinkPalette
#define STRIKE_COLOR_EMBER 3     ///< EmberStreakSheet's
#define STRIKE_COLOR_MARS 4      ///< MarsDjinnSheet's
#define STRIKE_COLOR_LIGHTNING 5 ///< LightningBoltSheet's

/// The twelve target strike styles, rows of gTargetStrike_Styles.
enum TargetStrikeStyle {
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
};

/// Where each picture begins in the sprite sheets many effects share: the ten
/// motes and twenty flares of a glow sheet, the nine sparkles, the six flame
/// puffs (size, drop and offset) and the six glints (offset and size).
extern u16 gBattleEffect_MotePictures[];
extern u16 gBattleEffect_FlarePictures[];
extern u16 gBattleEffect_SparklePictures[];
extern u8 gBattleEffect_SparkleSizes[];
extern u8 gBattleEffect_PuffArcPictureWidths[];
extern u8 gBattleEffect_PuffArcPictureHeights[];
extern u8 gBattleEffect_PuffArcPictureBiasY[];
extern u16 gBattleEffect_PuffArcPictures[];
extern u16 gBattleEffect_GlintPictures[];
extern u8 gBattleEffect_GlintPictureWidths[];
extern u8 gBattleEffect_GlintPictureHeights[];

/// Each Ice variant: its shards and how many frames it runs.
enum {
  ICE_SHARD_COUNT, ///< shards that fall
  ICE_FRAMES       ///< the effect's frames; the damage shows 16 before
};
extern u8 gIce_Counts[][2];

/// Each Douse variant: its drops, the droplets each splash throws, the
/// spacing and the frames.
enum {
  DOUSE_DROP_COUNT,        ///< drops that fall, of the 64 placed
  DOUSE_DROPLETS_PER_DROP, ///< droplets each landing drop throws up
  DOUSE_DROP_SPACING,      ///< pixels between drops along the slant
  DOUSE_FRAMES             ///< the effect's frames; the damage shows 64 before
};
extern u8 gDouse_Counts[][4];

/// Each streak style's effect, hit, motion and shake, and the pixels a struck
/// target trembles by, frame by frame.
extern s8 gStreak_Styles[][4];
extern s8 gStreak_Tremble[];
/// The target strike: its twelve styles by the TARGET_STRIKE_ columns, the
/// six slash pictures (size, offset and reach), the fang stage of each picture
/// of a bite, and the flips a glint may take.
extern u8 gTargetStrike_Styles[][7];
extern u8 gTargetStrike_Widths[];
extern u8 gTargetStrike_Heights[];
extern u16 gTargetStrike_Pictures[];
extern u8 gTargetStrike_Reach[];
extern u8 gTargetStrike_BiteStages[];
extern u8 gTargetStrike_GlintDrawFlags[];
/// The blade rain's four sword pictures, by how steeply the sword falls.
extern u8 gBladeRain_SwordWidths[];
extern u8 gBladeRain_SwordHeights[];
extern u16 gBladeRain_SwordPictures[];
/// The 33 pieces the shattering opening's picture breaks into: size, offset in
/// the picture and where each starts on the canvas.
extern u8 gShatteringOpening_PieceWidths[];
extern u8 gShatteringOpening_PieceHeights[];
extern u16 gShatteringOpening_PiecePictures[];
extern u8 gShatteringOpening_PieceStartX[];
extern u8 gShatteringOpening_PieceStartY[];
/// The element burst's four corner sparks: offsets and flips.
extern s8 gElementBurst_SparkX[];
extern s8 gElementBurst_SparkY[];
extern u8 gElementBurst_SparkFlips[];
/// The swirl swipe's six pictures (Spider Web): size, offset, an x for each
/// side and a y offset.
extern u8 gSwirlSwipe_PictureWidths[];
extern u8 gSwirlSwipe_PictureHeights[];
extern u16 gSwirlSwipe_Pictures[];
extern u8 gSwirlSwipe_PictureX[];
extern s8 gSwirlSwipe_PictureBiasY[];

/// Each gathering-lights mode: the lights per target and the frames before the
/// targets' turns.
enum {
  GATHERING_LIGHTS_COUNT, ///< lights for each target
  GATHERING_LIGHTS_FRAMES ///< frames before the targets' turns
};
/// The gathering lights' modes (BattleEffect_RunGatheringLights), each named
/// for the effect that plays it, and its row of gGatheringLights_Counts.
enum GatheringLightsMode {
  GATHERING_LIGHTS_DEMON_NIGHT, ///< pink
  GATHERING_LIGHTS_RUMBLE,      ///< Mars Djinn colours
  GATHERING_LIGHTS_HAUNTING     ///< Mercury Djinn colours
};
extern u8 gGatheringLights_Counts[][2];

/// The earth wall's three pictures, as the mirrored wall reads them.
extern u16 gMirroredWall_Pictures[];
extern u8 gMirroredWall_PictureWidths[];
extern u8 gMirroredWall_PictureHeights[];
/// Revive's beams: the mote picture of each step of its eight-step cycle, and
/// the three pictures of the glow at a target's feet.
extern u8 gDjinnBeams_MotePictures[];
extern u16 gDjinnBeams_HeadPictures[];
extern u8 gDjinnBeams_HeadWidths[];
extern u8 gDjinnBeams_HeadHeights[];
/// Each lobbed shells mode's style (enum LobbedShellsMode,
/// battle_effect_lobbed_shells.c).
enum {
  LOBBED_SHELLS_STYLE_SHATTER, ///< nonzero: the shells burst into shards, not
                               ///< smoke
  LOBBED_SHELLS_STYLE_UNUSED   ///< 0 in every row; nothing reads it
};
extern s8 gLobbedShells_Styles[][2];
/// The eighteen pictures of the lobbed shells' tumbling shards, a large set of
/// nine and a small one.
extern u8 gLobbedShells_ShardWidths[];
extern u8 gLobbedShells_ShardHeights[];
extern u16 gLobbedShells_ShardPictures[];
/// Where Condemn draws the reaper's seven pictures: an x for each casting
/// side, and a y.
extern u8 gCondemn_PictureX[][7];
extern u8 gCondemn_PictureY[][7];
/// The first vertex of Bind's spinning and striking triangles, 24 pixels up
/// from the centre.
extern struct Vector3 gBind_Vertex[];
extern struct Vector3 gBind_TriangleStrikeVertex[];
/// The stars' two points, 48 and 28 pixels up, and their radii.
extern struct Vector3 gStar_Points[];
extern u8 gStar_Radii[];
/// Procne's nine sprites on a 3x3 grid, the frames of its four strikes, and
/// its three comet pictures (size, lift and offset).
extern u8 gProcne_GridX[];
extern u8 gProcne_GridY[];
extern u16 gProcne_Frames[];
extern u8 gProcne_PictureWidths[];
extern u8 gProcne_PictureHeights[];
extern u8 gProcne_PictureBiasY[];
extern u16 gProcne_Pictures[];
/// Tiamat's embers (speed and size by ember index & 3), its sparks' 1x2
/// picture, and the six flame pictures (size, drop and offset).
extern u8 gTiamat_EmberSpeeds[];
extern u8 gTiamat_SparkPixels[];
extern u8 gTiamat_EmberSizes[];
extern u8 gTiamat_FlameWidths[];
extern u8 gTiamat_FlameHeights[];
extern u8 gTiamat_FlameDrops[];
extern u16 gTiamat_FlamePictures[];

/// The foot of each of Nereid's columns of fire.
enum {
  NEREID_COLUMN_X, ///< canvas x of the column's middle
  NEREID_COLUMN_Y  ///< canvas y of its foot
};
extern u8 gNereid_Columns[][2];

/// A falling drop's gravity in 16.16 pixels a frame each frame, by drop
/// index & 3.
extern s32 gNereid_Gravity[];

/// Each Froth variant: its motes and frames.
enum {
  FROTH_BUBBLE_COUNT, ///< bubbles, at most 30 (battle_effect_vortices.c)
  FROTH_FRAMES        ///< the effect's frames; the damage shows 16 before
};
extern u8 gVortices_Counts[][2];

/// Each Whirlwind variant: its tornadoes, their sway and the screen shake.
enum {
  WHIRLWIND_TORNADOES, ///< tornadoes drawn
  WHIRLWIND_SWAY,      ///< how far each sways across, in pixels
  WHIRLWIND_SHAKE      ///< BattleCamera_Shake's range
};
extern u8 gVortices_Shapes[][3];

/// Where each Prism shard starts in the crystal.
enum {
  PRISM_SHARD_START_X, ///< pixels from the crystal's left
  PRISM_SHARD_START_Y  ///< pixels from its top
};
extern u8 gPrism_ShardStarts[][2];

/// Each Prism variant: its crystals and frames.
enum {
  PRISM_CRYSTAL_COUNT, ///< crystals drawn
  PRISM_FRAMES         ///< the effect's frames
};
extern u8 gPrism_Counts[][2];

/// The twelve pictures of Prism's shards.
extern u8 gPrism_ShardWidths[];
extern u8 gPrism_ShardHeights[];
extern s32 gPrism_ShardPictures[];

/// Each Plasma variant: the sparks per pillar and the frames of shake.
enum {
  PLASMA_SPARKS,      ///< sparks each lightning pillar throws
  PLASMA_SHAKE_FRAMES ///< frames the screen shakes at each strike
};
extern u16 gPlasma_Sparks[][2];

/// Each Plasma pillar's canvas x, seven for each side, and how many strike by
/// variant.
extern u8 gPlasma_Columns[][7];
extern u8 gPlasma_Counts[];

/// Each spark group variant: sparks per burst, bursts, and from
/// SPARK_GROUPS_OFFSET each burst's offset across.
enum {
  SPARK_GROUPS_SPARKS, ///< sparks each burst throws
  SPARK_GROUPS_BURSTS, ///< bursts, 8 frames apart
  SPARK_GROUPS_OFFSET  ///< burst i's offset across, in pixels, at [2 + i]
};
/// The spark groups' variants past the three strengths of Blast and
/// Dynamite (enum EffectVariant), which their callers set before the run.
enum {
  SPARK_GROUPS_SEVERE_BLOW =
      3,              ///< 340 sparks and a flash: Severe Blow's
                      ///< impact and BattleEffect_RunUnusedSevereBlowLeap
  SPARK_GROUPS_THRASH ///< 32 sparks: Thrash
};
extern u16 gSparkGroups_Shapes[][5];

/// The flash picture for each three frames at the end of a flash's life.
extern u8 gPlasma_FlashPictures[];
/// Volcano's fire pillars by variant: how many stand, the depth of each and
/// the frame each starts.
extern u8 gVolcano_Counts[];
extern s8 gVolcano_Depths[][4];
extern u8 gVolcano_StartFrames[];
/// The flips a glint around a target may take.
extern u8 gTargetMotes_GlintFlips[];

/// Cybele's face as she spits the thorns: the animations of sprites 3 and 4,
/// the left and right halves of the middle row of SUMMON_FORMATION_CYBELE, as
/// the cast's recording shows them. Her mouth, in the left half, opens and
/// shuts; her far eye, in the right, stays as it is.
enum {
  CYBELE_POSE_FACE_LEFT, ///< sprite 3's animation: the half with her mouth
  CYBELE_POSE_FACE_RIGHT ///< sprite 4's animation
};
extern u8 gCybele_ObjectPoses[][2];

/// Cybele's pictures, each by offset and size: the five thorns, the three
/// flowers, the five chips (the thorns again), the five of a spark and
/// the seven of the blast sheet, of which she draws only the last.
extern u16 gCybele_ThornPictures[];
extern u8 gCybele_ThornWidths[];
extern u8 gCybele_ThornHeights[];
extern u16 gCybele_FlowerPictures[];
extern u8 gCybele_FlowerSizes[];
extern u16 gCybele_ChipPictures[];
extern u8 gCybele_ChipWidths[];
extern u8 gCybele_ChipHeights[];
extern u16 gCybele_SparkPictures[];
extern u8 gCybele_SparkSizes[];
extern u16 gCybele_BlastPictures[];
extern u16 gCybele_BlastSizes[];
/// Boreas: where the crystals stand, the background's jolt as the ice block
/// lands, its third sprite's animations as the block sinks, and the pictures
/// of the crystals, the shards thrown as they crumble, the spray, the crystals
/// that rise among the targets and those that fall.
extern u8 gBoreas_Points[][2];
extern s8 gBoreas_ShakeOffsets[];
extern u16 gBoreas_CrystalPictures[];
extern u8 gBoreas_CrystalWidths[];
extern u8 gBoreas_CrystalHeights[];
extern u8 gBoreas_Animations[];
extern u8 gBoreas_DebrisWidths[];
extern u8 gBoreas_DebrisHeights[];
extern u16 gBoreas_DebrisPictures[];
extern u8 gBoreas_SprayWidths[];
extern u8 gBoreas_SprayHeights[];
extern u16 gBoreas_SprayPictures[];
extern u16 gBoreas_RisingCrystalPictures[];
extern u8 gBoreas_RisingCrystalWidths[];
extern u8 gBoreas_RisingCrystalHeights[];
extern u8 gBoreas_FallWidths[];
extern u8 gBoreas_FallHeights[];
extern u16 gBoreas_FallPictures[];
/// The gravity of Drain's ice shards, by shard index & 3.
extern s32 gDrain_Gravities[];
/// The twelve pictures of the stat changes' shards: four shapes, three frames
/// each.
extern u16 gStatChange_ShardPictures[];
extern u8 gStatChange_ShardWidths[];
extern u8 gStatChange_ShardHeights[];
/// Each Quake variant: the effect's frames, each pillar's and the shake.
extern u8 gQuake_Timings[][3];
/// Fire's embers by variant, and the six pictures of its impact flare.
extern u8 gFire_Counts[];
extern u8 gFire_FlareWidths[];
extern u8 gFire_FlareHeights[];
extern u8 gFire_FlareBiasY[];
extern u16 gFire_FlarePictures[];

/// Where each ice mound of Frost stands: x and the y of its foot.
enum {
  FROST_MOUND_X,   ///< canvas x of the mound's left edge
  FROST_MOUND_FOOT ///< canvas y of its foot
};
extern u8 gFrost_MoundSpots[][2];

/// Frost: its motes, mounds and burst frame by variant, and the pictures of
/// its motes, its three ice mounds and its chips (the motes again).
extern u8 gFrost_Timings[][3];
extern u8 gFrost_MoteWidths[];
extern u8 gFrost_MoteHeights[];
extern u16 gFrost_MotePictures[];
extern u16 gFrost_MoundPictures[];
extern u8 gFrost_MoundHeights[];
extern u8 gFrost_ChipWidths[];
extern u8 gFrost_ChipHeights[];
extern u16 gFrost_ChipPictures[];
/// Ray's sparks by variant; [1] is how many each bolt throws a frame.
extern u8 gRay_Sparks[][2];
/// Eruption's motes, puffs and frames by variant, and its seven puff
/// pictures.
extern u8 gEruption_Counts[][3];
extern u16 gEruption_PuffPictures[];
extern u16 gEruption_PuffSizes[];
/// Unused: picture layouts nothing reads, from 6x6 to 20x20.
extern u16 gUnused_SheetPicturesShort[];
extern u16 gUnused_SheetPictures[];
extern u8 gUnused_SheetWidths[];
extern u8 gUnused_SheetHeights[];
/// The scale Atalanta's sprites keep throughout, one to one.
extern struct SpriteScale gAtalanta_UnitScale;
/// Growth's columns: the three shapes' vine and pillar pictures and height
/// caps, the columns by variant, their roots and the pillars' heights, and
/// the seven pictures of a puff of dust.
extern u8 gGrowth_VineWidths[];
extern u8 gGrowth_HeightLimits[];
extern u16 gGrowth_VinePictures[];
extern u8 gGrowth_PillarWidths[];
extern u16 gGrowth_PillarPictures[];
extern u8 gGrowth_ColumnCounts[];
extern u8 gGrowth_ColumnX[];
extern s8 gGrowth_PillarHeights[];
extern s8 gGrowth_PuffWidths[];
extern u8 gGrowth_PuffHeights[];
extern u16 gGrowth_PuffPictures[];
/// Each of Vine's roots, across from the middle of the targets.
extern s8 gVine_ColumnOffsets[];
// A thorn's kind in gThorn_Kinds.
#define THORN_MIRRORED 1 ///< drawn mirrored across
#define THORN_SMALL 2    ///< 32 pixels wide, not 48
/// Thorn: each thorn's kind and root, the thorns by variant, and the seven
/// pictures of a puff of dust.
extern u8 gThorn_Kinds[];
extern s8 gThorn_X[];
extern u8 gThorn_Counts[];
extern s8 gThorn_PuffWidths[];
extern u8 gThorn_PuffHeights[];
extern u16 gThorn_PuffPictures[];
/// Bolt by variant (sparks, glints, their spread and bolts), the flips a
/// glint may take, and the glints' blend mode by variant.
extern u8 gBolt_Counts[][4];
extern u8 gBolt_GlintFlips[];
extern u8 gBolt_GlintModes[];
/// The volley's three comet sheets: Slash, Wind Slash and Sonic
/// Slash fire the one their variant picks.
enum ProjectileVolleyComet {
  PROJECTILE_VOLLEY_COMET_A, ///< SlashCometSheet, Slash's
  PROJECTILE_VOLLEY_COMET_B, ///< WindSlashCometSheet, Wind Slash's
  PROJECTILE_VOLLEY_COMET_C ///< SonicSlashCometSheet, Sonic Slash's and Cutting
                            ///< Edge's
};
/// Each comet's picture height.
extern u8 gProjectileVolley_CometHeights[];
/// The projectile volley's modes (BattleEffect_RunProjectileVolley), each named
/// for the effect that fires it; the casting impacts fire two of them.
enum ProjectileVolleyMode {
  PROJECTILE_VOLLEY_SLASH,         ///< comets, picked by the variant
  PROJECTILE_VOLLEY_SHURIKEN,      ///< spinning shurikens
  PROJECTILE_VOLLEY_CRAZY_VOICE,   ///< bouncing pellets
  PROJECTILE_VOLLEY_WING_FLUTTER,  ///< yellow sparks
  PROJECTILE_VOLLEY_WING_BEAT,     ///< small yellow sparks
  PROJECTILE_VOLLEY_CUTTING_EDGE,  ///< one large comet in ice colours
  PROJECTILE_VOLLEY_HEAT_WAVE,     ///< fireballs trailing embers
  PROJECTILE_VOLLEY_SLEET,         ///< ice chips falling from high above
  PROJECTILE_VOLLEY_IMPACT_COMETS, ///< comets, after
                                   ///< CASTING_IMPACT_UNUSED_COMETS
  PROJECTILE_VOLLEY_BLIZZARD,      ///< scattered ice chips; also after
                                   ///< CASTING_IMPACT_UNUSED_ICE_CHIPS
  PROJECTILE_VOLLEY_ICE_BLESSING,  ///< ice chips streaming one a frame
  PROJECTILE_VOLLEY_WING_STROKE    ///< large yellow sparks, spread up and down
};
/// Each projectile volley mode by its columns, the seven pictures of the hit
/// flash (size, step, rise and offset), and the four ice chip pictures.
extern u8 gProjectileVolley_Modes[][5];
extern u8 gProjectileVolley_FlashWidths[];
extern u8 gProjectileVolley_FlashHeights[];
extern u8 gProjectileVolley_FlashSteps[];
extern u8 gProjectileVolley_FlashRises[];
extern u16 gProjectileVolley_FlashPictures[];
extern u16 gProjectileVolley_PelletPictures[];
/// The frames on which the Mercury Djinni fires each of its five flames.
extern u8 gDjinnAttacks_LaunchFrames[];
/// Hail's four 8x8 ice chip pictures.
extern u8 gHail_PictureWidths[];
extern u8 gHail_PictureHeights[];
extern u16 gHail_Pictures[];
/// Ground: the flips a crackle may take, and the portal's nine pictures (the
/// portal, its white core and its crackles) by size, offset and place.
extern u8 gGround_CrackleFlips[];
extern u8 gGround_PortalWidths[];
extern u8 gGround_PortalHeights[];
extern u16 gGround_PortalPictures[];
extern u8 gGround_PortalX[];
extern u8 gGround_PortalY[];
/// The colours a target flashes when Thor's bolt strikes it, and the flips of
/// the crackles around Thor.
extern u8 gThor_TargetPalettes[];
extern u8 gThor_CrackleFlips[];

/// Where each Spire shard starts, before its rock's offset across.
enum {
  SPIRE_SHARD_X, ///< canvas x
  SPIRE_SHARD_Y  ///< canvas y
};
extern u8 gSpire_ShardOffsets[][2];

/// Spire: each rock's offset across and the frame it appears, the rocks by
/// variant, and its shards' fifteen pictures.
extern s8 gSpire_RockX[];
extern u8 gSpire_DropFrames[];
extern u8 gSpire_RockCounts[];
extern u8 gSpire_ShardWidths[];
extern u8 gSpire_ShardHeights[];
extern u16 gSpire_ShardPictures[];
/// The flurry's modes (BattleEffect_RunFlurry), its rows of
/// gFlurry_Styles: Gust and Flying Attack take one, two or three
/// strikes as a double or triple blow raises their variant.
enum FlurryMode {
  FLURRY_BEAR_CLAW,            ///< five strikes; also Ransack
  FLURRY_BACTERIA_RUSH,        ///< seven strikes, in lime
  FLURRY_FREEBITE_RUSH,        ///< nine strikes, in blue
  FLURRY_FLYING_ATTACK,        ///< one strike, in embers
  FLURRY_FLYING_ATTACK_DOUBLE, ///< two
  FLURRY_FLYING_ATTACK_TRIPLE, ///< three
  FLURRY_GUST,                 ///< one strike, each filling the canvas
  FLURRY_GUST_DOUBLE,          ///< two
  FLURRY_GUST_TRIPLE           ///< three
};
/// A flurry's arcs, its FLURRY_ARC_SHEET.
enum FlurryArcs {
  FLURRY_ARCS_AND_WAVES, ///< blue arc A, with a wave for the third and
                         ///< fourth of every four strikes
  FLURRY_ARCS            ///< blue arc B for every strike
};
/// Whose palette a flurry takes, its FLURRY_PALETTE.
enum FlurryPalette {
  FLURRY_PALETTE_MARS_DJINNI, ///< the Mars Djinni sprite sheet's
  FLURRY_PALETTE_LIME,
  FLURRY_PALETTE_BLUE_ARC,
  FLURRY_PALETTE_EMBER ///< the ember streak sprite sheet's
};
extern u8 gFlurry_Styles[][7];
/// Gaia's rising wall: its nine rows, the three flames at its foot, BG2's
/// shift by side and variant, and the columns of the late rocks.
extern u16 gGaia_RowPictures[];
extern u8 gGaia_RowWidths[];
extern u16 gGaia_FlamePictures[];
extern u8 gGaia_FlameWidths[];
extern u8 gGaia_FlameHeights[];
extern s8 gGaia_Bg2Shifts[][3];
extern u8 gGaia_LateRockColumns[];
/// The six pictures of the growing beam, and how far each reaches short of the
/// user.
extern u8 gBeam_PictureWidths[];
extern u8 gBeam_PictureHeights[];
extern u16 gBeam_Pictures[];
extern u8 gBeam_PictureReaches[];
/// The casting impacts: the flips a glint may take, where Mortal Danger draws
/// the reaper's pictures, and the four ice chip pictures.
extern u8 gCastingImpact_GlintDrawFlags[];
extern u8 gReaper_PictureX[][7];
extern u8 gReaper_PictureY[];
extern u16 gCastingImpact_IceChipPictures[];
/// The seven pictures of Helm Splitter's fire swirl, and where each sits from
/// the user.
extern u8 gHelmSplitter_PictureWidths[];
extern u8 gHelmSplitter_PictureHeights[];
extern u16 gHelmSplitter_Pictures[];
extern s8 gHelmSplitter_PictureBiasX[];
extern s8 gHelmSplitter_PictureBiasY[];
/// Where the sprites of a summon's formation stand, in pixels, one axis at a
/// time (BattleEffect_PlaceFormationSprites).
extern u8 gSummonFormation_NereidX[];
extern u8 gSummonFormation_NereidZ[];
extern u8 gSummonFormation_CybeleX[];
extern u8 gSummonFormation_CybeleZ[];
extern u8 gSummonFormation_TiamatX[];
extern u8 gSummonFormation_TiamatZ[];
/// The eight pictures of the impact flash.
extern u8 gImpactBurst_PictureWidths[];
extern u8 gImpactBurst_PictureHeights[];
extern u16 gImpactBurst_Pictures[];
/// The starfield fall: the seven sparks that orbit the falling sprite, where
/// the seven fireballs fall, and the blast sheet's seven flash pictures.
extern s16 gStarfieldFall_OrbitPoints[][3];
extern s16 gStarfieldFall_FireballPoints[][2];
extern u16 gStarfieldFall_FlashPictures[];
extern u16 gStarfieldFall_FlashSizes[];
/// Where Ramses's sprites stand from the statue's corner, and the blast
/// sheet's puff pictures.
extern u8 gRamses_StatueColumns[];
extern u8 gRamses_StatueRows[];
extern u16 gRamses_PuffPictures[];
extern u16 gRamses_PuffSizes[];
/// Ragnarok's strike columns, three for each side, and the flash picture for
/// each three frames at the end of a flash's life.
extern u8 gRagnarok_StrikeColumns[][3];
extern u8 gRagnarok_FlashPictures[];
/// Titan Blade's fading flash pictures and its dust's gravity.
extern u8 gTitanBlade_FlashPictures[];
extern s32 gTitanBlade_DustGravity[];
/// Judgment: its burst's seven quarter pictures, and the six shards thrown
/// from it.
extern u8 gJudgment_BurstHeights[];
extern u16 gJudgment_ShieldSprites[];
extern u16 gJudgment_ShardPictures[];
extern u8 gJudgment_ShardWidths[];
extern u8 gJudgment_ShardHeights[];
/// Where Kirin's nine sprites stand, in pixels from its corner.
extern u8 gKirin_Columns[];
extern u8 gKirin_Rows[];
/// Neptune's eight sprites, offsets in pixels scaled by 1.5 as placed, and its
/// spray's gravity by particle index & 3.
extern u8 gNeptune_ObjectX[];
extern u8 gNeptune_ObjectY[];
extern s32 gNeptune_Gravity[];
/// The seven square pictures of Undead Sword's puffs.
extern u16 gUndeadSword_PuffPictures[];
extern u16 gUndeadSword_PuffSizes[];

#endif
