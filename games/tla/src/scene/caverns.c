/// @file
/// The four small caves that each keep a Summon's stone tablet, in four scenes
/// that share this overlay. In Indra Cavern, west of where the Lemurian ship
/// lands on Indra (SceneId_CavernsIndra), Lash ties a rope up to a ledge and
/// Move pushes a stone pillar across a gap, so that Felix can leap to Zagan's
/// tablet (Caverns_JumpEastAcrossGap). In Osenia Cavern, far north of Mikasalla
/// (CavernsOsenia), a maze of rocks walls Megaera's tablet in: Felix climbs
/// down a hole into a tunnel, and Scoop digs the hole at its other end
/// (Caverns_ScoopOseniaPassage). In Atteka Cavern, which only the flying ship
/// reaches (CavernsAtteka), Parch dries the pool before Coatlicue's tablet
/// (Caverns_ParchAttekaWater). In Angara Cavern, north of Loho
/// (CavernsAngara), a stone door leads down to Haurus's chamber
/// (Caverns_OpenAngaraDoor), where two grey spiral blocks are pushed, Moved and
/// Carried until one stands on the other as a step up to the tablet
/// (Caverns_FinishMovingAngaraBlock).
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
/// Angara Cavern: one block carried on top of the other, and the cells about
/// the block's path raised while it moves.
#define FLAG_ANGARA_BLOCK_ON_TOP FLAG_SCENE_TEMP_12
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

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
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

/// Nothing visible happens: Angara Cavern's two spiral blocks are made solid
/// where they stand. The ground under each is raised by a block's height, two
/// steps, and its cell marked as one the leader jumps onto, so that Felix can
/// climb onto a block and stand on it.
void Caverns_PlaceAngaraBlocks(const s16 *data) {
  const u16 *ids = (const u16 *)data;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *actor;
  s32 height;
  s32 cell;

  for (; *ids != ACTOR_LIST_END; ids++) {
    actor = Actor_GetById(*ids);
    actor->motionFlags = 0;
    actor->collisionFlags = 0;
    actor->bounce = 0;
    Actor_SetSpriteFlags((struct Actor *)actor, 0);
    height = CELL_HEIGHT_STEPS(
                 Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed)) +
             2;
    MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, height);
    MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 2, height);
    cell = MapCell_GetAttributes(0, FIELD_CELL(actor->x.fixed),
                                 FIELD_CELL(actor->z.fixed));
    actor->riseBaseY = cell;
    MapCell_SetAttributes(0, FIELD_CELL(actor->x.fixed),
                          FIELD_CELL(actor->z.fixed), cell | MAP_CELL_JUMP);
  }
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    wait(1);
    leader->y.fixed = leader->groundHeight =
        Map_GetTerrainHeight(leader->layer, leader->x.fixed, leader->z.fixed);
    leader->velocityY = 0;
    leader->targetY = ACTOR_NO_TARGET;
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
  height = CELL_HEIGHT_STEPS(GetHeight(x, z)) + offset;                        \
  FieldMapGrid_SetCellHeight(x, z, 2, height)

/// The z of Angara Cavern's two rows of cells the blocks start in: the second
/// block's, and the first's south of it.
#define ANGARA_NORTH_ROW_Z 536
#define ANGARA_SOUTH_ROW_Z 552

