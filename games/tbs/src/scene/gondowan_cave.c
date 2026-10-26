/// @file
/// Gondowan Passage and Gondowan Cave, the guarded bridge into Gondowan and the
/// cave east of it, which the party crosses on the way from Tolbi to Babi
/// Lighthouse. Three maps share this overlay:
///
/// - Gondowan Passage (SceneId_GondowanPassage, area name Gondowan Passage):
///   three soldiers guard the bridge, and Iodem waits there to go on to Babi
///   Lighthouse with the party once Babi has spoken to it after Colosso
///   (FLAG_BABI_SPOKE_AFTER_COLOSSO).
/// - Gondowan Cave (SceneId_GondowanCave, area name Gondowan Cave): both ways
///   in from the world map, a tree stump that Move sends leaping into the
///   water, a stone pillar to push or move in after it, and a boulder for
///   Lift.
/// - GondowanCaveRiver (SceneId_GondowanCaveRiver): the river between the
///   cave's two halves, with a boulder for Lift and two chests.
///
/// One exit table serves every map; the other getters choose by
/// gSaveBlock.sceneId, Gondowan Passage's actor templates and triggers
/// also by FLAG_BABI_SPOKE_AFTER_COLOSSO, and an empty set answers any other
/// map number. Every map's trigger table carries GondowanCaveRiver's two
/// chests.
///
/// Gondowan Passage sets FLAG_RETREAT_BLOCKED on every entry. While Iodem
/// travels with the party (FLAG_IODEM_FOLLOWING_PARTY) his actor is kept
/// out of sight; talking to him or walking up to him while he waits (
/// step trigger 11) makes him come along (EventScript_IodemRejoins), and until
/// he has reached Babi Lighthouse (FLAG_BABI_LIGHTHOUSE_IODEM_ARRIVED) turning
/// back (step trigger 10) brings him out to ask why
/// (EventScript_IodemAsksWhyTurningBack). If he stays to wait, the saved place
/// becomes the Tolbi Sanctum, so a defeat returns the party there.
///
/// GondowanCave's way across the water is two things that float in it, and
/// step triggers 10 and 11 jump the leader across them
/// (EventScript_JumpAcross). Both are kept in story flags, so the cave stays
/// solved and Scene_Initialize puts them back: FLAG_GONDOWAN_CAVE_STUMP_MOVED
/// once the stump has leapt in, and FLAG_GONDOWAN_CAVE_PILLAR_DROPPED once the
/// pillar has gone in past column 42, shown from then on by
/// ACTOR_DROPPED_PILLAR.
///
/// Cell (107, 17) of the map grid is cell (43, 17) of layout layer 2,
/// whose window starts 64 columns in: the crevice east of the pillar,
/// where it drops. The layout loads that cell with ground 6, as the two cells
/// west of it; copying (108, 17) over it gives it ground 0, as its east
/// neighbour. The drop makes that copy, and so does every arrival by
/// GONDOWAN_CAVE_FROM_WORLD_MAP_EAST, solved or not. Nothing the player sees
/// depends on it: in tbs-en the screen is the same either way, the leader
/// jumps the crevice at row 17 (step triggers 10 and 11) from either side
/// either way, and the pillar settles at the same height in each of columns
/// 41 to 44 either way.

#include "save_block.h"
#include "heap_slots.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/gondowan_cave.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "pushed_object.h"
#include "field_effect.h"
#include "text/msg_ids.h"
#include "actor.h"
#include "fixed_math.h"
#include "flags.h"
#include "songs.h"

