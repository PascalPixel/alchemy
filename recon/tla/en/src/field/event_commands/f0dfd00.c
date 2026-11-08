#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "field_psynergy_work.h"
#include "actor.h"
#include "sprite.h"

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
  struct Actor *anchor;
  void (*update)(struct OrbitEffect *);
};

// ⚓️'s composite sprite, as far as this reads it.
struct OrbTargetSprite {
  u8 unk00[0x28];
  struct AnimationEntry *entries[4];
};

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void BattleEffect_InitializeSharedScene(void);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct EffectVector *p);
void BattleEffect_ShrinkObjectScaleUntilHalf(struct Actor *object);
void BattleEffect_CircleAnchor(struct OrbitEffect *mote);
void BattleEffect_WanderAroundAnchor(struct OrbitEffect *mote);
void BattleEffect_AgeAndCueAtEighty(struct Actor *object);
void BattleEffect_SetObjectUpdateMode(void *object, s32 mode);
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080db0b0(s32 objectId);
void Func_08020340(struct Actor *object);
void Actor_Destroy(struct Actor *object);

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
void FieldEffect_RunWhirlwind(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *caster = scene->mainObject;
  struct Actor *target = scene->child;
  struct OrbTargetSprite *sprite = target->animation;
  struct EffectVector position;
  struct EffectVector from;
  struct EffectVector to;
  struct Actor *orb;
  struct OrbitEffect *mote;
  s32 kind = ORB_PLAIN;
  s32 step;
  s32 i;
  s32 frames = 11;

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
  orb = Actor_Spawn(ORB_OBJECT, 0, 0, 0);
  if (orb == 0)
    return;

  BattleEffect_InitializeSharedScene();
  Actor_SetMode(orb, 2);
  step = 0;
  do {
    s32 scale;

    orb->x = from.x + (to.x - from.x) * step / 10;
    orb->y = from.y + (to.y - from.y) * step / 10;
    orb->z = from.z + (to.z - from.z) * step / 10;
    scale = step * 0x10ccc / 10 + 0x4000; // from 0.25 to 1.3
    orb->scaleX = scale;
    orb->scaleY = scale;
    wait(1);
    step++;
  } while (step < frames);
  orb->scaleX = 0x1b333; // 1.7 wide
  orb->scaleY = 0x14ccc; // 1.3 high
  Sound_PlayFar(SE_ORB_ARRIVES);

  if (kind == ORB_PLAIN) {
    Actor_SetMode(orb, 1);
    wait(30);
    Sound_PlayFar(SE_ORB_FADES);
    Func_08020340(orb);
    Actor_Destroy(orb);
  } else if (kind == ORB_ORBITING) {
    // Sixteen motes, one every six frames, 4 pixels above the orb and 0.8
    // higher each, 3 to 8 pixels out, circle it while the target shrinks.
    wait(20);
    if (target != 0)
      target->update = (void (*)(void))BattleEffect_ShrinkObjectScaleUntilHalf;
    for (i = 0; i <= 15; i++) {
      s32 distance;

      position.x = orb->x;
      position.y = orb->y + i * 0xcccc + (4 << 16);
      position.z = orb->z;
      distance = Random16() * 5 + (3 << 16);
      Vector_AddPolarOffset(distance, Random16(), &position);
      mote = (struct OrbitEffect *)Actor_Spawn(MOTE_OBJECT, position.x,
                                               position.y, position.z);
      if (mote != 0) {
        mote->update = BattleEffect_CircleAnchor;
        mote->anchor = orb;
        mote->timer = 0;
        mote->pause = 0;
        mote->angle = Random16();
      }
      wait(6);
    }
    wait(20);
    wait(120);
    Actor_SetMode(orb, 1);
    wait(30);
    Sound_PlayFar(SE_ORB_FADES);
    Func_08020340(orb);
    Actor_Destroy(orb);
  } else if (kind == ORB_WANDER || kind == ORB_HANDOFF) {
    wait(5);
    Actor_SetMode(orb, 1);
    if (kind == ORB_WANDER) {
      // Sixteen motes that wander about the orb, the first pausing longest.
      for (i = 0; i <= 15; i++) {
        s32 distance;

        position.x = orb->x;
        position.y = orb->y + i * 0xcccc + (4 << 16);
        position.z = orb->z;
        distance = Random16() * 5;
        distance += 3 << 16;
        Vector_AddPolarOffset(distance, Random16(), &position);
        mote = (struct OrbitEffect *)Actor_Spawn(MOTE_OBJECT, position.x,
                                                 position.y, position.z);
        if (mote != 0) {
          mote->update = BattleEffect_WanderAroundAnchor;
          mote->timer = 20 + i * -2;
          mote->pause = 0;
          mote->speed = 0x1999;
          mote->scale = 3 << 16;
          mote->angle = Random16();
        }
      }
    }
    BattleEffect_SetObjectUpdateMode(scene->mainObject, 0);
    BattleEffect_PrepareBufferInterpolation();
    ((struct OrbitEffect *)orb)->timer = 0;
    orb->update = (void (*)(void))BattleEffect_AgeAndCueAtEighty;
    Func_080db0b0(scene->secondObjectId);
    Func_08020340(orb);
    Actor_Destroy(orb);
  }
  BattleEffect_PrepareBufferInterpolation();
}
