// Draft, differ 44 of 63: resets a battle actor, making its object when it
// has none: scale 0xe666, cleared anchors (unless keep), animation, effect
// and state, facing ArcTan2(0, 0) + 0x8000, then the actor object script.
// The tail of ☀️'s BattleObjMake (games/tbs/src/battle/battle.c), for an
// actor without a unit. Left: register choice of the zero: the native sets
// one zero in r2 before the keep test and stores it on both paths; here CSE
// knows keep is 0 inside the branch and stores r7 there, and a fresh r3 after
// the join, so every zero store and the pooled byte zero's register (r3
// natively, r2 here) differ; also the object is fetched into r6 directly
// where the native keeps it in r0 until after the call. Tried: the angle
// added in a halfword expression (47, 0x8000 pooled).
#include "types.h"
#include "motobj.h"
#include "objdisp.h"

struct TlaBattleObj {
  struct MotionObject *object;
  u16 resource;
  u16 overlay;
  u16 animation;
  u16 effect;
  s32 anchorX;
  s32 anchorZ;
  s32 palette;
  s32 scale;
  u16 activeConditions;
  u8 selectedCondition;
  s8 cycleTimer;
  void *animationEntry;
  void *effectEntry;
  s16 active;
  u8 fading;
  u8 visible;
};

extern const s32 Data_0812cacc[];

struct MotionObject *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
s32 ArcTan2(s32 y, s32 x);

void Func_0811a720(struct TlaBattleObj *actor, s32 keep) {
  struct MotionObject *object = actor->object;
  s32 facingAngle;

  if (object == 0)
    object = Func_080200c0(0xf000, 0, 0, 0);
  actor->scale = 0xe666;
  actor->object = object;
  if (keep == 0) {
    actor->anchorZ = 0;
    actor->anchorX = 0;
    object->x = actor->anchorX;
    object->z = actor->anchorZ;
  }
  actor->palette = 0;
  actor->resource = 0;
  actor->overlay = 0;
  actor->animation = 0;
  actor->effect = 0;
  actor->animationEntry = 0;
  actor->activeConditions = 0;
  actor->effectEntry = 0;
  actor->active = 0;
  actor->selectedCondition = 0;
  actor->fading = 0;
  facingAngle = ArcTan2(0, 0) + 0x8000;
  object->angle = facingAngle;
  object->unk59 = 3;
  object->motionFlags = 2;
  actor->visible = 1;
  ObjectDispatch_InitializeFar((struct DispatchObject *)object, Data_0812cacc);
}