/// Gondowan Passage's placed actors, numbered in the order of
/// gGondowanPassageActorTemplates and
/// gGondowanPassageActorTemplatesWithIodem from ACTOR_FIRST_PLACED: the
/// soldiers who guard the bridge, and in the second table Iodem after them.
enum GondowanPassageActorId {
  /// Asks whether the party will cross into Gondowan.
  /// ★ Named from the game: "Us? We're the soldiers who guard the bridge to
  /// Gondowan." (MsgGondowanCaveUsWereTheSoldiersWhoGuard)
  ACTOR_FIRST_SOLDIER = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "Us? We're the soldiers who guard the bridge to
  /// Gondowan." (MsgGondowanCaveUsWereTheSoldiersWhoGuard)
  ACTOR_SECOND_SOLDIER,
  /// Warns that the cave to the east leads nowhere.
  /// ★ Named from the game: "Us? We're the soldiers who guard the bridge to
  /// Gondowan." (MsgGondowanCaveUsWereTheSoldiersWhoGuard)
  ACTOR_THIRD_SOLDIER,
  /// Waits for the party once Babi has spoken after Colosso.
  /// ★ Named from the game: "Why would Iodem choose to wait for us"
  /// (MsgGondowanCaveWhyWouldIodemChooseToWait)
  ACTOR_IODEM
};

/// GondowanCaveRiver's placed actor, the one row of
/// gGondowanCaveRiverActorTemplates that a trigger names.
enum GondowanCaveRiverActorId {
  /// Raised by Lift.
  /// ★ Named from what the player sees: a round grey boulder by the river
  /// (its render)
  ACTOR_RIVER_BOULDER = ACTOR_FIRST_PLACED
};

/// GondowanCave's placed actors, numbered in the order of
/// gGondowanCaveActorTemplates from ACTOR_FIRST_PLACED.
enum GondowanCaveActorId {
  /// Moved with Move, in the sprite of the Goma Cave stump.
  /// ★ Named from the game: "that stump" (MsgGomaNoUsePsynergy)
  ACTOR_STUMP = ACTOR_FIRST_PLACED,
  /// Pushed or moved with Move, in the sprite the stone pillars of other maps
  /// use (crossbone_isle_stepping_stones.c).
  /// ★ Named from what the player sees: a tall tan stone pillar standing on
  /// the cave floor (its render)
  ACTOR_PILLAR,
  /// Stands in for the pillar once it has dropped in; out of sight at first.
  /// It is drawn in the stump's sprite, splashing in (STUMP_ANIMATION_SPLASH)
  /// and
  /// on later visits afloat (STUMP_ANIMATION_SINK).
  /// ★ Named from what the player sees: the pillar's top in the water, after
  /// the splash it drops in with (its render)
  ACTOR_DROPPED_PILLAR,
  /// Raised by Lift.
  /// ★ Named from what the player sees: a round boulder in the cave passage,
  /// which floats up under Lift (its render)
  ACTOR_BOULDER,
  /// An invisible actor (SPRITE_BLANK, its shadow turned off) 16 pixels
  /// west
  /// of the boulder. It fills the gap between the boulder and the passage's
  /// west wall, so the boulder blocks the passage until Lift takes both away
  /// (https://alchemy.pascalpixel.com/places/gondowan-cave/map-4.png shows
  /// the gap).
  /// ★ Named from what the player sees: the open-looking gap between the
  /// boulder and the passage's west wall, map-4.png (its render)
  ACTOR_BOULDER_GAP
};

// The leap script also runs after the shared pushed object movement.
static void EventScript_MovePillar(void);
static void GondowanCave_RunSpinningLeap(s32 actorId);

// Shared effect routines retain their position before the map's scripts.
#include "common/effect_create.h"

// Particle and movable-object routines precede this map's entry points.
#include "common/effect_spawn.h"
#include "common/object_staged_actor.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gSaveBlock.sceneId;

  if (sceneId == (s32)&SceneId_GondowanPassage) {
    return gGondowanPassageEntrances;
  }
  if (sceneId == (s32)&SceneId_GondowanCaveRiver) {
    return gGondowanCaveRiverEntrances;
  }
  if (sceneId == (s32)&SceneId_GondowanCave) {
    return gGondowanCaveEntrances;
  }
  return gGondowanCaveEntrancesOther;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

/// Every map of the overlay shares one exit table.
const u32 *Scene_GetExits(void) { return gGondowanCaveExits; }

/// Iodem waits in Gondowan Passage once Babi has spoken to the party after
/// Colosso.
const struct ActorTemplate *Scene_GetActorTemplates(void) {
  s16 sceneId = gSaveBlock.sceneId;

  if (sceneId == (s32)&SceneId_GondowanPassage) {
    if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
      return gGondowanPassageActorTemplatesWithIodem;
    }
    return gGondowanPassageActorTemplates;
  }
  if (sceneId == (s32)&SceneId_GondowanCaveRiver) {
    return gGondowanCaveRiverActorTemplates;
  }
  if (sceneId == (s32)&SceneId_GondowanCave) {
    return gGondowanCaveActorTemplates;
  }
  return gGondowanCaveActorTemplatesOther;
}

