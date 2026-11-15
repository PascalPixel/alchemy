/// @file
/// Prox, Felix's home in the frozen north, where the clan of Puelle lives in
/// poverty in the shadow of the Mars Lighthouse, in six rooms that share this
/// overlay (SceneId_Prox1 to SceneId_Prox6).
///
/// While the lighthouse is unlit a storm rages over the village. Up the
/// riverbed the Venus Djinni Mold bolts into the snow until Scoop digs it out;
/// in the upper village the party overhears the talk of freeing Felix's
/// parents and meets Puelle. After the ending the storm is over, the children
/// play tag, the people give thanks for the beacon and the party says its
/// farewell at the south gate before the credits.
#include "../field/actor_set_sprite_priority.c"
#include "resource_ids.h"
#include "field/sprite.h"
#include "field/actor_script.h"

#include "../field/field_effect_move.c"
#include "types.h"
#include "flags.h"
#include "djinn_ids.h"
#include "system/main_routine_work.h"
#include "field/field_sprite.h"
#include "system/field_effect.h"
#include "field/event_inline.h"
#include "shared/tables.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"
#include "field/actor.h"
#include "version.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "scene_ids.h"
#include "field/call.h"
#include "system/input.h"
#include "system/frame.h"
#include "system/heap_release.h"
#include "window/item_icon.h"
#include "field/field_bank.h"
#include "scene/magma_rock.h"
#include "system/system_bank.h"
#include "field/particle.h"
#include "graphics/resource.h"
#include "system/io_reg.h"
#include "field/field_event.h"
#include "fixed_math.h"
#include "system/iwram_call.h"
#include "system/task.h"

#define FLAG_HEARD_OF_PARENTS 0x9c0 ///< overheard the talk of freeing them
#define FLAG_CLAN_SPOKE_OF_LIGHTHOUSE 0x9c1
#define FLAG_MET_PUELLE 0x9c2
#define FLAG_THANKED_FOR_BEACON 0x9c5 ///< after the ending, in the house
#define FLAG_SAID_FAREWELL 0x9cc
#define FLAG_MOLD_HID_IN_SNOW 0xa2d
/// The door the mayor's men came out of stays open.
#define FLAG_DOOR_LEFT_OPEN FLAG_SCENE_TEMP_00
/// Set while the Mars Lighthouse is unlit: rain and storm over the village.
#define FLAG_STORM_RAGING FLAG_SCENE_TEMP_01
#define FLAG_DIG_SPOT_SCOOPED FLAG_AREA_TEMP_00

void Actor_SelectAnimation(struct Actor *object, s32 argument);
extern void Actor_StartScript(struct FieldEffect *effect, const s32 *script);
extern s32 gFieldEffect_DurationScripts[];
void Actor_SetSpriteFlags(struct Actor *object, u32 value);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct FieldEffect *party =
      ((struct FieldEffect *)Actor_GetById(gMainRoutineWork.leaderActorId));
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Import_ActorCreate(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 targetX;
    s32 count;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - 0x10000) / count;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

/// Mold, the Venus Djinni, joins the party.
void Prox_MoldJoins(void) {
  Djinn_JoinParty(0x14, DJINNI_MOLD / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_MOLD));
}

/// The Mercury Djinni kept in the house joins the party.
void Prox_MercuryDjinniJoins(void) { Djinn_JoinParty(0x18, 1, 0x13); }

#include "shared/distance.c"

#undef FIELD_AT

#ifndef TLA_PROX_LOCAL_H
#define TLA_PROX_LOCAL_H

/// Overlay 6A8's scene layer: the declarations its scenes share and the
/// inline helpers some of their calls go through, as overlay 694's do.

#include "types.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"

#define EVENT_WORK gFieldWork

#endif

extern s16 gProx_Room1FarewellPlacements[];
extern u32 gProx_Room2Placements[];
extern struct ActorPlacement gProx_Room3Placements[];
extern s32 gProx_Room3PlacementsOnceLit[];
extern u32 gProx_Room5Placements[];
extern u32 gProx_Room5PlacementsOnceLit[];
extern u32 gProx_Room6Placements[];
extern struct Trigger gProx_Triggers[];
extern struct Trigger gProx_Room1Triggers[];
extern struct Trigger gProx_Room2Triggers[];
extern struct Trigger gProx_Room3Triggers[];
extern struct Trigger gProx_Room5Triggers[];
extern struct Trigger gProx_Room6Triggers[];

extern u32 gProx_KradenStepsUpToPuelle[];

extern u32 gProx_StepBackFromPuelle[];
extern s32 gProx_EnterDoorScript[];

/// The lower village: after the ending its three children play tag.
void Prox_InitializeRoom2(void);

void Prox_InitializeRoom3(void);

/// The houses: the thanks for the beacon on the first visit after the ending.
void Prox_InitializeRoom4(void);

/// The basements: the leader comes down their stairs.
void Prox_InitializeRoom5(void);
extern u32 gProx_FirstChildRuns[];
extern u32 gProx_SecondChildRuns[];
extern u32 gProx_ThirdChildRuns[];
/// The leader comes down the stairs into a basement.
void Prox_ComeDownStairs(s32 value);
extern u32 gProx_KradenStepsForward[];
extern u32 gProx_SecondParentWalksSouth[];
extern u32 gProx_ThirdParentWalksSouth[];
extern u32 gProx_LeftFileWalksSouth[];
extern u32 gProx_JennaWalksSouth[];
extern u32 gProx_LeaderWalksSouth[];
extern u32 gProx_KradenWalksSouth[];
extern u32 gProx_JennaRunsAhead[];
void Leader_StopIfPending(void);
void Particles_UpdateFalling(void);

void Prox_MercuryDjinniJoins(void);

void Prox_PraiseReturnedWarrior(void);

void Prox_FearForWeyard(void);
void Prox_TalkToPlayingChild(void);

void Prox_TalkToTrainingChild(void);

void Prox_SearchForParents(void);

void Prox_DecideToGoToLighthouse(void);

void Prox_MeetPuelle(void);

/// Touched, Mold bolts up the riverbed and hides under a pile of snow.
void Prox_MoldHidesInSnow(void);

/// Scoop on the pile of snow digs Mold out again.
void Prox_ScoopUpMold(void);
void Prox_BeginScoop(void);
void Prox_EndScoop(void);

void Prox_OpenDoor2(void);

void Prox_OpenDoor3(void);
void Prox_OpenDoor5(void);
void Prox_OpenDoor6(void);

void Prox_OpenDoor7(void);

void Prox_OpenDoor8(void);

void Prox_OpenUpperDoor4(void);
/// The leader climbs the stairs out of a basement.
void Prox_ClimbStairs(void);
void Prox_SayFarewell(void);
void Leader_SlideOnIce(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

/// Puffs rise from the roofs of the village while the storm is over.
void Prox_PuffFromRoofs(void);

#include "text/msg_ids.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

void Actor_SelectAnimation(struct Actor *object, s32 mode);

/// Runs a child at its target while it is near and ahead, and says whether it
/// is chasing.
s32 Prox_ChaseTarget(struct Actor *object, struct Actor *target, s32 range,
                     s32 force) {
  s32 result = 0;
  s32 *targetPos;
  s32 *objectPos;

  if (BYTE_AT(object, 99) != 0) {
    object->paused = 1;
    return 1;
  }
  targetPos = &target->x.fixed;
  objectPos = &object->x.fixed;
  if (SceneActor_CalculateFixedPointDistance(targetPos, objectPos) < range ||
      force != 0) {
    u32 angle = (u16)ArcTan2(target->z.fixed - object->z.fixed,
                             *targetPos - *objectPos);
    u32 left = (angle - 0x1000) & 0xf000;
    u32 right = (angle + 0x1000) & 0xf000;
    u32 ahead = angle & 0xf000;
    u32 facing = object->facing & 0xf000;

    if (ahead == facing || right == facing || left == facing || force != 0) {
      /* FAKEMATCH(temporary): the count and movementState's address held in
       * locals before the test. */
      s16 count = gFieldWork->blockedSteps;
      u8 *movementState = &object->paused;

      if (count > 120) {
        *movementState = 0;
        Actor_SelectAnimation(object, 2);
      } else {
        *movementState = 1;
        Actor_SelectAnimation(object, 1);
        result = 1;
      }
    }
  } else {
    object->paused = 0;
    Actor_SelectAnimation(object, 2);
  }
  return result;
}

#if TLA_PATCH_1

/// The children's game of tag after the ending: each runs at the child it
/// chases, and stops when the leader comes up close (from patch 1).
s32 Prox_PlayTag(struct Actor *actor) {
  u8 *tiles = Ram_HeapSlots->windowWork;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 flag = 0;
  struct Actor *target;

  target = Actor_Get(gMainRoutineWork.leaderActorId);
  if (SceneActor_CalculateFixedPointDistance(&target->x.fixed,
                                             &actor->x.fixed) <= 11) {
    actor->paused = flag;
    Actor_SelectAnimation(actor, 2);
    return 0;
  }
  if (actor->timer == 0)
    target = Actor_Get(18);
  else if (actor->timer == 1)
    target = Actor_Get(16);
  else
    target = Actor_Get(17);
  if (Prox_ChaseTarget(actor, target, 24, 0) == 0) {
    target = Actor_Get(gMainRoutineWork.leaderActorId);
    if ((work->talk != 0 || tiles[4] != 0) && (actor->timer & 2))
      flag = 1;
    Prox_ChaseTarget(actor, target, 32, flag);
  }
  return 0;
}

#else

s32 Prox_PlayTag(struct Actor *actor) {
  u8 *tiles = Ram_HeapSlots->windowWork;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s16 *action = &actor->timer;
  s32 flag = 0;
  struct Actor *target;

  if (*action == 0)
    target = Actor_Get(18);
  else if (*action == 1)
    target = Actor_Get(16);
  else
    target = Actor_Get(17);
  if (Prox_ChaseTarget(actor, target, 24, 0) == 0) {
    target = Actor_Get(gMainRoutineWork.leaderActorId);
    if ((work->talk != 0 || tiles[4] != 0) && (*action & 2))
      flag = 1;
    Prox_ChaseTarget(actor, target, 32, flag);
  }
  return 0;
}

#endif

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern u32 gProx_Room1Placements[];
extern u32 gProx_Room4Placements[];
/* VIEW(prox): variable gProx_Room4PlacementsOnceLit as extern struct
 * ActorPlacement gProx_Room4PlacementsOnceLit[]; the definition has s16
 * gProx_Room4PlacementsOnceLit[] = { 175, - 1, 3, 0, 0, 104, 0, 0, 0, 196,
 * 0x8000, 0, 172, - 1, 1, 0, 0, 128, 0, 0, 0, 106, 0xb000, 1, 178, - 1, 1, 0,
 * 0...; tried as the definition's extern s16 gProx_Room4PlacementsOnceLit[], it
 * does not compile in tla-ja: games/tla/src/scene/prox.c:366: request for
 * member `talkFacing' in something not a structure or union. */
extern struct ActorPlacement gProx_Room4PlacementsOnceLit__prox[] __asm__(
    "gProx_Room4PlacementsOnceLit");

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_PROX_1 && Event_GetFlag(FLAG_THANKED_FOR_BEACON) &&
      !Event_GetFlag(FLAG_SAID_FAREWELL))
    return gProx_Room1FarewellPlacements;
  scene = gMainRoutineWork.next;
  if (scene == SCENE_PROX)
    return gProx_Room2Placements;
  if (scene == SCENE_PROX_MOLD) {
    if (Event_GetFlag(FLAG_MARS_BEACON_LIT))
      return gProx_Room3PlacementsOnceLit;
    return gProx_Room3Placements;
  }
  if (scene == SCENE_PROX_4) {
    if (Event_GetFlag(FLAG_MARS_BEACON_LIT)) {
      gProx_Room4PlacementsOnceLit__prox[1].talkFacing = 3;
      gProx_Room4PlacementsOnceLit__prox[4].talkFacing = 3;
      return gProx_Room4PlacementsOnceLit__prox;
    }
    return gProx_Room4Placements;
  }
  if (scene == SCENE_PROX_5) {
    if (Event_GetFlag(FLAG_MARS_BEACON_LIT))
      return gProx_Room5PlacementsOnceLit;
    return gProx_Room5Placements;
  }
  if (scene == SCENE_PROX_6)
    return gProx_Room6Placements;
  return gProx_Room1Placements;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

/// Scoop on the black dot under the boulder digs up its treasure.
void Prox_ScoopUpDarkMatter(s32 a0) {
  Event_DigUpTreasure(a0, FIELD_TREASURE_ACTOR, FLAG_DIG_SPOT_SCOOPED);
}

extern struct Trigger gProx_Room4Triggers[];

