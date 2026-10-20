/// @file
/// Babi Lighthouse, the lighthouse Babi is building on old ruins near
/// Lalivero. Four scenes share this overlay:
///
/// - room 1, the stone door Sheba's kidnappers went through, where Iodem and
///   Faran ask the party to follow them; a switch only Reveal shows opens it;
/// - room 2, inside, where the party drops between floors, with a Djinni;
/// - room 3, where the kidnappers knocked down four men on their way, with two
///   blocks on ledges and a sprout for Growth;
/// - the building site outside, with its workers and a block to move.

#include "game_state.h"
#include "version.h"
#include "global_cells.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/babi_lighthouse.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "macros.h"

#define FIELD_STAGED_ACTOR_IMPORTS
#include "mapcopy.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "djinn_ids.h"
#include "field_effect.h"
#include "staged_actor.h"
#include "spawn_configured_effect.h"
#include "select_overlay_data_by_runtime_selector.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "fx_scene.h"
#include "call.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "field_obj.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"

/// Room 1, before the stone door: the door's two halves, Iodem, the man who
/// saw the kidnappers go through, two more onlookers and Faran.
enum {
  ACTOR_LEFT_DOOR = 8,
  ACTOR_RIGHT_DOOR = 9,
  ACTOR_IODEM = 10,
  ACTOR_WITNESS = 11,
  ACTOR_FIRST_ONLOOKER = 12,
  ACTOR_SECOND_ONLOOKER = 13,
  ACTOR_FARAN = 14
};

/// Room 2's Djinni.
enum { ACTOR_DJINNI = 8 };

/// Room 3: the two blocks on the ledges, the sprout for Growth and the four
/// men the kidnappers knocked down on their way in.
enum {
  ACTOR_LEFT_BLOCK = 8,
  ACTOR_RIGHT_BLOCK = 9,
  ACTOR_SPROUT = 10,
  ACTOR_FIRST_FALLEN_MAN = 11
};

/// The building site outside: four workers and the block for Move and Carry.
enum { ACTOR_SITE_BLOCK = 12 };

/// The lighthouse keeps two flags in the area flags from 0x300.
enum {
  /// Growth has grown room 3's sprout into a vine.
  FLAG_GROWTH_PASSAGE_OPEN = 0x300,
  /// The party is on the far side of room 1's stone door.
  FLAG_BEYOND_DOOR = 0x301
};

/// A chest searched while facing the given way.
#define SEARCH_CHEST_FACING(facing)                                            \
  (SEARCH(SEARCH_CHEST) | EVENT_TOUCH_FACING | (facing))

static void BabiLighthouse_CloseTruthDoor(void);
static void BabiLighthouse_RefreshMovableBlockTiles(void);

/// Unused: a signed division nothing calls.
static __inline__ s32 UNUSED Entrance_Divide(s32 dividend, s32 divisor) {
  return dividend / divisor;
}

#include "common/object_staged_actor.h"

#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// Talking to the Djinni in room 2 starts the battle for Djinni 66.
static void BabiLighthouse_StartDjinnBattle(void) {
  Field_RequestDjinnBattle(ACTOR_DJINNI, DJINNI_LUFF);
}

#include "common/object_waitidle.h"
#include "common/effect_advance.h"

/// Arriving by falling from the floor above: the leader drops in, lands with
/// a thud that shakes the screen and throws up a ring of dust, and is startled.
static void BabiLighthouse_LandWithParticles(void) {
  u32 particle;
  struct FieldActor *leader;
  struct EffectOptions options;
  s32 velocity[3];

  // Start above the room with movement disabled until the fall begins.
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Object_GetById(ACTOR_PARTY_LEADER)->y.fixed = 0x820000;
  Object_GetById(ACTOR_PARTY_LEADER)->gravity = 0x8000;
  Object_GetById(ACTOR_PARTY_LEADER)->bounce = 0;
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(30);
  Engine_AudioPlayCue(SE_204);
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 3;
  Engine_EventWait(24);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  // Spread the landing particles around the leader before restoring bounce.
  options.palette = 7;
  options.update = (void (*)(union FieldObject *))Effect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  for (particle = 0; particle < 17; particle++) {
    SET_VECTOR(velocity, Engine_MathCos(particle << 12), 0,
               Engine_MathSin(particle << 12));
    velocity[0] += velocity[0] / 2;
    Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed, velocity[0],
                 velocity[1], velocity[2],
                 EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                     EFFECT_USE_PALETTE | 1,
                 &options);
  }
  Engine_AudioPlayCue(SE_188);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 22);
  Map_SetShake(0x50000, 0x50000, 0x10000);
  Map_SetShake(-1, -1, 0xe666);
  Engine_MapRenderWaitForValues();
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT);
  Object_GetById(ACTOR_PARTY_LEADER)->gravity = 0x10000;
  Object_GetById(ACTOR_PARTY_LEADER)->bounce = 0x4000;
  Engine_EventEnd();
}

/// Arriving at one of room 2's drop entrances: the leader drops in and falls
/// on, out through the given exit.
static void BabiLighthouse_FallToExit(s32 exit) {
  struct FieldActor *leader;

  // Start above the room with movement disabled until the fall begins.
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leader->y.fixed = 0x820000;
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leader->gravity = 0x4000;
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leader->bounce = 0;
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(10);
  Engine_AudioPlayCue(SE_204);
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 3;
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leader->velocityY = -0x50000;
  OverlayObject_WaitUntilIdle(Actor_Get(ACTOR_PARTY_LEADER));
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_EventRequestExit(exit);
  Engine_EventEnd();
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_BabiLighthouse3) {
    return gBabiLighthouse_Entrances3;
  }
  if (scene == (s32)&SceneId_BabiLighthouse2) {
    return gBabiLighthouse_Entrances2;
  }
  if (scene == (s32)&SceneId_BabiLighthouse1) {
    return gBabiLighthouse_Entrances1;
  }
  return gBabiLighthouse_EntrancesOther;
}

/// Only room 3 has regions.
const struct SceneRegion *Scene_GetRegions(void) {
  if (gGameState.scene == (s32)&SceneId_BabiLighthouse3) {
    return gBabiLighthouse_Regions3;
  }
  return 0;
}

/// The four scenes share one exit table.
u8 *BabiLighthouse_GetExits(void) { return gBabiLighthouse_Exits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_BabiLighthouse3) {
    return gBabiLighthouse_Placements3;
  }
  if (scene == (s32)&SceneId_BabiLighthouse2) {
    return gBabiLighthouse_Placements2;
  }
  if (scene == (s32)&SceneId_BabiLighthouse1) {
    return gBabiLighthouse_Placements1;
  }
  return gBabiLighthouse_PlacementsOther;
}

/// After Move on one of room 3's blocks, or a push: a leader left standing on
/// a block's ledge cell jumps off it, and the blocks' cells are redrawn.
static void BabiLighthouse_JumpFromLedge(void) {
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  if (((leader->x.fixed >> 20) == 6 || (leader->x.fixed >> 20) == 18) &&
      (leader->z.fixed >> 20) == 20) {
    leader->targetX = ACTOR_NO_TARGET;
    leader->targetZ = ACTOR_NO_TARGET;
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
    {
      u16 facing = leader->facing;
      if ((u16)(facing + 0x4fff) <= 0x1fff ||
          (u16)(facing - 0x3001) <= 0x1fff) {
        Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 16, 0);
        Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
      } else {
        FieldObjectMotion_OffsetPositionAndResetMotion(0, 0, -16);
        Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
      }
    }
  }
  BabiLighthouse_RefreshMovableBlockTiles();
  Engine_EventEnd();
}