/// The triggers of each map, with Iodem's in Gondowan Passage when he is
/// there.
const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gSaveBlock.sceneId;

  if (sceneId == (s32)&SceneId_GondowanPassage) {
    if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
      return gGondowanPassageTriggersWithIodem;
    }
    return gGondowanPassageTriggers;
  }
  if (sceneId == (s32)&SceneId_GondowanCaveRiver) {
    return gGondowanCaveRiverTriggers;
  }
  if (sceneId == (s32)&SceneId_GondowanCave) {
    return gGondowanCaveTriggers;
  }
  return gGondowanCaveTriggersOther;
}

/// Turning back in Gondowan Passage (step trigger 10) with Iodem in the party,
/// until he has reached Babi Lighthouse: he steps out and asks why Isaac is
/// going back. Yes: he waits here for the party's return. No: he rejoins the
/// party and Isaac steps away from the step trigger.
static void EventScript_IodemAsksWhyTurningBack(void) {
  struct Actor *leader;

  if (GameFlag_Get(FLAG_BABI_LIGHTHOUSE_IODEM_ARRIVED) == 0 &&
      Script_GetFlag(FLAG_IODEM_FOLLOWING_PARTY) != 0) {
    FieldScript_Start();
    // Bring Iodem out of the party before opening his question.
    ObjectMotion_SetSpeedParameters(ACTOR_IODEM, PIXELS(1), PIXELS(1) / 2);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetPosition(ACTOR_IODEM, leader->x.fixed, leader->z.fixed);
    }
    ObjectMotion_OffsetPositionAndResetMotion(ACTOR_IODEM, -8, 16);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_IODEM);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH + FACING_STEP, 0);
    FieldScript_WaitFrames(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
    FieldScript_SetMessageId(MsgKaragolWhyGoingBackIsaacDo);
    Script_OpenMessage(ACTOR_IODEM, 0);
    if (FieldScript_AskYesNoWithActorAnimation(0, 0) == 0) {
      // "I see... In that case, I shall wait here for you to return." The
      // saved place becomes Tolbi's.
      Script_ShowMessage(ACTOR_IODEM, 0);
      Actor_WalkTo(ACTOR_IODEM, 152, 232);
      Script_ClearFlag(FLAG_IODEM_FOLLOWING_PARTY);
      ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_IODEM);
      Import_ActorSetAnimation(ACTOR_IODEM, ANIM_STAND);
      gSaveBlock.restartScene = (s32)&SceneId_StuccoRoomsB;
      gSaveBlock.restartEntrance = STUCCO_ROOMS_B_SANCTUM_NORTH;
    } else {
      // "Then we should continue on our way to the lighthouse." He rejoins
      // the party, and Isaac steps away from the step trigger.
      gFieldWork->message += 1;
      Script_ShowMessage(ACTOR_IODEM, 0);
      Import_ActorSetAnimation(ACTOR_IODEM, ANIM_WALK);
      leader = Actor_Get(ACTOR_PARTY_LEADER);
      if (leader != 0) {
        Actor_SetDestination(ACTOR_IODEM, leader->x.part.pixel,
                             leader->z.part.pixel);
      }
      ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_IODEM);
      Actor_SetPosition(ACTOR_IODEM, PIXELS(0), PIXELS(0));
      FieldScript_WaitFrames(30);
      Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
      ObjectMotion_OffsetPositionAndResetMotion(ACTOR_PARTY_LEADER, 0, 16);
      ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_PARTY_LEADER);
      Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    }
    FieldScript_End();
  }
}

