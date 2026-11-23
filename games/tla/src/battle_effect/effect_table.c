/// @file
/// Battle effect tables, in the order of the effect code that reads them. Each
/// is read only by the effect its prefix names, except the gBattleEffect_
/// tables, which many effects share. Picture offsets count bytes from the start
/// of a sheet; widths and heights are pixels. A mesh is a list of triangles,
/// each three vertex numbers and their texture coordinates; corners are signed
/// x, y and z bytes.

#include "types.h"
#include "fixed_math.h"
#include "battle/render_model.h"
#include "battle_effect/battle_effect.h"
#include "battle_effect/battle_effect_table.h"

/// Each mote size's picture: its byte offset in the particle sheet, by size.
const u16 gBattleEffect_MotePictures[] = {0,   2,   10,  28,  60,
                                          110, 182, 280, 408, 570};

/// Each flare size's picture: its byte offset in the spark sheet, by size less
/// one.
const u16 gBattleEffect_FlarePictures[] = {0,   4,   20,  56,  120,
                                           220, 364, 560, 816, 1140};

/// Each circle picture's byte offset in Cruel Ruin's sheet.
const u16 gCruelRuin_CirclePictures[] = {0,   8,   40,   112,  240,
                                         440, 728, 1120, 1632, 2280};

/// Each sparkle picture's byte offset in the sheet (gBattleEffect_SparkleSizes
/// gives its side).
const u16 gBattleEffect_SparklePictures[] = {0,   25,  74,  123, 172,
                                             221, 246, 255, 256};

/// Each sparkle picture's side, in pixels.
const u8 gBattleEffect_SparkleSizes[] = {5, 7, 7, 7, 7, 5, 3, 1, 1};

/// The ember puff's six pictures, one after another as it rises: their widths,
/// heights, how far below its anchor each top goes, and their offsets.
const u8 gBattleEffect_PuffArcPictureWidths[] = {24, 29, 30, 27, 24, 16};

/// Each puff arc picture's height, in pixels.
const u8 gBattleEffect_PuffArcPictureHeights[] = {23, 32, 37, 49, 53, 52};

/// How far below its anchor each puff arc picture's top goes.
const u8 gBattleEffect_PuffArcPictureBiasY[] = {41, 32, 26, 11, 4, 2};

/// Each puff arc picture's byte offset in the sheet.
const u16 gBattleEffect_PuffArcPictures[] = {0, 552, 1480, 2590, 3913, 5185};

/// Each glint picture's byte offset in the sheet.
const u16 gBattleEffect_GlintPictures[] = {0, 589, 1302, 2015, 2414, 2750};

/// Each glint picture's width, in pixels.
const u8 gBattleEffect_GlintPictureWidths[] = {19, 23, 23, 19, 16, 17};

/// Each glint picture's height, in pixels.
const u8 gBattleEffect_GlintPictureHeights[] = {31, 31, 31, 21, 21, 24};

/// Each shard picture's width, in pixels.
const u8 gSpire_ShardWidths[] = {5,  11, 10, 7,  7,  12, 13, 9,
                                 19, 12, 17, 22, 15, 16, 14};

/// Each shard picture's height, in pixels.
const u8 gSpire_ShardHeights[] = {11, 6,  10, 11, 10, 7,  17, 21,
                                  12, 21, 17, 13, 16, 16, 15};

/// Each shard picture's byte offset in Spire's sheet.
const u16 gSpire_ShardPictures[] = {0,    55,   121,  221,  298,  368,
                                    452,  673,  862,  1090, 1342, 1631,
                                    1917, 2157, 2413, 2623};

/// Each shard picture's byte offset in a shade of the stat change's sheet.
const u16 gStatChange_ShardPictures[] = {0,   120, 180, 258, 298, 352,
                                         408, 471, 567, 607, 631, 661};

/// Each shard picture's width, in pixels.
const u8 gStatChange_ShardWidths[] = {10, 5, 6, 5, 6, 7, 9, 8, 4, 6, 6, 7};

/// Each shard picture's height, in pixels.
const u8 gStatChange_ShardHeights[] = {12, 12, 13, 8, 9, 8, 7, 12, 10, 4, 5, 5};

/// The reaper's seven panels: their x, seven for each side.
const u8 gCastingImpact_PictureX[] = {10, 30, 0, 0, 0, 6, 52,
                                      60, 0,  0, 0, 0, 0, 0};

/// The reaper's seven panels: their y.
const u8 gCastingImpact_PictureY[] = {0, 20, 2, 2, 34, 64, 68};

/// Ice's shards and frames, by variant.
const u8 gFalling_IceCounts[] = {16, 46, 20, 52, 32, 80};

/// The falling shards: four bytes for each variant.
const u8 gFalling_Counts[] = {6, 8, 32, 80, 8, 16, 24, 80, 32, 4, 8, 80};

/// The streak styles: four signed bytes for each mode.
const s8 gStreak_Styles[] = {0,  1, 0, 0, 0,  1, 4, 0, 0, 1,
                             -1, 1, 1, 1, -1, 1, 1, 1, 4, 0};

/// The struck target's sideways tremble, in pixels, one a frame.
const s8 gStreak_Tremble[] = {-1, -1, 1, 1, 1, 1, -1, -1};

/// The target strike styles, one row a style (enum TargetStrikeStyle), one byte
/// a column (enum TargetStrikeColumn).
const u8 gTargetStrike_Styles[][7] = {
    {1, 2, 8, 0, 6, 0, 70},  {1, 1, 1, 1, 6, 1, 54},  {1, 3, 1, 2, 6, 1, 96},
    {1, 3, 2, 3, 4, 1, 96},  {1, 3, 2, 4, 4, 1, 96},  {1, 3, 4, 5, 4, 0, 60},
    {0, 1, 4, 5, 4, 0, 38},  {0, 2, 8, 5, 2, 0, 48},  {0, 1, 24, 4, 2, 0, 50},
    {0, 1, 1, 1, 2, 0, 46},  {1, 1, 1, 4, 6, 1, 54},  {1, 3, 2, 4, 4, 0, 96},
    {0, 4, 8, 5, 2, 1, 64},  {0, 8, 8, 5, 1, 0, 64},  {0, 1, 44, 6, 2, 0, 50},
    {1, 3, 2, 4, 4, 0, 96},  {0, 1, 40, 6, 2, 1, 50}, {1, 1, 24, 4, 6, 0, 50},
    {1, 1, 40, 6, 2, 1, 50}, {0, 1, 64, 4, 2, 0, 22}, {1, 1, 8, 0, 6, 0, 70}};

/// Each slash picture's width, in pixels.
const u8 gTargetStrike_Widths[] = {38, 54, 71, 74, 77, 65};

/// Each Target Strike picture's height, in pixels.
const u8 gTargetStrike_Heights[] = {7, 9, 11, 13, 13, 11};

/// Each Target Strike picture's byte offset in Target Strike's sheet.
const u16 gTargetStrike_Pictures[] = {3200, 3466, 3952, 4733, 5695, 6696};

/// How far each slash picture reaches past its middle, in pixels.
const u8 gTargetStrike_Reach[] = {58, 42, 25, 13, 5, 0};

/// Which stage of the closing fangs each bite picture shows.
const u8 gTargetStrike_BiteStages[] = {0, 1, 2, 3, 2};

/// The flips a glint may be drawn with, one picked at random.
const u8 gTargetStrike_GlintDrawFlags[] = {0, 4, 8, 12};

/// The 33 pieces of the shattered picture: each one's width; the tables after
/// it give their heights, places in the sheet and starting points.
const u8 gDeathPlunge_PieceWidths[] = {
    21, 20, 24, 30, 35, 69, 19, 23, 9, 22, 33, 10, 18, 9, 7,  9, 17,
    55, 30, 25, 40, 29, 10, 28, 14, 5, 32, 23, 19, 32, 8, 43, 15};

/// Each piece picture's height, in pixels.
const u8 gDeathPlunge_PieceHeights[] = {
    23, 47, 25, 40, 20, 53, 28, 29, 23, 14, 35, 17, 18, 14, 15, 12, 13,
    37, 38, 41, 60, 41, 15, 45, 59, 41, 26, 50, 21, 35, 12, 57, 11, 0};

/// Each piece picture's byte offset in Death Plunge's sheet.
const u16 gDeathPlunge_PiecePictures[] = {
    0,     483,   1423,  2023,  3223,  3923,  7580,  8112,  8779,
    8986,  9294,  10449, 10619, 10943, 11069, 11174, 11282, 11503,
    13538, 14678, 15703, 18103, 19292, 19442, 20702, 21528, 21733,
    22565, 23715, 24114, 25234, 25330, 27781, 27946};

/// Each piece's starting x.
const u8 gDeathPlunge_PieceStartX[] = {
    0,  21, 28, 46, 70, 51, 0,  13, 37, 77, 87, 19, 25, 35, 44, 45, 48,
    65, 0,  0,  0,  5,  33, 15, 36, 46, 47, 50, 53, 65, 72, 77, 105};

/// Each piece's starting y.
const u8 gDeathPlunge_PieceStartY[] = {
    0,  0,  0,  0,  0,  0,  19, 17, 23, 28, 8,  36, 43, 43,  40, 47, 48,
    37, 42, 67, 60, 79, 62, 75, 61, 62, 60, 70, 74, 85, 108, 63, 74};

/// The four corner sparks: their x about the target.
const s8 gDeathPlunge_X[] = {0, 32, 0, 32};

/// The four corner sparks: their y about the target.
const s8 gDeathPlunge_Y[] = {0, 0, 32, 32};

/// The four corner sparks' flips.
const u8 gElementBurst_GlintFlips[] = {0, 4, 8, 12};

/// The streak's corners, as signed x, y and z.
const struct ModelVertex gCruelRuin_StreakCorners[] = {
    {-4, 127, 0}, {4, 127, 0}, {-6, 0, 0}, {6, 0, 0}};

/// Each disc's pitch.
const u16 gCruelRuin_DiscPitches[] = {10240, 56579, 56579, 58880};

/// Each disc's yaw.
const u16 gCruelRuin_DiscYaws[] = {39680, 42496, 32768, 22016};

/// How far each pillar sways, in pixels.
const u8 gCruelRuin_PillarSways[] = {24, 48};

/// Each pillar picture's width, in pixels.
const u8 gCruelRuin_PillarWidths[] = {24, 32};

/// The blast frames.
const u8 gCruelRuin_BlastFrames[] = {7, 37, 45, 58, 74, 80};

/// Each blast's size, in pixels.
const u8 gCruelRuin_BlastSizes[] = {96, 48, 64, 64, 96, 128};

/// Each blast's x, in pixels.
const u8 gCruelRuin_BlastX[] = {48, 48, 32, 0, 0, 24};

/// Each blast's y, in pixels.
const u8 gCruelRuin_BlastY[] = {96, 96, 96, 96, 120, 120};

/// The frame each streak starts on.
const u8 gCruelRuin_StreakStarts[] = {2, 26, 104, 150};

/// Each streak's size, in pixels.
const u8 gCruelRuin_StreakSizes[] = {32, 16, 24, 64};

/// Each streak's x, in pixels.
const u8 gCruelRuin_StreakX[] = {96, 48, 24, 16};

/// Each streak step's x, in pixels.
const u8 gCruelRuin_StreakStepX[] = {16, 8, 12, 8};

/// Each streak step's y, in pixels.
const u8 gCruelRuin_StreakStepY[] = {16, 4, 16, 24};

/// Each streak end's y, in pixels.
const u8 gCruelRuin_StreakEndY[] = {96, 96, 96, 120};

/// Each streak's roll.
const s32 gCruelRuin_StreakRolls[] = {36864, DEGtoANG(225), 36864, 36864};

/// The flips a glint may be drawn with, one picked at random.
const u8 gEclipse_GlintFlips[] = {0, 4, 8, 12};

/// The dragon's sprites that take their tiles anew as it turns.
const u8 gEclipse_TileObjects[] = {8, 9, 25, 14, 15, 20, 21};

/// The fill frames.
const u8 gEclipse_FillFrames[] = {148, 160, 162, 164, 170, 174, 178, 182, 188,
                                  194, 196, 200, 202, 206, 208, 210, 220};

/// The canvas fills of the opening flash, four brightness levels a word, one a
/// frame.
const u32 gEclipse_FillLevels[] = {
    404232216, 269488144, 269488144, 404232216, 741092396, 741092396,
    741092396, 741092396, 741092396, 741092396, 269488144, 269488144,
    269488144, 741092396, 741092396, 269488144, 269488144};

/// Each sprite's x, in pixels.
const u8 gEclipse_ObjectX[] = {0,   32,  64,  96,  160, 192, 0,   32,
                               64,  96,  128, 160, 192, 224, 64,  96,
                               128, 160, 192, 224, 64,  96,  128, 160,
                               192, 224, 64,  96,  128, 160, 192, 224};

/// Each sprite's depth, in pixels.
const u8 gEclipse_ObjectZ[] = {0,  0,  0,  0,  0,   0,   32,  32,  32,  32, 32,
                               32, 32, 32, 64, 64,  64,  64,  64,  64,  96, 96,
                               96, 96, 96, 96, 128, 128, 128, 128, 128, 128};

/// How fast each ring grows.
const u8 gEclipse_RingGrowth[] = {4, 3, 5, 3};

/// Each ring's height.
const s8 gEclipse_RingHeights[] = {16, 12, 4, -8};

/// The four crystals: the frame each starts after 48; the tables after it give
/// how fast each grows and how tall.
const u8 gFormChange_WaveStarts[] = {0, 4, 9, 10};

/// How fast each wave grows.
const u8 gFormChange_WaveGrowth[] = {3, 4, 3, 5};

/// How far each wave is stretched.
const u8 gFormChange_WaveStretch[] = {3, 4, 5, 6};

/// Each shard's place in the crystal, x and y.
const u8 gPrism_ShardStarts[] = {32, 60, 33, 82, 49, 94, 41, 89, 49, 76, 32, 99,
                                 54, 69, 34, 82, 45, 71, 31, 91, 32, 69, 26, 80,
                                 40, 68, 41, 60, 46, 60, 36, 63, 0,  0};

/// Each shard picture's width, in pixels.
const u8 gPrism_ShardWidths[] = {5, 10, 11, 14, 12, 11, 19, 18, 23, 12, 22, 22};

/// Each shard picture's height, in pixels.
const u8 gPrism_ShardHeights[] = {14, 10, 7,  13, 14, 14, 32,
                                  30, 24, 38, 20, 20, 0,  0};

/// Each crystal shard picture's byte offset in Prism's sheet.
const s32 gPrism_ShardPictures[] = {0,   70,   170,  247,  429,  597,
                                    751, 1359, 1899, 2451, 2907, 3347};

/// By mode: how many motes each target gets, and the frames before the targets'
/// share.
const u8 gGatheringLights_Counts[] = {16, 88, 8, 72, 12, 80, 16, 88};

/// The mote's picture for each step of its sway.
const u8 gDjinnBeams_MotePictures[] = {2, 1, 0, 1, 2, 3, 4, 3};

/// The beam's head: its three pictures' byte offsets in the sheet.
const u16 gMirroredWall_Pictures[] = {2702, 3535, 4580};

/// Each head picture's width, in pixels.
const u8 gDjinnBeams_HeadWidths[] = {17, 19, 20};

/// Each head picture's height, in pixels.
const u8 gDjinnBeams_HeadHeights[] = {49, 55, 64};

/// Each mode's record: whether its shots burst into shards (else into smoke),
/// the shots' blitter, shots per target, and frames before the targets' share.
const s8 gLobbedShells_Styles[] = {
    0, 0, 1, 72, 0, 0, 1, 72, 0, 0, 1, 72, 1, 0, 1, 40, 1, 0, 1, 56,
    0, 0, 1, 56, 0, 0, 1, 72, 1, 0, 4, 52, 0, 1, 1, 72, 0, 0, 1, 72};

/// Each shard picture's width, in pixels.
const u8 gLobbedShells_ShardWidths[] = {9, 10, 9, 9, 9, 11, 11, 11, 12,
                                        6, 7,  7, 7, 6, 6,  8,  8,  8};

/// Each shard picture's height, in pixels.
const u8 gLobbedShells_ShardHeights[] = {12, 12, 12, 12, 12, 12, 12, 12, 12,
                                         8,  7,  6,  6,  8,  6,  4,  3,  3};

/// Each shard picture's byte offset in the sheet.
const u16 gLobbedShells_ShardPictures[] = {0,    108,  228,  336,  444,  552,
                                           684,  816,  948,  1092, 1140, 1189,
                                           1231, 1273, 1321, 1357, 1389, 1413};