/// Walking into one of room 3's blocks pushes it.
static void BabiLighthouse_AdvancePairAndJump(void) {
  Engine_EventBegin();
  StagedActor_AdvancePair();
  BabiLighthouse_JumpFromLedge();
  Engine_EventEnd();
}

/// Walking south off room 3's ledge: pushes the block below on the leader's
/// side if it stands at the ledge's foot, and otherwise climbs down.
static void BabiLighthouse_ClimbDownOrAdvancePair(void) {
  struct FieldActor *block = Object_GetById(ACTOR_RIGHT_BLOCK);

  if ((Object_GetById(ACTOR_PARTY_LEADER)->x.fixed >> 20) <= 12) {
    block = Object_GetById(ACTOR_LEFT_BLOCK);
    if ((block->x.fixed >> 20) == 6) {
      if ((block->z.fixed >> 20) == 20) {
        BabiLighthouse_AdvancePairAndJump();
        return;
      }
    }
  } else {
    if ((block->x.fixed >> 20) == 18) {
      if ((block->z.fixed >> 20) == 20) {
        BabiLighthouse_AdvancePairAndJump();
        return;
      }
    }
  }
  Field_ClimbDown();
}

/// Raises the leader, then closes the screen before a room transition.
static void BabiLighthouse_RiseAndCloseScreen(void) {
  Camera_MoveTo(-1, -1, -1, 0);
  FieldBattleFx_RunRisingObjectSequence(ACTOR_PARTY_LEADER, 6, 0);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
}

/// Raises the leader and takes exit 11 without preparing an extra effect.
static void BabiLighthouse_RiseToExit(void) {
  Engine_EventBegin();
  BabiLighthouse_RiseAndCloseScreen();
  Engine_EventRequestExit(11);
  Engine_EventEnd();
}

/// Prepares one effect, raises the leader, then takes exit 12.
static void BabiLighthouse_RisePastSingleEffect(void) {
  Engine_EventBegin();
  OverlayObject_PrepareObject(PIXELS(464), 0, PIXELS(290), 223);
  BabiLighthouse_RiseAndCloseScreen();
  Engine_EventRequestExit(12);
  Engine_EventEnd();
}

/// Prepares two effects, raises the leader, then takes exit 13.
static void BabiLighthouse_RisePastPairedEffects(void) {
  Engine_EventBegin();
  OverlayObject_PrepareObject(PIXELS(143), 0, PIXELS(290), 223);
  OverlayObject_PrepareObject(PIXELS(121), 0, PIXELS(286), 253);
  BabiLighthouse_RiseAndCloseScreen();
  Engine_EventRequestExit(13);
  Engine_EventEnd();
}

/// Raises the leader and takes exit 15 without preparing an extra effect.
static void BabiLighthouse_RiseToAlternateExit(void) {
  Engine_EventBegin();
  BabiLighthouse_RiseAndCloseScreen();
  Engine_EventRequestExit(15);
  Engine_EventEnd();
}

/// Searching room 2's door. "It seems to be locked."
static void BabiLighthouse_InspectLockedDoor(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_MessageShowCentered(MsgBabiSeemsLocked, 1);
  Engine_EventEnd();
}

/// A companion walks back to the leader and rejoins the party.
static inline void BabiLighthouse_RegroupPartyMember(s32 actorId) {
  struct FieldActor *leader;

  Engine_ActorSetAnimation(actorId, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(actorId, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(actorId);
  Actor_SetPosition(actorId, 0, 0);
}

/// Walking up to room 1's stone door the first time: Faran asks whether the
/// kidnappers really went through it; the onlookers argue, and Iodem,
/// arriving with the party, tells who they are, and Faran, doubting them,
/// asks them to prove they can follow by opening the door.
static void BabiLighthouse_DiscussPursuit(void) {
  // Bring the party to the witnesses gathered around the sealed door.
  GameFlag_Set(FLAG_BABI_LIGHTHOUSE_PURSUIT_ASKED);
  GameFlag_Set(FLAG_IODEM_AT_BABI_LIGHTHOUSE);
  Engine_EventBegin();
  Battle_ResetEffectCounter();
  Engine_EventSetMessage(MsgBabiYoureSureTheyWentThrough);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 296, 352);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_EventWait(10);
  FieldMotion_LaunchFromFocusedObject(ACTOR_IODEM, 16, 0, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(GARET, -8, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(IVAN, 8, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(MIA, 24, 16, FACING_NORTH);
  Engine_ActorWaitForMove(MIA);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Camera_SetSpeed(0x30000, 0x6000);
  Camera_MoveTo(PIXELS(280), -1, PIXELS(288), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_WITNESS, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_WITNESS, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_FARAN, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  // The witnesses try the stone door and disagree about who passed through it.
  Engine_ActorStartRepeatedMotion(ACTOR_SECOND_ONLOOKER, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_ONLOOKER, 2);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(ACTOR_SECOND_ONLOOKER, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_ONLOOKER, 2);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(ACTOR_SECOND_ONLOOKER, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_ONLOOKER, 2);
  Engine_EventWait(40);
  Actor_ShowEmote(ACTOR_FIRST_ONLOOKER, EMOTE_IN_FRONT | 2, 50);
  Event_ShowMessage(ACTOR_FIRST_ONLOOKER, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_SECOND_ONLOOKER, FACING_EAST, 0);
  Engine_EventWait(25);
  Engine_ActorRunRepeatedMotion(ACTOR_SECOND_ONLOOKER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_SECOND_ONLOOKER, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FIRST_ONLOOKER, FACING_WEST, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_ONLOOKER, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_SECOND_ONLOOKER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_WITNESS, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_WITNESS, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_SECOND_ONLOOKER, EMOTE_IN_FRONT | 7, 40);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_SECOND_ONLOOKER, FACING_SOUTH, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_SECOND_ONLOOKER, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_SECOND_ONLOOKER, EMOTE_IN_FRONT | 1, 75);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | 1, 60);
  Actor_FaceDirection(ACTOR_FIRST_ONLOOKER, FACING_SOUTH, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_SOUTH, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_FARAN, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Camera_MoveTo(PIXELS(280), -1, PIXELS(320), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  // The party learns where the kidnappers went and is asked to pursue them.
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | 5, 60);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_IODEM, 0);
    gEventWork->message++;
  } else {
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    gEventWork->message++;
    Event_ShowMessage(ACTOR_IODEM, 0);
  }
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | 1, 60);
  Actor_SetSpeed(ACTOR_FARAN, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_FARAN, 0, 16);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
  Engine_EventWait(35);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_FARAN, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT, 40);
  Event_OpenMessage(ACTOR_FARAN, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_FARAN, 0);
    gEventWork->message++;
  } else {
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    gEventWork->message++;
    Event_ShowMessage(ACTOR_FARAN, 0);
  }
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 2, 50);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_SECOND_ONLOOKER, 2);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_SECOND_ONLOOKER, 0x14ccc, 0xa666);
  Actor_WalkByAndWait(ACTOR_SECOND_ONLOOKER, 0, 16);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_SECOND_ONLOOKER, 0);
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_FIRST_ONLOOKER, 0x14ccc, 0xa666);
  Actor_WalkByAndWait(ACTOR_FIRST_ONLOOKER, 0, 16);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_FIRST_ONLOOKER, EMOTE_IN_FRONT | 7, 50);
  Event_ShowMessage(ACTOR_FIRST_ONLOOKER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 2, 60);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Engine_EventWait(25);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
  Engine_EventWait(20);
  Event_OpenMessage(ACTOR_IODEM, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_IODEM, 0);
    gEventWork->message++;
  } else {
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    gEventWork->message++;
    Event_ShowMessage(ACTOR_IODEM, 0);
  }
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
  Engine_EventWait(35);
  Engine_ActorSetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_FARAN, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(40);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(ACTOR_FIRST_ONLOOKER, ACTOR_SECOND_ONLOOKER, 50);
  Engine_ActorSetAnimation(ACTOR_FIRST_ONLOOKER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_ONLOOKER, ANIM_NOD);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_FIRST_ONLOOKER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SECOND_ONLOOKER, FACING_SOUTH, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_FIRST_ONLOOKER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_ONLOOKER, ANIM_NOD);
  Engine_EventWait(30);
  Actor_SetSpeed(ACTOR_FIRST_ONLOOKER, 0x10000, 0x8000);
  Actor_SetSpeed(ACTOR_SECOND_ONLOOKER, 0x10000, 0x8000);
  Actor_WalkBy(ACTOR_FIRST_ONLOOKER, 32, 0);
  Actor_WalkByAndWait(ACTOR_SECOND_ONLOOKER, 32, 0);
  Actor_WalkBy(ACTOR_FIRST_ONLOOKER, 0, 16);
  Actor_WalkByAndWait(ACTOR_SECOND_ONLOOKER, 16, 0);
  Actor_WalkTo(ACTOR_SECOND_ONLOOKER, 344, 312);
  Actor_WalkToAndWait(ACTOR_FIRST_ONLOOKER, 344, 336);
  Engine_ActorSetAnimation(ACTOR_SECOND_ONLOOKER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_FIRST_ONLOOKER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SECOND_ONLOOKER, FACING_WEST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_SOUTH, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_FARAN, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_FARAN, 328, 312);
  Actor_FaceDirection(ACTOR_FARAN, FACING_WEST, 0);
  Engine_EventWait(30);
  Actor_SetSpeed(ACTOR_WITNESS, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_WITNESS, 328, 328);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_WEST, 0);
  Engine_EventWait(20);
  // Regroup the travelling party after the conversation.
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Actor_SetSpeed(IVAN, 0x13333, 0x9999);
  Actor_SetSpeed(MIA, 0x13333, 0x9999);
  BabiLighthouse_RegroupPartyMember(GARET);
  BabiLighthouse_RegroupPartyMember(IVAN);
  BabiLighthouse_RegroupPartyMember(MIA);
  Engine_EventWait(10);
  Engine_EventEnd();
}

