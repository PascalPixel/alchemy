/// @file
/// The Yampi Desert Cave, the hidden dungeon behind the desert's sand slide:
/// Teleport marks lead into its inner rooms, where the party pushes logs,
/// sinks stakes with Pound, bursts pillars, slips under walls with Sand,
/// Scoops up the Venus Djinni Crystal racing through the sand, and meets
/// Valukar, the guardian of fire.

#include "shared/map_cells.h"
#include "shared/actors.h"
#include "shared/effects.h"
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"
#include "macros.h"
#include "event_inline.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/scene/scene.h"

/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

struct Actor;

extern u16 gYampiDesertCave_LogSprites[];

void YampiDesertCave_StopLeaderAtLog(struct Actor *object);

void Actor_SelectAnimation(struct Actor *object, s32 mode);

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

#include "scene_ids.h"

struct Actor *Actor_FindAtCell(s32 *position, s32 mode);
extern u16 *gYampiDesertCave_MarkColors;
extern u16 *gYampiDesertCave_MarkDimColors;
extern u16 *gYampiDesertCave_MarkCycleSlots;
void Actor_DisableById(s32 id);

/* VIEW(first): routine gYampiDesertCave_LogFootprints as this part declared it;
 * the joined file has another. */
extern s8 gYampiDesertCave_LogFootprints__first[] __asm__(
    "gYampiDesertCave_LogFootprints");
#define gYampiDesertCave_LogFootprints gYampiDesertCave_LogFootprints__first

/// Makes a log's cells solid where it lies: its footprint's cells take the
/// raised layers' attributes and block the ground; returns 0 for an actor that
/// is no log.
s32 YampiDesertCave_SetLogCells(s32 id) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(id);
  s16 sprite = ((struct ObjectAnimationState *)object->sprite)->value28[0];
  s16 i;
  u32 kind;
  s32 w, h, x, z;
  s32 az;
  s32 bx, bz;
  s32 toX, toZ, fromX, fromZ;
  s32 layer1X, layer1Z;

  i = 0;
  if ((u16)sprite != gYampiDesertCave_LogSprites[i]) {
  loop:
    /* FAKEMATCH(temporary): kind set to i, then stepped; as kind = (u16)(i +
     * 1), 12 of 160 instructions differ in tla-en. */
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
  x = (object->x + (gYampiDesertCave_LogFootprints[kind * 4] << 16)) >> 20;
  z = (object->z + (i << 16)) >> 20;
  i = *(s32 *)&map->layers[1].unk08[0] >> 20;
  az = *(s32 *)&map->layers[1].unk08[4] >> 20;
  bx = *(s32 *)&map->layers[2].unk08[0];
  bz = *(s32 *)&map->layers[2].unk08[4];
  /* FAKEMATCH(temporary): the first call's last two arguments held in locals;
   * as direct arguments, 36 of 160 instructions differ in tla-en. */
  layer1X = (u16)i + x;
  layer1Z = (u16)az + z;
  MapGrid_CopyCellAttributes(x, z, w, h, layer1X, layer1Z);
  /* FAKEMATCH(temporary): the second call's arguments held in locals; as direct
   * arguments, 10 of 160 instructions differ in tla-en. */
  fromX = (u16)(bx >> 20) + x;
  fromZ = (u16)(bz >> 20) + z;
  toX = x + 64;
  toZ = z + 64;
  MapGrid_CopyCellAttributes(fromX, fromZ, w, h, toX, toZ);
  Map_FillCellRectByte2(0, x, z, w, h, 255);
  Map_FillCellRectByte2(2, x, z, w, h, 255);
  return 1;
}

#include "../field/map/cellfill.c"

extern s32 gPushLeaderSteps[];

/// Finds the log the leader faces and can push: its kind, its actor slot and
/// the direction, or NULL.
struct Actor *YampiDesertCave_FindPushedLog(s32 *direction, s32 *slot,
                                            s32 *kind);

static inline s32 YampiDesertCave_CheckCollision(struct Actor *object,
                                                 s32 *position) {
  return MapGrid_CheckCollision(object, position);
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
    state = &object->terrainId;
    *state = 2;
    position[0] = object->x + dx;
    position[2] = object->z + dz;
    probe[1] = object->y;
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
      /* FAKEMATCH(wrapper): the call through YampiDesertCave_CheckCollision; as
         MapGrid_CheckCollision(object, probe), 2 of 171 instructions differ in
         tla-en.
       */
      if (YampiDesertCave_CheckCollision(object, probe) == 2)
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
    position[0] = object->x + dx * count;
    position[1] = object->y;
    position[2] = object->z + dz * count;
    return 1;
  } else {
    return 0;
  }
}

#include "field/field.h"
#include "text/msg_ids.h"
#include "system/main_routine_work.h"

#include "event_work.h"

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

struct Actor *YampiDesertCave_FindPushedLog(s32 *direction, s32 *slot,
                                            s32 *kind) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor **entry;
  struct Actor *object;
  u32 i, k;
  s32 x, z, left, top, right, bottom;

  *direction = leader->facing >> 12;
  i = 8;
  entry = &work->objects[8];
outer:
  object = *entry;
  k = 0;