/// Iodem comes along with the party from Gondowan Passage, where he waits. He
/// comes along when he's talked to, or walked up to (step trigger 11) while he
/// isn't with the party. `I've been waiting for you, Isaac! We should go to
/// Babi Lighthouse soon.`
static void EventScript_IodemRejoins(void) {
  FieldScript_Start();
  ObjectMotion_SetAngleToward(ACTOR_IODEM, ACTOR_PARTY_LEADER, 0);
  ObjectMotion_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FieldScript_WaitFrames(10);
  Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
  FieldScript_SetMessageId(MsgKaragolIveBeenWaitingForIsaac);
  FieldScript_ShowMessageAndWait(ACTOR_IODEM, 0);
  Import_ActorSetAnimation(ACTOR_IODEM, ANIM_WALK);
  {
    struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

    if (leader != 0)
      ObjectMotion_ResetAndSetPosition(ACTOR_IODEM, leader->x.part.pixel,
                                       leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_IODEM);
  Import_ActorSetPosition(ACTOR_IODEM, 0, 0);
  FieldScript_WaitFrames(20);
  GameFlag_Set(FLAG_IODEM_FOLLOWING_PARTY);
  FieldScript_End();
}

/// Talking to the first soldier in Gondowan Passage before Babi has spoken
/// after Colosso: he asks whether the party will cross into Gondowan. `Do you
/// wish to cross into Gondowan?` Yes: `I won't stop you, but I wouldn't go
/// unless I had business there.`, with advice to watch Colosso until it is
/// over. No: `That's a good idea. They wouldn't let you into Babi Lighthouse,
/// anyway.`
static void EventScript_SoldierAsksAboutCrossing(void) {
  s32 firstMessage;

  firstMessage = MsgKaragolDoYouWishCrossInto;
  FieldScript_SetMessageId(firstMessage);
  FieldMessage_ShowAtObject(ACTOR_FIRST_SOLDIER, 0);
  if (FieldScript_AskYesNoWithActorAnimation(0, 0) == 0) {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) == 0) {
        // The shorter answer, without the advice to watch Colosso.
        FieldScript_SetMessageId(firstMessage + 8);
      }
    }
    FieldScript_ShowMessageAndWait(ACTOR_FIRST_SOLDIER, 0);
  } else {
    gFieldWork->message++;
    FieldScript_ShowMessageAndWait(ACTOR_FIRST_SOLDIER, 0);
  }
}

/// Mind Read on the second soldier in Gondowan Passage, before Babi has
/// spoken after Colosso: once Colosso is over (FLAG_COLOSSO_OVER) he grumbles
/// that guard duty made him miss all of it; before, that the warriors fight
/// while he guards the bridge.
static void EventScript_ReadSoldierMind(void) {
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0 &&
      GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) == 0)
    FieldScript_SetMessageId(MsgKaragolWeMissedColossoBecauseWe);
  else
    FieldScript_SetMessageId(MsgKaragolWarriorsHaveBeenFightingWhile);
  FieldScript_ShowMessageAndWait(ACTOR_SECOND_SOLDIER, 0);
}

/// Leaving GondowanCave by GONDOWAN_CAVE_TO_WORLD_MAP_EAST (exit 1),
/// out to the world map. With FLAG_SCENE_TEMP_04 set, which nothing in the
/// game sets, three unnamed flags and the pillar's are first put into one
/// arrangement.
static void EventScript_LeaveToWorldMapEast(void) {
  struct FieldWork *work;

  work = gFieldWork;
  FieldScript_Start();
  // XXX (no evidence): what flags 0x9a3, 0x9a4 and 0x9a6 hold; only this
  // block, which nothing reaches, touches them.
  if (GameFlag_Get(FLAG_SCENE_TEMP_04) != 0) {
    GameFlag_Clear(0x9a3);
    Script_ClearFlag(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED);
    Script_ClearFlag(0x9a4);
    GameFlag_Clear(0x9a6);
    GameFlag_Set(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED);
    GameFlag_Set(0x9a4);
  }
  Import_ScriptRequestExit(work->stepTrigger);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_End();
}

/// Walking west or east into step triggers 10 and 11 in GondowanCave: the
/// leader jumps two cells forward where the map allows (Field_TryJumpForward),
/// across the stump and the pillar once they float in the water.
static void EventScript_JumpAcross(void) { Field_TryJumpForward(); }