/// The door opens before Iodem and Faran the first time: everyone marvels,
/// the door closes again, and the party says those without Psynergy cannot
/// follow; Iodem and Faran leave Sheba's rescue to them.
static void BabiLighthouse_ExplainTruthDoor(void) {
  // Everyone reacts to the opened door before trying to follow the party.
  GameFlag_Set(FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN);
  Engine_EventBegin();
  Battle_ResetEffectCounter();
  Engine_EventSetMessage(MsgBabiTheDoor);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 296, 312);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Engine_EventWait(10);
  Motion_LaunchFromFocusedObject(GARET, 0, 16, FACING_EAST);
  FieldMotion_LaunchFromFocusedObject(IVAN, -16, -8, FACING_EAST);
  Motion_LaunchFromFocusedObject(MIA, -16, 24, FACING_EAST);
  Engine_ActorWaitForMove(MIA);
  Engine_EventWait(20);
  Camera_SetSpeed(0x30000, 0x6000);
  Camera_MoveTo(PIXELS(280), -1, PIXELS(328), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(10);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_ActorJump(ACTOR_IODEM, 4, 13);
  Engine_ActorJump(ACTOR_IODEM, 4, 30);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_WITNESS, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(ACTOR_FIRST_ONLOOKER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(ACTOR_SECOND_ONLOOKER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT, 40);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FIRST_ONLOOKER, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_ONLOOKER, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_FARAN, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_SECOND_ONLOOKER, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_SECOND_ONLOOKER, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_FIRST_ONLOOKER, EMOTE_IN_FRONT | 1, 50);
  Actor_FaceDirection(ACTOR_FIRST_ONLOOKER, FACING_WEST, 0);
  Engine_EventWait(25);
  Event_ShowMessage(ACTOR_FIRST_ONLOOKER, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SECOND_ONLOOKER, FACING_WEST, 0);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_FARAN, 2);
  Engine_EventWait(25);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_IODEM, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, -40);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_SECOND_ONLOOKER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_FIRST_ONLOOKER, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimation(ACTOR_FIRST_ONLOOKER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_ONLOOKER, ANIM_NOD);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_SECOND_ONLOOKER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FIRST_ONLOOKER, FACING_WEST, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Engine_EventWait(30);
  Engine_CameraFollowActor(ACTOR_IODEM, 1);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, -32);
  // Demonstrate that the door closes again when someone without Psynergy
  // enters.
  BabiLighthouse_CloseTruthDoor();
  GameFlag_Clear(FLAG_BEYOND_DOOR);
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_EventBegin();
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FIRST_ONLOOKER, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_ONLOOKER, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_WITNESS, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(ACTOR_FIRST_ONLOOKER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(ACTOR_SECOND_ONLOOKER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT, 70);
  Camera_MoveTo(PIXELS(280), -1, PIXELS(312), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_FIRST_ONLOOKER, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_FIRST_ONLOOKER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_ONLOOKER, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_SECOND_ONLOOKER, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(30);
  Actor_FaceDirection(IVAN, FACING_SOUTHEAST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(20);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | 3, 50);
  Actor_FaceDirection(ACTOR_FARAN, FACING_WEST, 0);
  Engine_EventWait(20);
  // Explain the restriction and ask the onlookers to leave the pursuit to us.
  Event_OpenMessage(ACTOR_FARAN, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FIRST_ONLOOKER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SECOND_ONLOOKER, FACING_WEST, 0);
  Actor_FaceDirection(IVAN, FACING_EAST + FACING_STEP, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(ACTOR_FARAN, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_FARAN, 0);
    gEventWork->message++;
  } else {
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(ACTOR_FARAN, 2);
    Engine_EventWait(20);
    gEventWork->message++;
    Event_ShowMessage(ACTOR_FARAN, 0);
  }
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
  Engine_EventWait(20);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, 16);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_NORTHWEST, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  // Iodem and Faran agree to stay behind and leave the rescue to the party.
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_FARAN, 2);
  Engine_EventWait(20);
  Event_OpenMessage(ACTOR_FARAN, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 1, 60);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_IODEM, 0);
    gEventWork->message++;
  } else {
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    gEventWork->message++;
    Event_ShowMessage(ACTOR_IODEM, 0);
  }
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | 2, 50);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(15);
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_FARAN, 2);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_FARAN, FACING_WEST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(25);
  Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
  Engine_EventWait(20);
  Actor_WalkToAndWait(ACTOR_IODEM, 312, 312);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Engine_EventWait(25);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  // Regroup the travelling party after the conversation.
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Actor_SetSpeed(IVAN, 0x13333, 0x9999);
  Actor_SetSpeed(MIA, 0x13333, 0x9999);
  BabiLighthouse_RegroupPartyMember(GARET);
  BabiLighthouse_RegroupPartyMember(IVAN);
  BabiLighthouse_RegroupPartyMember(MIA);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FARAN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FIRST_ONLOOKER, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_ONLOOKER, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(30);
  Engine_EventEnd();
}

