/* @file */
/* Battle effect tables, in the order of the effect code that reads them. Each
 */
/* is read only by the effect its prefix names, except the BattleEffect_ */
/* tables, which many effects share. Picture offsets count bytes from the start
 */
/* of a sheet; widths and heights are pixels; corners are signed x, y and z */
/* bytes. */

#include "types.h"
#include "fixed_math.h"
#include "battle/battle_effect_state.h"
#include "battle/render_model.h"
#include "battle_effect/battle_effect.h"
#include "battle_effect/battle_effect_table.h"

/* One triangle of a textured mesh, as the textured renderers read it: its */
/* three corners, by number in the model's corner list, and the texture point */
/* (u across, v down, in texels) each corner shows. */
#define MESH_FACE(a, b, c, ua, va, ub, vb, uc, vc)                             \
	(a) | (b) << 8, (c), (ua) | (va) << 8, (ub) | (vb) << 8, (uc) | (vc) << 8, 0
/* A face of no corners ends a textured mesh. */
#define MESH_END 0, 0, 0, 0, 0, 0

/* A row of LobbedShells_Styles, the four bytes battle_effect_djinn_beams.c */
/* reads through its LOBBED_SHELLS_ macros: whether the shells burst into */
/* shards rather than smoke; the blitter they are drawn with, 0 keeping the */
/* brighter level and 1 adding; the shells lobbed at each target; and the */
/* frames the effect runs past the targets' share. */
#define LOBBED_SHELLS_STYLE(shatter, blitter, shells, frames)                  \
	(shatter), (blitter), (shells), (frames)

/* Each mote size's picture: its byte offset in the particle sheet, by size. */
const u16 BattleEffect_MotePictures[] = {0,	  2,   10,	28,	 60,
										 110, 182, 280, 408, 570};

/* Each flare size's picture: its byte offset in the spark sprite sheet, by */
/* size less one. */
const u16 BattleEffect_FlarePictures[] = {0,   4,	20,	 56,  120,
										  220, 364, 560, 816, 1140};

/* Each circle picture's byte offset in Cruel Ruin's sprite sheet. */
const u16 CruelRuin_CirclePictures[] = {0,	 8,	  40,	112,  240,
										440, 728, 1120, 1632, 2280};

/* Each sparkle picture's byte offset in the sprite sheet */
/* (BattleEffect_SparkleSizes gives its side). */
const u16 BattleEffect_SparklePictures[] = {0,	 25,  74,  123, 172,
											221, 246, 255, 256};

/* Each sparkle picture's side, in pixels. */
const u8 BattleEffect_SparkleSizes[] = {5, 7, 7, 7, 7, 5, 3, 1, 1};

/* EmberStreakSheet's six pictures, a puff of flame drawing out into a trail of
 */
/* embers as it rises: each one's width, in pixels. */
const u8 BattleEffect_EmberStreakPictureWidths[] = {24, 29, 30, 27, 24, 16};

/* Each ember streak picture's height, in pixels. */
const u8 BattleEffect_EmberStreakPictureHeights[] = {23, 32, 37, 49, 53, 52};

/* How far below its anchor each ember streak picture's top goes, in pixels: */
/* the bottoms stay within ten pixels of 64 below. */
const u8 BattleEffect_EmberStreakPictureBiasY[] = {41, 32, 26, 11, 4, 2};

/* Each ember streak picture's byte offset in the sheet. */
const u16 BattleEffect_EmberStreakPictures[] = {0, 552, 1480, 2590, 3913, 5185};

/* Each glint picture's byte offset in the sprite sheet. */
const u16 BattleEffect_GlintPictures[] = {0, 589, 1302, 2015, 2414, 2750};

/* Each glint picture's width, in pixels. */
const u8 BattleEffect_GlintPictureWidths[] = {19, 23, 23, 19, 16, 17};

/* Each glint picture's height, in pixels. */
const u8 BattleEffect_GlintPictureHeights[] = {31, 31, 31, 21, 21, 24};

/* Each shard picture's width, in pixels. */
const u8 Spire_ShardWidths[] = {5,	11, 10, 7,	7,	12, 13, 9,
								19, 12, 17, 22, 15, 16, 14};

/* Each shard picture's height, in pixels. */
const u8 Spire_ShardHeights[] = {11, 6,	 10, 11, 10, 7,	 17, 21,
								 12, 21, 17, 13, 16, 16, 15};

/* Each shard picture's byte offset in Spire's sprite sheet. */
const u16 Spire_ShardPictures[] = {0,	 55,   121,	 221,  298,	 368,
								   452,	 673,  862,	 1090, 1342, 1631,
								   1917, 2157, 2413, 2623};

/* Each shard picture's byte offset in a shade of the stat change's sprite */
/* sheet. */
const u16 StatChange_ShardPictures[] = {0,	 120, 180, 258, 298, 352,
										408, 471, 567, 607, 631, 661};

/* Each shard picture's width, in pixels. */
const u8 StatChange_ShardWidths[] = {10, 5, 6, 5, 6, 7, 9, 8, 4, 6, 6, 7};

/* Each shard picture's height, in pixels. */
const u8 StatChange_ShardHeights[] = {12, 12, 13, 8, 9, 8, 7, 12, 10, 4, 5, 5};

/* The reaper's seven panels: their x in the canvas, in pixels, seven for each
 */
/* side. */
const u8 Reaper_PictureX[] = {10, 30, 0, 0, 0, 6, 52, 60, 0, 0, 0, 0, 0, 0};

/* The reaper's seven panels: their y in the canvas, in pixels. */
const u8 Reaper_PictureY[] = {0, 20, 2, 2, 34, 64, 68};

/* Ice's shards and frames, a pair for each variant (ICE_SHARDS, ICE_FRAMES). */
const u8 Ice_Counts[] = {16, 46, 20, 52, 32, 80};

/* Douse's drops, four bytes for each variant: the drops that fall, the */
/* droplets each splashes into, their spacing in pixels and the frames */
/* (battle_effect_falling.c's DOUSE_ macros). */
const u8 Douse_Counts[] = {6, 8, 32, 80, 8, 16, 24, 80, 32, 4, 8, 80};

/* The streak styles, four flags for each (STREAK_ANIMATE, STREAK_HIT, */
/* STREAK_MOTION, STREAK_SHAKE). */
const s8 Streak_Styles[] = {0,	1, 0, 0, 0,	 1, 4, 0, 0, 1,
							-1, 1, 1, 1, -1, 1, 1, 1, 4, 0};

/* The struck target's sideways tremble, in pixels, one a frame. */
const s8 Streak_Tremble[] = {-1, -1, 1, 1, 1, 1, -1, -1};

/* The target strike styles, one row a style (TARGET_STRIKE_STYLE), one byte
 */
/* a column (TARGET_STRIKE_COLUMN). */
const u8 TargetStrike_Styles[][7] = {
	{1, 2, 8, 0, 6, 0, 70},	 {1, 1, 1, 1, 6, 1, 54},  {1, 3, 1, 2, 6, 1, 96},
	{1, 3, 2, 3, 4, 1, 96},	 {1, 3, 2, 4, 4, 1, 96},  {1, 3, 4, 5, 4, 0, 60},
	{0, 1, 4, 5, 4, 0, 38},	 {0, 2, 8, 5, 2, 0, 48},  {0, 1, 24, 4, 2, 0, 50},
	{0, 1, 1, 1, 2, 0, 46},	 {1, 1, 1, 4, 6, 1, 54},  {1, 3, 2, 4, 4, 0, 96},
	{0, 4, 8, 5, 2, 1, 64},	 {0, 8, 8, 5, 1, 0, 64},  {0, 1, 44, 6, 2, 0, 50},
	{1, 3, 2, 4, 4, 0, 96},	 {0, 1, 40, 6, 2, 1, 50}, {1, 1, 24, 4, 6, 0, 50},
	{1, 1, 40, 6, 2, 1, 50}, {0, 1, 64, 4, 2, 0, 22}, {1, 1, 8, 0, 6, 0, 70}};

/* Each slash picture's width, in pixels. */
const u8 TargetStrike_Widths[] = {38, 54, 71, 74, 77, 65};

/* Each slash picture's height, in pixels. */
const u8 TargetStrike_Heights[] = {7, 9, 11, 13, 13, 11};

/* Each Target Strike picture's byte offset in Target Strike's sprite sheet. */
const u16 TargetStrike_Pictures[] = {3200, 3466, 3952, 4733, 5695, 6696};

/* How far each slash picture reaches past its middle, in pixels. */
const u8 TargetStrike_Reach[] = {58, 42, 25, 13, 5, 0};

/* Which stage of the closing fangs each bite picture shows. */
const u8 TargetStrike_BiteStages[] = {0, 1, 2, 3, 2};

/* The flips a glint may be drawn with, one picked at random. */
const u8 TargetStrike_GlintDrawFlags[] = {0, 4, 8, 12};

/* The 33 pieces of the shattered screen: each one's width; the tables after */
/* it give their heights, places in the sprite sheet and starting points. */
const u8 ShatteringOpening_PieceWidths[] = {
	21, 20, 24, 30, 35, 69, 19, 23, 9, 22, 33, 10, 18, 9, 7,  9, 17,
	55, 30, 25, 40, 29, 10, 28, 14, 5, 32, 23, 19, 32, 8, 43, 15};

/* Each piece picture's height, in pixels. */
const u8 ShatteringOpening_PieceHeights[] = {
	23, 47, 25, 40, 20, 53, 28, 29, 23, 14, 35, 17, 18, 14, 15, 12, 13,
	37, 38, 41, 60, 41, 15, 45, 59, 41, 26, 50, 21, 35, 12, 57, 11, 0};

/* Each piece picture's byte offset in Death Plunge's sprite sheet. */
const u16 ShatteringOpening_PiecePictures[] = {
	0,	   483,	  1423,	 2023,	3223,  3923,  7580,	 8112,	8779,
	8986,  9294,  10449, 10619, 10943, 11069, 11174, 11282, 11503,
	13538, 14678, 15703, 18103, 19292, 19442, 20702, 21528, 21733,
	22565, 23715, 24114, 25234, 25330, 27781, 27946};

/* Each piece's starting x, in pixels. */
const u8 ShatteringOpening_PieceStartX[] = {
	0,	21, 28, 46, 70, 51, 0,	13, 37, 77, 87, 19, 25, 35, 44, 45, 48,
	65, 0,	0,	0,	5,	33, 15, 36, 46, 47, 50, 53, 65, 72, 77, 105};

/* Each piece's starting y, in pixels. */
const u8 ShatteringOpening_PieceStartY[] = {
	0,	0,	0,	0,	0,	0,	19, 17, 23, 28, 8,	36, 43, 43,	 40, 47, 48,
	37, 42, 67, 60, 79, 62, 75, 61, 62, 60, 70, 74, 85, 108, 63, 74};

/* The four corner sparks: their x about the target, in pixels. */
const s8 ElementBurst_SparkX[] = {0, 32, 0, 32};

/* The four corner sparks: their y about the target, in pixels. */
const s8 ElementBurst_SparkY[] = {0, 0, 32, 32};

/* The four corner sparks' flips. */
const u8 ElementBurst_GlintFlips[] = {0, 4, 8, 12};

/* The streak's corners, as signed x, y and z. */
const MODEL_VERTEX CruelRuin_StreakCorners[] = {
	{-4, 127, 0}, {4, 127, 0}, {-6, 0, 0}, {6, 0, 0}};

/* Each disc's pitch, in 65,536ths of a turn. */
const u16 CruelRuin_DiscPitches[] = {10240, 56579, 56579, 58880};

/* Each disc's yaw, in 65,536ths of a turn. */
const u16 CruelRuin_DiscYaws[] = {39680, 42496, 32768, 22016};

/* How far each pillar sways, in pixels. */
const u8 CruelRuin_PillarSways[] = {24, 48};

/* Each pillar picture's width, in pixels. */
const u8 CruelRuin_PillarWidths[] = {24, 32};

/* The frame each blast goes off on. */
const u8 CruelRuin_BlastFrames[] = {7, 37, 45, 58, 74, 80};

/* Each blast's size, in pixels. */
const u8 CruelRuin_BlastSizes[] = {96, 48, 64, 64, 96, 128};

/* Each blast's x, in pixels. */
const u8 CruelRuin_BlastX[] = {48, 48, 32, 0, 0, 24};

/* Each blast's y, in pixels. */
const u8 CruelRuin_BlastY[] = {96, 96, 96, 96, 120, 120};

/* The frame each streak starts on. */
const u8 CruelRuin_StreakStarts[] = {2, 26, 104, 150};

/* Each streak's size, in pixels. */
const u8 CruelRuin_StreakSizes[] = {32, 16, 24, 64};

/* Each streak's x, in pixels. */
const u8 CruelRuin_StreakX[] = {96, 48, 24, 16};

/* Each streak step's x, in pixels. */
const u8 CruelRuin_StreakStepX[] = {16, 8, 12, 8};

/* Each streak step's y, in pixels. */
const u8 CruelRuin_StreakStepY[] = {16, 4, 16, 24};

/* Each streak end's y, in pixels. */
const u8 CruelRuin_StreakEndY[] = {96, 96, 96, 120};

/* Each streak's roll as it falls. */
const s32 CruelRuin_StreakRolls[] = {DEGtoANG(202.5), DEGtoANG(225),
									 DEGtoANG(202.5), DEGtoANG(202.5)};

/* The flips a glint may be drawn with, one picked at random. */
const u8 Eclipse_GlintFlips[] = {0, 4, 8, 12};

/* The dragon's sprites that take their tiles anew as it turns. */
const u8 Eclipse_TileObjects[] = {8, 9, 25, 14, 15, 20, 21};

/* The frames the canvas is filled anew on, a level from Eclipse_FillLevels */
/* each time. */
const u8 Eclipse_FillFrames[] = {148, 160, 162, 164, 170, 174, 178, 182, 188,
								 194, 196, 200, 202, 206, 208, 210, 220};

/* The level the canvas is filled with at each of Eclipse_FillFrames. */
const u32 Eclipse_FillLevels[] = {
	CANVAS_FILL_WORD(24), CANVAS_FILL_WORD(16), CANVAS_FILL_WORD(16),
	CANVAS_FILL_WORD(24), CANVAS_FILL_WORD(44), CANVAS_FILL_WORD(44),
	CANVAS_FILL_WORD(44), CANVAS_FILL_WORD(44), CANVAS_FILL_WORD(44),
	CANVAS_FILL_WORD(44), CANVAS_FILL_WORD(16), CANVAS_FILL_WORD(16),
	CANVAS_FILL_WORD(16), CANVAS_FILL_WORD(44), CANVAS_FILL_WORD(44),
	CANVAS_FILL_WORD(16), CANVAS_FILL_WORD(16)};

/* Each sprite's x, in pixels. */
const u8 Eclipse_ObjectX[] = {0,   32,	64,	 96,  160, 192, 0,	 32,
							  64,  96,	128, 160, 192, 224, 64,	 96,
							  128, 160, 192, 224, 64,  96,	128, 160,
							  192, 224, 64,	 96,  128, 160, 192, 224};

/* Each sprite's depth, in pixels. */
const u8 Eclipse_ObjectZ[] = {0,  0,  0,  0,  0,   0,	32,	 32,  32,  32, 32,
							  32, 32, 32, 64, 64,  64,	64,	 64,  64,  96, 96,
							  96, 96, 96, 96, 128, 128, 128, 128, 128, 128};

/* How fast each ring grows: its scale gains this many 64ths a frame, from a */
/* quarter. */
const u8 Eclipse_RingGrowth[] = {4, 3, 5, 3};

/* How far each ring sits along its band's axis, in the band's own pixels */
/* before the ring is scaled and stretched. */
const s8 Eclipse_RingHeights[] = {16, 12, 4, -8};

/* The four crystals: the frame each starts after 48; the tables after it give
 */
/* how fast each grows and how tall. */
const u8 FormChange_WaveStarts[] = {0, 4, 9, 10};

/* How fast each wave grows: its scale gains this many times 2000/65536 a */
/* frame, from a quarter. */
const u8 FormChange_WaveGrowth[] = {3, 4, 3, 5};

/* How much taller than wide each wave is drawn, multiplying its scale. */
const u8 FormChange_WaveStretch[] = {3, 4, 5, 6};

/* Each shard's place in the crystal, x and y, in pixels. */
const u8 Prism_ShardStarts[] = {32, 60, 33, 82, 49, 94, 41, 89, 49, 76, 32, 99,
								54, 69, 34, 82, 45, 71, 31, 91, 32, 69, 26, 80,
								40, 68, 41, 60, 46, 60, 36, 63, 0,	0};

/* Each shard picture's width, in pixels. */
const u8 Prism_ShardWidths[] = {5, 10, 11, 14, 12, 11, 19, 18, 23, 12, 22, 22};

/* Each shard picture's height, in pixels. */
const u8 Prism_ShardHeights[] = {14, 10, 7,	 13, 14, 14, 32,
								 30, 24, 38, 20, 20, 0,	 0};

/* Each crystal shard picture's byte offset in Prism's sprite sheet. */
const s32 Prism_ShardPictures[] = {0,	70,	  170,	247,  429,	597,
								   751, 1359, 1899, 2451, 2907, 3347};

/* By mode: how many motes each target gets, and the frames before the targets'
 */
/* share. */
const u8 GatheringLights_Counts[] = {16, 88, 8, 72, 12, 80, 16, 88};

/* The mote's picture for each step of its sway. */
const u8 DjinnBeams_MotePictures[] = {2, 1, 0, 1, 2, 3, 4, 3};

/* The beam's head, EarthWallSheet's three half walls of flame (the mirrored */
/* wall's EARTH_WALL_ pictures): their byte offsets in the sheet. */
const u16 DjinnBeams_HeadPictures[] = {2702, 3535, 4580};

/* Each head picture's width, in pixels. */
const u8 DjinnBeams_HeadWidths[] = {17, 19, 20};

/* Each head picture's height, in pixels. */
const u8 DjinnBeams_HeadHeights[] = {49, 55, 64};

/* The lobbed shells' styles, one LOBBED_SHELLS_STYLE row a mode (enum */
/* LOBBED_SHELLS_MODE). */
const s8 LobbedShells_Styles[] = {
	LOBBED_SHELLS_STYLE(0, 0, 1, 72), /* Bone Chiller */
	LOBBED_SHELLS_STYLE(0, 0, 1, 72), /* Sticky Goo */
	LOBBED_SHELLS_STYLE(0, 0, 1, 72), /* Sticky Poison */
	LOBBED_SHELLS_STYLE(1, 0, 1, 40), /* Rotten Blood */
	LOBBED_SHELLS_STYLE(1, 0, 1, 56), /* Mad Spatter */
	LOBBED_SHELLS_STYLE(0, 0, 1, 56), /* Decompose */
	LOBBED_SHELLS_STYLE(0, 0, 1, 72), /* Poison Ink */
	LOBBED_SHELLS_STYLE(1, 0, 4, 52), /* Mud */
	LOBBED_SHELLS_STYLE(0, 1, 1, 72), /* Rising Venom */
	LOBBED_SHELLS_STYLE(0, 0, 1, 72), /* Formic Acid */
};

/* Each shard picture's width, in pixels. */
const u8 LobbedShells_ShardWidths[] = {9, 10, 9, 9, 9, 11, 11, 11, 12,
									   6, 7,  7, 7, 6, 6,  8,  8,  8};

/* Each shard picture's height, in pixels. */
const u8 LobbedShells_ShardHeights[] = {12, 12, 12, 12, 12, 12, 12, 12, 12,
										8,	7,	6,	6,	8,	6,	4,	3,	3};

/* Each shard picture's byte offset in the sprite sheet. */
const u16 LobbedShells_ShardPictures[] = {0,	108,  228,	336,  444,	552,
										  684,	816,  948,	1092, 1140, 1189,
										  1231, 1273, 1321, 1357, 1389, 1413};

/* The reaper's seven panels as Condemn draws them: their x in the canvas, in */
/* pixels, seven for each side; the same as Reaper_PictureX, Condemn keeping */
/* its own copy. */
const u8 Condemn_PictureX[] = {10, 30, 0, 0, 0, 6, 52, 60, 0, 0, 0, 0, 0, 0};

/* The reaper's panels: their y in the canvas, in pixels, seven for each side.
 */
const u8 Condemn_PictureY[] = {0, 20, 2, 2, 34, 64, 68, 0, 9, 4, 4, 4, 4, 4};

/* The triangle's first corner, (0, 24, 0), in pixels. */
const s32 Bind_Vertex[] = {0, 24, 0};

/* The striking triangle's first corner, (0, 24, 0), in pixels; the same as */
/* Bind_Vertex, read by the strike's own code. */
const s32 Bind_TriangleStrikeVertex[] = {0, 24, 0};

/* The two ring points, alternating, x, y and z in 16.16 pixels. */
const s32 Star_Points[] = {0, toFIXED(48), 0, 0, toFIXED(28), 0};

/* The ring radii, alternating, in pixels. */
const u8 Star_Radii[] = {32, 16};

/* The grid Procne's nine sprites sit on: each one's x, in pixels. */
const u8 Procne_GridX[] = {0, 32, 64, 0, 32, 64, 0, 32, 64};

/* The grid Procne's nine sprites sit on: each one's depth, in pixels. */
const u8 Procne_GridY[] = {0, 0, 0, 32, 32, 32, 64, 64, 64};