const u8 *Scene_GetTriggers(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_PROX_1)
    return gProx_Room1Triggers;
  if (scene == SCENE_PROX)
    return gProx_Room2Triggers;
  if (scene == SCENE_PROX_MOLD)
    return gProx_Room3Triggers;
  if (scene == SCENE_PROX_4)
    return gProx_Room4Triggers;
  if (scene == SCENE_PROX_5)
    return gProx_Room5Triggers;
  if (scene == SCENE_PROX_6)
    return gProx_Room6Triggers;
  return gProx_Triggers;
}

s32 Prox_EndEvent();

void Prox_AskAboutIgloos(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgProxFelixYouLikeIgloosRight);
  Event_AskYesNo(15, 0);
  Prox_EndEvent();
}

#include "shared/leader_is_facing_north.h"

/// Prox's shops, as Shop_Run numbers them.
enum { SHOP_PROX = 0 };

/// Prox's inn, as Inn_CheckIn numbers the inns.
enum { PROX_INN = 0 };

void Prox_TalkAboutParents(void) {

  if (Leader_IsFacingNorth()) {
    Inn_CheckIn(0xe, 0x16);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    if (Event_GetFlag(FLAG_MARS_BEACON_LIT)) {
      FieldEvent_SetMessageId((s32)MsgProxWeHadNoIdeaThere);
    } else {
      FieldEvent_SetMessageId((s32)MsgProxFelixYourParentsHaveGone);
    }
    Event_ShowMessage(22, 0);
    Prox_EndEvent();
  }
}

s32 Shop_Run();

void Prox_PraiseReturnedWarrior(void) {

  if (Leader_IsFacingNorth()) {
    Shop_Run(0x1f, 0xa);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    if (Event_GetFlag(FLAG_MARS_BEACON_LIT)) {
      FieldEvent_SetMessageId((s32)MsgProxYourFosterGrandmotherKnew);
    } else {
      FieldEvent_SetMessageId((s32)MsgProxYouGoAwayForLittle);
    }
    Event_ShowMessage(10, 0);
    Prox_EndEvent();
  }
}

s32 Sanctum_Run();

void Prox_FearForWeyard(void) {

  if (Leader_IsFacingNorth()) {
    Sanctum_Run(8);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    if (Event_GetFlag(FLAG_MARS_BEACON_LIT)) {
      FieldEvent_SetMessageId((s32)MsgProxNowThatLighthouseHasBeen);
    } else {
      FieldEvent_SetMessageId((s32)MsgProxIHearAgatioAndKarst);
    }
    Event_ShowMessage(8, 0);
    Prox_EndEvent();
  }
}

/// The bytes of an actor these handlers use. unk63 is the last of the four
/// scene-owned bytes the Broken Seal's actors keep at this place.
struct TalkActor {
  u8 unk00[6];
  u16 angle; ///< 1/65536 turns
  u8 unk08[0x5b];
  u8 unk63; ///< Prox_ChaseTarget reads it
};

void Prox_TalkToPlayingChild(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct TalkActor *object = ((struct TalkActor *)Actor_Get(16));
  s16 angle = object->angle;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  object->unk63 = 1;
  if (work->psynergy == 0) {
    Actor_FaceActor(16, gMainRoutineWork.leaderActorId, 0);
    FieldEvent_SetMessageId((s32)MsgProxWeeeeWhatFunNow);
  } else {
    FieldEvent_SetMessageId((s32)MsgProxNowThatLighthouseIsLit);
  }
  // "Now that the lighthouse is lit, we don't have to play in that stupid..."
  Event_ShowMessage(16, 0);
  if (work->psynergy == 0)
    object->angle = angle;
  object->unk63 = 0;
  Prox_EndEvent();
}

void Prox_TalkToRestlessChild(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct TalkActor *object = ((struct TalkActor *)Actor_Get(17));
  s16 angle = object->angle;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  object->unk63 = 1;
  if (work->psynergy == 0) {
    Actor_FaceActor(17, gMainRoutineWork.leaderActorId, 0);
    FieldEvent_SetMessageId((s32)MsgProxIMissRunningAroundOutside);
  } else {
    FieldEvent_SetMessageId((s32)MsgProxThatBlizzardWasUnreal);
  }
  // "That blizzard was unreal, but it's gone now, and the weather's..."
  Event_ShowMessage(17, 0);
  if (work->psynergy == 0)
    object->angle = angle;
  object->unk63 = 0;
  Prox_EndEvent();
}

void Prox_TalkToTrainingChild(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct TalkActor *object = ((struct TalkActor *)Actor_Get(18));
  s16 angle = object->angle;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  object->unk63 = 1;
  if (work->psynergy == 0) {
    Actor_FaceActor(18, gMainRoutineWork.leaderActorId, 0);
    FieldEvent_SetMessageId((s32)MsgProxIfImGoing);
  } else {
    FieldEvent_SetMessageId((s32)MsgProxYouGetMoreExerciseRunning);
  }
  // "You get more exercise running in the snow, because it's a lot harder..."
  Event_ShowMessage(18, 0);
  if (work->psynergy == 0)
    object->angle = angle;
  object->unk63 = 0;
  Prox_EndEvent();
}

void Prox_SearchForParents(void) {
  s32 t0;
  s32 t8;

  t0 = Event_GetFlag(FLAG_MARS_BEACON_LIT);
  if (t0 != 0) {
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Event_PlaySound(SE_DOOR_OPEN);
    MapGrid_CopyCellsTo(89, 0, 77, 16, 1, 2);
    Event_SetFlag(FLAG_DOOR_LEFT_OPEN);
    FieldEvent_Wait(10);
    {
      s32 y = 0xc0;
      Actor_SetPositionAndFacing(8, px(520), px(360), y << 6);
    }
    t8 = ((s32)FieldCamera_GetActor());
    FIELD_AT((t8 + 0x55), u8, 0) = t0;
    FieldCamera_SetSpeed(1.6);
    FieldCamera_MoveTo(552, CAMERA_KEEP, 388, 1);
    FieldEvent_Wait(10);
    Actor_SetSpeed(8, 1.2);
    Actor_WalkToAndWait(8, 526, 384);
    Actor_FaceDirection(9, FACING_WEST, 0);
    Actor_WalkToAndWait(8, 540, 388);
    Actor_FaceDirectionAndWait(8, FACING_EAST);
    Actor_SetAnimation(8, 4);
    FieldEvent_SetMessageId((s32)MsgProxTheyreNotHere);
    // "They're not here..."
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 8, 0);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    // "Strange... We looked all over the village, and we can't find any..."
    Event_ShowMessage(9, 0);
    Actor_SetAnimation(8, 3);
    // "Everyone says they've practically vanished without a trace... It's..."
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 8, 0);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
    Actor_FaceDirection(9, FACING_NORTH_NORTHWEST, 0);
    // "You don't think they..."
    Event_ShowMessage(9, 0);
    Actor_FaceDirection(8, FACING_NORTH_NORTHWEST, 40);
    Actor_FaceDirectionAndWait(8, FACING_EAST);
    Actor_SetAnimation(8, 4);
    // "They wouldn't have gone to the lighthouse..."
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 8, 0);
    Actor_FaceDirectionAndWait(9, FACING_WEST);
    Actor_SetAnimationAndWait(9, 3);
    // "Yeah, you're probably right."
    Event_ShowMessage(9, 0);
    Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    // "Puelle's going to be upset that we didn't follow his order to free..."
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 8, 0);
    Actor_SetAnimation(9, 3);
    // "He was clear about it, too... "When Felix returns to Prox, set his..."
    Event_ShowMessage(9, 0);
    Actor_FaceDirection(9, FACING_NORTH_NORTHWEST, 0);
    Actor_FaceDirectionAndWait(8, FACING_NORTH_NORTHWEST);
    Event_SetFlag(FLAG_HEARD_OF_PARENTS);
    Prox_EndEvent();
  }
}

void FieldCamera_WaitForArrival(void);

