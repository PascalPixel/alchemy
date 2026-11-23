/// @file
/// Actors: a sprite dissolving pixel by pixel, the actor system, creating and
/// releasing actors, their animations and scripts, and placing their sprites
/// each frame under the scrolling or the fixed camera.
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
#include "field/actor_script.h"
#include "system/ram_code.h"
#include "battle/battle_system.h"
#include "system/matrix.h"
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
#include "field/field_sequence.h"
#include "graphics/glyph.h"

/// The frames Sprite_WaitForTrackEnd waits at most.
#define TRACK_END_WAIT_FRAMES 90

/// Waits, at most TRACK_END_WAIT_FRAMES, for a sprite's animation to change or
/// for its first layer's script to reach ANIMCMD_HOLD.
void Sprite_WaitForTrackEnd(struct Sprite *sprite) {
  u8 animation = sprite->lastNo;
  struct SpriteAnimation *entry = sprite->entries[0];
  s32 i;

  if (sprite != NULL) {
    i = 0;
    if (entry->script[entry->scriptOffset] != ANIMCMD_HOLD) {
      do {
        wait(1);
        i++;
        if (i > TRACK_END_WAIT_FRAMES - 1)
          break;
        if (entry->script[entry->scriptOffset] == ANIMCMD_HOLD)
          break;
      } while (sprite->lastNo == animation);
    }
  }
}

/// How much further along the pixel order each tile reads than the tile
/// before, besides the step it is ahead.
#define DISSOLVE_ORDER_STRIDE 16
/// Keeps a pixel pair's odd pixel, its high byte, clearing the even one.
#define PIXEL_PAIR_ODD_MASK 0xff00