/* The frames of the four comet strikes. */
const u16 Procne_Frames[] = {334, 347, 359, 372};

/* A strike's three pictures: their widths, in pixels. */
const u8 Procne_PictureWidths[] = {14, 24, 48};

/* A strike's three pictures: their heights, in pixels. */
const u8 Procne_PictureHeights[] = {14, 19, 24};

/* A strike's three pictures: how far each is raised above its mark, in pixels.
 */
const u8 Procne_PictureBiasY[] = {8, 12, 17};

/* A strike's three pictures: their byte offsets in the sprite sheet. */
const u16 Procne_Pictures[] = {13824, 14020, 14476};

/* What each grain adds to its random speed of 0 to 127, in 32nds of a pixel a
 */
/* frame. */
const u8 Tiamat_EmberSpeeds[] = {0, 8, 12, 16};

/* A spark's picture: one pixel wide and two tall, both at level 32. */
const u8 Tiamat_SparkPixels[] = {32, 32};

/* Each grain's size: the width of its mote picture, in pixels, which is twice
 */
/* as tall. */
const u8 Tiamat_EmberSizes[] = {1, 1, 2, 3};

/* The ember streak's pictures again, as Tiamat's fireballs read them: the same
 */
/* as BattleEffect_EmberStreakPictureWidths and the three tables after it, */
/* Tiamat keeping its own copy among its tables. Each picture's width, in */
/* pixels. */
const u8 Tiamat_EmberStreakWidths[] = {24, 29, 30, 27, 24, 16};

/* Each ember streak picture's height, in pixels. */
const u8 Tiamat_EmberStreakHeights[] = {23, 32, 37, 49, 53, 52};

/* How far below its anchor each ember streak picture's top goes, in pixels. */
const u8 Tiamat_EmberStreakBiasY[] = {41, 32, 26, 11, 4, 2};

/* Each ember streak picture's byte offset in the sheet. */
const u16 Tiamat_EmberStreakPictures[] = {0, 552, 1480, 2590, 3913, 5185};

/* The water columns' feet, x and y, in pixels; NEREID_COLUMNS of the four */
/* pairs are read. */
const u8 Nereid_Columns[] = {16, 104, 32, 104, 48, 104, 64, 104};

/* Each falling spark's gravity, in pixels a frame each frame. */
const s32 Nereid_Gravity[] = {toFIXED(0.5), toFIXED(0.25), toFIXED(0.25),
							  toFIXED(0.125)};

/* Froth, by variant: the FROTH_ columns. */
const u8 Vortices_Counts[][2] = {
	/* bubbles (at most 30, battle_effect_vortices.c), frames */
	[EFFECT_VARIANT_BASIC] = {6, 140},		/* Froth */
	[EFFECT_VARIANT_STRONGER] = {12, 160},	/* Froth Sphere */
	[EFFECT_VARIANT_STRONGEST] = {30, 200}, /* Froth Spiral */
};

/* Whirlwind, by variant: the WHIRLWIND_ columns. The sway is in pixels each */
/* way; the shake is btCameraShake's horizontal range, the vertical */
/* twice that. */
const u8 Vortices_Shapes[][3] = {
	/* tornadoes, sway, screen shake */
	[EFFECT_VARIANT_BASIC] = {1, 16, 2},	 /* Whirlwind, Gale */
	[EFFECT_VARIANT_STRONGER] = {2, 24, 2},	 /* Tornado, Typhoon */
	[EFFECT_VARIANT_STRONGEST] = {4, 32, 4}, /* Tempest, Hurricane */
};

/* The crystals, the frames and the frame the damage shows, three for each */
/* variant (PRISM_CRYSTAL_COUNT, PRISM_FRAMES, PRISM_DAMAGE_FRAME). */
const u8 Prism_Counts[] = {1, 85, 30, 4, 124, 54, 12, 166, 86};

/* The sparks each lightning pillar throws and the frames the screen shakes at
 */
/* each strike, a pair for each variant (PLASMA_SPARKS, PLASMA_SHAKE_FRAMES). */
const u16 Plasma_Sparks[] = {384, 2, 256, 3, 192, 4};

/* The columns' x, in pixels, seven for each side. */
const u8 Plasma_Columns[] = {32, 64, 16,  48, 72, 96, 64,
							 88, 56, 104, 72, 48, 24, 56};

/* The lightning pillars, by variant. */
const u8 Plasma_Counts[] = {1, 3, 7};

/* Five halfwords for each variant: the sparks in each burst, the bursts, and */
/* each burst's x offset in pixels. */
const u16 SparkGroups_Shapes[][5] = {{128, 1, 16, 0, 0},  {255, 1, 16, 0, 0},
									 {192, 3, 0, 24, 48}, {340, 1, 16, 24, 48},
									 {32, 1, 16, 0, 0},	  {200, 1, 16, 24, 48}};

/* The flash burst's picture for each three frames of a flash, by number. */
const u8 Plasma_FlashPictures[] = {5, 5, 4, 3, 2, 1};

/* The pillars of fire, by variant. */
const u8 Volcano_Counts[] = {1, 2, 4};

/* Each pillar's depth on the field, in pixels, by variant. */
const s8 Volcano_Depths[][4] = {
	{0, 0, 0, 0}, {24, -24, 0, 0}, {-20, 20, -60, 60}};

/* The frame each pillar erupts on. */
const u8 Volcano_StartFrames[] = {8, 18, 24, 30, 34};

/* The flips a glint may be drawn with, one picked at random. */
const u8 TargetMotes_GlintFlips[] = {0, 4, 8, 12};

/* Where each sprite of the charging Zagan sits, x and y, in pixels. */
const u16 Zagan_ChargePoints[][2] = {{0, 0},   {0, 32},	 {32, 0},  {32, 32},
									 {32, 64}, {64, 0},	 {64, 32}, {64, 64},
									 {96, 0},  {96, 32}, {96, 64}};

/* Where each sprite of the galloping Zagan sits, x and y, in pixels. */
const u16 Zagan_GallopPoints[][2] = {{0, 16},  {0, 48}, {32, 0},  {32, 32},
									 {32, 64}, {64, 0}, {64, 32}, {64, 64},
									 {64, 96}, {96, 0}, {96, 64}, {96, 96}};

/* The frame each dust ring starts on, counted from the blow. */
const u8 Zagan_RingStarts[] = {0, 6, 8, 9};

/* How fast each dust ring grows: its scale gains this many times 2000/65536 a
 */
/* frame. */
const u8 Zagan_RingGrowth[] = {5, 8, 7, 3};

/* Each dust ring's height, in pixels. */
const s8 Zagan_RingHeights[] = {32, 12, -24, 0};

