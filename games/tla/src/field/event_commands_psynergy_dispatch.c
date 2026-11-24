/// @file
/// Running a field Psynergy by its kind, and the priority and depth its
/// sprites take to be drawn with an actor.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "field/object.h"
#include "system/system.h"
#include "macros.h"

// Field Psynergy: the dispatchers that run each animation

void FieldPsynergy_HaltOnTarget(s32 id);
void FieldPsynergy_StartMindReadWave(s32 sourceId, s32 targetId);
void FieldPsynergy_SteerPushedTarget(s32 unused);
void FieldPsynergy_ForceOnTarget(s32 unused);
void FieldPsynergy_DouseOnTarget(s32 unused);
void FieldPsynergy_FrostOnTarget(s32 unused);
void FieldPsynergy_WhirlwindOnTarget(s32 unused);
void FieldPsynergy_CatchOnTarget(s32 targetId);
void FieldPsynergy_SandOnTarget(s32 targetId);
void FieldPsynergy_BlazeOnTarget(s32 targetId);
void FieldPsynergy_CycloneOnTarget(s32 targetId);
void FieldPsynergy_ScoopOnTarget(s32 targetId);
void FieldPsynergy_TremorOnTarget(s32 targetId);
void FieldPsynergy_GrindOnTarget(s32 targetId);
void FieldPsynergy_HoverOnTarget(s32 targetId);
void FieldPsynergy_TeleportOnTarget(s32 targetId);
void FieldPsynergy_ParchOnTarget(s32 targetId);
void FieldPsynergy_BurstOnTarget(s32 targetId);
void FieldPsynergy_PoundOnTarget(s32 targetId);

/// Runs one step of the scene's field Psynergy: cast with no target, cast on
/// the chosen target (secondActorId), Mind Read's end, or what a lasting
/// Psynergy does when it ends.
void FieldPsynergy_RunHandler(s32 kind) {
  if (kind == FIELD_PSYNERGY_HANDLER_UNTARGETED)
    FieldPsynergy_Run();
  else if (kind == FIELD_PSYNERGY_HANDLER_ON_TARGET)
    FieldPsynergy_Dispatch();
  else if (kind == FIELD_PSYNERGY_HANDLER_FINISH)
    FieldPsynergy_FinishMindRead();
  else if (kind == FIELD_PSYNERGY_HANDLER_EXTRA)
    FieldPsynergy_RunExtraHandler();
}