/// Move on the stump in GondowanCave, until it has leapt: from column 30 or
/// less it spins and leaps into the water, freeing the cell where it stood.
static void EventScript_MoveStump(void) {
  Actor_GetById(ACTOR_STUMP);
  FieldScript_Start();
  {
    struct Actor *actor = Actor_GetById(ACTOR_STUMP);

    if ((actor->x.fixed >> 20) <= 30) {
      GondowanCave_RunSpinningLeap(ACTOR_STUMP);
      MapGrid_CopyCellAttributes(29, 19, 1, 1, 27, 19);
      GameFlag_Set(FLAG_GONDOWAN_CAVE_STUMP_MOVED);
    }
  }
  FieldScript_End();
}

/// Pushing the pillar in GondowanCave (step trigger 50): it moves a
/// step, then may have reached its place.
static void EventScript_PushPillar(void) {
  PushedObject_AdvancePair();
  EventScript_MovePillar();
}

/// Hides the pillar and shows the actor that stands in for it in its
/// place.
static inline void GondowanCave_PlaceDroppedPillar(s32 animation) {
  Actor_SetPosition(ACTOR_PILLAR, PIXELS(0), PIXELS(0));
  Actor_SetPosition(ACTOR_DROPPED_PILLAR, PIXELS(696), PIXELS(288));
  Import_ActorSetAnimation(ACTOR_DROPPED_PILLAR, animation);
}

/// Move on the pillar in GondowanCave, and after each push: past column 42 it
/// drops into the water with a splash, for good.
static void EventScript_MovePillar(void) {
  FieldScript_Start();
  if ((Actor_GetById(ACTOR_PILLAR)->x.fixed >> 20) > 42) {
    s32 destinationCellX = 107;
    s32 destinationCellZ = 17;

    MapGrid_CopyCellAttributes(108, 17, 1, 1, destinationCellX,
                               destinationCellZ);
    FieldScript_WaitFrames(8);
    GondowanCave_PlaceDroppedPillar(STUMP_ANIMATION_SPLASH);
    Script_PlaySong(SE_CAST);
    GameFlag_Set(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED);
  }
  FieldScript_End();
}

static UNUSED void GondowanCave_UnusedActorCallback(void) {}

/// Rotates the sprite during the airborne part of the leap.
static void GondowanCave_RotateLeapingObject(union ActorOrEffect *fieldObject) {
  fieldObject->actor.sprite->rotation -= 0x400;
}

/// Waits at most forty frames for the actor to descend to the height
/// limit.
static void GondowanCave_WaitForDescent(struct Actor *actor, s32 heightLimit) {
  s32 framesRemaining = 40;

  while (framesRemaining != 0) {
    WaitFrames(1);
    framesRemaining--;
    if (actor->y.fixed <= heightLimit) {
      break;
    }
  }
}

/// The object spins in place, leaps and lands with a shake and a ring of dust,
/// then comes to rest a little further on.
static void GondowanCave_RunSpinningLeap(s32 actorId) {
  struct Actor *actor;
  u32 stepIndex;
  struct Vector3 particleVelocity;
  s32 angle;

  actor = Actor_GetById(actorId);
  // Wind up the spin while horizontal motion is disabled.
  actor->motionFlags = 0;
  for (stepIndex = 0; stepIndex < 9; stepIndex++) {
    WaitFrames(1);
    actor->sprite->rotation -= 0x100;
    actor->x.fixed -= Trig_Cos(actor->sprite->rotation) / 2;
    actor->targetX = ACTOR_NO_TARGET;
  }
  actor->update = GondowanCave_RotateLeapingObject;
  // Continue spinning through the first jump.
  Sound_PlaySong(SE_WHOOSH);
  Actor_SetSpeed(actorId, PIXELS(2), PIXELS(1));
  Actor_SetDestination(actorId, 472, 288);
  actor->gravity = 4 * PIXELS(1) / 5;
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  actor->layer = 0;
  ObjectMotion_CommitCurrentPositionAndActivate(actorId);
  GondowanCave_WaitForDescent(actor, PIXELS(32));
  MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  // Emit a complete particle fan, including its closing direction.
  for (stepIndex = 0; stepIndex < 17; stepIndex++) {
    angle = stepIndex << 12;
    particleVelocity.x = Trig_Cos(angle);
    particleVelocity.y = 0;
    particleVelocity.z = Trig_Sin(angle);
    particleVelocity.x -= particleVelocity.x / 4;
    particleVelocity.z -= particleVelocity.z / 2;
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed,
                      particleVelocity.x, particleVelocity.y,
                      particleVelocity.z, 0, 0);
  }
  Actor_SetDestination(actorId, 440, 308);
  // Finish the second movement before restoring the resting animation.
  ObjectMotion_CommitCurrentPositionAndActivate(actorId);
  GondowanCave_WaitForDescent(actor, PIXELS(32));
  actor->update = 0;
  actor->sprite->rotation = 0x1000;
  Sound_PlaySong(SE_CAST);
  Import_ActorSetAnimation(actorId, STUMP_ANIMATION_SPLASH);
  MapGrid_WaitForShakeToSettle();
  FieldScript_WaitFrames(10);
  Import_ActorSetAnimation(actorId, STUMP_ANIMATION_SINK);
}

