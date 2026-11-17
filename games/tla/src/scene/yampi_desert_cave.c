/// @file
/// The Yampi Desert Cave, the hidden dungeon behind the desert's sand slide:
/// Teleport marks lead into its inner rooms, where the party pushes logs,
/// sinks stakes with Pound, bursts pillars, slips under walls with Sand,
/// Scoops up the Venus Djinni Crystal racing through the sand, and meets
/// Valukar, the guardian of fire.
#include "djinn_ids.h"
#include "fixed_math.h"
#include "flags.h"
#include "macros.h"
#include "scene_ids.h"
#include "sprite_ids.h"
#include "types.h"
#include "version.h"
#include "battle/battle.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_event.h"
#include "field/field_sequence_event.h"
#include "field/map_grid.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/palette_buffers.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/scene.h"
#include "sound/sound.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/main_routine_work.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "system/task.h"
#include "text/msg_ids.h"

s32 MapGrid_CheckCollision(struct Actor *actor, struct FieldPosition *position);

#include "shared/tables.h"

/// The background colours at the start of the palette fade's source.
#define PALETTE_FADE_BG_COLORS (14 * PALETTE_COLORS)

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
/// RIPPLE_STEPS_PER_POINT steps it moves on one point of
/// gYampiDesertCave_RipplePath.
#define FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP FLAG_SCENE_TEMP_78
/// The steps the ripple takes at each point of its path.
#define RIPPLE_STEPS_PER_POINT 32

/// The 16.16 coordinate of an 8-pixel half cell's corner: the ripple's path
/// is counted in half cells.
#define FIELD_HALF_CELL_ORIGIN(halfCell) ((halfCell) << 19)

/// The lava cave's battle backdrop, Valukar's battle's.
extern char ResourceId_LavaCaveBackdrop;
/// Set once Valukar, the guardian of fire, has been beaten; he is gone.
#define FLAG_VALUKAR_BEATEN 0x9cd

/// The Water of Life chest's room's actors
/// (SceneId_YampiDesertCaveWaterOfLifeChest), the rows of
/// gYampiDesertCave_Placements.
enum YampiDesertCaveChestRoomActorId {
  /// The three fallen columns pushed as logs (gYampiDesertCave_LogSprites).
  ACTOR_CHEST_ROOM_FIRST_LOG = ACTOR_FIRST_PLACED,
  ACTOR_CHEST_ROOM_SECOND_LOG,
  ACTOR_CHEST_ROOM_THIRD_LOG,
  /// The pillar Burst breaks (SPRITE_BURST_PILLAR).
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

/// Valukar's room's hidden treasure, the first actor of its item table, as
/// the desert's.
enum { ACTOR_HIDDEN_TREASURE = 64 };

struct Actor;

/// The kinds of log, the rows of gYampiDesertCave_LogSprites and
/// gYampiDesertCave_LogFootprints.
#define LOG_KIND_COUNT 6

extern u16 gYampiDesertCave_LogSprites[];

void YampiDesertCave_StopLeaderAtLog(struct Actor *actor);

void YampiDesertCave_ArriveByTeleport(void);

extern u16 gYampiDesertCave_MarkCycle[];

void FieldEffect_StartBlankOverlay(s32 value);

void YampiDesertCave_FightCrystal(void);

void YampiDesertCave_WonderAtRipple(void);

/// The leader pushes the log ahead, if it can slide.
void YampiDesertCave_TryPushLog(void);

void YampiDesertCave_TeleportFromFirstMark(void);

void YampiDesertCave_TeleportFromSecondMark(void);

void YampiDesertCave_CastSand(void);
void YampiDesertCave_EndSand(void);

extern u32 gYampiDesertCave_Settle[];

struct Actor *Actor_FindAtCell(s32 *position, s32 mode);
u16 gYampiDesertCave_MarkBright BSS_DATA;
u16 *gYampiDesertCave_MarkCycleSlots BSS_DATA;
u16 gYampiDesertCave_MarkTimer BSS_DATA;
u16 gYampiDesertCave_MarkDelay BSS_DATA ALIGNED(4);
u16 *gYampiDesertCave_MarkSlots BSS_DATA;
u16 *gYampiDesertCave_MarkColors BSS_DATA;
u16 *gYampiDesertCave_MarkDimColors BSS_DATA;
u16 gYampiDesertCave_MarkCount BSS_DATA;
u16 gYampiDesertCave_MarkCycleIndex BSS_DATA ALIGNED(4);
void Actor_DisableById(s32 id);

extern s8 gYampiDesertCave_LogFootprints[];

/// Makes a log's cells solid where it lies: its footprint's cells take the
/// raised layers' attributes and block the ground; returns 0 for an actor that
/// is no log.
s32 YampiDesertCave_SetLogCells(s32 id) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *actor = Actor_GetById(id);
  s16 sprite = ((struct Sprite *)actor->sprite)->entries[0]->animationId;
  s16 i;
  s32 next;
  u32 kind;
  s32 w, h, x, z;
  s32 az;
  s32 bx, bz;

