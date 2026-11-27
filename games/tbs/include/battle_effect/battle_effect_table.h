#ifndef ALCHEMY_BATTLE_EFFECT_TABLE_H
#define ALCHEMY_BATTLE_EFFECT_TABLE_H

/* The tables the battle effects read (battle_effect_table.c), in its order:
   picture offsets, sizes, timings and counts by variant. A table read by
   columns is a two-dimensional array, its columns named by the enum above
   it. */

#include "types.h"
#include "graphics/render_placement.h"
#include "system.h"

/* The two kinds of target strike (TargetStrike_Styles). */
#define STRIKE_KIND_SLASH 0 /* sideways slashes */
/* rows of fangs above and below that close on it */
#define STRIKE_KIND_BITE  1

/* A target strike style's extras. each hit raises smoke puffs from the target's
   feet */
#define STRIKE_SMOKE  1
/* motes fly out of the target and back into the user */
#define STRIKE_ABSORB 2
#define STRIKE_GLINTS 4 /* glints sparkle around the target */
#define STRIKE_SPARKS 8 /* each hit scatters sparks */
/* one heavy hit: a big shake and the target recoils */
#define STRIKE_HEAVY  16

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

/* Where each picture begins in the sprite sheets many effects share: the ten
   motes and twenty flares of a glow sheet, the nine sparkles, the six flame
   puffs (size, drop and offset) and the six glints (offset and size). */
extern Uint16 BattleEffect_MotePictures[];
extern Uint16 BattleEffect_FlarePictures[];
extern Uint16 BattleEffect_SparklePictures[];
extern Uint8  BattleEffect_SparkleSizes[];
extern Uint8  BattleEffect_PuffArcPictureWidths[];
extern Uint8  BattleEffect_PuffArcPictureHeights[];
extern Uint8  BattleEffect_PuffArcPictureBiasY[];
extern Uint16 BattleEffect_PuffArcPictures[];
extern Uint16 BattleEffect_GlintPictures[];
extern Uint8  BattleEffect_GlintPictureWidths[];
extern Uint8  BattleEffect_GlintPictureHeights[];

/* Each Ice variant: its shards and how many frames it runs. */
enum {
	ICE_SHARD_COUNT, /* shards that fall */
	ICE_FRAMES		 /* the effect's frames; the damage shows 16 before */
};

extern Uint8 Ice_Counts[][2];

/* Each Douse variant: its drops, the droplets each splash throws, the spacing
   and the frames. */
enum {
	DOUSE_DROP_COUNT,		 /* drops that fall, of the 64 placed */
	DOUSE_DROPLETS_PER_DROP, /* droplets each landing drop throws up */
	DOUSE_DROP_SPACING,		 /* pixels between drops along the slant */
	DOUSE_FRAMES /* the effect's frames; the damage shows 64 before */
};

extern Uint8 Douse_Counts[][4];

/* Each streak style's effect, hit, motion and shake, and the pixels a struck
   target trembles by, frame by frame. */
extern Sint8 Streak_Styles[][4];
extern Sint8 Streak_Tremble[];
/* The target strike: its twelve styles by the TARGET_STRIKE_ columns, the six
   slash pictures (size, offset and reach), the fang stage of each picture of a
   bite, and the flips a glint may take. */
extern Uint8  TargetStrike_Styles[][7];
extern Uint8  TargetStrike_Widths[];
extern Uint8  TargetStrike_Heights[];
extern Uint16 TargetStrike_Pictures[];
extern Uint8  TargetStrike_Reach[];
extern Uint8  TargetStrike_BiteStages[];
extern Uint8  TargetStrike_GlintDrawFlags[];
/* The blade rain's four sword pictures, by how steeply the sword falls. */
extern Uint8  BladeRain_SwordWidths[];
extern Uint8  BladeRain_SwordHeights[];
extern Uint16 BladeRain_SwordPictures[];
/* The 33 pieces the shattering opening's picture breaks into: size, offset in
   the picture and where each starts on the canvas. */
