/// @file
/// The four caverns that each keep a Summon tablet, in four scenes that share
/// this overlay: Indra Cavern west of the Lemurian ship (SceneId_Caverns1),
/// where Lash and Move reach Zagan's tablet; Osenia Cavern north of Mikasalla
/// (Caverns2), where Scoop opens the way to Megaera's
/// (Caverns_ScoopOseniaPassage); Atteka Cavern (Caverns3), where Parch dries
/// the water before Coatlicue's (Caverns_ParchAttekaWater); and Angara Cavern
/// north of Loho (Caverns4), where two grey blocks are carried and stacked to
/// reach Haurus's (Caverns_StartMovingAngaraBlock,
/// Caverns_FinishMovingAngaraBlock).
#include "types.h"
#include "system/io_reg.h"
#include "scene_tables.h"
#include "system/ram_buffer.h"
#include "field_event.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"

// Story flags: Osenia Cavern's passage dug open by Scoop, Atteka Cavern's water
// dried by Parch.
#define FLAG_OSENIA_CAVERN_SCOOPED 0x9af
#define FLAG_ATTEKA_CAVERN_PARCHED 0x917
// Angara Cavern: one block carried onto the other, and the cells about the
// block's path raised while it moves.
#define FLAG_ANGARA_BLOCKS_STACKED FLAG_SCENE_TEMP_12
#define FLAG_ANGARA_CELLS_RAISED FLAG_SCENE_TEMP_13

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct CavernsMapLayer {
  u8 unk00[0x30];
  u8 *cells;
  u8 unk34[4];
};

struct CavernsMapWork {
  u8 unk000[0x108];
  struct CavernsMapLayer layers[3];
};

extern char SceneId_Caverns1;
extern char SceneId_Caverns2;
extern char SceneId_Caverns3;
extern char SceneId_Caverns4;
extern u8 gMapCellBuffer[];
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
void Actor_SelectAnimation(struct Actor *object, s32 value);
s32 MapGrid_CheckCollision(struct Actor *object, s16 *position);
void Rope_Initialize(void);
void Rope_Place(s32 a, s32 b, s32 c, s32 d);
void Caverns_InitializeIndra(void);
void Caverns_PrepareAngara(void);
void Caverns_InitializeAngara(void);
// Jumps the leader across to the position given when nothing is in the way.
s32 Caverns_JumpAcross(struct Actor *object, s16 *position);
#include "flags.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "module/field/object.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "field/event_inline.h"
#include "actor.h"
#include "field_sprite.h"
#include "system/ram_buffer.h"
#include "system/field_effect.h"
#include "fixed_math.h"
#include "macros.h"
#include "system/main_routine_work.h"
#include "field/field_sequence_event.h"
#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"

#include "shared/scene_create_imported_actor.h"

#include "../field/field_effect/move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect/spawn.c"

struct Actor;

void MapGrid_SetCellHeight(s32 x, s32 z, s32 layer, s32 height);

struct Actor *Actor_FindAtCell(s32 *position, s32 mode);

extern u32 gFieldEffect_ShortScript[];

extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const s16 *Scene_GetRegions(void) {
  const s16 *result = NULL;

  if (gMainRoutineWork.scene == (s32)&SceneId_Caverns2)
    result = gCaverns_OseniaRegions;
  return result;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Caverns1)
    return gCaverns_IndraPlacements;
  if (scene == (s32)&SceneId_Caverns4)
    return gCaverns_AngaraPlacements;
  return gCaverns_Placements;
}

s32 MapCell_GetAttributes(u32 layer, s32 x, s32 y) {
  struct CavernsMapWork *map = (struct CavernsMapWork *)Ram_HeapSlots->mapWork;
  u8 *cell;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += (x + (y << 7)) << 2;
  return (cell[2] << 8) | cell[3];
}