/// Each panel's x, seven for each side.
const u8 gCondemn_PictureX[] = {10, 30, 0, 0, 0, 6, 52, 60, 0, 0, 0, 0, 0, 0};

/// Each panel's y.
const u8 gCondemn_PictureY[] = {0, 20, 2, 2, 34, 64, 68, 0, 9, 4, 4, 4, 4, 4};

/// The triangle's first corner, (0, 24, 0).
const s32 gBind_Vertex[] = {0, 24, 0};

/// The striking triangle's first corner, (0, 24, 0).
const s32 gBind_TriangleStrikeVertex[] = {0, 24, 0};

/// The two ring points, alternating.
const s32 gStar_Points[] = {0, toFIXED(48), 0, 0, toFIXED(28), 0};

/// The ring radii, alternating: 32 and 16.
const u8 gStar_Radii[] = {32, 16};

/// The nine comets' grid: each comet's x.
const u8 gProcne_GridX[] = {0, 32, 64, 0, 32, 64, 0, 32, 64};

/// The nine comets' grid: each comet's depth.
const u8 gProcne_GridY[] = {0, 0, 0, 32, 32, 32, 64, 64, 64};

/// The frames of the four comet strikes.
const u16 gProcne_Frames[] = {334, 347, 359, 372};

/// A strike's three pictures: their widths.
const u8 gProcne_PictureWidths[] = {14, 24, 48};

/// A strike's three pictures: their heights.
const u8 gProcne_PictureHeights[] = {14, 19, 24};

/// A strike's three pictures: how far each is raised above its mark.
const u8 gProcne_PictureBiasY[] = {8, 12, 17};

/// A strike's three pictures: their byte offsets in the sheet.
const u16 gProcne_Pictures[] = {13824, 14020, 14476};

/// Each grain's added speed.
const u8 gTiamat_EmberSpeeds[] = {0, 8, 12, 16};

/// A spark's two pixels.
const u8 gTiamat_SparkPixels[] = {32, 32};

/// Each grain's size.
const u8 gTiamat_EmberSizes[] = {1, 1, 2, 3};

/// The streak's six pictures: their widths.
const u8 gTiamat_FlameWidths[] = {24, 29, 30, 27, 24, 16};

/// The streak's six pictures: their heights.
const u8 gTiamat_FlameHeights[] = {23, 32, 37, 49, 53, 52};

/// The streak's six pictures: how far below the streak each is drawn.
const u8 gTiamat_FlameDrops[] = {41, 32, 26, 11, 4, 2};

/// The streak's six pictures: their byte offsets in the sheet.
const u16 gTiamat_FlamePictures[] = {0, 552, 1480, 2590, 3913, 5185};

/// The three water columns' feet, x and y.
const u8 gNereid_Columns[] = {16, 104, 32, 104, 48, 104, 64, 104};

/// Each falling spark's gravity.
const s32 gNereid_Gravity[] = {toFIXED(0.5), toFIXED(0.25), toFIXED(0.25),
                               toFIXED(0.125)};

/// Froth, by variant: the FROTH_ columns.
const u8 gVortices_Counts[][2] = {
    // bubbles (at most 30, battle_effect_vortices.c), frames
    [EFFECT_VARIANT_BASIC] = {6, 140},      // Froth
    [EFFECT_VARIANT_STRONGER] = {12, 160},  // Froth Sphere
    [EFFECT_VARIANT_STRONGEST] = {30, 200}, // Froth Spiral
};

/// Whirlwind, by variant: the WHIRLWIND_ columns. The sway is in pixels each
/// way; the shake is BattleCamera_Shake's horizontal range, the vertical
/// twice that.
const u8 gVortices_Shapes[][3] = {
    // tornadoes, sway, screen shake
    [EFFECT_VARIANT_BASIC] = {1, 16, 2},     // Whirlwind, Gale
    [EFFECT_VARIANT_STRONGER] = {2, 24, 2},  // Tornado, Typhoon
    [EFFECT_VARIANT_STRONGEST] = {4, 32, 4}, // Tempest, Hurricane
};

/// The crystals, the frames and the frame the shatter begins, by variant.
const u8 gPrism_Counts[] = {1, 85, 30, 4, 124, 54, 12, 166, 86};

/// The sparks and the shake, by variant.
const u16 gPlasma_Sparks[] = {384, 2, 256, 3, 192, 4};

/// The columns' x, seven for each side.
const u8 gPlasma_Columns[] = {32, 64, 16,  48, 72, 96, 64,
                              88, 56, 104, 72, 48, 24, 56};

/// Plasma's counts, by variant.
const u8 gPlasma_Counts[] = {1, 3, 7};

/// Five halfwords for each variant: the sparks in each group, the groups, and
/// each group's x offset.
const u16 gPlasma_Shapes[][5] = {{128, 1, 16, 0, 0},  {255, 1, 16, 0, 0},
                                 {192, 3, 0, 24, 48}, {340, 1, 16, 24, 48},
                                 {32, 1, 16, 0, 0},   {200, 1, 16, 24, 48}};

/// Each flash picture's byte offset in Plasma's sheet.
const u8 gPlasma_FlashPictures[] = {5, 5, 4, 3, 2, 1};

/// Volcano's counts, by variant.
const u8 gVolcano_Counts[] = {1, 2, 4};

/// The eruptions' depths, four for each variant.
const s8 gVolcano_Depths[][4] = {
    {0, 0, 0, 0}, {24, -24, 0, 0}, {-20, 20, -60, 60}};

/// The frame each eruption starts on.
const u8 gVolcano_StartFrames[] = {8, 18, 24, 30, 34};

/// The flips a glint may be drawn with, one picked at random.
const u8 gTargetMotes_GlintFlips[] = {0, 4, 8, 12};

/// Where each sprite of the charging Zagan sits, x and y.
const u16 gZagan_ChargePoints[][2] = {{0, 0},   {0, 32},  {32, 0},  {32, 32},
                                      {32, 64}, {64, 0},  {64, 32}, {64, 64},
                                      {96, 0},  {96, 32}, {96, 64}};

/// Where each sprite of the galloping Zagan sits, x and y.
const u16 gZagan_GallopPoints[][2] = {{0, 16},  {0, 48}, {32, 0},  {32, 32},
                                      {32, 64}, {64, 0}, {64, 32}, {64, 64},
                                      {64, 96}, {96, 0}, {96, 64}, {96, 96}};

/// The frame each dust ring starts on.
const u8 gZagan_RingStarts[] = {0, 6, 8, 9};

/// How fast each dust ring grows.
const u8 gZagan_RingGrowth[] = {5, 8, 7, 3};

/// Each dust ring's height.
const s8 gZagan_RingHeights[] = {32, 12, -24, 0};