extern Uint8  ShatteringOpening_PieceWidths[];
extern Uint8  ShatteringOpening_PieceHeights[];
extern Uint16 ShatteringOpening_PiecePictures[];
extern Uint8  ShatteringOpening_PieceStartX[];
extern Uint8  ShatteringOpening_PieceStartY[];
/* The element burst's four corner sparks: offsets and flips. */
extern Sint8 ElementBurst_SparkX[];
extern Sint8 ElementBurst_SparkY[];
extern Uint8 ElementBurst_SparkFlips[];
/* The swirl swipe's six pictures (Spider Web): size, offset, an x for each side
   and a y offset. */
extern Uint8  SwirlSwipe_PictureWidths[];
extern Uint8  SwirlSwipe_PictureHeights[];
extern Uint16 SwirlSwipe_Pictures[];
extern Uint8  SwirlSwipe_PictureX[];
extern Sint8  SwirlSwipe_PictureBiasY[];

/* Each gathering-lights mode: the lights per target and the frames before the
   targets' turns. */
enum {
	GATHERING_LIGHTS_COUNT, /* lights for each target */
	GATHERING_LIGHTS_FRAMES /* frames before the targets' turns */
};

/* The gathering lights' modes (btEffectRunGatheringLights), each named for the
   effect that plays it, and its row of GatheringLights_Counts. */
typedef enum GATHERING_LIGHTS_MODE {
	GATHERING_LIGHTS_DEMON_NIGHT, /* pink */
	GATHERING_LIGHTS_RUMBLE,	  /* Mars Djinn colours */
	GATHERING_LIGHTS_HAUNTING	  /* Mercury Djinn colours */
} GATHERING_LIGHTS_MODE;

extern Uint8 GatheringLights_Counts[][2];

/* The earth wall's three pictures, as the mirrored wall reads them. */
extern Uint16 MirroredWall_Pictures[];
extern Uint8  MirroredWall_PictureWidths[];
extern Uint8  MirroredWall_PictureHeights[];
/* Revive's beams: the mote picture of each step of its eight-step cycle, and
   the three pictures of the glow at a target's feet. */
extern Uint8  DjinnBeams_MotePictures[];
extern Uint16 DjinnBeams_HeadPictures[];
extern Uint8  DjinnBeams_HeadWidths[];
extern Uint8  DjinnBeams_HeadHeights[];

/* Each lobbed shells mode's style (LOBBED_SHELLS_MODE,
   battle_effect_lobbed_shells.c). */
enum {
	/* nonzero: the shells burst into shards, not smoke */
	LOBBED_SHELLS_STYLE_SHATTER,
	LOBBED_SHELLS_STYLE_UNUSED /* 0 in every row; nothing reads it */
};

extern Sint8 LobbedShells_Styles[][2];
/* The eighteen pictures of the lobbed shells' tumbling shards, a large set of
   nine and a small one. */
extern Uint8  LobbedShells_ShardWidths[];
extern Uint8  LobbedShells_ShardHeights[];
extern Uint16 LobbedShells_ShardPictures[];
/* Where Condemn draws the reaper's seven pictures: an x for each casting side,
   and a y. */
extern Uint8 Condemn_PictureX[][7];
extern Uint8 Condemn_PictureY[][7];
/* The first vertex of Bind's spinning and striking triangles, 24 pixels up from
   the centre. */
extern VECTOR3 Bind_Vertex[];
extern VECTOR3 Bind_TriangleStrikeVertex[];
/* The stars' two points, 48 and 28 pixels up, and their radii. */
extern VECTOR3 Star_Points[];
extern Uint8   Star_Radii[];
/* Procne's nine sprites on a 3x3 grid, the frames of its four strikes, and its
   three comet pictures (size, lift and offset). */
extern Uint8  Procne_GridX[];
extern Uint8  Procne_GridY[];
extern Uint16 Procne_Frames[];
extern Uint8  Procne_PictureWidths[];
extern Uint8  Procne_PictureHeights[];
extern Uint8  Procne_PictureBiasY[];
extern Uint16 Procne_Pictures[];
/* Tiamat's embers (speed and size by ember index & 3), its sparks' 1x2 picture,
   and the six flame pictures (size, drop and offset). */