s32 MapCell_SetAttributes(u32 layer, s32 x, s32 y, s32 value) {
  struct CavernsMapWork *map = (struct CavernsMapWork *)Ram_HeapSlots->mapWork;
  u8 *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += (x + (y << 7)) << 2;
  old = (cell[2] << 8) | cell[3];
  cell[2] = value >> 8;
  cell[3] = value;
  return old;
}

#include "../field/object/wait_until_idle.c"

#include "text/msg_ids.h"

// Sets the grey blocks of Angara Cavern on their cells: each cell rises two
// steps under its block and is marked occupied.
void Caverns_PlaceAngaraBlocks(const s16 *data) {
  const u16 *ids = (const u16 *)data;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object;
  s32 height;
  s32 cell;

  if (*ids == 0xffff)
    goto done;
loop:
  object = Actor_GetById(*ids);
  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->bounce = 0;
  Actor_SetSpriteFlags((struct Actor *)object, 0);
  height = (Map_GetTerrainHeight(0, object->x, object->z) >> 19) + 2;
  MapGrid_SetCellHeight(object->x, object->z, 0, height);
  MapGrid_SetCellHeight(object->x, object->z, 2, height);
  cell = MapCell_GetAttributes(0, object->x >> 20, object->z >> 20);
  object->unk4c = cell;
  MapCell_SetAttributes(0, object->x >> 20, object->z >> 20, cell | 0x80);
  ids++;
  if (*ids != 0xffff)
    goto loop;
done:
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    wait(1);
    leader->y = leader->terrainHeight =
        Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
    leader->velocityY = 0;
    leader->targetY = 0x80000000;
    FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 0);
  }
}

static __inline__ s32 GetHeight(s32 x, s32 z) {
  return Map_GetTerrainHeight(2, x, z);
}

static __inline__ void SetHeight(s32 x, s32 z, s32 height) {
  MapGrid_SetCellHeight(x, z, 2, height);
}

static __inline__ s32 Half(u8 *base, s32 offset) {
  return HALF_AT(base, offset);
}

#define SET_CELL_HEIGHT(x, z)                                                  \
  height = (GetHeight(x, z) >> 19) + offset;                                   \
  SetHeight(x, z, height)

// Raises (offset 1) or lowers (-1) by a step the cells about the moving block's
// path, by where the block stands.
void Caverns_ShiftAngaraCellHeights(s32 offset) {
  u8 *work = (u8 *)gFieldWork->movedBlock;
  s32 height;

  offset <<= 3;
  if (HALF_AT(work, 14) > 31 || Event_GetFlag(FLAG_ANGARA_CELLS_RAISED)) {
    SET_CELL_HEIGHT(142 << 18, 134 << 18);
    SET_CELL_HEIGHT(146 << 18, 138 << 18);
    if (offset == 8)
      Event_SetFlag(FLAG_ANGARA_CELLS_RAISED);
    else
      Event_ClearFlag(FLAG_ANGARA_CELLS_RAISED);
  } else if (Half(work, 18) == 536 || Event_GetFlag(FLAG_ANGARA_CELLS_RAISED)) {
    SET_CELL_HEIGHT(134 << 18, 138 << 18);
    SET_CELL_HEIGHT(138 << 18, 138 << 18);
    SET_CELL_HEIGHT(142 << 18, 138 << 18);
    if (offset == 6)
      Event_SetFlag(FLAG_ANGARA_CELLS_RAISED);
    else
      Event_ClearFlag(FLAG_ANGARA_CELLS_RAISED);
  } else {
    SET_CELL_HEIGHT(134 << 18, 134 << 18);
    SET_CELL_HEIGHT(138 << 18, 134 << 18);
    SET_CELL_HEIGHT(142 << 18, 134 << 18);
  }
}

