#include "types.h"
#include "motobj.h"
#include "objdisp.h"

// This game's battle object: The Broken Seal's BattleObj with its last
// halfword split into the fade flag and a visible flag.
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

// Resets a battle actor, making its object when it has none: scale 0xe666,
// the anchors cleared unless keep is set, no sprite, animation, effect or
// state, facing ArcTan2(0, 0) + 0x8000, visible, then the actor object
// script. The tail of The Broken Seal's BattleObjMake, for an actor without
// a unit.
void BattleObj_Reset(struct TlaBattleObj *actor, s32 keep) {
  struct MotionObject *object;
  s32 facingAngle;
  s32 zero;

  if (actor->object == 0)
    object = Func_080200c0(0xf000, 0, 0, 0);
  else
    object = actor->object;
  /* FAKEMATCH(temporary): one zero, set before the keep test and stored on
   * both paths; as plain 0 stores, 38 of 63 instructions differ in tla-en. */
  zero = 0;
  actor->scale = 0xe666;
  actor->object = object;
  if (keep == 0) {
    actor->anchorZ = zero;
    actor->anchorX = zero;
    object->x = actor->anchorX;
    object->z = actor->anchorZ;
  }
  actor->palette = 0;
  actor->resource = 0;
  actor->overlay = 0;
  actor->animation = 0;
  actor->effect = 0;
  /* FAKEMATCH(one-pass): the clears and the facing in a one-pass block,
   * which loads the byte zero before the palette's store; as plain
   * statements, 4 of 63 instructions differ in tla-en. */
  do {
    actor->animationEntry = 0;
    actor->activeConditions = 0;
    actor->effectEntry = 0;
    actor->active = 0;
    actor->selectedCondition = 0;
    actor->fading = 0;
    facingAngle = ArcTan2(0, 0) + 0x8000;
    object->angle = facingAngle;
    object->unk59 = 3;
  } while (0);
  object->motionFlags = 2;
  actor->visible = 1;
  ObjectDispatch_InitializeFar((struct DispatchObject *)object, Data_0812cacc);
}
