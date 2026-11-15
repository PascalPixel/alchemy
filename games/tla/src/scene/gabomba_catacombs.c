/// @file
/// The Gabomba Catacombs, under the Gabomba Statue, in the rooms of this
/// overlay: stones hidden under bushes that carry the party a level down once
/// Cyclone clears them, and the Venus Djinni Mud, who fights
/// (GabombaCatacombs_FightMud).

#include "flags.h"
/// Set as the Great Gabomba bestows its power
/// (GabombaCatacombs_GabombaBestowsPower).
#define FLAG_GABOMBA_BESTOWED_POWER 0x9ce
#include "djinn_ids.h"
#include "shared/tables.h"
#include "field/field_sequence_event.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/scene_tables.h"
#include "system/frame.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "scene/scene.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "version.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "field/event_commands.h"

void Actor_StartScript(struct Actor *object, const void *script);
struct GabombaCatacombsVec;
s32 MapGrid_CheckCollision(struct Actor *object,
                           struct GabombaCatacombsVec *position);
void Event_End(void);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
void Cyclone_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void MadPlant_LeapAtLeader(s32 a, s32 b, s32 c);

extern u32 gGabombaCatacombs_SprayScript[];

static __inline__ void GabombaCatacombs_ClearBushes(s32 a, s32 b, s32 c, s32 d,
                                                    s32 e, s32 f, s32 g) {
  Cyclone_CopyTouchedCells(a, b, c, d, e, f, g);
}

static __inline__ void GabombaCatacombs_MadPlantLeaps(s32 a, s32 b, s32 c) {
  MadPlant_LeapAtLeader(a, b, c);
}

struct GabombaCatacombsVec {
  s32 x;
  s32 y;
  s32 z;
};

struct GabombaCatacombsFollower {
  u8 unk00[0x10];
  s32 z;
};

extern u32 gFieldEffect_MediumScript[];
extern u32 gGabombaCatacombs_TeleportersExitRegions[];
extern struct ActorPlacement gGabombaCatacombs_Placements[];
extern struct ActorPlacement gGabombaCatacombs_TeleportersActorPlacements[];
extern struct ActorPlacement gGabombaCatacombs_MudActorPlacements[];
extern struct ActorPlacement gGabombaCatacombs_HiddenWallActorPlacements[];
extern struct ActorPlacement gGabombaCatacombs_TomegathericonActorPlacements[];
extern struct ActorPlacement gGabombaCatacombs_StreamActorPlacements[];
extern struct Trigger gGabombaCatacombs_TeleportersTriggers[];
extern struct Trigger gGabombaCatacombs_MudTriggers[];
extern struct Trigger gGabombaCatacombs_HiddenWallTriggers[];
extern struct Trigger gGabombaCatacombs_TomegathericonTriggers[];
extern struct Trigger gGabombaCatacombs_StreamTriggers[];

void Actor_DepartInRing(s32 id);

#include "text/msg_ids.h"

#define LEADER_HOP()                                                           \
  do {                                                                         \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,                    \
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);  \
  } while (0)

/// Mud, the Djinni waiting in this room.
enum { ACTOR_MUD = 8 };

/// The hidden wall's seven puddles under the bushes Cyclone clears, the
/// stream's puddle, and the Tomegathericon's room: the seated idol the Great
/// Gabomba speaks through, and Kraden.
enum {
  ACTOR_PUDDLE_A = 8,
  ACTOR_PUDDLE_B,
  ACTOR_PUDDLE_C,
  ACTOR_PUDDLE_D,
  ACTOR_PUDDLE_E,
  ACTOR_PUDDLE_F,
  ACTOR_PUDDLE_G
};
enum { ACTOR_STREAM_PUDDLE = 8 };
enum { ACTOR_SEATED_IDOL = 8, ACTOR_KRADEN };

void GabombaCatacombs_FightMud(void) {
  Field_RequestDjinnBattle(ACTOR_MUD, DJINNI_MUD);
}

void GabombaCatacombs_ClearTeleportersBushes(void) {
  GabombaCatacombs_ClearBushes(7, 63, 1, 16, 16, 57, 63);
}

void GabombaCatacombs_ClearHiddenWallBushes(void) {
  GabombaCatacombs_ClearBushes(22, 15, 0, 5, 5, 63, 52);
}