/// One handler for each battle effect, by effect less one; 0 where none.
void (*const gBattleEffect_Handlers[])(struct BattleEffectParams *args) = {
    BattleEffect_RunQuake,                 // 1
    BattleEffect_RunSpire,                 // 2
    BattleEffect_RunGaia,                  // 3
    BattleEffect_RunGrowth,                // 4
    BattleEffect_RunThorn,                 // 5
    BattleEffect_RunFrost,                 // 6
    BattleEffect_RunIce,                   // 7
    BattleEffect_RunPrism,                 // 8
    BattleEffect_RunDouse,                 // 9
    BattleEffect_RunFroth,                 // 10
    BattleEffect_RunFlare,                 // 11
    BattleEffect_RunFire,                  // 12
    BattleEffect_RunVolcano,               // 13
    BattleEffect_RunBlast,                 // 14
    BattleEffect_RunNova,                  // 15
    BattleEffect_RunBolt,                  // 16
    BattleEffect_RunRay,                   // 17
    BattleEffect_RunPlasma,                // 18
    BattleEffect_RunSlash,                 // 19
    BattleEffect_RunWhirlwind,             // 20
    BattleEffect_RunCure,                  // 21
    BattleEffect_RunPly,                   // 22
    BattleEffect_RunWish,                  // 23
    BattleEffect_RunCurePoison,            // 24
    BattleEffect_RunRevive,                // 25
    BattleEffect_RunImpact,                // 26
    BattleEffect_RunDull,                  // 27
    BattleEffect_RunGuard,                 // 28
    BattleEffect_RunImpair,                // 29
    BattleEffect_RunWard,                  // 30
    BattleEffect_RunWeaken,                // 31
    BattleEffect_RunTaint,                 // 32
    BattleEffect_RunDelude,                // 33
    BattleEffect_RunUnusedOrbitingSpirals, // 34
    BattleEffect_RunHeartrender,           // 35
    BattleEffect_RunParalyze,              // 36
    BattleEffect_RunSleep,                 // 37
    BattleEffect_RunBind,                  // 38
    BattleEffect_RunHaunt,                 // 39
    BattleEffect_RunCondemn,               // 40
    BattleEffect_RunCurse,                 // 41
    BattleEffect_RunDrain,                 // 42
    BattleEffect_RunBreak,                 // 43
    BattleEffect_RunWish,                  // 44
    BattleEffect_RunWish,                  // 45
    BattleEffect_RunImpact,                // 46
    BattleEffect_RunImpair,                // 47
    0,                                     // 48
    BattleEffect_RunDragonCloud,           // 49
    BattleEffect_RunDemonNight,            // 50
    BattleEffect_RunHelmSplitter,          // 51
    BattleEffect_RunQuickStrike,           // 52
    BattleEffect_RunDeathPlunge,           // 53
    BattleEffect_RunShuriken,              // 54
    BattleEffect_RunAnnihilation,          // 55
    BattleEffect_RunPunji,                 // 56
    BattleEffect_RunCuttingEdge,           // 57
    BattleEffect_RunRagnarok,              // 58
    BattleEffect_RunHeatWave,              // 59
    BattleEffect_RunPlanetDiver,           // 60
    BattleEffect_RunTitanBlade,            // 61
    BattleEffect_RunShiningStar,           // 62
    BattleEffect_RunBlizzard,              // 63
    BattleEffect_RunDemonFire,             // 64
    BattleEffect_RunVorpalSlash,           // 65
    BattleEffect_RunDrown,                 // 66
    BattleEffect_RunUnusedTriangleStrike,  // 67
    BattleEffect_RunAstralBlast,           // 68
    BattleEffect_RunHeartbreak,            // 69
    0,                                     // 70
    BattleEffect_RunAura,                  // 71
    BattleEffect_RunFume,                  // 72
    BattleEffect_RunBeam,                  // 73
    BattleEffect_RunRagingHeat,            // 74
    BattleEffect_RunCool,                  // 75
    BattleEffect_RunSabreDance,            // 76
    BattleEffect_RunBackstab,              // 77
    BattleEffect_RunJuggle,                // 78
    BattleEffect_RunFlameCard,             // 79
    BattleEffect_RunWhiplash,              // 80
    BattleEffect_RunPoisonFlow,            // 81
    BattleEffect_RunFirePuppet,            // 82
    BattleEffect_RunDiamondDust,           // 83
    BattleEffect_RunDiamondBerg,           // 84
    BattleEffect_RunPlumeEdge,             // 85
    BattleEffect_RunThunderMine,           // 86
    BattleEffect_RunFrostCard,             // 87
    BattleEffect_RunBrambleCard,           // 88
    BattleEffect_RunThunderCard,           // 89
    BattleEffect_RunBaffleCard,            // 90
    BattleEffect_RunSwordCard,             // 91
    BattleEffect_RunSleepCard,             // 92
    BattleEffect_RunDeathCard,             // 93
    0,                                     // 94
    0,                                     // 95
    0,                                     // 96
    0,                                     // 97
    0,                                     // 98
    0,                                     // 99
    BattleEffect_RunStoneJustice,          // 100
    BattleEffect_RunHeavyDivide,           // 101
    0,                                     // 102
    BattleEffect_RunWyrdCurse,             // 103
    BattleEffect_RunAcheronsGrief,         // 104
    BattleEffect_RunHurricane,             // 105
    BattleEffect_RunDreamtide,             // 106
    BattleEffect_RunLetheAlbion,           // 107
    BattleEffect_RunRisingDragon,          // 108
    BattleEffect_RunBoostHack,             // 109
    BattleEffect_RunRadiantFire,           // 110
    BattleEffect_RunFireDance,             // 111
    BattleEffect_RunBlazeRush,             // 112
    BattleEffect_RunShred,                 // 113
    BattleEffect_RunPurgatory,             // 114
    BattleEffect_RunSoulShatter,           // 115
    BattleEffect_RunAgingGas,              // 116
    BattleEffect_RunMadZephyr,             // 117
    BattleEffect_RunLunarSlash,            // 118
    BattleEffect_RunNirvana,               // 119
    BattleEffect_RunLightSurge,            // 120
    BattleEffect_RunVoidBeam,              // 121
    BattleEffect_RunLegend,                // 122
    BattleEffect_RunLiquifier,             // 123
    BattleEffect_RunMegiddo,               // 124
    BattleEffect_RunOdyssey,               // 125
    BattleEffect_RunVengeance,             // 126
    BattleEffect_RunThornyGrave,           // 127
    BattleEffect_RunPlanetary,             // 128
    BattleEffect_RunLifeShear,             // 129
    0,                                     // 130
    0,                                     // 131
    0,                                     // 132
    0,                                     // 133
    0,                                     // 134
    0,                                     // 135
    0,                                     // 136
    0,                                     // 137
    0,                                     // 138
    0,                                     // 139
    0,                                     // 140
    0,                                     // 141
    0,                                     // 142
    0,                                     // 143
    0,                                     // 144
    0,                                     // 145
    0,                                     // 146
    0,                                     // 147
    0,                                     // 148
    0,                                     // 149
    0,                                     // 150
    0,                                     // 151
    0,                                     // 152
    0,                                     // 153
    0,                                     // 154
    0,                                     // 155
    0,                                     // 156
    0,                                     // 157
    0,                                     // 158
    0,                                     // 159
    0,                                     // 160
    0,                                     // 161
    0,                                     // 162
    0,                                     // 163
    0,                                     // 164
    0,                                     // 165
    0,                                     // 166
    0,                                     // 167
    0,                                     // 168
    0,                                     // 169
    0,                                     // 170
    0,                                     // 171
    0,                                     // 172
    0,                                     // 173
    0,                                     // 174
    0,                                     // 175
    0,                                     // 176
    0,                                     // 177
    0,                                     // 178
    0,                                     // 179
    0,                                     // 180
    BattleEffect_RunCrucibleFire,          // 181
    BattleEffect_RunReverseStar,           // 182
    0,                                     // 183
    0,                                     // 184
    0,                                     // 185
    0,                                     // 186
    0,                                     // 187
    0,                                     // 188
    0,                                     // 189
    0,                                     // 190
    0,                                     // 191
    0,                                     // 192
    0,                                     // 193
    0,                                     // 194
    0,                                     // 195
    0,                                     // 196
    0,                                     // 197
    0,                                     // 198
    0,                                     // 199
    BattleEffect_RunGranite,               // 200
    BattleEffect_RunRevive,                // 201
    BattleEffect_RunVine,                  // 202
    BattleEffect_RunGround,                // 203
    BattleEffect_RunPly,                   // 204
    BattleEffect_RunSleet,                 // 205
    BattleEffect_RunWish,                  // 206
    BattleEffect_RunHail,                  // 207
    BattleEffect_RunTonic,                 // 208
    BattleEffect_RunDew,                   // 209
    BattleEffect_RunImpact,                // 210
    BattleEffect_RunGuard,                 // 211
    BattleEffect_RunPsyCrystal,            // 212
    BattleEffect_RunFlash,                 // 213
    BattleEffect_RunTorch,                 // 214
    BattleEffect_RunWard,                  // 215
    BattleEffect_RunZephyr,                // 216
    BattleEffect_RunKite,                  // 217
    BattleEffect_RunBind,                  // 218
    BattleEffect_RunGust,                  // 219
    BattleEffect_RunIron,                  // 220
    BattleEffect_RunSteam,                 // 221
    BattleEffect_RunShade,                 // 222
    BattleEffect_RunImpact,                // 223
    BattleEffect_RunCoal,                  // 224
    BattleEffect_RunCure,                  // 225
    BattleEffect_RunSalt,                  // 226
    BattleEffect_RunSpring,                // 227
    BattleEffect_RunBreath,                // 228
    BattleEffect_RunEther,                 // 229
    BattleEffect_RunBalm,                  // 230
    BattleEffect_RunTinder,                // 231
    BattleEffect_RunWhorl,                 // 232
    BattleEffect_RunGale,                  // 233
    BattleEffect_RunWaft,                  // 234
    0,                                     // 235
    BattleEffect_RunCore,                  // 236
    BattleEffect_RunMud,                   // 237
    BattleEffect_RunHaze,                  // 238
    BattleEffect_RunPetra,                 // 239
    BattleEffect_RunRime,                  // 240
    BattleEffect_RunLull,                  // 241
    BattleEffect_RunGasp,                  // 242
    BattleEffect_RunFugue,                 // 243
    BattleEffect_RunCrystal,               // 244
    BattleEffect_RunEddy,                  // 245
    BattleEffect_RunCannon,                // 246
    BattleEffect_RunAroma,                 // 247
    BattleEffect_RunReflux,                // 248
    BattleEffect_RunDjinnCounter,          // 249
    BattleEffect_RunMold,                  // 250
    BattleEffect_RunMeld,                  // 251
    BattleEffect_RunShine,                 // 252
    BattleEffect_RunGeode,                 // 253
    BattleEffect_RunSerac,                 // 254
    BattleEffect_RunSpark,                 // 255
    BattleEffect_RunFury,                  // 256
    BattleEffect_RunChar,                  // 257
    0,                                     // 258
    0,                                     // 259
    0,                                     // 260
    0,                                     // 261
    0,                                     // 262
    0,                                     // 263
    0,                                     // 264
    0,                                     // 265
    0,                                     // 266
    0,                                     // 267
    0,                                     // 268
    0,                                     // 269
    0,                                     // 270
    0,                                     // 271
    0,                                     // 272
    0,                                     // 273
    0,                                     // 274
    0,                                     // 275
    0,                                     // 276
    0,                                     // 277
    0,                                     // 278
    0,                                     // 279
    BattleEffect_RunVenus,                 // 280
    BattleEffect_RunMercury,               // 281
    BattleEffect_RunMars,                  // 282
    BattleEffect_RunJupiter,               // 283
    0,                                     // 284
    0,                                     // 285
    0,                                     // 286
    0,                                     // 287
    0,                                     // 288
    0,                                     // 289
    0,                                     // 290
    0,                                     // 291
    0,                                     // 292
    0,                                     // 293
    0,                                     // 294
    0,                                     // 295
    0,                                     // 296
    0,                                     // 297
    0,                                     // 298
    0,                                     // 299
    BattleEffect_RunSonicWave,             // 300
    BattleEffect_RunShriek,                // 301
    BattleEffect_RunBansheeHowl,           // 302
    BattleEffect_RunCrazyVoice,            // 303
    BattleEffect_RunWarCry,                // 304
    BattleEffect_RunWickedHowl,            // 305
    BattleEffect_RunWingBeat,              // 306
    BattleEffect_RunWingFlutter,           // 307
    BattleEffect_RunRumble,                // 308
    BattleEffect_RunBoneChiller,           // 309
    BattleEffect_RunGust,                  // 310
    BattleEffect_RunBoneCharge,            // 311
    BattleEffect_RunMysticFlame,           // 312
    BattleEffect_RunNumbingSting,          // 313
    BattleEffect_RunBruteForce,            // 314
    BattleEffect_RunStickyGoo,             // 315
    BattleEffect_RunCannibalFang,          // 316
    BattleEffect_RunBearClaw,              // 317
    BattleEffect_RunPoisonousBite,         // 318
    BattleEffect_RunFlyingAttack,          // 319
    BattleEffect_RunUndeadSword,           // 320
    BattleEffect_RunRansack,               // 321
    BattleEffect_RunStickyPoison,          // 322
    BattleEffect_RunPoisonousBite,         // 323
    BattleEffect_RunElectricBite,          // 324
    BattleEffect_RunPoisonTail,            // 325
    BattleEffect_RunOnslaught,             // 326
    BattleEffect_RunVampiricFang,          // 327
    BattleEffect_RunBacteriaRush,          // 328
    BattleEffect_RunFlyingAttack,          // 329
    BattleEffect_RunRottenBlood,           // 330
    0,                                     // 331
    BattleEffect_RunDoubleFang,            // 332
    BattleEffect_RunMortalBlow,            // 333
    BattleEffect_RunFreebiteRush,          // 334
    BattleEffect_RunTwinBeaks,             // 335
    BattleEffect_RunRabidFang,             // 336
    BattleEffect_RunAcidBite,              // 337
    BattleEffect_RunDynamite,              // 338
    BattleEffect_RunHeadbutt,              // 339
    BattleEffect_RunPoisonInk,             // 340
    0,                                     // 341
    BattleEffect_RunBearClaw,              // 342
    BattleEffect_RunRansack,               // 343
    BattleEffect_RunSoothingStar,          // 344
    BattleEffect_RunSpiderWeb,             // 345
    BattleEffect_RunHeartrender,           // 346
    BattleEffect_RunMadSpatter,            // 347
    BattleEffect_RunSpasm,                 // 348
    BattleEffect_RunSleepStar,             // 349
    BattleEffect_RunDecompose,             // 350
    BattleEffect_RunHaunting,              // 351
    BattleEffect_RunCure,                  // 352
    BattleEffect_RunBerserk,               // 353
    BattleEffect_RunCurse,                 // 354
    0,                                     // 355
    0,                                     // 356
    0,                                     // 357
    0,                                     // 358
    0,                                     // 359
    0,                                     // 360
    0,                                     // 361
    0,                                     // 362
    0,                                     // 363
    0,                                     // 364
    0,                                     // 365
    0,                                     // 366
    0,                                     // 367
    0,                                     // 368
    0,                                     // 369
    0,                                     // 370
    0,                                     // 371
    0,                                     // 372
    0,                                     // 373
    0,                                     // 374
    0,                                     // 375
    0,                                     // 376
    0,                                     // 377
    0,                                     // 378
    0,                                     // 379
    BattleEffect_RunSmokeBomb,             // 380
    BattleEffect_RunWingStroke,            // 381
    BattleEffect_RunPoisonBeat,            // 382
    BattleEffect_RunSpinningBeat,          // 383
    BattleEffect_RunOuterSpace,            // 384
    BattleEffect_RunDragonDriver,          // 385
    BattleEffect_RunDrainFang,             // 386
    BattleEffect_RunSevereBlow,            // 387
    BattleEffect_RunThrash,                // 388
    BattleEffect_RunRecovery,              // 389
    BattleEffect_RunEchoCut,               // 390
    BattleEffect_RunWateryGrave,           // 391
    BattleEffect_RunRagingFlood,           // 392
    BattleEffect_RunHydroSlash,            // 393
    BattleEffect_RunMightyPress,           // 394
    BattleEffect_RunSlaver,                // 395
    BattleEffect_RunCounterRush,           // 396
    BattleEffect_RunDjinnfest,             // 397
    BattleEffect_RunHeatKiss,              // 398
    BattleEffect_RunAngleSpike,            // 399
    BattleEffect_RunFireBreath,            // 400
    BattleEffect_RunWaterBreath,           // 401
    BattleEffect_RunIceBlessing,           // 402
    BattleEffect_RunDarkBreath,            // 403
    BattleEffect_RunAcidBreath,            // 404
    BattleEffect_RunStormBreath,           // 405
    BattleEffect_RunEvilBlessing,          // 406
    BattleEffect_RunDeadlyGas,             // 407
    BattleEffect_RunSandBreath,            // 408
    BattleEffect_RunDesertGasp,            // 409
    BattleEffect_RunBlackIce,              // 410
    BattleEffect_RunTyphoonBlow,           // 411
    BattleEffect_RunToxicBreath,           // 412
    BattleEffect_RunGravelBlow,            // 413
    BattleEffect_RunBlastBreath,           // 414
    BattleEffect_RunDarksolGasp,           // 415
    BattleEffect_RunFireBreath,            // 416
    0,                                     // 417
    0,                                     // 418
    0,                                     // 419
    0,                                     // 420
    BattleEffect_RunManeater,              // 421
    BattleEffect_RunUrFlash,               // 422
    BattleEffect_RunCrusherGrip,           // 423
    BattleEffect_RunVanishClaw,            // 424
    BattleEffect_RunTwinShear,             // 425
    BattleEffect_RunTripleChomp,           // 426
    BattleEffect_RunDoublestep,            // 427
    BattleEffect_RunPoisonGel,             // 428
    BattleEffect_RunDelude,                // 429
    BattleEffect_RunRisingVenom,           // 430
    BattleEffect_RunSack,                  // 431
    BattleEffect_RunFireDanceUnleash,      // 432
    BattleEffect_RunStunMuscle,            // 433
    BattleEffect_RunRollingFlame,          // 434
    BattleEffect_RunCage,                  // 435
    BattleEffect_RunPhoenix,               // 436
    BattleEffect_RunHumanHunt,             // 437
    BattleEffect_RunUnusedHeavyBite,       // 438
    BattleEffect_RunUnusedFlashBite,       // 439
    BattleEffect_RunPoisonTail,            // 440
    BattleEffect_RunKillSting,             // 441
    BattleEffect_RunBeastNeedle,           // 442
    BattleEffect_RunPoisonSting,           // 443
    BattleEffect_RunStunSting,             // 444
    BattleEffect_RunDemonEye,              // 445
    BattleEffect_RunEmu,                   // 446
    BattleEffect_RunOceanFist,             // 447
    BattleEffect_RunCallDullahan,          // 448
    BattleEffect_RunGhostSoldier,          // 449
    BattleEffect_RunRisingDragonEnemy,     // 450
    BattleEffect_RunTrueCollide,           // 451
    BattleEffect_RunStarMine,              // 452
    BattleEffect_RunFormicAcid,            // 453
    BattleEffect_RunClawAttack,            // 454
    BattleEffect_RunBeatDance,             // 455
    BattleEffect_RunUnusedSparkBite,       // 456
    BattleEffect_RunLivingArmor,           // 457
    BattleEffect_RunHeatStun,              // 458
    BattleEffect_RunEarthForce,            // 459
    BattleEffect_RunGuardAura,             // 460
    BattleEffect_RunDjinnBlast,            // 461
    BattleEffect_RunDjinnStorm,            // 462
    BattleEffect_RunMineBall,              // 463
    BattleEffect_RunElementSwap,           // 464
    BattleEffect_RunDjinnStun,             // 465
    BattleEffect_RunMysticCall,            // 466
    BattleEffect_RunClarionCry,            // 467
    BattleEffect_RunPsyBoost,              // 468
    BattleEffect_RunStrongHit,             // 469
    BattleEffect_RunChillBreath,           // 470
    BattleEffect_RunStormBreathStrong,     // 471
};

/// The poses Cybele's sprites take, a pair for each pose.
const u8 gCybele_ObjectPoses[] = {8, 10, 9, 11, 3, 4};

/// Each thorn picture's byte offset in Cybele's sheet.
const u16 gCybele_ThornPictures[] = {3920, 3932, 3947, 3955, 3970};

/// Each thorn picture's width, in pixels.
const u8 gCybele_ThornWidths[] = {2, 5, 2, 5, 3};

/// Each thorn picture's height, in pixels.
const u8 gCybele_ThornHeights[] = {6, 3, 4, 3, 3};

/// Each flower picture's byte offset in Cybele's sheet.
const u16 gCybele_FlowerPictures[] = {3456, 3712, 3856};

/// Each flower's size, in pixels.
const u8 gCybele_FlowerSizes[] = {16, 12, 8};

/// Each chip picture's byte offset in Cybele's sheet.
const u16 gCybele_ChipPictures[] = {3920, 3932, 3947, 3955, 3970};

/// Each chip picture's width, in pixels.
const u8 gCybele_ChipWidths[] = {2, 5, 2, 5, 3};

/// Each chip picture's height, in pixels.
const u8 gCybele_ChipHeights[] = {6, 3, 4, 3, 3};

/// Each spark picture's byte offset in Cybele's sheet.
const u16 gCybele_SparkPictures[] = {3979, 4043, 4268, 4389, 4470};

/// Each spark's size, in pixels.
const u8 gCybele_SparkSizes[] = {8, 15, 11, 9, 5};

/// Each blast picture's byte offset in Cybele's sheet.
const u16 gCybele_BlastPictures[] = {5760,  6016,  7040, 9344,
                                     11648, 13952, 16256};

/// Each blast's size, in pixels.
const u16 gCybele_BlastSizes[] = {16, 32, 48, 48, 48, 48, 48};

/// Each crystal's point, x and y.
const u8 gBoreas_Points[][2] = {{57, 130}, {75, 128}, {71, 104}, {101, 130},
                                {86, 126}, {85, 98},  {64, 98},  {98, 96},
                                {77, 87},  {84, 88},  {68, 67},  {81, 49},
                                {94, 70},  {70, 51},  {93, 36},  {81, 50}};

/// Each shake's offset.
const s8 gBoreas_ShakeOffsets[] = {-4, -3, -2, 0};

/// Each crystal picture's byte offset in Boreas's sheet.
const u16 gBoreas_CrystalPictures[] = {0, 1152, 2064};

/// Each crystal picture's width, in pixels.
const u8 gBoreas_CrystalWidths[] = {24, 24, 20};

/// Each crystal picture's height, in pixels.
const u8 gBoreas_CrystalHeights[] = {48, 38, 26};

/// The animations Boreas's sprites cycle through, one every four frames.
const u8 gBoreas_Animations[] = {2, 9, 10, 11};

/// Each debris picture's width, in pixels.
const u8 gBoreas_DebrisWidths[] = {12, 11, 8, 10, 8, 12, 5, 7, 7, 6, 7};

/// Each debris picture's height, in pixels.
const u8 gBoreas_DebrisHeights[] = {10, 16, 14, 15, 4, 4, 3, 4, 6, 12, 10};

/// Each debris picture's byte offset in Boreas's sheet.
const u16 gBoreas_DebrisPictures[] = {2584, 2704, 2880, 2992, 5062, 5094,
                                      5142, 5157, 5185, 5227, 5299};

/// Each spray picture's width, in pixels.
const u8 gBoreas_SprayWidths[] = {16, 16, 16};

/// Each spray picture's height, in pixels.
const u8 gBoreas_SprayHeights[] = {16, 16, 16};

/// Each spray picture's byte offset in Boreas's sheet.
const u16 gBoreas_SprayPictures[] = {4294, 4550, 4806};

/// Each rising crystal picture's byte offset in Boreas's sheet.
const u16 gBoreas_RisingCrystalPictures[] = {0, 1152, 2064};

/// Each rising crystal picture's width, in pixels.
const u8 gBoreas_RisingCrystalWidths[] = {24, 24, 20};

/// Each rising crystal picture's height, in pixels.
const u8 gBoreas_RisingCrystalHeights[] = {48, 38, 26};

/// Each fall picture's width, in pixels.
const u8 gBoreas_FallWidths[] = {24, 24, 20};

/// Each fall picture's height, in pixels.
const u8 gBoreas_FallHeights[] = {48, 38, 26};

/// Each fall picture's byte offset in Boreas's sheet.
const u16 gBoreas_FallPictures[] = {0, 1152, 2064};

/// The gravities Drain's motes fall with.
const s32 gDrain_Gravities[] = {toFIXED(0.25), toFIXED(0.125), toFIXED(0.25),
                                toFIXED(0.5)};

/// The mote's size, by mode.
const u8 gDrainMotes_Sizes[] = {6, 6, 10};

/// Each variant's timing: its frames, its rise and its shake.
const u8 gQuake_Timings[][3] = {{80, 32, 2}, {112, 64, 4}, {144, 96, 8}};

/// Fire's counts, by variant.
const u8 gFire_Counts[] = {16, 32, 64};

/// Each flare picture's width, in pixels.
const u8 gFire_FlareWidths[] = {24, 29, 30, 27, 24, 16};

/// Each flare picture's height, in pixels.
const u8 gFire_FlareHeights[] = {23, 32, 37, 49, 53, 52};

/// How far below its anchor each flare picture's top goes.
const u8 gFire_FlareBiasY[] = {41, 32, 26, 11, 4, 2};

/// Each flare picture's byte offset in Fire's sheet.
const u16 gFire_FlarePictures[] = {0, 552, 1480, 2590, 3913, 5185};

/// Where each frost mound rises, x and y.
const u8 gFrost_MoundSpots[] = {53, 108, 73, 110, 29, 110, 73, 108, 33, 104,
                                96, 106, 8,  109, 62, 98,  33, 100, 87, 100,
                                21, 95,  53, 90,  33, 90,  72, 91,  82, 91,
                                77, 86,  52, 84,  18, 92,  56, 81};

