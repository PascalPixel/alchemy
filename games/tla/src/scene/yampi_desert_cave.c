/// @file
/// The Yampi Desert Cave, the hidden dungeon behind the desert's sand slide:
/// Teleport marks lead into its inner rooms, where the party pushes logs,
/// sinks stakes with Pound, bursts pillars, slips under walls with Sand,
/// Scoops up the Venus Djinni Crystal racing through the sand, and meets
/// Valukar, the guardian of fire.

#include "flags.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "field/map_grid.h"
#include "field/actor.h"
#include "macros.h"
#include "field/event_inline.h"
#include "field/field_sequence_event.h"
#include "field/event_commands.h"
#include "scene/scene.h"
#include "scene_ids.h"
#include "field/field.h"
#include "system/main_routine_work.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/task.h"
#include "field/field_event.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "version.h"
#include "shared/tables.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "djinn_ids.h"

/// Set once Burst has broken the pillar in the Water of Life chest's room.
#define FLAG_YAMPI_DESERT_CAVE_CHEST_ROOM_PILLAR_BURST FLAG_AREA_TEMP_00
/// Set once Burst has broken the pillar in Crystal's room.
#define FLAG_YAMPI_DESERT_CAVE_CRYSTAL_ROOM_PILLAR_BURST FLAG_AREA_TEMP_01
/// Set once Scoop has dug up the treasure in Valukar's room.
#define FLAG_YAMPI_DESERT_CAVE_TREASURE_DUG_UP FLAG_AREA_TEMP_02
/// Set as Move leaves each pillar where it opens the way: Valukar's room's,
/// and Crystal's room's west and east pillars.
#define FLAG_YAMPI_DESERT_CAVE_GUARDIAN_PILLAR_MOVED FLAG_AREA_TEMP_03
#define FLAG_YAMPI_DESERT_CAVE_WEST_PILLAR_MOVED FLAG_AREA_TEMP_04
#define FLAG_YAMPI_DESERT_CAVE_EAST_PILLAR_MOVED FLAG_AREA_TEMP_05
/// Set once Pound has sunk the stake in Crystal's room.
#define FLAG_YAMPI_DESERT_CAVE_STAKE_POUNDED FLAG_SCENE_TEMP_06
/// Set while the sand ripple runs, the Djinni Crystal racing under it.
#define FLAG_YAMPI_DESERT_CAVE_RIPPLE_RUNNING FLAG_SCENE_TEMP_70
/// Set once Scoop has dug Crystal out of the sand.
#define FLAG_YAMPI_DESERT_CAVE_CRYSTAL_DUG_UP FLAG_SCENE_TEMP_71
/// The ripple's step count, the byte of flags from FLAG_SCENE_TEMP_78: each
/// thirty-two steps it moves on one cell of gYampiDesertCave_RipplePath.
#define FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP FLAG_SCENE_TEMP_78
/// Set once Valukar, the guardian of fire, has been beaten; he is gone.
#define FLAG_VALUKAR_BEATEN 0x9cd
/// Which of the desert's rooms the party came into the cave from, set by
/// yampi_desert.c, for Retreat to return to it.
#define FLAG_YAMPI_DESERT_ENTERED_BY_GUARDIAN_RING_CHEST 0xa29
#define FLAG_YAMPI_DESERT_ENTERED_BY_TRAINERS_WHIP_CHEST 0xa2a
#define FLAG_YAMPI_DESERT_ENTERED_BY_MEDAL_SPOT 0xa2b

/// The Water of Life chest's room's actors
/// (SceneId_YampiDesertCaveWaterOfLifeChest), the rows of
/// gYampiDesertCave_Placements.
enum YampiDesertCaveChestRoomActorId {
  /// The three fallen columns pushed as logs (gYampiDesertCave_LogSprites).
  ACTOR_CHEST_ROOM_FIRST_LOG = ACTOR_FIRST_PLACED,
  ACTOR_CHEST_ROOM_SECOND_LOG,
  ACTOR_CHEST_ROOM_THIRD_LOG,
  /// The pillar Burst breaks (SPRITE 404).
  ACTOR_CHEST_ROOM_BURST_PILLAR
};

/// Valukar's room's actors (SceneId_YampiDesertCaveGuardian).
enum YampiDesertCaveGuardianActorId {
  /// The stone pillar Move pushes (SPRITE_STONE_PILLAR).
  ACTOR_GUARDIAN_ROOM_PILLAR = ACTOR_FIRST_PLACED,
  /// Valukar, the guardian of fire: "I am the..." (MsgIAmThe).
  ACTOR_VALUKAR
};

/// Crystal's room's actors (SceneId_YampiDesertCaveCrystal).
enum YampiDesertCaveCrystalActorId {
  ACTOR_CRYSTAL_ROOM_FIRST_LOG = ACTOR_FIRST_PLACED,
  ACTOR_CRYSTAL_ROOM_SECOND_LOG,
  ACTOR_CRYSTAL_ROOM_THIRD_LOG,
  ACTOR_CRYSTAL_ROOM_FOURTH_LOG,
  /// The two stone pillars Move pushes, east and west.
  ACTOR_EAST_PILLAR,
  ACTOR_WEST_PILLAR,
  /// The stake Pound sinks.
  ACTOR_STAKE,
  /// The pillar Burst breaks.
  ACTOR_CRYSTAL_ROOM_BURST_PILLAR,
  /// The ripple of sand the Djinni Crystal races under
  /// (YampiDesertCave_WonderAtRipple).
  ACTOR_SAND_RIPPLE,
  /// Crystal, the Venus Djinni, dug out of the sand
  /// (YampiDesertCave_FightCrystal).
  ACTOR_CRYSTAL
};

struct Actor;

extern u16 gYampiDesertCave_LogSprites[];

void YampiDesertCave_StopLeaderAtLog(struct Actor *object);

void YampiDesertCave_ArriveByTeleport(void);

extern u16 gYampiDesertCave_MarkCycle[];

void FieldEffect_StartBlankOverlay(s32 value);

void YampiDesertCave_FightCrystal(void);

void YampiDesertCave_WonderAtRipple(void);

void YampiDesertCave_TryPushLog(void);

void YampiDesertCave_TeleportFromFirstMark(void);

void YampiDesertCave_TeleportFromSecondMark(void);

void YampiDesertCave_CastSand(void);
void YampiDesertCave_EndSand(void);

extern u32 gYampiDesertCave_Settle[];

void Teleport_PlayWhirl(void);

struct Actor *Actor_FindAtCell(s32 *position, s32 mode);
extern u16 *gYampiDesertCave_MarkColors;
extern u16 *gYampiDesertCave_MarkDimColors;
extern u16 *gYampiDesertCave_MarkCycleSlots;
void Actor_DisableById(s32 id);