void GabombaCatacombs_ClearHiddenWallSmallBushes(void) {
  GabombaCatacombs_ClearBushes(7, 22, 0, 3, 3, 56, 58);
}

void GabombaCatacombs_ClearStreamBushes(void) {
  GabombaCatacombs_ClearBushes(44, 25, 0, 4, 3, 66, 57);
}

void GabombaCatacombs_ClearStreamWideBushes(void) {
  GabombaCatacombs_ClearBushes(16, 13, 0, 4, 8, 61, 52);
}

void GabombaCatacombs_ClearStreamSmallBushes(void) {
  GabombaCatacombs_ClearBushes(21, 9, 0, 2, 2, 57, 52);
}

/// Cyclone on the bushes before the statue, hopping the leader off the cleared
/// patch.
void GabombaCatacombs_ClearBushesByTheStatue(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  GabombaCatacombs_ClearBushes(22, 15, 0, 5, 5, 63, 52);
  if (GameFlag_Get(0xf53) == 0) {
    Actor_SetPosition(FIELD_TREASURE_ACTOR, 392, 344);
    if (FIELD_CELL(leader->x.fixed) == 24 &&
        FIELD_CELL(leader->z.fixed) == 21) {
      LEADER_HOP();
      Actor_SetDestinationOffsetById(gMainRoutineWork.leaderActorId, 16, 0);
      leader->velocityY = px(6);
      Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
      Event_End();
    }
  }
}

/// Cyclone on the bushes uncovers the teleporter stone, which carries the party
/// a level down.
void GabombaCatacombs_UncoverFirstTeleporter(void) {
  MapGrid_CopyCellsLayered(51, 83, 3, 1, 9, 65);
  Import_EventSetExitRequest(3);
}

void GabombaCatacombs_UncoverSecondTeleporter(void) {
  MapGrid_CopyCellsLayered(51, 83, 3, 1, 18, 66);
  Import_EventSetExitRequest(4);
}

void GabombaCatacombs_TakeThirdTeleporter(void) {
  Import_EventSetExitRequest(5);
}

#define DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(name, n1, n2, n3, n4, n5, \
                                                     n6, n7, n8, n9)           \
  void name(void) {                                                            \
    struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);      \
                                                                               \
    GabombaCatacombs_ClearBushes(n1, n2, 0, n3, 3, 56, n4);                    \
    Actor_SetPosition(n5, n6, n7);                                             \
    if (FIELD_CELL(leader->x.fixed) == n8 &&                                   \
        FIELD_CELL(leader->z.fixed) == n9) {                                   \
      LEADER_HOP();                                                            \
      Actor_SetDestinationOffsetById(gMainRoutineWork.leaderActorId, 16, 0);   \
      leader->velocityY = px(6);                                               \
      Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);                   \
      Event_End();                                                             \
    }                                                                          \
  }

DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(
    GabombaCatacombs_UncoverFirstPuddle, 7, 22, 3, 58, ACTOR_PUDDLE_A, 136, 376,
    8, 23)

DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(
    GabombaCatacombs_UncoverSecondPuddle, 5, 13, 5, 52, ACTOR_PUDDLE_B, 136,
    248, 8, 15)

void GabombaCatacombs_UncoverThirdPuddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  GabombaCatacombs_ClearBushes(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(ACTOR_PUDDLE_C, 152, 264);
  if (FIELD_CELL(leader->x.fixed) == 9 && FIELD_CELL(leader->z.fixed) == 16) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetDestinationOffsetById(gMainRoutineWork.leaderActorId, -16, 0);
    leader->velocityY = px(6);
    Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
    Event_End();
  }
}

DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(
    GabombaCatacombs_UncoverFourthPuddle, 5, 13, 5, 52, ACTOR_PUDDLE_D, 136,
    280, 8, 17)

DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(
    GabombaCatacombs_UncoverFifthPuddle, 5, 13, 5, 52, ACTOR_PUDDLE_E, 104, 280,
    6, 17)

DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(
    GabombaCatacombs_UncoverSixthPuddle, 5, 13, 5, 52, ACTOR_PUDDLE_F, 88, 264,
    5, 16)

DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(
    GabombaCatacombs_UncoverSeventhPuddle, 5, 13, 5, 52, ACTOR_PUDDLE_G, 104,
    248, 6, 15)