inner:
  if (((struct ObjectAnimationState *)object->sprite)->value28[0] ==
      gYampiDesertCave_LogSprites[k]) {
    x = (PIXELS(leader->x) + (gPushLeaderSteps[*direction] >> 16)) >> 4;
    z = (PIXELS(leader->z) + (s16)gPushLeaderSteps[*direction]) >> 4;
    left = (PIXELS(object->x) + gYampiDesertCave_LogFootprints[k * 4]) >> 4;
    top = (PIXELS(object->z) + gYampiDesertCave_LogFootprints[k * 4 + 1]) >> 4;
    right =
        (PIXELS(object->x) + gYampiDesertCave_LogFootprints[k * 4 + 2]) >> 4;
    bottom =
        (PIXELS(object->z) + gYampiDesertCave_LogFootprints[k * 4 + 3]) >> 4;
    if (left <= x && x < right && top <= z && z < bottom) {
      *kind = k;
      if (k & 1) {
        if (left != leader->x >> 20) {
          *slot = i;
          return object;
        }
      } else {
        if (top != leader->z >> 20) {
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

#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_inline.h"

extern char SceneId_Ov662Room1;
extern char SceneId_Ov662Room2;
extern char SceneId_Ov662Room3;

struct YampiDesertCaveRect {
  s32 word[6];
};

extern u16 gYampiDesertCave_MarkBright;
extern u16 gYampiDesertCave_MarkTimer;
extern u16 gYampiDesertCave_MarkPeriod;
extern u16 *gYampiDesertCave_MarkSlots;
extern u16 gYampiDesertCave_MarkCount;
extern u16 gYampiDesertCave_MarkCycleIndex;
/* VIEW(first): routine gYampiDesertCave_Placements as this part declared it;
 * the joined file has another. */
extern u32
    gYampiDesertCave_Placements__first[] __asm__("gYampiDesertCave_Placements");
#define gYampiDesertCave_Placements gYampiDesertCave_Placements__first
/* VIEW(first): routine gYampiDesertCave_Room2Placements as this part declared
 * it; the joined file has another. */
extern s32 gYampiDesertCave_Room2Placements__first[] __asm__(
    "gYampiDesertCave_Room2Placements");
#define gYampiDesertCave_Room2Placements gYampiDesertCave_Room2Placements__first
/* VIEW(first): routine gYampiDesertCave_Room3Placements as this part declared
 * it; the joined file has another. */
extern s32 gYampiDesertCave_Room3Placements__first[] __asm__(
    "gYampiDesertCave_Room3Placements");
#define gYampiDesertCave_Room3Placements gYampiDesertCave_Room3Placements__first
/* VIEW(first): routine gYampiDesertCave_Room3Stakes as this part declared it;
 * the joined file has another. */
extern const u8 gYampiDesertCave_Room3Stakes__first[] __asm__(
    "gYampiDesertCave_Room3Stakes");
#define gYampiDesertCave_Room3Stakes gYampiDesertCave_Room3Stakes__first
/* VIEW(first): routine gYampiDesertCave_Triggers as this part declared it; the
 * joined file has another. */
extern const u8
    gYampiDesertCave_Triggers__first[] __asm__("gYampiDesertCave_Triggers");
#define gYampiDesertCave_Triggers gYampiDesertCave_Triggers__first
/* VIEW(first): routine gYampiDesertCave_Room2Triggers as this part declared it;
 * the joined file has another. */
extern const u8 gYampiDesertCave_Room2Triggers__first[] __asm__(
    "gYampiDesertCave_Room2Triggers");
#define gYampiDesertCave_Room2Triggers gYampiDesertCave_Room2Triggers__first
/* VIEW(first): routine gYampiDesertCave_Room3Triggers as this part declared it;
 * the joined file has another. */
extern const u8 gYampiDesertCave_Room3Triggers__first[] __asm__(
    "gYampiDesertCave_Room3Triggers");
#define gYampiDesertCave_Room3Triggers gYampiDesertCave_Room3Triggers__first

s32 YampiDesertCave_FindLogSlide(s32 *position);
/// The leader pushes a log to where YampiDesertCave_FindLogSlide stopped it,
/// and its cells move with it.
void YampiDesertCave_PushLog(struct YampiDesertCaveRect rect);
void YampiDesertCave_StartMarkGlow(void);
void YampiDesertCave_StopMarkGlow(void);
s32 List_CountEntries(u16 *list);
void PaletteCycle_Fade(void);
void Statue_Sink(u16 *list, s32 mode, s32 count);
void Actor_ReturnHome(s32 a, s32 b, s32 c);
void Teleport_PlaySparks(void);

static inline void YampiDesertCave_Call1920(s32 a, s32 b, s32 c) {
  Actor_ReturnHome(a, b, c);
}

void YampiDesertCave_PushLog(struct YampiDesertCaveRect rect) {
  // Read as s32: the load then keeps its place after the argument stores.
  struct MapWork *map =
      (struct MapWork *)((struct HeapView *)Ram_HeapSlots)->word;
  s32 w, h;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
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

  from[0] = object->x;
  from[2] = object->z;
  cell[0] =
      (object->x + (gYampiDesertCave_LogFootprints[kind * 4] << 16)) >> 20;
  cell[2] =
      (object->z + (gYampiDesertCave_LogFootprints[kind * 4 + 1] << 16)) >> 20;
  w = (ABS(gYampiDesertCave_LogFootprints[kind * 4]) +
       ABS(gYampiDesertCave_LogFootprints[kind * 4 + 2])) >>
      4;
  h = (ABS(gYampiDesertCave_LogFootprints[kind * 4 + 1]) +
       ABS(gYampiDesertCave_LogFootprints[kind * 4 + 3])) >>
      4;
  Map_FillCellRectByte2(0, cell[0], cell[2], w, h, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x1999);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 8);
  wait(15);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner,
                             (rect.word[0] - from[0]) >> 17,
                             (rect.word[2] - from[2]) >> 17);
  Actor_GetById(gMainRoutineWork.currentOwner)->update =
      YampiDesertCave_StopLeaderAtLog;
  wait(4);
  Actor_SelectAnimation(object, 2);
  Event_PlaySound(239);
  Actor_SetSpeed(rect.word[4], 0x8000, 0x1999);
  Actor_SetMoveTarget(object, rect.word[0], rect.word[1], rect.word[2]);
  /* FAKEMATCH(dead): a loop of one pass with an untaken two-test branch, so the
   * second half starts afresh in cse; as no loop, 277 of 342 instructions
   * differ in tla-en. */
  for (j = 0; j < 1; j++) {
    if (j != 0 && j != 5)
      w = 1;
  }
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x4ccc, 0x1999);
  offset = gPushLeaderSteps[i] << 16;
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, offset >> 31,
                             offset >> 17);
  if (rect.word[5] != 0)
    ((void (*)(void))rect.word[5])();
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  leader->update = NULL;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  FieldActor_WaitForMove(object);
  Event_PlaySound(298);
  Event_PlaySound(213);
  Actor_SetPositionAndResetMotion(object, rect.word[0], object->y,
                                  rect.word[2]);
  Actor_SelectAnimation(object, 1);
  rect.word[0] =
      (rect.word[0] + (gYampiDesertCave_LogFootprints[kind * 4] << 16)) >> 20;
  rect.word[2] =
      (rect.word[2] + (gYampiDesertCave_LogFootprints[kind * 4 + 1] << 16)) >>
      20;
  ax = *(s32 *)&map->layers[1].unk08[0] >> 20;
  az = *(s32 *)&map->layers[1].unk08[4] >> 20;
  bx = *(s32 *)&map->layers[2].unk08[0] >> 20;
  bz = *(s32 *)&map->layers[2].unk08[4] >> 20;
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
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
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
    Actor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
  }
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  if (MapGrid_CheckCollision(object, position) > 0)
    Actor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}