/// Runs the field Psynergy animation the scene names, with no target chosen
/// yet: each kind runs its effect to the end.
void FieldPsynergy_Run(void) {
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  struct FieldState *event = Ram_HeapSlots->fieldState;
  s32 animation = scene->fieldPsynergy;
  s32 targetId = scene->secondActorId;
  s32 objectId;

  if (animation == FIELD_PSYNERGY_MOVE)
    FieldPsynergy_Move();
  else if (animation == FIELD_PSYNERGY_LIFT)
    FieldPsynergy_Lift();
  else if (animation == FIELD_PSYNERGY_CARRY)
    FieldPsynergy_Carry();
  else if (animation == FIELD_PSYNERGY_FORCE)
    FieldPsynergy_Force();
  else if (animation == FIELD_PSYNERGY_DOUSE)
    FieldPsynergy_Douse();
  else if (animation == FIELD_PSYNERGY_WHIRLWIND)
    FieldPsynergy_Whirlwind();
  else if (animation == FIELD_PSYNERGY_FROST)
    FieldPsynergy_Frost();
  else if (animation == FIELD_PSYNERGY_PLY)
    FieldPsynergy_Ply();
  else if (animation == FIELD_PSYNERGY_GROWTH)
    FieldPsynergy_Growth();
  else if (animation == FIELD_PSYNERGY_CATCH)
    FieldPsynergy_Catch();
  else if (animation == FIELD_PSYNERGY_RETREAT)
    FieldPsynergy_Retreat();
  else if (animation == FIELD_PSYNERGY_AVOID)
    FieldPsynergy_Avoid();
  else if (animation == FIELD_PSYNERGY_CLOAK)
    FieldPsynergy_StartCloak();
  else if (animation == FIELD_PSYNERGY_REVEAL)
    FieldPsynergy_Reveal();
  else if (animation == FIELD_PSYNERGY_LASH)
    FieldPsynergy_Lash();
  else if (animation == FIELD_PSYNERGY_ARROW)
    FieldPsynergy_Arrow();
  else if (animation == FIELD_PSYNERGY_CYCLONE)
    FieldPsynergy_Cyclone();
  else if (animation == FIELD_PSYNERGY_BLAZE)
    FieldPsynergy_Blaze();
  else if (animation == FIELD_PSYNERGY_BURST)
    FieldPsynergy_Burst();
  else if (animation == FIELD_PSYNERGY_POUND)
    FieldPsynergy_Pound();
  else if (animation == FIELD_PSYNERGY_SCOOP)
    FieldPsynergy_Scoop();
  else if (animation == FIELD_PSYNERGY_TREMOR)
    FieldPsynergy_Tremor();
  else if (animation == FIELD_PSYNERGY_GRIND)
    FieldPsynergy_Grind();
  else if (animation == FIELD_PSYNERGY_HOVER)
    FieldPsynergy_Hover();
  else if (animation == FIELD_PSYNERGY_SAND)
    FieldPsynergy_Sand();
  else if (animation == FIELD_PSYNERGY_TELEPORT)
    FieldPsynergy_Teleport();
  else if (animation == FIELD_PSYNERGY_PARCH)
    FieldPsynergy_Parch();
  else if (animation == FIELD_PSYNERGY_RING)
    FieldPsynergy_Ring();
  else if (animation == FIELD_PSYNERGY_HALT) {
    // Resume the actor stopped before, then stop the nearest one that has an
    // event to run; with none, the stopwatch is thrown at nothing.
    struct MainRoutineState *party = &gMainRoutineState;
    s32 paused;

    paused = party->haltedActor;
    if (paused != -1) {
      FieldPsynergy_ResumeHaltedActor(paused);
      party->haltedActor = -1;
    }
    objectId = Field_FindNearbyPsynergyTarget(scene->firstActorId, animation);
    objectId = FieldPsynergy_FilterHaltTarget(objectId);
    if (Scene_FindActorTalkTrigger(objectId) != 0) {
      FieldPsynergy_SetupActorPair(scene->firstActorId, objectId);
      FieldPsynergy_HaltOnTarget(objectId);
      FieldPsynergy_HaltActor(objectId);
      party->haltedActor = objectId;
    } else {
      FieldPsynergy_Halt();
    }
  } else if (animation == FIELD_PSYNERGY_MIND_READ) {
    // The wave and colour effect, stopping a running Reveal first.
    if (event->revealActive != 0)
      FieldPsynergy_EndReveal();
    FieldPsynergy_StartMindReadWave(scene->firstActorId, targetId);
  }
}

