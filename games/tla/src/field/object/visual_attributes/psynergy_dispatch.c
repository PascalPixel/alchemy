/// @file
/// Running a field Psynergy animation by its kind, an object's chain, and an
/// object's priority and depth.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/field/field_sequence.h"
#include "module/field/object.h"
#include "module/system/system.h"

struct Actor *Actor_GetById(u32 objectId);

#define CELL_MASK 0xfff00000 // 16-pixel cells

/// Hangs a vine of seven links from the anchor object objectId, clearing the
/// five cells it hangs over, starting the vines first if they are not running;
/// with grow set, the vine drops into place over 28 frames.
void HangingVine_Hang(s32 objectId, s32 grow) {
  struct ChainObject *object;
  struct MapWork *map;
  struct WorkA4Slot *slot;
  struct HangingVineLink *link;
  struct HangingVineLink **tail;
  struct MapCell *cells;
  struct MapCell *cell;
  s32 i;
  s32 x;
  s32 top;
  s32 z;
  s32 y;
  s32 frame;
  s32 last;
  s32 kind;
  s32 running;

  object = Actor_GetById(objectId);
  running = Ram_HeapSlots->unk84[8];
  kind = 372;
  map = Ram_HeapSlots->mapWork;
  if (running == 0) {
    if (object->sprite != 0 && object->sprite->entries[0] != 0)
      kind = *object->sprite->entries[0];
    HangingVine_Initialize(kind);
  }
  if (HangingVine_FindSlot(objectId) != 0)
    return;
  slot = HangingVine_FindFreeSlot();
  if (slot == 0)
    return;

  // Clear the collision codes of the five cells above the object.
  x = object->x >> 20;
  z = (object->z - object->y) >> 20;
  cells = map->layers[2].cells;
  for (i = 0; i < 5; i++) {
    z++;
    cell = cells;
    cell += x + z * 128;
    cell->collisionCode = 0;
  }

  x = (object->x & CELL_MASK) + (8 << 16);
  y = object->y & CELL_MASK;
  top = y + (8 << 16);
  z = (object->z & CELL_MASK) + (16 << 16);
  Actor_SelectAnimationFar((struct Actor *)object, 0);
  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->z += 16 << 16;
  object->y -= 64 << 16;
  Actor_SetSpriteFlagsFar(object, 0);

  // Hang the links.
  last = 6;
  tail = &slot->root;
  for (i = 0; i <= last && (link = HangingVine_AllocLink()) != 0; i++) {
    link->x = x;
    link->y = y + (15 << 16) - i * (14 << 16);
    link->z = z;
    if (i == 0)
      link->kind = 1;
    else if (i == 1)
      link->kind = 2;
    else if (i == last)
      link->kind = 5;
    else
      link->kind = i % 2 + 3;
    *tail = link;
    tail = &link->next;
  }

  if (grow == 0) {
    for (i = 0, link = slot->root; i <= last && link != 0; i++)
      link = link->next;
  } else {
    for (frame = 1; frame <= 28; frame++) {
      i = 0;
      link = slot->root;
      if (i <= last && link != 0) {
        s32 drop = -(frame << 15);

        do {
          link->x = x;
          link->y = top - drop / 2;
          link->z = z;
          drop += frame << 16;
          i++;
          link = link->next;
        } while (i <= last && link != 0);
      }
      wait(1);
    }
  }
  slot->active = 1;
  slot->phase = 0;
  slot->swing = 0x100;
  slot->id = objectId;
  slot->spacing = 14 << 16;
}

// The half turn on the leader's side of the anchor: 0 on its right, 1 on its
// left.
#define SIDE_TURN(side) ((side) * (TRIG_TURN / 2))

