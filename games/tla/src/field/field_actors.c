/// @file
/// Animated map tiles, then actors: creating and releasing them, their
/// animations, and starting scripts.
// Part of field.c.

#include "field_internal.h"
#include "field/flag_call.h"
#include "flags.h"
#include "field/object.h"
#include "field/resource_slot.h"
#include "system/system.h"
#include "system/task.h"
#include "field/objdisp.h"
#include "field/actor.h"
#include "field/actor_view.h"
#include "system/ram_code.h"
#include "battle/battle_system.h"
#include "system/transform.h"
#include "macros.h"
#include "system/main_routine_work.h"
#include "field/field_sprite.h"
#include "graphics/render_input.h"
#include "fixed_math.h"
#include "field/map_grid.h"
#include "system/iwram_call.h"
#include "field/sprite.h"
#include "field/field.h"
#include "field/script_operands.h"

/// Waits, at most 90 frames, for a sprite's animation to change or for its
/// first layer's script to reach code 241.
void Animation_WaitForTrackEnd(struct Sprite *sprite) {
  u8 animation = sprite->lastNo;
  struct SpriteAnimation *entry = sprite->entries[0];
  s32 i;

  if (sprite != NULL) {
    i = 0;
    if (entry->script[entry->scriptOffset] != 241) {
      do {
        wait(1);
        i++;
        if (i > 89)
          break;
        if (entry->script[entry->scriptOffset] == 241)
          break;
      } while (sprite->lastNo == animation);
    }
  }
}

/// One step of dissolving a sprite pixel by pixel.
void SpriteDissolve_Step(u8 *obj, u32 pos) {
  u16 *tile;
  s32 tileCount;
  u32 i;
  const u8 *order;

  struct Sprite *sprite = (struct Sprite *)obj;

  tile = (u16 *)(OBJ_VRAM0 + gVramCacheEntries[sprite->slot].offset);
  tileCount = (sprite->width * sprite->height) / 64;
  i = 0;

  if (i < tileCount) {
    order = gSpriteDissolve_PixelOrder;
    {
      u32 highMask = 0xff00;
      u32 indexMask = 0x3f;

      // A tile is 64 bytes (32 halfwords) at 8 bits a pixel.
      for (; i < tileCount; i++, tile += 32, pos++) {
        if ((pos - 64) <= 63) {
          u8 pixel;
          u16 *pixelPair;
          u32 isOdd;

          // Each tile reads the order 17 entries on from the tile before, so
          // neighbouring tiles lose different pixels at the same step.
          pixel = order[(pos + i * 16) & indexMask];
          pixelPair = (u16 *)((u8 *)tile + (pixel & 0x3e));
          isOdd = 1;
          isOdd &= pixel;
          if (isOdd != 0)
            *pixelPair = *(u8 *)pixelPair;
          else
            *pixelPair &= highMask;
        }
      }
    }
  }
}

/// Dissolves a sprite pixel by pixel, four steps a frame, over 32 frames.
void SpriteDissolve_Run(u8 *sprite) {
  u32 step;

  step = 0;
  // Finish all four dissolve steps before waiting for the next video frame.
  do {
    SpriteDissolve_Step(sprite, step);
    SpriteDissolve_Step(sprite, step + 1);
    SpriteDissolve_Step(sprite, step + 2);
    SpriteDissolve_Step(sprite, step + 3);
    step += 4;
    wait(1);
  } while (step <= 127);
}

/// Dissolves count sprites together, pixel by pixel, over 32 frames.
void SpriteDissolve_RunMany(u8 **sprites, s32 count) {
  u8 **firstSprite;
  u8 **spriteCursor;
  s32 spritesRemaining;
  u32 step;
  u32 secondDissolveStep;
  u32 thirdDissolveStep;
  u32 fourthDissolveStep;

  firstSprite = sprites;
  step = 0;
  do {
    if (count > 0) {
      secondDissolveStep = step + 1;
      thirdDissolveStep = step + 2;
      spriteCursor = firstSprite;
      fourthDissolveStep = step + 3;
      spritesRemaining = count;
      do {
        SpriteDissolve_Step(*spriteCursor, step);
        SpriteDissolve_Step(*spriteCursor, secondDissolveStep);
        SpriteDissolve_Step(*spriteCursor, thirdDissolveStep);
        spritesRemaining -= 1;
        SpriteDissolve_Step(*spriteCursor++, fourthDissolveStep);
      } while (spritesRemaining != 0);
    }
    wait(1U);
    step += 4;
  } while (step <= 127U);
}