extern s8 gYampiDesertCave_LogFootprints[];

/// Makes a log's cells solid where it lies: its footprint's cells take the
/// raised layers' attributes and block the ground; returns 0 for an actor that
/// is no log.
s32 YampiDesertCave_SetLogCells(s32 id) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *object = Actor_GetById(id);
  s16 sprite = ((struct ObjectAnimationState *)object->sprite)->value28[0];
  s16 i;
  u32 kind;
  s32 w, h, x, z;
  s32 az;
  s32 bx, bz;

  i = 0;
  if ((u16)sprite != gYampiDesertCave_LogSprites[i]) {
  loop:
    /* FAKEMATCH(temporary): kind set to i, then stepped. */
    kind = (u16)i;
    kind = (u16)(kind + 1);
    i = (s16)kind;
    if (kind <= 5 && (u16)sprite != gYampiDesertCave_LogSprites[kind])
      goto loop;
  }
  kind = (u16)i;
  if (kind == 6)
    return 0;
  w = (ABS(gYampiDesertCave_LogFootprints[kind * 4]) +
       ABS(gYampiDesertCave_LogFootprints[kind * 4 + 2])) >>
      4;
  // i is reused: the footprint's second byte here, layers[1]'s x offset in
  // cells below.
  i = gYampiDesertCave_LogFootprints[kind * 4 + 1];
  h = (ABS(i) + ABS(gYampiDesertCave_LogFootprints[kind * 4 + 3])) >> 4;
  x = (object->x.fixed + (gYampiDesertCave_LogFootprints[kind * 4] << 16)) >>
      20;
  z = (object->z.fixed + (i << 16)) >> 20;
  i = map->layers[1].offsetX >> 20;
  az = map->layers[1].offsetY >> 20;
  bx = map->layers[2].offsetX;
  bz = map->layers[2].offsetY;
  MapGrid_CopyCellAttributes(x, z, w, h, (u16)i + x, (u16)az + z);
  MapGrid_CopyCellAttributes((u16)(bx >> 20) + x, (u16)(bz >> 20) + z, w, h,
                             x + 64, z + 64);
  Map_FillCellRectByte2(0, x, z, w, h, 255);
  Map_FillCellRectByte2(2, x, z, w, h, 255);
  return 1;
}

#include "../field/map_cell_fill.c"

extern s32 gPushLeaderSteps[];

/// Finds the log the leader faces and can push: its kind, its actor slot and
/// the direction, or NULL.
struct Actor *YampiDesertCave_FindPushedLog(s32 *direction, s32 *slot,
                                            s32 *kind);

/// What MapGrid_CheckCollision says of a step to a position: the system call
/// layer's inline, as The Broken Seal's system_call.h has it, which this
/// game's does not yet.
static inline s32 FieldActor_CheckMovementCollision(struct Actor *actor,
                                                    s32 *position) {
  return MapGrid_CheckCollision(actor, position);
}

/// Finds how far a pushed object can slide in the leader's direction, probing
/// its footprint cell by cell until MapGrid_CheckCollision reports a wall.
/// Returns 1 with the end position in position[0] to position[2], or 0 if it
/// cannot move.
s32 YampiDesertCave_FindLogSlide(s32 *position) {
  s32 probe[3];
  s32 direction;
  struct Actor *object;
  s32 kind, w, h, dx, dz, count, i, j;
  u8 *state;
  s32 offset;

  object =
      YampiDesertCave_FindPushedLog(&direction, &position[4], &position[3]);
  if (object != NULL) {
    kind = position[3];
    w = (ABS(gYampiDesertCave_LogFootprints[kind * 4]) +
         ABS(gYampiDesertCave_LogFootprints[kind * 4 + 2])) >>
        4;
    h = (ABS(gYampiDesertCave_LogFootprints[kind * 4 + 1]) +
         ABS(gYampiDesertCave_LogFootprints[kind * 4 + 3])) >>
        4;
    dx = gPushLeaderSteps[direction] & 0xffff0000;
    dz = gPushLeaderSteps[direction] << 16;
    position[5] = 0;
    count = 0;
    state = &object->layer;
    *state = 2;
    position[0] = object->x.fixed + dx;
    position[2] = object->z.fixed + dz;
    probe[1] = object->y.fixed;
    // The row loop is built from labels: as a loop it grows by 14 bytes.
  next:
    offset = kind;
    probe[2] =
        position[2] + (gYampiDesertCave_LogFootprints[offset * 4 + 1] << 16);
    offset *= 4;
    j = 0;
    if (j >= h)
      goto advance;
  row:
    probe[0] = position[0] + (gYampiDesertCave_LogFootprints[offset] << 16);
    for (i = 0; i < w; i++) {
      if (FieldActor_CheckMovementCollision(object, probe) == 2)
        goto hit;
      probe[0] += 0x100000;
    }
    probe[2] += 0x100000;
    j++;
    if (j < h)
      goto row;
  advance:
    position[0] += dx;
    count++;
    position[2] += dz;
    goto next;
  hit:
    *state = 0;
    if (count == 0)
      return 0;
    position[0] = object->x.fixed + dx * count;
    position[1] = object->y.fixed;
    position[2] = object->z.fixed + dz * count;
    return 1;
  } else {
    return 0;
  }
}

#include "text/msg_ids.h"

/// The whole-pixel half of a 16.16 coordinate.
#define PIXEL_PART(v) (((s16 *)&(v))[1])

struct Actor *YampiDesertCave_FindPushedLog(s32 *direction, s32 *slot,
                                            s32 *kind) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor **entry;
  struct Actor *object;
  u32 i, k;
  s32 x, z, left, top, right, bottom;

  *direction = leader->facing >> 12;
  i = 8;
  entry = &work->actors[8];
outer:
  object = *entry;
  k = 0;
inner:
  if (((struct ObjectAnimationState *)object->sprite)->value28[0] ==
      gYampiDesertCave_LogSprites[k]) {
    x = (PIXEL_PART(leader->x.fixed) + (gPushLeaderSteps[*direction] >> 16)) >>
        4;
    z = (PIXEL_PART(leader->z.fixed) + (s16)gPushLeaderSteps[*direction]) >> 4;
    left =
        (PIXEL_PART(object->x.fixed) + gYampiDesertCave_LogFootprints[k * 4]) >>
        4;
    top = (PIXEL_PART(object->z.fixed) +
           gYampiDesertCave_LogFootprints[k * 4 + 1]) >>
          4;
    right = (PIXEL_PART(object->x.fixed) +
             gYampiDesertCave_LogFootprints[k * 4 + 2]) >>
            4;
    bottom = (PIXEL_PART(object->z.fixed) +
              gYampiDesertCave_LogFootprints[k * 4 + 3]) >>
             4;
    if (left <= x && x < right && top <= z && z < bottom) {
      *kind = k;
      if (k & 1) {
        if (left != FIELD_CELL(leader->x.fixed)) {
          *slot = i;
          return object;
        }
      } else {
        if (top != FIELD_CELL(leader->z.fixed)) {
          *slot = i;
          return object;
        }
      }
    }
  }
  k++;
  if (k <= 5)
    goto inner;
  entry++;
  i++;
  if (i <= 63)
    goto outer;
  return NULL;
}