  i = 0;
  if ((u16)sprite != gYampiDesertCave_LogSprites[i]) {
    do {
      /* FAKEMATCH(temporary): the step taken in the high halfword, as the push
       * footprints' copies of this search have it. */
      next = (i + 1) * 65536;
      kind = (u32)next >> 16;
      i = next >> 16;
    } while (kind < LOG_KIND_COUNT &&
             (u16)sprite != gYampiDesertCave_LogSprites[kind]);
  }
  kind = (u16)i;
  if (kind == LOG_KIND_COUNT)
    return 0;
  w = (ABS(gYampiDesertCave_LogFootprints[kind * 4]) +
       ABS(gYampiDesertCave_LogFootprints[kind * 4 + 2])) >>
      4;
  // i is reused: the footprint's second byte here, layers[1]'s x offset in
  // cells below.
  i = gYampiDesertCave_LogFootprints[kind * 4 + 1];
  h = (ABS(i) + ABS(gYampiDesertCave_LogFootprints[kind * 4 + 3])) >> 4;
  x = FIELD_CELL(actor->x.fixed +
                 toFIXED(gYampiDesertCave_LogFootprints[kind * 4]));
  z = FIELD_CELL(actor->z.fixed + toFIXED(i));
  i = FIELD_CELL(map->layers[1].offsetX);
  az = FIELD_CELL(map->layers[1].offsetY);
  bx = map->layers[2].offsetX;
  bz = map->layers[2].offsetY;
  MapGrid_CopyCellAttributes(x, z, w, h, (u16)i + x, (u16)az + z);
  MapGrid_CopyCellAttributes((u16)FIELD_CELL(bx) + x, (u16)FIELD_CELL(bz) + z,
                             w, h, x + 64, z + 64);
  PushedActor_FillGridAttributeRectangle(0, x, z, w, h, CELL_BEHAVIOR_WALL);
  PushedActor_FillGridAttributeRectangle(2, x, z, w, h, CELL_BEHAVIOR_WALL);
  return 1;
}

#include "../field/map_cell_fill.c"

extern s32 gPushLeaderSteps[];

/// Where a pushed log comes to rest, found before the leader pushes it.
struct LogSlide {
  FIXED x, y, z;          ///< the log's position at the end of the slide
  s32 kind;               ///< its row of gYampiDesertCave_LogFootprints
  s32 slot;               ///< its actor id
  void (*onPushed)(void); ///< run as the log slides, if not NULL
};

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
/// Returns 1 with the log's end position in slide, or 0 if it cannot move.
s32 YampiDesertCave_FindLogSlide(struct LogSlide *slide) {
  FIXED probe[3];
  s32 direction;
  struct Actor *actor;
  s32 kind, w, h, dx;
  FIXED dz;
  s32 count, i, j;
  u8 *state;
  s32 offset;

  actor = YampiDesertCave_FindPushedLog(&direction, &slide->slot, &slide->kind);
  if (actor != NULL) {
    kind = slide->kind;
    w = (ABS(gYampiDesertCave_LogFootprints[kind * 4]) +
         ABS(gYampiDesertCave_LogFootprints[kind * 4 + 2])) >>
        4;
    h = (ABS(gYampiDesertCave_LogFootprints[kind * 4 + 1]) +
         ABS(gYampiDesertCave_LogFootprints[kind * 4 + 3])) >>
        4;
    dx = gPushLeaderSteps[direction] & FIELD_PIXEL_MASK;
    dz = toFIXED(gPushLeaderSteps[direction]);
    slide->onPushed = NULL;
    count = 0;
    state = &actor->layer;
    *state = 2;
    slide->x = actor->x.fixed + dx;
    slide->z = actor->z.fixed + dz;
    probe[1] = actor->y.fixed;
    do {
      offset = kind;
      probe[2] =
          slide->z + toFIXED(gYampiDesertCave_LogFootprints[offset * 4 + 1]);
      offset *= 4;
      for (j = 0; j < h; j++) {
        probe[0] = slide->x + toFIXED(gYampiDesertCave_LogFootprints[offset]);
        for (i = 0; i < w; i++) {
          if (FieldActor_CheckMovementCollision(actor, probe) == 2)
            goto hit;
          probe[0] += toFIXED(16);
        }
        probe[2] += toFIXED(16);
      }
      slide->x += dx;
      count++;
      slide->z += dz;
    } while (1);
  hit:
    *state = 0;
    if (count == 0)
      return 0;
    slide->x = actor->x.fixed + dx * count;
    slide->y = actor->y.fixed;
    slide->z = actor->z.fixed + dz * count;
    return 1;
  } else {
    return 0;
  }
}