/// Writes the actor's screen position, in pixels, to screenPosition[0..1] and
/// returns 0 when it is on or near the screen; otherwise writes 0, 0 and
/// returns -1.
s32 MapGrid_GetScreenRelativePosition(struct Actor *actor,
                                      s32 *screenPosition) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 *view = &map->viewX;
  s32 viewX;
  s32 viewZ;
  s32 x;
  s32 y;

  viewX = view[0] & FIELD_PIXEL_MASK;
  viewZ = view[1] & FIELD_PIXEL_MASK;
  x = actor->x.fixed - viewX;
  y = actor->z.fixed - viewZ;
  // Across, from 32 pixels left of the screen to 32 past its 240; down,
  // below the view's top and above 224.
  if ((x >= -(px(32) - 1) && x <= px(272) - 1) && y > 0 && y < px(224)) {
    *screenPosition++ = x >> 16;
    *screenPosition = y >> 16;
    return 0;
  }
  *screenPosition++ = 0;
  *screenPosition = 0;
  return -1;
}

/// Sets up the actor system: allocates and zeroes the 64 actors and the
/// system's work, sets the outline's colours and registers the per-frame
/// updates. The battle's mode, 4, moves the actors with Actor_UpdateAllMotion;
/// modes 3 and 4 place their sprites with the fixed camera.
void ActorSystem_Initialize(s32 mode) {
  struct ObjectSystemWork *system;
  struct Actor *pool;
  // The fill word Dma_Fill32Cell reads its zero from, one for both fills.
  volatile u32 zero;

  system = HeapSlot_AllocEwram(HEAP_SLOT_ACTOR_SYSTEM, sizeof(*system));
  pool = HeapSlot_AllocEwram(HEAP_SLOT_ACTORS, 64 * sizeof(*pool));
  ActorSystem_Configure(mode);
  Dma_Fill32Cell(zero, 0, pool, 64 * sizeof(*pool));
  Dma_Fill32Cell(zero, 0, system, sizeof(*system));
  if (mode == 4) {
    Task_Add(Actor_UpdateAllMotion, TASK_PASS_VBLANK + 10);
  } else {
    Task_Add(Actor_UpdateAllThumb, TASK_PASS_VBLANK + 11);
    Task_Add(Actor_MoveWithCarriers, TASK_PASS_VBLANK + 10);
  }
  if (mode == 3 || mode == 4)
    Task_Add(ActorSystem_ProjectSprites, TASK_PASS_VBLANK);
  else
    Task_Add(ActorSystem_PlaceSprites, TASK_PASS_VBLANK);
  system->edge = 15;
  system->fill = 0;
}

UNUSED void ActorSystem_ReservedNoOpA(void) {}

UNUSED void ActorSystem_ReservedNoOpB(void) {}

// Objects: create and release field objects, and render them each frame.

/// Returns an unused object (one without a script) from the 64 slots, or 0
/// if none is free.
void *Actor_FindFree(void) {
  struct Actor *obj = Ram_HeapSlots->actors;
  void *ret = 0;
  s32 n = 0;

  while (n <= 63) {
    if (obj->script == 0) {
      ret = obj;
      break;
    }
    n++;
    obj++;
  }
  return ret;
}

/// Releases an actor's sprites and zeroes it, releasing its slot. Clearing the
/// script lets Actor_FindFree hand the actor out again.
void Actor_Release(struct Actor *actor) {
  s32 spritesRemaining;
  void **sprites;

  if (actor) {
    switch (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) {
    case ACTOR_SPRITE_ONE:
      Sprite_Destroy(actor->sprite);
      break;
    case ACTOR_SPRITE_GROUP:
      sprites = (void **)actor->sprite;
      spritesRemaining = 3;
      do {
        struct Sprite *sprite = *sprites++;
        if (sprite)
          Sprite_Destroy(sprite);
      } while (--spritesRemaining >= 0);
      break;
    }
    Dma_Fill32(0, actor, sizeof(struct Actor));
  }
}

/// Creates an actor at (x, y, z), in 16.16 pixels, or returns 0 when every slot
/// is in use. id / 4096 is the kind of actor and the low 12 bits the sprite's
/// animation id.
struct Actor *Actor_Create(s32 id, s32 x, s32 y, s32 z) {
  struct Actor *object;
  void *sprite;
  s32 kind;
  void **entry;
  volatile u32 zero;

