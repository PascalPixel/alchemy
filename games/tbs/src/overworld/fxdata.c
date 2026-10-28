/// @file
/// Effect tables: actor scripts, curves and lookup tables of the overworld
/// bank.

#include "speaker.h"
#include "module/overworld/overworld.h"
#include "module/overworld/event_commands.h"
#include "actor_script.h"
#include "types.h"
#include "scene_tables.h"

// Their readers declare them in battle.h, field.h, graphics.h, scroll.h and
// field_effect_scripts.h, and overworld.c declares the colour curves. Some
// readers declare their table without const: battle.h's
// gFieldEffect_TargetRangeByMode, gMindRead_CasterPaletteCycle,
// gSpeakerDefinitions, gFieldEffect_GroundParticleFrames,
// gFieldEffect_PulseScales, gFieldEffect_CyclePatternWords and
// gFieldEffect_RandomChildValues, graphics.h's gScreenTransition_NoiseWidths
// and gScreenTransition_EdgeOffsets, field.h's gActor_BlinkSpriteColors
// and overworld.c's three gColorBuffer_ChannelCurve tables. All are defined
// const all the same: in rom_main_04 .data follows scripts.o, and these tables
// would move. So this file includes none of those headers.

/// The message of each searchable cell behavior from 242 to 247, as an
/// offset from MsgTileChecked and MsgTileResult.
const u8 gField_ExamineTriggerMessageOffsets[6] = {
    2,  // a jar
    3,  // a barrel
    13, // a wooden box
    8,  // a grave
    30, // books
    31, // a well
};

/// {field Psynergy kind, range in pixels}, ended by -1; a kind not listed
/// reaches 16 pixels. Field_GetPsynergyTargetRange reads it for the search
/// for a target: 16 pixels for Ply, Frost, Lift, Carry and Growth, 32 for
/// Mind Read, Move, Whirlwind, Douse and Halt, 48 for Force and Catch.
// clang-format off
const s16 gFieldEffect_TargetRangeByMode[25] = {
    3, 16,
    6, 16,
    7, 16,
    11, 16,
    12, 16,
    2, 32,
    1, 32,
    14, 32,
    5, 32,
    9, 32,
    4, 48,
    13, 48,
    -1,
};

/// The palette cycle of Mind Read's caster (Actor_SetSpritePalette,
/// (gFrameCount >> 1) & 7): palettes 1 to 4 and back to 0, two frames each.
const u8 gMindRead_CasterPaletteCycle[8] = {1, 2, 3, 4, 3, 2, 1, 0};

