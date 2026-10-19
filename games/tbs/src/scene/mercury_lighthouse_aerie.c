#include "game_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/mercury_lighthouse_aerie.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/scene/common/object_nearest.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
#include "types.h"
#include "field_event.h"
#include "engine_call.h"
#include "field_scene.h"
#include "overlay_object.h"
#include "create_configured_overlay_object.h"
#include "global_cells.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "iwram_call.h"
#include "map_scroll.h"
#include "text/msg_ids.h"
#include "field_obj.h"
#include "macros.h"
#include "flags.h"
#include "songs.h"
#include "ability_ids.h"

// The aerie's actors. The party and Jenna stand in their own slots. The
// speakers are known from the text: Kraden ("It's all right, Jenna."), Felix
// ("Jenna, please... Just do as she says"), Saturos (Menardi: "Oh,
// Saturos..."), Menardi (Saturos: "Menardi... Wait!") and Alex (Mia:
// "Alex!"). Saturos is fought as encounter table 36, slot 2.
enum MercuryLighthouseAerieActor {
  ACTOR_BEACON_FLAME = ACTOR_FIRST_PLACED, // flickers and sheds sparks
  ACTOR_LIFT_STATUE,
  ACTOR_FIRST_PLATFORM = ACTOR_FIRST_PLACED + 6, // six, sprite 0xf2
  // The two platforms that stand on the lift and go up and down with it.
  ACTOR_LIFT_PLATFORM_1 = ACTOR_FIRST_PLATFORM + 3,
  ACTOR_LIFT_PLATFORM_2,
  ACTOR_KRADEN = ACTOR_FIRST_PLACED + 12,
  ACTOR_FELIX,
  ACTOR_SATUROS,
  ACTOR_MENARDI,
  ACTOR_ALEX
};

// Scene flags, kept while the party stays on the aerie.
enum {
  // The leader stands on a platform, which has dipped under the weight.
  FLAG_ON_PLATFORM = FLAG_SCENE_TEMP_50,
  FLAG_LIFT_UP = FLAG_SCENE_TEMP_00 + 0x51,
  // Set on every arrival but the first: Jenna, Kraden, Felix and Menardi
  // are placed only before it.
  FLAG_HOSTAGES_GONE = FLAG_SCENE_TEMP_00 + 0x53
};

enum { FLAG_PSYNERGY_STONE = 0x350 };

// Forward declarations.
static void MercuryLighthouseAerie_PartyArrives(void);
static void MercuryLighthouseAerie_SettlePlatforms(void);
static void MercuryLighthouseAerie_AlexAppears(void);
static void MercuryLighthouseAerie_SaturosAndAlexWarpOut(void);
static void MercuryLighthouseAerie_SaturosAndAlexWarpIn(void);
static void MercuryLighthouseAerie_LowerLift(void);
static void MercuryLighthouseAerie_RaiseLift(void);
static void MercuryLighthouseAerie_FlickerBeacon(void);

#define NULL ((void *)0)

// Shared source is included here in ROM order.
#include "common/effect_create.h"

struct ActorTable {
  u8 unk00[0x14];
  struct FieldActor *actors[66];
};

// Shared source is included here in ROM order.
#include "common/effect_spawn.h"
#include "objcmd.h"

// The entrance positions and camera bounds.
const struct SceneEntrance *MercuryLighthouseAerie_GetEntrances(void) {
  return gMercuryLighthouseAerieEntrances;
}

// The aerie has no clipping regions.
const struct SceneRegion *MercuryLighthouseAerie_GetRegions(void) { return 0; }

// One exit: the lift down to the lighthouse's foot.
const u32 *MercuryLighthouseAerie_GetExits(void) {
  return gMercuryLighthouseAerieExits;
}

// The actors; only the first arrival places Jenna, Kraden, Felix and Menardi.
const struct ScenePlacement *MercuryLighthouseAerie_GetPlacements(void) {
  if (gGameState.entrance != 1) {
    Engine_GameFlagSet(FLAG_HOSTAGES_GONE);
  }
  return gMercuryLighthouseAeriePlacements;
}

// The touch and search events.
const struct SceneEvent *MercuryLighthouseAerie_GetEvents(void) {
  return gMercuryLighthouseAerieEvents;
}

// On arriving: the beacon flickers and the six platforms sit 50 pixels down;
// the first arrival (entrance 1) plays the party's arrival, the return from
// the Saturos battle (entrance 2) Alex's scene, and arriving by the lift
// (entrance 5) leaves the lift up.
s32 MercuryLighthouseAerie_Initialize(void) {
  u32 i;
  s32 actor;
  struct FieldActor *obj;

  Engine_GameFlagSet(FLAG_MERCURY_LIGHTHOUSE_LIT);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gGameState.scene == (s32)&SceneId_MercuryLighthouseAerie1) {
    Engine_GameFlagSet(0x144);
    FieldScheduler_AddCallback(MercuryLighthouseAerie_FlickerBeacon, 0xc80);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
    Engine_ActorSetSpritePriority(GARET, 1);
    Engine_ActorSetSpritePriority(IVAN, 1);
    Engine_ActorSetSpritePriority(MIA, 1);
    Engine_ActorSetSpritePriority(JENNA, 1);
    Engine_ActorSetSpritePriority(ACTOR_KRADEN, 1);
    Engine_ActorSetSpritePriority(ACTOR_FELIX, 1);
    Engine_ActorSetSpritePriority(ACTOR_SATUROS, 1);
    Engine_ActorSetSpritePriority(ACTOR_MENARDI, 1);
    Engine_ActorSetSpritePriority(ACTOR_ALEX, 1);
    Engine_ActorSetSpritePriority(ACTOR_BEACON_FLAME, 1);
    Engine_ActorSetSpritePriority(ACTOR_LIFT_STATUE, 1);
    Engine_ActorSetSpritePriority(10, 1);
    Engine_ActorSetSpritePriority(11, 1);
    Engine_ActorSetSpritePriority(12, 1);
    Engine_ActorSetSpritePriority(13, 1);
    for (i = ACTOR_FIRST_PLATFORM; i < ACTOR_FIRST_PLATFORM + 6; i++) {
      Engine_ActorSetSpritePriority(i, 1);
      Object_GetById(i)->motionFlags = 4;
      Object_GetById(i)->priorityFlags |= 2;
      Object_GetById(i)->y.fixed = -0x328000;
    }
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) &&
        (actor = SceneActor_FindNearestSlotOfKindF2()) != 0 &&
        (obj = Object_GetById(actor)) != NULL) {
      obj->motionFlags = 0;
    }
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_LIFT_STATUE), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
    Object_GetById(12)->scaleX = -0x10000;
    Object_GetById(13)->scaleX = -0x10000;
    if (gGameState.entrance == 1) {
      if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        MercuryLighthouseAerie_PartyArrives();
      }
    } else if (gGameState.entrance == 2) {
      if (!Engine_GameFlagIsSet(FLAG_LIFT_UP)) {
        {
          struct MapLayerScroll *lift =
              &((struct MapScrollWork *)gMapWork[0])->layers[2];

          lift->offsetY = PIXELS(1024);
        }
        Engine_MapRedraw();
        Engine_TaskWait(1);
        Map_CopyCellsTo(4, 70, 4, 74, 5, 4);
        Engine_ActorSetPosition(ACTOR_LIFT_STATUE, 0, 0);
        if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
          MercuryLighthouseAerie_AlexAppears();
        }
      }
    } else if (gGameState.entrance == 5) {
      Engine_GameFlagSet(FLAG_LIFT_UP);
    }
  }
  return 0;
}

