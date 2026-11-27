/* Four effect scripts of the field mode module, the found item's picture
   growing in and pulsing, a sinking particle and a Reveal marker, then the
   screen transitions' tables and the palette tints' channel curves, as The
   Broken Seal's field_mode_tables.c has them. */

#include "field/actor_script.h"
#include "field/screen_transition.h"

/* The found item's picture growing in over the leader, from half size to full
   size, a step every 2 ticks (field_mode_item_display.c). */
const Sint32 ItemDisplayGrowInScript[] = {
	SC_SET(SW_SCALE_X, toFIXED(1) / 2),
	SC_SET(SW_SCALE_Y, toFIXED(1) / 2),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(3) / 4),
	SC_SET(SW_SCALE_Y, toFIXED(3) / 4),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(7) / 8),
	SC_SET(SW_SCALE_Y, toFIXED(7) / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(15) / 16),
	SC_SET(SW_SCALE_Y, toFIXED(15) / 16),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_END,
};

/* Pulses twice, to 1.5 times full size, then rests, over and over: the found
   item's picture held over the leader (field_mode_item_display.c). */
const Sint32 ItemDisplayPulseScript[] = {
	SC_WAIT(10),
	SC_SET(SW_SCALE_X, toFIXED(11) / 8),
	SC_SET(SW_SCALE_Y, toFIXED(11) / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(3) / 2),
	SC_SET(SW_SCALE_Y, toFIXED(3) / 2),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(9) / 8),
	SC_SET(SW_SCALE_Y, toFIXED(9) / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(11) / 8),
	SC_SET(SW_SCALE_Y, toFIXED(11) / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(3) / 2),
	SC_SET(SW_SCALE_Y, toFIXED(3) / 2),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(9) / 8),
	SC_SET(SW_SCALE_Y, toFIXED(9) / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_WAIT(2),
	SC_WAIT(50),
	SC_LOOP(SC_FOREVER, SC_TOP),
	SC_END,
};

/* A sparkle round the found item, sinking a quarter pixel a tick for 60 ticks
   before it goes (field_mode_item_display.c). */
const Sint32 ItemDisplaySparkleScript[] = {
	SC_ADD(SW_Y, -toFIXED(1) / 4),
	SC_WAIT(1),
	SC_LOOP(60, SC_TOP),
	SC_RELEASE,
};

/* A Reveal marker: stays while fdIsRevealActive returns nonzero (SC_CALL runs
   it again each tick), and releases itself once Reveal ends; the same as The
   Broken Seal's FieldEffect_MarkerParticleScript. */
const Sint32 RevealMarkerScript[] = {
	SC_CALL(fdIsRevealActive),
	SC_RELEASE,
};

/* clang-format off */

/* The width of the bars of WINDOW_TRANSITION_BARS, by progress. */
const Uint8 ScreenTransitionNoiseWidths[34] = {
	0, 1, 2, 2, 2, 2, 3, 3, 4, 4, 5, 6, 7, 8, 9, 11, 13,
	16, 18, 22, 26, 30, 36, 43, 51, 60, 72, 85, 101, 120, 143, 170, 202, 240,
};

/* The wobble of the circle edge of WINDOW_TRANSITION_RIPPLE. */
const Sint8 ScreenTransitionEdgeOffsets[32] = {
	-4, -3, -3, -3, -2, -2, -1, 0, 0, 0, 1, 2, 2, 3, 3, 3,
	4, 3, 3, 3, 2, 2, 1, 0, 0, 0, -1, -2, -2, -3, -3, -3,
};

/* 0 to 63 shuffled: the order in which the dissolve sets the pixels. */
const Uint8 ScreenTransitionDitherTable[64] = {
	0, 12, 35, 3, 29, 39, 6, 7, 50, 59, 4, 11, 18, 40, 46, 31,
	16, 42, 47, 51, 20, 23, 43, 32, 8, 52, 17, 41, 54, 15, 63, 24,
	58, 5, 2, 26, 36, 61, 9, 34, 56, 27, 10, 48, 53, 30, 28, 22,
	62, 45, 19, 60, 57, 49, 25, 55, 38, 14, 44, 13, 33, 37, 1, 21,
};

/* Colour channel curves of the palette tints (field_mode_palette_fade.c): */
/* each maps a 5-bit channel (0 to 31) to a palette fade channel, shifted up */
/* 10 bits, so 31 is 31 << 10. */
Uint16 FieldEffectChannelCurve[32] = {
	0, 524, 1071, 1643, 2240, 2863, 3513, 4191,
	4898, 5635, 6403, 7202, 8035, 8901, 9803, 10740,
	11715, 12729, 13783, 14877, 16014, 17195, 18422, 19694,
	21015, 22386, 23808, 25283, 26812, 28397, 30041, 31744,
};
Uint16 FieldEffectChannelCurveLight[32] = {
	0, 613, 1247, 1902, 2580, 3281, 4005, 4753,
	5526, 6324, 7148, 7999, 8876, 9782, 10716, 11680,
	12674, 13699, 14755, 15843, 16965, 18121, 19312, 20538,
	21801, 23101, 24440, 25818, 27237, 28697, 30199, 31744,
};
Uint16 FieldEffectChannelCurveContrast[32] = {
	0, 360, 759, 1201, 1689, 2226, 2817, 3466,
	4178, 4957, 5808, 6738, 7752, 8856, 10058, 11365,
	12784, 14325, 15996, 17806, 19767, 21888, 24183, 26662,
	29340, 31744, 31744, 31744, 31744, 31744, 31744, 31744,
};
/* clang-format on */
