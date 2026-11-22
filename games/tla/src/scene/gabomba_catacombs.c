/// @file
/// The Gabomba Catacombs, under the Gabomba Statue, in the rooms of this
/// overlay: stones hidden under weeds that carry the party a level down once
/// Cyclone clears them, and the Venus Djinni Mud, who fights
/// (GabombaCatacombs_FightMud).

#include "flags.h"
/// Set as the Great Gabomba bestows its power
/// (GabombaCatacombs_GabombaBestowsPower).
#define FLAG_GABOMBA_BESTOWED_POWER 0x9ce
#include "djinn_ids.h"
#include "shared/tables.h"
#include "field/field_sequence_event.h"
#include "field/field_sequence.h"
#include "types.h"
#include "graphics/palette_buffers.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/scene_tables.h"
#include "system/frame.h"
#include "system/scene.h"
#include "field/event_inline.h"
#include "scene/event_layer.h"
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

void Actor_StartScript(struct Actor *actor, const void *script);
s32 MapGrid_CheckCollision(struct Actor *actor, struct FieldPosition *position);
void Event_End(void);

extern u32 gGabombaCatacombs_SplashScript[];

/// A Mad Plant of the given sprite leaps at the leader from (x, z).
static __inline__ void GabombaCatacombs_MadPlantLeaps(s32 sprite, s32 x,
                                                      s32 z) {
  MadPlant_LeapAtLeader(sprite, x, z);
}

extern u32 gFieldEffect_MediumScript[];
extern struct SceneRegion gGabombaCatacombs_TeleportersExitRegions[];
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

/// Whether the leader stands on the cell (cellX, cellZ).
#define LEADER_ON_CELL(leader, cellX, cellZ)                                   \
  (FIELD_CELL((leader)->x.fixed) == (cellX) &&                                 \
   FIELD_CELL((leader)->z.fixed) == (cellZ))

#define LEADER_HOP()                                                           \
  do {                                                                         \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,                    \
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);  \
  } while (0)

/// Mud, the Djinni waiting in this room.
enum { ACTOR_MUD = ACTOR_FIRST_PLACED };

/// The hidden wall's seven puddles under the weeds Cyclone clears, the
/// stream's puddle, and the Tomegathericon's room: the seated idol the Great
/// Gabomba speaks through, and Kraden.
enum {
  ACTOR_FIRST_PUDDLE = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_PUDDLE,
  ACTOR_THIRD_PUDDLE,
  ACTOR_FOURTH_PUDDLE,
  ACTOR_FIFTH_PUDDLE,
  ACTOR_SIXTH_PUDDLE,
  ACTOR_SEVENTH_PUDDLE
};
enum { ACTOR_STREAM_PUDDLE = ACTOR_FIRST_PLACED };
enum { ACTOR_SEATED_IDOL = ACTOR_FIRST_PLACED, ACTOR_KRADEN };

/// Each puddle Frost has frozen into a pillar, by the puddles above.
#define FLAG_GABOMBA_CATACOMBS_FIRST_PUDDLE_FROZEN FLAG_SCENE_TEMP_00
#define FLAG_GABOMBA_CATACOMBS_SECOND_PUDDLE_FROZEN FLAG_SCENE_TEMP_01
#define FLAG_GABOMBA_CATACOMBS_THIRD_PUDDLE_FROZEN FLAG_SCENE_TEMP_02
#define FLAG_GABOMBA_CATACOMBS_FOURTH_PUDDLE_FROZEN FLAG_SCENE_TEMP_03
#define FLAG_GABOMBA_CATACOMBS_FIFTH_PUDDLE_FROZEN FLAG_SCENE_TEMP_04
#define FLAG_GABOMBA_CATACOMBS_SIXTH_PUDDLE_FROZEN FLAG_SCENE_TEMP_05
#define FLAG_GABOMBA_CATACOMBS_SEVENTH_PUDDLE_FROZEN FLAG_SCENE_TEMP_06
#define FLAG_GABOMBA_CATACOMBS_STREAM_PUDDLE_FROZEN FLAG_SCENE_TEMP_00