#include "dma.h"
#include "io_reg.h"

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
DEFINE_PALETTE_SET_COLORS(YampiDesertCave_SetMarkColors,
                          gYampiDesertCave_MarkSlots,
                          gYampiDesertCave_MarkColors,
                          gYampiDesertCave_MarkDimColors,
                          gYampiDesertCave_MarkCycleSlots,
                          YampiDesertCave_ArriveByTeleport)

#include "task.h"

DEFINE_PALETTE_CYCLE_START_VBLANK(
    YampiDesertCave_StartMarkGlow, gYampiDesertCave_MarkCycleIndex,
    gYampiDesertCave_MarkCount, List_CountEntries, gYampiDesertCave_MarkSlots,
    gYampiDesertCave_MarkPeriod, 15, gYampiDesertCave_MarkTimer, 16,
    gYampiDesertCave_MarkBright, PaletteCycle_Fade)

void YampiDesertCave_StopMarkGlow(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
  Task_Remove(PaletteCycle_Fade);
}

DEFINE_PALETTE_CYCLE_START_VBLANK(
    YampiDesertCave_StartMarkFlash, gYampiDesertCave_MarkCycleIndex,
    gYampiDesertCave_MarkCount, List_CountEntries, gYampiDesertCave_MarkSlots,
    gYampiDesertCave_MarkPeriod, 2, gYampiDesertCave_MarkTimer, 2,
    gYampiDesertCave_MarkBright, PaletteCycle_Fade)

DEFINE_LIST_COUNT_ENTRIES(List_CountEntries)

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
  struct YampiDesertCaveEventWork *work = Ram_HeapSlots->eventWork;
  s32 saved;

  saved = work->value1ac;
  BattleEffect_ApplyColorToTargetBuffer(0x202108, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  ((struct YampiDesertCaveEventWork *)Ram_HeapSlots->eventWork)->value1ac =
      0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  YampiDesertCave_StartMarkFlash();
  Teleport_PlayWhirl();
  wait(40);
  YampiDesertCave_StopMarkGlow();
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 1);
  PaletteFade_StartInterpolation(16);
  wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}

void YampiDesertCave_SetStakeCell(s32 id, u32 kind, s32 flag, s32 layer);

/// Walks a list of three-halfword entries (object id, column, row) ended by
/// 0xffff, clearing each object's collision bit and child flags before passing
/// the entry to YampiDesertCave_SetStakeCell. A struct of three halfwords would
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
    YampiDesertCave_SetStakeCell(id, (u16)column, (u16)row, object->terrainId);
    entry += 3;
  }
  if (entry[0] != 0xffff)
    goto loop;
}