  kind = id / 4096;
  id &= 0xfff;
  object = Actor_FindFree();
  if (object != NULL) {
    object->radius = 16;
    switch (kind) {
    case 0:
      sprite = Sprite_Create(id);
      if (sprite != NULL) {
        object->spriteStorageKind = 1;
        object->sprite = sprite;
        object->radius = Resource_GetMetadataRecordFar(id)->boxY >> 1;
      } else {
        object->spriteStorageKind = 0;
      }
      break;
    case ACTOR_CREATE_GROUP:
      entry = (void **)((u32 *)Ram_HeapSlots->actorSystem +
                        Ram_HeapSlots->actorSystem->count++ + 2);
      object->spriteStorageKind = kind;
      object->sprite = entry;
      zero = 0;
      Dma_Set(&zero, entry, 0x85000004, REG_DMA3);
      sprite = Sprite_Create(id);
      if (sprite != NULL) {
        object->radius = Resource_GetMetadataRecordFar(id)->boxY >> 1;
        *entry = sprite;
        entry++;
      }
      sprite = Sprite_Create(id + 1);
      if (sprite != NULL)
        *entry = sprite;
      break;
    }
  }
  if (object != NULL) {
    Actor_SetPositionAndResetMotion(object, x, y, z);
    object->script = (s32 *)gActorScript_EndScript;
    object->speedLimit = px(2);
    object->scriptCursor = 0;
    object->scaleX = Q16_ONE;
    object->scaleY = Q16_ONE;
    object->acceleration = px(1);
    object->motionFlags = 3;
    object->gravity = px(1);
    object->bounce = 0x4000;
    object->collisionFlags = 0;
    object->steeringFlags = 1;
    object->facing = TRIG_TURN / 4;
    object->riseBaseY = 0;
    object->homeX = x / px(1);
    object->homeZ = z / px(1);
    *(s32 *)&object->unk70[12] = 0;
    *(s32 *)&object->unk70[0] = 0;
    *(s32 *)&object->unk70[4] = 0;
    *(s32 *)&object->unk70[8] = 0;
  }
  return object;
}

/// Gives an actor a script and starts it from the top. Sets the script and
/// clears the cursor, the pause, the loop count and the last flag test.
void Actor_StartScript(struct Actor *actor, const s32 *script) {
  if (actor != 0) {
    actor->scriptCursor = 0;
    actor->script = script;
    actor->paused = 0;
    actor->repeatCount = 0;
    actor->conditionResult = 0;
  }
}

/// Starts animation argument on an actor's sprite, or on each of the four
/// sprites of a group (Sprite_StartAnimation).
void Actor_SelectAnimation(struct Actor *object, s32 argument) {
  struct Sprite **items;
  struct Sprite *item;
  s32 count;

  if (object != 0) {
    switch (object->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) {
    case ACTOR_SPRITE_ONE:
      Sprite_StartAnimation(((struct Sprite *)object->sprite), argument);
      break;
    case ACTOR_SPRITE_GROUP:
      items = ((struct Sprite **)object->sprite);
      count = 3;
      do {
        item = *items++;
        if (item != 0)
          Sprite_StartAnimation(item, argument);
        count--;
      } while (count >= 0);
      break;
    }
  }
}

/// Sets the timer step, the animation speed, of an actor's sprite, or of each
/// sprite of a group (16 is normal speed).
void Actor_SetAnimationSpeed(struct Actor *object, s32 value) {
  s32 count;
  struct Sprite *child;
  struct Sprite **children;

  if (object != 0) {
    switch (object->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) {
    case ACTOR_SPRITE_ONE:
      Sprite_SetAnimationSpeed(((struct Sprite *)object->sprite), value);
      return;
    case ACTOR_SPRITE_GROUP:
      children = ((struct Sprite **)object->sprite);
      count = 3;
      do {
        child = *children++;
        if (child != 0)
          Sprite_SetAnimationSpeed(child, value);
        count--;
      } while (count >= 0);
      break;
    }
  }
}

/// Both of the above: switches all of an actor's sprites to animation and
/// sets their timer step to step.
void Actor_SetAnimationAndSpeed(struct Actor *actor, s32 animation, s32 step) {
  void **sprites;
  void *sprite;
  s32 part;

  if (actor != 0) {
    switch (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) {
    case ACTOR_SPRITE_ONE:
      Sprite_StartAnimation(actor->sprite, animation);
      Sprite_SetAnimationSpeed(actor->sprite, step);
      break;
    case ACTOR_SPRITE_GROUP:
      sprites = (void **)actor->sprite;
      for (part = 0; part < 4; part++) {
        sprite = *sprites++;
        if (sprite != 0) {
          Sprite_StartAnimation(sprite, animation);
          Sprite_SetAnimationSpeed(sprite, step);
        }
      }
      break;
    }
  }
}