/* One handler for each battle effect, by its kind less one; the kinds with */
/* no handler are left empty. */
void (*const BattleEffect_Handlers[BATTLE_EFFECT_COUNT - 1])(
	BATTLE_EFFECT_PARM *args) = {
	[BATTLE_EFFECT_QUAKE - 1] = btEffectRunQuake,
	[BATTLE_EFFECT_SPIRE - 1] = btEffectRunSpire,
	[BATTLE_EFFECT_GAIA - 1] = btEffectRunGaia,
	[BATTLE_EFFECT_GROWTH - 1] = btEffectRunGrowth,
	[BATTLE_EFFECT_THORN - 1] = btEffectRunThorn,
	[BATTLE_EFFECT_FROST - 1] = btEffectRunFrost,
	[BATTLE_EFFECT_ICE - 1] = btEffectRunIce,
	[BATTLE_EFFECT_PRISM - 1] = btEffectRunPrism,
	[BATTLE_EFFECT_DOUSE - 1] = btEffectRunDouse,
	[BATTLE_EFFECT_FROTH - 1] = btEffectRunFroth,
	[BATTLE_EFFECT_FLARE - 1] = btEffectRunFlare,
	[BATTLE_EFFECT_FIRE - 1] = btEffectRunFire,
	[BATTLE_EFFECT_VOLCANO - 1] = btEffectRunVolcano,
	[BATTLE_EFFECT_BLAST - 1] = btEffectRunBlast,
	[BATTLE_EFFECT_NOVA - 1] = btEffectRunNova,
	[BATTLE_EFFECT_BOLT - 1] = btEffectRunBolt,
	[BATTLE_EFFECT_RAY - 1] = btEffectRunRay,
	[BATTLE_EFFECT_PLASMA - 1] = btEffectRunPlasma,
	[BATTLE_EFFECT_SLASH - 1] = btEffectRunSlash,
	[BATTLE_EFFECT_WHIRLWIND - 1] = btEffectRunWhirlwind,
	[BATTLE_EFFECT_CURE - 1] = btEffectRunCure,
	[BATTLE_EFFECT_PLY - 1] = btEffectRunPly,
	[BATTLE_EFFECT_WISH - 1] = btEffectRunWish,
	[BATTLE_EFFECT_CURE_POISON - 1] = btEffectRunCurePoison,
	[BATTLE_EFFECT_REVIVE - 1] = btEffectRunRevive,
	[BATTLE_EFFECT_IMPACT - 1] = btEffectRunImpact,
	[BATTLE_EFFECT_DULL - 1] = btEffectRunDull,
	[BATTLE_EFFECT_GUARD - 1] = btEffectRunGuard,
	[BATTLE_EFFECT_IMPAIR - 1] = btEffectRunImpair,
	[BATTLE_EFFECT_WARD - 1] = btEffectRunWard,
	[BATTLE_EFFECT_WEAKEN - 1] = btEffectRunWeaken,
	[BATTLE_EFFECT_TAINT - 1] = btEffectRunTaint,
	[BATTLE_EFFECT_DELUDE - 1] = btEffectRunDelude,
	[BATTLE_EFFECT_UNUSED_ORBITING_SPIRALS -
		1] = btEffectRunUnusedOrbitingSpirals,
	[BATTLE_EFFECT_UNUSED_HEARTRENDER - 1] = btEffectRunHeartrender,
	[BATTLE_EFFECT_PARALYZE - 1] = btEffectRunParalyze,
	[BATTLE_EFFECT_SLEEP - 1] = btEffectRunSleep,
	[BATTLE_EFFECT_BIND - 1] = btEffectRunBind,
	[BATTLE_EFFECT_HAUNT - 1] = btEffectRunHaunt,
	[BATTLE_EFFECT_CONDEMN - 1] = btEffectRunCondemn,
	[BATTLE_EFFECT_CURSE - 1] = btEffectRunCurse,
	[BATTLE_EFFECT_DRAIN - 1] = btEffectRunDrain,
	[BATTLE_EFFECT_BREAK - 1] = btEffectRunBreak,
	[BATTLE_EFFECT_REGENERATE - 1] = btEffectRunWish,
	[BATTLE_EFFECT_REFLECT - 1] = btEffectRunWish,
	[BATTLE_EFFECT_UNUSED_IMPACT - 1] = btEffectRunImpact,
	[BATTLE_EFFECT_UNUSED_IMPAIR - 1] = btEffectRunImpair,
	[BATTLE_EFFECT_DRAGON_CLOUD - 1] = btEffectRunDragonCloud,
	[BATTLE_EFFECT_DEMON_NIGHT - 1] = btEffectRunDemonNight,
	[BATTLE_EFFECT_HELM_SPLITTER - 1] = btEffectRunHelmSplitter,
	[BATTLE_EFFECT_QUICK_STRIKE - 1] = btEffectRunQuickStrike,
	[BATTLE_EFFECT_DEATH_PLUNGE - 1] = btEffectRunDeathPlunge,
	[BATTLE_EFFECT_SHURIKEN - 1] = btEffectRunShuriken,
	[BATTLE_EFFECT_ANNIHILATION - 1] = btEffectRunAnnihilation,
	[BATTLE_EFFECT_PUNJI - 1] = btEffectRunPunji,
	[BATTLE_EFFECT_CUTTING_EDGE - 1] = btEffectRunCuttingEdge,
	[BATTLE_EFFECT_RAGNAROK - 1] = btEffectRunRagnarok,
	[BATTLE_EFFECT_HEAT_WAVE - 1] = btEffectRunHeatWave,
	[BATTLE_EFFECT_PLANET_DIVER - 1] = btEffectRunPlanetDiver,
	[BATTLE_EFFECT_TITAN_BLADE - 1] = btEffectRunTitanBlade,
	[BATTLE_EFFECT_SHINING_STAR - 1] = btEffectRunShiningStar,
	[BATTLE_EFFECT_BLIZZARD - 1] = btEffectRunBlizzard,
	[BATTLE_EFFECT_DEMON_FIRE - 1] = btEffectRunDemonFire,
	[BATTLE_EFFECT_VORPAL_SLASH - 1] = btEffectRunVorpalSlash,
	[BATTLE_EFFECT_DROWN - 1] = btEffectRunDrown,
	[BATTLE_EFFECT_UNUSED_TRIANGLE_STRIKE -
		1] = btEffectRunUnusedTriangleStrike,
	[BATTLE_EFFECT_ASTRAL_BLAST - 1] = btEffectRunAstralBlast,
	[BATTLE_EFFECT_HEARTBREAK - 1] = btEffectRunHeartbreak,
	[BATTLE_EFFECT_AURA - 1] = btEffectRunAura,
	[BATTLE_EFFECT_FUME - 1] = btEffectRunFume,
	[BATTLE_EFFECT_BEAM - 1] = btEffectRunBeam,
	[BATTLE_EFFECT_RAGING_HEAT - 1] = btEffectRunRagingHeat,
	[BATTLE_EFFECT_COOL - 1] = btEffectRunCool,
	[BATTLE_EFFECT_SABRE_DANCE - 1] = btEffectRunSabreDance,
	[BATTLE_EFFECT_BACKSTAB - 1] = btEffectRunBackstab,
	[BATTLE_EFFECT_JUGGLE - 1] = btEffectRunJuggle,
	[BATTLE_EFFECT_FLAME_CARD - 1] = btEffectRunFlameCard,
	[BATTLE_EFFECT_WHIPLASH - 1] = btEffectRunWhiplash,
	[BATTLE_EFFECT_POISON_FLOW - 1] = btEffectRunPoisonFlow,
	[BATTLE_EFFECT_FIRE_PUPPET - 1] = btEffectRunFirePuppet,
	[BATTLE_EFFECT_DIAMOND_DUST - 1] = btEffectRunDiamondDust,
	[BATTLE_EFFECT_DIAMOND_BERG - 1] = btEffectRunDiamondBerg,
	[BATTLE_EFFECT_PLUME_EDGE - 1] = btEffectRunPlumeEdge,
	[BATTLE_EFFECT_THUNDER_MINE - 1] = btEffectRunThunderMine,
	[BATTLE_EFFECT_FROST_CARD - 1] = btEffectRunFrostCard,
	[BATTLE_EFFECT_BRAMBLE_CARD - 1] = btEffectRunBrambleCard,
	[BATTLE_EFFECT_THUNDER_CARD - 1] = btEffectRunThunderCard,
	[BATTLE_EFFECT_BAFFLE_CARD - 1] = btEffectRunBaffleCard,
	[BATTLE_EFFECT_SWORD_CARD - 1] = btEffectRunSwordCard,
	[BATTLE_EFFECT_SLEEP_CARD - 1] = btEffectRunSleepCard,
	[BATTLE_EFFECT_DEATH_CARD - 1] = btEffectRunDeathCard,
	[BATTLE_EFFECT_STONE_JUSTICE - 1] = btEffectRunStoneJustice,
	[BATTLE_EFFECT_HEAVY_DIVIDE - 1] = btEffectRunHeavyDivide,
	[BATTLE_EFFECT_WYRD_CURSE - 1] = btEffectRunWyrdCurse,
	[BATTLE_EFFECT_ACHERONS_GRIEF - 1] = btEffectRunAcheronsGrief,
	[BATTLE_EFFECT_HURRICANE - 1] = btEffectRunHurricane,
	[BATTLE_EFFECT_DREAMTIDE - 1] = btEffectRunDreamtide,
	[BATTLE_EFFECT_LETHE_ALBION - 1] = btEffectRunLetheAlbion,
	[BATTLE_EFFECT_RISING_DRAGON - 1] = btEffectRunRisingDragon,
	[BATTLE_EFFECT_BOOST_HACK - 1] = btEffectRunBoostHack,
	[BATTLE_EFFECT_RADIANT_FIRE - 1] = btEffectRunRadiantFire,
	[BATTLE_EFFECT_FIRE_DANCE - 1] = btEffectRunFireDance,
	[BATTLE_EFFECT_BLAZE_RUSH - 1] = btEffectRunBlazeRush,
	[BATTLE_EFFECT_SHRED - 1] = btEffectRunShred,
	[BATTLE_EFFECT_PURGATORY - 1] = btEffectRunPurgatory,
	[BATTLE_EFFECT_SOUL_SHATTER - 1] = btEffectRunSoulShatter,
	[BATTLE_EFFECT_AGING_GAS - 1] = btEffectRunAgingGas,
	[BATTLE_EFFECT_MAD_ZEPHYR - 1] = btEffectRunMadZephyr,
	[BATTLE_EFFECT_LUNAR_SLASH - 1] = btEffectRunLunarSlash,
	[BATTLE_EFFECT_NIRVANA - 1] = btEffectRunNirvana,
	[BATTLE_EFFECT_LIGHT_SURGE - 1] = btEffectRunLightSurge,
	[BATTLE_EFFECT_VOID_BEAM - 1] = btEffectRunVoidBeam,
	[BATTLE_EFFECT_LEGEND - 1] = btEffectRunLegend,
	[BATTLE_EFFECT_LIQUIFIER - 1] = btEffectRunLiquifier,
	[BATTLE_EFFECT_MEGIDDO - 1] = btEffectRunMegiddo,
	[BATTLE_EFFECT_ODYSSEY - 1] = btEffectRunOdyssey,
	[BATTLE_EFFECT_VENGEANCE - 1] = btEffectRunVengeance,
	[BATTLE_EFFECT_THORNY_GRAVE - 1] = btEffectRunThornyGrave,
	[BATTLE_EFFECT_PLANETARY - 1] = btEffectRunPlanetary,
	[BATTLE_EFFECT_LIFE_SHEAR - 1] = btEffectRunLifeShear,
	[BATTLE_EFFECT_CRUCIBLE_FIRE - 1] = btEffectRunCrucibleFire,
	[BATTLE_EFFECT_REVERSE_STAR - 1] = btEffectRunReverseStar,
	[BATTLE_EFFECT_GRANITE - 1] = btEffectRunGranite,
	[BATTLE_EFFECT_QUARTZ - 1] = btEffectRunRevive,
	[BATTLE_EFFECT_VINE - 1] = btEffectRunVine,
	[BATTLE_EFFECT_GROUND - 1] = btEffectRunGround,
	[BATTLE_EFFECT_FIZZ - 1] = btEffectRunPly,
	[BATTLE_EFFECT_SLEET - 1] = btEffectRunSleet,
	[BATTLE_EFFECT_SPRITZ - 1] = btEffectRunWish,
	[BATTLE_EFFECT_HAIL - 1] = btEffectRunHail,
	[BATTLE_EFFECT_TONIC - 1] = btEffectRunTonic,
	[BATTLE_EFFECT_DEW - 1] = btEffectRunDew,
	[BATTLE_EFFECT_FORGE - 1] = btEffectRunImpact,
	[BATTLE_EFFECT_CORONA - 1] = btEffectRunGuard,
	[BATTLE_EFFECT_PSY_CRYSTAL - 1] = btEffectRunPsyCrystal,
	[BATTLE_EFFECT_FLASH - 1] = btEffectRunFlash,
	[BATTLE_EFFECT_TORCH - 1] = btEffectRunTorch,
	[BATTLE_EFFECT_BREEZE - 1] = btEffectRunWard,
	[BATTLE_EFFECT_ZEPHYR - 1] = btEffectRunZephyr,
	[BATTLE_EFFECT_KITE - 1] = btEffectRunKite,
	[BATTLE_EFFECT_LUFF - 1] = btEffectRunBind,
	[BATTLE_EFFECT_GUST - 1] = btEffectRunGust,
	[BATTLE_EFFECT_IRON - 1] = btEffectRunIron,
	[BATTLE_EFFECT_STEAM - 1] = btEffectRunSteam,
	[BATTLE_EFFECT_SHADE - 1] = btEffectRunShade,
	[BATTLE_EFFECT_KINDLE - 1] = btEffectRunImpact,
	[BATTLE_EFFECT_COAL - 1] = btEffectRunCoal,
	[BATTLE_EFFECT_FLOWER - 1] = btEffectRunCure,
	[BATTLE_EFFECT_SALT - 1] = btEffectRunSalt,
	[BATTLE_EFFECT_SPRING - 1] = btEffectRunSpring,
	[BATTLE_EFFECT_BREATH - 1] = btEffectRunBreath,
	[BATTLE_EFFECT_ETHER - 1] = btEffectRunEther,
	[BATTLE_EFFECT_BALM - 1] = btEffectRunBalm,
	[BATTLE_EFFECT_TINDER - 1] = btEffectRunTinder,
	[BATTLE_EFFECT_WHORL - 1] = btEffectRunWhorl,
	[BATTLE_EFFECT_GALE - 1] = btEffectRunGale,
	[BATTLE_EFFECT_WAFT - 1] = btEffectRunWaft,
	[BATTLE_EFFECT_CORE - 1] = btEffectRunCore,
	[BATTLE_EFFECT_MUD - 1] = btEffectRunMud,
	[BATTLE_EFFECT_HAZE - 1] = btEffectRunHaze,
	[BATTLE_EFFECT_PETRA - 1] = btEffectRunPetra,
	[BATTLE_EFFECT_RIME - 1] = btEffectRunRime,
	[BATTLE_EFFECT_LULL - 1] = btEffectRunLull,
	[BATTLE_EFFECT_GASP - 1] = btEffectRunGasp,
	[BATTLE_EFFECT_FUGUE - 1] = btEffectRunFugue,
	[BATTLE_EFFECT_CRYSTAL - 1] = btEffectRunCrystal,
	[BATTLE_EFFECT_EDDY - 1] = btEffectRunEddy,
	[BATTLE_EFFECT_CANNON - 1] = btEffectRunCannon,
	[BATTLE_EFFECT_AROMA - 1] = btEffectRunAroma,
	[BATTLE_EFFECT_REFLUX - 1] = btEffectRunReflux,
	[BATTLE_EFFECT_DJINN_COUNTER - 1] = btEffectRunDjinnCounter,
	[BATTLE_EFFECT_MOLD - 1] = btEffectRunMold,
	[BATTLE_EFFECT_MELD - 1] = btEffectRunMeld,
	[BATTLE_EFFECT_SHINE - 1] = btEffectRunShine,
	[BATTLE_EFFECT_GEODE - 1] = btEffectRunGeode,
	[BATTLE_EFFECT_SERAC - 1] = btEffectRunSerac,
	[BATTLE_EFFECT_SPARK - 1] = btEffectRunSpark,
	[BATTLE_EFFECT_FURY - 1] = btEffectRunFury,
	[BATTLE_EFFECT_CHAR - 1] = btEffectRunChar,
	[BATTLE_EFFECT_VENUS - 1] = btEffectRunVenus,
	[BATTLE_EFFECT_MERCURY - 1] = btEffectRunMercury,
	[BATTLE_EFFECT_MARS - 1] = btEffectRunMars,
	[BATTLE_EFFECT_JUPITER - 1] = btEffectRunJupiter,
	[BATTLE_EFFECT_SONIC_WAVE - 1] = btEffectRunSonicWave,
	[BATTLE_EFFECT_SHRIEK - 1] = btEffectRunShriek,
	[BATTLE_EFFECT_BANSHEE_HOWL - 1] = btEffectRunBansheeHowl,
	[BATTLE_EFFECT_CRAZY_VOICE - 1] = btEffectRunCrazyVoice,
	[BATTLE_EFFECT_WAR_CRY - 1] = btEffectRunWarCry,
	[BATTLE_EFFECT_WICKED_HOWL - 1] = btEffectRunWickedHowl,
	[BATTLE_EFFECT_WING_BEAT - 1] = btEffectRunWingBeat,
	[BATTLE_EFFECT_WING_FLUTTER - 1] = btEffectRunWingFlutter,
	[BATTLE_EFFECT_RUMBLE - 1] = btEffectRunRumble,
	[BATTLE_EFFECT_BONE_CHILLER - 1] = btEffectRunBoneChiller,
	[BATTLE_EFFECT_RAPID_SMASH - 1] = btEffectRunGust,
	[BATTLE_EFFECT_BONE_CHARGE - 1] = btEffectRunBoneCharge,
	[BATTLE_EFFECT_MYSTIC_FLAME - 1] = btEffectRunMysticFlame,
	[BATTLE_EFFECT_NUMBING_STING - 1] = btEffectRunNumbingSting,
	[BATTLE_EFFECT_BRUTE_FORCE - 1] = btEffectRunBruteForce,
	[BATTLE_EFFECT_STICKY_GOO - 1] = btEffectRunStickyGoo,
	[BATTLE_EFFECT_CANNIBAL_FANG - 1] = btEffectRunCannibalFang,
	[BATTLE_EFFECT_BEAR_CLAW - 1] = btEffectRunBearClaw,
	[BATTLE_EFFECT_POISONOUS_BITE - 1] = btEffectRunPoisonousBite,
	[BATTLE_EFFECT_FLYING_ATTACK - 1] = btEffectRunFlyingAttack,
	[BATTLE_EFFECT_UNDEAD_SWORD - 1] = btEffectRunUndeadSword,
	[BATTLE_EFFECT_RANSACK - 1] = btEffectRunRansack,
	[BATTLE_EFFECT_STICKY_POISON - 1] = btEffectRunStickyPoison,
	[BATTLE_EFFECT_POISON_FANG - 1] = btEffectRunPoisonousBite,
	[BATTLE_EFFECT_ELECTRIC_BITE - 1] = btEffectRunElectricBite,
	[BATTLE_EFFECT_POISON_TAIL - 1] = btEffectRunPoisonTail,
	[BATTLE_EFFECT_ONSLAUGHT - 1] = btEffectRunOnslaught,
	[BATTLE_EFFECT_VAMPIRIC_FANG - 1] = btEffectRunVampiricFang,
	[BATTLE_EFFECT_BACTERIA_RUSH - 1] = btEffectRunBacteriaRush,
	[BATTLE_EFFECT_SWIFT_STRIKE - 1] = btEffectRunFlyingAttack,
	[BATTLE_EFFECT_ROTTEN_BLOOD - 1] = btEffectRunRottenBlood,
	[BATTLE_EFFECT_DOUBLE_FANG - 1] = btEffectRunDoubleFang,
	[BATTLE_EFFECT_MORTAL_BLOW - 1] = btEffectRunMortalBlow,
	[BATTLE_EFFECT_FREEBITE_RUSH - 1] = btEffectRunFreebiteRush,
	[BATTLE_EFFECT_TWIN_BEAKS - 1] = btEffectRunTwinBeaks,
	[BATTLE_EFFECT_RABID_FANG - 1] = btEffectRunRabidFang,
	[BATTLE_EFFECT_ACID_BITE - 1] = btEffectRunAcidBite,
	[BATTLE_EFFECT_DYNAMITE - 1] = btEffectRunDynamite,
	[BATTLE_EFFECT_HEADBUTT - 1] = btEffectRunHeadbutt,
	[BATTLE_EFFECT_POISON_INK - 1] = btEffectRunPoisonInk,
	[BATTLE_EFFECT_COUNTERSTRIKE - 1] = btEffectRunBearClaw,
	[BATTLE_EFFECT_MAD_DASH - 1] = btEffectRunRansack,
	[BATTLE_EFFECT_SOOTHING_STAR - 1] = btEffectRunSoothingStar,
	[BATTLE_EFFECT_SPIDER_WEB - 1] = btEffectRunSpiderWeb,
	[BATTLE_EFFECT_HEARTRENDER - 1] = btEffectRunHeartrender,
	[BATTLE_EFFECT_MAD_SPATTER - 1] = btEffectRunMadSpatter,
	[BATTLE_EFFECT_SPASM - 1] = btEffectRunSpasm,
	[BATTLE_EFFECT_SLEEP_STAR - 1] = btEffectRunSleepStar,
	[BATTLE_EFFECT_DECOMPOSE - 1] = btEffectRunDecompose,
	[BATTLE_EFFECT_HAUNTING - 1] = btEffectRunHaunting,
	[BATTLE_EFFECT_WORMS - 1] = btEffectRunCure,
	[BATTLE_EFFECT_BERSERK - 1] = btEffectRunBerserk,
	[BATTLE_EFFECT_LUCID_PROPHECY - 1] = btEffectRunCurse,
	[BATTLE_EFFECT_SMOKE_BOMB - 1] = btEffectRunSmokeBomb,
	[BATTLE_EFFECT_WING_STROKE - 1] = btEffectRunWingStroke,
	[BATTLE_EFFECT_POISON_BEAT - 1] = btEffectRunPoisonBeat,
	[BATTLE_EFFECT_SPINNING_BEAT - 1] = btEffectRunSpinningBeat,
	[BATTLE_EFFECT_OUTER_SPACE - 1] = btEffectRunOuterSpace,
	[BATTLE_EFFECT_DRAGON_DRIVER - 1] = btEffectRunDragonDriver,
	[BATTLE_EFFECT_DRAIN_FANG - 1] = btEffectRunDrainFang,
	[BATTLE_EFFECT_SEVERE_BLOW - 1] = btEffectRunSevereBlow,
	[BATTLE_EFFECT_THRASH - 1] = btEffectRunThrash,
	[BATTLE_EFFECT_RECOVERY - 1] = btEffectRunRecovery,
	[BATTLE_EFFECT_ECHO_CUT - 1] = btEffectRunEchoCut,
	[BATTLE_EFFECT_WATERY_GRAVE - 1] = btEffectRunWateryGrave,
	[BATTLE_EFFECT_RAGING_FLOOD - 1] = btEffectRunRagingFlood,
	[BATTLE_EFFECT_HYDRO_SLASH - 1] = btEffectRunHydroSlash,
	[BATTLE_EFFECT_MIGHTY_PRESS - 1] = btEffectRunMightyPress,
	[BATTLE_EFFECT_SLAVER - 1] = btEffectRunSlaver,
	[BATTLE_EFFECT_COUNTER_RUSH - 1] = btEffectRunCounterRush,
	[BATTLE_EFFECT_DJINNFEST - 1] = btEffectRunDjinnfest,
	[BATTLE_EFFECT_HEAT_KISS - 1] = btEffectRunHeatKiss,
	[BATTLE_EFFECT_ANGLE_SPIKE - 1] = btEffectRunAngSpike,
	[BATTLE_EFFECT_FIRE_BREATH - 1] = btEffectRunFireBreath,
	[BATTLE_EFFECT_WATER_BREATH - 1] = btEffectRunWaterBreath,
	[BATTLE_EFFECT_ICE_BLESSING - 1] = btEffectRunIceBlessing,
	[BATTLE_EFFECT_DARK_BREATH - 1] = btEffectRunDarkBreath,
	[BATTLE_EFFECT_ACID_BREATH - 1] = btEffectRunAcidBreath,
	[BATTLE_EFFECT_STORM_BREATH - 1] = btEffectRunStormBreath,
	[BATTLE_EFFECT_EVIL_BLESSING - 1] = btEffectRunEvilBlessing,
	[BATTLE_EFFECT_DEADLY_GAS - 1] = btEffectRunDeadlyGas,
	[BATTLE_EFFECT_SAND_BREATH - 1] = btEffectRunSandBreath,
	[BATTLE_EFFECT_DESERT_GASP - 1] = btEffectRunDesertGasp,
	[BATTLE_EFFECT_BLACK_ICE - 1] = btEffectRunBlackIce,
	[BATTLE_EFFECT_TYPHOON_BLOW - 1] = btEffectRunTyphoonBlow,
	[BATTLE_EFFECT_TOXIC_BREATH - 1] = btEffectRunToxicBreath,
	[BATTLE_EFFECT_GRAVEL_BLOW - 1] = btEffectRunGravelBlow,
	[BATTLE_EFFECT_BLAST_BREATH - 1] = btEffectRunBlastBreath,
	[BATTLE_EFFECT_DARKSOL_GASP - 1] = btEffectRunDarksolGasp,
	[BATTLE_EFFECT_FLAME_BREATH - 1] = btEffectRunFireBreath,
	[BATTLE_EFFECT_MANEATER - 1] = btEffectRunManeater,
	[BATTLE_EFFECT_UR_FLASH - 1] = btEffectRunUrFlash,
	[BATTLE_EFFECT_CRUSHER_GRIP - 1] = btEffectRunCrusherGrip,
	[BATTLE_EFFECT_VANISH_CLAW - 1] = btEffectRunVanishClaw,
	[BATTLE_EFFECT_TWIN_SHEAR - 1] = btEffectRunTwinShear,
	[BATTLE_EFFECT_TRIPLE_CHOMP - 1] = btEffectRunTripleChomp,
	[BATTLE_EFFECT_DOUBLESTEP - 1] = btEffectRunDoublestep,
	[BATTLE_EFFECT_POISON_GEL - 1] = btEffectRunPoisonGel,
	[BATTLE_EFFECT_MUCOUS_GEL - 1] = btEffectRunDelude,
	[BATTLE_EFFECT_RISING_VENOM - 1] = btEffectRunRisingVenom,
	[BATTLE_EFFECT_SACK - 1] = btEffectRunSack,
	[BATTLE_EFFECT_FIRE_DANCE_UNLEASH - 1] = btEffectRunFireDanceUnleash,
	[BATTLE_EFFECT_STUN_MUSCLE - 1] = btEffectRunStunMuscle,
	[BATTLE_EFFECT_ROLLING_FLAME - 1] = btEffectRunRollingFlame,
	[BATTLE_EFFECT_CAGE - 1] = btEffectRunCage,
	[BATTLE_EFFECT_PHOENIX - 1] = btEffectRunPhoenix,
	[BATTLE_EFFECT_HUMAN_HUNT - 1] = btEffectRunHumanHunt,
	[BATTLE_EFFECT_UNUSED_HEAVY_BITE - 1] = btEffectRunUnusedHeavyBite,
	[BATTLE_EFFECT_UNUSED_FLASH_BITE - 1] = btEffectRunUnusedFlashBite,
	[BATTLE_EFFECT_POISON_STING_TAIL - 1] = btEffectRunPoisonTail,
	[BATTLE_EFFECT_KILL_STING - 1] = btEffectRunKillSting,
	[BATTLE_EFFECT_BEAST_NEEDLE - 1] = btEffectRunBeastNeedle,
	[BATTLE_EFFECT_POISON_STING - 1] = btEffectRunPoisonSting,
	[BATTLE_EFFECT_STUN_STING - 1] = btEffectRunStunSting,
	[BATTLE_EFFECT_DEMON_EYE - 1] = btEffectRunDemonEye,
	[BATTLE_EFFECT_EMU - 1] = btEffectRunEmu,
	[BATTLE_EFFECT_OCEAN_FIST - 1] = btEffectRunOceanFist,
	[BATTLE_EFFECT_CALL_DULLAHAN - 1] = btEffectRunCallDullahan,
	[BATTLE_EFFECT_GHOST_SOLDIER - 1] = btEffectRunGhostSoldier,
	[BATTLE_EFFECT_RISING_DRAGON_ENEMY - 1] = btEffectRunRisingDragonEnemy,
	[BATTLE_EFFECT_TRUE_COLLIDE - 1] = btEffectRunTrueCollide,
	[BATTLE_EFFECT_STAR_MINE - 1] = btEffectRunStarMine,
	[BATTLE_EFFECT_FORMIC_ACID - 1] = btEffectRunFormicAcid,
	[BATTLE_EFFECT_CLAW_ATTACK - 1] = btEffectRunClawAttack,
	[BATTLE_EFFECT_BEAT_DANCE - 1] = btEffectRunBeatDance,
	[BATTLE_EFFECT_UNUSED_SPARK_BITE - 1] = btEffectRunUnusedSparkBite,
	[BATTLE_EFFECT_LIVING_ARMOR - 1] = btEffectRunLivingArmor,
	[BATTLE_EFFECT_HEAT_STUN - 1] = btEffectRunHeatStun,
	[BATTLE_EFFECT_EARTH_FORCE - 1] = btEffectRunEarthForce,
	[BATTLE_EFFECT_GUARD_AURA - 1] = btEffectRunGuardAura,
	[BATTLE_EFFECT_DJINN_BLAST - 1] = btEffectRunDjinnBlast,
	[BATTLE_EFFECT_DJINN_STORM - 1] = btEffectRunDjinnStorm,
	[BATTLE_EFFECT_MINE_BALL - 1] = btEffectRunMineBall,
	[BATTLE_EFFECT_ELEMENT_SWAP - 1] = btEffectRunElementSwap,
	[BATTLE_EFFECT_DJINN_STUN - 1] = btEffectRunDjinnStun,
	[BATTLE_EFFECT_MYSTIC_CALL - 1] = btEffectRunMysticCall,
	[BATTLE_EFFECT_CLARION_CRY - 1] = btEffectRunClarionCry,
	[BATTLE_EFFECT_PSY_BOOST - 1] = btEffectRunPsyBoost,
	[BATTLE_EFFECT_STRONG_HIT - 1] = btEffectRunStrongHit,
	[BATTLE_EFFECT_CHILL_BREATH - 1] = btEffectRunChillBreath,
	[BATTLE_EFFECT_STORM_BREATH_STRONG - 1] = btEffectRunStormBreathStrong,
};

/* The poses Cybele's sprites take, a pair for each pose. */
const u8 Cybele_ObjectPoses[] = {8, 10, 9, 11, 3, 4};

/* Each thorn picture's byte offset in Cybele's sprite sheet. */
const u16 Cybele_ThornPictures[] = {3920, 3932, 3947, 3955, 3970};

/* Each thorn picture's width, in pixels. */
const u8 Cybele_ThornWidths[] = {2, 5, 2, 5, 3};

/* Each thorn picture's height, in pixels. */
const u8 Cybele_ThornHeights[] = {6, 3, 4, 3, 3};

/* Each flower picture's byte offset in Cybele's sprite sheet. */
const u16 Cybele_FlowerPictures[] = {3456, 3712, 3856};

/* Each flower's size, in pixels. */
const u8 Cybele_FlowerSizes[] = {16, 12, 8};

/* The same five pictures as Cybele_ThornPictures and the two tables after it,
 */
/* as the falling chips read them: each one's byte offset in Cybele's sprite */
/* sheet. */
const u16 Cybele_ChipPictures[] = {3920, 3932, 3947, 3955, 3970};

/* Each chip picture's width, in pixels. */
const u8 Cybele_ChipWidths[] = {2, 5, 2, 5, 3};

/* Each chip picture's height, in pixels. */
const u8 Cybele_ChipHeights[] = {6, 3, 4, 3, 3};

/* Each spark picture's byte offset in Cybele's sprite sheet. */
const u16 Cybele_SparkPictures[] = {3979, 4043, 4268, 4389, 4470};

/* Each spark's size, in pixels. */
const u8 Cybele_SparkSizes[] = {8, 15, 11, 9, 5};

/* BlastSheet's seven pictures, decoded behind YellowRingSheet at */
/* CYBELE_BLAST_PICTURES: each one's byte offset from the start of Cybele's */
/* pictures. */
const u16 Cybele_BlastPictures[] = {5760,  6016,  7040, 9344,
									11648, 13952, 16256};

/* Each blast picture's side, in pixels. */
const u16 Cybele_BlastSizes[] = {16, 32, 48, 48, 48, 48, 48};

/* Each crystal's point, x and y, in pixels. */
const u8 Boreas_Points[][2] = {{57, 130}, {75, 128}, {71, 104}, {101, 130},
							   {86, 126}, {85, 98},	 {64, 98},	{98, 96},
							   {77, 87},  {84, 88},	 {68, 67},	{81, 49},
							   {94, 70},  {70, 51},	 {93, 36},	{81, 50}};

/* The shake after the block lands, in pixels, one a frame: y moves by it and x
 */
/* by as much the other way. */
const s8 Boreas_ShakeOffsets[] = {-4, -3, -2, 0};

/* Each crystal picture's byte offset in Boreas's sprite sheet. */
const u16 Boreas_CrystalPictures[] = {0, 1152, 2064};

/* Each crystal picture's width, in pixels. */
const u8 Boreas_CrystalWidths[] = {24, 24, 20};

/* Each crystal picture's height, in pixels. */
const u8 Boreas_CrystalHeights[] = {48, 38, 26};

/* The animations Boreas's sprites cycle through, one every four frames. */
const u8 Boreas_Animations[] = {2, 9, 10, 11};

/* Each debris picture's width, in pixels. */
const u8 Boreas_DebrisWidths[] = {12, 11, 8, 10, 8, 12, 5, 7, 7, 6, 7};

/* Each debris picture's height, in pixels. */
const u8 Boreas_DebrisHeights[] = {10, 16, 14, 15, 4, 4, 3, 4, 6, 12, 10};

/* Each debris picture's byte offset in Boreas's sprite sheet. */
const u16 Boreas_DebrisPictures[] = {2584, 2704, 2880, 2992, 5062, 5094,
									 5142, 5157, 5185, 5227, 5299};

/* Each spray picture's width, in pixels. */
const u8 Boreas_SprayWidths[] = {16, 16, 16};

/* Each spray picture's height, in pixels. */
const u8 Boreas_SprayHeights[] = {16, 16, 16};

/* Each spray picture's byte offset in Boreas's sprite sheet. */
const u16 Boreas_SprayPictures[] = {4294, 4550, 4806};

/* The same three ice block pictures as Boreas_CrystalPictures and the two */
/* tables after it, as the rising crystals read them: each one's byte offset in
 */
/* Boreas's sprite sheet. */
const u16 Boreas_RisingCrystalPictures[] = {0, 1152, 2064};

/* Each ice block picture's width, in pixels. */
const u8 Boreas_RisingCrystalWidths[] = {24, 24, 20};

/* Each ice block picture's height, in pixels. */
const u8 Boreas_RisingCrystalHeights[] = {48, 38, 26};

/* The same three ice block pictures as Boreas_CrystalPictures and the two */
/* tables after it, as the falling crystals read them: each one's width, in */
/* pixels. */
const u8 Boreas_FallWidths[] = {24, 24, 20};

/* Each ice block picture's height, in pixels. */
const u8 Boreas_FallHeights[] = {48, 38, 26};