// Finds the current target's table row and shrinks it.
DEFINE_STATUE_SINK(Statue_Sink, YampiDesertCave_SetStakeCell)

#include "field_event.h"

s32 Actor_GetCellIndex(struct Actor *object);

DEFINE_ACTOR_SET_CELL_OPEN(YampiDesertCave_SetStakeCell, Actor_GetCellIndex,
                           Actor_SelectAnimation)

#include "battle.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

/// Talking to Crystal, the Venus Djinni Scoop dug up, starts its battle
/// (flag 0x271).
void YampiDesertCave_FightCrystal(void) {
  Event_SetFlag(0x271);
  Event_RequestDjinnBattle(17, 17);
  return;
}

void Event_End();

/// Talking to the ripple racing through the sand only puzzles the leader.
void YampiDesertCave_WonderAtRipple(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldActor_ShowEmote(gMainRoutineWork.currentOwner, 0x101, 32);
  Event_End();
  return;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

u32 *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room2)
    return (u32 *)gYampiDesertCave_Room2Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3)
    return (u32 *)gYampiDesertCave_Room3Placements;
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
    {                                                                          \
      u8 *party = (u8 *)&gMainRoutineWork;                                     \
                                                                               \
      party[0x2c2] = 2;                                                        \
    }                                                                          \
    Event_SetDestination((s32)&SceneId_Ov662Room1, n1);                        \
    Event_Wait(40);                                                            \
    YampiDesertCave_StopMarkGlow();                                            \
  }

DEFINE_TELEPORT_FROM_MARK(YampiDesertCave_TeleportFromFirstMark, 10)

DEFINE_TELEPORT_FROM_MARK(YampiDesertCave_TeleportFromSecondMark, 9)

/// Burst breaks a pillar for good (flag n1) as it acts (phase 3).
#define DEFINE_BURST_PILLAR(name, n1, n2)                                      \
  void name(s32 arg0) {                                                        \
    s32 result0;                                                               \
                                                                               \
    result0 = Event_GetFlag(n1);                                               \
    if (result0 != 0)                                                          \
      goto done;                                                               \
    if (arg0 != 3)                                                             \
      goto done;                                                               \
    Actor_SetHorizontalPositionWithTerrain(n2, 0, 0);                          \
    GameFlag_Set(n1);                                                          \
  done:                                                                        \
    return;                                                                    \
  }

DEFINE_BURST_PILLAR(YampiDesertCave_BurstRoom1Pillar, 0x300, 11)

DEFINE_BURST_PILLAR(YampiDesertCave_BurstRoom3Pillar, 0x301, 15)

void YampiDesertCave_StartMovingPillar(s32 unused, s32 id) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room2) {
    s32 row = 12;

    MapGrid_CopyCellAttributes(98, 3, 3, 3, 97, row);
    MapGrid_CopyCellAttributes(33, 12, 1, 1, 34, row);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3) {
    if (id == 12) {
      MapGrid_CopyCellAttributes(102, 15, 1, 1, 103, 15);
      MapGrid_CopyCellAttributes(43, 14, 1, 1, 39, 16);
    }
    if (id == 13) {
      struct Actor *object = Actor_GetById(13);

      s32 column = object->x >> 20;
      s32 row = object->z >> 20;

      MapGrid_CopyCellAttributes(7, 10, 1, 1, column, row);
    }
  }
}

/// Rooms 2 and 3: by where object id stands, sets flags 0x303 to 0x305 and
/// makes the rooms' Map_CopyCellAttributeRect calls.
void YampiDesertCave_FinishMovingPillar(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room2) {
    s32 row = object->z >> 20;

    if (row == 13) {
      object->motionFlags = 0;
      Event_SetFlag(0x303);
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, row);
      MapGrid_CopyCellAttributes(95, 3, 3, 3, 97, 12);
    } else {
      row = 12;
      MapGrid_CopyCellAttributes(92, 3, 3, 3, 97, row);
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, row);
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3) {
    if (id == 12) {
      s32 column;

      MapGrid_CopyCellAttributes(103, 16, 1, 1, 103, 15);
      column = object->x >> 20;
      if (column == 38) {
        Event_SetFlag(0x305);
        MapGrid_CopyCellAttributes(38, 15, 1, 1, column, 16);
      } else {
        MapGrid_CopyCellAttributes(38, 15, 1, 1, 39, 16);
      }
    }
    if (id == 13) {
      if (object->x >> 20 == 10 && object->z >> 20 == 11)
        Event_SetFlag(0x304);
      MapGrid_CopyCellAttributes(12, 11, 1, 1, object->x >> 20,
                                 object->z >> 20);
    }
  }
}

/// YampiDesertCave_EndSand's counterpart: the leader's terrain becomes 2, and
/// in rooms 1 and 3 the same calls run with their first and last pairs swapped,
/// each followed by more.
/// Sand: the leader sinks into the sand and the rooms' sand-only cells open.
void YampiDesertCave_CastSand(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 2;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room1) {
    MapGrid_CopyCellAttributes(55, 71, 4, 1, 43, 70);
    MapGrid_CopyCellAttributes(43, 71, 4, 1, 55, 71);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3) {
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
  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 0;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room1)
    MapGrid_CopyCellAttributes(43, 70, 4, 1, 55, 71);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3) {
    MapGrid_CopyCellAttributes(90, 50, 5, 2, 110, 45);
    MapGrid_CopyCellAttributes(90, 52, 2, 1, 113, 47);
    MapGrid_CopyCellAttributes(90, 53, 3, 2, 107, 48);
  }
}