/// Each variant's timing, three bytes.
const u8 gFrost_Timings[] = {32, 8, 85, 48, 12, 97, 64, 19, 109};

/// Each mote picture's width, in pixels.
const u8 gFrost_MoteWidths[] = {12, 10, 7, 10, 8, 8, 8, 8};

/// Each mote picture's height, in pixels.
const u8 gFrost_MoteHeights[] = {9, 16, 14, 15, 8, 8, 8, 8};

/// Each mote picture's byte offset in Frost's sheet.
const u16 gFrost_MoteCells[] = {1792, 1900, 2060, 2158, 2308, 2372, 2436, 2500};

/// Each mound picture's byte offset in Frost's sheet.
const u16 gFrost_MoundPictures[] = {0, 768, 1280};

/// Each mound picture's height, in pixels.
const u8 gFrost_MoundHeights[] = {24, 16, 16};

/// Each chip picture's width, in pixels.
const u8 gFrost_ChipWidths[] = {12, 10, 7, 10, 8, 8, 8, 8};

/// Each chip picture's height, in pixels.
const u8 gFrost_ChipHeights[] = {9, 16, 14, 15, 8, 8, 8, 8};

/// Each ice chip picture's byte offset in Frost's sheet.
const u16 gFrost_ChipCells[] = {1792, 1900, 2060, 2158, 2308, 2372, 2436, 2500};

/// The sparks, a pair for each variant.
const u8 gRay_Sparks[][2] = {{2, 4}, {2, 6}, {4, 8}};

/// Eruption's counts, by variant.
const u8 gEruption_Counts[][3] = {{16, 8, 72}, {32, 16, 80}, {128, 32, 88}};

/// Each puff picture's byte offset in Eruption's sheet.
const u16 gEruption_PuffPictures[] = {0, 256, 1280, 3584, 5888, 8192, 10496};

/// Each puff's size, in pixels.
const u16 gEruption_PuffSizes[] = {16, 32, 48, 48, 48, 48, 48};

/// The flips a glint may be drawn with, one picked at random.
const u8 gProjectileVolleyB_GlintFlips[] = {0, 4, 8, 12};

/// Atalanta's sprites' scale, one to one.
const s32 gAtalanta_UnitScale[] = {toFIXED(1), toFIXED(1)};

/// Each vine picture's width, in pixels.
const u8 gGrowth_VineWidths[] = {21, 28, 19};

/// The tallest a column grows, by column in threes.
const u8 gGrowth_HeightLimits[] = {119, 119, 102};

/// Each vine picture's byte offset in Growth's sheet.
const u16 gGrowth_VinePictures[] = {0, 2499, 5831};

/// Each pillar picture's width, in pixels.
const u8 gGrowth_PillarWidths[] = {10, 8, 9};

/// Each pillar picture's byte offset in Growth's sheet.
const u16 gGrowth_PillarPictures[] = {0, 1080, 1888};

/// The columns, by strength.
const u8 gGrowth_ColumnCounts[] = {4, 8, 12};

/// Each column's root, its x.
const u8 gGrowth_ColumnX[] = {64, 72,  56, 80, 48, 88, 40, 96,
                              32, 104, 24, 72, 56, 80, 48, 88};

/// Each pillar picture's height, in pixels.
const s8 gGrowth_PillarHeights[] = {100, 60, 88, 96, 64, 104, 82, 98};

/// Each puff picture's width, in pixels.
const s8 gGrowth_PuffWidths[] = {5, 7, 11, 16, 23, 27, 16};

/// Each puff picture's height, in pixels.
const u8 gGrowth_PuffHeights[] = {10, 14, 21, 32, 45, 50, 32};

/// Each dust puff picture's byte offset in Growth's sheet.
const u16 gGrowth_PuffPictures[] = {0, 50, 148, 379, 891, 1926, 3276};

/// Each vine's x about the middle of the targets.
const s8 gGrowth_ColumnOffsets[] = {0,   14, -32, 8,   32, -48, 4,   12,
                                    -24, 24, -12, -40, -4, -16, -32, 12};

/// Each thorn's kind: 0 and 1 large, 2 and 3 small; the odd ones grow to the
/// left.
const u8 gGrowth_Kinds[] = {0, 3, 2, 1, 2, 3, 2, 1};

/// Each thorn's root, its x.
const s8 gGrowth_X[] = {24, 80, 24, 48, 88, 40, 8, 96};

/// The thorns, by strength.
const u8 gGrowth_Counts[] = {3, 5, 8};

/// Each thorn puff picture's width, in pixels.
const s8 gGrowth_ThornPuffWidths[] = {5, 7, 11, 17, 32, 32, 19};

/// Each dust puff picture's height, in pixels, as a thorn bursts out.
const u8 gGrowth_ThornPuffHeights[] = {10, 14, 21, 34, 59, 60, 38};

/// Each thorn puff picture's byte offset in Growth's sheet.
const u16 gGrowth_ThornPuffPictures[] = {5888, 5938, 6036, 6267,
                                         6845, 8733, 10653};

/// Four bytes for each strength: the sparks, the glints, the glints' spread and
/// the bolts.
const u8 gBolt_Counts[] = {16, 0, 15, 1, 32, 3, 31, 1, 128, 5, 48, 3};

/// The flips a glint may be drawn with, one picked at random.
const u8 gBolt_GlintFlips[] = {0, 4, 8, 12};

/// How the glints are blended, by variant.
const u8 gBolt_GlintModes[] = {1, 1, 0};

/// How high each kind of comet flies.
const u8 gProjectileVolley_CometHeights[] = {24, 36, 48};

/// Each volley mode's record, five bytes.
const u8 gProjectileVolley_Modes[][5] = {
    {6, 10, 3, 32, 4},  {4, 10, 5, 32, 8},   {10, 32, 3, 32, 4},
    {5, 10, 5, 32, 4},  {10, 10, 1, 32, 4},  {1, 5, 0, 32, 80},
    {7, 8, 2, 32, 40},  {30, 24, 1, 32, 4},  {6, 2, 3, 32, 4},
    {60, 10, 1, 32, 4}, {60, 12, 1, 32, 4},  {10, 10, 1, 32, 4},
    {7, 8, 2, 32, 40},  {20, 12, 1, 32, 40}, {30, 24, 2, 32, 8},
    {30, 16, 1, 32, 8}, {30, 16, 1, 32, 8},  {30, 16, 1, 32, 8},
    {60, 12, 1, 32, 4}};

/// Each flash picture's width, in pixels.
const u8 gProjectileVolley_FlashWidths[] = {12, 18, 24, 30, 36, 39, 40};

/// Each flash picture's height, in pixels.
const u8 gProjectileVolley_FlashHeights[] = {10, 15, 18, 22, 27, 28, 30};

/// How far each step of the flash spreads.
const u8 gProjectileVolley_FlashSteps[] = {2, 3, 4, 4, 5, 6, 6};

/// How far each step of the flash rises.
const u8 gProjectileVolley_FlashRises[] = {0, 3, 5, 7, 10, 12, 12, 0};

/// Each flash picture's byte offset in Projectile Volley's sheet.
const u16 gProjectileVolley_FlashPictures[] = {8192, 8312,  8582, 9014,
                                               9674, 10646, 11738};

/// Each pellet picture's byte offset in Projectile Volley's sheet.
const u16 gProjectileVolley_PelletPictures[] = {2308, 2372, 2436, 2500};

/// The glints' flips, one picked at random.
const u8 gProjectileVolley_GlintFlips[] = {0, 4, 8, 12};

/// The frames the Djinni throws its five flames on.
const u8 gDjinnAttacks_LaunchFrames[] = {4, 7, 10, 21, 34};

/// Each Hail picture's width, in pixels.
const u8 gHail_PictureWidths[] = {8, 8, 8, 8};

/// Each Hail picture's height, in pixels.
const u8 gHail_PictureHeights[] = {8, 8, 8, 8};

/// The ice chip's four pictures: their byte offsets; the tables after it give
/// their widths and heights.
const u16 gHail_Pictures[] = {2308, 2372, 2436, 2500};

/// The chips' flips, a blitter's flags; the tables after it give, by shape,
/// their pictures, places and sizes.
const u8 gHail_CrackleFlips[] = {3, 7, 11, 15};

/// Each portal picture's width, in pixels.
const u8 gHail_PortalWidths[] = {24, 7, 7, 8, 6, 13, 10, 11, 7};

/// Each portal picture's height, in pixels.
const u8 gHail_PortalHeights[] = {48, 13, 14, 24, 24, 17, 20, 14, 20};

/// Each portal picture's byte offset in Hail's sheet.
const u16 gHail_PortalPictures[] = {0,    1152, 1243, 1341, 1533,
                                    1677, 1898, 2098, 2252};

/// Each portal's x, in pixels.
const u8 gHail_PortalX[] = {0, 9, 9, 11, 10, 0, 3, 13, 7};

/// Each portal's y, in pixels.
const u8 gHail_PortalY[] = {0, 18, 18, 0, 24, 20, 5, 22, 26};

/// The palettes a struck target flashes in, one picked at random.
const u8 gThor_TargetPalettes[] = {7, 14, 10, 4};

/// The flips a crackle may be drawn with, one picked at random.
const u8 gThor_CrackleFlips[] = {0, 4, 8, 12};

/// Each shard's place in the boulder, x and y.
const u8 gSpire_ShardOffsets[] = {
    65, 104, 68, 95, 64, 93, 61, 93, 64, 87, 72, 85, 56, 88, 51, 80,
    61, 75,  72, 81, 69, 71, 66, 70, 71, 64, 49, 71, 59, 69, 53, 68,
    78, 73,  55, 67, 75, 62, 61, 65, 53, 61, 0,  0,  0,  0,  0,  0,
    0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};

/// Each boulder's x.
const s8 gSpire_RockX[] = {0, -12, 12, -8, 16};

/// The drop frames.
const u8 gSpire_DropFrames[] = {8, 16, 20, 24, 32};

/// The boulders, by variant.
const u8 gSpire_RockCounts[] = {1, 3, 5};

/// A flurry's style, one row a mode (enum FlurryMode), one byte a column (enum
/// FlurryColumn).
const u8 gFlurry_Styles[][8] = {
    {1, 0, 5, 16, 12, 80, 0, 0},  {1, 1, 7, 16, 12, 104, 0, 0},
    {1, 2, 9, 16, 12, 128, 0, 0}, {1, 3, 1, 16, 16, 36, 0, 0},
    {1, 3, 2, 20, 16, 52, 0, 0},  {1, 3, 3, 24, 16, 68, 0, 0},
    {0, 2, 1, 32, 12, 28, 1, 0},  {0, 2, 2, 32, 12, 40, 1, 0},
    {0, 2, 3, 32, 12, 52, 1, 0},  {1, 0, 3, 16, 12, 50, 0, 0},
    {1, 0, 1, 16, 12, 40, 0, 1},  {1, 0, 1, 16, 12, 80, 0, 2},
    {1, 3, 1, 16, 16, 30, 0, 0}};

/// A glint's four flips.
const u8 gFlurry_GlintFlips[] = {0, 4, 8, 12, 0};

/// Each row picture's byte offset in Gaia's sheet.
const u16 gGaia_RowPictures[] = {0,    160,  352,  640, 1024,
                                 1504, 2080, 2752, 3776};

/// The growing wall's nine rows: their widths.
const u8 gGaia_RowWidths[] = {5, 6, 9, 12, 15, 18, 21, 32, 40};

/// Each flame picture's byte offset in Gaia's sheet.
const u16 gGaia_FlamePictures[] = {10672, 11505, 12550};

/// The flame's three pictures: their widths.
const u8 gGaia_FlameWidths[] = {17, 19, 20};

/// Each flame picture's height, in pixels.
const u8 gGaia_FlameHeights[] = {49, 55, 64};

/// The wall layer's shift, by side and variant.
const s8 gGaia_Bg2Shifts[][3] = {{-8, 0, 12}, {-72, -40, -16}};

/// The x of each rock that falls late.
const u8 gGaia_LateRockColumns[] = {136, 40, 120, 56, 104, 72};

/// Each sprite's x, in pixels.
const u8 gUlysses_ObjectX[] = {0, 32, 0, 32, 0, 32, 32};

/// Each sprite's depth, in pixels.
const u8 gUlysses_ObjectZ[] = {8, 0, 40, 32, 72, 64, 96};

/// Each Ulysses picture's width, in pixels.
const u8 gUlysses_PictureWidths[] = {12, 9, 6,  5,  8,  9, 12, 9, 8,
                                     8,  8, 10, 11, 10, 8, 6,  6, 9};

/// Each Ulysses picture's height, in pixels.
const u8 gUlysses_PictureHeights[] = {18, 17, 15, 12, 14, 15, 16, 13, 8,
                                      4,  4,  6,  9,  9,  10, 10, 13, 16};

/// Each Ulysses picture's byte offset in Ulysses's sheet.
const u16 gUlysses_Pictures[] = {0,    216,  369,  459,  519,  631,
                                 766,  958,  1075, 1139, 1171, 1203,
                                 1263, 1362, 1452, 1532, 1592, 1670};

/// Where each of the six particles starts: x and y in pixels, z in 1024ths of
/// a pixel, and its x and y speeds in halves of a pixel a frame.
const s8 gUlysses_ParticleStarts[][5] = {
    {100, 60, 64, -6, -6}, {112, 44, -64, -6, 4}, {96, 28, 64, -6, -5},
    {112, 80, -64, -6, 4}, {104, 32, 64, -6, 5},  {102, 68, -64, -6, 4}};

/// Each Spider Web picture's width, in pixels.
const u8 gSpiderWeb_PictureWidths[] = {40, 112, 112, 112, 88, 64};

/// Each Spider Web picture's height, in pixels.
const u8 gSpiderWeb_PictureHeights[] = {56, 80, 64, 64, 80, 88};

/// The swirl's six pictures: their byte offsets; the tables after it give their
/// x (six for each side), widths, rise and heights.
const u16 gSpiderWeb_Pictures[] = {0, 2240, 11200, 18368, 0, 7040};

/// Each picture's x, in pixels.
const u8 gSpiderWeb_PictureX[] = {72, 0, 0, 0, 32, 64, 16, 16, 16, 16, 8, 0};

/// How far below its anchor each picture's top goes.
const s8 gSpiderWeb_PictureBiasY[] = {0, 4, 24, -6, -20, -28};

/// Each part's point, x and y.
const u8 gAzul_PartPoints[][2] = {
    {23, 13},   {0, 0},     {42, 0},    {28, 41},   {28, 41},   {48, 28},
    {42, 0},    {78, 31},   {28, 41},   {28, 41},   {66, 7},    {42, 0},
    {79, 0},    {78, 31},   {78, 31},   {94, 8},    {79, 0},    {128, 0},
    {78, 31},   {78, 31},   {112, 28},  {128, 0},   {132, 48},  {78, 31},
    {78, 31},   {162, 12},  {182, 0},   {174, 42},  {128, 0},   {128, 0},
    {145, 39},  {128, 0},   {174, 42},  {134, 71},  {134, 71},  {196, 12},
    {182, 0},   {239, 0},   {174, 42},  {172, 42},  {231, 20},  {239, 0},
    {239, 52},  {211, 16},  {211, 16},  {7, 33},    {0, 0},     {28, 41},
    {0, 48},    {0, 48},    {59, 49},   {78, 31},   {69, 73},   {28, 41},
    {28, 41},   {91, 51},   {78, 31},   {132, 48},  {69, 73},   {69, 73},
    {16, 58},   {28, 41},   {20, 86},   {0, 48},    {0, 48},    {36, 70},
    {28, 41},   {69, 73},   {20, 86},   {20, 86},   {96, 76},   {132, 48},
    {99, 97},   {69, 73},   {69, 73},   {6, 80},    {0, 48},    {20, 86},
    {0, 98},    {0, 98},    {168, 66},  {174, 42},  {205, 88},  {134, 71},
    {134, 71},  {194, 48},  {211, 16},  {205, 88},  {174, 42},  {174, 42},
    {217, 52},  {211, 16},  {239, 52},  {205, 88},  {208, 88},  {7, 114},
    {0, 98},    {30, 119},  {0, 119},   {0, 119},   {29, 106},  {20, 86},
    {64, 119},  {30, 119},  {0, 98},    {51, 93},   {69, 73},   {64, 119},
    {20, 86},   {20, 86},   {78, 96},   {69, 73},   {99, 97},   {64, 119},
    {64, 119},  {98, 112},  {99, 97},   {137, 104}, {112, 119}, {64, 119},
    {123, 85},  {132, 48},  {137, 104}, {99, 97},   {99, 97},   {156, 88},
    {134, 71},  {205, 88},  {137, 104}, {137, 104}, {139, 115}, {137, 104},
    {175, 119}, {112, 119}, {112, 119}, {217, 52},  {211, 16},  {239, 52},
    {205, 88},  {205, 88},  {228, 87},  {239, 52},  {239, 119}, {205, 88},
    {205, 88},  {200, 108}, {185, 93},  {205, 88},  {239, 119}, {175, 119},
    {169, 106}, {185, 93},  {175, 119}, {137, 104}, {137, 104}};

