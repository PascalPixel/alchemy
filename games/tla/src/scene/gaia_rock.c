/// @file
/// Gaia Rock's outside, near Izumo, in the rooms of this overlay: pillars
/// Moved across gaps, slides and climbing walls, vines that swing in the wind
/// of the statues (HangingVine_Place), and the altar where Reveal shows the
/// Dancing Idol the party carries inside (gaia_rock_inside.c); after the
/// Serpent, Susa rejoices here that Kushinada is safe.

#include "shared/tables.h"
#include "scene/event_layer.h"
#include "battle/battle.h"
#include "field/field_sequence.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "flags.h"
/// Set once Susa has said he only needs rest ("I'm not dying... I'm just so
/// tired...").
#define FLAG_SUSA_RESTED 0xa61
/// Set once Susa has rejoiced with the party (GaiaRock_SusaRejoices).
#define FLAG_SUSA_REJOICED 0x9ab
#include "field/scene_tables.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "field/actor.h"
#include "version.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "field/call.h"
#include "field/field_sequence_event.h"
#include "system/iwram_call.h"
#include "field/hanging_vine.h"
#include "system/frame.h"
#include "field/map_grid.h"
#include "system/task.h"
#include "field/actor_view.h"
#include "field/event_commands.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "text/msg_ids.h"

/// The ledge where the party finds Susa beaten (SceneId_GaiaRockSusa): Susa
/// ("Kushinada's safe... She's safe! Nothing else matters.") and the statue
/// that blows wind.
enum { ACTOR_WIND_STATUE = 17, ACTOR_SUSA = 20 };

/// The two pillars Moved into the gaps: the first dropped in at the west end,
/// the second at (79, 12).
#define FLAG_FIRST_PILLAR_DROPPED 0x920
#define FLAG_SECOND_PILLAR_DROPPED 0x921

struct Actor;

extern s16 gGaiaRock_SusaExitRegions[];
extern u32 gGaiaRock_StatueSpotsExitRegions[];

void GaiaRock_StatueRisesUnderLeader(s32 id, s32 flag);

extern struct ActorPlacement gGaiaRock_Placements[];
extern struct ActorPlacement gGaiaRock_SusaActorPlacements[];

extern struct ActorPlacement gGaiaRock_StatueSpotsActorPlacements[];
extern s16 gGaiaRock_DancingIdolActorPlacements[];

extern struct Trigger gGaiaRock_SusaTriggers[];
extern struct Trigger gGaiaRock_StatueSpotsTriggers[];
extern struct Trigger gGaiaRock_Triggers[];
extern u32 gGaiaRock_LeafScript[];

extern s32 gGaiaRock_WindScript[];

void GaiaRock_StartWallTriggers(void);
void GaiaRock_PlaceStatue(s32 id, s32 x, s32 y, s32 z);

extern s32 gGaiaRock_StatueRiseScript[];

extern u32 gGaiaRock_DustScript[];
extern u32 gGaiaRock_CrateScript[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) {
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_SUSA)
    return (const struct SceneRegion *)gGaiaRock_SusaExitRegions;
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_STATUE_SPOTS)
    return (const struct SceneRegion *)gGaiaRock_StatueSpotsExitRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_SUSA)
    return (const struct ActorPlacement *)gGaiaRock_SusaActorPlacements;
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_STATUE_SPOTS)
    return (const struct ActorPlacement *)gGaiaRock_StatueSpotsActorPlacements;
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_DANCING_IDOL)
    return (const struct ActorPlacement *)gGaiaRock_DancingIdolActorPlacements;
  return (const struct Trigger *)gGaiaRock_Placements;
}

void GaiaRock_StepOnFirstStatueSpot(void) {
  GaiaRock_StatueRisesUnderLeader(17, 0x300);
}

void GaiaRock_StepOnSecondStatueSpot(void) {
  GaiaRock_StatueRisesUnderLeader(18, 0x301);
}