// Entrance 1, unless resuming: the Adepts of lit elements regain their PP;
// the aerie shakes, the camera pans across and in a flash the party stands
// at the top. Isaac shakes his head and the others gather on him.
static void MercuryLighthouseAerie_PartyArrives(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  Party_RestorePpOfLitElements();
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(GARET), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(IVAN), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(MIA), 0);
  Camera_MoveTo(PIXELS(304), -1, PIXELS(120), 0);
  Engine_TaskWait(1);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Event_PlayCue(SE_141);
  Map_SetShake(0x50000, 0x50000, 0x10000);
  Event_PlayCue(SE_289);
  Map_SetShake(-1, -1, 0xe666);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_MapRenderWaitForValues();
  Engine_EventWait(30);
  Engine_EventGetViewCenter()->motionFlags = 0;
  Camera_SetSpeed(0xcccc, 0x1999);
  Camera_MoveTo(PIXELS(512), -PIXELS(24), PIXELS(160), 1);
  Engine_CameraWaitForMove();
  ColorBuffer_ApplySource(0x10000, 0);
  ColorBuffer_ApplyTarget(0x10005, 0);
  Engine_ColorBufferInterpolate(50);
  Engine_EventWait(50);
  ColorBuffer_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(30);
  Engine_EventWait(30);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(504), PIXELS(168));
  Actor_SetPosition(GARET, PIXELS(528), PIXELS(144));
  Actor_SetPosition(IVAN, PIXELS(488), PIXELS(144));
  Actor_SetPosition(MIA, PIXELS(512), PIXELS(152));
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
  Engine_ActorSetAnimation(GARET, 19);
  Engine_ActorSetAnimation(IVAN, 19);
  Engine_ActorSetAnimation(MIA, 19);
  Engine_EventWait(10);
  ColorBuffer_ApplyTarget(0x10000, 0);
  Engine_ColorBufferInterpolate(30);
  Engine_EventWait(30);
  Engine_EventWait(80);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 1);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(60);
  Engine_ActorSetSpriteFlags(Object_GetById(GARET), 1);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Engine_ActorSetSpriteFlags(Object_GetById(IVAN), 1);
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  Engine_EventWait(40);
  Engine_ActorSetSpriteFlags(Object_GetById(MIA), 1);
  Engine_ActorSetAnimation(MIA, ANIM_STAND);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);
  Actor_SetSpeed(GARET, 0xcccc, 0x6666);
  Actor_SetSpeed(IVAN, 0xcccc, 0x6666);
  Actor_SetSpeed(MIA, 0xcccc, 0x6666);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  Engine_ActorSetAnimation(IVAN, ANIM_WALK);
  Engine_ActorSetAnimation(MIA, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0)
    Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0)
    Actor_SetDestination(IVAN, leader->x.part.pixel, leader->z.part.pixel);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0)
    Actor_SetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
  Engine_ActorWaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Engine_ActorWaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_EventEnd();
}

// Shared source is included here in ROM order.
#include "common/object_distance.h"
#include "common/object_nearest.h"

// Walking into trigger 10 or 11: if nothing is in the way the leader jumps
// two cells ahead, off any platform and onto the next.
static void MercuryLighthouseAerie_JumpToPlatform(void) {
  struct FieldActor *actor;
  s32 position[3];
  s32 angle;
  u8 flags;

  actor = Object_GetById(ACTOR_PARTY_LEADER);
  angle = (actor->facing + 0x1000) & 0xe000;
  flags = actor->motionFlags;
  SET_VECTOR(position, (actor->x.fixed & 0xfff00000) + 0x80000, actor->y.fixed,
             (actor->z.fixed & 0xfff00000) + 0x80000);
  Vector_AddPolarOffset(0x200000, angle, position);
  if (FieldObject_CheckMovementCollision(actor, position) == 0) {
    Engine_GameFlagClear(FLAG_ON_PLATFORM);
    MercuryLighthouseAerie_SettlePlatforms();
    Object_SetMode(actor, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(SE_JUMP);
    Object_SetMode(actor, 7);
    actor->speed = 0x30000;
    actor->acceleration = 0x20000;
    actor->velocityY = 0x40000;
    actor->motionFlags &= 0x7e;
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, position[0] >> 16,
                              position[2] >> 16);
    Object_SetMode(actor, 6);
    Engine_ActorSetSpriteFlags(actor, 1);
    actor->motionFlags = flags;
  }
}

// Touching trigger 10 when not already on one: the platform underfoot dips
// with the leader and bobs back up.
static void MercuryLighthouseAerie_StepOntoPlatform(void) {
  struct FieldActor *leader;
  struct FieldActor *actor;
  s32 id;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  id = SceneActor_FindNearestSlotOfKindF2();
  gMercuryLighthouseAerie_PlatformUnderfoot = id;
  if (id != 0) {
    Engine_GameFlagSet(FLAG_ON_PLATFORM);
    actor = Object_GetById(gMercuryLighthouseAerie_PlatformUnderfoot);
    actor->motionFlags = 0;
    leader->motionFlags &= 0xfe;
    actor->y.fixed -= 0x30000;
    leader->y.fixed -= 0x30000;
    leader->terrainHeight -= 0x30000;
    Engine_TaskWait(2);
    actor->y.fixed -= 0x20000;
    leader->y.fixed -= 0x20000;
    leader->terrainHeight -= 0x20000;
    Engine_TaskWait(10);
    actor->y.fixed += 0x20000;
    leader->y.fixed += 0x20000;
    leader->terrainHeight += 0x20000;
    Engine_TaskWait(4);
    actor->y.fixed += 0x20000;
    leader->y.fixed += 0x20000;
    leader->terrainHeight += 0x20000;
    Engine_TaskWait(4);
    actor->y.fixed += 0x10000;
    leader->y.fixed += 0x10000;
    leader->terrainHeight += 0x10000;
  }
  Engine_EventEnd();
}