/// The serpent's sprites' scale, one to one.
const struct Scale gAzul_UnitScale = {FIXED_ONE, FIXED_ONE};

/// Each sprite's x, in pixels.
const u8 gAzul_ObjectX[] = {35, 45, 14, 1, 1, 40, 9, 40, 9, 25, 39, 8};

/// Each sprite's depth, in pixels.
const u8 gAzul_ObjectZ[] = {0, 32, 32, 64, 96, 51, 51, 83, 83, 106, 129, 129};

/// The late sprites' scale, one to one.
const struct Scale gAzul_LateUnitScale = {FIXED_ONE, FIXED_ONE};

/// Each late sprite's x, in pixels.
const s8 gAzul_LateObjectX[] = {0, 32, 32, 64, 96, 50, 51, 82};

/// Each late sprite's depth, in pixels.
const s8 gAzul_LateObjectZ[] = {10, 0, 32, 45, 45, 5, 36, 5};

/// Each Beam picture's width, in pixels.
const u8 gBeam_PictureWidths[] = {38, 54, 71, 74, 77, 65};

/// Each Beam picture's height, in pixels.
const u8 gBeam_PictureHeights[] = {7, 9, 11, 13, 13, 11};

/// The strike's six pictures: their byte offsets; the tables after it give
/// their reach, widths and heights.
const u16 gBeam_Pictures[] = {0, 266, 752, 1533, 2495, 3496};

/// How far each strike picture reaches past its middle, in pixels.
const u8 gBeam_PictureReaches[] = {58, 42, 25, 13, 5, 0};

/// The flips a glint may be drawn with, one picked at random.
const u8 gCastingImpact_SquallGlintFlips[] = {0, 4, 8, 12};

/// The flips a glint may be drawn with, one picked at random.
const u8 gCastingImpact_SapGlintFlips[] = {0, 4, 8, 12};

/// Each bite picture's byte offset in the casting impacts' sheet.
const u8 gCastingImpact_BitePictures[] = {0, 1, 2, 3};

/// Each Raiden's Wrath picture's byte offset in the casting impacts' sheet.
const u16 gCastingImpact_RaidensWrathPictures[] = {
    8448, 8712, 11462, 16646, 16646, 24351, 24351, 28201, 28201, 28201};

/// Each Raiden's Wrath picture's width, in pixels.
const u16 gCastingImpact_RaidensWrathWidths[] = {24, 55, 64, 115, 115,
                                                 70, 70, 22, 22,  22};

/// Each Raiden's Wrath picture's height, in pixels.
const u16 gCastingImpact_RaidensWrathHeights[] = {11, 50, 81, 67, 67,
                                                  55, 55, 29, 29, 29};

/// Each Raiden's Wrath's x, in pixels.
const u16 gCastingImpact_RaidensWrathX[] = {58, 19, 0,   6,   6,
                                            50, 50, 100, 100, 100};

/// Each Raiden's Wrath's y, in pixels.
const u16 gCastingImpact_RaidensWrathY[] = {17, 19, 24, 44, 44,
                                            42, 42, 37, 37, 37};

/// Each Apocalypse picture's byte offset in the casting impacts' sheet.
const u16 gCastingImpact_ApocalypsePictures[] = {5700,  7128,  10414, 14089,
                                                 19152, 23148, 26752};

/// Each Apocalypse picture's width, in pixels.
const u16 gCastingImpact_ApocalypseWidths[] = {42, 62, 75, 83, 54, 68, 37};

/// Each Apocalypse picture's height, in pixels.
const u16 gCastingImpact_ApocalypseHeights[] = {34, 53, 49, 61, 74, 53, 28};

/// Each Apocalypse's x, in pixels.
const u16 gCastingImpact_ApocalypseX[] = {85, 65, 50, 19, 0, 0, 58};

/// Each Apocalypse's y, in pixels.
const u16 gCastingImpact_ApocalypseY[] = {62, 66, 78, 41, 19, 0, 5};

/// Each Apocalypse burst picture's byte offset in the casting impacts' sheet.
const u16 gCastingImpact_ApocalypseBurstPictures[] = {27788, 28464, 29724};

/// Each Apocalypse burst picture's width, in pixels.
const u8 gCastingImpact_ApocalypseBurstWidths[] = {26, 35, 52};

/// Each Apocalypse burst picture's height, in pixels.
const u8 gCastingImpact_ApocalypseBurstHeights[] = {26, 36, 48};

/// Each Apocalypse burst's point, x and y.
const u8 gCastingImpact_ApocalypseBurstPoints[][2] = {
    {37, 104}, {19, 81}, {20, 47}};

/// Each ice chip picture's byte offset in the casting impacts' sheet.
const u16 gCastingImpact_IceChipPictures[] = {2308, 2372, 2436, 2500};

/// The flips a glint may be drawn with, one picked at random.
const u8 gCastingImpact_StunBoltGlintFlips[] = {0, 4, 8, 12};

/// The frame each Stun Jip band starts on.
const u8 gCastingImpact_StunJipBandStarts[] = {0, 2, 9, 10};

/// How fast each Stun Jip band grows.
const u8 gCastingImpact_StunJipBandGrowth[] = {5, 8, 10, 8};

/// Each Stun Jip band's height.
const s8 gCastingImpact_StunJipBandHeights[] = {32, 16, 24, 32};

/// The frame each Armor Crush wave starts on.
const u8 gCastingImpact_ArmorCrushWaveStarts[] = {0, 4, 9, 10};

/// How fast each Armor Crush wave grows.
const u8 gCastingImpact_ArmorCrushWaveGrowth[] = {3, 4, 3, 5};

/// How far each Armor Crush wave is stretched.
const u8 gCastingImpact_ArmorCrushWaveStretch[] = {3, 4, 5, 6};

/// The water columns' feet, x and y.
const u8 gNereidCopy_Columns[] = {16, 104, 32, 104, 48, 104, 64, 104, 0, 0};

/// Each falling spark's gravity.
const s32 gNereidCopy_Gravity[] = {toFIXED(0.5), toFIXED(0.25), toFIXED(0.25),
                                   toFIXED(0.125)};

/// The twelve-sprite formation: each sprite's x.
const u8 gHelmSplitter_TwelveX[] = {52, 25,  57, 89, 7,  39,
                                    71, 103, 21, 53, 85, 117};

/// The twelve-sprite formation: each sprite's depth.
const u8 gHelmSplitter_TwelveZ[] = {0,  32, 32, 32, 64, 64,
                                    64, 64, 96, 96, 96, 96};

/// The eight-sprite formation facing right: each sprite's x.
const u8 gHelmSplitter_RightEightX[] = {0, 32, 64, 16, 48, 0, 32, 64};

/// The eight-sprite formation facing right: each sprite's depth.
const u8 gHelmSplitter_RightEightZ[] = {0, 0, 0, 32, 32, 64, 64, 64};

/// Each formation sprite's x.
const u8 gFormationObjects_X[] = {0, 32, 64, 0, 32, 64, 32, 64};

/// Each formation sprite's depth.
const u8 gFormationObjects_Z[] = {10, 0, 0, 42, 32, 32, 64, 64};

/// The small eight-sprite formation: each sprite's x.
const u8 gHelmSplitter_SmallEightX[] = {0, 40, 80, 0, 40, 80, 40, 80};

/// The small eight-sprite formation: each sprite's depth.
const u8 gHelmSplitter_SmallEightZ[] = {13, 0, 0, 53, 40, 40, 80, 80};

/// The wide formation: each sprite's x.
const s8 gFormationObjects_WideX[] = {16,  48, 80, 16, 48, 80,
                                      -16, 16, 48, 80, 42};

/// The wide formation: each sprite's depth.
const u8 gFormationObjects_WideZ[] = {0, 0, 0, 32, 32, 32, 50, 64, 64, 64, 96};

/// Each impact burst picture's width, in pixels.
const u8 gHelmSplitter_ImpactBurstPictureWidths[] = {48, 48, 46, 36,
                                                     36, 32, 26, 6};

/// Each impact burst picture's height, in pixels.
const u8 gHelmSplitter_ImpactBurstPictureHeights[] = {96, 96, 90, 74,
                                                      74, 62, 52, 12};

/// Each impact burst picture's byte offset in Helm Splitter's sheet.
const u16 gHelmSplitter_ImpactBurstPictures[] = {0,     4608,  9216,  13356,
                                                 16020, 18684, 20668, 22020};

/// Each orbit's point, x and y.
const s16 gParticleStreams_OrbitPoints[][3] = {
    {-64, -96, 24}, {-48, -120, 32}, {-32, -80, -48}, {-16, -72, -24},
    {16, -80, 16},  {32, -112, -32}, {48, -96, 56}};

/// Each fireball's point, x and y.
const s16 gParticleStreams_FireballPoints[][2] = {
    {32, 32}, {48, 96}, {80, 80}, {112, 100}, {176, 80}, {196, 64}, {204, 96}};

/// Each flash picture's byte offset in Particle Streams's sheet.
const u16 gParticleStreams_FlashPictures[] = {0,    256,  1280, 3584,
                                              5888, 8192, 10496};

/// Each flash's size, in pixels.
const u16 gParticleStreams_FlashSizes[] = {16, 32, 48, 48, 48, 48, 48};

/// Each of the statue's sprites: its x in the statue.
const u8 gRamses_StatueColumns[] = {12, 44, 28, 60, 0, 32, 64, 0, 32};

/// Each of the statue's sprites: its y in the statue.
const u8 gRamses_StatueRows[] = {0, 0, 32, 32, 64, 64, 64, 32, 32};

/// Each puff picture's byte offset in Ramses's sheet.
const u16 gRamses_PuffPictures[] = {0, 256, 1280, 3584, 5888, 8192, 10496};

/// Each puff's size, in pixels.
const u16 gRamses_PuffSizes[] = {16, 32, 48, 48, 48, 48, 48};

/// The strike columns' x, three for each side.
const u8 gRagnarok_StrikeColumns[] = {32, 24, 0, 96, 104, 128};

/// Each flash picture's byte offset in Ragnarok's sheet.
const u8 gRagnarok_FlashPictures[] = {5, 5, 4, 3, 2, 1};

/// Each flash picture's byte offset in Titan Blade's sheet.
const u8 gTitanBlade_FlashPictures[] = {5, 5, 4, 3, 2, 1};

/// The dust's four gravities.
const s32 gTitanBlade_DustGravity[] = {toFIXED(0.5), toFIXED(0.25),
                                       toFIXED(0.25), toFIXED(0.125)};

/// Each burst picture's byte offset in Judgment's sheet.
const u8 gJudgment_BurstPictures[] = {0, 1, 2,  3,  4,  5,  6,
                                      7, 9, 10, 11, 13, 14, 0};

/// Each shard picture's byte offset in Judgment's sheet.
const u16 gJudgment_ShardPictures[] = {1536, 2125, 2838, 3551, 3950, 4286};

/// Each shard picture's width, in pixels.
const u8 gJudgment_ShardWidths[] = {19, 23, 23, 19, 16, 17};

/// Each shard picture's height, in pixels.
const u8 gJudgment_ShardHeights[] = {31, 31, 31, 21, 21, 24};

/// The row's x offsets.
const u8 gKirin_Columns[] = {8, 40, 72, 104, 8, 40, 72, 104, 0};

/// The row's heights.
const u8 gKirin_Rows[] = {0, 0, 0, 24, 32, 32, 32, 56, 64};

/// The spray's four gravities.
const s32 gNeptune_Gravity[] = {toFIXED(0.5), toFIXED(0.25), toFIXED(0.25),
                                toFIXED(0.125)};

/// Each spark picture's byte offset in Undead Sword's sheet.
const u16 gUndeadSword_SparkPictures[] = {0,    256,  1280, 3584,
                                          5888, 8192, 10496};

/// Each spark's size, in pixels.
const u16 gUndeadSword_SparkSizes[] = {16,   32,   48,   48,  48,   48,  48,
                                       1536, 2312, 2053, 775, 8224, 8224};

/// The funnels, by mode.
const u8 gWindFunnel_Counts[] = {3, 3};

/// The frame each ripple starts on.
const u8 gDiamondBerg_RingStarts[4] = {0, 2, 4, 5};

/// How fast each ripple grows.
const u8 gDiamondBerg_RingGrowth[4] = {6, 7, 6, 8};

/// Each ripple's height.
const s8 gDiamondBerg_RingHeights[4] = {32, 16, -8, -24};

/// Each flash's roll.
const s16 gDiamondBerg_ShardRolls[8] = {8192, 24576, 8192, 24576,
                                        8192, 24576, 8192, 24576};

/// Each flash's pitch.
const s16 gDiamondBerg_ShardPitches[8] = {-2048, -2048, -2048, -2048,
                                          -2048, -2048, -2048, -2048};

/// The frame each ring starts on.
const u8 gGeode_WaveStarts[] = {0, 6, 8, 9};

/// How fast each wave grows.
const u8 gGeode_WaveGrowth[] = {5, 8, 7, 3};

/// Each wave's height.
const s8 gGeode_WaveHeights[] = {32, 32, 32, 32};

/// The spark picture for each stage.
const u16 gSoulShatter_SparkPictures[] = {0,    256,  1280, 3584,
                                          5888, 8192, 10496};

/// Each spark's size, in pixels.
const u16 gSoulShatter_SparkSizes[] = {16, 32, 48, 48, 48, 48, 48};

/// The heights of the cone's rings.
const s8 gSerac_RowHeights[] = {-96, -88, -72, -64, -48, -32, -8, 0};

/// The radii of the cone's rings.
const s8 gSerac_RowRadii[] = {1, 6, 10, 12, 16, 24, 30, 32};

/// Each ripple's delay, in frames.
const u8 gSerac_RingStarts[] = {6, 8, 10, 11};

/// How fast each ripple grows.
const u8 gSerac_RingGrowth[] = {8, 7, 6, 3};

/// Each ripple's height.
const s8 gSerac_RingHeights[] = {16, 32, 56, 72};

/// The frame of the strike, by mode.
const u8 gHeavyBlow_StrikeFrames[] = {38, 40};

/// The chips, by mode.
const u8 gHeavyBlow_ChipCounts[] = {128, 32};

/// A chip's two pixels.
const u8 gHeavyBlow_ChipPixels[] = {58, 58};

/// The columns, by number: their x; the tables after it give their start
/// frames, full heights and leans.
const s16 gCool_ColumnX[] = {92, 72, 32, 48, 88, 56, 80, 88, -8, 132};

/// The frame each column starts on.
const u8 gCool_ColumnStarts[] = {0, 4, 7, 10, 13, 16, 20, 23, 25, 27};

/// Each column's height.
const u8 gCool_ColumnHeights[] = {64, 64, 48, 64, 64, 56, 62, 64, 46, 52};

/// Each column's lean.
const u8 gCool_ColumnLeans[] = {0, 3, 1, 2, 3, 0, 1, 2, 1, 0};

/// The spark picture for each stage.
const u16 gCool_SparkPictures[] = {0, 256, 1280, 3584, 5888, 8192, 10496};

/// Each spark's size, in pixels.
const u16 gCool_SparkSizes[] = {16, 32, 48, 48, 48, 48, 48};

/// The drops' bounce speeds.
const s8 gWyrdCurse_DropBounces[] = {-12, 4, -8, 14};

/// The flips a glint may be drawn with, one picked at random.
const u8 gBoltTrail_GlintFlips[] = {0, 4, 8, 12, 0, 0};

/// The flips a glint may be drawn with, one picked at random.
const u8 gMegaera_GlintFlips[] = {0, 4, 8, 12};

/// How the glints are blended, by variant.
const u8 gMegaera_GlintModes[] = {1, 1, 0, 0};

/// Each pillar's size, in pixels.
const s32 gLightSurge_PillarSizes[4] = {18000, 12000, 30000, 40000};

