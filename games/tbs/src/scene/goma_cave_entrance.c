// Goma Cave's entrance: the stump caught in ivy, the two pillars, and Ivan,
// who rejoins the party and blows the ivy away with Whirlwind.
#include "unit/scene/goma_cave_entrance.h"
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
#include "macros.h"
#include "flags.h"
#include "item_ids.h"

// The room's actors: Garet and Ivan take their party slots, the rest follow
// the placement table from 8.
#define STUMP 8
#define WEST_PILLAR 9
#define EAST_PILLAR 10
#define STUMP_IVY 15 // the ivy holding the stump
#define IVY 16

// Ivan's Shaman's Rod: item names are message 386 plus the item number.
#define SHAMANS_ROD 65

// Private scene actions used by the reunion sequence.
static void GomaCaveEntrance_WhirlwindOnStumpIvy(void);
static void GomaCaveEntrance_LeapStump(s32 id);
static void GomaCaveEntrance_GiveShamansRod(void);

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

// Show the next message over the speaker, then pause.
static void GomaCaveEntrance_SayThenWait(s32 actorId, s32 frames) {
  Engine_EventShowMessage(actorId, 0);
  Engine_EventWait(frames);
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gGomaCaveEntranceEntrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gGomaCaveEntranceExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  return gGomaCaveEntrancePlacements;
}

// Move on the west pillar, and after each push: the pillar stands on one of
// three columns, and the map cells and flags follow it.
static void Event_MoveWestPillar(void) {
  struct FieldActor *pillar;
  s32 column;

  pillar = Object_GetById(WEST_PILLAR);
  column = pillar->x.fixed / PIXELS(16);
  GameFlag_Clear(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP);
  GameFlag_Clear(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS);
  if (column == 15) {
    Map_CopyCellAttributes(47, 18, 1, 2, 16, 18);
  } else if (column == 16) {
    Map_CopyCellAttributes(48, 18, 1, 2, column, 18);
    GameFlag_Set(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP);
  } else {
    Map_CopyCellAttributes(47, 18, 1, 2, 16, 18);
    GameFlag_Set(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS);
  }
}

// Move on the east pillar: once it reaches column 23 it settles into the floor
// and can be walked over.
static void Event_MoveEastPillar(void) {
  struct FieldActor *pillar;
  s32 column;

  pillar = Object_GetById(EAST_PILLAR);
  column = pillar->x.fixed / PIXELS(16);
  if (column == 23) {
    Engine_EventWait(10);
    Object_GetById(EAST_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Object_GetById(EAST_PILLAR)->motionFlags = 0;
    Engine_ActorSetSpriteFlags(Object_GetById(EAST_PILLAR), 0);
    Map_CopyCellAttributes(54, 17, 1, 1, column, 17);
    Engine_GameFlagSet(FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE);
  }
}

// Walking into the west pillar from the west (trigger 5) or the east
// (trigger 6): the leader leans on it and pushes it one step.
static void Event_PushWestPillar(void) {
  struct EventWork *work;
  s32 offsetX;

  work = gEventWork;
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 8);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
  Actor_SetSpeed(WEST_PILLAR, 0x3333, 0x1999);
  Event_PlayCue(185);
  // 16 pixels east from trigger 5, west from trigger 6.
  offsetX = (11 - (work->touchedTrigger << 1)) << 4;
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, offsetX, 0);
  Actor_SetDestinationOffset(WEST_PILLAR, offsetX, 0);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorWaitForMove(WEST_PILLAR);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_MoveWestPillar();
  BattleFx_PlayQueuedSound();
  Engine_EventEnd();
}

// Reserved overlay hook; no current scene calls it.
UNUSED static void GomaCaveEntrance_NoOpA(void) {}

// A second reserved hook, also unused by the scene.
UNUSED static void GomaCaveEntrance_NoOpB(void) {}

// Whirlwind on the other ivy: it is blown away for good.
static void Event_BlowAwayIvy(void) {
  Engine_EventBegin();
  Engine_GameFlagSet(FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY);
  Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void) {
  return gGomaCaveEntranceEvents;
}

