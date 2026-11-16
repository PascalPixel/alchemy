/// @file
/// The four caverns that each keep a Summon tablet, in four scenes that share
/// this overlay: Indra Cavern west of the Lemurian ship (SceneId_CavernsIndra),
/// where Lash and Move reach Zagan's tablet; Osenia Cavern north of Mikasalla
/// (CavernsOsenia), where Scoop opens the way to Megaera's
/// (Caverns_ScoopOseniaPassage); Atteka Cavern (CavernsAtteka), where Parch
/// dries the water before Coatlicue's (Caverns_ParchAttekaWater); and Angara
/// Cavern north of Loho (CavernsAngara), where two grey blocks are carried and
/// stacked to reach Haurus's (Caverns_StartMovingAngaraBlock,
/// Caverns_FinishMovingAngaraBlock).
#include "types.h"
#include "system/io_reg.h"
#include "field/scene_tables.h"
#include "system/ram_buffer.h"
#include "field/field_event.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "field/actor_script.h"
#include "field/map.h"
#include "field/objdisp.h"
#include "scene/dehkan_plateau.h"

/// Story flags: Osenia Cavern's passage dug open by Scoop, Atteka Cavern's
/// water dried by Parch.
#define FLAG_OSENIA_CAVERN_SCOOPED 0x9af
#define FLAG_ATTEKA_CAVERN_PARCHED 0x917
/// Angara Cavern: one block carried onto the other, and the cells about the
/// block's path raised while it moves.
#define FLAG_ANGARA_BLOCKS_STACKED FLAG_SCENE_TEMP_12
#define FLAG_ANGARA_CELLS_RAISED FLAG_SCENE_TEMP_13
/// Indra Cavern: set once Lash has tied the rope between its posts.
#define FLAG_INDRA_ROPE_TIED FLAG_SCENE_TEMP_00

extern s16 gCaverns_AngaraBlocks[];
extern s16 gCaverns_OseniaRegions[];
extern struct ActorPlacement gCaverns_Placements[];
extern struct ActorPlacement gCaverns_IndraPlacements[];
extern struct ActorPlacement gCaverns_AngaraPlacements[];
extern struct Trigger gCaverns_OseniaTriggers[];
extern struct Trigger gCaverns_IndraTriggers[];
extern struct Trigger gCaverns_AttekaTriggers[];
extern struct Trigger gCaverns_AngaraTriggers[];

void Caverns_PlaceAngaraBlocks(const s16 *data);
void Caverns_PrepareOsenia(void);
void Caverns_PrepareAtteka(void);
s32 MapGrid_CheckCollision(struct Actor *actor,
                           struct FieldCoordinate *position);
void Caverns_InitializeIndra(void);
void Caverns_PrepareAngara(void);
void Caverns_InitializeAngara(void);
/// Jumps the leader across to the position given when nothing is in the way.
s32 Caverns_JumpAcross(struct Actor *actor, struct FieldCoordinate *position);
#include "flags.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "field/object.h"
#include "scene/scene.h"
#include "scene/event_layer.h"
#include "field/event_inline.h"
#include "field/actor.h"
#include "field/field_sprite.h"
#include "system/ram_buffer.h"
#include "system/field_effect.h"
#include "fixed_math.h"
#include "macros.h"
#include "system/main_routine_work.h"
#include "field/field_sequence_event.h"
#include "field/field_work.h"
#include "field/map_grid.h"
#include "field/field_psynergy_work.h"
#include "ability_ids.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_prepare.c"

#include "shared/scene_create_imported_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

struct Actor;

extern u32 gFieldEffect_ShortScript[];

extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const s16 *Scene_GetExitRegions(void) {
  const s16 *result = NULL;

  if (gMainRoutineWork.next == SCENE_CAVERNS_OSENIA)
    result = gCaverns_OseniaRegions;
  return result;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_CAVERNS_INDRA)
    return gCaverns_IndraPlacements;
  if (scene == SCENE_CAVERNS_ANGARA)
    return gCaverns_AngaraPlacements;
  return gCaverns_Placements;
}

#include "../field/map_cell_get.c"

#include "shared/map_cell_set.c"

#include "../field/actor_wait_until_idle.c"

#include "text/msg_ids.h"