// Touching trigger 11: the leader walks normally again and the platforms
// stop following.
static void MercuryLighthouseAerie_SettlePlatforms(void) {
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 3;
  Object_GetById(ACTOR_FIRST_PLATFORM)->motionFlags = 4;
  Object_GetById(ACTOR_FIRST_PLATFORM + 1)->motionFlags = 4;
  Object_GetById(ACTOR_FIRST_PLATFORM + 2)->motionFlags = 4;
  Object_GetById(ACTOR_LIFT_PLATFORM_1)->motionFlags = 4;
  Object_GetById(ACTOR_LIFT_PLATFORM_2)->motionFlags = 4;
  Object_GetById(ACTOR_FIRST_PLATFORM + 5)->motionFlags = 4;
}

// Touching trigger 5 before the confrontation: Garet sees "It's too late! The
// lighthouse has already been lit!" Mia learns the party is from Vale; Jenna
// calls out, and Felix, Menardi and Saturos face the party. Menardi leaves
// with Jenna, Kraden and Felix for the next lighthouse, and Saturos fights
// alone (encounter table 36, slot 2).
static void MercuryLighthouseAerie_ConfrontSaturos(void) {
  struct FieldActor *actor;

  Engine_EventBegin();
  Actor_Get(ACTOR_LIFT_PLATFORM_1)->motionFlags &= 0xfa;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x0000cccc, 0x00006666);
  Actor_SetSpeed(GARET, 0x0000cccc, 0x00006666);
  Actor_SetSpeed(IVAN, 0x0000cccc, 0x00006666);
  Actor_SetSpeed(MIA, 0x0000cccc, 0x00006666);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(MIA, actor->x.fixed, actor->z.fixed);
  }
  Engine_TaskWait(1);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 344, 232);
  Actor_WalkTo(GARET, 328, 232);
  Actor_WalkTo(IVAN, 344, 248);
  Actor_WalkTo(MIA, 328, 248);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Engine_ActorWaitForMove(GARET);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Engine_ActorWaitForMove(IVAN);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Engine_ActorWaitForMove(MIA);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Engine_EventWait(50);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Camera_SetSpeed(0x18000, 0x3000);
  Camera_MoveTo(PIXELS(328), PIXELS(40), PIXELS(176), 1);
  Engine_EventGetViewCenter()->motionFlags = 0;
  Actor_SetSpeed(GARET, 0x18000, 0xC000);
  Actor_WalkToAndWait(GARET, 328, 216);
  Engine_CameraWaitForMove();
  Actor_SetSpeed(GARET, 0x0000cccc, 0x00006666);
  Engine_EventWait(60);
  Camera_MoveTo(PIXELS(344), PIXELS(24), PIXELS(232), 1);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 20);
  Engine_CameraWaitForMove();
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventSetMessage(MsgMercuryLighthouseAlreadyLit);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_SetAttachedEffect(MIA, 0x102);
  Engine_EventWait(60);
  Actor_SetSpeed(MIA, 0x18000, 0xC000);
  Actor_WalkToAndWait(MIA, 328, 232);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 30);
  Engine_ActorFaceEachOther(IVAN, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(60);
  Actor_WalkToAndWait(IVAN, 336, 248);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Engine_EventWait(30);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Actor_FaceDirection(MIA, FACING_EAST, 30);
  Engine_EventOpenMessage(MIA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 60);
    Actor_FaceDirection(GARET, FACING_SOUTH, 10);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    Event_ShowMessageAndWait(GARET, 0, 20);
    Actor_FaceDirection(MIA, FACING_NORTH, 0);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(GARET, 0, 20);
    gEventWork->message += 2;
  } else {
    gEventWork->message += 2;
    Engine_EventWait(20);
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 60);
    Actor_FaceDirection(GARET, FACING_SOUTH, 10);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
    Event_ShowMessageAndWait(GARET, 0, 20);
    Actor_FaceDirection(MIA, FACING_NORTH, 0);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(GARET, 0, 20);
  }
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 60);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Actor_FaceDirection(MIA, FACING_EAST, 10);
  Engine_ActorSetAnimation(MIA, 0x10);
  Event_ShowMessageAndWait(MIA, 0, 60);
  // Jenna calls out: "Isaac! Garet!"
  Event_PlayCue(MUS_017);
  Actor_SetPosition(JENNA, PIXELS(216), PIXELS(200));
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Actor_SetPosition(JENNA, PIXELS(120), PIXELS(160));
  Actor_FaceDirection(JENNA, FACING_EAST, 0);
  Engine_ActorSetAnimation(MIA, ANIM_STAND);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 10);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 20);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 10);
  Actor_FaceDirection(MIA, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 5);
  Actor_FaceDirection(GARET, FACING_NORTH, 10);
  Actor_FaceDirection(IVAN, FACING_WEST, 5);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 5);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_FaceDirection(GARET, FACING_WEST, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Camera_SetSpeed(0x30000, 0x6000);
  Camera_MoveTo(PIXELS(120), -PIXELS(24), PIXELS(168), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  Engine_EventWait(20);
  Event_PlayCue(MUS_061);
  Event_ShowMessageAndWait(ACTOR_FELIX, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH, 60);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 20);
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
  Actor_SetPosition(GARET, PIXELS(264), PIXELS(288));
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_SetPosition(GARET, PIXELS(328), PIXELS(216));
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTHWEST, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  Engine_EventWait(20);
  Actor_FaceDirection(JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 30);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 4);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
  Actor_SetSpeed(ACTOR_FELIX, 0x0000cccc, 0x00006666);
  Actor_WalkToAndWait(ACTOR_FELIX, 104, 168);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 10);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | 2, 70);
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(264), PIXELS(288));
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(296), PIXELS(120));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Camera_SetSpeed(0x18000, 0x3000);
  Camera_MoveTo(PIXELS(232), PIXELS(40), PIXELS(152), 1);
  Actor_SetSpeed(ACTOR_SATUROS, 0x0000cccc, 0x00006666);
  Actor_WalkToAndWait(ACTOR_SATUROS, 272, 128);
  Actor_WalkToAndWait(ACTOR_SATUROS, 264, 152);
  Actor_WalkToAndWait(ACTOR_SATUROS, 280, 168);
  Engine_CameraWaitForMove();
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 20);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_MENARDI, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Camera_MoveTo(PIXELS(296), PIXELS(40), PIXELS(216), 1);
  Actor_WalkToAndWait(ACTOR_SATUROS, 304, 176);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTHEAST, 0);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Engine_EventWait(20);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 7, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 7, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 7, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 7, 70);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 5, 70);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_MENARDI, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(40);
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_MENARDI, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Engine_EventWait(20);
  Actor_Get(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkTo(GARET, 328, 224);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 344, 224);
  Actor_WalkTo(IVAN, 344, 232);
  Engine_ActorWaitForMove(GARET);
  Actor_Get(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Engine_ActorWaitForMove(IVAN);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(30);
  Actor_SetPosition(ACTOR_MENARDI, PIXELS(168), PIXELS(200));
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
  Actor_SetPosition(ACTOR_MENARDI, PIXELS(104), PIXELS(200));
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
  Camera_SetSpeed(0x30000, 0x6000);
  Camera_MoveTo(PIXELS(120), -PIXELS(24), PIXELS(168), 1);
  Engine_CameraWaitForMove();
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
  Actor_FaceDirection(JENNA, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 70);
  Engine_ActorFaceEachOther(JENNA, ACTOR_KRADEN, 0);
  Engine_EventWait(50);
  Actor_FaceDirection(JENNA, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(JENNA, 4);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT, 60);
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 5, 60);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
  Actor_SetAttachedEffect(ACTOR_FELIX, 0x102);
  Engine_EventWait(60);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_FELIX, 0, 20);
  Actor_SetAttachedEffect(JENNA, 0x102);
  Engine_EventWait(60);
  Event_ShowMessageAndWait(JENNA, 0, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 3);
  Engine_EventWait(60);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 30);
  Actor_SetSpeed(JENNA, 0x0000b333, 0x00005999);
  Actor_SetSpeed(ACTOR_KRADEN, 0x0000b333, 0x00005999);
  Actor_WalkTo(JENNA, 128, 144);
  Actor_WalkToAndWait(ACTOR_KRADEN, 120, 136);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 0);
  Engine_ActorWaitForMove(JENNA);
  Actor_FaceDirection(JENNA, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 20);
  Object_GetById(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_FELIX, 88, 152);
  Actor_Get(ACTOR_FELIX)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorSetAnimation(ACTOR_MENARDI, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 3);
  Engine_EventWait(40);
  Actor_SetSpeed(ACTOR_MENARDI, 0x30000, 0x20000);
  Actor_Get(ACTOR_MENARDI)->velocityY = 0x40000;
  Event_PlayCue(SE_JUMP);
  Actor_Get(ACTOR_MENARDI)->motionFlags &= 0x7e;
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_MENARDI), 0);
  Object_GetById(ACTOR_LIFT_PLATFORM_1)->motionFlags = 4;
  Engine_ActorMoveToAndWait(ACTOR_MENARDI, 104, 168);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_MENARDI), 1);
  Actor_Get(ACTOR_MENARDI)->motionFlags = 3;
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 10);
  Event_ShowMessageAndWait(ACTOR_FELIX, 0, 20);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_ActorSetAnimationAndWait(JENNA, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
  Engine_ActorSetSpritePriority(ACTOR_LIFT_PLATFORM_1, 0);
  Engine_ActorSetSpritePriority(ACTOR_LIFT_PLATFORM_2, 0);
  MercuryLighthouseAerie_LowerLift();
  Engine_ActorSetSpritePriority(ACTOR_LIFT_PLATFORM_1, 1);
  Engine_ActorSetSpritePriority(ACTOR_LIFT_PLATFORM_2, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_SOUTHWEST, 0);
  Camera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(216), 1);
  Engine_CameraWaitForMove();
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(272), PIXELS(280));
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 20);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_SetSpeed(GARET, 0x18000, 0xC000);
  Actor_WalkToAndWait(GARET, 312, 216);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 10);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_WalkToAndWait(ACTOR_SATUROS, 312, 184);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 60);
  Actor_SetSpeed(GARET, 0x20000, 0x10000);
  Actor_Get(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(GARET, 328, 224);
  Engine_EventWait(1);
  Actor_Get(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 1, 60);
  Engine_EventOpenMessage(ACTOR_SATUROS, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
    gEventWork->message++;
  } else {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 4);
    Engine_EventWait(20);
    gEventWork->message++;
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
  }
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
  Actor_WalkToAndWait(ACTOR_SATUROS, 328, 200);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_Get(ACTOR_SATUROS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_SATUROS, 336, 208);
  Engine_EventWait(1);
  Object_GetById(ACTOR_SATUROS)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 2, 60);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 60);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
  Actor_WalkToAndWait(ACTOR_SATUROS, 336, 216);
  Event_SetNextDestination((s32)&SceneId_MercuryLighthouseAerie1, 2);
  Battle_SetStart(3);
  // Saturos: encounter table 36, slot 2.
  Field_RequestBattleFromEncounterTable(36, 2);
  Engine_EventEnd();
}