struct YampiDesertCaveRect {
  s32 word[6];
};

extern u16 gYampiDesertCave_MarkBright;
extern u16 gYampiDesertCave_MarkTimer;
extern u16 gYampiDesertCave_MarkPeriod;
extern u16 *gYampiDesertCave_MarkSlots;
extern u16 gYampiDesertCave_MarkCount;
extern u16 gYampiDesertCave_MarkCycleIndex;
extern struct ActorPlacement gYampiDesertCave_Placements[];
extern struct ActorPlacement gYampiDesertCaveGuardianPlacements[];
extern struct ActorPlacement gYampiDesertCaveCrystalPlacements[];
extern u32 gYampiDesertCaveCrystalStakes[];
extern struct Trigger gYampiDesertCave_Triggers[];
extern struct Trigger gYampiDesertCaveGuardianTriggers[];
extern struct Trigger gYampiDesertCaveCrystalTriggers[];

s32 YampiDesertCave_FindLogSlide(s32 *position);
/// The leader pushes a log to where YampiDesertCave_FindLogSlide stopped it,
/// and its cells move with it.
void YampiDesertCave_PushLog(struct YampiDesertCaveRect rect);
void YampiDesertCave_StartMarkGlow(void);
void YampiDesertCave_StopMarkGlow(void);
s32 List_CountEntries(u16 *list);
void PaletteCycle_Fade(void);
void Pound_SinkTarget(u16 *list, s32 mode, s32 count);
void Teleport_PlaySparks(void);

void YampiDesertCave_PushLog(struct YampiDesertCaveRect rect) {
  // Read as s32: the load then keeps its place after the argument stores.
  struct MapWork *map =
      (struct MapWork *)((struct HeapView *)Ram_HeapSlots)->word;
  s32 w, h;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *object = Actor_GetById(rect.word[4]);
  // Always 0: angle is a u16. Direction 0 of the table of steps.
  s32 i = leader->facing >> 16;
  s32 kind = rect.word[3];
  s32 speed = leader->speedLimit;
  s32 acceleration = leader->acceleration;
  s32 cell[3];
  s32 from[3];
  s32 j;
  s32 offset;
  s32 ax, az, bx, bz;

  from[0] = object->x.fixed;
  from[2] = object->z.fixed;
  cell[0] =
      (object->x.fixed + (gYampiDesertCave_LogFootprints[kind * 4] << 16)) >>
      20;
  cell[2] = (object->z.fixed +
             (gYampiDesertCave_LogFootprints[kind * 4 + 1] << 16)) >>
            20;
  w = (ABS(gYampiDesertCave_LogFootprints[kind * 4]) +
       ABS(gYampiDesertCave_LogFootprints[kind * 4 + 2])) >>
      4;
  h = (ABS(gYampiDesertCave_LogFootprints[kind * 4 + 1]) +
       ABS(gYampiDesertCave_LogFootprints[kind * 4 + 3])) >>
      4;
  Map_FillCellRectByte2(0, cell[0], cell[2], w, h, 0);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.5, 0.1);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 8);
  wait(15);
  Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId,
                             (rect.word[0] - from[0]) >> 17,
                             (rect.word[2] - from[2]) >> 17);
  Actor_GetById(gMainRoutineWork.leaderActorId)->update =
      YampiDesertCave_StopLeaderAtLog;
  wait(4);
  Actor_SelectAnimation(object, 2);
  Event_PlaySound(SE_HEAVY_MOVE);
  Actor_SetSpeedAndAcceleration(rect.word[4], 0.5, 0.1);
  Actor_SetMoveTarget(object, rect.word[0], rect.word[1], rect.word[2]);
  /* FAKEMATCH(dead): a loop of one pass with an untaken two-test branch. */
  for (j = 0; j < 1; j++) {
    if (j != 0 && j != 5)
      w = 1;
  }
  Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.3, 0.1);
  offset = gPushLeaderSteps[i] << 16;
  Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, offset >> 31,
                             offset >> 17);
  if (rect.word[5] != 0)
    ((void (*)(void))rect.word[5])();
  Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  leader->update = NULL;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  FieldActor_WaitForMove(object);
  Event_PlaySound(SE_STOP_MUSIC_PLAYER_5);
  Event_PlaySound(SE_HEAVY_STOP);
  Actor_SetPositionAndResetMotion(object, rect.word[0], object->y.fixed,
                                  rect.word[2]);
  Actor_SelectAnimation(object, 1);
  rect.word[0] =
      (rect.word[0] + (gYampiDesertCave_LogFootprints[kind * 4] << 16)) >> 20;
  rect.word[2] =
      (rect.word[2] + (gYampiDesertCave_LogFootprints[kind * 4 + 1] << 16)) >>
      20;
  ax = map->layers[1].offsetX >> 20;
  az = map->layers[1].offsetY >> 20;
  bx = map->layers[2].offsetX >> 20;
  bz = map->layers[2].offsetY >> 20;
  MapGrid_CopyCellAttributes(rect.word[0], rect.word[2], w, h,
                             ax + rect.word[0], az + rect.word[2]);
  MapGrid_CopyCellAttributes(rect.word[0] + bx, rect.word[2] + bz, w, h,
                             rect.word[0] + 64, rect.word[2] + 64);
  Map_FillCellRectByte2(0, rect.word[0], rect.word[2], w, h, 255);
  Map_FillCellRectByte2(2, rect.word[0], rect.word[2], w, h, 255);
  cell[0] = (from[0] + (gYampiDesertCave_LogFootprints[kind * 4] << 16)) >> 20;
  cell[2] =
      (from[2] + (gYampiDesertCave_LogFootprints[kind * 4 + 1] << 16)) >> 20;
  MapGrid_CopyCellAttributes(ax + cell[0], az + cell[2], w, h, cell[0],
                             cell[2]);
  MapGrid_CopyCellAttributes(cell[0] + 64, cell[2] + 64, w, h, cell[0] + bx,
                             cell[2] + bz);
  Event_NoOp();
}