void Prox_DecideToGoToLighthouse(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(160, CAMERA_KEEP, 160, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  Actor_RunRepeatedMotion(10, 1);
  Actor_FaceDirectionAndWait(10, FACING_NORTH_NORTHWEST);
  FieldEvent_SetMessageId((s32)MsgProxWeCantWaitForAgatio);
  // "We can't wait for Agatio and his men any longer..."
  Event_ShowMessage(10, 0);
  Actor_SetAttachedEffect(12, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(13, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(14, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(15, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(16, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(17, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(10, FACING_SOUTH_SOUTHWEST);
  // "If we wait much longer, this wind and snow will make it impossible..."
  Event_ShowMessage(10, 0);
  // "I'm going to the lighthouse... Will any of you come with me?"
  Event_ShowMessage(10, 0);
  Actor_SetAttachedEffect(12, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(13, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(14, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(15, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(16, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(17, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  Actor_SetAnimation(12, 4);
  // "Puelle, I can't let you go on your own..."
  Event_ShowMessage(12, 0);
  Actor_FaceDirection(10, FACING_SOUTH_SOUTHEAST, 0);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_SetAnimationAndWait(13, 4);
  // "If something were to happen to you, who would lead our village?"
  Event_ShowMessage(13, 0);
  FieldActor_Jump(15, 4, 10);
  // "Please, you must let us go in your stead."
  Event_ShowMessage(15, 0);
  Actor_FaceDirection(10, FACING_SOUTH_SOUTHWEST, 0);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "How can you expect to accomplish what Agatio and Karst could not?"
  Event_ShowMessage(10, 0);
  Actor_ShowEmote(16, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
  // "They may have been more powerful than any of us, but we can't just..."
  Event_ShowMessage(16, 0);
  FieldActor_Jump(14, 2, 0);
  Actor_SetAnimation(14, 3);
  // "They're not coming back, just like Saturos... You have to let us try."
  Event_ShowMessage(14, 0);
  // "I can't. I can't allow it."
  Event_ShowMessage(11, 0);
  Actor_FaceDirectionAndWait(10, FACING_WEST);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Actor_FaceDirectionAndWait(10, FACING_SOUTH_SOUTHWEST);
  // "Listen to your elder... He speaks wisely."
  Event_ShowMessage(10, 0);
  Actor_FaceDirectionAndWait(11, FACING_EAST);
  // "I'm speaking to you too, Puelle."
  Event_ShowMessage(11, 0);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirectionAndWait(10, FACING_WEST);
  Actor_SetAnimation(11, 4);
  // "If Agatio and Karst cannot do the task, how can you, a single man..."
  Event_ShowMessage(11, 0);
  Actor_SetAttachedEffect(10, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_FaceDirection(13, FACING_EAST, 0);
  Actor_FaceDirection(14, FACING_WEST, 0);
  Actor_FaceDirection(15, FACING_EAST, 0);
  Actor_FaceDirection(16, FACING_WEST, 0);
  Actor_FaceDirection(17, FACING_EAST, 40);
  Actor_FaceDirection(12, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(13, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(14, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(15, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(16, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(17, FACING_NORTH_NORTHEAST, 20);
  // "What are you saying? If we stand around debating, our town is doomed..."
  Event_ShowMessage(10, 0);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  // "We can't risk sacrificing any more lives."
  Event_ShowMessage(11, 0);
  Actor_FaceDirection(10, FACING_NORTH_NORTHWEST, 80);
  Actor_SetAnimationAndWait(10, 4);
  // "So, we must sit here and wait for a miracle to come."
  Event_ShowMessage(10, 0);
  Event_SetFlag(FLAG_CLAN_SPOKE_OF_LIGHTHOUSE);
  Prox_EndEvent();
}

s32 Actor_MoveToObject();
extern u32 gProx_IsaacStepsUpToPuelle[];

void Prox_MeetPuelle(void) {
  u32 i;
  s32 result;
  s32 owner;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(160, CAMERA_KEEP, 160, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 134, 156);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_NORTH);
  FieldActor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirectionAndWait(11, FACING_SOUTH_SOUTHWEST);
  FieldEvent_SetMessageId((s32)MsgProxYouFelix);
  // "You... Felix, it's you..."
  Event_ShowMessage(11, 0);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(10, FACING_WEST, 40);
  // "I'd heard that you had returned to us... You look so much stronger..."
  Event_ShowMessage(10, 0);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 144, 154);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_MoveToObject(0x12, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_ISAAC, gMainRoutineWork.leaderActorId);
  System_Wait(1);
  Actor_FaceDirection(14, FACING_NORTH_NORTHWEST, 0);
  Actor_SetSpeed(18, 0.8);
  Actor_SetSpeed(ACTOR_ISAAC, 0.8);
  Actor_SetScript(ACTOR_ISAAC, (s32)gProx_IsaacStepsUpToPuelle);
  Actor_RunScriptAndWait(18, (s32)gProx_KradenStepsUpToPuelle);
  Actor_FaceDirection(11, FACING_SOUTH_SOUTHEAST, 0);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  // "It seems he has brought some allies with him."
  Event_ShowMessage(11, 0);
  Actor_SetAnimation(18, 3);
  // "Yes... I am Kraden."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 18, 0);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  // "My name is Isaac."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC,
                    0);
  Actor_FaceDirection(11, FACING_EAST, 0);
  Actor_FaceDirection(10, FACING_WEST, 40);
  Actor_FaceDirectionAndWait(11, FACING_SOUTH_SOUTHEAST);
  // "Isaac... Isn't that..."
  Event_ShowMessage(11, 0);
  Actor_FaceDirectionAndWait(10, FACING_SOUTH_SOUTHWEST);
  // "That is the name of the one Agatio said had killed Saturos and Menardi."
  Event_ShowMessage(10, 0);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_FaceDirection(13, FACING_EAST, 0);
  Actor_FaceDirection(14, FACING_WEST, 0);
  Actor_FaceDirection(15, FACING_EAST, 0);
  Actor_FaceDirection(16, FACING_WEST, 0);
  Actor_FaceDirection(17, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
  Actor_SetSpeed(ACTOR_ISAAC, 2.4);
  Actor_Get(ACTOR_ISAAC)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_ISAAC, 160, 140);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_ISAAC)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetAnimationAndWait(ACTOR_ISAAC, 0x15);
  Actor_FaceDirection(12, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(13, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(14, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(15, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(16, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(17, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(18, FACING_NORTH_NORTHWEST);
  // "Isaac, stop that..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 18, 0);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_STILL);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_STAND);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
  Actor_SetAnimation(18, 3);
  // "You are correct, but Isaac had reasons for fighting them."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 18, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_SetSpeed(ACTOR_ISAAC, 0.8);
  Actor_WalkToAndWait(ACTOR_ISAAC, 160, 154);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHWEST, 0);
  Actor_SetAnimationAndWait(18, 4);
  // "Saturos and Menardi had invaded Vale, stolen its sacred treasure..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 18, 0);
  FieldActor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "You cannot blame Isaac for pursuing them and fighting to regain what..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 18, 0);
  FieldActor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "We only wanted to stop them... We didn't think they'd fight us to..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC,
                    0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(18, FACING_WEST, 40);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
  // "They flung themselves into the opening of Venus Lighthouse's beacon..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC,
                    0);
  // Waits up to 80 frames, or until a key is held.
  i = 0;
  goto test_wait;
next_wait:
  i++;
test_wait:
  if (i < 80) {
    System_Wait(1);
    if (gInput.held == 0)
      goto next_wait;
  }
  Actor_RunRepeatedMotion(12, 1);
  Event_ShowMessage(12, 0);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  FieldActor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Event_ShowMessage(15, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(11, FACING_EAST);
  Actor_SetAnimation(13, 3);
  Event_ShowMessage(13, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  Actor_FaceDirection(11, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(10, FACING_SOUTH_SOUTHWEST, 0);
  Actor_SetAnimationAndWait(16, 3);
  Event_ShowMessage(16, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHWEST);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Actor_SetAnimation(14, 3);
  Event_ShowMessage(14, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(17, 4);
  Event_ShowMessage(17, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTHWEST, 20);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(18, FACING_NORTH_NORTHWEST, 20);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  Event_ShowMessage(11, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 40);
  Actor_SetAnimationAndWait(10, 3);
  Event_ShowMessage(10, 0);
  Actor_FaceDirectionAndWait(18, FACING_NORTH_NORTHEAST);
  Actor_SetAnimation(18, 3);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 18, 0);
  Actor_SetAnimation(11, 3);
  Event_ShowMessage(11, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC,
                    0);
  Actor_SetAnimation(10, 3);
  Event_ShowMessage(10, 0);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 18, 0);
  Actor_SetAnimation(11, 4);
  Event_ShowMessage(11, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_WEST);
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 0, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  result =
      FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0);
  owner = gMainRoutineWork.leaderActorId;
  if (result == 0) {
    Actor_WaitForAnimationChange(owner);
    Event_ShowMessage(
        SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
    EVENT_WORK->message++;
  } else {
    FieldActor_WaitForMoveById(owner);
    EVENT_WORK->message++;
    Event_ShowMessage(
        SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
  }
  FieldActor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_FaceDirectionAndWait(18, FACING_WEST);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 18, 0);
  Actor_FaceDirection(11, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(10, FACING_WEST);
  Actor_SetAnimation(11, 3);
  Actor_SetAnimationAndWait(10, 3);
  Actor_FaceDirection(11, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(10, FACING_SOUTH_SOUTHWEST);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessage(10, 0);
  Actor_SetAnimation(11, 3);
  Event_ShowMessage(11, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(18, FACING_NORTHWEST);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimationAndWait(18, 3);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_FaceDirection(13, FACING_EAST, 0);
  Actor_FaceDirection(14, FACING_WEST, 0);
  Actor_FaceDirection(15, FACING_EAST, 0);
  Actor_FaceDirection(16, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(17, FACING_EAST);
  Actor_SetAnimation(12, 3);
  Actor_SetAnimation(13, 3);
  Actor_SetAnimation(14, 3);
  Actor_SetAnimation(15, 3);
  Actor_SetAnimation(16, 3);
  Actor_SetAnimationAndWait(17, 3);
  Actor_FaceDirectionAndWait(18, FACING_WEST);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 18, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetScript(ACTOR_ISAAC, (s32)gProx_StepBackFromPuelle);
  Actor_RunScriptAndWait(18, (s32)gProx_StepBackFromPuelle);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 160, 154);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_NORTH);
  Actor_RunRepeatedMotion(10, 1);
  Event_ShowMessage(10, 0);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 20);
  Actor_SetAnimation(10, 4);
  Event_ShowMessage(10, 0);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirectionAndWait(11, FACING_EAST);
  Event_ShowMessage(11, 0);
  Actor_FaceDirectionAndWait(10, FACING_WEST);
  Actor_SetAnimationAndWait(10, 3);
  Actor_FaceDirectionAndWait(10, FACING_SOUTH_SOUTHWEST);
  Event_ShowMessage(10, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_FaceDirectionAndWait(11, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationAndWait(11, 3);
  Event_ShowMessage(11, 0);
  Event_SetFlag(FLAG_MET_PUELLE);
  Prox_EndEvent();
}

void Prox_MoldHidesInSnow(void) {
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 2);
  Actor_SetSpeed(20, 2);
  object = Actor_Get(20);
  object->layer = 1;
  FieldActor_SetSpritePriority(20, 3);
  object = Actor_Get(gMainRoutineWork.leaderActorId);
  object->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 408, 326);
  FieldEvent_Wait(1);
  object = Actor_Get(gMainRoutineWork.leaderActorId);
  object->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Event_PlaySound(SE_RATTLE);
  object = (struct Actor *)((s32)FieldCamera_GetActor());
  object->y.fixed += px(2);
  object = Actor_Get(20);
  object->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(20, 408, 200);
  FieldEvent_Wait(1);
  object = Actor_Get(20);
  object->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetPosition(20, 0, 0);
  Event_PlaySound(SE_KOLIMA_BRIDGE_HOP);
  MapGrid_SetShake(1, 2, 1);
  FieldEvent_Wait(30);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_PlaySound(SE_LANDING_THUD);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_ENEMY_CRY);
  FieldEvent_Wait(20);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  FieldActor_SetSpritePriority(20, 2);
  object = Actor_Get(20);
  object->priorityFlags |= 1;
  MapGrid_CopyCellsTo(72, 3, 69, 3, 3, 3);
  MapGrid_CopyCellsTo(48, 36, 69, 8, 3, 2);
  MapGrid_CopyCellAttributes(72, 13, 1, 3, 70, 20);
  Event_SetFlag(FLAG_MOLD_HID_IN_SNOW);
  Prox_EndEvent();
}

void Prox_ScoopUpMold(void) {
  if (Event_GetFlag(FLAG_MARS_BEACON_LIT) == 0 &&
      Event_GetFlag(FLAG_DJINNI(DJINNI_MOLD)) == 0) {
    u8 *work = Ram_HeapSlots->fieldWork;

    Event_Start();
    FieldEvent_ResolvePendingActions(1);
    {
      s32 y = 0x80;
      Actor_SetPositionAndFacing(20, px(408), px(145), y << 7);
    }
    System_Wait(1);
    FieldActor_Jump(20, 4, 20);
    Actor_FaceActor(20, gMainRoutineWork.leaderActorId, 0);
    FIELD_AT(work, u16, 362) = 200;
    Prox_EndEvent();
  }
}

void Prox_BeginScoop(void) { MapGrid_CopyCellAttributes(25, 2, 1, 1, 25, 9); }

void Prox_EndScoop(void) { MapGrid_CopyCellAttributes(25, 3, 1, 1, 25, 9); }

struct RisingObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  u8 unk10[8];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[0x44];
  s16 drift;
  u8 unk66[2];
  s32 life;
};

void Actor_Release(struct RisingObject *object);

void Prox_RisePuff(struct RisingObject *object) {
  object->x += object->drift << 8;
  object->y += 0x8000;
  object->scaleX += 0x51e;
  object->scaleY += 0x51e;
  object->drift += 2;
  if (--object->life == 0)
    Actor_Release(object);
}

/// Adds one puff rising from a roof.
void Prox_AddPuff(s32 a0, s32 a1, s32 a2) {
  s32 t0;

  t0 = FieldActor_Create(0x11d, a0, a1, a2);
  if (t0 != 0) {
    FIELD_AT((t0 + 0x5c), u8, 0) = 2;
    FIELD_AT((t0 + 0x55), u8, 0) = 0;
    FIELD_AT(((t0 + 0x55) + 0xf), u16, 0) = 0;
    FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
    FieldActor_SetSpriteColor(t0, 7);
    FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
        ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
    Actor_SetSpriteFlags(t0, 0);
    FIELD_AT(t0, s32, 24) = 0x8000;
    FIELD_AT(t0, s32, 28) = 0x8000;
    FIELD_AT(t0, s32, 104) = 0x3c;
    FIELD_AT(t0, s32, 108) = (s32)Prox_RisePuff;
    Actor_SelectAnimation(t0, 5);
    Actor_SetBlendMode(t0, 1);
  }
}

void Prox_PuffFromRoofs(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_PROX) {
    if ((gFrameCount & 63) == 0)
      Prox_AddPuff(536 << 16, 40 << 16, 232 << 16);
    if (((gFrameCount + 60) & 63) == 0)
      Prox_AddPuff(168 << 16, 40 << 16, 296 << 16);
    if (((gFrameCount + 20) & 63) == 0) {
      Prox_AddPuff(280 << 16, 42 << 16, 184 << 16);
      Prox_AddPuff(152 << 16, 48 << 16, 424 << 16);
    }
    if (((gFrameCount + 40) & 63) == 0)
      Prox_AddPuff(336 << 16, 42 << 16, 362 << 16);
  } else if (scene == SCENE_PROX_MOLD) {
    if ((gFrameCount & 63) == 0)
      Prox_AddPuff(568 << 16, 104 << 16, 296 << 16);
  }
}

void Prox_EnterDoor(s32 a, s32 b, s32 c, s32 d, s32 mode, s32 f) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *object;
  s32 cell;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (!Event_GetFlag(FLAG_DOOR_LEFT_OPEN)) {
    if (mode == 2)
      Event_PlaySound(SE_THUMP);
    else
      Event_PlaySound(SE_DOOR_OPEN);
    MapGrid_CopyCellsTo(a, b, c, d, mode, f);
    FieldEvent_Wait(20);
  }
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  if (mode == 1) {
    object = Actor_Get(gMainRoutineWork.leaderActorId);
    cell = object->x.fixed >> 16;
    FieldActor_SetMoveTargetFixed(object,
                                  ((8 - cell % 16) << 16) + object->x.fixed,
                                  object->y.fixed, object->z.fixed);
    FieldActor_WaitForMove(object);
    object->facing = FACING_NORTH;
  }
  Actor_SetScript(gMainRoutineWork.leaderActorId, gProx_EnterDoorScript);
  System_Wait(12);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(work->stepTrigger);
}

void Prox_OpenDoor2(void) { Prox_EnterDoor(0x30, 0x27, 0x39, 0x1a, 1, 2); }

void Prox_OpenDoor3(void) { Prox_EnterDoor(0x31, 0x27, 0x41, 0x18, 1, 2); }

void Prox_OpenDoor5(void) { Prox_EnterDoor(0x32, 0x27, 0x38, 0x13, 1, 2); }

void Prox_OpenDoor6(void) { Prox_EnterDoor(0x33, 0x27, 0x3d, 0xd, 1, 2); }

void Prox_OpenDoor7(void) { Prox_EnterDoor(0x33, 0x27, 0x50, 0x10, 1, 2); }

void Prox_OpenDoor8(void) { Prox_EnterDoor(0x34, 0x27, 0x32, 9, 2, 2); }

void Prox_OpenDoor10(void) { Prox_EnterDoor(0x57, 2, 0x50, 7, 1, 2); }

void Prox_OpenUpperDoor4(void) { Prox_EnterDoor(0x59, 0, 0x4d, 0x10, 1, 2); }

void Prox_ClimbStairs(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 entrance = work->stepTrigger;
  struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 cell;
  s32 id;

  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  cell = object->x.fixed >> 16;
  FieldActor_SetMoveTargetFixed(object,
                                ((8 - cell % 16) << 16) + object->x.fixed,
                                object->y.fixed, object->z.fixed - px(4));
  FieldActor_WaitForMove(object);
  object->facing = FACING_NORTH;
  Event_PlaySound(SE_STAIRS_UP);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.leaderActorId), 0);
  id = gMainRoutineWork.leaderActorId;
  if (entrance == 6)
    Actor_SetAnimation(id, 25);
  else
    Actor_SetAnimation(id, 26);
  System_Wait(4);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(entrance);
}

void Prox_ComeDownStairs(s32 a0) {
  struct Actor *object;

  object = Actor_Get(gMainRoutineWork.leaderActorId);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (a0 != 0) {
    Actor_SetSpriteFlags(((s32)Actor_Get(gMainRoutineWork.leaderActorId)), 0);
    FieldActor_SetPositionAndResetMotion(object, (object->x.fixed + -65536),
                                         object->y.fixed,
                                         (object->z.fixed + -393216));
    System_Wait(1);
    Event_StartScreenTransition();
    Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, 0x17);
  } else {
    FieldActor_SetPositionAndResetMotion(object, (object->x.fixed + px(1)),
                                         object->y.fixed,
                                         (object->z.fixed + -393216));
    System_Wait(1);
    Event_StartScreenTransition();
    Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, 0x18);
  }
  Actor_SetSpriteFlags(((s32)Actor_Get(gMainRoutineWork.leaderActorId)), 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 0, 8);
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  Prox_EndEvent();
}
#undef BYTE_AT
#undef FIELD_AT

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void FieldCamera_WaitForArrival(void);
s32 Prox_EndEvent();

void Prox_GiveThanksForBeacon(void) {
  s32 t144;
  s32 t159;
  s32 t202;
  s32 t207;
  s32 angleC000;
  s32 angleB000;
  s32 angle3000;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldActor_Stop(20);
  System_Wait(1);
  Actor_SetPosition(gMainRoutineWork.leaderActorId, 184, 484);
  /* FAKEMATCH(temporary): the three facings held in locals set before their
   * first placement, where as constants each placement rebuilds them and the
   * later turns cannot share them. */
  angleC000 = FACING_NORTH;
  angleB000 = FACING_NORTH_NORTHWEST;
  Actor_SetPositionAndFacing(ACTOR_ISAAC, px(168), px(484), angleC000);
  Actor_SetPositionAndFacing(27, px(200), px(484), angleB000);
  angle3000 = FACING_SOUTH_SOUTHEAST;
  Actor_SetPositionAndFacing(17, px(138), px(412), angle3000);
  Actor_SetPositionAndFacing(20, px(220), px(600), angleB000);
  Actor_SetPosition(ACTOR_JENNA, 184, 684);
  System_Wait(1);
  EVENT_WORK->transition = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(184, CAMERA_KEEP, 448, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId((s32)MsgProxThankYouFelix);
  // "Thank you, Felix. And you, too, Isaac and Kraden."
  Event_ShowMessage(26, 0);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(27, 3);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_FaceDirection(26, FACING_SOUTH_SOUTHWEST, 0);
  FieldActor_ShowEmote(26, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  // "By igniting Mars Lighthouse, you stopped Gaia Falls from devouring..."
  Event_ShowMessage(26, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 20);
  FieldActor_ShowEmote(gMainRoutineWork.leaderActorId,
                       EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_NORTH);
  // "We're just glad to have helped save Weyard..."
  Event_ShowMessage(ACTOR_ISAAC, 0);
  Actor_SetAttachedEffect(25, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "You've been through many trials, more than any of us can ever know..."
  Event_ShowMessage(25, 0);
  Actor_SetAnimation(27, 4);
  // "We had no idea what to expect when we lit the final beacon..."
  Event_ShowMessage(27, 0);
  Actor_SetAnimation(19, 4);
  // "When we saw the flash of the beacon, we had no idea what you..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 19, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "I thought we were done for... There was no way we could escape with..."
  Event_ShowMessage(ACTOR_ISAAC, 0);
  Actor_FaceDirection(27, FACING_WEST, 20);
  FieldActor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  // "Well, at least, not if we hadn't pulled together and overcome our..."
  Event_ShowMessage(27, 0);
  // "That light was brilliant!"
  Event_ShowMessage(17, 0);
  Actor_FaceDirection(27, FACING_NORTH_NORTHWEST, 0);
  FieldActor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "You know, I shall always regret that I didn't get to look upon the..."
  Event_ShowMessage(27, 0);
  Actor_FaceDirectionAndWait(26, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimation(26, 4);
  // "I understand your regret... As a scholar of Alchemy, you would have..."
  Event_ShowMessage(26, 0);
  // "I saw the beams of blue, red, and purple streaming out toward the..."
  Event_ShowMessage(ACTOR_ISAAC, 0);
  Actor_SetAttachedEffect(19, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "That's the direction in which Vale lies, is it not? Was the light..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 19, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  // "Indeed it was. The Wise One said the beams were going to merge into..."
  Event_ShowMessage(ACTOR_ISAAC, 0);
  Actor_FaceDirection(26, FACING_SOUTH_SOUTHWEST, 0);
  Actor_SetAttachedEffect(26, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "The orb of golden light we saw... That was shining directly over Vale?"
  Event_ShowMessage(26, 0);
  Actor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "Yes. That was the Golden Sun forming over Mt. Aleph."
  Event_ShowMessage(27, 0);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHEAST, 0);
  Actor_ShowEmote(25, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  // "If I remember the tales correctly... The Golden Sun is the blast of..."
  Event_ShowMessage(25, 0);
  Actor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "Such legends exist even here? I wish that I'd known that sooner!"
  Event_ShowMessage(27, 0);
  Actor_FaceDirection(26, FACING_SOUTH_SOUTHEAST, 0);
  // "I fear for Vale, to think that all this energy is being released..."
  Event_ShowMessage(26, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  // "We're all worried about that. We're hoping we can return to Vale as..."
  Event_ShowMessage(ACTOR_ISAAC, 0);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHWEST, 0);
  Actor_ShowEmote(25, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  // "Oh... Is there nothing we can do to convince you to stay?"
  Event_ShowMessage(25, 0);
  Actor_SetAnimation(26, 4);
  // "We had hoped you might stay in Prox a while, but that sounds unlikely..."
  FieldEvent_ShowMessageThenWait(26, 0, 20);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  // "Don't worry... We plan on returning as soon as things quiet down..." Yes:
  // "You have just completed a long and difficult..." No: "Our village will not
  // soon forget that you..."
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | 0, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  if (!FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId,
                                             0)) {
    Actor_SetAnimationAndWait(26, 3);
    // "Our village will not soon forget that you saved us from the edge of..."
    Event_ShowMessage(26, 0);
    EVENT_WORK->message++;
  } else {
    EVENT_WORK->message++;
    Actor_SetAnimationAndWait(26, 4);
    // "You have just completed a long and difficult journey. Perhaps when..."
    Event_ShowMessage(26, 0);
  }
  Actor_FaceDirectionAndWait(27, FACING_WEST);
  Actor_SetAnimation(27, 4);
  // "When that time comes, I may no longer be in this world."
  Event_ShowMessage(27, 0);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHEAST, 0);
  FieldActor_ShowEmote(25, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  // "Don't say such things... I'm sure a long life awaits you."
  Event_ShowMessage(25, 0);
  Actor_FaceDirection(27, FACING_NORTH_NORTHWEST, 20);
  Actor_SetAnimationAndWait(27, 3);
  Actor_FaceDirectionAndWait(27, FACING_WEST);
  // "Well, I suppose we should be departing for Vale soon."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 27, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  // "You're right, Kraden... If we don't, you-know-who might start..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
  Actor_SetAttachedEffect(26, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "At least some of your cares have been remedied... You must be glad..."
  Event_ShowMessage(26, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirection(27, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(25, FACING_WEST);
  Actor_SetAnimationAndWait(25, 3);
  Actor_FaceDirectionAndWait(25, FACING_SOUTH_SOUTHWEST);
  // "I would not have thought it possible had I not seen it."
  Event_ShowMessage(25, 0);
  FieldActor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "Even I do not fully understand it..."
  Event_ShowMessage(27, 0);
  Actor_SetAnimation(27, 4);
  // "But their can be no doubt. The fire Psynergy released by the beacon..."
  Event_ShowMessage(27, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHEAST, 0);
  FieldActor_ShowEmote(gMainRoutineWork.leaderActorId,
                       EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(26, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(25, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_SetAnimationAndWait(27, 3);
  // "I hadn't expected so much Psynergy to be released by the..."
  Event_ShowMessage(27, 0);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimation(26, 3);
  Actor_SetAnimation(25, 3);
  Actor_FaceDirection(27, FACING_WEST, 0);
  Actor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  // "I have no real explanation, other than that their life force had not..."
  Event_ShowMessage(27, 0);
  Actor_ShowEmote(26, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "And had your parents not been Adepts, the wave of Psynergy would..."
  FieldEvent_ShowMessageThenWait(26, 0, 20);
  // "Isaac! What's taking you two so long? Don't keep me waiting!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAttachedEffect(27, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(27, FACING_SOUTH_SOUTHWEST);
  // "Uh-oh! It's Jenna... We're late, and she looks rather displeased..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 27, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
  // "Should I let her know we're leaving now?"
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | 0, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  t144 =
      FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0);
  if (t144 == 0) {
    FieldCamera_SetSpeed(2.4);
    FieldCamera_MoveTo(184, CAMERA_KEEP, 512, 1);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
    Actor_SetSpeed(ACTOR_ISAAC, 1.6);
    Actor_WalkToAndWait(ACTOR_ISAAC, 168, 520);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
    EVENT_WORK->message += 2;
  } else {
    EVENT_WORK->message += 2;
    FieldCamera_SetSpeed(2.4);
    FieldCamera_MoveTo(184, CAMERA_KEEP, 512, 1);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
    Actor_SetSpeed(ACTOR_ISAAC, 1.6);
    Actor_WalkToAndWait(ACTOR_ISAAC, 168, 520);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  }
  t159 = ((s32)FieldCamera_GetActor());
  FIELD_AT((t159 + 0x55), u8, 0) = 0;
  FieldCamera_MoveTo(158, CAMERA_KEEP, 432, 1);
  FieldCamera_WaitForArrival();
  FieldActor_Jump(17, 4, 10);
  Event_ShowMessage(17, 0);
  FieldCamera_SetSpeed(1.2);
  FieldCamera_MoveTo(172, CAMERA_KEEP, 520, 1);
  FieldActor_WatchAndSetScript(ACTOR_ISAAC, 0x11);
  FieldActor_WatchAndSetScript(gMainRoutineWork.leaderActorId, 0x11);
  FieldActor_WatchAndSetScript(27, 0x11);
  Actor_SetSpeed(17, 1.6);
  Actor_WalkToAndWait(17, 140, 478);
  Actor_WalkToAndWait(17, 150, 540);
  Actor_WalkToAndWait(17, 160, 560);
  Actor_WalkToAndWait(17, 190, 640);
  FieldActor_Stop(ACTOR_ISAAC);
  FieldActor_Stop(gMainRoutineWork.leaderActorId);
  FieldActor_Stop(27);
  System_Wait(1);
  Actor_SetPosition(17, 0, 0);
  FieldEvent_Wait(10);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(184, CAMERA_KEEP, 448, 1);
  Actor_WalkToAndWait(ACTOR_ISAAC, 168, 484);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(27, FACING_WEST);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 27, 0);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHWEST, 0);
  Actor_SetAnimationAndWait(25, 3);
  Event_ShowMessage(25, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirection(27, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_NORTH);
  Actor_SetAnimationAndWait(26, 3);
  Event_ShowMessage(26, 0);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationAndWait(27, 3);
  Actor_SetSpeed(ACTOR_ISAAC, 1.6);
  Actor_SetSpeed(27, 1.6);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_WALK);
  t202 = ((s32)Actor_Get(gMainRoutineWork.leaderActorId));
  if (t202 != 0) {
    Actor_SetDestination(ACTOR_ISAAC, FIELD_AT(t202, s16, 10),
                         FIELD_AT(t202, s16, 18));
  }
  FieldActor_WaitForMoveById(ACTOR_ISAAC);
  Actor_SetPosition(ACTOR_ISAAC, 0, 0);
  Actor_SetAnimation(27, 2);
  t207 = ((s32)Actor_Get(gMainRoutineWork.leaderActorId));
  if (t207 != 0) {
    Actor_SetDestination(27, FIELD_AT(t207, s16, 10), FIELD_AT(t207, s16, 18));
  }
  FieldActor_WaitForMoveById(27);
  Actor_SetPosition(27, 0, 0);
  {
    struct FieldWork *work = EVENT_WORK;

    work->transition = 0x209;
    work->transitionFrames = 0x10;
  }
  Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  Event_SetFlag(FLAG_THANKED_FOR_BEACON);
  Prox_EndEvent();
}

void Actor_SetWideSprite(s32 id, s32 wide);
void TilesetAnim_Resume(s32 layer);
void TilesetAnim_Pause(u32 layer);
void FieldEffect_StartBlankOverlay(s32 a);
void Weather_StartRain(s32 a);

s32 Scene_Initialize(void) {
  s32 scene;

  if (Event_GetFlag(FLAG_MARS_BEACON_LIT)) {
    Event_SetFlag(FLAG_CANNOT_SAVE_HERE);
    Event_SetFlag(FLAG_RETREAT_BLOCKED);
  }
  scene = gMainRoutineWork.next;
  if (scene == SCENE_PROX_4) {
    gFieldWork->transition = 0x209;
    Prox_InitializeRoom4();
  } else if (scene == SCENE_PROX_5) {
    gFieldWork->transition = 0x209;
    Prox_InitializeRoom5();
  } else if (scene == SCENE_PROX_6) {
    s32 *effect = &gFieldWork->transition;
    /* FAKEMATCH(pin): the call's argument pinned to the r0 it is passed in. */
    register s32 skip __asm__("r0");
    s32 value = 0x209;

    skip = 0;
    *effect = value;
    FieldEffect_StartBlankOverlay(skip);
  } else {
    if (!Event_GetFlag(FLAG_MARS_BEACON_LIT))
      Event_SetFlag(FLAG_STORM_RAGING);
    if (Event_GetFlag(FLAG_STORM_RAGING)) {
      Weather_StartRain(1);
    } else {
      REG_BLDALPHA = 0xc08;
      REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
      Task_Add(Prox_PuffFromRoofs, TASK_PASS_VBLANK);
      TilesetAnim_Pause(0);
      TilesetAnim_Pause(1);
      TilesetAnim_Resume(2);
      TilesetAnim_Resume(3);
      TilesetAnim_Resume(4);
      TilesetAnim_Resume(5);
      TilesetAnim_Resume(6);
    }
    scene = gMainRoutineWork.next;
    if (scene == SCENE_PROX) {
      if (!Event_GetFlag(FLAG_DIG_SPOT_SCOOPED))
        Actor_SetWideSprite(64, 1);
      Prox_InitializeRoom2();
    } else if (scene == SCENE_PROX_MOLD) {
      Prox_InitializeRoom3();
    }
    if (Event_GetFlag(FLAG_STORM_RAGING)) {
      PaletteFade_SetTarget(0x10004, 1);
      PaletteFade_StartInterpolation(16);
      wait(16);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

struct Walker {
  u8 unk00[0x63];
  u8 arrived;
  u16 frames;
  u8 unk66[6];
  void (*update)(void);
};

/// Puts a child at its place and starts its game of tag.
void Prox_StartPlayingTag(s32 actor, s32 x, s32 z, s32 frames) {
  struct Walker *object = ((struct Walker *)Actor_Get(actor));

  object->frames = frames;
  object->arrived = 0;
  object->update = Prox_PlayTag;
  Actor_SetPosition(actor, x, z);
}

void Prox_InitializeRoom2(void) {

  if (Event_GetFlag(FLAG_MARS_BEACON_LIT)) {
    if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
      Prox_StartPlayingTag(0x10, 536, 360, 0);
      Prox_StartPlayingTag(0x11, 536, 424, 1);
      Prox_StartPlayingTag(0x12, 580, 420, 2);
      System_Wait(1);
      Actor_SetScript(16, (s32)gProx_FirstChildRuns);
      Actor_SetScript(17, (s32)gProx_SecondChildRuns);
      Actor_SetScript(18, (s32)gProx_ThirdChildRuns);
      System_Wait(1);
    }
  }
  FieldActor_SetSpritePriority(20, 1);
  FieldActor_SetSpriteColor(((s32)Actor_Get(20)), 0xf);
}

void Prox_InitializeRoom3(void) {
  struct Actor *object;

  FieldActor_SetSpritePriority(19, 1);
  FieldActor_SetSpriteColor(((s32)Actor_Get(19)), 0xf);
  if (!Event_GetFlag(FLAG_MARS_BEACON_LIT)) {
    if (!Event_GetFlag(FLAG_HEARD_OF_PARENTS)) {
      Actor_SetPosition(8, 0, 0);
    }
    if (Event_GetFlag(FLAG_DOOR_LEFT_OPEN)) {
      MapGrid_CopyCellsTo(89, 0, 77, 16, 1, 2);
    }
    if (Event_GetFlag(FLAG_CLAN_SPOKE_OF_LIGHTHOUSE)) {
      object = Actor_Get(10);
      object->facing = FACING_NORTH_NORTHWEST;
      System_Wait(1);
    }
  }
  if (Event_GetFlag(FLAG_MARS_BEACON_LIT)) {
    MapGrid_CopyCellsTo(72, 3, 69, 3, 3, 3);
    MapGrid_CopyCellsTo(48, 36, 69, 8, 3, 2);
    MapGrid_CopyCellAttributes(72, 13, 1, 3, 70, 20);
    Actor_SetPosition(20, 0, 0);
  } else {
    if (Event_GetFlag(FLAG_MOLD_HID_IN_SNOW)) {
      MapGrid_CopyCellsTo(72, 3, 69, 3, 3, 3);
      MapGrid_CopyCellsTo(48, 36, 69, 8, 3, 2);
      MapGrid_CopyCellAttributes(72, 13, 1, 3, 70, 20);
      if (!Event_GetFlag(FLAG_DJINNI(DJINNI_MOLD))) {
        Actor_SetPosition(20, 0, 0);
      }
    }
  }
}

void Prox_MatchRoom4Priority(void) {
  struct Actor *object;

  object = Actor_Get(gMainRoutineWork.leaderActorId);
  FieldActor_SetSpritePriority(
      16,
      ((u32)(FIELD_AT(FIELD_AT((u8 *)object, s32, 80), u8, 9) << 28) >> 30));
}

void Prox_InitializeRoom4(void) {

  FieldEffect_StartBlankOverlay(0);
  Task_AddFrame((s32)Prox_MatchRoom4Priority, TASK_PASS_VBLANK);
  if (gMainRoutineWork.entrance == 0x14) {
    if (!Event_GetFlag(FLAG_THANKED_FOR_BEACON)) {
      Prox_GiveThanksForBeacon();
    }
  }
}

void Prox_MatchRoom5Priority(void) {
  s32 variant =
      ((struct FieldSprite *)Actor_Get(gMainRoutineWork.leaderActorId)->sprite)
          ->priority;

  FieldActor_SetSpritePriority(8, variant);
  FieldActor_SetSpritePriority(10, variant);
}

void Prox_InitializeRoom5(void) {
  s32 priority;

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
    s32 entrance = (u16)gMainRoutineWork.entrance;

    if (gMainRoutineWork.entrance == 6)
      Prox_ComeDownStairs(0);
    else if ((u32)((entrance - 7) << 16) <= 0x10000)
      Prox_ComeDownStairs(1);
  }
  FieldEffect_StartBlankOverlay(0);
  // The priority in a local is built before the task function's address (passed
  // directly, the address loads first).
  priority = 0x480;
  Task_AddFrame(Prox_MatchRoom5Priority, priority);
}

s32 Actor_MoveToObject();
extern u32 gProx_IsaacStepsForward[];
extern u32 gProx_RightFileWalksSouth[];
extern u32 gProx_GaretWalksSouth[];
extern u32 gProx_IsaacWalksSouth[];
extern u32 gProx_GaretRunsAhead[];

void Prox_SayFarewell(void) {
  u32 i;

  if (Event_GetFlag(FLAG_THANKED_FOR_BEACON) != 0) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    FieldActor_Jump(ACTOR_JENNA, 2, 10);
    FieldActor_Jump(ACTOR_JENNA, 4, 10);
    FieldEvent_SetMessageId((s32)MsgProxWhatTookYouSoLong);
    // "What took you so long? Mom and Dad are tired of waiting for you..."
    Event_ShowMessage(ACTOR_JENNA, 0);
    FieldCamera_SetSpeed(3.2);
    FieldCamera_MoveTo(144, CAMERA_KEEP, 228, 1);
    FieldCamera_WaitForArrival();
    FieldActor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    // "That's not true, Felix. You take as long as you need to say farewell..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 10, 0);
    Event_PlaySound(MUS_SHAMAN_VILLAGE);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 136, 196);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
    Actor_MoveToObject(ACTOR_ISAAC, gMainRoutineWork.leaderActorId);
    Actor_MoveToObject(0xb, gMainRoutineWork.leaderActorId);
    System_Wait(1);
    Actor_SetSpeed(ACTOR_ISAAC, 0.8);
    Actor_SetSpeed(11, 0.8);
    Actor_SetScript(ACTOR_ISAAC, (s32)gProx_IsaacStepsForward);
    Actor_RunScriptAndWait(11, (s32)gProx_KradenStepsForward);
    Actor_SetAnimationAndWait(9, 3);
    // "Don't worry about us, Felix... Puelle and the others took good care..."
    Event_ShowMessage(9, 0);
    Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHWEST);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
    // "Don't tell him that! We'll be stuck here forever!"
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
    FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
    // "You look like you're feeling back to your old self, Jenna!"
    Event_ShowMessage(ACTOR_SHEBA, 0);
    Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_WEST);
    Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
    Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHEAST);
    // "You were weeping such mournful tears after the battle..."
    Event_ShowMessage(ACTOR_PIERS, 0);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
    // "What!? As if! You can't prove anything!"
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
    // "Go easy on her. You have to remember, she thought that both of her..."
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    Actor_FaceDirectionAndWait(10, FACING_NORTH_NORTHEAST);
    // "Oh, were you crying, Jenna?"
    Event_ShowMessage(10, 0);
    Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
    FieldActor_Jump(ACTOR_JENNA, 4, 10);
    // "No! I said... I said I wasn't!"
    Event_ShowMessage(ACTOR_JENNA, 0);
    FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
    Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_NORTHWEST);
    // "Now you've got me thinking about my family... I didn't think I'd..."
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHWEST);
    // "I only hope that they're all safe back in Vale..."
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
    // "I just want to be home again... I want to see how my mother is doing..."
    FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0, 10);
    Actor_FaceDirection(8, FACING_SOUTH_SOUTHEAST, 20);
    // "Don't worry, Son. I'm sure Dora's doing fine. She's a strong woman."
    FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 8, 0, 20);
    if (Event_GetFlag(0x824) != 0) {
      Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
      Actor_SetAnimation(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
      Event_ShowMessage(ACTOR_ISAAC, 0);
      Actor_FaceDirectionAndWait(8, FACING_NORTH_NORTHEAST);
      Event_ShowMessage(8, 0);
      EVENT_WORK->message++;
    } else {
      EVENT_WORK->message += 2;
      Actor_FaceDirectionAndWait(8, FACING_NORTH_NORTHEAST);
      Event_ShowMessage(8, 0);
    }
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
    Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
    Actor_SetAnimationAndWait(11, 3);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 11, 0);
    Actor_FaceDirection(11, FACING_SOUTH_SOUTHEAST, 20);
    Actor_FaceDirection(11, FACING_WEST, 20);
    Actor_FaceDirectionAndWait(11, FACING_SOUTH_SOUTHWEST);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 11, 0);
    Actor_FaceDirectionAndWait(10, FACING_WEST);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | 10, 0);
    Actor_FaceDirectionAndWait(9, FACING_EAST);
    Actor_SetAnimation(9, 3);
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | 9, 0);
    Actor_FaceDirectionAndWait(8, FACING_EAST);
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | 8, 0);
    Actor_SetAnimationAndWait(11, 3);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 11, 0);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
    Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
    Event_OpenMessage(SPEAKER_WINDOW_BELOW | 0, 0);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId,
                                              0) == 0) {
      Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
      Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
      EVENT_WORK->message++;
    } else {
      EVENT_WORK->message++;
      Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
      Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
    }
    Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_NORTHWEST);
    FieldActor_Jump(ACTOR_JENNA, 2, 10);
    FieldActor_Jump(ACTOR_JENNA, 4, 10);
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0);
    Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
    Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_NOD);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
    Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
    Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Actor_SetSpeed(ACTOR_ISAAC, 0.8);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
    Actor_SetSpeed(11, 0.8);
    Actor_SetSpeed(ACTOR_JENNA, 0.8);
    Actor_SetSpeed(ACTOR_SHEBA, 0.8);
    Actor_SetSpeed(ACTOR_PIERS, 0.8);
    Actor_SetSpeed(8, 0.8);
    Actor_SetSpeed(9, 0.8);
    Actor_SetSpeed(10, 0.8);
    Actor_SetSpeed(ACTOR_MIA, 0.8);
    Actor_SetSpeed(ACTOR_IVAN, 0.8);
    Actor_SetSpeed(ACTOR_GARET, 0.8);
    FieldActor_SetSpritePriority(ACTOR_ISAAC, 2);
    FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
    FieldActor_SetSpritePriority(11, 2);
    FieldActor_SetSpritePriority(ACTOR_JENNA, 2);
    FieldActor_SetSpritePriority(ACTOR_SHEBA, 2);
    FieldActor_SetSpritePriority(ACTOR_PIERS, 2);
    FieldActor_SetSpritePriority(8, 2);
    FieldActor_SetSpritePriority(9, 2);
    FieldActor_SetSpritePriority(10, 2);
    FieldActor_SetSpritePriority(ACTOR_MIA, 2);
    FieldActor_SetSpritePriority(ACTOR_IVAN, 2);
    FieldActor_SetSpritePriority(ACTOR_GARET, 2);
    FieldCamera_SetSpeed(0.4);
    FieldCamera_MoveTo(144, CAMERA_KEEP, 246, 1);
    Actor_SetScript(9, (s32)gProx_SecondParentWalksSouth);
    Actor_SetScript(10, (s32)gProx_ThirdParentWalksSouth);
    FieldEvent_Wait(10);
    Actor_SetScript(8, (s32)gProx_LeftFileWalksSouth);
    Actor_SetScript(ACTOR_MIA, (s32)gProx_RightFileWalksSouth);
    FieldEvent_Wait(10);
    Actor_SetScript(ACTOR_SHEBA, gProx_LeftFileWalksSouth);
    Actor_SetScript(ACTOR_PIERS, (s32)gProx_LeftFileWalksSouth);
    FieldEvent_Wait(10);
    Actor_SetScript(ACTOR_IVAN, gProx_RightFileWalksSouth);
    Actor_SetScript(ACTOR_GARET, (s32)gProx_GaretWalksSouth);
    FieldEvent_Wait(30);
    Actor_SetScript(ACTOR_JENNA, (s32)gProx_JennaWalksSouth);
    FieldEvent_Wait(20);
    Actor_SetScript(ACTOR_ISAAC, (s32)gProx_IsaacWalksSouth);
    Actor_SetScript(gMainRoutineWork.leaderActorId,
                    (s32)gProx_LeaderWalksSouth);
    FieldEvent_Wait(10);
    Actor_RunScriptAndWait(11, (s32)gProx_KradenWalksSouth);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
    Actor_FaceDirection(11, FACING_NORTH_NORTHWEST, 0);
    FieldActor_WaitForScriptEndById(ACTOR_JENNA);
    Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 40);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
    Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId,
                                              0) == 0) {
      Actor_SetAnimationAndWait(11, 3);
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
      EVENT_WORK->message++;
    } else {
      EVENT_WORK->message++;
      Actor_SetAnimationAndWait(11, 4);
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    }
    Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 20);
    Event_ShowMessage(ACTOR_ISAAC, 0);
    Actor_SetAnimation(11, 4);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    FieldActor_Jump(ACTOR_GARET, 2, 10);
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTHEAST, 0);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_FaceDirection(11, FACING_SOUTH_SOUTHEAST, 20);
    Event_ShowMessage(11, 0);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_SetAnimationAndWait(11, 4);
    Event_ShowMessage(11, 0);
    FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(11, 0);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 40);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
    Event_ShowMessage(ACTOR_ISAAC, 0);
    Actor_FaceDirectionAndWait(11, FACING_NORTH_NORTHWEST);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId,
                                              0) == 0) {
      Actor_SetAnimationAndWait(11, 4);
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
      EVENT_WORK->message++;
    } else {
      EVENT_WORK->message++;
      Actor_SetAnimationAndWait(11, 3);
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    }
    FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_FaceDirectionAndWait(11, FACING_SOUTH_SOUTHEAST);
    Event_ShowMessage(11, 0);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_SetAnimationAndWait(11, 3);
    Event_ShowMessage(ACTOR_ISAAC, 0);
    Actor_FaceDirectionAndWait(11, FACING_NORTH_NORTHWEST);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_NOD);
    Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    FieldEvent_Wait(20);
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Actor_FaceDirectionAndWait(11, FACING_SOUTH_SOUTHEAST);
    Event_ShowMessage(11, 0);
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(11, 0);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    Event_ShowMessage(ACTOR_JENNA, 0);
    FieldActor_Jump(ACTOR_GARET, 2, 10);
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_SetAnimationAndWait(11, 3);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
    Event_ShowMessage(ACTOR_ISAAC, 0);
    Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
    Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_EAST);
    Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    FieldEvent_Wait(10);
    Actor_SetSpeed(ACTOR_JENNA, 1.6);
    Actor_SetSpeed(ACTOR_GARET, 1.6);
    Actor_SetScript(ACTOR_JENNA, (s32)gProx_JennaRunsAhead);
    Actor_RunScriptAndWait(ACTOR_GARET, (s32)gProx_GaretRunsAhead);
    FieldEvent_Wait(20);
    Actor_FaceDirectionAndWait(11, FACING_NORTH_NORTHWEST);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_WalkToAndWait(11, 156, 256);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Event_ShowMessage(ACTOR_ISAAC, 0);
    Actor_SetAnimation(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
    Event_ShowMessage(ACTOR_ISAAC, 0);
    Actor_FaceDirectionAndWait(11, FACING_NORTH_NORTHWEST);
    Event_ShowMessage(ACTOR_ISAAC, 0);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
    Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId,
                                              0) == 0) {
      Actor_SetAnimationAndWait(11, 3);
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
      EVENT_WORK->message++;
    } else {
      EVENT_WORK->message++;
      Actor_SetAnimationAndWait(11, 4);
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    }
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
    Actor_SetAnimation(11, 4);
    FieldEvent_ShowMessageThenWait(
        SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0, 20);
    Event_ShowMessage(ACTOR_ISAAC, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(11, 3);
    FieldEvent_Wait(20);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 40);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
    Event_ShowMessage(ACTOR_ISAAC, 0);
    Actor_SetAnimation(11, 4);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                    EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
    Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
    Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
    FieldEvent_Wait(20);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_FaceDirection(11, FACING_SOUTH_SOUTHEAST, 20);
    FieldActor_Jump(11, 4, 10);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_FaceDirectionAndWait(11, FACING_NORTH_NORTHWEST);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_FaceDirectionAndWait(11, FACING_SOUTH_SOUTHEAST);
    FieldActor_Jump(11, 2, 10);
    FieldActor_Jump(11, 4, 10);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
    Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_WEST);
    Actor_FaceDirectionAndWait(11, FACING_NORTH_NORTHWEST);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTHEAST, 0);
    Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTH);
    Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
    Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    Actor_FaceDirectionAndWait(11, FACING_SOUTH_SOUTHEAST);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_SetAnimation(11, 9);
    Actor_SetAnimation(gMainRoutineWork.leaderActorId, 0x21);
    Actor_SetAnimation(ACTOR_ISAAC, 0x28);
    TilesetAnim_Pause(2);
    TilesetAnim_Pause(3);
    TilesetAnim_Pause(4);
    TilesetAnim_Pause(5);
    TilesetAnim_Pause(6);
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    FieldEvent_Wait(40);
    PaletteFade_ApplyTarget(0x10009, 0);
    PaletteFade_Interpolate(120);
    System_Wait(120);
    // Waits up to 120 frames, or until a key is held.
    i = 0;
    goto test_wait;
  next_wait:
    i++;
  test_wait:
    if (i < 120) {
      System_Wait(1);
      if (gInput.held == 0)
        goto next_wait;
    }
    PaletteFade_ApplyTarget(0, 0);
    PaletteFade_Interpolate(16);
    System_Wait(16);
    Event_SetFlag(FLAG_SAID_FAREWELL);
    FieldEvent_SetExitRequest(3);
  }
}