/// Pound sinks Room3's stake and opens its cell.
void YampiDesertCave_PoundStake(s32 a, s32 b) {
  Statue_Sink(gYampiDesertCave_Room3Stakes, a, b);
  MapGrid_CopyCellAttributes(47, 12, 1, 1, 46, 12);
}

/* VIEW(first): routine gYampiDesertCave_RipplePath as this part declared it;
 * the joined file has another. */
extern const s16
    gYampiDesertCave_RipplePath__first[] __asm__("gYampiDesertCave_RipplePath");
#define gYampiDesertCave_RipplePath gYampiDesertCave_RipplePath__first

static __inline__ s32 GameFlag_GetByte_(s32 flag) {
  return ((s32)GameFlag_GetByte(flag));
}

/// Moves the ripple to the next cell of its path, step by step on the step
/// counter in flag byte 0x278.
void YampiDesertCave_MoveSandRipple(void) {
  s32 step = GameFlag_GetByte_(0x278);
  s32 index = (step / 32) * 2;
  s32 column = gYampiDesertCave_RipplePath[index];
  s32 row = gYampiDesertCave_RipplePath[index + 1];

  if (Actor_GetById(16)->movementState == 0) {
    Actor_SetPosition(16, column << 19, row << 19);
    if (Event_GetFlag(0x104) == 0)
      step++;
    GameFlag_SetByte(0x278, step);
  }
}

#include "version.h"

/// The chest that follows the step count: when the tracked object stands on
/// its cell, mode 1 stops the follower and mode 2 drops the chest.
/// Scoop on the ripple's cell stops it (mode 1) and digs up Crystal, who
/// drops to the ground (mode 2).
void YampiDesertCave_ScoopDjinni(s32 mode) {
  s32 index = (((s32)GameFlag_GetByte(0x278)) / 32) * 2;
  s16 column = gYampiDesertCave_RipplePath[index];
  s32 px = column * 8;
  s16 row = gYampiDesertCave_RipplePath[index + 1];
  s32 pz = row * 8;
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *tracked =
      *(struct Actor **)(Ram_HeapSlots->battleFxWork[0] + 0x1930);
  s32 x = 0, z = 0;

  if (tracked != NULL) {
    x = tracked->x >> 20;
    z = tracked->z >> 20;
  }
  if (px / 16 != x || pz / 16 != z)
    return;
  if (mode == 1) {
    Task_Remove(YampiDesertCave_MoveSandRipple);
    Actor_SetPosition(16, 0, 0);
  }
  if (mode == 2) {
    struct Actor *chest = Actor_GetById(17);

    Actor_SetPosition(17, column << 19, row << 19);
    chest->y = 0x200000;
    chest->motionFlags = 3;
    chest->gravity = 0x170a;
    wait(50);
    // Through an s32 and a pointer the 17 is built with a move; as a halfword
    // constant it comes from the pool.
    {
      s16 *counter = (s16 *)(work + 0x160);
      s32 value = 17;

      *counter = value;
    }
#if TLA_PATCH_1
    Event_ClearFlag(0x270);
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
  s32 leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(9, 2);
  Event_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  PaletteFade_StartInterpolation(60);
  Event_Wait(60);
  EventMessage_ShowCentered((s32)MsgIAmThe, 0, 6);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(60);
  Event_Wait(60);
  leader = ((s32)Actor_GetById(FIELD_AT(&gMainRoutineWork, s32, 0x214)));
  if (leader != 0)
    Actor_SetDestination(9, FIELD_AT(leader, s16, 10),
                         FIELD_AT(leader, s16, 18));
  Event_Wait(20);
  /* FAKEMATCH(dead): a once-only block around the first two stores keeps the
   * call's first argument ahead of the scene store's address, where without it
   * the argument follows the store; as plain stores, 2 of 71 instructions
   * differ in tla-en. */
  do {
    FIELD_AT(&gMainRoutineWork, u16, 0x254) = 0x19cd;
    FIELD_AT(&gMainRoutineWork, u8, 0x24b) = 2;
  } while (0);
  FIELD_AT(&gMainRoutineWork, u16, 0x1f6) = (s32)&SceneId_Ov664Room4;
  Field_RequestBattleFromEncounterTable(0x67, 1);
}

void YampiDesertCave_ScoopTreasure(s32 a) {
  YampiDesertCave_Call1920(a, 64, 0x302);
}

const u8 *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room2)
    return gYampiDesertCave_Room2Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3)
    return gYampiDesertCave_Room3Triggers;
  return gYampiDesertCave_Triggers;
}

// Sets up Yampi Desert Cave's three rooms as each one loads: the first
// room's palette cycle and Retreat destination, the third room's background
// priorities, and in each room the objects its flags place.

