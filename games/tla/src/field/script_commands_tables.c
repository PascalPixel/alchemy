// Tables of the field commands, in the order of The Broken Seal's field_mode_tables.c: the rope's
// headings, the line scroll's sine, the ground particles' ticks and offsets, the sparkles and
// Jupiter's pulse.

#include "fixed_math.h"
#include "types.h"
#include "field/script_commands.h"
#include "field/field.h"
#include "field/scene_tables.h"

// clang-format off

// The heading the +Control Pad points, by its right, left, up and down bits (DPAD_INDEX), where fdLeaderStepOffRope and fdLeaderStepOffTightrope step; DPAD_ANGLE_NONE for none or opposed keys. The same table as InputDpadAngles.
const Uint16 RopeKeyHeadings[16] = {
	DPAD_ANGLE_NONE, FACING_EAST,     FACING_WEST,     DPAD_ANGLE_NONE,
	FACING_NORTH,    DPAD_ANGLE_NONE, DPAD_ANGLE_NONE, FACING_NORTH,
	FACING_SOUTH,    DPAD_ANGLE_NONE, DPAD_ANGLE_NONE, FACING_SOUTH,
	DPAD_ANGLE_NONE, FACING_EAST,     FACING_WEST,     DPAD_ANGLE_NONE,
};

// A sine wave of 256 steps and amplitude 4096 that the line scrolls ride (script_commands_line_scroll.c), as The Broken Seal's.
const Sint16 LineScrollWaveSine[256] = {
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

// {dx, dy, tile, shape, size} for each of 16 frames of a ground particle (script_commands_weather.c): dx and dy in screen pixels from its spot, tile from its first tile.
const Sint16 WeatherRainSplashFrames[80] = {
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

// {x, y} screen offsets of a falling raindrop in pixels by the ticks it has left (fdWeatherUpdateRain); The Broken Seal has no such table.
const Sint16 GroundParticlesOffsets[22] = {
	-32, -16,
	-48, -23,
	-64, -34,
	-80, -43,
	-96, -52,
	-112, -61,
	-128, -70,
	-144, -77,
	-160, -84,
	-176, -91,
	-192, -98,
};

// {dy, tile} of a sparkle (script_commands_motes.c): dy in screen pixels from its spot, a row every two ticks.
const SPARKLE_PICTURE FieldEffectSparklePictures[32] = {
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

// The two scales fdEffectPulseActorScale switches between every 4 ticks, full size and 0.97 of it: the leader pulsing as a Mars Djinni joins; The Broken Seal's table, its third word gone.
const Uint32 FieldEffectPulseScales[2] = {toFIXED(1), toFIXED(97) / 100};
// clang-format on
