#ifndef ALCHEMY_FIELD_SCREEN_TRANSITION_H
#define ALCHEMY_FIELD_SCREEN_TRANSITION_H

/// @file
/// What the field sequence's parts share with the files that define their
/// tables: the field effects' scripts and colour curves, and the screen
/// transitions' shapes and window values.

#include "types.h"
#include "resource_ids.h"
#include "battle/battle.h"

/// A run of lines of a window transition's shape (TransitionShape): its count
/// of lines and its WIN_RANGE, or when that is 0 the window edges it moves
/// between.
struct TransitionSpan {
  u16 count;
  u16 value;
  u8 x0;
  u8 x1;
  u8 y0;
  u8 y1;
};

/// WININ or WINOUT showing every layer in both windows (bits 6 and 14 set too).
#define TRANSITION_SHOW_ALL 0x7f7f
/// WININ or WINOUT showing every layer in its low byte (window 0, or outside
/// both windows; bit 6 set too) and nothing in its high byte.
#define TRANSITION_SHOW_LOW_BYTE 0x007f

/// A shape of WINDOW_TRANSITION_SHAPE: WINOUT and WININ while opening (swapped
/// while closing), then up to three runs of lines down the screen, each a
/// count of lines and either a WIN_RANGE for all of them or, when 0, the window
/// edges from (x0, y0) on the first line toward (x1, y1) on the last.
struct TransitionShape {
  u16 first;
  u16 second;
  struct TransitionSpan spans[3];
};

/// The found item's picture over the leader: growing in from half size, then
/// pulsing to 1.5 times its size and resting, over and over.
extern const s32 gItemDisplay_GrowInScript[];
extern const s32 gItemDisplay_PulseScript[];

/// A Reveal marker's script, which releases it once Field_IsRevealActive
/// returns 0, Reveal having ended.
extern const s32 gReveal_MarkerScript[];
s32 Field_IsRevealActive(struct Actor *interpreter);

/// The palette tints' colour channel curves: each maps a 5-bit channel to a
/// palette fade channel, shifted up 10 bits.
extern u16 gFieldEffect_ChannelCurve[];
extern u16 gFieldEffect_ChannelCurveLight[];
extern u16 gFieldEffect_ChannelCurveContrast[];

/// The width of the bars of WINDOW_TRANSITION_BARS, by progress.
extern const u8 gScreenTransition_NoiseWidths[];
/// The wobble of the circle edge of WINDOW_TRANSITION_RIPPLE.
extern const s8 gScreenTransition_EdgeOffsets[];
/// The window shape of each step of WINDOW_TRANSITION_SHAPE.
extern struct TransitionShape gScreenTransition_Shapes[];
/// 0 to 63 shuffled: the order in which the dissolve sets the pixels.
extern const u8 gScreenTransition_DitherTable[];

#endif