// Before a push, Move or Carry: finds the block ahead of the leader (or the one
// Psynergy holds) and frees its cell.
void Caverns_StartMovingAngaraBlock(void) {
  s32 position[3];
  u8 *work = Ram_HeapSlots->fieldWork;
  struct CurrentUnit *current = (struct CurrentUnit *)Ram_HeapSlots->effectWork;
  const u16 *ids = (const u16 *)gCaverns_AngaraBlocks;
  struct Actor *object = NULL;
  struct Actor *leader;
  s32 height;
  s32 angle;

  WORD_AT(work, 0xcbc) = 0;
  if (current != NULL && HALF_AT(work, 358) != 0) {
    object = (struct Actor *)(WORD_AT(work, 0xcbc) = (s32)current->object);
    if (current->unk1c != 147)
      Caverns_ShiftAngaraCellHeights(1);
  } else {
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    angle = (leader->facing + 0x2000) & 0xc000;
    position[0] = leader->x;
    position[1] = leader->y;
    position[2] = leader->z;
    Vector_AddPolarOffset(0x100000, angle, position);
    if (*ids == 0xffff)
      goto search_done;
  search:
    object = Actor_GetById(*ids);
    if (position[0] >> 20 == object->x >> 20 &&
        position[1] / 0x10000 == object->y / 0x10000 &&
        position[2] >> 20 == object->z >> 20)
      goto search_done;
    ids++;
    object = NULL;
    if (*ids != 0xffff)
      goto search;
  search_done:
    if (object == NULL)
      return;
    WORD_AT(work, 0xcbc) = (s32)object;
    Caverns_ShiftAngaraCellHeights(1);
  }
  height = (Map_GetTerrainHeight(0, object->x, object->z) >> 19) - 2;
  MapGrid_SetCellHeight(object->x, object->z, 0, height);
  MapGrid_SetCellHeight(object->x, object->z, 2, height);
  MapCell_SetAttributes(0, object->x >> 20, object->z >> 20, object->unk4c);
}

// A dust puff's step: drifts, slows, grows and turns.
void Caverns_DriftDust(struct Actor *object) {
  object->x += object->bounce;
  object->y += object->gravity;
  object->z += object->unk4c;
  object->bounce -= object->bounce / 12;
  object->unk4c -= object->unk4c / 10;
  object->scaleX += object->speedLimit;
  object->scaleY += object->acceleration;
  ((struct FieldSprite *)object->sprite)->rotation += object->timer;
}

// The stacked block's update: draws the leader in front of it while he is
// south of it.
void Caverns_LayerLeaderByStack(struct Actor *object) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 leaderZ = leader->z;
  s32 z = object->z;
  u8 *flags = &leader->priorityFlags;

  if (leaderZ > z) {
    s32 value = 4;

    value |= *flags;
    *flags = value;
  } else {
    *flags &= ~6;
  }
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
void Actor_Release(struct Actor *object);