/* Each ice block picture's byte offset in Boreas's sprite sheet. */
const u16 Boreas_FallPictures[] = {0, 1152, 2064};

/* The gravities Drain's motes fall with, in pixels a frame each frame. */
const s32 Drain_Gravities[] = {toFIXED(0.25), toFIXED(0.125), toFIXED(0.25),
							   toFIXED(0.5)};

/* The mote's width, in pixels, by mode; it is drawn twice as tall. */
const u8 DrainMotes_Sizes[] = {6, 6, 10};

/* Each variant's timing: its frames, the frames each rock pillar rises, and */
/* the screen shake's range. */
const u8 Quake_Timings[][3] = {{80, 32, 2}, {112, 64, 4}, {144, 96, 8}};

/* Fire's embers, by variant; the effect lasts 132 frames and half this many */
/* more. */
const u8 Fire_Counts[] = {16, 32, 64};

/* The ember streak's pictures again, as Fire's embers read them: the same as */
/* BattleEffect_EmberStreakPictureWidths and the three tables after it, Fire */
/* keeping its own copy among its tables. Each picture's width, in pixels. */
const u8 Fire_EmberStreakWidths[] = {24, 29, 30, 27, 24, 16};

/* Each ember streak picture's height, in pixels. */
const u8 Fire_EmberStreakHeights[] = {23, 32, 37, 49, 53, 52};

/* How far below its anchor each ember streak picture's top goes, in pixels. */
const u8 Fire_EmberStreakBiasY[] = {41, 32, 26, 11, 4, 2};

/* Each ember streak picture's byte offset in the sheet. */
const u16 Fire_EmberStreakPictures[] = {0, 552, 1480, 2590, 3913, 5185};

/* Where each frost mound rises, x and its foot's y, in pixels. */
const u8 Frost_MoundSpots[] = {53, 108, 73, 110, 29, 110, 73, 108, 33, 104,
							   96, 106, 8,	109, 62, 98,  33, 100, 87, 100,
							   21, 95,	53, 90,	 33, 90,  72, 91,  82, 91,
							   77, 86,	52, 84,	 18, 92,  56, 81};

/* Each variant's timing, three bytes (FROST_TIMING_MOTES, */
/* FROST_TIMING_COLUMNS, FROST_TIMING_BURST_FRAME). */
const u8 Frost_Timings[] = {32, 8, 85, 48, 12, 97, 64, 19, 109};

/* IceChipSheet's eight chips, after its three mounds, as Frost's motes read */
/* them: each one's width, in pixels. */
const u8 Frost_MoteWidths[] = {12, 10, 7, 10, 8, 8, 8, 8};

/* Each ice chip picture's height, in pixels. */
const u8 Frost_MoteHeights[] = {9, 16, 14, 15, 8, 8, 8, 8};

/* Each ice chip picture's byte offset in the sheet. */
const u16 Frost_MoteCells[] = {1792, 1900, 2060, 2158, 2308, 2372, 2436, 2500};

/* Each mound picture's byte offset in Frost's sprite sheet. */
const u16 Frost_MoundPictures[] = {0, 768, 1280};

/* Each mound picture's height, in pixels. */
const u8 Frost_MoundHeights[] = {24, 16, 16};

/* The same eight ice chip pictures as Frost_MoteWidths and the two tables */
/* after it, as Frost's chips read them: each one's width, in pixels. */
const u8 Frost_ChipWidths[] = {12, 10, 7, 10, 8, 8, 8, 8};

/* Each ice chip picture's height, in pixels. */
const u8 Frost_ChipHeights[] = {9, 16, 14, 15, 8, 8, 8, 8};

/* Each ice chip picture's byte offset in the sheet. */
const u16 Frost_ChipCells[] = {1792, 1900, 2060, 2158, 2308, 2372, 2436, 2500};

/* Each variant's sparks: a byte nothing reads, then the sparks each bolt */
/* throws a frame. */
const u8 Ray_Sparks[][2] = {{2, 4}, {2, 6}, {4, 8}};

/* Eruption's counts, by variant. */
const u8 Eruption_Counts[][3] = {{16, 8, 72}, {32, 16, 80}, {128, 32, 88}};

/* BlastSheet's seven pictures, an explosion flaring up and fading to smoke: */
/* each one's byte offset in the sheet. */
const u16 Eruption_PuffPictures[] = {0, 256, 1280, 3584, 5888, 8192, 10496};

/* Each blast picture's side, in pixels; each is square. */
const u16 Eruption_PuffSizes[] = {16, 32, 48, 48, 48, 48, 48};

/* The flips a glint may be drawn with, one picked at random. */
const u8 BurstShower_GlintFlips[] = {0, 4, 8, 12};

/* Atalanta's sprites' scale, one to one. */
const s32 Atalanta_UnitScale[] = {toFIXED(1), toFIXED(1)};

/* Each vine picture's width, in pixels. */
const u8 Growth_VineWidths[] = {21, 28, 19};

/* The tallest a column grows, in pixels, by column in threes. */
const u8 Growth_HeightLimits[] = {119, 119, 102};

/* Each vine picture's byte offset in Growth's sprite sheet. */
const u16 Growth_VinePictures[] = {0, 2499, 5831};

/* Each pillar picture's width, in pixels. */
const u8 Growth_PillarWidths[] = {10, 8, 9};

/* Each pillar picture's byte offset in Growth's sprite sheet. */
const u16 Growth_PillarPictures[] = {0, 1080, 1888};

/* The columns, by strength. */
const u8 Growth_ColumnCounts[] = {4, 8, 12};

/* Each column's root, its x in the canvas, in pixels. */
const u8 Growth_ColumnX[] = {64, 72,  56, 80, 48, 88, 40, 96,
							 32, 104, 24, 72, 56, 80, 48, 88};

/* Each pillar picture's height, in pixels. */
const s8 Growth_PillarHeights[] = {100, 60, 88, 96, 64, 104, 82, 98};

/* Each puff picture's width, in pixels. */
const s8 Growth_PuffWidths[] = {5, 7, 11, 16, 23, 27, 16};

/* Each puff picture's height, in pixels. */
const u8 Growth_PuffHeights[] = {10, 14, 21, 32, 45, 50, 32};

/* Each dust puff picture's byte offset in Growth's sprite sheet. */
const u16 Growth_PuffPictures[] = {0, 50, 148, 379, 891, 1926, 3276};

/* Each vine's x about the middle of the targets, in pixels. */
const s8 Vine_ColumnOffsets[] = {0,	  14, -32, 8,	32, -48, 4,	  12,
								 -24, 24, -12, -40, -4, -16, -32, 12};

/* Each thorn's kind: 0 and 1 large, 2 and 3 small; the odd ones grow to the */
/* left. */
const u8 Thorn_Kinds[] = {0, 3, 2, 1, 2, 3, 2, 1};

/* Each thorn's root, its x in the canvas, in pixels. */
const s8 Thorn_X[] = {24, 80, 24, 48, 88, 40, 8, 96};

/* The thorns, by strength. */
const u8 Thorn_Counts[] = {3, 5, 8};

/* Each thorn puff picture's width, in pixels. */
const s8 Thorn_PuffWidths[] = {5, 7, 11, 17, 32, 32, 19};

/* Each dust puff picture's height, in pixels, as a thorn bursts out. */
const u8 Thorn_PuffHeights[] = {10, 14, 21, 34, 59, 60, 38};

/* Each thorn puff picture's byte offset in Growth's sprite sheet. */
const u16 Thorn_PuffPictures[] = {5888, 5938, 6036, 6267, 6845, 8733, 10653};

/* Four bytes for each strength: the sparks, the glints, the glints' spread and
 */
/* the bolts. */
const u8 Bolt_Counts[] = {16, 0, 15, 1, 32, 3, 31, 1, 128, 5, 48, 3};

/* The flips a glint may be drawn with, one picked at random. */
const u8 Bolt_GlintFlips[] = {0, 4, 8, 12};

/* How the glints are blended, by variant. */
const u8 Bolt_GlintModes[] = {1, 1, 0};

/* How tall each kind of comet is drawn above its point, in pixels. */
const u8 ProjectileVolley_CometHeights[] = {24, 36, 48};

/* The projectile volley's modes, one row a mode (PROJECTILE_VOLLEY_MODE), */
/* one byte a column (VOLLEY_FIELD): the shots at each target, the frames */
/* a shot takes to reach it, the frames between shots and between targets, and
 */
/* the sparks each hit throws. */
const u8 ProjectileVolley_Modes[][PROJECTILE_VOLLEY_COLUMNS] = {
	[PROJECTILE_VOLLEY_SLASH] = {[PROJECTILE_VOLLEY_SHOTS] = 6,
								 [PROJECTILE_VOLLEY_STEPS] = 10,
								 [PROJECTILE_VOLLEY_STAGGER] = 3,
								 [PROJECTILE_VOLLEY_DELAY] = 32,
								 [PROJECTILE_VOLLEY_SPARKS] = 4}, /* Slash */
	[PROJECTILE_VOLLEY_SHURIKEN] = {[PROJECTILE_VOLLEY_SHOTS] = 4,
									[PROJECTILE_VOLLEY_STEPS] = 10,
									[PROJECTILE_VOLLEY_STAGGER] = 5,
									[PROJECTILE_VOLLEY_DELAY] = 32,
									[PROJECTILE_VOLLEY_SPARKS] =
										8}, /* Shuriken */
	[PROJECTILE_VOLLEY_CRAZY_VOICE] = {[PROJECTILE_VOLLEY_SHOTS] = 10,
									   [PROJECTILE_VOLLEY_STEPS] = 32,
									   [PROJECTILE_VOLLEY_STAGGER] = 3,
									   [PROJECTILE_VOLLEY_DELAY] = 32,
									   [PROJECTILE_VOLLEY_SPARKS] =
										   4}, /* Crazy Voice */
	[PROJECTILE_VOLLEY_WING_FLUTTER] = {[PROJECTILE_VOLLEY_SHOTS] = 5,
										[PROJECTILE_VOLLEY_STEPS] = 10,
										[PROJECTILE_VOLLEY_STAGGER] = 5,
										[PROJECTILE_VOLLEY_DELAY] = 32,
										[PROJECTILE_VOLLEY_SPARKS] =
											4}, /* Wing Flutter */
	[PROJECTILE_VOLLEY_WING_BEAT] = {[PROJECTILE_VOLLEY_SHOTS] = 10,
									 [PROJECTILE_VOLLEY_STEPS] = 10,
									 [PROJECTILE_VOLLEY_STAGGER] = 1,
									 [PROJECTILE_VOLLEY_DELAY] = 32,
									 [PROJECTILE_VOLLEY_SPARKS] =
										 4}, /* Wing Beat */
	[PROJECTILE_VOLLEY_CUTTING_EDGE] = {[PROJECTILE_VOLLEY_SHOTS] = 1,
										[PROJECTILE_VOLLEY_STEPS] = 5,
										[PROJECTILE_VOLLEY_STAGGER] = 0,
										[PROJECTILE_VOLLEY_DELAY] = 32,
										[PROJECTILE_VOLLEY_SPARKS] =
											80}, /* Cutting Edge */
	[PROJECTILE_VOLLEY_HEAT_WAVE] = {[PROJECTILE_VOLLEY_SHOTS] = 7,
									 [PROJECTILE_VOLLEY_STEPS] = 8,
									 [PROJECTILE_VOLLEY_STAGGER] = 2,
									 [PROJECTILE_VOLLEY_DELAY] = 32,
									 [PROJECTILE_VOLLEY_SPARKS] =
										 40}, /* Heat Wave */
	[PROJECTILE_VOLLEY_SLEET] = {[PROJECTILE_VOLLEY_SHOTS] = 30,
								 [PROJECTILE_VOLLEY_STEPS] = 24,
								 [PROJECTILE_VOLLEY_STAGGER] = 1,
								 [PROJECTILE_VOLLEY_DELAY] = 32,
								 [PROJECTILE_VOLLEY_SPARKS] = 4}, /* Sleet */
	[PROJECTILE_VOLLEY_IMPACT_COMETS] = {[PROJECTILE_VOLLEY_SHOTS] = 6,
										 [PROJECTILE_VOLLEY_STEPS] = 2,
										 [PROJECTILE_VOLLEY_STAGGER] = 3,
										 [PROJECTILE_VOLLEY_DELAY] = 32,
										 [PROJECTILE_VOLLEY_SPARKS] =
											 4}, /* a casting impact's comets */
	[PROJECTILE_VOLLEY_BLIZZARD] = {[PROJECTILE_VOLLEY_SHOTS] = 60,
									[PROJECTILE_VOLLEY_STEPS] = 10,
									[PROJECTILE_VOLLEY_STAGGER] = 1,
									[PROJECTILE_VOLLEY_DELAY] = 32,
									[PROJECTILE_VOLLEY_SPARKS] =
										4}, /* Blizzard */
	[PROJECTILE_VOLLEY_ICE_BLESSING] = {[PROJECTILE_VOLLEY_SHOTS] = 60,
										[PROJECTILE_VOLLEY_STEPS] = 12,
										[PROJECTILE_VOLLEY_STAGGER] = 1,
										[PROJECTILE_VOLLEY_DELAY] = 32,
										[PROJECTILE_VOLLEY_SPARKS] =
											4}, /* Ice Blessing */
	[PROJECTILE_VOLLEY_WING_STROKE] = {[PROJECTILE_VOLLEY_SHOTS] = 10,
									   [PROJECTILE_VOLLEY_STEPS] = 10,
									   [PROJECTILE_VOLLEY_STAGGER] = 1,
									   [PROJECTILE_VOLLEY_DELAY] = 32,
									   [PROJECTILE_VOLLEY_SPARKS] =
										   4}, /* Wing Stroke */
	[PROJECTILE_VOLLEY_BLAZE_RUSH] = {[PROJECTILE_VOLLEY_SHOTS] = 7,
									  [PROJECTILE_VOLLEY_STEPS] = 8,
									  [PROJECTILE_VOLLEY_STAGGER] = 2,
									  [PROJECTILE_VOLLEY_DELAY] = 32,
									  [PROJECTILE_VOLLEY_SPARKS] =
										  40}, /* Blaze Rush */
	[PROJECTILE_VOLLEY_HYDRO_SLASH] = {[PROJECTILE_VOLLEY_SHOTS] = 20,
									   [PROJECTILE_VOLLEY_STEPS] = 12,
									   [PROJECTILE_VOLLEY_STAGGER] = 1,
									   [PROJECTILE_VOLLEY_DELAY] = 32,
									   [PROJECTILE_VOLLEY_SPARKS] =
										   40}, /* Hydro Slash */
	[PROJECTILE_VOLLEY_DIAMOND_DUST] = {[PROJECTILE_VOLLEY_SHOTS] = 30,
										[PROJECTILE_VOLLEY_STEPS] = 24,
										[PROJECTILE_VOLLEY_STAGGER] = 2,
										[PROJECTILE_VOLLEY_DELAY] = 32,
										[PROJECTILE_VOLLEY_SPARKS] =
											8}, /* Diamond Dust */
	[PROJECTILE_VOLLEY_BEAST_NEEDLE] = {[PROJECTILE_VOLLEY_SHOTS] = 30,
										[PROJECTILE_VOLLEY_STEPS] = 16,
										[PROJECTILE_VOLLEY_STAGGER] = 1,
										[PROJECTILE_VOLLEY_DELAY] = 32,
										[PROJECTILE_VOLLEY_SPARKS] =
											8}, /* Beast Needle */
	[PROJECTILE_VOLLEY_POISON_STING] = {[PROJECTILE_VOLLEY_SHOTS] = 30,
										[PROJECTILE_VOLLEY_STEPS] = 16,
										[PROJECTILE_VOLLEY_STAGGER] = 1,
										[PROJECTILE_VOLLEY_DELAY] = 32,
										[PROJECTILE_VOLLEY_SPARKS] =
											8}, /* Poison Sting */
	[PROJECTILE_VOLLEY_STUN_STING] = {[PROJECTILE_VOLLEY_SHOTS] = 30,
									  [PROJECTILE_VOLLEY_STEPS] = 16,
									  [PROJECTILE_VOLLEY_STAGGER] = 1,
									  [PROJECTILE_VOLLEY_DELAY] = 32,
									  [PROJECTILE_VOLLEY_SPARKS] =
										  8}, /* Stun Sting */
	[PROJECTILE_VOLLEY_CHILL_BREATH] =
		{[PROJECTILE_VOLLEY_SHOTS] = 60,
		 [PROJECTILE_VOLLEY_STEPS] = 12,
		 [PROJECTILE_VOLLEY_STAGGER] = 1,
		 [PROJECTILE_VOLLEY_DELAY] = 32,
		 [PROJECTILE_VOLLEY_SPARKS] = 4}, /* Chill Breath */
};

/* Each flash picture's width, in pixels. */
const u8 ProjectileVolley_FlashWidths[] = {12, 18, 24, 30, 36, 39, 40};

/* Each flash picture's height, in pixels. */
const u8 ProjectileVolley_FlashHeights[] = {10, 15, 18, 22, 27, 28, 30};

/* How far each step of the flash spreads, in pixels. */
const u8 ProjectileVolley_FlashSteps[] = {2, 3, 4, 4, 5, 6, 6};

/* How far each step of the flash rises, in pixels. */
const u8 ProjectileVolley_FlashRises[] = {0, 3, 5, 7, 10, 12, 12, 0};

/* Each flash picture's byte offset in Projectile Volley's sprite sheet. */
const u16 ProjectileVolley_FlashPictures[] = {8192, 8312,  8582, 9014,
											  9674, 10646, 11738};

/* The four small ice chip pictures, as Hail_Pictures gives them, the volley */
/* keeping its own copy: their byte offsets in the sheet. */
const u16 ProjectileVolley_PelletPictures[] = {2308, 2372, 2436, 2500};

/* The glints' flips, one picked at random. */
const u8 ProjectileVolley_GlintFlips[] = {0, 4, 8, 12};

/* The frames the Djinni throws its five flames on. */
const u8 DjinnAttacks_LaunchFrames[] = {4, 7, 10, 21, 34};

/* IceChipSheet's last four pictures, the small 8x8 ice chips, as Hail reads */
/* them: their widths, in pixels; the tables after it give their heights and */
/* offsets. */
const u8 Hail_PictureWidths[] = {8, 8, 8, 8};

/* Each ice chip picture's height, in pixels. */
const u8 Hail_PictureHeights[] = {8, 8, 8, 8};

/* Each ice chip picture's byte offset in the sheet, as Frost_MoteCells' last */
/* four give them. */
const u16 Hail_Pictures[] = {2308, 2372, 2436, 2500};

/* The chips' flips, a blitter's flags; the tables after it give, by shape, */
/* their pictures, places and sizes. */
const u8 Ground_CrackleFlips[] = {3, 7, 11, 15};

/* Each portal picture's width, in pixels. */
const u8 Ground_PortalWidths[] = {24, 7, 7, 8, 6, 13, 10, 11, 7};

/* Each portal picture's height, in pixels. */
const u8 Ground_PortalHeights[] = {48, 13, 14, 24, 24, 17, 20, 14, 20};

/* Each portal picture's byte offset in Hail's sprite sheet. */
const u16 Ground_PortalPictures[] = {0,	   1152, 1243, 1341, 1533,
									 1677, 1898, 2098, 2252};

/* Each portal's x, in pixels. */
const u8 Ground_PortalX[] = {0, 9, 9, 11, 10, 0, 3, 13, 7};

/* Each portal's y, in pixels. */
const u8 Ground_PortalY[] = {0, 18, 18, 0, 24, 20, 5, 22, 26};

/* The palettes a struck target flashes in, one picked at random. */
const u8 Thor_TargetPalettes[] = {7, 14, 10, 4};

/* The flips a crackle may be drawn with, one picked at random. */
const u8 Thor_CrackleFlips[] = {0, 4, 8, 12};

/* Each shard's place in the boulder, x and y, in pixels. */
const u8 Spire_ShardOffsets[] = {
	65, 104, 68, 95, 64, 93, 61, 93, 64, 87, 72, 85, 56, 88, 51, 80,
	61, 75,	 72, 81, 69, 71, 66, 70, 71, 64, 49, 71, 59, 69, 53, 68,
	78, 73,	 55, 67, 75, 62, 61, 65, 53, 61, 0,	 0,	 0,	 0,	 0,	 0,
	0,	0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0};

/* Each boulder's x about the middle of the canvas, in pixels. */
const s8 Spire_RockX[] = {0, -12, 12, -8, 16};

/* The frame each boulder is dropped on; it lands SPIRE_FALL_FRAMES later. */
const u8 Spire_DropFrames[] = {8, 16, 20, 24, 32};

/* The boulders, by variant. */
const u8 Spire_RockCounts[] = {1, 3, 5};

/* A flurry's style, one row a mode (FLURRY_MODE), one byte a column (enum
 */
