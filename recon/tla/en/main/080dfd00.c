// Draft, differ 125 of 334 (2026-10-15), The Broken Seal's
// FieldFx_RunWhirlwind (shape 0.64, structure 0.73). ⚓️ keeps ☀️'s carried orb and
// its orbiting motes, and adds two kinds of target, told apart by the target's
// first animation (372 and 373): for both the orb shrinks away at once, for 372
// with motes that wander off round it, and the target is then handed to
// Func_080db0b0. The kinds are a switch: the listing tests 0, then 1, then 2-3
// as one range (subs r3, #2; cmp r3, #1; bhi), and kind 1 falls into kind 0's
// ending. Left: the prologue's vector copies come in another order and register
// (the native keeps from in r9 and to in r8 through the carry loop), and
// the wander loop's 3 << 16 stays in r11 in the native.
// Tried: the three vectors in each declaration order, 125-131 (this order
// is best).
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "fx_scene.h"
#include "object_runtime.h"
#include "animspr.h"

struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};

// An orbiting mote: radial.c's OrbitEffect.
struct OrbitEffect {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[20];
  s32 scale;
  u8 unk2c[28];
  s32 speed;
  u8 unk4c[24];
  u16 timer;
  s16 pause;
  struct ObjectRuntime *anchor;
  void (*update)(struct OrbitEffect *);
};

// ⚓️'s composite sprite, as far as this reads it.
struct OrbTargetSprite {
  u8 unk00[0x28];
  struct AnimationEntry *entries[4];
};

struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void BattleEffect_InitializeSharedScene(void);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct EffectVector *p);
void BattleEffect_ShrinkObjectScaleUntilHalf(struct ObjectRuntime *object);
void Func_080dfc3c(struct OrbitEffect *mote);
void BattleEffect_WanderAroundAnchor(struct OrbitEffect *mote);
void BattleEffect_AgeAndCueAtEighty(struct ObjectRuntime *object);
void BattleEffect_SetObjectTickMode(void *object, s32 mode);
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080db0b0(s32 objectId);
void Func_08020340(struct ObjectRuntime *object);
void Object_Destroy(struct ObjectRuntime *object);

#define ORB_OBJECT 280
#define MOTE_OBJECT 308
#define SE_ORB_ARRIVES 207
#define SE_ORB_FADES 136

// The two targets ⚓️ treats apart, by their first animation.
#define TARGET_ANIMATION_A 372
#define TARGET_ANIMATION_B 373

enum OrbKind {
  ORB_PLAIN,    // the scene is enabled: the orb only comes and goes
  ORB_ORBITING, // motes circle the orb while the target shrinks
  ORB_WANDER,   // target 372: motes wander off, the target is handed on
  ORB_HANDOFF,  // target 373: the target is handed on
};

// Animation 14: carry an orb from the caster to the scene's position over
// ten frames, growing it from a quarter size, then play out the kind.
void FieldFx_RunWhirlwind(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct ObjectRuntime *target = scene->child;
  struct ObjectRuntime *caster = scene->mainObject;
  struct OrbTargetSprite *sprite = target->animation;
  struct EffectVector position;
  struct EffectVector to;
  struct EffectVector from;
  struct ObjectRuntime *orb;
  struct OrbitEffect *mote;
  s32 kind = ORB_PLAIN;
  s32 step;

  if (scene->childOption != 0)
    return;
  if (scene->enabled == 0) {
    s32 animation;

    kind = ORB_ORBITING;
    animation = sprite->entries[0]->animId;
    if (animation == TARGET_ANIMATION_A)
      kind = ORB_WANDER;
    if (animation == TARGET_ANIMATION_B)
      kind = ORB_HANDOFF;
  }

  from.x = caster->x;
  from.y = caster->y;
  from.z = caster->z;
  to.x = scene->x;
  to.y = scene->y - (4 << 16);
  to.z = scene->z;
  orb = Object_Spawn(ORB_OBJECT, 0, 0, 0);
  if (orb == 0)
    return;

  BattleEffect_InitializeSharedScene();
  Object_SetMode(orb, 2);
  for (step = 0; step < 11; step++) {
    s32 scale;

    orb->x = from.x + step * (to.x - from.x) / 10;
    orb->y = from.y + step * (to.y - from.y) / 10;
    orb->z = from.z + step * (to.z - from.z) / 10;
    scale = step * 0x10ccc / 10 + 0x4000; // from 0.25 to 1.3
    orb->scaleX = scale;
    orb->scaleY = scale;
    WaitFrames(1);
  }
  orb->scaleX = 0x1b333; // 1.7 wide
  orb->scaleY = 0x14ccc; // 1.3 high
  Audio_PlayCue(SE_ORB_ARRIVES);

  switch (kind) {
  case ORB_ORBITING:
    // Sixteen motes, one every six frames, 4 pixels above the orb and 0.8
    // higher each, 3 to 8 pixels out, circle it while the target shrinks.
    WaitFrames(20);
    if (target != 0)
      target->update = (void (*)(void))BattleEffect_ShrinkObjectScaleUntilHalf;
    for (step = 0; step <= 15; step++) {
      s32 distance;

      position.x = orb->x;
      position.y = orb->y + step * 0xcccc + (4 << 16);
      position.z = orb->z;
      distance = Random16() * 5 + (3 << 16);
      Vector_AddPolarOffset(distance, Random16(), &position);
      mote = (struct OrbitEffect *)Object_Spawn(MOTE_OBJECT, position.x,
                                                position.y, position.z);
      if (mote != 0) {
        mote->update = Func_080dfc3c;
        mote->anchor = orb;
        mote->timer = 0;
        mote->pause = 0;
        mote->angle = Random16();
      }
      WaitFrames(6);
    }
    WaitFrames(20);
    WaitFrames(120);
    // fall through
  case ORB_PLAIN:
    Object_SetMode(orb, 1);
    WaitFrames(30);
    Audio_PlayCue(SE_ORB_FADES);
    Func_08020340(orb);
    Object_Destroy(orb);
    break;

  case ORB_WANDER:
  case ORB_HANDOFF:
    WaitFrames(5);
    Object_SetMode(orb, 1);
    if (kind == ORB_WANDER) {
      // Sixteen motes that wander about the orb, the first pausing longest.
      s32 pause = 20;

      for (step = 0; step <= 15; step++) {
        s32 distance;

        position.x = orb->x;
        position.y = orb->y + step * 0xcccc + (4 << 16);
        position.z = orb->z;
        distance = Random16() * 5;
        distance += 3 << 16;
        Vector_AddPolarOffset(distance, Random16(), &position);
        mote = (struct OrbitEffect *)Object_Spawn(MOTE_OBJECT, position.x,
                                                  position.y, position.z);
        if (mote != 0) {
          mote->update = BattleEffect_WanderAroundAnchor;
          mote->timer = pause;
          mote->pause = 0;
          mote->speed = 0x1999;
          mote->scale = 3 << 16;
          mote->angle = Random16();
        }
        pause -= 2;
      }
    }
    BattleEffect_SetObjectTickMode(scene->mainObject, 0);
    BattleEffect_PrepareBufferInterpolation();
    ((struct OrbitEffect *)orb)->timer = 0;
    orb->update = (void (*)(void))BattleEffect_AgeAndCueAtEighty;
    Func_080db0b0(scene->secondObjectId);
    Func_08020340(orb);
    Object_Destroy(orb);
    break;
  }
  BattleEffect_PrepareBufferInterpolation();
}