/// Sets the grey blocks of Angara Cavern on their cells: each cell rises two
/// steps under its block and is marked occupied.
void Caverns_PlaceAngaraBlocks(const s16 *data) {
  const u16 *ids = (const u16 *)data;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *actor;
  s32 height;
  s32 cell;

  while (*ids != ACTOR_LIST_END) {
    actor = Actor_GetById(*ids);
    actor->motionFlags = 0;
    actor->collisionFlags = 0;
    actor->bounce = 0;
    Actor_SetSpriteFlags((struct Actor *)actor, 0);
    height =
        (Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed) >> 19) + 2;
    MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, height);
    MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 2, height);
    cell = MapCell_GetAttributes(0, FIELD_CELL(actor->x.fixed),
                                 FIELD_CELL(actor->z.fixed));
    actor->riseBaseY = cell;
    MapCell_SetAttributes(0, FIELD_CELL(actor->x.fixed),
                          FIELD_CELL(actor->z.fixed), cell | 0x80);
    ids++;
  }
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    wait(1);
    leader->y.fixed = leader->groundHeight =
        Map_GetTerrainHeight(leader->layer, leader->x.fixed, leader->z.fixed);
    leader->velocityY = 0;
    leader->targetY = 0x80000000;
    FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 0);
  }
}

/// The terrain height of layer 2 at a point.
static __inline__ s32 GetHeight(s32 x, s32 z) {
  return Map_GetTerrainHeight(2, x, z);
}

/// The whole pixels of an actor's z coordinate.
static __inline__ s32 Actor_GetPixelZ(struct Actor *actor) {
  return actor->z.part[0].integer;
}

#define SET_CELL_HEIGHT(x, z)                                                  \
  height = (GetHeight(x, z) >> 19) + offset;                                   \
  FieldMapGrid_SetCellHeight(x, z, 2, height)

/// Raises (offset 1) or lowers (-1) by a step the cells about the moving
/// block's path, by where the block stands.
void Caverns_ShiftAngaraCellHeights(s32 offset) {
  struct Actor *block = gFieldWork->movedBlock;
  s32 height;

  offset <<= 3;
  if (block->y.part[0].integer > 31 ||
      Event_GetFlag(FLAG_ANGARA_CELLS_RAISED)) {
    SET_CELL_HEIGHT(toFIXED(568), toFIXED(536));
    SET_CELL_HEIGHT(toFIXED(584), toFIXED(552));
    if (offset == 8)
      Event_SetFlag(FLAG_ANGARA_CELLS_RAISED);
    else
      Event_ClearFlag(FLAG_ANGARA_CELLS_RAISED);
  } else if (Actor_GetPixelZ(block) == 536 ||
             Event_GetFlag(FLAG_ANGARA_CELLS_RAISED)) {
    SET_CELL_HEIGHT(toFIXED(536), toFIXED(552));
    SET_CELL_HEIGHT(toFIXED(552), toFIXED(552));
    SET_CELL_HEIGHT(toFIXED(568), toFIXED(552));
    if (offset == 6)
      Event_SetFlag(FLAG_ANGARA_CELLS_RAISED);
    else
      Event_ClearFlag(FLAG_ANGARA_CELLS_RAISED);
  } else {
    SET_CELL_HEIGHT(toFIXED(536), toFIXED(536));
    SET_CELL_HEIGHT(toFIXED(552), toFIXED(536));
    SET_CELL_HEIGHT(toFIXED(568), toFIXED(536));
  }
}

