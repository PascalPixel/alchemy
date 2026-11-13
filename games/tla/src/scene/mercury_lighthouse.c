/// @file
/// The base of Mercury Lighthouse in Imil, near the end of the game: with all
/// four beacons lit, two of Imil's children hear the voices of the scattered
/// heroes, motes of light drifting round them.

#include "types.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "actor.h"
#include "scene_tables.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field_sprite.h"
#include "system/ram_buffer.h"
#include "system/task.h"
#include "system/io_reg.h"

#include "objcmd.h"
#include "system/frame.h"
#include "event_work.h"

/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

void MercuryLighthouse_HearVoices(void);

extern s32 gMercuryLighthouse_MoteScript[];
extern u32 gMercuryLighthouse_MoteFrames;

void FieldPsynergy_StopMindReadWave(void);

#include "module/scene/scene.h"
#include "text/msg_ids.h"

/// A mote's script call: it flickers between colours 15 and 7 every other
/// frame.
s32 MercuryLighthouse_FlickerMote(struct Actor *object) {
  if ((gMercuryLighthouse_MoteFrames >> 1) & 1)
    FieldActor_SetSpriteColor(object, 15);
  else
    FieldActor_SetSpriteColor(object, 7);
  return 0;
}

/// An Imil child's update while the voices talk: every 40 to 120 frames it
/// turns between two headings, as if looking around for them.
void MercuryLighthouse_LookAround(struct Actor *object) {
  u8 *timer = (u8 *)object + 98;
  u8 *state;

  if (*timer != 0) {
    (*timer)--;
    return;
  }
  *timer = ((Random16() * 80) >> 16) + 40;
  state = (u8 *)object + 99;
  switch (*state) {
  case 0:
    object->facing = FACING_WEST;
    *state = 1;
    break;
  case 1:
    object->facing = FACING_NORTHWEST + FACING_STEP;
    *state = 0;
    break;
  case 2:
    object->facing = 0;
    *state = 3;
    break;
  case 3:
    object->facing = FACING_NORTH + FACING_STEP;
    *state = 2;
    break;
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Actor_StartScript(struct Actor *object, const void *script);

/// The task MercuryLighthouse_HearVoices starts: every 16 frames it starts a
/// flickering mote (sprite 0x2a2) at a random spot in front of the
/// lighthouse, and it counts the frames in gMercuryLighthouse_MoteFrames.
void MercuryLighthouse_SpawnMotes(void) {
  struct Actor *object;
  struct FieldSprite *sprite;
  s32 phase;
  s32 x;
  s32 z;

  Actor_SetSpritePriority(8, 3);
  Actor_SetSpritePriority(9, 3);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  phase = gFrameCount & 15;
  if (phase == 0) {
    object = ((struct Actor *)Import_ActorCreate(0x2a2, 0, 0, 0));
    if (object != NULL) {
      sprite = object->sprite;
      x = Random16() * 40 + 0x32c0000;
      /* FAKEMATCH(temporary): z is built before the motion flag and x are
       * stored, as in the native; as object->z = 0x2b80000, 12 of 76
       * instructions differ in tla-en. */
      z = 0x2b80000;
      object->motionFlags = phase;
      object->x = x;
      object->y = PIXELS(160);
      object->z = z;
      Actor_SetSpriteFlags(object, 0);
      object->priorityFlags &= ~1;
      sprite->priority = 2;
      Actor_StartScript(object, gMercuryLighthouse_MoteScript);
      Actor_SelectAnimation(object, 1);
    }
  }
  gMercuryLighthouse_MoteFrames++;
}

/// With all four beacons lit, two children of Imil at the base of Mercury
/// Lighthouse hear Mia across the world by Mind Read, and the Wise One and
/// Master Hamma join the voices.
/// "We're in Imil! We're at the base of Mercury Lighthouse!"
void MercuryLighthouse_HearVoices(void) {
  struct Actor *actor;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldActor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  wait(1);
  gMercuryLighthouse_MoteFrames = 0;
  Task_Add(MercuryLighthouse_SpawnMotes, TASK_PASS_VBLANK);
  Actor_SetSpritePriority(8, 2);
  Actor_SetSpritePriority(9, 2);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 436) = 40;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  FieldPsynergy_StartMindReadWave(13, 14);
  Event_SetMessageId(MsgImilVoicesDisbeliefReply);
  Event_ShowMessage(3, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_ShowEmote(8, 256, 0);
  Actor_ShowEmote(8, 256, 20);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_Jump(8, 4, 10);
  Event_ShowMessage(0x8008, 0);
  Actor_ShowEmote(11, 257, 0);
  Actor_ShowEmote(12, 257, 40);
  Actor_FaceDirection(11, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(12, FACING_NORTHWEST + FACING_STEP, 0);
  FieldPsynergy_StartMindReadWave(13, 14);
  Event_ShowMessage(3, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_SetAnimation(9, 3);
  Event_ShowMessage(0x8009, 0);
  Actor_FaceDirectionAndWait(12, FACING_EAST);
  Event_ShowMessage(0x800c, 0);
  Actor_FaceDirectionAndWait(11, FACING_WEST);
  Actor_SetAnimation(11, 4);
  Event_ShowMessage(0x400b, 0);
  actor = Actor_GetById(11);
  *((u8 *)actor + 98) = ((Random16() * 80) >> 16) + 40;
  *((u8 *)actor + 99) = 0;
  actor->update = (void (*)(void))MercuryLighthouse_LookAround;
  actor = Actor_GetById(12);
  {
    /* FAKEMATCH(temporary): the timer's + 40 comes before its address, as in
     * the native; as a direct store, 2 of 303 instructions differ in tla-en. */
    u8 timer = ((Random16() * 80) >> 16) + 40;

    *((u8 *)actor + 98) = timer;
  }
  *((u8 *)actor + 99) = 2;
  actor->update = (void (*)(void))MercuryLighthouse_LookAround;
  FieldPsynergy_StartMindReadWave(13, 14);
  Event_ShowMessage(3, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_SetAnimation(8, 4);
  Event_ShowMessage(0x8008, 0);
  FieldPsynergy_StartMindReadWave(13, 14);
  Event_ShowMessage(3, 0);
  FieldPsynergy_StopMindReadWave();
  Event_ShowMessage(0x8009, 0);
  FieldPsynergy_StartMindReadWave(13, 14);
  Event_ShowMessage(3, 0);
  FieldPsynergy_StopMindReadWave();
  Event_ShowMessage(0x8008, 0);
  FieldPsynergy_StartMindReadWave(13, 14);
  Event_ShowMessage(3, 0);
  FieldPsynergy_StopMindReadWave();
  Event_ShowMessage(0x8009, 0);
  FieldPsynergy_StartMindReadWave(13, 14);
  Event_ShowMessage(3, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_SetAnimationAndWait(8, 3);
  Event_ShowMessage(0x8008, 0);
  FieldPsynergy_StartMindReadWave(13, 14);
  Event_ShowMessage(1, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_Jump(9, 2, 0);
  FieldActor_ShowEmote(9, 263, 20);
  Event_ShowMessage(0x8009, 0);
  FieldPsynergy_StartMindReadWave(13, 14);
  Event_ShowMessage(3, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_SetAnimationAndWait(8, 3);
  Event_ShowMessage(0x8008, 0);
  FieldPsynergy_StartMindReadWave(13, 14);
  Event_ShowMessage(6, 0);
  FieldPsynergy_StopMindReadWave();
  Event_Wait(20);
  *(u16 *)BG_PLTT = 0;
  wait(1);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 436) = 16;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0x201;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetFlag(0xa3c);
  Import_EventSetExitRequest(3);
}

s32 Scene_Initialize(void) {
  if (gMainRoutineWork.entrance == 12) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_PlaySound(201);
    BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
    PaletteFade_StartInterpolation(20);
    wait(20);
    Import_EventSetExitRequest(9);
  } else {
    MercuryLighthouse_HearVoices();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

s32 gMercuryLighthouse_MoteScript[] = {
    SC_ADD(SW_Y, PIXELS(-2)),
    SC_CALL(MercuryLighthouse_FlickerMote),
    SC_WAIT(1),
    SC_ADD(SW_Y, PIXELS(-2)),
    SC_CALL(MercuryLighthouse_FlickerMote),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_Y, PIXELS(-1)),
    SC_CALL(MercuryLighthouse_FlickerMote),
    SC_WAIT(1),
    SC_LOOP(40, SC_TO(1)),
    SC_FREE,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(309),      SCENE_EXIT(2, 309, 2), SCENE_EXIT(3, 297, 5),
    SCENE_EXIT(9, 309, 2), SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gActorPlacements[] = {
    {191, CONDITION_ALWAYS, 1, PIXELS(840), 0, PIXELS(696), FACING_SOUTH + FACING_STEP, 2, 0},
    {186, CONDITION_ALWAYS, 1, PIXELS(824), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, 2, 0},
    {190, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTHWEST + FACING_STEP, 2, 0},
    {188, CONDITION_ALWAYS, 1, PIXELS(906), 0, PIXELS(744), FACING_WEST, 2, 0},
    {194, CONDITION_ALWAYS, 1, PIXELS(782), 0, PIXELS(762), FACING_EAST, 2, 0},
    {3, CONDITION_ALWAYS, 1, PIXELS(904), 0, PIXELS(946), FACING_SOUTH, 2, 0},
    {1, CONDITION_ALWAYS, 1, PIXELS(904), 0, PIXELS(946), FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, PIXELS(780), 0, PIXELS(946), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(10), 0, PIXELS(10), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(10), 0, PIXELS(10), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gSceneEvents[] = {
    {TRIGGER_TABLE_END},
};