/// Talking to Mud starts its battle.
void GabombaCatacombs_FightMud(void) {
  Field_RequestDjinnBattle(ACTOR_MUD, DJINNI_MUD);
}

/// Cyclone on the weeds of the teleport stones' room.
void GabombaCatacombs_ScatterTeleportersWeeds(void) {
  FieldCyclone_CopyTouchedCells(7, 63, 1, 16, 16, 57, 63);
}

/// Cyclone on the weeds by the statue in the hidden wall's room.
void GabombaCatacombs_ScatterHiddenWallWeeds(void) {
  FieldCyclone_CopyTouchedCells(22, 15, 0, 5, 5, 63, 52);
}

/// Cyclone on the small weeds of the hidden wall's room.
void GabombaCatacombs_ScatterHiddenWallSmallWeeds(void) {
  FieldCyclone_CopyTouchedCells(7, 22, 0, 3, 3, 56, 58);
}

/// Cyclone on the stream room's weeds.
void GabombaCatacombs_ScatterStreamWeeds(void) {
  FieldCyclone_CopyTouchedCells(44, 25, 0, 4, 3, 66, 57);
}

/// Cyclone on the stream room's wide weeds.
void GabombaCatacombs_ScatterStreamWideWeeds(void) {
  FieldCyclone_CopyTouchedCells(16, 13, 0, 4, 8, 61, 52);
}

/// Cyclone on the stream room's small weeds.
void GabombaCatacombs_ScatterStreamSmallWeeds(void) {
  FieldCyclone_CopyTouchedCells(21, 9, 0, 2, 2, 57, 52);
}

/// Cyclone on the weeds before the statue: until it is taken, the Mint hidden
/// there shows, and the leader hops a cell east off its spot.
void GabombaCatacombs_ScatterWeedsByTheStatue(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  FieldCyclone_CopyTouchedCells(22, 15, 0, 5, 5, 63, 52);
  if (GameFlag_Get(FLAG_GABOMBA_CATACOMBS_MINT_TAKEN) == 0) {
    Actor_SetPosition(FIELD_TREASURE_ACTOR, 392, 344);
    if (LEADER_ON_CELL(leader, 24, 21)) {
      LEADER_HOP();
      Actor_SetDestinationOffsetById(gMainRoutineWork.leaderActorId, 16, 0);
      leader->velocityY = toFIXED(6);
      Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
      Event_End();
    }
  }
}

/// Cyclone on the weeds uncovers the first teleport stone, which carries the
/// party a level down.
void GabombaCatacombs_UncoverFirstTeleportStone(void) {
  MapGrid_CopyCellsLayered(51, 83, 3, 1, 9, 65);
  Import_EventSetExitRequest(
      GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_WEST);
}

/// Cyclone on the weeds uncovers the second teleport stone, which carries
/// the party a level down.
void GabombaCatacombs_UncoverSecondTeleportStone(void) {
  MapGrid_CopyCellsLayered(51, 83, 3, 1, 18, 66);
  Import_EventSetExitRequest(
      GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_EAST);
}

/// Cyclone on the third teleport stone carries the party a level down.
void GabombaCatacombs_TakeThirdTeleportStone(void) {
  Import_EventSetExitRequest(
      GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_SOUTH);
}

/// Cyclone on the weeds over the first puddle uncovers it, hopping the
/// leader off it if he stands there.
void GabombaCatacombs_UncoverFirstPuddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  FieldCyclone_CopyTouchedCells(7, 22, 0, 3, 3, 56, 58);
  Actor_SetPosition(ACTOR_FIRST_PUDDLE, 136, 376);
  if (LEADER_ON_CELL(leader, 8, 23)) {
    LEADER_HOP();
    Actor_SetDestinationOffsetById(gMainRoutineWork.leaderActorId, 16, 0);
    leader->velocityY = toFIXED(6);
    Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
    Event_End();
  }
}

/// Cyclone on the weeds over the second puddle uncovers it, hopping the
/// leader off it if he stands there.
void GabombaCatacombs_UncoverSecondPuddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  FieldCyclone_CopyTouchedCells(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(ACTOR_SECOND_PUDDLE, 136, 248);
  if (LEADER_ON_CELL(leader, 8, 15)) {
    LEADER_HOP();
    Actor_SetDestinationOffsetById(gMainRoutineWork.leaderActorId, 16, 0);
    leader->velocityY = toFIXED(6);
    Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
    Event_End();
  }
}

