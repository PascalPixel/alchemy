/// @file
/// Retreat.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

// The scene owns PSYNERGY_EFFECT_SLOT_COUNT particle slots.

/// Per-frame update: spins by 0x1000 + timer * 0x50 a frame, speeding up
/// (timer counts up) until timer * 0x50 reaches 0x1000: from 1/16 turn a
/// frame to just over 1/8 in 52 frames, as FieldEffect_AdvanceAngleWithRamp.
void FieldEffect_UpdateSpinAngle(union FieldObject *fieldObject) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  u32 step;

  step = objectEvent->timer * 0x50;
  objectEvent->facing = objectEvent->facing + step + 0x1000;
  if (step < 0x1000)
    objectEvent->timer = objectEvent->timer + 1;
}

/// Per-frame update: spins 1/8 turn a frame as it shrinks by 0x280, rising a
/// pixel a frame; once its scale falls below 0x3000 (84 frames from full
/// size) its sprites are hidden (spriteStorageKind 0), but it is not
/// released. Retreat gives it to the caster.
void FieldEffect_UpdateDescendingObject(union FieldObject *fieldObject) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  u32 scale;

  objectEvent->scaleY -= 0x280;
  scale = objectEvent->scaleX - 0x280;
  objectEvent->facing += 0x2000;
  objectEvent->y.fixed += 1 << 16;
  objectEvent->scaleX = scale;
  if ((s32)scale < 0x3000)
    objectEvent->spriteStorageKind = OBJECT_SPRITE_NONE;
}

/// The states of a Retreat particle (FieldEffect_UpdateRadialLaunch).
enum RetreatParticleState {
  RETREAT_PARTICLE_AIM = 0,    ///< placed on the main object and aimed
  RETREAT_PARTICLE_FLYING = 1, ///< back to RETREAT_PARTICLE_AIM on arrival
  /// Set by FieldEffect_RunRetreat: freed on arrival.
  RETREAT_PARTICLE_LAST_FLIGHT = 2
};