/// Swings the leader across on the vine hanging from object id.
void HangingVine_Swing(s32 id) {
  s32 rootX, rootY;
  s32 motionFlags;
  struct Unk_080db0b0Sprite *heldSprite;
  s32 side = 0;
  struct Actor *leader = Actor_GetById(Party_GetLeaderActor());
  struct Actor *anchor = Actor_GetById(id);
  /* FAKEMATCH(wrapper): the slot read through a one-member union, which may
   * alias every store, so its reads stay after the stores before them; as
   * struct WorkA4Slot *, 12 of 378 instructions differ in tla-en. */
  union {
    struct WorkA4Slot s;
  } *slot;
  struct Unk_080db0b0Sprite *sprite;
  s32 swing;
  s32 start;
  s32 dir;
  u16 letGo;
  s32 radius;
  s32 steps;
  s32 angle;
  s32 sine;
  s32 i;

  if (leader == NULL || anchor == NULL)
    return;
  motionFlags = leader->motionFlags;
  heldSprite = leader->sprite;
  if (leader->x < anchor->x)
    side = 1;
  slot = (void *)HangingVine_FindSlot(id);
  if (slot == NULL)
    return;

  start = slot->s.swing;
  if ((slot->s.phase ^ SIDE_TURN(side)) & (TRIG_TURN / 2)) {
    for (swing = start; swing >= 0; swing -= 64) {
      slot->s.swing = swing;
      wait(1);
    }
    start = 0;
  }
  for (swing = start; swing <= 1536; swing += 64) {
    slot->s.swing = swing;
    slot->s.phase = SIDE_TURN(side) + TRIG_TURN / 4;
    wait(1);
  }
  while ((slot->s.phase & 0xff00) != SIDE_TURN(side ^ 1) + TRIG_TURN / 4) {
    slot->s.swing = 1536;
    wait(1);
  }

  sprite = leader->sprite;
  dir = 1;
  if (side) {
    letGo = KEY_DOWN | KEY_LEFT | KEY_B;
    dir -= 2;
  } else
    letGo = KEY_DOWN | KEY_RIGHT | KEY_B;
  slot->s.active = 1;
  Actor_SelectAnimationFar(leader, 32);
  while (1) {
    slot->s.phase = SIDE_TURN(side ^ 1) + TRIG_TURN / 4;
    switch (gFrameCount & 63) {
    case 0:
    case 16:
      slot->s.swing = 1568;
      slot->s.spacing = Q16_ONE * 29 / 2;
      sprite->unk12 = dir << 10;
      break;
    case 1:
    case 2:
    case 17:
    case 18:
      slot->s.swing += 16;
      slot->s.spacing += Q16_ONE / 4;
      sprite->unk12 += dir << 9;
      break;
    default:
      if (slot->s.swing > 1536)
        slot->s.swing -= 32;
      else
        slot->s.swing = 1536;
      if (slot->s.spacing > Q16_ONE * 14)
        slot->s.spacing -= Q16_ONE / 32;
      if (sprite->unk12 != 0)
        sprite->unk12 -= dir << 7;
      break;
    }
    if (gInput.pressed & letGo) {
      sprite->unk12 = 0;
      slot->s.active = 1;
      return;
    }
    if (gInput.pressed != 0)
      break;
    wait(1);
  }

  sprite->unk12 = 0;
  slot->s.active = 0;
  Actor_SelectAnimationFar(leader, 6);
  wait(6);
  Sound_PlayFar(152);
  Actor_SelectAnimationFar(leader, 7);
  leader->speedLimit = Q16_ONE * 3;
  leader->acceleration = Q16_ONE * 2;
  leader->velocityY = Q16_ONE * 4;
  leader->motionFlags &= 0x7e;
  Actor_SetSpriteFlagsFar(leader, 0);
  wait(5);
  Actor_SelectAnimationFar(leader, 16);
  leader->motionFlags = 0;
  rootX = slot->s.root->next->x;
  rootY = slot->s.root->next->y;
  radius = Q16_ONE * 65;
  steps = 8;
  while ((slot->s.phase & 0xff00) != SIDE_TURN(side) + TRIG_TURN * 7 / 32) {
    if (steps != 0) {
      radius += Q16_ONE * 5 / 16;
      slot->s.spacing += Q16_ONE / 16;
      steps--;
    }
    slot->s.swing = 2048;
    slot->s.active = 1;
    // A once-only block: the sine is taken before Iwram_MulQ16 is loaded.
    do {
      sine = Trig_Sin(slot->s.phase - 4000);
    } while (0);
    angle = Iwram_MulQ16(sine, slot->s.swing + slot->s.swing * 5 / 2);
    leader->x = rootX - Iwram_MulQ16(Trig_Sin(angle), radius);
    leader->y = rootY - Iwram_MulQ16(Trig_Cos(angle), radius);
    leader->targetX = leader->x;
    leader->targetY = leader->y;
    heldSprite->unk12 = angle;
    wait(1);
  }
  slot->s.active = 1;
  Actor_SelectAnimationFar(leader, 7);
  leader->motionFlags = motionFlags & ~1;
  leader->velocityY = Q16_ONE * 4;
  for (i = 7; i >= 0; i--) {
    slot->s.spacing -= Q16_ONE / 16;
    wait(1);
  }
  slot->s.spacing = Q16_ONE * 14;
  heldSprite->unk12 = 0;
  Actor_SelectAnimationFar(leader, 6);
  Actor_SetSpriteFlagsFar(leader, 1);
  leader->motionFlags = motionFlags;
  wait(6);
  slot->s.active = 1;
}