// Put the stump and pillars where the party left them, and the stump's ivy
// away once Ivan has blown it off.
s32 Scene_Initialize(void) {
  struct FieldActor *movedObject;
  s32 v5;

  Engine_GameFlagSet(0x144);
  Engine_EventWait(10);
  BattleFx_SetQueuedSoundAndPlay(170);
  Engine_ActorSetAnimation(11, 2);
  Object_GetById(11)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  {
    struct FieldActor *record = Object_GetById(STUMP);

    record->collisionFlags = 16 | record->collisionFlags;
  }
  {
    struct FieldActor *record = Object_GetById(STUMP_IVY);

    record->collisionFlags = 8 | record->collisionFlags;
  }
  if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_IVAN_REJOINED) != 0) {
    Map_CopyCellAttributes(74, 11, 1, 1, 73, 11);
  }
  if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED) != 0) {
    Engine_ActorSetPosition(STUMP, PIXELS(136), PIXELS(196));
    Object_GetById(STUMP)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    v5 = 12;
    Engine_ActorSetAnimation(STUMP, 2);
    Map_CopyCellAttributes(39, 12, 3, 1, 8, v5);
    Map_CopyCellAttributes(43, 11, 3, 1, v5, 11);
  }
  if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP) != 0) {
    Actor_SetPosition(WEST_PILLAR, PIXELS(264), PIXELS(312));
    Map_CopyCellAttributes(48, 18, 1, 2, 16, 18);
  } else {
    if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS) !=
        0) {
      Actor_SetPosition(WEST_PILLAR, PIXELS(280), PIXELS(312));
      Map_CopyCellAttributes(47, 18, 1, 2, 16, 18);
    }
  }
  if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE) != 0) {
    Engine_ActorSetPosition(EAST_PILLAR, PIXELS(376), PIXELS(280));
    ((struct Flags35 *)Object_GetById(EAST_PILLAR))->flags =
        ACTOR_PRIORITY_UNDERFOOT;
    v5 = 0;
    Object_GetById(EAST_PILLAR)->motionFlags = v5;
    movedObject = Object_GetById(EAST_PILLAR);
    Engine_ActorSetSpriteFlags(movedObject, 0);
    Map_CopyCellAttributes(54, 17, 1, 1, 23, 17);
  }
  return 0;
}