/// Effect slot callback: a particle flies 120 pixels from the main object's
/// screen position (8 pixels lower) in a direction within 1/8 turn of up the
/// screen, at up to 5 pixels a frame, and starts again from the main object on
/// arrival until it takes its last flight.
void FieldEffect_UpdateRadialLaunch(struct EffectSlot *effect) {
  struct EffectVector pos;
  struct ObjectEvent *main;
  s32 state;
  u32 spread;

  main = ((struct FieldEffectWork *)gFieldEffectWork)->mainObject;
  state = effect->state;

  if (state == RETREAT_PARTICLE_AIM) {
    pos.x = main->x.fixed;
    pos.y = main->y.fixed;
    pos.z = main->z.fixed;
    Camera_WorldToScreen(&pos);

    effect->x = pos.x;
    effect->z = pos.z + (8 << 16);
    effect->originZ = effect->z;
    effect->originX = effect->x;
    pos.z = effect->originZ;
    pos.x = effect->originX;

    spread = Random16();
    Vector_AddPolarOffset(
        120 << 16, ((spread << 13) >> 16) - ((Random16() << 13) >> 16) + 0xc000,
        &pos);

    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = 5 << 16;
    effect->speedMax = 5 << 16;
    effect->turnStepLimited = state;
    effect->state++;

    if ((gFrameCount & 2) != 0)
      Sound_PlaySongFar(SE_246);
  } else if (state == RETREAT_PARTICLE_FLYING) {
    if (EffectSlot_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == RETREAT_PARTICLE_LAST_FLIGHT) {
    if (EffectSlot_HasPendingTarget(effect) == 0)
      FieldEffect_ClearOwnedSlot(effect);
  }
}

/// Spins the main object and shrinks it while 24 particles fly out, one a
/// frame.
///
/// Abilities: Retreat (FIELD_KIND_RETREAT).
///
/// The caster turns south, plays animation 28 with its colours cycling,
/// spins up for 70 frames (FieldEffect_UpdateSpinAngle), then rises and shrinks
/// out of sight (FieldEffect_UpdateDescendingObject) as the particles fly. It
/// is hidden, not released, and the effect opens a script (FieldScript_Start)
/// it does not end: the Retreat warp raised before it (WARP_RETREAT, by
/// Field_UsePsynergy or Field_UseItem) then leaves the map. Called by
/// FieldEffect_Run and FieldEffect_DispatchRequestKind.
void FieldEffect_RunRetreat(void) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct ObjectEvent *objectEvent = effectWork->mainObject;
  struct EffectVector pos;
  struct EffectSlot *effect;
  u8 last;
  u32 i;

  FieldScript_Start();
  Camera_MoveWithinMapBounds(-1, -1, -1, 0);
  FieldEffect_FadeToEffectColor();
  WaitFrames(10);
  ObjectMotion_ArmCallback(effectWork->firstObjectId, 0x4000, 0);
  WaitFrames(30);
  objectEvent->update = ObjectGroup_ApplyRandomChildValues;
  Sound_PlaySongFar(SE_131);
  ObjectDispatch_SelectAnimationFar(objectEvent, 28);
  WaitFrames(40);

  // Start spinning.
  Sound_PlaySongFar(SE_220);
  Animation_ApplyChildValuesFar((struct DispatchObject *)objectEvent,
                                SPRITE_COLOR_NORMAL);
  ObjectDispatch_SelectAnimationFar(objectEvent, 3);
  objectEvent->update = FieldEffect_UpdateSpinAngle;
  objectEvent->timer = 0;
  WaitFrames(70);

  // Start shrinking, and send particles from where it is on screen.
  ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)objectEvent,
                                        0);
  objectEvent->motionFlags = 0;
  objectEvent->update = FieldEffect_UpdateDescendingObject;
  objectEvent->targetX = OBJ_EVENT_NO_TARGET;
  pos.x = objectEvent->x.fixed;
  pos.y = objectEvent->y.fixed;
  pos.z = objectEvent->z.fixed;
  Camera_WorldToScreen(&pos);
  i = 0;
  effect = effectWork->effects;
  do {
    s32 scale;
    EffectSlot_Initialize(effect, SPRITE_PARTICLE, pos.x, pos.z);
    EffectSlot_SetCallback(effect, FieldEffect_UpdateRadialLaunch);
    EffectSlot_SetObjectMode(effect, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(effect->object,
                                              (Random16() * 7) >> 16);
    scale = (Random16() >> 1) + 0x13333; // 1.2 to 1.7
    effect->scaleY = scale;
    effect->scaleX = scale;
    i++;
    WaitFrames(1);
    effect++;
  } while (i < PSYNERGY_EFFECT_SLOT_COUNT);
  WaitFrames(70);

  // The flying particles vanish once they arrive.
  i = 0;
  last = RETREAT_PARTICLE_LAST_FLIGHT;
  do {
    if (effectWork->effects[i].active != 0)
      effectWork->effects[i].state = last;
    i++;
  } while (i < PSYNERGY_EFFECT_SLOT_COUNT);
  WaitFrames(40);
  FieldEffect_FadeFromEffectColor();
  WaitFrames(10);
}