/// The leader's update while pushing: it stops at a wall or another log.
void YampiDesertCave_StopLeaderAtLog(struct Actor *object) {
  s32 position[3];
  struct Actor *hit;
  u32 i;
  s32 angle = object->facing >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = object->x.fixed;
  position[1] = object->y.fixed;
  position[2] = object->z.fixed;
  Vector_AddPolarOffset(0x100000, angle, position);
  hit = Actor_FindAtCell(position, 1);
  if (hit != 0) {
    i = 0;
  next:
    if (*((struct ObjectAnimationState *)hit->sprite)->value28 ==
        gYampiDesertCave_LogSprites[i])
      return;
    i++;
    if (i <= 5)
      goto next;
    Actor_SetPositionAndResetMotion(object, object->x.fixed, object->y.fixed,
                                    object->z.fixed);
  }
  position[0] = object->x.fixed;
  position[1] = object->y.fixed;
  position[2] = object->z.fixed;
  if (MapGrid_CheckCollision(object, position) > 0)
    Actor_SetPositionAndResetMotion(object, object->x.fixed, object->y.fixed,
                                    object->z.fixed);
}

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
/// Sets palette colours from a list and saves both palettes.
void YampiDesertCave_SetMarkColors(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->paletteFade;

  gYampiDesertCave_MarkSlots = slots;
  gYampiDesertCave_MarkColors = colours;
  gYampiDesertCave_MarkDimColors = a;
  gYampiDesertCave_MarkCycleSlots = b;
  if (*slots != 0xffff && *colours != 0xffff) {
  next:
    {
      int red = *colours++;
      int index = *slots++;
      int green = *colours++;
      int blue = *colours++;
      SET_RGB(((u16 *)BG_PLTT)[index], red, green, blue);
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)BG_PLTT, saved, DMA_COPY_WORDS(112), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, saved + 448, DMA_COPY_WORDS(112), REG_DMA3);
  PaletteFade_SetTarget(0x10000, 0);
  if (gMainRoutineWork.teleportState == 2)
    YampiDesertCave_ArriveByTeleport();
}

/// PaletteCycle_Start with a VBlank task.
void YampiDesertCave_StartMarkGlow(void) {
  gYampiDesertCave_MarkCycleIndex = 0;
  gYampiDesertCave_MarkCount = List_CountEntries(gYampiDesertCave_MarkSlots);
  gYampiDesertCave_MarkPeriod = 15;
  gYampiDesertCave_MarkTimer = 16;
  gYampiDesertCave_MarkBright = 1;
  Task_Add(PaletteCycle_Fade, TASK_PASS_VBLANK);
}

void YampiDesertCave_StopMarkGlow(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Task_Remove(PaletteCycle_Fade);
}

void YampiDesertCave_StartMarkFlash(void) {
  gYampiDesertCave_MarkCycleIndex = 0;
  gYampiDesertCave_MarkCount = List_CountEntries(gYampiDesertCave_MarkSlots);
  gYampiDesertCave_MarkPeriod = 2;
  gYampiDesertCave_MarkTimer = 2;
  gYampiDesertCave_MarkBright = 1;
  Task_Add(PaletteCycle_Fade, TASK_PASS_VBLANK);
}

#include "shared/list_count_entries.h"

void PaletteCycle_Fade(void) {
  s32 step = 0;
  u16 *color;
  volatile u16 *dst;
  u32 i;
  s32 r, g, b;

  if (gYampiDesertCave_MarkTimer != 0 && --gYampiDesertCave_MarkTimer == 0) {
    step = 1;
    gYampiDesertCave_MarkTimer = gYampiDesertCave_MarkPeriod;
    if (gYampiDesertCave_MarkPeriod > 2)
      gYampiDesertCave_MarkPeriod--;
  }
  if (!step)
    return;
  color = gYampiDesertCave_MarkDimColors;
  for (i = 0; i < gYampiDesertCave_MarkCount; i++) {
    dst = &BG_PLTT
              [gYampiDesertCave_MarkCycleSlots[gYampiDesertCave_MarkCycleIndex *
                                                   gYampiDesertCave_MarkCount +
                                               i]];
    r = *color++;
    g = *color++;
    b = *color++;
    SET_RGB(*dst, r, g, b);
  }
  dst = &BG_PLTT[gYampiDesertCave_MarkSlots[gYampiDesertCave_MarkCount]];
  if (gYampiDesertCave_MarkBright)
    color = gYampiDesertCave_MarkColors + gYampiDesertCave_MarkCount * 3;
  else
    color = gYampiDesertCave_MarkDimColors + gYampiDesertCave_MarkCount * 3;
  r = *color++;
  g = *color++;
  b = *color++;
  SET_RGB(*dst, r, g, b);
  gYampiDesertCave_MarkBright ^= 1;
  if (++gYampiDesertCave_MarkCycleIndex == gYampiDesertCave_MarkCount)
    gYampiDesertCave_MarkCycleIndex = 0;
}

/// Arriving by Teleport: the screen flashes as the marks' colours race, then
/// fades back.
void YampiDesertCave_ArriveByTeleport(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 saved;

  saved = work->transition;
  PaletteFade_SetTarget(0x202108, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  YampiDesertCave_StartMarkFlash();
  Teleport_PlayWhirl();
  wait(40);
  YampiDesertCave_StopMarkGlow();
  PaletteFade_SetTarget(0x10000, 1);
  PaletteFade_StartInterpolation(16);
  wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->teleportState = 0;
  }
  work->transition = saved;
}

void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

/// Walks a list of three-halfword entries (object id, column, row) ended by
/// 0xffff, clearing each object's collision bit and child flags before passing
/// the entry to Actor_SetCellOpen. A struct of three halfwords would
/// be padded to 8 bytes, so the list is read as u16 triples.
void YampiDesertCave_PlaceStakes(const u16 *list) {
  const u16 *entry;

  if (list[0] == 0xffff)
    return;
  entry = list;
loop:
  {
    u32 id = entry[0];
    struct Actor *object = Actor_GetById(id);
    s16 column = entry[1];
    s16 row = entry[2];

    object->collisionFlags &= ~1;
    Actor_SetSpriteFlags(object, 0);
    Actor_SetCellOpen(id, (u16)column, (u16)row, object->layer);
    entry += 3;
  }
  if (entry[0] != 0xffff)
    goto loop;
}

// Finds the current target's table row and shrinks it.
#include "shared/pound_sink_target.h"

s32 Actor_GetCellIndex(struct Actor *object);

#include "shared/actor_set_cell_open.h"

/// Talking to Crystal, the Venus Djinni Scoop dug up, starts its battle
/// (flag 0x271).
void YampiDesertCave_FightCrystal(void) {
  Event_SetFlag(FLAG_YAMPI_DESERT_CAVE_CRYSTAL_DUG_UP);
  Event_RequestDjinnBattle(17, 17);
  return;
}

void Event_End();