struct Actor *YampiDesertCave_FindPushedLog(s32 *direction, s32 *slot,
                                            s32 *kind) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor **entry;
  struct Actor *actor;
  u32 i, k;
  s32 x, z, left, top, right, bottom;

  *direction = leader->facing >> 12;
  i = ACTOR_FIRST_PLACED;
  entry = &work->actors[ACTOR_FIRST_PLACED];
  do {
    actor = *entry;
    for (k = 0; k < LOG_KIND_COUNT; k++) {
      if (((struct Sprite *)actor->sprite)->entries[0]->animationId ==
          gYampiDesertCave_LogSprites[k]) {
        x = (leader->x.part[0].integer +
             (gPushLeaderSteps[*direction] >> 16)) >>
            4;
        z = (leader->z.part[0].integer + (s16)gPushLeaderSteps[*direction]) >>
            4;
        left = (actor->x.part[0].integer +
                gYampiDesertCave_LogFootprints[k * 4]) >>
               4;
        top = (actor->z.part[0].integer +
               gYampiDesertCave_LogFootprints[k * 4 + 1]) >>
              4;
        right = (actor->x.part[0].integer +
                 gYampiDesertCave_LogFootprints[k * 4 + 2]) >>
                4;
        bottom = (actor->z.part[0].integer +
                  gYampiDesertCave_LogFootprints[k * 4 + 3]) >>
                 4;
        if (left <= x && x < right && top <= z && z < bottom) {
          *kind = k;
          if (k & 1) {
            if (left != FIELD_CELL(leader->x.fixed)) {
              *slot = i;
              return actor;
            }
          } else {
            if (top != FIELD_CELL(leader->z.fixed)) {
              *slot = i;
              return actor;
            }
          }
        }
      }
    }
    entry++;
    i++;
  } while (i < 64);
  return NULL;
}

extern struct ActorPlacement gYampiDesertCave_Placements[];
extern struct ActorPlacement gYampiDesertCaveGuardianPlacements[];
extern struct ActorPlacement gYampiDesertCaveCrystalPlacements[];
extern u16 gYampiDesertCaveCrystalStakes[];
extern struct Trigger gYampiDesertCave_Triggers[];
extern struct Trigger gYampiDesertCaveGuardianTriggers[];
extern struct Trigger gYampiDesertCaveCrystalTriggers[];

s32 YampiDesertCave_FindLogSlide(struct LogSlide *slide);
/// The leader pushes a log to where YampiDesertCave_FindLogSlide stopped it,
/// and its cells move with it.
void YampiDesertCave_PushLog(struct LogSlide slide);
void YampiDesertCave_StartMarkGlow(void);
/// The marks stop glowing, and the scene leaves by a backdrop fade.
void YampiDesertCave_StopMarkGlow(void);
s32 List_CountEntries(u16 *list);
void PaletteCycle_Fade(void);
void Pound_SinkTarget(u16 *list, s32 mode, s32 count);
void Teleport_PlaySparks(void);

void YampiDesertCave_PushLog(struct LogSlide slide) {
  /* FAKEMATCH(signature): the map work read through the heap slots as words;
     as Ram_HeapSlots->mapGrid. */
  struct MapWork *map =
      (struct MapWork *)((struct HeapSlotWords *)Ram_HeapSlots)->mapWork;
  s32 w, h;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *actor = Actor_GetById(slide.slot);
  // Always 0: angle is a u16. Direction 0 of the table of steps.
  s32 i = leader->facing >> 16;
  s32 kind = slide.kind;
  s32 speed = leader->speedLimit;
  s32 acceleration = leader->acceleration;
  s32 cell[3];
  FIXED from[3];
  FIXED offset;
  s32 ax, az, bx, bz;

  from[0] = actor->x.fixed;
  from[2] = actor->z.fixed;
  cell[0] = FIELD_CELL(actor->x.fixed +
                       toFIXED(gYampiDesertCave_LogFootprints[kind * 4]));
  cell[2] = FIELD_CELL(actor->z.fixed +
                       toFIXED(gYampiDesertCave_LogFootprints[kind * 4 + 1]));
  w = (ABS(gYampiDesertCave_LogFootprints[kind * 4]) +
       ABS(gYampiDesertCave_LogFootprints[kind * 4 + 2])) >>
      4;
  h = (ABS(gYampiDesertCave_LogFootprints[kind * 4 + 1]) +
       ABS(gYampiDesertCave_LogFootprints[kind * 4 + 3])) >>
      4;
  PushedActor_FillGridAttributeRectangle(0, cell[0], cell[2], w, h,
                                         CELL_BEHAVIOR_NORMAL);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.5, 0.1);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 8);
  wait(15);
  Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId,
                             (slide.x - from[0]) >> 17,
                             (slide.z - from[2]) >> 17);
  Actor_GetById(gMainRoutineWork.leaderActorId)->update =
      YampiDesertCave_StopLeaderAtLog;
  wait(4);
  if ((u32)(i - 6) <= 7)
    Actor_SelectAnimation(actor, 3);
  else
    Actor_SelectAnimation(actor, 2);
  Event_PlaySound(SE_HEAVY_MOVE);
  Actor_SetSpeedAndAcceleration(slide.slot, 0.5, 0.1);
  Actor_SetMoveTarget(actor, slide.x, slide.y, slide.z);
  Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.3, 0.1);
  offset = toFIXED(gPushLeaderSteps[i]);
  Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, offset >> 31,
                             offset >> 17);
  if (slide.onPushed != NULL)
    slide.onPushed();
  Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  leader->update = NULL;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  FieldActor_WaitForMove(actor);
  Event_PlaySound(SE_STOP_EXIT_SE);
  Event_PlaySound(SE_HEAVY_STOP);
  Actor_SetPositionAndResetMotion(actor, slide.x, actor->y.fixed, slide.z);
  Actor_SelectAnimation(actor, 1);
  slide.x =
      FIELD_CELL(slide.x + toFIXED(gYampiDesertCave_LogFootprints[kind * 4]));
  slide.z = FIELD_CELL(slide.z +
                       toFIXED(gYampiDesertCave_LogFootprints[kind * 4 + 1]));
  ax = FIELD_CELL(map->layers[1].offsetX);
  az = FIELD_CELL(map->layers[1].offsetY);
  bx = FIELD_CELL(map->layers[2].offsetX);
  bz = FIELD_CELL(map->layers[2].offsetY);
  MapGrid_CopyCellAttributes(slide.x, slide.z, w, h, ax + slide.x,
                             az + slide.z);
  MapGrid_CopyCellAttributes(slide.x + bx, slide.z + bz, w, h, slide.x + 64,
                             slide.z + 64);
  PushedActor_FillGridAttributeRectangle(0, slide.x, slide.z, w, h,
                                         CELL_BEHAVIOR_WALL);
  PushedActor_FillGridAttributeRectangle(2, slide.x, slide.z, w, h,
                                         CELL_BEHAVIOR_WALL);
  cell[0] =
      FIELD_CELL(from[0] + toFIXED(gYampiDesertCave_LogFootprints[kind * 4]));
  cell[2] = FIELD_CELL(from[2] +
                       toFIXED(gYampiDesertCave_LogFootprints[kind * 4 + 1]));
  MapGrid_CopyCellAttributes(ax + cell[0], az + cell[2], w, h, cell[0],
                             cell[2]);
  MapGrid_CopyCellAttributes(cell[0] + 64, cell[2] + 64, w, h, cell[0] + bx,
                             cell[2] + bz);
  Event_NoOp();
}