/// Each pillar's shade.
const s8 gLightSurge_PillarShades[4] = {-2, -8, -4, 0};

/// Each pillar's offset.
const s8 gLightSurge_PillarOffsets[4] = {-24, -8, 24, 48};

/// Each mode's flags for the Djinni's entrance.
const u8 gDjinniCharge_EntranceFlags[] = {2, 10, 0, 0};

/// The debris's gravities.
const s32 gDjinniCharge_DebrisGravity[4] = {-8192, -4096, -2048, -1024};

/// How far along its path the streak's head is at each step, in sixteenths.
const s8 gGaspFamily_StreakSteps[] = {0, 0, 1, 2, 4, 6, 9, 12, 16, 0, 0, 0};

/// The streak's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gGaspFamily_StreakMesh[] = {769, 2,  31,   7967, 7936, 0, 256, 2,
                                      0,   31, 7936, 0,    0,    0, 0,   0,
                                      0,   0,  0,    0,    0,    0, 0,   0};

/// Gasp Family's counts, by variant.
const u8 gGaspFamily_Counts[] = {32, 64, 255};

/// The flash frames.
const u8 gCharon_FlashFrames[] = {16, 60, 108, 0, 0};

/// A band of sixteen corners: an octagon below and one above.
const struct ModelVertex gBattleEffect_OctagonBandCorners[] = {
    {0, -8, 32},  {22, -8, 22},   {31, -8, 0},  {22, -8, -22},
    {0, -8, -31}, {-22, -8, -22}, {-31, -8, 0}, {-22, -8, 22},
    {0, 8, 32},   {22, 8, 22},    {31, 8, 0},   {22, 8, -22},
    {0, 8, -31},  {-22, 8, -22},  {-31, 8, 0},  {-22, 8, 22}};

/// The ripple's 32 corners: a ring of sixteen below and one above.
const struct ModelVertex gBattleEffect_CylinderCorners[] = {
    {0, -8, 31},  {12, -8, 29},   {22, -8, 22},   {29, -8, 12},
    {31, -8, 0},  {29, -8, -12},  {22, -8, -22},  {12, -8, -29},
    {0, -8, -31}, {-13, -8, -29}, {-23, -8, -22}, {-30, -8, -12},
    {-32, -8, 0}, {-30, -8, 13},  {-23, -8, 23},  {-13, -8, 30},
    {0, 8, 31},   {12, 8, 29},    {22, 8, 22},    {29, 8, 12},
    {31, 8, 0},   {29, 8, -12},   {22, 8, -22},   {12, 8, -29},
    {0, 8, -31},  {-13, 8, -29},  {-23, 8, -22},  {-30, 8, -12},
    {-32, 8, 0},  {-30, 8, 13},   {-23, 8, 23},   {-13, 8, 30}};

/// The band's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gCastingImpact_BandMesh[] = {
    2305, 8,  31,  3871, 3840, 0, 256,  8,  0,  31,  3840, 0,
    2562, 9,  63,  3903, 3872, 0, 513,  9,  32, 63,  3872, 0,
    2819, 10, 95,  3935, 3904, 0, 770,  10, 64, 95,  3904, 0,
    3076, 11, 127, 3967, 3936, 0, 1027, 11, 96, 127, 3936, 0,
    3333, 12, 31,  3871, 3840, 0, 1284, 12, 0,  31,  3840, 0,
    3590, 13, 63,  3903, 3872, 0, 1541, 13, 32, 63,  3872, 0,
    3847, 14, 95,  3935, 3904, 0, 1798, 14, 64, 95,  3904, 0,
    2048, 15, 127, 3967, 3936, 0, 7,    15, 96, 127, 3936, 0,
    0,    0,  0,   0,    0,    0};

/// The geyser's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gGeyser_Mesh[] = {
    2305, 8,  31,  16159, 16128, 0, 256,  8,  0,  31,  16128, 0,
    2562, 9,  63,  16191, 16160, 0, 513,  9,  32, 63,  16160, 0,
    2819, 10, 95,  16223, 16192, 0, 770,  10, 64, 95,  16192, 0,
    3076, 11, 127, 16255, 16224, 0, 1027, 11, 96, 127, 16224, 0,
    3333, 12, 31,  16159, 16128, 0, 1284, 12, 0,  31,  16128, 0,
    3590, 13, 63,  16191, 16160, 0, 1541, 13, 32, 63,  16160, 0,
    3847, 14, 95,  16223, 16192, 0, 1798, 14, 64, 95,  16192, 0,
    2048, 15, 127, 16255, 16224, 0, 7,    15, 96, 127, 16224, 0,
    0,    0,  0,   0,     0,     0};

/// The ripple's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gBattleEffect_CylinderMesh[] = {
    4353, 16, 15,  3855, 3840, 0, 256,  16, 0,   15,  3840, 0,
    4610, 17, 31,  3871, 3855, 0, 513,  17, 15,  31,  3855, 0,
    4867, 18, 47,  3887, 3871, 0, 770,  18, 31,  47,  3871, 0,
    5124, 19, 63,  3903, 3887, 0, 1027, 19, 47,  63,  3887, 0,
    5381, 20, 15,  3855, 3840, 0, 1284, 20, 0,   15,  3840, 0,
    5638, 21, 31,  3871, 3855, 0, 1541, 21, 15,  31,  3855, 0,
    5895, 22, 47,  3887, 3871, 0, 1798, 22, 32,  47,  3871, 0,
    6152, 23, 63,  3903, 3887, 0, 2055, 23, 47,  63,  3887, 0,
    6409, 24, 79,  3919, 3904, 0, 2312, 24, 64,  79,  3904, 0,
    6666, 25, 95,  3935, 3919, 0, 2569, 25, 79,  95,  3919, 0,
    6923, 26, 111, 3951, 3935, 0, 2826, 26, 95,  111, 3935, 0,
    7180, 27, 127, 3967, 3951, 0, 3083, 27, 111, 127, 3951, 0,
    7437, 28, 79,  3919, 3904, 0, 3340, 28, 64,  79,  3904, 0,
    7694, 29, 95,  3935, 3919, 0, 3597, 29, 79,  95,  3919, 0,
    7951, 30, 111, 3951, 3935, 0, 3854, 30, 96,  111, 3935, 0,
    4096, 31, 127, 3967, 3951, 0, 15,   31, 111, 127, 3951, 0,
    0,    0,  0,   0,    0,    0};

/// The ring's corners, as signed x, y and z.
const struct ModelVertex gGeode_RingCorners[] = {
    {0, -8, 64},  {44, -8, 44},   {62, -8, 0},  {44, -8, -44},
    {0, -8, -62}, {-44, -8, -44}, {-62, -8, 0}, {-44, -8, 44},
    {0, 0, 32},   {22, 0, 22},    {31, 0, 0},   {22, 0, -22},
    {0, 0, -31},  {-22, 0, -22},  {-31, 0, 0},  {-22, 0, 22}};

/// A ring's corners: eight about the edge and eight within.
const struct ModelVertex gBattleEffect_RingCorners[] = {
    {0, -8, 38},  {26, -8, 26},   {37, -8, 0},  {26, -8, -26},
    {0, -8, -37}, {-26, -8, -26}, {-37, -8, 0}, {-26, -8, 26},
    {0, 0, 32},   {22, 0, 22},    {31, 0, 0},   {22, 0, -22},
    {0, 0, -31},  {-22, 0, -22},  {-31, 0, 0},  {-22, 0, 22}};

/// The ring's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gBattleEffect_RingMesh[] = {
    2305, 8,  31,  16159, 16128, 0, 256,  8,  0,  31,  16128, 0,
    2562, 9,  63,  16191, 16160, 0, 513,  9,  31, 63,  16160, 0,
    2819, 10, 95,  16223, 16192, 0, 770,  10, 63, 95,  16192, 0,
    3076, 11, 127, 16255, 16224, 0, 1027, 11, 95, 127, 16224, 0,
    3333, 12, 31,  16159, 16128, 0, 1284, 12, 0,  31,  16128, 0,
    3590, 13, 63,  16191, 16160, 0, 1541, 13, 31, 63,  16160, 0,
    3847, 14, 95,  16223, 16192, 0, 1798, 14, 63, 95,  16192, 0,
    2048, 15, 127, 16255, 16224, 0, 7,    15, 95, 127, 16224, 0,
    0,    0,  0,   0,     0,     0};

/// The triangle's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gBattleEffect_TriangleMesh[] = {256, 2, 0, 0};

/// A small triangle lying on the ground: its three corners.
const struct ModelVertex gBattleEffect_TriangleCorners[] = {
    {-6, 0, -6}, {6, 0, -6}, {-6, 0, 6}};

/// A beam 127 tall, 12 wide at the top and 4 at its foot: its four corners.
const struct ModelVertex gBattleEffect_TaperedBeamCorners[] = {
    {-6, 127, 0}, {6, 127, 0}, {-2, 0, 0}, {2, 0, 0}};

/// A beam 127 tall and 12 wide standing on the origin: its four corners.
const struct ModelVertex gBattleEffect_BeamCorners[] = {
    {-6, 127, 0}, {6, 127, 0}, {-6, 0, 0}, {6, 0, 0}};

/// A beam 131 tall and 12 wide reaching below the origin: its four corners.
const struct ModelVertex gBattleEffect_LongBeamCorners[] = {
    {6, 127, 0}, {-6, 127, 0}, {6, -4, 0}, {-6, -4, 0}};

/// A 64 by 64 square about the origin: its four corners.
const struct ModelVertex gBattleEffect_SquareCorners[] = {
    {-32, -32, 0}, {32, -32, 0}, {-32, 32, 0}, {32, 32, 0}};

/// A 64 by 64 square standing on the origin: its four corners.
const struct ModelVertex gBattleEffect_StandingSquareCorners[] = {
    {-32, -64, 0}, {32, -64, 0}, {-32, 0, 0}, {32, 0, 0}};

/// The wing's corners, as signed x, y and z.
const struct ModelVertex gLegend_WingCorners[] = {
    {-16, 64, -64}, {16, 64, -64}, {-16, 56, 0}, {16, 56, 0}};

/// A 128 by 128 square lying on the ground about the origin: its four corners.
const struct ModelVertex gBattleEffect_GroundSquareCorners[] = {
    {-64, 0, -64}, {64, 0, -64}, {-64, 0, 64}, {64, 0, 64}};

/// A small square's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gBattleEffect_SmallSquareMesh[] = {
    769, 2, 31, 7967, 7936, 0, 256, 2, 0, 31, 7936, 0, 0, 0, 0, 0, 0, 0};

/// The small square's mesh with its triangles wound the other way.
const u16 gBattleEffect_SmallSquareMeshReversed[] = {
    256, 2, 0, 31, 7936, 0, 769, 2, 31, 7967, 7936, 0, 0, 0, 0, 0, 0, 0};

/// A column's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gBattleEffect_ColumnMesh[] = {
    769, 2, 31, 30495, 30464, 0, 256, 2, 0, 31, 30464, 0, 0, 0, 0, 0, 0, 0};

/// The sword's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gHeavyBlow_SwordMesh[] = {256,   2,     0, 31, 32512, 0, 769, 2, 31,
                                    32543, 32512, 0, 0,  0,     0, 0,   0, 0};

/// A tall quad's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gBattleEffect_TallMesh[] = {
    256, 2, 256, 287, 16128, 0, 769, 2, 287, 16159, 16128, 0, 0, 0, 0, 0, 0, 0,
    256, 2, 0,   15,  16128, 0, 769, 2, 15,  16143, 16128, 0, 0, 0, 0, 0, 0, 0};

/// The band's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gHaures_BandMesh[] = {256,  2,    0, 63, 7936, 0, 769, 2, 63,
                                7999, 7936, 0, 0,  0,    0, 0,   0, 0};

/// The burst's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gCatastrophe_ForkMesh[] = {256,   2,     0, 31, 12032, 0, 769, 2, 31,
                                     12063, 12032, 0, 0,  0,     0, 0,   0, 0};

/// A square's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gBattleEffect_SquareMesh[] = {
    769, 2, 319, 16191, 16129, 0, 256, 2, 257, 319, 16129, 0, 0, 0, 0, 0, 0, 0};

/// A large square's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gBattleEffect_LargeSquareMesh[] = {
    769, 2, 127, 32639, 32512, 0, 256, 2, 0, 127, 32512, 0, 0, 0, 0, 0, 0, 0};

/// The burst's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gHaures_BurstMesh[] = {
    769, 2, 47,   18223, 18176, 0, 256, 2, 0,    47,    18176, 0,
    0,   0, 0,    0,     0,     0, 769, 2, 4143, 12079, 12048, 0,
    256, 2, 4112, 4143,  12048, 0, 0,   0, 0,    0,     0,     0};

/// The swing's pictures: their widths; the tables after it give their heights
/// and offsets, and the fist's heights, y offsets and offsets.
const u8 gHeartbreak_SwingWidths[] = {32, 39, 42, 71};

/// Each swing picture's height, in pixels.
const u8 gHeartbreak_SwingHeights[] = {29, 32, 48, 57};

/// Each swing picture's byte offset in Heartbreak's sheet.
const u16 gHeartbreak_SwingPictures[] = {0, 928, 2176, 4192};

/// Each fist picture's height, in pixels.
const u8 gHeartbreak_FistHeights[] = {29, 37, 48, 47, 52, 32, 22, 17};

/// Each fist's y, in pixels.
const u8 gHeartbreak_FistY[] = {34, 26, 15, 14, 7, 11, 8, 6};

/// Each fist picture's byte offset in Heartbreak's sheet.
const u16 gHeartbreak_FistPictures[] = {0,    928,  2112, 3648,
                                        5152, 6816, 7840, 8544};

/// By level (effect->variant): the sparks, their spread, the rings' size and
/// the embers.
const u8 gRagingHeat_Styles[][4] = {
    {8, 127, 64, 32}, {12, 191, 96, 64}, {16, 255, 128, 255}};

/// Juggle's style, one row a mode.
const u8 gJuggle_Styles[][7] = {{1, 24, 4, 64, 32, 4, 0},
                                {2, 32, 8, 32, 64, 8, 0},
                                {2, 48, 16, 32, 128, 16, 0}};

/// Each petal's size, in pixels.
const u8 gFlora_PetalSizes[] = {4, 8, 16, 24, 0};

/// Each petal picture's byte offset in Flora's sheet.
const u16 gFlora_PetalPictures[] = {0, 32, 160, 672};

/// The bloom frames.
const u16 gFlora_BloomFrames[] = {114, 117, 120, 250};

/// Each bloom's roll.
const s16 gFlora_BloomRolls[] = {0, 24576, -28576, 11008};

/// Each bloom's pitch.
const s16 gFlora_BloomPitches[] = {16384, 4096, 2096, -7168};

/// Each bloom's x, in pixels.
const s8 gFlora_BloomX[] = {16, 16, 16, -16};

/// Each bloom's y, in pixels.
const s8 gFlora_BloomY[] = {0, 0, 0, 12, 0, 0};

/// The dagger's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gSabreDance_DaggerMesh[] = {256,  2,    6, 25, 7942, 0, 769, 2, 25,
                                      7961, 7942, 0, 0,  0,    0, 0,   0, 0};

/// The dagger, 32 by 80 pixels from (-16, -16) to (16, 64), the point down: its
/// four corners.
const struct ModelVertex gSabreDance_DaggerCorners[] = {
    {-16, -16, 0}, {16, -16, 0}, {-16, 64, 0}, {16, 64, 0}};

/// Each Fume level's row, one column a FumeLevelColumn.
const u16 gFume_Levels[][FUME_COLUMNS] = {{16384, 16, 24, 96, 8, 4096},
                                          {24576, 16, 48, 192, 16, 8192},
                                          {40768, 32, 38, 256, 32, 16384}};

/// The early drops' gravities.
const s32 gOceanFist_DropGravity[4] = {-8192, -16384, -16384, -32768};

/// The streak's scale, 16.16, by mode.
const s32 gRushingStrike_Scales[] = {toFIXED(0.5), toFIXED(0.5), toFIXED(0.5),
                                     toFIXED(0.5), 55536,        52768};

/// By mode: the quad's scale (16.16, doubled when used) and a second word.
const s32 gMirroredWall_Scales[] = {39768, 44152, 44152, 55536};

/// Each burst's shade.
const s32 gMirroredWall_Shades[] = {-8, -8, -8, 0};