/* VIEW(first): routine gYampiDesertCave_MarkPaletteSlots as this part declared
 * it; the joined file has another. */
extern u16 gYampiDesertCave_MarkPaletteSlots__first[] __asm__(
    "gYampiDesertCave_MarkPaletteSlots");
#define gYampiDesertCave_MarkPaletteSlots                                      \
  gYampiDesertCave_MarkPaletteSlots__first
/* VIEW(first): routine gYampiDesertCave_MarkColorsLit as this part declared it;
 * the joined file has another. */
extern u16 gYampiDesertCave_MarkColorsLit__first[] __asm__(
    "gYampiDesertCave_MarkColorsLit");
#define gYampiDesertCave_MarkColorsLit gYampiDesertCave_MarkColorsLit__first
/* VIEW(first): routine gYampiDesertCave_MarkColorsDim as this part declared it;
 * the joined file has another. */
extern u16 gYampiDesertCave_MarkColorsDim__first[] __asm__(
    "gYampiDesertCave_MarkColorsDim");
#define gYampiDesertCave_MarkColorsDim gYampiDesertCave_MarkColorsDim__first

void Actor_SetWideSprite(s32 id, s32 wide);

/// Sets up Yampi Desert Cave's three rooms as each one loads: the first room's
/// palette cycle and Retreat destination, the third room's background
/// priorities, and in each room the objects its flags place.
s32 Scene_Initialize(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room1) {
    YampiDesertCave_SetMarkColors(
        gYampiDesertCave_MarkPaletteSlots, gYampiDesertCave_MarkColorsLit,
        gYampiDesertCave_MarkColorsDim, gYampiDesertCave_MarkCycle);
    Actor_DisableById(8);
    Actor_DisableById(9);
    Actor_DisableById(10);
    YampiDesertCave_SetLogCells(8);
    YampiDesertCave_SetLogCells(9);
    YampiDesertCave_SetLogCells(10);
    if (Event_GetFlag(0x300)) {
      Actor_SetPosition(11, 0, 0);
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, 13);
    }
    if (gMainRoutineWork.entrance == 1) {
      if (Event_GetFlag(0xa29)) {
        gMainRoutineWork.retreatScene = (s32)&SceneId_Ov661Room1;
        gMainRoutineWork.retreatEntrance = 1;
      } else if (Event_GetFlag(0xa2a)) {
        gMainRoutineWork.retreatScene = (s32)&SceneId_Ov661Room8;
        gMainRoutineWork.retreatEntrance = 2;
      } else if (Event_GetFlag(0xa2b)) {
        gMainRoutineWork.retreatScene = (s32)&SceneId_Ov661Room9;
        gMainRoutineWork.retreatEntrance = 2;
      }
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room2) {
    if (Event_GetFlag(0x303)) {
      Actor_SetPosition(8, 138 << 18, 216 << 16);
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, 13);
      MapGrid_CopyCellAttributes(95, 3, 3, 3, 98, 13);
    } else {
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, 12);
    }
    if (!Event_GetFlag(0x302))
      Actor_SetWideSprite(64, 1);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3) {
    if ((u16)(gMainRoutineWork.entrance - 3) <= 2) {
      REG_BG1CNT &= ~3;
      REG_BG1CNT |= 1;
      REG_BG2CNT &= ~3;
      REG_BG2CNT |= 3;
      REG_BG3CNT &= ~3;
      REG_BG3CNT |= 3;
    }
    Actor_DisableById(8);
    Actor_DisableById(9);
    Actor_DisableById(10);
    Actor_DisableById(11);
    YampiDesertCave_SetLogCells(8);
    YampiDesertCave_SetLogCells(9);
    YampiDesertCave_SetLogCells(10);
    YampiDesertCave_SetLogCells(11);
    YampiDesertCave_PlaceStakes(gYampiDesertCave_Room3Stakes);
    if (!Event_GetFlag(0x206))
      MapGrid_CopyCellAttributes(48, 12, 1, 1, 46, 12);
    if (Event_GetFlag(0x301))
      Actor_SetPosition(15, 0, 0);
    if (Event_GetFlag(0x304)) {
      Actor_SetPosition(13, 168 << 16, 184 << 16);
      MapGrid_CopyCellAttributes(12, 11, 1, 1, 10, 11);
    } else {
#if TLA_PATCH_1
      struct Actor *actor = Actor_GetById(13);

      MapGrid_CopyCellAttributes(12, 11, 1, 1, actor->x >> 20, actor->z >> 20);
#else
      MapGrid_CopyCellAttributes(12, 11, 1, 1, 8, 10);
#endif
    }
    if (Event_GetFlag(0x305)) {
      Actor_SetPosition(12, 154 << 18, 132 << 17);
      MapGrid_CopyCellAttributes(38, 15, 1, 1, 38, 16);
    } else {
      MapGrid_CopyCellAttributes(38, 15, 1, 1, 39, 16);
    }
    if (!Event_GetFlag(65) && !Event_GetFlag(145)) {
      s32 step = FieldGameFlag_GetByte(0x278);
      s32 placed = Event_GetFlag(0x271);

      if (placed) {
        step = step / 32 * 2;
        Actor_SetPosition(16, 0, 0);
        Actor_SetPosition(17, gYampiDesertCave_RipplePath[step] << 19,
                          gYampiDesertCave_RipplePath[step + 1] << 19);
      } else {
        if (Event_GetFlag(0x109))
          GameFlag_SetByte(0x278, 0);
        Task_Add(YampiDesertCave_MoveSandRipple, 0x480);
        Event_SetFlag(0x270);
        Actor_SetSpritePriority(16, 3);
        Actor_GetById(16)->collisionFlags = 0;
      }
    }
  }
  FieldEffect_StartBlankOverlay(0);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#undef PIXELS