/// Reveal shows the door's switch, flipped one way or the other as the door
/// stands.
static void BabiLighthouse_RefreshTruthDoorTiles(void) {
  if (Engine_GameFlagIsSet(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) == 0) {
    Map_CopyCellsLayered(36, 78, 1, 2, 17, 78);
  } else {
    Map_CopyCellsLayered(34, 78, 1, 2, 17, 78);
  }
}

/// Reveal's light ends and the switch is hidden again.
static void BabiLighthouse_HideTruthDoorSwitch(void) {
  Map_CopyCellsLayered(32, 78, 1, 2, 17, 78);
}

/// The stone door's halves slide open (SE_GATE_MOVE); the first time, Iodem
/// and Faran see it.
static void BabiLighthouse_OpenTruthDoor(void) {
  if (GameFlag_IsSet(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) == 0) {
    GameFlag_Set(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN);
    Engine_AudioPlayCue(SE_GATE_MOVE);
    Engine_EventBegin();
    Actor_SetDestination(ACTOR_LEFT_DOOR, 280, 240);
    Actor_SetDestination(ACTOR_RIGHT_DOOR, 328, 240);
    Engine_ActorWaitForMove(ACTOR_LEFT_DOOR);
    Engine_ActorWaitForMove(ACTOR_RIGHT_DOOR);
    Map_CopyCellAttributes(81, 14, 4, 1, 17, 14);
    Engine_EventEnd();
    if (GameFlag_IsSet(FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN) == 0) {
      BabiLighthouse_ExplainTruthDoor();
    }
  }
}

/// Walking through the open door: it slides shut behind the party, now on its
/// other side.
static void BabiLighthouse_CloseTruthDoor(void) {
  if (GameFlag_IsSet(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) != 0) {
    GameFlag_Clear(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN);
    Engine_AudioPlayCue(SE_GATE_MOVE);
    Engine_EventBegin();
    Actor_SetDestination(ACTOR_LEFT_DOOR, 296, 240);
    Actor_SetDestination(ACTOR_RIGHT_DOOR, 312, 240);
    Engine_ActorWaitForMove(ACTOR_LEFT_DOOR);
    Engine_ActorWaitForMove(ACTOR_RIGHT_DOOR);
    Map_CopyCellAttributes(0, 14, 4, 1, 17, 14);
    Engine_EventEnd();
    if (GameFlag_IsSet(FLAG_BEYOND_DOOR) != 0) {
      GameFlag_Clear(FLAG_BEYOND_DOOR);
    } else {
      GameFlag_Set(FLAG_BEYOND_DOOR);
    }
  }
}

/// Searching the door's switch: under Reveal's light it is flipped and the
/// door opens; otherwise it reads an inscription. "The truth of this door is
/// open to those seeing with their true heart."
static void BabiLighthouse_InspectTruthDoorSwitch(void) {
  u8 *work = (u8 *)gEventWork;
  s16 *revealActive;

  Engine_EventBegin();
  revealActive = (s16 *)(work + 0xcb8);
  if (revealActive[0] != 0) {
    if (Engine_GameFlagIsSet(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) == 0) {

      Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
      Engine_AudioPlayCue(SE_155);
      Map_CopyCellsLayered(35, 78, 1, 2, 17, 78);
      Engine_EventWait(10);
      Map_CopyCellsLayered(34, 78, 1, 2, 17, 78);
      Engine_EventWait(10);
      BabiLighthouse_OpenTruthDoor();
    }
  } else {
    Engine_EventSetMessage(MsgBabiTruthDoorOpenThoseSeeing);
    Engine_EventShowMessage(-1, 0);
  }
  Engine_EventEnd();
}

/// Finds the staged actor one direction-table step ahead, including terrain
/// height.
static struct StagedActor *
BabiLighthouse_FindActorAhead(struct StagedActor *actor) {
  s32 position[3];
  s32 *target = position;
  s32 directionStep =
      gStagedActor_DirectionSteps[actor->directionAndKind >> 12];

  {
    s32 x = actor->x.value;
    s32 z = actor->z.value;

    x += -0x10000 & directionStep;
    z += directionStep << 16;
    target[0] = x;
    target[2] = z;
  }
  target[1] = Map_GetTerrainHeight(actor->transitionMode, target[0], target[2]);
  return StagedActor_FindAtTile(target, actor);
}

/// Carry's first phase on the building site's block sets the effect's
/// childOption; what that changes is not known.
static void BabiLighthouse_EnableChildOption(void) {
  ((struct BattleFxScene *)gEffectWork)->childOption = 1;
}

/// After Move or Carry on the building site's block: in its cell at (20, 12)
/// it settles at a height of 48 pixels.
static void BabiLighthouse_UpdateMovedBlock(void) {
  struct FieldActor *block = Object_GetById(ACTOR_SITE_BLOCK);
  s32 column = block->x.fixed >> 20;

  if (column == 20) {
    s32 row = block->z.fixed >> 20;

    if (row == 12) {
      block->motionFlags = 2;
      block->terrainHeight = 0x300000;
      block->priorityFlags = 2;
      {
        s32 targetColumn = column, targetRow = row;

        Map_CopyCellAttributes(38, 12, 1, 1, targetColumn, targetRow);
      }
    }
  }
}

/// Advances the staged pair only when the actor ahead is less than eight pixels
/// away vertically.
static void BabiLighthouse_AdvanceNearbyPair(void) {
  // Trial: inverting the empty success arm changes two branch instructions
  // (three text bytes in tbs-en); retain the existing branch order.
  struct StagedActor *leader =
      (struct StagedActor *)Object_GetById(ACTOR_PARTY_LEADER);
  struct StagedActor *ahead = BabiLighthouse_FindActorAhead(leader);
  s32 heightDifference;

  if (ahead == 0) {
    return;
  }

  heightDifference = ahead->y - leader->y;

  if (heightDifference >= 0) {

    if (heightDifference < 0x80000) {
    } else {
      return;
    }
  } else if (leader->y - ahead->y >= 0x80000) {
    return;
  }

  StagedActor_AdvancePair();
}

/// Tries vertical climbing first, then advances the staged pair when
/// appropriate.
static void BabiLighthouse_ClimbOrAdvancePair(void) {
  struct StagedActor *leader =
      (struct StagedActor *)Object_GetById(ACTOR_PARTY_LEADER);
  struct StagedActor *ahead = BabiLighthouse_FindActorAhead(leader);
  s32 climbDirection = (leader->directionAndKind + 0x2000) & 0xc000;
  s32 climbResult = -1;

  if (gGameState.movementMode == 1 || ahead == 0) {
    if (climbDirection == FACING_NORTH) {
      climbResult = Field_ClimbUp();
    }
    if (climbDirection == FACING_SOUTH) {
      climbResult = Field_ClimbDown();
    }
  }
  if (climbResult != 0) {
    if (gGameState.movementMode != 1) {
      BabiLighthouse_AdvanceNearbyPair();
    }
  }
}

const struct SceneEvent *Scene_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_BabiLighthouse3) {
    return gBabiLighthouse_Events3;
  }
  if (scene == (s32)&SceneId_BabiLighthouse2) {
    return gBabiLighthouse_Events2;
  }
  if (scene == (s32)&SceneId_BabiLighthouse1) {
    return gBabiLighthouse_Events1;
  }
  return gBabiLighthouse_EventsOther;
}