void Actor_SelectAnimation(struct Actor *object, s32 mode);
extern s16 gSlideOnIce_SteerAngles[];

#include "shared/actor_steer_by_pad.h"

#include "shared/leader_stop_if_pending.h"
#undef FIELD_AT

/// A point on the map: 16.16 fixed-point pixels.
struct Position {
  s32 x;
  s32 y;
  s32 z;
};

void Actor_SelectAnimation(struct Actor *actor, s32 mode);
s32 Map_GetTerrainHeight(u8 layer, s32 x, s32 z);
extern s16 gSlideOnIce_AnglesByKey[];

#include "shared/leader_slide_on_ice.h"

extern volatile s16 gSlideOnIce_Stopped;

s32 Actor_HasArrived(struct Actor *object);

#include "shared/slide_on_ice_from_edge.c"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Leader_SetSlideStopped(s32 a0) { gSlideOnIce_Stopped = a0; }

#include "shared/leader_slide_on_ice_ahead.h"

struct ParticleSprite {
  u8 unk0[8];
  u16 attr;
  u8 unkA[30];
};

struct ParticleWork {
  u8 tiles[0x500];
  struct Particle particles[64];
  struct ParticleSprite sprites[64];
  s16 next;
  u16 tile;
};

void EffectSprite_DrawAtWorldPosition(struct ParticleSprite *sprite,
                                      struct Particle *particle);