/// The leader's update while pushing: it stops at a wall or another log.
void YampiDesertCave_StopLeaderAtLog(struct Actor *actor) {
  FIXED position[3];
  struct Actor *hit;
  u32 i;
  s32 angle = actor->facing >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = actor->x.fixed;
  position[1] = actor->y.fixed;
  position[2] = actor->z.fixed;
  Vector_AddPolarOffset(toFIXED(16), angle, position);
  hit = Actor_FindAtCell(position, 1);
  if (hit != 0) {
    for (i = 0; i < LOG_KIND_COUNT; i++) {
      if (((struct Sprite *)hit->sprite)->entries[0]->animationId ==
          gYampiDesertCave_LogSprites[i])
        return;
    }
    Actor_SetPositionAndResetMotion(actor, actor->x.fixed, actor->y.fixed,
                                    actor->z.fixed);
  }
  position[0] = actor->x.fixed;
  position[1] = actor->y.fixed;
  position[2] = actor->z.fixed;
  if (MapGrid_CheckCollision(actor, position) > 0)
    Actor_SetPositionAndResetMotion(actor, actor->x.fixed, actor->y.fixed,
                                    actor->z.fixed);
}

/// Keeps the marks' palette lists, colours the marks' palette entries, saves
/// both palettes for the fade and, arriving by Teleport, plays the arrival.
void YampiDesertCave_SetMarkColors(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  struct PaletteFade *fade = Ram_HeapSlots->paletteFade;

  gYampiDesertCave_MarkSlots = slots;
  gYampiDesertCave_MarkColors = colours;
  gYampiDesertCave_MarkDimColors = a;
  gYampiDesertCave_MarkCycleSlots = b;
  while (*slots != PALETTE_LIST_END && *colours != PALETTE_LIST_END) {
    int red = *colours++;
    int index = *slots++;
    int green = *colours++;
    int blue = *colours++;
    SET_RGB(BG_PLTT[index], red, green, blue);
  }
  Dma_Set((const void *)BG_PLTT, fade->palette,
          DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS / 2), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, &fade->palette[PALETTE_FADE_BG_COLORS],
          DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS / 2), REG_DMA3);
  PaletteFade_SetTarget(FIXED_ONE, 0);
  if (gMainRoutineWork.teleportState == 2)
    YampiDesertCave_ArriveByTeleport();
}

/// The Teleport marks start to glow, slowly at first and quicker each step.
void YampiDesertCave_StartMarkGlow(void) {
  gYampiDesertCave_MarkCycleIndex = 0;
  gYampiDesertCave_MarkCount = List_CountEntries(gYampiDesertCave_MarkSlots);
  gYampiDesertCave_MarkDelay = 15;
  gYampiDesertCave_MarkTimer = 16;
  gYampiDesertCave_MarkBright = 1;
  Task_Add(PaletteCycle_Fade, TASK_PASS_FRAME);
}

