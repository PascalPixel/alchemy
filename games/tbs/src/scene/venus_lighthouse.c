/// @file
/// Venus Lighthouse, six scenes in one overlay: the approach outside, the floor
/// the front door opens onto, and the four floors of the climb that ends in the
/// Lighthouse Aerie at the top (venus_lighthouse_aerie.c). Each scene has its
/// own entrance, placement and event tables (VenusLighthouseGroundFloor's
/// entrances are the fallback table), all share one exit table, and
/// Scene_RunEntrySetup redraws each scene's puzzles as their flags left them,
/// case by entrance. The party reaches the lighthouse after the strange
/// wanderers, as the wounded outside call them, have opened it and gone on.
///
/// - Venus Lighthouse Entry (SceneId_VenusLighthouseEntry), outside. The
///   scholars and Babi's soldiers the wanderers attacked lie hurt: a young boy
///   seemed to glow and the entrance opened, one soldier thought he
///   recognized one of them as Kraden ("An alchemist who served Babi... Was
///   it Kraden?"), and they soon headed north for Babi Lighthouse. A man who
///   tells of the attack flickers and vanishes
///   (FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD), and the collision of his spot
///   changes. VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_EAST and _WEST lead to the
///   world map, and VENUS_LIGHTHOUSE_ENTRY_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR,
///   the open door, into VenusLighthouseGroundFloor.
/// - Venus Lighthouse (SceneId_VenusLighthouseGroundFloor, named by a scene
///   rule of its own), the floor behind the front door. Three scholars say
///   Kraden told the attackers to stop, went in, and came back saying this is
///   not the entrance but something about its being the exit, and that his
///   group did something to the statue that lit up the floor. The statue
///   (search trigger 41) speaks to Isaac's soul: the path long closed begins
///   far away, on the soil of an ancient people. The room flashes and the
///   statue opens one of its two paths (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or
///   _SECOND_PATH, chosen the first time by the frame counter's parity and
///   switched at each search after), redrawing cells here; the Tunnel Ruins
///   under Babi Lighthouse redraw their rooms to match (tunnel_ruins.c), and
///   they lead up into VenusLighthouseFirstFloor. Reliefs (search triggers 35
///   to 40) say to seek the power within with the eyes of truth, and Reveal
///   shows a hidden path in two places. Move pushes a pillar to a wall, where
///   Garet wishes they could carry it (FLAG_VENUS_LIGHTHOUSE_PILLAR_AT_WALL),
///   and Carry, from the Carry Stone in one of the floor's two chests, sets it
///   down beyond (FLAG_VENUS_LIGHTHOUSE_PILLAR_CARRIED). A floor switch is
///   pressed while the leader stands on it (FLAG_SCENE_TEMP_01) and held down
///   by a pillar Moved onto it (FLAG_SCENE_TEMP_00); until then, walking off a
///   ledge (trigger 45) drops the leader to the floor below.
///   VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR (trigger 23)
///   and VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR link the
///   two scenes.
/// - Venus Lighthouse (SceneId_VenusLighthouseFirstFloor), the first floor
///   of the climb, where the Tunnel Ruins come up
///   (VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_TUNNEL_RUINS_LADY_STATUES;
///   VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_TUNNEL_RUINS_LADY_STATUES leads back
///   down), in the room
///   VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR also
///   reaches. A floor drifts the party south (trigger 30), a hole drops the
///   leader into the room below (trigger 35), three pillars can be Moved or
///   pushed, and two pillars by a gap are carried across it with Carry, or
///   dropped into it to fall through the floor. A chest holds the Thunder
///   Crown, and a Psynergy Stone lies here.
/// - Venus Lighthouse (SceneId_VenusLighthouseSecondFloor). A sparkling
///   passage (trigger 2) takes the leader through in a trail of sparks; a floor
///   switch (trigger 35) slides a wall aside in a cloud of dust the first time
///   (FLAG_VENUS_LIGHTHOUSE_WALL_MOVED); a pillar Moved or pushed into its
///   socket sinks and opens a gate (FLAG_VENUS_LIGHTHOUSE_GATE_OPEN); and a
///   door is locked.
///   VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR leads up
///   into VenusLighthouseThirdFloor.
/// - Venus Lighthouse (SceneId_VenusLighthouseThirdFloor). Three more
///   sparkling passages, a drifting floor, and three pillars that sink into
///   free sockets (FLAG_VENUS_LIGHTHOUSE_SOCKETS_A and _B record which); with
///   all three sunk the gate opens
///   (FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN). A door is locked and a
///   chest holds Dragon Scales.
/// - Venus Lighthouse (SceneId_VenusLighthouseFourthFloor). A passage raises
///   the leader to the floor above (trigger 18), floors drift the party each
///   way (triggers 30 to 33), and a west and an east floor switch slide their
///   walls aside (FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED and _EAST_WALL_MOVED,
///   which VenusLighthouseThirdFloor also redraws). Carry sets down four
///   floating pillars, and the platforms under them rise or sink to their
///   weight (FLAG_SCENE_TEMP_00 plus the pillar's number; Move on them does
///   nothing). Four pillars sunk into sockets open the gate of the room their
///   pillars stand in (FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN). A door is
///   locked, a chest holds the Gaia Blade, a Psynergy Stone lies here, and
///   VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_AERIE leads to the
///   Lighthouse Aerie.
///
/// The wall and gate flags (FLAG_VENUS_LIGHTHOUSE_WALL_MOVED to
/// FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN) are place flags: they last while the
/// party stays in the scenes of the climb (SceneId_VenusLighthouseFirstFloor to
/// SceneId_VenusLighthouseAerie, one place group),
/// so the floors stay solved between them. The FLAG_SCENE_TEMP_ states last
/// only until the next change of scene other than a return from battle.
/// Every arrival sets FLAG_VENUS_LIGHTHOUSE_LIT.

#include "script.h"
#include "game_state.h"
#include "unit/import.h"
#include "unit/scene/venus_lighthouse.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "iowrite.h"
#include "object_effect.h"
#include "macros.h"

#define NULL ((void *)0)
#include "global_cells.h"
#include "types.h"
#include "animspr.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_effect.h"
#include "staged_actor.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "iwram_call.h"
#include "call.h"
#include "text/msg_ids.h"
#include "field_service.h"
#include "io_reg.h"
#include "scene_ids.h"
#include "map.h"
#include "map_scroll.h"
#include "field_runtime.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"

// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another place group, and other places use the numbers for their own.

// The floor switches have slid the walls aside.
#define FLAG_VENUS_LIGHTHOUSE_WALL_MOVED 0x300
#define FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED 0x306
/// ★ Named from what the player sees: the floor switch on the east side of
/// the fourth floor (its render)
#define FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED 0x307
// The gates the sunken pillars opened: one pillar, three pillars (and which of
// two sets of sockets they sank into) and the pillar room's four.
#define FLAG_VENUS_LIGHTHOUSE_GATE_OPEN 0x301
#define FLAG_VENUS_LIGHTHOUSE_SOCKETS_A 0x302
/// The second set of sockets.
/// ★ Named from what the player sees: the square sockets in the third floor
/// that the pillars sink into (its render)
#define FLAG_VENUS_LIGHTHOUSE_SOCKETS_B 0x303
#define FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN 0x304
#define FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN 0x308

// UB: passage offsets and dust directions can be negative. The callers use
// bounded pixel offsets or directions -1..1, so these products fit s32.
#ifdef UBFIX
#define VENUS_LIGHTHOUSE_FIXED_OFFSET(value, bits) ((value) * (1 << (bits)))
#else
#define VENUS_LIGHTHOUSE_FIXED_OFFSET(value, bits) ((value) << (bits))
#endif

#ifdef UBFIX
#define VENUS_LIGHTHOUSE_RUN_OPENING_SEQUENCE                                  \
  VenusLighthouse_ArriveThroughSparkPassage
#else
#define VENUS_LIGHTHOUSE_RUN_OPENING_SEQUENCE                                  \
  ((s32 (*)(void))VenusLighthouse_ArriveThroughSparkPassage)
#endif

#if TBS_PATCH_3
#define VENUS_DESCENT_COLLISION_X 33
#define VENUS_DESCENT_COLLISION_WIDTH 7
#else
#define VENUS_DESCENT_COLLISION_X 34
#define VENUS_DESCENT_COLLISION_WIDTH 5
#endif

static void VenusLighthouse_StepActorWithDust(struct Vector3 *direction);
static void VenusLighthouse_AnimatePairedTileWaves(void);
static void VenusLighthouse_EmitWestPassageParticles(void);
static void VenusLighthouse_EmitEastPassageParticles(void);
static void VenusLighthouse_AnimateWestTileWave(void);
static void VenusLighthouse_AnimateEastTileWave(void);
static void VenusLighthouse_RaiseLeaderWithSpray(void);

// The lighthouse's scene flags:
// FLAG_SCENE_TEMP_00: in the room with the floor switch, the pillar holds the
// switch down; in the room of floating pillars, FLAG_SCENE_TEMP_00 plus a
// pillar's number (0 to 3): that pillar is set down in its place.
// FLAG_SCENE_TEMP_01: the leader stands on the floor switch.

// Each scene numbers its actors by the rows of its own placement table, so
// each has an enum of its own; a row drawn as one of the party's characters
// (Garet in VenusLighthouseGroundFloor) is that party member in his own slot
// and takes no number.

/// Venus Lighthouse Entry's actors, numbering the rows of
/// gVenusLighthouseEntryPlacements: the scholars (sprite 0x70) and Babi's
/// soldiers (SPRITE_SOLDIER) the wanderers attacked; after the man by the
/// entrance, each is named by what they say.
enum VenusLighthouseEntryActor {
  /// Drawn as the soldiers are: "Venus Lighthouse was... attacked by
  /// strange... wanderers...", and he flickers and is gone
  /// (Event_HearOfTheAttack).
  ACTOR_ENTRANCE_MAN = ACTOR_FIRST_PLACED,
  /// "Iodem, be happy. The entrance to Venus Lighthouse is open..."
  /// ★ Named from the game: "The entrance to Venus Lighthouse is open"
  /// (MsgVenusLighthouseIodemBeHappyTheEntranceTo)
  ACTOR_SCHOLAR_LIGHTHOUSE_OPEN,
  /// "Ohhh! How could they attack mere scholars? Those barbarians!"
  /// ★ Named from the game: "Those barbarians!"
  /// (MsgVenusLighthouseOhhhHowCouldTheyAttackMere)
  ACTOR_SCHOLAR_BARBARIANS,
  /// "I've waited so long to see what it's like inside Venus Lighthouse,
  /// but..." (Event_TalkToWaitingScholar)
  /// ★ Named from the game: "I've waited so long" (MsgVenusIveWaitedLongSeeIts)
  ACTOR_WAITING_SCHOLAR,
  /// "How... How did they open the door to Venus Lighthouse?"
  /// ★ Named from the game: "How did they open the door to Venus Lighthouse?"
  /// (MsgVenusLighthouseHowHowDidTheyOpenThe)
  ACTOR_SOLDIER_OPENED_DOOR,
  /// "They are already... headed for Babi Lighthouse..."
  /// ★ Named from the game: "headed for Babi Lighthouse..."
  /// (MsgVenusLighthouseTheyAreAlreadyHeadedForBabi)
  ACTOR_SOLDIER_HEADED_FOR_BABI,
  /// "Babi's soldiers are all Colosso victors... How could anyone beat all of
  /// us?" His thoughts: "Was it Kraden?"
  /// ★ Named from the game: "Babi's soldiers are all Colosso victors..."
  /// (MsgVenusLighthouseBabisSoldiersAreAllColossoVictors)
  ACTOR_SOLDIER_COLOSSO_VICTORS
};

/// VenusLighthouseGroundFloor's actors, numbering the rows of
/// gVenusLighthouseGroundFloorPlacements after Garet's.
enum VenusLighthouseGroundFloorActor {
  /// "Kraden told them to stop, but they attacked anyway."
  /// ★ Named from the game: "Kraden told them to stop, but they attacked
  /// anyway." (MsgVenusLighthouseKradenToldThemToStopBut)
  ACTOR_SCHOLAR_ATTACKED_ANYWAY = ACTOR_FIRST_PLACED,
  /// "Kraden went inside, but then came back and said this isn't the
  /// entrance."
  /// ★ Named from the game: "said this isn't the entrance."
  /// (MsgVenusLighthouseKradenWentInsideButThenCame)
  ACTOR_SCHOLAR_NOT_THE_ENTRANCE,
  /// Event_TalkToScholarByDoor.
  /// ★ Named from the game: "I thought I'd explore after the door opened."
  /// (MsgVenusThoughtIdExploreAfterDoor)
  ACTOR_DOOR_SCHOLAR,
  /// The pillar Move pushes to the wall and Carry sets down beyond it
  /// (Event_MovePillarTowardWall).
  /// ★ Named from what the player sees: a grey pillar carved with spirals, two
  /// blocks tall, short of the wall (its render)
  ACTOR_WALL_PILLAR = ACTOR_FIRST_PLACED + 4,
  /// The lady statue Move pushes onto the floor switch (SPRITE_LADY_STATUE)
  /// (Event_MovePillarOntoSwitch).
  /// ★ Named from what the player sees: a white statue of a robed woman beside
  /// a floor switch (its render)
  ACTOR_FLOOR_SWITCH_STATUE
};

/// VenusLighthouseFirstFloor's actors, numbering the rows of
/// gVenusLighthouseFirstFloorPlacements.
enum VenusLighthouseFirstFloorActor {
  /// The three pillars, SPRITE_CAPPED_PILLAR, that Move or a push shifts, the
  /// nearer
  /// drawn in front (Event_MoveDepthPillar).
  /// ★ Named from what the player sees: grey pillars with white tops, one alone
  /// and two side by side (its render)
  ACTOR_FIRST_DEPTH_PILLAR = ACTOR_FIRST_PLACED,
  /// The two pillars by the gap, carried across it or dropped into it
  /// (Event_CarryPillarAcrossGap).
  /// ★ Named from what the player sees: two grey pillars carved with spirals
  /// beside the gap (its render)
  ACTOR_FIRST_CARRY_PILLAR = ACTOR_FIRST_DEPTH_PILLAR + 3
};

/// VenusLighthouseSecondFloor's actors, numbering the rows of
/// gVenusLighthouseSecondFloorPlacements.
enum VenusLighthouseSecondFloorActor {
  /// A lady statue, as ACTOR_FLOOR_SWITCH_STATUE is, where
  /// ACTOR_GATE_EFFECT sets out for the gate.
  /// ★ Named from what the player sees: a white statue of a robed woman beside
  /// the gate (its render)
  ACTOR_GATE_EFFECT_STATUE = ACTOR_FIRST_PLACED,
  /// The pillar before the gate, which sinks into its socket
  /// (Event_MoveGatePillar).
  /// ★ Named from what the player sees: a grey pillar carved with spirals on
  /// the track before the gate (its render)
  ACTOR_GATE_PILLAR,
  /// SPRITE_PARTICLE, the kind of object
  /// VenusLighthouse_CreateScriptedGateEffect makes: once the pillar sinks it
  /// travels gVenusPushScript to the gate, which opens, and stays there
  /// shedding particles.
  /// ★ Named from what the player sees: a small sparkle that rises by the
  /// gate as the pillar sinks (its render)
  ACTOR_GATE_EFFECT
};

/// VenusLighthouseThirdFloor's actors, numbering the rows of
/// gVenusLighthouseThirdFloorPlacements.
enum VenusLighthouseThirdFloorActor {
  /// A lady statue (SPRITE_LADY_STATUE), where the two gate effects set out
  /// once all
  /// three switch pillars have sunk.
  /// ★ Named from what the player sees: a white statue of a robed woman at the
  /// end of the gold track (its render)
  ACTOR_SWITCH_EFFECT_STATUE = ACTOR_FIRST_PLACED,
  /// The three pillars that sink into free sockets (Event_MoveSwitchPillar).
  /// ★ Named from what the player sees: three grey pillars carved with spirals
  /// on the gold track (its render)
  ACTOR_FIRST_SWITCH_PILLAR,
  /// SPRITE_PARTICLE, as ACTOR_GATE_EFFECT, placed where the gate effects end
  /// once the gate is open.
  /// ★ Named from what the player sees: a sparkle, as on the second floor,
  /// where a gate has opened (its render)
  ACTOR_SWITCH_GATE_EFFECT = ACTOR_FIRST_SWITCH_PILLAR + 3
};

/// VenusLighthouseFourthFloor's actors, numbering the rows of
/// gVenusLighthouseFourthFloorPlacements.
enum VenusLighthouseFourthFloorActor {
  /// The two platforms, SPRITE_BALANCE_PLATFORM, that rise or sink to the
  /// weight of the
  /// floating pillars set down on them (VenusLighthouse_LowerFloatingPillars).
  /// ★ Named from what the player sees: a white platform on a pole, the west of
  /// two (its render)
  ACTOR_BALANCE_WEST = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: the east white platform on its pole
  /// (its render)
  ACTOR_BALANCE_EAST,
  /// The four floating pillars Carry sets down (Event_CarryFloatingPillar).
  /// ★ Named from what the player sees: four grey pillars carved with spirals,
  /// three in a row above the platforms (its render)
  ACTOR_FIRST_FLOATING_PILLAR,
  /// A lady statue (SPRITE_LADY_STATUE), where the first gate effect sets out
  /// once all
  /// four socket pillars have sunk; the camera goes to it first.
  /// ★ Named from what the player sees: a white statue of a robed woman at the
  /// foot of the gold track (its render)
  ACTOR_SOCKET_EFFECT_STATUE = ACTOR_FIRST_FLOATING_PILLAR + 4,
  /// The four pillars of the pillar room (Event_MoveSocketPillar).
  /// ★ Named from what the player sees: four grey pillars carved with spirals
  /// on the gold track (its render)
  ACTOR_FIRST_SOCKET_PILLAR,
  /// SPRITE_PARTICLE, as ACTOR_GATE_EFFECT, placed where the gate effects end
  /// once the gate is open.
  /// ★ Named from what the player sees: a sparkle, as on the second floor,
  /// where a gate has opened (its render)
  ACTOR_SOCKET_GATE_EFFECT = ACTOR_FIRST_SOCKET_PILLAR + 4,
  /// The platform, SPRITE_FLOATING_PLATFORM as the floating platforms of
  /// Mercury
  /// Lighthouse's aerie, that dips as the leader lands on it at
  /// VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE_RISING
  /// and then carries him up to the Lighthouse Aerie
  /// (VenusLighthouse_RaiseLeaderWithSpray).
  /// ★ Named from what the player sees: a round white platform in a pit of its
  /// own (its render)
  ACTOR_RISING_PLATFORM
};

static __inline__ void SetFlagBits(u8 *flags, u8 bits) { *flags |= bits; }

/// Sets up a passage effect's motion, collision and child value.
static void VenusLighthouse_ConfigurePassageEffect(struct FieldActor *object,
                                                   s32 childValue) {
  object->motionFlags = 0;
  object->collisionFlags = 8;
  Engine_ActorSetSpriteFlags(object, 0);
  ObjectGroup_SetChildValue(object, childValue);
}

/// Creates a blended passage effect behind the actors.
static struct FieldActor *
VenusLighthouse_CreateBlendedPassageEffect(s32 x, s32 y, s32 z, s32 type) {
  struct FieldActor *effect;

  effect = Engine_ObjectCreate(type, x, y, z);
  if (effect != 0) {
    ((struct AnimationObject *)effect->sprite)->image.priority = 0;
    VenusLighthouse_ConfigurePassageEffect(effect, 14);
    Engine_ObjectSetBlendMode(effect, 1);
    return effect;
  }
  return 0;
}

/// Creates a passage effect that preserves the overlapping actors' draw order.
static struct FieldActor *
VenusLighthouse_CreateOccludingPassageEffect(s32 x, s32 y, s32 z, s32 type) {
  struct FieldActor *object;

  object = Engine_ObjectCreate(type, x, y, z);
  if (object != 0) {
    ((struct AnimationObject *)object->sprite)->image.priority = 1;
    VenusLighthouse_ConfigurePassageEffect(object, 15);
    object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    return object;
  }
  return 0;
}

#include "common/effect_spawn.h"
#include "ability_ids.h"
#include "objcmd.h"

#include "common/object_distance.h"
#include "common/object_findslot.h"

/// Pushes the adjacent pillar only when its destination and overhead space are
/// clear.
static void VenusLighthouse_PushAdjacentPillar(void) {
  struct FieldPosition targetPosition;
  struct FieldActor *leader;
  struct FieldActor *pillar;
  struct FieldActor *blockingActor;
  s32 directionIndex;
  u32 packedStep;
  s32 moveRate;
  s32 transitionBusy;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  directionIndex = leader->facing >> 12;
  // Look ahead for a pillar before testing its destination and overhead space.
  packedStep = gStagedActor_DirectionSteps[directionIndex];
  targetPosition.x = leader->x.fixed + (packedStep & 0xffff0000);
  targetPosition.y = leader->y.fixed;
  packedStep <<= 16;
  targetPosition.z = leader->z.fixed + packedStep;
  pillar = SceneData_FindSlotAtPosition(&targetPosition, leader);
  if (pillar == 0)
    return;

  packedStep = gStagedActor_DirectionSteps[directionIndex];
  targetPosition.x = pillar->x.fixed + (packedStep & 0xffff0000);
  targetPosition.y = pillar->y.fixed;
  packedStep <<= 16;
  targetPosition.z = pillar->z.fixed + packedStep;
  blockingActor = SceneData_FindSlotAtPosition(&targetPosition, pillar);
  if (blockingActor != 0 && (blockingActor->collisionFlags & 1) != 0)
    return;

  targetPosition.x = pillar->x.fixed;
  targetPosition.y = pillar->y.fixed + 0x100000;
  targetPosition.z = pillar->z.fixed;
  blockingActor = SceneData_FindSlotAtPosition(&targetPosition, pillar);
  if (blockingActor != 0 && (blockingActor->collisionFlags & 1) != 0)
    return;

  pillar->terrainId = 2;
  packedStep = gStagedActor_DirectionSteps[directionIndex];
  targetPosition.x = pillar->x.fixed + (packedStep & 0xffff0000);
  targetPosition.y = pillar->y.fixed;
  packedStep <<= 16;
  targetPosition.z = pillar->z.fixed + packedStep;
  if (FieldObject_CheckMovementCollision(pillar, &targetPosition) > 0)
    return;

  transitionBusy = pillar->riseCounter;
  if (transitionBusy != 0)
    return;

  // Move the pillar and leader together, then restore their standing state.
  Object_SetMode(leader, 8);
  moveRate = 0x3333;
  Engine_TaskWait(15);
  Engine_AudioPlayCue(SE_185);
  pillar->speed = moveRate;
  pillar->acceleration = moveRate;
  Engine_ObjectSetPosition(pillar, targetPosition.x, targetPosition.y,
                           targetPosition.z);
  leader->speed = moveRate;
  leader->acceleration = moveRate;
  Engine_ObjectSetPosition(leader, targetPosition.x, targetPosition.y,
                           targetPosition.z);
  FieldObject_WaitForMove(pillar);
  BattleFx_PlayQueuedSound();
  pillar->x.fixed = targetPosition.x;
  pillar->z.fixed = targetPosition.z;
  pillar->velocityX = transitionBusy;
  pillar->velocityZ = transitionBusy;
  leader->targetX = ACTOR_NO_TARGET;
  leader->targetZ = ACTOR_NO_TARGET;
  leader->velocityX = transitionBusy;
  leader->velocityZ = transitionBusy;
  leader->x.fixed = leader->x.part.pixel << 16;
  leader->z.fixed = leader->z.part.pixel << 16;
  Object_SetMode(leader, 1);
}

/// Unused: fills collision codes in a rectangle of the requested map layer;
/// nothing in Venus Lighthouse calls it.
static UNUSED s32 StagedActor_FillGridAttributeRectangle(u32 layerIndex, s32 x,
                                                         s32 z, u32 width,
                                                         u32 height,
                                                         s32 collisionCode) {
  struct MapScrollWork *map = gMapWork[0];
  struct MapCell *cells;
  u32 row;
  u32 column;

  if (map != 0) {
    if (layerIndex <= 2) {
      cells = map->layers[layerIndex].cells;
    } else {
      cells = (struct MapCell *)gMapCellBuffer;
    }
    cells += x + (z << 7);
    for (row = 0; row < height; row++) {
      struct MapCell *cell = cells + (row << 7);

      for (column = 0; column < width; column++) {
        cell->collisionCode = collisionCode;
        cell++;
      }
    }
  }
  return 0;
}