void GaiaRock_StepOnThirdStatueSpot(void) {
  GaiaRock_StatueRisesUnderLeader(19, 0x302);
}

// Sets the object's motion flags to 3 while it moves, for at most 60 frames
// until its vertical speed is 0, then clears them ten frames later. The
// editions after the Japanese one also run the given cue first.
#if !TLA_PATCH_1
void GaiaRock_DropPillar(s32 id) {
  s32 frames = 60;
  struct Actor *object = Actor_Get(id);

  object->motionFlags = 3;
  while (frames != 0) {
    System_Wait(1);
    if (object->velocityY == 0)
      break;
    frames--;
  }
  System_Wait(10);
  object->motionFlags = 0;
}
#else
void GaiaRock_DropPillar(s32 id, s32 cue) {
  s32 frames = 60;
  struct Actor *object = Actor_Get(id);

  object->motionFlags = 3;
  while (frames != 0) {
    System_Wait(1);
    frames--;
    if (object->velocityY == 0)
      break;
  }
  if (cue != 0)
    Event_PlaySound(cue);
  System_Wait(10);
  object->motionFlags = 0;
}
#endif

void GaiaRock_StartMovingPillar(void) {
  MapGrid_CopyCellAttributes(7, 90, 1, 1, 8, 91);
}

// The Japanese edition drops an object with no cue; the later editions play
// cue 181 as it drops. The first drop goes through the call layer.
#if !TLA_PATCH_1
#define DROP_OBJECT_CALLED(id) GaiaRock_DropPillar(id)
#define DROP_OBJECT(id) GaiaRock_DropPillar(id)
#else
#define DROP_OBJECT_CALLED(id) GaiaRock_DropPillar(id, 181)
#define DROP_OBJECT(id) GaiaRock_DropPillar(id, 181)
#endif

/// A pillar set down after a Move: the first drops into its gap once Moved far
/// enough west, the second bridges its gap as it passes over it to (79, 12).
void GaiaRock_FinishMovingPillar(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  MapGrid_CopyCellAttributes(8, 90, 1, 1, 8, 91);
  if (id == 16 && Event_GetFlag(FLAG_FIRST_PILLAR_DROPPED) == 0 &&
      (object->x.fixed >> 19) <= 11) {
    DROP_OBJECT_CALLED(16);
    Event_SetFlag(FLAG_FIRST_PILLAR_DROPPED);
    MapGrid_CopyCellAttributes(6, 27, 1, 1, 5, 27);
  }
  if (id == 19 && Event_GetFlag(FLAG_SECOND_PILLAR_DROPPED) == 0) {
    if ((object->x.fixed >> 19) == 77)
      MapGrid_CopyCellAttributes(33, 102, 1, 2, 37, 102);
    if ((object->x.fixed >> 19) == 79 && (object->y.fixed >> 19) == 12) {
      DROP_OBJECT(19);
      MapGrid_CopyCellAttributes(38, 46, 1, 1, 39, 46);
    }
    if ((object->x.fixed >> 19) == 81) {
      Event_SetFlag(FLAG_SECOND_PILLAR_DROPPED);
      MapGrid_CopyCellAttributes(41, 46, 1, 1, 39, 46);
      MapGrid_CopyCellAttributes(27, 44, 1, 1, 40, 46);
    }
  }
}

void TileCopies_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                 s32 g);

void GaiaRock_StartPushingBlock(void) {
  MapGrid_CopyCellAttributes(13, 18, 1, 1, 13,
                             FIELD_CELL(Actor_Get(8)->z.fixed));
}

void GaiaRock_FinishPushingBlock(void) {
  MapGrid_CopyCellAttributes(11, 14, 1, 1, 13,
                             FIELD_CELL(Actor_Get(8)->z.fixed));
}

void GaiaRock_CycloneClearsWall(void) {
  FieldTileCopies_CopyTouchedCells(110, 64, 14, 12, 110, 47, 1);
}