/* FLURRY_COLUMN). */
const u8 Flurry_Styles[][8] = {
	{1, 0, 5, 16, 12, 80, 0, 0},  {1, 1, 7, 16, 12, 104, 0, 0},
	{1, 2, 9, 16, 12, 128, 0, 0}, {1, 3, 1, 16, 16, 36, 0, 0},
	{1, 3, 2, 20, 16, 52, 0, 0},  {1, 3, 3, 24, 16, 68, 0, 0},
	{0, 2, 1, 32, 12, 28, 1, 0},  {0, 2, 2, 32, 12, 40, 1, 0},
	{0, 2, 3, 32, 12, 52, 1, 0},  {1, 0, 3, 16, 12, 50, 0, 0},
	{1, 0, 1, 16, 12, 40, 0, 1},  {1, 0, 1, 16, 12, 80, 0, 2},
	{1, 3, 1, 16, 16, 30, 0, 0}};

/* A glint's four flips. */
const u8 Flurry_GlintFlips[] = {0, 4, 8, 12, 0};

/* Each row picture's byte offset in Gaia's sprite sheet. */
const u16 Gaia_RowPictures[] = {0, 160, 352, 640, 1024, 1504, 2080, 2752, 3776};

/* The growing wall's nine rows: their widths, in pixels. */
const u8 Gaia_RowWidths[] = {5, 6, 9, 12, 15, 18, 21, 32, 40};

/* The flame, EarthWallSheet's three half walls of flame as */
/* DjinnBeams_HeadPictures has them: their byte offsets where Gaia decodes the
 */
/* sheet. */
const u16 Gaia_FlamePictures[] = {10672, 11505, 12550};

/* The flame's three pictures: their widths, in pixels. */
const u8 Gaia_FlameWidths[] = {17, 19, 20};

/* The flame's three pictures: their heights, in pixels. */
const u8 Gaia_FlameHeights[] = {49, 55, 64};

/* The wall layer's shift, in pixels, by side and variant. */
const s8 Gaia_Bg2Shifts[][3] = {{-8, 0, 12}, {-72, -40, -16}};

/* The x of each rock that falls late, in pixels. */
const u8 Gaia_LateRockColumns[] = {136, 40, 120, 56, 104, 72};

/* Each sprite's x, in pixels. */
const u8 Ulysses_ObjectX[] = {0, 32, 0, 32, 0, 32, 32};

/* Each sprite's depth, in pixels. */
const u8 Ulysses_ObjectZ[] = {8, 0, 40, 32, 72, 64, 96};

/* Each Ulysses picture's width, in pixels. */
const u8 Ulysses_PictureWidths[] = {12, 9, 6,  5,  8,  9, 12, 9, 8,
									8,	8, 10, 11, 10, 8, 6,  6, 9};

/* Each Ulysses picture's height, in pixels. */
const u8 Ulysses_PictureHeights[] = {18, 17, 15, 12, 14, 15, 16, 13, 8,
									 4,	 4,	 6,	 9,	 9,	 10, 10, 13, 16};

/* Each Ulysses picture's byte offset in Ulysses's sprite sheet. */
const u16 Ulysses_Pictures[] = {0,	  216,	369,  459,	519,  631,
								766,  958,	1075, 1139, 1171, 1203,
								1263, 1362, 1452, 1532, 1592, 1670};

/* Where each of the six particles starts: x and y in pixels, z in 1024ths of */
/* a pixel, and its x and y speeds in halves of a pixel a frame. */
const s8 Ulysses_ParticleStarts[][5] = {
	{100, 60, 64, -6, -6}, {112, 44, -64, -6, 4}, {96, 28, 64, -6, -5},
	{112, 80, -64, -6, 4}, {104, 32, 64, -6, 5},  {102, 68, -64, -6, 4}};

/* Each Swirl Swipe picture's width, in pixels. */
const u8 SwirlSwipe_PictureWidths[] = {40, 112, 112, 112, 88, 64};

/* Each Swirl Swipe picture's height, in pixels. */
const u8 SwirlSwipe_PictureHeights[] = {56, 80, 64, 64, 80, 88};

/* The swirl's six pictures: their byte offsets; the tables after it give their
 */
/* x (six for each side), widths, rise and heights. */
const u16 SwirlSwipe_Pictures[] = {0, 2240, 11200, 18368, 0, 7040};

/* Each picture's x, in pixels. */
const u8 SwirlSwipe_PictureX[] = {72, 0, 0, 0, 32, 64, 16, 16, 16, 16, 8, 0};

/* How far below its anchor each picture's top goes, in pixels. */
const s8 SwirlSwipe_PictureBiasY[] = {0, 4, 24, -6, -20, -28};

/* Each part's point, x and y, in pixels. */
const u8 Azul_PartPoints[][2] = {
	{23, 13},	{0, 0},		{42, 0},	{28, 41},	{28, 41},	{48, 28},
	{42, 0},	{78, 31},	{28, 41},	{28, 41},	{66, 7},	{42, 0},
	{79, 0},	{78, 31},	{78, 31},	{94, 8},	{79, 0},	{128, 0},
	{78, 31},	{78, 31},	{112, 28},	{128, 0},	{132, 48},	{78, 31},
	{78, 31},	{162, 12},	{182, 0},	{174, 42},	{128, 0},	{128, 0},
	{145, 39},	{128, 0},	{174, 42},	{134, 71},	{134, 71},	{196, 12},
	{182, 0},	{239, 0},	{174, 42},	{172, 42},	{231, 20},	{239, 0},
	{239, 52},	{211, 16},	{211, 16},	{7, 33},	{0, 0},		{28, 41},
	{0, 48},	{0, 48},	{59, 49},	{78, 31},	{69, 73},	{28, 41},
	{28, 41},	{91, 51},	{78, 31},	{132, 48},	{69, 73},	{69, 73},
	{16, 58},	{28, 41},	{20, 86},	{0, 48},	{0, 48},	{36, 70},
	{28, 41},	{69, 73},	{20, 86},	{20, 86},	{96, 76},	{132, 48},
	{99, 97},	{69, 73},	{69, 73},	{6, 80},	{0, 48},	{20, 86},
	{0, 98},	{0, 98},	{168, 66},	{174, 42},	{205, 88},	{134, 71},
	{134, 71},	{194, 48},	{211, 16},	{205, 88},	{174, 42},	{174, 42},
	{217, 52},	{211, 16},	{239, 52},	{205, 88},	{208, 88},	{7, 114},
	{0, 98},	{30, 119},	{0, 119},	{0, 119},	{29, 106},	{20, 86},
	{64, 119},	{30, 119},	{0, 98},	{51, 93},	{69, 73},	{64, 119},
	{20, 86},	{20, 86},	{78, 96},	{69, 73},	{99, 97},	{64, 119},
	{64, 119},	{98, 112},	{99, 97},	{137, 104}, {112, 119}, {64, 119},
	{123, 85},	{132, 48},	{137, 104}, {99, 97},	{99, 97},	{156, 88},
	{134, 71},	{205, 88},	{137, 104}, {137, 104}, {139, 115}, {137, 104},
	{175, 119}, {112, 119}, {112, 119}, {217, 52},	{211, 16},	{239, 52},
	{205, 88},	{205, 88},	{228, 87},	{239, 52},	{239, 119}, {205, 88},
	{205, 88},	{200, 108}, {185, 93},	{205, 88},	{239, 119}, {175, 119},
	{169, 106}, {185, 93},	{175, 119}, {137, 104}, {137, 104}};

/* The serpent's sprites' scale, one to one. */
const SCALE Azul_UnitScale = {FIXED_ONE, FIXED_ONE};

/* Each sprite's x, in pixels. */
const u8 Azul_ObjectX[] = {35, 45, 14, 1, 1, 40, 9, 40, 9, 25, 39, 8};

/* Each sprite's depth, in pixels. */
const u8 Azul_ObjectZ[] = {0, 32, 32, 64, 96, 51, 51, 83, 83, 106, 129, 129};

/* The late sprites' scale, one to one. */
const SCALE Azul_LateUnitScale = {FIXED_ONE, FIXED_ONE};

/* Each late sprite's x, in pixels. */
const s8 Azul_LateObjectX[] = {0, 32, 32, 64, 96, 50, 51, 82};

/* Each late sprite's depth, in pixels. */
const s8 Azul_LateObjectZ[] = {10, 0, 32, 45, 45, 5, 36, 5};

/* The same six slash pictures as TargetStrike_Widths and the tables after it,
 */
/* where the beam decodes BeamImage: each one's width, in pixels. */
const u8 Beam_PictureWidths[] = {38, 54, 71, 74, 77, 65};

/* Each slash picture's height, in pixels. */
const u8 Beam_PictureHeights[] = {7, 9, 11, 13, 13, 11};

/* Each slash picture's byte offset in the beam's pictures. */
const u16 Beam_Pictures[] = {0, 266, 752, 1533, 2495, 3496};

/* How far each slash picture reaches past its middle, in pixels. */
const u8 Beam_PictureReaches[] = {58, 42, 25, 13, 5, 0};

/* The flips a glint may be drawn with, one picked at random. */
const u8 CastingImpact_SquallGlintFlips[] = {0, 4, 8, 12};

/* The flips a glint may be drawn with, one picked at random. */
const u8 CastingImpact_SapGlintFlips[] = {0, 4, 8, 12};

/* The fangs' picture for each two of their first eight frames, by number. */
const u8 CastingImpact_BitePictures[] = {0, 1, 2, 3};

/* Each Raiden's Wrath picture's byte offset in the casting impacts' sprite */
/* sheet. */
const u16 CastingImpact_RaidensWrathPictures[] = {
	8448, 8712, 11462, 16646, 16646, 24351, 24351, 28201, 28201, 28201};

/* Each Raiden's Wrath picture's width, in pixels. */
const u16 CastingImpact_RaidensWrathWidths[] = {24, 55, 64, 115, 115,
												70, 70, 22, 22,	 22};

/* Each Raiden's Wrath picture's height, in pixels. */
const u16 CastingImpact_RaidensWrathHeights[] = {11, 50, 81, 67, 67,
												 55, 55, 29, 29, 29};

/* Each Raiden's Wrath's x, in pixels. */
const u16 CastingImpact_RaidensWrathX[] = {58, 19, 0,	6,	 6,
										   50, 50, 100, 100, 100};

/* Each Raiden's Wrath's y, in pixels. */
const u16 CastingImpact_RaidensWrathY[] = {17, 19, 24, 44, 44,
										   42, 42, 37, 37, 37};

/* Each Apocalypse picture's byte offset in the casting impacts' sprite sheet.
 */
const u16 CastingImpact_ApocalypsePictures[] = {5700,  7128,  10414, 14089,
												19152, 23148, 26752};

/* Each Apocalypse picture's width, in pixels. */
const u16 CastingImpact_ApocalypseWidths[] = {42, 62, 75, 83, 54, 68, 37};

/* Each Apocalypse picture's height, in pixels. */
const u16 CastingImpact_ApocalypseHeights[] = {34, 53, 49, 61, 74, 53, 28};

/* Each Apocalypse's x, in pixels. */
const u16 CastingImpact_ApocalypseX[] = {85, 65, 50, 19, 0, 0, 58};

/* Each Apocalypse's y, in pixels. */
const u16 CastingImpact_ApocalypseY[] = {62, 66, 78, 41, 19, 0, 5};

/* Each Apocalypse burst picture's byte offset in the casting impacts' sprite */
/* sheet. */
const u16 CastingImpact_ApocalypseBurstPictures[] = {27788, 28464, 29724};

/* Each Apocalypse burst picture's width, in pixels. */
const u8 CastingImpact_ApocalypseBurstWidths[] = {26, 35, 52};

/* Each Apocalypse burst picture's height, in pixels. */
const u8 CastingImpact_ApocalypseBurstHeights[] = {26, 36, 48};

/* Each Apocalypse burst's point, x and y, in pixels. */
const u8 CastingImpact_ApocalypseBurstPoints[][2] = {
	{37, 104}, {19, 81}, {20, 47}};

/* The four small ice chip pictures, as Hail_Pictures gives them, the casting */
/* impacts keeping their own copy: their byte offsets in the sheet. */
const u16 CastingImpact_IceChipPictures[] = {2308, 2372, 2436, 2500};

/* The flips a glint may be drawn with, one picked at random. */
const u8 CastingImpact_StunBoltGlintFlips[] = {0, 4, 8, 12};

/* The frame each Stun Jip band starts on, counted from the impact's fourth */
/* frame. */
const u8 CastingImpact_StunJipBandStarts[] = {0, 2, 9, 10};

/* How fast each Stun Jip band grows: its scale gains this many times */
/* 1500/65536 a frame. */
const u8 CastingImpact_StunJipBandGrowth[] = {5, 8, 10, 8};

/* Each Stun Jip band's height, in pixels. */
const s8 CastingImpact_StunJipBandHeights[] = {32, 16, 24, 32};

/* The frame each Armor Crush wave starts on, counted from the impact's fourth
 */
/* frame. */
const u8 CastingImpact_ArmorCrushWaveStarts[] = {0, 4, 9, 10};

/* How fast each Armor Crush wave grows: its scale gains this many times */
/* 2000/65536 a frame, from a quarter. */
const u8 CastingImpact_ArmorCrushWaveGrowth[] = {3, 4, 3, 5};

/* How much taller than wide each Armor Crush wave is drawn, multiplying its */
/* scale. */
const u8 CastingImpact_ArmorCrushWaveStretch[] = {3, 4, 5, 6};

/* The water columns' feet, x and y, in pixels: Nereid_Columns' pairs and a */
/* pair of zeros, the particle streams keeping their own copy; the first three
 */
/* are read. */
const u8 NereidCopy_Columns[] = {16, 104, 32, 104, 48, 104, 64, 104, 0, 0};

/* Each falling spark's gravity, in pixels a frame each frame; the same as */
/* Nereid_Gravity, the particle streams keeping their own copy. */
const s32 NereidCopy_Gravity[] = {toFIXED(0.5), toFIXED(0.25), toFIXED(0.25),
								  toFIXED(0.125)};

/* The twelve-sprite formation: each sprite's x, in pixels. */
const u8 SummonFormation_NereidX[] = {52, 25,  57, 89, 7,  39,
									  71, 103, 21, 53, 85, 117};

/* The twelve-sprite formation: each sprite's depth, in pixels. */
const u8 SummonFormation_NereidZ[] = {0,  32, 32, 32, 64, 64,
									  64, 64, 96, 96, 96, 96};

/* The eight-sprite formation facing right: each sprite's x, in pixels. */
const u8 SummonFormation_CybeleX[] = {0, 32, 64, 16, 48, 0, 32, 64};

/* The eight-sprite formation facing right: each sprite's depth, in pixels. */
const u8 SummonFormation_CybeleZ[] = {0, 0, 0, 32, 32, 64, 64, 64};

/* The other eight-sprite formation: each sprite's x, in pixels. */
const u8 SummonFormation_TiamatAlternateX[] = {0, 32, 64, 0, 32, 64, 32, 64};

/* The other eight-sprite formation: each sprite's depth, in pixels. */
const u8 SummonFormation_TiamatAlternateZ[] = {10, 0, 0, 42, 32, 32, 64, 64};

/* The small eight-sprite formation: each sprite's x, in pixels. */
const u8 SummonFormation_TiamatX[] = {0, 40, 80, 0, 40, 80, 40, 80};

/* The small eight-sprite formation: each sprite's depth, in pixels. */
const u8 SummonFormation_TiamatZ[] = {13, 0, 0, 53, 40, 40, 80, 80};

/* The wide formation: each sprite's x, in pixels. */
const s8 SummonFormation_MegaeraX[] = {16,	48, 80, 16, 48, 80,
									   -16, 16, 48, 80, 42};

/* The wide formation: each sprite's depth, in pixels. */
const u8 SummonFormation_MegaeraZ[] = {0, 0, 0, 32, 32, 32, 50, 64, 64, 64, 96};

/* Each impact burst picture's width, in pixels. */
const u8 ImpactBurst_PictureWidths[] = {48, 48, 46, 36, 36, 32, 26, 6};

/* Each impact burst picture's height, in pixels. */
const u8 ImpactBurst_PictureHeights[] = {96, 96, 90, 74, 74, 62, 52, 12};

/* Each impact burst picture's byte offset in Helm Splitter's sprite sheet. */
const u16 ImpactBurst_Pictures[] = {0,	   4608,  9216,	 13356,
									16020, 18684, 20668, 22020};

/* Each orbit's point, x, y and z, in pixels; x and z are halved. */
const s16 StarfieldFall_OrbitPoints[][3] = {
	{-64, -96, 24}, {-48, -120, 32}, {-32, -80, -48}, {-16, -72, -24},
	{16, -80, 16},	{32, -112, -32}, {48, -96, 56}};

/* Each fireball's point, x and y, in pixels. */
const s16 StarfieldFall_FireballPoints[][2] = {
	{32, 32}, {48, 96}, {80, 80}, {112, 100}, {176, 80}, {196, 64}, {204, 96}};

/* BlastSheet's seven pictures, as Eruption_PuffPictures gives them, the */
/* particle streams keeping their own copy: each one's byte offset in the */
/* sheet. */
const u16 StarfieldFall_FlashPictures[] = {0,	 256,  1280, 3584,
										   5888, 8192, 10496};

/* Each blast picture's side, in pixels. */
const u16 StarfieldFall_FlashSizes[] = {16, 32, 48, 48, 48, 48, 48};

/* Each of the statue's sprites: its x in the statue, in pixels. */
const u8 Ramses_StatueColumns[] = {12, 44, 28, 60, 0, 32, 64, 0, 32};

/* Each of the statue's sprites: its y in the statue, in pixels. */
const u8 Ramses_StatueRows[] = {0, 0, 32, 32, 64, 64, 64, 32, 32};

/* BlastSheet's seven pictures, as Eruption_PuffPictures gives them, Ramses */
/* keeping its own copy: each one's byte offset in the sheet. */
const u16 Ramses_PuffPictures[] = {0, 256, 1280, 3584, 5888, 8192, 10496};

/* Each blast picture's side, in pixels. */
const u16 Ramses_PuffSizes[] = {16, 32, 48, 48, 48, 48, 48};

/* The strike columns' x, in pixels, three for each side. */
const u8 Ragnarok_StrikeColumns[] = {32, 24, 0, 96, 104, 128};

/* The flash burst's picture for each three frames of a flash, by number. */
const u8 Ragnarok_FlashPictures[] = {5, 5, 4, 3, 2, 1};

/* The flash burst's picture for each three frames of a flash, by number. */
const u8 TitanBlade_FlashPictures[] = {5, 5, 4, 3, 2, 1};

/* The dust's four gravities, in pixels a frame each frame. */
const s32 TitanBlade_DustGravity[] = {toFIXED(0.5), toFIXED(0.25),
									  toFIXED(0.25), toFIXED(0.125)};

/* The sprite each 32x32 cell of the shield's sheet is copied into, in order. */
const u8 Judgment_ShieldSprites[] = {0, 1, 2,  3,  4,  5,  6,
									 7, 9, 10, 11, 13, 14, 0};

/* Each shard picture's byte offset in Judgment's sprite sheet. */
const u16 Judgment_ShardPictures[] = {1536, 2125, 2838, 3551, 3950, 4286};

/* Each shard picture's width, in pixels. */
const u8 Judgment_ShardWidths[] = {19, 23, 23, 19, 16, 17};

/* Each shard picture's height, in pixels. */
const u8 Judgment_ShardHeights[] = {31, 31, 31, 21, 21, 24};

/* Each of Kirin's sprites: its x, in pixels. */
const u8 Kirin_Columns[] = {8, 40, 72, 104, 8, 40, 72, 104, 0};

/* Each of Kirin's sprites: its y, in pixels. */
const u8 Kirin_Rows[] = {0, 0, 0, 24, 32, 32, 32, 56, 64};

/* The spray's four gravities, in pixels a frame each frame. */
const s32 Neptune_Gravity[] = {toFIXED(0.5), toFIXED(0.25), toFIXED(0.25),
							   toFIXED(0.125)};

/* BlastSheet's seven pictures, as Eruption_PuffPictures gives them, Undead */
/* Sword keeping its own copy: each one's byte offset in the sheet. */
const u16 UndeadSword_PuffPictures[] = {0, 256, 1280, 3584, 5888, 8192, 10496};

/* Each blast picture's side, in pixels; the six words after the seventh are */
/* never read. */
const u16 UndeadSword_PuffSizes[] = {16,   32,	 48,   48,	48,	  48,  48,
									 1536, 2312, 2053, 775, 8224, 8224};

/* The funnels, by mode. */
const u8 WindFunnel_Counts[] = {3, 3};

/* The frame each ripple starts on, counted from the sixth. */
const u8 DiamondBerg_RingStarts[4] = {0, 2, 4, 5};

/* How fast each ripple grows: its scale gains this many times */
/* DIAMOND_BERG_RIPPLE_GROWTH a frame. */
const u8 DiamondBerg_RingGrowth[4] = {6, 7, 6, 8};

/* Each ripple's height, in pixels. */
const s8 DiamondBerg_RingHeights[4] = {32, 16, -8, -24};

/* Each flash's roll. */
const s16 DiamondBerg_ShardRolls[8] = {
	DEGtoANG(45), DEGtoANG(135), DEGtoANG(45), DEGtoANG(135),
	DEGtoANG(45), DEGtoANG(135), DEGtoANG(45), DEGtoANG(135)};

/* Each flash's pitch. */
const s16 DiamondBerg_ShardPitches[8] = {
	-DEGtoANG(11.25), -DEGtoANG(11.25), -DEGtoANG(11.25), -DEGtoANG(11.25),
	-DEGtoANG(11.25), -DEGtoANG(11.25), -DEGtoANG(11.25), -DEGtoANG(11.25)};