static UNUSED void GondowanCave_UnusedMapCallback(void) {}

/// Lift on the boulder in GondowanCave also takes away the
/// invisible actor beside it.
static void EventScript_LiftBoulder(void) {
  Import_ActorSetPosition(ACTOR_BOULDER_GAP, 0, 0);
}

/// Sets up the map as the party enters. Gondowan Passage blocks Retreat and
/// keeps Iodem out of sight while he travels with the party; Gondowan Cave's
/// GondowanCave puts the stump and the pillar where the party left them.
s32 Scene_Initialize(void) {
  struct Actor *actor;

  if (gSaveBlock.sceneId == (s32)&SceneId_GondowanPassage) {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    if (GameFlag_Get(FLAG_IODEM_FOLLOWING_PARTY) != 0) {
      Actor_SetPosition(ACTOR_IODEM, PIXELS(0), PIXELS(0));
    }
  }
  if (gSaveBlock.sceneId == (s32)&SceneId_GondowanCave) {
    // XXX (no evidence): why this arrival changes the drop's cell; the
    // change has no effect that play shows.
    if (gSaveBlock.entrance == GONDOWAN_CAVE_FROM_WORLD_MAP_EAST) {
      MapGrid_CopyCellAttributes(108, 17, 1, 1, 107, 17);
    }
    if (GameFlag_Get(FLAG_GONDOWAN_CAVE_STUMP_MOVED) != 0) {
      Actor_SetPosition(ACTOR_STUMP, PIXELS(440), PIXELS(308));
      Import_ActorSetAnimation(ACTOR_STUMP, STUMP_ANIMATION_SINK);
      MapGrid_CopyCellAttributes(29, 19, 1, 1, 27, 19);
    }
    if (GameFlag_Get(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED) != 0) {
      GondowanCave_PlaceDroppedPillar(STUMP_ANIMATION_SINK);
    }
    actor = Actor_Get(ACTOR_BOULDER_GAP);
    Actor_SetSpriteFlags(actor, 0);
  }
  return 0;
}

s32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

/// Packs signed pixel steps with X in the upper halfword and Z in the lower.
/// Shift the unsigned representation so westward steps remain defined C.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))

s32 gPushedObject_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedObject_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedObject_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