void GaiaRock_RevealIdol(void) {
  struct Actor *object = Actor_Get(FIELD_TREASURE_ACTOR);
  s32 done;

  if (object == NULL)
    return;
  done = Event_GetFlag(0xf74);
  if (done == 0) {
    Actor_SetPosition(FIELD_TREASURE_ACTOR, 657, 246);
    object->motionFlags = done;
    object->y.fixed = toFIXED(80);
  }
}

void GaiaRock_HideIdol(void) {
  if (Actor_Get(FIELD_TREASURE_ACTOR) != NULL)
    Actor_SetPosition(FIELD_TREASURE_ACTOR, 0, 0);
}

void GaiaRock_CheckAltar(void) {
  struct Actor *object = Actor_Get(FIELD_TREASURE_ACTOR);

  Message_ShowCentered(MsgCheckedTheAltar, 1);
  if (object != NULL && object->x.fixed != 0) {
    Scene_TalkToActor(4160);
  } else if (Event_GetFlag(0xf74) == 0) {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;

    Message_ShowCentered(MsgIMSensing, 1);
    work->fieldMenu = 1;
  } else {
    Message_ShowCentered(MsgICanT, 1);
  }
}

void GaiaRock_SusaRejoices(void) {
  if (Event_GetFlag(FLAG_SERPENT_SLAIN)) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId(MsgYouReGoing);
    FieldActor_StartRepeatedMotion(ACTOR_SUSA, 1);
    FieldEvent_Wait(15);
    Event_AskYesNo(ACTOR_SUSA, 0);
    Event_SetFlag(FLAG_SUSA_REJOICED);
    Event_End();
  }
}

void GaiaRock_TalkToSusa(void) {
  if (Event_GetFlag(FLAG_SUSA_REJOICED)) {
    FieldEvent_SetMessageId(MsgYouReGoing);
    Event_AskYesNo(ACTOR_SUSA, 0);
  } else {
    FieldEvent_SetMessageId(MsgIfYouRe);
    // "If you're going to Izumo, tell my sister that I'm still alive."
    Event_ShowMessage(ACTOR_SUSA, 0);
  }
}

void GaiaRock_MindReadSusa(void) {
  if (Event_GetFlag(FLAG_SUSA_REJOICED)) {
    FieldEvent_SetMessageId(MsgKushinadaSSafe);
    // "Kushinada's safe... She's safe! Nothing else matters."
    Event_ShowMessage(ACTOR_SUSA, 0);
  } else {
    FieldEvent_SetMessageId(MsgIReallyGot);
    // "I really got worked over, but as long as Kushinada's safe, it was..."
    Event_ShowMessage(ACTOR_SUSA, 0);
  }
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_SUSA)
    return gGaiaRock_SusaTriggers;
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_STATUE_SPOTS)
    return gGaiaRock_StatueSpotsTriggers;
  return gGaiaRock_Triggers;
}

/// A circling leaf as its update reads it: struct Actor's fields under a type
/// of its own, the timer unsigned and the update taking the leaf.
struct OrbitEffect {
  u8 unused[6]; ///< the script and its cursor, not read here
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  u8 unusedAfterZ[15];
  u8 priorityFlags;
  u8 unusedAfterPriority[4];
  s32 velocityY;
  u8 unusedAfterVelocityY[28];
  s32 gravity;
  u8 unusedAfterGravity[24];
  u16 timer;
  s16 pause; ///< Actor.userValue
  struct OrbitEffect *watchedActor;
  void (*update)(struct OrbitEffect *);
};

FIXED Trig_Cos(s32 angle);

void GaiaRock_CircleLeaf(struct OrbitEffect *effect) {
  s32 radius;
  s32 angle;
  FIXED dx;
  FIXED dz;

  radius = Random16() + 0x20000;
  angle = effect->facing;
  dx = Iwram_MulFX(radius, Trig_Cos(angle));
  dz = Iwram_MulFX(radius, Math_Sin(angle));
  effect->x += dx;
  effect->z += dz;
  effect->facing += 0xfff0;
  if (effect->pause != 0) {
    effect->pause--;
    effect->facing += 0x800;
  } else if ((Random16() << 5) >> 16 == 0) {
    effect->pause = ((Random16() << 4) >> 16) + 8;
  }
  if (++effect->timer == 71)
    Actor_StartScript((struct Actor *)effect, gGaiaRock_LeafScript);
}