/// Before a push, Move or Carry: finds the block ahead of the leader (or the
/// one Psynergy holds) and frees its cell.
void Caverns_StartMovingAngaraBlock(void) {
  FIXED position[3];
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct FieldEffectWork *current = Ram_HeapSlots->fieldEffectWork;
  const u16 *ids = (const u16 *)gCaverns_AngaraBlocks;
  struct Actor *actor = NULL;
  struct Actor *leader;
  s32 height;
  s32 angle;

  work->movedBlock = NULL;
  if (current != NULL && work->psynergy != 0) {
    actor = work->movedBlock = current->child;
    if (current->action != ABILITY_CARRY)
      Caverns_ShiftAngaraCellHeights(1);
  } else {
    leader = Actor_GetById(gMainRoutineWork.leaderActorId);
    angle = (leader->facing + DEGtoANG(45)) & 0xc000;
    position[0] = leader->x.fixed;
    position[1] = leader->y.fixed;
    position[2] = leader->z.fixed;
    Vector_AddPolarOffset(toFIXED(16), angle, position);
    while (*ids != ACTOR_LIST_END) {
      actor = Actor_GetById(*ids);
      if (FIELD_CELL(position[0]) == FIELD_CELL(actor->x.fixed) &&
          position[1] / toFIXED(1) == actor->y.fixed / toFIXED(1) &&
          position[2] >> 20 == FIELD_CELL(actor->z.fixed))
        break;
      ids++;
      actor = NULL;
    }
    if (actor == NULL)
      return;
    work->movedBlock = actor;
    Caverns_ShiftAngaraCellHeights(1);
  }
  height = (Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed) >> 19) - 2;
  MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, height);
  MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 2, height);
  MapCell_SetAttributes(0, FIELD_CELL(actor->x.fixed),
                        FIELD_CELL(actor->z.fixed), actor->riseBaseY);
}

/// A dust puff's step: drifts, slows, grows and turns.
void Caverns_DriftDust(struct Actor *actor) {
  actor->x.fixed += actor->bounce;
  actor->y.fixed += actor->gravity;
  actor->z.fixed += actor->riseBaseY;
  actor->bounce -= actor->bounce / 12;
  actor->riseBaseY -= actor->riseBaseY / 10;
  actor->scaleX += actor->speedLimit;
  actor->scaleY += actor->acceleration;
  ((struct FieldSprite *)actor->sprite)->rotation += actor->timer;
}

/// The stacked block's update: draws the leader in front of it while he is
/// south of it.
void Caverns_LayerLeaderByStack(struct Actor *actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  FIXED leaderZ = leader->z.fixed;
  FIXED z = actor->z.fixed;
  u8 *flags = &leader->priorityFlags;

  if (leaderZ > z) {
    ActorFlags_Set(flags, 4);
  } else {
    *flags &= ~6;
  }
}

/// After the block has moved: a block left over a drop falls with a thump and a
/// ring of dust, and one landing on the other block stacks on it; then its new
/// cell rises under it.
void Caverns_FinishMovingAngaraBlock(void) {
  FIXED velocity[3];
  struct EffectOptions options;
  struct FieldWork *work;
  struct FieldEffectWork *current;
  struct Actor *actor;
  struct Actor *other;
  s32 kind;
  s32 height;
  s32 value;
  s32 angle;
  u32 i;
  struct Actor *spawned;
  u16 *blendControl;
  s32 sum;

  work = Ram_HeapSlots->fieldWork;
  current = Ram_HeapSlots->fieldEffectWork;
  actor = work->movedBlock;
  if (current != NULL && work->psynergy != 0) {
    if (current->action != ABILITY_CARRY)
      Caverns_ShiftAngaraCellHeights(-1);
  } else {
    Caverns_ShiftAngaraCellHeights(-1);
  }
  if (Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed) !=
      actor->y.fixed) {
    actor->layer = 2;
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    actor->bounce = 0;
    wait(1);
    kind = MapGrid_GetCellBehaviorAt(2, actor->x.fixed, actor->z.fixed);
    if (kind != 0) {
      actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      REG_BLDALPHA = BLEND_ALPHA(0, 16);
      blendControl = (u16 *)&REG_BLDCNT;
      *blendControl = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
      sum = kind + 2;
      spawned = OverlayObject_PrepareObject(
          actor->x.fixed, 0, actor->z.fixed + (sum << 20), SPRITE_YELLOW_BLOCK);
      Sound_Play(SE_DUST_PUFF);
      OverlayObject_WaitUntilIdle(actor);
      OverlayObject_WaitUntilIdle(actor);
      actor->x.fixed = actor->y.fixed = actor->z.fixed = 0;
      Actor_Release(spawned);
      wait(1);
      *blendControl = 0;
    } else {
      OverlayObject_WaitUntilIdle(actor);
      OverlayObject_WaitUntilIdle(actor);
      Sound_Play(SE_THUMP);
      options.palette = 7;
      options.priority = 2;
      options.update = (void (*)(union FieldObject *))Caverns_DriftDust;
      options.startScaleX = FIXED_ONE * 4 / 5;
      options.startScaleY = FIXED_ONE * 4 / 5;
      i = 0;
      do {
        angle = i << 12;
        velocity[0] = Trig_Cos(angle) * 2;
        velocity[1] = 0;
        velocity[2] = Trig_Sin(angle) * 2;
        velocity[0] += velocity[0] >> 1;
        velocity[0] += ((u32)(Random16() * 3 << 14) >> 16) - (toFIXED(3) / 8);
        velocity[2] += ((u32)(Random16() * 3 << 13) >> 16) - (toFIXED(3) / 16);
        /* FAKEMATCH(dead): sum, never read again, assigned the flags inside the
         * call's arguments. */
        FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed,
                          velocity[0], velocity[1], velocity[2],
                          sum = EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                                EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                          &options);
        i++;
      } while (i <= 16);
    }
    actor->motionFlags = 0;
    actor->layer = 0;
    // The velocity array is reused for the position under the actor.
    {
      velocity[0] = actor->x.fixed;
      velocity[1] = actor->y.fixed - toFIXED(16);
      velocity[2] = actor->z.fixed;
      other = Actor_FindAtCell(velocity, 1);
      if (other != NULL) {
        actor->update = (void (*)(void))Caverns_LayerLeaderByStack;
        actor->riseCounter = 1;
        other->riseCounter = 1;
        GameFlag_Set(FLAG_ANGARA_BLOCKS_STACKED);
      }
    }
  }
  height = (Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed) >> 19) + 2;
  MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, height);
  MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 2, height);
  value = MapCell_GetAttributes(0, FIELD_CELL(actor->x.fixed),
                                FIELD_CELL(actor->z.fixed));
  actor->riseBaseY = value;
  MapCell_SetAttributes(0, FIELD_CELL(actor->x.fixed),
                        FIELD_CELL(actor->z.fixed), value | 0x80);
}