extern Uint8  Tiamat_EmberSpeeds[];
extern Uint8  Tiamat_SparkPixels[];
extern Uint8  Tiamat_EmberSizes[];
extern Uint8  Tiamat_FlameWidths[];
extern Uint8  Tiamat_FlameHeights[];
extern Uint8  Tiamat_FlameDrops[];
extern Uint16 Tiamat_FlamePictures[];

/* The foot of each of Nereid's columns of fire. */
enum {
	NEREID_COLUMN_X, /* canvas x of the column's middle */
	NEREID_COLUMN_Y	 /* canvas y of its foot */
};

extern Uint8 Nereid_Columns[][2];

/* A falling drop's gravity in 16.16 pixels a frame each frame, by drop index &
   3. */
extern Sint32 Nereid_Gravity[];

/* Each Froth variant: its motes and frames. */
enum {
	FROTH_BUBBLE_COUNT, /* bubbles, at most 30 (battle_effect_vortices.c) */
	FROTH_FRAMES		/* the effect's frames; the damage shows 16 before */
};

extern Uint8 Vortices_Counts[][2];

/* Each Whirlwind variant: its tornadoes, their sway and the screen shake. */
enum {
	WHIRLWIND_TORNADOES, /* tornadoes drawn */
	WHIRLWIND_SWAY,		 /* how far each sways across, in pixels */
	WHIRLWIND_SHAKE		 /* btCameraShake's range */
};

extern Uint8 Vortices_Shapes[][3];

/* Where each Prism shard starts in the crystal. */
enum {
	PRISM_SHARD_START_X, /* pixels from the crystal's left */
	PRISM_SHARD_START_Y	 /* pixels from its top */
};

extern Uint8 Prism_ShardStarts[][2];

/* Each Prism variant: its crystals and frames. */
enum {
	PRISM_CRYSTAL_COUNT, /* crystals drawn */
	PRISM_FRAMES		 /* the effect's frames */
};

extern Uint8 Prism_Counts[][2];

/* The twelve pictures of Prism's shards. */
extern Uint8  Prism_ShardWidths[];
extern Uint8  Prism_ShardHeights[];
extern Sint32 Prism_ShardPictures[];

/* Each Plasma variant: the sparks per pillar and the frames of shake. */
enum {
	PLASMA_SPARKS,		/* sparks each lightning pillar throws */
	PLASMA_SHAKE_FRAMES /* frames the screen shakes at each strike */
};

extern Uint16 Plasma_Sparks[][2];

/* Each Plasma pillar's canvas x, seven for each side, and how many strike by
   variant. */
extern Uint8 Plasma_Columns[][7];
extern Uint8 Plasma_Counts[];

/* Each spark group variant: sparks per burst, bursts, and from
   SPARK_GROUPS_OFFSET each burst's offset across. */
enum {
	SPARK_GROUPS_SPARKS, /* sparks each burst throws */
	SPARK_GROUPS_BURSTS, /* bursts, 8 frames apart */
	SPARK_GROUPS_OFFSET	 /* burst i's offset across, in pixels, at [2 + i] */
};

/* The spark groups' variants past the three strengths of Blast and Dynamite
   (EFFECT_VARIANT), which their callers set before the run. */
enum {
	SPARK_GROUPS_SEVERE_BLOW = 3, /* 340 sparks and a flash: Severe Blow's */
	/* impact and btEffectRunUnusedSevereBlowLeap */
	SPARK_GROUPS_THRASH /* 32 sparks: Thrash */
};

extern Uint16 SparkGroups_Shapes[][5];

/* The flash picture for each three frames at the end of a flash's life. */
extern Uint8 Plasma_FlashPictures[];
/* Volcano's fire pillars by variant: how many stand, the depth of each and the
   frame each starts. */
extern Uint8 Volcano_Counts[];
extern Sint8 Volcano_Depths[][4];
extern Uint8 Volcano_StartFrames[];
/* The flips a glint around a target may take. */
extern Uint8 TargetMotes_GlintFlips[];