// Entrance 2, back from the battle, unless resuming: "Isaac, we got him!"
// Alex appears and explains that the lighthouse bound Saturos' fire
// Psynergy and lent Mia endless power. Saturos stands again; the two warp
// over to the lift and ride it down, and the party regroups on the leader.
static void MercuryLighthouseAerie_AlexAppears(void) {

  Engine_EventBegin();
  Actor_Get(ACTOR_LIFT_STATUE)->motionFlags = 0;
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(344), PIXELS(224));
  Actor_SetPosition(GARET, PIXELS(328), PIXELS(224));
  Actor_SetPosition(IVAN, PIXELS(344), PIXELS(232));
  Actor_SetPosition(MIA, PIXELS(328), PIXELS(232));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(336), PIXELS(176));
  Engine_ActorSetAnimation(ACTOR_SATUROS, 9);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SATUROS), 0);
  Camera_MoveTo(PIXELS(336), -1, PIXELS(208), 0);
  Engine_TaskWait(1);
  Engine_MapRedraw();
  Engine_TaskWait(1);

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_EAST, 20);
  Engine_EventSetMessage(MsgMercuryGot);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 20);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 60);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 30);
  Actor_SetPosition(ACTOR_ALEX, PIXELS(312), PIXELS(112));
  Event_PlayCue(SE_288);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 60);
  Event_ShowMessageAndWait(MIA, 0, 30);
  Event_PlayCue(MUS_029);
  Camera_SetSpeed(0x0000cccc, 0x00001999);
  Engine_EventGetViewCenter()->motionFlags = 0;
  Camera_MoveTo(PIXELS(336), -1, PIXELS(168), 1);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_ALEX, 0x0000cccc, 0x00006666);
  Actor_WalkToAndWait(ACTOR_ALEX, 344, 136);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH + FACING_STEP, 20);
  Engine_CameraWaitForMove();
  Event_ShowMessageAndWait(MIA, 0, 30);
  Camera_MoveTo(PIXELS(336), -1, PIXELS(184), 1);
  Actor_WalkToAndWait(ACTOR_ALEX, 344, 160);
  Actor_WalkToAndWait(ACTOR_ALEX, 328, 168);
  Actor_WalkToAndWait(ACTOR_ALEX, 312, 176);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_CameraWaitForMove();
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 20);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 60);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 20);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_ALEX, 2);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT, 60);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_WalkToAndWait(ACTOR_ALEX, 312, 184);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_EAST, 20);
  Engine_EventOpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Event_ShowMessageAndWait(GARET, 0, 20);
    gEventWork->message++;
  } else {
    gEventWork->message++;
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Event_ShowMessageAndWait(GARET, 0, 20);
  }
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Actor_FaceDirection(GARET, FACING_NORTH, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 60);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTHWEST + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_ALEX, 2);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(30);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 4);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_ALEX, 2);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 60);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_ALEX, 2);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST + FACING_STEP, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | 8, 60);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
  Engine_EventWait(30);
  Engine_ActorSetAnimation(ACTOR_SATUROS, 8);
  Engine_EventWait(45);
  Engine_ActorSetAnimation(ACTOR_SATUROS, 1);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SATUROS), 1);
  Engine_EventWait(40);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 60);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTHEAST, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 4);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(30);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(ACTOR_ALEX, ACTOR_SATUROS, 0);
  Engine_EventWait(35);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
  Engine_EventWait(20);
  Actor_WalkToAndWait(ACTOR_ALEX, 328, 176);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ActorSetAnimation(ACTOR_ALEX, 5);
  Engine_ActorSetAnimation(ACTOR_SATUROS, 7);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | 1, 60);
  Engine_EventOpenMessage(ACTOR_ALEX, 0);
  Engine_EventWait(30);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 0);
  Engine_EventWait(20);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_ALEX, 0);
    gEventWork->message++;
  } else {
    Engine_EventWait(30);

    gEventWork->message++;
    Event_ShowMessage(ACTOR_ALEX, 0);
  }
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 4);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
  MercuryLighthouseAerie_SaturosAndAlexWarpOut();
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 5);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 5);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 5);
  Actor_FaceDirection(MIA, FACING_WEST, 5);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 5);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 5);
  Actor_FaceDirection(IVAN, FACING_WEST, 5);
  Actor_FaceDirection(MIA, FACING_EAST, 5);
  Engine_EventGetViewCenter()->motionFlags = 0;
  Camera_MoveTo(PIXELS(280), -1, PIXELS(232), 1);
  Engine_CameraWaitForMove();
  Actor_SetChildValue(ACTOR_SATUROS, 0xF);
  Actor_SetChildValue(ACTOR_ALEX, 0xF);
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(240), PIXELS(208));
  Actor_SetPosition(ACTOR_ALEX, PIXELS(232), PIXELS(208));
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST + FACING_STEP, 0);
  MercuryLighthouseAerie_SaturosAndAlexWarpIn();
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
  Actor_FaceDirection(GARET, FACING_WEST, 15);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 20);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 328, 208);
  Actor_WalkTo(IVAN, 336, 224);
  Actor_WalkToAndWait(GARET, 312, 216);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Engine_ActorWaitForMove(IVAN);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 30);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_FaceDirection(GARET, FACING_WEST, 20);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_FaceDirection(IVAN, FACING_WEST, 20);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  Camera_SetSpeed(0x30000, 0x6000);
  {
    struct MapLayerScroll *lift;
    void **cells;
    Camera_MoveTo(PIXELS(152), -1, PIXELS(216), 1);
    lift = &((struct MapScrollWork *)gMapWork[0])->layers[2];
    lift->offsetY = 0x3800000;
    cells = gMapWork;
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Actor_Get(ACTOR_LIFT_STATUE)->motionFlags = 0;
    Actor_SetPosition(ACTOR_LIFT_STATUE, PIXELS(104), PIXELS(264));
    Actor_Get(ACTOR_LIFT_STATUE)->y.fixed = -PIXELS(32);
    Object_GetById(ACTOR_LIFT_STATUE)->targetY = -PIXELS(32);
    Map_CopyCellsTo(0x1D, 0x4A, 4, 0x4A, 5, 4);
    Engine_ActorSetSpritePriority(ACTOR_LIFT_PLATFORM_1, 0);
    Engine_ActorSetSpritePriority(ACTOR_LIFT_PLATFORM_2, 0);
    MercuryLighthouseAerie_RaiseLift();
    Engine_ActorSetSpritePriority(ACTOR_LIFT_PLATFORM_1, 1);
    Engine_ActorSetSpritePriority(ACTOR_LIFT_PLATFORM_2, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_ALEX, FACING_WEST, 20);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
    Camera_MoveTo(PIXELS(280), -1, PIXELS(216), 1);
    Engine_CameraWaitForMove();
    Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST + FACING_STEP, 20);
    Engine_ActorSetAnimation(ACTOR_ALEX, 5);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
    Engine_ActorRunRepeatedMotion(IVAN, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
    Event_ShowMessageAndWait(IVAN, 0, 20);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(GARET, 0, 20);
    Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | 1, 60);
    Engine_EventOpenMessage(ACTOR_ALEX, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventWait(20);
      Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
      Engine_EventWait(20);
      Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
      ((struct EventWork *)cells[19])->message++;
    } else {
      ((struct EventWork *)cells[19])->message++;
      Engine_EventWait(20);
      Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
    }
  }
  Engine_ActorRunRepeatedMotion(ACTOR_ALEX, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 60);
  Engine_EventOpenMessage(ACTOR_ALEX, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_ALEX, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
    gEventWork->message++;
  } else {
    gEventWork->message++;
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_ALEX, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  }
  Engine_EventWait(20);
  MercuryLighthouseAerie_SaturosAndAlexWarpOut();
  Engine_EventWait(20);
  Camera_MoveTo(PIXELS(128), -1, PIXELS(200), 1);
  Engine_CameraWaitForMove();
  Engine_ActorSetAnimation(ACTOR_SATUROS, 1);
  Engine_ActorSetAnimation(ACTOR_ALEX, 1);
  Actor_SetChildValue(ACTOR_SATUROS, 0xF);
  Actor_SetChildValue(ACTOR_ALEX, 0xF);
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(120), PIXELS(152));
  Actor_SetPosition(ACTOR_ALEX, PIXELS(112), PIXELS(160));
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST + FACING_STEP, 0);
  MercuryLighthouseAerie_SaturosAndAlexWarpIn();
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 20);
  Engine_EventOpenMessage(ACTOR_ALEX, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
    gEventWork->message++;
  } else {
    gEventWork->message++;
    Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  }
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_EAST, 20);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 20);
  Engine_ActorSetSpritePriority(ACTOR_LIFT_PLATFORM_1, 0);
  Engine_ActorSetSpritePriority(ACTOR_LIFT_PLATFORM_2, 0);
  MercuryLighthouseAerie_LowerLift();
  Engine_ActorSetSpritePriority(ACTOR_LIFT_PLATFORM_1, 1);
  Engine_ActorSetSpritePriority(ACTOR_LIFT_PLATFORM_2, 1);
  Event_PlayCue(MUS_017);
  Camera_MoveTo(PIXELS(336), -1, PIXELS(216), 1);
  Engine_CameraWaitForMove();
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Audio_PlayCueFromEventWork();
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 22);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 5, 60);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_FaceDirection(MIA, FACING_WEST, 20);
  Engine_ActorSetAnimation(MIA, 0x10);
  Actor_Get(MIA)->scaleX = -0x10000;
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 40);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventOpenMessage(IVAN, 0);
  Engine_ActorSetAnimation(MIA, ANIM_STAND);
  Actor_Get(MIA)->scaleX = 0x10000;
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 20);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(IVAN, 0, 20);
    gEventWork->message++;
  } else {
    gEventWork->message++;
    Engine_ActorRunRepeatedMotion(IVAN, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(IVAN, 0, 20);
  }
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 30);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 60);
  Actor_SetSpeed(MIA, 0x0000cccc, 0x00006666);
  Actor_SetSpeed(GARET, 0x0000cccc, 0x00006666);
  Actor_WalkToAndWait(MIA, 328, 216);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_Get(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkTo(GARET, 312, 200);
  Actor_WalkTo(MIA, 280, 216);
  Engine_ActorWaitForMove(GARET);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  Engine_ActorWaitForMove(MIA);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(30);
  Actor_FaceDirection(MIA, FACING_EAST, 20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Actor_SetAttachedEffect(IVAN, 0x102);
  Engine_EventWait(60);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_FaceDirection(MIA, FACING_EAST, 20);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 30);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 312, 216);
  Actor_WalkTo(GARET, 312, 216);
  Actor_WalkTo(IVAN, 312, 216);
  Actor_WalkTo(MIA, 312, 216);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorWaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Engine_ActorWaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  {
    struct MapLayerScroll *lift =
        &((struct MapScrollWork *)gMapWork[0])->layers[2];

    lift->offsetY = 0x4000000;
  }
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Map_CopyCellsTo(4, 0x46, 4, 0x4A, 5, 4);
  GameFlag_Set(FLAG_MERCURY_AERIE_CONFRONTATION_DONE);
  GameFlag_Set(FLAG_SATUROS_BEATEN_AT_MERCURY);
  Engine_EventEnd();
}