/// Nothing visible happens: while a spiral block moves, the floor around its
/// path in Angara's chamber is raised out of its way (`offset` 1) and lowered
/// back afterwards (-1), by eight steps: the two cells by the ledge while the
/// block is up on it, else the three cells of whichever of the two rows the
/// block is not in.
void Caverns_ShiftAngaraCellHeights(s32 offset) {
  struct Actor *block = gFieldWork->movedBlock;
  s32 height;

  offset <<= 3;
  if (block->y.part[0].integer > 31 ||
      Event_GetFlag(FLAG_ANGARA_CELLS_RAISED)) {
    SET_CELL_HEIGHT(toFIXED(568), toFIXED(ANGARA_NORTH_ROW_Z));
    SET_CELL_HEIGHT(toFIXED(584), toFIXED(ANGARA_SOUTH_ROW_Z));
    if (offset == 8)
      Event_SetFlag(FLAG_ANGARA_CELLS_RAISED);
    else
      Event_ClearFlag(FLAG_ANGARA_CELLS_RAISED);
  } else if (Actor_GetPixelZ(block) == ANGARA_NORTH_ROW_Z ||
             Event_GetFlag(FLAG_ANGARA_CELLS_RAISED)) {
    SET_CELL_HEIGHT(toFIXED(536), toFIXED(ANGARA_SOUTH_ROW_Z));
    SET_CELL_HEIGHT(toFIXED(552), toFIXED(ANGARA_SOUTH_ROW_Z));
    SET_CELL_HEIGHT(toFIXED(568), toFIXED(ANGARA_SOUTH_ROW_Z));
    // DEAD: offset is 8 or -8, never 6, so this branch only clears the flag;
    // the ROM keeps the compare and the set.
    if (offset == 6)
      Event_SetFlag(FLAG_ANGARA_CELLS_RAISED);
    else
      Event_ClearFlag(FLAG_ANGARA_CELLS_RAISED);
  } else {
    SET_CELL_HEIGHT(toFIXED(536), toFIXED(ANGARA_NORTH_ROW_Z));
    SET_CELL_HEIGHT(toFIXED(552), toFIXED(ANGARA_NORTH_ROW_Z));
    SET_CELL_HEIGHT(toFIXED(568), toFIXED(ANGARA_NORTH_ROW_Z));
  }
}

/// Nothing visible happens as Felix starts to push, Move or Carry one of the
/// two spiral blocks in Angara Cavern's chamber: the game finds the block (the
/// one ahead of him, or the one the Psynergy's hand holds) and lowers the
/// ground under it back to the floor, so that the block can leave its cell.
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
          FIELD_CELL(position[2]) == FIELD_CELL(actor->z.fixed))
        break;
      ids++;
      actor = NULL;
    }
    if (actor == NULL)
      return;
    work->movedBlock = actor;
    Caverns_ShiftAngaraCellHeights(1);
  }
  height = CELL_HEIGHT_STEPS(
               Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed)) -
           2;
  MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, height);
  MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 2, height);
  MapCell_SetAttributes(0, FIELD_CELL(actor->x.fixed),
                        FIELD_CELL(actor->z.fixed), actor->riseBaseY);
}

/// A dust puff loses a DUST_DRAG_X-th of its speed east or west each frame, and
/// a DUST_DRAG_Z-th of its speed north or south.
enum { DUST_DRAG_X = 12, DUST_DRAG_Z = 10 };

/// One puff of the ring of dust a fallen block throws up: each frame it drifts
/// outwards, slows, grows and spins.
void Caverns_DriftDust(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;

  effect->x += effect->velocity.x;
  effect->y += effect->velocity.y;
  effect->z += effect->velocity.z;
  effect->velocity.x -= effect->velocity.x / DUST_DRAG_X;
  effect->velocity.z -= effect->velocity.z / DUST_DRAG_Z;
  effect->scaleX += effect->scaleRateX;
  effect->scaleY += effect->scaleRateY;
  effect->sprite->rotation += effect->spin;
}

/// The block stacked on top of the other, each frame: Felix is drawn in front
/// of it while he stands south of it, and behind it otherwise.
void Caverns_LayerLeaderByTopBlock(struct Actor *actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  FIXED leaderZ = leader->z.fixed;
  FIXED z = actor->z.fixed;
  u8 *flags = &leader->priorityFlags;

  if (leaderZ > z) {
    ActorFlags_Set(flags, ACTOR_PRIORITY_OVERHEAD);
  } else {
    *flags &= ~(ACTOR_PRIORITY_UNDERFOOT | ACTOR_PRIORITY_OVERHEAD);
  }
}