/// Sets the rotation of an actor's sprite, when it has just one.
void Actor_SetSpriteRotation(struct Actor *object, u32 value) {
  if (object != 0 &&
      (object->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
    ((struct Sprite *)object->sprite)->rotation = value;
}

/// Gives a one-sprite actor's first sprite animation animation id animationId
/// and restarts its animations. A negative animationId does nothing.
void Actor_SetFirstSpriteAnimation(struct Actor *object, s32 index) {
  struct Sprite *child;

  if (object != NULL && (object->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) ==
                            ACTOR_SPRITE_ONE) {
    child = ((struct Sprite *)object->sprite);
    if (index >= 0) {
      child->entries[0]->animationId = index;
      Sprite_ResetAnimations(child);
    }
  }
}

/// Adds a sprite animation of animation id animationId to a one-sprite actor's
/// sprite. A negative animationId does nothing.
void Actor_RegisterSpriteMetadata(struct Actor *actor, s32 animationId) {
  if (actor != 0 &&
      (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
    struct Sprite *sprite = actor->sprite;
    if (animationId >= 0)
      Sprite_AddAnimation(sprite, animationId);
  }
}

/// Starts an actor on the script that approaches its watched actor
/// (SC_APPROACH_WATCHED), watching watchedActorHandle.
void Actor_StartApproachWatched(struct Actor *actor, s32 watchedActorHandle) {
  Actor_StartScript(actor, gActor_ApproachWatchedScript);
  actor->watchedActor = (struct Actor *)watchedActorHandle;
}

/// Starts the dispatch script for gMainRoutineWork's render mode, waiting a
/// frame first in mode 9.
void Actor_StartRenderModeScript(struct Actor *object) {
  const s32 *script;
  struct MainRoutineWork *party = &gMainRoutineWork;

  switch (party->movementMode) {
  case 0:
    script = gActor_PlayerScript;
    break;
  case 1:
    script = gActor_WorldMapScript;
    break;
  case 2:
    script = gActor_KeysScript;
    break;
  case 3:
    script = gActor_RopeScript;
    break;
  case 4:
    script = gActor_TightropeScript;
    break;
  case 5:
    script = gActor_SandScript;
    break;
  case 6:
    script = gActor_SailOnMapScript;
    break;
  case 7:
    script = gActor_SailScript;
    break;
  case 8:
    script = gActor_FlyScript;
    break;
  case 9:
    wait(1);
    script = gActor_SandOnWorldMapScript;
    break;
  case 10:
    script = gActor_HoverScript;
    break;
  case 11:
    script = gActor_WalkingScript;
    break;
  case 12:
    script = gActorScript_EndScript;
    break;
  default:
    script = gActor_KeyCheckScript;
    break;
  }
  Actor_StartScript(object, script);
}

/// Starts an actor on the idle script (a lone SC_END).
void Actor_StartIdle(struct Actor *object) {
  Actor_StartScript(object, gActorScript_EndScript);
}

/// Starts an actor on the camera script (SC_CAMERA). With a leader to follow,
/// it also gets a speed limit of 4 pixels a frame and an acceleration of 0.5.
void Actor_StartCamera(struct Actor *actor, s32 watchedActorHandle) {
  Actor_StartScript(actor, gActor_CameraScript);
  if (watchedActorHandle != 0) {
    actor->acceleration = px(1) / 2;
    actor->speedLimit = px(4);
    actor->watchedActor = (struct Actor *)watchedActorHandle;
    actor->timer = 0;
  }
}

/// Waits until the actor's script reaches SC_END, at most 600 frames; it
/// returns as soon as the script idles, and after the limit whether or not it
/// has.
void Actor_WaitForScriptEnd(struct Actor *object) {
  s32 cnt = 0;

  if (object->script != 0 &&
      ((const s32 *)object->script)[object->scriptCursor] != 17) {
    do {
      wait(1);
      cnt++;
      if (cnt > 599)
        break;
    } while (object->script != 0 &&
             ((const s32 *)object->script)[object->scriptCursor] != 17);
  }
}

/// Sets the flags of a one-sprite actor's sprite (bit 0: draw its shadow).
void Actor_SetSpriteFlags(struct Actor *object, u32 value) {
  if (object != NULL &&
      (object->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
    ((struct Sprite *)object->sprite)->flags = value;
}

/// Sets the OAM blend mode of a one-sprite actor's sprite.
void Actor_SetBlendMode(struct Actor *actor, u32 mode) {
  if (actor != 0 && actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
    struct Sprite *sprite = actor->sprite;
    sprite->image.objectMode = mode;
  }
}

/// Sets a one-sprite actor's sprite's own-scale bit: drawn at its own scale,
/// not scaled by distance.
void Animation_SetDisplayFlag(struct Actor *object, u32 value) {
  if (object != NULL && object->spriteStorageKind == ACTOR_SPRITE_ONE)
    ((struct Sprite *)object->sprite)->useOwnScale = value;
}

/// Sets the colour mode (SpriteColorMode) of a one-sprite actor's sprite
/// animations, except those drawn hidden.
void Actor_SetSpriteColorMode(struct Actor *object, u32 value) {
  if (object != NULL && object->spriteStorageKind == ACTOR_SPRITE_ONE)
    Animation_ApplyChildValuesToRecord(((struct Sprite *)object->sprite),
                                       value);
}

/// Fixes the priority of a one-sprite actor's sprite and its shadow, which
/// then no longer follow its depth (clears priorityFlags bit 0).
void Actor_SetSpritePriorityDirect(struct Actor *object, s32 priority) {
  struct Sprite *sprite;

  if (object == NULL || object->spriteStorageKind != ACTOR_SPRITE_ONE) {
    return;
  }
  sprite = object->sprite;
  sprite->image.priority = priority;
  sprite->shadow.priority = priority;
  object->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
}

/* VIEW(resource_slot): tag struct IconSprite as struct
 * IconSprite__resource_slot; item_icon.h has another definition; tried with the
 * definition the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/field_actors.c:613: dereferencing pointer to incomplete
 * type. */
struct IconSprite__resource_slot {
  u8 unused[4]; ///< no code names it
  struct RenderOam oam;
  s32 scale;
  u8 vramSlot;
  u8 unusedAfterVramSlot[3]; ///< no code names it
  u8 width;
  u8 height;
  u8 unk16;
  s8 unk17;
  u8 unk18;
  u8 unk19;
  u8 unk1a;
  u8 unk1b;
};

/// Shows icon id on its owner's 16 by 16 sprite: Icon_DrawItemFar prepares the
/// icon in heap slot 68, whose tiles at +0x400 go to the sprite's VRAM cache
/// slot, and the sprite is reset to scale 1.0, 16-colour palette 0.
void Actor_SetItemIcon(struct IconOwner *owner, s32 id) {
  struct IconSprite__resource_slot *sprite;
  u8 *buffer;
  s32 tile;

  if (owner == 0)
    return;
  owner->unk5c = 1;
  owner->iconId = id;
  sprite = owner->sprite;
  if (sprite == 0)
    return;
  buffer = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
  Icon_DrawItemFar(id);
  tile = VramCache_Load(sprite->vramSlot, 128, buffer + 0x400);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  sprite->unk1b = 0;
  sprite->unk19 = 0;
  sprite->width = 16;
  sprite->height = 16;
  sprite->unk16 = 0;
  sprite->unk17 = 4;
  sprite->scale = Q16_ONE;
  sprite->oam.index = tile;
  sprite->oam.fullColor = 0;
  sprite->oam.palette = 0;
}

/// The sprite id an actor is drawn with (the first halfword of its sprite's
/// definition), or 0 for none.
s32 Actor_GetSpriteId(u8 *object) {
  u8 *track = *(u8 **)(object + 0x50);

  if (object != NULL && track != NULL && track != (u8 *)-0x28)
    return **(s16 **)(track + 0x28);
  return 0;
}

/// Animation_WaitForTrackEnd on an actor's sprite.
void Actor_WaitForTrackEnd(u8 *object) {
  if (object != NULL)
    Animation_WaitForTrackEnd(*(void **)(object + 0x50));
}

/// Suspends the actor camera updates, starts the palette back to normal and, a
/// frame later, shows the sprites with BG1-3 hidden, leaving BG0 unchanged.
void ActorSystem_SuspendProjectionAndShowSprites(void) {
  u16 dispcnt;

  Task_SuspendAll(ActorSystem_PlaceSprites);
  Task_SuspendAll(ActorSystem_ProjectSprites);
  PaletteFade_SetTargetFar(0x10000, 1);
  PaletteFade_StartInterpolationFar(1);
  wait(1);
  dispcnt = REG_DISPCNT;
  REG_DISPCNT = (dispcnt & 0xf1ff) | DISPCNT_OBJ_ON;
}

/// Resumes the actor camera updates and hides BG1-3 and the sprites.
void ActorSystem_ResumeProjectionAndHideLayers(void) {
  Task_ResumeAll(ActorSystem_PlaceSprites);
  Task_ResumeAll(ActorSystem_ProjectSprites);
  REG_DISPCNT &=
      ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON);
}

/// Unused: returns 1.
UNUSED s32 Actor_ReturnTrue(void) { return 1; }

#define BASE_PRIORITY(map) (((struct MapPriorityWork *)(map))->basePriority)
// The priority field (bits 15..14) of a cell's word.
#define CELL_PRIORITY(cells, x, z)                                             \
  (((cells)[(FIELD_CELL(z) << 7) + FIELD_CELL(x)] << 16) >> 30)
// The layer field (bits 13..12) of a cell's word: the layer plus one.
#define CELL_LAYER(cells, x, z)                                                \
  (((cells)[(FIELD_CELL(z) << 7) + FIELD_CELL(x)] << 18) >> 30)

/// Each frame, with the scrolling camera: places every actor's sprite on the
/// screen.
void ActorSystem_PlaceSprites(void) {
  // Unused: gives the stack frame its 36 bytes, as The Broken Seal's.
  struct ActorCameraPoint unusedPoints[3];
  struct SpritePlacement point;
  struct SpriteScale scale;
  s32 objectIndex;
  struct MapWork *map;
  s32 *view;
  s32 viewX;
  s32 viewZ;
  struct ObjectSystemWork *system;
  struct Actor *object;
  u32 kind;
  s32 dx;
  s32 dz;
  s32 originY;
  struct PlacedSprite *sprite;
  s32 screenTop;
  u8 *priorityFlags;
  s32 layer;
  u8 *terrainId;
  u32 *cells;
  s32 feetZ;
  s32 objectHeight;
  s32 probeX;
  s32 priority;
  s32 leftPriority;
  s32 middlePriority;
  s32 rightPriority;
  s32 height;
  s32 rise;
  u32 layerPlusOne;
  s8 *basePriority;

  map = Ram_HeapSlots->mapGrid;
  view = &map->viewX;
  viewX = view[0] & FIELD_PIXEL_MASK;
  viewZ = view[1] & FIELD_PIXEL_MASK;
  system = Ram_HeapSlots->actorSystem;
  originY = map->origin[1];
  Render_LoadDecoder();
  {
    // The zero is set before the objects are read.
    u32 zero = 0;

    object = Ram_HeapSlots->actors;
    system->used = zero;
  }
  for (objectIndex = 0; objectIndex < 64; objectIndex++, object++) {
    if (object->script == NULL)
      continue;
    if (object->x.fixed != 0 || object->z.fixed != 0) {
      kind = object->spriteStorageKind & ACTOR_SPRITE_KIND_MASK;
      if (kind == 0)
        continue;
      if (kind != 1)
        continue;
      if ((u16)system->suspended != 0 && !(object->retainSpriteCache & 1)) {
        struct PlacedSprite *frozen = object->sprite;

        VramCache_ReleaseVram(frozen->slot);
        frozen->dirty = kind;
        continue;
      }
      sprite = object->sprite;
      dx = object->x.fixed - viewX;
      dz = object->z.fixed - viewZ;
      screenTop = dz - object->y.fixed;
      if (dx > -px(32) && dx < 0x1100000 && screenTop > -px(32) &&
          screenTop < 0xe00000 &&
          (priorityFlags = &object->priorityFlags,
           !(*priorityFlags & ACTOR_PRIORITY_NOT_DRAWN))) {
        terrainId = &object->layer;
        layer = *terrainId & 3;
        cells = (u32 *)map->layers[layer].cells;
        if (*priorityFlags & ACTOR_PRIORITY_AUTOMATIC) {
          probeX = object->x.fixed - px(7);
          feetZ = object->z.fixed + px(2);
          objectHeight = FIELD_PIXEL(object->groundHeight);
          priority = 0;
          leftPriority = 0;
          middlePriority = 0;
          if (!(*priorityFlags & ACTOR_PRIORITY_IGNORES_RISES)) {
            /* FAKEMATCH(temporary): the base priority read through a
             * function-scope pointer. */
            basePriority = &BASE_PRIORITY(map);
            priority = *basePriority;
            if (MapGrid_GetTerrainHeight(layer, probeX, feetZ) ==
                object->groundHeight) {
              leftPriority = CELL_PRIORITY(cells, probeX, feetZ);
              height =
                  MapGrid_GetTerrainHeight(layer, probeX, feetZ + px(16)) >> 16;
              rise = (MapGrid_GetTerrainHeight(layer, probeX, feetZ + px(32)) >>
                      16) -
                     16;
              if (rise > height)
                height = rise;
              if (height > objectHeight)
                priority = BASE_PRIORITY(map) + 1;
            }
            probeX += px(13);
            if (MapGrid_GetTerrainHeight(layer, probeX, feetZ) ==
                object->groundHeight) {
              middlePriority = CELL_PRIORITY(cells, probeX, feetZ);
              height =
                  MapGrid_GetTerrainHeight(layer, probeX, feetZ + px(16)) >> 16;
              rise = (MapGrid_GetTerrainHeight(layer, probeX, feetZ + px(32)) >>
                      16) -
                     16;
              if (rise > height)
                height = rise;
              if (height > objectHeight)
                priority = BASE_PRIORITY(map) + 1;
            }
            probeX -= px(6);
            rightPriority = CELL_PRIORITY(cells, probeX, feetZ);
            height =
                MapGrid_GetTerrainHeight(layer, probeX, feetZ + px(16)) >> 16;
            rise = (MapGrid_GetTerrainHeight(layer, probeX, feetZ + px(32)) >>
                    16) -
                   16;
            if (rise > height)
              height = rise;
            if (height > objectHeight)
              priority = BASE_PRIORITY(map) + 1;
          } else {
            if (object->groundHeight ==
                MapGrid_GetTerrainHeight(layer, probeX, feetZ))
              leftPriority = CELL_PRIORITY(cells, probeX, feetZ);
            probeX += px(13);
            if (object->groundHeight ==
                MapGrid_GetTerrainHeight(layer, probeX, feetZ))
              middlePriority = CELL_PRIORITY(cells, probeX, feetZ);
            probeX -= px(6);
            rightPriority = CELL_PRIORITY(cells, probeX, feetZ);
          }
          // An odd priority is preferred, left first.
          if (leftPriority != 0 || middlePriority != 0 || rightPriority != 0) {
            priority = leftPriority;
            if (!(priority & 1)) {
              if (middlePriority != 0)
                priority = middlePriority;
              if (!(priority & 1) && rightPriority != 0)
                priority = rightPriority;
            }
          }
          if (priority != 0 && object->spriteStorageKind == ACTOR_SPRITE_ONE) {
            struct PlacedSprite *placed = object->sprite;

            if ((u32)priority > 3)
              priority = 3;
            placed->priority = priority;
            placed->shadowPriority = priority;
          }
        }
        layerPlusOne = CELL_LAYER(cells, object->x.fixed, object->z.fixed);
        if (layerPlusOne != 0)
          *terrainId = layerPlusOne - 1;
        scale.x = Iwram_MulQ16(object->scaleX, sprite->scale);
        scale.y = Iwram_MulQ16(object->scaleY, sprite->scale);
        point.x = dx;
        point.height = object->y.fixed - originY;
        point.y = dz - originY;
        point.groundHeight = object->groundHeight - originY;
        if (*priorityFlags & ACTOR_PRIORITY_UNDERFOOT) {
          point.height += 0xfec00000;
          point.y += 0xfec00000;
          point.groundHeight += 0xfec00000;
        }
        if (*priorityFlags & ACTOR_PRIORITY_OVERHEAD) {
          point.height += px(320);
          point.y += px(320);
          point.groundHeight += px(320);
        }
        if (*priorityFlags & ACTOR_PRIORITY_OWN_DEPTH)
          Render_ApplyProjectedPlacementAtDepth(sprite, &point, scale.values,
                                                object->facing,
                                                ((u8 *)object)[0x63]);
        else
          Render_ApplyProjectedPlacement(sprite, SpritePlacement_Values(&point),
                                         scale.values, object->facing);
        continue;
      }
      if (object->retainSpriteCache & 2) {
        Sprite_ComposePicture(sprite, object->facing);
      } else if (!(object->retainSpriteCache & 1) &&
                 !(sprite->retainCacheEntry & 1)) {
        VramCache_ReleaseVram(sprite->slot);
        sprite->dirty = 1;
      }
    } else {
      // At (0, 0): not placed; only the sprite's VRAM is given up.
      kind = object->spriteStorageKind & ACTOR_SPRITE_KIND_MASK;
      if (kind == 1) {
        struct PlacedSprite *frozen = object->sprite;

        if (object->retainSpriteCache == 0 && !(frozen->retainCacheEntry & 1)) {
          VramCache_ReleaseVram(frozen->slot);
          frozen->dirty = kind;
        }
      }
    }
  }
  HeapSlot_Release(HEAP_SLOT_FRAME_SCRATCH);
}

// The fixed camera: each frame it places every object's sprites, facing them
// by the angle from the eye to the target, each object in a function of its
// own.

/// Unused: returns 1.
s32 ActorCamera_ReturnTrue(void) { return 1; }

/// Places an object's sprite (kind 1, unless flag 0x10 hides it) or each of its
/// group's four sprites (kind 2), turned by angle. While battle commands are
/// chosen (FLAG_CHOOSING_COMMANDS) the sprites are drawn at 1.0 scale.
void Actor_PlaceSprites(struct Actor *actor, s32 angle) {
  const s32 *scale = &actor->scaleX;
  s32 *point;
  void *sprite;
  void **sprites;
  s32 remaining;

  if (GameFlag_Get(FLAG_CHOOSING_COMMANDS))
    scale = gActorSprite_FullScale;
  switch (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) {
  case ACTOR_SPRITE_ONE:
    point = &actor->x.fixed;
    sprite = actor->sprite;
    if (!(actor->priorityFlags & ACTOR_PRIORITY_NOT_DRAWN))
      Render_PlaceProjectedSprite(sprite, point, scale, actor->facing + angle,
                                  actor->layer);
    break;
  case ACTOR_SPRITE_GROUP:
    sprites = actor->sprite;
    for (remaining = 3; remaining >= 0; remaining--) {
      sprite = *sprites++;
      if (sprite != NULL)
        Render_PlaceProjectedSprite(sprite, &actor->x.fixed, scale,
                                    actor->facing + angle, actor->layer);
    }
    break;
  case 0:
    break;
  }
}

/// Each frame, with the fixed camera: places every active object, from the
/// 64th down. While battle commands are chosen (FLAG_CHOOSING_COMMANDS) it uses
/// the fixed view matrix and turns the objects by an eighth of a turn less.
void ActorSystem_ProjectSprites(void) {
  struct ActorCameraFixed *camera = Ram_HeapSlots->cameraWork;
  struct ObjectSystemWork *system = Ram_HeapSlots->actorSystem;
  struct ActorCameraPoint *eye;
  struct ActorCameraPoint *target;
  struct Actor *actor;
  s32 angle;
  s32 i;
  // Unused: gives the stack frame its 12 bytes.
  struct ActorCameraPoint unused;

  Render_LoadDecoder();
  eye = &camera->eye;
  target = &camera->target;
  if (camera->followEye != NULL)
    eye = camera->followEye;
  if (camera->followPosition != NULL)
    target = camera->followPosition;
  angle = (s16)ArcTan2(FIELD_PIXEL(eye->x - target->x),
                       FIELD_PIXEL(eye->z - target->z));
  system->used = 0;
  if (GameFlag_Get(FLAG_CHOOSING_COMMANDS)) {
    Transform_Reset();
    angle += 0xffffe000;
    Iwram_TransformMatrixEntry(gCamera_FixedViewMatrix);
    Transform_ApplyLookAt((s32 *)eye, (s32 *)target);
  } else {
    Transform_RewindStack();
    Transform_SetLookAt((s32 *)eye, (s32 *)target);
  }
  actor = Ram_HeapSlots->actors;
  actor += 63;
  for (i = 63; i >= 0; i--, actor--) {
    // The upper half of a script's address in ROM is never zero.
    if (((u16 *)&actor->script)[1] != 0)
      Actor_PlaceSprites(actor, angle);
  }
  HeapSlot_Release(HEAP_SLOT_FRAME_SCRATCH);
}

/// SC_GOTO(script): goes on in script, from its first command, this frame.
s32 ActorScript_Goto(struct Actor *interpreter) {
  interpreter->script =
      (const s32 *)interpreter->script[interpreter->scriptCursor + 1];
  interpreter->scriptCursor = 0;
  return 1;
}

/// SC_HIDE_SPRITES: clears spriteStorageKind, hiding the sprites, and moves on
/// to the next command.
s32 ActorScript_HideSprites(struct Actor *interpreter) {
  interpreter->spriteStorageKind = 0;
  interpreter->scriptCursor++;
  return 1;
}

/// Shows one sprite (spriteStorageKind 1) and advances to the next command.
s32 ActorScript_ShowSprite(struct Actor *interpreter) {
  interpreter->spriteStorageKind = 1;
  interpreter->scriptCursor++;
  return 1;
}

/// Waits up to 600 frames for a field object's movement target to be unset.
void Actor_WaitForMove(void *fieldObject) {
  s32 waitedFrames;

  waitedFrames = 0;
  while (waitedFrames < 600 && Actor_IsTargetUnset(fieldObject) == 0) {
    wait(1);
    waitedFrames++;
  }
}