void YampiDesertCave_StopMarkGlow(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Task_Remove(PaletteCycle_Fade);
}

/// The marks flash quickly, as the party arrives by Teleport.
void YampiDesertCave_StartMarkFlash(void) {
  gYampiDesertCave_MarkCycleIndex = 0;
  gYampiDesertCave_MarkCount = List_CountEntries(gYampiDesertCave_MarkSlots);
  gYampiDesertCave_MarkDelay = 2;
  gYampiDesertCave_MarkTimer = 2;
  gYampiDesertCave_MarkBright = 1;
  Task_Add(PaletteCycle_Fade, TASK_PASS_FRAME);
}

#include "shared/list_count_entries.h"

/// The marks' glow, once a frame: at each step the cycle's entries move on one
/// place, and the mark itself turns bright and dim in turn.
void PaletteCycle_Fade(void) {
  s32 step = 0;
  u16 *color;
  volatile u16 *dst;
  u32 i;
  s32 r, g, b;

  if (gYampiDesertCave_MarkTimer != 0 && --gYampiDesertCave_MarkTimer == 0) {
    step = 1;
    gYampiDesertCave_MarkTimer = gYampiDesertCave_MarkDelay;
    if (gYampiDesertCave_MarkDelay > 2)
      gYampiDesertCave_MarkDelay--;
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
  PaletteFade_SetTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 8), 1);
  PaletteFade_ApplyTarget(FIXED_ONE, 2);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  YampiDesertCave_StartMarkFlash();
  Teleport_PlayWhirl();
  wait(40);
  YampiDesertCave_StopMarkGlow();
  PaletteFade_SetTarget(FIXED_ONE, 1);
  PaletteFade_StartInterpolation(16);
  wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->teleportState = 0;
  }
  work->transition = saved;
}

void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

/// Places the stakes already sunk: each entry of the list (actor, kind,
/// flag) loses its collision, and its cell opens. A struct of three halfwords
/// would be padded to 8 bytes, so the list is read as u16 triples.
void YampiDesertCave_PlaceStakes(const u16 *list) {
  const u16 *entry;

  if (list[0] == ACTOR_LIST_END)
    return;
  entry = list;
  do {
    u32 id = entry[0];
    struct Actor *actor = Actor_GetById(id);
    s16 kind = entry[1];
    s16 flag = entry[2];

    actor->collisionFlags &= ~1;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetCellOpen(id, (u16)kind, (u16)flag, actor->layer);
    entry += 3;
  } while (entry[0] != ACTOR_LIST_END);
}

// Finds the current target's table row and shrinks it.
#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

/// Talking to Crystal, the Venus Djinni Scoop dug up, starts its battle.
void YampiDesertCave_FightCrystal(void) {
  Event_SetFlag(FLAG_YAMPI_DESERT_CAVE_CRYSTAL_DUG_UP);
  Event_RequestDjinnBattle(ACTOR_CRYSTAL, DJINNI_CRYSTAL);
  return;
}

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
  struct LogSlide slide;

  if (YampiDesertCave_FindLogSlide(&slide))
    YampiDesertCave_PushLog(slide);
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
    if (!Event_GetFlag(flag) && phase == 3) {                                  \
      Actor_SetPosition(actorId, 0, 0);                                        \
      GameFlag_Set(flag);                                                      \
    }                                                                          \
  }

DEFINE_BURST_PILLAR(YampiDesertCave_BurstChestRoomPillar,
                    FLAG_YAMPI_DESERT_CAVE_CHEST_ROOM_PILLAR_BURST,
                    ACTOR_CHEST_ROOM_BURST_PILLAR)

DEFINE_BURST_PILLAR(YampiDesertCave_BurstCrystalRoomPillar,
                    FLAG_YAMPI_DESERT_CAVE_CRYSTAL_ROOM_PILLAR_BURST,
                    ACTOR_CRYSTAL_ROOM_BURST_PILLAR)

/// Move takes a pillar: the cells it stood on open, as the room has them
/// without it.
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
      struct Actor *actor = Actor_GetById(ACTOR_WEST_PILLAR);

      s32 column = FIELD_CELL(actor->x.fixed);
      s32 row = FIELD_CELL(actor->z.fixed);

      MapGrid_CopyCellAttributes(7, 10, 1, 1, column, row);
    }
  }
}