/// Growth on room 3's sprout: it grows into a vine the party can climb.
static void BabiLighthouse_OpenGrowthPassage(void) {

  Map_CopyCellAttributes(82, 7, 1, 2, 18, 7);
  Engine_TaskWait(1);
  Engine_GameFlagSet(FLAG_GROWTH_PASSAGE_OPEN);
}

/// Redraws room 3's ledge cells, then blocks the cells under its two blocks.
static void BabiLighthouse_RefreshMovableBlockTiles(void) {
  struct FieldActor *leftBlock = Object_GetById(ACTOR_LEFT_BLOCK);

  Engine_ActorSetSpritePriority(ACTOR_LEFT_BLOCK, 1);
  Engine_ActorSetSpritePriority(ACTOR_RIGHT_BLOCK, 1);
  Map_CopyCellAttributes(69, 19, 3, 3, 5, 19);
  Map_CopyCellAttributes(69, 19, 3, 3, 17, 19);
  {
    s32 column = leftBlock->x.fixed >> 20, row = leftBlock->z.fixed >> 20;

    Map_CopyCellAttributes(3, 3, 1, 1, column, row);
  }
  {
    struct FieldActor *rightBlock = Object_GetById(ACTOR_RIGHT_BLOCK);
    s32 column = rightBlock->x.fixed >> 20, row = rightBlock->z.fixed >> 20;

    Map_CopyCellAttributes(3, 3, 1, 1, column, row);
  }
}

#if TBS_PATCH_2
/// Update callback of room 3's blocks (TBS_PATCH_2 and later): each is drawn
/// in front of the leader when nearer the screen, and on its layer otherwise.
static void BabiLighthouse_MatchLeaderPriority(union FieldObject *fieldObject) {
  struct AnimationObject *sprite;
  struct FieldActor *actor;
  s32 priority;

  sprite = Object_GetById(ACTOR_PARTY_LEADER)->sprite;
  actor = Object_GetById(ACTOR_LEFT_BLOCK);
  if (actor->z.fixed < Object_GetById(ACTOR_PARTY_LEADER)->z.fixed) {
    Engine_ActorSetSpritePriority(ACTOR_LEFT_BLOCK, sprite->image.priority);
  } else {
    priority = sprite->image.priority;
    if (priority != 1)
      priority--;
    Engine_ActorSetSpritePriority(ACTOR_LEFT_BLOCK, priority);
    Object_GetById(ACTOR_LEFT_BLOCK)->priorityFlags |= 1;
  }
  actor = Object_GetById(ACTOR_RIGHT_BLOCK);
  if (actor->z.fixed < Object_GetById(ACTOR_PARTY_LEADER)->z.fixed) {
    Engine_ActorSetSpritePriority(ACTOR_RIGHT_BLOCK, sprite->image.priority);
  } else {
    priority = sprite->image.priority;
    if (priority != 1)
      priority--;
    Engine_ActorSetSpritePriority(ACTOR_RIGHT_BLOCK, priority);
    Object_GetById(ACTOR_RIGHT_BLOCK)->priorityFlags |= 1;
  }
}
#endif

/// A field actor seen by the scene setup, for the moved block and the fallen
/// men: position, sprite, collision and a staggered delay.
struct RowActor {
  u8 unk00[8];
  union FieldCoordinate x;
  union FieldCoordinate y;
  union FieldCoordinate z;
  u8 unk14[0x0f];
  u8 priorityFlags;
  u8 unk24[0x2c];
  struct AnimationObject *sprite;
  u8 unk54[5];
  u8 collisionFlags;
  u8 unk5a[4];
  u16 delay;
};