#define PIXELS(value) ((value) << 16)
#undef DEFINE_TELEPORT_FROM_MARK
#undef DEFINE_BURST_PILLAR
#undef FIELD_AT
#undef gYampiDesertCave_LogFootprints
#undef gYampiDesertCave_Placements
#undef gYampiDesertCave_Room2Placements
#undef gYampiDesertCave_Room3Placements
#undef gYampiDesertCave_Room3Stakes
#undef gYampiDesertCave_Triggers
#undef gYampiDesertCave_Room2Triggers
#undef gYampiDesertCave_Room3Triggers
#undef gYampiDesertCave_RipplePath
#undef gYampiDesertCave_MarkPaletteSlots
#undef gYampiDesertCave_MarkColorsLit
#undef gYampiDesertCave_MarkColorsDim
// ---- from scene/data/yampi_desert_cave.c ----
#include "shared/tables.h"
#include "types.h"
#include "scene_tables.h"

/* VIEW(data): routine YampiDesertCave_BurstRoom1Pillar as this part declared
 * it; the joined file has another. */
void YampiDesertCave_BurstRoom1Pillar__data(void) __asm__(
    "YampiDesertCave_BurstRoom1Pillar");
#define YampiDesertCave_BurstRoom1Pillar YampiDesertCave_BurstRoom1Pillar__data
/* VIEW(data): routine YampiDesertCave_BurstRoom3Pillar as this part declared
 * it; the joined file has another. */
void YampiDesertCave_BurstRoom3Pillar__data(void) __asm__(
    "YampiDesertCave_BurstRoom3Pillar");
#define YampiDesertCave_BurstRoom3Pillar YampiDesertCave_BurstRoom3Pillar__data
/* VIEW(data): routine YampiDesertCave_StartMovingPillar as this part declared
 * it; the joined file has another. */
void YampiDesertCave_StartMovingPillar__data(void) __asm__(
    "YampiDesertCave_StartMovingPillar");
#define YampiDesertCave_StartMovingPillar                                      \
  YampiDesertCave_StartMovingPillar__data
/* VIEW(data): routine YampiDesertCave_FinishMovingPillar as this part declared
 * it; the joined file has another. */
void YampiDesertCave_FinishMovingPillar__data(void) __asm__(
    "YampiDesertCave_FinishMovingPillar");
#define YampiDesertCave_FinishMovingPillar                                     \
  YampiDesertCave_FinishMovingPillar__data
/* VIEW(data): routine YampiDesertCave_PoundStake as this part declared it; the
 * joined file has another. */
void YampiDesertCave_PoundStake__data(void) __asm__(
    "YampiDesertCave_PoundStake");
#define YampiDesertCave_PoundStake YampiDesertCave_PoundStake__data
/* VIEW(data): routine YampiDesertCave_ScoopDjinni as this part declared it; the
 * joined file has another. */
void YampiDesertCave_ScoopDjinni__data(void) __asm__(
    "YampiDesertCave_ScoopDjinni");
#define YampiDesertCave_ScoopDjinni YampiDesertCave_ScoopDjinni__data
void YampiDesertCave_MeetGuardianOfFire(void);
/* VIEW(data): routine YampiDesertCave_ScoopTreasure as this part declared it;
 * the joined file has another. */
void YampiDesertCave_ScoopTreasure__data(void) __asm__(
    "YampiDesertCave_ScoopTreasure");
#define YampiDesertCave_ScoopTreasure YampiDesertCave_ScoopTreasure__data

// clang-format off
u16 gYampiDesertCave_LogSprites[] = {
    258, 259, 293, 294, 332, 331,
};

u32 gYampiDesertCave_LogFootprints[] = {
    0x820f8e0, 0x2008e0f8, 0x20f0e0, 0x2008e0f8, 0x820f8e0, 0x2008e0f8,
};

DEFINE_PUSH_LEADER_STEPS(gPushLeaderSteps)

u32 gYampiDesertCave_RipplePath[] = {
    0x270065, 0, 0x230061, 0, 0x250063, 0,
    0x21005f, 0,
};

s16 gYampiDesertCave_MarkPaletteSlots[] = {
    9, 10, 11, 12, 13, 14, 15, -1,
};