const struct Trigger *Scene_GetTriggers(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_CAVERNS_INDRA)
    return gCaverns_IndraTriggers;
  if (scene == SCENE_CAVERNS_ATTEKA)
    return gCaverns_AttekaTriggers;
  if (scene == SCENE_CAVERNS_ANGARA)
    return gCaverns_AngaraTriggers;
  return gCaverns_OseniaTriggers;
}

s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gMainRoutineWork.next == SCENE_CAVERNS_INDRA)
    Caverns_InitializeIndra();
  if (gMainRoutineWork.next == SCENE_CAVERNS_ANGARA)
    Caverns_InitializeAngara();
  return 0;
}

s32 Scene_PrepareMap(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_CAVERNS_OSENIA)
    Caverns_PrepareOsenia();
  else if (scene == SCENE_CAVERNS_ATTEKA)
    Caverns_PrepareAtteka();
  else if (scene == SCENE_CAVERNS_ANGARA)
    Caverns_PrepareAngara();
  return 0;
}

/// Indra's cavern: the posts the rope Lash ties is strung between, and the
/// pillar Move pushes.
enum CavernsIndraActorId {
  ACTOR_ROPE_POST = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_ROPE_POST,
  ACTOR_PILLAR
};

/// Indra Cavern on entry: strings the rope between its posts (tied once
/// FLAG_INDRA_ROPE_TIED is set), stills the pillar and marks its cell.
void Caverns_InitializeIndra(void) {
  struct Actor *actor;
  s32 arg;

  Actor_GetById(ACTOR_FELIX)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Rope_Initialize();
  Rope_Place(0, FLAG_INDRA_ROPE_TIED, ACTOR_ROPE_POST, ACTOR_SECOND_ROPE_POST);
  actor = Actor_GetById(ACTOR_PILLAR);
  actor->motionFlags = 0;

  arg = 10;
  Map_CopyCellAttributeRect(16, arg, 1, 1, FIELD_CELL(actor->x.fixed), arg);
}

/// Move on Indra Cavern's pillar: stills it and copies the cells of its new
/// place into the floor.
void Caverns_MoveIndraPillar(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);

  actor->motionFlags = 0;
  Map_CopyCellAttributeRect(13, 12, 2, 1, 13, 10);
  Map_CopyCellAttributeRect(16, 10, 1, 1, FIELD_CELL(actor->x.fixed), 10);
}