void GabombaCatacombs_UncoverStreamPuddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  GabombaCatacombs_ClearBushes(16, 13, 0, 4, 8, 61, 52);
  Actor_SetPosition(ACTOR_STREAM_PUDDLE, 280, 296);
  if (FIELD_CELL(leader->x.fixed) == 17 && FIELD_CELL(leader->z.fixed) == 18) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 0, -16);
    leader->velocityY = px(6);
    Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
    Event_End();
  }
}

void GabombaCatacombs_ClearTomegathericonBushes(void) {
  GabombaCatacombs_ClearBushes(7, 23, 0, 4, 5, 60, 56);
}

/// Cyclone on the last bushes: a Mad Plant hides there, and attacks.
void GabombaCatacombs_UncoverMadPlant(void) {
  GabombaCatacombs_ClearBushes(7, 23, 0, 4, 5, 60, 56);
  if (GameFlag_Get(FLAG_MAD_PLANT(0)) == 0) {
    GabombaCatacombs_MadPlantLeaps(SPRITE_MAD_PLANT, 168, 424);
    gMainRoutineWork.pendingDjinni = IF_FLAG_SET(FLAG_MAD_PLANT(0));
    Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
    Field_RequestBattleFromEncounterTable(ENCOUNTER_TABLE_MAD_PLANTS, 0);
  }
}

void GabombaCatacombs_RevealWall(void) {
  MapGrid_CopyCellPictureCellsRect(0, 0, 1, 2, 7, 11);
  Map_CopyCellAttributeRect(2, 4, 1, 1, 7, 14);
}

void GabombaCatacombs_HideWall(void) {
  MapGrid_CopyCellPictureCellsRect(1, 0, 1, 2, 7, 11);
  Map_CopyCellAttributeRect(2, 5, 1, 1, 7, 14);
}

#define DEFINE_GABOMBA_CATACOMBS_SET_MOVED(name, n1, n2, n3, n4, n5, n6)       \
  void name(void) {                                                            \
    struct Actor *object = Actor_GetById(n1);                                  \
                                                                               \
    GameFlag_Set(n2);                                                          \
    object->motionFlags = 0;                                                   \
    object->priorityFlags = 0;                                                 \
    MapGrid_CopyCellAttributes(n3, n4, 1, 1, n5, n6);                          \
  }

DEFINE_GABOMBA_CATACOMBS_SET_MOVED(GabombaCatacombs_FreezeFirstPuddle, 8, 0x200,
                                   6, 23, 8, 23)

DEFINE_GABOMBA_CATACOMBS_SET_MOVED(GabombaCatacombs_FreezeSecondPuddle, 9,
                                   0x201, 0, 4, 8, 15)

DEFINE_GABOMBA_CATACOMBS_SET_MOVED(GabombaCatacombs_FreezeThirdPuddle, 10,
                                   0x202, 0, 4, 9, 16)

DEFINE_GABOMBA_CATACOMBS_SET_MOVED(GabombaCatacombs_FreezeFourthPuddle, 11,
                                   0x203, 0, 4, 8, 17)

DEFINE_GABOMBA_CATACOMBS_SET_MOVED(GabombaCatacombs_FreezeFifthPuddle, 12,
                                   0x204, 0, 4, 6, 17)

DEFINE_GABOMBA_CATACOMBS_SET_MOVED(GabombaCatacombs_FreezeSixthPuddle, 13,
                                   0x205, 0, 4, 5, 16)

DEFINE_GABOMBA_CATACOMBS_SET_MOVED(GabombaCatacombs_FreezeSeventhPuddle, 14,
                                   0x206, 0, 4, 6, 15)

DEFINE_GABOMBA_CATACOMBS_SET_MOVED(GabombaCatacombs_FreezeStreamPuddle, 8,
                                   0x200, 15, 18, 17, 18)

/// Moves the leader and camera along z by dz when nothing is in the way.
void GabombaCatacombs_DriftWithStream(struct Actor *object, s32 dz) {
  struct GabombaCatacombsFollower *follower;
  struct GabombaCatacombsVec position;

  follower = gFieldWork->camera;

  position.x = object->x.fixed;
  position.y = object->y.fixed;
  position.z = object->z.fixed + dz;
  MapGrid_CheckCollision(object, &position);
  position.x = object->x.fixed;
  position.y = object->y.fixed;
  position.z = object->z.fixed + px(8);
  if (MapGrid_CheckCollision(object, &position) <= 0) {
    position.x = object->x.fixed + 0x5b333;
    position.y = object->y.fixed;
    position.z = object->z.fixed + 0x5b333;
    if (MapGrid_CheckCollision(object, &position) <= 0) {
      position.x = object->x.fixed - 0x5b333;
      position.y = object->y.fixed;
      position.z = object->z.fixed + 0x5b333;
      if (MapGrid_CheckCollision(object, &position) <= 0) {
        follower->z += dz;
        object->z.fixed += dz;
      }
    }
  }
}