// Touching trigger 6 while the lift is down: the camera watches it rise with
// its statue.
static void MercuryLighthouseAerie_CallLift(void) {
  struct MapLayerScroll *lift;

  lift = &((struct MapScrollWork *)gMapWork[0])->layers[2];
  Engine_EventBegin();
  lift->offsetY = 0x3800000;
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Object_GetById(ACTOR_LIFT_STATUE)->motionFlags = 0;
  Engine_ActorSetPosition(ACTOR_LIFT_STATUE, PIXELS(104), PIXELS(264));
  Object_GetById(ACTOR_LIFT_STATUE)->y.fixed = -PIXELS(32);
  Object_GetById(ACTOR_LIFT_STATUE)->targetY = -PIXELS(32);
  Engine_EventGetViewCenter()->motionFlags = 0;
  Engine_CameraSetSpeed(0xcccc, 0x1999);
  Engine_CameraMoveTo(PIXELS(128), -1, PIXELS(184), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(30);
  Map_CopyCellsTo(29, 74, 4, 74, 5, 4);
  Engine_ActorSetSpritePriority(ACTOR_LIFT_PLATFORM_1, 0);
  Engine_ActorSetSpritePriority(ACTOR_LIFT_PLATFORM_2, 0);
  MercuryLighthouseAerie_RaiseLift();
  Engine_ActorSetSpritePriority(ACTOR_LIFT_PLATFORM_1, 1);
  Engine_ActorSetSpritePriority(ACTOR_LIFT_PLATFORM_2, 1);
  Engine_EventWait(20);
  Engine_GameFlagSet(FLAG_LIFT_UP);
  Engine_EventEnd();
}

// Touching trigger 12: the leader steps onto the lift and rides it down to
// the lighthouse's foot (exit 1).
static void MercuryLighthouseAerie_RideLiftDown(void) {
  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 104, 152);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 60);
  Engine_ActorSetSpritePriority(ACTOR_LIFT_PLATFORM_1, 0);
  Engine_ActorSetSpritePriority(ACTOR_LIFT_PLATFORM_2, 0);
  MercuryLighthouseAerie_LowerLift();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_EventRequestExit(1);
  Engine_EventEnd();
}