/// When a push, Move or Carry has finished moving one of the spiral blocks in
/// Angara Cavern's chamber, a block left with no ground under it drops: onto
/// the floor below with a thump and a ring of dust, or onto the other block,
/// where it stays, the two stacked into a step up to Haurus's tablet. Then the
/// ground under the block rises again so that it stands solid where it is.
/// A block dropping onto a cell that has a behaviour sinks out of sight
/// instead, as a yellow block, and is gone.
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
      REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
      /* FAKEMATCH(temporary): sum holds kind + 2 for the one call; as
       * kind + 2 at its use. */
      sum = kind + 2;
      spawned = OverlayObject_PrepareObject(
          actor->x.fixed, 0, actor->z.fixed + FIELD_CELL_ORIGIN(sum),
          SPRITE_YELLOW_BLOCK);
      Sound_Play(SE_DUST_PUFF);
      OverlayObject_WaitUntilIdle(actor);
      OverlayObject_WaitUntilIdle(actor);
      actor->x.fixed = actor->y.fixed = actor->z.fixed = 0;
      Actor_Release(spawned);
      wait(1);
      REG_BLDCNT = 0;
    } else {
      OverlayObject_WaitUntilIdle(actor);
      OverlayObject_WaitUntilIdle(actor);
      Sound_Play(SE_THUMP);
      options.palette = SPRITE_COLOR_WHITE;
      options.priority = 2;
      options.update = Caverns_DriftDust;
      options.startScaleX = FIXED_ONE * 4 / 5;
      options.startScaleY = FIXED_ONE * 4 / 5;
      i = 0;
      do {
        angle = i * (TRIG_TURN / DUST_RING_PUFFS);
        velocity[0] = Trig_Cos(angle) * 2;
        velocity[1] = 0;
        velocity[2] = Trig_Sin(angle) * 2;
        velocity[0] += velocity[0] >> 1;
        velocity[0] += (u16)(Random16() * 3 >> 2) - (toFIXED(3) / 8);
        velocity[2] += (u16)(Random16() * 3 >> 3) - (toFIXED(3) / 16);
        /* FAKEMATCH(dead): sum, never read again, assigned the flags inside the
         * call's arguments. */
        FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed,
                          velocity[0], velocity[1], velocity[2],
                          sum = EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                                EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                          &options);
        i++;
      } while (i <= DUST_RING_PUFFS);
    }
    actor->motionFlags = 0;
    actor->layer = 0;
    // The velocity array is reused for the position under the actor.
    velocity[0] = actor->x.fixed;
    velocity[1] = actor->y.fixed - toFIXED(16);
    velocity[2] = actor->z.fixed;
    other = Actor_FindAtCell(velocity, 1);
    if (other != NULL) {
      actor->update = (ObjectUpdate)Caverns_LayerLeaderByTopBlock;
      actor->riseCounter = 1;
      other->riseCounter = 1;
      GameFlag_Set(FLAG_ANGARA_BLOCK_ON_TOP);
    }
  }
  height = CELL_HEIGHT_STEPS(
               Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed)) +
           2;
  MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, height);
  MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 2, height);
  value = MapCell_GetAttributes(0, FIELD_CELL(actor->x.fixed),
                                FIELD_CELL(actor->z.fixed));
  actor->riseBaseY = value;
  MapCell_SetAttributes(0, FIELD_CELL(actor->x.fixed),
                        FIELD_CELL(actor->z.fixed), value | MAP_CELL_JUMP);
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

/// Indra Cavern's row of cells the pillar stands and moves in (z 168), and the
/// column of its cell marked as a gap to jump onto, which is copied under the
/// pillar.
#define INDRA_PILLAR_ROW 10
#define INDRA_GAP_COLUMN 16

/// Indra Cavern's things: the post on the floor with the rope Lash throws, the
/// post up on the east ledge it is tied to, and the stone pillar Move pushes
/// across the gap between the ledges.
enum CavernsIndraActorId {
  ACTOR_LOWER_ROPE_POST = ACTOR_FIRST_PLACED,
  ACTOR_UPPER_ROPE_POST,
  ACTOR_PILLAR
};