/// Sets up the scene being entered: Retreat leads back to room 3; the site's
/// block, room 3's blocks, vine and fallen men, room 2's arrivals by falling,
/// and room 1's door and onlookers are restored. Returns 0.
s32 BabiLighthouse_SetupScene(void) {
  struct FieldActor *actor;
  struct RowActor *row;
  s32 scene;
  s32 sceneCopy;
  struct GameState *state;
  s32 x;
  s32 z;
  u8 zero;

  Engine_TaskWait(1);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  state = &gGameState;
  scene = state->scene;
  if (scene != (s32)&SceneId_BabiLighthouse4) {
    state->retreatEntrance = 1;
    state->retreatScene = (s32)&SceneId_BabiLighthouse3;
    sceneCopy = state->scene;
  }
  // Restore the moved block's collision in the construction-area room.
  if (scene == (s32)&SceneId_BabiLighthouse4) {
    row = (struct RowActor *)Object_GetById(ACTOR_SITE_BLOCK);
    x = row->x.fixed >> 20;
    if (x == 20) {
      z = row->z.fixed >> 20;
      if (z == 12) {
        Map_CopyCellAttributes(38, 12, 1, 1, x, z);
      }
    }
  } else if (sceneCopy == (s32)&SceneId_BabiLighthouse3) {
    // Set up the movable ledge blocks and the staggered actor animations.
    Actor_SetChildValue(ACTOR_LEFT_BLOCK, 6);
    Actor_SetChildValue(ACTOR_RIGHT_BLOCK, 6);
#if TBS_PATCH_2
    ((union FieldObject *)Object_GetById(ACTOR_LEFT_BLOCK))->actor.update =
        BabiLighthouse_MatchLeaderPriority;
    ((union FieldObject *)Object_GetById(ACTOR_RIGHT_BLOCK))->actor.update =
        BabiLighthouse_MatchLeaderPriority;
#endif
    if (state->entrance == 5 && GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Actor_SetPosition(ACTOR_RIGHT_BLOCK, PIXELS(312), PIXELS(328));
    }
    BabiLighthouse_RefreshMovableBlockTiles();
    actor = Object_GetById(ACTOR_RIGHT_BLOCK);
    actor->y.fixed = Object_GetById(ACTOR_RIGHT_BLOCK)->terrainHeight;
    if (GameFlag_IsSet(FLAG_GROWTH_PASSAGE_OPEN) != 0) {
      Engine_ActorSetAnimation(ACTOR_SPROUT, 4);
      Object_GetById(ACTOR_SPROUT)->collisionFlags = 0xfe;
      BabiLighthouse_OpenGrowthPassage();
    }
    row = (struct RowActor *)Object_GetById(ACTOR_FIRST_FALLEN_MAN);
    row->collisionFlags = 0;
    row->priorityFlags = 0;
    row->delay = 0;
    ((u8 *)row->sprite)[9] |= 12;
    zero = 0;
    ((u8 *)row->sprite)[38] = zero;
    row->sprite->rotation = 0xc000;
    Engine_ActorSetAnimation(ACTOR_FIRST_FALLEN_MAN, 0);
    row = (struct RowActor *)Object_GetById(ACTOR_FIRST_FALLEN_MAN + 1);
    row->collisionFlags = zero;
    row->priorityFlags = zero;
    row->delay = 30;
    ((u8 *)row->sprite)[9] |= 12;
    ((u8 *)row->sprite)[38] = zero;
    row->sprite->rotation = 0x4000;
    Engine_ActorSetAnimation(ACTOR_FIRST_FALLEN_MAN + 1, 0);
    row = (struct RowActor *)Object_GetById(ACTOR_FIRST_FALLEN_MAN + 2);
    row->collisionFlags = zero;
    row->priorityFlags = zero;
    row->delay = 60;
    ((u8 *)row->sprite)[9] |= 12;
    ((u8 *)row->sprite)[38] = zero;
    row->sprite->rotation = 0x8000;
    Engine_ActorSetAnimation(ACTOR_FIRST_FALLEN_MAN + 2, 0);
    row = (struct RowActor *)Object_GetById(ACTOR_FIRST_FALLEN_MAN + 3);
    row->collisionFlags = zero;
    row->priorityFlags = zero;
    row->delay = 90;
    ((u8 *)row->sprite)[9] |= 12;
    ((u8 *)row->sprite)[38] = zero;
    row->sprite->rotation = 0x8000;
    Engine_ActorSetAnimation(ACTOR_FIRST_FALLEN_MAN + 3, 0);
  } else if (sceneCopy == (s32)&SceneId_BabiLighthouse2) {
    // Match the arrival effect and fall to the entrance used in this room.
    switch (state->entrance) {
    case 10:
      // Once the party has come in at entrance 10, flag 0x980 keeps three
      // cells there redrawn from (120, 7) and one cell's attributes changed.
      GameFlag_Set(0x980);
      // Fall through: both entrances restore the same remembered tile changes.
    case 11:
      if (GameFlag_IsSet(0x980) != 0) {
        Map_CopyCellsTo(120, 7, 109, 7, 1, 3);
        Map_CopyCellAttributes(45, 8, 1, 1, 45, 9);
      }
      break;
    case 14:
      OverlayObject_PrepareObject(PIXELS(440), 0, PIXELS(290), 223);
      Map_CopyCellAttributes(22, 13, 1, 1, 27, 13);
      BabiLighthouse_FallToExit(14);
      break;
    case 16:
      OverlayObject_PrepareObject(PIXELS(448), 0, PIXELS(290), 223);
      Map_CopyCellAttributes(22, 12, 1, 1, 28, 10);
      BabiLighthouse_FallToExit(16);
      break;
    case 17:
      OverlayObject_PrepareObject(PIXELS(232), 0, PIXELS(594), 223);
      Map_CopyCellAttributes(22, 12, 1, 1, 14, 33);
      BabiLighthouse_FallToExit(17);
      break;
    case 12:
    case 13:
    case 15:
      if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
        BabiLighthouse_LandWithParticles();
      }
      break;
    }
  } else if (scene == (s32)&SceneId_BabiLighthouse1) {
    // Restore the truth door and the onlookers' positions after each
    // discussion.
    Object_GetById(ACTOR_LEFT_DOOR)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Object_GetById(ACTOR_RIGHT_DOOR)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_SetSpeed(ACTOR_LEFT_DOOR, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_RIGHT_DOOR, 0x10000, 0x8000);
    if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      if (state->entrance == 1) {
        GameFlag_Set(FLAG_BEYOND_DOOR);
      } else {
        GameFlag_Clear(FLAG_BEYOND_DOOR);
      }
    }
    if (GameFlag_IsSet(FLAG_BABI_LIGHTHOUSE_PURSUIT_ASKED) == 0) {
      Actor_SetPosition(ACTOR_IODEM, -PIXELS(64), -PIXELS(64));
      Actor_SetPosition(ACTOR_WITNESS, PIXELS(280), PIXELS(296));
      Actor_SetPosition(ACTOR_FIRST_ONLOOKER, PIXELS(312), PIXELS(248));
      Actor_SetPosition(ACTOR_SECOND_ONLOOKER, PIXELS(296), PIXELS(248));
      Actor_SetPosition(ACTOR_FARAN, PIXELS(320), PIXELS(296));
      Actor_FaceDirection(ACTOR_WITNESS, FACING_EAST, 0);
      Actor_FaceDirection(ACTOR_FIRST_ONLOOKER, FACING_NORTH, 0);
      Actor_FaceDirection(ACTOR_SECOND_ONLOOKER, FACING_NORTH, 0);
      Actor_FaceDirection(ACTOR_FARAN, FACING_WEST, 0);
      Engine_EventWait(5);
    } else if (GameFlag_IsSet(FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN) != 0) {
      Actor_SetPosition(ACTOR_IODEM, PIXELS(312), PIXELS(312));
      Actor_FaceDirection(ACTOR_IODEM, FACING_NORTHWEST + FACING_STEP, 0);
      Actor_FaceDirection(ACTOR_WITNESS, FACING_NORTHWEST + FACING_STEP, 0);
      Actor_FaceDirection(ACTOR_FIRST_ONLOOKER, FACING_NORTHWEST + FACING_STEP,
                          0);
      Actor_FaceDirection(ACTOR_SECOND_ONLOOKER, FACING_NORTHWEST + FACING_STEP,
                          0);
      Actor_FaceDirection(ACTOR_FARAN, FACING_NORTHWEST + FACING_STEP, 0);
      Engine_EventWait(5);
    }
    if (GameFlag_IsSet(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) != 0) {
      Actor_SetPosition(ACTOR_LEFT_DOOR, PIXELS(280), PIXELS(240));
      Actor_SetPosition(ACTOR_RIGHT_DOOR, PIXELS(328), PIXELS(240));
      Actor_FaceDirection(ACTOR_LEFT_DOOR, FACING_WEST, 0);
      Actor_FaceDirection(ACTOR_RIGHT_DOOR, FACING_EAST, 0);
      Map_CopyCellAttributes(81, 14, 4, 1, 17, 14);
    }
    if (gGameState.entrance == 3) {
      if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
        BabiLighthouse_LandWithParticles();
      }
    }
  } else {
    Engine_ActorSetAnimation(12, 2);
  }
  return 0;
}

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8,  32, 8,  -8,  -32, 8,  32, -32, -16, 32, 0,
    -8,  -32, 8,  32, -32, -8,  32, 8,  -8,  -32, 8,  32,
};

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