s32 GaiaRock_SpawnCirclingLeaves(struct OrbitEffect *source) {
  s32 i;
  s32 timer;

  i = 0;
  timer = 20;
  do {
    {
      struct OrbitEffect *effect;
      s32 radius;
      FIXED position[3];

      position[0] = source->x;
      position[1] = source->y + i * 0xcccc + toFIXED(4);
      position[2] = source->z;
      radius = Random16();
      FieldVector_AddPolarOffsetFX(radius * 5 + toFIXED(3), Random16(),
                                   position);
      effect = (struct OrbitEffect *)Value4(
          (s32 (*)())Actor_Spawn, 308, position[0], position[1], position[2]);
      if (effect != NULL) {
        effect->update = GaiaRock_CircleLeaf;
        effect->timer = timer;
        effect->pause = 0;
        effect->gravity = 0x1999;
        effect->velocityY = toFIXED(3);
        effect->facing = Random16();
        effect->priorityFlags = 0;
        Actor_SetSpritePriorityDirect((struct Actor *)effect, 2);
      }
    }
    timer -= 2;
    i++;
  } while (i < 16);
  return 0;
}

static inline struct HangingVineSlot *FindSlot(s32 id) {
  struct HangingVineWork *work = HANGING_VINE_WORK;
  struct HangingVineSlot *found = NULL;
  s32 i;

  // Default: as HangingVine_FindSlot, the index zero is copied from found.
  i = 0;

  for (i = 0; i < 8; i++) {
    if (work->slots[i].id == id) {
      found = &work->slots[i];
      break;
    }
  }
  return found;
}

void GaiaRock_StartVineSwing(void) {
  struct HangingVineSlot *slot = FindSlot(8);

  slot->phase = 0x4000;
  slot->swing = 64;
}

void GaiaRock_SwingVineHarder(void) {
  struct HangingVineSlot *slot = FindSlot(8);

  slot->phase = 0x4000;
  slot->swing += 128;
}

void Actor_PlaySoundInView(struct Actor *object, s32 a);

void GaiaRock_StatueBlowsWind(void) {
  struct Actor *object = Actor_Get(ACTOR_WIND_STATUE);
  u8 phase = gFrameCount;

  if (phase == 0) {
    Actor_PlaySoundInView(object, 202);
    object =
        Actor_Spawn(0x118, object->x.fixed, object->y.fixed, object->z.fixed);
    if (object != NULL) {
      object->acceleration = toFIXED(2);
      object->speedLimit = toFIXED(2);
      object->scaleX = FIXED_ONE / 2;
      object->scaleY = FIXED_ONE / 10;
      object->priorityFlags = phase;
      Actor_SelectAnimation(object, 2);
      FieldActor_SetMoveTargetFX(object, object->x.fixed - toFIXED(32),
                                 object->y.fixed, object->z.fixed);
      Actor_StartScript(object, gGaiaRock_WindScript);
      Actor_SetSpritePriorityDirect(object, 2);
    }
  }
}

#if !TLA_PATCH_1
#define SCREEN_EFFECT 0x201
#else
#define SCREEN_EFFECT 0x204
#endif

s32 HangingVine_Place();

