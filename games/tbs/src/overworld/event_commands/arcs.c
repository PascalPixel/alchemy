/// @file
/// The swelling arcs over the caster.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// Per-frame update of arc A: over 32 frames it swells and shrinks again
/// (scale sin(frame / 64 turn)) while it rises a pixel a frame on the object
/// it follows, 9 pixels in front of it and up to 5 pixels further while
/// small.
void FieldEffect_UpdateScaledArcActorA(struct Actor *actor) {
  // A shared inline kernel with a constant inversion argument changes both
  // arc updates from 0x58 to 0x5c bytes in the forced native English build.
  struct Actor *arc = (struct Actor *)actor;
  struct Actor *anchor;
  s32 frame;
  s32 scale;

  anchor = arc->linkedActor;
  frame = ++arc->timer;
  if (frame > 31) {
    Actor_StartScriptFar(arc, gFieldEffect_CommonParticleScript);
    return;
  }
  scale = Trig_Sin(frame << 10);
  arc->scaleX = scale;
  arc->scaleY = scale;
  arc->x.fixed = anchor->x.fixed;
  arc->y.fixed += Q16_ONE;
  arc->z.fixed = anchor->z.fixed + (Q16_ONE - scale) * 5 + 0x90000;
}

/// Per-frame update of arc B: arc A upside down, 16 pixels in front of the
/// object and up to 5 pixels nearer while small.
void FieldEffect_UpdateScaledArcActorB(struct Actor *actor) {
  struct Actor *arc = (struct Actor *)actor;
  struct Actor *anchor;
  s32 frame;
  s32 scale;

  anchor = arc->linkedActor;
  frame = ++arc->timer;
  if (frame > 31) {
    Actor_StartScriptFar(arc, gFieldEffect_CommonParticleScript);
    return;
  }
  scale = Trig_Sin(frame << 10);
  arc->scaleX = scale;
  arc->scaleY = -scale;
  arc->x.fixed = anchor->x.fixed;
  arc->y.fixed += Q16_ONE;
  arc->z.fixed = anchor->z.fixed - (Q16_ONE - scale) * 5 + 0x100000;
}

/// A sprite (struct Sprite) as the arcs set it up: its image's three
/// OAM attributes and its bookkeeping; the notes give the Sprite
/// fields at the same offsets.
struct SpriteArcOamView {
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
  /// (effectWork->tileSlot), which it borrows, loaded.
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
/// FieldPsynergy_End waits for the last to end (at most 30
/// frames).
void FieldEffect_SpawnScaledArcActors(struct Actor *linked) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct Actor *source = effectWork->mainActor;
  struct Actor *arcs[2];
  struct Actor *arc;
  struct SpriteArcOamView *sprite;
  s32 arcIndex;

  for (arcIndex = 0; arcIndex <= 1; arcIndex++) {
    arc = Actor_CreateFar(SPRITE_GOLDEN_RINGS, linked->x.fixed, linked->y.fixed,
                          linked->z.fixed);
    arcs[arcIndex] = arc;
    if (arc == 0)
      continue;
    arc->groundHeight = linked->groundHeight;
    sprite = (struct SpriteArcOamView *)arc->sprite;
    arc->motionFlags = 0;
    arc->timer = 0;
    arc->linkedActor = linked;
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
  arcs[0]->update = FieldEffect_UpdateScaledArcActorB;
  arcs[0]->sprite->image.priority = 0;
  arcs[1]->update = FieldEffect_UpdateScaledArcActorA;
  arcs[1]->sprite->image.priority = source->sprite->image.priority;
  arcs[1]->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
}

/// Per-frame update: flashes an object between sprite param 7 and 0 every 2
/// frames and spawns the swelling arcs on it every 16. FieldPsynergy_StartCast
/// gives it to the caster as a cast starts with a flash (effectWork->visible).
void FieldEffect_FlickerActorAndTick(struct Actor *updated) {
  struct Actor *actor = (struct Actor *)updated;
  if ((gFrameCount & 2) != 0) {
    Anim_ApplyChildValuesFar(actor, SPRITE_COLOR_WHITE);
  } else {
    Anim_ApplyChildValuesFar(actor, SPRITE_COLOR_NORMAL);
  }
  if ((gFrameCount & 15) == 0) {
    FieldEffect_SpawnScaledArcActors(actor);
  }
}

/// Per-frame update: cycles an object through sprite params 0 to 5, one step
/// every 2 frames, and spawns the swelling arcs on it every 16.
/// FieldPsynergy_StartCast gives it to the caster in place of the flicker
/// while flag 0x140 is set.
void FieldEffect_CycleActorValueByCounter(struct Actor *updated) {
  struct Actor *actor = (struct Actor *)updated;
  if ((gFrameCount & 1) != 0) {
    s32 colorIndex = (gFrameCount >> 1) % 6;

    Anim_ApplyChildValuesFar(actor, colorIndex);
  }
  if ((gFrameCount & 15) == 0) {
    FieldEffect_SpawnScaledArcActors(actor);
  }
}