// Saturos and Alex warp out in a column of sparkles and vanish.
static void MercuryLighthouseAerie_SaturosAndAlexWarpOut(void) {
  struct EffectOptions options;
  struct EffectOptions *effectOptions;
  struct FieldActor *left;
  struct FieldActor *right;
  u32 i;
  s32 x;
  s32 r;

  left = Object_GetById(ACTOR_SATUROS);
  right = Object_GetById(ACTOR_ALEX);
  Engine_AudioPlayCue(SE_190);
  Actor_SetChildValue(ACTOR_SATUROS, 0x100);
  Engine_ActorSetChildValue(ACTOR_ALEX, 0x100);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SATUROS), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ALEX), 0);
  effectOptions = &options;
  effectOptions->priority = 1;
  effectOptions->palette = 5;
  effectOptions->type = 0x11c;
  effectOptions->startScaleX = 0x6666;
  effectOptions->startScaleY = 0x30000;
  for (i = 0; i <= 31; i++) {
    Engine_EventWait(1);
    if (i & 1) {
      r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
      x = left->x.fixed;
      x += r;
      x += -0xc0000;
      Effect_Spawn(x,
                   left->y.fixed +
                       ((((u32)Engine_RandomNext() << 5) >> 16) << 16) +
                       -0x100000,
                   left->z.fixed, 0, 0x40000, 0,
                   EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                       EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                   effectOptions);
    } else {
      r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
      x = right->x.fixed;
      x += r;
      x += -0xc0000;
      Effect_Spawn(x,
                   right->y.fixed +
                       ((((u32)Engine_RandomNext() << 5) >> 16) << 16) +
                       -0x100000,
                   right->z.fixed, 0, 0x40000, 0,
                   EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                       EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                   effectOptions);
    }
  }
  Engine_ActorSetPosition(ACTOR_SATUROS, 0, 0);
  Engine_ActorSetPosition(ACTOR_ALEX, 0, 0);
}