/// Talking to the ripple racing through the sand only puzzles the leader.
void YampiDesertCave_WonderAtRipple(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldActor_ShowEmote(gMainRoutineWork.leaderActorId,
                       EMOTE_IN_FRONT | EMOTE_QUESTION, 32);
  Event_End();
  return;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_GUARDIAN)
    return gYampiDesertCaveGuardianPlacements;
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL)
    return gYampiDesertCaveCrystalPlacements;
  return gYampiDesertCave_Placements;
}

void YampiDesertCave_TryPushLog(void) {
  struct YampiDesertCaveRect rect;

  if (YampiDesertCave_FindLogSlide(&rect))
    YampiDesertCave_PushLog(rect);
}

/// Teleport on one of the first room's marks: the marks glow and the party
/// arrives in the inner rooms by entrance n1.
#define DEFINE_TELEPORT_FROM_MARK(name, n1)                                    \
  void name(void) {                                                            \
    YampiDesertCave_StartMarkGlow();                                           \
    Teleport_PlaySparks();                                                     \
    gMainRoutineWork.teleportState = 2;                                        \
    Event_SetDestination(SCENE_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST, n1);     \
    Event_Wait(40);                                                            \
    YampiDesertCave_StopMarkGlow();                                            \
  }

DEFINE_TELEPORT_FROM_MARK(YampiDesertCave_TeleportFromFirstMark, 10)

DEFINE_TELEPORT_FROM_MARK(YampiDesertCave_TeleportFromSecondMark, 9)

/// Burst breaks a pillar for good (flag) as it acts (phase 3).
#define DEFINE_BURST_PILLAR(name, flag, actorId)                               \
  void name(s32 phase) {                                                       \
    s32 burst;                                                                 \
                                                                               \
    burst = Event_GetFlag(flag);                                               \
    if (burst != 0)                                                            \
      goto done;                                                               \
    if (phase != 3)                                                            \
      goto done;                                                               \
    Actor_SetPosition(actorId, 0, 0);                                          \
    GameFlag_Set(flag);                                                        \
  done:                                                                        \
    return;                                                                    \
  }

DEFINE_BURST_PILLAR(YampiDesertCave_BurstChestRoomPillar,
                    FLAG_YAMPI_DESERT_CAVE_CHEST_ROOM_PILLAR_BURST,
                    ACTOR_CHEST_ROOM_BURST_PILLAR)

DEFINE_BURST_PILLAR(YampiDesertCave_BurstCrystalRoomPillar,
                    FLAG_YAMPI_DESERT_CAVE_CRYSTAL_ROOM_PILLAR_BURST,
                    ACTOR_CRYSTAL_ROOM_BURST_PILLAR)

void YampiDesertCave_StartMovingPillar(s32 unused, s32 id) {
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_GUARDIAN) {
    s32 row = 12;

    MapGrid_CopyCellAttributes(98, 3, 3, 3, 97, row);
    MapGrid_CopyCellAttributes(33, 12, 1, 1, 34, row);
  }
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL) {
    if (id == ACTOR_EAST_PILLAR) {
      MapGrid_CopyCellAttributes(102, 15, 1, 1, 103, 15);
      MapGrid_CopyCellAttributes(43, 14, 1, 1, 39, 16);
    }
    if (id == ACTOR_WEST_PILLAR) {
      struct Actor *object = Actor_GetById(ACTOR_WEST_PILLAR);

      s32 column = FIELD_CELL(object->x.fixed);
      s32 row = FIELD_CELL(object->z.fixed);

      MapGrid_CopyCellAttributes(7, 10, 1, 1, column, row);
    }
  }
}

/// Rooms 2 and 3: by where object id stands, sets flags 0x303 to 0x305 and
/// makes the rooms' Map_CopyCellAttributeRect calls.
void YampiDesertCave_FinishMovingPillar(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_GUARDIAN) {
    s32 row = FIELD_CELL(object->z.fixed);

    if (row == 13) {
      object->motionFlags = 0;
      Event_SetFlag(FLAG_YAMPI_DESERT_CAVE_GUARDIAN_PILLAR_MOVED);
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, row);
      MapGrid_CopyCellAttributes(95, 3, 3, 3, 97, 12);
    } else {
      row = 12;
      MapGrid_CopyCellAttributes(92, 3, 3, 3, 97, row);
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, row);
    }
  }
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL) {
    if (id == ACTOR_EAST_PILLAR) {
      s32 column;

      MapGrid_CopyCellAttributes(103, 16, 1, 1, 103, 15);
      column = FIELD_CELL(object->x.fixed);
      if (column == 38) {
        Event_SetFlag(FLAG_YAMPI_DESERT_CAVE_EAST_PILLAR_MOVED);
        MapGrid_CopyCellAttributes(38, 15, 1, 1, column, 16);
      } else {
        MapGrid_CopyCellAttributes(38, 15, 1, 1, 39, 16);
      }
    }
    if (id == ACTOR_WEST_PILLAR) {
      if (FIELD_CELL(object->x.fixed) == 10 &&
          FIELD_CELL(object->z.fixed) == 11)
        Event_SetFlag(FLAG_YAMPI_DESERT_CAVE_WEST_PILLAR_MOVED);
      MapGrid_CopyCellAttributes(12, 11, 1, 1, FIELD_CELL(object->x.fixed),
                                 FIELD_CELL(object->z.fixed));
    }
  }
}

/// YampiDesertCave_EndSand's counterpart: the leader's terrain becomes 2, and
/// in rooms 1 and 3 the same calls run with their first and last pairs swapped,
/// each followed by more.
/// Sand: the leader sinks into the sand and the rooms' sand-only cells open.
void YampiDesertCave_CastSand(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->layer = 2;
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST) {
    MapGrid_CopyCellAttributes(55, 71, 4, 1, 43, 70);
    MapGrid_CopyCellAttributes(43, 71, 4, 1, 55, 71);
  }
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL) {
    MapGrid_CopyCellAttributes(110, 45, 5, 2, 90, 50);
    MapGrid_CopyCellAttributes(113, 47, 2, 1, 90, 52);
    MapGrid_CopyCellAttributes(107, 48, 3, 2, 90, 53);
    MapGrid_CopyCellAttributes(97, 58, 5, 2, 110, 45);
    MapGrid_CopyCellAttributes(97, 58, 2, 1, 113, 47);
    MapGrid_CopyCellAttributes(97, 58, 3, 2, 107, 48);
  }
}

/// Sand fades: the leader rises and the cells close again.
void YampiDesertCave_EndSand(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->layer = 0;
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST)
    MapGrid_CopyCellAttributes(43, 70, 4, 1, 55, 71);
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL) {
    MapGrid_CopyCellAttributes(90, 50, 5, 2, 110, 45);
    MapGrid_CopyCellAttributes(90, 52, 2, 1, 113, 47);
    MapGrid_CopyCellAttributes(90, 53, 3, 2, 107, 48);
  }
}