void BattleEffect_StepVector3(struct Particle *particle, s32 unk1, s32 step);

#include "shared/particles_update_falling.h"

/// One falling rock: where it is, how it moves and how long it has fallen.
struct Rock {
  s32 position[3];
  s32 velocity[3];
  s32 timer;
};

/// A falling rock's sprite.
struct RockSprite {
  u8 unk00[5];
  u8 unk05;
  u8 unk06[2];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[0x14];
  u16 unk1e;
  u8 unk20[8];
};

/// The falling rocks' work: their tiles, 64 rocks and their sprites, in a
/// ring that next walks round.
struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

/// Spawns count rocks around (x, y, z), each pushed out at a random angle and
/// thrown up and out along it.
#include "shared/rocks_spawn.h"
#undef FIELD_AT

/* VIEW(s4254): tag struct RockSprite as struct RockSprite__s4254; prox.c has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja: games/tla/src/scene/prox.c:2106: structure has no
 * member named `unk05_5'. */
struct RockSprite__s4254 {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 unk05_5 : 1;
  u8 unk05_6 : 2;
  u8 unk06[3];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

void Prox_InitRockSprite(struct RockSprite__s4254 *sprite, s32 width,
                         s32 height, u32 attributes, s32 tile);

UNUSED void Prox_StartFallingRocks(void) {
  struct RockWork *work =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct RockWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct RockSprite__s4254 *sprite;

  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_FallingRock),
                      work);
  entry = VramCache_FindFreeEntry();
  tile = Vram_Load(entry, 0x500, work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--, sprite++, rock++) {
    Prox_InitRockSprite(sprite, 8, 8, 0x40000000, tile);
    sprite->unk05_5 = 1;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    rock->timer = -1;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  FieldTask_AddFrame(Particles_UpdateFalling, TASK_PASS_VBLANK);
}