/* Cybele's face as she spits the thorns: the animations of sprites 3 and 4, the
   left and right halves of the middle row of SUMMON_FORMATION_CYBELE, as the
   cast's recording shows them. Her mouth, in the left half, opens and shuts;
   her far eye, in the right, stays as it is. */
enum {
	CYBELE_POSE_FACE_LEFT, /* sprite 3's animation: the half with her mouth */
	CYBELE_POSE_FACE_RIGHT /* sprite 4's animation */
};

extern Uint8 Cybele_ObjectPoses[][2];

/* Cybele's pictures, each by offset and size: the five thorns, the three
   flowers, the five chips (the thorns again), the five of a spark and the seven
   of the blast sheet, of which she draws only the last. */
extern Uint16 Cybele_ThornPictures[];
extern Uint8  Cybele_ThornWidths[];
extern Uint8  Cybele_ThornHeights[];
extern Uint16 Cybele_FlowerPictures[];
extern Uint8  Cybele_FlowerSizes[];
extern Uint16 Cybele_ChipPictures[];
extern Uint8  Cybele_ChipWidths[];
extern Uint8  Cybele_ChipHeights[];
extern Uint16 Cybele_SparkPictures[];
extern Uint8  Cybele_SparkSizes[];
extern Uint16 Cybele_BlastPictures[];
extern Uint16 Cybele_BlastSizes[];
/* Boreas: where the crystals stand, the background's jolt as the ice block
   lands, its third sprite's animations as the block sinks, and the pictures of
   the crystals, the shards thrown as they crumble, the spray, the crystals that
   rise among the targets and those that fall. */
extern Uint8  Boreas_Points[][2];
extern Sint8  Boreas_ShakeOffsets[];
extern Uint16 Boreas_CrystalPictures[];
extern Uint8  Boreas_CrystalWidths[];
extern Uint8  Boreas_CrystalHeights[];
extern Uint8  Boreas_Animations[];
extern Uint8  Boreas_DebrisWidths[];
extern Uint8  Boreas_DebrisHeights[];
extern Uint16 Boreas_DebrisPictures[];
extern Uint8  Boreas_SprayWidths[];
extern Uint8  Boreas_SprayHeights[];
extern Uint16 Boreas_SprayPictures[];
extern Uint16 Boreas_RisingCrystalPictures[];
extern Uint8  Boreas_RisingCrystalWidths[];
extern Uint8  Boreas_RisingCrystalHeights[];
extern Uint8  Boreas_FallWidths[];
extern Uint8  Boreas_FallHeights[];
extern Uint16 Boreas_FallPictures[];
/* The gravity of Drain's ice shards, by shard index & 3. */
extern Sint32 Drain_Gravities[];
/* The twelve pictures of the stat changes' shards: four shapes, three frames
   each. */
extern Uint16 StatChange_ShardPictures[];
extern Uint8  StatChange_ShardWidths[];
extern Uint8  StatChange_ShardHeights[];
/* Each Quake variant: the effect's frames, each pillar's and the shake. */
extern Uint8 Quake_Timings[][3];
/* Fire's embers by variant, and the six pictures of its impact flare. */
extern Uint8  Fire_Counts[];
extern Uint8  Fire_FlareWidths[];
extern Uint8  Fire_FlareHeights[];
extern Uint8  Fire_FlareBiasY[];
extern Uint16 Fire_FlarePictures[];

/* Where each ice mound of Frost stands: x and the y of its foot. */
enum {
	FROST_MOUND_X,	 /* canvas x of the mound's left edge */
	FROST_MOUND_FOOT /* canvas y of its foot */
};

extern Uint8 Frost_MoundSpots[][2];

/* Frost: its motes, mounds and burst frame by variant, and the pictures of its
   motes, its three ice mounds and its chips (the motes again). */