/* The frame each wave starts on, counted from the crash. */
const u8 Geode_WaveStarts[] = {0, 6, 8, 9};

/* How fast each wave grows: its scale gains this many times 2000/65536 a */
/* frame, from a half. */
const u8 Geode_WaveGrowth[] = {5, 8, 7, 3};

/* Each wave's height, in pixels. */
const s8 Geode_WaveHeights[] = {32, 32, 32, 32};

/* BlastSheet's seven pictures, as Eruption_PuffPictures gives them, Soul */
/* Shatter keeping its own copy: each one's byte offset in the sheet. */
const u16 SoulShatter_SparkPictures[] = {0, 256, 1280, 3584, 5888, 8192, 10496};

/* Each blast picture's side, in pixels. */
const u16 SoulShatter_SparkSizes[] = {16, 32, 48, 48, 48, 48, 48};

/* The heights of the cone's rings, in pixels, the tip first. */
const s8 Serac_RowHeights[] = {-96, -88, -72, -64, -48, -32, -8, 0};

/* The radii of the cone's rings, in pixels. */
const s8 Serac_RowRadii[] = {1, 6, 10, 12, 16, 24, 30, 32};

/* Each ripple's delay, in frames. */
const u8 Serac_RingStarts[] = {6, 8, 10, 11};

/* How fast each ripple grows: its scale gains this many times */
/* SERAC_RING_GROWTH a frame. */
const u8 Serac_RingGrowth[] = {8, 7, 6, 3};

/* Each ripple's height, in pixels. */
const s8 Serac_RingHeights[] = {16, 32, 56, 72};

/* The frame of the strike, by mode. */
const u8 HeavyBlow_StrikeFrames[] = {38, 40};

/* The chips, by mode. */
const u8 HeavyBlow_ChipCounts[] = {128, 32};

/* A chip's two pixels. */
const u8 HeavyBlow_ChipPixels[] = {58, 58};

/* The columns, by number: their x, in pixels; the tables after it give their */
/* start frames, full heights and leans. */
const s16 Cool_ColumnX[] = {92, 72, 32, 48, 88, 56, 80, 88, -8, 132};

/* The frame each column starts on. */
const u8 Cool_ColumnStarts[] = {0, 4, 7, 10, 13, 16, 20, 23, 25, 27};

/* Each column's full height, in pixels. */
const u8 Cool_ColumnHeights[] = {64, 64, 48, 64, 64, 56, 62, 64, 46, 52};

/* Each column's lean. */
const u8 Cool_ColumnLeans[] = {0, 3, 1, 2, 3, 0, 1, 2, 1, 0};

/* BlastSheet's seven pictures, as Eruption_PuffPictures gives them, Cool */
/* keeping its own copy: each one's byte offset in the sheet. */
const u16 Cool_SparkPictures[] = {0, 256, 1280, 3584, 5888, 8192, 10496};

/* Each blast picture's side, in pixels. */
const u16 Cool_SparkSizes[] = {16, 32, 48, 48, 48, 48, 48};

/* The drops' sideways speeds as they bounce, in eighths of a pixel a frame. */
const s8 WyrdCurse_DropBounces[] = {-12, 4, -8, 14};

/* The flips a glint may be drawn with, one picked at random. */
const u8 BoltTrail_GlintFlips[] = {0, 4, 8, 12, 0, 0};

/* The flips a glint may be drawn with, one picked at random. */
const u8 Megaera_GlintFlips[] = {0, 4, 8, 12};

/* How the glints are blended, by variant. */
const u8 Megaera_GlintModes[] = {1, 1, 0, 0};

/* Each ring's full scale, in 65536ths, which its growth multiplies. */
const s32 LightSurge_RingSizes[4] = {18000, 12000, 30000, 40000};

/* Each ring's shade, before it dims. */
const s8 LightSurge_RingShades[4] = {-2, -8, -4, 0};

/* How far each ring sways, in pixels at the sway's widest. */
const s8 LightSurge_RingSways[4] = {-24, -8, 24, 48};

/* Each mode's flags for the Djinni's entrance. */
const u8 DjinniCharge_EntranceFlags[] = {2, 10, 0, 0};

/* The debris's gravities, in pixels a frame each frame: negative, so it rises.
 */
const s32 DjinniCharge_DebrisGravity[4] = {-toFIXED(1) / 8, -toFIXED(1) / 16,
										   -toFIXED(1) / 32, -toFIXED(1) / 64};

/* How far along its path the streak's head is at each step, in sixteenths. */
const s8 GaspFamily_StreakSteps[] = {0, 0, 1, 2, 4, 6, 9, 12, 16, 0, 0, 0};

/* The streak's mesh. */
const u16 GaspFamily_StreakMesh[] = {MESH_FACE(1, 3, 2, 31, 0, 31, 31, 0, 31),
									 MESH_FACE(0, 1, 2, 0, 0, 31, 0, 0, 31),
									 MESH_END, MESH_END};

/* Gasp Family's counts, by variant. */
const u8 GaspFamily_Counts[] = {32, 64, 255};

/* The frames the backdrop flashes on. */
const u8 Charon_FlashFrames[] = {16, 60, 108, 0, 0};

/* A band of sixteen corners: an octagon below and one above. */
const MODEL_VERTEX BattleEffect_OctagonBandCorners[] = {
	{0, -8, 32},  {22, -8, 22},	  {31, -8, 0},	{22, -8, -22},
	{0, -8, -31}, {-22, -8, -22}, {-31, -8, 0}, {-22, -8, 22},
	{0, 8, 32},	  {22, 8, 22},	  {31, 8, 0},	{22, 8, -22},
	{0, 8, -31},  {-22, 8, -22},  {-31, 8, 0},	{-22, 8, 22}};

/* The ripple's 32 corners: a ring of sixteen below and one above. */
const MODEL_VERTEX BattleEffect_CylinderCorners[] = {
	{0, -8, 31},  {12, -8, 29},	  {22, -8, 22},	  {29, -8, 12},
	{31, -8, 0},  {29, -8, -12},  {22, -8, -22},  {12, -8, -29},
	{0, -8, -31}, {-13, -8, -29}, {-23, -8, -22}, {-30, -8, -12},
	{-32, -8, 0}, {-30, -8, 13},  {-23, -8, 23},  {-13, -8, 30},
	{0, 8, 31},	  {12, 8, 29},	  {22, 8, 22},	  {29, 8, 12},
	{31, 8, 0},	  {29, 8, -12},	  {22, 8, -22},	  {12, 8, -29},
	{0, 8, -31},  {-13, 8, -29},  {-23, 8, -22},  {-30, 8, -12},
	{-32, 8, 0},  {-30, 8, 13},	  {-23, 8, 23},	  {-13, 8, 30}};

/* The band's mesh. */
const u16 CastingImpact_BandMesh[] = {
	MESH_FACE(1, 9, 8, 31, 0, 31, 15, 0, 15),
	MESH_FACE(0, 1, 8, 0, 0, 31, 0, 0, 15),
	MESH_FACE(2, 10, 9, 63, 0, 63, 15, 32, 15),
	MESH_FACE(1, 2, 9, 32, 0, 63, 0, 32, 15),
	MESH_FACE(3, 11, 10, 95, 0, 95, 15, 64, 15),
	MESH_FACE(2, 3, 10, 64, 0, 95, 0, 64, 15),
	MESH_FACE(4, 12, 11, 127, 0, 127, 15, 96, 15),
	MESH_FACE(3, 4, 11, 96, 0, 127, 0, 96, 15),
	MESH_FACE(5, 13, 12, 31, 0, 31, 15, 0, 15),
	MESH_FACE(4, 5, 12, 0, 0, 31, 0, 0, 15),
	MESH_FACE(6, 14, 13, 63, 0, 63, 15, 32, 15),
	MESH_FACE(5, 6, 13, 32, 0, 63, 0, 32, 15),
	MESH_FACE(7, 15, 14, 95, 0, 95, 15, 64, 15),
	MESH_FACE(6, 7, 14, 64, 0, 95, 0, 64, 15),
	MESH_FACE(0, 8, 15, 127, 0, 127, 15, 96, 15),
	MESH_FACE(7, 0, 15, 96, 0, 127, 0, 96, 15),
	MESH_END};

/* The geyser's mesh. */
const u16 Geyser_Mesh[] = {MESH_FACE(1, 9, 8, 31, 0, 31, 63, 0, 63),
						   MESH_FACE(0, 1, 8, 0, 0, 31, 0, 0, 63),
						   MESH_FACE(2, 10, 9, 63, 0, 63, 63, 32, 63),
						   MESH_FACE(1, 2, 9, 32, 0, 63, 0, 32, 63),
						   MESH_FACE(3, 11, 10, 95, 0, 95, 63, 64, 63),
						   MESH_FACE(2, 3, 10, 64, 0, 95, 0, 64, 63),
						   MESH_FACE(4, 12, 11, 127, 0, 127, 63, 96, 63),
						   MESH_FACE(3, 4, 11, 96, 0, 127, 0, 96, 63),
						   MESH_FACE(5, 13, 12, 31, 0, 31, 63, 0, 63),
						   MESH_FACE(4, 5, 12, 0, 0, 31, 0, 0, 63),
						   MESH_FACE(6, 14, 13, 63, 0, 63, 63, 32, 63),
						   MESH_FACE(5, 6, 13, 32, 0, 63, 0, 32, 63),
						   MESH_FACE(7, 15, 14, 95, 0, 95, 63, 64, 63),
						   MESH_FACE(6, 7, 14, 64, 0, 95, 0, 64, 63),
						   MESH_FACE(0, 8, 15, 127, 0, 127, 63, 96, 63),
						   MESH_FACE(7, 0, 15, 96, 0, 127, 0, 96, 63),
						   MESH_END};

/* The ripple's mesh. */
const u16 BattleEffect_CylinderMesh[] = {
	MESH_FACE(1, 17, 16, 15, 0, 15, 15, 0, 15),
	MESH_FACE(0, 1, 16, 0, 0, 15, 0, 0, 15),
	MESH_FACE(2, 18, 17, 31, 0, 31, 15, 15, 15),
	MESH_FACE(1, 2, 17, 15, 0, 31, 0, 15, 15),
	MESH_FACE(3, 19, 18, 47, 0, 47, 15, 31, 15),
	MESH_FACE(2, 3, 18, 31, 0, 47, 0, 31, 15),
	MESH_FACE(4, 20, 19, 63, 0, 63, 15, 47, 15),
	MESH_FACE(3, 4, 19, 47, 0, 63, 0, 47, 15),
	MESH_FACE(5, 21, 20, 15, 0, 15, 15, 0, 15),
	MESH_FACE(4, 5, 20, 0, 0, 15, 0, 0, 15),
	MESH_FACE(6, 22, 21, 31, 0, 31, 15, 15, 15),
	MESH_FACE(5, 6, 21, 15, 0, 31, 0, 15, 15),
	MESH_FACE(7, 23, 22, 47, 0, 47, 15, 31, 15),
	MESH_FACE(6, 7, 22, 32, 0, 47, 0, 31, 15),
	MESH_FACE(8, 24, 23, 63, 0, 63, 15, 47, 15),
	MESH_FACE(7, 8, 23, 47, 0, 63, 0, 47, 15),
	MESH_FACE(9, 25, 24, 79, 0, 79, 15, 64, 15),
	MESH_FACE(8, 9, 24, 64, 0, 79, 0, 64, 15),
	MESH_FACE(10, 26, 25, 95, 0, 95, 15, 79, 15),
	MESH_FACE(9, 10, 25, 79, 0, 95, 0, 79, 15),
	MESH_FACE(11, 27, 26, 111, 0, 111, 15, 95, 15),
	MESH_FACE(10, 11, 26, 95, 0, 111, 0, 95, 15),
	MESH_FACE(12, 28, 27, 127, 0, 127, 15, 111, 15),
	MESH_FACE(11, 12, 27, 111, 0, 127, 0, 111, 15),
	MESH_FACE(13, 29, 28, 79, 0, 79, 15, 64, 15),
	MESH_FACE(12, 13, 28, 64, 0, 79, 0, 64, 15),
	MESH_FACE(14, 30, 29, 95, 0, 95, 15, 79, 15),
	MESH_FACE(13, 14, 29, 79, 0, 95, 0, 79, 15),
	MESH_FACE(15, 31, 30, 111, 0, 111, 15, 95, 15),
	MESH_FACE(14, 15, 30, 96, 0, 111, 0, 95, 15),
	MESH_FACE(0, 16, 31, 127, 0, 127, 15, 111, 15),
	MESH_FACE(15, 0, 31, 111, 0, 127, 0, 111, 15),
	MESH_END};

/* The ring's corners, as signed x, y and z. */
const MODEL_VERTEX Geode_RingCorners[] = {
	{0, -8, 64},  {44, -8, 44},	  {62, -8, 0},	{44, -8, -44},
	{0, -8, -62}, {-44, -8, -44}, {-62, -8, 0}, {-44, -8, 44},
	{0, 0, 32},	  {22, 0, 22},	  {31, 0, 0},	{22, 0, -22},
	{0, 0, -31},  {-22, 0, -22},  {-31, 0, 0},	{-22, 0, 22}};

/* A ring's corners: eight about the edge and eight within. */
const MODEL_VERTEX BattleEffect_RingCorners[] = {
	{0, -8, 38},  {26, -8, 26},	  {37, -8, 0},	{26, -8, -26},
	{0, -8, -37}, {-26, -8, -26}, {-37, -8, 0}, {-26, -8, 26},
	{0, 0, 32},	  {22, 0, 22},	  {31, 0, 0},	{22, 0, -22},
	{0, 0, -31},  {-22, 0, -22},  {-31, 0, 0},	{-22, 0, 22}};

/* The ring's mesh. */
const u16 BattleEffect_RingMesh[] = {
	MESH_FACE(1, 9, 8, 31, 0, 31, 63, 0, 63),
	MESH_FACE(0, 1, 8, 0, 0, 31, 0, 0, 63),
	MESH_FACE(2, 10, 9, 63, 0, 63, 63, 32, 63),
	MESH_FACE(1, 2, 9, 31, 0, 63, 0, 32, 63),
	MESH_FACE(3, 11, 10, 95, 0, 95, 63, 64, 63),
	MESH_FACE(2, 3, 10, 63, 0, 95, 0, 64, 63),
	MESH_FACE(4, 12, 11, 127, 0, 127, 63, 96, 63),
	MESH_FACE(3, 4, 11, 95, 0, 127, 0, 96, 63),
	MESH_FACE(5, 13, 12, 31, 0, 31, 63, 0, 63),
	MESH_FACE(4, 5, 12, 0, 0, 31, 0, 0, 63),
	MESH_FACE(6, 14, 13, 63, 0, 63, 63, 32, 63),
	MESH_FACE(5, 6, 13, 31, 0, 63, 0, 32, 63),
	MESH_FACE(7, 15, 14, 95, 0, 95, 63, 64, 63),
	MESH_FACE(6, 7, 14, 63, 0, 95, 0, 64, 63),
	MESH_FACE(0, 8, 15, 127, 0, 127, 63, 96, 63),
	MESH_FACE(7, 0, 15, 95, 0, 127, 0, 96, 63),
	MESH_END};

/* A solid triangle's mesh, as the solid renderers read it, a word a face: the
 */
/* face of corners 0, 1 and 2, and the empty face that ends it. */
const u16 BattleEffect_TriangleMesh[] = {256, 2, 0, 0};

/* A small triangle lying on the ground: its three corners. */
const MODEL_VERTEX BattleEffect_TriangleCorners[] = {
	{-6, 0, -6}, {6, 0, -6}, {-6, 0, 6}};

/* A beam 127 tall, 12 wide at the top and 4 at its foot: its four corners. */
const MODEL_VERTEX BattleEffect_TaperedBeamCorners[] = {
	{-6, 127, 0}, {6, 127, 0}, {-2, 0, 0}, {2, 0, 0}};

/* A beam 127 tall and 12 wide standing on the origin: its four corners. */
const MODEL_VERTEX BattleEffect_BeamCorners[] = {
	{-6, 127, 0}, {6, 127, 0}, {-6, 0, 0}, {6, 0, 0}};

/* A beam 131 tall and 12 wide reaching below the origin: its four corners. */
const MODEL_VERTEX BattleEffect_LongBeamCorners[] = {
	{6, 127, 0}, {-6, 127, 0}, {6, -4, 0}, {-6, -4, 0}};

/* A 64 by 64 square about the origin: its four corners. */
const MODEL_VERTEX BattleEffect_SquareCorners[] = {
	{-32, -32, 0}, {32, -32, 0}, {-32, 32, 0}, {32, 32, 0}};

/* A 64 by 64 square standing on the origin: its four corners. */
const MODEL_VERTEX BattleEffect_StandingSquareCorners[] = {
	{-32, -64, 0}, {32, -64, 0}, {-32, 0, 0}, {32, 0, 0}};

/* The fan flame's corners, as signed x, y and z. */
const MODEL_VERTEX Legend_FanFlameCorners[] = {
	{-16, 64, -64}, {16, 64, -64}, {-16, 56, 0}, {16, 56, 0}};

/* A 128 by 128 square lying on the ground about the origin: its four corners.
 */
const MODEL_VERTEX BattleEffect_GroundSquareCorners[] = {
	{-64, 0, -64}, {64, 0, -64}, {-64, 0, 64}, {64, 0, 64}};

/* A small square's mesh. */
const u16 BattleEffect_SmallSquareMesh[] = {
	MESH_FACE(1, 3, 2, 31, 0, 31, 31, 0, 31),
	MESH_FACE(0, 1, 2, 0, 0, 31, 0, 0, 31), MESH_END};

/* The small square's mesh with its triangles wound the other way. */
const u16 BattleEffect_SmallSquareMeshReversed[] = {
	MESH_FACE(0, 1, 2, 0, 0, 31, 0, 0, 31),
	MESH_FACE(1, 3, 2, 31, 0, 31, 31, 0, 31), MESH_END};

/* A column's mesh. */
const u16 BattleEffect_ColumnMesh[] = {
	MESH_FACE(1, 3, 2, 31, 0, 31, 119, 0, 119),
	MESH_FACE(0, 1, 2, 0, 0, 31, 0, 0, 119), MESH_END};

/* The sword's mesh. */
const u16 HeavyBlow_SwordMesh[] = {MESH_FACE(0, 1, 2, 0, 0, 31, 0, 0, 127),
								   MESH_FACE(1, 3, 2, 31, 0, 31, 127, 0, 127),
								   MESH_END};

/* A tall quad's mesh. */
const u16 BattleEffect_TallMesh[] = {MESH_FACE(0, 1, 2, 0, 1, 31, 1, 0, 63),
									 MESH_FACE(1, 3, 2, 31, 1, 31, 63, 0, 63),
									 MESH_END,
									 MESH_FACE(0, 1, 2, 0, 0, 15, 0, 0, 63),
									 MESH_FACE(1, 3, 2, 15, 0, 15, 63, 0, 63),
									 MESH_END};

/* The band's mesh. */
const u16 Haures_BandMesh[] = {MESH_FACE(0, 1, 2, 0, 0, 63, 0, 0, 31),
							   MESH_FACE(1, 3, 2, 63, 0, 63, 31, 0, 31),
							   MESH_END};

/* The burst's mesh. */
const u16 Catastrophe_ForkMesh[] = {MESH_FACE(0, 1, 2, 0, 0, 31, 0, 0, 47),
									MESH_FACE(1, 3, 2, 31, 0, 31, 47, 0, 47),
									MESH_END};

/* A square's mesh. */
const u16 BattleEffect_SquareMesh[] = {MESH_FACE(1, 3, 2, 63, 1, 63, 63, 1, 63),
									   MESH_FACE(0, 1, 2, 1, 1, 63, 1, 1, 63),
									   MESH_END};

/* A large square's mesh. */
const u16 BattleEffect_LargeSquareMesh[] = {
	MESH_FACE(1, 3, 2, 127, 0, 127, 127, 0, 127),
	MESH_FACE(0, 1, 2, 0, 0, 127, 0, 0, 127), MESH_END};

/* The burst's mesh. */
const u16 Haures_BurstMesh[] = {MESH_FACE(1, 3, 2, 47, 0, 47, 71, 0, 71),
								MESH_FACE(0, 1, 2, 0, 0, 47, 0, 0, 71),
								MESH_END,
								MESH_FACE(1, 3, 2, 47, 16, 47, 47, 16, 47),
								MESH_FACE(0, 1, 2, 16, 16, 47, 16, 16, 47),
								MESH_END};

/* The spark burst's pictures: their widths; the tables after it give their */
/* heights and offsets, and the heart burn's heights, y offsets and offsets. */
const u8 Heartbreak_SparkBurstWidths[] = {32, 39, 42, 71};

/* Each spark burst picture's height, in pixels. */
const u8 Heartbreak_SparkBurstHeights[] = {29, 32, 48, 57};

/* Each spark burst picture's byte offset in Heartbreak's sprite sheet. */
const u16 Heartbreak_SparkBurstPictures[] = {0, 928, 2176, 4192};

/* Each heart burn picture's height, in pixels. */
const u8 Heartbreak_HeartBurnHeights[] = {29, 37, 48, 47, 52, 32, 22, 17};

