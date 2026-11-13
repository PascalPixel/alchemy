/// @file
/// Scene code of overlay 0x69c, in ROM order.

#include "../shared/push.h"
#include "types.h"
#include "map.h"
#include "actor.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "event_inline.h"
#include "module/field/field.h"
#include "module/scene/jupiter_lighthouse_room_3.h"
#include "module/scene/scene.h"

/// Where a pushed object comes to rest, read by the push that follows.
struct PushTarget {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into the footprint table
  s32 id;
  void (*callback)(void);
};

extern s8 gPushFootprints[][4];
extern s32 gData_0200249c[];

#define ABS(x) ((x) < 0 ? -(x) : (x))

static inline s32 Func_02002338_(struct Actor *object, s32 *position) {
  return MapGrid_CheckCollision(object, position);
}

/// Finds the object the leader is pushing and steps its footprint, one cell
/// at a time in the leader's direction, until a cell of it is blocked
/// (MapGrid_CheckCollision returns 2). Leaves the object's resting place in
/// target and returns 1, or 0 when nothing is pushed or it cannot move at all.
DEFINE_PUSH_TARGET_FIND_SLIDE(PushTarget_FindSlide, PushTarget_FindAhead,
                              gPushFootprints, gData_0200249c, Func_02002338_)

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

/// Finds the object the leader is pushing: among objects 8 to 63 whose sprite
/// shape is in the six-entry footprint table, the one whose footprint holds the
/// cell one step ahead of the leader, when the leader is not in line with its
/// near edge. Returns the object, with its id and the footprint's index.
DEFINE_PUSH_TARGET_FIND_AHEAD(PushTarget_FindAhead, gPushKinds, gData_0200249c,
                              gPushFootprints)

void Func_02001300(void);

DEFINE_PUSH_TARGET_PUSH_FILL(PushTarget_PushFill, gPushFootprints,
                             Func_02001300, Actor_SelectAnimation,
                             gData_0200249c)