s32 gBabiLighthouse_TrembleScript[] = {
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(120),
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

struct SceneEntrance gBabiLighthouse_Entrances3[] = {
    {0, CONDITION_ALWAYS, 136, 0, 376, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 200, 0, 424, FACING_NORTH, 0, 0, 0, 384, 448, 0},
    {2, CONDITION_ALWAYS, 200, 0, 200, FACING_SOUTH, 0, 0, 0, 384, 448, 0},
    {3, CONDITION_ALWAYS, 216, 0, 184, FACING_SOUTH, 0, 0, 0, 384, 448, 0},
    {4, CONDITION_ALWAYS, 104, 0, 312, FACING_SOUTH, 0, 0, 0, 384, 448, 0},
    {5, CONDITION_ALWAYS, 296, 0, 312, FACING_SOUTH, 0, 0, 0, 384, 448, 0},
    {6, CONDITION_ALWAYS, 312, 0, 88, FACING_SOUTH, 0, 0, 0, 384, 448, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gBabiLighthouse_Entrances2[] = {
    {0, CONDITION_ALWAYS, 136, 0, 376, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 232, 0, 488, FACING_SOUTH, 0, 128, 432, 368, 672, 0},
    {2, CONDITION_ALWAYS, 280, 0, 488, FACING_SOUTH, 0, 128, 432, 368, 672, 0},
    {3, CONDITION_ALWAYS, 248, 0, 648, FACING_SOUTH, 0, 128, 432, 368, 672, 0},
    {4, CONDITION_ALWAYS, 424, 0, 120, FACING_SOUTH, 0, 352, 64, 592, 320, 0},
    {5, CONDITION_ALWAYS, 456, 0, 120, FACING_SOUTH, 0, 352, 64, 592, 320, 0},
    {6, CONDITION_ALWAYS, 504, 0, 120, FACING_SOUTH, 0, 352, 64, 592, 320, 0},
    {7, CONDITION_ALWAYS, 472, 0, 288, FACING_NORTH, 0, 352, 64, 592, 320, 0},
    {8, CONDITION_ALWAYS, 120, 0, 120, FACING_SOUTH, 0, 48, 64, 288, 320, 0},
    {9, CONDITION_ALWAYS, 184, 0, 288, FACING_NORTH, 0, 48, 64, 288, 320, 0},
    {10, CONDITION_ALWAYS, 728, 0, 168, FACING_SOUTH, 0, 624, 96, 864, 256, 0},
    {11, CONDITION_ALWAYS, 776, 0, 184, FACING_SOUTH, 0, 624, 96, 864, 256, 0},
    {12, CONDITION_ALWAYS, 488, 0, 184, FACING_SOUTH, 0, 352, 64, 592, 320, 0},
    {13, CONDITION_ALWAYS, 248, 0, 584, FACING_SOUTH, 0, 128, 432, 368, 672, 0},
    {14, CONDITION_ALWAYS, 440, 0, 216, FACING_SOUTH, 0, 352, 64, 592, 320, 0},
    {15, CONDITION_ALWAYS, 216, 0, 584, FACING_SOUTH, 0, 128, 432, 368, 672, 0},
    {16, CONDITION_ALWAYS, 456, 0, 168, FACING_SOUTH, 0, 352, 64, 592, 320, 0},
    {17, CONDITION_ALWAYS, 232, 0, 536, FACING_SOUTH, 0, 128, 432, 368, 672, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gBabiLighthouse_Entrances1[] = {
    {1, CONDITION_ALWAYS, 248, 0, 120, FACING_SOUTH, 0, 96, 48, 384, 432, 0},
    {2, CONDITION_ALWAYS, 136, 0, 368, FACING_NORTH, 0, 96, 48, 384, 432, 0},
    {3, CONDITION_ALWAYS, 168, 0, 344, FACING_SOUTH, 0, 96, 48, 384, 432, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gBabiLighthouse_EntrancesOther[] = {
    {0, CONDITION_ALWAYS, 328, 0, 240, FACING_NORTH, 0, 0, 0, 464, 400, 0},
    {1, CONDITION_ALWAYS, 112, 0, 360, FACING_NORTH, 0, 0, 0, 464, 400, 0},
    {2, CONDITION_ALWAYS, 136, 0, 32, FACING_SOUTH, 0, 0, 0, 464, 400, 0},
    {SCENE_TABLE_END},
};

s16 gBabiLighthouse_Regions3[] = {
    96,  -20, 320, 112, -4, 336, -1, 4, 288, -20, 320, 304, -4, 336, -1, 5,
    304, -20, 96,  320, -4, 112, -1, 6, -1,  0,   0,   0,   0,  0,   0,  0,
};

u32 gBabiLighthouse_Exits[] = {
    SCENE_EXITS(176),        SCENE_EXIT(1, 177, 2),   SCENE_EXIT(2, 175, 7),
    SCENE_EXIT(3, 175, 9),   SCENE_EXIT(4, 173, 1),   SCENE_EXIT(5, 173, 3),
    SCENE_EXIT(6, 173, 2),   SCENE_EXIT(0, 175, 0),   SCENE_EXIT(1, 175, 4),
    SCENE_EXIT(2, 176, 3),   SCENE_EXIT(3, 175, 8),   SCENE_EXIT(4, 175, 1),
    SCENE_EXIT(5, 175, 2),   SCENE_EXIT(6, 176, 2),   SCENE_EXIT(7, 175, 5),
    SCENE_EXIT(8, 175, 6),   SCENE_EXIT(9, 175, 11),  SCENE_EXIT(10, 175, 3),
    SCENE_EXIT(11, 175, 12), SCENE_EXIT(12, 175, 13), SCENE_EXIT(13, 175, 14),
    SCENE_EXIT(14, 175, 15), SCENE_EXIT(15, 175, 16), SCENE_EXIT(16, 175, 17),
    SCENE_EXIT(17, 174, 3),  SCENE_EXIT(18, 174, 2),  SCENE_EXIT(0, 174, 0),
    SCENE_EXIT(1, 172, 1),   SCENE_EXIT(2, 175, 10),  SCENE_EXIT(0, 177, 0),
    SCENE_EXIT(1, 2, 31),    SCENE_EXIT(2, 176, 1),   SCENE_EXITS_END,
};

struct ScenePlacement gBabiLighthouse_NoPlacements[] = {
    {SCENE_TABLE_END},
};

struct ScenePlacement gBabiLighthouse_Placements3[] = {
    {0xfd, CONDITION_ALWAYS, 7, PIXELS(88), 0, PIXELS(328), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0xfd, CONDITION_ALWAYS, 7, PIXELS(296), 0, PIXELS(328), FACING_SOUTH,
     TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {0xee, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(296), 0, PIXELS(130),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x98, CONDITION_ALWAYS, (s32)gBabiLighthouse_TrembleScript, PIXELS(176), 0,
     PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0x74, CONDITION_ALWAYS, (s32)gBabiLighthouse_TrembleScript, PIXELS(240), 0,
     PIXELS(240), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0x98, CONDITION_ALWAYS, (s32)gBabiLighthouse_TrembleScript, PIXELS(216), 0,
     PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0x72, CONDITION_ALWAYS, (s32)gBabiLighthouse_TrembleScript, PIXELS(184), 0,
     PIXELS(248), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gBabiLighthouse_Placements2[] = {
    {0x5d, FLAG_DJINNI_0 + 66, ACTOR_STAND, PIXELS(472), 0, PIXELS(184),
     FACING_SOUTHEAST, TALK_FACE_PARTY, 0},
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gBabiLighthouse_Placements1[] = {
    {GARET, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(-64), 0, PIXELS(-64), FACING_EAST,
        TALK_FACE_PARTY, 0},
    {IVAN, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_FACE_PARTY,
        0},
    {MIA, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(-64), 0, PIXELS(-64), FACING_EAST,
        TALK_FACE_PARTY, 0},
    {0x128, CONDITION_ALWAYS, 7, PIXELS(296), 0, PIXELS(240), FACING_WEST, TALK_KEEP_FACING, 0},
    {0x128, CONDITION_ALWAYS, 7, PIXELS(312), 0, PIXELS(240), FACING_EAST, TALK_KEEP_FACING, 0},
    {0x39, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(312), 0, PIXELS(352), FACING_NORTH,
        TALK_FACE_PARTY_AND_BACK, 0},
    {0x74, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(328), 0, PIXELS(328), FACING_WEST,
        TALK_FACE_PARTY_AND_BACK, 0},
    {0x65, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(344), 0, PIXELS(336), FACING_WEST,
        TALK_FACE_PARTY_AND_BACK, 0},
    {0x6c, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(344), 0, PIXELS(312), FACING_WEST,
        TALK_FACE_PARTY_AND_BACK, 0},
    {0x46, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(328), 0, PIXELS(312), FACING_WEST,
        TALK_FACE_PARTY_AND_BACK, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gBabiLighthouse_PlacementsOther[] = {
    {0x72, CONDITION_ALWAYS, ACTOR_WANDER, PIXELS(128), 0, PIXELS(264), FACING_WEST,
        TALK_FACE_PARTY, 0},
    {0x74, CONDITION_ALWAYS, ACTOR_WANDER, PIXELS(96), 0, PIXELS(184), FACING_EAST, TALK_FACE_PARTY,
        0},
    {0x87, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(184), 0, PIXELS(200), FACING_WEST, TALK_FACE_PARTY,
        0},
    {0x87, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(232), 0, PIXELS(184), FACING_SOUTH,
        TALK_FACE_PARTY, 0},
    {0x100, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(376), 0, PIXELS(216), FACING_SOUTH,
        TALK_FACE_PARTY, 0},
    {SCENE_TABLE_END},
};

struct SceneEvent gBabiLighthouse_Events3[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_AdvancePairAndJump)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_ClimbDownOrAdvancePair)},
    {EVENT_TALK, ACTOR_FIRST_FALLEN_MAN, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseTheyWereTerribleWeWereAbsolutely)},
    {EVENT_TALK, ACTOR_FIRST_FALLEN_MAN + 1, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseOnlyTwoOfThoseSixEver)},
    {EVENT_TALK, ACTOR_FIRST_FALLEN_MAN + 2, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseTheyJustKickedUsOutOf)},
    {EVENT_TALK, ACTOR_FIRST_FALLEN_MAN + 3, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseTheySetFireToEverythingThats)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_FALLEN_MAN, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseWereTheOnesWhoAttackedBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_FALLEN_MAN + 1, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseTheOtherFourDidntSeemTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_FALLEN_MAN + 2, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseWhatCouldTheyBePlanningFor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_FALLEN_MAN + 3, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseNoneOfThemHadAnyTorches)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEFT_BLOCK, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_JumpFromLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RIGHT_BLOCK, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_JumpFromLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_GROWTH_PASSAGE_OPEN, EVENT_SCRIPT(BabiLighthouse_OpenGrowthPassage)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gBabiLighthouse_Events2[] = {
    {0x31, 5, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 6, CONDITION_ALWAYS, 2},
    {0x21, 1, CONDITION_ALWAYS, 3},
    {0x31, 2, CONDITION_ALWAYS, 4},
    {0x31, 3, CONDITION_ALWAYS, 5},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 6},
    {0x21, 7, CONDITION_ALWAYS, 7},
    {0x21, 8, CONDITION_ALWAYS, 8},
    {0x31, 9, CONDITION_ALWAYS, 9},
    {0x21, 10, CONDITION_ALWAYS, 10},
    {EVENT_EXIT, 11, CONDITION_ALWAYS, 18},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_RiseToExit)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_RisePastSingleEffect)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 27, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_RisePastPairedEffects)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 28, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_RiseToAlternateExit)},
    {EVENT_TALK, ACTOR_DJINNI, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_StartDjinnBattle)},
    {SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_InspectLockedDoor)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gBabiLighthouse_Events1[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_TOUCH, 10, FLAG_BABI_LIGHTHOUSE_PURSUIT_ASKED, EVENT_SCRIPT(BabiLighthouse_DiscussPursuit)},
    {EVENT_TOUCH, 20, IF_FLAG_SET(FLAG_BEYOND_DOOR), EVENT_SCRIPT(BabiLighthouse_CloseTruthDoor)},
    {EVENT_TOUCH, 21, FLAG_BEYOND_DOOR, EVENT_SCRIPT(BabiLighthouse_CloseTruthDoor)},
    {EVENT_TOUCH, 22, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_OpenTruthDoor)},
    {EVENT_TALK, ACTOR_IODEM, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseIAmCountingOnYouIsaac)},
    {EVENT_TALK, ACTOR_WITNESS, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseWhenISawThemTheyHad)},
    {EVENT_TALK, ACTOR_FIRST_ONLOOKER, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseIAmSorryButNotEven)},
    {EVENT_TALK, ACTOR_SECOND_ONLOOKER, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseThisAllStartedWhenActorWas)},
    {EVENT_TALK, ACTOR_FARAN, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseIfYouCanOpenThisDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_IODEM, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseIDoNotMeanToDoubt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WITNESS, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN,
        MSGNO(MsgBabiLighthouseNobodyThinksTheDoorCanBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_ONLOOKER, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN,
        MSGNO(MsgBabiLighthouseIDontKnowWhatTheyreWaiting)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_ONLOOKER, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseThisIsAllAHugeWaste)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARAN, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseIHopeTheyCanOpenThe)},
    {EVENT_TALK, ACTOR_IODEM, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseIBegOfYouIsaacPlease)},
    {EVENT_TALK, ACTOR_WITNESS, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseICantBelieveThisDoorActually)},
    {EVENT_TALK, ACTOR_FIRST_ONLOOKER, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseItTakesGreatCourageToContinue)},
    {EVENT_TALK, ACTOR_SECOND_ONLOOKER, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseImVeryInterestedInWhatLies)},
    {EVENT_TALK, ACTOR_FARAN, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseIAmNotAfraidOfDanger)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_IODEM, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseIfAnythingHappensToActorTolbi)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WITNESS, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseIfThisDoorOpensMaybeWe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_ONLOOKER, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseIBetTheMonstersAroundHere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_ONLOOKER, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseThisIsABigWasteOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARAN, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseIDontWantToHinderActors)},
    {SEARCH(SEARCH_UNNAMED), 60, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_InspectTruthDoorSwitch)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(BabiLighthouse_RefreshTruthDoorTiles)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(BabiLighthouse_HideTruthDoorSwitch)},
    {SCENE_EVENTS_END},
};
// clang-format on

