#ifndef ALCHEMY_BATTLE_EFFECT_BATTLE_EFFECT_TABLE_H
#define ALCHEMY_BATTLE_EFFECT_BATTLE_EFFECT_TABLE_H

#include "types.h"
#include "render_placement.h"
#include "vector3.h"

// The two kinds of target strike (gTargetStrike_Styles).
#define STRIKE_KIND_SLASH 0 ///< sideways slashes
/// Rows of fangs above and below the target that close on it.
#define STRIKE_KIND_BITE 1

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

// Every table battle_effect_table.c defines, in its order. A table read by
// columns is a two-dimensional array, its columns named by the enum above it.
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
extern u8 gFalling_IceCounts[][2];

/// Each Douse variant: its bolt waves, sparks per bolt, spacing and frames.
enum {
  FALLING_BOLT_WAVE_COUNT,      ///< bolts that drop, of the 64 placed
  FALLING_BOLT_SPARKS_PER_BOLT, ///< sparks each bolt throws
  FALLING_BOLT_SPACING,         ///< pixels between bolts along the slant
  FALLING_BOLT_DURATION         ///< the effect's frames
};
extern u8 gFalling_Counts[][4];

extern s8 gStreaks_Styles[][4];
extern s8 gStreaks_Tremble[];
extern u8 gTargetStrike_Styles[][7];
extern u8 gTargetStrike_Widths[];
extern u8 gTargetStrike_Heights[];
extern u16 gTargetStrike_Pictures[];
extern u8 gTargetStrike_Reach[];
extern u8 gTargetStrike_BiteStages[];
extern u8 gTargetStrike_GlintDrawFlags[];
extern u8 gDeathPlunge_SwordWidths[];
extern u8 gDeathPlunge_SwordHeights[];
extern u16 gDeathPlunge_SwordPictures[];
extern u8 gDeathPlunge_PieceWidths[];
extern u8 gDeathPlunge_PieceHeights[];
extern u16 gDeathPlunge_PiecePictures[];
extern u8 gDeathPlunge_PieceStartX[];
extern u8 gDeathPlunge_PieceStartY[];
extern s8 gDeathPlunge_X[];
extern s8 gDeathPlunge_Y[];
extern u8 gDeathPlunge_DrawFlags[];
extern u8 gSpiderWeb_PictureWidths[];
extern u8 gSpiderWeb_PictureHeights[];
extern u16 gSpiderWeb_Pictures[];
extern u8 gSpiderWeb_PictureX[];
extern s8 gSpiderWeb_PictureBiasY[];

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

extern u16 gMirroredWall_Pictures[];
extern u8 gMirroredWall_PictureWidths[];
extern u8 gMirroredWall_PictureHeights[];
extern u8 gDjinnBeams_MotePictures[];
extern u16 gDjinnBeams_HeadPictures[];
extern u8 gDjinnBeams_HeadWidths[];
extern u8 gDjinnBeams_HeadHeights[];
extern s8 gLobbedShells_Styles[];
extern u8 gLobbedShells_ShardWidths[];
extern u8 gLobbedShells_ShardHeights[];
extern u16 gLobbedShells_ShardPictures[];
extern u8 gCondemn_PictureX[][7];
extern u8 gCondemn_PictureY[][7];
extern struct Vector3 gBind_Vertex[];
extern struct Vector3 gBind_TriangleStrikeVertex[];
extern struct Vector3 gStar_Points[];
extern u8 gStar_Radii[];
extern u8 gProcne_GridX[];
extern u8 gProcne_GridY[];
extern u16 gProcne_Frames[];
extern u8 gProcne_PictureWidths[];
extern u8 gProcne_PictureHeights[];
extern u8 gProcne_PictureBiasY[];
extern u16 gProcne_Pictures[];
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

extern s32 gNereid_Gravity[];