/// Restores child values encoded in the placed actors' script timers.
static void VenusLighthouse_RestoreScriptedChildValues(void) {
  struct FieldActor **actors = gEventWork->placedActors;
  struct FieldActor *actor;
  u32 actorId;

  for (actorId = ACTOR_FIRST_PLACED; actorId <= 65; actorId++) {
    actor = actors[actorId - ACTOR_FIRST_PLACED];
    if (actor->timer >> 4 == 0x212)
      ObjectGroup_SetChildValue(actor, actor->timer & 15);
  }
}

/// Applies the current script timer's child value and advances the script.
static s32
VenusLighthouse_UpdateChildFromTimer(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  ObjectGroup_SetChildValue(actor, actor->timer & 15);
  return 0;
}

// Pulse the child effect and play its timed sound cue.
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 6 bytes of the 56-byte function. Keep the unused
// native result in matching builds.
#ifdef UBFIX
static void VenusLighthouse_PulseChildEffect(union FieldObject *fieldObject) {
#else
static s32 VenusLighthouse_PulseChildEffect(union FieldObject *fieldObject) {
#endif
  void *actor = (void *)fieldObject;
  if ((gFrameCount & 3) == 0)
    ObjectGroup_SetChildValue(actor, 7);
  else
    ObjectGroup_SetChildValue(actor, 0);

  if ((gFrameCount & 7) == 0)
    Engine_AudioPlayCue(SE_ITEM_BREAK);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

// Emit a rising particle near the actor every eight frames.
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 14 bytes of the 172-byte function. Keep the unused
// native result in matching builds.
#ifdef UBFIX
static void
VenusLighthouse_EmitRisingParticles(union FieldObject *fieldObject) {
#else
static s32 VenusLighthouse_EmitRisingParticles(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct EffectOptions options;
  s32 framePhase, particleX, particleY, riseSpeed;
  framePhase = gFrameCount & 7;
  if (framePhase == 0) {
    options.priority = 3 - ((u32)(Engine_RandomNext() * 2) >> 16);
    options.startScaleX = 0x6666;
    options.startScaleY = 0x6666;
    options.palette = 14;
    particleX =
        actor->x.fixed + ((((u32)(Engine_RandomNext() * 9) >> 16) - 4) << 16);
    particleY =
        actor->y.fixed + ((32 - ((u32)(Engine_RandomNext() * 32) >> 16)) << 16);
    riseSpeed = Math_Divide(
        (((u32)(Engine_RandomNext() * 5) >> 16) << 16) + 0x00050000, 10);
    Effect_Spawn(particleX, particleY, actor->z.fixed, 0, riseSpeed, framePhase,
                 0x000b0000, &options);
  }
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Clears the scripted actor's sprite flags and advances its script.
static s32 VenusLighthouse_ClearScriptedSpriteFlags(
    struct ScriptInterpreter *interpreter) {
  void *actor = (void *)interpreter;
  Engine_ActorSetSpriteFlags(actor, 0);
  return 0;
}

// Emit the two particle patterns used by the leader's passage trail.
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 14 bytes of the 180-byte function. Keep the unused
// native result in matching builds.
#ifdef UBFIX
static void
VenusLighthouse_EmitPassageSparkTrail(union FieldObject *fieldObject) {
#else
static s32
VenusLighthouse_EmitPassageSparkTrail(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct EffectOptions options;
  s32 fallSpeed;
  s32 framePhase;

  options.startScaleX = 0x0000cccc;
  options.startScaleY = 0x0000cccc;
  options.priority = 0;

  fallSpeed = ((u32)(Engine_RandomNext() * 8) >> 16) * 0x3333;
  framePhase = gFrameCount & 15;

  Effect_Spawn(actor->x.fixed +
                   VENUS_LIGHTHOUSE_FIXED_OFFSET(8 - framePhase, 16),
               actor->y.fixed + 0x001a0000, actor->z.fixed, 0, -fallSpeed, 0,
               0x000a0000, &options);

  framePhase = gFrameCount & 15;
  if (framePhase == 0) {
    options.startScaleX = 0x00008000;
    options.startScaleY = 0x00008000;
    Effect_Spawn(actor->x.fixed +
                     ((((u32)(Engine_RandomNext() * 9) >> 16) - 4) << 16),
                 actor->y.fixed, actor->z.fixed, 0, 0, 0, 0x000a0000, &options);
  }
#ifdef UBFIX

  return;
#else

  return 0;
#endif
}

/// Walking north into a sparkling passage, in VenusLighthouseSecondFloor
/// (trigger 2) or VenusLighthouseThirdFloor (triggers 4, 5 and 11): the leader
/// dissolves in sparks and leaves by the exit of the trigger's number, coming
/// out at its far end.
static void Event_EnterSparklingPassage(void) {
  struct FieldActor *leader;
  struct EventWork *eventWork;

  eventWork = gEventWork;
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Engine_AudioPlayCue(SE_228);
#ifdef UBFIX
  leader->update = VenusLighthouse_EmitPassageSparkTrail;
#else
  leader->update =
      (void (*)(union FieldObject *))VenusLighthouse_EmitPassageSparkTrail;
#endif
  leader->speed = 0x3333;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  leader->update = 0;
  Engine_EventWait(30);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(eventWork->touchedTrigger);
  Engine_EventEnd();
}

/// Brings the leader out of a sparkling passage on a fresh entrance.
static void VenusLighthouse_ArriveThroughSparkPassage(void) {
  struct FieldActor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    leader->motionFlags = 0;
    Actor_SetPosition(ACTOR_PARTY_LEADER, leader->x.part.pixel << 16,
                      (leader->z.part.pixel << 16) - 0x100000);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Event_PlayCue(SE_228);
#ifdef UBFIX
    leader->update = VenusLighthouse_EmitPassageSparkTrail;
#else
    leader->update =
        (void (*)(union FieldObject *))VenusLighthouse_EmitPassageSparkTrail;
#endif
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 1);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
    leader->motionFlags = 3;
    leader->update = 0;
    BattleFx_PlayQueuedSound();
    Engine_EventEnd();
  }
}

/// Stepping onto this spot in VenusLighthouseFirstFloor (trigger 32) makes the
/// leader the actor the room's effect follows.
static void Event_SetLeaderAsEffectTarget(void) {
  gActorEffectWork->actor = Object_GetById(ACTOR_PARTY_LEADER);
}

/// Stepping onto this spot in VenusLighthouseFirstFloor (trigger 31): the
/// room's effect follows no one.
static void Event_ClearEffectTarget(void) { gActorEffectWork->actor = NULL; }

#include "common/object_depth.h"
#include "common/object_waitidle.h"
#include "common/effect_advance.h"

/// Drops the leader onto the floor and emits the radial landing dust.
static void VenusLighthouse_LandWithRadialDust(void) {
  struct FieldActor *leader;
  u32 particleIndex;
  struct EffectOptions options;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  leader->y.fixed = PIXELS(130);
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(30);
  Event_PlayCue(SE_204);
  leader->motionFlags = 3;
  Engine_EventWait(24);
  options.palette = 7;
  options.update = (void (*)(union FieldObject *))Effect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  for (particleIndex = 0; particleIndex <= 16; particleIndex++) {
    s32 angle;
    s32 velocity[3];

    angle = particleIndex << 12;
    SET_VECTOR(velocity, Engine_MathCos(angle), 0, Engine_MathSin(angle));
    velocity[0] += velocity[0] / 2;
    Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed, velocity[0],
                 velocity[1], velocity[2], 0x1090001, &options);
  }
  Event_PlayCue(SE_188);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 22);
  Map_SetShake(0x50000, 0x50000, 0x10000);
  Map_SetShake(-1, -1, 0xe666);
  Engine_MapRenderWaitForValues();
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT);
  BattleFx_PlayQueuedSound();
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  Engine_EventEnd();
}

#include "common/effect_radial.h"

// Emit shrinking particles around a moving gate effect every four frames.
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 172 bytes (extent 196 to 192 bytes). Keep the
// unused native result in matching builds.
#ifdef UBFIX
static void VenusLighthouse_EmitGateParticles(union FieldObject *fieldObject) {
#else
static s32 VenusLighthouse_EmitGateParticles(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct EffectOptions options;
  s32 velocityY;
  s32 velocityX;
  s32 velocityZ;
  s32 random;

  if ((gFrameCount & 3) != 0) {
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  }
  if ((u32)(Engine_RandomNext() * 6) >> 16 == 0) {
    if (actor->targetX != ACTOR_NO_TARGET ||
        actor->targetZ != ACTOR_NO_TARGET) {
      Engine_AudioPlayCue(SE_246);
    }
  }
  velocityY = 0;
  options.type = 286;
  options.startScaleX = 0x10000;
  options.startScaleY = 0x10000;
  options.targetScaleX = -327;
  options.targetScaleY = -327;
  random = Engine_RandomNext();
  velocityX = Math_Divide(((((u32)(random * 9)) >> 16) - 4) << 16, 10);
  random = Engine_RandomNext();
  velocityZ = Math_Divide(((((u32)(random * 9)) >> 16) - 4) << 16, 10);
  Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed - 0x10000,
               velocityX, velocityY, velocityZ, 0x001c0001, &options);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Creates a scripted gate effect at the supplied pixel coordinates.
static struct FieldActor *
VenusLighthouse_CreateScriptedGateEffect(s32 x, s32 z, const s32 *script) {
  struct FieldActor *effect;
  s32 fixedX;
  s32 fixedZ;

  fixedX = x << 16;
  fixedZ = z << 16;
  effect = Engine_ObjectCreate(SPRITE_PARTICLE, fixedX, 0, fixedZ);
  if (effect == 0) {
    return 0;
  }
  effect->speed = 0x10000;
  effect->acceleration = 0x10000;
  Engine_ActorSetSpriteFlags(effect, 0);
  Object_SetMode(effect, 7);
  effect->motionFlags = 0;
  effect->y.fixed = 0;
  effect->collisionFlags = 0;
  effect->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
#ifdef UBFIX
  effect->update = VenusLighthouse_EmitGateParticles;
#else
  effect->update =
      (void (*)(union FieldObject *))VenusLighthouse_EmitGateParticles;
#endif
  effect->riseEnabled = 0;
  Engine_ObjectSetScript(effect, script);
  return effect;
}

/// Walking into a one-tile gap, in VenusLighthouseFourthFloor and through the
/// jump-or-push events of VenusLighthouseFirstFloor to
/// VenusLighthouseFourthFloor: the leader jumps it if the far side is clear.
/// Returns 1 after a jump, 0 when there was none.
static s32 Event_JumpAcrossGap(void) {
  struct FieldActor *leader;
  struct FieldPosition landing;
  u8 *motionFlags;
  u8 savedMotionFlags;
  s32 direction;
#if !TBS_PATCH_1
  u32 step;
#endif

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  motionFlags = &leader->motionFlags;
  // Check the intervening cell before looking for a clear landing two cells
  // ahead.
  savedMotionFlags = *motionFlags;
#if TBS_PATCH_1
  landing.x = (leader->x.fixed & 0xfff00000) + 0x80000;
  landing.y = leader->y.fixed;
  landing.z = (leader->z.fixed & 0xfff00000) + 0x80000;
  direction = (leader->facing + 0x2000) & 0xc000;
  Vector_AddPolarOffset(0x100000, direction, &landing);
#else
  direction = leader->facing >> 12;
  step = gStagedActor_DirectionSteps[direction];
  landing.x = leader->x.fixed + (step & 0xffff0000);
  landing.y = leader->y.fixed;
  step <<= 16;
  landing.z = leader->z.fixed + step;
#endif
  if (FieldObject_CheckMovementCollision(leader, &landing) != 1 &&
      SceneData_FindSlotAtPosition(&landing, leader) == 0) {
#if TBS_PATCH_1
    landing.x = (leader->x.fixed & 0xfff00000) + 0x80000;
    landing.y = leader->y.fixed;
    landing.z = (leader->z.fixed & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x200000, (leader->facing + 0x2000) & 0xc000,
                          &landing);
#else
    step = gStagedActor_DirectionSteps[direction];
    landing.x = leader->x.fixed + ((step & 0xffff0000) << 1);
    landing.y = leader->y.fixed;
    step <<= 17;
    landing.z = leader->z.fixed + step;
#endif
    if (SceneData_FindSlotAtPosition(&landing, leader) == 0 &&
        FieldObject_CheckMovementCollision(leader, &landing) == 0) {
      Engine_EventBegin();
      Object_SetMode(leader, 6);
      Engine_TaskWait(6);
      Engine_AudioPlayCue(SE_JUMP);
      Object_SetMode(leader, 7);
      leader->speed = 0x30000;
      leader->acceleration = 0x20000;
      leader->velocityY = 0x40000;
      *motionFlags &= 0x7e;
      Engine_ActorSetSpriteFlags(leader, 0);
      Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, landing.x >> 16,
                                landing.z >> 16);
      Object_SetMode(leader, 6);
      Engine_ActorSetSpriteFlags(leader, 1);
      *motionFlags = savedMotionFlags;
      Engine_EventEnd();
      return 1;
    }
  }
  return 0;
}

/// Selects the scene's entrances; VenusLighthouseGroundFloor takes the fallback
/// table.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_VenusLighthouseEntry) {
    return gVenusLighthouseEntryEntrances;
  }
  if (scene == (s32)&SceneId_VenusLighthouseFirstFloor) {
    return gVenusLighthouseFirstFloorEntrances;
  }
  if (scene == (s32)&SceneId_VenusLighthouseSecondFloor) {
    return gVenusLighthouseSecondFloorEntrances;
  }
  if (scene == (s32)&SceneId_VenusLighthouseThirdFloor) {
    return gVenusLighthouseThirdFloorEntrances;
  }
  if (scene == (s32)&SceneId_VenusLighthouseFourthFloor) {
    return gVenusLighthouseFourthFloorEntrances;
  }
  return gVenusLighthouseEntrancesOther;
}

/// These scenes supply no exit regions.
const struct SceneRegion *SceneData_ReturnZero(void) { return 0; }

/// Returns the packed exits shared by the six scenes.
const u32 *SceneData_GetPrimaryTable(void) {
  return gVenusLighthousePrimaryTable;
}

/// Selects the scene's placements. Inside the lighthouse,
/// FieldScene_PrepareActors first gives each row with condition 0 whether it
/// lies inside the view bounds, so only the room entered is populated.
const struct ScenePlacement *Scene_GetPlacements(void) {
  struct ScenePlacement *placements;
  s16 scene;

  scene = gGameState.scene;
  if (scene == (s32)&SceneId_VenusLighthouseEntry) {
    return gVenusLighthouseEntryPlacements;
  }
  if (scene == (s32)&SceneId_VenusLighthouseGroundFloor) {
    placements = gVenusLighthouseGroundFloorPlacements;
    FieldScene_PrepareActors(placements);
    return placements;
  }
  if (scene == (s32)&SceneId_VenusLighthouseFirstFloor) {
    placements = gVenusLighthouseFirstFloorPlacements;
    FieldScene_PrepareActors(placements);
    return placements;
  }
  if (scene == (s32)&SceneId_VenusLighthouseSecondFloor) {
    placements = gVenusLighthouseSecondFloorPlacements;
    FieldScene_PrepareActors(placements);
    return placements;
  }
  if (scene == (s32)&SceneId_VenusLighthouseThirdFloor) {
    placements = gVenusLighthouseThirdFloorPlacements;
    FieldScene_PrepareActors(placements);
    return placements;
  }
  if (scene == (s32)&SceneId_VenusLighthouseFourthFloor) {
    placements = gVenusLighthouseFourthFloorPlacements;
    FieldScene_PrepareActors(placements);
    return placements;
  }
  return gVenusLighthousePlacementsOther;
}

/// Searching a locked door, in VenusLighthouseSecondFloor to
/// VenusLighthouseFourthFloor. "The door is tightly locked."
static void Event_SearchLockedDoor(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldDoorTightlyLocked, 1);
  Engine_EventEnd();
}

/// At Venus Lighthouse Entry, talking to the man by the entrance (or reading
/// his mind): "Venus Lighthouse was attacked by strange wanderers..." He
/// flickers ten times and is gone (FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD), and the
/// collision of his spot changes, as it does on later arrivals.
static void Event_HearOfTheAttack(void) {
  u32 flickersLeft;
  u32 flickerDelay;

  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(ACTOR_ENTRANCE_MAN, 3);
  Engine_EventSetMessage(MsgFieldVenusLighthouseWasAttackedBy);
  flickersLeft = 10;
  flickerDelay = 8;
  Engine_EventShowMessageAndWait(ACTOR_ENTRANCE_MAN, 0, 20);
  do {
    Engine_ActorSetChildValue(ACTOR_ENTRANCE_MAN, 15);
    Engine_TaskWait(2);
    Engine_ActorSetChildValue(ACTOR_ENTRANCE_MAN, 0);
    Engine_TaskWait(flickerDelay);
    if (flickerDelay > 3) {
      flickerDelay--;
    }
    flickersLeft--;
  } while (flickersLeft != 0);
  Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD);
  Engine_ActorSetPosition(ACTOR_ENTRANCE_MAN, 0, 0);
  Map_CopyCellAttributes(7, 17, 2, 1, 7, 16);
  Engine_EventEnd();
}

/// At Venus Lighthouse Entry, talking to the scholar who has waited so long to
/// see inside Venus Lighthouse, but cannot move for the pain.
static void Event_TalkToWaitingScholar(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVenusIveWaitedLongSeeIts);
  Engine_EventShowMessageAndWait(ACTOR_WAITING_SCHOLAR, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_WAITING_SCHOLAR, 2);
  Engine_EventShowMessage(ACTOR_WAITING_SCHOLAR, 0);
  Engine_EventEnd();
}

/// In VenusLighthouseGroundFloor, Carry on the pillar Move could not push past
/// the wall: once it is set down beyond, it stays there
/// (FLAG_VENUS_LIGHTHOUSE_PILLAR_CARRIED).
static void Event_CarryPillarPastWall(void) {
  struct FieldActor *pillar;

  pillar = Object_GetById(ACTOR_WALL_PILLAR);
  Engine_EventBegin();
  if (pillar->x.fixed >> 20 == 54 || pillar->z.fixed >> 20 == 6) {
    Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_PILLAR_CARRIED);
  }
  Engine_EventEnd();
}

/// Searching a relief in VenusLighthouseGroundFloor shows its words, the relief
/// trigger's own message. "There are words carved into the relief..."
///
/// The Japanese text gives each of the six reliefs its own words; every
/// translation joins the first five into one message and leaves the next four
/// empty, so outside Japanese only the first and sixth reliefs can be read
/// (gVenusLighthouseGroundFloorEvents). The English edition, patch 1, also
/// dropped the sixth relief and showed the first message for every relief;
/// patch 2 put the per-relief message and the sixth relief back.
static void Event_ReadRelief(void) {
  s32 message;
#if !TBS_PATCH_1 || TBS_PATCH_2
  struct EventWork *work = gEventWork;
#endif
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
#if TBS_PATCH_1 && !TBS_PATCH_2
  message = (s32)MSGNO(MsgVenusThereWordsCarvedIntoRelief);
#else
  message =
      work->facingTrigger + MSGNO(MsgVenusThereWordsCarvedIntoRelief) - 35;
#endif
  Engine_MessageShowCentered(message, 1);
  Engine_EventEnd();
}

/// Reveal fading in VenusLighthouseGroundFloor: the hidden path disappears
/// again, in both its places.
static void Event_HidePath(void) {
  s32 column;
  s32 row;

#if TBS_PATCH_1
  Engine_GameFlagClear(FLAG_NO_ENCOUNTERS_IN_PLACE);
#endif
  column = 23;
  row = 8;
  Map_CopyCellAttributes(35, 8, 1, 3, column, row);
  Map_CopyCellsTo(35, 8, 23, 8, 1, 3);
  Map_CopyCellsTo(99, 8, 87, 8, 1, 3);
  column = 46;
  row = 55;
  Map_CopyCellAttributes(57, 55, 3, 3, column, row);
  Map_CopyCellsTo(57, 55, 46, 55, 3, 3);
  Map_CopyCellsTo(121, 55, 110, 55, 3, 3);
}

/// Reveal in VenusLighthouseGroundFloor: the hidden path appears, in two
/// places, and can be walked on.
static void Event_RevealPath(void) {
  s32 column;
  s32 row;

#if TBS_PATCH_1
  Engine_GameFlagSet(FLAG_NO_ENCOUNTERS_IN_PLACE);
#endif
  column = 23;
  row = 8;
  Map_CopyCellAttributes(36, 8, 1, 3, column, row);
  Map_CopyCellsTo(36, 8, 23, 8, 1, 3);
  Map_CopyCellsTo(100, 8, 87, 8, 1, 3);
  column = 46;
  row = 55;
  Map_CopyCellAttributes(53, 55, 3, 3, column, row);
  Map_CopyCellsTo(53, 55, 46, 55, 3, 3);
  Map_CopyCellsTo(117, 55, 110, 55, 3, 3);
}

/// In VenusLighthouseGroundFloor, walking off the ledge (trigger 45) before the
/// floor switch is held down: the leader jumps down to the floor below and
/// shakes his head.
static void Event_JumpDown(void) {
  struct FieldActor *leader;
  s32 landingQuarterZ;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gVenusLeaderApproachScript);
  Object_RefreshSelectorById(ACTOR_PARTY_LEADER);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 6);
  leader->velocityY = 0x40000;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x40000, 0x20000);
  if (leader->z.fixed >> 20 <= 54) {
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    landingQuarterZ = 210;
  } else {
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    landingQuarterZ = 238;
  }
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, leader->x.part.pixel,
                      landingQuarterZ << 2);
  Engine_EventWait(1);
  SetFlagBits(&Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags,
              ACTOR_FACES_MOTION);
  Engine_EventWait(20);
#ifdef UBFIX
  leader->update = VenusLighthouse_EmitRisingParticles;
#else
  leader->update =
      (void (*)(union FieldObject *))VenusLighthouse_EmitRisingParticles;
#endif
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  leader->update = 0;
  Engine_EventEnd();
}

/// Searching the statue in VenusLighthouseGroundFloor: "The statue speaks to
/// Isaac's soul..." The room flashes, the path it opens changes (the first time
/// by the frame counter's parity, then to the other path) and its cells are
/// redrawn, and the camera shows it.
static void Event_SearchStatue(void) {
  struct FieldWork *work;

  work = (struct FieldWork *)gEventWork;
  work->revealFrames = 0;
  work->busy = 1;
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_MessageShowCentered(MsgVenusStatueSpeaksIsaacSoulYe, 1);
  ColorBuffer_ApplySource(0x10000, 0);
  ColorBuffer_ApplyTarget(0x10005, 0);
  Engine_ColorBufferInterpolate(120);
  Engine_EventWait(100);
  Event_PlayCue(SE_142);
  Engine_EventWait(30);
  ColorBuffer_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(60);
  Engine_EventWait(70);
  // The first consultation selects a route; later consultations alternate it.
  if (GameFlag_IsSet(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH) == 0) {
    if (GameFlag_IsSet(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH) == 0) {
      if ((gFrameCount & 1) != 0) {
        GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH);
      } else {
        GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH);
      }
    }
  }
  if (GameFlag_IsSet(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH) == 0) {
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH);
    GameFlag_Clear(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH);
    Map_CopyCellsTo(103, 27, 89, 27, 7, 8);
    Map_CopyCellsTo(41, 90, 27, 92, 3, 2);
    Map_CopyCellsTo(41, 90, 29, 93, 3, 2);
    Map_CopyCellsTo(41, 90, 27, 94, 3, 2);
    Map_CopyCellsTo(41, 90, 27, 96, 3, 2);
    Map_CopyCellsTo(41, 90, 29, 97, 3, 2);
    Map_CopyCellsTo(41, 96, 25, 91, 3, 2);
    Map_CopyCellsTo(41, 92, 25, 93, 3, 2);
    Map_CopyCellsTo(41, 96, 25, 95, 3, 2);
    Map_CopyCellsTo(41, 96, 25, 97, 3, 2);
    Map_CopyCellsTo(41, 96, 27, 96, 3, 2);
    Map_CopyCellsTo(41, 96, 29, 97, 3, 2);
  } else {
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH);
    GameFlag_Clear(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH);
    Map_CopyCellsTo(111, 27, 89, 27, 7, 8);
    Map_CopyCellsTo(41, 90, 25, 91, 3, 2);
    Map_CopyCellsTo(41, 90, 25, 93, 3, 2);
    Map_CopyCellsTo(41, 90, 25, 95, 3, 2);
    Map_CopyCellsTo(41, 90, 25, 97, 3, 2);
    Map_CopyCellsTo(41, 90, 27, 96, 3, 2);
    Map_CopyCellsTo(41, 90, 29, 97, 3, 2);
    Map_CopyCellsTo(41, 94, 27, 92, 3, 2);
    Map_CopyCellsTo(41, 96, 29, 93, 3, 2);
    Map_CopyCellsTo(41, 94, 27, 94, 3, 2);
    Map_CopyCellsTo(41, 96, 27, 96, 3, 2);
    Map_CopyCellsTo(41, 96, 29, 97, 3, 2);
  }
  ColorBuffer_ApplyTarget(0x10000, 0);
  Engine_ColorBufferInterpolate(20);
  Engine_EventWait(40);
  // Follow the revealed path after the statue and color effect have finished.
  Camera_SetSpeed(0x8000, 0x1000);
  Camera_MoveTo(PIXELS(456), -1, PIXELS(542), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(50);
  Camera_MoveTo(PIXELS(456), -1, PIXELS(423), 1);
  Engine_CameraWaitForMove();
  Engine_EventEnd();
  work->busy = 0;
}