s32 Caverns_JumpAcross(struct Actor *actor, struct FieldCoordinate *position) {
  s32 saved = actor->motionFlags;
  s32 blocked = MapGrid_CheckCollision(actor, position);

  if (blocked == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(actor, ANIM_LEADER_LEAP);
    actor->speedLimit = toFIXED(3);
    actor->acceleration = toFIXED(2);
    actor->velocityY = toFIXED(4);
    actor->motionFlags &= 0x7e;
    Actor_SetSpriteFlags(actor, 0);
    {
      struct MainRoutineWork *party = &gMainRoutineWork;
      s32 x = position[0].part[0].integer;
      s32 z = position[2].part[0].integer;
      s32 owner = party->leaderActorId;

      Actor_MoveToAndWaitById(owner, x, z);
    }
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(actor, 1);
    actor->motionFlags = saved;
    Event_End();
    return 1;
  }
  return 0;
}

/// Indra Cavern: walking east onto the gap's edge jumps the leader 32 pixels
/// across it.
void Caverns_JumpEastAcrossGap(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 position[3];
  FIXED x;

  x = leader->x.fixed;
  x += toFIXED(32);
  SET_VECTOR(position, x, leader->y.fixed, leader->z.fixed);
  Caverns_JumpAcross(leader, (struct FieldCoordinate *)position);
}

/// Osenia Cavern's one actor, the row of gCaverns_Placements: an unseen marker
/// set at the passage the leader steps down into.
enum CavernsOseniaActorId { ACTOR_PASSAGE_MARKER = ACTOR_FIRST_PLACED };

void Caverns_OpenOseniaPassage(void);

/// Osenia Cavern's map: once Scoop has dug it, the passage stands open.
void Caverns_PrepareOsenia(void) {
  s32 scooped;

  scooped = Event_GetFlag(FLAG_OSENIA_CAVERN_SCOOPED);
  if (scooped != 0) {
    Caverns_OpenOseniaPassage();
  }
  return;
}

/// Opens Osenia Cavern's passage in the map: the dug cells and their
/// attributes copied over the closed ones.
void Caverns_OpenOseniaPassage(void) {
  MapGrid_CopyCellPictureIndicesRect(35, 12, 18, 12, 3, 3);
  MapGrid_CopyCellAttributes(35, 12, 3, 3, 18, 12);
  MapGrid_CopyCellPictureIndicesRect(84, 13, 83, 13, 1, 1);
  return;
}

/// Osenia Cavern: the leader steps down into a passage and leaves for its other
/// end, by the trigger stepped on.
void Caverns_WalkIntoPassage(void) {
  s32 value = ((struct FieldWork *)Ram_HeapSlots->fieldWork)->stepTrigger;

  if (value == 2)
    Actor_SetPosition(ACTOR_PASSAGE_MARKER, 144, 224);
  else
    Actor_SetPosition(ACTOR_PASSAGE_MARKER, 224, 224);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_GetById(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  MapGrid_CopyCellsTo(96, 14, 72, 14, 2, 1);
  Actor_SetDestinationOffsetById(ACTOR_FELIX, 0, 4);
  Actor_WaitForMoveById(ACTOR_FELIX);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FELIX), 0);
  Actor_SetDestinationOffsetById(ACTOR_FELIX, 0, 16);
  Actor_WaitForMoveById(ACTOR_FELIX);
  Actor_SetAnimationById(ACTOR_FELIX, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
  Sound_Play(SE_MAP_EXIT);
  Event_Wait(14);
  Import_EventSetExitRequest(value);
}

/// Osenia Cavern: walking east into a passage's mouth steps up and across, then
/// down into it.
void Caverns_WalkEastIntoPassage(void) {
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -14);
  Actor_WalkByAndWaitById(ACTOR_FELIX, 16, 0);
  Event_Wait(10);
  Caverns_WalkIntoPassage();
  return;
}

/// Osenia Cavern: walking west into a passage's mouth steps up and across, then
/// down into it.
void Caverns_WalkWestIntoPassage(void) {
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -14);
  Actor_WalkByAndWaitById(ACTOR_FELIX, -16, 0);
  Event_Wait(10);
  Caverns_WalkIntoPassage();
  return;
}

/// Osenia Cavern: walking south into a passage's mouth turns the leader north
/// and takes him down into it.
void Caverns_WalkSouthIntoPassage(void) {
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
  Event_Wait(10);
  Caverns_WalkIntoPassage();
  return;
}