/// Grows from half size to full size, a step every 2 frames.
const s32 gFieldEffect_ParticleEmitterScript[] = {
    SC_SET(SW_SCALE_X, 0x8000),
    SC_SET(SW_SCALE_Y, 0x8000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0xc000),
    SC_SET(SW_SCALE_Y, 0xc000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0xe000),
    SC_SET(SW_SCALE_Y, 0xe000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0xf000),
    SC_SET(SW_SCALE_Y, 0xf000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_END,
};

/// Pulses twice, to 1.5 times full size (0x18000), then rests, over and
/// over: the found item's picture held over the leader (overworld/field_fx.c).
const s32 gFieldEffect_RisingPulseScript[] = {
    SC_WAIT(10),
    SC_SET(SW_SCALE_X, 0x16000),
    SC_SET(SW_SCALE_Y, 0x16000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x18000),
    SC_SET(SW_SCALE_Y, 0x18000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x12000),
    SC_SET(SW_SCALE_Y, 0x12000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x16000),
    SC_SET(SW_SCALE_Y, 0x16000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x18000),
    SC_SET(SW_SCALE_Y, 0x18000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x12000),
    SC_SET(SW_SCALE_Y, 0x12000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_WAIT(2),
    SC_WAIT(50),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/// Sinks a quarter pixel a frame (y, the height, less 0x4000) for 60 frames,
/// then disappears.
const s32 gFieldEffect_ParticleScript[] = {
    SC_ADD(SW_Y, -0x4000),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_RELEASE,
};

/// A Reveal marker: stays while Field_IsRevealActive returns nonzero (SC_CALL
/// runs it again each frame), and releases itself once Reveal ends.
const s32 gFieldEffect_MarkerParticleScript[] = {
    SC_CALL(Field_IsRevealActive),
    SC_RELEASE,
};

/// The width of the bars of WINDOW_TRANSITION_BARS, by progress.
const u8 gScreenTransition_NoiseWidths[34] = {
    0, 1, 2, 2, 2, 2, 3, 3, 4, 4, 5, 6, 7, 8, 9, 11, 13,
    16, 18, 22, 26, 30, 36, 43, 51, 60, 72, 85, 101, 120, 143, 170, 202, 240,
};

/// The wobble of the circle edge of WINDOW_TRANSITION_RIPPLE.
const s8 gScreenTransition_EdgeOffsets[32] = {
    -4, -3, -3, -3, -2, -2, -1, 0, 0, 0, 1, 2, 2, 3, 3, 3,
    4, 3, 3, 3, 2, 2, 1, 0, 0, 0, -1, -2, -2, -3, -3, -3,
};

/// 0 to 63 shuffled: the order in which the dissolve sets the pixels.
const u8 gScreenTransition_DitherTable[64] = {
    0, 12, 35, 3, 29, 39, 6, 7, 50, 59, 4, 11, 18, 40, 46, 31,
    16, 42, 47, 51, 20, 23, 43, 32, 8, 52, 17, 41, 54, 15, 63, 24,
    58, 5, 2, 26, 36, 61, 9, 34, 56, 27, 10, 48, 53, 30, 28, 22,
    62, 45, 19, 60, 57, 49, 25, 55, 38, 14, 44, 13, 33, 37, 1, 21,
};

/// Colour channel curves of the palette tints (overworld/color.c): each
/// maps a 5-bit channel (0 to 31) to a palette fade channel, shifted up 10
/// bits, so 31 is 0x7c00. All three darken the middle tones: 16 becomes
/// about 11.4 here (0x2dc3), 12.4 in the Light curve, 12.5 in the Contrast
/// curve, which also reaches 31 already at 25.
const u16 gColorBuffer_ChannelCurve[32] = {
    0x0000, 0x020c, 0x042f, 0x066b, 0x08c0, 0x0b2f, 0x0db9, 0x105f,
    0x1322, 0x1603, 0x1903, 0x1c22, 0x1f63, 0x22c5, 0x264b, 0x29f4,
    0x2dc3, 0x31b9, 0x35d7, 0x3a1d, 0x3e8e, 0x432b, 0x47f6, 0x4cee,
    0x5217, 0x5772, 0x5d00, 0x62c3, 0x68bc, 0x6eed, 0x7559, 0x7c00,
};

const u16 gColorBuffer_ChannelCurveLight[32] = {
    0x0000, 0x0265, 0x04df, 0x076e, 0x0a14, 0x0cd1, 0x0fa5, 0x1291,
    0x1596, 0x18b4, 0x1bec, 0x1f3f, 0x22ac, 0x2636, 0x29dc, 0x2da0,
    0x3182, 0x3583, 0x39a3, 0x3de3, 0x4245, 0x46c9, 0x4b70, 0x503a,
    0x5529, 0x5a3d, 0x5f78, 0x64da, 0x6a65, 0x7019, 0x75f7, 0x7c00,
};

const u16 gColorBuffer_ChannelCurveContrast[32] = {
    0x0000, 0x0168, 0x02f7, 0x04b1, 0x0699, 0x08b2, 0x0b01, 0x0d8a,
    0x1052, 0x135d, 0x16b0, 0x1a52, 0x1e48, 0x2298, 0x274a, 0x2c65,
    0x31f0, 0x37f5, 0x3e7c, 0x458e, 0x4d37, 0x5580, 0x5e77, 0x6826,
    0x729c, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00,
};

/// Speaker definitions: {sprite, voice (0xff for none), idle line flags}.
/// The voice is the first of four blip sounds less 256: a message plays
/// sound 256 + voice as it types (Speaker_GetVoice), so the values
/// step by 4. Speaker_FindDefinition compares the first 130 and returns
/// the last, never compared, as the default.
const struct SpeakerDefinition gSpeakerDefinitions[131] = {
    {0, 16, 0x00},
    {1, 16, 0x00},
    {2, 16, 0x00},
    {3, 12, 0x00},
    {4, 28, 0x00},
    {5, 12, 0x00},
    {6, 12, 0x00},
    {18, 8, 0x00},
    {19, 8, 0x00},
    {30, 24, 0x00},
    {31, 20, 0x00},
    {32, 28, 0x00},
    {33, 20, 0x00},
    {34, 28, 0x00},
    {35, 28, 0x00},
    {36, 16, 0x00},
    {37, 8, 0x00},
    {38, 20, 0x00},
    {39, 12, 0x00},
    {40, 28, 0x00},
    {41, 16, 0x00},
    {42, 12, 0x00},
    {43, 28, 0x00},
    {44, 28, 0x00},
    {45, 20, 0x00},
    {46, 28, 0x00},
    {47, 28, 0x00},
    {48, 4, 0x00},
    {49, 24, 0x00},
    {50, 28, 0x00},
    {51, 24, 0x00},
    {52, 28, 0x00},
    {53, 28, 0x00},
    {54, 28, 0x00},
    {55, 24, 0x00},
    {71, 28, 0x00},
    {56, 20, 0x00},
    {57, 28, 0x00},
    {58, 28, 0x00},
    {59, 28, 0x00},
    {60, 20, 0x00},
    {61, 20, 0x00},
    {62, 28, 0x00},
    {63, 28, 0x00},
    {64, 28, 0x00},
    {65, 24, 0x00},
    {66, 16, 0x00},
    {67, 28, 0x00},
    {68, 24, 0x00},
    {69, 28, 0x00},
    {93, 16, 0x00},
    {90, 16, 0x00},
    {92, 16, 0x00},
    {91, 16, 0x00},
    {101, 28, 0x00},
    {102, 4, 0x00},
    {103, 24, 0x00},
    {104, 0, 0x00},
    {106, 12, 0x00},
    {107, 8, 0x00},
    {108, 16, 0x00},
    {109, 28, 0x00},
    {111, 20, 0x00},
    {114, 28, 0x00},
    {115, 20, 0x00},
    {116, 28, 0x00},
    {117, 24, 0x00},
    {118, 12, 0x00},
    {119, 28, 0x00},
    {120, 20, 0x00},
    {121, 24, 0x00},
    {122, 28, 0x00},
    {123, 28, 0x00},
    {124, 28, 0x00},
    {125, 28, 0x00},
    {128, 20, 0x00},
    {129, 16, 0x00},
    {130, 16, 0x00},
    {131, 12, 0x00},
    {132, 28, 0x00},
    {133, 16, 0x00},
    {134, 28, 0x00},
    {135, 28, 0x00},
    {136, 0, 0x00},
    {144, 28, 0x00},
    {145, 28, 0x00},
    {146, 28, 0x00},
    {147, 28, 0x00},
    {148, 12, 0x00},
    {149, 16, 0x00},
    {150, 28, 0x00},
    {151, 8, 0x00},
    {152, 28, 0x00},
    {153, 28, 0x00},
    {155, 0, 0x00},
    {156, 16, 0x00},
    {157, 28, 0x00},
    {158, 28, 0x00},
    {159, 16, 0x00},
    {160, 28, 0x00},
    {162, 20, 0x00},
    {163, 28, 0x00},
    {164, 24, 0x00},
    {165, 16, 0x00},
    {166, 20, 0x00},
    {167, 24, 0x00},
    {168, 8, 0x00},
    {169, 4, 0x00},
    {170, 28, 0x00},
    {171, 28, 0x00},
    {187, 28, 0xff},
    {189, 20, 0xff},
    {190, 28, 0xff},
    {191, 28, 0xff},
    {192, 16, 0xff},
    {193, 16, 0xff},
    {194, 24, 0xff},
    {195, 28, 0xff},
    {196, 0, 0xff},
    {197, 12, 0xff},
    {198, 28, 0xff},
    {199, 24, 0xff},
    {20, 255, 0xff},
    {200, 255, 0xff},
    {105, 255, 0xff},
    {110, 255, 0xff},
    {274, 255, 0xff},
    {231, 255, 0xff},
    {293, 255, 0xff},
    {22, 255, 0xff},
    {0, 16, 0x00},
};

/// Squashes as many times as the variant (1 to 3): variant v starts at word
/// (3 - v) * 32 and runs to the end. field.h declares it const u8[], and
/// event_commands.c adds (3 - v) << 7 bytes and reads it through an s32 *.
const s32 gActor_VariantScripts[97] = {
    // variant 3
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -0x800),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -0x800),
    SC_WAIT(4),
    // variant 2
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -0x800),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -0x800),
    SC_WAIT(4),
    // variant 1
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -0x800),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -0x800),
    SC_WAIT(4),
    SC_END,
};

