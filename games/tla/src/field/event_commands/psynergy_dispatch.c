/// @file
/// Running a field Psynergy by its kind, and an object's priority and depth.
// Part of event_commands.c.

#include "../event_commands_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/field/field_sequence.h"
#include "module/field/object.h"
#include "module/system/system.h"
#include "macros.h"

// Field Psynergy: the dispatchers that run each animation

void FieldPsynergy_HaltOnTarget(s32 id);
void FieldPsynergy_StartMindReadWave(s32 sourceId, s32 targetId);
#define FieldPsynergy_SteerPushedTarget FieldPsynergy_SteerPushedTarget
#define FieldPsynergy_ForceOnTarget FieldPsynergy_ForceOnTarget
#define FieldPsynergy_DouseOnTarget FieldPsynergy_DouseOnTarget
#define FieldPsynergy_FrostOnTarget FieldPsynergy_FrostOnTarget
/* VIEW(f0daecc): routine FieldPsynergy_WhirlwindOnTarget as void
 * FieldPsynergy_WhirlwindOnTarget(s32 targetId); battle.h has void
 * FieldPsynergy_WhirlwindOnTarget(void); tried as the definition's void
 * FieldPsynergy_WhirlwindOnTarget(void), which the object already declares, it
 * does not compile in tla-ja:
 * games/tla/src/field/event_commands/psynergy_dispatch.c:455: too many
 * arguments to function `FieldPsynergy_WhirlwindOnTarget'. */
extern void FieldEffect_RunWhirlwindOnTarget__f0daecc(s32 targetId) __asm__(
    "FieldPsynergy_WhirlwindOnTarget");
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

/// Runs the animation's handler for kind: 0 runs it with no target chosen, 1
/// on the chosen target (secondActorId), 2 finishes it and 3 runs the kind's
/// extra step.
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
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
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
  else if (animation == FIELD_PSYNERGY_UNNAMED_29)
    FieldPsynergy_Unnamed29();
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
  else if (animation == FIELD_PSYNERGY_UNNAMED_28)
    FieldPsynergy_Unnamed28();
  else if (animation == FIELD_PSYNERGY_HALT) {
    // Let the object stopped last time go, then stop the nearest one that
    // has an event to run; with none, play the fallback transition.
    struct MainRoutineWork *party = &gMainRoutineWork;
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
    // A wave still running from the last cast is reset first.
    if (event->revealActive != 0)
      FieldPsynergy_EndReveal();
    FieldPsynergy_StartMindReadWave(scene->firstActorId, targetId);
  }
}

/// Runs the field Psynergy animation the scene names on the target the player
/// chose (FieldEffectWork.secondActorId).
void FieldPsynergy_Dispatch(void) {
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
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
    FieldEffect_RunWhirlwindOnTarget__f0daecc(targetId);
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
  else if (animation == FIELD_PSYNERGY_UNNAMED_28)
    FieldPsynergy_Unnamed28();
  else if (animation == FIELD_PSYNERGY_MIND_READ) {
    // A wave still running from the last cast is reset first; a target that
    // is not the paused object stops walking while the wave runs.
    struct MainRoutineWork *party;

    if (event->revealActive != 0)
      FieldPsynergy_EndReveal();
    party = &gMainRoutineWork;
    if (party->haltedActor != scene->secondActorId)
      ((struct Actor *)scene->child)->paused = 1;
    FieldPsynergy_StartMindReadWave(scene->firstActorId, targetId);
  } else if (animation == FIELD_PSYNERGY_HALT) {
    // Let the object stopped last time go, then stop the target.
    struct MainRoutineWork *party = &gMainRoutineWork;

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
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;

  if (scene->fieldPsynergy == FIELD_PSYNERGY_MIND_READ) {
    struct PartyPauseView *party;

    FieldPsynergy_StopMindReadWave();
    party = (struct PartyPauseView *)&gMainRoutineWork;
    if (party->pausedObject != scene->secondActorId)
      ((struct Actor *)scene->child)->paused = 0;
  }
}

/// The extra handler of Reveal, Cloak, Avoid, Sand, Hover and the unnamed kind
/// 28: what each does when it ends.
void FieldPsynergy_RunExtraHandler(void) {
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;

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
  case FIELD_PSYNERGY_UNNAMED_28:
    FieldPsynergy_EndUnnamed28();
    break;
  }
}