/// Pound sinks Room3's stake and opens its cell.
void YampiDesertCave_PoundStake(s32 a, s32 b) {
  Pound_SinkTarget(gYampiDesertCaveCrystalStakes, a, b);
  MapGrid_CopyCellAttributes(47, 12, 1, 1, 46, 12);
}

extern s16 gYampiDesertCave_RipplePath[];

static __inline__ s32 GameFlag_GetByte_(s32 flag) {
  return ((s32)GameFlag_GetByte(flag));
}

/// Moves the ripple to the next cell of its path, step by step on the step
/// counter in flag byte 0x278.
void YampiDesertCave_MoveSandRipple(void) {
  s32 step = GameFlag_GetByte_(FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP);
  s32 index = (step / 32) * 2;
  s32 column = gYampiDesertCave_RipplePath[index];
  s32 row = gYampiDesertCave_RipplePath[index + 1];

  if (Actor_GetById(ACTOR_SAND_RIPPLE)->paused == 0) {
    Actor_SetPositionFixed(ACTOR_SAND_RIPPLE, column << 19, row << 19);
    if (Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS) == 0)
      step++;
    GameFlag_SetByte(FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP, step);
  }
}

/// The chest that follows the step count: when the tracked object stands on
/// its cell, mode 1 stops the follower and mode 2 drops the chest.
/// Scoop on the ripple's cell stops it (mode 1) and digs up Crystal, who
/// drops to the ground (mode 2).
void YampiDesertCave_ScoopDjinni(s32 mode) {
  s32 index =
      (((s32)GameFlag_GetByte(FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP)) / 32) *
      2;
  s16 column = gYampiDesertCave_RipplePath[index];
  s32 px = column * 8;
  s16 row = gYampiDesertCave_RipplePath[index + 1];
  s32 pz = row * 8;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *tracked =
      *(struct Actor **)(Ram_HeapSlots->battleEffect + 0x1930);
  s32 x = 0, z = 0;

  if (tracked != NULL) {
    x = FIELD_CELL(tracked->x.fixed);
    z = FIELD_CELL(tracked->z.fixed);
  }
  if (px / 16 != x || pz / 16 != z)
    return;
  if (mode == 1) {
    Task_Remove(YampiDesertCave_MoveSandRipple);
    Actor_SetPosition(ACTOR_SAND_RIPPLE, 0, 0);
  }
  if (mode == 2) {
    struct Actor *crystal = Actor_GetById(ACTOR_CRYSTAL);

    Actor_SetPositionFixed(ACTOR_CRYSTAL, column << 19, row << 19);
    crystal->y.fixed = px(32);
    crystal->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    crystal->gravity = 0x170a;
    wait(50);
    {
      s16 *counter = &work->talk;
      s32 value = ACTOR_CRYSTAL;

      *counter = value;
    }
#if TLA_PATCH_1
    Event_ClearFlag(FLAG_YAMPI_DESERT_CAVE_RIPPLE_RUNNING);
#endif
  }
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

/// Valukar, the guardian, speaks, and its battle begins; afterwards the party
/// leaves for overlay 664's fourth room.
void YampiDesertCave_MeetGuardianOfFire(void) {
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(ACTOR_VALUKAR, 2);
  Event_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_SetTarget(0x10003, 1);
  PaletteFade_StartInterpolation(60);
  Event_Wait(60);
  EventMessage_ShowCentered((s32)MsgIAmThe, 0, 6);
  PaletteFade_SetTarget(0x10000, 0);
  PaletteFade_StartInterpolation(60);
  Event_Wait(60);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(ACTOR_VALUKAR, leader->x.part[0].pixel,
                         leader->z.part[0].pixel);
  Event_Wait(20);
  gMainRoutineWork.pendingDjinni = IF_FLAG_SET(FLAG_VALUKAR_BEATEN);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  // XXX: the battle backdrop is resource 88, the lava cave's
  // (Battle_LavaCaveBackdrop), a link-time number;
  // SceneId_AirsRockInsideTightropes is 88 too and stands in until
  // resource_directory.s names the backdrop.
  gMainRoutineWork.battleBackground = SCENE_AIRS_ROCK_INSIDE_TIGHTROPES;
  Field_RequestBattleFromEncounterTable(103, 1);
}

void YampiDesertCave_ScoopTreasure(s32 a) {
  Event_DigUpTreasure(a, 64, FLAG_YAMPI_DESERT_CAVE_TREASURE_DUG_UP);
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_GUARDIAN)
    return gYampiDesertCaveGuardianTriggers;
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL)
    return gYampiDesertCaveCrystalTriggers;
  return gYampiDesertCave_Triggers;
}

// Sets up Yampi Desert Cave's three rooms as each one loads: the first
// room's palette cycle and Retreat destination, the third room's background
// priorities, and in each room the objects its flags place.

extern s16 gYampiDesertCave_MarkPaletteSlots[];
extern u16 gYampiDesertCave_MarkColorsLit[];
extern u16 gYampiDesertCave_MarkColorsDim[];

void Actor_SetWideSprite(s32 id, s32 wide);