/// In VenusLighthouseGroundFloor, Move on the pillar by the wall: once it
/// reaches the wall, the first time only
/// (FLAG_VENUS_LIGHTHOUSE_PILLAR_AT_WALL), Garet comes out. "Hmmm. We can't
/// push the pillar farther than this." He wishes they could carry it, which is
/// what Carry then does.
static void Event_MovePillarTowardWall(void) {
  struct FieldActor *actor;
  struct FieldActor *leader;
  s32 conversationFlag;
  s32 walkSpeed;
  s32 walkAcceleration;
  s32 emote;
  s32 garetX;

  conversationFlag = FLAG_VENUS_LIGHTHOUSE_PILLAR_AT_WALL;
  walkSpeed = 0xcccc;
  walkAcceleration = 0x6666;
  emote = EMOTE_IN_FRONT;
  garetX = 0x338;
  Engine_EventBegin();
  actor = Object_GetById(ACTOR_WALL_PILLAR);
  if (actor->x.fixed >> 20 == 53) {
    if (Engine_GameFlagIsSet(conversationFlag) == 0) {
      // Record the one-time conversation before bringing Garet out to inspect
      // the pillar.
      Engine_GameFlagSet(conversationFlag);
      actor = Object_GetById(ACTOR_PARTY_LEADER);
      if (actor != 0) {
        Engine_ActorSetPosition(GARET, actor->x.fixed, actor->z.fixed);
      }
      Engine_ActorSetSpeed(GARET, walkSpeed, walkAcceleration);
      Engine_ActorWalkToAndWait(GARET, garetX, 88);
      Engine_ActorWalkToAndWait(GARET, garetX, 104);
      Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
      Engine_EventWait(20);
      Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
      Engine_EventWait(20);
      Engine_EventSetMessage(MsgVenusHmmmWeCantPushBlock);
      Engine_EventShowMessageAndWait(GARET, 0, 20);
      Engine_ActorFaceDirection(GARET, FACING_EAST, 10);
      Engine_ActorShowEmote(GARET, emote, 60);
      Engine_ActorFaceActor(GARET, ACTOR_PARTY_LEADER, 0);
      Engine_EventWait(20);
      Engine_ActorRunRepeatedMotion(GARET, 2);
      Engine_EventWait(20);
      Engine_EventShowMessageAndWait(GARET, 0, 20);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
      Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
      Engine_EventWait(30);
      Engine_ActorWalkToAndWait(GARET, garetX, 88);
      Engine_ActorSetAnimation(GARET, ANIM_WALK);
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      if (leader != 0) {
        Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                                   leader->z.part.pixel);
      }
      Engine_ActorWaitForMove(GARET);
      Engine_ActorSetPosition(GARET, 0, 0);
      Engine_EventEnd();
    }
  }
}

/// In VenusLighthouseGroundFloor, pushing the pillar by the wall (touch trigger
/// 50): as Move, with Garet's remark at the wall.
static void Event_PushPillarTowardWall(void) {

  Engine_EventBegin();
  VenusLighthouse_PushAdjacentPillar();
  Engine_EventWait(20);
  Engine_EventEnd();
  Event_MovePillarTowardWall();
}

/// In VenusLighthouseGroundFloor, Move on the pillar by the floor switch: on
/// the switch it sinks in and holds it down (FLAG_SCENE_TEMP_00).
static void Event_MovePillarOntoSwitch(void) {
  struct FieldActor *pillar;
  s32 switchHeight;

  pillar = Object_GetById(ACTOR_FLOOR_SWITCH_STATUE);
  Engine_EventBegin();
  if (pillar->x.fixed >> 20 == 42) {
    Engine_EventWait(30);
    Engine_AudioPlayCue(SE_188);
    pillar->motionFlags = 0;
    switchHeight = 0xfffe0000;
    pillar->terrainHeight = switchHeight;
    pillar->y.fixed = switchHeight;
    Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
    Map_CopyCellsTo(44, 117, 41, 117, 3, 5);
  }
  Engine_EventEnd();
}

/// In VenusLighthouseGroundFloor, standing on the floor switch presses it
/// (FLAG_SCENE_TEMP_01); stepping off releases it.
static void Event_StepOnFloorSwitch(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 x = leader->x.part.pixel;
  s32 z = leader->z.part.pixel;

  if ((u32)(x - 676) > 7 || z < 788 || z >= 796) {
    Map_CopyCellsTo(53, 50, 42, 49, 1, 1);
    Map_CopyCellsTo(55, 117, 41, 117, 3, 5);
    GameFlag_Clear(FLAG_SCENE_TEMP_01);
    leader->motionFlags |= 1;
    leader->terrainHeight = 0;
    leader->y.fixed = 0;
  } else if (!GameFlag_IsSet(FLAG_SCENE_TEMP_01)) {
    Engine_EventBegin();
    Engine_EventWait(5);
    Map_CopyCellsTo(52, 50, 42, 49, 1, 1);
    Map_CopyCellsTo(52, 117, 41, 117, 3, 5);
    GameFlag_Set(FLAG_SCENE_TEMP_01);
    Engine_AudioPlayCue(SE_161);
    leader->motionFlags &= ~1;
    leader->terrainHeight = -PIXELS(2);
    leader->y.fixed = -PIXELS(2);
    Engine_EventEnd();
  }
}

/// In VenusLighthouseGroundFloor, talking to the scholar by the door: he
/// explored once it opened, until Kraden and the others appeared and did
/// something to the statue that lit up the floor; the camera looks at the door.
static void Event_TalkToScholarByDoor(void) {
  struct FieldWork *work;

  work = (struct FieldWork *)gEventWork;
  work->revealFrames = 0;
  work->busy = 1;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVenusThoughtIdExploreAfterDoor);
  Actor_FaceActor(ACTOR_DOOR_SCHOLAR, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(ACTOR_DOOR_SCHOLAR, 0, 20);
  Actor_FaceDirection(ACTOR_DOOR_SCHOLAR, FACING_NORTHEAST, 0);
  Camera_SetSpeed(0x10000, 0x2000);
  Camera_MoveTo(PIXELS(448), -1, PIXELS(432), 1);
  Engine_CameraWaitForMove();
  Event_ShowMessage(ACTOR_DOOR_SCHOLAR, 0);
  Engine_EventEnd();
}

/// In VenusLighthouseFirstFloor, stepping onto the hole in the floor (trigger
/// 35): the leader looks down, falls through and lands in the room below (
/// VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_LOWER_ROOM_HOLE).
static void Event_FallThroughFloor(void) {
  struct FieldActor *leader;
  struct FieldActor *leaderActor;
  s32 zero;
#if TBS_PATCH_3
  u8 *motionFlags;
#endif

#if !TBS_PATCH_3
  zero = 0;
#endif
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
#if TBS_PATCH_3
  motionFlags = (u8 *)&leader->motionFlags;
  zero = 0;
  *motionFlags = zero;
#endif
  Camera_MoveTo(-1, -1, -1, 0);
  leaderActor = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(leaderActor, 0);
  {
    u16 facing = FACING_SOUTH;

    leader->facing = facing;
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x30000, 0x18000);
  Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, leader->x.part.pixel, 0x228);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 22);
  Engine_EventWait(30);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  {
    s32 facing = FACING_NORTH;

    leader->facing = facing;
  }
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 5);
  Object_SetActionById(ACTOR_PARTY_LEADER, 24);
  Engine_EventWait(40);
  // Start the fall only after the approach and descent animation have finished.
  leader->gravity = 0x9999;
  {
    s32 z = leader->z.fixed + PIXELS(72);

    leader->bounce = zero;
    VenusLighthouse_CreateBlendedPassageEffect(leader->x.fixed, 0, z, 223);
  }
  Map_CopyCellAttributes(VENUS_DESCENT_COLLISION_X, 35,
                         VENUS_DESCENT_COLLISION_WIDTH, 1,
                         VENUS_DESCENT_COLLISION_X, 34);
#if TBS_PATCH_3
  *motionFlags = 3;
#endif
  OverlayObject_WaitUntilIdle(0);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_EventRequestExit(
      VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_LOWER_ROOM_HOLE);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

/// In VenusLighthouseFirstFloor, Move on one of the three pillars in a row: the
/// floor under each pillar is solid again and the nearer pillar draws in front.
static void Event_MoveDepthPillar(void) {

  u32 pillarIndex;
  struct FieldActor *pillar;
  s32 destinationColumn;
  s32 destinationRow;
  s32 column;
  s32 row;

  Object_GetById(ACTOR_FIRST_DEPTH_PILLAR);
  Engine_EventBegin();
  destinationColumn = 12;
  destinationRow = 44;
  Map_CopyCellAttributes(19, 44, 4, 1, destinationColumn, destinationRow);
  destinationColumn = 11;
  destinationRow = 51;
  Map_CopyCellAttributes(17, 51, 2, 2, destinationColumn, destinationRow);
  pillarIndex = 0;
  do {
    pillar = Object_GetById(pillarIndex + ACTOR_FIRST_DEPTH_PILLAR);
    column = pillar->x.fixed >> 20;
    row = pillar->z.fixed >> 20;
    Map_CopyCellAttributes(12, 50, 1, 1, column, row);
    pillarIndex++;
  } while (pillarIndex <= 2);
  SceneActor_SwapPositionsByDepth(ACTOR_FIRST_DEPTH_PILLAR + 2,
                                  ACTOR_FIRST_DEPTH_PILLAR + 1);
  Engine_EventEnd();
}

/// In VenusLighthouseFirstFloor, pushing one of the three pillars in a row.
static void Event_PushDepthPillar(void) {

  Engine_EventBegin();
  Map_CopyCellAttributes(19, 44, 4, 1, 12, 44);
  VenusLighthouse_PushAdjacentPillar();
  Event_MoveDepthPillar();
  Engine_EventEnd();
}

/// In VenusLighthouseFirstFloor, Move or Carry starting on one of the two
/// pillars by the gap: their tiles are marked before they move.
static void Event_LiftCarryPillar(void) {
  u32 pillarIndex;
  struct FieldActor *pillar;

  pillarIndex = 0;
  do {
    pillar = Object_GetById(pillarIndex + ACTOR_FIRST_CARRY_PILLAR);
    pillarIndex++;
    SetMapCellCollision(0, pillar->x.fixed, pillar->z.fixed, 45);
  } while (pillarIndex <= 1);
}

/// In VenusLighthouseFirstFloor, Move on one of the two pillars by the gap:
/// pillars still above the floor are solid again.
static void Event_MoveCarryPillar(void) {
  u32 pillarIndex;
  struct FieldActor *pillar;

  pillarIndex = 0;
  do {
    pillar = Object_GetById(pillarIndex + ACTOR_FIRST_CARRY_PILLAR);
    if (pillar->y.fixed > -0x100000) {
      SetMapCellCollision(0, pillar->x.fixed, pillar->z.fixed, 255);
    }
    pillarIndex++;
  } while (pillarIndex <= 1);
}

/// In VenusLighthouseFirstFloor, walking into a gap or a pillar by it: jump the
/// gap, else push the pillar.
static void Event_JumpOrPushCarryPillar(void) {

  Engine_EventBegin();
  if (Event_JumpAcrossGap() == 0) {
    Event_LiftCarryPillar();
    VenusLighthouse_PushAdjacentPillar();
    Event_MoveCarryPillar();
  }
  Engine_EventEnd();
}

/// Fixes a pillar at its destination and restores the cell beneath it.
static void VenusLighthouse_FixPillarAtDestination(struct FieldActor *pillar) {
  s32 column;
  s32 row;

  pillar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  pillar->motionFlags = 0;
  column = pillar->x.fixed >> 20;
  row = pillar->z.fixed >> 20;
  Map_CopyCellAttributes(9, 24, 1, 1, column, row);
}

/// Removes a fallen Carry pillar, or restores collision under the remaining
/// pillars.
static void
VenusLighthouse_ReleaseFallenCarryPillar(struct FieldActor *pillar) {
  struct AnimationObject *sprite;
  s32 zero;
  struct FieldActor *passageEffect;

  sprite = pillar->sprite;
  if (sprite->image.priority == 3) {
    sprite->image.priority = 1;
    zero = 0;
    pillar->bounce = zero;
    passageEffect = VenusLighthouse_CreateBlendedPassageEffect(
        pillar->x.fixed, 0, 0x2000000, 223);
    OverlayObject_WaitUntilIdle(pillar);
    pillar->x.fixed = zero;
    pillar->z.fixed = zero;
    Engine_ObjectDispatchRelease(passageEffect);
  } else {
    Event_MoveCarryPillar();
  }
}

/// In VenusLighthouseFirstFloor, Carry on one of the two pillars by the gap:
/// set down in its place it stays fixed; dropped into the gap it falls through
/// the floor.
static void Event_CarryPillarAcrossGap(void) {
  struct FieldActor *pillar;

  Engine_EventBegin();
  pillar = Object_GetById(ACTOR_FIRST_CARRY_PILLAR);
  if (pillar->x.fixed >> 20 == 8) {
    OverlayObject_WaitUntilIdle(pillar);
    VenusLighthouse_FixPillarAtDestination(pillar);
  } else {
    VenusLighthouse_ReleaseFallenCarryPillar(pillar);
  }
  pillar = Object_GetById(ACTOR_FIRST_CARRY_PILLAR + 1);
  if (pillar->x.fixed >> 20 == 7) {
    OverlayObject_WaitUntilIdle(pillar);
    VenusLighthouse_FixPillarAtDestination(pillar);
  } else {
    VenusLighthouse_ReleaseFallenCarryPillar(pillar);
  }
  Engine_EventEnd();
}

/// In VenusLighthouseSecondFloor, stepping on the floor switch (trigger 35):
/// the wall slides aside in a cloud of dust, the first time only
/// (FLAG_VENUS_LIGHTHOUSE_WALL_MOVED), and the scene cuts to
/// VenusLighthouseFirstFloor by
/// VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_EAST, where
/// VenusLighthouse_AnimatePairedTileWaves shows what moved there before
/// bringing the party back.
static void Event_StepOnWallSwitch(void) {
  struct EffectOptions options;
  struct MapLayerScroll *scrollLayer;
  struct FieldActor *leader;
  s32 column;
  s32 leaderZ;
  s32 tileDelay;
  u32 frame;
  s32 dustX;

  scrollLayer = &((struct MapScrollWork *)gMapWork[0])->layers[2];
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  column = leader->x.part.pixel;
  leaderZ = leader->z.part.pixel;
  leader->y.fixed = 0;
  if (column >= 308 && column <= 315 && leaderZ >= 532 && leaderZ < 540) {
    leader->y.fixed = -PIXELS(2);
    if (!GameFlag_IsSet(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED)) {
      Engine_EventBegin();
      Engine_AudioPlayCue(SE_161);
      GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED);
      Map_CopyCellsTo(26, 33, 19, 33, 1, 1);
      Engine_EventWait(30);
      Engine_AudioPlayCue(SE_239);
      Map_SetShake(0x10000, 0x10000, 0x10000);
      Engine_EventWait(20);
      dustX = PIXELS(288);
      column = 29;
      tileDelay = 40;
      // Scroll the layer while replacing the wall edge and emitting dust along
      // it.
      for (frame = 0; frame <= 479; frame++, tileDelay--) {
        scrollLayer->offsetX += 0x3333;
        dustX += -0x3333;
        options.priority = 2;
        options.startScaleX =
            ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
        options.startScaleY =
            ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
        options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
        Effect_Spawn(dustX, 0, 0x2100000, 0, -(((gFrameCount & 1) * 3) << 16),
                     0, 0x8a0000, &options);
        if (frame == 240) {
          dustX += -PIXELS(48);
        }
        if (tileDelay == 0) {
          tileDelay = 40;
          if (frame <= 240) {
            column -= 4;
            Map_CopyCellsTo(column, 50, 15, 32, 3, 4);
          } else {
            column += 4;
            Map_CopyCellsTo(column, 45, 9, 32, 3, 4);
          }
        }
        Engine_TaskWait(1);
      }
      scrollLayer->offsetX += 0x8000;
      scrollLayer->offsetX = scrollLayer->offsetX / 0x10000 << 16;
      Map_CopyCellAttributes(15, 32, 3, 1, 9, 32);
      Map_CopyCellAttributes(12, 32, 3, 1, 15, 32);
      Engine_AudioPlayCue(SE_288);
      Engine_AudioPlayCue(SE_188);
      Map_SetShake(-1, -1, 0xe666);
      Engine_MapRenderWaitForValues();
      gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
      Engine_EventRequestExit(
          VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_EAST);
      Engine_EventEnd();
    }
  }
}

/// In VenusLighthouseSecondFloor, Move on the pillar before the gate: in its
/// socket it sinks in and the gate opens, the first time only
/// (FLAG_VENUS_LIGHTHOUSE_GATE_OPEN).
static void Event_MoveGatePillar(void) {
  u8 *flags;
  s32 row;
  s32 column;
  s32 gateOpened;
  struct FieldActor *leader;
  struct FieldActor *pillar;
  struct FieldActor *pillarActor;
  struct FieldActor *occlusionEffect;

  occlusionEffect = 0;
  pillar = Object_GetById(ACTOR_GATE_PILLAR);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Map_CopyCellAttributes(109, 43, 7, 5, 45, 43);
  flags = &pillar->priorityFlags;
  if ((2 & pillar->priorityFlags) != 0) {
    Map_CopyCellAttributes(45, 45, 1, 1, 46, 45);
  } else {
    Map_CopyCellAttributes(48, 42, 1, 1, pillar->x.fixed >> 20,
                           pillar->z.fixed >> 20);
  }
  column = pillar->x.fixed >> 20;
  if (column != 46) {
  } else {
    row = pillar->z.fixed >> 20;
    if (row != 45) {
    } else {
      // Sink an unsolved pillar only when both of its target cell coordinates
      // match.
      gateOpened = Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_GATE_OPEN);
      if (gateOpened != 0) {
      } else {
        if ((leader->z.fixed >> 20) <= 45) {
          occlusionEffect = VenusLighthouse_CreateOccludingPassageEffect(
              0x2e80000, 0, 0x2c00000, 20);
          Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
        }
        pillarActor = Object_GetById(ACTOR_GATE_PILLAR);
        Engine_ActorSetSpriteFlags(pillarActor, 0);
        pillar->terrainId = gateOpened;
        pillar->motionFlags = 3;
        pillar->gravity = 0x1999;
        pillar->bounce = gateOpened;
        Map_CopyCellAttributes(43, 45, 1, 1, column, row);
        OverlayObject_WaitUntilIdle(pillar);
        Engine_AudioPlayCue(SE_188);
        pillar->motionFlags = gateOpened;
        pillar->y.fixed = -PIXELS(16);
        Engine_ActorSetSpritePriority(ACTOR_GATE_PILLAR, 3);
        {
          u8 two = 2;

          *flags = two;
        }
        Map_CopyCellAttributes(45, 45, 1, 1, column, row);
        Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |=
            ACTOR_PRIORITY_AUTOMATIC;
        Engine_ObjectDispatchRelease(occlusionEffect);
        pillar->collisionFlags = gateOpened;
        *flags |= 2;
        Object_GetById(ACTOR_GATE_EFFECT)->collisionFlags = gateOpened;
        Object_GetById(ACTOR_GATE_EFFECT)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetPosition(ACTOR_GATE_EFFECT, PIXELS(808), PIXELS(728));
        Actor_SetActionScript(ACTOR_GATE_EFFECT, gVenusPushScript);
        Object_RefreshSelectorById(ACTOR_GATE_EFFECT);
        Engine_EventWait(30);
        Engine_AudioPlayCue(SE_DOOR_OPEN);
        Engine_MapAnimateCells(gVenusPushCells, 110, 41);
        Map_CopyCellAttributes(46, 41, 1, 1, column, 42);
        Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_GATE_OPEN);
      }
    }
  }
  Engine_EventEnd();
}

/// In VenusLighthouseSecondFloor, walking into the gap or the pillar before the
/// gate: jump the gap, else push the pillar, which may sink into its socket.
static void Event_JumpOrPushGatePillar(void) {

  Engine_EventBegin();
  if (Event_JumpAcrossGap() == 0) {
    Map_CopyCellAttributes(109, 43, 7, 5, 45, 43);
    VenusLighthouse_PushAdjacentPillar();
  }
  Engine_EventEnd();
  Event_MoveGatePillar();
}

/// In VenusLighthouseThirdFloor, Move on one of the three pillars: a pillar on
/// a free socket sinks in, and with all three sunk the gate opens
/// (FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN).
static void Event_MoveSwitchPillar(void) {
  struct FieldActor *occlusionEffect = 0;
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldActor *pillar;
  struct FieldActor *other;
  struct FieldActor *firstGateEffect;
  struct FieldActor *secondGateEffect;
  u8 *flags;
  u32 pillarId;
  u32 candidateIndex;
  u32 switchIndex;
  s32 savedPriority;

  Engine_EventBegin();
  Map_CopyCellAttributes(108, 39, 13, 7, 44, 39);
  // Find pillars on unoccupied switch cells before committing any sinking
  // motion.
  for (pillarId = ACTOR_FIRST_SWITCH_PILLAR;
       pillarId <= ACTOR_FIRST_SWITCH_PILLAR + 2; pillarId++) {
    pillar = Object_GetById(pillarId);
    flags = &pillar->priorityFlags;
    if (*flags != 2) {
      Map_CopyCellAttributes(47, 39, 1, 1, pillar->x.fixed >> 20,
                             pillar->z.fixed >> 20);
    } else {
      Map_CopyCellAttributes(46, 39, 1, 1, pillar->x.fixed >> 20,
                             pillar->z.fixed >> 20);
    }
    switchIndex = 5;
    for (candidateIndex = 0; candidateIndex <= 3; candidateIndex++) {
      if (pillar->x.fixed >> 20 == gVenusSwitchCells[candidateIndex].x &&
          pillar->z.fixed >> 20 == gVenusSwitchCells[candidateIndex].z &&
          pillar->y.fixed >= 0) {
        switchIndex = candidateIndex;
        break;
      }
    }
    if (switchIndex == 5) {
      continue;
    }
    for (candidateIndex = ACTOR_FIRST_SWITCH_PILLAR;
         candidateIndex <= ACTOR_FIRST_SWITCH_PILLAR + 2; candidateIndex++) {
      other = Object_GetById(candidateIndex);
      if (pillarId != candidateIndex &&
          pillar->x.fixed >> 20 == other->x.fixed >> 20 &&
          pillar->z.fixed >> 20 == other->z.fixed >> 20) {
        switchIndex = 5;
        break;
      }
    }
    if (switchIndex == 5) {
      continue;
    }
    // Preserve overlapping actors while the selected pillar sinks into its
    // socket.
    savedPriority = leader->sprite->image.priority;
    if (leader->z.fixed >> 20 <= gVenusSwitchCells[switchIndex].z) {
      occlusionEffect = VenusLighthouse_CreateOccludingPassageEffect(
          pillar->x.fixed, pillar->y.fixed, pillar->z.fixed - 0x40000, 20);
      Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
    }
    Engine_ActorSetSpriteFlags(Object_GetById(pillarId), 0);
    pillar->terrainId = 0;
    pillar->motionFlags = 3;
    pillar->bounce = 0;
    pillar->gravity = 0x1999;
    Map_CopyCellAttributes(42, 41, 1, 1, gVenusSwitchCells[switchIndex].x,
                           gVenusSwitchCells[switchIndex].z);
    OverlayObject_WaitUntilIdle(pillar);
    Engine_AudioPlayCue(SE_188);
    pillar->collisionFlags = 0;
    pillar->motionFlags = 0;
    pillar->y.fixed = -PIXELS(16);
    Engine_ActorSetSpritePriority(pillarId, 3);
    {
      u8 two = 2;

      *flags = two;
    }
    Map_CopyCellAttributes(46, 39, 1, 1, gVenusSwitchCells[switchIndex].x,
                           gVenusSwitchCells[switchIndex].z);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, savedPriority);
    Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |=
        ACTOR_PRIORITY_AUTOMATIC;
    if (occlusionEffect != 0) {
      Engine_ObjectDispatchRelease(occlusionEffect);
    }
    if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN)) {
      Engine_EventEnd();
      return;
    }
    if ((Object_GetById(ACTOR_FIRST_SWITCH_PILLAR)->priorityFlags &
         Object_GetById(ACTOR_FIRST_SWITCH_PILLAR + 1)->priorityFlags &
         Object_GetById(ACTOR_FIRST_SWITCH_PILLAR + 2)->priorityFlags & 2) ==
        0) {
      continue;
    }
    // Once all three pillars settle, run the paired scripts that open the gate.
    firstGateEffect =
        VenusLighthouse_CreateScriptedGateEffect(888, 680, gVenusSettleScriptA);
    secondGateEffect =
        VenusLighthouse_CreateScriptedGateEffect(888, 680, gVenusSettleScriptB);
    // SC_FREE clears the script pointer; work value 0x28 signals the
    // opening cue earlier, while the gate effects are still running.
    while (((struct ScriptInterpreter *)firstGateEffect)->script != 0 ||
           ((struct ScriptInterpreter *)secondGateEffect)->script != 0) {
      if (firstGateEffect->riseEnabled != 0 ||
          secondGateEffect->riseEnabled != 0) {
        Engine_EventWait(30);
        Engine_AudioPlayCue(SE_DOOR_OPEN);
        Engine_MapAnimateCells(gVenusSettleCells, 109, 37);
        Map_CopyCellAttributes(45, 37, 1, 1, 45, 38);
        if (Object_GetById(ACTOR_FIRST_SWITCH_PILLAR)->x.fixed >> 20 ==
                gVenusSwitchCells[0].x &&
            Object_GetById(ACTOR_FIRST_SWITCH_PILLAR)->z.fixed >> 20 ==
                gVenusSwitchCells[0].z) {
          Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_SOCKETS_A);
        } else {
          Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_SOCKETS_B);
        }
        Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN);
        break;
      }
      Engine_TaskWait(1);
    }
  }
  Engine_EventEnd();
}