/// Move sets a pillar down: where it opens the way it stays for good, and the
/// cells under it close again.
void YampiDesertCave_FinishMovingPillar(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);

  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_GUARDIAN) {
    s32 row = FIELD_CELL(actor->z.fixed);

    if (row == 13) {
      actor->motionFlags = 0;
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
      column = FIELD_CELL(actor->x.fixed);
      if (column == 38) {
        Event_SetFlag(FLAG_YAMPI_DESERT_CAVE_EAST_PILLAR_MOVED);
        MapGrid_CopyCellAttributes(38, 15, 1, 1, column, 16);
      } else {
        MapGrid_CopyCellAttributes(38, 15, 1, 1, 39, 16);
      }
    }
    if (id == ACTOR_WEST_PILLAR) {
      if (FIELD_CELL(actor->x.fixed) == 10 && FIELD_CELL(actor->z.fixed) == 11)
        Event_SetFlag(FLAG_YAMPI_DESERT_CAVE_WEST_PILLAR_MOVED);
      MapGrid_CopyCellAttributes(12, 11, 1, 1, FIELD_CELL(actor->x.fixed),
                                 FIELD_CELL(actor->z.fixed));
    }
  }
}

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

/// Pound sinks the stake in Crystal's room and opens its cell.
void YampiDesertCave_PoundStake(s32 mode, s32 count) {
  Pound_SinkTarget(gYampiDesertCaveCrystalStakes, mode, count);
  MapGrid_CopyCellAttributes(47, 12, 1, 1, 46, 12);
}

extern s16 gYampiDesertCave_RipplePath[];

static __inline__ s32 GameFlag_GetByte_(s32 flag) {
  return ((s32)GameFlag_GetByte(flag));
}

/// Moves the ripple along its path, a row of gYampiDesertCave_RipplePath for
/// every thirty-two steps of its counter, which counts only while the party
/// walks.
void YampiDesertCave_MoveSandRipple(void) {
  s32 step = GameFlag_GetByte_(FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP);
  s32 index = (step / RIPPLE_STEPS_PER_POINT) * 2;
  s32 column = gYampiDesertCave_RipplePath[index];
  s32 row = gYampiDesertCave_RipplePath[index + 1];

  if (Actor_GetById(ACTOR_SAND_RIPPLE)->paused == 0) {
    Actor_SetPositionFX(ACTOR_SAND_RIPPLE, FIELD_HALF_CELL_ORIGIN(column),
                        FIELD_HALF_CELL_ORIGIN(row));
    if (Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS) == 0)
      step++;
    GameFlag_SetByte(FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP, step);
  }
}