s16 gYampiDesertCave_MarkColorsLit[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gYampiDesertCave_MarkColorsDim[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

DEFINE_G_DATA_020013B4(gYampiDesertCave_MarkCycle, 14, 9, 10, 11, 12, 13)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(79),
    SCENE_EXIT(1, 74, 4),
    SCENE_EXIT(2, 81, 1),
    SCENE_EXIT(3, 79, 6),
    SCENE_EXIT(4, 79, 5),
    SCENE_EXIT(5, 79, 4),
    SCENE_EXIT(6, 79, 3),
    SCENE_EXIT(7, 81, 8),
    SCENE_EXIT(8, 80, 1),
    SCENE_EXITS(80),
    SCENE_EXIT(1, 79, 8),
    SCENE_EXIT(2, 80, 17),
    SCENE_EXIT(14, 81, 13),
    SCENE_EXIT(15, 81, 12),
    SCENE_EXIT(16, 81, 11),
    SCENE_EXIT(17, 80, 2),
    SCENE_EXITS(81),
    SCENE_EXIT(1, 79, 2),
    SCENE_EXIT(2, 81, 3),
    SCENE_EXIT(3, 81, 2),
    SCENE_EXIT(4, 81, 7),
    SCENE_EXIT(5, 81, 6),
    SCENE_EXIT(6, 81, 5),
    SCENE_EXIT(7, 81, 4),
    SCENE_EXIT(8, 79, 7),
    SCENE_EXIT(9, 81, 10),
    SCENE_EXIT(10, 81, 9),
    SCENE_EXIT(11, 80, 16),
    SCENE_EXIT(12, 80, 15),
    SCENE_EXIT(13, 80, 14),
    SCENE_EXITS_END,
};

u32 gYampiDesertCave_Settle[] = {
    22, 26, 0, 17,
};

struct ActorPlacement gYampiDesertCave_Placements[] = {
    {331, CONDITION_ALWAYS, 7, PIXELS(888), 0, PIXELS(336), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 7, PIXELS(832), 0, PIXELS(312), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 7, PIXELS(912), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {404, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(712), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct ActorPlacement gYampiDesertCave_Room2Placements[] = {
    {272, CONDITION_ALWAYS, (s32)gYampiDesertCave_Settle, PIXELS(552), 0,
     PIXELS(200), FACING_SOUTH, 2, 0},
    {177, 0x9cd, 1, PIXELS(200), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gYampiDesertCave_Room3Placements[] = {
    {331, CONDITION_ALWAYS, 7, PIXELS(712), 0, PIXELS(752), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 7, PIXELS(752), 0, PIXELS(744), FACING_SOUTH, 2, 0},
    {331, CONDITION_ALWAYS, 7, PIXELS(728), 0, PIXELS(784), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 7, PIXELS(800), 0, PIXELS(760), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, (s32)gYampiDesertCave_Settle, PIXELS(632), 0,
     PIXELS(264), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, (s32)gYampiDesertCave_Settle, PIXELS(136), 0,
     PIXELS(168), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gYampiDesertCave_Settle, PIXELS(744), 0,
     PIXELS(200), FACING_SOUTH, 2, 0},
    {404, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(632), FACING_SOUTH, 2, 0},
    {251, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {243, 0x41, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

u32 gYampiDesertCave_Room3Stakes[] = {
    14,
    0xffff0206,
};

struct Trigger gYampiDesertCave_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {49, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x202, 42, CONDITION_ALWAYS, EVENT(YampiDesertCave_TryPushLog)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 33, 0x300,
     EVENT(YampiDesertCave_BurstRoom1Pillar)},
    {0x9c05, 30, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_TeleportFromFirstMark)},
    {0x9c05, 31, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_TeleportFromSecondMark)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(YampiDesertCave_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_EndSand)},
    {TRIGGER_TABLE_END},
};

struct Trigger gYampiDesertCave_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {33, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_STEP, 52, 0x9cd, EVENT(YampiDesertCave_MeetGuardianOfFire)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     0x303, EVENT(YampiDesertCave_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x303,
     EVENT(YampiDesertCave_FinishMovingPillar)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(YampiDesertCave_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_EndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 100, 0x302,
     EVENT(YampiDesertCave_ScoopTreasure)},
    {TRIGGER_TABLE_END},
};

struct Trigger gYampiDesertCave_Room3Triggers[] = {
    {33, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {33, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(YampiDesertCave_WonderAtRipple)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(YampiDesertCave_FightCrystal)},
    {0x202, 42, CONDITION_ALWAYS, EVENT(YampiDesertCave_TryPushLog)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 33, 0x301,
     EVENT(YampiDesertCave_BurstRoom3Pillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12,
     0x305, EVENT(YampiDesertCave_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x305,
     EVENT(YampiDesertCave_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13,
     0x304, EVENT(YampiDesertCave_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, 0x304,
     EVENT(YampiDesertCave_FinishMovingPillar)},
    {8, 0, CONDITION_ALWAYS, EVENT(YampiDesertCave_StartMovingPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(YampiDesertCave_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 14, 0x206,
     EVENT(YampiDesertCave_PoundStake)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(YampiDesertCave_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_EndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 33,
     IF_FLAG_SET(0x270), EVENT(YampiDesertCave_ScoopDjinni)},
    {TRIGGER_TABLE_END},
};
#undef YampiDesertCave_BurstRoom1Pillar
#undef YampiDesertCave_BurstRoom3Pillar
#undef YampiDesertCave_StartMovingPillar
#undef YampiDesertCave_FinishMovingPillar
#undef YampiDesertCave_PoundStake
#undef YampiDesertCave_ScoopDjinni
#undef YampiDesertCave_ScoopTreasure