/// In VenusLighthouseThirdFloor, walking into the gap or one of the three
/// pillars: jump the gap, else push the pillar, which may sink into a socket.
static void Event_JumpOrPushSwitchPillar(void) {
  Engine_EventBegin();
  if (Event_JumpAcrossGap() == 0) {
    Map_CopyCellAttributes(108, 39, 13, 7, 44, 39);
    VenusLighthouse_PushAdjacentPillar();
  }
  Engine_EventEnd();
  Event_MoveSwitchPillar();
}

// Keep a stacked pillar sixteen pixels above its supporting actor.
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 7 bytes (extent 34 to 32 bytes). Keep the
// unused native result in matching builds.
#ifdef UBFIX
static void
VenusLighthouse_FollowSupportingPillarHeight(union FieldObject *fieldObject) {
#else
static s32
VenusLighthouse_FollowSupportingPillarHeight(union FieldObject *fieldObject) {
#endif
  struct FieldActor *pillar = (struct FieldActor *)fieldObject;
  struct FieldActor *support;

  support = Object_GetById(pillar->timer);
  pillar->y.fixed = support->y.fixed + PIXELS(16);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Moves the floating supports to the heights selected by their timers.
static void VenusLighthouse_LowerFloatingPillars(s32 waitFrames) {
  struct FieldActor *westSupport = Object_GetById(ACTOR_BALANCE_WEST);
  struct FieldActor *eastSupport = Object_GetById(ACTOR_BALANCE_EAST);
  u32 pillarIndex;

  Actor_SetSpeed(ACTOR_BALANCE_WEST, 0x8000, 0x4000);
  Engine_ActorSetSpeed(ACTOR_BALANCE_EAST, 0x8000, 0x4000);
  if (waitFrames != 0) {
    Engine_AudioPlayCue(SE_180);
  }
  FieldObject_SetPosition(westSupport, westSupport->x.fixed,
                          gVenusPillarHeights[westSupport->timer],
                          westSupport->z.fixed);
  FieldObject_SetPosition(eastSupport, eastSupport->x.fixed,
                          gVenusPillarHeights[eastSupport->timer],
                          eastSupport->z.fixed);
  Engine_ActorWaitForMove(ACTOR_BALANCE_WEST);
  Engine_ActorWaitForMove(ACTOR_BALANCE_EAST);
  westSupport->y.fixed = gVenusPillarHeights[westSupport->timer];
  eastSupport->y.fixed = gVenusPillarHeights[eastSupport->timer];
  if (waitFrames != 0) {
    Engine_AudioPlayCue(SE_289);
  }
  for (pillarIndex = 0; pillarIndex < 5; pillarIndex++) {
    struct FieldActor *actor = Object_GetById(pillarIndex + ACTOR_BALANCE_WEST);

    if (actor->y.fixed / 0x10000 < 0 && actor->y.fixed / 0x10000 > -30) {
      Map_CopyCellAttributes(4, 19, 1, 1, actor->x.fixed >> 20,
                             actor->z.fixed >> 20);
    }
  }
  Engine_EventWait(waitFrames);
}

/// Moves a floating pillar atop the highest support in its map cell.
static void VenusLighthouse_StackFloatingPillar(s32 pillarId) {

  struct FieldActor *candidate;
  s32 supportHeight = 0xffb00000;
  struct FieldActor *pillar = 0;
  u32 candidateIndex;

  for (candidateIndex = 0; candidateIndex <= 5; candidateIndex++) {
    s32 candidateId = candidateIndex + ACTOR_BALANCE_WEST;
    s32 candidateTop;

    if (candidateId == pillarId) {
      continue;
    }

    candidate = Object_GetById(candidateId);
    pillar = Object_GetById(pillarId);

    if ((candidate->x.fixed >> 20) != (pillar->x.fixed >> 20) ||
        (candidate->z.fixed >> 20) != (pillar->z.fixed >> 20)) {
      continue;
    }

    candidateTop = candidate->y.fixed + PIXELS(16);
    if (supportHeight <= candidateTop) {
      pillar->timer = candidateId;
      supportHeight = candidateTop;
    }
  }

  Engine_ActorSetSpeed(pillarId, 0x40000, 0x20000);
  Engine_ObjectSetPosition(pillar, pillar->x.fixed, supportHeight,
                           pillar->z.fixed);
  Engine_ActorWaitForMove(pillarId);
  Engine_AudioPlayCue(SE_188);
  SceneEffect_SpawnNineRadialEffects(pillarId);
  Engine_EventWait(30);
}

/// Stops a floating pillar and resets its vertical motion and timer.
static __inline__ void FloatingPillar_Settle(struct FieldActor *pillar,
                                             s32 resetValue) {
  pillar->targetY = ACTOR_NO_TARGET;
  pillar->terrainHeight = resetValue;
  pillar->velocityY = resetValue;
  pillar->motionFlags = resetValue;
  pillar->timer = resetValue;
}

/// Copies all three fixed-point position components without changing motion.
static __inline__ void Actor_CopyPosition(struct FieldActor *destination,
                                          struct FieldActor *source) {
  destination->x.fixed = source->x.fixed;
  destination->y.fixed = source->y.fixed;
  destination->z.fixed = source->z.fixed;
}

/// In VenusLighthouseFourthFloor, Carry on one of the four floating pillars: it
/// settles where it is set down (FLAG_SCENE_TEMP_00 plus its number) and the
/// platforms under the pillars rise or sink to their weight.
static void Event_CarryFloatingPillar(void) {
  struct FieldActor savedPosition;
  struct FieldActor *firstPassageEffect = NULL;
  struct FieldActor *secondPassageEffect = NULL;
  struct FieldActor *pillar;
  struct FieldActor *other;
  s32 pillarIndex;
  s32 earlierIndex;
  s32 settledIndex;
  s32 column;
  s32 row;

  Engine_EventBegin();
  // Handle completed destinations and fallen pillars before reordering a new
  // stack.
  for (pillarIndex = 0; pillarIndex < 4; pillarIndex++) {
    pillar = Object_GetById(pillarIndex + ACTOR_FIRST_FLOATING_PILLAR);
    column = pillar->x.fixed >> 20;
    if (column == 13 && (row = pillar->z.fixed >> 20) == 7 &&
        !GameFlag_IsSet(FLAG_SCENE_TEMP_00 + pillarIndex)) {
      OverlayObject_WaitUntilIdle(pillar);
      GameFlag_Set(FLAG_SCENE_TEMP_00 + pillarIndex);
      pillar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      pillar->collisionFlags = 0;
      pillar->motionFlags = 0;
      Map_CopyCellAttributes(4, 19, 1, 1, column, row);
      break;
    }
    if (pillar->sprite->image.priority == 3 &&
        !GameFlag_IsSet(FLAG_SCENE_TEMP_00 + pillarIndex)) {
      /* FAKEMATCH(temporary): the zero in the search counter, set before the
       * priority call; as block->bounce = 0 after the call, 2 of 272
       * instructions differ in tbs-en. */
      earlierIndex = 0;
      Engine_ActorSetSpritePriority(pillarIndex + ACTOR_FIRST_FLOATING_PILLAR,
                                    1);
      pillar->bounce = earlierIndex;
      if (pillar->z.fixed >> 20 <= 12) {
        firstPassageEffect = VenusLighthouse_CreateBlendedPassageEffect(
            pillar->x.fixed, 0, 0xe00000, 253);
        secondPassageEffect = VenusLighthouse_CreateBlendedPassageEffect(
            pillar->x.fixed, 0, 0xf00000, 253);
      }
      OverlayObject_WaitUntilIdle(pillar);
      Engine_ActorSetPosition(pillarIndex + ACTOR_FIRST_FLOATING_PILLAR, 0, 0);
      Engine_ObjectDispatchRelease(firstPassageEffect);
      Engine_ObjectDispatchRelease(secondPassageEffect);
      GameFlag_Set(FLAG_SCENE_TEMP_00 + pillarIndex);
      break;
    }
    if (pillar->z.fixed >> 20 == 19 &&
        !GameFlag_IsSet(FLAG_SCENE_TEMP_00 + pillarIndex)) {
      // Keep settled pillars at the front of the actor slots before finding
      // their support.
      FloatingPillar_Settle(pillar, 0);
      settledIndex = pillarIndex;
      for (earlierIndex = 0; earlierIndex < pillarIndex; earlierIndex++) {
        if (!GameFlag_IsSet(FLAG_SCENE_TEMP_00 + earlierIndex)) {
          other = Object_GetById(earlierIndex + ACTOR_FIRST_FLOATING_PILLAR);
          Actor_CopyPosition(&savedPosition, pillar);
          Actor_CopyPosition(pillar, other);
          Actor_CopyPosition(other, &savedPosition);
          settledIndex = earlierIndex;
          break;
        }
      }
      other = Object_GetById(settledIndex + ACTOR_FIRST_FLOATING_PILLAR);
      FloatingPillar_Settle(other, 0);
      Engine_CameraSetSpeed(0x30000, 0x6000);
      Camera_GetEventObject()->motionFlags = 0;
      Engine_CameraMoveTo(PIXELS(136), PIXELS(8), PIXELS(344), 1);
      Engine_CameraWaitForMove();
      // Stack the pillar, then rebalance the two supports and attach the height
      // follower.
      VenusLighthouse_StackFloatingPillar(settledIndex +
                                          ACTOR_FIRST_FLOATING_PILLAR);
      if (Object_GetById(settledIndex + ACTOR_FIRST_FLOATING_PILLAR)->x.fixed >>
              20 ==
          6) {
        Object_GetById(ACTOR_BALANCE_WEST)->timer++;
        Object_GetById(ACTOR_BALANCE_EAST)->timer--;
      } else {
        Object_GetById(ACTOR_BALANCE_WEST)->timer--;
        Object_GetById(ACTOR_BALANCE_EAST)->timer++;
      }
#ifdef UBFIX
      Object_GetById(settledIndex + ACTOR_FIRST_FLOATING_PILLAR)->update =
          VenusLighthouse_FollowSupportingPillarHeight;
#else
      Object_GetById(settledIndex + ACTOR_FIRST_FLOATING_PILLAR)->update =
          (void (*)(
              union FieldObject *))VenusLighthouse_FollowSupportingPillarHeight;
#endif
      VenusLighthouse_LowerFloatingPillars(40);
      Object_GetById(settledIndex + ACTOR_FIRST_FLOATING_PILLAR)
          ->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      GameFlag_Set(FLAG_SCENE_TEMP_00 + settledIndex);
      break;
    }
  }
  Engine_EventEnd();
}

/// In VenusLighthouseFourthFloor, pushing a pillar (triggers 35 to 37).
static void Event_PushPuzzlePillar(void) {

  Engine_EventBegin();
  VenusLighthouse_PushAdjacentPillar();
  Engine_EventEnd();
}

/// In VenusLighthouseFourthFloor, stepping on the west floor switch: the west
/// wall slides aside, the first time only
/// (FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED), and the scene cuts to
/// VenusLighthouseThirdFloor by
/// VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST,
/// where VenusLighthouse_AnimateWestTileWave shows what moved there before
/// bringing the party back.
static void Event_StepOnWestWallSwitch(void) {
  struct EffectOptions options;
  struct MapLayerScroll *scrollLayer;
  struct FieldActor *leader;
  s32 column;
  s32 leaderZ;
  s32 tileDelay;
  u32 frame;
  s32 dustX;

  scrollLayer = &((struct MapScrollWork *)gMapWork[0])->layers[2];
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  column = leader->x.part.pixel;
  leaderZ = leader->z.part.pixel;
  leader->y.fixed = 0;
  if (column >= 532 && column <= 539 && leaderZ >= 324 && leaderZ < 332) {
    leader->y.fixed = -PIXELS(2);
    if (!Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED)) {
      Engine_EventBegin();
      Map_CopyCellsTo(63, 29, 33, 20, 1, 1);
      Engine_AudioPlayCue(SE_161);
      Map_CopyCellsTo(44, 83, 44, 80, 3, 3);
      Engine_EventWait(30);
      Map_SetShake(0x10000, 0x10000, 0x10000);
      Engine_AudioPlayCue(SE_239);
      Engine_EventWait(20);
      dustX = PIXELS(616);
      column = 0;
      tileDelay = 60;
      // Move the west wall while its visible edge exposes successive tile
      // strips.
      for (frame = 0; frame <= 319; frame++, tileDelay--) {
        scrollLayer->offsetX += 0x3333;
        if (dustX >= 0x2400000 && frame > 40) {
          dustX += -0x3333;
          options.priority = 2;
          options.startScaleX =
              ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
          options.startScaleY =
              ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
          options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
          Effect_Spawn(dustX, 0, 0x1200000, 0, -(((gFrameCount & 1) * 3) << 16),
                       0, 0x8a0000, &options);
          if (tileDelay == 0) {
            tileDelay = 40;
            column += 4;
            Map_CopyCellsTo(column, 56, 36, 17, 3, 4);
          }
        }
        Engine_TaskWait(1);
      }
      scrollLayer->offsetX += 0x8000;
      scrollLayer->offsetX = scrollLayer->offsetX / 0x10000 << 16;
      Engine_AudioPlayCue(SE_288);
      Engine_AudioPlayCue(SE_188);
      Map_SetShake(-1, -1, 0xe666);
      Engine_MapRenderWaitForValues();
      gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
      Engine_EventRequestExit(
          VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST);
      Engine_EventEnd();
    }
  }
}

/// In VenusLighthouseFourthFloor, stepping on the east floor switch: the east
/// wall slides aside, the first time only
/// (FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED), and the scene cuts to
/// VenusLighthouseThirdFloor by
/// VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST,
/// where VenusLighthouse_AnimateEastTileWave shows what moved there before
/// bringing the party back.
static void Event_StepOnEastWallSwitch(void) {
  struct EffectOptions options;
  struct MapLayerScroll *scrollLayer;
  struct FieldActor *leader;
  s32 column;
  s32 leaderZ;
  s32 tileDelay;
  u32 frame;
  s32 dustX;

  scrollLayer = &((struct MapScrollWork *)gMapWork[0])->layers[2];
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  column = leader->x.part.pixel;
  leaderZ = leader->z.part.pixel;
  leader->y.fixed = 0;
  if (column >= 788 && column <= 795 && leaderZ >= 324 && leaderZ < 332) {
    leader->y.fixed = -PIXELS(2);
    if (!Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED)) {
      Engine_EventBegin();
      Map_CopyCellsTo(63, 29, 49, 20, 1, 1);
      Engine_AudioPlayCue(SE_161);
      Engine_EventWait(30);
      Map_SetShake(0x10000, 0x10000, 0x10000);
      Engine_AudioPlayCue(SE_239);
      Engine_EventWait(20);
      dustX = PIXELS(704);
      column = 61;
      tileDelay = 60;
      // Move the east wall in the opposite direction, keeping its distinct dust
      // limits.
      for (frame = 0; frame <= 319; frame++, tileDelay--) {
        scrollLayer->offsetX += -0x3333;
        dustX += 0x3333;
        if (dustX >= 0x2c80000 && dustX < 0x2f00000) {
          options.priority = 2;
          options.startScaleX =
              ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
          options.startScaleY =
              ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
          options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
          Effect_Spawn(dustX, 0, 0x1200000, 0, -(((gFrameCount & 1) * 3) << 16),
                       0, 0x8a0000, &options);
          if (tileDelay == 0) {
            tileDelay = 40;
            column -= 4;
            Map_CopyCellsTo(column, 56, 44, 17, 3, 4);
          }
        }
        Engine_TaskWait(1);
      }
      scrollLayer->offsetX += 0x8000;
      scrollLayer->offsetX = scrollLayer->offsetX / 0x10000 << 16;
      Engine_AudioPlayCue(SE_288);
      Engine_AudioPlayCue(SE_188);
      Map_SetShake(-1, -1, 0xe666);
      Engine_MapRenderWaitForValues();
      gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
      Engine_EventRequestExit(
          VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST);
      Engine_EventEnd();
    }
  }
}

/// In VenusLighthouseFourthFloor, Move on one of the four pillars in the pillar
/// room: on a free socket it sinks in, and with all four sunk the gate opens
/// (FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN).
static void Event_MoveSocketPillar(void) {
  struct FieldActor *occlusionEffect;
  struct FieldActor *pillar;
  struct FieldActor *otherPillar;
  struct FieldActor *leader;
  struct FieldActor *firstPillar;
  struct FieldActor *secondPillar;
  struct FieldActor *thirdPillar;
  struct FieldActor *fourthPillar;
  u8 *flags;
  u8 *motionFlags;
  u32 pillarId;
  u32 candidateIndex;
  u32 switchIndex;
  u32 savedPriority;

  occlusionEffect = 0;
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Event_Begin();
#if TBS_PATCH_3
  Battle_ResetEffectCounter();
#endif
  Map_CopyCellAttributes(69, 48, 4, 2, 5, 48);
  Map_CopyCellAttributes(73, 37, 9, 13, 9, 37);
  // Find a pillar on a clear pillar socket and preserve overlapping draw order.
  for (pillarId = ACTOR_FIRST_SOCKET_PILLAR;
       pillarId <= ACTOR_FIRST_SOCKET_PILLAR + 3; pillarId++) {
    pillar = Object_GetById(pillarId);
    flags = &pillar->priorityFlags;
    if (*flags != 2)
      Map_CopyCellAttributes(72, 48, 1, 1, pillar->x.fixed >> 20,
                             pillar->z.fixed >> 20);
    else
      Map_CopyCellAttributes(73, 48, 1, 1, pillar->x.fixed >> 20,
                             pillar->z.fixed >> 20);

    switchIndex = 8;
    for (candidateIndex = 0; candidateIndex < 8; candidateIndex++) {
      if ((pillar->x.fixed >> 20) == gVenusPillarCells[candidateIndex].x &&
          (pillar->z.fixed >> 20) == gVenusPillarCells[candidateIndex].z &&
          pillar->y.fixed >= 0) {
        switchIndex = candidateIndex;
        break;
      }
    }
    if (switchIndex == 8)
      continue;
    for (candidateIndex = ACTOR_FIRST_SOCKET_PILLAR;
         candidateIndex <= ACTOR_FIRST_SOCKET_PILLAR + 3; candidateIndex++) {
      otherPillar = Object_GetById(candidateIndex);
      if (pillarId != candidateIndex &&
          (pillar->x.fixed >> 20) == (otherPillar->x.fixed >> 20) &&
          (pillar->z.fixed >> 20) == (otherPillar->z.fixed >> 20)) {
        switchIndex = 8;
        break;
      }
    }
    if (switchIndex == 8)
      continue;

    savedPriority = leader->sprite->image.priority;
    if ((leader->z.fixed >> 20) <= gVenusPillarCells[switchIndex].z) {
      occlusionEffect = VenusLighthouse_CreateOccludingPassageEffect(
          pillar->x.fixed, pillar->y.fixed, pillar->z.fixed - 0x40000, 20);
      Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
    }
    for (candidateIndex = ACTOR_FIRST_SOCKET_PILLAR;
         candidateIndex <= ACTOR_FIRST_SOCKET_PILLAR + 3; candidateIndex++) {
      otherPillar = Object_GetById(candidateIndex);
      if (pillarId != candidateIndex &&
          (pillar->x.fixed >> 20) == (otherPillar->x.fixed >> 20) &&
          (pillar->z.fixed >> 20) - 1 == (otherPillar->z.fixed >> 20))
        Actor_SetSpritePriority(candidateIndex, 3);
    }
    Actor_SetSpriteFlags(Object_GetById(pillarId), 0);
    pillar->terrainId = 0;
    motionFlags = &pillar->motionFlags;
    *motionFlags = 3;
    ((union FieldObject *)pillar)->effect.velocity.y = 0x1999;
    ((union FieldObject *)pillar)->effect.velocity.x = 0;
    Map_CopyCellAttributes(6, 44, 1, 1, gVenusPillarCells[switchIndex].x,
                           gVenusPillarCells[switchIndex].z);
    OverlayObject_WaitUntilIdle(pillar);
    Event_PlayCue(SE_188);
    pillar->collisionFlags = 0;
    *motionFlags = 0;
    pillar->y.fixed = -PIXELS(16);
    Actor_SetSpritePriority(pillarId, 3);
    *flags = 2;
    Map_CopyCellAttributes(73, 48, 1, 1, gVenusPillarCells[switchIndex].x,
                           gVenusPillarCells[switchIndex].z);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, savedPriority);
    Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |=
        ACTOR_PRIORITY_AUTOMATIC;
    for (candidateIndex = ACTOR_FIRST_SOCKET_PILLAR;
         candidateIndex <= ACTOR_FIRST_SOCKET_PILLAR + 3; candidateIndex++) {
      otherPillar = Object_GetById(candidateIndex);
      if (pillarId != candidateIndex &&
          (pillar->x.fixed >> 20) == (otherPillar->x.fixed >> 20) &&
          (pillar->z.fixed >> 20) - 1 == (otherPillar->z.fixed >> 20)) {
        Actor_SetSpritePriority(candidateIndex, 1);
        Object_GetById(candidateIndex)->priorityFlags |=
            ACTOR_PRIORITY_AUTOMATIC;
      }
    }
    Engine_ObjectDispatchRelease(occlusionEffect);
    if (GameFlag_IsSet(FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN)) {
      Event_End();
      return;
    }
    // Open the paired gate only after all four pillars are seated.
    firstPillar = Actor_Get(ACTOR_FIRST_SOCKET_PILLAR);
    secondPillar = Actor_Get(ACTOR_FIRST_SOCKET_PILLAR + 1);
    thirdPillar = Actor_Get(ACTOR_FIRST_SOCKET_PILLAR + 2);
    fourthPillar = Object_GetById(ACTOR_FIRST_SOCKET_PILLAR + 3);
    if ((firstPillar->priorityFlags & secondPillar->priorityFlags &
         thirdPillar->priorityFlags & fourthPillar->priorityFlags) &
        2) {
      struct FieldActor *firstGateEffect;
      struct FieldActor *secondGateEffect;

      Camera_SetSpeed(0x10000, 0x2000);
      Object_PlaceCurrentWithinCameraBounds(ACTOR_SOCKET_EFFECT_STATUE, 1);
      Camera_WaitForMove();
      firstGateEffect = VenusLighthouse_CreateScriptedGateEffect(
          136, 776, gVenusFirstGateScript);
      Event_Wait(30);
      Camera_SetSpeed(0x6666, 0xccc);
      Camera_MoveTo(PIXELS(216), -1, PIXELS(632), 1);
      ObjectDispatch_WaitForValue16(firstGateEffect);
      Object_SetScript(firstGateEffect, gVenusFirstGateOpenScript);
      secondGateEffect = VenusLighthouse_CreateScriptedGateEffect(
          216, 760, gVenusSecondGateScript);
      while (((struct ScriptInterpreter *)firstGateEffect)->script != 0 ||
             ((struct ScriptInterpreter *)secondGateEffect)->script != 0) {
        if (firstGateEffect->riseEnabled != 0 ||
            secondGateEffect->riseEnabled != 0) {
          Event_Wait(30);
          Map_AnimateCells(gVenusGateCells, 77, 35);
          Map_CopyCellAttributes(13, 35, 1, 1, 13, 36);
          GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN);
          break;
        }
        Task_Wait(1);
      }
    }
  }
  Event_End();
}