extern Uint8  Frost_Timings[][3];
extern Uint8  Frost_MoteWidths[];
extern Uint8  Frost_MoteHeights[];
extern Uint16 Frost_MotePictures[];
extern Uint16 Frost_MoundPictures[];
extern Uint8  Frost_MoundHeights[];
extern Uint8  Frost_ChipWidths[];
extern Uint8  Frost_ChipHeights[];
extern Uint16 Frost_ChipPictures[];
/* Ray's sparks by variant; [1] is how many each bolt throws a frame. */
extern Uint8 Ray_Sparks[][2];
/* Eruption's motes, puffs and frames by variant, and its seven puff
   pictures. */
extern Uint8  Eruption_Counts[][3];
extern Uint16 Eruption_PuffPictures[];
extern Uint16 Eruption_PuffSizes[];
/* Unused: picture layouts nothing reads, from 6x6 to 20x20. */
extern Uint16 Unused_SheetPicturesShort[];
extern Uint16 Unused_SheetPictures[];
extern Uint8  Unused_SheetWidths[];
extern Uint8  Unused_SheetHeights[];
/* The scale Atalanta's sprites keep throughout, one to one. */
extern SPR_SCALE Atalanta_UnitScale;
/* Growth's columns: the three shapes' vine and pillar pictures and height caps,
   the columns by variant, their roots and the pillars' heights, and the seven
   pictures of a puff of dust. */
extern Uint8  Growth_VineWidths[];
extern Uint8  Growth_HeightLimits[];
extern Uint16 Growth_VinePictures[];
extern Uint8  Growth_PillarWidths[];
extern Uint16 Growth_PillarPictures[];
extern Uint8  Growth_ColumnCounts[];
extern Uint8  Growth_ColumnX[];
extern Sint8  Growth_PillarHeights[];
extern Sint8  Growth_PuffWidths[];
extern Uint8  Growth_PuffHeights[];
extern Uint16 Growth_PuffPictures[];
/* Each of Vine's roots, across from the middle of the targets. */
extern Sint8 Vine_ColumnOffsets[];
/* A thorn's kind in Thorn_Kinds. */
#define THORN_MIRRORED 1 /* drawn mirrored across */
#define THORN_SMALL	   2 /* 32 pixels wide, not 48 */
/* Thorn: each thorn's kind and root, the thorns by variant, and the seven
   pictures of a puff of dust. */
extern Uint8  Thorn_Kinds[];
extern Sint8  Thorn_X[];
extern Uint8  Thorn_Counts[];
extern Sint8  Thorn_PuffWidths[];
extern Uint8  Thorn_PuffHeights[];
extern Uint16 Thorn_PuffPictures[];
/* Bolt by variant (sparks, glints, their spread and bolts), the flips a glint
   may take, and the glints' blend mode by variant. */
extern Uint8 Bolt_Counts[][4];
extern Uint8 Bolt_GlintFlips[];
extern Uint8 Bolt_GlintModes[];

/* The volley's three comet sheets: Slash, Wind Slash and Sonic */
/* Slash fire the one their variant picks. */
typedef enum PROJECTILE_VOLLEY_COMET {
	PROJECTILE_VOLLEY_COMET_A, /* SlashCometSheet, Slash's */
	PROJECTILE_VOLLEY_COMET_B, /* WindSlashCometSheet, Wind Slash's */
	/* SonicSlashCometSheet, Sonic Slash's and Cutting Edge's */
	PROJECTILE_VOLLEY_COMET_C
} PROJECTILE_VOLLEY_COMET;

/* Each comet's picture height. */
extern Uint8 ProjectileVolley_CometHeights[];

/* The projectile volley's modes (btEffectRunProjectileVolley), each named for
   the effect that fires it; the casting impacts fire two of them. */