/// A blinking actor's sprite param by frame ((gFrameCount >> 1) & 3,
/// Actor_UpdateBlink): 7 for two frames out of eight.
const u8 gActor_BlinkSpriteColors[4] = {7, 0, 0, 0};

/// Sine times 4096, 256 steps to a turn.
const s16 gLineScroll_WaveSine[256] = {
    0, 100, 200, 301, 401, 501, 601, 700,
    799, 897, 995, 1092, 1189, 1284, 1379, 1474,
    1567, 1659, 1751, 1841, 1930, 2018, 2105, 2191,
    2275, 2358, 2439, 2519, 2598, 2675, 2750, 2824,
    2896, 2966, 3034, 3101, 3166, 3229, 3289, 3348,
    3405, 3460, 3513, 3563, 3612, 3658, 3702, 3744,
    3784, 3821, 3856, 3889, 3919, 3947, 3973, 3996,
    4017, 4035, 4051, 4065, 4076, 4084, 4091, 4094,
    4096, 4094, 4091, 4084, 4076, 4065, 4051, 4035,
    4017, 3996, 3973, 3947, 3919, 3889, 3856, 3821,
    3784, 3744, 3702, 3658, 3612, 3563, 3513, 3460,
    3405, 3348, 3289, 3229, 3166, 3101, 3034, 2966,
    2896, 2824, 2750, 2675, 2598, 2519, 2439, 2358,
    2275, 2191, 2105, 2018, 1930, 1841, 1751, 1659,
    1567, 1474, 1379, 1284, 1189, 1092, 995, 897,
    799, 700, 601, 501, 401, 301, 200, 100,
    0, -100, -200, -301, -401, -501, -601, -700,
    -799, -897, -995, -1092, -1189, -1284, -1379, -1474,
    -1567, -1659, -1751, -1841, -1930, -2018, -2105, -2191,
    -2275, -2358, -2439, -2519, -2598, -2675, -2750, -2824,
    -2896, -2966, -3034, -3101, -3166, -3229, -3289, -3348,
    -3405, -3460, -3513, -3563, -3612, -3658, -3702, -3744,
    -3784, -3821, -3856, -3889, -3919, -3947, -3973, -3996,
    -4017, -4035, -4051, -4065, -4076, -4084, -4091, -4094,
    -4096, -4094, -4091, -4084, -4076, -4065, -4051, -4035,
    -4017, -3996, -3973, -3947, -3919, -3889, -3856, -3821,
    -3784, -3744, -3702, -3658, -3612, -3563, -3513, -3460,
    -3405, -3348, -3289, -3229, -3166, -3101, -3034, -2966,
    -2896, -2824, -2750, -2675, -2598, -2519, -2439, -2358,
    -2275, -2191, -2105, -2018, -1930, -1841, -1751, -1659,
    -1567, -1474, -1379, -1284, -1189, -1092, -995, -897,
    -799, -700, -601, -501, -401, -301, -200, -100,
};