/// Scoop on the ripple's cell stops it (mode 1) and digs up Crystal, who
/// drops to the ground (mode 2).
void YampiDesertCave_ScoopDjinni(s32 mode) {
  s32 index =
      (((s32)GameFlag_GetByte(FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP)) /
       RIPPLE_STEPS_PER_POINT) *
      2;
  s16 column = gYampiDesertCave_RipplePath[index];
  s32 px = column * 8;
  s16 row = gYampiDesertCave_RipplePath[index + 1];
  s32 pz = row * 8;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  // The Scoop's hand, where it digs: the ScoopWork's actor, which only
  // field_psynergy_scoop.c declares.
  struct Actor *scoopHand =
      *(struct Actor **)(Ram_HeapSlots->battleEffect + 0x1930);
  s32 x = 0, z = 0;

  if (scoopHand != NULL) {
    x = FIELD_CELL(scoopHand->x.fixed);
    z = FIELD_CELL(scoopHand->z.fixed);
  }
  if (px / 16 != x || pz / 16 != z)
    return;
  if (mode == 1) {
    Task_Remove(YampiDesertCave_MoveSandRipple);
    Actor_SetPosition(ACTOR_SAND_RIPPLE, 0, 0);
  }
  if (mode == 2) {
    struct Actor *crystal = Actor_GetById(ACTOR_CRYSTAL);

    Actor_SetPositionFX(ACTOR_CRYSTAL, FIELD_HALF_CELL_ORIGIN(column),
                        FIELD_HALF_CELL_ORIGIN(row));
    crystal->y.fixed = toFIXED(32);
    crystal->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    crystal->gravity = toFIXED(0.09);
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

/// Valukar, the guardian of fire, speaks in a warm glow and comes at the
/// leader, and its battle begins; there is no escape from it.
void YampiDesertCave_MeetGuardianOfFire(void) {
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(ACTOR_VALUKAR, 2);
  Event_Wait(20);
  PaletteFade_ApplySource(FIXED_ONE, 0);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, 1);
  PaletteFade_StartInterpolation(60);
  Event_Wait(60);
  EventMessage_ShowCentered((s32)MsgIAmThe, 0, 6);
  PaletteFade_SetTarget(FIXED_ONE, 0);
  PaletteFade_StartInterpolation(60);
  Event_Wait(60);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(ACTOR_VALUKAR, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  Event_Wait(20);
  gMainRoutineWork.pendingDjinni = IF_FLAG_SET(FLAG_VALUKAR_BEATEN);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  gMainRoutineWork.battleBackground = (s32)&ResourceId_LavaCaveBackdrop;
  Field_RequestBattleFromEncounterTable(103, 1);
}

/// Scoop digs up the treasure hidden in Valukar's room.
void YampiDesertCave_ScoopTreasure(s32 mode) {
  Event_DigUpTreasure(mode, ACTOR_HIDDEN_TREASURE,
                      FLAG_YAMPI_DESERT_CAVE_TREASURE_DUG_UP);
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_GUARDIAN)
    return gYampiDesertCaveGuardianTriggers;
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL)
    return gYampiDesertCaveCrystalTriggers;
  return gYampiDesertCave_Triggers;
}

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
      Actor_SetWideSprite(ACTOR_HIDDEN_TREASURE, 1);
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
        step = step / RIPPLE_STEPS_PER_POINT * 2;
        Actor_SetPosition(ACTOR_SAND_RIPPLE, 0, 0);
        Actor_SetPositionFX(
            ACTOR_CRYSTAL,
            FIELD_HALF_CELL_ORIGIN(gYampiDesertCave_RipplePath[step]),
            FIELD_HALF_CELL_ORIGIN(gYampiDesertCave_RipplePath[step + 1]));
      } else {
        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
          GameFlag_SetByte(FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP, 0);
        Task_Add(YampiDesertCave_MoveSandRipple, TASK_PASS_FRAME);
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

void YampiDesertCave_MeetGuardianOfFire(void);

// clang-format off
/// The sprites pushed as logs, in the order of their footprints.
u16 gYampiDesertCave_LogSprites[] = {
    SPRITE_LOG_WEST_EAST, SPRITE_LOG_NORTH_SOUTH,
    SPRITE_PIPE_WEST_EAST, SPRITE_PIPE_NORTH_SOUTH,
    SPRITE_FALLEN_COLUMN_WEST_EAST, SPRITE_FALLEN_COLUMN_NORTH_SOUTH,
};

/// Each log's footprint around its position, in pixels: left, top, right and
/// bottom.
s8 gYampiDesertCave_LogFootprints[] = {
    -32, -8, 32, 8, -8, -32, 8, 32,
    -32, -16, 32, 0, -8, -32, 8, 32,
    -32, -8, 32, 8, -8, -32, 8, 32,
};

#include "shared/push_leader_steps.c"

/// The ripple's path, a column and a row in half cells (8 pixels) for each
/// thirty-two steps: it shows at four places in turn, and between them lies at
/// the origin, out of sight.
s16 gYampiDesertCave_RipplePath[] = {
    101, 39, 0, 0, 97, 35, 0, 0,
    99, 37, 0, 0, 95, 33, 0, 0,
};

/// The background palette entries the Teleport marks are drawn in.
s16 gYampiDesertCave_MarkPaletteSlots[] = {
    9, 10, 11, 12, 13, 14, 15, -1,
};

/// The marks' bright colours, as red, green and blue for each entry.
u16 gYampiDesertCave_MarkColorsLit[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

/// The marks' dim colours, which the glow moves round the entries.
u16 gYampiDesertCave_MarkColorsDim[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

/// The glow's steps: for each, the palette entries the dim colours go to.
u16 gYampiDesertCave_MarkCycle[] = {                                                               
      14, 9, 10, 11, 12, 13, 13, 14, 9, 10, 11, 12, 12, 13, 14, 9, 10, 11,  
      11, 12, 13, 14, 9, 10, 10, 11, 12, 13, 14, 9, 9, 10, 11, 12, 13, 14,  
  };

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(YampiDesertCaveWaterOfLifeChest),
    SCENE_EXIT(YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST, YampiDesertTrainersWhipChest, YAMPI_DESERT_TRAINERS_WHIP_CHEST_FROM_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST),
    SCENE_EXIT(YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_CAVE_CRYSTAL_SOUTH, YampiDesertCaveCrystal, YAMPI_DESERT_CAVE_CRYSTAL_FROM_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_SOUTH),
    SCENE_EXIT(3, YampiDesertCaveWaterOfLifeChest, 6),
    SCENE_EXIT(4, YampiDesertCaveWaterOfLifeChest, 5),
    SCENE_EXIT(5, YampiDesertCaveWaterOfLifeChest, 4),
    SCENE_EXIT(6, YampiDesertCaveWaterOfLifeChest, 3),
    SCENE_EXIT(YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_CAVE_CRYSTAL_NORTH, YampiDesertCaveCrystal, YAMPI_DESERT_CAVE_CRYSTAL_FROM_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_NORTH),
    SCENE_EXIT(YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_CAVE_GUARDIAN, YampiDesertCaveGuardian, YAMPI_DESERT_CAVE_GUARDIAN_FROM_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST),
    SCENE_EXITS(YampiDesertCaveGuardian),
    SCENE_EXIT(YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST, YampiDesertCaveWaterOfLifeChest, YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_FROM_YAMPI_DESERT_CAVE_GUARDIAN),
    SCENE_EXIT(2, YampiDesertCaveGuardian, 17),
    SCENE_EXIT(YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_CRYSTAL_NORTHWEST, YampiDesertCaveCrystal, YAMPI_DESERT_CAVE_CRYSTAL_FROM_YAMPI_DESERT_CAVE_GUARDIAN_NORTHWEST),
    SCENE_EXIT(YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_CRYSTAL_SOUTH, YampiDesertCaveCrystal, YAMPI_DESERT_CAVE_CRYSTAL_FROM_YAMPI_DESERT_CAVE_GUARDIAN_SOUTH),
    SCENE_EXIT(YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_CRYSTAL_SOUTHEAST, YampiDesertCaveCrystal, YAMPI_DESERT_CAVE_CRYSTAL_FROM_YAMPI_DESERT_CAVE_GUARDIAN_SOUTHEAST),
    SCENE_EXIT(17, YampiDesertCaveGuardian, 2),
    SCENE_EXITS(YampiDesertCaveCrystal),
    SCENE_EXIT(YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_SOUTH, YampiDesertCaveWaterOfLifeChest, YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_FROM_YAMPI_DESERT_CAVE_CRYSTAL_SOUTH),
    SCENE_EXIT(2, YampiDesertCaveCrystal, 3),
    SCENE_EXIT(3, YampiDesertCaveCrystal, 2),
    SCENE_EXIT(4, YampiDesertCaveCrystal, 7),
    SCENE_EXIT(5, YampiDesertCaveCrystal, 6),
    SCENE_EXIT(6, YampiDesertCaveCrystal, 5),
    SCENE_EXIT(7, YampiDesertCaveCrystal, 4),
    SCENE_EXIT(YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_NORTH, YampiDesertCaveWaterOfLifeChest, YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_FROM_YAMPI_DESERT_CAVE_CRYSTAL_NORTH),
    SCENE_EXIT(9, YampiDesertCaveCrystal, 10),
    SCENE_EXIT(10, YampiDesertCaveCrystal, 9),
    SCENE_EXIT(YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_GUARDIAN_SOUTHEAST, YampiDesertCaveGuardian, YAMPI_DESERT_CAVE_GUARDIAN_FROM_YAMPI_DESERT_CAVE_CRYSTAL_SOUTHEAST),
    SCENE_EXIT(YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_GUARDIAN_SOUTH, YampiDesertCaveGuardian, YAMPI_DESERT_CAVE_GUARDIAN_FROM_YAMPI_DESERT_CAVE_CRYSTAL_SOUTH),
    SCENE_EXIT(YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_GUARDIAN_NORTHWEST, YampiDesertCaveGuardian, YAMPI_DESERT_CAVE_GUARDIAN_FROM_YAMPI_DESERT_CAVE_CRYSTAL_NORTHWEST),
    SCENE_EXITS_END,
};

/// A pushed pillar or stake stands still where it was placed.
u32 gYampiDesertCave_Settle[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

/// The Water of Life chest's room: three logs and the pillar Burst breaks.
struct ActorPlacement gYampiDesertCave_Placements[] = {
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 888, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 832, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 912, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

/// Valukar's room: the pillar Move pushes, and Valukar until he is beaten.
struct ActorPlacement gYampiDesertCaveGuardianPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS,
                    (s32)gYampiDesertCave_Settle, 552, 0, 200, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_VALUKAR, FLAG_VALUKAR_BEATEN, MOVEMENT_TYPE_STAND,
                    200, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// Crystal's room, in YampiDesertCaveCrystalActorId order.
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
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS,
                    (s32)gYampiDesertCave_Settle, 744, 0, 200, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    264, 0, 632, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SAND_RIPPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_CRYSTAL),
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The stake Pound sinks in Crystal's room: its actor, its kind and the flag
/// set once it is sunk, as Pound_SinkTarget reads them.
u16 gYampiDesertCaveCrystalStakes[] ALIGNED(4) = {
    ACTOR_STAKE,
    0,
    FLAG_YAMPI_DESERT_CAVE_STAKE_POUNDED,
    ACTOR_LIST_END,
};

/// The Water of Life chest's room: its exits, the logs, the pillar, the two
/// Teleport marks and Sand.
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

/// Valukar's room: its exits, Valukar's step, the pillar, Sand and the hidden
/// treasure.
struct Trigger gYampiDesertCaveGuardianTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_STEP, 52, FLAG_VALUKAR_BEATEN,
     EVENT(YampiDesertCave_MeetGuardianOfFire)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_GUARDIAN_ROOM_PILLAR, FLAG_YAMPI_DESERT_CAVE_GUARDIAN_PILLAR_MOVED,
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

/// Crystal's room: its exits, the ripple and Crystal, the logs, the pillars,
/// the stake, Sand and Scoop on the ripple.
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
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_EAST_PILLAR, FLAG_YAMPI_DESERT_CAVE_EAST_PILLAR_MOVED,
     EVENT(YampiDesertCave_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR,
     FLAG_YAMPI_DESERT_CAVE_EAST_PILLAR_MOVED,
     EVENT(YampiDesertCave_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_WEST_PILLAR, FLAG_YAMPI_DESERT_CAVE_WEST_PILLAR_MOVED,
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
     IF_FLAG_SET(FLAG_YAMPI_DESERT_CAVE_RIPPLE_RUNNING),
     EVENT(YampiDesertCave_ScoopDjinni)},
    {TRIGGER_TABLE_END},
};