/// Indra Cavern on entry: the rope lies by its post, or runs up to the ledge
/// once Lash has tied it (FLAG_INDRA_ROPE_TIED); the stone pillar stands still,
/// and its cell is one the leader jumps onto.
void Caverns_InitializeIndra(void) {
  struct Actor *actor;

  Actor_GetById(ACTOR_FELIX)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Rope_Initialize();
  Rope_Place(0, FLAG_INDRA_ROPE_TIED, ACTOR_LOWER_ROPE_POST,
             ACTOR_UPPER_ROPE_POST);
  actor = Actor_GetById(ACTOR_PILLAR);
  actor->motionFlags = 0;
  {
    s32 row = INDRA_PILLAR_ROW;

    Map_CopyCellAttributeRect(INDRA_GAP_COLUMN, row, 1, 1,
                              FIELD_CELL(actor->x.fixed), row);
  }
}

/// Move on the stone pillar at the edge of the ledge by Zagan's tablet pushes
/// it one cell east into the gap; afterwards its new cell is the one the
/// leader jumps onto, and the cells it left are plain floor again.
void Caverns_MoveIndraPillar(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);

  actor->motionFlags = 0;
  Map_CopyCellAttributeRect(13, 12, 2, 1, 13, INDRA_PILLAR_ROW);
  Map_CopyCellAttributeRect(INDRA_GAP_COLUMN, INDRA_PILLAR_ROW, 1, 1,
                            FIELD_CELL(actor->x.fixed), INDRA_PILLAR_ROW);
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
    actor->motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
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

/// Walking east off the ledge by Zagan's tablet in Indra Cavern (cell (12, 10))
/// makes Felix spring and leap two cells east over the gap, when nothing blocks
/// where he would land.
void Caverns_JumpEastAcrossGap(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 position[3];
  FIXED x;

  // Once Move has pushed the stone pillar east into the gap, Felix does not
  // leap: walking east from the ledge he steps onto the pillar's top, level
  // with the ledges, and on onto the far ledge by the rope's post.
  x = leader->x.fixed;
  x += toFIXED(32);
  SET_VECTOR(position, x, leader->y.fixed, leader->z.fixed);
  Caverns_JumpAcross(leader, (struct FieldCoordinate *)position);
}

/// Osenia Cavern's one actor, the row of gCaverns_Placements: an unseen marker
/// Caverns_WalkIntoPassage sets down by the hole as Felix climbs into it.
enum CavernsOseniaActorId { ACTOR_PASSAGE_MARKER = ACTOR_FIRST_PLACED };

void Caverns_OpenOseniaPassage(void);

/// Osenia Cavern's map: once Scoop has dug it, the second hole stays open.
void Caverns_PrepareOsenia(void) {
  s32 scooped;

  scooped = Event_GetFlag(FLAG_OSENIA_CAVERN_SCOOPED);
  if (scooped != 0) {
    Caverns_OpenOseniaPassage();
  }
  return;
}

/// Digs Osenia Cavern's second hole into the map: a hole and the cells around
/// it, copied with their behaviours from a spare one beside the cave, over the
/// sand in the pocket of rocks (cells (18, 12) to (20, 14)).
void Caverns_OpenOseniaPassage(void) {
  MapGrid_CopyCellPictureIndicesRect(35, 12, 18, 12, 3, 3);
  MapGrid_CopyCellAttributes(35, 12, 3, 3, 18, 12);
  MapGrid_CopyCellPictureIndicesRect(84, 13, 83, 13, 1, 1);
  return;
}

/// Felix, over one of Osenia Cavern's two holes, turns his back and climbs down
/// into it, and the screen leaves for the tunnel below: from the west hole to
/// its west end, from the hole Scoop dug to its east end.
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

