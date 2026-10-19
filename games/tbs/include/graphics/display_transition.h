#ifndef ALCHEMY_DISPLAY_TRANSITION_H
#define ALCHEMY_DISPLAY_TRANSITION_H

/// @file
/// The display transition's frames, windows and state.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

struct DisplayTransitionFrame {
  u8 unk000[0x508];
  u8 paletteNibbles[0x22];
  u16 value;
  u8 unk52c[13];
  u8 ditherToggle;
  s8 start;
  s8 end;
  s8 duration;
  s8 step;
};

struct DisplayTransitionWindow {
  u32 unk000[5];
  u16 dispcnt;
  u8 unk016[0xea];
  u16 firstLine;
  u16 secondLine;
  u16 irqFirstLine;
  u16 irqSecondLine;
  u16 irqPhase;
};

struct DisplayTransitionState {
  u16 pages[2][322];
  u32 solidTile[8];
  u16 mode;
  u16 value;
  s32 centreX;
  s32 centreY;
  s16 mask;
  s16 active;
  u8 unk538;
  u8 page;
  s8 start;
  s8 end;
  s8 duration;
  s8 step;
  s8 nextMode;
};

STATIC_ASSERT(offsetof(struct DisplayTransitionFrame, value) == 0x52a);
STATIC_ASSERT(offsetof(struct DisplayTransitionFrame, start) == 0x53a);
STATIC_ASSERT(sizeof(struct DisplayTransitionFrame) == 0x540);
STATIC_ASSERT(offsetof(struct DisplayTransitionWindow, dispcnt) == 0x14);
STATIC_ASSERT(offsetof(struct DisplayTransitionWindow, firstLine) == 0x100);
STATIC_ASSERT(offsetof(struct DisplayTransitionWindow, irqPhase) == 0x108);
STATIC_ASSERT(sizeof(struct DisplayTransitionWindow) == 0x10c);
STATIC_ASSERT(offsetof(struct DisplayTransitionState, pages) == 0);
STATIC_ASSERT(offsetof(struct DisplayTransitionState, solidTile) == 0x508);
STATIC_ASSERT(offsetof(struct DisplayTransitionState, page) == 0x539);
STATIC_ASSERT(offsetof(struct DisplayTransitionState, mode) == 0x528);
STATIC_ASSERT(offsetof(struct DisplayTransitionState, centreX) == 0x52c);
STATIC_ASSERT(offsetof(struct DisplayTransitionState, mask) == 0x534);
STATIC_ASSERT(offsetof(struct DisplayTransitionState, nextMode) == 0x53e);
STATIC_ASSERT(offsetof(struct DisplayTransitionState, duration) == 0x53c);
STATIC_ASSERT(sizeof(struct DisplayTransitionState) == 0x540);

#endif