/// Walking in the stream: spray rises every eighth frame and the water pushes
/// the leader along.
void GabombaCatacombs_WadeStream(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *steam;

  if ((gFrameCount & 7) == 0) {
    steam = ((struct Actor *)Import_ActorCreate(
        14, leader->x.fixed, leader->y.fixed + px(6), leader->z.fixed));
    if (steam) {
      Actor_SetSpriteFlags(steam, 0);
      Actor_SelectAnimation(steam, 1);
      Actor_StartScript(steam, gGabombaCatacombs_SprayScript);
    }
    Sound_Play(SE_WADE);
  }
  GabombaCatacombs_DriftWithStream(leader, 55705);
}

s32 GabombaCatacombs_ShrinkSpray(struct Actor *object) {
  object->scaleX -= 3276;
  object->scaleY -= 3276;
  return 0;
}

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void Actor_MoveToObject(s32 actor, s32 target);

#define TALK()                                                                 \
  EventMessage_ShowCentered((s16)((union EventLine *)work)->line++, 0, -3)

/// At the end of the catacombs the Great Gabomba speaks: "By reaching this
/// place, you have proven your power."
void GabombaCatacombs_GabombaBestowsPower(void) {
  struct Actor *follower = Actor_GetById(ACTOR_KRADEN);
  struct Actor *statue = Actor_GetById(FIELD_TREASURE_ACTOR);
  u8 *work = (u8 *)Ram_HeapSlots->fieldWork;
  struct Actor *glow, *leader;
  s32 speed;

  Event_Start();
  Event_ResolvePendingActions(0);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x403108, 1);
  PaletteFade_StartInterpolation(60);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 158, 1);
  Sound_Play(MUS_JUPITER_LIGHTHOUSE);
  Event_Wait(60);
  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 2);
  Sound_Play(SE_576);
  Event_Wait(60);
  Event_SetMessageId((s32)MsgByReachingThis);
  work += 0x1c4;
  TALK();
  Actor_SetPosition(FIELD_TREASURE_ACTOR, 168, 152);
  statue->motionFlags = 6;
  statue->groundHeight += px(16);
  Sound_Play(SE_SHIMMER);
  glow = ((struct Actor *)Import_ActorCreate(SPRITE_FIREBALL, 0, 0, 0));
  Actor_StartScript(glow, gFieldEffect_MediumScript);
  Actor_SetSpriteColorMode(glow, 1);
  glow->motionFlags = 0;
  glow->x.fixed = statue->x.fixed;
  glow->y.fixed = statue->y.fixed + px(8);
  glow->z.fixed = statue->z.fixed;
  glow->groundHeight = statue->groundHeight;
  glow->update = GabombaCatacombs_ShrinkSpray;
  Event_Wait(15);
  glow->x.fixed = 0;
  glow->y.fixed = 0;
  glow->z.fixed = 0;
  glow->update = 0;
  TALK();
  Actor_SetDestinationById(gMainRoutineWork.leaderActorId, 166, 188);
  Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  speed = 0x10000;
  follower->speedLimit = speed;
  Actor_SetDestinationById(ACTOR_KRADEN, 184, 188);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  // "By reaching this place, you have proven your power."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  TALK();
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  // "By rights, Akafubu should bear this power... But I shall keep my..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  TALK();
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(30);
  TALK();
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  Event_Wait(30);
  // "I am grateful for your promise... But I am worried that we have done..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  TALK();
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_WEST, 0);
  Event_Wait(10);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  TALK();
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(60);
  TALK();
  Event_Wait(40);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Event_Wait(40);
  Actor_SetAnimationById(ACTOR_KRADEN, 3);
  // "Akafubu has not honed his own powers, and the fault lies with him..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 2);
  TALK();
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 4);
  Actor_SetAnimationById(ACTOR_KRADEN, 2);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_KRADEN, leader->x.part[0].pixel,
                             leader->z.part[0].pixel);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  GameFlag_Set(FLAG_GABOMBA_BESTOWED_POWER);
  Scene_PlayMusic();
  PaletteFade_ApplyTarget(speed, 0);
  PaletteFade_StartInterpolation(60);
  FieldCamera_MoveToActorById(gMainRoutineWork.leaderActorId, 1);
  Event_Wait(60);
  Event_End();
}