s32 Scene_Initialize(void) {
  struct Actor *actor;
  struct FieldWork *work;

  gFieldWork->transition = SCREEN_EFFECT;
  gMainRoutineWork.retreatScene = SCENE_GAIA_ROCK_SUSA;
  gMainRoutineWork.retreatEntrance = 5;
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_SUSA) {
    Task_Add((s32)GaiaRock_StatueBlowsWind, TASK_PASS_VBLANK);
    Call2((void (*)())HangingVine_Place, 8, 1);
    Call2((void (*)())HangingVine_Place, 9, 1);
    Call2((void (*)())HangingVine_Place, 10, 1);
    Call2((void (*)())HangingVine_Place, 11, 1);
    Call2((void (*)())HangingVine_Place, 12, 1);
    Call2((void (*)())HangingVine_Place, 13, 1);
    Call2((void (*)())HangingVine_Place, 14, 1);
    HangingVine_Place(15, 1);
    if (GameFlag_Get(FLAG_FIRST_PILLAR_DROPPED)) {
      actor = Actor_GetById(16);
      actor->motionFlags = 0;
      actor->x.fixed = toFIXED(88);
      actor->y.fixed = toFIXED(128);
      Actor_SetSpriteFlags(actor, 0);
      MapGrid_CopyCellAttributes(6, 27, 1, 1, 5, 27);
    }
    actor = Actor_GetById(19);
    actor->priorityFlags = 0;
    if (GameFlag_Get(FLAG_SECOND_PILLAR_DROPPED)) {
      actor->motionFlags = 0;
      actor->x.fixed = toFIXED(648);
      actor->y.fixed = toFIXED(64);
      Actor_SetSpriteFlags(actor, 0);
      MapGrid_CopyCellAttributes(33, 102, 1, 2, 37, 102);
      MapGrid_CopyCellAttributes(27, 44, 1, 1, 40, 46);
    } else if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      if ((actor->x.fixed >> 19) != 75) {
        MapGrid_CopyCellAttributes(33, 102, 1, 2, 37, 102);
      }
      if ((actor->x.fixed >> 19) == 79) {
        actor->motionFlags = 0;
        MapGrid_CopyCellAttributes(38, 46, 1, 1, 39, 46);
      }
    }
    if (GameFlag_Get(FLAG_SERPENT_SLAIN) && !GameFlag_Get(FLAG_SUSA_REJOICED)) {
      Actor_SetPosition(ACTOR_SUSA, 798, 800);
      Map_CopyCellAttributeRect(53, 52, 1, 1, 49, 49);
      Map_CopyCellAttributeRect(53, 52, 1, 1, 50, 49);
      Actor_SetAnimationById(ACTOR_SUSA, 5);
      if (!Event_GetFlag(FLAG_SUSA_RESTED)) {
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        FieldActor_StartRepeatedMotion(ACTOR_SUSA, 3);
        FieldCamera_MoveToActorById(ACTOR_SUSA, 1);
        FieldEvent_Wait(30);
        FieldEvent_SetMessageId((s32)MsgIMNot);
        // "I'm not dying... I'm just so tired... I'm going to lie down for
        // a..."
        Event_ShowMessage(ACTOR_SUSA, 0);
        FieldActor_StartRepeatedMotion(ACTOR_SUSA, 2);
        FieldEvent_Wait(30);
        Event_SetFlag(FLAG_SUSA_RESTED);
        Event_End();
      }
    }
  } else if (gMainRoutineWork.next == SCENE_GAIA_ROCK_STATUE_SPOTS) {
    Call2((void (*)())HangingVine_Place, 8, 1);
    Call2((void (*)())HangingVine_Place, 9, 1);
    Call2((void (*)())HangingVine_Place, 10, 1);
    Call2((void (*)())HangingVine_Place, 11, 1);
    Call2((void (*)())HangingVine_Place, 12, 1);
    Call2((void (*)())HangingVine_Place, 13, 1);
    HangingVine_Place(14, 1);
    if ((u16)(gMainRoutineWork.entrance - 3) <= 1) {
      GaiaRock_StartWallTriggers();
    }
    if (GameFlag_Get(FLAG_AREA_TEMP_00)) {
      GaiaRock_PlaceStatue(17, toFIXED(764), -toFIXED(391), toFIXED(424));
    }
    if (GameFlag_Get(FLAG_AREA_TEMP_01)) {
      GaiaRock_PlaceStatue(18, toFIXED(789), -toFIXED(248), toFIXED(424));
    }
    if (GameFlag_Get(FLAG_AREA_TEMP_02)) {
      GaiaRock_PlaceStatue(19, toFIXED(778), -toFIXED(119), toFIXED(424));
    }
  } else if (gMainRoutineWork.next == SCENE_GAIA_ROCK_DANCING_IDOL) {
    actor = Actor_GetById(FIELD_TREASURE_ACTOR);
    if (actor != 0) {
      Actor_SetPosition(FIELD_TREASURE_ACTOR, 0, 0);
    }
    work = Ram_HeapSlots->fieldWork;
    actor = work->actors[16];
    work->actors[64] = actor;
    actor = Actor_GetById(8);
    actor->motionFlags = 0;
    MapGrid_CopyCellAttributes(11, 14, 1, 1, 13, FIELD_CELL(actor->z.fixed));
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_STATUE_SPOTS &&
      (u16)(gMainRoutineWork.entrance - 3) <= 1) {
    s32 *layer = (s32 *)&Ram_HeapSlots->mapGrid->layers[1];

    layer[2] = -toFIXED(128);
    layer[3] = 0x4480000;
  }
  return 0;
}

