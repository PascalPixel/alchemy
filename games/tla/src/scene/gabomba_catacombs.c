/// @file
/// The Gabomba Catacombs, under the Gabomba Statue, in the rooms of this
/// overlay: stones hidden under bushes that carry the party a level down once
/// Cyclone clears them, and the Venus Djinni Mud, who fights
/// (GabombaCatacombs_FightMud).

#include "flags.h"
#include "shared/tables.h"
#include "field/field_sequence_event.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_tables.h"
#include "system/frame.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "module/scene/scene.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "version.h"
#include "objcmd.h"

void Actor_SelectAnimation(struct Actor *object, s32 mode);
void Actor_StartScript(struct Actor *object, const void *script);
struct GabombaCatacombsVec;
s32 MapGrid_CheckCollision(struct Actor *object,
                           struct GabombaCatacombsVec *position);
void Event_End(void);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
void Scene_TalkToActor(s32 a);
void Cyclone_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void FieldItem_JumpToLeader(s32 a, s32 b, s32 c);

extern u32 gGabombaCatacombs_SprayScript[];

extern char SceneId_GabombaCatacombs1;
extern char SceneId_GabombaCatacombs2;
extern char SceneId_GabombaCatacombs3;
extern char SceneId_GabombaCatacombs4;
extern char SceneId_GabombaCatacombs5;

static __inline__ void GabombaCatacombs_ClearBushes(s32 a, s32 b, s32 c, s32 d,
                                                    s32 e, s32 f, s32 g) {
  Cyclone_CopyTouchedCells(a, b, c, d, e, f, g);
}

static __inline__ void GabombaCatacombs_GiveItem(s32 a, s32 b, s32 c) {
  FieldItem_JumpToLeader(a, b, c);
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

struct GabombaCatacombsParty {
  u8 unk000[0x254];
  u16 value254;
};

extern u32 gFieldEffect_MediumScript[];
extern u32 gGabombaCatacombs_Room1Regions[];
extern struct ActorPlacement gGabombaCatacombs_Placements[];
extern struct ActorPlacement gGabombaCatacombs_Room1Placements[];
extern struct ActorPlacement gGabombaCatacombs_Room2Placements[];
extern struct ActorPlacement gGabombaCatacombs_Room3Placements[];
extern struct ActorPlacement gGabombaCatacombs_Room4Placements[];
extern struct ActorPlacement gGabombaCatacombs_Room5Placements[];
extern struct Trigger gGabombaCatacombs_Room1Triggers[];
extern struct Trigger gGabombaCatacombs_Room2Triggers[];
extern struct Trigger gGabombaCatacombs_Room3Triggers[];
extern struct Trigger gGabombaCatacombs_Room4Triggers[];
extern struct Trigger gGabombaCatacombs_Room5Triggers[];

void Actor_DepartInRing(s32 id);

#include "text/msg_ids.h"

#define LEADER_HOP()                                                           \
  do {                                                                         \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    FieldEffect_SetAttached(gMainRoutineWork.currentOwner,                     \
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);  \
  } while (0)

void GabombaCatacombs_FightMud(void) { Field_RequestDjinnBattle(8, 10); }

void GabombaCatacombs_ClearRoom1Bushes(void) {
  GabombaCatacombs_ClearBushes(7, 63, 1, 16, 16, 57, 63);
}

void GabombaCatacombs_ClearRoom3Bushes(void) {
  GabombaCatacombs_ClearBushes(22, 15, 0, 5, 5, 63, 52);
}

void GabombaCatacombs_ClearRoom3SmallBushes(void) {
  GabombaCatacombs_ClearBushes(7, 22, 0, 3, 3, 56, 58);
}

void GabombaCatacombs_ClearRoom5Bushes(void) {
  GabombaCatacombs_ClearBushes(44, 25, 0, 4, 3, 66, 57);
}

void GabombaCatacombs_ClearRoom5WideBushes(void) {
  GabombaCatacombs_ClearBushes(16, 13, 0, 4, 8, 61, 52);
}

void GabombaCatacombs_ClearRoom5SmallBushes(void) {
  GabombaCatacombs_ClearBushes(21, 9, 0, 2, 2, 57, 52);
}

// Cyclone on the bushes before the statue, hopping the leader off the cleared
// patch.
void GabombaCatacombs_ClearBushesByTheStatue(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  GabombaCatacombs_ClearBushes(22, 15, 0, 5, 5, 63, 52);
  if (GameFlag_Get(0xf53) == 0) {
    Actor_SetPosition(64, 392, 344);
    if (leader->x >> 20 == 24 && leader->z >> 20 == 21) {
      LEADER_HOP();
      Actor_SetDestinationOffsetById(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = px(6);
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Event_End();
    }
  }
}

// Cyclone on the bushes uncovers the teleporter stone, which carries the party
// a level down.
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
    struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);       \
                                                                               \
    GabombaCatacombs_ClearBushes(n1, n2, 0, n3, 3, 56, n4);                    \
    Actor_SetPosition(n5, n6, n7);                                             \
    if (leader->x >> 20 == n8 && leader->z >> 20 == n9) {                      \
      LEADER_HOP();                                                            \
      Actor_SetDestinationOffsetById(gMainRoutineWork.currentOwner, 16, 0);    \
      leader->velocityY = px(6);                                               \
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);                    \
      Event_End();                                                             \
    }                                                                          \
  }

DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(
    GabombaCatacombs_UncoverFirstPuddle, 7, 22, 3, 58, 8, 136, 376, 8, 23)

DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(
    GabombaCatacombs_UncoverSecondPuddle, 5, 13, 5, 52, 9, 136, 248, 8, 15)

void GabombaCatacombs_UncoverThirdPuddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  GabombaCatacombs_ClearBushes(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(10, 152, 264);
  if (leader->x >> 20 == 9 && leader->z >> 20 == 16) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldEffect_SetAttached(gMainRoutineWork.currentOwner,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetDestinationOffsetById(gMainRoutineWork.currentOwner, -16, 0);
    leader->velocityY = px(6);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    Event_End();
  }
}

DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(
    GabombaCatacombs_UncoverFourthPuddle, 5, 13, 5, 52, 11, 136, 280, 8, 17)

DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(
    GabombaCatacombs_UncoverFifthPuddle, 5, 13, 5, 52, 12, 104, 280, 6, 17)

DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(
    GabombaCatacombs_UncoverSixthPuddle, 5, 13, 5, 52, 13, 88, 264, 5, 16)

DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(
    GabombaCatacombs_UncoverSeventhPuddle, 5, 13, 5, 52, 14, 104, 248, 6, 15)

void GabombaCatacombs_UncoverRoom5Puddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  GabombaCatacombs_ClearBushes(16, 13, 0, 4, 8, 61, 52);
  Actor_SetPosition(8, 280, 296);
  if (leader->x >> 20 == 17 && leader->z >> 20 == 18) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldEffect_SetAttached(gMainRoutineWork.currentOwner,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, -16);
    leader->velocityY = px(6);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    Event_End();
  }
}

void GabombaCatacombs_ClearRoom4Bushes(void) {
  GabombaCatacombs_ClearBushes(7, 23, 0, 4, 5, 60, 56);
}