/* Each heart burn picture's y, in pixels. */
const u8 Heartbreak_HeartBurnY[] = {34, 26, 15, 14, 7, 11, 8, 6};

/* Each heart burn picture's byte offset in Heartbreak's sprite sheet. */
const u16 Heartbreak_HeartBurnPictures[] = {0,	  928,	2112, 3648,
											5152, 6816, 7840, 8544};

/* By level (effect->variant): the sparks, their spread, the rings' size and */
/* the embers. */
const u8 RagingHeat_Styles[][4] = {
	{8, 127, 64, 32}, {12, 191, 96, 64}, {16, 255, 128, 255}};

/* Juggle's style, one row a mode. */
const u8 Juggle_Styles[][7] = {{1, 24, 4, 64, 32, 4, 0},
							   {2, 32, 8, 32, 64, 8, 0},
							   {2, 48, 16, 32, 128, 16, 0}};

/* Each petal's size, in pixels. */
const u8 Flora_PetalSizes[] = {4, 8, 16, 24, 0};

/* Each petal picture's byte offset in Flora's sprite sheet. */
const u16 Flora_PetalPictures[] = {0, 32, 160, 672};

/* The frame each ring of light starts on. */
const u16 Flora_BloomFrames[] = {114, 117, 120, 250};

/* Each ring of light's roll; the third, off the 256ths of a turn the others */
/* keep, is in 65,536ths. */
const s16 Flora_BloomRolls[] = {0, DEGtoANG(135), -28576, DEGtoANG(60.46875)};

/* Each ring of light's pitch; the third is in 65,536ths of a turn. */
const s16 Flora_BloomPitches[] = {DEGtoANG(90), DEGtoANG(22.5), 2096,
								  -DEGtoANG(39.375)};

/* Each bloom's x, in pixels. */
const s8 Flora_BloomX[] = {16, 16, 16, -16};

/* Each bloom's y, in pixels. */
const s8 Flora_BloomY[] = {0, 0, 0, 12, 0, 0};

/* The dagger's mesh. */
const u16 SabreDance_DaggerMesh[] = {MESH_FACE(0, 1, 2, 6, 0, 25, 0, 6, 31),
									 MESH_FACE(1, 3, 2, 25, 0, 25, 31, 6, 31),
									 MESH_END};

/* The dagger, 32 by 80 pixels from (-16, -16) to (16, 64), the point down: its
 */
/* four corners. */
const MODEL_VERTEX SabreDance_DaggerCorners[] = {
	{-16, -16, 0}, {16, -16, 0}, {-16, 64, 0}, {16, 64, 0}};

/* Each Fume level's row, one column a FUME_LEVEL_COLUMN. */
const u16 Fume_Levels[][FUME_COLUMNS] = {{16384, 16, 24, 96, 8, 4096},
										 {24576, 16, 48, 192, 16, 8192},
										 {40768, 32, 38, 256, 32, 16384}};

/* The early drops' gravities, in pixels a frame each frame: negative, so they
 */
/* rise. */
const s32 OceanFist_DropGravity[4] = {-toFIXED(1) / 8, -toFIXED(1) / 4,
									  -toFIXED(1) / 4, -toFIXED(1) / 2};

/* The size of the burst at the user, by mode (RUSHING_STRIKE_MODE): it is */
/* drawn twice this wide and tall. Stun Muscle's and Living Armor's are the */
/* largest. */
const s32 RushingStrike_Scales[] = {toFIXED(0.5),	  toFIXED(0.5),
									toFIXED(0.5),	  toFIXED(0.5),
									toFIXED(0.84742), toFIXED(0.80518)};

/* The wall's size, by mode (MIRRORED_WALL_MODE): it is drawn at twice */
/* this, Spasm's the smallest, Berserk's and Recovery's a ninth larger, and */
/* Sack's two fifths larger. */
const s32 MirroredWall_Scales[] = {toFIXED(0.60682), toFIXED(0.67371),
								   toFIXED(0.67371), toFIXED(0.84742)};

/* The wall's shade, by mode. */
const s32 MirroredWall_Shades[] = {-8, -8, -8, 0};

/* The shake, in pixels, one a frame of state->shakeFrm: what it moves rises
 */
/* by it. */
const s8 Daedalus_ShakeOffsets[] = {0,	-1, 1,	-1, 0,	2,	1,	0,
									-1, -1, -2, -2, -3, -3, -4, -4,
									-4, -3, -3, -3, -2, -2, -1, -1};

/* A bird's picture for each four frames of its wing beat, by number. */
const u8 Daedalus_WingBeatPictures[] = {0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1};

/* Where each bird starts, x and y in the canvas, in pixels. */
const u8 Daedalus_BirdStarts[][2] = {
	{112, 128}, {128, 112}, {112, 128}, {128, 112}, {128, 112},
	{112, 112}, {180, 120}, {140, 120}, {140, 120}, {136, 120}};

/* Each bird's speed, x and y, in 65,536ths of a pixel a frame: up and to the */
/* left. */
const s32 Daedalus_BirdSpeeds[][2] = {
	{-120000, -70000}, {-117000, -68000}, {-120000, -70000}, {-117000, -68000},
	{-80000, -80000},  {-80000, -95000},  {-100000, -80000}, {-100000, -90000},
	{-120000, -95000}, {-100000, -95000}};

/* The frame each bird appears on. */
const u8 Daedalus_BirdFrames[] = {16, 16, 40, 40, 40, 68, 40, 72, 72, 96};

/* The frames the machine clatters on. */
const u16 Daedalus_ClatterFrames[] = {16, 115, 243, 371, 499};

/* The frame each ring starts on. */
const u16 Daedalus_RingStarts[] = {568, 5};

/* Each ring's x, in pixels. */
const s8 Daedalus_RingX[] = {66, 64};

/* Each sprite of the right leg: its point, x and y, in pixels. */
const u8 Daedalus_RightLegPoints[][2] = {
	{0, 0}, {32, 0}, {0, 32}, {32, 32}, {32, 64}};

/* Each splash's point, x and y, in pixels. */
const u8 Daedalus_SplashPoints[][6][2] = {
	{{60, 64}, {58, 72}, {56, 80}, {54, 88}, {54, 96}, {56, 104}},
	{{78, 56}, {78, 64}, {80, 72}, {82, 80}, {84, 88}, {84, 96}},
	{{76, 60}, {74, 60}, {72, 60}, {70, 60}, {68, 60}, {66, 60}},
	{{60, 64}, {61, 64}, {62, 63}, {63, 62}, {64, 61}, {66, 60}}};

/* The frame each particle starts on. */
const u16 Daedalus_DiveStarts[] = {0,  24, 8,  52, 16, 40, 36, 28,
								   32, 4,  20, 56, 60, 12, 44, 48};

/* The frames of the seven strikes. */
const u8 DaedalusStrike_HitFrames[] = {80, 84, 86, 88, 90, 92, 94};

/* By level: which of the seven arrows strike. */
const u8 Vengeance_ArrowStrikes[][7] = {
	{0, 0, 1, 0, 0, 0, 1}, {1, 0, 1, 1, 0, 0, 1}, {1, 1, 1, 1, 1, 1, 1}};

/* The pale blade's mesh. */
const u16 LifeShear_PaleBladeMesh[] = {
	MESH_FACE(1, 3, 2, 127, 14, 127, 53, 0, 53),
	MESH_FACE(0, 1, 2, 0, 14, 127, 14, 0, 53), MESH_END};

/* The orange blade's mesh. */
const u16 LifeShear_OrangeBladeMesh[] = {
	MESH_FACE(1, 3, 2, 127, 0, 127, 31, 0, 31),
	MESH_FACE(0, 1, 2, 0, 0, 127, 0, 0, 31), MESH_END};

/* The scythe, 128 by 32 pixels: its four corners. */
const MODEL_VERTEX LifeShear_BladeCorners[] = {
	{-81, -16, 0}, {47, -16, 0}, {-81, 16, 0}, {47, 16, 0}};

/* The drops' gravity, by mode (GEYSER_MODE). */
const s32 Geyser_DropGravity[] = {-toFIXED(1) / 4, -toFIXED(1) / 2,
								  -toFIXED(1) / 2};

/* The drops' smallest picture size, by mode. */
const s8 Geyser_DropMinSizes[] = {1, 4, 5};

/* The fifteen pictures of a glowing oval, from 16 by 32 down to 3 by 4: their
 */
/* widths, in pixels. */
const u8 BubbleColumn_PictureWidths[] = {16, 15, 14, 13, 12, 11, 10, 9,
										 8,	 7,	 6,	 5,	 4,	 4,	 3};

/* The glowing oval's fifteen pictures: their heights, in pixels. */
const u8 BubbleColumn_PictureHeights[] = {32, 30, 28, 26, 24, 22, 20, 18,
										  16, 14, 12, 10, 8,  6,  4,  0};

/* The glowing oval's fifteen pictures: their byte offsets in the sprite sheet.
 */
const u16 BubbleColumn_Pictures[] = {0,	   512,	 962,  1354, 1692,
									 1980, 2222, 2422, 2584, 2712,
									 2810, 2882, 2932, 2964, 2988};

/* Each Coatlicue picture's width, in pixels. */
const u8 Coatlicue_PictureWidths[] = {16, 15, 14, 13, 12, 11, 10, 9,
									  8,  7,  6,  5,  4,  4,  3};

/* Each Coatlicue picture's height, in pixels. */
const u8 Coatlicue_PictureHeights[] = {32, 30, 28, 26, 24, 22, 20, 18,
									   16, 14, 12, 10, 8,  6,  4};

/* Each Coatlicue picture's byte offset in Coatlicue's sprite sheet. */
const u16 Coatlicue_Pictures[] = {0,	512,  962,	1354, 1692,
								  1980, 2222, 2422, 2584, 2712,
								  2810, 2882, 2932, 2964, 2988};

/* The frame each burst of bubbles starts on. */
const u16 Coatlicue_BurstFrames[] = {0, 0, 10, 78, 82, 112, 250, 254, 284};

/* Each burst's x, in pixels. */
const u8 Coatlicue_BurstX[] = {32, 64, 80, 32, 16, 88, 32, 16, 88};

/* Each burst's y, in pixels. */
const u8 Coatlicue_BurstY[] = {80, 96, 120, 120, 120, 120, 120, 120, 120};

/* How widely each burst scatters in x, in pixels. */
const u8 Coatlicue_BurstSpreadX[] = {32, 8, 32, 8, 16, 16, 8, 16, 16};

/* How far apart each burst's bubbles start in y, in pixels. */
const u8 Coatlicue_BurstSpacingY[] = {16, 8, 16, 8, 12, 12, 8, 12, 12};

/* Each burst's speed in x, in 16ths of a pixel a frame. */
const s8 Coatlicue_BurstVelocityX[] = {-24, 4, 20, 0, -2, 2, 0, -2, 2};

/* Each burst's speed in y, in 16ths of a pixel a frame. */
const s8 Coatlicue_BurstVelocityY[] = {-64, -32, -64, -16, -32,
									   -32, -16, -32, -32};

/* Each burst's depth, in pixels. */
const s8 Coatlicue_BurstZ[] = {1, 0, 4, -2, 1, 0, -2, 1, 0};

/* Each burst's speed in depth, in 64ths of a pixel a frame. */
const s8 Coatlicue_BurstVelocityZ[] = {16, 1, 12, 0, 0, 0, 0, 0, 0};

/* The smallest bubble picture for each kind of bubble. */
const u8 Coatlicue_PictureBias[] = {3, 5, 7, 9};

/* The frame each flash of the fire ring starts on. */
const u16 Coatlicue_RingFrames[] = {81, 209, 518, 540};

/* The frames the screen flashes on. */
const u16 Coatlicue_FlashFrames[] = {498, 503, 508};

/* The frame the cog's glow starts on. */
const u16 Coatlicue_GlowFrames[] = {518};

/* The frame each coil starts to grow on. */
const u8 Coatlicue_CoilStarts[] = {0, 82};

/* Each coil's first scale, in 65,536ths. */
const u16 Coatlicue_CoilScales[] = {4096, 18384};

/* Each coil's roll as it starts; it turns a 256th of a turn a frame. */
const u16 Coatlicue_CoilRolls[] = {DEGtoANG(45), DEGtoANG(225)};

/* How fast each coil grows, in 65,536ths of its scale a frame. */
const u16 Coatlicue_CoilGrowth[] = {256, 572};

/* Each coil's largest scale, in 65,536ths. */
const u16 Coatlicue_CoilMaxScales[] = {44160, 52720};

/* The frame each flash of the first act starts on. */
const u16 Catastrophe_FlashFrames[] = {4,	26,	 30,  34,  104, 112,
									   116, 120, 150, 154, 202, 214,
									   244, 278, 400, 402, 404, 420};

/* How long each flash of the first act lasts, in frames. */
const u16 Catastrophe_FlashLengths[] = {8,	2, 2, 16, 2,  2, 2, 10, 2,
										16, 4, 8, 10, 12, 2, 2, 10, 4};

/* Each of the demon's sprites: its x, in pixels. */
const u8 Catastrophe_DemonX[] = {0,	  32,  64, 96, 32,	64,	 96, 128, 64, 96,
								 128, 160, 32, 64, 96,	128, 25, 57,  89, 121,
								 153, 32,  64, 96, 128, 160, 48, 96};

/* Each of the demon's sprites: its depth, in pixels. */
const u8 Catastrophe_DemonZ[] = {
	0,	0,	0,	 0,	  32,  32,	32,	 32,  64,  64,	64,	 64,  96,  96,
	96, 96, 128, 128, 128, 128, 128, 160, 160, 160, 160, 160, 192, 192};

/* Each of the armour's sprites: its x, in pixels. */
const u8 Catastrophe_ArmourX[] = {16, 0, 32, 0, 32, 16, 16, 16, 16, 16, 16, 0};

/* Each of the armour's sprites: its depth, in pixels. */
const u16 Catastrophe_ArmourZ[] = {0,	32,	 32,  64,  64, 96,
								   128, 160, 192, 224, 256};

/* Each of the mirrored demon's sprites: its x, in pixels. */
const u8 Catastrophe_MirroredDemonX[] = {224, 192, 160, 128, 192, 160};

/* Each of the mirrored demon's sprites: its depth, in pixels. */
const u8 Catastrophe_MirroredDemonZ[] = {0, 0, 0, 0, 32, 32};

/* The frame each fork of lightning strikes on. */
const u16 Catastrophe_ForkStarts[] = {26, 104, 116, 216, 278, 402, 416};

/* Each fork's x, in pixels. */
const u8 Catastrophe_ForkX[] = {16, 96, 20, 40, 50, 10, 96};

/* Each fork's y, in pixels. */
const u8 Catastrophe_ForkY[] = {64, 80, 30, 40, 80, 60, 20};

/* Each fork's picture of lightning, by number. */
const u8 Catastrophe_ForkPictures[] = {0, 1, 0, 1, 0, 0, 1, 0};

/* The frames the canvas flashes on. */
const u16 Catastrophe_CanvasFlashFrames[] = {246, 249, 254, 258, 260, 264,
											 266, 269, 272, 275, 278, 280,
											 282, 284, 287, 290, 292};

/* Each step's x, in pixels, a row of twelve for each strike. */
const u8 Catastrophe_StrikeX[] = {
	71, 71, 71, 70, 70, 70, 70, 70, 70, 69, 69, 0,	67, 67, 67, 70, 67, 68,
	70, 69, 70, 69, 67, 0,	69, 69, 69, 68, 67, 68, 70, 69, 69, 69, 0,	0};

/* Each step's y, in pixels, a row of twelve for each strike. */
const u8 Catastrophe_StrikeY[] = {92,  92,	92,	 120, 120, 120, 154, 154, 154,
								  220, 200, 0,	 100, 100, 100, 120, 120, 154,
								  154, 154, 154, 220, 240, 0,	120, 100, 100,
								  140, 120, 154, 154, 154, 184, 220, 0,	  0};

/* The strike frames, a row of steps for each of the three strikes; 65535 ends
 */
/* a row, a frame that never comes. */
const u16 Catastrophe_StrikeFrames[] = {
	492, 506, 526, 528, 542, 562, 572, 578, 598, 600, 608, 65535, 0,
	504, 518, 526, 546, 548, 554, 562, 564, 570, 590, 594, 596,	  65535,
	510, 524, 544, 546, 548, 558, 560, 580, 582, 596, 616, 65535, 0};

/* How fast each step's bolt falls, in quarters of a pixel a frame. */
const u8 Catastrophe_StrikeSpeedsY[] = {
	6,	4,	0, 8,  8, 0, 10, 20, 0, 20, 0, 0,  6, 4, 0,	 8,	 8, 0,
	10, 20, 8, 20, 0, 0, 6,	 4,	 0, 8,	8, 20, 0, 0, 20, 20, 0, 0};

/* Each step's roll, in 256ths of a turn (the code shifts it into 65,536ths); 0
 */
/* leaves the bolt where the step before put it. */
const s16 Catastrophe_StrikeRolls[] = {
	144, 0,	 0, 128, 0,	  0, 180, 0, 0, 150, 0, 0,	 112, 0, 0,	  0, 112, 48,
	0,	 86, 0, 0,	 112, 0, 96,  0, 0, 96,	 0, 112, 128, 0, 104, 0, 0,	  0};

/* How fast each step's bolt turns, in 32,768ths of a turn a frame. */
const s16 Catastrophe_StrikeTurns[] = {
	256,  64,	0,	  712, 500, 0,	 0,	  0, 0,	   356,	 0, 0,
	-356, -124, -256, 712, -64, 256, 0,	  0, -228, 356,	 0, 0,
	406,  64,	-256, 0,   -64, 0,	 128, 0, 328,  -356, 0, 0};

/* Which of the three jagged streak pictures each step draws; -1 draws none. */
const s16 Catastrophe_StrikePictures[] = {
	0,	1, -1, 0,  1, -1, 0, 1, -1, 0, -1, 0, 2, 1,	 0, -1, 2,	2,
	-1, 0, 1,  -1, 2, -1, 0, 1, -1, 0, -1, 0, 1, -1, 0, 1,	-1, 0};

/* The flips a glint may be drawn with, one picked at random. */
const u8 Catastrophe_GlintFlips[] = {0, 4, 8, 12};

/* Each bolt's roll as it shoots out, in 65,536ths of a turn: scattered, all */
/* within a quarter turn of 0 either way. */
const s16 Catastrophe_BoltRolls[] = {14797,	 -1097, -8198,	4962,  15494, -5489,
									 -9298,	 3744,	11252,	-9996, 15443, -4373,
									 -15930, 15898, -13623, 3906};

/* Where each of Haures's eleven pieces is drawn, x and y, on a 32-pixel grid.
 */
const u16 Haures_BodyPiecePoints[][2] = {{0, 0},   {0, 32},	 {32, 32}, {64, 32},
										 {96, 42}, {0, 64},	 {32, 64}, {64, 64},
										 {96, 74}, {32, 96}, {64, 96}};

/* Each particle's x, in pixels. */
const u8 Haures_ParticleX[] = {120, 88, 144, 112, 0, 0};

/* Haures's sprites' scale, one to one. */
const s32 Haures_UnitScale[] = {toFIXED(1), toFIXED(1)};

/* The wings' shapes, 32 heights a shape, in pixels; the wings morph from one */
/* to the next. */
const u8 Haures_MoundShapes[] = {
	0,	 0,	  0,   2,	6,	 9,	  10,  11,	13,	 14,  15,  16,	17,	 18,  18,
	16,	 16,  16,  17,	16,	 15,  14,  13,	12,	 11,  9,   8,	5,	 0,	  0,
	0,	 0,	  0,   2,	8,	 12,  13,  16,	18,	 21,  24,  27,	31,	 34,  36,
	34,	 32,  28,  18,	18,	 21,  29,  35,	40,	 40,  38,  37,	32,	 25,  15,
	10,	 8,	  4,   0,	0,	 8,	  11,  14,	17,	 19,  22,  24,	28,	 31,  35,
	45,	 58,  54,  49,	46,	 47,  56,  103, 120, 118, 98,  52,	45,	 49,  49,
	38,	 32,  26,  18,	8,	 0,	  0,   8,	13,	 16,  20,  22,	26,	 29,  31,
	36,	 42,  47,  52,	59,	 65,  69,  70,	69,	 66,  63,  59,	56,	 53,  49,
	46,	 42,  37,  32,	27,	 18,  7,   0,	0,	 8,	  13,  18,	22,	 26,  40,
	54,	 59,  57,  39,	31,	 32,  37,  48,	66,	 73,  61,  49,	54,	 48,  45,
	45,	 31,  32,  44,	41,	 32,  24,  20,	14,	 0,	  0,   8,	11,	 16,  22,
	27,	 55,  90,  87,	76,	 66,  55,  45,	45,	 53,  58,  58,	50,	 31,  37,
	42,	 48,  57,  56,	37,	 29,  31,  31,	32,	 31,  27,  0,	0,	 10,  17,
	25,	 32,  42,  46,	48,	 51,  55,  60,	66,	 72,  77,  80,	84,	 85,  83,
	76,	 63,  53,  49,	45,	 43,  39,  35,	32,	 27,  22,  18,	10,	 0,	  0,
	10,	 16,  21,  26,	29,	 36,  46,  58,	69,	 77,  78,  74,	67,	 59,  52,
	52,	 59,  65,  69,	71,	 72,  71,  70,	59,	 66,  62,  44,	26,	 20,  13,
	0,	 0,	  9,   15,	21,	 27,  29,  34,	43,	 47,  51,  53,	55,	 58,  62,
	67,	 69,  72,  72,	71,	 67,  63,  60,	56,	 53,  51,  45,	38,	 26,  17,
	10,	 8,	  0,   0,	8,	 12,  20,  27,	52,	 73,  74,  68,	60,	 53,  46,
	41,	 40,  48,  61,	72,	 77,  78,  82,	88,	 93,  97,  32,	35,	 40,  44,
	40,	 28,  15,  8,	0,	 0,	  8,   16,	34,	 44,  50,  56,	60,	 65,  69,
	73,	 77,  79,  82,	83,	 77,  72,  68,	63,	 59,  54,  40,	26,	 23,  26,
	29,	 31,  32,  26,	18,	 8,	  0,   0,	8,	 11,  18,  30,	56,	 77,  86,
	92,	 93,  92,  90,	88,	 85,  82,  79,	76,	 72,  70,  66,	62,	 60,  56,
	51,	 47,  42,  36,	24,	 15,  10,  8,	0,	 0,	  8,   83,	96,	 104, 110,
	115, 118, 120, 116, 114, 114, 115, 116, 119, 121, 120, 117, 114, 111, 108,
	106, 102, 100, 97,	87,	 30,  22,  15,	10,	 8,	  0};