/// Each shake's offset.
const s8 gDaedalus_ShakeOffsets[] = {0,  -1, 1,  -1, 0,  2,  1,  0,
                                     -1, -1, -2, -2, -3, -3, -4, -4,
                                     -4, -3, -3, -3, -2, -2, -1, -1};

/// Each drift picture's byte offset in Daedalus's sheet.
const u8 gDaedalus_WingBeatPictures[] = {0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1};

/// The frame each drift starts on.
const u8 gDaedalus_BirdStarts[][2] = {
    {112, 128}, {128, 112}, {112, 128}, {128, 112}, {128, 112},
    {112, 112}, {180, 120}, {140, 120}, {140, 120}, {136, 120}};

/// Each drift's speed.
const s32 gDaedalus_BirdSpeeds[][2] = {
    {-120000, -70000}, {-117000, -68000}, {-120000, -70000}, {-117000, -68000},
    {-80000, -80000},  {-80000, -95000},  {-100000, -80000}, {-100000, -90000},
    {-120000, -95000}, {-100000, -95000}};

/// The drift frames.
const u8 gDaedalus_BirdFrames[] = {16, 16, 40, 40, 40, 68, 40, 72, 72, 96};

/// The clatter frames.
const u16 gDaedalus_ClatterFrames[] = {16, 115, 243, 371, 499};

/// The frame each ring starts on.
const u16 gDaedalus_RingStarts[] = {568, 5};

/// Each ring's x, in pixels.
const s8 gDaedalus_RingX[] = {66, 64};

/// Each sprite's point, x and y.
const u8 gDaedalus_RightLegPoints[][2] = {
    {0, 0}, {32, 0}, {0, 32}, {32, 32}, {32, 64}};

/// Each splash's point, x and y.
const u8 gDaedalus_SplashPoints[][6][2] = {
    {{60, 64}, {58, 72}, {56, 80}, {54, 88}, {54, 96}, {56, 104}},
    {{78, 56}, {78, 64}, {80, 72}, {82, 80}, {84, 88}, {84, 96}},
    {{76, 60}, {74, 60}, {72, 60}, {70, 60}, {68, 60}, {66, 60}},
    {{60, 64}, {61, 64}, {62, 63}, {63, 62}, {64, 61}, {66, 60}}};

/// The frame each particle starts on.
const u16 gDaedalus_DiveStarts[] = {0,  24, 8,  52, 16, 40, 36, 28,
                                    32, 4,  20, 56, 60, 12, 44, 48};

/// The seven strike frames.
const u8 gDaedalusStrike_HitFrames[] = {80, 84, 86, 88, 90, 92, 94};

/// By level: which of the seven blades strike.
const u8 gVengeance_ArrowStrikes[][7] = {
    {0, 0, 1, 0, 0, 0, 1}, {1, 0, 1, 1, 0, 0, 1}, {1, 1, 1, 1, 1, 1, 1}};

/// The blade's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gLifeShear_PaleBladeMesh[] = {769, 2, 3711, 13695, 13568, 0,
                                        256, 2, 3584, 3711,  13568, 0,
                                        0,   0, 0,    0,     0,     0};

/// The handle's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gLifeShear_OrangeBladeMesh[] = {
    769, 2, 127, 8063, 7936, 0, 256, 2, 0, 127, 7936, 0, 0, 0, 0, 0, 0, 0};

/// The scythe, 128 by 32 pixels: its four corners.
const struct ModelVertex gLifeShear_BladeCorners[] = {
    {-81, -16, 0}, {47, -16, 0}, {-81, 16, 0}, {47, 16, 0}};

/// By mode: the drops' gravity, and their smallest picture.
const s32 gGeyser_DropGravity[] = {-16384, -32768, -32768};

/// Each drop's smallest size.
const s8 gGeyser_DropMinSizes[] = {1, 4, 5};

/// The fifteen pictures of a glowing oval, from 16 by 32 down to 3 by 4: their
/// widths.
const u8 gBubbleColumn_PictureWidths[] = {16, 15, 14, 13, 12, 11, 10, 9,
                                          8,  7,  6,  5,  4,  4,  3};

/// The glowing oval's fifteen pictures: their heights.
const u8 gBubbleColumn_PictureHeights[] = {32, 30, 28, 26, 24, 22, 20, 18,
                                           16, 14, 12, 10, 8,  6,  4,  0};

/// The glowing oval's fifteen pictures: their byte offsets in the sheet.
const u16 gBubbleColumn_Pictures[] = {0,    512,  962,  1354, 1692,
                                      1980, 2222, 2422, 2584, 2712,
                                      2810, 2882, 2932, 2964, 2988};

/// Each Coatlicue picture's width, in pixels.
const u8 gCoatlicue_PictureWidths[] = {16, 15, 14, 13, 12, 11, 10, 9,
                                       8,  7,  6,  5,  4,  4,  3};

/// Each Coatlicue picture's height, in pixels.
const u8 gCoatlicue_PictureHeights[] = {32, 30, 28, 26, 24, 22, 20, 18,
                                        16, 14, 12, 10, 8,  6,  4};

/// Each Coatlicue picture's byte offset in Coatlicue's sheet.
const u16 gCoatlicue_Pictures[] = {0,    512,  962,  1354, 1692,
                                   1980, 2222, 2422, 2584, 2712,
                                   2810, 2882, 2932, 2964, 2988};

/// The burst frames.
const u16 gCoatlicue_BurstFrames[] = {0, 0, 10, 78, 82, 112, 250, 254, 284};

/// Each burst's x, in pixels.
const u8 gCoatlicue_BurstX[] = {32, 64, 80, 32, 16, 88, 32, 16, 88};

/// Each burst's y, in pixels.
const u8 gCoatlicue_BurstY[] = {80, 96, 120, 120, 120, 120, 120, 120, 120};

/// Each burst's spread in x.
const u8 gCoatlicue_BurstSpreadX[] = {32, 8, 32, 8, 16, 16, 8, 16, 16};

/// Each burst's spacing in y.
const u8 gCoatlicue_BurstSpacingY[] = {16, 8, 16, 8, 12, 12, 8, 12, 12};

/// Each burst's speed in x.
const s8 gCoatlicue_BurstVelocityX[] = {-24, 4, 20, 0, -2, 2, 0, -2, 2};

/// Each burst's speed in y.
const s8 gCoatlicue_BurstVelocityY[] = {-64, -32, -64, -16, -32,
                                        -32, -16, -32, -32};

/// Each burst's depth, in pixels.
const s8 gCoatlicue_BurstZ[] = {1, 0, 4, -2, 1, 0, -2, 1, 0};

/// Each burst's speed in depth.
const s8 gCoatlicue_BurstVelocityZ[] = {16, 1, 12, 0, 0, 0, 0, 0, 0};

/// The smallest bubble picture for each kind of bubble.
const u8 gCoatlicue_PictureBias[] = {3, 5, 7, 9};

/// The ring frames.
const u16 gCoatlicue_RingFrames[] = {81, 209, 518, 540};

/// The flash frames.
const u16 gCoatlicue_FlashFrames[] = {498, 503, 508};

/// The glow frames.
const u16 gCoatlicue_GlowFrames[] = {518};

/// The frame each coil starts on.
const u8 gCoatlicue_CoilStarts[] = {0, 82};

/// Each ring's first scale.
const u16 gCoatlicue_CoilScales[] = {4096, 18384};

/// Each coil's roll.
const u16 gCoatlicue_CoilRolls[] = {8192, 40960};

/// How fast each coil grows.
const u16 gCoatlicue_CoilGrowth[] = {256, 572};

/// Each ring's largest scale.
const u16 gCoatlicue_CoilMaxScales[] = {44160, 52720};

/// The flash frames.
const u16 gCatastrophe_FlashFrames[] = {4,   26,  30,  34,  104, 112,
                                        116, 120, 150, 154, 202, 214,
                                        244, 278, 400, 402, 404, 420};

/// How long each flash of the first act lasts, in frames.
const u16 gCatastrophe_FlashLengths[] = {8,  2, 2, 16, 2,  2, 2, 10, 2,
                                         16, 4, 8, 10, 12, 2, 2, 10, 4};

/// Each of the demon's sprites: its x.
const u8 gCatastrophe_DemonX[] = {0,   32,  64, 96, 32,  64,  96, 128, 64, 96,
                                  128, 160, 32, 64, 96,  128, 25, 57,  89, 121,
                                  153, 32,  64, 96, 128, 160, 48, 96};

/// Each of the demon's sprites: its depth.
const u8 gCatastrophe_DemonZ[] = {
    0,  0,  0,   0,   32,  32,  32,  32,  64,  64,  64,  64,  96,  96,
    96, 96, 128, 128, 128, 128, 128, 160, 160, 160, 160, 160, 192, 192};

/// Each of the armour's sprites: its x.
const u8 gCatastrophe_ArmourX[] = {16, 0, 32, 0, 32, 16, 16, 16, 16, 16, 16, 0};

/// Each of the armour's sprites: its depth.
const u16 gCatastrophe_ArmourZ[] = {0,   32,  32,  64,  64, 96,
                                    128, 160, 192, 224, 256};

/// Each of the mirrored demon's sprites: its x.
const u8 gCatastrophe_MirroredDemonX[] = {224, 192, 160, 128, 192, 160};

/// Each of the mirrored demon's sprites: its depth.
const u8 gCatastrophe_MirroredDemonZ[] = {0, 0, 0, 0, 32, 32};

/// The frame each burst starts on.
const u16 gCatastrophe_ForkStarts[] = {26, 104, 116, 216, 278, 402, 416};

/// Each burst's x, in pixels.
const u8 gCatastrophe_ForkX[] = {16, 96, 20, 40, 50, 10, 96};

/// Each burst's y, in pixels.
const u8 gCatastrophe_ForkY[] = {64, 80, 30, 40, 80, 60, 20};

/// Each burst picture's byte offset in Catastrophe's sheet.
const u8 gCatastrophe_ForkPictures[] = {0, 1, 0, 1, 0, 0, 1, 0};

/// The canvas flash frames.
const u16 gCatastrophe_CanvasFlashFrames[] = {246, 249, 254, 258, 260, 264,
                                              266, 269, 272, 275, 278, 280,
                                              282, 284, 287, 290, 292};

/// Each strike's x, in pixels.
const u8 gCatastrophe_StrikeX[] = {
    71, 71, 71, 70, 70, 70, 70, 70, 70, 69, 69, 0,  67, 67, 67, 70, 67, 68,
    70, 69, 70, 69, 67, 0,  69, 69, 69, 68, 67, 68, 70, 69, 69, 69, 0,  0};

/// Each strike's y, in pixels.
const u8 gCatastrophe_StrikeY[] = {92,  92,  92,  120, 120, 120, 154, 154, 154,
                                   220, 200, 0,   100, 100, 100, 120, 120, 154,
                                   154, 154, 154, 220, 240, 0,   120, 100, 100,
                                   140, 120, 154, 154, 154, 184, 220, 0,   0};

/// The strike frames.
const u16 gCatastrophe_StrikeFrames[] = {
    492, 506, 526, 528, 542, 562, 572, 578, 598, 600, 608, 65535, 0,
    504, 518, 526, 546, 548, 554, 562, 564, 570, 590, 594, 596,   65535,
    510, 524, 544, 546, 548, 558, 560, 580, 582, 596, 616, 65535, 0};

/// Each falling shard's speed in y.
const u8 gCatastrophe_StrikeSpeedsY[] = {
    6,  4,  0, 8,  8, 0, 10, 20, 0, 20, 0, 0,  6, 4, 0,  8,  8, 0,
    10, 20, 8, 20, 0, 0, 6,  4,  0, 8,  8, 20, 0, 0, 20, 20, 0, 0};

/// Each strike's roll.
const s16 gCatastrophe_StrikeRolls[] = {
    144, 0,  0, 128, 0,   0, 180, 0, 0, 150, 0, 0,   112, 0, 0,   0, 112, 48,
    0,   86, 0, 0,   112, 0, 96,  0, 0, 96,  0, 112, 128, 0, 104, 0, 0,   0};

/// How fast each of the falling shards turns.
const s16 gCatastrophe_StrikeTurns[] = {
    256,  64,   0,    712, 500, 0,   0,   0, 0,    356,  0, 0,
    -356, -124, -256, 712, -64, 256, 0,   0, -228, 356,  0, 0,
    406,  64,   -256, 0,   -64, 0,   128, 0, 328,  -356, 0, 0};

/// Each strike picture's byte offset in Catastrophe's sheet.
const s16 gCatastrophe_StrikePictures[] = {
    0,  1, -1, 0,  1, -1, 0, 1, -1, 0, -1, 0, 2, 1,  0, -1, 2,  2,
    -1, 0, 1,  -1, 2, -1, 0, 1, -1, 0, -1, 0, 1, -1, 0, 1,  -1, 0};

/// The flips a glint may be drawn with, one picked at random.
const u8 gCatastrophe_GlintFlips[] = {0, 4, 8, 12};

/// Each shard's roll.
const s16 gCatastrophe_BoltRolls[] = {
    14797, -1097, -8198, 4962,  15494,  -5489, -9298,  3744,
    11252, -9996, 15443, -4373, -15930, 15898, -13623, 3906};

/// Where each of Haures's eleven pieces is drawn, x and y, on a 32-pixel grid.
const u16 gHaures_BodyPiecePoints[][2] = {
    {0, 0},   {0, 32},  {32, 32}, {64, 32}, {96, 42}, {0, 64},
    {32, 64}, {64, 64}, {96, 74}, {32, 96}, {64, 96}};

/// Each particle's x, in pixels.
const u8 gHaures_ParticleX[] = {120, 88, 144, 112, 0, 0};

/// Haures's sprites' scale, one to one.
const s32 gHaures_UnitScale[] = {toFIXED(1), toFIXED(1)};

/// The wings' shapes, 32 heights a shape; the wings morph from one to the next.
const u8 gHaures_WingShapes[] = {
    0,   0,   0,   2,   6,   9,   10,  11,  13,  14,  15,  16,  17,  18,  18,
    16,  16,  16,  17,  16,  15,  14,  13,  12,  11,  9,   8,   5,   0,   0,
    0,   0,   0,   2,   8,   12,  13,  16,  18,  21,  24,  27,  31,  34,  36,
    34,  32,  28,  18,  18,  21,  29,  35,  40,  40,  38,  37,  32,  25,  15,
    10,  8,   4,   0,   0,   8,   11,  14,  17,  19,  22,  24,  28,  31,  35,
    45,  58,  54,  49,  46,  47,  56,  103, 120, 118, 98,  52,  45,  49,  49,
    38,  32,  26,  18,  8,   0,   0,   8,   13,  16,  20,  22,  26,  29,  31,
    36,  42,  47,  52,  59,  65,  69,  70,  69,  66,  63,  59,  56,  53,  49,
    46,  42,  37,  32,  27,  18,  7,   0,   0,   8,   13,  18,  22,  26,  40,
    54,  59,  57,  39,  31,  32,  37,  48,  66,  73,  61,  49,  54,  48,  45,
    45,  31,  32,  44,  41,  32,  24,  20,  14,  0,   0,   8,   11,  16,  22,
    27,  55,  90,  87,  76,  66,  55,  45,  45,  53,  58,  58,  50,  31,  37,
    42,  48,  57,  56,  37,  29,  31,  31,  32,  31,  27,  0,   0,   10,  17,
    25,  32,  42,  46,  48,  51,  55,  60,  66,  72,  77,  80,  84,  85,  83,
    76,  63,  53,  49,  45,  43,  39,  35,  32,  27,  22,  18,  10,  0,   0,
    10,  16,  21,  26,  29,  36,  46,  58,  69,  77,  78,  74,  67,  59,  52,
    52,  59,  65,  69,  71,  72,  71,  70,  59,  66,  62,  44,  26,  20,  13,
    0,   0,   9,   15,  21,  27,  29,  34,  43,  47,  51,  53,  55,  58,  62,
    67,  69,  72,  72,  71,  67,  63,  60,  56,  53,  51,  45,  38,  26,  17,
    10,  8,   0,   0,   8,   12,  20,  27,  52,  73,  74,  68,  60,  53,  46,
    41,  40,  48,  61,  72,  77,  78,  82,  88,  93,  97,  32,  35,  40,  44,
    40,  28,  15,  8,   0,   0,   8,   16,  34,  44,  50,  56,  60,  65,  69,
    73,  77,  79,  82,  83,  77,  72,  68,  63,  59,  54,  40,  26,  23,  26,
    29,  31,  32,  26,  18,  8,   0,   0,   8,   11,  18,  30,  56,  77,  86,
    92,  93,  92,  90,  88,  85,  82,  79,  76,  72,  70,  66,  62,  60,  56,
    51,  47,  42,  36,  24,  15,  10,  8,   0,   0,   8,   83,  96,  104, 110,
    115, 118, 120, 116, 114, 114, 115, 116, 119, 121, 120, 117, 114, 111, 108,
    106, 102, 100, 97,  87,  30,  22,  15,  10,  8,   0};