void GabombaCatacombs_TalkToStatue(void) {
  struct Actor *object = Actor_GetById(FIELD_TREASURE_ACTOR);

  if (object != NULL && object->x.fixed != 0)
    Scene_TalkToActor(4160);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) {
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_TELEPORTERS)
    return (const struct SceneRegion *)gGabombaCatacombs_TeleportersExitRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct Trigger gGabombaCatacombs_Triggers[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_TELEPORTERS)
    return (const struct ActorPlacement *)
        gGabombaCatacombs_TeleportersActorPlacements;
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_MUD)
    return (const struct ActorPlacement *)gGabombaCatacombs_MudActorPlacements;
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_HIDDEN_WALL)
    return (const struct ActorPlacement *)
        gGabombaCatacombs_HiddenWallActorPlacements;
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_TOMEGATHERICON)
    return (const struct ActorPlacement *)
        gGabombaCatacombs_TomegathericonActorPlacements;
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_STREAM)
    return (
        const struct ActorPlacement *)gGabombaCatacombs_StreamActorPlacements;
  return (const struct ActorPlacement *)gGabombaCatacombs_Placements;
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_TELEPORTERS)
    return gGabombaCatacombs_TeleportersTriggers;
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_MUD)
    return gGabombaCatacombs_MudTriggers;
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_HIDDEN_WALL)
    return gGabombaCatacombs_HiddenWallTriggers;
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_TOMEGATHERICON)
    return gGabombaCatacombs_TomegathericonTriggers;
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_STREAM)
    return gGabombaCatacombs_StreamTriggers;
  return gGabombaCatacombs_Triggers;
}

#if !TLA_PATCH_1
#define CATACOMBS_SCREEN_EFFECT 513
#else
#define CATACOMBS_SCREEN_EFFECT 516
#endif

extern char SceneId_GabombaStatuePinion, SceneId_GabombaStatueChamberIdol;
void FieldEffect_StartBlankOverlay(s32 a);

s32 Scene_Initialize(void) {
  s32 entrance;
  struct Actor *statue;

  gFieldWork->transition = CATACOMBS_SCREEN_EFFECT;
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_TELEPORTERS &&
      (entrance = gMainRoutineWork.entrance) == 1) {
    if (GameFlag_Get(0xa27)) {
      gMainRoutineWork.retreatScene = SCENE_GABOMBA_STATUE_PINION;
      gMainRoutineWork.retreatEntrance = entrance;
    } else if (GameFlag_Get(0xa28)) {
      gMainRoutineWork.retreatScene = SCENE_GABOMBA_STATUE_CHAMBER_IDOL;
      gMainRoutineWork.retreatEntrance = 3;
    }
  }
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
      gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_MUD &&
      gMainRoutineWork.entrance > 2)
    Actor_DepartInRing(gMainRoutineWork.leaderActorId);
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_HIDDEN_WALL) {
    if (GameFlag_Get(FLAG_SCENE_TEMP_00))
      MapGrid_CopyCellAttributes(6, 23, 1, 1, 8, 23);
    if (GameFlag_Get(FLAG_SCENE_TEMP_01))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 8, 15);
    if (GameFlag_Get(FLAG_SCENE_TEMP_02))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 9, 16);
    if (GameFlag_Get(FLAG_SCENE_TEMP_03))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 8, 17);
    if (GameFlag_Get(FLAG_SCENE_TEMP_04))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 6, 17);
    if (GameFlag_Get(FLAG_SCENE_TEMP_05))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 5, 16);
    if (GameFlag_Get(FLAG_SCENE_TEMP_06))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 6, 15);
  }
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_STREAM &&
      GameFlag_Get(FLAG_SCENE_TEMP_00))
    MapGrid_CopyCellAttributes(15, 18, 1, 1, 17, 18);
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_TOMEGATHERICON &&
      GameFlag_Get(FLAG_GABOMBA_BESTOWED_POWER) && !GameFlag_Get(0xf93) &&
      !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    statue = Actor_GetById(FIELD_TREASURE_ACTOR);
    Actor_SetPosition(FIELD_TREASURE_ACTOR, 168, 152);
    statue->motionFlags = 6;
    statue->groundHeight += px(16);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