/// Sets up Yampi Desert Cave's three rooms as each one loads: the first room's
/// palette cycle and Retreat destination, the third room's background
/// priorities, and in each room the objects its flags place.
s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST) {
    YampiDesertCave_SetMarkColors(
        gYampiDesertCave_MarkPaletteSlots, gYampiDesertCave_MarkColorsLit,
        gYampiDesertCave_MarkColorsDim, gYampiDesertCave_MarkCycle);
    Actor_DisableById(ACTOR_CHEST_ROOM_FIRST_LOG);
    Actor_DisableById(ACTOR_CHEST_ROOM_SECOND_LOG);
    Actor_DisableById(ACTOR_CHEST_ROOM_THIRD_LOG);
    YampiDesertCave_SetLogCells(ACTOR_CHEST_ROOM_FIRST_LOG);
    YampiDesertCave_SetLogCells(ACTOR_CHEST_ROOM_SECOND_LOG);
    YampiDesertCave_SetLogCells(ACTOR_CHEST_ROOM_THIRD_LOG);
    if (Event_GetFlag(FLAG_YAMPI_DESERT_CAVE_CHEST_ROOM_PILLAR_BURST)) {
      Actor_SetPosition(ACTOR_CHEST_ROOM_BURST_PILLAR, 0, 0);
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, 13);
    }
    if (gMainRoutineWork.entrance == 1) {
      if (Event_GetFlag(FLAG_YAMPI_DESERT_ENTERED_BY_GUARDIAN_RING_CHEST)) {
        gMainRoutineWork.retreatScene = SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST;
        gMainRoutineWork.retreatEntrance = 1;
      } else if (Event_GetFlag(
                     FLAG_YAMPI_DESERT_ENTERED_BY_TRAINERS_WHIP_CHEST)) {
        gMainRoutineWork.retreatScene = SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST;
        gMainRoutineWork.retreatEntrance = 2;
      } else if (Event_GetFlag(FLAG_YAMPI_DESERT_ENTERED_BY_MEDAL_SPOT)) {
        gMainRoutineWork.retreatScene = SCENE_YAMPI_DESERT_MEDAL_SPOT;
        gMainRoutineWork.retreatEntrance = 2;
      }
    }
  }
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_GUARDIAN) {
    if (Event_GetFlag(FLAG_YAMPI_DESERT_CAVE_GUARDIAN_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_GUARDIAN_ROOM_PILLAR, 552, 216);
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, 13);
      MapGrid_CopyCellAttributes(95, 3, 3, 3, 98, 13);
    } else {
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, 12);
    }
    if (!Event_GetFlag(FLAG_YAMPI_DESERT_CAVE_TREASURE_DUG_UP))
      Actor_SetWideSprite(64, 1);
  }
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL) {
    if ((u16)(gMainRoutineWork.entrance - 3) <= 2) {
      REG_BG1CNT &= ~3;
      REG_BG1CNT |= 1;
      REG_BG2CNT &= ~3;
      REG_BG2CNT |= 3;
      REG_BG3CNT &= ~3;
      REG_BG3CNT |= 3;
    }
    Actor_DisableById(ACTOR_CRYSTAL_ROOM_FIRST_LOG);
    Actor_DisableById(ACTOR_CRYSTAL_ROOM_SECOND_LOG);
    Actor_DisableById(ACTOR_CRYSTAL_ROOM_THIRD_LOG);
    Actor_DisableById(ACTOR_CRYSTAL_ROOM_FOURTH_LOG);
    YampiDesertCave_SetLogCells(ACTOR_CRYSTAL_ROOM_FIRST_LOG);
    YampiDesertCave_SetLogCells(ACTOR_CRYSTAL_ROOM_SECOND_LOG);
    YampiDesertCave_SetLogCells(ACTOR_CRYSTAL_ROOM_THIRD_LOG);
    YampiDesertCave_SetLogCells(ACTOR_CRYSTAL_ROOM_FOURTH_LOG);
    YampiDesertCave_PlaceStakes(gYampiDesertCaveCrystalStakes);
    if (!Event_GetFlag(FLAG_YAMPI_DESERT_CAVE_STAKE_POUNDED))
      MapGrid_CopyCellAttributes(48, 12, 1, 1, 46, 12);
    if (Event_GetFlag(FLAG_YAMPI_DESERT_CAVE_CRYSTAL_ROOM_PILLAR_BURST))
      Actor_SetPosition(ACTOR_CRYSTAL_ROOM_BURST_PILLAR, 0, 0);
    if (Event_GetFlag(FLAG_YAMPI_DESERT_CAVE_WEST_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_WEST_PILLAR, 168, 184);
      MapGrid_CopyCellAttributes(12, 11, 1, 1, 10, 11);
    } else {
#if TLA_PATCH_1
      struct Actor *actor = Actor_GetById(ACTOR_WEST_PILLAR);

      MapGrid_CopyCellAttributes(12, 11, 1, 1, FIELD_CELL(actor->x.fixed),
                                 FIELD_CELL(actor->z.fixed));
#else
      MapGrid_CopyCellAttributes(12, 11, 1, 1, 8, 10);
#endif
    }
    if (Event_GetFlag(FLAG_YAMPI_DESERT_CAVE_EAST_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_EAST_PILLAR, 616, 264);
      MapGrid_CopyCellAttributes(38, 15, 1, 1, 38, 16);
    } else {
      MapGrid_CopyCellAttributes(38, 15, 1, 1, 39, 16);
    }
    if (!Event_GetFlag(FLAG_DJINNI(DJINNI_CRYSTAL)) &&
        !Event_GetFlag(FLAG_DJINNI_HIDING(DJINNI_CRYSTAL))) {
      s32 step = FieldGameFlag_GetByte(FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP);
      s32 placed = Event_GetFlag(FLAG_YAMPI_DESERT_CAVE_CRYSTAL_DUG_UP);

      if (placed) {
        step = step / 32 * 2;
        Actor_SetPosition(ACTOR_SAND_RIPPLE, 0, 0);
        Actor_SetPositionFixed(ACTOR_CRYSTAL,
                               gYampiDesertCave_RipplePath[step] << 19,
                               gYampiDesertCave_RipplePath[step + 1] << 19);
      } else {
        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
          GameFlag_SetByte(FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP, 0);
        Task_Add(YampiDesertCave_MoveSandRipple, TASK_PASS_VBLANK);
        Event_SetFlag(FLAG_YAMPI_DESERT_CAVE_RIPPLE_RUNNING);
        Actor_SetSpritePriority(ACTOR_SAND_RIPPLE, 3);
        Actor_GetById(ACTOR_SAND_RIPPLE)->collisionFlags = 0;
      }
    }
  }
  FieldEffect_StartBlankOverlay(0);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#undef PIXEL_PART
#undef DEFINE_TELEPORT_FROM_MARK
#undef DEFINE_BURST_PILLAR
#undef FIELD_AT

void YampiDesertCave_MeetGuardianOfFire(void);

// clang-format off
u16 gYampiDesertCave_LogSprites[] = {
    258, 259, 293, 294, 332, 331,
};

s8 gYampiDesertCave_LogFootprints[] = {
    -32, -8, 32, 8, -8, -32, 8, 32,
    -32, -16, 32, 0, -8, -32, 8, 32,
    -32, -8, 32, 8, -8, -32, 8, 32,
};

s32 gPushLeaderSteps[] = {                                                               
      PUSH_STEP(16, 0),  PUSH_STEP(16, 0),  PUSH_STEP(16, 0),                  
      PUSH_STEP(0, 16),  PUSH_STEP(0, 16),  PUSH_STEP(0, 16),                  
      PUSH_STEP(0, 16),  PUSH_STEP(-16, 0), PUSH_STEP(-16, 0),                 
      PUSH_STEP(-16, 0), PUSH_STEP(-16, 0), PUSH_STEP(0, -16),                 
      PUSH_STEP(0, -16), PUSH_STEP(0, -16), PUSH_STEP(0, -16),                 
      PUSH_STEP(16, 0),                                                        
  };

s16 gYampiDesertCave_RipplePath[] = {
    101, 39, 0, 0, 97, 35, 0, 0,
    99, 37, 0, 0, 95, 33, 0, 0,
};

s16 gYampiDesertCave_MarkPaletteSlots[] = {
    9, 10, 11, 12, 13, 14, 15, -1,
};