void GaiaRock_TriggerWallCell(void) {
  if (gMainRoutineWork.movementMode == 2) {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;
    struct MapWork *map = Ram_HeapSlots->mapGrid;
    struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);
    s32 x = object->x.fixed / toFIXED(16);
    s32 z = (object->z.fixed - object->y.fixed + 0x7fff) / toFIXED(16);
    struct MapCell *cells = map->layers[2].cells;
    struct MapCell *cell = &cells[x + (z << 7)];
    s32 code = cell->collisionCode;

    if (code >= 1 && code <= 230)
      work->stepTrigger = code;
  }
}

void GaiaRock_PlaceStatue(s32 id, s32 x, s32 y, s32 z) {
  struct Actor *object = Actor_Get(id);
  struct MapCell *cell = Ram_HeapSlots->mapGrid->layers[2].cells;
  s32 cellX = x / toFIXED(16);
  s32 cellZ = (z - y + 0x7fff) / toFIXED(16);

  cell += cellX + (cellZ << 7);
  cell->collisionCode = 255;
  object->x.fixed = (x & FIELD_CELL_MASK) + toFIXED(8);
  object->z.fixed = (z & FIELD_CELL_MASK) + toFIXED(8);
  object->y.fixed = object->z.fixed - (cellZ << 20);
  object->motionFlags = 0;
  object->priorityFlags |= 2;
  Actor_SetSpriteFlags(object, 0);
}

void GaiaRock_StatueRisesUnderLeader(s32 id, s32 flag) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 gravity = leader->gravity;
  struct Actor *pillar;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  pillar = Actor_Get(id);
  GaiaRock_PlaceStatue(id, leader->x.fixed, leader->y.fixed, leader->z.fixed);
  Actor_StartScript(pillar, gGaiaRock_StatueRiseScript);
  i = 15;
  do {
    struct Actor *dust =
        FieldActor_Create(SPRITE_ROCK, leader->x.fixed,
                          leader->y.fixed + toFIXED(16), leader->z.fixed);

    if (dust != 0) {
      s32 angle;
      s32 speed;
      u32 size;
      s32 scale;

      Actor_StartScript(dust, gGaiaRock_DustScript);
      angle = Random16();
      speed = Random16();
      size = Random16();
      scale = (size >> 1) + 0x2000;
      dust->velocityY = Iwram_MulFX(speed * 8, Trig_Cos(angle));
      dust->velocityX = Iwram_MulFX(speed * 8, Math_Sin(angle));
      dust->acceleration = 0;
      dust->scaleX = scale;
      dust->scaleY = scale;
      dust->gravity = toFIXED(33) / 100;
      dust->bounce = FIXED_ONE / 2;
    }
  } while (--i >= 0);
  Event_PlaySound(SE_STATUE_RISE);
  Event_PlaySound(SE_FLOOR_SWITCH);
  MapGrid_SetShake(2, 2, 0.9);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SelectAnimation(leader, 40);
  FieldEvent_Wait(40);
  Event_PlaySound(SE_DUST_PUFF);
  leader->motionFlags = 3;
  leader->gravity = toFIXED(33) / 100;
  for (id = 0; id < 180; id++) {
    System_Wait(1);
    if (leader->y.fixed <= leader->groundHeight)
      break;
  }
  Event_PlaySound(SE_THUMP);
  leader->gravity = gravity;
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_REMOVE);
  Actor_SelectAnimation(leader, 38);
  MapGrid_SetShake(1, 4, 1);
  System_Wait(10);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  System_Wait(20);
  FieldCamera_MoveToFX(leader->x.fixed, leader->y.fixed, leader->z.fixed, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetSpriteFlags(leader, 1);
  Actor_SelectAnimation(leader, ANIM_STAND);
  leader->velocityY = toFIXED(4);
  System_Wait(20);
  gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
  Event_End();
  Event_SetFlag(flag);
}