struct SparkWork {
  u8 unk0000[0x1604];
  s16 resource;
};

UNUSED void Prox_EndFallingRocks(void) {
  struct SparkWork *work = Ram_HeapSlots->menuWork;

  FieldTask_Remove(Particles_UpdateFalling);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

#include "text/msg_ids.h"

void Prox_MoldJoins(void);
void Prox_AskAboutIgloos(void);
void Prox_TalkAboutParents(void);
void Prox_TalkToRestlessChild(void);

extern u32 gFieldEffect_LongScript[];

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

s32 gProx_EnterDoorScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Z, -0x8000),
    SC_ADD(SW_SCALE_X, -Q16_ONE / 64),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 64),
    SC_WAIT(1),
    SC_LOOP(24, SC_TOP),
    SC_END,
};

s16 gSlideOnIce_SteerAngles[] = {-1, 0, -32768, -1, -16384, -8192, -24576, -16384, 16384, 8192, 24576, 16384, -1, 0, -32768, -1, -1, 0, -32768, -1, -16384, -1, -1, -16384, 16384, -1, -1, 16384, -1, 0, -32768, -1};

s16 gSlideOnIce_AnglesByKey[] = {-1, 0, -32768, -1, -16384, 0, -32768, -16384, 16384, 0, -32768, 16384, -1, 0, -32768, -1};

s32 gFieldEffect_DurationScripts[] = {
    (s32)gFieldEffect_ShortScript,
    (s32)gFieldEffect_MediumScript,
    (s32)gFieldEffect_LongScript,
};

u32 gProx_FirstChildRuns[] = {                                                               
      39, 2,  22, 15, 0x10000, 22, 16, 0x6666, 39, 2,  4,  0x2300000,     0,          
      0x17c0000, 1,  39, 2,  4,       0x2340000, 0,  0x1a80000,     1,  39, 2,  4,      0x2180000,         
      0,  0x1a80000, 1,  39, 2,       4,  0x2180000, 0,      0x17c0000, 1,  13, 0xffff, 0,          
  };