typedef enum PROJECTILE_VOLLEY_MODE {
	PROJECTILE_VOLLEY_SLASH,		/* comets, picked by the variant */
	PROJECTILE_VOLLEY_SHURIKEN,		/* spinning shurikens */
	PROJECTILE_VOLLEY_CRAZY_VOICE,	/* bouncing pellets */
	PROJECTILE_VOLLEY_WING_FLUTTER, /* yellow sparks */
	PROJECTILE_VOLLEY_WING_BEAT,	/* small yellow sparks */
	PROJECTILE_VOLLEY_CUTTING_EDGE, /* one large comet in ice colours */
	PROJECTILE_VOLLEY_HEAT_WAVE,	/* fireballs trailing embers */
	PROJECTILE_VOLLEY_SLEET,		/* ice chips falling from high above */
	/* comets, after CASTING_IMPACT_UNUSED_COMETS */
	PROJECTILE_VOLLEY_IMPACT_COMETS,
	/* scattered ice chips; also after CASTING_IMPACT_UNUSED_ICE_CHIPS */
	PROJECTILE_VOLLEY_BLIZZARD,
	PROJECTILE_VOLLEY_ICE_BLESSING, /* ice chips streaming one a frame */
	PROJECTILE_VOLLEY_WING_STROKE /* large yellow sparks, spread up and down */
} PROJECTILE_VOLLEY_MODE;

/* Each projectile volley mode by its columns, the seven pictures of the hit
   flash (size, step, rise and offset), and the four ice chip pictures. */
extern Uint8  ProjectileVolley_Modes[][5];
extern Uint8  ProjectileVolley_FlashWidths[];
extern Uint8  ProjectileVolley_FlashHeights[];
extern Uint8  ProjectileVolley_FlashSteps[];
extern Uint8  ProjectileVolley_FlashRises[];
extern Uint16 ProjectileVolley_FlashPictures[];
extern Uint16 ProjectileVolley_PelletPictures[];
/* The frames on which the Mercury Djinni fires each of its five flames. */
extern Uint8 DjinnAttacks_LaunchFrames[];
/* Hail's four 8x8 ice chip pictures. */
extern Uint8  Hail_PictureWidths[];
extern Uint8  Hail_PictureHeights[];
extern Uint16 Hail_Pictures[];
/* Ground: the flips a crackle may take, and the portal's nine pictures (the
   portal, its white core and its crackles) by size, offset and place. */
extern Uint8  Ground_CrackleFlips[];
extern Uint8  Ground_PortalWidths[];
extern Uint8  Ground_PortalHeights[];
extern Uint16 Ground_PortalPictures[];
extern Uint8  Ground_PortalX[];
extern Uint8  Ground_PortalY[];
/* The colours a target flashes when Thor's bolt strikes it, and the flips of
   the crackles around Thor. */
extern Uint8 Thor_TargetPalettes[];
extern Uint8 Thor_CrackleFlips[];

/* Where each Spire shard starts, before its rock's offset across. */
enum {
	SPIRE_SHARD_X, /* canvas x */
	SPIRE_SHARD_Y  /* canvas y */
};

extern Uint8 Spire_ShardOffsets[][2];

/* Spire: each rock's offset across and the frame it appears, the rocks by
   variant, and its shards' fifteen pictures. */
extern Sint8  Spire_RockX[];
extern Uint8  Spire_DropFrames[];
extern Uint8  Spire_RockCounts[];
extern Uint8  Spire_ShardWidths[];
extern Uint8  Spire_ShardHeights[];
extern Uint16 Spire_ShardPictures[];

/* The flurry's modes (btEffectRunFlurry), its rows of Flurry_Styles: Gust and
   Flying Attack take one, two or three strikes as a double or triple blow
   raises their variant. */
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
	/* blue arc A, with a wave for the third and fourth of every four strikes */
	FLURRY_ARCS_AND_WAVES,
	FLURRY_ARCS /* blue arc B for every strike */
} FLURRY_ARC;

/* Whose palette a flurry takes, its FLURRY_PALETTE. */
typedef enum FLURRY_PAL {
	FLURRY_PALETTE_MARS_DJINNI, /* the Mars Djinni sprite sheet's */
	FLURRY_PALETTE_LIME,
	FLURRY_PALETTE_BLUE_ARC,
	FLURRY_PALETTE_EMBER /* the ember streak sprite sheet's */
} FLURRY_PAL;

extern Uint8 Flurry_Styles[][7];
/* Gaia's rising wall: its nine rows, the three flames at its foot, BG2's shift
   by side and variant, and the columns of the late rocks. */