/// In VenusLighthouseFourthFloor, walking into the gap or a pillar-room pillar:
/// jump the gap, else push the pillar, which may sink into a socket.
static void Event_JumpOrPushSocketPillar(void) {

  Engine_EventBegin();
  if (Event_JumpAcrossGap() == 0) {
    {
      Map_CopyCellAttributes(69, 48, 4, 2, 5, 48);
    }
    {
      Map_CopyCellAttributes(73, 37, 9, 13, 9, 37);
    }
    VenusLighthouse_PushAdjacentPillar();
  }
  Engine_EventEnd();
  Event_MoveSocketPillar();
}

/// In VenusLighthouseFourthFloor, walking south into the passage (trigger 18):
/// the leader rises out of the room to the floor above (
/// VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_TO_WALL_SWITCHES_RISING, where
/// VenusLighthouse_RaiseLeaderWithSpray lands him on ACTOR_RISING_PLATFORM).
static void Event_EnterRisingPassage(void) {
  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x208, 0x2c8);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
  (void)VenusLighthouse_CreateBlendedPassageEffect(PIXELS(520), 0, PIXELS(784),
                                                   223);
  FieldBattleFx_RunRisingObjectSequence(0, 6, 0);
  Engine_EventWait(60);
  Engine_EventRequestExit(
      VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_TO_WALL_SWITCHES_RISING);
  Engine_EventEnd();
}

/// Selects the puzzle interactions and dialogue for the current lighthouse
/// scene.
const struct SceneEvent *Scene_GetEvents(void) {
  s32 scene = gGameState.scene;

  if (scene == (s32)&SceneId_VenusLighthouseEntry) {
    return gVenusLighthouseEntryEvents;
  }
  if (scene == (s32)&SceneId_VenusLighthouseGroundFloor) {
    return gVenusLighthouseGroundFloorEvents;
  }
  if (scene == (s32)&SceneId_VenusLighthouseFirstFloor) {
    return gVenusLighthouseFirstFloorEvents;
  }
  if (scene == (s32)&SceneId_VenusLighthouseSecondFloor) {
    return gVenusLighthouseSecondFloorEvents;
  }
  if (scene == (s32)&SceneId_VenusLighthouseThirdFloor) {
    return gVenusLighthouseThirdFloorEvents;
  }
  if (scene == (s32)&SceneId_VenusLighthouseFourthFloor) {
    return gVenusLighthouseFourthFloorEvents;
  }
  return gVenusLighthouseGroundFloorEvents;
}

/// Prepares a stationary actor with the room's initial collision and draw
/// state.
static void VenusLighthouse_PrepareStandingActor(s32 actorId) {
  struct FieldActor *actor;
  s32 collisionFlags;

  actor = Object_GetById(actorId);
  actor->motionFlags = 0;
  collisionFlags = 252;
  collisionFlags &= actor->collisionFlags;
  actor->collisionFlags = collisionFlags;
  Engine_ActorSetSpriteFlags(actor, 0);
  Object_SetMode(actor, 5);
  Engine_ActorSetSpritePriority(actorId, 3);
  {
    s32 priorityFlags = 2;
    priorityFlags |= actor->priorityFlags;
    actor->priorityFlags = priorityFlags;
  }
}

/// Visits the four pillar actors through the object lookup service.
static void VenusLighthouse_VisitPillarActors(void) {
  u32 actorId;

  actorId = ACTOR_FIRST_SOCKET_PILLAR;
  do {
    Object_GetById(actorId);
    actorId++;
  } while (actorId <= ACTOR_FIRST_SOCKET_PILLAR + 3);
}

/// Restores puzzle progress, actors and entrance effects for all six scenes,
/// by scene and entrance; on every arrival it sets FLAG_VENUS_LIGHTHOUSE_LIT
/// and, after a battle, restores the scripted actors' child values.
/// VenusLighthouseGroundFloor's
/// VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR, with
/// VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_PILLARS and
/// _FROM_GREAT_STATUE into the same room, and VenusLighthouseFirstFloor's
/// VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_GROUND_FLOOR clear
/// FLAG_SHOW_LOCATION_NAME.
s32 Scene_RunEntrySetup(void) {
  struct FieldActor *actor;
  s32 scene;
  u32 pillarIndex;
  s32 column;
  s32 row;

  Engine_EventWait(1);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION))
    VenusLighthouse_RestoreScriptedChildValues();
  Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_LIT);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  scene = gGameState.scene;
  // Restore the scholars and soldiers outside and each interior puzzle by its
  // entrance.
  if (scene == (s32)&SceneId_VenusLighthouseEntry) {
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    if (!Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD))
      VenusLighthouse_PrepareStandingActor(ACTOR_ENTRANCE_MAN);
    else
      Map_CopyCellAttributes(7, 17, 2, 1, 7, 16);
    VenusLighthouse_PrepareStandingActor(ACTOR_SCHOLAR_LIGHTHOUSE_OPEN);
    VenusLighthouse_PrepareStandingActor(ACTOR_SCHOLAR_BARBARIANS);
    VenusLighthouse_PrepareStandingActor(ACTOR_WAITING_SCHOLAR);
    Engine_ActorSetSpritePriority(ACTOR_WAITING_SCHOLAR, 2);
    VenusLighthouse_PrepareStandingActor(ACTOR_SOLDIER_OPENED_DOOR);
    Engine_ActorSetSpritePriority(ACTOR_SOLDIER_OPENED_DOOR, 2);
    VenusLighthouse_PrepareStandingActor(ACTOR_SOLDIER_HEADED_FOR_BABI);
    VenusLighthouse_PrepareStandingActor(ACTOR_SOLDIER_COLOSSO_VICTORS);
  } else if (scene == (s32)&SceneId_VenusLighthouseGroundFloor) {
    switch (gGameState.entrance) {
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_ENTRY:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_FRONT_HALL_FROM_RELIEFS:
      VenusLighthouse_PrepareStandingActor(ACTOR_SCHOLAR_ATTACKED_ANYWAY);
      break;
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_CARRY_STONE_WEST:
      Engine_GameFlagClear(FLAG_EXIT_ARRIVAL_PENDING);
      // The other scholar setup still applies.
      // fall through
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_FRONT_HALL:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_CARRY_STONE_EAST:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_PILLARS:
      VenusLighthouse_PrepareStandingActor(ACTOR_SCHOLAR_NOT_THE_ENTRANCE);
      break;
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_FROM_FLOOR_SWITCH:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_FROM_STAIRWELL:
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH)) {
        Map_CopyCellsTo(103, 27, 89, 27, 7, 8);
        Map_CopyCellsTo(41, 90, 27, 92, 3, 2);
        Map_CopyCellsTo(41, 90, 29, 93, 3, 2);
        Map_CopyCellsTo(41, 90, 27, 94, 3, 2);
        Map_CopyCellsTo(41, 90, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 90, 29, 97, 3, 2);
        Map_CopyCellsTo(41, 96, 25, 91, 3, 2);
        Map_CopyCellsTo(41, 92, 25, 93, 3, 2);
        Map_CopyCellsTo(41, 96, 25, 95, 3, 2);
        Map_CopyCellsTo(41, 96, 25, 97, 3, 2);
        Map_CopyCellsTo(41, 96, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 96, 29, 97, 3, 2);
      } else {
        if (!Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH))
          break;
        Map_CopyCellsTo(111, 27, 89, 27, 7, 8);
        Map_CopyCellsTo(41, 90, 25, 91, 3, 2);
        Map_CopyCellsTo(41, 90, 25, 93, 3, 2);
        Map_CopyCellsTo(41, 90, 25, 95, 3, 2);
        Map_CopyCellsTo(41, 90, 25, 97, 3, 2);
        Map_CopyCellsTo(41, 90, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 90, 29, 97, 3, 2);
        Map_CopyCellsTo(41, 94, 27, 92, 3, 2);
        Map_CopyCellsTo(41, 96, 29, 93, 3, 2);
        Map_CopyCellsTo(41, 94, 27, 94, 3, 2);
        Map_CopyCellsTo(41, 96, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 96, 29, 97, 3, 2);
      }
      break;
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_GREAT_STATUE:
      Engine_GameFlagClear(FLAG_EXIT_ARRIVAL_ALTERNATE);
      // Restore the same passage tiles as the room's other two entrances.
      // fall through
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_PILLARS:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR:
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00))
        Map_CopyCellsTo(44, 117, 41, 117, 3, 5);
      break;
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_RELIEFS:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_CARRY_STONE:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_FLOOR_SWITCH:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_STATUE_ROW_NORTH:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_STATUE_ROW_WEST:
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_PILLAR_CARRIED)) {
        Actor_SetPosition(ACTOR_WALL_PILLAR, PIXELS(872), PIXELS(88));
        actor = Object_GetById(ACTOR_WALL_PILLAR);
        actor->y.fixed = -PIXELS(24);
        actor->targetY = ACTOR_NO_TARGET;
      }
      break;
    }
  } else if (scene == (s32)&SceneId_VenusLighthouseFirstFloor) {
    // Restore depth pillars, the Carry puzzle and the paired tile-wave passage.
    switch (gGameState.entrance) {
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_GROUND_FLOOR:
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
      break;
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_EAST:
      VenusLighthouse_AnimatePairedTileWaves();
      break;
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_FROM_DRIFTING_FLOOR_HOLE:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION))
        VenusLighthouse_LandWithRadialDust();
      break;
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_FROM_DRIFTING_FLOOR:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_FROM_LADDERS:
      actor = Object_GetById(ACTOR_FIRST_CARRY_PILLAR);
      Event_MoveCarryPillar();
      if (actor->x.fixed >> 20 == 8)
        VenusLighthouse_FixPillarAtDestination(actor);
      actor = Object_GetById(ACTOR_FIRST_CARRY_PILLAR + 1);
      if (actor->x.fixed >> 20 == 7)
        VenusLighthouse_FixPillarAtDestination(actor);
      VenusLighthouse_CreateBlendedPassageEffect(PIXELS(206), 0, PIXELS(449),
                                                 223);
      VenusLighthouse_CreateBlendedPassageEffect(PIXELS(210), 0, PIXELS(449),
                                                 223);
      break;
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_PILLARS_FROM_LADDERS:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_WEST:
      BattleFx_StartFadeOverlay(0);
      Engine_TaskWait(2);
      actor = Object_GetById(ACTOR_FIRST_DEPTH_PILLAR);
      actor->motionFlags = 0;
      actor->update = SceneActor_SetFlagBitByRelativeDepth;
      actor = Object_GetById(ACTOR_FIRST_DEPTH_PILLAR + 1);
      actor->motionFlags = 0;
      actor->update = SceneActor_SetFlagBitByRelativeDepth;
      actor = Object_GetById(ACTOR_FIRST_DEPTH_PILLAR + 2);
      actor->motionFlags = 0;
      actor->update = SceneActor_SetFlagBitByRelativeDepth;
      Event_MoveDepthPillar();
      break;
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_GAP:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_NORTH:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_THUNDER_CROWN:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_PILLARS:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_PSYNERGY_STONE:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      BattleFx_StartFadeOverlay(0);
      Engine_TaskWait(2);
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED)) {
        Map_CopyCellsTo(111, 5, 117, 5, 5, 2);
        Map_CopyCellsTo(111, 10, 117, 10, 5, 2);
        Map_CopyCellsTo(111, 7, 111, 5, 5, 2);
        Map_CopyCellsTo(111, 7, 111, 10, 5, 2);
        Map_CopyCells(48, 3, 3, 10, 54, 3);
        Map_CopyCells(55, 26, 3, 10, 48, 3);
      }
      break;
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_LOWER_ROOM:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_GAP:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_THUNDER_CROWN:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_FROM_DRIFTING_FLOOR:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      break;
    }
  } else if (scene == (s32)&SceneId_VenusLighthouseSecondFloor) {
    // Restore the first sliding wall and its sunken-pillar gate.
    switch (gGameState.entrance) {
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_SPARKLING_PASSAGE_FROM_PILLAR_PIT:
      VenusLighthouse_ArriveThroughSparkPassage();
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      break;
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_EAST:
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR:
      BattleFx_StartFadeOverlay(0);
      break;
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_FROM_WALL_SWITCH:
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_FROM_PILLAR_PIT:
      actor = Object_GetById(ACTOR_GATE_EFFECT_STATUE);
      actor->motionFlags = 0;
      actor->y.fixed = 0;
      actor = Object_GetById(ACTOR_GATE_PILLAR);
      actor->motionFlags = 0;
      actor->collisionFlags = 0;
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_GATE_OPEN)) {
        Engine_TaskWait(1);
        Map_CopyCellsTo(124, 41, 110, 41, 1, 2);
        Map_CopyCellAttributes(46, 41, 1, 1, 46, 42);
        Actor_SetPosition(ACTOR_GATE_PILLAR, PIXELS(744), PIXELS(728));
        actor->motionFlags = 0;
        actor->y.fixed = -PIXELS(16);
        Engine_ActorSetSpritePriority(ACTOR_GATE_PILLAR, 3);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Map_CopyCellAttributes(45, 45, 1, 1, 46, 45);
        Engine_ActorSetAnimation(ACTOR_GATE_EFFECT, 7);
        Engine_ActorSetSpritePriority(ACTOR_GATE_EFFECT, 1);
        actor = Object_GetById(ACTOR_GATE_EFFECT);
        actor->collisionFlags = 0;
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetPosition(ACTOR_GATE_EFFECT, PIXELS(743), PIXELS(696));
#ifdef UBFIX
        actor->update = VenusLighthouse_EmitGateParticles;
#else
        actor->update =
            (void (*)(union FieldObject *))VenusLighthouse_EmitGateParticles;
#endif
      }
      Event_MoveGatePillar();
      break;
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH:
      gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
      Object_GetById(ACTOR_PARTY_LEADER)->y.fixed = -PIXELS(2);
      // These entrances share the tile-wave setup.
      // fall through
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_WEST:
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_WALL_SWITCH_FROM_GATE:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      DisplayBlend_SuspendScript();
      {
        s32 zero = 0;

        /* FAKEMATCH(barrier): the empty asm gives the zero the refs that put
         * it in r3 and the address in r2; as Io_Write16(0, &REG_BLDCNT), 3 of
         * 1431 instructions differ in tbs-en. */
        asm("" : "+l"(zero));
        Io_Write16(zero, &REG_BLDCNT);
      }
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED)) {
        Map_CopyCellsTo(15, 96, 9, 96, 3, 3);
        Map_CopyCellsTo(12, 96, 15, 96, 3, 3);
        Map_CopyCellsTo(5, 50, 15, 32, 3, 4);
        Map_CopyCellsTo(25, 45, 9, 32, 3, 4);
        Map_CopyCellAttributes(15, 32, 3, 1, 9, 32);
        Map_CopyCellAttributes(12, 32, 3, 1, 15, 32);
      }
      if (gGameState.entrance !=
          VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH)
        break;
      Engine_EventOpenScreen();
      Engine_EventWaitForScreen();
      gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
      break;
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_NORTH:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      break;
    }
  } else if (scene == (s32)&SceneId_VenusLighthouseThirdFloor) {
    // Restore the two tile waves and the three-pillar socket puzzle.
    switch (gGameState.entrance) {
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST:
      VenusLighthouse_AnimateWestTileWave();
      break;
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SPARKLING_PASSAGES:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHWEST:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SOUTHWEST_ROOM:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHEAST:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_SOUTHWEST:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_PILLAR_SOCKETS:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED)) {
        Map_CopyCells(53, 12, 3, 13, 26, 12);
        Map_CopyCellsTo(81, 41, 89, 14, 9, 2);
        Engine_TaskWait(1);
        FieldScheduler_AddCallback(VenusLighthouse_EmitWestPassageParticles,
                                   0xc80);
      }
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED)) {
        Map_CopyCells(58, 12, 3, 13, 34, 12);
        Map_CopyCellsTo(81, 41, 97, 14, 5, 2);
        Engine_TaskWait(1);
        FieldScheduler_AddCallback(VenusLighthouse_EmitEastPassageParticles,
                                   0xc80);
      }
      if (gGameState.entrance ==
          VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SOUTHWEST_ROOM)
        VenusLighthouse_ArriveThroughSparkPassage();
      else if (
          gGameState.entrance ==
          VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST)
        VenusLighthouse_AnimateEastTileWave();
      break;
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_NORTHWEST_ROOM:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_DRAGON_SCALES:
      /* FAKEMATCH(wrapper): called through a cast to an s32 function, so it
       * does not cross-jump with the second floor's same call; as a call by
       * its void prototype, 2 of 1431 instructions differ in tbs-en. */
      VENUS_LIGHTHOUSE_RUN_OPENING_SEQUENCE();
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      break;
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_FROM_DRIFTING_FLOOR:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_FROM_WEST_ROOM:
      actor = Object_GetById(ACTOR_SWITCH_EFFECT_STATUE);
      actor->motionFlags = 0;
      actor->y.fixed = 0;
      Object_GetById(ACTOR_FIRST_SWITCH_PILLAR)->motionFlags = 0;
      Object_GetById(ACTOR_FIRST_SWITCH_PILLAR + 1)->motionFlags = 0;
      Object_GetById(ACTOR_FIRST_SWITCH_PILLAR + 2)->motionFlags = 0;
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN)) {
        Engine_TaskWait(1);
        Map_CopyCellsTo(111, 59, 109, 37, 1, 2);
        Map_CopyCellAttributes(45, 37, 1, 1, 45, 38);
        if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_SOCKETS_A)) {
          Actor_SetPosition(ACTOR_FIRST_SWITCH_PILLAR, PIXELS(776),
                            PIXELS(664));
          Actor_SetPosition(ACTOR_FIRST_SWITCH_PILLAR + 1, PIXELS(840),
                            PIXELS(664));
          Actor_SetPosition(ACTOR_FIRST_SWITCH_PILLAR + 2, PIXELS(776),
                            PIXELS(696));
        } else {
          Actor_SetPosition(ACTOR_FIRST_SWITCH_PILLAR, PIXELS(840),
                            PIXELS(664));
          Actor_SetPosition(ACTOR_FIRST_SWITCH_PILLAR + 1, PIXELS(776),
                            PIXELS(696));
          Actor_SetPosition(ACTOR_FIRST_SWITCH_PILLAR + 2, PIXELS(840),
                            PIXELS(696));
        }
        Engine_ActorSetSpritePriority(ACTOR_FIRST_SWITCH_PILLAR, 3);
        actor = Object_GetById(ACTOR_FIRST_SWITCH_PILLAR);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetSpritePriority(ACTOR_FIRST_SWITCH_PILLAR + 1, 3);
        actor = Object_GetById(ACTOR_FIRST_SWITCH_PILLAR + 1);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetSpritePriority(ACTOR_FIRST_SWITCH_PILLAR + 2, 3);
        actor = Object_GetById(ACTOR_FIRST_SWITCH_PILLAR + 2);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetAnimation(ACTOR_SWITCH_GATE_EFFECT, 7);
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SWITCH_GATE_EFFECT), 0);
        Engine_ActorSetSpritePriority(ACTOR_SWITCH_GATE_EFFECT, 1);
        actor = Object_GetById(ACTOR_SWITCH_GATE_EFFECT);
        actor->collisionFlags = 0;
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetPosition(ACTOR_SWITCH_GATE_EFFECT, PIXELS(727), PIXELS(632));
#ifdef UBFIX
        actor->update = VenusLighthouse_EmitGateParticles;
#else
        actor->update =
            (void (*)(union FieldObject *))VenusLighthouse_EmitGateParticles;
#endif
      }
      Event_MoveSwitchPillar();
      break;
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_DRIFTING_FLOOR:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      break;
    }
  } else if (scene == (s32)&SceneId_VenusLighthouseFourthFloor) {
    // Restore floating supports, opposing walls and the four-pillar pillar
    // puzzle.
    switch (gGameState.entrance) {
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_NORTHWEST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLARS_FROM_WALL_SWITCHES:
      if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        VenusLighthouse_LowerFloatingPillars(0);
        OverlayObject_WaitUntilIdle(Object_GetById(ACTOR_PARTY_LEADER));
        for (pillarIndex = 0; pillarIndex <= 3; pillarIndex++) {
          actor = Object_GetById(pillarIndex + ACTOR_FIRST_FLOATING_PILLAR);
          column = actor->x.fixed >> 20;
          if (column == 13) {
            row = actor->z.fixed >> 20;
            if (row == 7 &&
                Engine_GameFlagIsSet(pillarIndex + FLAG_SCENE_TEMP_00)) {
              actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
              actor->collisionFlags = 0;
              actor->motionFlags = 0;
              Map_CopyCellAttributes(4, 19, 1, 1, column, row);
              break;
            }
          }
        }
      } else {
        actor = Object_GetById(ACTOR_BALANCE_WEST);
        actor->motionFlags = 0;
        actor->y.fixed = -PIXELS(48);
        actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        actor->collisionFlags &= 0xfe;
        actor->timer = 3;
        Engine_ActorSetSpritePriority(ACTOR_BALANCE_WEST, 1);
        actor = Object_GetById(ACTOR_BALANCE_EAST);
        actor->motionFlags = 0;
        actor->y.fixed = -PIXELS(48);
        actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        actor->collisionFlags &= 0xfe;
        actor->timer = 3;
        Engine_ActorSetSpritePriority(ACTOR_BALANCE_EAST, 1);
        actor = Object_GetById(ACTOR_FIRST_FLOATING_PILLAR);
        actor->motionFlags = 0;
        actor->timer = 0;
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_FLOATING_PILLAR),
                                   0);
        actor = Object_GetById(ACTOR_FIRST_FLOATING_PILLAR + 1);
        actor->motionFlags = 0;
        actor->timer = 0;
        Engine_ActorSetSpriteFlags(
            Object_GetById(ACTOR_FIRST_FLOATING_PILLAR + 1), 0);
        actor = Object_GetById(ACTOR_FIRST_FLOATING_PILLAR + 2);
        actor->motionFlags = 0;
        actor->timer = 0;
        Engine_ActorSetSpriteFlags(
            Object_GetById(ACTOR_FIRST_FLOATING_PILLAR + 2), 0);
        actor = Object_GetById(ACTOR_FIRST_FLOATING_PILLAR + 3);
        actor->motionFlags = 0;
        actor->timer = 0;
        Engine_ActorSetSpriteFlags(
            Object_GetById(ACTOR_FIRST_FLOATING_PILLAR + 3), 0);
      }
      break;
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_FROM_WALL_SWITCHES_WEST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_FROM_WALL_SWITCHES_EAST:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      break;
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST:
      gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
      Object_GetById(ACTOR_PARTY_LEADER)->y.fixed = -PIXELS(2);
      // Restore the same spray and passage state.
      // fall through
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLARS:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHWEST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHEAST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHEAST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLAR_SOCKETS_EAST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_GAIA_BLADE_WEST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_GAIA_BLADE_EAST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLAR_SOCKETS_SOUTH:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE_RISING:
      Object_GetById(ACTOR_RISING_PLATFORM)->motionFlags = 4;
      Object_GetById(ACTOR_RISING_PLATFORM)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      Object_GetById(ACTOR_RISING_PLATFORM)->y.fixed = -0x108000;
      DisplayBlend_SuspendScript();
      Io_Write16(0, &REG_BLDCNT);
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED)) {
        BattleFx_SetQueuedSoundAndPlay(SE_170);
        Map_CopyCellsTo(36, 81, 32, 81, 3, 2);
        Map_CopyCellsTo(36, 83, 36, 81, 3, 2);
        Map_CopyCellAttributes(36, 17, 3, 1, 32, 17);
        Map_CopyCellAttributes(36, 18, 3, 1, 36, 17);
        Map_CopyCellsTo(63, 29, 33, 20, 1, 1);
        Map_CopyCellsTo(20, 56, 36, 17, 3, 4);
      }
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED)) {
        Map_CopyCellsTo(44, 81, 48, 81, 3, 2);
        Map_CopyCellsTo(44, 83, 44, 81, 3, 2);
        Map_CopyCellAttributes(44, 17, 3, 1, 48, 17);
        Map_CopyCellAttributes(44, 18, 3, 1, 44, 17);
        Map_CopyCellsTo(63, 29, 49, 20, 1, 1);
        Map_CopyCellsTo(41, 56, 44, 17, 3, 4);
      }
      if (gGameState.entrance ==
              VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST ||
          gGameState.entrance ==
              VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST) {
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
      }
      if (gGameState.entrance ==
          VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE_RISING)
        VenusLighthouse_RaiseLeaderWithSpray();
      break;
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_FROM_WALL_SWITCHES_SOUTH:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_FROM_WALL_SWITCHES_EAST:
      Engine_TaskWait(1);
      FieldScheduler_AddCallback(VenusLighthouse_VisitPillarActors, 0xc80);
      actor = Object_GetById(ACTOR_SOCKET_EFFECT_STATUE);
      actor->motionFlags = 0;
      actor->y.fixed = 0;
      Object_GetById(ACTOR_FIRST_SOCKET_PILLAR)->motionFlags = 0;
      Object_GetById(ACTOR_FIRST_SOCKET_PILLAR + 1)->motionFlags = 0;
      Object_GetById(ACTOR_FIRST_SOCKET_PILLAR + 2)->motionFlags = 0;
      Object_GetById(ACTOR_FIRST_SOCKET_PILLAR + 3)->motionFlags = 0;
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN)) {
        Engine_TaskWait(1);
        Map_CopyCellsTo(95, 56, 77, 35, 1, 2);
        Map_CopyCellAttributes(13, 35, 1, 1, 13, 36);
        Engine_ActorSetPosition(ACTOR_FIRST_SOCKET_PILLAR, PIXELS(264),
                                PIXELS(744));
        actor = Object_GetById(ACTOR_FIRST_SOCKET_PILLAR);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetSpritePriority(ACTOR_FIRST_SOCKET_PILLAR, 3);
        Actor_SetPosition(ACTOR_FIRST_SOCKET_PILLAR + 1, PIXELS(184),
                          PIXELS(632));
        actor = Object_GetById(ACTOR_FIRST_SOCKET_PILLAR + 1);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetSpritePriority(ACTOR_FIRST_SOCKET_PILLAR + 1, 3);
        Engine_ActorSetPosition(ACTOR_FIRST_SOCKET_PILLAR + 2, PIXELS(232),
                                PIXELS(696));
        actor = Object_GetById(ACTOR_FIRST_SOCKET_PILLAR + 2);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetSpritePriority(ACTOR_FIRST_SOCKET_PILLAR + 2, 3);
        Engine_ActorSetPosition(ACTOR_FIRST_SOCKET_PILLAR + 3, PIXELS(184),
                                PIXELS(664));
        actor = Object_GetById(ACTOR_FIRST_SOCKET_PILLAR + 3);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetSpritePriority(ACTOR_FIRST_SOCKET_PILLAR + 3, 3);
        Engine_ActorSetAnimation(ACTOR_SOCKET_GATE_EFFECT, 7);
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SOCKET_GATE_EFFECT), 0);
        Engine_ActorSetSpritePriority(ACTOR_SOCKET_GATE_EFFECT, 1);
        actor = Object_GetById(ACTOR_SOCKET_GATE_EFFECT);
        actor->collisionFlags = 0;
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetPosition(ACTOR_SOCKET_GATE_EFFECT, PIXELS(215), PIXELS(600));
#ifdef UBFIX
        actor->update = VenusLighthouse_EmitGateParticles;