u32 gProx_SecondChildRuns[] = {                                                               
      39, 2,  22, 15, 0x10000, 22, 16, 0x6666, 39, 2,  4,  0x2180000,     0,          
      0x17c0000, 1,  39, 2,  4,       0x2300000, 0,  0x17c0000,     1,  39, 2,  4,      0x2340000,         
      0,  0x1a80000, 1,  39, 2,       4,  0x2180000, 0,      0x1a80000, 1,  13, 0xffff, 0,          
  };

u32 gProx_ThirdChildRuns[] = {                                                               
      39, 2,  22, 15, 0x10000, 22, 16, 0x6666, 39, 2,  4,  0x2180000,     0,          
      0x1a80000, 1,  39, 2,  4,       0x2180000, 0,  0x17c0000,     1,  39, 2,  4,      0x2300000,         
      0,  0x17c0000, 1,  39, 2,       4,  0x2340000, 0,      0x1a80000, 1,  13, 0xffff, 0,          
  };

u32 gProx_KradenStepsUpToPuelle[] = {
    SC_ANIMATION(2),
    SC_MOVE(176, 0, 154),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

u32 gProx_IsaacStepsUpToPuelle[] = {
    SC_ANIMATION(2),
    SC_MOVE(160, 0, 154),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

u32 gProx_StepBackFromPuelle[] = {
    SC_ANIMATION(2),
    SC_MOVE(144, 0, 154),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(Prox1),
    SCENE_EXIT(1, WorldMap, 62),
    SCENE_EXIT(2, Prox, 1),
    SCENE_EXIT(3, StaffCredit, 0),
    SCENE_EXITS(Prox),
    SCENE_EXIT(1, Prox1, 2),
    SCENE_EXIT(2, Prox4, 8),
    SCENE_EXIT(3, Prox4, 1),
    SCENE_EXIT(4, Prox4, 10),
    SCENE_EXIT(5, Prox4, 4),
    SCENE_EXIT(6, Prox4, 5),
    SCENE_EXIT(7, Prox4, 2),
    SCENE_EXIT(8, Prox6, 7),
    SCENE_EXIT(9, ProxMold, 2),
    SCENE_EXIT(10, Prox5, 9),
    SCENE_EXIT(11, ProxMold, 1),
    SCENE_EXITS(ProxMold),
    SCENE_EXIT(1, Prox, 11),
    SCENE_EXIT(2, Prox, 9),
    SCENE_EXIT(3, WorldMap, 63),
    SCENE_EXIT(4, Prox4, 7),
    SCENE_EXITS(Prox4),
    SCENE_EXIT(1, Prox, 3),
    SCENE_EXIT(2, Prox, 7),
    SCENE_EXIT(3, Prox5, 6),
    SCENE_EXIT(4, Prox, 5),
    SCENE_EXIT(5, Prox, 6),
    SCENE_EXIT(6, Prox5, 7),
    SCENE_EXIT(7, ProxMold, 4),
    SCENE_EXIT(8, Prox, 2),
    SCENE_EXIT(9, Prox5, 8),
    SCENE_EXIT(10, Prox, 4),
    SCENE_EXITS(Prox5),
    SCENE_EXIT(6, Prox4, 3),
    SCENE_EXIT(7, Prox4, 6),
    SCENE_EXIT(8, Prox4, 9),
    SCENE_EXIT(9, Prox, 10),
    SCENE_EXITS(Prox6),
    SCENE_EXIT(7, Prox, 8),
    SCENE_EXITS_END,
};

u32 gProx_Room1Placements[] = {
    0xffff, 0, 0, 0, 0, 0,
};

s16 gProx_Room1FarewellPlacements[] = {
    6, -1, 1, 0, 0, 88, 0, 0,
    0, 248, 0xc000, 2, 7, -1, 1, 0,
    0, 104, 0, 0, 0, 248, 0xc000, 2,
    62, -1, 1, 0, 0, 120, 0, 0,
    0, 248, 0xd000, 2, 60, -1, 1, 0,
    0, 136, 0, 0, 0, 248, 0xd000, 2,
    61, -1, 1, 0, 0, 152, 0, 0,
    0, 248, 0xb000, 2, 3, -1, 1, 0,
    0, 168, 0, 0, 0, 248, 0xc000, 2,
    2, -1, 1, 0, 0, 184, 0, 0,
    0, 248, 0xc000, 2, 1, -1, 1, 0,
    0, 200, 0, 0, 0, 248, 0xc000, 2,
    5, -1, 1, 0, 0, 168, 0, 0,
    0, 216, 0xa000, 2, 56, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    0, -1, 1, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

u32 gProx_Room2Placements[] = {
    0xffff00af, 1, 0xf80000, 0, 0x1c80000, 0x33000,
    0xffff00ac, 1, 0xd80000, 0, 0x1500000, 0x15000,
    0xffff00ae, 3, 0x700000, 0, 0xf80000, 0,
    0xffff00b0, 1, 0x1c40000, 0, 0x12c0000, 0x15000,
    0xffff00ab, 1, 0x1140000, 0, 0x940000, 0x1b000,
    0xffff00ad, 1, 0x2440000, 0, 0x15c0000, 0x38000,
    0xffff00b2, 1, 0xf00000, 0, 0xf40000, 0x33000,
    0xffff00b3, 1, 0x1760000, 0, 0x1a40000, 0x15000,
    0xffff00b2, 1, 0, 0, 0, 0x20000,
    0xffff00b3, 1, 0, 0, 0, 0x2c000,
    0xffff00b2, 1, 0, 0, 0, 0x28000,
    0xffff0149, 1, 0x19c0000, 0, 0x1d80000, 0x24000,
    0xffff011e, 7, 0x2380000, 0, 0xb00000, 0x24000,
    0xffff, 0, 0, 0, 0, 0,
};

struct ActorPlacement gProx_Room3Placements[] = {
    ACTOR_PLACEMENT(175, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 540, 0, 388, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(172, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 388, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(44, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 180, 0, 132, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(45, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 140, 0, 132, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(155, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 178, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(155, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 178, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(155, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 178, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(153, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 178, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(153, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 178, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(153, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 178, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(0, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(286, CONDITION_ALWAYS, 7, 436, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(243, FLAG_DJINNI(DJINNI_MOLD), MOVEMENT_TYPE_STAND, 404, 0, 320, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

s32 gProx_Room3PlacementsOnceLit[] = {
    (s32)0xffff00af, 1, 0, 0, 0, 0x2b000,
    (s32)0xffff00ac, 1, 0, 0, 0, 0x2b000,
    (s32)0xffff002c, 1, 0, 0, 0, 0x25000,
    (s32)0xffff002d, 1, 0, 0, 0, 0x23000,
    (s32)0xffff009b, 1, 0xa00000, 0, 0x780000, 0x1b000,
    (s32)0xffff009b, 1, 0xb00000, 0, 0x780000, 0x1b000,
    (s32)0xffff009b, 3, 0xb00000, 0, 0xa20000, 0xd000,
    (s32)0xffff0099, 1, 0x800000, 0, 0xd80000, 0xd000,
    (s32)0xffff0099, 1, 0x920000, 0, 0x780000, 0x1d000,
    (s32)0xffff0099, 1, 0x780000, 0, 0xa00000, 0x33000,
    (s32)0xffff0038, 1, 0, 0, 0, 0x24000,
    -0x10000, 1, 0, 0, 0, 0x24000,
    (s32)0xffff011e, 7, 0x1b40000, 0, 0xd00000, 0x24000,
    0x4000f3, 1, 0x1940000, 0, 0x1400000, 0x4000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gProx_Room4Placements[] = {
    0xffff00af, 1, 0x580000, 0, 0xc00000, 0x8000,
    0xffff00ac, 1, 0x800000, 0, 0x6a0000, 0x1b000,
    0xffff00b2, 1, 0x980000, 0, 0x700000, 0x5000,
    0xffff00b0, 3, 0x16c0000, 0, 0x7b0000, 0x13000,
    0xffff00ac, 1, 0x1d40000, 0, 0xa00000, 0x3000,
    0xffff00ab, 3, 0x2d80000, 0, 0xc80000, 0x18000,
    0xffff00ad, 1, 0x2ae0000, 0, 0x6f0000, 0x1d000,
    0xffff00b2, 1, 0x2240000, 0, 0x3300000, 0x8000,
    0xffff00b3, 1, 0x1d20000, 0, 0x32a0000, 0x1d000,
    0xffff00b2, 1, 0x1e00000, 0, 0x3200000, 0x15000,
    0xffff00ac, 1, 0x2120000, 0, 0x3200000, 0x35000,
    0xffff00ac, 3, 0xa40000, 0, 0x20c0000, 0x10000,
    0xffff00ab, 3, 0xde0000, 0, 0x1ea0000, 0x15000,
    0xffff00ad, 1, 0xa80000, 0, 0x18a0000, 0x1d000,
    0xffff00ae, 1, 0x10c0000, 0, 0x31a0000, 0x15000,
    0xffff00b0, 1, 0x880000, 0, 0x3300000, 0x13000,
    0x5700f4, 1, 0x2380000, 0, 0x1a80000, 0x8000,
    0xffff, 0, 0, 0, 0, 0,
};

s16 gProx_Room4PlacementsOnceLit[] = {
    175, -1, 3, 0, 0, 104, 0, 0,
    0, 196, 0x8000, 0, 172, -1, 1, 0,
    0, 128, 0, 0, 0, 106, 0xb000, 1,
    178, -1, 1, 0, 0, 152, 0, 0,
    0, 112, 0x5000, 0, 176, -1, 3, 0,
    0, 364, 0, 0, 0, 123, 0x3000, 1,
    172, -1, 1, 0, 0, 468, 0, 0,
    0, 160, 0x3000, 0, 171, -1, 3, 0,
    0, 728, 0, 0, 0, 200, 0x8000, 1,
    173, -1, 3, 0, 0, 666, 0, 0,
    0, 134, 0xd000, 1, 178, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x5000, 2,
    179, -1, 1, 0, 0, 0, 0, 0,
    0, 0, 0xd000, 2, 178, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x5000, 2,
    172, -1, 1, 0, 0, 530, 0, 0,
    0, 800, 0x5000, 3, 172, -1, 1, 0,
    0, 234, 0, 0, 0, 440, 0x5000, 0,
    171, -1, 3, 0, 0, 224, 0, 0,
    0, 554, 0x5000, 0, 173, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0xd000, 2,
    174, -1, 1, 0, 0, 268, 0, 0,
    0, 794, 0x5000, 1, 176, -1, 1, 0,
    0, 136, 0, 0, 0, 816, 0x3000, 1,
    244, 87, 1, 0, 0, 568, 0, 0,
    0, 424, 0x8000, 0, 44, -1, 1, 0,
    0, 192, 0, 0, 0, 428, 0x5000, 3,
    45, -1, 1, 0, 0, 176, 0, 0,
    0, 428, 0x3000, 1, 56, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    0, -1, 1, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 5, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

u32 gProx_Room5Placements[] = {
    0xffff00ae, 1, 0x2740000, 0, 0xc80000, 0x15000,
    0xffff00ae, 1, 0x1740000, 0, 0x3280000, 0x3b000,
    0xffff00b0, 1, 0x1720000, 0, 0x2e40000, 0x15000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gProx_Room5PlacementsOnceLit[] = {
    0xffff00ae, 3, 0x29a0000, 0, 0x7c0000, 0x15000,
    0xffff00ae, 1, 0x1740000, 0, 0x3280000, 0x3b000,
    0xffff00b0, 1, 0x1720000, 0, 0x2e40000, 0x15000,
    0xffff00ad, 1, 0x2980000, 0, 0x1c80000, 0x5000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gProx_Room6Placements[] = {
    0xffff00c9, 1, 0xb00000, 0, 0x3090000, 0x15000,
    0xffff, 0, 0, 0, 0, 0,
};

struct Trigger gProx_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gProx_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 10, FLAG_SAID_FAREWELL, EVENT(Prox_SayFarewell)},
    {TRIGGER_TABLE_END},
};

struct Trigger gProx_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(Prox_OpenDoor2)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(Prox_OpenDoor3)},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 4, CONDITION_ALWAYS, 4},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT(Prox_OpenDoor5)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(Prox_OpenDoor6)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT(Prox_OpenDoor7)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(Prox_OpenDoor8)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT(Prox_OpenDoor10)},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_TALK, 8, 0xa3d, MSGNO(MsgProxFelixWelcomeBackToProx)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgProxThankYouForLightingFires)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0xa3d, MSGNO(MsgProxAgatioAndKarstToldMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgProxFelixHasProvenHimself)},
    {TRIGGER_TALK, 9, 0xa3d, MSGNO(MsgProxAgatioAndKarstLeft)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgProxItsTooBadAboutAgatio)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0xa3d, MSGNO(MsgProxAgatioAndKarstAreStrongest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgProxIHeardKarstAndAgatio)},
    {TRIGGER_TALK, 10, 0xa3d, MSGNO(MsgProxImGladYouMade)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgProxIDontRememberLastTime)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0xa3d, MSGNO(MsgProxEverSinceAgatioReturned)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgProxBlizzardsFinallyOver)},
    {TRIGGER_TALK, 11, 0xa3d, MSGNO(MsgProxWereUsedToColdUp)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgProxWinterInProxCan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0xa3d, MSGNO(MsgProxItGetsColdAroundHere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgProxNowThatOurWeather)},
    {TRIGGER_TALK, 12, 0xa3d, MSGNO(MsgProxIveBeenKeepingCloseEye)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgProxIWasSoMovedWhen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0xa3d, MSGNO(MsgProxEverythingIsSupposedToReturn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgProxIWasFirstPerson)},
    {TRIGGER_TALK, 13, 0xa3d, MSGNO(MsgProxHaveYouSeenSaturos)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgProxLighthousesHaveAllBeenLit)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0xa3d, MSGNO(MsgProxICantBelieveAnyoneCould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgProxTheyHadNoChoiceThey)},
    {TRIGGER_TALK, 14, 0xa3d, MSGNO(MsgProxFelixYoureBack)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgProxYouDidAmazingJob)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0xa3d, MSGNO(MsgProxIfFelixCouldBecomeAdept)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgProxWhenFelixLeftTownHe)},
    {TRIGGER_TALK, 15, 0xa3d, MSGNO(MsgProxOnceFinalLighthouseIsLit)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgProxWillYouBeGoingBack)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0xa3d, MSGNO(MsgProxIfThisWeatherContinuesMuch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgProxIBetFelixAndHis)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(Prox_TalkToPlayingChild)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT(Prox_TalkToPlayingChild)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(Prox_TalkToRestlessChild)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, EVENT(Prox_TalkToRestlessChild)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(Prox_TalkToTrainingChild)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, EVENT(Prox_TalkToTrainingChild)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 100, FLAG_DIG_SPOT_SCOOPED, EVENT(Prox_ScoopUpDarkMatter)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 19, CONDITION_ALWAYS, 0},
    {TRIGGER_TABLE_END},
};

struct Trigger gProx_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT(Prox_OpenUpperDoor4)},
    {TRIGGER_STEP, 9, FLAG_MOLD_HID_IN_SNOW, EVENT(Prox_MoldHidesInSnow)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(Leader_SlideOnIce)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_BEFORE_CAST), 255, CONDITION_ALWAYS, EVENT(Prox_BeginScoop)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_AFTER_CAST), 255, CONDITION_ALWAYS, EVENT(Prox_EndScoop)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 8, CONDITION_ALWAYS, EVENT(Prox_ScoopUpMold)},
    {TRIGGER_RAISED, 200, 0xa3d, EVENT(Prox_MoldJoins)},
    {TRIGGER_STEP, 11, FLAG_HEARD_OF_PARENTS, EVENT(Prox_SearchForParents)},
    {TRIGGER_STEP, 12, FLAG_CLAN_SPOKE_OF_LIGHTHOUSE, EVENT(Prox_DecideToGoToLighthouse)},
    {TRIGGER_STEP, 13, FLAG_MET_PUELLE, EVENT(Prox_MeetPuelle)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgProxPuelleOrderedMeToSet)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgProxPuelleWantedToSetFelixs)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgProxFelixTookTroubleOfComing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgProxICantThinkOfAnywhere)},
    {TRIGGER_TALK, 12, 0xa3d, MSGNO(MsgProxIsThatYouFelixWhen)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgProxSorryFelixButThisRoad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0xa3d, MSGNO(MsgProxIfLighthouseIsntLitSoon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgProxRoadToLighthouse)},
    {TRIGGER_TALK, 13, 0xa3d, MSGNO(MsgProxIfWeWereBetterAdepts)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgProxIfYoureWorriedAboutLighthouse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0xa3d, MSGNO(MsgProxNeitherOurElderNorPuelle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgProxEldersWereSayingThatLighthouse)},
    {TRIGGER_TALK, 14, 0xa3d, MSGNO(MsgProxIFeelSoHelplessAgatio)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgProxIJustDontThink)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0xa3d, MSGNO(MsgProxPuelleIsStrongerThanAny)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgProxOnOtherHandNow)},
    {TRIGGER_TALK, 15, 0xa3d, MSGNO(MsgProxAllWeCanDoNow)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgProxFelixTaughtMeLotAbout)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0xa3d, MSGNO(MsgProxIfTheyDidntLightBeacon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgProxMaybeIfITrainHard)},
    {TRIGGER_TALK, 16, 0xa3d, MSGNO(MsgProxOurPsynergyFeelsWeak)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgProxEvenJustStandingOutsideLighthouse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0xa3d, MSGNO(MsgProxIfOnlyMyFirePsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgProxIfLightOfBeaconKeeps)},
    {TRIGGER_TALK, 17, 0xa3d, MSGNO(MsgProxPuellePromisedThatHeWould)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgProxWellYouLookLikeYoure)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0xa3d, MSGNO(MsgProxPuelleNeverIntendedToTake)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgProxWhenFelixLeftProx)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgProxYouMustFindKarstGet)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgProxIfThisDoesntWork)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgProxFelixYouAreOnlyHope)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgProxIfTheyWereStrongEnough)},
    {TRIGGER_TABLE_END},
};