struct SceneEntrance gGondowanCaveEntrancesOther[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

struct SceneEntrance gGondowanPassageEntrances[] = {
    { GONDOWAN_PASSAGE_FROM_WORLD_MAP_NORTH, CONDITION_ALWAYS, 152, 0, 120, FACING_SOUTH, 0, 32, 32, 272, 480, 0 },
    { GONDOWAN_PASSAGE_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 152, 0, 440, FACING_NORTH, 0, 32, 32, 272, 480, 0 },
    { MAP_TABLE_END },
};

struct SceneEntrance gGondowanCaveRiverEntrances[] = {
    { GONDOWAN_CAVE_RIVER_FROM_GONDOWAN_CAVE_NORTH, CONDITION_ALWAYS, 728, 0, 312, FACING_NORTH, 0, 96, 48, 768, 432, 0 },
    { GONDOWAN_CAVE_RIVER_FROM_BOULDER_PASSAGE, CONDITION_ALWAYS, 168, 0, 408, FACING_NORTH, 0, 96, 48, 768, 432, 0 },
    { GONDOWAN_CAVE_RIVER_BOULDER_PASSAGE_FROM_RIVER, CONDITION_ALWAYS, 168, 0, 520, FACING_SOUTH, 0, 112, 464, 576, 704, 0 },
    { GONDOWAN_CAVE_RIVER_FROM_GONDOWAN_CAVE_SOUTH, CONDITION_ALWAYS, 504, 0, 664, FACING_NORTH, 0, 112, 464, 576, 704, 0 },
    { MAP_TABLE_END },
};

struct SceneEntrance gGondowanCaveEntrances[] = {
    { GONDOWAN_CAVE_FROM_WORLD_MAP_EAST, CONDITION_ALWAYS, 792, 0, 456, FACING_NORTH, 0, 240, 48, 864, 464, 0 },
    { GONDOWAN_CAVE_FROM_GONDOWAN_CAVE_RIVER_NORTH, CONDITION_ALWAYS, 360, 0, 152, FACING_SOUTH, 0, 240, 48, 864, 464, 0 },
    { GONDOWAN_CAVE_FROM_WEST_PASSAGE, CONDITION_ALWAYS, 344, 0, 440, FACING_NORTH, 0, 240, 48, 864, 464, 0 },
    { GONDOWAN_CAVE_WEST_PASSAGE_FROM_EAST_CAVE, CONDITION_ALWAYS, 344, 0, 584, FACING_SOUTH, 0, 48, 432, 408, 896, 0 },
    { GONDOWAN_CAVE_FROM_GONDOWAN_CAVE_RIVER_SOUTH, CONDITION_ALWAYS, 136, 0, 504, FACING_SOUTH, 0, 48, 432, 408, 896, 0 },
    { GONDOWAN_CAVE_FROM_WORLD_MAP_WEST, CONDITION_ALWAYS, 216, 0, 888, FACING_NORTH, 0, 48, 432, 408, 896, 0 },
    { MAP_TABLE_END },
};

u32 gGondowanCaveExits[] = {
    SCENE_EXITS(GondowanPassage),
    SCENE_EXIT(GONDOWAN_PASSAGE_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_GONDOWAN_PASSAGE_EAST),
    SCENE_EXIT(GONDOWAN_PASSAGE_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_GONDOWAN_PASSAGE_WEST),
    SCENE_EXITS(GondowanCaveRiver),
    SCENE_EXIT(GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_NORTH, GondowanCave, GONDOWAN_CAVE_FROM_GONDOWAN_CAVE_RIVER_NORTH),
    SCENE_EXIT(GONDOWAN_CAVE_RIVER_TO_BOULDER_PASSAGE, GondowanCaveRiver, GONDOWAN_CAVE_RIVER_BOULDER_PASSAGE_FROM_RIVER),
    SCENE_EXIT(GONDOWAN_CAVE_RIVER_BOULDER_PASSAGE_TO_RIVER, GondowanCaveRiver, GONDOWAN_CAVE_RIVER_FROM_BOULDER_PASSAGE),
    SCENE_EXIT(GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_SOUTH, GondowanCave, GONDOWAN_CAVE_FROM_GONDOWAN_CAVE_RIVER_SOUTH),
    SCENE_EXITS(GondowanCave),
    SCENE_EXIT(GONDOWAN_CAVE_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_GONDOWAN_CAVE_NORTH),
    SCENE_EXIT(GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_NORTH, GondowanCaveRiver, GONDOWAN_CAVE_RIVER_FROM_GONDOWAN_CAVE_NORTH),
    SCENE_EXIT(GONDOWAN_CAVE_TO_WEST_PASSAGE, GondowanCave, GONDOWAN_CAVE_WEST_PASSAGE_FROM_EAST_CAVE),
    SCENE_EXIT(GONDOWAN_CAVE_WEST_PASSAGE_TO_EAST_CAVE, GondowanCave, GONDOWAN_CAVE_FROM_WEST_PASSAGE),
    SCENE_EXIT(GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_SOUTH, GondowanCaveRiver, GONDOWAN_CAVE_RIVER_FROM_GONDOWAN_CAVE_SOUTH),
    SCENE_EXIT(GONDOWAN_CAVE_TO_WORLD_MAP_WEST, WorldMap, WORLD_MAP_FROM_GONDOWAN_CAVE_SOUTH),
    SCENE_EXITS_END,
};

struct ActorTemplate gGondowanCaveActorTemplatesOther[] = {
    { MAP_TABLE_END },
};

struct ActorTemplate gGondowanPassageActorTemplates[] = {
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(144), 0, PIXELS(168), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(120), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

struct ActorTemplate gGondowanPassageActorTemplatesWithIodem[] = {
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(136), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(120), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    // Iodem
    { SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(232), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

struct ActorTemplate gGondowanCaveRiverActorTemplates[] = {
    { SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(504), 0, PIXELS(624), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(440), 0, PIXELS(104), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ActorTemplate gGondowanCaveActorTemplates[] = {
    { SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(496), 0, PIXELS(304), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(664), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(636), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(640), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

struct Trigger gGondowanCaveTriggersOther[] = {
    { TRIGGERS_END },
};

struct Trigger gGondowanPassageTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_NORTH },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_SOUTH },
    { TRIGGER_TALK, ACTOR_FIRST_SOLDIER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SoldierAsksAboutCrossing) },
    { TRIGGER_TALK, ACTOR_SECOND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveUsWereTheSoldiersWhoGuard) },
    { TRIGGER_TALK, ACTOR_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEvenIfYouHeadEastYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveThoseWhoGoToGondowanRarely) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_SOLDIER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadSoldierMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveYoudThinkYouCouldGetThrough) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_GONDOWAN_CAVE_APPLE, FIND_ITEM(ITEM_APPLE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_GONDOWAN_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGERS_END },
};

struct Trigger gGondowanPassageTriggersWithIodem[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_NORTH },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_SOUTH },
    { TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_IodemAsksWhyTurningBack) },
    { TRIGGER_STEP, 11, FLAG_IODEM_FOLLOWING_PARTY, EVENT_SCRIPT(EventScript_IodemRejoins) },
    { TRIGGER_TALK, ACTOR_FIRST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWhyWouldIodemChooseToWait) },
    { TRIGGER_TALK, ACTOR_SECOND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWeHaventSeenAnybodyUnusualCrossing) },
    { TRIGGER_TALK, ACTOR_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEvenIfYouGoEastFrom) },
    { TRIGGER_TALK, ACTOR_IODEM, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_IodemRejoins) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveIodemIsGoingToBabiLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveItsALotOfStressBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEveryoneComesDownThisWayWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_IODEM, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWeHaveToHeadForThe) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_GONDOWAN_CAVE_APPLE, FIND_ITEM(ITEM_APPLE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_GONDOWAN_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGERS_END },
};