u16 gYampiDesertCave_MarkColorsLit[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

u16 gYampiDesertCave_MarkColorsDim[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

u16 gYampiDesertCave_MarkCycle[] = {                                                               
      14, 9, 10, 11, 12, 13, 13, 14, 9, 10, 11, 12, 12, 13, 14, 9, 10, 11,  
      11, 12, 13, 14, 9, 10, 10, 11, 12, 13, 14, 9, 9, 10, 11, 12, 13, 14,  
  };

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(YampiDesertCaveWaterOfLifeChest),
    SCENE_EXIT(1, YampiDesertTrainersWhipChest, 4),
    SCENE_EXIT(2, YampiDesertCaveCrystal, 1),
    SCENE_EXIT(3, YampiDesertCaveWaterOfLifeChest, 6),
    SCENE_EXIT(4, YampiDesertCaveWaterOfLifeChest, 5),
    SCENE_EXIT(5, YampiDesertCaveWaterOfLifeChest, 4),
    SCENE_EXIT(6, YampiDesertCaveWaterOfLifeChest, 3),
    SCENE_EXIT(7, YampiDesertCaveCrystal, 8),
    SCENE_EXIT(8, YampiDesertCaveGuardian, 1),
    SCENE_EXITS(YampiDesertCaveGuardian),
    SCENE_EXIT(1, YampiDesertCaveWaterOfLifeChest, 8),
    SCENE_EXIT(2, YampiDesertCaveGuardian, 17),
    SCENE_EXIT(14, YampiDesertCaveCrystal, 13),
    SCENE_EXIT(15, YampiDesertCaveCrystal, 12),
    SCENE_EXIT(16, YampiDesertCaveCrystal, 11),
    SCENE_EXIT(17, YampiDesertCaveGuardian, 2),
    SCENE_EXITS(YampiDesertCaveCrystal),
    SCENE_EXIT(1, YampiDesertCaveWaterOfLifeChest, 2),
    SCENE_EXIT(2, YampiDesertCaveCrystal, 3),
    SCENE_EXIT(3, YampiDesertCaveCrystal, 2),
    SCENE_EXIT(4, YampiDesertCaveCrystal, 7),
    SCENE_EXIT(5, YampiDesertCaveCrystal, 6),
    SCENE_EXIT(6, YampiDesertCaveCrystal, 5),
    SCENE_EXIT(7, YampiDesertCaveCrystal, 4),
    SCENE_EXIT(8, YampiDesertCaveWaterOfLifeChest, 7),
    SCENE_EXIT(9, YampiDesertCaveCrystal, 10),
    SCENE_EXIT(10, YampiDesertCaveCrystal, 9),
    SCENE_EXIT(11, YampiDesertCaveGuardian, 16),
    SCENE_EXIT(12, YampiDesertCaveGuardian, 15),
    SCENE_EXIT(13, YampiDesertCaveGuardian, 14),
    SCENE_EXITS_END,
};

u32 gYampiDesertCave_Settle[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

struct ActorPlacement gYampiDesertCave_Placements[] = {
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 888, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 832, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 912, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(404, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct ActorPlacement gYampiDesertCaveGuardianPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS,
                    (s32)gYampiDesertCave_Settle, 552, 0, 200, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(177, FLAG_VALUKAR_BEATEN, MOVEMENT_TYPE_STAND, 200, 0, 152,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gYampiDesertCaveCrystalPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 712, 0, 752, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 752, 0, 744, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 728, 0, 784, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 800, 0, 760, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS,
                    (s32)gYampiDesertCave_Settle, 632, 0, 264, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS,
                    (s32)gYampiDesertCave_Settle, 136, 0, 168, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, (s32)gYampiDesertCave_Settle, 744, 0,
                    200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(404, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 632,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(251, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_CRYSTAL),
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

u32 gYampiDesertCaveCrystalStakes[] = {
    14,
    0xffff0206,
};

struct Trigger gYampiDesertCave_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {STEP_TRIGGER_WALKING, 42, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_TryPushLog)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 33,
     FLAG_YAMPI_DESERT_CAVE_CHEST_ROOM_PILLAR_BURST,
     EVENT(YampiDesertCave_BurstChestRoomPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 30,
     CONDITION_ALWAYS, EVENT(YampiDesertCave_TeleportFromFirstMark)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 31,
     CONDITION_ALWAYS, EVENT(YampiDesertCave_TeleportFromSecondMark)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(YampiDesertCave_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_EndSand)},
    {TRIGGER_TABLE_END},
};

struct Trigger gYampiDesertCaveGuardianTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_STEP, 52, FLAG_VALUKAR_BEATEN,
     EVENT(YampiDesertCave_MeetGuardianOfFire)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     FLAG_YAMPI_DESERT_CAVE_GUARDIAN_PILLAR_MOVED,
     EVENT(YampiDesertCave_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GUARDIAN_ROOM_PILLAR,
     FLAG_YAMPI_DESERT_CAVE_GUARDIAN_PILLAR_MOVED,
     EVENT(YampiDesertCave_FinishMovingPillar)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(YampiDesertCave_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_EndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 100,
     FLAG_YAMPI_DESERT_CAVE_TREASURE_DUG_UP,
     EVENT(YampiDesertCave_ScoopTreasure)},
    {TRIGGER_TABLE_END},
};

struct Trigger gYampiDesertCaveCrystalTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_TALK, ACTOR_SAND_RIPPLE, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_WonderAtRipple)},
    {TRIGGER_TALK, ACTOR_CRYSTAL, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_FightCrystal)},
    {STEP_TRIGGER_WALKING, 42, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_TryPushLog)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 33,
     FLAG_YAMPI_DESERT_CAVE_CRYSTAL_ROOM_PILLAR_BURST,
     EVENT(YampiDesertCave_BurstCrystalRoomPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12,
     FLAG_YAMPI_DESERT_CAVE_EAST_PILLAR_MOVED,
     EVENT(YampiDesertCave_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR,
     FLAG_YAMPI_DESERT_CAVE_EAST_PILLAR_MOVED,
     EVENT(YampiDesertCave_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13,
     FLAG_YAMPI_DESERT_CAVE_WEST_PILLAR_MOVED,
     EVENT(YampiDesertCave_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_PILLAR,
     FLAG_YAMPI_DESERT_CAVE_WEST_PILLAR_MOVED,
     EVENT(YampiDesertCave_FinishMovingPillar)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_StartMovingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_STAKE,
     FLAG_YAMPI_DESERT_CAVE_STAKE_POUNDED, EVENT(YampiDesertCave_PoundStake)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(YampiDesertCave_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_EndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 33,
     IF_FLAG_SET(0x270), EVENT(YampiDesertCave_ScoopDjinni)},
    {TRIGGER_TABLE_END},
};