/// One step of dissolving a sprite pixel by pixel.
void SpriteDissolve_Step(u8 *obj, u32 pos) {
  u16 *tile;
  s32 tileCount;
  u32 i;
  const u8 *order;

  struct Sprite *sprite = (struct Sprite *)obj;

  tile = (u16 *)(OBJ_VRAM0 + gVramCacheEntries[sprite->slot].offset);
  tileCount = (sprite->width * sprite->height) / DISSOLVE_TILE_PIXELS;
  i = 0;

  if (i < tileCount) {
    order = gSpriteDissolve_PixelOrder;
    {
      u32 highMask = PIXEL_PAIR_ODD_MASK;
      u32 indexMask = DISSOLVE_TILE_PIXELS - 1;

      for (; i < tileCount; i++, tile += TILE_SIZE_8BPP / 2, pos++) {
        if ((pos - DISSOLVE_FIRST_STEP) <= DISSOLVE_TILE_PIXELS - 1) {
          u8 pixel;
          u16 *pixelPair;
          u32 isOdd;

          // Each tile reads the order 17 entries on from the tile before, so
          // neighbouring tiles lose different pixels at the same step.
          pixel = order[(pos + i * DISSOLVE_ORDER_STRIDE) & indexMask];
          // The halfword holding the pixel: its offset rounded down to even.
          pixelPair =
              (u16 *)((u8 *)tile + (pixel & (DISSOLVE_TILE_PIXELS - 2)));
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
  } while (step <= DISSOLVE_STEPS - 1);
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
  for (step = 0; step <= DISSOLVE_STEPS - 1U; step += 4) {
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
  }
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
  FIXED x;
  FIXED y;

  viewX = view[0] & FIELD_PIXEL_MASK;
  viewZ = view[1] & FIELD_PIXEL_MASK;
  x = actor->x.fixed - viewX;
  y = actor->z.fixed - viewZ;
  // Across, from 32 pixels left of the screen to 32 past its 240; down,
  // below the view's top and above 224.
  if ((x >= -(toFIXED(32) - 1) && x <= toFIXED(272) - 1) && y > 0 &&
      y < toFIXED(224)) {
    *screenPosition++ = FIXEDtoINT(x);
    *screenPosition = FIXEDtoINT(y);
    return 0;
  }
  *screenPosition++ = 0;
  *screenPosition = 0;
  return -1;
}

/// The colour Grid_DrawOutlineArm draws an outlined sprite's edge in: the
/// basic sprite palette's white, which SPRITE_COLOR_WHITE paints with too.
#define OUTLINE_EDGE_COLOR 15

/// Sets up the actor system: allocates and zeroes the 64 actors and the
/// system's work, sets the outline's colours and registers the per-frame
/// updates (mode is an enum ActorSystemMode). A battle moves its actors with
/// Actor_UpdateAllMotion; the World Map and a battle place their sprites with
/// the fixed camera.
void ActorSystem_Initialize(s32 mode) {
  struct ActorSystemWork *system;
  struct Actor *pool;
  // The fill word Dma_Fill32Cell reads its zero from, one for both fills.
  volatile u32 zero;

  system = HeapSlot_AllocEwram(HEAP_SLOT_ACTOR_SYSTEM, sizeof(*system));
  pool = HeapSlot_AllocEwram(HEAP_SLOT_ACTORS, ACTOR_COUNT * sizeof(*pool));
  ActorSystem_Configure(mode);
  Dma_Fill32Cell(zero, 0, pool, ACTOR_COUNT * sizeof(*pool));
  Dma_Fill32Cell(zero, 0, system, sizeof(*system));
  if (mode == ACTOR_SYSTEM_BATTLE) {
    Task_Add(Actor_UpdateAllMotion, TASK_PASS_FRAME + 10);
  } else {
    Task_Add(Actor_UpdateAllThumb, TASK_PASS_FRAME + 11);
    Task_Add(Actor_MoveWithCarriers, TASK_PASS_FRAME + 10);
  }
  if (mode == ACTOR_SYSTEM_WORLD_MAP || mode == ACTOR_SYSTEM_BATTLE)
    Task_Add(ActorSystem_ProjectSprites, TASK_PASS_FRAME);
  else
    Task_Add(ActorSystem_PlaceSprites, TASK_PASS_FRAME);
  system->edge = OUTLINE_EDGE_COLOR;
  system->fill = 0;
}

/// Unused: an empty routine beside the actor system's, in both games.
UNUSED void ActorSystem_ReservedNoOpA(void) {}

/// Unused: a second empty routine.
UNUSED void ActorSystem_ReservedNoOpB(void) {}

// Creating and releasing actors, and what they show.

/// Returns an unused actor (one without a script) from the 64 slots, or 0
/// if none is free.
void *Actor_FindFree(void) {
  struct Actor *actor = ((struct Actor *)Ram_HeapSlots->actors);
  void *freeActor = 0;
  s32 actorIndex = 0;

  while (actorIndex <= ACTOR_COUNT - 1) {
    if (actor->script == 0) {
      freeActor = actor;
      break;
    }
    actorIndex++;
    actor++;
  }
  return freeActor;
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
      for (spritesRemaining = 0; spritesRemaining < 4; spritesRemaining++) {
        struct Sprite *sprite = *sprites++;
        if (sprite)
          Sprite_Destroy(sprite);
      }
      break;
    }
    Dma_Fill32(0, actor, sizeof(struct Actor));
  }
}

/// Creates an actor at (x, y, z), in 16.16 pixels, or returns 0 when every slot
/// is in use. encodedSprite / 4096 is the kind of actor (enum
/// ActorCreateKind) and its low 12 bits the sprite's animation id.
struct Actor *Actor_Create(s32 encodedSprite, FIXED x, FIXED y, FIXED z) {
  struct Actor *actor;
  void *sprite;
  s32 spriteStorageKind;
  void **spriteSlots;
  volatile u32 zero;

  spriteStorageKind = encodedSprite / (ACTOR_CREATE_SPRITE_MASK + 1);
  encodedSprite &= ACTOR_CREATE_SPRITE_MASK;
  actor = Actor_FindFree();
  if (actor != NULL) {
    actor->radius = 16;
    switch (spriteStorageKind) {
    case ACTOR_CREATE_ONE_SPRITE:
      sprite = Sprite_Create(encodedSprite);
      if (sprite != NULL) {
        actor->spriteStorageKind = ACTOR_SPRITE_ONE;
        actor->sprite = sprite;
        actor->radius = Resource_GetMetadataRecordFar(encodedSprite)->boxY >> 1;
      } else {
        actor->spriteStorageKind = ACTOR_SPRITE_NONE;
      }
      break;
    case ACTOR_CREATE_GROUP:
      // Take a zeroed list of four sprites, and fill it with id and id + 1.
      spriteSlots = (void **)((u32 *)Ram_HeapSlots->actorSystem +
                              Ram_HeapSlots->actorSystem->count++ + 2);
      actor->spriteStorageKind = spriteStorageKind;
      actor->sprite = spriteSlots;
      zero = 0;
      Dma_Set(&zero, spriteSlots, DMA_FILL_WORDS(4), REG_DMA3);
      sprite = Sprite_Create(encodedSprite);
      if (sprite != NULL) {
        actor->radius = Resource_GetMetadataRecordFar(encodedSprite)->boxY >> 1;
        *spriteSlots = sprite;
        spriteSlots++;
      }
      sprite = Sprite_Create(encodedSprite + 1);
      if (sprite != NULL)
        *spriteSlots = sprite;
      break;
    }
  }
  if (actor != NULL) {
    Actor_SetPositionAndResetMotion(actor, x, y, z);
    actor->script = (s32 *)gActorScript_EndScript;
    actor->speedLimit = toFIXED(2);
    actor->scriptCursor = 0;
    actor->scaleX = FIXED_ONE;
    actor->scaleY = FIXED_ONE;
    actor->acceleration = FIXED_ONE;
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    actor->gravity = FIXED_ONE;
    actor->bounce = FIXED_ONE / 4;
    actor->collisionFlags = 0;
    actor->steeringFlags = ACTOR_FACES_MOTION;
    actor->facing = DEGtoANG(90);
    actor->riseBaseY = 0;
    actor->homeX = x / toFIXED(1);
    actor->homeZ = z / toFIXED(1);
    actor->carrier = NULL;
    actor->stepX = 0;
    actor->stepY = 0;
    actor->stepZ = 0;
  }
  return actor;
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
void Actor_SelectAnimation(struct Actor *actor, s32 argument) {
  struct Sprite **items;
  struct Sprite *item;
  s32 count;

  if (actor != 0) {
    switch (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) {
    case ACTOR_SPRITE_ONE:
      Sprite_StartAnimation(((struct Sprite *)actor->sprite), argument);
      break;
    case ACTOR_SPRITE_GROUP:
      items = ((struct Sprite **)actor->sprite);
      for (count = 0; count < 4; count++) {
        item = *items++;
        if (item != 0)
          Sprite_StartAnimation(item, argument);
      }
      break;
    }
  }
}

/// Sets the timer step, the animation speed, of an actor's sprite, or of each
/// sprite of a group (ANIM_STEP_NORMAL is normal speed).
void Actor_SetAnimationSpeed(struct Actor *actor, s32 value) {
  s32 count;
  struct Sprite *child;
  struct Sprite **children;

  if (actor != 0) {
    switch (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) {
    case ACTOR_SPRITE_ONE:
      Sprite_SetAnimationSpeed(((struct Sprite *)actor->sprite), value);
      return;
    case ACTOR_SPRITE_GROUP:
      children = ((struct Sprite **)actor->sprite);
      for (count = 0; count < 4; count++) {
        child = *children++;
        if (child != 0)
          Sprite_SetAnimationSpeed(child, value);
      }
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
void Actor_SetSpriteRotation(struct Actor *actor, u32 value) {
  if (actor != 0 &&
      (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
    ((struct Sprite *)actor->sprite)->rotation = value;
}

/// Gives a one-sprite actor's first sprite animation animation id animationId
/// and restarts its animations. A negative animationId does nothing.
void Actor_SetFirstSpriteAnimation(struct Actor *actor, s32 animationId) {
  struct Sprite *child;

  if (actor != NULL &&
      (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
    child = ((struct Sprite *)actor->sprite);
    if (animationId >= 0) {
      child->entries[0]->animationId = animationId;
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

/// Starts the leader's control script for the way the party moves now
/// (gMainRoutineWork.movementMode), waiting a frame first for Sand on the World
/// Map.
void Actor_StartRenderModeScript(struct Actor *actor) {
  const s32 *script;
  struct MainRoutineWork *party = &gMainRoutineWork;

  switch (party->movementMode) {
  case LEADER_MOVEMENT_WALK:
    script = gActor_PlayerScript;
    break;
  case LEADER_MOVEMENT_WORLD_MAP:
    script = gActor_WorldMapScript;
    break;
  case LEADER_MOVEMENT_KEYS:
    script = gActor_KeysScript;
    break;
  case LEADER_MOVEMENT_ROPE:
    script = gActor_RopeScript;
    break;
  case LEADER_MOVEMENT_TIGHTROPE:
    script = gActor_TightropeScript;
    break;
  case LEADER_MOVEMENT_SAND:
    script = gActor_SandScript;
    break;
  case LEADER_MOVEMENT_SHIP_ON_MAP:
    script = gActor_SailOnMapScript;
    break;
  case LEADER_MOVEMENT_SHIP:
    script = gActor_SailScript;
    break;
  case LEADER_MOVEMENT_FLYING_SHIP:
    script = gActor_FlyScript;
    break;
  case LEADER_MOVEMENT_WORLD_SAND:
    wait(1);
    script = gActor_SandOnWorldMapScript;
    break;
  case LEADER_MOVEMENT_HOVER:
    script = gActor_HoverScript;
    break;
  case LEADER_MOVEMENT_WALKING:
    script = gActor_WalkingScript;
    break;
  case LEADER_MOVEMENT_NONE:
    script = gActorScript_EndScript;
    break;
  default:
    script = gActor_KeyCheckScript;
    break;
  }
  Actor_StartScript(actor, script);
}

/// Starts an actor on the idle script (a lone SC_END).
void Actor_StartIdle(struct Actor *actor) {
  Actor_StartScript(actor, gActorScript_EndScript);
}

/// Starts an actor on the camera script (SC_CAMERA). With a leader to follow,
/// it also gets a speed limit of 4 pixels a frame and an acceleration of 0.5.
void Actor_StartCamera(struct Actor *actor, s32 watchedActorHandle) {
  Actor_StartScript(actor, gActor_CameraScript);
  if (watchedActorHandle != 0) {
    actor->acceleration = toFIXED(1) / 2;
    actor->speedLimit = toFIXED(4);
    actor->watchedActor = (struct Actor *)watchedActorHandle;
    actor->timer = 0;
  }
}

/// Waits until the actor's script reaches SC_END, at most 600 frames; it
/// returns as soon as the script idles, and after the limit whether or not it
/// has.
void Actor_WaitForScriptEnd(struct Actor *actor) {
  s32 waitedFrames = 0;

  if (actor->script != 0 &&
      ((const s32 *)actor->script)[actor->scriptCursor] != SC_END) {
    do {
      wait(1);
      waitedFrames++;
      if (waitedFrames > 599)
        break;
    } while (actor->script != 0 &&
             ((const s32 *)actor->script)[actor->scriptCursor] != SC_END);
  }
}

/// Sets the flags of a one-sprite actor's sprite (enum SpriteFlag).
void Actor_SetSpriteFlags(struct Actor *actor, u32 value) {
  if (actor != NULL &&
      (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
    ((struct Sprite *)actor->sprite)->flags = value;
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
void Actor_SetOwnScale(struct Actor *actor, u32 value) {
  if (actor != NULL && actor->spriteStorageKind == ACTOR_SPRITE_ONE)
    ((struct Sprite *)actor->sprite)->useOwnScale = value;
}

/// Sets the colour mode (SpriteColorMode) of a one-sprite actor's sprite
/// animations, except those drawn hidden.
void Actor_SetSpriteColorMode(struct Actor *actor, u32 value) {
  if (actor != NULL && actor->spriteStorageKind == ACTOR_SPRITE_ONE)
    Sprite_SetAnimationColorModes(((struct Sprite *)actor->sprite), value);
}

/// Fixes the priority of a one-sprite actor's sprite and its shadow, which
/// then no longer follow the cells under it (ACTOR_PRIORITY_AUTOMATIC).
void Actor_SetSpritePriorityDirect(struct Actor *actor, s32 priority) {
  struct Sprite *sprite;

  if (actor == NULL || actor->spriteStorageKind != ACTOR_SPRITE_ONE) {
    return;
  }
  sprite = actor->sprite;
  sprite->image.priority = priority;
  sprite->shadow.priority = priority;
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
}

/// Shows item id's icon on its owner's sprite, 16 pixels square at scale
/// 1.0 in 16-colour palette 0.
void Actor_SetItemIcon(struct Actor *owner, s32 id) {
  struct Sprite *sprite;
  GlyphTransfer *transfer;
  s32 tile;

  if (owner == 0)
    return;
  owner->retainSpriteCache = 1;
  owner->iconItem = id;
  sprite = owner->sprite;
  if (sprite == 0)
    return;
  transfer = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  Icon_DrawItemFar(id);
  tile = VramCache_Load(sprite->slot, ICON_TILE_BYTES, transfer->tiles);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  sprite->count = 0;
  sprite->dirty = 0;
  sprite->width = 16;
  sprite->height = 16;
  sprite->offsetX = 0;
  sprite->offsetY = 4;
  sprite->scale = FIXED_ONE;
  sprite->image.tile = tile;
  sprite->image.fullColor = 0;
  sprite->image.palette = 0;
}

/// The sprite id an actor is drawn with (the first halfword of its sprite's
/// definition), or 0 for none.
s32 Actor_GetSpriteId(struct Actor *actor) {
  struct Sprite *sprite = actor->sprite;

  if (actor != NULL && sprite != NULL && sprite->entries != NULL)
    return sprite->entries[0]->animationId;
  return 0;
}

/// Sprite_WaitForTrackEnd on an actor's sprite.
void Actor_WaitForTrackEnd(struct Actor *actor) {
  if (actor != NULL)
    Sprite_WaitForTrackEnd(actor->sprite);
}

/// Suspends the actor camera updates, starts the palette back to normal and, a
/// frame later, shows the sprites with BG1-3 hidden, leaving BG0 unchanged.
void ActorSystem_SuspendProjectionAndShowSprites(void) {
  u16 dispcnt;

  Task_SuspendAll(ActorSystem_PlaceSprites);
  Task_SuspendAll(ActorSystem_ProjectSprites);
  PaletteFade_SetTargetFar(FIXED_ONE, 1);
  PaletteFade_StartInterpolationFar(1);
  wait(1);
  dispcnt = REG_DISPCNT;
  REG_DISPCNT =
      (dispcnt & (u16)~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)) |
      DISPCNT_OBJ_ON;
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

/// The sprite priority an actor takes on the cell under a point.
#define CELL_PRIORITY(cells, x, z)                                             \
  ((cells)[(FIELD_CELL(z) << 7) + FIELD_CELL(x)].actorPriority)
/// The map layer an actor takes on the cell under a point, plus one.
#define CELL_LAYER(cells, x, z)                                                \
  ((cells)[(FIELD_CELL(z) << 7) + FIELD_CELL(x)].actorLayer)

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
  struct ActorSystemWork *system;
  struct Actor *actor;
  u32 kind;
  FIXED dx;
  FIXED dz;
  s32 originY;
  struct PlacedSprite *sprite;
  FIXED screenTop;
  u8 *priorityFlags;
  s32 layer;
  u8 *terrainId;
  struct MapCell *cells;
  FIXED feetZ;
  s32 objectHeight;
  FIXED probeX;
  s32 priority;
  s32 leftPriority;
  s32 rightPriority;
  s32 middlePriority;
  s32 height;
  s32 rise;
  u32 layerPlusOne;
  s8 *basePriority;

  map = Ram_HeapSlots->mapGrid;
  /* FAKEMATCH(temporary): view, a pointer standing in for map->viewX's
   * address, viewZ read as view[1]. */
  view = &map->viewX;
  viewX = view[0] & FIELD_PIXEL_MASK;
  viewZ = view[1] & FIELD_PIXEL_MASK;
  system = Ram_HeapSlots->actorSystem;
  originY = map->origin[1];
  Render_LoadDecoder();
  {
    // The zero is set before the actors are read.
    /* FAKEMATCH(temporary): zero, 0 held in a local; as 0 at its uses. */
    u32 zero = 0;

    actor = Ram_HeapSlots->actors;
    system->used = zero;
  }
  for (objectIndex = 0; objectIndex < ACTOR_COUNT; objectIndex++, actor++) {
    if (actor->script == NULL)
      continue;
    if (actor->x.fixed != 0 || actor->z.fixed != 0) {
      kind = actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK;
      if (kind == ACTOR_SPRITE_NONE)
        continue;
      if (kind != ACTOR_SPRITE_ONE)
        continue;
      if (system->suspended != 0 && !(actor->retainSpriteCache & 1)) {
        struct PlacedSprite *frozen = actor->sprite;

        VramCache_ReleaseVram(frozen->slot);
        frozen->dirty = kind;
        continue;
      }
      sprite = actor->sprite;
      dx = actor->x.fixed - viewX;
      dz = actor->z.fixed - viewZ;
      screenTop = dz - actor->y.fixed;
      if (dx > -toFIXED(32) && dx < toFIXED(272) && screenTop > -toFIXED(32) &&
          screenTop < toFIXED(224) &&
          /* FAKEMATCH(temporary): priorityFlags, a pointer standing in for
           * actor->priorityFlags's address. */
          (priorityFlags = &actor->priorityFlags,
           !(*priorityFlags & ACTOR_PRIORITY_NOT_DRAWN))) {
        /* FAKEMATCH(temporary): terrainId, a pointer standing in for
         * actor->layer's address. */
        terrainId = &actor->layer;
        layer = *terrainId & 3;
        cells = map->layers[layer].cells;
        // The sprite takes the priority of the cells under its feet: 7 pixels
        // left of its middle and 6 right where the ground is level with its
        // own, and its middle. With none, it takes the map's base priority,
        // or one further back where the ground a cell in front, or two cells
        // in front less a cell's height, rises above its own; with
        // ACTOR_PRIORITY_IGNORES_RISES, it keeps its own.
        if (*priorityFlags & ACTOR_PRIORITY_AUTOMATIC) {
          probeX = actor->x.fixed - toFIXED(7);
          feetZ = actor->z.fixed + toFIXED(2);
          objectHeight = FIXEDtoINT(actor->groundHeight);
          priority = 0;
          leftPriority = 0;
          rightPriority = 0;
          if (!(*priorityFlags & ACTOR_PRIORITY_IGNORES_RISES)) {
            /* FAKEMATCH(temporary): basePriority, a pointer standing in for
             * map->basePriority's address. */
            basePriority = &map->basePriority;
            priority = *basePriority;
            if (MapGrid_GetTerrainHeight(layer, probeX, feetZ) ==
                actor->groundHeight) {
              leftPriority = CELL_PRIORITY(cells, probeX, feetZ);
              height = FIXEDtoINT(MapGrid_GetTerrainHeight(
                  layer, probeX, feetZ + toFIXED(MAPGRID_CELL_PIXELS)));
              rise = FIXEDtoINT(MapGrid_GetTerrainHeight(
                         layer, probeX,
                         feetZ + toFIXED(2 * MAPGRID_CELL_PIXELS))) -
                     MAPGRID_CELL_PIXELS;
              if (rise > height)
                height = rise;
              if (height > objectHeight)
                priority = map->basePriority + 1;
            }
            probeX += toFIXED(13);
            if (MapGrid_GetTerrainHeight(layer, probeX, feetZ) ==
                actor->groundHeight) {
              rightPriority = CELL_PRIORITY(cells, probeX, feetZ);
              height = FIXEDtoINT(MapGrid_GetTerrainHeight(
                  layer, probeX, feetZ + toFIXED(MAPGRID_CELL_PIXELS)));
              rise = FIXEDtoINT(MapGrid_GetTerrainHeight(
                         layer, probeX,
                         feetZ + toFIXED(2 * MAPGRID_CELL_PIXELS))) -
                     MAPGRID_CELL_PIXELS;
              if (rise > height)
                height = rise;
              if (height > objectHeight)
                priority = map->basePriority + 1;
            }
            probeX -= toFIXED(6);
            middlePriority = CELL_PRIORITY(cells, probeX, feetZ);
            height = FIXEDtoINT(MapGrid_GetTerrainHeight(
                layer, probeX, feetZ + toFIXED(MAPGRID_CELL_PIXELS)));
            rise =
                FIXEDtoINT(MapGrid_GetTerrainHeight(
                    layer, probeX, feetZ + toFIXED(2 * MAPGRID_CELL_PIXELS))) -
                MAPGRID_CELL_PIXELS;
            if (rise > height)
              height = rise;
            if (height > objectHeight)
              priority = map->basePriority + 1;
          } else {
            if (actor->groundHeight ==
                MapGrid_GetTerrainHeight(layer, probeX, feetZ))
              leftPriority = CELL_PRIORITY(cells, probeX, feetZ);
            probeX += toFIXED(13);
            if (actor->groundHeight ==
                MapGrid_GetTerrainHeight(layer, probeX, feetZ))
              rightPriority = CELL_PRIORITY(cells, probeX, feetZ);
            probeX -= toFIXED(6);
            middlePriority = CELL_PRIORITY(cells, probeX, feetZ);
          }
          // An odd priority is preferred: the left's, the right's, the
          // middle's.
          if (leftPriority != 0 || rightPriority != 0 || middlePriority != 0) {
            priority = leftPriority;
            if (!(priority & 1)) {
              if (rightPriority != 0)
                priority = rightPriority;
              if (!(priority & 1) && middlePriority != 0)
                priority = middlePriority;
            }
          }
          if (priority != 0 && actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
            struct PlacedSprite *placed = actor->sprite;

            if ((u32)priority > 3)
              priority = 3;
            placed->priority = priority;
            placed->shadowPriority = priority;
          }
        }
        layerPlusOne = CELL_LAYER(cells, actor->x.fixed, actor->z.fixed);
        if (layerPlusOne != 0)
          *terrainId = layerPlusOne - 1;
        scale.x = Iwram_MulFX(actor->scaleX, sprite->scale);
        scale.y = Iwram_MulFX(actor->scaleY, sprite->scale);
        point.x = dx;
        point.height = actor->y.fixed - originY;
        point.y = dz - originY;
        point.groundHeight = actor->groundHeight - originY;
        if (*priorityFlags & ACTOR_PRIORITY_UNDERFOOT) {
          point.height -= toFIXED(320);
          point.y -= toFIXED(320);
          point.groundHeight -= toFIXED(320);
        }
        if (*priorityFlags & ACTOR_PRIORITY_OVERHEAD) {
          point.height += toFIXED(320);
          point.y += toFIXED(320);
          point.groundHeight += toFIXED(320);
        }
        if (*priorityFlags & ACTOR_PRIORITY_OWN_DEPTH)
          Render_ApplyProjectedPlacementAtDepth(
              sprite, &point, scale.values, actor->facing, actor->riseEnabled);
        else
          Render_ApplyProjectedPlacement(sprite, SpritePlacement_Values(&point),
                                         scale.values, actor->facing);
        continue;
      }
      if (actor->retainSpriteCache & 2) {
        Sprite_ComposePicture(sprite, actor->facing);
      } else if (!(actor->retainSpriteCache & 1) &&
                 !(sprite->retainCacheEntry & 1)) {
        VramCache_ReleaseVram(sprite->slot);
        sprite->dirty = 1;
      }
    } else {
      // At (0, 0): not placed; only the sprite's VRAM is given up.
      kind = actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK;
      if (kind == ACTOR_SPRITE_ONE) {
        struct PlacedSprite *frozen = actor->sprite;

        if (actor->retainSpriteCache == 0 && !(frozen->retainCacheEntry & 1)) {
          VramCache_ReleaseVram(frozen->slot);
          frozen->dirty = kind;
        }
      }
    }
  }
  HeapSlot_Release(HEAP_SLOT_FRAME_SCRATCH);
}

// The fixed camera, on the World Map and in battle: each frame it places
// every actor's sprites, facing them by the angle from the eye to the target.

/// Unused: returns 1.
UNUSED s32 ActorCamera_ReturnTrue(void) { return 1; }

/// Places an actor's sprite (unless ACTOR_PRIORITY_NOT_DRAWN hides it) or each
/// of its group's four sprites, turned by angle. While battle commands are
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
    for (remaining = 0; remaining < 4; remaining++) {
      sprite = *sprites++;
      if (sprite != NULL)
        Render_PlaceProjectedSprite(sprite, &actor->x.fixed, scale,
                                    actor->facing + angle, actor->layer);
    }
    break;
  case ACTOR_SPRITE_NONE:
    break;
  }
}

/// Each frame, with the fixed camera: places every actor in use, from the
/// 64th down. While battle commands are chosen (FLAG_CHOOSING_COMMANDS) it uses
/// the fixed view matrix and turns the objects by an eighth of a turn less.
void ActorSystem_ProjectSprites(void) {
  struct ActorCameraFixed *camera = Ram_HeapSlots->cameraWork;
  struct ActorSystemWork *system = Ram_HeapSlots->actorSystem;
  struct ActorCameraPoint *eye;
  struct ActorCameraPoint *target;
  struct Actor *actor;
  s32 angle;
  s32 i;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  struct ActorCameraPoint unused;

  Render_LoadDecoder();
  eye = &camera->eye;
  target = &camera->target;
  if (camera->followEye != NULL)
    eye = camera->followEye;
  if (camera->followPosition != NULL)
    target = camera->followPosition;
  angle = (s16)Trig_Atan(FIXEDtoINT(eye->x - target->x),
                         FIXEDtoINT(eye->z - target->z));
  system->used = 0;
  if (GameFlag_Get(FLAG_CHOOSING_COMMANDS)) {
    Matrix_Initialize();
    angle -= DEGtoANG(45);
    Iwram_MultiplyMatrixEntry(gCamera_FixedViewMatrix);
    Matrix_LookAt((s32 *)eye, (s32 *)target);
  } else {
    Matrix_RewindStack();
    Matrix_SetLookAt((s32 *)eye, (s32 *)target);
  }
  actor = Ram_HeapSlots->actors;
  actor += ACTOR_COUNT - 1;
  for (i = ACTOR_COUNT - 1; i >= 0; i--, actor--) {
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
  interpreter->spriteStorageKind = ACTOR_SPRITE_NONE;
  interpreter->scriptCursor++;
  return 1;
}

/// SC_SHOW_SPRITE: shows the actor's one sprite again (ACTOR_SPRITE_ONE) and
/// moves on to the next command.
s32 ActorScript_ShowSprite(struct Actor *interpreter) {
  interpreter->spriteStorageKind = ACTOR_SPRITE_ONE;
  interpreter->scriptCursor++;
  return 1;
}

/// Waits, at most 600 frames, for an actor to reach where it is going.
void Actor_WaitForMove(void *actor) {
  s32 waitedFrames;

  for (waitedFrames = 0; waitedFrames < 600 && Actor_IsTargetUnset(actor) == 0;
       waitedFrames++) {
    wait(1);
  }
}