/// {dx, dy, tile, shape, size} for each of 16 frames of a ground particle
/// (weather.c): dx and dy in screen pixels from its spot, tile from its
/// first tile. Its timer counts down, so the rows play from the last: it
/// comes down from 64 pixels right and 160 up, 8 left and 16 down a frame,
/// then stays 8 pixels left and 12 up of its spot for seven frames while its
/// tile goes from 8 to 20.
const s16 gFieldEffect_GroundParticleFrames[80] = {
    -8, -12, 20, 0, 1,
    -8, -12, 20, 0, 1,
    -8, -12, 20, 0, 1,
    -8, -12, 16, 0, 1,
    -8, -12, 16, 0, 1,
    -8, -12, 12, 0, 1,
    -8, -12, 8, 0, 1,
    0, -32, 0, 2, 2,
    8, -48, 0, 2, 2,
    16, -64, 0, 2, 2,
    24, -80, 0, 2, 2,
    32, -96, 0, 2, 2,
    40, -112, 0, 2, 2,
    48, -128, 0, 2, 2,
    56, -144, 0, 2, 2,
    64, -160, 0, 2, 2,
};

/// {dy, tile} of a sparkle (weather.c): dy in screen pixels from its spot, a
/// row every two frames. Its timer counts down, so the rows play from the
/// one it starts at (row 15 for a burst's first sparkle, as far as row 29
/// for the rest) to row 0: it comes down to its spot, slowing as it nears
/// it, its tile going 0, 4, 8, 12.
const struct SparklePicture gFieldEffect_SparklePictures[32] = {
    {-2, 12},
    {-4, 12},
    {-6, 12},
    {-8, 8},
    {-10, 8},
    {-12, 8},
    {-14, 8},
    {-16, 8},
    {-19, 4},
    {-21, 4},
    {-24, 4},
    {-27, 4},
    {-30, 4},
    {-33, 4},
    {-36, 4},
    {-40, 4},
    {-44, 4},
    {-48, 0},
    {-52, 0},
    {-56, 0},
    {-60, 0},
    {-64, 0},
    {-68, 0},
    {-72, 0},
    {-76, 0},
    {-80, 0},
    {-85, 0},
    {-90, 0},
    {-95, 0},
    {-100, 0},
    {-110, 0},
    {-120, 0},
};

