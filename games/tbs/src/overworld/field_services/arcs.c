/// @file
/// The swelling arcs over the caster.
// Part of field_services.c.

#include "../field_services_internal.h"

/// Per-frame update of arc A: over 32 frames it swells and shrinks again
/// (scale sin(frame / 64 turn)) while it rises a pixel a frame on the object
/// it follows, 9 pixels in front of it and up to 5 pixels further while
/// small.
void FieldEffect_UpdateScaledArcObjectA(union FieldObject *fieldObject) {
  // A shared inline kernel with a constant inversion argument changes both
  // arc updates from 0x58 to 0x5c bytes in the forced native English build.
  struct ObjectEvent *arc = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *anchor;
  s32 frame;
  s32 scale;

  anchor = arc->linkedObject;
  frame = ++arc->timer;
  if (frame > 31) {
    ObjectDispatch_InitializeFar((struct DispatchObject *)arc,
                                 gFieldEffect_CommonParticleScript);
    return;
  }
  scale = Trig_Sin(frame << 10);
  arc->scaleX = scale;
  arc->scaleY = scale;
  arc->x.fixed = anchor->x.fixed;
  arc->y.fixed += 0x10000;
  arc->z.fixed = anchor->z.fixed + (0x10000 - scale) * 5 + 0x90000;
}

/// Per-frame update of arc B: arc A upside down, 16 pixels in front of the
/// object and up to 5 pixels nearer while small.
void FieldEffect_UpdateScaledArcObjectB(union FieldObject *fieldObject) {
  struct ObjectEvent *arc = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *anchor;
  s32 frame;
  s32 scale;

  anchor = arc->linkedObject;
  frame = ++arc->timer;
  if (frame > 31) {
    ObjectDispatch_InitializeFar((struct DispatchObject *)arc,
                                 gFieldEffect_CommonParticleScript);
    return;
  }
  scale = Trig_Sin(frame << 10);
  arc->scaleX = scale;
  arc->scaleY = -scale;
  arc->x.fixed = anchor->x.fixed;
  arc->y.fixed += 0x10000;
  arc->z.fixed = anchor->z.fixed - (0x10000 - scale) * 5 + 0x100000;
}

/// A sprite (struct Sprite) as the arcs set it up: its image's three
/// OAM attributes and its bookkeeping; the notes give the Sprite
/// fields at the same offsets.
struct ArcSprite {
  u8 pad00[4];
  u16 y : 8; ///< screen pixels
  u16 affine : 1;
  u16 doubleSize : 1;
  u16 mode : 2;
  u16 mosaic : 1;
  u16 color : 1; ///< 1 = 256 colours
  u16 shape : 2; ///< 1 wide
  u16 x : 9;     ///< screen pixels
  u16 unused : 3;
  u16 hflip : 1;
  u16 vflip : 1;
  u16 size : 2;
  u16 tile : 10; ///< first sprite tile in VRAM
  u16 priority : 2;
  u16 palette : 4;
  u8 pad0a[18];
  u8 slot; ///< VRAM cache entry holding its tiles
  /// retainCacheEntry: set, releasing the arc leaves the effect's spark tiles
  /// (scene->tileSlot), which it borrows, loaded.
  u8 active : 1;
  u8 displayFlags : 7;
  u8 pad1e[8];
  u8 flags; ///< bit 0 draws its shadow; the arcs clear it
  u8 pad27;
  struct SpriteAnim *child; ///< entries[0], its first layer
};

/// Spawns the two swelling arcs (object 26) on an object, A and B (B upside
/// down); they follow it for 32 frames, then free themselves. They draw
/// with the effect's spark tiles, so they need the effect's work; the
/// casting hooks below spawn them every 16 frames, and
/// FieldEffect_End waits for the last to end (at most 30
/// frames).
void FieldEffect_SpawnScaledArcObjects(struct ObjectEvent *linked) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct ObjectEvent *source = effectWork->mainObject;
  struct ObjectEvent *arcs[2];
  struct ObjectEvent *arc;
  struct ArcSprite *sprite;
  s32 arcIndex;

  for (arcIndex = 0; arcIndex <= 1; arcIndex++) {
    arc = (struct ObjectEvent *)Object_CreateFar(
        26, linked->x.fixed, linked->y.fixed, linked->z.fixed);
    arcs[arcIndex] = arc;
    if (arc == 0)
      continue;
    arc->groundHeight = linked->groundHeight;
    sprite = (struct ArcSprite *)arc->sprite;
    arc->motionFlags = 0;
    arc->timer = 0;
    arc->linkedObject = linked;
    arc->scaleX = arc->scaleY = 0x1999; // 0.1
    if (sprite == 0)
      continue;
    // A wide 16-colour sprite drawn from the effect's tiles.
    Sprite_StartAnimFar((struct Sprite *)sprite, 0);
    sprite->flags = 0;
    VramCache_FreeEntry(sprite->slot);
    sprite->slot = effectWork->tileSlot;
    sprite->active = 1;
    sprite->tile = (gVramCacheEntries[sprite->slot].offset >> 5) & 0x3ff;
    sprite->color = 0;
    sprite->shape = 1;
    sprite->size = 2;
    sprite->child->frame = 0;
  }
  // Arc B in front of everything, arc A at the caster's priority.
  // BUG: allocation failures above leave a missing arc or sprite here.
  // The original does not define a partial-pair recovery path.
  arcs[0]->update = FieldEffect_UpdateScaledArcObjectB;
  arcs[0]->sprite->image.priority = 0;
  arcs[1]->update = FieldEffect_UpdateScaledArcObjectA;
  arcs[1]->sprite->image.priority = source->sprite->image.priority;
  arcs[1]->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
}

/// Per-frame update: flashes an object between sprite param 7 and 0 every 2
/// frames and spawns the swelling arcs on it every 16. FieldEffect_StartCast
/// gives it to the caster as a cast starts with a flash (scene->visible).
void FieldEffect_FlickerObjectAndTick(union FieldObject *fieldObject) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  if ((gFrameCount & 2) != 0) {
    Anim_ApplyChildValuesFar((struct DispatchObject *)objectEvent,
                             SPRITE_COLOR_WHITE);
  } else {
    Anim_ApplyChildValuesFar((struct DispatchObject *)objectEvent,
                             SPRITE_COLOR_NORMAL);
  }
  if ((gFrameCount & 15) == 0) {
    FieldEffect_SpawnScaledArcObjects(objectEvent);
  }
}

/// Per-frame update: cycles an object through sprite params 0 to 5, one step
/// every 2 frames, and spawns the swelling arcs on it every 16.
/// FieldEffect_StartCast gives it to the caster in place of the flicker
/// while flag 0x140 is set.
void FieldEffect_CycleObjectValueByCounter(union FieldObject *fieldObject) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  if ((gFrameCount & 1) != 0) {
    s32 colorIndex = (gFrameCount >> 1) % 6;

    Anim_ApplyChildValuesFar((struct DispatchObject *)objectEvent, colorIndex);
  }
  if ((gFrameCount & 15) == 0) {
    FieldEffect_SpawnScaledArcObjects(objectEvent);
  }
}
