#include "types.h"
#include "field_obj.h"

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
  flag = *fp | 2;
  *fp = flag;
  if (ref->z.fixed < actor->z.fixed) {
    s32 lim = actor->z.fixed - ref->z.fixed;
    s32 ay;
    lim += 0x00040000;
    ay = actor->y.fixed;
    ay += lim;
    if (ref->y.fixed <= ay) {
      flag &= 0xfd;
      *fp = flag;
    }
  }
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

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