/// Scoop on Osenia Cavern's sand: as it digs, opens the passage for good.
void Caverns_ScoopOseniaPassage(s32 mode) {
  if (mode == 1) {
    Caverns_OpenOseniaPassage();
    GameFlag_Set(FLAG_OSENIA_CAVERN_SCOOPED);
  }
  return;
}

/// Atteka Cavern's map: once Parch has dried it, the water stays gone.
void Caverns_PrepareAtteka(void) {
  s32 parched;

  parched = Event_GetFlag(FLAG_ATTEKA_CAVERN_PARCHED);
  if (parched != 0) {
    MapGrid_CopyCellAttributes(31, 43, 9, 12, 48, 47);
    MapGrid_CopyCellsTo(30, 110, 48, 110, 9, 13);
    MapGrid_CopyCellAttributes(52, 110, 1, 1, 52, 111);
  }
  return;
}

/// Parch on Atteka Cavern's water: dries it, opening the way to Coatlicue's
/// tablet, for good.
void Caverns_ParchAttekaWater(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  MapGrid_CopyCellAttributes(31, 43, 9, 11, 48, 47);
  MapGrid_CopyCellsTo(30, 110, 48, 110, 9, 13);
  MapGrid_CopyCellAttributes(52, 110, 1, 1, 52, 111);
  GameFlag_Set(FLAG_ATTEKA_CAVERN_PARCHED);
  Event_End();
  return;
}

/// Angara Cavern's map: entered by entrance 5 or 6, the cells at columns 28 and
/// 36 of row 5 take the attributes of those of row 0.
void Caverns_PrepareAngara(void) {
  if ((u16)(gMainRoutineWork.entrance - 5) <= 1) {
    MapGrid_CopyCellAttributes(28, 0, 1, 1, 28, 5);
    Map_CopyCellAttributeRect(36, 0, 1, 1, 36, 5);
  }
}

/// Angara Cavern's actors, numbering gCaverns_AngaraPlacements.
enum CavernsAngaraActorId {
  /// The two grey blocks carried and stacked (sprite SPRITE_SPIRAL_PILLAR).
  ACTOR_FIRST_BLOCK = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_BLOCK,
  /// The two stones on either side of Haurus's tablet, which the searches for
  /// a block skip.
  ACTOR_WEST_OF_TABLET,
  ACTOR_EAST_OF_TABLET
};

/// Angara Cavern on entry: sets the two grey blocks on their cells, lets them
/// stand over rises, and keeps the searches for a block off the tablet's
/// stones.
void Caverns_InitializeAngara(void) {
  Caverns_PlaceAngaraBlocks(gCaverns_AngaraBlocks);
  Actor_GetById(ACTOR_FIRST_BLOCK)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Actor_GetById(ACTOR_SECOND_BLOCK)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Actor_GetById(ACTOR_WEST_OF_TABLET)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_GetById(ACTOR_EAST_OF_TABLET)->collisionFlags |= ACTOR_NOT_TARGETED;
}

/// Walking into Angara Cavern's door opens it and leaves by exit 8.
void Caverns_OpenAngaraDoor(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_DOOR_OPEN);
  Event_Wait(10);
  MapGrid_CopyCellsTo(7, 94, 6, 94, 1, 2);
  Event_Wait(16);
  Sound_Play(SE_MAP_EXIT);
  Import_EventSetExitRequest(8);
  Event_End();
}

// clang-format off
#include "shared/field_effect_duration_scripts.c"