#else
        actor->update =
            (void (*)(union FieldObject *))VenusLighthouse_EmitGateParticles;
#endif
      }
      Event_MoveSocketPillar();
      break;
    }
  }
  return 0;
}

/// Standing on this floor (VenusLighthouseFourthFloor's trigger 31) carries the
/// party north in a trail of dust; at a ledge they drop down. The floors of
/// triggers 30, 32 and 33 do the same south, west and east, and the southward
/// floor also lies in VenusLighthouseFirstFloor and VenusLighthouseThirdFloor.
static void Event_DriftNorth(void) {
  struct Vector3 args;

  args.x = 0;
  args.z = -1;
  VenusLighthouse_StepActorWithDust(&args);
}

/// As Event_DriftNorth, south.
static void Event_DriftSouth(void) {
  struct Vector3 args;

  args.x = 0;
  args.z = 1;
  VenusLighthouse_StepActorWithDust(&args);
}

/// As Event_DriftNorth, west.
static void Event_DriftWest(void) {
  struct Vector3 args;

  args.x = -1;
  args.z = 0;
  VenusLighthouse_StepActorWithDust(&args);
}

/// As Event_DriftNorth, east.
static void Event_DriftEast(void) {
  struct Vector3 args;

  args.x = 1;
  args.z = 0;
  VenusLighthouse_StepActorWithDust(&args);
}

/// Follows the selected actor through the passage, adjusting for walls and
/// drops.
/// @param direction The way the drifting floor carries the party: -1, 0 or 1
/// on x and on z; y is not read.
static void VenusLighthouse_StepActorWithDust(struct Vector3 *direction) {
  s32 actorId;
  struct FieldActor *viewCenter;
  struct FieldActor *actor;
  struct FieldPosition nextPosition;
  struct EffectOptions options;
  s32 collision;
  s32 framePhase;
  s32 particleX;

  {
    struct EventWork *event = gEventWork;

    actorId = gGameState.selectedActor;
    viewCenter = event->viewCenter;
  }
  actor = Object_GetById(actorId);
  nextPosition.x =
      actor->x.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
  nextPosition.y = actor->y.fixed;
  nextPosition.z =
      actor->z.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
  collision = FieldObject_CheckMovementCollision(actor, &nextPosition);
  framePhase = gFrameCount & 4;
  if (framePhase == 0) {
    options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    particleX = actor->x.fixed +
                (((u32)(Engine_RandomNext() * 12) >> 16) << 16) - 0x60000;
    Effect_Spawn(particleX, actor->y.fixed, actor->z.fixed,
                 (((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999 + 0x7ffd) *
                     direction->x,
                 framePhase,
                 direction->z *
                     (((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999 + 0x7ffd),
                 0x800000, &options);
  }
  // A drop uses the fall animation; solid terrain instead constrains each step.
  if (collision < 0) {
    Engine_ActorSetAttachedEffect(actorId, EMOTE_IN_FRONT | 2);
    Engine_ObjectSetPosition(actor, actor->x.fixed, actor->y.fixed,
                             actor->z.fixed + PIXELS(8));
    Object_SetMode(actor, 7);
    FieldObject_WaitForMove(actor);
    do {
      Engine_TaskWait(1);
    } while (actor->y.fixed != actor->terrainHeight);
    Object_SetMode(actor, 6);
    Engine_TaskWait(3);
    return;
  }
  nextPosition.x =
      actor->x.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x, 19);
  nextPosition.y = actor->y.fixed;
  nextPosition.z =
      actor->z.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z, 19);
  collision = FieldObject_CheckMovementCollision(actor, &nextPosition);
  if (collision > 0) {
    return;
  }
  nextPosition.x =
      actor->x.fixed + direction->x * 0x5b333 - direction->z * 0x5b333;
  nextPosition.y = actor->y.fixed;
  nextPosition.z =
      actor->z.fixed + direction->z * 0x5b333 - direction->x * 0x5b333;
  collision = FieldObject_CheckMovementCollision(actor, &nextPosition);
  if (collision > 0) {
    actor->x.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
    actor->z.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
    return;
  }
  nextPosition.x = (direction->x + direction->z) * 0x5b333 + actor->x.fixed;
  nextPosition.y = actor->y.fixed;
  nextPosition.z = (direction->x + direction->z) * 0x5b333 + actor->z.fixed;
  collision = FieldObject_CheckMovementCollision(actor, &nextPosition);
  if (collision > 0) {
    actor->x.fixed -= VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
    actor->z.fixed -= VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
    return;
  }
  // Advance the camera anchor together with an unobstructed actor.
  viewCenter->x.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
  actor->x.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
  viewCenter->z.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
  actor->z.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
}

/// Chooses the particle scales and spin used by the tile-wave animations.
static __inline__ void Rubble_SetScaleAndSpin(struct EffectOptions *options) {
  options->startScaleX =
      ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
  options->startScaleY =
      ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
  options->spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
}

/// Animates the two tile waves Event_StepOnWallSwitch sets off in
/// VenusLighthouseFirstFloor, then takes the party back to
/// VenusLighthouseSecondFloor by
/// VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH.
static void VenusLighthouse_AnimatePairedTileWaves(void) {
  struct EffectOptions options;
  u32 extraBursts;
  u32 row;
  u32 particleIndex;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventBegin();
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  BattleFx_SetQueuedSoundAndPlay(SE_170);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_AudioPlayCue(SE_162);
  extraBursts = 0;
  // Advance the first tile wave, holding its fourth row for three extra bursts.
  for (row = 0; row < 10; row++) {
    Rubble_SetScaleAndSpin(&options);
    for (;;) {
      for (particleIndex = 0; particleIndex < 4 && row < 8; particleIndex++) {
        Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x3600000,
                     0, 0x300000 + row * 0x100000 + particleIndex * 0x40000, 0,
                     0, 0, 0x880000, &options);
      }
      Engine_TaskWait(3);
      if (row != 3 || extraBursts >= 3) {
        break;
      }
      extraBursts++;
    }
    Map_CopyCellsTo(48, row + 3, 54, row + 3, 3, 1);
  }
  Map_CopyCellsTo(111, 5, 117, 5, 5, 2);
  Map_CopyCellsTo(111, 10, 117, 10, 5, 2);
  Map_CopyCellsTo(111, 7, 111, 5, 5, 2);
  Map_CopyCellsTo(111, 7, 111, 10, 5, 2);
  for (row = 0; row < 10; row++) {
    Rubble_SetScaleAndSpin(&options);
    for (particleIndex = 0; particleIndex < 4 && row < 8; particleIndex++) {
      Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x3000000,
                   0, 0x300000 + row * 0x100000 + particleIndex * 0x40000, 0, 0,
                   0, 0x880000, &options);
    }
    Engine_TaskWait(3);
    Map_CopyCellsTo(55, row + 26, 48, row + 3, 3, 1);
  }
  Engine_AudioPlayCue(SE_289);
  Engine_EventWait(60);
  Engine_EventRequestExit(
      VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH);
  Engine_EventEnd();
}

/// Emits the scheduled particles at the supplied passage coordinates.
static void VenusLighthouse_SpawnRandomParticles(s32 x, s32 z) {
  struct EffectOptions options;

  options.startScaleX = 0xb333;
  options.startScaleY = 0xb333;
  options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
  if ((gFrameCount & 3) == 0) {
    if (((u32)(Engine_RandomNext() << 1) >> 16) != 0) {
      Effect_Spawn(
          (x + (((u32)(Engine_RandomNext() << 1) >> 16) << 4)) << 16, 0,
          z << 19, 0, 0,
          (s32)((((u32)(Engine_RandomNext() * 5) >> 16) << 16) + 0x70000) / 10,
          0x880000, &options);
    } else {
      Effect_Spawn((x + ((u32)(Engine_RandomNext() * 17) >> 16)) << 16, 0,
                   (x << 19) - 0x40000, 0, 0, 0, 0x880000, &options);
    }
  }
}

static void VenusLighthouse_EmitWestPassageParticles(void) {
  VenusLighthouse_SpawnRandomParticles(432, 32);
}

static void VenusLighthouse_EmitEastPassageParticles(void) {
  VenusLighthouse_SpawnRandomParticles(0x230, 44);
}

/// Animates the western tile wave Event_StepOnWestWallSwitch sets off and
/// starts its lingering particle task, then takes the party back to
/// VenusLighthouseFourthFloor by
/// VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST.
static void VenusLighthouse_AnimateWestTileWave(void) {
  struct EffectOptions options;
  u32 extraBursts;
  s32 zero;
  u32 row;
  s32 rowOffset;
  s32 particleZ;
  u32 particleIndex;
  struct EffectOptions *effectOptions;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventBegin();
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  BattleFx_SetQueuedSoundAndPlay(SE_170);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_AudioPlayCue(SE_162);
  Engine_CameraSetSpeed(0x8000, 0x1000);
  Engine_CameraMoveTo(PIXELS(440), -1, PIXELS(360), 1);
  extraBursts = 0;
  row = 0;
  effectOptions = &options;
  rowOffset = 0;
  do {
    Rubble_SetScaleAndSpin(&options);
  // Repeat particle bursts on the fourth row before advancing the copied tiles.
  again:
    particleZ = 0xc00000;
    for (particleIndex = 0, zero = 0, particleZ += rowOffset;
         particleIndex <= 3; particleIndex++) {
      Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x1a00000,
                   0, particleZ, 0, zero, zero, 0x880000, effectOptions);
      particleZ += 0x40000;
    }
    Engine_TaskWait(3);
    if (row == 3) {
      if (extraBursts <= 2) {
        extraBursts++;
        goto again;
      }
      if (extraBursts == 3)
        FieldScheduler_AddCallback(VenusLighthouse_EmitWestPassageParticles,
                                   3200);
    }
    Map_CopyCellsTo(53, row + 12, 26, row + 12, 3, 1);
    rowOffset += 0x100000;
    row++;
  } while (row <= 12);
  Map_CopyCellsTo(81, 41, 89, 14, 9, 2);
  Engine_CameraWaitForMove();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_EventWait(60);
  Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED);
  Engine_EventRequestExit(
      VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST);
  Engine_EventEnd();
}

/// Animates the eastern tile wave Event_StepOnEastWallSwitch sets off and
/// starts its lingering particle task, then takes the party back to
/// VenusLighthouseFourthFloor by
/// VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST.
static void VenusLighthouse_AnimateEastTileWave(void) {
  struct EffectOptions options;
  u32 extraBursts;
  s32 zero;
  u32 row;
  s32 rowOffset;
  s32 particleZ;
  u32 particleIndex;
  struct EffectOptions *effectOptions;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventBegin();
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  BattleFx_SetQueuedSoundAndPlay(SE_170);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_AudioPlayCue(SE_162);
  Engine_CameraSetSpeed(0x8000, 0x1000);
  Engine_CameraMoveTo(PIXELS(568), -1, PIXELS(360), 1);
  extraBursts = 0;
  row = 0;
  effectOptions = &options;
  rowOffset = 0;
  do {
    Rubble_SetScaleAndSpin(&options);
  // Keep this route task scheduling after each completed row.
  again:
    particleZ = 0xc00000;
    for (particleIndex = 0, zero = 0, particleZ += rowOffset;
         particleIndex <= 3; particleIndex++) {
      Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x2200000,
                   0, particleZ, 0, zero, zero, 0x880000, effectOptions);
      particleZ += 0x40000;
    }
    Engine_TaskWait(3);
    if (row == 3 && extraBursts <= 2) {
      extraBursts++;
      goto again;
    }
    FieldScheduler_AddCallback(VenusLighthouse_EmitEastPassageParticles, 3200);
    Map_CopyCellsTo(58, row + 12, 34, row + 12, 3, 1);
    rowOffset += 0x100000;
    row++;
  } while (row <= 12);
  Map_CopyCellsTo(86, 41, 97, 14, 5, 2);
  Engine_CameraWaitForMove();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_EventWait(60);
  Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED);
  Engine_EventRequestExit(
      VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST);
  Engine_EventEnd();
}

// Emit rising particles around the actor every four frames.
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 14 bytes of the 196-byte function. Keep the unused
// native result in matching builds.
#ifdef UBFIX
static void VenusLighthouse_UpdateRisingSpray(union FieldObject *fieldObject) {
#else
static s32 VenusLighthouse_UpdateRisingSpray(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct EffectOptions options;
  struct EffectOptions *effectOptions = &options;
  s32 framePhase;
  s32 particleX, particleY, particleZ, riseSpeed;

  effectOptions->priority = 1;
  effectOptions->palette = 5;
  effectOptions->type = 286;
  effectOptions->script = gVenusSprayScript;
  framePhase = gFrameCount & 3;
  if (framePhase == 0) {
    if ((gFrameCount & 7) == 0) {
      Engine_AudioPlayCue(SE_246);
    }
    particleX =
        actor->x.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
    particleY =
        actor->y.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
    particleZ =
        actor->z.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
    riseSpeed = (((u32)(Engine_RandomNext() * 4) >> 16) << 15) + 0x8000;
    Effect_Spawn(particleX, particleY, particleZ, 0, riseSpeed, framePhase,
                 0x330000, effectOptions);
  }
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Lands the leader on ACTOR_RISING_PLATFORM, then raises both in a rising
/// spray and leaves by VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_AERIE.
static void VenusLighthouse_RaiseLeaderWithSpray(void) {
  u32 riseFrame;
  struct FieldActor *leader;
  struct FieldActor *platform;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  platform = Object_GetById(ACTOR_RISING_PLATFORM);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  // Land the leader on the platform, then synchronize their small settling
  // offsets.
  leader->y.fixed = PIXELS(130);
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Event_PlayCue(SE_204);
  Engine_EventWait(30);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Engine_EventWait(24);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 22);
  leader->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
  platform->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-3, 16);
  leader->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-3, 16);
  leader->terrainHeight += VENUS_LIGHTHOUSE_FIXED_OFFSET(-3, 16);
  Engine_TaskWait(2);
  platform->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-2, 16);
  leader->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-2, 16);
  leader->terrainHeight += VENUS_LIGHTHOUSE_FIXED_OFFSET(-2, 16);
  Engine_TaskWait(10);
  platform->y.fixed += PIXELS(2);
  leader->y.fixed += PIXELS(2);
  leader->terrainHeight += PIXELS(2);
  Engine_TaskWait(4);
  platform->y.fixed += PIXELS(2);
  leader->y.fixed += PIXELS(2);
  leader->terrainHeight += PIXELS(2);
  Engine_TaskWait(4);
  platform->y.fixed += PIXELS(1);
  leader->y.fixed += PIXELS(1);
  leader->terrainHeight += PIXELS(1);
  leader->motionFlags = 0;
  platform->motionFlags = 0;
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventWait(40);
#ifdef UBFIX
  leader->update = VenusLighthouse_UpdateRisingSpray;
#else
  leader->update = (void *)VenusLighthouse_UpdateRisingSpray;
#endif
  Engine_EventWait(60);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
  Engine_ActorSetSpritePriority(ACTOR_RISING_PLATFORM, 1);
  Event_PlayCue(MUS_017);
  Event_PlayCue(SE_308);
  GameFlag_Set(FLAG_SKIP_SCENE_CHANGE_SOUND);
  // Raise the leader, terrain contact and platform by the same pixel each
  // frame.
  for (riseFrame = 0; riseFrame < 128; riseFrame++) {
    leader->y.fixed += PIXELS(1);
    leader->terrainHeight += PIXELS(1);
    platform->y.fixed += PIXELS(1);
    Engine_TaskWait(1);
  }
  Engine_EventRequestExit(
      VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_AERIE);
}

/// Selects the pillar's push script from its direction, support animation and
/// socket.
static s32
VenusLighthouse_DispatchPushScript(struct ScriptInterpreter *interpreter) {
  struct FieldActor *pillar = (struct FieldActor *)interpreter;
  struct FieldPosition supportPosition;
  struct FieldActor *support;
  struct AnimationObject *sprite;
  const u8 *allowedShapes;
  u8 shapeId;
  u32 switchIndex;
  s32 shapeIndex;
  const u8 *shapeCursor;

  supportPosition.x = pillar->x.fixed;
  supportPosition.y = pillar->y.fixed - 0x100000;
  supportPosition.z = pillar->z.fixed;
  support = SceneData_FindSlotAtPosition(&supportPosition, 0);
  // XXX: Do the pillar placement and action-script tables guarantee a support
  // with an animation entry every time this script runs?
  sprite = support->sprite;
  if (sprite->entries[0]->animId == 0x100) {
    s32 velocityX = pillar->velocityX;
    s32 speedX = velocityX < 0 ? -velocityX : velocityX;
    s32 velocityZ = pillar->velocityZ;
    s32 speedZ = velocityZ < 0 ? -velocityZ : velocityZ;

    if (speedX > speedZ) {
      if (velocityX / 0x10000 < 0) {
        allowedShapes = gVenusShapesPushedLeft;
      } else {
        allowedShapes = gVenusShapesPushedRight;
      }
    } else {
      if (velocityZ / 0x10000 < 0) {
        allowedShapes = gVenusShapesPushedUp;
      } else {
        allowedShapes = gVenusShapesPushedDown;
      }
    }
    for (; *allowedShapes != 0; allowedShapes++) {
      if (sprite->lastNo == *allowedShapes) {
        break;
      }
    }
    if (*allowedShapes == 0) {
      Engine_ObjectSetScript(pillar, gVenusRefusedPushScript);
      return 0;
    }
    // The socket tables retain their moving cursors between push-script calls.
    if (gGameState.scene == (s32)&SceneId_VenusLighthouseThirdFloor) {
      for (switchIndex = 0;
           switchIndex <= 3 &&
           !(pillar->x.fixed >> 20 == gVenusSwitchCells[switchIndex].x &&
             pillar->z.fixed >> 20 == gVenusSwitchCells[switchIndex].z);
           switchIndex++) {
      }
      for (shapeIndex = 0;; shapeIndex++) {
        shapeCursor = gVenusSwitchShapes[switchIndex];
        shapeId = *shapeCursor;
        if (shapeId == 0) {
          Engine_ObjectSetScript(pillar, gVenusRefusedPushScript);
          return 0;
        }
        if (shapeId == support->sprite->lastNo) {
          break;
        }
        gVenusSwitchShapes[switchIndex] = shapeCursor + 1;
      }
      Engine_ObjectSetScript(pillar,
                             gVenusSwitchPushScripts[switchIndex][shapeIndex]);
    } else {
      for (switchIndex = 0;
           switchIndex <= 7 &&
           !(pillar->x.fixed >> 20 == gVenusPillarCells[switchIndex].x &&
             pillar->z.fixed >> 20 == gVenusPillarCells[switchIndex].z);
           switchIndex++) {
      }
      for (shapeIndex = 0;; shapeIndex++) {
        shapeCursor = gVenusPillarShapes[switchIndex];
        shapeId = *shapeCursor;
        if (shapeId == 0) {
          Engine_ObjectSetScript(pillar, gVenusRefusedPushScript);
          return 0;
        }
        if (shapeId == support->sprite->lastNo) {
          break;
        }
        gVenusPillarShapes[switchIndex] = shapeCursor + 1;
      }
      Engine_ObjectSetScript(pillar,
                             gVenusPillarPushScripts[switchIndex][shapeIndex]);
    }
  } else {
    Engine_ObjectSetScript(pillar, gVenusRefusedPushScript);
  }
  return 0;
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

// Step for each of 16 directions: X in the upper halfword, Z in the lower.
// UB: westward rows contain a negative X step; pack its unsigned bits.
#ifdef UBFIX
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))
#else
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))
#endif

u32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

struct SwitchCell gVenusSwitchCells[] = {
    {48, 41},
    {52, 41},
    {48, 43},
    {52, 43},
};

// clang-format off
u32 gVenusPillarHeights[] = {
    0xfff00000, 0xfff00000, 0xffe00000, 0xffd00000, 0xffc00000, 0xffb00000, 0xffb00000,
};

struct SwitchCell gVenusPillarCells[] = {
    { 11, 39 },
    { 14, 39 },
    { 11, 41 },
    { 16, 42 },
    { 10, 43 },
    { 14, 43 },
    { 10, 46 },
    { 16, 46 },
};

u8 gVenusShapesPushedLeft[] = {
    5, 6, 7, 0,
};

u8 gVenusShapesPushedRight[] = {
    4, 6, 8, 0,
};

u8 gVenusShapesPushedUp[] = {
    3, 7, 8, 0,
};

u8 gVenusShapesPushedDown[] = {
    3, 4, 5, 0,
};

u8 gVenusSwitchShape0[] = {
    6, 7, 8, 0,
};

u8 gVenusSwitchShape1[] = {
    6, 7, 8, 0,
};

u8 gVenusSwitchShape2[] = {
    4, 5, 6, 0,
};

u8 gVenusSwitchShape3[] = {
    4, 5, 6, 0,
};

u8 gVenusPillarShape0[] = {
    7, 0,
};