/// Cyclone on the weeds over the third puddle uncovers it, hopping the leader
/// off it westward if he stands there.
void GabombaCatacombs_UncoverThirdPuddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  FieldCyclone_CopyTouchedCells(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(ACTOR_THIRD_PUDDLE, 152, 264);
  if (LEADER_ON_CELL(leader, 9, 16)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetDestinationOffsetById(gMainRoutineWork.leaderActorId, -16, 0);
    leader->velocityY = toFIXED(6);
    Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
    Event_End();
  }
}

/// Cyclone on the weeds over the fourth puddle uncovers it, hopping the
/// leader off it if he stands there.
void GabombaCatacombs_UncoverFourthPuddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  FieldCyclone_CopyTouchedCells(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(ACTOR_FOURTH_PUDDLE, 136, 280);
  if (LEADER_ON_CELL(leader, 8, 17)) {
    LEADER_HOP();
    Actor_SetDestinationOffsetById(gMainRoutineWork.leaderActorId, 16, 0);
    leader->velocityY = toFIXED(6);
    Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
    Event_End();
  }
}

/// Cyclone on the weeds over the fifth puddle uncovers it, hopping the
/// leader off it if he stands there.
void GabombaCatacombs_UncoverFifthPuddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  FieldCyclone_CopyTouchedCells(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(ACTOR_FIFTH_PUDDLE, 104, 280);
  if (LEADER_ON_CELL(leader, 6, 17)) {
    LEADER_HOP();
    Actor_SetDestinationOffsetById(gMainRoutineWork.leaderActorId, 16, 0);
    leader->velocityY = toFIXED(6);
    Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
    Event_End();
  }
}

/// Cyclone on the weeds over the sixth puddle uncovers it, hopping the
/// leader off it if he stands there.
void GabombaCatacombs_UncoverSixthPuddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  FieldCyclone_CopyTouchedCells(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(ACTOR_SIXTH_PUDDLE, 88, 264);
  if (LEADER_ON_CELL(leader, 5, 16)) {
    LEADER_HOP();
    Actor_SetDestinationOffsetById(gMainRoutineWork.leaderActorId, 16, 0);
    leader->velocityY = toFIXED(6);
    Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
    Event_End();
  }
}

/// Cyclone on the weeds over the seventh puddle uncovers it, hopping the
/// leader off it if he stands there.
void GabombaCatacombs_UncoverSeventhPuddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  FieldCyclone_CopyTouchedCells(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(ACTOR_SEVENTH_PUDDLE, 104, 248);
  if (LEADER_ON_CELL(leader, 6, 15)) {
    LEADER_HOP();
    Actor_SetDestinationOffsetById(gMainRoutineWork.leaderActorId, 16, 0);
    leader->velocityY = toFIXED(6);
    Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
    Event_End();
  }
}

/// Cyclone on the weeds over the stream's puddle uncovers it, hopping the
/// leader off it northward if he stands there.
void GabombaCatacombs_UncoverStreamPuddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  FieldCyclone_CopyTouchedCells(16, 13, 0, 4, 8, 61, 52);
  Actor_SetPosition(ACTOR_STREAM_PUDDLE, 280, 296);
  if (LEADER_ON_CELL(leader, 17, 18)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 0, -16);
    leader->velocityY = toFIXED(6);
    Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
    Event_End();
  }
}

/// Cyclone on the weeds of the Tomegathericon's room.
void GabombaCatacombs_ScatterTomegathericonWeeds(void) {
  FieldCyclone_CopyTouchedCells(7, 23, 0, 4, 5, 60, 56);
}

/// Cyclone on the last weeds: a Mad Plant hides there, and attacks.
void GabombaCatacombs_UncoverMadPlant(void) {
  FieldCyclone_CopyTouchedCells(7, 23, 0, 4, 5, 60, 56);
  if (GameFlag_Get(FLAG_MAD_PLANT(0)) == 0) {
    GabombaCatacombs_MadPlantLeaps(SPRITE_MAD_PLANT, 168, 424);
    gMainRoutineWork.pendingDjinni = IF_FLAG_SET(FLAG_MAD_PLANT(0));
    Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
    Field_RequestBattleFromEncounterTable(ENCOUNTER_TABLE_MAD_PLANTS, 0);
  }
}