/// The grey blocks of Angara Cavern, by actor id, ended by -1.
s16 gCaverns_AngaraBlocks[] = {
    ACTOR_FIRST_BLOCK, ACTOR_SECOND_BLOCK, -1, 0,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 467, 0, 153, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

/// Osenia Cavern's exit regions: the two passages' mouths, exits 4 and 5.
s16 gCaverns_OseniaRegions[] = {
    132, 28, 692, 140, 36, 700, -1, 4,
    292, 28, 692, 300, 36, 700, -1, 5,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(CavernsOsenia),
    SCENE_EXIT_UNLESS(1, WorldMap, 82, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, CavernsOsenia, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, CavernsOsenia, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, CavernsOsenia, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, CavernsOsenia, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(CavernsIndra),
    SCENE_EXIT_UNLESS(1, WorldMap, 83, EXIT_FLAG_NONE),
    SCENE_EXITS(CavernsAtteka),
    SCENE_EXIT_UNLESS(5, WorldMap, 88, EXIT_FLAG_NONE),
    SCENE_EXITS(CavernsAngara),
    SCENE_EXIT_UNLESS(1, WorldMap, 89, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, CavernsAngara, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, CavernsAngara, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, CavernsAngara, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, CavernsAngara, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, CavernsAngara, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, CavernsAngara, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, CavernsAngara, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, CavernsAngara, 8, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// Osenia and Atteka Caverns' actors: the passage marker.
struct ActorPlacement gCaverns_Placements[] = {
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// Indra Cavern's actors, in CavernsIndraActorId's order.
struct ActorPlacement gCaverns_IndraPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 168, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 168, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 168, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// Angara Cavern's actors, in CavernsAngaraActorId's order.
struct ActorPlacement gCaverns_AngaraPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 552, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 536, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 512, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 512, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// Osenia Cavern's exit, its two passages' mouths walked into from three sides,
/// and Scoop on its sand.
struct Trigger gCaverns_OseniaTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 2, CONDITION_ALWAYS, EVENT(Caverns_WalkSouthIntoPassage)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 2, CONDITION_ALWAYS, EVENT(Caverns_WalkWestIntoPassage)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 2, CONDITION_ALWAYS, EVENT(Caverns_WalkEastIntoPassage)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 3, CONDITION_ALWAYS, EVENT(Caverns_WalkSouthIntoPassage)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 3, CONDITION_ALWAYS, EVENT(Caverns_WalkWestIntoPassage)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 3, CONDITION_ALWAYS, EVENT(Caverns_WalkEastIntoPassage)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 10, FLAG_OSENIA_CAVERN_SCOOPED, EVENT(Caverns_ScoopOseniaPassage)},
    {TRIGGER_TABLE_END},
};

/// Indra Cavern's exit, its gap, Lash on the rope post and Move on the pillar.
struct Trigger gCaverns_IndraTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 10, CONDITION_ALWAYS, EVENT(Caverns_JumpEastAcrossGap)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_POST, FLAG_INDRA_ROPE_TIED, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, CONDITION_ALWAYS, EVENT(Caverns_MoveIndraPillar)},
    {TRIGGER_TABLE_END},
};

/// Atteka Cavern's exit and Parch on its water.
struct Trigger gCaverns_AttekaTriggers[] = {
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 60, CONDITION_ALWAYS, EVENT(Caverns_ParchAttekaWater)},
    {TRIGGER_TABLE_END},
};

/// Angara Cavern's exits and stairs, its door, and pushing, Move and Carry on
/// either grey block until they are stacked.
struct Trigger gCaverns_AngaraTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(Caverns_OpenAngaraDoor)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_PUSH, 0, FLAG_ANGARA_BLOCKS_STACKED, EVENT(Caverns_StartMovingAngaraBlock)},
    {TRIGGER_PUSHED, 0, FLAG_ANGARA_BLOCKS_STACKED, EVENT(Caverns_FinishMovingAngaraBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_BLOCK, FLAG_ANGARA_BLOCKS_STACKED, EVENT(Caverns_StartMovingAngaraBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_BLOCK, FLAG_ANGARA_BLOCKS_STACKED, EVENT(Caverns_FinishMovingAngaraBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SECOND_BLOCK, FLAG_ANGARA_BLOCKS_STACKED, EVENT(Caverns_StartMovingAngaraBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_BLOCK, FLAG_ANGARA_BLOCKS_STACKED, EVENT(Caverns_FinishMovingAngaraBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_BLOCK, FLAG_ANGARA_BLOCKS_STACKED, EVENT(Caverns_StartMovingAngaraBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_BLOCK, FLAG_ANGARA_BLOCKS_STACKED, EVENT(Caverns_FinishMovingAngaraBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SECOND_BLOCK, FLAG_ANGARA_BLOCKS_STACKED, EVENT(Caverns_StartMovingAngaraBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_SECOND_BLOCK, FLAG_ANGARA_BLOCKS_STACKED, EVENT(Caverns_FinishMovingAngaraBlock)},
    {TRIGGER_TABLE_END},
};
