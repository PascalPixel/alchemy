#ifndef ALCHEMY_BATTLE_EFFECT_TABLE_H
#define ALCHEMY_BATTLE_EFFECT_TABLE_H

/* @file */
/* The tables the battle effects read (battle_effect_table.c), in its order: */
/* picture offsets, sizes, timings and counts by variant. A table read by */
/* columns is a two-dimensional array, its columns named by the enum above it.
 */

#include "types.h"
#include "graphics/render_placement.h"
#include "system.h"

/* The two kinds of target strike (TargetStrike_Styles). */
#define STRIKE_KIND_SLASH 0 /* sideways slashes */
#define STRIKE_KIND_BITE                                                       \
	1 /* rows of fangs above and below that close on it                        \
	   */

/* A target strike style's extras. */
#define STRIKE_SMOKE                                                           \
	1 /* each hit raises smoke puffs from the target's feet                    \
	   */
#define STRIKE_ABSORB                                                          \
	2					/* motes fly out of the target and back into the user  \
						 */
#define STRIKE_GLINTS 4 /* glints sparkle around the target */
#define STRIKE_SPARKS 8 /* each hit scatters sparks */
#define STRIKE_HEAVY                                                           \
	16 /* one heavy hit: a big shake and the target recoils                    \
		*/

/* The palettes a target strike's background takes (TARGET_STRIKE_COLOR). */
#define STRIKE_COLOR_BLUE	   0 /* BlueBeamSheet's */
#define STRIKE_COLOR_LIME	   1 /* LimePalette */
#define STRIKE_COLOR_PINK	   2 /* PinkPalette */
#define STRIKE_COLOR_EMBER	   3 /* EmberStreakSheet's */
#define STRIKE_COLOR_MARS	   4 /* MarsDjinnSheet's */
#define STRIKE_COLOR_LIGHTNING 5 /* LightningBoltSheet's */