/// Hangs a vine from object id (HangingVine_Hang). An object with no motion
/// flags is first moved -16 pixels in z and 64 pixels in y; one with motion
/// flags starts it only when force is set.
void HangingVine_Place(s32 id, s32 force) {
  struct Actor *object = Actor_GetById(id);
  s32 y;
  s32 zero;

  if (object != NULL) {
    if (object->motionFlags == 0) {
      zero = 0;
      object->z += 0xfff00000;
      y = object->y;
      object->y = y + 0x400000;
      HangingVine_Hang(id, zero);
    } else if (force) {
      HangingVine_Hang(id, 0);
    }
  }
}

// Field Psynergy: the dispatchers that run each animation

void FieldPsynergy_HaltOnTarget(s32 id);
void FieldPsynergy_StartMindReadWave(s32 sourceId, s32 targetId);
#define FieldPsynergy_SteerPushedTarget FieldPsynergy_SteerPushedTarget
#define FieldPsynergy_ForceOnTarget FieldPsynergy_ForceOnTarget
#define FieldPsynergy_DouseOnTarget FieldPsynergy_DouseOnTarget
#define FieldPsynergy_FrostOnTarget FieldPsynergy_FrostOnTarget
/* VIEW(f0daecc): routine FieldPsynergy_WhirlwindOnTarget as void
 * FieldPsynergy_WhirlwindOnTarget(s32 targetId); battle.h has void
 * FieldPsynergy_WhirlwindOnTarget(void); not tried:
 * FieldPsynergy_WhirlwindOnTarget's bound declaration was not found. */
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
/// on the chosen target (secondObjectId), 2 finishes it and 3 runs the kind's
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
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  s32 animation = scene->fieldKind;
  s32 targetId = scene->secondObjectId;
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

    paused = party->pausedObject;
    if (paused != -1) {
      FieldPsynergy_ResumeHaltedActor(paused);
      party->pausedObject = -1;
    }
    objectId = Field_FindNearbyPsynergyTarget(scene->firstObjectId, animation);
    objectId = FieldPsynergy_FilterHaltTarget(objectId);
    if (Scene_FindActorTalkTrigger(objectId) != 0) {
      FieldPsynergy_SetupActorPair(scene->firstObjectId, objectId);
      FieldPsynergy_HaltOnTarget(objectId);
      FieldPsynergy_HaltActor(objectId);
      party->pausedObject = objectId;
    } else {
      FieldPsynergy_Halt();
    }
  } else if (animation == FIELD_PSYNERGY_MIND_READ) {
    // A wave still running from the last cast is reset first.
    if (*(s8 *)(event + 0xca4) != 0)
      FieldPsynergy_EndReveal();
    FieldPsynergy_StartMindReadWave(scene->firstObjectId, targetId);
  }
}