// clang-format off
u32 gGabombaCatacombs_SprayScript[] = {
    SC_WAIT(15),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gGabombaCatacombs_TeleportersExitRegions[] = {
    0x2e0184, 0x18c0114, 0x11c0036, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(GabombaCatacombsTeleporters),
    SCENE_EXIT(1, GabombaStatueChamberWitchDoctor, 2),
    SCENE_EXIT(2, GabombaCatacombsMud, 1),
    SCENE_EXIT(3, GabombaCatacombsMud, 3),
    SCENE_EXIT(4, GabombaCatacombsMud, 4),
    SCENE_EXIT(5, GabombaCatacombsMud, 5),
    SCENE_EXITS(GabombaCatacombsMud),
    SCENE_EXIT(1, GabombaCatacombsTeleporters, 2),
    SCENE_EXIT(2, GabombaCatacombsHiddenWall, 1),
    SCENE_EXITS(GabombaCatacombsHiddenWall),
    SCENE_EXIT(1, GabombaCatacombsMud, 2),
    SCENE_EXIT(2, GabombaCatacombsStream, 1),
    SCENE_EXITS(GabombaCatacombsTomegathericon),
    SCENE_EXIT(1, GabombaCatacombsStream, 2),
    SCENE_EXITS(GabombaCatacombsStream),
    SCENE_EXIT(1, GabombaCatacombsHiddenWall, 2),
    SCENE_EXIT(2, GabombaCatacombsTomegathericon, 1),
    SCENE_EXITS_END,
};

struct ActorPlacement gGabombaCatacombs_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gGabombaCatacombs_TeleportersActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gGabombaCatacombs_MudActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_MUD), MOVEMENT_TYPE_STAND, 728, 0, 312, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGabombaCatacombs_HiddenWallActorPlacements[] = {                                             
      {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {SCENE_TABLE_END},                                                       
  };

struct ActorPlacement gGabombaCatacombs_TomegathericonActorPlacements[] = {
    ACTOR_PLACEMENT(375, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGabombaCatacombs_StreamActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gGabombaCatacombs_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaCatacombs_TeleportersTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ClearTeleportersBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 43,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverFirstTeleporter)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 44,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverSecondTeleporter)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 45,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_TakeThirdTeleporter)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaCatacombs_MudTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TALK_TRIGGER_FACING(FACING_NORTH), 8, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_FightMud)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaCatacombs_HiddenWallTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_RevealWall)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_HideWall)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 40,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ClearHiddenWallBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ClearHiddenWallSmallBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 42,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverFirstPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 43,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverSecondPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 44,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverThirdPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 45,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverFourthPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 46,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverFifthPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 47,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverSixthPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 48,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverSeventhPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 49,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ClearBushesByTheStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, FLAG_SCENE_TEMP_00,
     EVENT(GabombaCatacombs_FreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 9, FLAG_SCENE_TEMP_01,
     EVENT(GabombaCatacombs_FreezeSecondPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 10, FLAG_SCENE_TEMP_02,
     EVENT(GabombaCatacombs_FreezeThirdPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, FLAG_SCENE_TEMP_03,
     EVENT(GabombaCatacombs_FreezeFourthPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 12, FLAG_SCENE_TEMP_04,
     EVENT(GabombaCatacombs_FreezeFifthPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 13, FLAG_SCENE_TEMP_05,
     EVENT(GabombaCatacombs_FreezeSixthPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 14, FLAG_SCENE_TEMP_06,
     EVENT(GabombaCatacombs_FreezeSeventhPuddle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaCatacombs_TomegathericonTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, 40, FLAG_GABOMBA_BESTOWED_POWER,
     EVENT(GabombaCatacombs_GabombaBestowsPower)},
    {TRIGGER_TALK, 8, 0xf93, EVENT(GabombaCatacombs_TalkToStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ClearTomegathericonBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 31,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverMadPlant)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaCatacombs_StreamTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 40,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ClearStreamBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ClearStreamWideBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 43,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ClearStreamSmallBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 42,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverStreamPuddle)},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(GabombaCatacombs_WadeStream)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, FLAG_SCENE_TEMP_00,
     EVENT(GabombaCatacombs_FreezeStreamPuddle)},
    {TRIGGER_TABLE_END},
};
