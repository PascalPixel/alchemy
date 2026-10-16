// Goma Cave's entrance: moving obstacles, Ivan's reunion and the Whirlwind
// demonstration that clears the party's route into the cave.

#include "unit/scene/goma_entrance.h"
#include "version.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "battle.h"
#include "field.h"
#include "menu.h"
#include "system.h"
#include "field_obj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "create_configured_overlay_object.h"
#include "debug.h"
#include "game.h"
#include "graphics.h"
#include "minigame.h"
#include "sound.h"
#include "edition.h"
#include "call.h"
#include "battle_system.h"
#include "item.h"
#include "inventory.h"
#include "text/msg_ids.h"
#include "objcmd.h"
#include "global_cells.h"
#include "ability_ids.h"

// Private scene actions used by the reunion sequence.
static void GomaEntrance_RunWhirlwindEffect(void);
static void GomaEntrance_RunSpinningLeap(s32 id);
static void GomaEntrance_GiveShamansRod(void);

// Shared effects stay at their native positions in the overlay.
#include "common/effect_create.h"
#include "common/effect_spawn.h"

// A legacy actor view that also reads sprite rotation as a signed halfword.
struct Actor {
  u8 pad00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 pad14[10];
  s16 angle;
  u8 pad20[2];
  u8 flags34;
  u8 pad23[5];
  s32 speed;
  u8 pad2c[12];
  s32 hover;
  u8 pad3c[12];
  s32 accel;
  u8 pad4c[4];
  struct Actor *sprite;
  u8 pad54;
  u8 layer;
  u8 pad56[22];
  s32 callback;
};

struct Vec {
  s32 x;
  s32 y;
  s32 z;
};

struct Member_387 {
  u8 pad[216];
  u16 items[15];
};

struct Item_387 {
  u8 pad[2];
  u16 flags;
  u8 pad4[8];
  u8 equippable;
};

// Shows the next message at the actor and leaves a pause before the next cue.
static void GomaEntrance_ShowActorMessageAndWait(s32 actorId, s32 frames) {
  Engine_EventShowMessage(actorId, 0);
  Engine_EventWait(frames);
}

// Returns the cave entrance's entrance table.
void *GomaEntrance_GetEntrances(void) { return gGomaEntrance_SceneTableA; }

// No exit-region table is used here.
int SceneData_ReturnZero(void) { return 0; }

// Returns the two numbered exit destinations.
void *GomaEntrance_GetExits(void) { return gGomaEntrance_SceneTableB; }

// Returns the obstacles, companions and temporary effect actors.
void *GomaEntrance_GetPlacements(void) { return gGomaEntrance_SceneTableC; }

// Records the moved object's column and restores the cells underneath it.
static void GomaEntrance_UpdateMovableObjectTiles(void) {
  struct FieldActor *object;
  s32 tileX;

  object = Object_GetById(9);
  tileX = object->x.fixed / 0x100000;
  GameFlag_Clear(0x861);
  GameFlag_Clear(0x862);
  if (tileX == 15) {
    Map_CopyCellAttributes(47, 18, 1, 2, 16, 18);
  } else if (tileX == 16) {
    Map_CopyCellAttributes(48, 18, 1, 2, tileX, 18);
    GameFlag_Set(0x861);
  } else {
    Map_CopyCellAttributes(47, 18, 1, 2, 16, 18);
    GameFlag_Set(0x862);
  }
}

// Once actor 10 reaches column 23, makes it stationary and restores its cell.
static void GomaEntrance_SetMovedObjectUnderfoot(void) {
  struct FieldActor *actor;
  s32 tileX;

  actor = Object_GetById(10);
  tileX = actor->x.fixed / 0x100000;
  if (tileX == 23) {
    Engine_EventWait(10);
    Object_GetById(10)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Object_GetById(10)->motionFlags = 0;
    Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
    Map_CopyCellAttributes(54, 17, 1, 1, tileX, 17);
    Engine_GameFlagSet(0x863);
  }
}

// Touch triggers 5 and 6 push the leader and object 16 pixels in opposite
// horizontal directions, then update the object's saved map position.
static void GomaEntrance_PushObjectSideways(void) {
  struct EventWork *work;
  s32 offsetX;

  work = gEventWork;
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 8);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
  Actor_SetSpeed(9, 0x3333, 0x1999);
  Event_PlayCue(185);
  offsetX = (11 - (work->touchedTrigger << 1)) << 4;
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, offsetX, 0);
  Actor_SetDestinationOffset(9, offsetX, 0);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorWaitForMove(9);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  GomaEntrance_UpdateMovableObjectTiles();
  BattleFx_PlayQueuedSound();
  Engine_EventEnd();
}