/* The twelve target strike styles, rows of TargetStrike_Styles. */
typedef enum TARGET_STRIKE_STYLE {
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

/* Where each picture begins in the sprite sheets many effects share: the ten */
/* motes and twenty flares of a glow sheet, the nine sparkles, the six flame */
/* puffs (size, drop and offset) and the six glints (offset and size). */
extern u16 BattleEffect_MotePictures[];
extern u16 BattleEffect_FlarePictures[];
extern u16 BattleEffect_SparklePictures[];
extern u8 BattleEffect_SparkleSizes[];
extern u8 BattleEffect_PuffArcPictureWidths[];
extern u8 BattleEffect_PuffArcPictureHeights[];
extern u8 BattleEffect_PuffArcPictureBiasY[];
extern u16 BattleEffect_PuffArcPictures[];
extern u16 BattleEffect_GlintPictures[];
extern u8 BattleEffect_GlintPictureWidths[];
extern u8 BattleEffect_GlintPictureHeights[];

/* Each Ice variant: its shards and how many frames it runs. */
enum {
	ICE_SHARD_COUNT, /* shards that fall */
	ICE_FRAMES		 /* the effect's frames; the damage shows 16 before */
};
extern u8 Ice_Counts[][2];

/* Each Douse variant: its drops, the droplets each splash throws, the */
/* spacing and the frames. */
enum {
	DOUSE_DROP_COUNT,		 /* drops that fall, of the 64 placed */
	DOUSE_DROPLETS_PER_DROP, /* droplets each landing drop throws up */
	DOUSE_DROP_SPACING,		 /* pixels between drops along the slant */
	DOUSE_FRAMES /* the effect's frames; the damage shows 64 before */
};
extern u8 Douse_Counts[][4];

/* Each streak style's effect, hit, motion and shake, and the pixels a struck */
/* target trembles by, frame by frame. */
extern s8 Streak_Styles[][4];
extern s8 Streak_Tremble[];
/* The target strike: its twelve styles by the TARGET_STRIKE_ columns, the */
/* six slash pictures (size, offset and reach), the fang stage of each picture
 */
/* of a bite, and the flips a glint may take. */
extern u8 TargetStrike_Styles[][7];
extern u8 TargetStrike_Widths[];
extern u8 TargetStrike_Heights[];
extern u16 TargetStrike_Pictures[];
extern u8 TargetStrike_Reach[];
extern u8 TargetStrike_BiteStages[];
extern u8 TargetStrike_GlintDrawFlags[];
/* The blade rain's four sword pictures, by how steeply the sword falls. */
extern u8 BladeRain_SwordWidths[];
extern u8 BladeRain_SwordHeights[];
extern u16 BladeRain_SwordPictures[];
/* The 33 pieces the shattering opening's picture breaks into: size, offset in
 */
/* the picture and where each starts on the canvas. */
extern u8 ShatteringOpening_PieceWidths[];
extern u8 ShatteringOpening_PieceHeights[];
extern u16 ShatteringOpening_PiecePictures[];
extern u8 ShatteringOpening_PieceStartX[];
extern u8 ShatteringOpening_PieceStartY[];
/* The element burst's four corner sparks: offsets and flips. */
extern s8 ElementBurst_SparkX[];
extern s8 ElementBurst_SparkY[];
extern u8 ElementBurst_SparkFlips[];
/* The swirl swipe's six pictures (Spider Web): size, offset, an x for each */
/* side and a y offset. */
extern u8 SwirlSwipe_PictureWidths[];
extern u8 SwirlSwipe_PictureHeights[];
extern u16 SwirlSwipe_Pictures[];
extern u8 SwirlSwipe_PictureX[];
extern s8 SwirlSwipe_PictureBiasY[];

/* Each gathering-lights mode: the lights per target and the frames before the
 */
/* targets' turns. */
enum {
	GATHERING_LIGHTS_COUNT, /* lights for each target */
	GATHERING_LIGHTS_FRAMES /* frames before the targets' turns */
};
/* The gathering lights' modes (btEffectRunGatheringLights), each named */
/* for the effect that plays it, and its row of GatheringLights_Counts. */
typedef enum GATHERING_LIGHTS_MODE {
	GATHERING_LIGHTS_DEMON_NIGHT, /* pink */
	GATHERING_LIGHTS_RUMBLE,	  /* Mars Djinn colours */
	GATHERING_LIGHTS_HAUNTING	  /* Mercury Djinn colours */
} GATHERING_LIGHTS_MODE;
extern u8 GatheringLights_Counts[][2];

/* The earth wall's three pictures, as the mirrored wall reads them. */
extern u16 MirroredWall_Pictures[];
extern u8 MirroredWall_PictureWidths[];
extern u8 MirroredWall_PictureHeights[];
/* Revive's beams: the mote picture of each step of its eight-step cycle, and */
/* the three pictures of the glow at a target's feet. */
extern u8 DjinnBeams_MotePictures[];
extern u16 DjinnBeams_HeadPictures[];
extern u8 DjinnBeams_HeadWidths[];
extern u8 DjinnBeams_HeadHeights[];
/* Each lobbed shells mode's style (LOBBED_SHELLS_MODE, */
/* battle_effect_lobbed_shells.c). */
enum {
	LOBBED_SHELLS_STYLE_SHATTER, /* nonzero: the shells burst into shards, not
								  */
								 /* smoke */
	LOBBED_SHELLS_STYLE_UNUSED	 /* 0 in every row; nothing reads it */
};
extern s8 LobbedShells_Styles[][2];
/* The eighteen pictures of the lobbed shells' tumbling shards, a large set of
 */
/* nine and a small one. */
extern u8 LobbedShells_ShardWidths[];
extern u8 LobbedShells_ShardHeights[];
extern u16 LobbedShells_ShardPictures[];
/* Where Condemn draws the reaper's seven pictures: an x for each casting */
/* side, and a y. */
extern u8 Condemn_PictureX[][7];
extern u8 Condemn_PictureY[][7];
/* The first vertex of Bind's spinning and striking triangles, 24 pixels up */
/* from the centre. */
extern VECTOR3 Bind_Vertex[];
extern VECTOR3 Bind_TriangleStrikeVertex[];
/* The stars' two points, 48 and 28 pixels up, and their radii. */
extern VECTOR3 Star_Points[];
extern u8 Star_Radii[];
/* Procne's nine sprites on a 3x3 grid, the frames of its four strikes, and */
/* its three comet pictures (size, lift and offset). */
extern u8 Procne_GridX[];
extern u8 Procne_GridY[];
extern u16 Procne_Frames[];
extern u8 Procne_PictureWidths[];
extern u8 Procne_PictureHeights[];
extern u8 Procne_PictureBiasY[];
extern u16 Procne_Pictures[];
/* Tiamat's embers (speed and size by ember index & 3), its sparks' 1x2 */
/* picture, and the six flame pictures (size, drop and offset). */
extern u8 Tiamat_EmberSpeeds[];
extern u8 Tiamat_SparkPixels[];
extern u8 Tiamat_EmberSizes[];
extern u8 Tiamat_FlameWidths[];
extern u8 Tiamat_FlameHeights[];
extern u8 Tiamat_FlameDrops[];
extern u16 Tiamat_FlamePictures[];

/* The foot of each of Nereid's columns of fire. */
enum {
	NEREID_COLUMN_X, /* canvas x of the column's middle */
	NEREID_COLUMN_Y	 /* canvas y of its foot */
};
extern u8 Nereid_Columns[][2];

/* A falling drop's gravity in 16.16 pixels a frame each frame, by drop */
/* index & 3. */
extern s32 Nereid_Gravity[];

/* Each Froth variant: its motes and frames. */
enum {
	FROTH_BUBBLE_COUNT, /* bubbles, at most 30 (battle_effect_vortices.c) */
	FROTH_FRAMES		/* the effect's frames; the damage shows 16 before */
};
extern u8 Vortices_Counts[][2];

/* Each Whirlwind variant: its tornadoes, their sway and the screen shake. */
enum {
	WHIRLWIND_TORNADOES, /* tornadoes drawn */
	WHIRLWIND_SWAY,		 /* how far each sways across, in pixels */
	WHIRLWIND_SHAKE		 /* btCameraShake's range */
};
extern u8 Vortices_Shapes[][3];

/* Where each Prism shard starts in the crystal. */
enum {
	PRISM_SHARD_START_X, /* pixels from the crystal's left */
	PRISM_SHARD_START_Y	 /* pixels from its top */
};
extern u8 Prism_ShardStarts[][2];

/* Each Prism variant: its crystals and frames. */
enum {
	PRISM_CRYSTAL_COUNT, /* crystals drawn */
	PRISM_FRAMES		 /* the effect's frames */
};
extern u8 Prism_Counts[][2];

/* The twelve pictures of Prism's shards. */
extern u8 Prism_ShardWidths[];
extern u8 Prism_ShardHeights[];
extern s32 Prism_ShardPictures[];

/* Each Plasma variant: the sparks per pillar and the frames of shake. */
enum {
	PLASMA_SPARKS,		/* sparks each lightning pillar throws */
	PLASMA_SHAKE_FRAMES /* frames the screen shakes at each strike */
};
extern u16 Plasma_Sparks[][2];

/* Each Plasma pillar's canvas x, seven for each side, and how many strike by */
/* variant. */
extern u8 Plasma_Columns[][7];
extern u8 Plasma_Counts[];

/* Each spark group variant: sparks per burst, bursts, and from */
/* SPARK_GROUPS_OFFSET each burst's offset across. */
enum {
	SPARK_GROUPS_SPARKS, /* sparks each burst throws */
	SPARK_GROUPS_BURSTS, /* bursts, 8 frames apart */
	SPARK_GROUPS_OFFSET	 /* burst i's offset across, in pixels, at [2 + i] */
};
/* The spark groups' variants past the three strengths of Blast and */
/* Dynamite (EFFECT_VARIANT), which their callers set before the run. */
enum {
	SPARK_GROUPS_SEVERE_BLOW = 3, /* 340 sparks and a flash: Severe Blow's */
	/* impact and btEffectRunUnusedSevereBlowLeap */
	SPARK_GROUPS_THRASH /* 32 sparks: Thrash */
};
extern u16 SparkGroups_Shapes[][5];

/* The flash picture for each three frames at the end of a flash's life. */
extern u8 Plasma_FlashPictures[];
/* Volcano's fire pillars by variant: how many stand, the depth of each and */
/* the frame each starts. */
extern u8 Volcano_Counts[];
extern s8 Volcano_Depths[][4];
extern u8 Volcano_StartFrames[];
/* The flips a glint around a target may take. */
extern u8 TargetMotes_GlintFlips[];

/* Cybele's face as she spits the thorns: the animations of sprites 3 and 4, */
/* the left and right halves of the middle row of SUMMON_FORMATION_CYBELE, as */
/* the cast's recording shows them. Her mouth, in the left half, opens and */
/* shuts; her far eye, in the right, stays as it is. */
enum {
	CYBELE_POSE_FACE_LEFT, /* sprite 3's animation: the half with her mouth */
	CYBELE_POSE_FACE_RIGHT /* sprite 4's animation */
};
extern u8 Cybele_ObjectPoses[][2];

/* Cybele's pictures, each by offset and size: the five thorns, the three */
/* flowers, the five chips (the thorns again), the five of a spark and */
/* the seven of the blast sheet, of which she draws only the last. */
extern u16 Cybele_ThornPictures[];
extern u8 Cybele_ThornWidths[];
extern u8 Cybele_ThornHeights[];
extern u16 Cybele_FlowerPictures[];
extern u8 Cybele_FlowerSizes[];
extern u16 Cybele_ChipPictures[];
extern u8 Cybele_ChipWidths[];
extern u8 Cybele_ChipHeights[];
extern u16 Cybele_SparkPictures[];
extern u8 Cybele_SparkSizes[];
extern u16 Cybele_BlastPictures[];
extern u16 Cybele_BlastSizes[];
/* Boreas: where the crystals stand, the background's jolt as the ice block */
/* lands, its third sprite's animations as the block sinks, and the pictures */
/* of the crystals, the shards thrown as they crumble, the spray, the crystals
 */
/* that rise among the targets and those that fall. */
extern u8 Boreas_Points[][2];
extern s8 Boreas_ShakeOffsets[];
extern u16 Boreas_CrystalPictures[];
extern u8 Boreas_CrystalWidths[];
extern u8 Boreas_CrystalHeights[];
extern u8 Boreas_Animations[];
extern u8 Boreas_DebrisWidths[];
extern u8 Boreas_DebrisHeights[];
extern u16 Boreas_DebrisPictures[];
extern u8 Boreas_SprayWidths[];
extern u8 Boreas_SprayHeights[];
extern u16 Boreas_SprayPictures[];
extern u16 Boreas_RisingCrystalPictures[];
extern u8 Boreas_RisingCrystalWidths[];
extern u8 Boreas_RisingCrystalHeights[];
extern u8 Boreas_FallWidths[];
extern u8 Boreas_FallHeights[];
extern u16 Boreas_FallPictures[];
/* The gravity of Drain's ice shards, by shard index & 3. */
extern s32 Drain_Gravities[];
/* The twelve pictures of the stat changes' shards: four shapes, three frames */
/* each. */
extern u16 StatChange_ShardPictures[];
extern u8 StatChange_ShardWidths[];
extern u8 StatChange_ShardHeights[];
/* Each Quake variant: the effect's frames, each pillar's and the shake. */
extern u8 Quake_Timings[][3];
/* Fire's embers by variant, and the six pictures of its impact flare. */
extern u8 Fire_Counts[];
extern u8 Fire_FlareWidths[];
extern u8 Fire_FlareHeights[];
extern u8 Fire_FlareBiasY[];
extern u16 Fire_FlarePictures[];

/* Where each ice mound of Frost stands: x and the y of its foot. */
enum {
	FROST_MOUND_X,	 /* canvas x of the mound's left edge */
	FROST_MOUND_FOOT /* canvas y of its foot */
};
extern u8 Frost_MoundSpots[][2];

/* Frost: its motes, mounds and burst frame by variant, and the pictures of */
/* its motes, its three ice mounds and its chips (the motes again). */
extern u8 Frost_Timings[][3];
extern u8 Frost_MoteWidths[];
extern u8 Frost_MoteHeights[];
extern u16 Frost_MotePictures[];
extern u16 Frost_MoundPictures[];
extern u8 Frost_MoundHeights[];
extern u8 Frost_ChipWidths[];
extern u8 Frost_ChipHeights[];
extern u16 Frost_ChipPictures[];
/* Ray's sparks by variant; [1] is how many each bolt throws a frame. */
extern u8 Ray_Sparks[][2];
/* Eruption's motes, puffs and frames by variant, and its seven puff */
/* pictures. */
extern u8 Eruption_Counts[][3];
extern u16 Eruption_PuffPictures[];
extern u16 Eruption_PuffSizes[];
/* Unused: picture layouts nothing reads, from 6x6 to 20x20. */
extern u16 Unused_SheetPicturesShort[];
extern u16 Unused_SheetPictures[];
extern u8 Unused_SheetWidths[];
extern u8 Unused_SheetHeights[];
/* The scale Atalanta's sprites keep throughout, one to one. */
extern SPR_SCALE Atalanta_UnitScale;
/* Growth's columns: the three shapes' vine and pillar pictures and height */
/* caps, the columns by variant, their roots and the pillars' heights, and */
/* the seven pictures of a puff of dust. */
extern u8 Growth_VineWidths[];
extern u8 Growth_HeightLimits[];
extern u16 Growth_VinePictures[];
extern u8 Growth_PillarWidths[];
extern u16 Growth_PillarPictures[];
extern u8 Growth_ColumnCounts[];
extern u8 Growth_ColumnX[];
extern s8 Growth_PillarHeights[];
extern s8 Growth_PuffWidths[];
extern u8 Growth_PuffHeights[];
extern u16 Growth_PuffPictures[];
/* Each of Vine's roots, across from the middle of the targets. */
extern s8 Vine_ColumnOffsets[];
/* A thorn's kind in Thorn_Kinds. */
#define THORN_MIRRORED 1 /* drawn mirrored across */
#define THORN_SMALL	   2 /* 32 pixels wide, not 48 */
/* Thorn: each thorn's kind and root, the thorns by variant, and the seven */
/* pictures of a puff of dust. */
extern u8 Thorn_Kinds[];
extern s8 Thorn_X[];
extern u8 Thorn_Counts[];
extern s8 Thorn_PuffWidths[];
extern u8 Thorn_PuffHeights[];
extern u16 Thorn_PuffPictures[];
/* Bolt by variant (sparks, glints, their spread and bolts), the flips a */
/* glint may take, and the glints' blend mode by variant. */
extern u8 Bolt_Counts[][4];
extern u8 Bolt_GlintFlips[];
extern u8 Bolt_GlintModes[];
/* The volley's three comet sheets: Slash, Wind Slash and Sonic */
/* Slash fire the one their variant picks. */
typedef enum PROJECTILE_VOLLEY_COMET {
	PROJECTILE_VOLLEY_COMET_A, /* SlashCometSheet, Slash's */
	PROJECTILE_VOLLEY_COMET_B, /* WindSlashCometSheet, Wind Slash's */
	PROJECTILE_VOLLEY_COMET_C /* SonicSlashCometSheet, Sonic Slash's and Cutting
							   */
							  /* Edge's */
} PROJECTILE_VOLLEY_COMET;
/* Each comet's picture height. */
extern u8 ProjectileVolley_CometHeights[];
/* The projectile volley's modes (btEffectRunProjectileVolley), each named */
/* for the effect that fires it; the casting impacts fire two of them. */
typedef enum PROJECTILE_VOLLEY_MODE {
	PROJECTILE_VOLLEY_SLASH,		 /* comets, picked by the variant */
	PROJECTILE_VOLLEY_SHURIKEN,		 /* spinning shurikens */
	PROJECTILE_VOLLEY_CRAZY_VOICE,	 /* bouncing pellets */
	PROJECTILE_VOLLEY_WING_FLUTTER,	 /* yellow sparks */
	PROJECTILE_VOLLEY_WING_BEAT,	 /* small yellow sparks */
	PROJECTILE_VOLLEY_CUTTING_EDGE,	 /* one large comet in ice colours */
	PROJECTILE_VOLLEY_HEAT_WAVE,	 /* fireballs trailing embers */
	PROJECTILE_VOLLEY_SLEET,		 /* ice chips falling from high above */
	PROJECTILE_VOLLEY_IMPACT_COMETS, /* comets, after */
									 /* CASTING_IMPACT_UNUSED_COMETS */
	PROJECTILE_VOLLEY_BLIZZARD,		 /* scattered ice chips; also after */
									 /* CASTING_IMPACT_UNUSED_ICE_CHIPS */
	PROJECTILE_VOLLEY_ICE_BLESSING,	 /* ice chips streaming one a frame */
	PROJECTILE_VOLLEY_WING_STROKE /* large yellow sparks, spread up and down */
} PROJECTILE_VOLLEY_MODE;
/* Each projectile volley mode by its columns, the seven pictures of the hit */
/* flash (size, step, rise and offset), and the four ice chip pictures. */
extern u8 ProjectileVolley_Modes[][5];
extern u8 ProjectileVolley_FlashWidths[];
extern u8 ProjectileVolley_FlashHeights[];
extern u8 ProjectileVolley_FlashSteps[];
extern u8 ProjectileVolley_FlashRises[];
extern u16 ProjectileVolley_FlashPictures[];
extern u16 ProjectileVolley_PelletPictures[];
/* The frames on which the Mercury Djinni fires each of its five flames. */
extern u8 DjinnAttacks_LaunchFrames[];
/* Hail's four 8x8 ice chip pictures. */
extern u8 Hail_PictureWidths[];
extern u8 Hail_PictureHeights[];
extern u16 Hail_Pictures[];
/* Ground: the flips a crackle may take, and the portal's nine pictures (the */
/* portal, its white core and its crackles) by size, offset and place. */
extern u8 Ground_CrackleFlips[];
extern u8 Ground_PortalWidths[];
extern u8 Ground_PortalHeights[];
extern u16 Ground_PortalPictures[];
extern u8 Ground_PortalX[];
extern u8 Ground_PortalY[];
/* The colours a target flashes when Thor's bolt strikes it, and the flips of */
/* the crackles around Thor. */
extern u8 Thor_TargetPalettes[];
extern u8 Thor_CrackleFlips[];

/* Where each Spire shard starts, before its rock's offset across. */
enum {
	SPIRE_SHARD_X, /* canvas x */
	SPIRE_SHARD_Y  /* canvas y */
};
extern u8 Spire_ShardOffsets[][2];

/* Spire: each rock's offset across and the frame it appears, the rocks by */
/* variant, and its shards' fifteen pictures. */
extern s8 Spire_RockX[];
extern u8 Spire_DropFrames[];
extern u8 Spire_RockCounts[];
extern u8 Spire_ShardWidths[];
extern u8 Spire_ShardHeights[];
extern u16 Spire_ShardPictures[];
/* The flurry's modes (btEffectRunFlurry), its rows of */
/* Flurry_Styles: Gust and Flying Attack take one, two or three */
/* strikes as a double or triple blow raises their variant. */
typedef enum FLURRY_MODE {
	FLURRY_BEAR_CLAW,			 /* five strikes; also Ransack */
	FLURRY_BACTERIA_RUSH,		 /* seven strikes, in lime */
	FLURRY_FREEBITE_RUSH,		 /* nine strikes, in blue */
	FLURRY_FLYING_ATTACK,		 /* one strike, in embers */
	FLURRY_FLYING_ATTACK_DOUBLE, /* two */
	FLURRY_FLYING_ATTACK_TRIPLE, /* three */
	FLURRY_GUST,				 /* one strike, each filling the canvas */
	FLURRY_GUST_DOUBLE,			 /* two */
	FLURRY_GUST_TRIPLE			 /* three */
} FLURRY_MODE;
/* A flurry's arcs, its FLURRY_ARC_SHEET. */
typedef enum FLURRY_ARC {
	FLURRY_ARCS_AND_WAVES, /* blue arc A, with a wave for the third and */
						   /* fourth of every four strikes */
	FLURRY_ARCS			   /* blue arc B for every strike */
} FLURRY_ARC;
/* Whose palette a flurry takes, its FLURRY_PALETTE. */
typedef enum FLURRY_PAL {
	FLURRY_PALETTE_MARS_DJINNI, /* the Mars Djinni sprite sheet's */
	FLURRY_PALETTE_LIME,
	FLURRY_PALETTE_BLUE_ARC,
	FLURRY_PALETTE_EMBER /* the ember streak sprite sheet's */
} FLURRY_PAL;
extern u8 Flurry_Styles[][7];
/* Gaia's rising wall: its nine rows, the three flames at its foot, BG2's */
/* shift by side and variant, and the columns of the late rocks. */
extern u16 Gaia_RowPictures[];
extern u8 Gaia_RowWidths[];
extern u16 Gaia_FlamePictures[];
extern u8 Gaia_FlameWidths[];
extern u8 Gaia_FlameHeights[];
extern s8 Gaia_Bg2Shifts[][3];
extern u8 Gaia_LateRockColumns[];
/* The six pictures of the growing beam, and how far each reaches short of the
 */
/* user. */
extern u8 Beam_PictureWidths[];
extern u8 Beam_PictureHeights[];
extern u16 Beam_Pictures[];
extern u8 Beam_PictureReaches[];
/* The casting impacts: the flips a glint may take, where Mortal Danger draws */
/* the reaper's pictures, and the four ice chip pictures. */
extern u8 CastingImpact_GlintDrawFlags[];
extern u8 Reaper_PictureX[][7];
extern u8 Reaper_PictureY[];
extern u16 CastingImpact_IceChipPictures[];
/* The seven pictures of Helm Splitter's fire swirl, and where each sits from */
/* the user. */
extern u8 HelmSplitter_PictureWidths[];
extern u8 HelmSplitter_PictureHeights[];
extern u16 HelmSplitter_Pictures[];
extern s8 HelmSplitter_PictureBiasX[];
extern s8 HelmSplitter_PictureBiasY[];
/* Where the sprites of a summon's formation stand, in pixels, one axis at a */
/* time (btEffectPlaceFormationSprs). */
extern u8 SummonFormation_NereidX[];
extern u8 SummonFormation_NereidZ[];
extern u8 SummonFormation_CybeleX[];
extern u8 SummonFormation_CybeleZ[];
extern u8 SummonFormation_TiamatX[];
extern u8 SummonFormation_TiamatZ[];
/* The eight pictures of the impact flash. */
extern u8 ImpactBurst_PictureWidths[];
extern u8 ImpactBurst_PictureHeights[];
extern u16 ImpactBurst_Pictures[];
/* The starfield fall: the seven sparks that orbit the falling sprite, where */
/* the seven fireballs fall, and the blast sheet's seven flash pictures. */
extern s16 StarfieldFall_OrbitPoints[][3];
extern s16 StarfieldFall_FireballPoints[][2];
extern u16 StarfieldFall_FlashPictures[];
extern u16 StarfieldFall_FlashSizes[];
/* Where Ramses's sprites stand from the statue's corner, and the blast */
/* sheet's puff pictures. */
extern u8 Ramses_StatueColumns[];
extern u8 Ramses_StatueRows[];
extern u16 Ramses_PuffPictures[];
extern u16 Ramses_PuffSizes[];
/* Ragnarok's strike columns, three for each side, and the flash picture for */
/* each three frames at the end of a flash's life. */
extern u8 Ragnarok_StrikeColumns[][3];
extern u8 Ragnarok_FlashPictures[];
/* Titan Blade's fading flash pictures and its dust's gravity. */
extern u8 TitanBlade_FlashPictures[];
extern s32 TitanBlade_DustGravity[];
/* Judgment: its burst's seven quarter pictures, and the six shards thrown */
/* from it. */
extern u8 Judgment_BurstHeights[];
extern u16 Judgment_ShieldSprites[];
extern u16 Judgment_ShardPictures[];
extern u8 Judgment_ShardWidths[];
extern u8 Judgment_ShardHeights[];
/* Where Kirin's nine sprites stand, in pixels from its corner. */
extern u8 Kirin_Columns[];
extern u8 Kirin_Rows[];
/* Neptune's eight sprites, offsets in pixels scaled by 1.5 as placed, and its
 */
/* spray's gravity by particle index & 3. */
extern u8 Neptune_ObjectX[];
extern u8 Neptune_ObjectY[];
extern s32 Neptune_Gravity[];
/* The seven square pictures of Undead Sword's puffs. */
extern u16 UndeadSword_PuffPictures[];
extern u16 UndeadSword_PuffSizes[];

#endif