// Move on the stump while the ivy holds it: Garet says their Psynergy is not
// strong enough. Once the thieves in Vault are jailed, Ivan turns up, blows
// the ivy away with Whirlwind and asks to join the party, and gets back the
// Shaman's Rod.
static void Event_IvanRejoins(void) {
  struct FieldActor *leader;

#if TBS_PATCH_1
  BattleFx_PlayQueuedSound();
#endif
  Engine_EventBegin();
  Engine_EventWait(30);
  Engine_EventSetMessage(MsgGomaNoUsePsynergy);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  Actor_SetSpeed(GARET, 0xcccc, 0x6666);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  Actor_WalkToAndWait(GARET, 0x108, 168);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 20);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  if (GameFlag_IsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    // Garet falls back into line.
    Engine_ActorSetAnimation(GARET, ANIM_WALK);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(GARET);
    Actor_SetPosition(GARET, 0, 0);
    Engine_EventEnd();
  } else {
    // Ivan comes up from the south: "You want to move the stump, right?"
    Actor_SetPosition(IVAN, PIXELS(360), PIXELS(248));
    Actor_SetSpeed(IVAN, 0xcccc, 0x6666);
    Actor_WalkToAndWait(IVAN, 0x110, 248);
    Actor_WalkToAndWait(IVAN, 0x110, 208);
    Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
    Engine_ActorRunRepeatedMotion(IVAN, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(IVAN, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(GARET, FACING_SOUTH, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 60);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Actor_WalkToAndWait(IVAN, 0x108, 200);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 168);
    Actor_WalkToAndWait(IVAN, 248, 184);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
    Actor_WalkToAndWait(IVAN, 232, 184);
    Engine_EventWait(20);
    Actor_ShowEmote(IVAN, 0x105, 60);
    Actor_FaceDirection(IVAN, FACING_NORTHEAST, 20);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(IVAN, 0, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    Engine_EventWait(20);
    Actor_FaceDirection(IVAN, FACING_WEST, 20);
    Event_ShowMessageAndWait(IVAN, 0, 120);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
    Actor_ShowEmote(GARET, 0x105, 60);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
    Engine_EventWait(60);
    Actor_ShowEmote(IVAN, 0x106, 0);
    Engine_ActorStartRepeatedMotion(IVAN, 1);
    Engine_EventWait(30);
    Event_ShowMessageAndWait(IVAN, 0, 30);
    Actor_FaceDirection(IVAN, FACING_NORTHEAST, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(IVAN, 0, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Engine_EventWait(20);
    Actor_FaceDirection(IVAN, FACING_WEST, 20);
    // "I have a great idea. Leave it to me!": Whirlwind.
    GomaCaveEntrance_WhirlwindOnStumpIvy();
    Engine_ActorSetAnimation(IVAN, ANIM_STAND);
    Engine_EventWait(20);
#if TBS_PATCH_1
    BattleFx_PlayQueuedSound();
#endif
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 60);
    Engine_ActorJump(GARET, 2, 0);
    Engine_EventWait(20);
    GomaCaveEntrance_SayThenWait(GARET, 20);
    Actor_WalkToAndWait(IVAN, 0x108, 184);
    Engine_EventWait(10);
    Actor_FaceActor(IVAN, GARET, 0);
    Actor_FaceActor(GARET, IVAN, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Engine_EventWait(20);
    GomaCaveEntrance_SayThenWait(IVAN, 60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
    Actor_ShowEmote(GARET, 0x105, 60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(GARET, 0x101, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(GARET, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(GARET, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Engine_EventWait(10);
    GomaCaveEntrance_SayThenWait(GARET, 20);
    Actor_ShowEmote(IVAN, 0x105, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    GomaCaveEntrance_SayThenWait(IVAN, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
    Actor_FaceActor(GARET, IVAN, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(GARET, 0x102, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(IVAN, 2);
    Engine_EventWait(20);
    GomaCaveEntrance_SayThenWait(IVAN, 30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(GARET, 0x101, 0);
    Engine_EventWait(80);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Engine_EventWait(20);
    GomaCaveEntrance_SayThenWait(IVAN, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(GARET, 1);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(GARET, 0x102);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    GomaCaveEntrance_SayThenWait(IVAN, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_FaceDirection(GARET, FACING_WEST, 0);
    Engine_EventWait(80);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
    Actor_FaceActor(GARET, IVAN, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Engine_EventWait(20);
    GomaCaveEntrance_SayThenWait(IVAN, 30);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    GomaCaveEntrance_SayThenWait(IVAN, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Engine_EventWait(20);
    GomaCaveEntrance_SayThenWait(IVAN, 40);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    Engine_EventWait(20);
    // "Please, allow me to join your quest.": Ivan rejoins with his rod.
    Event_PrepareObjectAndApplyValue(IVAN, 1);
    Engine_EventWait(60);
    GomaCaveEntrance_GiveShamansRod();
    Engine_ActorRunRepeatedMotion(IVAN, 1);
    Engine_EventWait(20);
    Actor_WalkToAndWait(IVAN, 248, 184);
    Engine_EventWait(20);
    GomaCaveEntrance_SayThenWait(IVAN, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(GARET, FACING_WEST, 0);
    Engine_EventWait(120);
    GomaCaveEntrance_SayThenWait(IVAN, 30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
    Actor_FaceActor(GARET, IVAN, 0);
    Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(GARET, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Engine_EventWait(50);
    Actor_SetSpeed(GARET, 0xcccc, 0x6666);
    Actor_SetSpeed(IVAN, 0xcccc, 0x6666);
    Actor_WalkTo(GARET, 248, 168);
    Actor_WalkToAndWait(IVAN, 248, 168);
    Actor_SetPosition(IVAN, 0, 0);
    Engine_ActorWaitForMove(GARET);
    Actor_SetPosition(GARET, 0, 0);
    Map_CopyCellAttributes(74, 11, 1, 1, 73, 11);
    GameFlag_Set(FLAG_GOMA_CAVE_IVAN_REJOINED);
    Engine_EventEnd();
  }
}

// Move on the freed stump: from row 11 it spins, leaps away and lies where it
// lands.
static void Event_MoveStump(void) {
  s32 depth;
  struct OverlayActorState *state;

  Engine_EventBegin();
  depth =
      ((struct OverlayActorPosition *)Object_GetById(STUMP))->depthFixed >> 20;
  if (depth == 11) {
    GomaCaveEntrance_LeapStump(STUMP);
    state = Object_GetById(STUMP);
    state->flags |= 2;
    Map_CopyCellAttributes(39, 12, 3, 1, 8, 12);
    Map_CopyCellAttributes(43, 11, 3, 1, 12, depth);
    Engine_GameFlagSet(FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED);
  }
  Engine_EventEnd();
}

// Ivan casts Whirlwind on the stump's ivy, and the scene waits for it.
static void GomaCaveEntrance_WhirlwindOnStumpIvy(void) {
  u8 *effectWork;

  effectWork = gEffectWork;
  BattleFx_LoadActionEffectResources(ABILITY_WHIRLWIND, 1);
  BattleFx_SetupObjectPair(IVAN, STUMP_IVY);
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

// The leaping stump's update hook turns its sprite a fixed amount per frame.
static void GomaCaveEntrance_SpinStump(union FieldObject *fieldObject) {
  struct FieldActor *obj = (struct FieldActor *)fieldObject;
  struct AnimationObject *state = obj->sprite;
  s32 v = state->rotation - 0x800;

  state->rotation = v;
}

// Shared height-wait code stays at its native position in the overlay.
#include "common/object_waitdown.h"

// The stump spins in place, leaps and lands with a shake and a ring of dust,
// then comes to rest a little further on.
static void GomaCaveEntrance_LeapStump(s32 id) {
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
  actor->callback = (s32)GomaCaveEntrance_SpinStump;
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
    SET_POSITION(v, Engine_MathCos(i << 12), 0, Engine_MathSin(i << 12));
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

// Give Ivan the Shaman's Rod and equip it. With his items full, he drops
// another to make room; later patches stop dropping items whose byte 3 is 8.
static void GomaCaveEntrance_GiveShamansRod(void) {
  struct Member_387 *member;
  struct Item_387 *item;
  s32 tries;
  s32 i;
  s32 id;

  id = SHAMANS_ROD;
  member = UnitGet(IVAN);
  tries = 0;
retry:
  if (++tries > 1000) {
#if TBS_PATCH_3
    for (i = 0; i <= 14; i++) {
      item = Item_Get(member->items[i]);
      if (((u8 *)item)[3] != 8) {
        Inventory_Discard(IVAN, i);
        break;
      }
    }
    if (i == 15)
#endif
      member->items[14] = 0;
  }
  if (Inventory_AddItem(IVAN, id) == -1) {
    for (i = 0; i <= 14; i++) {
      item = Item_Get(member->items[i]);
      if (((u8 *)item)[2] == 1) {
        Inventory_Discard(IVAN, i);
        goto retry;
      }
    }
    for (i = 0; i <= 14; i++) {
      item = Item_Get(member->items[i]);
      if ((item->flags & 0x8ff) == 0 && item->equippable == 1) {
        Inventory_Discard(IVAN, i);
        goto retry;
      }
    }
    goto retry;
  }
  for (i = 0; i <= 14; i++) {
    if (member->items[i] == id)
      Inventory_Equip(IVAN, i);
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

s32 gGomaCaveEntranceTallIvyScript[] = {
    SC_SET(SW_SCALE_Y, 0x19999),
    SC_END,
};

struct SceneEntrance gGomaCaveEntranceEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 197, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 96, 0, 440, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 200, 0, 184, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gGomaCaveEntranceExits[] = {
    SCENE_EXITS(25),
    SCENE_EXIT(1, 28, 1),
    SCENE_EXIT(2, 2, 4),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gGomaCaveEntrancePlacements[] = {
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
    { 0xf8, FLAG_GOMA_CAVE_IVAN_REJOINED, 7,
        PIXELS(191), 0, PIXELS(194), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0xf8, FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY, (s32)gGomaCaveEntranceTallIvyScript,
        PIXELS(200), 0, PIXELS(168), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gGomaCaveEntranceEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_EAST), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushWestPillar) },
    { TOUCH_WALKING_INTO(FACING_WEST), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushWestPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), STUMP, FLAG_GOMA_CAVE_IVAN_REJOINED, EVENT_SCRIPT(Event_IvanRejoins) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), STUMP, FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED, EVENT_SCRIPT(Event_MoveStump) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), WEST_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveWestPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), EAST_PILLAR, FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE, EVENT_SCRIPT(Event_MoveEastPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), IVY, FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY, EVENT_SCRIPT(Event_BlowAwayIvy) },
    { SCENE_EVENTS_END },
};
// clang-format on