/// Walking east into one of Osenia Cavern's holes in the floor (cell (8, 13),
/// or (18, 13) once Scoop has dug the second) brings Felix round over the hole,
/// and he climbs down into it to the tunnel below.
void Caverns_WalkEastIntoPassage(void) {
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -14);
  Actor_WalkByAndWaitById(ACTOR_FELIX, 16, 0);
  Event_Wait(10);
  Caverns_WalkIntoPassage();
  return;
}

/// Walking west into one of Osenia Cavern's holes in the floor (cell (10, 13),
/// or (20, 13) once Scoop has dug the second) brings Felix round over the hole,
/// and he climbs down into it to the tunnel below.
void Caverns_WalkWestIntoPassage(void) {
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -14);
  Actor_WalkByAndWaitById(ACTOR_FELIX, -16, 0);
  Event_Wait(10);
  Caverns_WalkIntoPassage();
  return;
}

/// Walking south into one of Osenia Cavern's holes in the floor (cell (9, 12),
/// or (19, 12) once Scoop has dug the second) has Felix turn his back, as on a
/// ladder, and climb down into it to the tunnel below.
void Caverns_WalkSouthIntoPassage(void) {
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
  Event_Wait(10);
  Caverns_WalkIntoPassage();
  return;
}

/// Scoop on the sand in the pocket of rocks east of Osenia Cavern's middle
/// (cell (19, 13)) digs a second hole there for good, the tunnel's way up to
/// Megaera's side of the rocks.
void Caverns_ScoopOseniaPassage(s32 mode) {
  if (mode == 1) {
    Caverns_OpenOseniaPassage();
    GameFlag_Set(FLAG_OSENIA_CAVERN_SCOOPED);
  }
  return;
}

/// Atteka Cavern's map: once Parch has dried the pool, it stays dry.
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

/// Parch on the green pool before Coatlicue's tablet in Atteka Cavern dries it
/// up for good in a burst of heat and a white flash, opening the way to the
/// tablet.
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

/// Angara Cavern's map: when the party arrives by entrance 5 or 6, the two
/// staircases at cells (28, 5) and (36, 5) lead on by the exits of cells
/// (28, 0) and (36, 0) instead of their own.
void Caverns_PrepareAngara(void) {
  if ((u16)(gMainRoutineWork.entrance - 5) <= 1) {
    MapGrid_CopyCellAttributes(28, 0, 1, 1, 28, 5);
    Map_CopyCellAttributeRect(36, 0, 1, 1, 36, 5);
  }
}

/// Angara Cavern's things in Haurus's chamber, numbering
/// gCaverns_AngaraPlacements.
enum CavernsAngaraActorId {
  /// The two grey blocks carved with a spiral, one on the floor and one up on
  /// the ledge, carried until one stands on top of the other.
  ACTOR_FIRST_BLOCK = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_BLOCK,
  /// The lit candlesticks on either side of Haurus's tablet, which the searches
  /// for a block skip.
  ACTOR_WEST_CANDLESTICK,
  ACTOR_EAST_CANDLESTICK
};

/// Angara Cavern on entry: the two spiral blocks stand solid where they were
/// left, are drawn over the ground's rises, and the tablet's candlesticks are
/// left out of the search for a block to push.
void Caverns_InitializeAngara(void) {
  Caverns_PlaceAngaraBlocks(gCaverns_AngaraBlocks);
  Actor_GetById(ACTOR_FIRST_BLOCK)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Actor_GetById(ACTOR_SECOND_BLOCK)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Actor_GetById(ACTOR_WEST_CANDLESTICK)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_GetById(ACTOR_EAST_CANDLESTICK)->collisionFlags |= ACTOR_NOT_TARGETED;
}

/// Walking north into the grey stone door at the west end of the small cave
/// room in Angara Cavern (cell (6, 32)) opens it, and the screen wipes to
/// Haurus's chamber beyond.
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

/// The spiral blocks of Angara Cavern's chamber, by actor id, ended by -1.
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