/// Reveal shows the hidden wall's passage.
void GabombaCatacombs_RevealWall(void) {
  MapGrid_CopyCellPictureCellsRect(0, 0, 1, 2, 7, 11);
  Map_CopyCellAttributeRect(2, 4, 1, 1, 7, 14);
}

/// As Reveal fades, the hidden wall's passage is hidden again.
void GabombaCatacombs_HideWall(void) {
  MapGrid_CopyCellPictureCellsRect(1, 0, 1, 2, 7, 11);
  Map_CopyCellAttributeRect(2, 5, 1, 1, 7, 14);
}

/// Frost on the first puddle freezes it into a pillar, which blocks its
/// cell.
void GabombaCatacombs_FreezeFirstPuddle(void) {
  struct Actor *actor = Actor_GetById(ACTOR_FIRST_PUDDLE);

  GameFlag_Set(FLAG_GABOMBA_CATACOMBS_FIRST_PUDDLE_FROZEN);
  actor->motionFlags = 0;
  actor->priorityFlags = 0;
  MapGrid_CopyCellAttributes(6, 23, 1, 1, 8, 23);
}

/// Frost on the second puddle freezes it into a pillar, which blocks its
/// cell.
void GabombaCatacombs_FreezeSecondPuddle(void) {
  struct Actor *actor = Actor_GetById(ACTOR_SECOND_PUDDLE);

  GameFlag_Set(FLAG_GABOMBA_CATACOMBS_SECOND_PUDDLE_FROZEN);
  actor->motionFlags = 0;
  actor->priorityFlags = 0;
  MapGrid_CopyCellAttributes(0, 4, 1, 1, 8, 15);
}

/// Frost on the third puddle freezes it into a pillar, which blocks its
/// cell.
void GabombaCatacombs_FreezeThirdPuddle(void) {
  struct Actor *actor = Actor_GetById(ACTOR_THIRD_PUDDLE);

  GameFlag_Set(FLAG_GABOMBA_CATACOMBS_THIRD_PUDDLE_FROZEN);
  actor->motionFlags = 0;
  actor->priorityFlags = 0;
  MapGrid_CopyCellAttributes(0, 4, 1, 1, 9, 16);
}

/// Frost on the fourth puddle freezes it into a pillar, which blocks its
/// cell.
void GabombaCatacombs_FreezeFourthPuddle(void) {
  struct Actor *actor = Actor_GetById(ACTOR_FOURTH_PUDDLE);

  GameFlag_Set(FLAG_GABOMBA_CATACOMBS_FOURTH_PUDDLE_FROZEN);
  actor->motionFlags = 0;
  actor->priorityFlags = 0;
  MapGrid_CopyCellAttributes(0, 4, 1, 1, 8, 17);
}

/// Frost on the fifth puddle freezes it into a pillar, which blocks its
/// cell.
void GabombaCatacombs_FreezeFifthPuddle(void) {
  struct Actor *actor = Actor_GetById(ACTOR_FIFTH_PUDDLE);

  GameFlag_Set(FLAG_GABOMBA_CATACOMBS_FIFTH_PUDDLE_FROZEN);
  actor->motionFlags = 0;
  actor->priorityFlags = 0;
  MapGrid_CopyCellAttributes(0, 4, 1, 1, 6, 17);
}

/// Frost on the sixth puddle freezes it into a pillar, which blocks its
/// cell.
void GabombaCatacombs_FreezeSixthPuddle(void) {
  struct Actor *actor = Actor_GetById(ACTOR_SIXTH_PUDDLE);

  GameFlag_Set(FLAG_GABOMBA_CATACOMBS_SIXTH_PUDDLE_FROZEN);
  actor->motionFlags = 0;
  actor->priorityFlags = 0;
  MapGrid_CopyCellAttributes(0, 4, 1, 1, 5, 16);
}