struct SceneEvent gBabiLighthouse_EventsOther[] = {
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 50, CONDITION_ALWAYS,
     EVENT_SCRIPT(BabiLighthouse_AdvanceNearbyPair)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 51, CONDITION_ALWAYS,
     EVENT_SCRIPT(BabiLighthouse_ClimbOrAdvancePair)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SITE_BLOCK, CONDITION_ALWAYS,
     EVENT_SCRIPT(BabiLighthouse_UpdateMovedBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, 1), ACTOR_SITE_BLOCK,
     CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_EnableChildOption)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_SITE_BLOCK, CONDITION_ALWAYS,
     EVENT_SCRIPT(BabiLighthouse_UpdateMovedBlock)},
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_TALK, 8, CONDITION_ALWAYS,
     MSGNO(MsgBabiLighthouseWeHaventBeenOrderedToStart)},
    {EVENT_TALK, 9, CONDITION_ALWAYS,
     MSGNO(MsgBabiLighthouseThereWereStrangeFolkGoingTo)},
    {EVENT_TALK, 10, CONDITION_ALWAYS,
     MSGNO(MsgBabiLighthouseItsOddThatBabiWouldBuild)},
    {EVENT_TALK, 11, CONDITION_ALWAYS,
     MSGNO(MsgBabiLighthouseCuttingTheseStonesAndStackingThem)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgBabiLighthouseThereWasABigFightThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgBabiLighthouseIfFaranWentAfterThemThen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgBabiLighthouseWhyWouldTheyWantToRush)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgBabiLighthouseNowThatIThinkOfIt)},
    {SEARCH_CHEST_FACING(FACING_EAST), 100, FLAG_BABI_LIGHTHOUSE_LUCKY_PEPPER,
     FIND_ITEM(ITEM_LUCKY_PEPPER)},
    {SCENE_EVENTS_END},
};