// Cyclone on the last bushes: a Mad Plant hides there, and attacks.
void GabombaCatacombs_UncoverMadPlant(void) {
  GabombaCatacombs_ClearBushes(7, 23, 0, 4, 5, 60, 56);
  if (GameFlag_Get(0x9b5) == 0) {
    GabombaCatacombs_GiveItem(249, 168, 424);
    ((struct GabombaCatacombsParty *)&gMainRoutineWork)->value254 = 0x19b5;
    gMainRoutineWork.battleRule24b = 2;
    Field_RequestBattleFromEncounterTable(106, 0);
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

DEFINE_GABOMBA_CATACOMBS_SET_MOVED(GabombaCatacombs_FreezeRoom5Puddle, 8, 0x200,
                                   15, 18, 17, 18)

// Moves the leader and camera along z by dz when nothing is in the way.
void GabombaCatacombs_DriftWithStream(struct Actor *object, s32 dz) {
  struct GabombaCatacombsFollower *follower;
  struct GabombaCatacombsVec position;

  follower = gFieldWork->camera;

  position.x = object->x;
  position.y = object->y;
  position.z = object->z + dz;
  MapGrid_CheckCollision(object, &position);
  position.x = object->x;
  position.y = object->y;
  position.z = object->z + px(8);
  if (MapGrid_CheckCollision(object, &position) <= 0) {
    position.x = object->x + 0x5b333;
    position.y = object->y;
    position.z = object->z + 0x5b333;
    if (MapGrid_CheckCollision(object, &position) <= 0) {
      position.x = object->x - 0x5b333;
      position.y = object->y;
      position.z = object->z + 0x5b333;
      if (MapGrid_CheckCollision(object, &position) <= 0) {
        follower->z += dz;
        object->z += dz;
      }
    }
  }
}

// Walking in the stream: spray rises every eighth frame and the water pushes
// the leader along.
void GabombaCatacombs_WadeStream(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *steam;

  if ((gFrameCount & 7) == 0) {
    steam = ((struct Actor *)Import_ActorCreate(14, leader->x,
                                                leader->y + px(6), leader->z));
    if (steam) {
      Actor_SetSpriteFlags(steam, 0);
      Actor_SelectAnimation(steam, 1);
      Actor_StartScript(steam, gGabombaCatacombs_SprayScript);
    }
    Sound_Play(SE_623);
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

// At the end of the catacombs the Great Gabomba speaks: "By reaching this
// place, you have proven your power."
void GabombaCatacombs_GabombaBestowsPower(void) {
  struct Actor *follower = Actor_GetById(9);
  struct Actor *statue = Actor_GetById(64);
  u8 *work = (u8 *)Ram_HeapSlots->fieldWork;
  struct Actor *glow, *leader;
  s32 speed;

  Event_Start();
  Event_ResolvePendingActions(0);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x403108, 1);
  PaletteFade_StartInterpolation(60);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 158, 1);
  Sound_Play(MUS_026);
  Event_Wait(60);
  Actor_SetModeById(8, 2);
  Sound_Play(SE_576);
  Event_Wait(60);
  Event_SetMessageId((s32)MsgByReachingThis);
  work += 0x1c4;
  TALK();
  Actor_SetPosition(64, 168, 152);
  statue->motionFlags = 6;
  statue->terrainHeight += px(16);
  Sound_Play(SE_SHIMMER);
  glow = ((struct Actor *)Import_ActorCreate(412, 0, 0, 0));
  Actor_StartScript(glow, gFieldEffect_MediumScript);
  Actor_SetSpriteColorMode(glow, 1);
  glow->motionFlags = 0;
  glow->x = statue->x;
  glow->y = statue->y + px(8);
  glow->z = statue->z;
  glow->terrainHeight = statue->terrainHeight;
  glow->update = GabombaCatacombs_ShrinkSpray;
  Event_Wait(15);
  glow->x = 0;
  glow->y = 0;
  glow->z = 0;
  glow->update = 0;
  TALK();
  Actor_SetDestinationById(gMainRoutineWork.currentOwner, 166, 188);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_MoveToObject(9, gMainRoutineWork.currentOwner);
  speed = 0x10000;
  follower->speedLimit = speed;
  Actor_SetDestinationById(9, 184, 188);
  Actor_WaitForMoveById(9);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Event_ShowMessageAndWait(9, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  TALK();
  Actor_ShowEmoteById(9, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_FaceDirection(9, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(9, 0);
  TALK();
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(30);
  TALK();
  Actor_ShowEmoteById(9, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(9, 0);
  TALK();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_ArmCallback(9, 0x8000, 0);
  Event_Wait(10);
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmoteById(9, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  TALK();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 0);
  Actor_FaceDirection(9, FACING_NORTH_NORTHWEST, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmoteById(9, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(60);
  TALK();
  Event_Wait(40);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Event_Wait(40);
  Actor_SetModeById(9, 3);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetModeById(8, 2);
  TALK();
  Event_Wait(30);
  Actor_SetModeById(8, 4);
  Actor_SetModeById(9, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestinationById(9, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(9);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Actor_WaitForMoveById(9);
  GameFlag_Set(0x9ce);
  Scene_PlayMusic();
  PaletteFade_ApplyTarget(speed, 0);
  PaletteFade_StartInterpolation(60);
  FieldCamera_MoveToActorById(gMainRoutineWork.currentOwner, 1);
  Event_Wait(60);
  Event_End();
}

void GabombaCatacombs_TalkToStatue(void) {
  struct Actor *object = Actor_GetById(64);

  if (object != NULL && object->x != 0)
    Scene_TalkToActor(4160);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_GabombaCatacombs1)
    return (const struct SceneRegion *)gGabombaCatacombs_Room1Regions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct Trigger gGabombaCatacombs_Triggers[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_GabombaCatacombs1)
    return (const struct ActorPlacement *)gGabombaCatacombs_Room1Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_GabombaCatacombs2)
    return (const struct ActorPlacement *)gGabombaCatacombs_Room2Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_GabombaCatacombs3)
    return (const struct ActorPlacement *)gGabombaCatacombs_Room3Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_GabombaCatacombs4)
    return (const struct ActorPlacement *)gGabombaCatacombs_Room4Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_GabombaCatacombs5)
    return (const struct ActorPlacement *)gGabombaCatacombs_Room5Placements;
  return (const struct ActorPlacement *)gGabombaCatacombs_Placements;
}

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_GabombaCatacombs1)
    return gGabombaCatacombs_Room1Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_GabombaCatacombs2)
    return gGabombaCatacombs_Room2Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_GabombaCatacombs3)
    return gGabombaCatacombs_Room3Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_GabombaCatacombs4)
    return gGabombaCatacombs_Room4Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_GabombaCatacombs5)
    return gGabombaCatacombs_Room5Triggers;
  return gGabombaCatacombs_Triggers;
}