/// Per-frame update of a falling arc: grows toward full size (0xc00 a frame)
/// as it falls 2 pixels a frame on the object it follows, 9 pixels in front
/// of it and up to 5 pixels further while small; released 10 pixels above
/// the ground (4 on the world map, whose map is MapId_MainMenu).
void FieldEffect_UpdateDescendingParticlePositiveArc(
    union FieldObject *fieldObject) {
  // One shared inline kernel with constant inversion grows the positive and
  // negative callbacks 0x74 -> 0x7c and 0x78 -> 0x7c in native English.
  struct ObjectEvent *particle = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *source;
  s32 y;
  s32 limit;
  s32 scale;

  limit = particle->terrainHeight + 0xA0000;
  source = particle->linkedObject;
  if (gGameState.layoutId == (s32)&MapId_MainMenu)
    limit = particle->terrainHeight + 0x40000;

  y = particle->y.fixed;
  if (y <= limit) {
    ObjectDispatch_ReleaseFar((struct DispatchObject *)particle);
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

/// As FieldEffect_UpdateDescendingParticlePositiveArc for the arc drawn upside
/// down, 16 pixels in front of the object and up to 5 pixels nearer.
void FieldEffect_UpdateDescendingParticleNegativeArc(
    union FieldObject *fieldObject) {
  struct ObjectEvent *particle = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *source;
  s32 y;
  s32 limit;
  s32 scale;

  limit = particle->terrainHeight + 0xA0000;
  source = particle->linkedObject;
  if (gGameState.layoutId == (s32)&MapId_MainMenu)
    limit = particle->terrainHeight + 0x40000;

  y = particle->y.fixed;
  if (y <= limit) {
    ObjectDispatch_ReleaseFar((struct DispatchObject *)particle);
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

/// A falling arc's sprite (struct AnimationObject), seen as the attribute
/// bits, resource slot and animation that Avoid's arcs set on it; the notes
/// give the AnimationObject fields, some under other names here.
struct DescendingArcSprite {
  u8 padding00[5];
  u8 flags05_0 : 5;
  u8 flags05_5 : 1; ///< image.fullColor: 0 for 16 colours
  u8 layer : 2;     ///< image.shape, despite the name: 1 wide
  u8 padding06;
  u8 flags07_0 : 6;
  u8 size : 2; ///< image.size
  union {
    struct {
      u16 tile : 10; ///< image.tile, the first sprite tile in VRAM
      u16 flags08_10 : 6;
    } half;
    struct {
      u8 tileLow;
      u8 flags09_0 : 2;
      u8 variant : 2; ///< image.priority, despite the name
      u8 flags09_4 : 4;
    } byte;
  } attr;
  u8 padding0c[16];
  u8 resource;   ///< slot: the VRAM cache entry holding its tiles
  u8 active : 1; ///< retainCacheEntry: the borrowed tiles stay loaded
  u8 flags1d1 : 7;
  u8 padding1e[8];
  u8 animation; ///< flags, despite the name: 0 drops the shadow
  u8 padding27;
  struct AnimationEntry *frame; ///< entries[0], its first layer
};

/// Spawns a pair of falling arcs (one upside down) on arc, 64 pixels above
/// it, at 0.4 of full size, drawn with the tiles in scene->arcTileSlot. The
/// upside-down arc draws in front of everything (priority 0), the other at
/// the caster's priority, as FieldEffect_SpawnScaledArcObjects does. Called by
/// FieldEffect_UpdatePairedArcSpawner, so arc is Avoid's caster.
void FieldEffect_SpawnDescendingArcParticles(struct ObjectEvent *arc) {
  struct ObjectEvent *source = arc;
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct ObjectEvent *leader = effectWork->mainObject;
  struct ObjectEvent *objectEvents[2];
  struct ObjectEvent *objectEvent;
  struct DescendingArcSprite *sprite;
  s32 arcIndex;
  u8 zero;

  for (arcIndex = 0; arcIndex <= 1; arcIndex++) {
    objectEvent = (struct ObjectEvent *)Object_CreateFar(
        26, source->x.fixed, source->y.fixed + 0x400000, source->z.fixed);
    objectEvents[arcIndex] = objectEvent;
    if (objectEvent == NULL)
      continue;
    objectEvent->terrainHeight = source->terrainHeight;
    sprite = (struct DescendingArcSprite *)objectEvent->sprite;
    objectEvent->motionFlags = 0;
    objectEvent->timer = 0;
    zero = 0;
    objectEvent->linkedObject = source;
    objectEvent->scaleY = 0x6666;
    objectEvent->scaleX = 0x6666;
    if (sprite == NULL)
      continue;
    AnimationObject_SelectAnimationFar((struct AnimationObject *)sprite, 0);
    sprite->animation = zero;
    Resource_ResetEntry(sprite->resource);
    sprite->resource = effectWork->arcTileSlot;
    sprite->active = 1;
    sprite->attr.half.tile = gVramBlockCache[sprite->resource].offset >> 5;
    sprite->flags05_5 = 0;
    sprite->layer = 1;
    sprite->size = 2;
    sprite->frame->frame = zero;
  }
  // BUG: failed allocations above leave a missing arc or sprite here.
  // Preserve the original partial-pair behavior until recovery is established.
  objectEvents[0]->update = FieldEffect_UpdateDescendingParticleNegativeArc;
  ((struct DescendingArcSprite *)objectEvents[0]->sprite)->attr.byte.variant =
      0;
  objectEvents[1]->update = FieldEffect_UpdateDescendingParticlePositiveArc;
  ((struct DescendingArcSprite *)objectEvents[1]->sprite)->attr.byte.variant =
      ((struct DescendingArcSprite *)leader->sprite)->attr.byte.variant;
}