/// Two scales that alternate ((gVBlankCount >> 2) & 1): full size and 0.97 of
/// it (FieldEffect_SetActorAlternatingWords, jupiter.c); nothing reads the
/// third word.
const u32 gFieldEffect_PulseScales[3] = {0x10000, 0xf851, 16};

/// [0] only releases the actor, +1 waits for it to arrive and releases it; +3 is
/// gFieldEffect_SourceHoldScript, which follows.
const s32 gFieldEffect_SharedParticleScript[] = {
    SC_RELEASE,
    SC_SYNC,
    SC_RELEASE,
};

/// Faces the watched actor every frame, forever: the caster holding its
/// target while Move, Carry or Lift has it (each gives the caster its target
/// first).
const s32 gFieldEffect_SourceHoldScript[] = {
    SC_FACE_WATCHED,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// A shard or particle: waits to arrive, slows to 1/16 pixel a frame, and
/// releases itself 10 frames later.
const s32 gFieldEffect_FragmentScript[] = {
    SC_SYNC,
    SC_SET(SW_SPEED, 0x1000),
    SC_ANIMATION(0),
    SC_WAIT(10),
    SC_RELEASE,
};

/// An angle for each value of the right, left, up and down bits of the
/// +Control Pad; DPAD_ANGLE_NONE for none (Input_GetDpadAngle, move.c). Opposite
/// directions cancel, and a diagonal gives none, so Move pushes only along
/// the four directions.
const u16 gFieldEffect_CyclePatternWords[16] = {
    DPAD_ANGLE_NONE, FACING_EAST,     FACING_WEST,     DPAD_ANGLE_NONE,
    FACING_NORTH,    DPAD_ANGLE_NONE, DPAD_ANGLE_NONE, FACING_NORTH,
    FACING_SOUTH,    DPAD_ANGLE_NONE, DPAD_ANGLE_NONE, FACING_SOUTH,
    DPAD_ANGLE_NONE, FACING_EAST,     FACING_WEST,     DPAD_ANGLE_NONE,
};

/// Move's hand animation by the quarter turns from its side of the target to
/// the push (move.c): 3 toward its side, 2 away from it, 1 either way across.
const u8 gFieldEffect_QuadrantModes[4] = {3, 1, 2, 1};

/// Waits 60 frames and releases the actor. Word +4 is
/// gFieldEffect_HeavyImpactScript.
const s32 gFieldEffect_BurstParticleActorScript[] = {
    SC_WAIT(60),
    SC_RELEASE,
};

/// The same four bytes as gFieldEffect_QuadrantModes; nothing reads them.
const u8 gUnused_BattleEffectBytes[4] = {3, 1, 2, 1};

/// Waits to arrive, sets off SpawnHeavyImpactEffect (lift.c) and releases the
/// actor.
const s32 gFieldEffect_HeavyImpactScript[] = {
    SC_SYNC,
    SC_CALL(SpawnHeavyImpactEffect),
    SC_RELEASE,
};

/// The three scripts of gFieldEffect_DurationScripts.
const s32 gFieldEffect_ShortEffectScript[] = {
    SC_WAIT(24),
    SC_RELEASE,
};

const s32 gFieldEffect_MediumEffectScript[] = {
    SC_WAIT(46),
    SC_RELEASE,
};

const s32 gFieldEffect_LongEffectScript[] = {
    SC_WAIT(128),
    SC_RELEASE,
};

/// The sprite params a paused actor flickers through at random
/// (FieldPsynergy_UpdateHaltedActor, halt.c): 0 twice as often as
/// each of 1 to 6.
const s8 gFieldEffect_RandomChildValues[8] = {0, 0, 1, 2, 3, 4, 5, 6};

/// The World Map markers' blend weight in sixteenths, against the picture's
/// 16 less it ((gFrameCount >> 1) & 31, WorldPicture_UpdateMarkers): solid
/// for 32 frames, then down to half and back over the next 32.
const u8 gWorldMap_MarkerBlendCycle[32] = {
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 15, 15, 14, 12, 10, 9, 9, 8, 9, 9, 10, 12, 14, 14, 15,
};

/// An angle for each +Control Pad direction (eight of them); 0xffff for none.
const u16 gWorldMap_CursorDirectionAngles[16] = {
    0xffff, 0x0000, 0x8000, 0xffff,
    0xc000, 0xe000, 0xa000, 0xc000,
    0x4000, 0x2000, 0x6000, 0x4000,
    0xffff, 0x0000, 0x8000, 0xffff,
};
// clang-format on
