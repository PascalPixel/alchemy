/// @file
/// Actors: creating and releasing them, dissolving sprites, starting scripts.
// Part of field.c.

#include "field_internal.h"

/// A dissolve clears one pixel of each 8-bit tile a step, in
/// gSpriteDissolve_PixelOrder's order, so a tile is gone after this many.
#define DISSOLVE_TILE_PIXELS 64
/// The step the first tile starts to dissolve at; each tile after it starts a
/// step sooner.
#define DISSOLVE_FIRST_STEP 64
/// The steps a dissolve takes, four a frame.
#define DISSOLVE_STEPS (DISSOLVE_FIRST_STEP + DISSOLVE_TILE_PIXELS)
/// How much further along the pixel order each tile reads than the tile
/// before, besides the step it is ahead.
#define DISSOLVE_ORDER_STRIDE 16
/// Keeps a pixel pair's odd pixel, its high byte, clearing the even one.
#define PIXEL_PAIR_ODD_MASK 0xff00

/// One step of dissolving a sprite pixel by pixel.
void SpriteDissolve_Step(u8 *rawSprite, u32 step) {
  u16 *tile;
  s32 tileCount;
  u32 i;
  const u8 *order;

  struct Sprite *sprite = (struct Sprite *)rawSprite;

  tile = (u16 *)(OBJ_VRAM0 + gVramCacheEntries[sprite->slot].offset);
  tileCount = (sprite->width * sprite->height) / DISSOLVE_TILE_PIXELS;
  i = 0;

  if (i < tileCount) {
    order = gSpriteDissolve_PixelOrder;
    {
      u32 highMask = PIXEL_PAIR_ODD_MASK;
      u32 indexMask = DISSOLVE_TILE_PIXELS - 1;

      for (; i < tileCount; i++, tile += TILE_SIZE_8BPP / 2, step++) {
        if ((step - DISSOLVE_FIRST_STEP) <= DISSOLVE_TILE_PIXELS - 1) {
          u8 pixel;
          u16 *pixelPair;
          u32 isOdd;

          // Each tile reads the order DISSOLVE_ORDER_STRIDE entries and a step
          // on from the tile before, so neighbouring tiles lose different
          // pixels at the same step.
          pixel = order[(step + i * DISSOLVE_ORDER_STRIDE) & indexMask];
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
  struct MapGridScrollState *grid = gMapGridState[0];
  s32 viewX;
  s32 viewZ;
  FIXED x;
  FIXED y;

  viewX = MapGridScrollState_View(grid)[0] & FIELD_PIXEL_MASK;
  viewZ = MapGridScrollState_View(grid)[1] & FIELD_PIXEL_MASK;
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

/// The colour an outlined sprite's edge is drawn in (Sprite_ComposePicture,
/// SPRITE_FLAG_OUTLINED): the sprite palette's white, colour 15, which
/// SPRITE_COLOR_WHITE paints with too.
#define OUTLINE_EDGE_COLOR 15

/// Sets up the actor system: allocates and zeroes the 64 actors and registers
/// the per-frame updates.
void ActorSystem_Initialize(s32 controlMode) {
  struct ActorSystemState *system;
  struct Actor *pool;
  volatile u32 fill;

  system = HeapSlot_AllocEwram(HEAP_SLOT_ACTOR_SYSTEM, sizeof(*system));
  pool = HeapSlot_AllocEwram(HEAP_SLOT_ACTORS, ACTOR_COUNT * sizeof(*pool));
  ActorSystem_Configure(controlMode);
  Dma_Fill32Cell(fill, 0, pool, ACTOR_COUNT * sizeof(*pool));
  Dma_Fill32Cell(fill, 0, system, sizeof(*system));
  // The task runner runs higher priorities first: the actors move before
  // the camera follows them.
  if (controlMode == ACTOR_SYSTEM_BATTLE)
    Task_Add(Actor_UpdateAllMotion, TASK_PASS_FRAME + 10);
  else
    Task_Add(Actor_UpdateAllOnField, TASK_PASS_FRAME + 10);
  if (controlMode >= ACTOR_SYSTEM_WORLD_MAP &&
      controlMode <= ACTOR_SYSTEM_BATTLE) {
    Task_Add(ActorSystem_ProjectSprites, TASK_PASS_FRAME);
  } else {
    Task_Add(ActorSystem_PlaceSprites, TASK_PASS_FRAME);
    gCamera_UnreadWordB = 0;
    gCamera_UnreadWordA = 0;
  }
  system->edge = OUTLINE_EDGE_COLOR;
  system->fill = 0;
}

/// Unused: two empty routines.
UNUSED void ActorSystem_ReservedNoOpA(void) {}

UNUSED void ActorSystem_ReservedNoOpB(void) {}

/// Returns the first of the 64 actors not in use (with no script), or 0.
void *Actor_FindFree(void) {
  struct Actor *actor = gActors;
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
/// is in use. encodedSprite / 4096 is its enum ActorCreateKind and the low 12
/// bits its sprite.
struct Actor *Actor_Create(s32 encodedSprite, FIXED x, FIXED y, FIXED z) {
  struct Actor *actor;
  struct Sprite *sprite;
  s32 spriteStorageKind;
  void **spriteSlots;

  // DEAD: a first search whose result is dropped; the ROM keeps its call.
  Actor_FindFree();
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
        actor->radius =
            Resource_GetMetadataRecordFar(encodedSprite)->footprint >> 1;
      } else {
        actor->spriteStorageKind = ACTOR_SPRITE_NONE;
      }
      break;
    case ACTOR_CREATE_GROUP:
      // Take a zeroed list of four sprites, and fill it with id and id + 1.
      spriteSlots =
          (void **)gActorSystemState->spriteGroups + gActorSystemState->count++;
      actor->spriteStorageKind = spriteStorageKind;
      actor->sprite = (struct Sprite *)spriteSlots;
      Dma_Fill32(0, spriteSlots, 4 * sizeof(void *));
      sprite = Sprite_Create(encodedSprite);
      if (sprite != NULL) {
        actor->radius =
            Resource_GetMetadataRecordFar(encodedSprite)->footprint >> 1;
        *spriteSlots++ = sprite;
      }
      sprite = Sprite_Create(encodedSprite + 1);
      if (sprite != NULL)
        *spriteSlots = sprite;
      break;
    }
  }
  if (actor != NULL) {
    Actor_SetPositionAndResetMotion(actor, x, y, z);
    // Up to 2 pixels a frame, gaining 1 a frame; unit scale; following the
    // ground and falling; turning to face its motion; keeping a quarter of its
    // speed when it bounces; facing down the screen (FACING_SOUTH, the
    // heading gWorldMap_KeyHeadings gives Down).
    actor->script = (s32 *)gActor_DefaultScript;
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
    actor->sceneWord = 0;
    actor->facing = FACING_SOUTH;
    // Its home position, in pixels.
    actor->homeX = x / toFIXED(1);
    actor->homeZ = z / toFIXED(1);
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

/// Defines name(actor, value), which calls apply(sprite, value) on an
/// actor's one sprite, or on each sprite of its group.
#define ACTOR_DEFINE_APPLY(name, actor_type, apply)                            \
  void name(actor_type rawActor, s32 value) {                                  \
    struct Actor *actor;                                                       \
    void **sprites;                                                            \
    s32 part;                                                                  \
    void *sprite;                                                              \
                                                                               \
    actor = rawActor;                                                          \
    if (actor != 0) {                                                          \
      switch (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) {             \
      case ACTOR_SPRITE_ONE:                                                   \
        apply(actor->sprite, value);                                           \
        break;                                                                 \
      case ACTOR_SPRITE_GROUP:                                                 \
        sprites = (void **)actor->sprite;                                      \
        for (part = 0; part < 4; part++) {                                     \
          sprite = *sprites++;                                                 \
          if (sprite != 0)                                                     \
            apply(sprite, value);                                              \
        }                                                                      \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  }

/// Switches all of an actor's sprites to animation value.
ACTOR_DEFINE_APPLY(Actor_SelectAnimation, void *, Sprite_StartAnimation)

/// Sets the animation speed (timer step) of all of an actor's sprites.
ACTOR_DEFINE_APPLY(Actor_SetAnimationSpeed, struct Actor *,
                   Sprite_SetAnimationSpeed)

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
void Actor_SetSpriteRotation(struct Actor *actor, u32 rotation) {
  if (actor != 0 &&
      (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
    actor->sprite->rotation = rotation;
}

/// Gives a one-sprite actor's first sprite animation the animation animationId
/// and restarts its animations. A negative animationId does nothing.
void Actor_SetFirstSpriteAnimation(struct Actor *actor, s32 animationId) {
  struct Sprite *sprite;

  if (actor != NULL &&
      (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
    sprite = actor->sprite;
    if (animationId >= 0) {
      sprite->entries[0]->animationId = animationId;
      Sprite_ResetAnimations(sprite);
    }
  }
}

/// Adds a layer playing animationId to a one-sprite actor's sprite. A negative
/// animationId does nothing.
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
  actor->watchedActor = (union ActorOrEffect *)watchedActorHandle;
}

/// Starts an actor on the script that runs SC_KEY_COUNT every frame.
void Actor_StartKeyCheck(struct Actor *actor) {
  Actor_StartScript(actor, gActor_KeyCheckScript);
}

/// Starts an actor on the script that moves it by the keys (SC_KEYS).
/// The field sequence gives it to the leader while he climbs
/// (LEADER_MOVEMENT_CLIMB).
void Actor_StartMoveByKeys(struct Actor *actor) {
  Actor_StartScript(actor, gActor_KeysScript);
}

/// Starts an actor on the player control script (SC_PLAYER).
/// The field sequence's usual choice for the leader on a layered map.
void Actor_StartPlayerControl(struct Actor *actor) {
  Actor_StartScript(actor, gActor_PlayerScript);
}

/// Starts an actor on the World Map control script (SC_WORLD).
/// The field sequence gives it to the leader on the World Map.
void Actor_StartWorldMapControl(struct Actor *actor) {
  Actor_StartScript(actor, gActor_WorldMapScript);
}

/// Starts an actor on the idle script (a lone SC_END).
void Actor_StartIdle(struct Actor *actor) {
  Actor_StartScript(actor, gActor_IdleScript);
}

/// Starts an actor on the camera script (SC_CAMERA). With a leader to follow,
/// it also gets a speed limit of 4 pixels a frame and an acceleration of 0.5.
void Actor_StartCamera(struct Actor *actor, s32 watchedActorHandle) {
  Actor_StartScript(actor, gActor_CameraScript);
  if (watchedActorHandle != 0) {
    actor->acceleration = toFIXED(1) / 2;
    actor->speedLimit = toFIXED(4);
    actor->watchedActor = (union ActorOrEffect *)watchedActorHandle;
    actor->timer = 0;
  }
}

/// Waits until the actor's script reaches SC_END, at most 300 frames (five
/// seconds); it returns as soon as the script idles, and after the limit
/// whether or not it has.
void Actor_WaitForScriptEnd(void *rawActor) {
  struct Actor *interpreter = rawActor;
  s32 waitedFrames;

  waitedFrames = 0;
  if (interpreter->script[interpreter->scriptCursor] != SC_END) {
    do {
      wait(1);
      waitedFrames++;
      if (waitedFrames > 299)
        break;
    } while (interpreter->script[interpreter->scriptCursor] != SC_END);
  }
}

/// Sets the flags of a one-sprite actor's sprite (bit 0: draw its shadow).
void Actor_SetSpriteFlags(struct Actor *actor, u32 flags) {
  if (actor != NULL) {
    if ((actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
      actor->sprite->flags = flags;
  }
}

/// Sets the OAM blend mode of a one-sprite actor's sprite.
void Actor_SetBlendMode(struct Actor *actor, u32 mode) {
  if (actor != 0 && actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
    struct Sprite *sprite = actor->sprite;
    sprite->image.objectMode = mode;
  }
}

/// Makes the fixed camera draw a one-sprite actor at its own scale instead of
/// scaling it by distance.
void Actor_SetSpriteScale(struct Actor *actor, u32 fixedScale) {
  if (actor != 0 && actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
    struct Sprite *sprite = actor->sprite;
    sprite->useOwnScale = fixedScale;
  }
}

/// Sets the colour mode (SpriteColorMode) of a one-sprite actor's sprite
/// animations, except those drawn hidden (see Sprite_SetAnimationColorModes).
void Actor_SetSpriteColorMode(struct Actor *actor, u32 colorMode) {
  if (actor != NULL && actor->spriteStorageKind == ACTOR_SPRITE_ONE)
    Sprite_SetAnimationColorModes(actor->sprite, colorMode);
}

/// Suspends the actor camera updates, starts the palette back to normal and, a
/// frame later, shows the sprites with BG1-3 hidden, leaving BG0 unchanged.
void ActorSystem_SuspendProjectionAndShowSprites(void) {
  Task_Suspend(ActorSystem_PlaceSprites);
  Task_Suspend(ActorSystem_ProjectSprites);
  PaletteFade_SetTargetFar(PALETTE_TINT_NONE, 1);
  PaletteFade_StartInterpolationFar(1);
  wait(1);
  REG_DISPCNT =
      (REG_DISPCNT & ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)) |
      DISPCNT_OBJ_ON;
}

/// Resumes the actor camera updates and hides BG1-3 and the sprites.
void ActorSystem_ResumeProjectionAndHideLayers(void) {
  Task_Resume(ActorSystem_PlaceSprites);
  Task_Resume(ActorSystem_ProjectSprites);
  REG_DISPCNT &=
      ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON);
}

/// Unused: returns 1.
UNUSED s32 Actor_ReturnTrue(void) { return 1; }

#undef ACTOR_DEFINE_APPLY
