// Part of psynergy_effects.c, compiled only through it.
// Retreat.

#include "../psynergy_effects_internal.h"

// The scene owns PSYNERGY_EFFECT_SLOT_COUNT particle slots.

// Per-frame update: spins by 0x1000 + timer * 0x50 a frame, speeding up
// (timer counts up) until timer * 0x50 reaches 0x1000.
void FieldFx_UpdateSpinAngle(union FieldObject *fieldObject) {
  struct FieldActor *obj = (struct FieldActor *)fieldObject;
  u32 step;

  step = obj->timer * 0x50;
  obj->facing = obj->facing + step + 0x1000;
  if (step < 0x1000)
    obj->timer = obj->timer + 1;
}

// Per-frame update: spins 1/8 turn a frame as it shrinks, rising a pixel a
// frame; released once its scale falls below 0x3000.
void BattleFx_UpdateDescendingObject(union FieldObject *fieldObject) {
  struct FieldActor *obj = (struct FieldActor *)fieldObject;
  u32 scale;

  obj->scaleY -= 0x280;
  scale = obj->scaleX - 0x280;
  obj->facing += 0x2000;
  obj->y.fixed += 1 << 16;
  obj->scaleX = scale;
  if ((s32)scale < 0x3000)
    obj->spriteStorageKind = 0;
}

// Effect slot callback: a particle flies 120 pixels from the main object's
// screen position (8 pixels lower) in a direction within 1/8 turn of up the
// screen. In state 1 it flies again on arrival; in state 2 it is freed.
void BattleFx_UpdateRadialLaunch(struct EffectSlot *slot) {
  struct EffectVector pos;
  struct FieldActor *main;
  s32 state;
  u32 spread;

  main = ((struct BattleFxScene *)gEffectWork)->mainObject;
  state = slot->state;

  if (state == 0) {
    pos.x = main->x.fixed;
    pos.y = main->y.fixed;
    pos.z = main->z.fixed;
    Camera_WorldToScreen(&pos);

    slot->x = pos.x;
    slot->z = pos.z + (8 << 16);
    slot->originZ = slot->z;
    slot->originX = slot->x;
    pos.z = slot->originZ;
    pos.x = slot->originX;

    spread = Random16();
    Vector_AddPolarOffset(
        120 << 16, ((spread << 13) >> 16) - ((Random16() << 13) >> 16) + 0xc000,
        &pos);

    slot->targetX = pos.x;
    slot->targetZ = pos.z;
    slot->acceleration = 5 << 16;
    slot->maxSpeed = 5 << 16;
    slot->limitTurnStep = state;
    slot->state++;

    if ((gFrameCount & 2) != 0)
      AudioCommand_PlayFar(SE_246);
  } else if (state == 1) {
    if (EffectSlot_HasPendingTarget(slot) == 0)
      slot->state--;
  } else if (state == 2) {
    if (EffectSlot_HasPendingTarget(slot) == 0)
      BattleFx_ClearOwnedSlot(slot);
  }
}