/// Osenia Cavern's exit regions: the tunnel's two ends, climbed up to the west
/// hole (exit 4) and to the one Scoop digs (exit 5).
s16 gCaverns_OseniaRegions[] = {
    132, 28, 692, 140, 36, 700, -1, 4,
    292, 28, 692, 300, 36, 700, -1, 5,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(CavernsOsenia),
    SCENE_EXIT_UNLESS(CAVERNS_OSENIA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_CAVERNS_OSENIA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, CavernsOsenia, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, CavernsOsenia, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, CavernsOsenia, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, CavernsOsenia, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(CavernsIndra),
    SCENE_EXIT_UNLESS(CAVERNS_INDRA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_CAVERNS_INDRA, EXIT_FLAG_NONE),
    SCENE_EXITS(CavernsAtteka),
    SCENE_EXIT_UNLESS(CAVERNS_ATTEKA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_CAVERNS_ATTEKA, EXIT_FLAG_NONE),
    SCENE_EXITS(CavernsAngara),
    SCENE_EXIT_UNLESS(CAVERNS_ANGARA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_CAVERNS_ANGARA, EXIT_FLAG_NONE),
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
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
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

/// Osenia Cavern's exit, its two holes walked into from three sides, and Scoop
/// on the sand where the second is dug.
struct Trigger gCaverns_OseniaTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, CAVERNS_OSENIA_TO_WORLD_MAP},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 2, CONDITION_ALWAYS, EVENT(Caverns_WalkSouthIntoPassage)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 2, CONDITION_ALWAYS, EVENT(Caverns_WalkWestIntoPassage)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 2, CONDITION_ALWAYS, EVENT(Caverns_WalkEastIntoPassage)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 3, CONDITION_ALWAYS, EVENT(Caverns_WalkSouthIntoPassage)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 3, CONDITION_ALWAYS, EVENT(Caverns_WalkWestIntoPassage)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 3, CONDITION_ALWAYS, EVENT(Caverns_WalkEastIntoPassage)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 10, FLAG_OSENIA_CAVERN_SCOOPED, EVENT(Caverns_ScoopOseniaPassage)},
    {TRIGGER_TABLE_END},
};

/// Indra Cavern's exit, the ledge's edge by the gap, Lash on the rope post and
/// Move on the pillar.
struct Trigger gCaverns_IndraTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, CAVERNS_INDRA_TO_WORLD_MAP},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 10, CONDITION_ALWAYS, EVENT(Caverns_JumpEastAcrossGap)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_LOWER_ROPE_POST, FLAG_INDRA_ROPE_TIED, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, CONDITION_ALWAYS, EVENT(Caverns_MoveIndraPillar)},
    {TRIGGER_TABLE_END},
};

/// Atteka Cavern's exit and Parch on its pool.
struct Trigger gCaverns_AttekaTriggers[] = {
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, CAVERNS_ATTEKA_TO_WORLD_MAP},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 60, CONDITION_ALWAYS, EVENT(Caverns_ParchAttekaWater)},
    {TRIGGER_TABLE_END},
};

/// Angara Cavern's exits and stairs, its stone door, and pushing, Move and Carry
/// on either spiral block until they are stacked.
struct Trigger gCaverns_AngaraTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, CAVERNS_ANGARA_TO_WORLD_MAP},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(Caverns_OpenAngaraDoor)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_PUSH, 0, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(Caverns_StartMovingAngaraBlock)},
    {TRIGGER_PUSHED, 0, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(Caverns_FinishMovingAngaraBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_BLOCK, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(Caverns_StartMovingAngaraBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_BLOCK, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(Caverns_FinishMovingAngaraBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SECOND_BLOCK, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(Caverns_StartMovingAngaraBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_BLOCK, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(Caverns_FinishMovingAngaraBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_BLOCK, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(Caverns_StartMovingAngaraBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_BLOCK, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(Caverns_FinishMovingAngaraBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SECOND_BLOCK, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(Caverns_StartMovingAngaraBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_SECOND_BLOCK, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(Caverns_FinishMovingAngaraBlock)},
    {TRIGGER_TABLE_END},
};