void GaiaRock_StartWallTriggers(void) {
  Task_AddFrame(GaiaRock_TriggerWallCell, TASK_PASS_VBLANK);
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 24, 0, 408, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// clang-format off
s16 gGaiaRock_SusaExitRegions[] = {
    771, -28, 484, 779, -20, 492, -1, 2,
    771, 180, 707, 779, 188, 715, -1, 3,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gGaiaRock_StatueSpotsExitRegions[] = {
    0x12800c3, 0xcb0143, 0x14b0130, 0x2ffff, 0xfdd602c3, 0x2cb01c4,
    0x1ccfdde, 0x3ffff, 0xffff, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(GaiaRockSusa),
    SCENE_EXIT_UNLESS(1, GaiaRockStatueSpots, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, GaiaRockSusa, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, GaiaRockSusa, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, GaiaRockInsideFirstAltar, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, WorldMap, 39, EXIT_FLAG_NONE),
    SCENE_EXITS(GaiaRockStatueSpots),
    SCENE_EXIT_UNLESS(1, GaiaRockSusa, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, GaiaRockStatueSpots, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, GaiaRockStatueSpots, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, GaiaRockDancingIdol, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(GaiaRockDancingIdol),
    SCENE_EXIT_UNLESS(1, GaiaRockStatueSpots, 4, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

u32 gGaiaRock_CrateScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

struct ActorPlacement gGaiaRock_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaiaRock_SusaActorPlacements[] = {
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1000, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 936, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, (s32)gGaiaRock_CrateScript, 120, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 295, 0, 442, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHIRLWIND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, (s32)gGaiaRock_CrateScript, 600, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SUSA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaiaRock_StatueSpotsActorPlacements[] = {
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(378, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(378, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(378, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

s16 gGaiaRock_DancingIdolActorPlacements[] = {
    272, -1, 1, 0, 0, 216, 0, 0,
    0, 232, 0x4000, 2, 272, -1, 1, 0,
    0, 680, 0, 0, 0, 216, 0x4000, 258,
    272, -1, 1, 0, 0, 712, 0, 0,
    0, 248, 0x4000, 258, 272, -1, 1, 0,
    0, 680, 0, 0, 0, 280, 0x4000, 258,
    272, -1, 1, 0, 0, 632, 0, 0,
    0, 280, 0x4000, 258, 272, -1, 1, 0,
    0, 600, 0, 0, 0, 248, 0x4000, 258,
    272, -1, 1, 0, 0, 632, 0, 0,
    0, 216, 0x4000, 258, 385, -1, 1, 0,
    0, 656, 0, 0, 0, 248, 0x4000, 2,
    397, -1, 1, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, -1, 0, 0, 0, 0,
};

struct Trigger gGaiaRock_SusaTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, 42, FLAG_SUSA_REJOICED, EVENT(GaiaRock_SusaRejoices)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(GaiaRock_TalkToSusa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, EVENT(GaiaRock_MindReadSusa)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 9, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 11, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 12, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 13, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 14, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 15, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 20, CONDITION_ALWAYS, EVENT(GaiaRock_CycloneClearsWall)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16, FLAG_FIRST_PILLAR_DROPPED, EVENT(GaiaRock_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, FLAG_FIRST_PILLAR_DROPPED, EVENT(GaiaRock_FinishMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, FLAG_SECOND_PILLAR_DROPPED, EVENT(GaiaRock_FinishMovingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(GaiaRock_FinishMovingPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaiaRock_StatueSpotsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_STEP, 20, FLAG_AREA_TEMP_00, EVENT(GaiaRock_StepOnFirstStatueSpot)},
    {TRIGGER_STEP, 21, FLAG_AREA_TEMP_01, EVENT(GaiaRock_StepOnSecondStatueSpot)},
    {TRIGGER_STEP, 22, FLAG_AREA_TEMP_02, EVENT(GaiaRock_StepOnThirdStatueSpot)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 9, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 11, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 12, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 13, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 14, CONDITION_ALWAYS, 0},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaiaRock_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(GaiaRock_CheckAltar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(GaiaRock_StartPushingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT(GaiaRock_FinishPushingBlock)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(GaiaRock_StartPushingBlock)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(GaiaRock_FinishPushingBlock)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(GaiaRock_RevealIdol)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(GaiaRock_HideIdol)},
    {TRIGGER_TABLE_END},
};

u32 gGaiaRock_LeafScript[] = {
    SC_RELEASE,
};

s32 gGaiaRock_WindScript[] = {
    SC_ADD(SW_SCALE_X, 0x1999),
    SC_ADD(SW_SCALE_Y, 0x1999),
    SC_WAIT(1),
    SC_LOOP(12, SC_TOP),
    SC_SET(SW_SCALE_X, 0x1b333),
    SC_SET(SW_SCALE_Y, 0x14ccc),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(6),
    SC_CALL(GaiaRock_StartVineSwing),
    SC_WAIT(2),
    SC_LABEL(1),
    SC_CALL(GaiaRock_SwingVineHarder),
    SC_WAIT(2),
    SC_LOOP(3, SC_TO(1)),
    SC_CALL(GaiaRock_SpawnCirclingLeaves),
    SC_LABEL(2),
    SC_CALL(GaiaRock_SwingVineHarder),
    SC_WAIT(2),
    SC_LOOP(14, SC_TO(2)),
    SC_CALL(GaiaRock_SwingVineHarder),
    SC_WAIT(10),
    SC_RELEASE,
};

u32 gGaiaRock_DustScript[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s32 gGaiaRock_StatueRiseScript[] = {                                                               
      SC_ADD(SW_SCALE_X, 0x8000),                                              
      SC_ADD(SW_SCALE_Y, 0x8000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, -0x4000),                                             
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x2000),                                             
      SC_ADD(SW_SCALE_Y, -0x2000),                                             
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x2000),                                             
      SC_ADD(SW_SCALE_Y, -0x2000),                                             
      SC_WAIT(4),                                                              
      SC_ADD(SW_SCALE_X, 0x8000),                                              
      SC_ADD(SW_SCALE_Y, 0x8000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, -0x4000),                                             
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x2000),                                             
      SC_ADD(SW_SCALE_Y, -0x2000),                                             
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x2000),                                             
      SC_ADD(SW_SCALE_Y, -0x2000),                                             
      SC_WAIT(4),                                                              
      SC_ADD(SW_SCALE_X, 0x8000),                                              
      SC_ADD(SW_SCALE_Y, 0x8000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, -0x4000),                                             
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x2000),                                             
      SC_ADD(SW_SCALE_Y, -0x2000),                                             
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x2000),                                             
      SC_ADD(SW_SCALE_Y, -0x2000),                                             
      SC_WAIT(4),                                                              
      SC_WAIT(20),                                                             
      SC_LOOP(3, SC_TOP),                                                      
      SC_END,                                                                  
  };
// clang-format on
