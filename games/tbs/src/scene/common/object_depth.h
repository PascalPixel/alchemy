#include "types.h"
#include "field_obj.h"

// Tunnel Ruins and Venus Lighthouse: blocks the leader can stand beside, behind
// or on.

// An update hook: draw the block under the actors (ACTOR_PRIORITY_UNDERFOOT)
// unless the leader is north of it and no higher than its height plus their
// distance and 4 pixels, that is, behind it rather than on it.
//
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 7 bytes (extent 70 to 68 bytes). Keep the
// unused native result in matching builds.
#ifdef UBFIX
static void
SceneActor_SetFlagBitByRelativeDepth(union FieldObject *fieldObject) {
#else
static s32
SceneActor_SetFlagBitByRelativeDepth(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct FieldActor *ref;
  u8 *fp;
  u8 flag;
  ref = Object_GetById(ACTOR_PARTY_LEADER);
  fp = &actor->priorityFlags;
  flag = *fp | ACTOR_PRIORITY_UNDERFOOT;
  *fp = flag;
  if (ref->z.fixed < actor->z.fixed) {
    s32 lim = actor->z.fixed - ref->z.fixed;
    s32 ay;
    lim += PIXELS(4);
    ay = actor->y.fixed;
    ay += lim;
    if (ref->y.fixed <= ay) {
      flag &= ~ACTOR_PRIORITY_UNDERFOOT;
      *fp = flag;
    }
  }
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Unless actor first is south of actor second, swap their positions and wait a
/// frame.
static void SceneActor_SwapPositionsByDepth(s32 first, s32 second) {
  struct FieldActor *a = Object_GetById(first);
  struct FieldActor *b = Object_GetById(second);

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
    Engine_TaskWait(1);
  }
}