/// Frost on the seventh puddle freezes it into a pillar, which blocks its
/// cell.
void GabombaCatacombs_FreezeSeventhPuddle(void) {
  struct Actor *actor = Actor_GetById(ACTOR_SEVENTH_PUDDLE);

  GameFlag_Set(FLAG_GABOMBA_CATACOMBS_SEVENTH_PUDDLE_FROZEN);
  actor->motionFlags = 0;
  actor->priorityFlags = 0;
  MapGrid_CopyCellAttributes(0, 4, 1, 1, 6, 15);
}

/// Frost on the stream puddle freezes it into a pillar, which blocks its
/// cell.
void GabombaCatacombs_FreezeStreamPuddle(void) {
  struct Actor *actor = Actor_GetById(ACTOR_STREAM_PUDDLE);

  GameFlag_Set(FLAG_GABOMBA_CATACOMBS_STREAM_PUDDLE_FROZEN);
  actor->motionFlags = 0;
  actor->priorityFlags = 0;
  MapGrid_CopyCellAttributes(15, 18, 1, 1, 17, 18);
}

/// Moves the leader and camera along z by dz when nothing is in the way.
void GabombaCatacombs_DriftWithStream(struct Actor *actor, s32 dz) {
  struct Actor *camera;
  struct FieldPosition position;

  camera = gFieldWork->camera;

  position.x = actor->x.fixed;
  position.y = actor->y.fixed;
  position.z = actor->z.fixed + dz;
  MapGrid_CheckCollision(actor, &position);
  position.x = actor->x.fixed;
  position.y = actor->y.fixed;
  position.z = actor->z.fixed + toFIXED(8);
  if (MapGrid_CheckCollision(actor, &position) <= 0) {
    position.x = actor->x.fixed + toFIXED(57) / 10;
    position.y = actor->y.fixed;
    position.z = actor->z.fixed + toFIXED(57) / 10;
    if (MapGrid_CheckCollision(actor, &position) <= 0) {
      position.x = actor->x.fixed - toFIXED(57) / 10;
      position.y = actor->y.fixed;
      position.z = actor->z.fixed + toFIXED(57) / 10;
      if (MapGrid_CheckCollision(actor, &position) <= 0) {
        camera->z.fixed += dz;
        actor->z.fixed += dz;
      }
    }
  }
}

/// Walking in the stream: a splash rises every eighth frame and the water
/// pushes the leader along.
void GabombaCatacombs_WadeStream(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *splash;

  if ((gFrameCount & 7) == 0) {
    splash = ((struct Actor *)Import_ActorCreate(
        SPRITE_WADING_SPLASH, leader->x.fixed, leader->y.fixed + toFIXED(6),
        leader->z.fixed));
    if (splash) {
      Actor_SetSpriteFlags(splash, 0);
      Actor_SelectAnimation(splash, 1);
      Actor_StartScript(splash, gGabombaCatacombs_SplashScript);
    }
    Sound_Play(SE_WADE);
  }
  GabombaCatacombs_DriftWithStream(leader, toFIXED(17) / 20);
}

/// Each frame, shrinks the glow by a twentieth of its full size.
s32 GabombaCatacombs_ShrinkGlow(struct Actor *actor) {
  actor->scaleX -= FIXED_ONE / 20;
  actor->scaleY -= FIXED_ONE / 20;
  return 0;
}

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);
void Actor_PlaceOnActor(s32 actorId, s32 target);

#define TALK()                                                                 \
  EventMessage_ShowCentered((s16)((union EventLine *)&work->message)->line++,  \
                            0, -3)