struct Trigger gGondowanCaveRiverTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_NORTH },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_TO_BOULDER_PASSAGE },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_BOULDER_PASSAGE_TO_RIVER },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_SOUTH },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_RIVER_BOULDER, CONDITION_ALWAYS, 0 },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_GONDOWAN_CAVE_APPLE, FIND_ITEM(ITEM_APPLE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_GONDOWAN_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGERS_END },
};

/// FLAG_SCENE_TEMP_05 ends Move on the pillar.
struct Trigger gGondowanCaveTriggers[] = {
    { TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToWorldMapEast) },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_NORTH },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_WEST_PASSAGE },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, GONDOWAN_CAVE_WEST_PASSAGE_TO_EAST_CAVE },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_SOUTH },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_WORLD_MAP_WEST },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_BOULDER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LiftBoulder) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP, FLAG_GONDOWAN_CAVE_STUMP_MOVED, EVENT_SCRIPT(EventScript_MoveStump) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, FLAG_SCENE_TEMP_05, EVENT_SCRIPT(EventScript_MovePillar) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_JumpAcross) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_JumpAcross) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 50, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushPillar) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_GONDOWAN_CAVE_APPLE, FIND_ITEM(ITEM_APPLE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_GONDOWAN_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGERS_END },
};
// clang-format on