extern Uint16 Gaia_RowPictures[];
extern Uint8  Gaia_RowWidths[];
extern Uint16 Gaia_FlamePictures[];
extern Uint8  Gaia_FlameWidths[];
extern Uint8  Gaia_FlameHeights[];
extern Sint8  Gaia_Bg2Shifts[][3];
extern Uint8  Gaia_LateRockColumns[];
/* The six pictures of the growing beam, and how far each reaches short of the
   user. */
extern Uint8  Beam_PictureWidths[];
extern Uint8  Beam_PictureHeights[];
extern Uint16 Beam_Pictures[];
extern Uint8  Beam_PictureReaches[];
/* The casting impacts: the flips a glint may take, where Mortal Danger draws
   the reaper's pictures, and the four ice chip pictures. */
extern Uint8  CastingImpact_GlintDrawFlags[];
extern Uint8  Reaper_PictureX[][7];
extern Uint8  Reaper_PictureY[];
extern Uint16 CastingImpact_IceChipPictures[];
/* The seven pictures of Helm Splitter's fire swirl, and where each sits from
   the user. */
extern Uint8  HelmSplitter_PictureWidths[];
extern Uint8  HelmSplitter_PictureHeights[];
extern Uint16 HelmSplitter_Pictures[];
extern Sint8  HelmSplitter_PictureBiasX[];
extern Sint8  HelmSplitter_PictureBiasY[];
/* Where the sprites of a summon's formation stand, in pixels, one axis at a
   time (btEffectPlaceFormationSprs). */
extern Uint8 SummonFormation_NereidX[];
extern Uint8 SummonFormation_NereidZ[];
extern Uint8 SummonFormation_CybeleX[];
extern Uint8 SummonFormation_CybeleZ[];
extern Uint8 SummonFormation_TiamatX[];
extern Uint8 SummonFormation_TiamatZ[];
/* The eight pictures of the impact flash. */
extern Uint8  ImpactBurst_PictureWidths[];
extern Uint8  ImpactBurst_PictureHeights[];
extern Uint16 ImpactBurst_Pictures[];
/* The starfield fall: the seven sparks that orbit the falling sprite, where the
   seven fireballs fall, and the blast sheet's seven flash pictures. */
extern Sint16 StarfieldFall_OrbitPoints[][3];
extern Sint16 StarfieldFall_FireballPoints[][2];
extern Uint16 StarfieldFall_FlashPictures[];
extern Uint16 StarfieldFall_FlashSizes[];
/* Where Ramses's sprites stand from the statue's corner, and the blast sheet's
   puff pictures. */
extern Uint8  Ramses_StatueColumns[];
extern Uint8  Ramses_StatueRows[];
extern Uint16 Ramses_PuffPictures[];
extern Uint16 Ramses_PuffSizes[];
/* Ragnarok's strike columns, three for each side, and the flash picture for
   each three frames at the end of a flash's life. */
extern Uint8 Ragnarok_StrikeColumns[][3];
extern Uint8 Ragnarok_FlashPictures[];
/* Titan Blade's fading flash pictures and its dust's gravity. */
extern Uint8  TitanBlade_FlashPictures[];
extern Sint32 TitanBlade_DustGravity[];
/* Judgment: its burst's seven quarter pictures, and the six shards thrown from
   it. */
extern Uint8  Judgment_BurstHeights[];
extern Uint16 Judgment_ShieldSprites[];
extern Uint16 Judgment_ShardPictures[];
extern Uint8  Judgment_ShardWidths[];
extern Uint8  Judgment_ShardHeights[];
/* Where Kirin's nine sprites stand, in pixels from its corner. */
extern Uint8 Kirin_Columns[];
extern Uint8 Kirin_Rows[];
/* Neptune's eight sprites, offsets in pixels scaled by 1.5 as placed, and its
   spray's gravity by particle index & 3. */
extern Uint8  Neptune_ObjectX[];
extern Uint8  Neptune_ObjectY[];
extern Sint32 Neptune_Gravity[];
/* The seven square pictures of Undead Sword's puffs. */
extern Uint16 UndeadSword_PuffPictures[];
extern Uint16 UndeadSword_PuffSizes[];

#endif