/// Each Froth variant: its motes and frames.
enum {
  FROTH_BUBBLE_COUNT, ///< bubbles, at most 30 (vortices.c)
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

extern u8 gPrism_ShardWidths[];
extern u8 gPrism_ShardHeights[];
extern s32 gPrism_ShardPictures[];

/// Each Plasma variant: the sparks per pillar and the frames of shake.
enum {
  PLASMA_SPARKS,      ///< sparks each lightning pillar throws
  PLASMA_SHAKE_FRAMES ///< frames the screen shakes at each strike
};
extern u16 gPlasma_Sparks[][2];

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
extern u16 gPlasma_Shapes[][5];

extern u8 gPlasma_FlashPictures[];
extern u8 gVolcano_Counts[];
extern s8 gVolcano_Depths[][4];
extern u8 gVolcano_StartFrames[];
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
extern s32 gDrain_Gravities[];
extern u16 gStatChange_ShardPictures[];
extern u8 gStatChange_ShardWidths[];
extern u8 gStatChange_ShardHeights[];
extern u8 gQuake_Timings[][3];
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

extern u8 gFrost_Timings[][3];
extern u8 gFrost_MoteWidths[];
extern u8 gFrost_MoteHeights[];
extern u16 gFrost_MotePictures[];
extern u16 gFrost_MoundPictures[];
extern u8 gFrost_MoundHeights[];
extern u8 gFrost_ChipWidths[];
extern u8 gFrost_ChipHeights[];
extern u16 gFrost_ChipPictures[];
extern u8 gRay_Sparks[][2];
extern u8 gEruption_Counts[][3];
extern u16 gEruption_PuffPictures[];
extern u16 gEruption_PuffSizes[];
extern u16 gUnused_SheetPictures6[];
extern u16 gUnused_SheetPictures[];
extern u8 gUnused_SheetWidths[];
extern u8 gUnused_SheetHeights[];
extern struct SpriteScale gAtalanta_UnitScale;
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
extern s8 gGrowth_ColumnOffsets[];
// A thorn's kind in gGrowth_Kinds.
#define THORN_MIRRORED 1 ///< drawn mirrored across
#define THORN_SMALL 2    ///< 32 pixels wide, not 48
extern u8 gGrowth_Kinds[];
extern s8 gGrowth_X[];
extern u8 gGrowth_Counts[];
extern s8 gGrowth_ThornPuffWidths[];
extern u8 gGrowth_ThornPuffHeights[];
extern u16 gGrowth_ThornPuffPictures[];
extern u8 gBolt_Counts[][4];
extern u8 gBolt_GlintFlips[];
extern u8 gBolt_GlintModes[];
/// The volley's comets, its CometSheet resources: Slash, Wind Slash and Sonic
/// Slash fire the one their variant picks.
enum ProjectileVolleyComet {
  PROJECTILE_VOLLEY_COMET_A, ///< CometSheetA
  PROJECTILE_VOLLEY_COMET_B, ///< CometSheetB
  PROJECTILE_VOLLEY_COMET_C  ///< CometSheetC, also Cutting Edge's
};
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
                                   ///< CASTING_IMPACT_IMPACT_UNUSED_COMETS
  PROJECTILE_VOLLEY_BLIZZARD,      ///< scattered ice chips; also after
                                   ///< CASTING_IMPACT_IMPACT_UNUSED_ICE_CHIPS
  PROJECTILE_VOLLEY_ICE_BLESSING,  ///< ice chips streaming one a frame
  PROJECTILE_VOLLEY_WING_STROKE    ///< large yellow sparks, spread up and down
};
extern u8 gProjectileVolley_Modes[][5];
extern u8 gProjectileVolley_FlashWidths[];
extern u8 gProjectileVolley_FlashHeights[];
extern u8 gProjectileVolley_FlashSteps[];
extern u8 gProjectileVolley_FlashRises[];
extern u16 gProjectileVolley_FlashPictures[];
extern u16 gProjectileVolley_PelletPictures[];
extern u8 gDjinnAttacks_LaunchFrames[];
extern u8 gHail_PictureWidths[];
extern u8 gHail_PictureHeights[];
extern u16 gHail_Pictures[];
extern u8 gHail_CrackleFlips[];
extern u8 gHail_PortalWidths[];
extern u8 gHail_PortalHeights[];
extern u16 gHail_PortalPictures[];
extern u8 gHail_PortalX[];
extern u8 gHail_PortalY[];
extern u8 gThor_TargetPalettes[];
extern u8 gThor_CrackleFlips[];

