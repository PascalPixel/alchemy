#ifndef ALCHEMY_VIEW_MOVE_H
#define ALCHEMY_VIEW_MOVE_H

#include "types.h"
#include "static_assert.h"

struct ObjectRuntime;

// The view moves of the field Psynergy scenes (kinds 19 and 27). Their work
// sits in heap slot 92 (battle_fx_work[0]): Func_080e89e4 opens the map's
// view bounds and keeps the old ones, Func_080e8b44 swings the view centre
// each frame, and Func_080e8c9c puts the bounds back. Func_080e8cfc gathers
// the nearby objects whose sprites shake with the view, and Func_080e8d80
// sets them back.

#define HEAP_SLOT_VIEW_MOVE 92

// Directions a view move swings in.
#define VIEW_MOVE_X 1
#define VIEW_MOVE_Z 2

struct ViewMoveWork {
  s32 x; // the view centre's position when the move began
  s32 y;
  s32 z;
  s32 savedMinX; // the map's view bounds when the move began
  s32 savedMaxX;
  s32 savedMinZ;
  s32 savedMaxZ;
  s16 edgeX; // -1 beside the low edge, 1 beside the high edge, else 0
  s16 edgeZ;
  struct ObjectRuntime *objects[8];
  u16 spriteOffsets[8];
  s32 count;
};

STATIC_ASSERT(offsetof(struct ViewMoveWork, edgeX) == 0x1c);
STATIC_ASSERT(offsetof(struct ViewMoveWork, objects) == 0x20);
STATIC_ASSERT(offsetof(struct ViewMoveWork, count) == 0x50);

void Func_080e89e4(s32 direction);
void Func_080e8b44(s32 direction, s32 phase, s32 amplitude);
void Func_080e8c9c(void);
void Func_080e8cfc(void);
void Func_080e8d80(void);

#endif