// After the block has moved: a block left over a drop falls with a thump and a
// ring of dust, and one landing on the other block stacks on it; then its new
// cell rises under it.
void Caverns_FinishMovingAngaraBlock(void) {
  s32 velocity[3];
  struct EffectOptions options;
  u8 *work;
  u8 *current;
  struct Actor *object;
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
  current = Ram_HeapSlots->effectWork;
  object = FIELD_AT(work, struct Actor *, 3260);
  if (current != NULL && FIELD_AT(work, s16, 358) != 0) {
    if (FIELD_AT(current, s16, 28) != 147)
      Caverns_ShiftAngaraCellHeights(-1);
  } else {
    Caverns_ShiftAngaraCellHeights(-1);
  }
  if (Map_GetTerrainHeight(0, object->x, object->z) != object->y) {
    object->terrainId = 2;
    object->motionFlags = 3;
    object->bounce = 0;
    wait(1);
    kind = MapGrid_GetCellBehaviorAt(2, object->x, object->z);
    if (kind != 0) {
      object->priorityFlags |= 2;
      REG_BLDALPHA = 0x1000;
      blendControl = (u16 *)&REG_BLDCNT;
      *blendControl = 0x3f10;
      sum = kind + 2;
      spawned = OverlayObject_PrepareObject(object->x, 0,
                                            object->z + (sum << 20), 312);
      Sound_Play(SE_DUST_PUFF);
      OverlayObject_WaitUntilIdle(object);
      OverlayObject_WaitUntilIdle(object);
      object->x = object->y = object->z = 0;
      Actor_Release(spawned);
      wait(1);
      *blendControl = 0;
    } else {
      OverlayObject_WaitUntilIdle(object);
      OverlayObject_WaitUntilIdle(object);
      Sound_Play(SE_THUMP);
      options.palette = 7;
      options.priority = 2;
      options.update = (void (*)(union FieldObject *))Caverns_DriftDust;
      options.startScaleX = 0xcccc;
      options.startScaleY = 0xcccc;
      i = 0;
    loop:
      {
        angle = i << 12;
        velocity[0] = Trig_Cos(angle) * 2;
        velocity[1] = 0;
        velocity[2] = Trig_Sin(angle) * 2;
        velocity[0] += velocity[0] >> 1;
        velocity[0] += ((u32)(Random16() * 3 << 14) >> 16) - 0x6000;
        velocity[2] += ((u32)(Random16() * 3 << 13) >> 16) - 0x3000;
        /* FAKEMATCH(dead): sum, never read again, assigned the flags inside the
         * call's arguments. */
        FieldEffect_Spawn(object->x, object->y, object->z, velocity[0],
                          velocity[1], velocity[2], sum = 0x10b0000, &options);
        i++;
      }
      if (i <= 16)
        goto loop;
    }
    object->motionFlags = 0;
    object->terrainId = 0;
    // The velocity array is reused for the position under the object.
    {
      velocity[0] = object->x;
      velocity[1] = object->y - (16 << 16);
      velocity[2] = object->z;
      other = Actor_FindAtCell(velocity, 1);
      if (other != NULL) {
        object->update = (void (*)(void))Caverns_LayerLeaderByStack;
        object->riseCounter = 1;
        other->riseCounter = 1;
        GameFlag_Set(FLAG_ANGARA_BLOCKS_STACKED);
      }
    }
  }
  height = (Map_GetTerrainHeight(0, object->x, object->z) >> 19) + 2;
  MapGrid_SetCellHeight(object->x, object->z, 0, height);
  MapGrid_SetCellHeight(object->x, object->z, 2, height);
  value = MapCell_GetAttributes(0, object->x >> 20, object->z >> 20);
  object->unk4c = value;
  MapCell_SetAttributes(0, object->x >> 20, object->z >> 20, value | 128);
}

const struct Trigger *Scene_GetEvents(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Caverns1)
    return gCaverns_IndraTriggers;
  if (scene == (s32)&SceneId_Caverns3)
    return gCaverns_AttekaTriggers;
  if (scene == (s32)&SceneId_Caverns4)
    return gCaverns_AngaraTriggers;
  return gCaverns_OseniaTriggers;
}

s32 Scene_Initialize(void) {
  gFieldWork->transition = 0x204;
  if (gMainRoutineWork.scene == (s32)&SceneId_Caverns1)
    Caverns_InitializeIndra();
  if (gMainRoutineWork.scene == (s32)&SceneId_Caverns4)
    Caverns_InitializeAngara();
  return 0;
}

s32 Scene_PrepareMap(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Caverns2)
    Caverns_PrepareOsenia();
  else if (scene == (s32)&SceneId_Caverns3)
    Caverns_PrepareAtteka();
  else if (scene == (s32)&SceneId_Caverns4)
    Caverns_PrepareAngara();
  return 0;
}

// Indra Cavern: the rope Lash reaches and the pillar Move pushes.
void Caverns_InitializeIndra(void) {
  struct Actor *object;
  s32 arg;

  Actor_GetById(4)->priorityFlags |= 0x20;
  Rope_Initialize();
  Rope_Place(0, 0x200, 8, 9);
  object = Actor_GetById(10);
  object->motionFlags = 0;

  arg = 10;
  Map_CopyCellAttributeRect(16, arg, 1, 1, object->x >> 20, arg);
}