struct Trigger gProx_Room4Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_TALK, 8, 0xa3d, MSGNO(MsgProxRiftAtMarsLighthouseGets)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgProxIThinkRiftToNorth)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0xa3d, MSGNO(MsgProxIThoughtProxWould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgProxIWasAfraidFalls)},
    {TRIGGER_TALK, 9, 0xa3d, MSGNO(MsgProxOurChieftainToldMe)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgProxIWonderIfThatHole)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0xa3d, MSGNO(MsgProxNobodyBelievedThatRiftWould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgProxWhatDoWeDo)},
    {TRIGGER_TALK, 10, 0xa3d, MSGNO(MsgProxNotTooLongAgo)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgProxLighthouseWasSoBeautifulWhen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0xa3d, MSGNO(MsgProxNorthernRiftHasGotten)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgProxAllLightsJoinedUpTogether)},
    {TRIGGER_TALK, 11, 0xa3d, MSGNO(MsgProxNoOneThoughtAboutOur)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgProxIGuessFourLighthouses)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0xa3d, MSGNO(MsgProxToThinkThatWeAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgProxNowThatAlchemyIsFree)},
    {TRIGGER_TALK, 12, 0xa3d, MSGNO(MsgProxIfMarsLighthousesFires)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgProxPeopleOfProxWontForget)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0xa3d, MSGNO(MsgProxIThinkItsTooLate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgProxFelixAndHisAlliesSaved)},
    {TRIGGER_TALK, 13, 0xa3d, MSGNO(MsgProxWeSentEmissariesToVale)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgProxFelixTakeSomeAdvice)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0xa3d, MSGNO(MsgProxAtLeastJourneyWasnt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgProxIfTheyFoundOutFelix)},
    {TRIGGER_TALK, 14, 0xa3d, MSGNO(MsgProxValesEldersWereStubbornThey)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgProxIsaacAndHisFriendsLeft)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0xa3d, MSGNO(MsgProxThreeYearsAgoWeSent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgProxMayorAndEldersInVale)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(Prox_AskAboutIgloos)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgProxIHateIgloos)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgProxWindIsSoCold)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgProxMamaAndPapaHaveBeen)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgProxMyParentsToldMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgProxIAlwaysCatchColdWhen)},
    {TRIGGER_TALK, 18, 0xa3d, MSGNO(MsgProxThosePoorChildrenTheyDont)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgProxICanHearSound)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, 0xa3d, MSGNO(MsgProxICantBringMyself)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgProxItsBeenSoLongSince)},
    {TRIGGER_TALK, 19, 0xa3d, MSGNO(MsgProxPuelleOurChieftainGrewWorried)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgProxTheresNoTellingWhatCould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, 0xa3d, MSGNO(MsgProxNowThatSaturosAndHis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgProxWeveLostSoManySoldiers)},
    {TRIGGER_TALK, 20, 0xa3d, MSGNO(MsgProxPeopleSaturosRescuedFromStorm)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgProxFelixIAlwaysThought)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, 0xa3d, MSGNO(MsgProxMarsIsLastLighthouse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgProxWhoKnewThatPoorInjured)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgProxDontForgetFelixYourMother)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgProxSaturosUsedFelixsParents)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgProxIHaveMuchToDo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgProxWeMayHaveUnderestimatedPower)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgProxYourBodyHasAbsorbedGreat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgProxPowerToReviveDead)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(Prox_TalkAboutParents)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, 0xa3d, MSGNO(MsgProxFelixsParentsWereWorriedAbout)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgProxWellNeverBeAble)},
    {TRIGGER_TALK, 23, 0xa3d, MSGNO(MsgProxYourMotherKnowsThatYoure)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgProxAfterThatEarthquakeIFeel)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, 0xa3d, MSGNO(MsgProxFelixsParentsWereSoWorried)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgProxWhenEarthquakeHitFelix)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, EVENT(Prox_MercuryDjinniJoins)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxThoseBoilingVegetablesLookLike))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxTheresCheeseInThisStew))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxThereIsSomeStrangeDish))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxTheyveGotSomeStrangeMeat))},
    {TRIGGER_TABLE_END},
};

struct Trigger gProx_Room5Triggers[] = {
    {0xca02, 6, CONDITION_ALWAYS, EVENT(Prox_ClimbStairs)},
    {0xca02, 7, CONDITION_ALWAYS, EVENT(Prox_ClimbStairs)},
    {0xca02, 8, CONDITION_ALWAYS, EVENT(Prox_ClimbStairs)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_TALK, 8, 0xa3d, MSGNO(MsgProxProxOriginallyFlourishedBecauseConcerned)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgProxIveBeenToldThatLighting)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0xa3d, MSGNO(MsgProxWhenLighthousesWereAllExtinguished)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgProxAlchemyHasBeenFreed)},
    {TRIGGER_TALK, 9, 0xa3d, MSGNO(MsgProxWhenYouLeftProx)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgProxThankYouSoMuchFelix)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0xa3d, MSGNO(MsgProxWhatHasHeGoneThrough)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgProxINeverImaginedThatLittle)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Prox_PraiseReturnedWarrior)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0xa3d, MSGNO(MsgProxFelixLooksSoConfident)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgProxItsTooBadFelixCant)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgProxThinkOfYourOldGrandmas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgProxImSoSadFelixDoesnt)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxThoseBoilingVegetablesLookLike))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxTheresCheeseInThisStew))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxThereIsSomeStrangeDish))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxTheyveGotSomeStrangeMeat))},
    {TRIGGER_TABLE_END},
};

struct Trigger gProx_Room6Triggers[] = {
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Prox_FearForWeyard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0xa3d, MSGNO(MsgProxTheseCatastrophesAreResult)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgProxLighthousesAreGoingToRestore)},
    {TRIGGER_TABLE_END},
};

u32 gProx_IsaacStepsForward[] = {
    SC_ANIMATION(2),
    SC_MOVE(120, 0, 196),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(10),
    SC_END,
};

u32 gProx_KradenStepsForward[] = {
    SC_ANIMATION(2),
    SC_MOVE(152, 0, 196),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(10),
    SC_END,
};

u32 gProx_SecondParentWalksSouth[] = {
    SC_ANIMATION(2),
    SC_MOVE(136, 0, 348),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

u32 gProx_ThirdParentWalksSouth[] = {
    SC_ANIMATION(2),
    SC_MOVE(152, 0, 348),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

u32 gProx_LeftFileWalksSouth[] = {
    SC_ANIMATION(2),
    SC_MOVE(136, 0, 248),
    SC_SYNC,
    SC_MOVE(136, 0, 348),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

u32 gProx_RightFileWalksSouth[] = {
    SC_ANIMATION(2),
    SC_MOVE(152, 0, 248),
    SC_SYNC,
    SC_MOVE(152, 0, 348),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

u32 gProx_GaretWalksSouth[] = {
    SC_ANIMATION(2),
    SC_MOVE(152, 0, 248),
    SC_SYNC,
    SC_MOVE(152, 0, 272),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_END,
};

u32 gProx_JennaWalksSouth[] = {
    SC_ANIMATION(2),
    SC_MOVE(168, 0, 272),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_END,
};

u32 gProx_IsaacWalksSouth[] = {
    SC_ANIMATION(2),
    SC_MOVE(120, 0, 220),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(1),
    SC_END,
};

u32 gProx_LeaderWalksSouth[] = {
    SC_ANIMATION(2),
    SC_MOVE(140, 0, 220),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(1),
    SC_END,
};

u32 gProx_KradenWalksSouth[] = {
    SC_ANIMATION(2),
    SC_MOVE(152, 0, 240),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_END,
};

u32 gProx_JennaRunsAhead[] = {
    SC_ANIMATION(2),
    SC_MOVE(168, 0, 348),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

u32 gProx_GaretRunsAhead[] = {
    SC_ANIMATION(2),
    SC_MOVE(152, 0, 348),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};
// clang-format on
#undef FIELD_AT