// Reserved overlay hook; no current scene calls it.
UNUSED static void GomaEntrance_NoOpA(void) {}

// A second reserved hook, also unused by the scene.
UNUSED static void GomaEntrance_NoOpB(void) {}

// Remembers Whirlwind being used on the second temporary effect actor.
static void GomaEntrance_RecordWhirlwindUse(void) {
  Engine_EventBegin();
  Engine_GameFlagSet(0x866);
  Engine_EventEnd();
}

// Returns the Move, Whirlwind and touch handlers for the entrance puzzle.
void *GomaEntrance_GetEvents(void) { return gGomaEntrance_SceneTableD; }

// Reapplies saved obstacle positions and their map cells when re-entering.
s32 GomaEntrance_RestoreEntryState(void) {
  struct FieldActor *movedObject;
  s32 v5;

  Engine_GameFlagSet(0x144);
  Engine_EventWait(10);
  BattleFx_SetQueuedSoundAndPlay(170);
  Engine_ActorSetAnimation(11, 2);
  Object_GetById(11)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  {
    struct FieldActor *record = Object_GetById(8);

    record->collisionFlags = 16 | record->collisionFlags;
  }
  {
    struct FieldActor *record = Object_GetById(15);

    record->collisionFlags = 8 | record->collisionFlags;
  }
  if (Engine_GameFlagIsSet(0x865) != 0) {
    Map_CopyCellAttributes(74, 11, 1, 1, 73, 11);
  }
  if (Engine_GameFlagIsSet(0x860) != 0) {
    Engine_ActorSetPosition(8, 0x880000, 0xc40000);
    Object_GetById(8)->priorityFlags |= 2;
    v5 = 12;
    Engine_ActorSetAnimation(8, 2);
    Map_CopyCellAttributes(39, 12, 3, 1, 8, v5);
    Map_CopyCellAttributes(43, 11, 3, 1, v5, 11);
  }
  if (Engine_GameFlagIsSet(0x861) != 0) {
    Actor_SetPosition(9, 0x1080000, 0x1380000);
    Map_CopyCellAttributes(48, 18, 1, 2, 16, 18);
  } else {
    if (Engine_GameFlagIsSet(0x862) != 0) {
      Actor_SetPosition(9, 0x1180000, 0x1380000);
      Map_CopyCellAttributes(47, 18, 1, 2, 16, 18);
    }
  }
  if (Engine_GameFlagIsSet(0x863) != 0) {
    Engine_ActorSetPosition(10, 0x1780000, 0x1180000);
    ((struct Flags35 *)Object_GetById(10))->flags = 2;
    v5 = 0;
    Object_GetById(10)->motionFlags = v5;
    movedObject = Object_GetById(10);
    Engine_ActorSetSpriteFlags(movedObject, 0);
    Map_CopyCellAttributes(54, 17, 1, 1, 23, 17);
  }
  return 0;
}