/// At the end of the catacombs the Great Gabomba speaks: "By reaching this
/// place, you have proven your power."
void GabombaCatacombs_GabombaBestowsPower(void) {
  struct Actor *kraden = Actor_GetById(ACTOR_KRADEN);
  struct Actor *treasure = Actor_GetById(FIELD_TREASURE_ACTOR);
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *glow, *leader;
  s32 speed;

  Event_Start();
  Event_ResolvePendingActions(0);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(8, 8, 12), 1);
  PaletteFade_StartInterpolation(60);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 158, 1);
  Sound_Play(MUS_JUPITER_LIGHTHOUSE);
  Event_Wait(60);
  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 2);
  Sound_Play(SE_STATUE_STIRS);
  Event_Wait(60);
  Event_SetMessageId((s32)MsgByReachingThis);
  TALK();
  Actor_SetPosition(FIELD_TREASURE_ACTOR, 168, 152);
  treasure->motionFlags = 6;
  treasure->groundHeight += toFIXED(16);
  Sound_Play(SE_SHIMMER);
  glow = ((struct Actor *)Import_ActorCreate(SPRITE_FIREBALL, 0, 0, 0));
  Actor_StartScript(glow, gFieldEffect_MediumScript);
  Actor_SetSpriteColorMode(glow, 1);
  glow->motionFlags = 0;
  glow->x.fixed = treasure->x.fixed;
  glow->y.fixed = treasure->y.fixed + toFIXED(8);
  glow->z.fixed = treasure->z.fixed;
  glow->groundHeight = treasure->groundHeight;
  glow->update = GabombaCatacombs_ShrinkGlow;
  Event_Wait(15);
  glow->x.fixed = 0;
  glow->y.fixed = 0;
  glow->z.fixed = 0;
  glow->update = 0;
  TALK();
  Actor_SetDestinationById(gMainRoutineWork.leaderActorId, 166, 188);
  Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_PlaceOnActor(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  speed = toFIXED(1);
  kraden->speedLimit = speed;
  Actor_SetDestinationById(ACTOR_KRADEN, 184, 188);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  // "I am grateful for your promise...But I am worried that we have done..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  TALK();
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  // "Then what are we to do? Does this mean we won't receive your magic?"
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  TALK();
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(30);
  TALK();
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  Event_Wait(30);
  // "Wh-What kind of conditions?"
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
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  // "Of course!"
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 2);
  TALK();
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 4);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_KRADEN, leader->x.part[0].integer,
                             leader->z.part[0].integer);
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

/// The seated idol: once the Great Gabomba has shown it, the party takes the
/// Tomegathericon.
void GabombaCatacombs_TalkToStatue(void) {
  struct Actor *actor = Actor_GetById(FIELD_TREASURE_ACTOR);

  if (actor != NULL && actor->x.fixed != 0)
    Scene_TalkToActor(FIELD_TALK_SILENT | FIELD_TREASURE_ACTOR);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) {
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_TELEPORTERS)
    return gGabombaCatacombs_TeleportersExitRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct Trigger gGabombaCatacombs_Triggers[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_TELEPORTERS)
    return gGabombaCatacombs_TeleportersActorPlacements;
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_MUD)
    return gGabombaCatacombs_MudActorPlacements;
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_HIDDEN_WALL)
    return gGabombaCatacombs_HiddenWallActorPlacements;
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_TOMEGATHERICON)
    return gGabombaCatacombs_TomegathericonActorPlacements;
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_STREAM)
    return gGabombaCatacombs_StreamActorPlacements;
  return gGabombaCatacombs_Placements;
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
#define CATACOMBS_SCREEN_EFFECT                                                \
  SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE)
#else
#define CATACOMBS_SCREEN_EFFECT                                                \
  SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE)
#endif

extern char SceneId_GabombaStatuePinion, SceneId_GabombaStatueChamberIdol;
void FieldEffect_StartBlankOverlay(s32 a);