#if !TLA_PATCH_1
#define CATACOMBS_SCREEN_EFFECT 513
#else
#define CATACOMBS_SCREEN_EFFECT 516
#endif

extern char SceneId_GabombaStatue7, SceneId_GabombaStatueChamber1;
void FieldEffect_StartBlankOverlay(s32 a);

s32 Scene_Initialize(void) {
  s32 entrance;
  struct Actor *statue;

  gFieldWork->transition = CATACOMBS_SCREEN_EFFECT;
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_GabombaCatacombs1 &&
      (entrance = gMainRoutineWork.entrance) == 1) {
    if (GameFlag_Get(0xa27)) {
      gMainRoutineWork.retreatScene = (s32)&SceneId_GabombaStatue7;
      gMainRoutineWork.retreatEntrance = entrance;
    } else if (GameFlag_Get(0xa28)) {
      gMainRoutineWork.retreatScene = (s32)&SceneId_GabombaStatueChamber1;
      gMainRoutineWork.retreatEntrance = 3;
    }
  }
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
      gMainRoutineWork.scene == (s32)&SceneId_GabombaCatacombs2 &&
      gMainRoutineWork.entrance > 2)
    Actor_DepartInRing(gMainRoutineWork.currentOwner);
  if (gMainRoutineWork.scene == (s32)&SceneId_GabombaCatacombs3) {
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
  if (gMainRoutineWork.scene == (s32)&SceneId_GabombaCatacombs5 &&
      GameFlag_Get(FLAG_SCENE_TEMP_00))
    MapGrid_CopyCellAttributes(15, 18, 1, 1, 17, 18);
  if (gMainRoutineWork.scene == (s32)&SceneId_GabombaCatacombs4 &&
      GameFlag_Get(0x9ce) && !GameFlag_Get(0xf93) &&
      !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    statue = Actor_GetById(64);
    Actor_SetPosition(64, 168, 152);
    statue->motionFlags = 6;
    statue->terrainHeight += px(16);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

// clang-format off
u32 gGabombaCatacombs_SprayScript[] = {
    0, 15, 3, 0, 1, 38,
};

u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gGabombaCatacombs_Room1Regions[] = {
    0x2e0184, 0x18c0114, 0x11c0036, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(GabombaCatacombs1),
    SCENE_EXIT(1, GabombaStatueChamber3, 2),
    SCENE_EXIT(2, GabombaCatacombs2, 1),
    SCENE_EXIT(3, GabombaCatacombs2, 3),
    SCENE_EXIT(4, GabombaCatacombs2, 4),
    SCENE_EXIT(5, GabombaCatacombs2, 5),
    SCENE_EXITS(GabombaCatacombs2),
    SCENE_EXIT(1, GabombaCatacombs1, 2),
    SCENE_EXIT(2, GabombaCatacombs3, 1),
    SCENE_EXITS(GabombaCatacombs3),
    SCENE_EXIT(1, GabombaCatacombs2, 2),
    SCENE_EXIT(2, GabombaCatacombs5, 1),
    SCENE_EXITS(GabombaCatacombs4),
    SCENE_EXIT(1, GabombaCatacombs5, 2),
    SCENE_EXITS(GabombaCatacombs5),
    SCENE_EXIT(1, GabombaCatacombs3, 2),
    SCENE_EXIT(2, GabombaCatacombs4, 1),
    SCENE_EXITS_END,
};

struct ActorPlacement gGabombaCatacombs_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gGabombaCatacombs_Room1Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gGabombaCatacombs_Room2Placements[] = {
    ACTOR_PLACEMENT(243, 0x3a, MOVEMENT_TYPE_STAND, 728, 0, 312, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGabombaCatacombs_Room3Placements[] = {                                             
      {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {SCENE_TABLE_END},                                                       
  };

struct ActorPlacement gGabombaCatacombs_Room4Placements[] = {
    ACTOR_PLACEMENT(375, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGabombaCatacombs_Room5Placements[] = {
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gGabombaCatacombs_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaCatacombs_Room1Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ClearRoom1Bushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 43,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverFirstTeleporter)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 44,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverSecondTeleporter)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 45,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_TakeThirdTeleporter)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaCatacombs_Room2Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TALK_TRIGGER_FACING(FACING_NORTH), 8, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_FightMud)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaCatacombs_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_RevealWall)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_HideWall)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 40,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ClearRoom3Bushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ClearRoom3SmallBushes)},
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
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, 0x200,
     EVENT(GabombaCatacombs_FreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 9, 0x201,
     EVENT(GabombaCatacombs_FreezeSecondPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 10, 0x202,
     EVENT(GabombaCatacombs_FreezeThirdPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, 0x203,
     EVENT(GabombaCatacombs_FreezeFourthPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 12, 0x204,
     EVENT(GabombaCatacombs_FreezeFifthPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 13, 0x205,
     EVENT(GabombaCatacombs_FreezeSixthPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 14, 0x206,
     EVENT(GabombaCatacombs_FreezeSeventhPuddle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaCatacombs_Room4Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, 40, 0x9ce, EVENT(GabombaCatacombs_GabombaBestowsPower)},
    {TRIGGER_TALK, 8, 0xf93, EVENT(GabombaCatacombs_TalkToStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ClearRoom4Bushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 31,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverMadPlant)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaCatacombs_Room5Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 40,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ClearRoom5Bushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ClearRoom5WideBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 43,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ClearRoom5SmallBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 42,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverRoom5Puddle)},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(GabombaCatacombs_WadeStream)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, 0x200,
     EVENT(GabombaCatacombs_FreezeRoom5Puddle)},
    {TRIGGER_TABLE_END},
};
