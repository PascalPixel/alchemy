#include "types.h"
#include "object_event.h"

// Tunnel Ruins and Venus Lighthouse: blocks the leader can stand beside, behind
// or on.

// An update hook: draw the block under the object events
// (OBJ_EVENT_PRIORITY_UNDERFOOT) unless the leader is north of it and no higher
// than its height plus their distance and 4 pixels, that is, behind it rather
// than on it.
//
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 7 bytes (extent 70 to 68 bytes). Keep the
// unused native result in matching builds.
#ifdef UBFIX
static void
ObjectEvent_SetFlagBitByRelativeDepth(union ObjectEventOrEffect *fieldObject) {
#else
static s32
ObjectEvent_SetFlagBitByRelativeDepth(union ObjectEventOrEffect *fieldObject) {
#endif
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *ref;
  u8 *fp;
  u8 flag;
  ref = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  fp = &objectEvent->priorityFlags;
  flag = *fp | OBJ_EVENT_PRIORITY_UNDERFOOT;
  *fp = flag;
  if (ref->z.fixed < objectEvent->z.fixed) {
    s32 lim = objectEvent->z.fixed - ref->z.fixed;
    s32 ay;
    lim += PIXELS(4);
    ay = objectEvent->y.fixed;
    ay += lim;
    if (ref->y.fixed <= ay) {
      flag &= ~OBJ_EVENT_PRIORITY_UNDERFOOT;
      *fp = flag;
    }
  }
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Unless object event first is south of object event second, swaps their
/// positions and waits a frame.
static void ObjectEvent_SwapPositionsByDepth(s32 first, s32 second) {
  struct ObjectEvent *a = ObjectEvent_GetById(first);
  struct ObjectEvent *b = ObjectEvent_GetById(second);

  if (a->z.fixed <= b->z.fixed) {
    s32 t;

    t = a->x.fixed;
    a->x.fixed = b->x.fixed;
    b->x.fixed = t;
    t = a->y.fixed;
    a->y.fixed = b->y.fixed;
    b->y.fixed = t;
    t = a->z.fixed;
    a->z.fixed = b->z.fixed;
    b->z.fixed = t;
    WaitFrames(1);
  }
}