/// How many frames each morph of the wings takes.
const u8 gHaures_WingMorphSteps[] = {27, 15, 13, 5,  14, 16, 11,
                                     13, 8,  18, 24, 26, 100};

/// Each part's point, x and y.
const u8 gHaures_PartPoints[] = {
    23,  13,  0,   0,   42,  0,   28,  41,  28,  41,  48,  28,  42,  0,   78,
    31,  28,  41,  28,  41,  66,  7,   42,  0,   79,  0,   78,  31,  78,  31,
    94,  8,   79,  0,   128, 0,   78,  31,  78,  31,  112, 28,  128, 0,   132,
    48,  78,  31,  78,  31,  162, 12,  182, 0,   174, 42,  128, 0,   128, 0,
    145, 39,  128, 0,   174, 42,  134, 71,  134, 71,  196, 12,  182, 0,   239,
    0,   174, 42,  172, 42,  231, 20,  239, 0,   239, 52,  211, 16,  211, 16,
    7,   33,  0,   0,   28,  41,  0,   48,  0,   48,  59,  49,  78,  31,  69,
    73,  28,  41,  28,  41,  91,  51,  78,  31,  132, 48,  69,  73,  69,  73,
    16,  58,  28,  41,  20,  86,  0,   48,  0,   48,  36,  70,  28,  41,  69,
    73,  20,  86,  20,  86,  96,  76,  132, 48,  99,  97,  69,  73,  69,  73,
    6,   80,  0,   48,  20,  86,  0,   98,  0,   98,  168, 66,  174, 42,  205,
    88,  134, 71,  134, 71,  194, 48,  211, 16,  205, 88,  174, 42,  174, 42,
    217, 52,  211, 16,  239, 52,  205, 88,  208, 88,  7,   114, 0,   98,  30,
    119, 0,   119, 0,   119, 29,  106, 20,  86,  64,  119, 30,  119, 0,   98,
    51,  93,  69,  73,  64,  119, 20,  86,  20,  86,  78,  96,  69,  73,  99,
    97,  64,  119, 64,  119, 98,  112, 99,  97,  137, 104, 112, 119, 64,  119,
    123, 85,  132, 48,  137, 104, 99,  97,  99,  97,  156, 88,  134, 71,  205,
    88,  137, 104, 137, 104, 139, 115, 137, 104, 175, 119, 112, 119, 112, 119,
    217, 52,  211, 16,  239, 52,  205, 88,  205, 88,  228, 87,  239, 52,  239,
    119, 205, 88,  205, 88,  200, 108, 185, 93,  205, 88,  239, 119, 175, 119,
    169, 106, 185, 93,  175, 119, 137, 104, 137, 104};

/// The frame each band starts on.
const u8 gHaures_BandStarts[] = {10, 30};

/// Each Helm Splitter picture's width, in pixels.
const u8 gHelmSplitter_PictureWidths[] = {26, 46, 63, 77, 60, 39, 32};

/// Each Helm Splitter picture's height, in pixels.
const u8 gHelmSplitter_PictureHeights[] = {11, 20, 37, 65, 65, 54, 39, 0};

/// Each Helm Splitter picture's byte offset in Helm Splitter's sheet.
const u16 gHelmSplitter_Pictures[] = {0, 286, 1206, 3537, 8542, 12442, 14548};

/// How far right of its anchor each picture is drawn, mirrored on the other
/// side.
const s8 gHelmSplitter_PictureBiasX[] = {53, 36, 16, 2, 2, 2, 2};

/// How far below its anchor each picture's top goes.
const s8 gHelmSplitter_PictureBiasY[] = {2, 3, 3, 3, 4, 14, 29};

/// How many of the 56 embers are sparks, by variant; the rest are flashes.
const u8 gDragonCloud_SparkCounts[] = {48, 40, 0, 0};

/// Each card's length, in frames.
const u8 gElementCards_Frames[] = {124, 140, 132, 130};

/// How each card's sparks move.
const struct SparkMotion gElementCards_SparkMotions[] = {
    {toFIXED(0.125), 62, 1},
    {toFIXED(0.125), 62, 1},
    {-toFIXED(0.125), 62, 1},
    {2048, 64, 4}};

/// The three bites' frames.
const u8 gTripleChomp_BiteFrames[] = {0, 12, 24};

/// Each breath's length, the frame its burst starts, and the glow's widest size
/// by variant.
const u8 gBreath_Frames[] = {90, 38, 32, 48};

/// The burst frames.
const u8 gBreath_BurstFrames[] = {56, 0, 0, 0};

/// The glow's widest size, by variant.
const u8 gBreath_MaxSizes[] = {24, 32, 32, 24};

/// The four jaws of variant 2, by breath: their start frames; the tables after
/// it give their growth and their x from the mouth.
const u8 gBreath_RingStarts[] = {56, 58, 60, 61, 8, 12, 16, 20, 0, 2, 4, 5};

/// How fast each ring grows.
const u8 gBreath_RingGrowth[] = {5, 6, 7, 6, 10, 9, 11, 10, 5, 6, 7, 6};

/// Each ring's x, in pixels.
const s8 gBreath_RingX[] = {-32, -48, -56, -72, -16, -32,
                            -64, -72, -32, -48, -56, -72};

/// The frame each breath hits the targets.
const u8 gBreath_FlashFrames[] = {56, 14, 6, 56};

/// The frame each ring starts on.
const u8 gTrident_RingStarts[] = {8, 10, 12, 13, 64, 67, 70, 72};

/// How fast each ring grows.
const u8 gTrident_RingGrowth[] = {8, 7, 6, 7, 8, 8, 9, 8};

/// Each ring's height.
const s8 gTrident_RingHeights[] = {16, 0, -24, -40, 16, 0, -24, -40, 0};

/// Each trident's yaw and turn.
const s32 gTrident_Spins[] = {5528,  488, 62272, 308, 52392, -698, 34584, 634,
                              19692, 736, 28062, 746, 48958, -694, 1508,  -576};

/// The prong's corners, as signed x, y and z.
const struct ModelVertex gTrident_ProngCorners[] = {
    {-6, 127, 0},  {6, 127, 0}, {-6, -4, 0},  {6, -4, 0},
    {-6, 127, 0},  {6, 127, 0}, {-6, 0, 0},   {6, 0, 0},
    {-10, 127, 0}, {2, 127, 0}, {-10, -1, 0}, {2, -1, 0}};

/// The flips a glint may be drawn with, one picked at random.
const u8 gTrident_GlintFlips[] = {0, 4, 8, 12};

/// The frame each wave starts on.
const u8 gLeapingStrike_WaveStarts[] = {0, 4, 8, 9};

/// How fast each wave grows.
const u8 gLeapingStrike_WaveGrowth[] = {9, 8, 9, 3};

/// Each wave's height.
const s8 gLeapingStrike_WaveHeights[] = {32, 32, 32, 32};

/// The frame each ring starts on.
const u8 gLeapingStrike_RingStarts[] = {0, 4, 12, 19};

/// How fast each ring grows.
const u8 gLeapingStrike_RingGrowth[] = {7, 8, 7, 8};

/// Each ring's height.
const s8 gLeapingStrike_RingHeights[] = {32, 16, 16, 24};

/// Each Meteor Blow picture's width, in pixels.
const u8 gMeteorBlow_PictureWidths[] = {30, 16, 16, 16, 10, 8, 8};

/// Each Meteor Blow picture's height, in pixels.
const u8 gMeteorBlow_PictureHeights[] = {30, 16, 16, 16, 16, 8, 8};

/// Each Meteor Blow picture's byte offset in Meteor Blow's sheet.
const u16 gMeteorBlow_Pictures[] = {0, 900, 1156, 1412, 1668, 1828, 1892};

/// The frame each wave starts on.
const u8 gMeteorBlow_WaveStarts[] = {0, 4, 9, 10};

/// How fast each wave grows.
const u8 gMeteorBlow_WaveGrowth[] = {3, 4, 3, 5};

/// How far each wave is stretched.
const u8 gMeteorBlow_WaveStretch[] = {3, 4, 5, 6};

/// The frame each ring starts on.
const u8 gMeteorBlow_RingStarts[] = {0, 3, 16, 7};

/// How fast each ring grows.
const u8 gMeteorBlow_RingGrowth[] = {5, 8, 10, 8};

/// Each ring's height.
const s8 gMeteorBlow_RingHeights[] = {-40, 24, 24, 24};

/// The splash's corners, as signed x, y and z.
const struct ModelVertex gLiquifier_SplashCorners[] = {
    {-48, -64, 0}, {48, -64, 0}, {-32, 4, 0}, {32, 4, 0}};

/// The frame each splash starts on.
const u8 gLiquifier_SplashStarts[] = {18, 21, 16, 17, 22, 19, 23, 18};

/// Each splash's height.
const u8 gLiquifier_SplashHeights[] = {40, 56, 70, 48, 64, 80, 58, 64, 72};

/// Each flame picture's byte offset in Canvas Burst's sheet.
const u8 gCanvasBurst_FlamePictures[] = {5, 5, 4, 3, 2, 1, 0};

/// Each flash's pitch.
const s16 gMegiddo_FlashPitches[] = {0, -10240, 0, -10240, 10240};

/// Each flash's yaw.
const s16 gMegiddo_FlashYaws[] = {0, -4864, 0, -4864, -4864};

/// The frames the five rings start on.
const u16 gMegiddo_FlashStarts[] = {4, 8, 171, 175, 176};

/// Each ray's pitch.
const s16 gMegiddo_RayPitches[] = {-12544, -10496, -8704, 8704,
                                   -8448,  12800,  10752, 11264};

/// Each ray's yaw.
const s16 gMegiddo_RayYaws[] = {0,     11776,  0,      -14336,
                                11264, -19968, -22016, -22272};

/// Each ray's roll.
const s16 gMegiddo_RayRolls[] = {-21504, 13312, 22224, -18944,
                                 -32768, 8448,  25600, -28928};

/// Each ray's x, in pixels.
const s8 gMegiddo_RayX[] = {6, 9, 6, 2, 6, 3, 6, 7};

/// Each ray's y, in pixels.
const s8 gMegiddo_RayY[] = {-6, -4, -7, -9, -6, -7, -9, -7};

/// Each flash's pitch.
const s16 gOdyssey_FlashPitches[] = {0, -10240};

/// Each flash's yaw.
const s16 gOdyssey_FlashYaws[] = {0, -4864, 0};

/// The frame each ring starts on.
const u8 gMoloch_RingStarts[] = {0, 6, 8, 9};

/// How fast each ring grows.
const u8 gMoloch_RingGrowth[] = {7, 8, 7, 8};

/// Each ring's height.
const s8 gMoloch_RingHeights[] = {24, 12, -24, 0};

/// Each splash's radius.
const s32 gIris_SplashRadii[] = {24, 16};

/// Each splash's offset in y.
const u8 gIris_SplashOffsetsY[] = {7, 5, 0, 0};

/// The pillar's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gIris_PillarMesh[] = {
    256,  2,    0, 15,  3840, 0,    258,  3,    3840, 15, 3855, 0, 770, 4, 3584,
    3599, 3840, 0, 772, 5,    3840, 3599, 3855, 0,    0,  0,    0, 0,   0, 0};

/// The particle's corners, as signed x, y and z.
const struct ModelVertex gIris_ParticleCorners[] = {
    {-4, -8, 0}, {4, -8, 0}, {-4, 8, 0}, {4, 8, 0}};

/// The particle's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gIris_ParticleMesh[] = {256, 2,     0, 31, 16128, 0, 258, 3, 16128,
                                  31,  16159, 0, 0,  0,     0, 0,   0, 0};

/// The psynergy sound frames.
const u8 gIris_PsynergySoundFrames[] = {84,  93,  108, 111, 116, 132,
                                        135, 141, 153, 158, 171, 0};

/// The door sound frames.
const u16 gIris_DoorSoundFrames[] = {231, 244, 259, 265, 270};

/// The power up sound frames.
const u16 gIris_PowerUpSoundFrames[] = {279, 287, 293, 298, 302, 307};

/// Each streak's x, in pixels.
const s8 gIris_StreakX[] = {-10, 80, 110, -10, 76, 50, 110, 80,
                            40,  30, 80,  20,  50, 80, 20};

/// Each streak's y, in pixels.
const u8 gIris_StreakY[] = {116, 136, 128, 112, 136, 136, 128, 136,
                            136, 136, 136, 136, 136, 136, 136};

/// Each streak's starting angle.
const u16 gIris_StreakAngles[] = {18384, 62488, 51152, 16384, 62488,
                                  3048,  51152, 62488, 4048,  6048,
                                  60488, 7048,  3048,  62488, 6048};

/// Each streak's speed.
const s16 gIris_StreakSpeeds[] = {-406, 64,  380, -326, 64,  -64, 380, 64,
                                  -84,  -64, 94,  -94,  -64, 64,  -64};

/// Each line's shade.
const u8 gIris_LineShades[] = {23, 47, 56, 63, 63, 56, 47, 23};

/// Each mote's size, in pixels.
const s32 gIris_MoteSizes[] = {7, 5};

/// The frame each splash starts on.
const u16 gIris_SplashStarts[] = {230, 238, 260, 265, 253, 258};

/// The frame each drop stops falling and splashes.
const u16 gIris_SplashEnds[] = {320, 328, 330, 335, 326, 332};

/// Each splash's x, in pixels.
const u8 gIris_SplashX[] = {34, 86, 18, 102, 8, 112};

/// Each splash's y, in pixels.
const u8 gIris_SplashY[] = {64, 62, 32, 32, 80, 80};

/// Whether each splash is a small one.
const u8 gIris_SplashSmall[] = {0, 0, 1, 1, 0, 0};

/// Each drop's speed.
const u8 gIris_DropSpeeds[] = {8, 4, 1, 2, 4, 8};

/// The drop frames.
const u8 gIris_DropFrames[] = {16, 16, 16, 16, 32, 32};

/// How many frames each drop shows each picture.
const u8 gIris_DropPictureSteps[] = {1, 1, 1, 1, 2, 2};

/// The left strip's corners, as signed x, y and z.
const struct ModelVertex gIris_LeftStripCorners[] = {
    {-6, -64, 0}, {18, -64, 0}, {-9, -32, 0}, {15, -32, 0}, {-13, 0, 0},
    {8, 0, 0},    {-19, 32, 0}, {-1, 32, 0},  {-28, 64, 0}, {-12, 64, 0}};

/// The right strip's corners, as signed x, y and z.
const struct ModelVertex gIris_RightStripCorners[] = {
    {35, -64, 0}, {72, -64, 0}, {28, -32, 0}, {66, -32, 0}, {18, 0, 0},
    {51, 0, 0},   {6, 32, 0},   {33, 32, 0},  {-12, 64, 0}, {9, 64, 0}};

/// The strip's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gIris_StripMesh[] = {
    256,   2,     0,     31,   3840, 0,     258,   3,     3840, 31,   3871,
    0,     770,   4,     3840, 3871, 7936,  0,     772,   5,    7936, 3871,
    7967,  0,     1284,  6,    7936, 7967,  12032, 0,     1286, 7,    12032,
    7967,  12063, 0,     1798, 8,    12032, 12063, 16128, 0,    1800, 9,
    16128, 12063, 16159, 0,    0,    0,     0,     0,     0,    0};

/// The cone's corners, as signed x, y and z.
const struct ModelVertex gIris_ConeCorners[] = {
    {0, -40, 0}, {-5, -4, 0},  {5, -4, 0},  {-10, 16, 0},
    {10, 16, 0}, {-20, 32, 0}, {20, 32, 0}, {0, 0, 0}};

/// The cone's mesh: each triangle's three corners and their texture
/// coordinates.
const u16 gIris_ConeMesh[] = {1,    7,  7936, 0,  7951, 0,  513,  3,  0,    31,
                              7936, 0,  515,  4,  7936, 31, 7967, 0,  1027, 5,
                              0,    31, 7936, 0,  1029, 6,  7936, 31, 7967, 0,
                              7,    2,  7951, 31, 7967, 0};