void Actor_ApplyRandomChildValues(struct Actor *owner) {
  s32 state;
  struct CompositeAnimation *target;
  s32 initialCount;
  s32 count;
  struct AnimationEntry **entry;
  struct AnimationEntry *current;
  u32 frame;
  s32 value;

  state = owner->spriteStorageKind;
  if (state == 1) {
    target = owner->sprite;
    if (target != 0 && (target->displayFlags & state) == 0) {
      initialCount = target->count;
      if (initialCount != 0) {
        entry = target->entries;
        frame = gFrameCount;
        count = initialCount;
        do {
          current = *entry++;
          value = frame % 6;
          count--;
          current->param = value;
        } while (count != 0);
      }
      target->dirty = 1;
    }
  }
}

void Actor_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                               s32 magnitude, s32 angle) {
  s32 values[3];

  if (object != 0) {
    SET_VECTOR(values, object->x.fixed, object->y.fixed, object->z.fixed);
    Vector_AddPolarOffset(magnitude, angle, values);
    Actor_SetMoveTargetFar(object, values[0], values[1], values[2]);
  }
}

/// The OBJ priority a Psynergy's canvas takes: the caster's own when it
/// casts facing away (0xc000), else 0, in front of everything.
s32 FieldPsynergy_GetCanvasPriority(struct Actor *object, s32 kind) {
  if (kind == 0xc000)
    return ((struct DepthPart *)object->sprite)->priority;
  return 0;
}

/// Returns the 2-bit OBJ priority of the object's animation record.
s32 BattleEffect_GetObjectPriority(struct Actor *object) {
  return ((struct DepthPart *)object->sprite)->priority;
}

/// Returns the object's sort depth: from its projected z on the world map
/// (mode 3), otherwise from its y and z against the map's scroll.
s32 BattleEffect_GetObjectDepth(struct Actor *object) {
  s32 point[3];
  s32 screen[3];
  s32 depth;
  s32 *out;

  if (gFieldWork->mode == 3) {
    point[0] = object->x.fixed;
    out = screen;
    point[1] = object->y.fixed;
    point[2] = object->z.fixed;
    Render_ProjectPoint(point, out);
    depth = (0x200 - out[2]) / 2 + 128;
    if (depth <= 0)
      depth = 1;
  } else {
    struct DepthMap *map = (struct DepthMap *)Ram_HeapSlots->mapGrid;
    struct DepthScroll *scroll = &map->scroll;
    s32 bias = scroll->offset & 0xffff0000;
    s32 base = map->base->base;

    depth =
        ((object->y.fixed - base + object->z.fixed - base - bias) >> 16) + 62;
  }
  return depth;
}

/// Per-frame callback: draws the effect scene's sprite (effect work +0x7c8).
void BattleEffect_DrawSceneSprite(void) {
  EffectSprite_Draw((u8 *)Ram_HeapSlots->fieldEffectWork + 0x7c8);
}

void Func_080dba5c(void) {
  struct DepthScene *scene = Ram_HeapSlots->fieldEffectWork;
  struct Actor *mainActor;
  struct DepthSprite *sprite;
  s32 position[3];
  void *buffer;
  s32 slot;
  s32 tiles;

  mainActor = scene->mainActor;
  if (scene->mode == 0xc000) {
    scene->active = 1;
    buffer = Heap_Alloc(0x200);
    /* FAKEMATCH(dead): an empty once-only loop. */
    do {
    } while (0);
    Iwram_FillWordsEntry(buffer, 0x200, 0);
    slot = VramCache_FindFreeEntry();
    tiles = VramCache_Load(slot, 0x200, buffer);
    scene->vramSlot = slot;
    Heap_Release(buffer);
    sprite = &scene->sprite;
    EffectSprite_Init(sprite, 16, 31, 0x80000000, tiles);
    sprite->frame = 0;
    sprite->priority = 0;
    sprite->hidden = 0;
    sprite->layer = 0;
    sprite->scale = Q16_ONE * 2;
    position[0] = scene->x;
    position[1] = mainActor->y.fixed + 0x1c0000;
    position[2] = mainActor->z.fixed;
    Camera_WorldToScreen(position);
    sprite->x = position[0];
    sprite->y = position[2];
    Task_Add(BattleEffect_DrawSceneSprite, TASK_PASS_VBLANK);
  }
}
#undef CELL_MASK
#undef SIDE_TURN