/// Runs the field Psynergy animation the scene names on the target the player
/// chose (FieldEffectState.secondActorId).
void FieldPsynergy_Dispatch(void) {
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  struct FieldState *event = Ram_HeapSlots->fieldState;
  s32 animation = scene->fieldPsynergy;
  s32 targetId = scene->secondActorId;

  scene->enabled = 0;
  if (animation == FIELD_PSYNERGY_MOVE)
    FieldPsynergy_SteerPushedTarget(targetId);
  else if (animation == FIELD_PSYNERGY_LIFT)
    FieldPsynergy_LiftOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_CARRY)
    FieldPsynergy_SteerLiftedTarget(targetId);
  else if (animation == FIELD_PSYNERGY_FORCE)
    FieldPsynergy_ForceOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_DOUSE)
    FieldPsynergy_DouseOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_FROST)
    FieldPsynergy_FrostOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_GROWTH)
    FieldPsynergy_GrowthOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_PLY)
    FieldPsynergy_PlyOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_WHIRLWIND)
    FieldPsynergy_WhirlwindOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_CATCH)
    FieldPsynergy_CatchOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_AVOID)
    FieldPsynergy_Avoid();
  else if (animation == FIELD_PSYNERGY_LASH)
    FieldPsynergy_LashOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_CLOAK)
    FieldPsynergy_StartCloak();
  else if (animation == FIELD_PSYNERGY_RETREAT)
    FieldPsynergy_Retreat();
  else if (animation == FIELD_PSYNERGY_REVEAL)
    FieldPsynergy_Reveal();
  else if (animation == FIELD_PSYNERGY_SAND)
    FieldPsynergy_SandOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_BLAZE)
    FieldPsynergy_BlazeOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_CYCLONE)
    FieldPsynergy_CycloneOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_SCOOP)
    FieldPsynergy_ScoopOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_TREMOR)
    FieldPsynergy_TremorOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_GRIND)
    FieldPsynergy_GrindOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_HOVER)
    FieldPsynergy_HoverOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_TELEPORT)
    FieldPsynergy_TeleportOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_PARCH)
    FieldPsynergy_ParchOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_BURST)
    FieldPsynergy_BurstOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_POUND)
    FieldPsynergy_PoundOnTarget(targetId);
  else if (animation == FIELD_PSYNERGY_RING)
    FieldPsynergy_Ring();
  else if (animation == FIELD_PSYNERGY_MIND_READ) {
    // The wave and colour effect, stopping a running Reveal first; a target
    // other than the halted actor stands still while the wave runs.
    struct MainRoutineState *party;

    if (event->revealActive != 0)
      FieldPsynergy_EndReveal();
    party = &gMainRoutineState;
    if (party->haltedActor != scene->secondActorId)
      ((struct Actor *)scene->child)->paused = 1;
    FieldPsynergy_StartMindReadWave(scene->firstActorId, targetId);
  } else if (animation == FIELD_PSYNERGY_HALT) {
    // Resume the actor stopped before, then stop the target.
    struct MainRoutineState *party = &gMainRoutineState;

    if (party->haltedActor != -1) {
      FieldPsynergy_ResumeHaltedActor(party->haltedActor);
      party->haltedActor = -1;
    }
    FieldPsynergy_HaltActor(targetId);
    party->haltedActor = targetId;
    FieldPsynergy_HaltOnTarget(targetId);
  }
}

/// Finishes the current field Psynergy when it is Mind Read: ends it and lets
/// its target move again, unless the target is the actor kept stopped.
void FieldPsynergy_FinishMindRead(void) {
  struct FieldEffectState *effectState = Ram_HeapSlots->fieldEffectState;

  if (effectState->fieldPsynergy == FIELD_PSYNERGY_MIND_READ) {
    FieldPsynergy_StopMindReadWave();
    if (gMainRoutineState.haltedActor != effectState->secondActorId) {
      ((struct Actor *)effectState->child)->paused = 0;
    }
  }
}

/// The extra handler of Reveal, Cloak, Avoid, Sand, Hover and the Ring: what
/// each does when it ends.
void FieldPsynergy_RunExtraHandler(void) {
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;

  switch (scene->fieldPsynergy) {
  case FIELD_PSYNERGY_REVEAL:
    FieldPsynergy_EndReveal();
    break;
  case FIELD_PSYNERGY_CLOAK:
    FieldPsynergy_EndCloak();
    break;
  case FIELD_PSYNERGY_AVOID:
    FieldPsynergy_ShowAvoidWoreOff();
    break;
  case FIELD_PSYNERGY_SAND:
    FieldPsynergy_EndSand();
    break;
  case FIELD_PSYNERGY_HOVER:
    FieldPsynergy_EndHover();
    break;
  case FIELD_PSYNERGY_RING:
    FieldPsynergy_EndRing();
    break;
  }
}

/// Per-frame update: sets every part of an actor's sprite to the next of the
/// six orders of its colour channels (SPRITE_COLOR_NORMAL to SPRITE_COLOR_BRG).
void Actor_CycleSpriteColors(struct Actor *actor) {
  struct Sprite *sprite;
  s32 count;
  s32 i;

  if (actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
    sprite = actor->sprite;
    if (sprite != 0 && sprite->retainCacheEntry == 0) {
      count = sprite->count;
      for (i = 0; i < count; i++)
        sprite->entries[i]->colorMode = gFrameCount % (SPRITE_COLOR_BRG + 1);
      sprite->dirty = 1;
    }
  }
}