u8 gVenusPillarShape1[] = {
    0, 5,
};

u8 gVenusPillarShape2[] = {
    0, 8,
};

u8 gVenusPillarShape3[] = {
    0, 5,
};

u8 gVenusPillarShape4[] = {
    0, 7, 8, 0,
};

u8 gVenusPillarShape5[] = {
    3, 0,
};

u8 gVenusPillarShape6[] = {
    3, 0,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gVenusLighthousePlacements2_0[] = {
    SC_SET(0x23, 0x2126),
    SC_SET(0x27, 1),
    SC_CALL(VenusLighthouse_UpdateChildFromTimer),
    SC_END,
};

s32 gVenusLighthousePlacements3_Data[] = {
    SC_SET(0x23, 0x2126),
    SC_CALL(VenusLighthouse_UpdateChildFromTimer),
    SC_END,
};

s32 gVenusLeaderApproachScript[] = {
    SC_ARG(12),
    SC_SET(SW_HOOK, (s32)VenusLighthouse_PulseChildEffect),
    SC_LABEL(1),
    SC_SET(SW_SCALE_X, 0x11999),
    SC_SET(SW_SCALE_Y, 0x11999),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0xe666),
    SC_SET(SW_SCALE_Y, 0xe666),
    SC_WAIT(2),
    SC_LOOP(10, SC_TO(1)),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_END,
};

s32 gVenusLighthousePlacements2_1[] = {
    SC_SET(SW_SCALE_X, 0x9999),
    SC_SET(SW_SCALE_Y, 0x9999),
    SC_SET(SW_COLLISION, 0),
    SC_CALL(VenusLighthouse_ClearScriptedSpriteFlags),
    SC_END,
};

s32 gVenusSprayScript[] = {
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 0xa000),
    SC_SET(SW_SCALE_X, 0xa000),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 0x4000),
    SC_SET(SW_SCALE_X, 0x4000),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 0xa000),
    SC_SET(SW_SCALE_X, 0xa000),
    SC_WAIT(6),
    SC_LOOP(4, SC_TOP),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gVenusLighthousePlacements4_1[] = {
    SC_ARG(3),
    SC_END,
};

s32 gVenusLighthousePlacements5_3[] = {
    SC_ARG(4),
    SC_END,
};

s32 gVenusLighthousePlacements6_4[] = {
    SC_ARG(5),
    SC_END,
};

s32 gVenusLighthousePlacements5_2[] = {
    SC_ARG(6),
    SC_END,
};

s32 gVenusLighthousePlacements5_1[] = {
    SC_ARG(7),
    SC_END,
};

s32 gVenusLighthousePlacements6_3[] = {
    SC_ARG(8),
    SC_END,
};

s32 gVenusPushScript[] = {
    SC_CALL(VenusLighthouse_ClearScriptedSpriteFlags),
    SC_ARG(7),
    SC_SET(SW_SPEED, 0x10000),
    SC_SET(SW_HOOK, (s32)VenusLighthouse_EmitGateParticles),
    SC_MOVE(PIXELS(760), 0, PIXELS(728)),
    SC_SYNC,
    SC_POS(PIXELS(760), 0, PIXELS(728)),
    SC_MOVE(PIXELS(760), 0, PIXELS(744)),
    SC_SYNC,
    SC_POS(PIXELS(760), 0, PIXELS(744)),
    SC_MOVE(PIXELS(744), 0, PIXELS(744)),
    SC_SYNC,
    SC_POS(PIXELS(743), 0, PIXELS(744)),
    SC_MOVE(PIXELS(744), 0, PIXELS(696)),
    SC_SYNC,
    SC_POS(PIXELS(743), 0, PIXELS(696)),
    SC_WAIT(1),
    SC_END,
};

s32 gVenusSettleScriptA[] = {
    SC_MOVE(PIXELS(872), 0, PIXELS(680)),
    SC_SYNC,
    SC_POS(PIXELS(872), 0, PIXELS(680)),
    SC_MOVE(PIXELS(872), 0, PIXELS(664)),
    SC_SYNC,
    SC_POS(PIXELS(872), 0, PIXELS(664)),
    SC_MOVE(PIXELS(840), 0, PIXELS(664)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(840)),
    SC_SET(SW_Z, PIXELS(664)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusSettleScriptB[] = {
    SC_MOVE(PIXELS(888), 0, PIXELS(696)),
    SC_SYNC,
    SC_POS(PIXELS(888), 0, PIXELS(696)),
    SC_MOVE(PIXELS(840), 0, PIXELS(696)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(840)),
    SC_SET(SW_Z, PIXELS(696)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusRefusedPushScript[] = {
    SC_SET(SW_HOOK, 0),
    SC_WAIT(10),
    SC_FREE,
};

s32 gVenusSwitchPushScript0_0[] = {
    SC_MOVE(PIXELS(728), 0, PIXELS(664)),
    SC_SYNC,
    SC_POS(PIXELS(728), 0, PIXELS(664)),
    SC_MOVE(PIXELS(728), 0, PIXELS(632)),
    SC_SYNC,
    SC_POS(PIXELS(728), 0, PIXELS(632)),
    SC_SET(0x28, 1),
    SC_END,
};

s32 gVenusSwitchPushScript0_1[] = {
    SC_MOVE(PIXELS(776), 0, PIXELS(696)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(776)),
    SC_SET(SW_Z, PIXELS(696)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusSwitchPushScript1_0[] = {
    SC_MOVE(PIXELS(776), 0, PIXELS(664)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(776)),
    SC_SET(SW_Z, PIXELS(664)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusSwitchPushScript1_1[] = {
    SC_MOVE(PIXELS(840), 0, PIXELS(696)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(840)),
    SC_SET(SW_Z, PIXELS(696)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusSwitchPushScript2_1[] = {
    SC_MOVE(PIXELS(776), 0, PIXELS(664)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(776)),
    SC_SET(SW_Z, PIXELS(664)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusSwitchPushScript2_0[] = {
    SC_MOVE(PIXELS(728), 0, PIXELS(696)),
    SC_SYNC,
    SC_POS(PIXELS(728), 0, PIXELS(696)),
    SC_MOVE(PIXELS(728), 0, PIXELS(632)),
    SC_SYNC,
    SC_POS(PIXELS(728), 0, PIXELS(632)),
    SC_SET(0x28, 1),
    SC_END,
};

s32 gVenusSwitchPushScript3_1[] = {
    SC_MOVE(PIXELS(840), 0, PIXELS(664)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(840)),
    SC_SET(SW_Z, PIXELS(664)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusSwitchPushScript3_0[] = {
    SC_MOVE(PIXELS(776), 0, PIXELS(696)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(776)),
    SC_SET(SW_Z, PIXELS(696)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusFirstGateScript[] = {
    SC_MOVE(PIXELS(216), 0, PIXELS(776)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(776)),
    SC_MOVE(PIXELS(216), 0, PIXELS(760)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(760)),
    SC_END,
};

s32 gVenusFirstGateOpenScript[] = {
    SC_MOVE(PIXELS(264), 0, PIXELS(760)),
    SC_SYNC,
    SC_POS(PIXELS(264), 0, PIXELS(760)),
    SC_MOVE(PIXELS(264), 0, PIXELS(744)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(264)),
    SC_SET(SW_Z, PIXELS(744)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusPillarPushScript7_0[] = {
    SC_MOVE(PIXELS(264), 0, PIXELS(728)),
    SC_SYNC,
    SC_POS(PIXELS(264), 0, PIXELS(728)),
    SC_MOVE(PIXELS(232), 0, PIXELS(728)),
    SC_SYNC,
    SC_POS(PIXELS(232), 0, PIXELS(728)),
    SC_MOVE(PIXELS(232), 0, PIXELS(696)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(232)),
    SC_SET(SW_Z, PIXELS(696)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusPillarPushScript5_1[] = {
    SC_MOVE(PIXELS(216), 0, PIXELS(696)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(696)),
    SC_MOVE(PIXELS(216), 0, PIXELS(664)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(664)),
    SC_MOVE(PIXELS(184), 0, PIXELS(664)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(184)),
    SC_SET(SW_Z, PIXELS(664)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusPillarPushScript5_0[] = {
    SC_MOVE(PIXELS(264), 0, PIXELS(696)),
    SC_SYNC,
    SC_POS(PIXELS(264), 0, PIXELS(696)),
    SC_MOVE(PIXELS(264), 0, PIXELS(680)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(264)),
    SC_SET(SW_Z, PIXELS(680)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusPillarPushScript2_0[] = {
    SC_MOVE(PIXELS(184), 0, PIXELS(632)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(184)),
    SC_SET(SW_Z, PIXELS(632)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusPillarPushScript3_0[] = {
    SC_MOVE(PIXELS(248), 0, PIXELS(680)),
    SC_SYNC,
    SC_POS(PIXELS(248), 0, PIXELS(680)),
    SC_MOVE(PIXELS(248), 0, PIXELS(664)),
    SC_SYNC,
    SC_POS(PIXELS(248), 0, PIXELS(664)),
    SC_MOVE(PIXELS(264), 0, PIXELS(664)),
    SC_SYNC,
    SC_POS(PIXELS(264), 0, PIXELS(664)),
    SC_MOVE(PIXELS(264), 0, PIXELS(648)),
    SC_SYNC,
    SC_POS(PIXELS(264), 0, PIXELS(648)),
    SC_MOVE(PIXELS(248), 0, PIXELS(648)),
    SC_SYNC,
    SC_POS(PIXELS(248), 0, PIXELS(648)),
    SC_MOVE(PIXELS(248), 0, PIXELS(632)),
    SC_SYNC,
    SC_POS(PIXELS(248), 0, PIXELS(632)),
    SC_MOVE(PIXELS(232), 0, PIXELS(632)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(232)),
    SC_SET(SW_Z, PIXELS(632)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusSecondGateScript[] = {
    SC_MOVE(PIXELS(216), 0, PIXELS(744)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(744)),
    SC_MOVE(PIXELS(184), 0, PIXELS(744)),
    SC_SYNC,
    SC_POS(PIXELS(184), 0, PIXELS(744)),
    SC_MOVE(PIXELS(184), 0, PIXELS(760)),
    SC_SYNC,
    SC_POS(PIXELS(184), 0, PIXELS(760)),
    SC_MOVE(PIXELS(168), 0, PIXELS(760)),
    SC_SYNC,
    SC_POS(PIXELS(168), 0, PIXELS(760)),
    SC_MOVE(PIXELS(168), 0, PIXELS(744)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(168)),
    SC_SET(SW_Z, PIXELS(744)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusPillarPushScript6_0[] = {
    SC_MOVE(PIXELS(168), 0, PIXELS(728)),
    SC_SYNC,
    SC_POS(PIXELS(168), 0, PIXELS(728)),
    SC_MOVE(PIXELS(184), 0, PIXELS(728)),
    SC_SYNC,
    SC_POS(PIXELS(184), 0, PIXELS(728)),
    SC_MOVE(PIXELS(184), 0, PIXELS(696)),
    SC_SYNC,
    SC_POS(PIXELS(184), 0, PIXELS(696)),
    SC_MOVE(PIXELS(168), 0, PIXELS(696)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(168)),
    SC_SET(SW_Z, PIXELS(696)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusPillarPushScript4_0[] = {
    SC_MOVE(PIXELS(168), 0, PIXELS(664)),
    SC_SYNC,
    SC_POS(PIXELS(168), 0, PIXELS(664)),
    SC_MOVE(PIXELS(184), 0, PIXELS(664)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(184)),
    SC_SET(SW_Z, PIXELS(664)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusPillarPushScript0_0[] = {
    SC_MOVE(PIXELS(216), 0, PIXELS(632)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(632)),
    SC_MOVE(PIXELS(216), 0, PIXELS(600)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(600)),
    SC_SET(0x28, 1),
    SC_END,
};

u16 gVenusPushCells[] = {
    MAP_CELL_STEP(124, 50, 1, 2, 6), MAP_CELL_STEP(124, 47, 1, 2, 6), MAP_CELL_STEP(124, 44, 1, 2, 6), MAP_CELL_STEP(124, 41, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 gVenusSettleCells[] = {
    MAP_CELL_STEP(117, 59, 1, 2, 6), MAP_CELL_STEP(115, 59, 1, 2, 6), MAP_CELL_STEP(113, 59, 1, 2, 6), MAP_CELL_STEP(111, 59, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 gVenusGateCells[] = {
    MAP_CELL_STEP(96, 58, 1, 2, 6), MAP_CELL_STEP(95, 58, 1, 2, 6), MAP_CELL_STEP(96, 56, 1, 2, 6), MAP_CELL_STEP(95, 56, 1, 2, 6), MAP_CELL_STEPS_END,
};

struct SceneEntrance gVenusLighthouseEntryEntrances[] = {
    { VENUS_LIGHTHOUSE_ENTRY_FROM_WORLD_MAP_WEST, CONDITION_ALWAYS, 40, 0, 264, FACING_EAST, 0, 0, 0, 512, 544, 0 },
    { VENUS_LIGHTHOUSE_ENTRY_FROM_VENUS_LIGHTHOUSE_GROUND_FLOOR, CONDITION_ALWAYS, 376, 0, 88, FACING_SOUTH, 0, 0, 0, 512, 544, 0 },
    { VENUS_LIGHTHOUSE_ENTRY_FROM_WORLD_MAP_EAST, CONDITION_ALWAYS, 488, 0, 424, FACING_WEST, 0, 0, 0, 512, 544, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVenusLighthouseEntrancesOther[] = {
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_ENTRY, CONDITION_ALWAYS, 120, 0, 200, FACING_NORTH, 0, 0, 56, 240, 216, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_FRONT_HALL_FROM_RELIEFS, CONDITION_ALWAYS, 120, 0, 104, FACING_SOUTH, 0, 0, 56, 240, 216, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_FRONT_HALL, CONDITION_ALWAYS, 344, 0, 232, FACING_NORTH, 0, 256, 48, 512, 248, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_CARRY_STONE_EAST, CONDITION_ALWAYS, 472, 0, 184, FACING_SOUTH, 0, 256, 48, 512, 248, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_CARRY_STONE_WEST, CONDITION_ALWAYS, 376, 0, 184, FACING_SOUTH, 0, 256, 48, 512, 248, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_PILLARS, CONDITION_ALWAYS, 344, 0, 104, FACING_SOUTH, 0, 256, 48, 512, 248, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_RELIEFS, CONDITION_ALWAYS, 840, 0, 248, FACING_NORTH, 0, 672, 32, 1008, 256, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_CARRY_STONE, CONDITION_ALWAYS, 968, 0, 168, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_FLOOR_SWITCH, CONDITION_ALWAYS, 856, 0, 88, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_STATUE_ROW_NORTH, CONDITION_ALWAYS, 808, 0, 88, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_STATUE_ROW_WEST, CONDITION_ALWAYS, 712, 0, 168, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_FROM_PILLARS_NORTH, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 16, 288, 256, 520, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_FROM_PILLARS_WEST, CONDITION_ALWAYS, 104, 0, 504, FACING_NORTH, 0, 16, 288, 256, 520, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_FROM_STAIRWELL, CONDITION_ALWAYS, 104, 0, 472, FACING_SOUTH, 0, 16, 288, 256, 520, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_FROM_FLOOR_SWITCH, CONDITION_ALWAYS, 520, 0, 696, FACING_SOUTH, 0, 304, 344, 576, 728, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_FROM_STAIRWELL, CONDITION_ALWAYS, 344, 0, 568, FACING_SOUTH, 0, 304, 344, 576, 728, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_PILLARS, CONDITION_ALWAYS, 728, 0, 936, FACING_SOUTH, 0, 600, 736, 840, 952, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR, CONDITION_ALWAYS, 744, 0, 792, FACING_SOUTH, 0, 600, 736, 840, 952, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_STAIRWELL_FROM_STATUE_ROW, CONDITION_ALWAYS, 392, 0, 952, FACING_SOUTH, 0, 272, 808, 512, 984, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_STAIRWELL_FROM_GREAT_STATUE, CONDITION_ALWAYS, 392, 0, 856, FACING_SOUTH, 0, 272, 808, 512, 984, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_GREAT_STATUE, CONDITION_ALWAYS, 760, 0, 936, FACING_SOUTH, 0, 600, 736, 840, 952, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_FROM_RELIEFS_WEST, CONDITION_ALWAYS, 88, 0, 872, FACING_SOUTH, 0, 16, 600, 256, 920, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_FROM_RELIEFS_EAST, CONDITION_ALWAYS, 184, 0, 872, FACING_SOUTH, 0, 16, 600, 256, 920, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_FROM_PILLARS, CONDITION_ALWAYS, 184, 0, 648, FACING_SOUTH, 0, 16, 600, 256, 920, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVenusLighthouseFirstFloorEntrances[] = {
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_LOWER_ROOM, CONDITION_ALWAYS, 456, 0, 552, FACING_SOUTH, 0, 376, 344, 736, 624, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_GAP, CONDITION_ALWAYS, 456, 0, 592, FACING_NORTH, 0, 376, 344, 736, 624, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH, CONDITION_ALWAYS, 696, 0, 472, FACING_SOUTH, 0, 376, 344, 736, 624, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_THUNDER_CROWN, CONDITION_ALWAYS, 696, 0, 512, FACING_NORTH, 0, 376, 344, 736, 624, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_FROM_DRIFTING_FLOOR, CONDITION_ALWAYS, 424, 0, 136, FACING_SOUTH, 0, 344, 48, 592, 216, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_FROM_LADDERS, CONDITION_ALWAYS, 520, 0, 176, FACING_NORTH, 0, 344, 48, 592, 216, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_PILLARS_FROM_LADDERS, CONDITION_ALWAYS, 104, 0, 752, FACING_NORTH, 0, 56, 584, 304, 880, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_WEST, CONDITION_ALWAYS, 200, 0, 680, FACING_SOUTH, 0, 56, 584, 304, 880, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_FROM_DRIFTING_FLOOR, CONDITION_ALWAYS, 88, 0, 344, FACING_SOUTH, 0, 40, 280, 288, 496, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_FROM_LADDERS, CONDITION_ALWAYS, 248, 0, 432, FACING_NORTH, 0, 40, 280, 288, 496, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_GAP, CONDITION_ALWAYS, 664, 0, 264, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_NORTH, CONDITION_ALWAYS, 728, 0, 136, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_THUNDER_CROWN, CONDITION_ALWAYS, 840, 0, 136, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_PILLARS, CONDITION_ALWAYS, 952, 0, 136, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_PSYNERGY_STONE, CONDITION_ALWAYS, 840, 0, 360, FACING_NORTH, 0, 616, 40, 1000, 360, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_GROUND_FLOOR, CONDITION_ALWAYS, 88, 0, 152, FACING_SOUTH, 0, 40, 40, 328, 208, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE_FROM_LADDERS, CONDITION_ALWAYS, 184, 0, 104, FACING_SOUTH, 0, 40, 40, 328, 208, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_TUNNEL_RUINS_LADY_STATUES, CONDITION_ALWAYS, 280, 0, 152, FACING_SOUTH, 0, 40, 40, 328, 208, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_FROM_DRIFTING_FLOOR, CONDITION_ALWAYS, 472, 0, 776, FACING_SOUTH, 0, 424, 688, 672, 848, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_FROM_DRIFTING_FLOOR_HOLE, CONDITION_ALWAYS, 600, 0, 776, FACING_SOUTH, 0, 424, 688, 672, 848, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_EAST, CONDITION_ALWAYS, 840, 0, 136, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVenusLighthouseSecondFloorEntrances[] = {
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_NORTH, CONDITION_ALWAYS, 104, 0, 120, FACING_SOUTH, 0, 56, 56, 368, 272, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_SPARKLING_PASSAGE_FROM_PILLAR_PIT, CONDITION_ALWAYS, 312, 0, 200, FACING_SOUTH, 0, 56, 56, 368, 272, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_FROM_SPARKLING_PASSAGE, CONDITION_ALWAYS, 600, 0, 216, FACING_NORTH, 0, 552, 88, 848, 544, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_EAST, CONDITION_ALWAYS, 696, 0, 312, FACING_SOUTH, 0, 552, 88, 848, 544, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_FROM_GATE, CONDITION_ALWAYS, 648, 0, 536, FACING_NORTH, 0, 552, 88, 848, 544, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR, CONDITION_ALWAYS, 776, 0, 536, FACING_SOUTH, 0, 552, 88, 848, 544, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_WEST, CONDITION_ALWAYS, 104, 0, 520, FACING_SOUTH, 0, 56, 392, 352, 608, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_WALL_SWITCH_FROM_GATE, CONDITION_ALWAYS, 312, 0, 584, FACING_NORTH, 0, 56, 392, 352, 608, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_FROM_WALL_SWITCH, CONDITION_ALWAYS, 744, 0, 696, FACING_SOUTH, 0, 648, 632, 896, 800, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_FROM_PILLAR_PIT, CONDITION_ALWAYS, 808, 0, 696, FACING_SOUTH, 0, 648, 632, 896, 800, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH, CONDITION_ALWAYS, 312, 0, 536, FACING_SOUTH, 0, 56, 392, 352, 608, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVenusLighthouseThirdFloorEntrances[] = {
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_WEST_NORTHWEST, CONDITION_ALWAYS, 136, 0, 136, FACING_SOUTH, 0, 8, 56, 256, 224, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST_ROOM_FROM_SPARKLING_PASSAGES, CONDITION_ALWAYS, 136, 0, 184, FACING_NORTH, 0, 8, 56, 256, 224, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_DRAGON_SCALES_FROM_SPARKLING_PASSAGES, CONDITION_ALWAYS, 136, 0, 392, FACING_NORTH, 0, 8, 264, 256, 432, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_NORTHWEST_ROOM, CONDITION_ALWAYS, 312, 0, 680, FACING_SOUTH, 0, 248, 600, 512, 768, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_DRAGON_SCALES, CONDITION_ALWAYS, 392, 0, 680, FACING_SOUTH, 0, 248, 600, 512, 768, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_DRIFTING_FLOOR, CONDITION_ALWAYS, 472, 0, 680, FACING_SOUTH, 0, 248, 600, 512, 768, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHEAST, CONDITION_ALWAYS, 136, 0, 792, FACING_SOUTH, 0, 8, 728, 256, 928, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST_ROOM_FROM_DRIFTING_FLOOR, CONDITION_ALWAYS, 136, 0, 904, FACING_NORTH, 0, 8, 728, 256, 928, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SPARKLING_PASSAGES, CONDITION_ALWAYS, 312, 0, 232, FACING_NORTH, 0, 264, 56, 720, 480, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHWEST, CONDITION_ALWAYS, 376, 0, 184, FACING_SOUTH, 0, 264, 56, 720, 480, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SOUTHWEST_ROOM, CONDITION_ALWAYS, 504, 0, 248, FACING_SOUTH, 0, 264, 56, 720, 480, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHEAST, CONDITION_ALWAYS, 680, 0, 120, FACING_SOUTH, 0, 264, 56, 720, 480, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_SOUTHWEST, CONDITION_ALWAYS, 312, 0, 440, FACING_SOUTH, 0, 264, 56, 720, 480, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_PILLAR_SOCKETS, CONDITION_ALWAYS, 632, 0, 392, FACING_NORTH, 0, 264, 56, 720, 480, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_FROM_DRIFTING_FLOOR, CONDITION_ALWAYS, 728, 0, 632, FACING_SOUTH, 0, 680, 568, 928, 768, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_FROM_WEST_ROOM, CONDITION_ALWAYS, 888, 0, 632, FACING_SOUTH, 0, 680, 568, 928, 768, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR, CONDITION_ALWAYS, 136, 0, 568, FACING_SOUTH, 0, 8, 480, 256, 648, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_ROOM_FROM_PILLAR_SOCKETS, CONDITION_ALWAYS, 136, 0, 600, FACING_NORTH, 0, 8, 480, 256, 648, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST, CONDITION_ALWAYS, 440, 0, 264, FACING_NORTH, 0, 264, 56, 720, 480, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST, CONDITION_ALWAYS, 568, 0, 264, FACING_NORTH, 0, 264, 56, 720, 480, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVenusLighthouseFourthFloorEntrances[] = {
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_NORTHWEST, CONDITION_ALWAYS, 120, 0, 248, FACING_SOUTH, 0, 72, 56, 320, 416, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLARS_FROM_WALL_SWITCHES, CONDITION_ALWAYS, 280, 0, 280, FACING_NORTH, 0, 72, 56, 320, 416, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLARS, CONDITION_ALWAYS, 472, 0, 328, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHWEST, CONDITION_ALWAYS, 536, 0, 216, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHEAST, CONDITION_ALWAYS, 664, 0, 88, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE, CONDITION_ALWAYS, 744, 0, 152, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHEAST, CONDITION_ALWAYS, 840, 0, 152, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLAR_SOCKETS_EAST, CONDITION_ALWAYS, 936, 0, 280, FACING_NORTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST, CONDITION_ALWAYS, 472, 0, 472, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_GAIA_BLADE_WEST, CONDITION_ALWAYS, 616, 0, 456, FACING_NORTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_GAIA_BLADE_EAST, CONDITION_ALWAYS, 728, 0, 456, FACING_NORTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLAR_SOCKETS_SOUTH, CONDITION_ALWAYS, 824, 0, 456, FACING_NORTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_FROM_WALL_SWITCHES_WEST, CONDITION_ALWAYS, 792, 0, 648, FACING_SOUTH, 0, 712, 584, 976, 864, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_FROM_WALL_SWITCHES_EAST, CONDITION_ALWAYS, 904, 0, 648, FACING_SOUTH, 0, 712, 584, 976, 864, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_FROM_WALL_SWITCHES_SOUTH, CONDITION_ALWAYS, 104, 0, 776, FACING_SOUTH, 0, 56, 536, 304, 848, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_FROM_WALL_SWITCHES_EAST, CONDITION_ALWAYS, 216, 0, 600, FACING_SOUTH, 0, 56, 536, 304, 848, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_FROM_WALL_SWITCHES, CONDITION_ALWAYS, 600, 0, 744, FACING_SOUTH, 0, 392, 648, 640, 832, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST, CONDITION_ALWAYS, 536, 0, 328, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST, CONDITION_ALWAYS, 792, 0, 328, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE_RISING, CONDITION_ALWAYS, 664, 0, 136, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { SCENE_TABLE_END },
};

u32 gVenusLighthousePrimaryTable[] = {
    SCENE_EXITS(VenusLighthouseEntry),
    SCENE_EXIT(VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_VENUS_LIGHTHOUSE_ENTRY_EAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_ENTRY_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_ENTRY),
    SCENE_EXIT(VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_WEST, WorldMap, WORLD_MAP_FROM_VENUS_LIGHTHOUSE_ENTRY_WEST),
    SCENE_EXITS(VenusLighthouseGroundFloor),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_ENTRY, VenusLighthouseEntry, VENUS_LIGHTHOUSE_ENTRY_FROM_VENUS_LIGHTHOUSE_GROUND_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_FRONT_HALL_TO_RELIEFS, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_FRONT_HALL),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_FRONT_HALL, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_FRONT_HALL_FROM_RELIEFS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_EAST, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_FROM_RELIEFS_EAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_WEST, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_FROM_RELIEFS_WEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_PILLARS, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_RELIEFS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_RELIEFS, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_PILLARS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_CARRY_STONE, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_FROM_PILLARS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_FLOOR_SWITCH, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_PILLARS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_NORTH, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_FROM_PILLARS_NORTH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_WEST, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_FROM_PILLARS_WEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_NORTH, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_STATUE_ROW_NORTH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_WEST, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_STATUE_ROW_WEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_STAIRWELL, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_STAIRWELL_FROM_STATUE_ROW),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_FLOOR_SWITCH, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_GREAT_STATUE),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_STAIRWELL, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_STAIRWELL_FROM_GREAT_STATUE),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_PILLARS, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_FLOOR_SWITCH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_GROUND_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_STAIRWELL_TO_STATUE_ROW, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_FROM_STAIRWELL),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_STAIRWELL_TO_GREAT_STATUE, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_FROM_STAIRWELL),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_GREAT_STATUE, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_FROM_FLOOR_SWITCH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_WEST, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_CARRY_STONE_WEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_EAST, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_CARRY_STONE_EAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_PILLARS, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_CARRY_STONE),
    SCENE_EXITS(VenusLighthouseFirstFloor),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_LOWER_ROOM, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_FROM_DRIFTING_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_GAP, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_FROM_DRIFTING_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_NORTH, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_NORTH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_THUNDER_CROWN, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_FROM_DRIFTING_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_DRIFTING_FLOOR, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_THUNDER_CROWN),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_LADDERS, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_THUNDER_CROWN),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_PILLARS_TO_LADDERS, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_PILLARS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_EAST, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_EAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_DRIFTING_FLOOR, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_GAP),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_LADDERS, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_GAP),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_GAP, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_FROM_LADDERS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_WEST, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_WEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_THUNDER_CROWN, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_FROM_LADDERS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PILLARS, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_PILLARS_FROM_LADDERS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PSYNERGY_STONE, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE_FROM_LADDERS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE_TO_LADDERS, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_PSYNERGY_STONE),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_TUNNEL_RUINS_LADY_STATUES, TunnelRuinsLadyStatues, TUNNEL_RUINS_LADY_STATUES_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_TO_DRIFTING_FLOOR, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_LOWER_ROOM),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_LOWER_ROOM_HOLE, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_FROM_DRIFTING_FLOOR_HOLE),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH),
    SCENE_EXITS(VenusLighthouseSecondFloor),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_SPARKLING_PASSAGE_TO_PILLAR_PIT, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_FROM_SPARKLING_PASSAGE),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_SPARKLING_PASSAGE, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_SPARKLING_PASSAGE_FROM_PILLAR_PIT),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_WEST, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_WEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_GATE, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_FROM_PILLAR_PIT),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_NORTH, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_NORTH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_WALL_SWITCH_TO_GATE, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_FROM_WALL_SWITCH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_WALL_SWITCH, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_WALL_SWITCH_FROM_GATE),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_PILLAR_PIT, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_FROM_GATE),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_EAST, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_EAST),
    SCENE_EXITS(VenusLighthouseThirdFloor),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_WEST_NORTHWEST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_NORTHWEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST_ROOM_TO_SPARKLING_PASSAGES, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_NORTHWEST_ROOM),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_DRAGON_SCALES_TO_SPARKLING_PASSAGES, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_DRAGON_SCALES),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_NORTHWEST_ROOM, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST_ROOM_FROM_SPARKLING_PASSAGES),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_DRAGON_SCALES, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRAGON_SCALES_FROM_SPARKLING_PASSAGES),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_DRIFTING_FLOOR, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SPARKLING_PASSAGES),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHEAST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHEAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST_ROOM_TO_DRIFTING_FLOOR, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SOUTHWEST_ROOM),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_SPARKLING_PASSAGES, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_DRIFTING_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHWEST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHWEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_SOUTHWEST_ROOM, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST_ROOM_FROM_DRIFTING_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHEAST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHEAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_SOUTHWEST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_PILLAR_SOCKETS, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_FROM_DRIFTING_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_DRIFTING_FLOOR, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_PILLAR_SOCKETS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_WEST_ROOM, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_ROOM_FROM_PILLAR_SOCKETS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_ROOM_TO_PILLAR_SOCKETS, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_FROM_WEST_ROOM),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST),
    SCENE_EXITS(VenusLighthouseFourthFloor),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_NORTHWEST, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_WEST_NORTHWEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLARS_TO_WALL_SWITCHES, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLARS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLARS, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLARS_FROM_WALL_SWITCHES),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHWEST, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHWEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHEAST, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHEAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PSYNERGY_STONE, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_FROM_WALL_SWITCHES),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHEAST, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHEAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_EAST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_FROM_WALL_SWITCHES_EAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_SOUTHWEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_WEST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_FROM_WALL_SWITCHES_WEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_EAST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_FROM_WALL_SWITCHES_EAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_SOUTH, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_FROM_WALL_SWITCHES_SOUTH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_WEST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_GAIA_BLADE_WEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_EAST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_GAIA_BLADE_EAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_SOUTH, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLAR_SOCKETS_SOUTH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_EAST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLAR_SOCKETS_EAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_TO_WALL_SWITCHES, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_TO_WALL_SWITCHES_RISING, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE_RISING),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_AERIE, VenusLighthouseAerie, VENUS_LIGHTHOUSE_AERIE_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR),
    SCENE_EXITS_END,
};

struct ScenePlacement gVenusLighthousePlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gVenusLighthouseEntryPlacements[] = {
    { SPRITE_SOLDIER, FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD, ACTOR_STAND,
        PIXELS(128), 0, PIXELS(268), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x70, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(352), 0, PIXELS(300), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x70, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(428), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x70, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(368), 0, PIXELS(140), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(400), 0, PIXELS(172), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(416), 0, PIXELS(412), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(192), 0, PIXELS(364), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVenusLighthouseGroundFloorPlacements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x70, 0x0, ACTOR_STAND,
        PIXELS(128), 0, PIXELS(140), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x71, 0x0, ACTOR_STAND,
        PIXELS(304), 0, PIXELS(204), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x70, 0x0, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(592), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x100, 0x0, ACTOR_STAND,
        PIXELS(872), 0, PIXELS(120), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, ACTOR_STAND,
        PIXELS(840), 0, PIXELS(88), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LADY_STATUE, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(664), 0, PIXELS(792), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x114, 0x0, (s32)gVenusLighthousePlacements2_1,
        PIXELS(488), 0, PIXELS(536), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LADY_STATUE, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(72), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LADY_STATUE, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(72), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LADY_STATUE, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(168), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LADY_STATUE, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(168), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LADY_STATUE, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(72), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LADY_STATUE, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(104), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LADY_STATUE, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(136), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LADY_STATUE, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(168), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LADY_STATUE, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(200), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVenusLighthouseFirstFloorPlacements[] = {
    { SPRITE_CAPPED_PILLAR, 0x0, (s32)gVenusLighthousePlacements3_Data,
        PIXELS(248), 0, PIXELS(712), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_CAPPED_PILLAR, 0x0, (s32)gVenusLighthousePlacements3_Data,
        PIXELS(184), 0, PIXELS(824), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CAPPED_PILLAR, 0x0, (s32)gVenusLighthousePlacements3_Data,
        PIXELS(200), 0, PIXELS(824), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x100, 0x0, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(392), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(376), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVenusLighthouseSecondFloorPlacements[] = {
    { SPRITE_LADY_STATUE, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(808), 0, PIXELS(728), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, (s32)gVenusLighthousePlacements4_1,
        PIXELS(776), 0, PIXELS(712), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_PARTICLE, CONDITION_ALWAYS, 7,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVenusLighthouseThirdFloorPlacements[] = {
    { SPRITE_LADY_STATUE, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(888), 0, PIXELS(680), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, (s32)gVenusLighthousePlacements5_1,
        PIXELS(792), 0, PIXELS(680), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, (s32)gVenusLighthousePlacements5_2,
        PIXELS(808), 0, PIXELS(648), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, (s32)gVenusLighthousePlacements5_3,
        PIXELS(824), 0, PIXELS(680), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_PARTICLE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVenusLighthouseFourthFloorPlacements[] = {
    { SPRITE_BALANCE_PLATFORM, 0x0, 7,
        PIXELS(104), 0, PIXELS(312), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BALANCE_PLATFORM, 0x0, 7,
        PIXELS(168), 0, PIXELS(312), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x100, 0x0, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(280), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(280), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x100, 0x0, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(280), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x100, 0x0, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(120), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LADY_STATUE, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(136), 0, PIXELS(776), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, (s32)gVenusLighthousePlacements4_1,
        PIXELS(184), 0, PIXELS(616), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, (s32)gVenusLighthousePlacements5_1,
        PIXELS(200), 0, PIXELS(616), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, (s32)gVenusLighthousePlacements6_3,
        PIXELS(232), 0, PIXELS(616), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, (s32)gVenusLighthousePlacements6_4,
        PIXELS(248), 0, PIXELS(616), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_PARTICLE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_FLOATING_PLATFORM, 0x0, 7,
        PIXELS(664), 0, PIXELS(136), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gVenusLighthouseEntryEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_EAST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_ENTRY_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_WEST },
    { EVENT_TALK, ACTOR_ENTRANCE_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HearOfTheAttack) },
    { EVENT_TALK, ACTOR_SCHOLAR_LIGHTHOUSE_OPEN, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIodemBeHappyTheEntranceTo) },
    { EVENT_TALK, ACTOR_SCHOLAR_BARBARIANS, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseOhhhHowCouldTheyAttackMere) },
    { EVENT_TALK, ACTOR_WAITING_SCHOLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWaitingScholar) },
    { EVENT_TALK, ACTOR_SOLDIER_OPENED_DOOR, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseHowHowDidTheyOpenThe) },
    { EVENT_TALK, ACTOR_SOLDIER_HEADED_FOR_BABI, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheyAreAlreadyHeadedForBabi) },
    { EVENT_TALK, ACTOR_SOLDIER_COLOSSO_VICTORS, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseBabisSoldiersAreAllColossoVictors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_ENTRANCE_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HearOfTheAttack) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_LIGHTHOUSE_OPEN, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseUhnnnIfOnlyICouldMove) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_BARBARIANS, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheirPowersWereSoStrangeWho) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WAITING_SCHOLAR, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIMustGetInsideVenusLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_OPENED_DOOR, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheYoungBoySeemedToGlow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_HEADED_FOR_BABI, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheyWerentInVenusLighthouseVery) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_COLOSSO_VICTORS, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIThoughtIRecognizedOneOf) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVenusLighthouseGroundFloorEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_ENTRY },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_FRONT_HALL_TO_RELIEFS },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_FRONT_HALL },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 4, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_EAST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 5, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_WEST },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_PILLARS },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_RELIEFS },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 8, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_CARRY_STONE },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 9, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_FLOOR_SWITCH },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_NORTH },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_WEST },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_NORTH },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_WEST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 14, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_STAIRWELL },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 20, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_FLOOR_SWITCH },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 21, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_STAIRWELL },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 22, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_PILLARS },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 23, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 24, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_STAIRWELL_TO_STATUE_ROW },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 25, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_STAIRWELL_TO_GREAT_STATUE },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 26, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_GREAT_STATUE },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 30, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_WEST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 31, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_EAST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 32, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_PILLARS },
    { EVENT_TOUCH, 45, FLAG_SCENE_TEMP_00, EVENT_SCRIPT(Event_JumpDown) },
    { EVENT_TOUCH, 46, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnFloorSwitch) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 50, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillarTowardWall) },
    { EVENT_TALK, ACTOR_SCHOLAR_ATTACKED_ANYWAY, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseKradenToldThemToStopBut) },
    { EVENT_TALK, ACTOR_SCHOLAR_NOT_THE_ENTRANCE, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseKradenWentInsideButThenCame) },
    { EVENT_TALK, ACTOR_DOOR_SCHOLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToScholarByDoor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_ATTACKED_ANYWAY, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIfKradenHadntBeenThereWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_NOT_THE_ENTRANCE, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseKradenSaidSomethingAboutThisBeing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOOR_SCHOLAR, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIWonderWhatMysteriesTheStatue) },
    { SEARCH(SEARCH_UNNAMED), 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRelief) },