// Abilities: Retreat (field kind 15).
//
// Spins the main object and shrinks it while 24 particles fly out, one a frame.
void FieldFx_RunRetreat(void) {
  struct BattleFxScene *scene = gEffectWork;
  struct FieldActor *obj = scene->mainObject;
  struct EffectVector pos;
  struct EffectSlot *slot;
  u8 last;
  u32 i;

  FieldEvent_Begin();
  Camera_MoveWithinMapBounds(-1, -1, -1, 0);
  BattleEffect_InitializeSharedScene();
  WaitFrames(10);
  ObjectMotion_ArmCallback(scene->firstObjectId, 0x4000, 0);
  WaitFrames(30);
  obj->update = ObjectGroup_ApplyRandomChildValues;
  AudioCommand_PlayFar(SE_131);
  ObjectDispatch_ApplyArgumentToChildrenFar(obj, 28);
  WaitFrames(40);

  // Start spinning.
  AudioCommand_PlayFar(SE_220);
  Animation_ApplyChildValuesFar(obj, 0);
  ObjectDispatch_ApplyArgumentToChildrenFar(obj, 3);
  obj->update = FieldFx_UpdateSpinAngle;
  obj->timer = 0;
  WaitFrames(70);

  // Start shrinking, and send particles from where it is on screen.
  ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
  obj->motionFlags = 0;
  obj->update = BattleFx_UpdateDescendingObject;
  obj->targetX = ACTOR_NO_TARGET;
  pos.x = obj->x.fixed;
  pos.y = obj->y.fixed;
  pos.z = obj->z.fixed;
  Camera_WorldToScreen(&pos);
  i = 0;
  slot = scene->slots;
  do {
    s32 scale;
    EffectSlot_Initialize(slot, 0x11c, pos.x, pos.z);
    EffectSlot_SetCallback(slot, BattleFx_UpdateRadialLaunch);
    EffectSlot_SetObjectMode(slot, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(slot->object,
                                              (Random16() * 7) >> 16);
    scale = (Random16() >> 1) + 0x13333; // 1.2 to 1.7
    slot->scaleY = scale;
    slot->scaleX = scale;
    i++;
    WaitFrames(1);
    slot++;
  } while (i < PSYNERGY_EFFECT_SLOT_COUNT);
  WaitFrames(70);

  // The flying particles vanish once they arrive.
  i = 0;
  last = 2;
  do {
    if (scene->slots[i].active != 0)
      scene->slots[i].state = last;
    i++;
  } while (i < PSYNERGY_EFFECT_SLOT_COUNT);
  WaitFrames(40);
  BattleFx_PrepareBufferInterpolation();
  WaitFrames(10);
}

// Per-frame update of a falling arc: grows toward full size as it falls 2
// pixels a frame on the object it follows, 9 pixels in front of it and up
// to 5 pixels further while small; released 10 pixels above the ground (4
// on the clear screen).
void BattleFx_UpdateDescendingParticlePositiveArc(
    union FieldObject *fieldObject) {
  // One shared inline kernel with constant inversion grows the positive and
  // negative callbacks 0x74 -> 0x7c and 0x78 -> 0x7c in native English.
  struct FieldActor *particle = (struct FieldActor *)fieldObject;
  struct FieldActor *source;
  s32 y;
  s32 limit;
  s32 scale;

  limit = particle->terrainHeight + 0xA0000;
  source = particle->linkedObject;
  if (gGameState.map == (s32)&SceneId_Clear)
    limit = particle->terrainHeight + 0x40000;

  y = particle->y.fixed;
  if (y <= limit) {
    ObjectDispatch_ReleaseFar(particle);
    return;
  }

  scale = particle->scaleX + 0xC00;
  if (scale > 0x10000)
    scale = 0x10000;
  particle->scaleX = scale;
  particle->scaleY = scale;
  particle->x.fixed = source->x.fixed;
  particle->y.fixed = y - 0x20000;
  particle->z.fixed = source->z.fixed + ((0x10000 - scale) * 5) + 0x90000;
}

// As BattleFx_UpdateDescendingParticlePositiveArc for the arc drawn upside
// down, 16 pixels in front of the object and up to 5 pixels nearer.
void BattleFx_UpdateDescendingParticleNegativeArc(
    union FieldObject *fieldObject) {
  struct FieldActor *particle = (struct FieldActor *)fieldObject;
  struct FieldActor *source;
  s32 y;
  s32 limit;
  s32 scale;

  limit = particle->terrainHeight + 0xA0000;
  source = particle->linkedObject;
  if (gGameState.map == (s32)&SceneId_Clear)
    limit = particle->terrainHeight + 0x40000;

  y = particle->y.fixed;
  if (y <= limit) {
    ObjectDispatch_ReleaseFar(particle);
    return;
  }

  scale = particle->scaleX + 0xC00;
  if (scale > 0x10000)
    scale = 0x10000;
  particle->scaleX = scale;
  particle->scaleY = -scale;
  particle->x.fixed = source->x.fixed;
  particle->y.fixed = y - 0x20000;
  particle->z.fixed = source->z.fixed - ((0x10000 - scale) * 5) + 0x100000;
}

struct DescendingArcSprite {
  u8 padding00[5];
  u8 flags05_0 : 5;
  u8 flags05_5 : 1;
  u8 layer : 2;
  u8 padding06;
  u8 flags07_0 : 6;
  u8 size : 2;
  union {
    struct {
      u16 tile : 10;
      u16 flags08_10 : 6;
    } half;
    struct {
      u8 tileLow;
      u8 flags09_0 : 2;
      u8 variant : 2;
      u8 flags09_4 : 4;
    } byte;
  } attr;
  u8 padding0c[16];
  u8 resource;
  u8 active : 1;
  u8 flags1d1 : 7;
  u8 padding1e[8];
  u8 animation;
  u8 padding27;
  struct AnimationEntry *frame;
};

// Spawns a pair of falling arcs (one upside down) on arc.
void BattleFx_SpawnDescendingArcParticles(struct FieldActor *arc) {
  struct FieldActor *source = arc;
  struct BattleFxScene *scene = gEffectWork;
  struct FieldActor *leader = scene->mainObject;
  struct FieldActor *objects[2];
  struct FieldActor *object;
  struct DescendingArcSprite *sprite;
  s32 arcIndex;
  u8 zero;

  for (arcIndex = 0; arcIndex <= 1; arcIndex++) {
    object = Object_CreateFar(26, source->x.fixed, source->y.fixed + 0x400000,
                              source->z.fixed);
    objects[arcIndex] = object;
    if (object == NULL)
      continue;
    object->terrainHeight = source->terrainHeight;
    sprite = object->sprite;
    object->motionFlags = 0;
    object->timer = 0;
    zero = 0;
    object->linkedObject = source;
    object->scaleY = 0x6666;
    object->scaleX = 0x6666;
    if (sprite == NULL)
      continue;
    AnimationObjects_SelectAnimationFar(sprite, 0);
    sprite->animation = zero;
    Resource_ResetEntry(sprite->resource);
    sprite->resource = scene->arcTileSlot;
    sprite->active = 1;
    sprite->attr.half.tile = gVramBlockCache[sprite->resource].offset >> 5;
    sprite->flags05_5 = 0;
    sprite->layer = 1;
    sprite->size = 2;
    sprite->frame->frame = zero;
  }
  // BUG: failed allocations above leave a missing arc or sprite here.
  // Preserve the original partial-pair behavior until recovery is established.
  objects[0]->update = BattleFx_UpdateDescendingParticleNegativeArc;
  ((struct DescendingArcSprite *)objects[0]->sprite)->attr.byte.variant = 0;
  objects[1]->update = BattleFx_UpdateDescendingParticlePositiveArc;
  ((struct DescendingArcSprite *)objects[1]->sprite)->attr.byte.variant =
      ((struct DescendingArcSprite *)leader->sprite)->attr.byte.variant;
}