// Saturos and Alex warp in where they have been placed.
static void MercuryLighthouseAerie_SaturosAndAlexWarpIn(void) {
  struct EffectOptions options;
  struct EffectOptions *effectOptions;
  struct FieldActor *left;
  struct FieldActor *right;
  u32 i;
  s32 x;
  s32 r;

  left = Object_GetById(ACTOR_SATUROS);
  right = Object_GetById(ACTOR_ALEX);
  Engine_AudioPlayCue(SE_190);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SATUROS), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ALEX), 0);
  effectOptions = &options;
  effectOptions->priority = 1;
  effectOptions->palette = 5;
  effectOptions->type = 0x11c;
  effectOptions->startScaleX = 0x6666;
  effectOptions->startScaleY = 0x30000;
  for (i = 0; i <= 31; i++) {
    Engine_EventWait(1);
    if (i & 1) {
      r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
      x = left->x.fixed;
      x += r;
      x += -0xc0000;
      Effect_Spawn(x,
                   left->y.fixed +
                       ((((u32)Engine_RandomNext() << 5) >> 16) << 16) +
                       0x200000,
                   left->z.fixed, 0, -0x40000, 0,
                   EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                       EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                   effectOptions);
    } else {
      r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
      x = right->x.fixed;
      x += r;
      x += -0xc0000;
      Effect_Spawn(x,
                   right->y.fixed +
                       ((((u32)Engine_RandomNext() << 5) >> 16) << 16) +
                       0x200000,
                   right->z.fixed, 0, -0x40000, 0,
                   EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                       EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                   effectOptions);
    }
    if (i == 20) {
      Actor_SetChildValue(ACTOR_SATUROS, 0x100);
      Engine_ActorSetChildValue(ACTOR_ALEX, 0x100);
    }
  }
  Engine_ActorSetChildValue(ACTOR_SATUROS, 0);
  Engine_ActorSetChildValue(ACTOR_ALEX, 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SATUROS), 1);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ALEX), 1);
}

// MercuryLighthouseAerie_LowerLift passes its lift as a third
// argument, which this function does not take.
static void MercuryLighthouseAerie_FindLiftRiders(s32 *out, s32 y) {
  struct ActorTable *table;
  u32 slot;

  table = (struct ActorTable *)gEventWork;
  y = 64 - (y >> 20);
  for (slot = 0; slot <= 65; slot++) {
    struct FieldActor *actor = table->actors[slot];

    if (actor != 0) {
      s32 cell = actor->x.fixed >> 20;
      s32 row = actor->z.fixed >> 20;

      if (cell >= 4 && cell <= 8 && y + 8 <= row && row < y + 11)
        *out++ = slot;
    }
  }
}

// The lift sinks with everything standing on it, speeding up.
static void MercuryLighthouseAerie_LowerLift(void) {
  s32 ids[5];
  struct MapLayerScroll *lift;
  u32 i;
  u32 j;
  u32 n;
  s32 speed;
  struct FieldActor *actor;

  lift = &((struct MapScrollWork *)gMapWork[0])->layers[2];
  speed = 0x1999;
  n = 0;
  for (i = 0; i <= 4; i++)
    ids[i] = 66;
  // the lift goes as a third argument, as the bytes show: the cast keeps it
  ((void (*)())MercuryLighthouseAerie_FindLiftRiders)(ids, lift->offsetY, lift);
  for (i = 0; i <= 4; i++) {
    if (ids[i] == 66)
      break;
    Object_GetById(ids[i])->motionFlags = 0;
    n++;
  }
  Engine_AudioPlayCue(SE_223);
  for (i = 0; i <= 227; i++) {
    lift->offsetY -= speed;
    for (j = 0; j < n; j++) {
      Object_GetById(ids[j])->z.fixed += speed;
      actor = Object_GetById(ids[j]);
      actor->targetZ = Object_GetById(ids[j])->z.fixed;
    }
    if ((i & 3) == 3)
      speed += 0x1999;
    if (speed > 0x17fff)
      speed = 0x18000;
    Engine_TaskWait(1);
  }
  for (i = 0; i < n; i++)
    Object_GetById(ids[i])->motionFlags = 0;
}

// The lift rises with everything standing on it, slowing at the top.
static void MercuryLighthouseAerie_RaiseLift(void) {
  s32 ids[5];
  struct MapLayerScroll *lift;
  u32 i;
  u32 j;
  u32 n;
  s32 speed;
  struct FieldActor *actor;

  lift = &((struct MapScrollWork *)gMapWork[0])->layers[2];
  speed = 0x18000;
  n = 0;
  for (i = 0; i <= 4; i++)
    ids[i] = 66;
  MercuryLighthouseAerie_FindLiftRiders(ids, lift->offsetY);
  for (i = 0; i <= 4; i++) {
    if (ids[i] == 66)
      break;
    Object_GetById(ids[i])->motionFlags = 0;
    n++;
  }
  Engine_AudioPlayCue(SE_223);
  for (i = 0; i <= 85; i++) {
    lift->offsetY += speed;
    for (j = 0; j < n; j++) {
      Object_GetById(ids[j])->z.fixed -= speed;
      actor = Object_GetById(ids[j]);
      actor->targetZ = Object_GetById(ids[j])->z.fixed;
    }
    if ((i & 3) == 3 && i > 75)
      speed += -0x3333;
    if (speed < 0xccc)
      speed = 0xccc;
    Engine_TaskWait(1);
  }
  lift->offsetY = 0x4000000;
  Engine_MapRedraw();
  Engine_TaskWait(2);
}

// Alex casts Ply on the statue; nothing calls it.
UNUSED static void MercuryLighthouseAerie_AlexCastsPly(void) {
  Engine_PsynergyBegin(ABILITY_PLY, 1);
  Engine_PsynergySetTarget(ACTOR_ALEX, ACTOR_LIFT_STATUE);
  Engine_PsynergyRaiseHands();
  Engine_PsynergyPlayEffect(1);
  Engine_PsynergyLowerHands();
  BattleEffect_CleanupSceneObjects();
}