#if LANGUAGE == LANGUAGE_JAPANESE
    { SEARCH(SEARCH_UNNAMED), 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRelief) },
    { SEARCH(SEARCH_UNNAMED), 37, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRelief) },
    { SEARCH(SEARCH_UNNAMED), 38, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRelief) },
    { SEARCH(SEARCH_UNNAMED), 39, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRelief) },
#endif
#if !TBS_PATCH_1 || TBS_PATCH_2
    { SEARCH(SEARCH_UNNAMED), 40, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRelief) },
#endif
    { SEARCH(SEARCH_UNNAMED), 41, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchStatue) },
    { SEARCH(SEARCH_CHEST), 100, 0xf37, FIND_ITEM(ITEM_LUCKY_CAP) },
    { SEARCH(SEARCH_CHEST), 101, 0xf38, FIND_ITEM(ITEM_CARRY_STONE) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WALL_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MovePillarTowardWall) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FLOOR_SWITCH_STATUE, FLAG_SCENE_TEMP_00, EVENT_SCRIPT(Event_MovePillarOntoSwitch) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_WALL_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryPillarPastWall) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RevealPath) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HidePath) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVenusLighthouseFirstFloorEvents[] = {
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 1, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_LOWER_ROOM },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_GAP },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 3, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_NORTH },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_THUNDER_CROWN },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_DRIFTING_FLOOR },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_LADDERS },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_PILLARS_TO_LADDERS },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 8, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_EAST },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_DRIFTING_FLOOR },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_LADDERS },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_GAP },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 12, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_WEST },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_THUNDER_CROWN },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PILLARS },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PSYNERGY_STONE },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 16, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR },
    { EVENT_EXIT, 17, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE_TO_LADDERS },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 18, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_TUNNEL_RUINS_LADY_STATUES },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 19, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_TO_DRIFTING_FLOOR },
    { EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftSouth) },
    { EVENT_TOUCH, 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_FallThroughFloor) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushDepthPillar) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushDepthPillar) },
    { EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ClearEffectTarget) },
    { EVENT_TOUCH, 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SetLeaderAsEffectTarget) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 45, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpOrPushCarryPillar) },
    { SEARCH(SEARCH_CHEST), 100, 0xf34, FIND_ITEM(ITEM_THUNDER_CROWN) },
    { SEARCH(SEARCH_UNNAMED), 110, 0x350, FIND_PSYNERGY_STONE },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_DEPTH_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveDepthPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_DEPTH_PILLAR + 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveDepthPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_DEPTH_PILLAR + 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveDepthPillar) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_CARRY_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LiftCarryPillar) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_CARRY_PILLAR + 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LiftCarryPillar) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_CARRY_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LiftCarryPillar) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_CARRY_PILLAR + 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LiftCarryPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CARRY_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveCarryPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CARRY_PILLAR + 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveCarryPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_CARRY_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryPillarAcrossGap) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_CARRY_PILLAR + 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryPillarAcrossGap) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVenusLighthouseSecondFloorEvents[] = {
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 1, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSparklingPassage) },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_SPARKLING_PASSAGE },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 4, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_WEST },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_GATE },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 6, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 7, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_NORTH },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_WALL_SWITCH_TO_GATE },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_WALL_SWITCH },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_PILLAR_PIT },
    { EVENT_TOUCH, 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnWallSwitch) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 36, FLAG_VENUS_LIGHTHOUSE_GATE_OPEN, EVENT_SCRIPT(Event_JumpOrPushGatePillar) },
    { SEARCH(SEARCH_UNNAMED), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchLockedDoor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GATE_PILLAR, FLAG_VENUS_LIGHTHOUSE_GATE_OPEN, EVENT_SCRIPT(Event_MoveGatePillar) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVenusLighthouseThirdFloorEvents[] = {
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 1, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_WEST_NORTHWEST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST_ROOM_TO_SPARKLING_PASSAGES },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRAGON_SCALES_TO_SPARKLING_PASSAGES },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSparklingPassage) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSparklingPassage) },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_DRIFTING_FLOOR },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 7, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHEAST },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST_ROOM_TO_DRIFTING_FLOOR },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_SPARKLING_PASSAGES },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 10, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHWEST },
    { TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSparklingPassage) },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 12, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHEAST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 13, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_SOUTHWEST },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_PILLAR_SOCKETS },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_DRIFTING_FLOOR },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_WEST_ROOM },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 17, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_ROOM_TO_PILLAR_SOCKETS },
    { EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftSouth) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftSouth) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpOrPushSwitchPillar) },
    { SEARCH(SEARCH_UNNAMED), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchLockedDoor) },
    { SEARCH(SEARCH_CHEST), 100, 0xf35, FIND_ITEM(ITEM_DRAGON_SCALES) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SWITCH_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveSwitchPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SWITCH_PILLAR + 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveSwitchPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SWITCH_PILLAR + 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveSwitchPillar) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVenusLighthouseFourthFloorEvents[] = {
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 1, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_NORTHWEST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLARS_TO_WALL_SWITCHES },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLARS },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 4, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHWEST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 5, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHEAST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 6, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PSYNERGY_STONE },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 7, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHEAST },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_EAST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 9, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_WEST },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_EAST },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_SOUTH },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_WEST },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_EAST },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_SOUTH },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_EAST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 17, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_TO_WALL_SWITCHES },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 18, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterRisingPassage) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPuzzlePillar) },
    { TOUCH_WALKING_INTO(FACING_WEST), 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpAcrossGap) },
    { TOUCH_WALKING_INTO(FACING_EAST), 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpAcrossGap) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPuzzlePillar) },
    { TOUCH_WALKING_INTO(FACING_EAST), 37, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpAcrossGap) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 37, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPuzzlePillar) },
    { EVENT_TOUCH, 40, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnWestWallSwitch) },
    { EVENT_TOUCH, 41, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnEastWallSwitch) },
    { EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftSouth) },
    { EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftNorth) },
    { EVENT_TOUCH, 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftWest) },
    { EVENT_TOUCH, 33, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftEast) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 45, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpOrPushSocketPillar) },
    { SEARCH(SEARCH_UNNAMED), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchLockedDoor) },
    { SEARCH(SEARCH_CHEST), 100, 0xf36, FIND_ITEM(ITEM_GAIA_BLADE) },
    { SEARCH(SEARCH_UNNAMED), 110, 0x351, FIND_PSYNERGY_STONE },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_FLOATING_PILLAR, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_FLOATING_PILLAR + 1, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_FLOATING_PILLAR + 2, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_FLOATING_PILLAR + 3, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_FLOATING_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryFloatingPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_FLOATING_PILLAR + 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryFloatingPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_FLOATING_PILLAR + 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryFloatingPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_FLOATING_PILLAR + 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryFloatingPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SOCKET_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveSocketPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SOCKET_PILLAR + 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveSocketPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SOCKET_PILLAR + 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveSocketPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SOCKET_PILLAR + 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveSocketPillar) },
    { SCENE_EVENTS_END },
};
// clang-format on

const u8 *gVenusSwitchShapes[] = {
    gVenusSwitchShape0,
    gVenusSwitchShape1,
    gVenusSwitchShape2,
    gVenusSwitchShape3,
};

s32 *gVenusSwitchPushScript0[] = {
    gVenusSwitchPushScript0_0,
    gVenusSwitchPushScript0_1,
    gVenusRefusedPushScript,
    gVenusRefusedPushScript,
};

s32 *gVenusSwitchPushScript1[] = {
    gVenusSwitchPushScript1_0,
    gVenusSwitchPushScript1_1,
    gVenusRefusedPushScript,
    gVenusRefusedPushScript,
};

s32 *gVenusSwitchPushScript2[] = {
    gVenusSwitchPushScript2_0,
    gVenusSwitchPushScript2_1,
    gVenusSwitchPushScript2_0,
    gVenusRefusedPushScript,
};

s32 *gVenusSwitchPushScript3[] = {
    gVenusSwitchPushScript3_0,
    gVenusSwitchPushScript3_1,
    gVenusSwitchPushScript3_0,
    gVenusRefusedPushScript,
};

s32 **gVenusSwitchPushScripts[] = {
    gVenusSwitchPushScript0,
    gVenusSwitchPushScript1,
    gVenusSwitchPushScript2,
    gVenusSwitchPushScript3,
};

const u8 *gVenusPillarShapes[] = {
    gVenusPillarShape0,     gVenusPillarShape1,     gVenusPillarShape1 + 1,
    gVenusPillarShape2 + 1, gVenusPillarShape3 + 1, gVenusPillarShape4 + 1,
    gVenusPillarShape5,     gVenusPillarShape6,
};

s32 *gVenusPillarPushScript0[] = {
    gVenusPillarPushScript0_0,
    gVenusRefusedPushScript,
};

s32 *gVenusPillarPushScript1[] = {
    gVenusRefusedPushScript,
};

s32 *gVenusPillarPushScript2[] = {
    gVenusPillarPushScript2_0,
    gVenusRefusedPushScript,
};

s32 *gVenusPillarPushScript3[] = {
    gVenusPillarPushScript3_0,
    gVenusRefusedPushScript,
};

s32 *gVenusPillarPushScript4[] = {
    gVenusPillarPushScript4_0,
    gVenusRefusedPushScript,
};

s32 *gVenusPillarPushScript5[] = {
    gVenusPillarPushScript5_0,
    gVenusPillarPushScript5_1,
    gVenusRefusedPushScript,
};

s32 *gVenusPillarPushScript6[] = {
    gVenusPillarPushScript6_0,
    gVenusRefusedPushScript,
};

s32 *gVenusPillarPushScript7[] = {
    gVenusPillarPushScript7_0,
    gVenusRefusedPushScript,
};

s32 **gVenusPillarPushScripts[] = {
    gVenusPillarPushScript0, gVenusPillarPushScript1, gVenusPillarPushScript2,
    gVenusPillarPushScript3, gVenusPillarPushScript4, gVenusPillarPushScript5,
    gVenusPillarPushScript6, gVenusPillarPushScript7,
};