void Caverns_MoveIndraPillar(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  object->motionFlags = 0;
  Map_CopyCellAttributeRect(13, 12, 2, 1, 13, 10);
  Map_CopyCellAttributeRect(16, 10, 1, 1, object->x >> 20, 10);
}

s32 Caverns_JumpAcross(struct Actor *object, s16 *position) {
  u8 *flags = &object->motionFlags;
  s32 saved = *flags;
  s32 blocked = MapGrid_CheckCollision(object, position);

  if (blocked == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_SelectAnimation(object, 6);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(object, 7);
    object->speedLimit = px(3);
    object->acceleration = px(2);
    object->velocityY = px(4);
    *flags &= 0x7e;
    Actor_SetSpriteFlags(object, 0);
    {
      struct MainRoutineWork *party = &gMainRoutineWork;
      s32 x = position[1];
      s32 z = position[5];
      s32 owner = party->currentOwner;

      Actor_MoveToAndWaitById(owner, x, z);
    }
    Actor_SelectAnimation(object, 6);
    Actor_SetSpriteFlags(object, 1);
    *flags = saved;
    Event_End();
    return 1;
  }
  return 0;
}

void Caverns_JumpEastAcrossGap(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 x;

  x = leader->x;
  x += 0x200000;
  SET_VECTOR(position, x, leader->y, leader->z);
  Caverns_JumpAcross(leader, (s16 *)position);
}

void Caverns_OpenOseniaPassage(void);

void Caverns_PrepareOsenia(void) {
  s32 result0;

  result0 = Event_GetFlag(FLAG_OSENIA_CAVERN_SCOOPED);
  if (result0 != 0) {
    Caverns_OpenOseniaPassage();
  }
  return;
}

void Caverns_OpenOseniaPassage(void) {
  MapGrid_CopyCellPictureIndicesRect(35, 12, 18, 12, 3, 3);
  MapGrid_CopyCellAttributes(35, 12, 3, 3, 18, 12);
  MapGrid_CopyCellPictureIndicesRect(84, 13, 83, 13, 1, 1);
  return;
}