// Garet cannot free the obstacle alone. If Ivan has arrived, he demonstrates
// Whirlwind, rejoins the party and receives the Shaman's Rod.
static void GomaEntrance_RecruitIvan(void) {
  struct FieldActor *leader;

#if TBS_PATCH_1
  BattleFx_PlayQueuedSound();
#endif
  Engine_EventBegin();
  Engine_EventWait(30);
  Engine_EventSetMessage(MsgGomaNoUsePsynergy);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  Actor_SetSpeed(GARET, 0xcccc, 0x6666);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 20);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  Actor_WalkToAndWait(GARET, 0x108, 168);
  Actor_FaceDirection(GARET, 0x6000, 20);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(GARET, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(20);
  if (GameFlag_IsSet(0x855) == 0) {
    Engine_ActorSetAnimation(GARET, 2);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(GARET);
    Actor_SetPosition(GARET, 0, 0);
    Engine_EventEnd();
  } else {
    // Ivan joins the conversation and clears the obstacle with Whirlwind.
    Actor_SetPosition(IVAN, 0x1680000, 0xf80000);
    Actor_SetSpeed(IVAN, 0xcccc, 0x6666);
    Actor_WalkToAndWait(IVAN, 0x110, 248);
    Actor_WalkToAndWait(IVAN, 0x110, 208);
    Actor_FaceDirection(IVAN, 0xa000, 20);
    Engine_ActorRunRepeatedMotion(IVAN, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(IVAN, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(GARET, 0x4000, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(GARET, 0x100, 60);
    Engine_ActorSetAnimationAndWait(IVAN, 3);
    Actor_WalkToAndWait(IVAN, 0x108, 200);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 168);
    Actor_WalkToAndWait(IVAN, 248, 184);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(GARET, 0x6000, 0);
    Actor_WalkToAndWait(IVAN, 232, 184);
    Engine_EventWait(20);
    Actor_ShowEmote(IVAN, 0x105, 60);
    Actor_FaceDirection(IVAN, 0xe000, 20);
    Engine_ActorSetAnimationAndWait(IVAN, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(IVAN, 0, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(GARET, 3);
    Engine_EventWait(20);
    Actor_FaceDirection(IVAN, 0x8000, 20);
    Event_ShowMessageAndWait(IVAN, 0, 120);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
    Actor_ShowEmote(GARET, 0x105, 60);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(GARET, 0x6000, 0);
    Engine_EventWait(60);
    Actor_ShowEmote(IVAN, 0x106, 0);
    Engine_ActorStartRepeatedMotion(IVAN, 1);
    Engine_EventWait(30);
    Event_ShowMessageAndWait(IVAN, 0, 30);
    Actor_FaceDirection(IVAN, 0xe000, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(IVAN, 0, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(GARET, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(IVAN, 3);
    Engine_EventWait(20);
    Actor_FaceDirection(IVAN, 0x8000, 20);
    GomaEntrance_RunWhirlwindEffect();
    Engine_ActorSetAnimation(IVAN, 1);
    Engine_EventWait(20);
#if TBS_PATCH_1
    BattleFx_PlayQueuedSound();
#endif
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(GARET, 0x100, 60);
    Engine_ActorJump(GARET, 2, 0);
    Engine_EventWait(20);
    GomaEntrance_ShowActorMessageAndWait(1, 20);
    Actor_WalkToAndWait(IVAN, 0x108, 184);
    Engine_EventWait(10);
    Actor_FaceActor(IVAN, GARET, 0);
    Actor_FaceActor(GARET, IVAN, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(IVAN, 3);
    Engine_EventWait(20);
    GomaEntrance_ShowActorMessageAndWait(2, 60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
    Actor_ShowEmote(GARET, 0x105, 60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(GARET, 0x101, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(GARET, 0x8000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(GARET, 0x4000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Engine_EventWait(10);
    GomaEntrance_ShowActorMessageAndWait(1, 20);
    Actor_ShowEmote(IVAN, 0x105, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(IVAN, 4);
    Engine_EventWait(20);
    GomaEntrance_ShowActorMessageAndWait(2, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
    Actor_FaceActor(GARET, IVAN, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(GARET, 0x102, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(IVAN, 2);
    Engine_EventWait(20);
    GomaEntrance_ShowActorMessageAndWait(2, 30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(GARET, 0x101, 0);
    Engine_EventWait(80);
    Engine_ActorSetAnimationAndWait(IVAN, 3);
    Engine_EventWait(20);
    GomaEntrance_ShowActorMessageAndWait(2, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(GARET, 1);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(GARET, 0x102);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(IVAN, 4);
    Engine_EventWait(20);
    GomaEntrance_ShowActorMessageAndWait(2, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(GARET, 0x8000, 0);
    Engine_EventWait(80);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
    Actor_FaceActor(GARET, IVAN, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(IVAN, 3);
    Engine_EventWait(20);
    GomaEntrance_ShowActorMessageAndWait(2, 30);
    Engine_ActorSetAnimationAndWait(IVAN, 4);
    Engine_EventWait(20);
    GomaEntrance_ShowActorMessageAndWait(2, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(IVAN, 3);
    Engine_EventWait(20);
    GomaEntrance_ShowActorMessageAndWait(2, 40);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(GARET, 3);
    Engine_EventWait(20);
    // Restore Ivan's party membership and equipment before everyone departs.
    Event_PrepareObjectAndApplyValue(IVAN, 1);
    Engine_EventWait(60);
    GomaEntrance_GiveShamansRod();
    Engine_ActorRunRepeatedMotion(IVAN, 1);
    Engine_EventWait(20);
    Actor_WalkToAndWait(IVAN, 248, 184);
    Engine_EventWait(20);
    GomaEntrance_ShowActorMessageAndWait(2, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(GARET, 0x8000, 0);
    Engine_EventWait(120);
    GomaEntrance_ShowActorMessageAndWait(2, 30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
    Actor_FaceActor(GARET, IVAN, 0);
    Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GARET, 3);
    Engine_ActorSetAnimationAndWait(IVAN, 3);
    Engine_EventWait(50);
    Actor_SetSpeed(GARET, 0xcccc, 0x6666);
    Actor_SetSpeed(IVAN, 0xcccc, 0x6666);
    Actor_WalkTo(GARET, 248, 168);
    Actor_WalkToAndWait(IVAN, 248, 168);
    Actor_SetPosition(IVAN, 0, 0);
    Engine_ActorWaitForMove(GARET);
    Actor_SetPosition(GARET, 0, 0);
    Map_CopyCellAttributes(74, 11, 1, 1, 73, 11);
    GameFlag_Set(0x865);
    Engine_EventEnd();
  }
}

// Triggers the spinning leap when the obstacle is in map row 11.
static void GomaEntrance_LeapObjectIntoPlace(void) {
  s32 depth;
  struct OverlayActorState *state;

  Engine_EventBegin();
  depth = ((struct OverlayActorPosition *)Object_GetById(8))->depthFixed >> 20;
  if (depth == 11) {
    GomaEntrance_RunSpinningLeap(8);
    state = Object_GetById(8);
    state->flags |= 2;
    Map_CopyCellAttributes(39, 12, 3, 1, 8, 12);
    Map_CopyCellAttributes(43, 11, 3, 1, 12, depth);
    Engine_GameFlagSet(2144);
  }
  Engine_EventEnd();
}

// Runs Ivan's Whirlwind effect on actor 15 and waits for its dispatcher.
static void GomaEntrance_RunWhirlwindEffect(void) {
  u8 *effectWork;

  effectWork = gEffectWork;
  BattleFx_LoadActionEffectResources(ABILITY_WHIRLWIND, 1);
  BattleFx_SetupObjectPair(2, 15);
  {
    u8 *f = (u8 *)((s32)effectWork + 0x71c);
    s32 t = 8;

    t |= *f;
    *f = t;
  }
  EventObject_Initialize();
  Field_DispatchTypeHandler(1);
  EffectRuntime_StopCurrentObject();
}

// The leaping object's update hook turns its sprite a fixed amount per frame.
static void GomaEntrance_RotateLeapingObject(union FieldObject *fieldObject) {
  struct FieldActor *obj = (struct FieldActor *)fieldObject;
  struct AnimationObject *state = obj->sprite;
  s32 v = state->rotation - 0x800;

  state->rotation = v;
}

// Shared height-wait code stays at its native position in the overlay.
#include "common/object_waitdown.h"

// Spins the obstacle, sends it through two landing points and emits a ring
// of particles before the final stretch effect.
static void GomaEntrance_RunSpinningLeap(s32 id) {
  struct Vec dir;
  struct EffectOptions params;
  struct Vec *v;
  struct EffectOptions *p;
  struct Actor *actor;
  u32 i;
  s32 x;
  s32 z;

  actor = (struct Actor *)Object_GetById(id);
  actor->layer = 0;
  for (i = 0; i <= 17; i++) {
    Engine_TaskWait(1);
    actor->sprite->angle += -256;
    actor->x -= Engine_MathCos((u16)actor->sprite->angle) / 2;
    actor->hover = 0x80000000;
  }
  actor->callback = (s32)GomaEntrance_RotateLeapingObject;
  Actor_SetSpeed(id, 0x30000, 0x18000);
  Engine_ActorSetDestination(id, 160, 192);
  actor->accel = 0xcccc;
  actor->layer = 3;
  actor->flags34 = 0;
  Engine_ActorWaitForMove(id);
  SceneActor_WaitObjectBelowHeight(actor, 0x200000);
  Engine_AudioPlayCue(188);
  Map_SetShake(0x50000, 0x50000, 0x10000);
  Engine_AudioPlayCue(141);
  Map_SetShake(-1, -1, 0xe666);
  for (i = 0; i <= 16; i++) {
    v = &dir;
    v->x = Engine_MathCos(i << 12);
    v->y = 0;
    v->z = Engine_MathSin(i << 12);
    v->x -= v->x / 4;
    v->z -= v->z / 2;
    Effect_Spawn(actor->x, actor->y, actor->z, v->x, v->y, v->z, 0, NULL);
  }
  actor->speed = 0x50000;
  Engine_ActorSetDestination(id, 139, 196);
  Engine_ActorWaitForMove(id);
  SceneActor_WaitObjectBelowHeight(actor, 0x200000);
  actor->callback = 0;
  actor->sprite->angle = 0x1000;
  p = &params;
  p->type = 214;
  p->startScaleX = 0x8000;
  p->startScaleY = 0xcccc;
  p->targetScaleX = 0x18000;
  p->targetScaleY = 0x13333;
  Effect_Spawn(actor->x, actor->y, actor->z, 0, 0, 0, 0x1c0000, p);
  Engine_AudioPlayCue(154);
  Engine_ActorSetAnimation(id, 3);
  Engine_MapWaitWorkValuesBelow256();
}

// Shared inventory equipment code stays at its native overlay position.
#include "common/item_equip.h"

// Gives Ivan the rod, discarding eligible items and retrying if his inventory
// is full. The localized fallback prefers preserving items with byte 3 == 8.
static void GomaEntrance_GiveShamansRod(void) {
  struct Member_387 *member;
  struct Item_387 *item;
  s32 tries;
  s32 i;
  s32 id;

  id = 65;
  member = UnitGet(2);
  tries = 0;
retry:
  if (++tries > 1000) {
#if TBS_PATCH_3
    for (i = 0; i <= 14; i++) {
      item = Item_Get(member->items[i]);
      if (((u8 *)item)[3] != 8) {
        Inventory_Discard(2, i);
        break;
      }
    }
    if (i == 15)
#endif
      member->items[14] = 0;
  }
  if (Inventory_AddItem(2, id) == -1) {
    for (i = 0; i <= 14; i++) {
      item = Item_Get(member->items[i]);
      if (((u8 *)item)[2] == 1) {
        Inventory_Discard(2, i);
        goto retry;
      }
    }
    for (i = 0; i <= 14; i++) {
      item = Item_Get(member->items[i]);
      if ((item->flags & 0x8ff) == 0 && item->equippable == 1) {
        Inventory_Discard(2, i);
        goto retry;
      }
    }
    goto retry;
  }
  for (i = 0; i <= 14; i++) {
    if (member->items[i] == id)
      Inventory_Equip(2, i);
  }
}

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gGomaEntrance_SceneTableCData[] = {
    SC_SET(SW_SCALE_Y, 0x19999),
    SC_END,
};

struct SceneEntrance gGomaEntrance_SceneTableA[] = {
    {0, CONDITION_ALWAYS, 192, 0, 197, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 96, 0, 440, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 200, 0, 184, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gGomaEntrance_SceneTableB[] = {
    SCENE_EXITS(25),
    SCENE_EXIT(1, 28, 1),
    SCENE_EXIT(2, 2, 4),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gGomaEntrance_SceneTableC[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0xd4, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(192), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd4, CONDITION_ALWAYS, 7,
        PIXELS(136), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd7, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0xd7, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_WEST, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xd4, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, 0x865, 7,
        PIXELS(191), 0, PIXELS(194), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0xf8, 0x866, (s32)gGomaEntrance_SceneTableCData,
        PIXELS(200), 0, PIXELS(168), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gGomaEntrance_SceneTableD[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_EAST), 5, CONDITION_ALWAYS, EVENT_SCRIPT(GomaEntrance_PushObjectSideways) },
    { TOUCH_WALKING_INTO(FACING_WEST), 6, CONDITION_ALWAYS, EVENT_SCRIPT(GomaEntrance_PushObjectSideways) },
    { PSYNERGY_ON_ACTOR(0x8c), 8, 0x865, EVENT_SCRIPT(GomaEntrance_RecruitIvan) },
    { PSYNERGY_ON_ACTOR(0x8c), 8, 0x860, EVENT_SCRIPT(GomaEntrance_LeapObjectIntoPlace) },
    { PSYNERGY_ON_ACTOR(0x8c), 9, CONDITION_ALWAYS, EVENT_SCRIPT(GomaEntrance_UpdateMovableObjectTiles) },
    { PSYNERGY_ON_ACTOR(0x8c), 10, 0x863, EVENT_SCRIPT(GomaEntrance_SetMovedObjectUnderfoot) },
    { PSYNERGY_ON_ACTOR(0x4e), 16, 0x866, EVENT_SCRIPT(GomaEntrance_RecordWhirlwindUse) },
    { SCENE_EVENTS_END },
};
// clang-format on
