#ifndef ALCHEMY_FIELD_SCREEN_TRANSITION_H
#define ALCHEMY_FIELD_SCREEN_TRANSITION_H

// What the field mode's parts share with the files that define their tables: the field effects'
// scripts and colour curves, and the screen transitions' shapes and window values.

#include "types.h"
#include "resource_ids.h"
#include "battle/battle.h"

// A run of lines of a window transition's shape (TRANSITION_SHAPE): its count of lines and its
// WIN_RANGE, or when that is 0 the window edges it moves between.
typedef struct TRANSITION_SPAN
{
	Uint16 count;
	Uint16 value;
	Uint8  x0;
	Uint8  x1;
	Uint8  y0;
	Uint8  y1;
} TRANSITION_SPAN;

// WININ or WINOUT showing every layer in both windows (bits 6 and 14 set too).
#define TRANSITION_SHOW_ALL      0x7f7f
// WININ or WINOUT showing every layer in its low byte (window 0, or outside both windows; bit 6 set
// too) and nothing in its high byte.
#define TRANSITION_SHOW_LOW_BYTE 0x007f

// A shape of WINDOW_TRANSITION_SHAPE: WINOUT and WININ while opening (swapped while closing), then
// up to three runs of lines down the screen, each a count of lines and either a WIN_RANGE for all
// of them or, when 0, the window edges from (x0, y0) on the first line toward (x1, y1) on the last.
typedef struct TRANSITION_SHAPE
{
	Uint16          first;
	Uint16          second;
	TRANSITION_SPAN spans[3];
} TRANSITION_SHAPE;

// The found item's picture over the leader: growing in from half size, then pulsing to 1.5 times
// its size and resting, over and over.
extern const Sint32     ItemDisplayGrowInScript[];
extern const Sint32     ItemDisplayPulseScript[];

// A Reveal marker's script, which releases it once fdIsRevealActive returns 0, Reveal having ended.
extern const Sint32     RevealMarkerScript[];
Bool                    fdIsRevealActive(struct ACTOR *interpreter);

// The palette tints' colour channel curves: each maps a 5-bit channel to a palette fade channel,
// shifted up 10 bits.
extern Uint16           FieldEffectChannelCurve[];
extern Uint16           FieldEffectChannelCurveLight[];
extern Uint16           FieldEffectChannelCurveContrast[];

// The width of the bars of WINDOW_TRANSITION_BARS, by progress.
extern const Uint8      ScreenTransitionNoiseWidths[];
// The wobble of the circle edge of WINDOW_TRANSITION_RIPPLE.
extern const Sint8      ScreenTransitionEdgeOffsets[];
// The window shape of each step of WINDOW_TRANSITION_SHAPE.
extern TRANSITION_SHAPE ScreenTransitionShapes[];
// 0 to 63 shuffled: the order in which the dissolve sets the pixels.
extern const Uint8      ScreenTransitionDitherTable[];

#endif