// A beacon spark's update: it swings round the flame, its angle stepping
// back each frame.
static void SceneEffect_UpdateArcPosition(union FieldObject *fieldObject) {
  struct OverlayObject *object = (struct OverlayObject *)fieldObject;
  struct OverlayObject *parent;
  u16 angle;

  parent = object->linkedObject;
  angle = object->angle64;
  object->coordinate08 =
      parent->coordinate08 + Engine_MathCos(angle) * (object->field30 + 28);
  object->coordinate10 = (Engine_MathSin(angle) << 4) + 0x900000;
  object->coordinate38 = object->coordinate08;
  object->coordinate40 = object->coordinate10;
  object->angle64 -= 0x200;
}

struct BeaconSpark {
  u8 unk00[0x30];
  s32 swing;
  u8 unk34[0x1c];
  struct AnimationObject *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[0x0e];
  u16 angle;
  u16 turn;
  struct FieldActor *source;
  void (*update)();
};

// Every frame: while the view is near the top the beacon flame flickers
// between two sizes, and every 16 frames it throws off a spark that swings
// round it.
static void MercuryLighthouseAerie_FlickerBeacon(void) {
  struct FieldActor *beacon;
  struct BeaconSpark *spark;
  struct AnimationObject *sprite;
  struct MapScrollWork *map;
  s32 height;
  s32 scale;
  s32 tick;
  u32 angle;

  beacon = Object_GetById(ACTOR_BEACON_FLAME);
  map = gMapWork[0];
  height = ((u32)(Engine_RandomNext() * 48) >> 16) << 16;
  if (((union FieldCoordinate *)&map->viewY)->part.pixel <= 129) {
    if (gFrameCount & 1) {
      Engine_ActorSetPosition(ACTOR_BEACON_FLAME, PIXELS(304), PIXELS(144));
      scale = 0x10000;
      Object_GetById(ACTOR_BEACON_FLAME)->scaleX = scale;
    } else {
      Engine_ActorSetPosition(ACTOR_BEACON_FLAME, PIXELS(304), PIXELS(151));
      scale = 0x14ccc;
      Object_GetById(ACTOR_BEACON_FLAME)->scaleX = scale;
    }
    Object_GetById(ACTOR_BEACON_FLAME)->scaleY = scale;
  } else {
    Actor_SetPosition(ACTOR_BEACON_FLAME, PIXELS(8), PIXELS(8));
  }
  if (beacon == NULL) {
    return;
  }
  tick = gFrameCount & 15;
  if (tick != 0) {
    return;
  }
  spark = (struct BeaconSpark *)Object_Create(
      0x11c, beacon->x.fixed + 0x80000, beacon->y.fixed + height + 0x80000,
      beacon->z.fixed);
  height = height / 0x60000;
  height <<= 16;
  if (spark == NULL) {
    return;
  }
  sprite = spark->sprite;
  Engine_ObjectSetScript((struct FieldActor *)spark,
                         gMercuryLighthouseAerie_SparkScript);
  ObjectGroup_SetChildValue((struct FieldActor *)spark, 3);
  spark->motionFlags = tick;
  angle = Engine_RandomNext() & 0x0ffff000;
  spark->angle = angle;
  spark->turn = tick;
  spark->source = beacon;
  spark->update = SceneEffect_UpdateArcPosition;
  spark->swing = (Engine_MathSin((height & 0xfffff) >> 4) * 24) >> 16;
  /* FAKEMATCH: the byte view keeps this store ahead of the beacon's reload; the
   * field store lets the reload schedule first (score 210). Field store:
   * differ 2. */
  *(u8 *)&sprite->flags = 0;
  sprite->image.priority = beacon->sprite->image.priority;
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

struct SceneEntrance gMercuryLighthouseAerieEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 8, 0, 8, FACING_SOUTH, 0, 0, 0, 608, 424, 0},
    {2, CONDITION_ALWAYS, 8, 0, 8, FACING_SOUTH, 0, 0, 0, 608, 424, 0},
    {5, CONDITION_ALWAYS, 504, 0, 152, FACING_SOUTH, 0, 0, 0, 608, 424, 0},
    {51, CONDITION_ALWAYS, 504, 0, 168, FACING_SOUTH, 0, 0, 0, 608, 424, 0},
    {SCENE_TABLE_END},
};

u32 gMercuryLighthouseAerieExits[] = {
    SCENE_EXITS(58),
    SCENE_EXIT(1, 57, 15),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gMercuryLighthouseAeriePlacements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTHWEST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTHEAST, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { JENNA, FLAG_HOSTAGES_GONE, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(160), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0xf4, CONDITION_ALWAYS, 7,
        PIXELS(304), PIXELS(40), PIXELS(144), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xf2, CONDITION_ALWAYS, 7,
        PIXELS(440), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf2, CONDITION_ALWAYS, 7,
        PIXELS(472), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xf2, CONDITION_ALWAYS, 7,
        PIXELS(504), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xf2, CONDITION_ALWAYS, 7,
        PIXELS(104), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xf2, CONDITION_ALWAYS, 7,
        PIXELS(136), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xf2, CONDITION_ALWAYS, 7,
        PIXELS(168), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x1e, FLAG_HOSTAGES_GONE, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(152), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x23, FLAG_HOSTAGES_GONE, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(160), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x20, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x21, FLAG_HOSTAGES_GONE, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(200), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x2b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gMercuryLighthouseAerieEvents[] = {
    {EVENT_TOUCH, 5, FLAG_MERCURY_AERIE_CONFRONTATION_DONE,
     EVENT_SCRIPT(MercuryLighthouseAerie_ConfrontSaturos)},
    {EVENT_TOUCH, 6, FLAG_LIFT_UP,
     EVENT_SCRIPT(MercuryLighthouseAerie_CallLift)},
    {0x202, 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(MercuryLighthouseAerie_JumpToPlatform)},
    {EVENT_TOUCH, 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(MercuryLighthouseAerie_SettlePlatforms)},
    {0x202, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(MercuryLighthouseAerie_JumpToPlatform)},
    {EVENT_TOUCH, 10, FLAG_ON_PLATFORM,
     EVENT_SCRIPT(MercuryLighthouseAerie_StepOntoPlatform)},
    {SEARCH(SEARCH_UNNAMED), 100, FLAG_PSYNERGY_STONE, FIND_PSYNERGY_STONE},
    {EVENT_TOUCH, 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(MercuryLighthouseAerie_RideLiftDown)},
    {SCENE_EVENTS_END},
};

s32 gMercuryLighthouseAerie_PlatformUnderfoot = 0;

s32 gMercuryLighthouseAerie_SparkScript[] = {
    SC_SET(SW_SCALE_Y, 0x16666),
    SC_SET(SW_SCALE_X, 0x16666),
    SC_ARG(4),
    SC_WAIT(30),
    SC_FREE,
};