/// Runs the field Psynergy animation the scene names on the target the player
/// chose (BattleFxScene.secondObjectId).
void FieldPsynergy_Dispatch(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  s32 animation = scene->fieldKind;
  s32 targetId = scene->secondObjectId;

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

    if (*(s8 *)(event + 0xca4) != 0)
      FieldPsynergy_EndReveal();
    party = &gMainRoutineWork;
    if (party->pausedObject != scene->secondObjectId)
      ((struct Actor *)scene->child)->movementState = 1;
    FieldPsynergy_StartMindReadWave(scene->firstObjectId, targetId);
  } else if (animation == FIELD_PSYNERGY_HALT) {
    // Let the object stopped last time go, then stop the target.
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->pausedObject != -1) {
      FieldPsynergy_ResumeHaltedActor(party->pausedObject);
      party->pausedObject = -1;
    }
    FieldPsynergy_HaltActor(targetId);
    party->pausedObject = targetId;
    FieldPsynergy_HaltOnTarget(targetId);
  }
}

void FieldPsynergy_FinishMindRead(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;

  if (scene->fieldKind == FIELD_PSYNERGY_MIND_READ) {
    struct PartyPauseView *party;

    FieldPsynergy_StopMindReadWave();
    party = (struct PartyPauseView *)&gMainRoutineWork;
    if (party->pausedObject != scene->secondObjectId)
      ((struct Actor *)scene->child)->movementState = 0;
  }
}

/// The extra handler of Reveal, Cloak, Avoid, Sand, Hover and the unnamed kind
/// 28: what each does when it ends.
void FieldPsynergy_RunExtraHandler(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;

  switch (scene->fieldKind) {
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

  state = owner->animationKind;
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
    SET_VECTOR(values, object->x, object->y, object->z);
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

  if (*((s8 *)Ram_HeapSlots->eventWork + 394) == 3) {
    point[0] = object->x;
    out = screen;
    point[1] = object->y;
    point[2] = object->z;
    Render_ProjectPoint(point, out);
    depth = (0x200 - out[2]) / 2 + 128;
    if (depth <= 0)
      depth = 1;
  } else {
    struct DepthMap *map = (struct DepthMap *)Ram_HeapSlots->mapWork;
    struct DepthScroll *scroll = &map->scroll;
    s32 bias = scroll->offset & 0xffff0000;
    s32 base = map->base->base;

    depth = ((object->y - base + object->z - base - bias) >> 16) + 62;
  }
  return depth;
}

/// Per-frame callback: draws the effect scene's sprite (effect work +0x7c8).
void BattleEffect_DrawSceneSprite(void) {
  EffectSprite_Draw((u8 *)Ram_HeapSlots->effectWork + 0x7c8);
}

void Func_080dba5c(void) {
  struct DepthScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *mainObject;
  struct DepthSprite *sprite;
  s32 position[3];
  void *buffer;
  s32 slot;
  s32 tiles;

  mainObject = scene->mainObject;
  if (scene->mode == 0xc000) {
    scene->active = 1;
    buffer = Heap_Alloc(0x200);
    // An empty once-only loop keeps the zero kept in sl after the fill's
    // arguments; without it, 12 of 103 instructions differ in tla-en.
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
    sprite->scale = 0x20000;
    position[0] = scene->x;
    position[1] = mainObject->y + 0x1c0000;
    position[2] = mainObject->z;
    Camera_WorldToScreen(position);
    sprite->x = position[0];
    sprite->y = position[2];
    Task_Add(BattleEffect_DrawSceneSprite, TASK_PASS_VBLANK);
  }
}
#undef CELL_MASK
#undef SIDE_TURN