/// Where each Spire shard starts, before its rock's offset across.
enum {
  SPIRE_SHARD_X, ///< canvas x
  SPIRE_SHARD_Y  ///< canvas y
};
extern u8 gSpire_ShardOffsets[][2];

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
  FLURRY_PALETTE_MARS_DJINNI, ///< the Mars Djinni sheet's
  FLURRY_PALETTE_LIME,
  FLURRY_PALETTE_BLUE_ARC,
  FLURRY_PALETTE_EMBER ///< the ember streak sheet's
};
extern u8 gFlurry_Styles[][7];
extern u16 gGaia_RowPictures[];
extern u8 gGaia_RowWidths[];
extern u16 gGaia_FlamePictures[];
extern u8 gGaia_FlameWidths[];
extern u8 gGaia_FlameHeights[];
extern s8 gGaia_Bg2Shifts[][3];
extern u8 gGaia_LateRockColumns[];
extern u8 gBeam_PictureWidths[];
extern u8 gBeam_PictureHeights[];
extern u16 gBeam_Pictures[];
extern u8 gBeam_PictureReaches[];
extern u8 gCastingImpact_GlintDrawFlags[];
extern u8 gCastingImpact_PictureX[][7];
extern u8 gCastingImpact_PictureY[];
extern u16 gCastingImpact_IceChipPictures[];
extern u8 gHelmSplitter_PictureWidths[];
extern u8 gHelmSplitter_PictureHeights[];
extern u16 gHelmSplitter_Pictures[];
extern s8 gHelmSplitter_PictureBiasX[];
extern s8 gHelmSplitter_PictureBiasY[];
extern u8 gHelmSplitter_TwelveX[];
extern u8 gHelmSplitter_TwelveZ[];
extern u8 gHelmSplitter_RightEightX[];
extern u8 gHelmSplitter_RightEightZ[];
extern u8 gHelmSplitter_SmallEightX[];
extern u8 gHelmSplitter_SmallEightZ[];
extern u8 gHelmSplitter_ImpactBurstPictureWidths[];
extern u8 gHelmSplitter_ImpactBurstPictureHeights[];
extern u16 gHelmSplitter_ImpactBurstPictures[];
extern s16 gParticleStreams_OrbitPoints[][3];
extern s16 gParticleStreams_FireballPoints[][2];
extern u16 gParticleStreams_FlashPictures[];
extern u16 gParticleStreams_FlashSizes[];
extern u8 gRamses_StatueColumns[];
extern u8 gRamses_StatueRows[];
extern u16 gRamses_PuffPictures[];
extern u16 gRamses_PuffSizes[];
extern u8 gRagnarok_StrikeColumns[][3];
extern u8 gRagnarok_FlashPictures[];
extern u8 gTitanBlade_FlashPictures[];
extern s32 gTitanBlade_DustGravity[];
extern u8 gJudgment_BurstHeights[];
extern u16 gJudgment_BurstPictures[];
extern u16 gJudgment_ShardPictures[];
extern u8 gJudgment_ShardWidths[];
extern u8 gJudgment_ShardHeights[];
extern u8 gKirin_Columns[];
extern u8 gKirin_Rows[];
extern u8 gNeptune_ObjectX[];
extern u8 gNeptune_ObjectY[];
extern s32 gNeptune_Gravity[];
extern u16 gUndeadSword_SparkPictures[];
extern u16 gUndeadSword_SparkSizes[];

#endif
