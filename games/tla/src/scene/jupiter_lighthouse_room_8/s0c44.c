/// @file
/// Scene code of overlay 0x6a1, in ROM order, from s0c44's offset up to the
/// next object the overlay links.

#include "../shared/routines.h"
#include "types.h"
#include "map.h"
#include "actor.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "event_inline.h"
#include "module/field/field.h"
#include "module/scene/jupiter_lighthouse_room_8.h"
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

extern s8 gData_020022dc[][4];
extern s32 gData_020022f4[];


#define ABS(x) ((x) < 0 ? -(x) : (x))

static inline s32 Func_020021b8_(struct Actor *object, s32 *position) {
  return Func_020021b8(object, position);
}

/// Finds the object the leader is pushing and steps its footprint, one cell
/// at a time in the leader's direction, until a cell of it is blocked
/// (Func_020021b8 returns 2). Leaves the object's resting place in target and
/// returns 1, or 0 when nothing is pushed or it cannot move at all.
DEFINE_FUNC_02000C44(Func_02000c44, Func_02000dac, gData_020022dc,
    gData_020022f4, Func_020021b8_)

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

DEFINE_FUNC_02000DAC(Func_02000dac, gData_020022d0, gData_020022f4,
    gData_020022dc)

void Func_02001178(void);

DEFINE_FUNC_02000EC8(Func_02000ec8, gData_020022dc, Func_02001178,
    Func_02002160, gData_020022f4)