// Osenia Cavern: the leader steps down into a passage and leaves for its other
// end, by the trigger stepped on.
void Caverns_WalkIntoPassage(void) {
  s32 value = HALF_AT(Ram_HeapSlots->fieldWork, 340);

  if (value == 2)
    Actor_SetPosition(8, 144, 224);
  else
    Actor_SetPosition(8, 224, 224);
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(10);
  BYTE_AT(Actor_GetById(4), 90) &= ~1;
  MapGrid_CopyCellsTo(96, 14, 72, 14, 2, 1);
  Actor_SetDestinationOffsetById(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSpriteFlags(Actor_GetById(4), 0);
  Actor_SetDestinationOffsetById(4, 0, 16);
  Actor_WaitForMoveById(4);
  Actor_SetModeById(4, 13);
  Sound_Play(SE_MAP_EXIT);
  Event_Wait(14);
  Import_EventSetExitRequest(value);
}

void Caverns_WalkEastIntoPassage(void) {
  Actor_WalkByAndWait(4, 0, -14);
  Actor_WalkByAndWaitById(4, 16, 0);
  Event_Wait(10);
  Caverns_WalkIntoPassage();
  return;
}

void Caverns_WalkWestIntoPassage(void) {
  Actor_WalkByAndWait(4, 0, -14);
  Actor_WalkByAndWaitById(4, -16, 0);
  Event_Wait(10);
  Caverns_WalkIntoPassage();
  return;
}

void Caverns_WalkSouthIntoPassage(void) {
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(10);
  Caverns_WalkIntoPassage();
  return;
}

void Caverns_ScoopOseniaPassage(s32 arg0) {
  if (arg0 == 1) {
    Caverns_OpenOseniaPassage();
    GameFlag_Set(FLAG_OSENIA_CAVERN_SCOOPED);
  }
  return;
}

void Caverns_PrepareAtteka(void) {
  s32 result0;

  result0 = Event_GetFlag(FLAG_ATTEKA_CAVERN_PARCHED);
  if (result0 != 0) {
    MapGrid_CopyCellAttributes(31, 43, 9, 12, 48, 47);
    MapGrid_CopyCellsTo(30, 110, 48, 110, 9, 13);
    MapGrid_CopyCellAttributes(52, 110, 1, 1, 52, 111);
  }
  return;
}

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

void Caverns_PrepareAngara(void) {
  if ((u16)(gMainRoutineWork.entrance - 5) <= 1) {
    MapGrid_CopyCellAttributes(28, 0, 1, 1, 28, 5);
    Map_CopyCellAttributeRect(36, 0, 1, 1, 36, 5);
  }
}

void Caverns_InitializeAngara(void) {
  Caverns_PlaceAngaraBlocks(gCaverns_AngaraBlocks);
  Actor_GetById(8)->priorityFlags |= 0x20;
  Actor_GetById(9)->priorityFlags |= 0x20;
  Actor_GetById(10)->collisionFlags |= 8;
  Actor_GetById(11)->collisionFlags |= 8;
}

// Walking into Angara Cavern's door opens it and leaves by exit 8.
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
u32 gFieldEffect_ShortScript[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_MediumScript[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_LongScript[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s16 gCaverns_AngaraBlocks[] = {
    8, 9, -1, 0,
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

s16 gCaverns_OseniaRegions[] = {
    132, 28, 692, 140, 36, 700, -1, 4,
    292, 28, 692, 300, 36, 700, -1, 5,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(Caverns2),
    SCENE_EXIT_UNLESS(1, WorldMap, 82, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Caverns2, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Caverns2, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Caverns2, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Caverns2, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(Caverns1),
    SCENE_EXIT_UNLESS(1, WorldMap, 83, EXIT_FLAG_NONE),
    SCENE_EXITS(Caverns3),
    SCENE_EXIT_UNLESS(5, WorldMap, 88, EXIT_FLAG_NONE),
    SCENE_EXITS(Caverns4),
    SCENE_EXIT_UNLESS(1, WorldMap, 89, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Caverns4, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Caverns4, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Caverns4, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Caverns4, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, Caverns4, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, Caverns4, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, Caverns4, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, Caverns4, 8, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gCaverns_Placements[] = {
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gCaverns_IndraPlacements[] = {
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 168, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 168, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 168, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gCaverns_AngaraPlacements[] = {
    ACTOR_PLACEMENT(315, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 552, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(315, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 536, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 512, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 512, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

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

struct Trigger gCaverns_IndraTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 10, CONDITION_ALWAYS, EVENT(Caverns_JumpEastAcrossGap)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, FLAG_SCENE_TEMP_00, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT(Caverns_MoveIndraPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gCaverns_AttekaTriggers[] = {
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 60, CONDITION_ALWAYS, EVENT(Caverns_ParchAttekaWater)},
    {TRIGGER_TABLE_END},
};

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
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, FLAG_ANGARA_BLOCKS_STACKED, EVENT(Caverns_StartMovingAngaraBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, FLAG_ANGARA_BLOCKS_STACKED, EVENT(Caverns_FinishMovingAngaraBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9, FLAG_ANGARA_BLOCKS_STACKED, EVENT(Caverns_StartMovingAngaraBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, FLAG_ANGARA_BLOCKS_STACKED, EVENT(Caverns_FinishMovingAngaraBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, PSYNERGY_PHASE_BEFORE_CAST), 8, FLAG_ANGARA_BLOCKS_STACKED, EVENT(Caverns_StartMovingAngaraBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), 8, FLAG_ANGARA_BLOCKS_STACKED, EVENT(Caverns_FinishMovingAngaraBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, PSYNERGY_PHASE_BEFORE_CAST), 9, FLAG_ANGARA_BLOCKS_STACKED, EVENT(Caverns_StartMovingAngaraBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), 9, FLAG_ANGARA_BLOCKS_STACKED, EVENT(Caverns_FinishMovingAngaraBlock)},
    {TRIGGER_TABLE_END},
};