s32 Scene_Initialize(void) {
  s32 entrance;
  struct Actor *treasure;

  gFieldWork->transition = CATACOMBS_SCREEN_EFFECT;
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_TELEPORTERS &&
      (entrance = gMainRoutineWork.entrance) == 1) {
    if (GameFlag_Get(FLAG_GABOMBA_RETREAT_TO_PINION)) {
      gMainRoutineWork.retreatScene = SCENE_GABOMBA_STATUE_PINION;
      gMainRoutineWork.retreatEntrance = entrance;
    } else if (GameFlag_Get(FLAG_GABOMBA_RETREAT_TO_IDOL)) {
      gMainRoutineWork.retreatScene = SCENE_GABOMBA_STATUE_CHAMBER_IDOL;
      gMainRoutineWork.retreatEntrance = 3;
    }
  }
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
      gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_MUD &&
      gMainRoutineWork.entrance > 2)
    Actor_DepartInRing(gMainRoutineWork.leaderActorId);
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_HIDDEN_WALL) {
    if (GameFlag_Get(FLAG_GABOMBA_CATACOMBS_FIRST_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(6, 23, 1, 1, 8, 23);
    if (GameFlag_Get(FLAG_GABOMBA_CATACOMBS_SECOND_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 8, 15);
    if (GameFlag_Get(FLAG_GABOMBA_CATACOMBS_THIRD_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 9, 16);
    if (GameFlag_Get(FLAG_GABOMBA_CATACOMBS_FOURTH_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 8, 17);
    if (GameFlag_Get(FLAG_GABOMBA_CATACOMBS_FIFTH_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 6, 17);
    if (GameFlag_Get(FLAG_GABOMBA_CATACOMBS_SIXTH_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 5, 16);
    if (GameFlag_Get(FLAG_GABOMBA_CATACOMBS_SEVENTH_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 6, 15);
  }
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_STREAM &&
      GameFlag_Get(FLAG_GABOMBA_CATACOMBS_STREAM_PUDDLE_FROZEN))
    MapGrid_CopyCellAttributes(15, 18, 1, 1, 17, 18);
  if (gMainRoutineWork.next == SCENE_GABOMBA_CATACOMBS_TOMEGATHERICON &&
      GameFlag_Get(FLAG_GABOMBA_BESTOWED_POWER) &&
      !GameFlag_Get(FLAG_TOMEGATHERICON_TAKEN) &&
      !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    treasure = Actor_GetById(FIELD_TREASURE_ACTOR);
    Actor_SetPosition(FIELD_TREASURE_ACTOR, 168, 152);
    treasure->motionFlags = 6;
    treasure->groundHeight += toFIXED(16);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

// clang-format off
u32 gGabombaCatacombs_SplashScript[] = {
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

#include "shared/scene_entrances_default.c"

struct SceneRegion gGabombaCatacombs_TeleportersExitRegions[] = {
    {388, 46, 276, 396, 54, 284, CONDITION_ALWAYS, 1},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(GabombaCatacombsTeleporters),
    SCENE_EXIT(GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR, GabombaStatueChamberWitchDoctor, GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_FROM_GABOMBA_CATACOMBS_TELEPORTERS),
    SCENE_EXIT(GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_NORTHEAST, GabombaCatacombsMud, GABOMBA_CATACOMBS_MUD_FROM_GABOMBA_CATACOMBS_TELEPORTERS_NORTHEAST),
    SCENE_EXIT(GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_WEST, GabombaCatacombsMud, GABOMBA_CATACOMBS_MUD_FROM_GABOMBA_CATACOMBS_TELEPORTERS_WEST),
    SCENE_EXIT(GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_EAST, GabombaCatacombsMud, GABOMBA_CATACOMBS_MUD_FROM_GABOMBA_CATACOMBS_TELEPORTERS_EAST),
    SCENE_EXIT(GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_SOUTH, GabombaCatacombsMud, GABOMBA_CATACOMBS_MUD_FROM_GABOMBA_CATACOMBS_TELEPORTERS_SOUTH),
    SCENE_EXITS(GabombaCatacombsMud),
    SCENE_EXIT(GABOMBA_CATACOMBS_MUD_TO_GABOMBA_CATACOMBS_TELEPORTERS_NORTHEAST, GabombaCatacombsTeleporters, GABOMBA_CATACOMBS_TELEPORTERS_FROM_GABOMBA_CATACOMBS_MUD_NORTHEAST),
    SCENE_EXIT(GABOMBA_CATACOMBS_MUD_TO_GABOMBA_CATACOMBS_HIDDEN_WALL, GabombaCatacombsHiddenWall, GABOMBA_CATACOMBS_HIDDEN_WALL_FROM_GABOMBA_CATACOMBS_MUD),
    SCENE_EXITS(GabombaCatacombsHiddenWall),
    SCENE_EXIT(GABOMBA_CATACOMBS_HIDDEN_WALL_TO_GABOMBA_CATACOMBS_MUD, GabombaCatacombsMud, GABOMBA_CATACOMBS_MUD_FROM_GABOMBA_CATACOMBS_HIDDEN_WALL),
    SCENE_EXIT(GABOMBA_CATACOMBS_HIDDEN_WALL_TO_GABOMBA_CATACOMBS_STREAM, GabombaCatacombsStream, GABOMBA_CATACOMBS_STREAM_FROM_GABOMBA_CATACOMBS_HIDDEN_WALL),
    SCENE_EXITS(GabombaCatacombsTomegathericon),
    SCENE_EXIT(GABOMBA_CATACOMBS_TOMEGATHERICON_TO_GABOMBA_CATACOMBS_STREAM, GabombaCatacombsStream, GABOMBA_CATACOMBS_STREAM_FROM_GABOMBA_CATACOMBS_TOMEGATHERICON),
    SCENE_EXITS(GabombaCatacombsStream),
    SCENE_EXIT(GABOMBA_CATACOMBS_STREAM_TO_GABOMBA_CATACOMBS_HIDDEN_WALL, GabombaCatacombsHiddenWall, GABOMBA_CATACOMBS_HIDDEN_WALL_FROM_GABOMBA_CATACOMBS_STREAM),
    SCENE_EXIT(GABOMBA_CATACOMBS_STREAM_TO_GABOMBA_CATACOMBS_TOMEGATHERICON, GabombaCatacombsTomegathericon, GABOMBA_CATACOMBS_TOMEGATHERICON_FROM_GABOMBA_CATACOMBS_STREAM),
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
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGabombaCatacombs_TomegathericonActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SEATED_IDOL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
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
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ScatterTeleportersWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 43,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverFirstTeleportStone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 44,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverSecondTeleportStone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 45,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_TakeThirdTeleportStone)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaCatacombs_MudTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_MUD, CONDITION_ALWAYS,
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
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ScatterHiddenWallWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ScatterHiddenWallSmallWeeds)},
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
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ScatterWeedsByTheStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE,
     FLAG_GABOMBA_CATACOMBS_FIRST_PUDDLE_FROZEN,
     EVENT(GabombaCatacombs_FreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE,
     FLAG_GABOMBA_CATACOMBS_SECOND_PUDDLE_FROZEN,
     EVENT(GabombaCatacombs_FreezeSecondPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_THIRD_PUDDLE,
     FLAG_GABOMBA_CATACOMBS_THIRD_PUDDLE_FROZEN,
     EVENT(GabombaCatacombs_FreezeThirdPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FOURTH_PUDDLE,
     FLAG_GABOMBA_CATACOMBS_FOURTH_PUDDLE_FROZEN,
     EVENT(GabombaCatacombs_FreezeFourthPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIFTH_PUDDLE,
     FLAG_GABOMBA_CATACOMBS_FIFTH_PUDDLE_FROZEN,
     EVENT(GabombaCatacombs_FreezeFifthPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIXTH_PUDDLE,
     FLAG_GABOMBA_CATACOMBS_SIXTH_PUDDLE_FROZEN,
     EVENT(GabombaCatacombs_FreezeSixthPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SEVENTH_PUDDLE,
     FLAG_GABOMBA_CATACOMBS_SEVENTH_PUDDLE_FROZEN,
     EVENT(GabombaCatacombs_FreezeSeventhPuddle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaCatacombs_TomegathericonTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, 40, FLAG_GABOMBA_BESTOWED_POWER,
     EVENT(GabombaCatacombs_GabombaBestowsPower)},
    {TRIGGER_TALK, ACTOR_SEATED_IDOL, FLAG_TOMEGATHERICON_TAKEN,
     EVENT(GabombaCatacombs_TalkToStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ScatterTomegathericonWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 31,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverMadPlant)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaCatacombs_StreamTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 40,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ScatterStreamWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ScatterStreamWideWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 43,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_ScatterStreamSmallWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 42,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverStreamPuddle)},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(GabombaCatacombs_WadeStream)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_STREAM_PUDDLE,
     FLAG_GABOMBA_CATACOMBS_STREAM_PUDDLE_FROZEN,
     EVENT(GabombaCatacombs_FreezeStreamPuddle)},
    {TRIGGER_TABLE_END},
};