/* How many frames each morph of the wings takes. */
const u8 Haures_MoundMorphSteps[] = {27, 15, 13, 5,	 14, 16, 11,
									 13, 8,	 18, 24, 26, 100};

/* Each shard's points, its middle then its four corners, x and y in screen */
/* pixels. */
const u8 Haures_PartPoints[] = {
	23,	 13,  0,   0,	42,	 0,	  28,  41,	28,	 41,  48,  28,	42,	 0,	  78,
	31,	 28,  41,  28,	41,	 66,  7,   42,	0,	 79,  0,   78,	31,	 78,  31,
	94,	 8,	  79,  0,	128, 0,	  78,  31,	78,	 31,  112, 28,	128, 0,	  132,
	48,	 78,  31,  78,	31,	 162, 12,  182, 0,	 174, 42,  128, 0,	 128, 0,
	145, 39,  128, 0,	174, 42,  134, 71,	134, 71,  196, 12,	182, 0,	  239,
	0,	 174, 42,  172, 42,	 231, 20,  239, 0,	 239, 52,  211, 16,	 211, 16,
	7,	 33,  0,   0,	28,	 41,  0,   48,	0,	 48,  59,  49,	78,	 31,  69,
	73,	 28,  41,  28,	41,	 91,  51,  78,	31,	 132, 48,  69,	73,	 69,  73,
	16,	 58,  28,  41,	20,	 86,  0,   48,	0,	 48,  36,  70,	28,	 41,  69,
	73,	 20,  86,  20,	86,	 96,  76,  132, 48,	 99,  97,  69,	73,	 69,  73,
	6,	 80,  0,   48,	20,	 86,  0,   98,	0,	 98,  168, 66,	174, 42,  205,
	88,	 134, 71,  134, 71,	 194, 48,  211, 16,	 205, 88,  174, 42,	 174, 42,
	217, 52,  211, 16,	239, 52,  205, 88,	208, 88,  7,   114, 0,	 98,  30,
	119, 0,	  119, 0,	119, 29,  106, 20,	86,	 64,  119, 30,	119, 0,	  98,
	51,	 93,  69,  73,	64,	 119, 20,  86,	20,	 86,  78,  96,	69,	 73,  99,
	97,	 64,  119, 64,	119, 98,  112, 99,	97,	 137, 104, 112, 119, 64,  119,
	123, 85,  132, 48,	137, 104, 99,  97,	99,	 97,  156, 88,	134, 71,  205,
	88,	 137, 104, 137, 104, 139, 115, 137, 104, 175, 119, 112, 119, 112, 119,
	217, 52,  211, 16,	239, 52,  205, 88,	205, 88,  228, 87,	239, 52,  239,
	119, 205, 88,  205, 88,	 200, 108, 185, 93,	 205, 88,  239, 119, 175, 119,
	169, 106, 185, 93,	175, 119, 137, 104, 137, 104};

/* The frame each band starts on. */
const u8 Haures_BandStarts[] = {10, 30};

/* Each Helm Splitter picture's width, in pixels. */
const u8 HelmSplitter_PictureWidths[] = {26, 46, 63, 77, 60, 39, 32};

/* Each Helm Splitter picture's height, in pixels. */
const u8 HelmSplitter_PictureHeights[] = {11, 20, 37, 65, 65, 54, 39, 0};

/* Each Helm Splitter picture's byte offset in Helm Splitter's sprite sheet. */
const u16 HelmSplitter_Pictures[] = {0, 286, 1206, 3537, 8542, 12442, 14548};

/* How far right of its anchor each picture is drawn, in pixels, mirrored on */
/* the other side. */
const s8 HelmSplitter_PictureBiasX[] = {53, 36, 16, 2, 2, 2, 2};

/* How far below its anchor each picture's top goes, in pixels. */
const s8 HelmSplitter_PictureBiasY[] = {2, 3, 3, 3, 4, 14, 29};

/* How many of the 56 embers are sparks, by variant; the rest are flashes. */
const u8 DragonCloud_SparkCounts[] = {48, 40, 0, 0};

/* Each card's length, in frames. */
const u8 ElementCards_Frames[] = {124, 140, 132, 130};

/* How each card's sparks move. */
const SPARK_MOTION ElementCards_SparkMotions[] = {{toFIXED(0.125), 62, 1},
												  {toFIXED(0.125), 62, 1},
												  {-toFIXED(0.125), 62, 1},
												  {2048, 64, 4}};

/* The three bites' frames. */
const u8 TripleChomp_BiteFrames[] = {0, 12, 24};

/* Each breath's length, the frame its burst starts, and the glow's widest size
 */
/* by variant. */
const u8 Breath_Frames[] = {90, 38, 32, 48};

/* The frame the burst starts on, by breath. */
const u8 Breath_BurstFrames[] = {56, 0, 0, 0};

/* The glow's widest size, in pixels, by variant. */
const u8 Breath_MaxSizes[] = {24, 32, 32, 24};

/* The four jaws of variant 2, by breath: their start frames; the tables after
 */
/* it give their growth and their x from the mouth. */
const u8 Breath_RingStarts[] = {56, 58, 60, 61, 8, 12, 16, 20, 0, 2, 4, 5};

/* How fast each jaw grows: its scale gains this many times BREATH_RING_GROWTH
 */
/* a frame. */
const u8 Breath_RingGrowth[] = {5, 6, 7, 6, 10, 9, 11, 10, 5, 6, 7, 6};

/* Each jaw's x from the mouth, in pixels. */
const s8 Breath_RingX[] = {-32, -48, -56, -72, -16, -32,
						   -64, -72, -32, -48, -56, -72};

/* The frame each breath hits the targets. */
const u8 Breath_FlashFrames[] = {56, 14, 6, 56};

/* The frame each ring starts on. */
const u8 Trident_RingStarts[] = {8, 10, 12, 13, 64, 67, 70, 72};

/* How fast each ring grows: its scale gains this many times */
/* CYLINDER_RING_GROWTH a frame. */
const u8 Trident_RingGrowth[] = {8, 7, 6, 7, 8, 8, 9, 8};

/* Each ring's height, in pixels. */
const s8 Trident_RingHeights[] = {16, 0, -24, -40, 16, 0, -24, -40, 0};

/* Each bolt's roll as it spins in, then how far it rolls each frame, both in */
/* 65,536ths of a turn: scattered round the circle. */
const s32 Trident_Spins[] = {5528,	488, 62272, 308, 52392, -698, 34584, 634,
							 19692, 736, 28062, 746, 48958, -694, 1508,	 -576};

/* The prong's corners, as signed x, y and z. */
const MODEL_VERTEX Trident_ProngCorners[] = {
	{-6, 127, 0},  {6, 127, 0}, {-6, -4, 0},  {6, -4, 0},
	{-6, 127, 0},  {6, 127, 0}, {-6, 0, 0},	  {6, 0, 0},
	{-10, 127, 0}, {2, 127, 0}, {-10, -1, 0}, {2, -1, 0}};

/* The flips a glint may be drawn with, one picked at random. */
const u8 Trident_GlintFlips[] = {0, 4, 8, 12};

/* The frame each wave starts on, counted from LEAPING_STRIKE_WAVE_FRAME. */
const u8 LeapingStrike_WaveStarts[] = {0, 4, 8, 9};

/* How fast each wave grows: its scale gains this many times */
/* LEAPING_STRIKE_WAVE_GROWTH a frame, from a half. */
const u8 LeapingStrike_WaveGrowth[] = {9, 8, 9, 3};

/* Each wave's height, in pixels. */
const s8 LeapingStrike_WaveHeights[] = {32, 32, 32, 32};

/* The frame each ring starts on, counted from LEAPING_STRIKE_PILLAR_FRAME. */
const u8 LeapingStrike_RingStarts[] = {0, 4, 12, 19};

/* How fast each ring grows: its scale gains this many times */
/* CYLINDER_RING_GROWTH a frame. */
const u8 LeapingStrike_RingGrowth[] = {7, 8, 7, 8};

/* Each ring's height, in pixels. */
const s8 LeapingStrike_RingHeights[] = {32, 16, 16, 24};

/* Each Meteor Blow picture's width, in pixels. */
const u8 MeteorBlow_PictureWidths[] = {30, 16, 16, 16, 10, 8, 8};

/* Each Meteor Blow picture's height, in pixels. */
const u8 MeteorBlow_PictureHeights[] = {30, 16, 16, 16, 16, 8, 8};

/* Each Meteor Blow picture's byte offset in Meteor Blow's sprite sheet. */
const u16 MeteorBlow_Pictures[] = {0, 900, 1156, 1412, 1668, 1828, 1892};

/* The frame each wave starts on, counted from METEOR_BLOW_BURST_FRAME. */
const u8 MeteorBlow_WaveStarts[] = {0, 4, 9, 10};

/* How fast each wave grows: its scale gains this many times 2000/65536 a */
/* frame. */
const u8 MeteorBlow_WaveGrowth[] = {3, 4, 3, 5};

/* How much taller than wide each wave is drawn, multiplying its scale. */
const u8 MeteorBlow_WaveStretch[] = {3, 4, 5, 6};

/* The frame each ring starts on, counted from the eighth. */
const u8 MeteorBlow_RingStarts[] = {0, 3, 16, 7};

/* How fast each ring grows: its scale gains this many times 1500/65536 a */
/* frame. */
const u8 MeteorBlow_RingGrowth[] = {5, 8, 10, 8};

/* Each ring's height, in pixels. */
const s8 MeteorBlow_RingHeights[] = {-40, 24, 24, 24};

/* The splash's corners, as signed x, y and z. */
const MODEL_VERTEX Liquifier_SplashCorners[] = {
	{-48, -64, 0}, {48, -64, 0}, {-32, 4, 0}, {32, 4, 0}};

/* The frame each splash starts on. */
const u8 Liquifier_SplashStarts[] = {18, 21, 16, 17, 22, 19, 23, 18};

/* How tall each splash leaps, as a scale in 32nds. */
const u8 Liquifier_SplashHeights[] = {40, 56, 70, 48, 64, 80, 58, 64, 72};

/* The flame's picture for each three frames of its life, by number. */
const u8 CanvasBurst_FlamePictures[] = {5, 5, 4, 3, 2, 1, 0};

/* Each ring's pitch. */
const s16 Megiddo_FlashPitches[] = {0, -DEGtoANG(56.25), 0, -DEGtoANG(56.25),
									DEGtoANG(56.25)};

/* Each ring's yaw. */
const s16 Megiddo_FlashYaws[] = {0, -DEGtoANG(26.71875), 0, -DEGtoANG(26.71875),
								 -DEGtoANG(26.71875)};

/* The frame each of the five rings starts on. */
const u16 Megiddo_FlashStarts[] = {4, 8, 171, 175, 176};

/* Each ray's pitch. */
const s16 Megiddo_RayPitches[] = {-DEGtoANG(68.90625), -DEGtoANG(57.65625),
								  -DEGtoANG(47.8125),  DEGtoANG(47.8125),
								  -DEGtoANG(46.40625), DEGtoANG(70.3125),
								  DEGtoANG(59.0625),   DEGtoANG(61.875)};

/* Each ray's yaw. */
const s16 Megiddo_RayYaws[] = {0,
							   DEGtoANG(64.6875),
							   0,
							   -DEGtoANG(78.75),
							   DEGtoANG(61.875),
							   -DEGtoANG(109.6875),
							   -DEGtoANG(120.9375),
							   -DEGtoANG(122.34375)};

/* Each ray's roll, mirrored on the other side; the third ray's, a little off */
/* its neighbours' 256ths of a turn, is in 65,536ths. */
const s16 Megiddo_RayRolls[] = {
	-DEGtoANG(118.125),	 DEGtoANG(73.125),	  22224,
	-DEGtoANG(104.0625), -DEGtoANG(180),	  DEGtoANG(46.40625),
	DEGtoANG(140.625),	 -DEGtoANG(158.90625)};

/* Each ray's x, in pixels. */
const s8 Megiddo_RayX[] = {6, 9, 6, 2, 6, 3, 6, 7};

/* Each ray's y, in pixels. */
const s8 Megiddo_RayY[] = {-6, -4, -7, -9, -6, -7, -9, -7};

/* Each flash's pitch: Megiddo's first two rings'. */
const s16 Odyssey_FlashPitches[] = {0, -DEGtoANG(56.25)};

/* Each flash's yaw: Megiddo's first two rings', and a third nothing reads. */
const s16 Odyssey_FlashYaws[] = {0, -DEGtoANG(26.71875), 0};

/* The frame each ring starts on, counted from MOLOCH_SPREAD_FRAME. */
const u8 Moloch_RingStarts[] = {0, 6, 8, 9};

/* How fast each ring grows: its scale gains this many times 2000/65536 a */
/* frame, from a half. */
const u8 Moloch_RingGrowth[] = {7, 8, 7, 8};

/* Each ring's height, in pixels. */
const s8 Moloch_RingHeights[] = {24, 12, -24, 0};

/* A splash's size, a large one's then a small one's: its ring starts twice */
/* this many pixels out and closes a pixel a frame, then its drops spread out */
/* for this many frames. */
const s32 Iris_SplashRadii[] = {24, 16};

/* How far below the splash's point its wider ring of drops is centred, in */
/* steps of two pixels: a large one's, then a small one's. */
const u8 Iris_SplashOffsetsY[] = {7, 5, 0, 0};

/* The pillar's mesh. */
const u16 Iris_PillarMesh[] = {MESH_FACE(0, 1, 2, 0, 0, 15, 0, 0, 15),
							   MESH_FACE(2, 1, 3, 0, 15, 15, 0, 15, 15),
							   MESH_FACE(2, 3, 4, 0, 14, 15, 14, 0, 15),
							   MESH_FACE(4, 3, 5, 0, 15, 15, 14, 15, 15),
							   MESH_END};

/* The particle's corners, as signed x, y and z. */
const MODEL_VERTEX Iris_ParticleCorners[] = {
	{-4, -8, 0}, {4, -8, 0}, {-4, 8, 0}, {4, 8, 0}};

/* The particle's mesh. */
const u16 Iris_ParticleMesh[] = {MESH_FACE(0, 1, 2, 0, 0, 31, 0, 0, 63),
								 MESH_FACE(2, 1, 3, 0, 63, 31, 0, 31, 63),
								 MESH_END};

/* The frames the Psynergy sound plays on. */
const u8 Iris_PsynergySoundFrames[] = {84,	93,	 108, 111, 116, 132,
									   135, 141, 153, 158, 171, 0};

/* The frames the door sound plays on. */
const u16 Iris_DoorSoundFrames[] = {231, 244, 259, 265, 270};

/* The frames the power up sound plays on. */
const u16 Iris_PowerUpSoundFrames[] = {279, 287, 293, 298, 302, 307};

/* Each streak's x as it sets out, in pixels. */
const s8 Iris_StreakX[] = {-10, 80, 110, -10, 76, 50, 110, 80,
						   40,	30, 80,	 20,  50, 80, 20};

/* Each streak's y as it sets out, in pixels. */
const u8 Iris_StreakY[] = {116, 136, 128, 112, 136, 136, 128, 136,
						   136, 136, 136, 136, 136, 136, 136};

/* Each streak's heading as it sets out, 0 straight up and DEGtoANG(90) to */
/* the right: each a little off one of those, by a number of 65,536ths of a */
/* turn. */
const u16 Iris_StreakAngles[] = {DEGtoANG(90) + 2000,
								 TRIG_TURN - 3048,
								 DEGtoANG(270) + 2000,
								 DEGtoANG(90),
								 TRIG_TURN - 3048,
								 3048,
								 DEGtoANG(270) + 2000,
								 TRIG_TURN - 3048,
								 4048,
								 6048,
								 TRIG_TURN - 5048,
								 7048,
								 3048,
								 TRIG_TURN - 3048,
								 6048};

/* How far each streak's heading turns, in 65,536ths of a turn, two thirds of */
/* this a frame. */
const s16 Iris_StreakSpeeds[] = {-406, 64,	380, -326, 64,	-64, 380, 64,
								 -84,  -64, 94,	 -94,  -64, 64,	 -64};

/* The spray picture's columns, left to right: the level each is filled with, */
/* dimmest at the edges. */
const u8 Iris_LineShades[] = {23, 47, 56, 63, 63, 56, 47, 23};

/* Each mote's size: the width of its mote picture, in pixels, which is twice */
/* as tall. */
const s32 Iris_MoteSizes[] = {7, 5};

/* The frame each splash starts on. */
const u16 Iris_SplashStarts[] = {230, 238, 260, 265, 253, 258};

/* The frame each splash ends on. */
const u16 Iris_SplashEnds[] = {320, 328, 330, 335, 326, 332};

/* Each splash's x, in pixels. */
const u8 Iris_SplashX[] = {34, 86, 18, 102, 8, 112};

/* Each splash's y, in pixels. */
const u8 Iris_SplashY[] = {64, 62, 32, 32, 80, 80};

/* Whether each splash is a small one. */
const u8 Iris_SplashSmall[] = {0, 0, 1, 1, 0, 0};

/* How fast each drop rises, in half pixels a frame. */
const u8 Iris_DropSpeeds[] = {8, 4, 1, 2, 4, 8};

/* How long each drop rises, in frames, from its splash's end. */
const u8 Iris_DropFrames[] = {16, 16, 16, 16, 32, 32};

/* How many frames each drop shows each picture. */
const u8 Iris_DropPictureSteps[] = {1, 1, 1, 1, 2, 2};

/* The left strip's corners, as signed x, y and z. */
const MODEL_VERTEX Iris_LeftStripCorners[] = {
	{-6, -64, 0}, {18, -64, 0}, {-9, -32, 0}, {15, -32, 0}, {-13, 0, 0},
	{8, 0, 0},	  {-19, 32, 0}, {-1, 32, 0},  {-28, 64, 0}, {-12, 64, 0}};

/* The right strip's corners, as signed x, y and z. */
const MODEL_VERTEX Iris_RightStripCorners[] = {
	{35, -64, 0}, {72, -64, 0}, {28, -32, 0}, {66, -32, 0}, {18, 0, 0},
	{51, 0, 0},	  {6, 32, 0},	{33, 32, 0},  {-12, 64, 0}, {9, 64, 0}};

/* The strip's mesh. */
const u16 Iris_StripMesh[] = {MESH_FACE(0, 1, 2, 0, 0, 31, 0, 0, 15),
							  MESH_FACE(2, 1, 3, 0, 15, 31, 0, 31, 15),
							  MESH_FACE(2, 3, 4, 0, 15, 31, 15, 0, 31),
							  MESH_FACE(4, 3, 5, 0, 31, 31, 15, 31, 31),
							  MESH_FACE(4, 5, 6, 0, 31, 31, 31, 0, 47),
							  MESH_FACE(6, 5, 7, 0, 47, 31, 31, 31, 47),
							  MESH_FACE(6, 7, 8, 0, 47, 31, 47, 0, 63),
							  MESH_FACE(8, 7, 9, 0, 63, 31, 47, 31, 63),
							  MESH_END};

/* The cone's corners, as signed x, y and z. */
const MODEL_VERTEX Iris_ConeCorners[] = {
	{0, -40, 0}, {-5, -4, 0},  {5, -4, 0},	{-10, 16, 0},
	{10, 16, 0}, {-20, 32, 0}, {20, 32, 0}, {0, 0, 0}};

/* The cone's mesh. */
const u16 Iris_ConeMesh[] = {MESH_FACE(1, 0, 7, 0, 31, 0, 0, 15, 31),
							 MESH_FACE(1, 2, 3, 0, 0, 31, 0, 0, 31),
							 MESH_FACE(3, 2, 4, 0, 31, 31, 0, 31, 31),
							 MESH_FACE(3, 4, 5, 0, 0, 31, 0, 0, 31),
							 MESH_FACE(5, 4, 6, 0, 31, 31, 0, 31, 31),
							 MESH_FACE(7, 0, 2, 15, 31, 31, 0, 31, 31)};