/// Sends the actor toward the point magnitude away from it at angle.
void Actor_SetTargetPositionFromMagnitudeAngle(struct Actor *actor,
                                               s32 magnitude, s32 angle) {
  s32 values[3];

  if (actor != 0) {
    SET_VECTOR(values, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(magnitude, angle, values);
    Actor_SetMoveTargetFar(actor, values[0], values[1], values[2]);
  }
}

/// The OBJ priority a Psynergy's canvas takes: the caster's own when it
/// casts facing away (FACING_NORTH), else 0, in front of everything.
s32 FieldPsynergy_GetCanvasPriority(struct Actor *actor, s32 facing) {
  if (facing == FACING_NORTH)
    return ((struct PlacedSprite *)actor->sprite)->priority;
  return 0;
}

/// Returns the OBJ priority of the actor's sprite.
s32 Actor_GetSpritePriority(struct Actor *actor) {
  return ((struct PlacedSprite *)actor->sprite)->priority;
}

/// On the world map a sprite's depth slot is one more for each two pixels its
/// point is nearer the camera than 512, about the camera's distance from the
/// ground it looks at, where the slot is 128, as Render_PlaceProjectedSprite
/// sorts a projected sprite.
#define WORLD_MAP_GROUND_DEPTH 512
#define WORLD_MAP_GROUND_SLOT 128

/// Returns the depth slot the actor's sprite takes, so that a sprite drawn
/// with it sorts with it: from its projected depth on the world map, otherwise
/// from its screen row.
s32 Actor_GetSpriteDepth(struct Actor *actor) {
  FIXED point[3];
  s32 screen[3];
  s32 depth;

  if (gFieldState->mode == ACTOR_SYSTEM_WORLD_MAP) {
    point[0] = actor->x.fixed;
    point[1] = actor->y.fixed;
    point[2] = actor->z.fixed;
    Matrix_ProjectPoint(point, screen);
    depth = (WORLD_MAP_GROUND_DEPTH - screen[2]) / 2 + WORLD_MAP_GROUND_SLOT;
    if (depth <= 0)
      depth = 1;
  } else {
    struct DepthMap *map = (struct DepthMap *)Ram_HeapSlots->mapGrid;
    struct DepthScroll *view = &map->view;
    s32 bias = view->z & FIELD_PIXEL_MASK;
    s32 base = map->origin->y;

    depth = FIXEDtoINT(actor->y.fixed - base + actor->z.fixed - base - bias) +
            PLACEMENT_IMAGE_DEPTH;
  }
  return depth;
}

/// Draws the Psynergy's canvas sprite (FieldEffectState.sprite) each frame.
void FieldPsynergy_DrawCanvasSprite(void) {
  EffectSprite_Draw(
      &((struct FieldEffectState *)Ram_HeapSlots->fieldEffectState)->sprite);
}

/// When the caster faces away (FACING_NORTH), starts the Psynergy's canvas
/// sprite (FieldEffectState.sprite): a blank 32x32 sprite of its own VRAM cache
/// entry, twice as tall, 28 pixels above the caster in front of everything,
/// drawn each frame by FieldPsynergy_DrawCanvasSprite until
/// FieldPsynergy_ReleaseSceneSprite.
void FieldPsynergy_StartSceneSprite(void) {
  struct FieldEffectState *effectState = Ram_HeapSlots->fieldEffectState;
  struct Actor *mainActor;
  struct EffectSprite *sprite;
  FIXED position[3];
  void *buffer;
  s32 slot;
  s32 tiles;

  mainActor = effectState->mainActor;
  if (effectState->angle == FACING_NORTH) {
    effectState->spriteActive = 1;
    buffer = Heap_Alloc(512);
    IWRAM_FILL_WORDS(buffer, 512, 0);
    slot = VramCache_FindFreeEntry();
    tiles = VramCache_Load(slot, 512, buffer);
    effectState->spriteVramSlot = slot;
    Heap_Release(buffer);
    sprite = &effectState->sprite;
    EffectSprite_Init(sprite, 16, 31, OBJ_SIZE_32X32, tiles);
    sprite->depth = 0;
    sprite->oam.priority = 0;
    sprite->oam.fullColor = 0;
    sprite->oam.palette = 0;
    sprite->scaleY = FIXED_ONE * 2;
    position[0] = effectState->x;
    position[1] = mainActor->y.fixed + toFIXED(28);
    position[2] = mainActor->z.fixed;
    Camera_WorldToScreen(position);
    sprite->x = position[0];
    sprite->y = position[2];
    Task_Add(FieldPsynergy_DrawCanvasSprite, TASK_PASS_FRAME);
  }
}
#undef WORLD_MAP_GROUND_DEPTH
#undef WORLD_MAP_GROUND_SLOT
