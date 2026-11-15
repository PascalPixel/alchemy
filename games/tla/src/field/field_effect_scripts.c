/// @file
/// Four effect scripts of the field sequence bank, the found item's picture
/// growing in and pulsing, a sinking particle and a Reveal marker, then the
/// screen transitions' tables and the palette tints' channel curves, as The
/// Broken Seal's field_sequence_tables.c has them.

#include "fixed_math.h"
#include "types.h"
#include "field/actor_script.h"
#include "field/screen_transition.h"
#include "battle/battle.h"

/// Grows from half size to full size, a step every 2 frames.
const s32 gFieldEffect_ParticleEmitterScript[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE / 2),
    SC_SET(SW_SCALE_Y, FIXED_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE * 3 / 4),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 3 / 4),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE * 7 / 8),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 7 / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE * 15 / 16),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 15 / 16),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_END,
};

/// Pulses twice, to 1.5 times full size, then rests, over and over: the found
/// item's picture held over the leader (field_sequence_item_display.c).
const s32 gFieldEffect_RisingPulseScript[] = {
    SC_WAIT(10),
    SC_SET(SW_SCALE_X, FIXED_ONE * 11 / 8),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 11 / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE * 3 / 2),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 3 / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE * 9 / 8),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 9 / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE * 11 / 8),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 11 / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE * 3 / 2),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 3 / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE * 9 / 8),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 9 / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_WAIT(2),
    SC_WAIT(50),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/// Sinks a quarter pixel a frame for 60 frames, then disappears.
const s32 gBattleEffect_ParticleScript[] = {
    SC_ADD(SW_Y, -toFIXED(1) / 4),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_RELEASE,
};

/// A Reveal marker: stays while Field_IsRevealActive returns nonzero (SC_CALL
/// runs it again each frame), and releases itself once Reveal ends; the same
/// as The Broken Seal's gFieldEffect_MarkerParticleScript.
const s32 gBattleEffect_MarkerParticleScript[] = {
    SC_CALL(Field_IsRevealActive),
    SC_RELEASE,
};

// clang-format off

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

/// Colour channel curves of the palette tints (field_sequence_palette_fade.c):
/// each maps a 5-bit channel (0 to 31) to a palette fade channel, shifted up 10
/// bits, so 31 is 0x7c00.
u16 gFieldEffect_ChannelCurve[32] = {
    0x0000, 0x020c, 0x042f, 0x066b, 0x08c0, 0x0b2f, 0x0db9, 0x105f,
    0x1322, 0x1603, 0x1903, 0x1c22, 0x1f63, 0x22c5, 0x264b, 0x29f4,
    0x2dc3, 0x31b9, 0x35d7, 0x3a1d, 0x3e8e, 0x432b, 0x47f6, 0x4cee,
    0x5217, 0x5772, 0x5d00, 0x62c3, 0x68bc, 0x6eed, 0x7559, 0x7c00,
};

u16 gFieldEffect_ChannelCurveLight[32] = {
    0x0000, 0x0265, 0x04df, 0x076e, 0x0a14, 0x0cd1, 0x0fa5, 0x1291,
    0x1596, 0x18b4, 0x1bec, 0x1f3f, 0x22ac, 0x2636, 0x29dc, 0x2da0,
    0x3182, 0x3583, 0x39a3, 0x3de3, 0x4245, 0x46c9, 0x4b70, 0x503a,
    0x5529, 0x5a3d, 0x5f78, 0x64da, 0x6a65, 0x7019, 0x75f7, 0x7c00,
};

u16 gFieldEffect_ChannelCurveContrast[32] = {
    0x0000, 0x0168, 0x02f7, 0x04b1, 0x0699, 0x08b2, 0x0b01, 0x0d8a,
    0x1052, 0x135d, 0x16b0, 0x1a52, 0x1e48, 0x2298, 0x274a, 0x2c65,
    0x31f0, 0x37f5, 0x3e7c, 0x458e, 0x4d37, 0x5580, 0x5e77, 0x6826,
    0x729c, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00,
};
// clang-format on
