#include "unit/field/field.h"
#include "battle.h"
#include "engine_call.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "heap_slots.h"
#include "map_grid_copy.h"
#include "pushed_object.h"
#include "fixed_point_position.h"
#include "iwram_call.h"
#include "call.h"
#include "map_header.h"
#include "map_grid.h"
#include "map_grid_scroll.h"
#include "script_layer.h"
#include "types.h"
#include "songs.h"

// Seventeen maps with things the leader pushes (Altmiller Cave, Babi
// Lighthouse, Crossbone Isle and its rooms, Goma Cave's pillar room, Gondowan
// Cave, Kolima Forest (MapId_KolimaForestEntrance to
// MapId_KolimaForestCliff), Lunpa's fortress, Mercury Lighthouse and its
// entrance, Mogall Forest, the Tunnel Ruins, Vale and its cave): things
// pushed a cell at a time, such as stone pillars, and logs and other large
// things that roll until they meet something, redrawn into the map grid where
// they stop.
//
// A large thing is one of six footprints, by its sprite's animation
// (gPushedObject_FootprintKinds): its bounds in
// gPushedObject_FootprintBounds are four words a footprint, the left, top,
// right and bottom edges in pixels from its position.

/// Pixel step by direction: X in the upper halfword, Z in the lower.
extern s32 gPushedObject_DirectionSteps[];
extern s32 gPushedObject_FootprintBounds[];

#include "object_distance.h"

/// Returns the placed object event on the cell of the 16.16 position and at its
/// height (in whole pixels), or 0. origin is not read.
static struct ObjectEvent *
PushedObject_FindAtTile(const struct FieldPosition *position,
                        const struct ObjectEvent *origin) {
  struct FieldWork *work = (struct FieldWork *)gFieldWork;
  struct ObjectEvent **slots = work->objectEvents;
  u32 i;

  for (i = LOCALID_FIRST_PLACED;
       i < sizeof(work->objectEvents) / sizeof(work->objectEvents[0]); i++) {
    struct ObjectEvent *objectEvent = (struct ObjectEvent *)slots[i];

    // UB: ObjectEvent_ClearAll leaves unfilled slots null. The guard grows
    // this helper from 0x58 to 0x5c bytes in the native English trial.
#ifdef UBFIX
    if (objectEvent == NULL)
      continue;
#endif
    if ((position->x >> 20) == (objectEvent->x.fixed >> 20) &&
        (position->y / 0x10000) == (objectEvent->y.fixed / 0x10000) &&
        (position->z >> 20) == (objectEvent->z.fixed >> 20)) {
      return objectEvent;
    }
  }
  return 0;
}

/// The leader pushes what stands in front of them a cell on, unless
/// something solid is beyond it or on top of it, the ground beyond refuses it
/// or it is still moving: after a quarter second's push (animation 8) it and
/// the leader slide one cell together, with SE_PUSH.
UNUSED static void PushedObject_AdvancePair(void) {
  struct FieldPosition dst;
  struct ObjectEvent *lead;
  struct ObjectEvent *next;
  struct ObjectEvent *blocker;
  s32 dir;
  u32 step;
  s32 rate;

  lead = ObjectEvent_GetById(0);
  dir = lead->facing >> 12;
  step = gPushedObject_DirectionSteps[dir];
  dst.x = lead->x.fixed + (step & 0xffff0000);
  dst.y = lead->y.fixed;
  step <<= 16;
  dst.z = lead->z.fixed + step;
  next = PushedObject_FindAtTile(&dst, lead);
  if (next == 0)
    return;

  step = gPushedObject_DirectionSteps[dir];
  dst.x = next->x.fixed + (step & 0xffff0000);
  dst.y = next->y.fixed;
  step <<= 16;
  dst.z = next->z.fixed + step;
  blocker = PushedObject_FindAtTile(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  dst.x = next->x.fixed;
  dst.y = next->y.fixed + 0x100000;
  dst.z = next->z.fixed;
  blocker = PushedObject_FindAtTile(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  next->layer = 2;
  step = gPushedObject_DirectionSteps[dir];
  dst.x = next->x.fixed + (step & 0xffff0000);
  dst.y = next->y.fixed;
  step <<= 16;
  dst.z = next->z.fixed + step;
  if (FieldObject_CheckMovementCollision(next, &dst) > 0)
    return;

  if (next->riseCounter != 0)
    return;

  ObjectEvent_SetMode(lead, 8);
  rate = 0x3333;
  Engine_TaskWait(15);
  Engine_AudioPlayCue(SE_PUSH);
  next->speed = rate;
  next->acceleration = rate;
  Engine_ObjectSetPosition(next, dst.x, dst.y, dst.z);
  lead->speed = rate;
  lead->acceleration = rate;
  Engine_ObjectSetPosition(lead, dst.x, dst.y, dst.z);
  Object_WaitForMove(next);
  Map_PlayQueuedMusic();
  next->x.fixed = dst.x;
  next->z.fixed = dst.z;
  next->velocityX = 0;
  next->velocityZ = 0;
  lead->targetX = 0x80000000;
  lead->targetZ = 0x80000000;
  lead->velocityX = 0;
  lead->velocityZ = 0;
  lead->x.fixed = lead->x.part.pixel << 16;
  lead->z.fixed = lead->z.part.pixel << 16;
  ObjectEvent_SetMode(lead, 1);
}

/// Sets the metatile behavior of a width by height rectangle of cells, from
/// cell (x, z), to value, on map grid layer 0 to 2, or (3 and up) in
/// gMapGridCellBuffer.
static s32 PushedObject_FillGridAttributeRectangle(u32 layer, s32 x, s32 z,
                                                   u32 width, u32 height,
                                                   s32 value) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  struct MapGridCell *cells;
  u32 row;
  u32 column;

  if (grid != 0) {
    if (layer <= 2) {
      cells = grid->layers[layer].cells;
    } else {
      cells = (struct MapGridCell *)gMapGridCellBuffer;
    }
    cells += x + (z << 7);
    for (row = 0; row < height; row++) {
      struct MapGridCell *cell = cells + (row << 7);

      for (column = 0; column < width; column++) {
        cell->behavior = value;
        cell++;
      }
    }
  }
  return 0;
}

// UB: the native s32 callback is called through ObjectEvent's void hook. The
// void form keeps 0xa4 bytes in Altmiller English but changes the return move.
#ifdef UBFIX
/// The leader's movement callback while pushing a large thing: stop the leader
/// when another object event that is not a large thing, or a wall, is in the
/// way.
static void PushedObject_StopBlockedMotion(union FieldObject *fieldObject) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
#else
/// The leader's movement callback while pushing a large thing: stop the leader
/// when another object event that is not a large thing, or a wall, is in the
/// way.
static s32 PushedObject_StopBlockedMotion(struct ObjectEvent *objectEvent) {
#endif
  struct FieldPosition probe;
  u32 dir;
  s32 step;
  struct ObjectEvent *target;

  dir = objectEvent->facing >> 12;
  step = gPushedObject_DirectionSteps[dir];
  probe.x = objectEvent->x.fixed + (step & 0xffff0000);
  probe.y = objectEvent->y.fixed;
  step <<= 16;
  probe.z = objectEvent->z.fixed + step;
  target = PushedObject_FindAtTile(&probe, objectEvent);
  if (target != 0) {
    u32 i = 0;
    s32 kind = target->sprite->entries[0]->animationId;
    s32 *kinds = gPushedObject_FootprintKinds;

    do {
      if (kind == *kinds++) {
#ifdef UBFIX
        return;
#else
        return 0;
#endif
      }
      i++;
    } while (i <= 5);
    objectEvent->velocityX = 0;
    objectEvent->velocityZ = 0;
    objectEvent->targetX = 0x80000000;
    objectEvent->targetZ = 0x80000000;
  }
  step = gPushedObject_DirectionSteps[dir];
  probe.x = objectEvent->x.fixed + (step & 0xffff0000);
  probe.y = objectEvent->y.fixed;
  step <<= 16;
  probe.z = objectEvent->z.fixed + step;
  if (FieldObject_CheckMovementCollision(objectEvent, &probe) > 0) {
    objectEvent->velocityX = 0;
    objectEvent->velocityZ = 0;
    objectEvent->targetX = 0x80000000;
    objectEvent->targetZ = 0x80000000;
  }
#ifndef UBFIX
  return 0;
#endif
}

/// Returns the large thing whose footprint covers the cell ahead of the leader
/// (the leader not standing in line with its first column or row), with the
/// leader's direction, its local id and its footprint; or NULL.
static struct ObjectEvent *Map_FindObjectEventRegion(s32 *direction, s32 *slot,
                                                     s32 *footprint) {
  struct FieldWork *work = (struct FieldWork *)gFieldWork;
  struct ObjectEvent *player = ObjectEvent_GetById(0);
  struct ObjectEvent **list;
  u32 i, j;

  *direction = player->facing >> 12;
  list = work->objectEvents + LOCALID_FIRST_PLACED;
  for (i = LOCALID_FIRST_PLACED;
       i < sizeof(work->objectEvents) / sizeof(work->objectEvents[0]);
       list++, i++) {
    struct ObjectEvent *objectEvent = (struct ObjectEvent *)*list;
    s32 kind;
    j = 0;
    // UB: the same empty slots are scanned here; the null guard grows the
    // native English helper from 0x128 to 0x134 bytes.
#ifdef UBFIX
    if (objectEvent == NULL)
      continue;
#endif
    kind = objectEvent->sprite->entries[0]->animationId;
    for (j = 0; j < 6; j++) {
      if (kind == gPushedObject_FootprintKinds[j]) {
        s32 x, z, left, top, right, bottom;
        *footprint = j;
        x = ((player->x.fixed >> 16) +
             (gPushedObject_DirectionSteps[*direction] >> 16)) >>
            4;
        z = ((player->z.fixed >> 16) +
             (s16)gPushedObject_DirectionSteps[*direction]) >>
            4;
        left = (objectEvent->x.part.pixel +
                gPushedObject_FootprintBounds[j * 4 + 0]) >>
               4;
        top = (objectEvent->z.part.pixel +
               gPushedObject_FootprintBounds[j * 4 + 1]) >>
              4;
        right = (objectEvent->x.part.pixel +
                 gPushedObject_FootprintBounds[j * 4 + 2]) >>
                4;
        bottom = (objectEvent->z.part.pixel +
                  gPushedObject_FootprintBounds[j * 4 + 3]) >>
                 4;
        if (left <= x && x < right && top <= z && z < bottom) {
          if (j & 1) {
            if (left != (player->x.fixed >> 20)) {
              *slot = i;
              return objectEvent;
            }
          } else if (top != (player->z.fixed >> 20)) {
            *slot = i;
            return objectEvent;
          }
        }
      }
    }
  }
  return NULL;
}

/// Finds where the large thing ahead of the leader would roll to: step its
/// footprint on, a cell at a time the way the leader faces, until a cell of it
/// meets something (a collision result of 2). Fills probe with the thing, its
/// footprint and that position; returns 0 when there is no thing or it cannot
/// move.
UNUSED static s32
PushedObject_FindClearPosition(struct PushedObjectProbe *probe) {
  s32 direction;
  struct FieldPosition position;
  struct ObjectEvent *objectEvent;
  s32 steps;
  s32 width, height;
  u8 *mode;
  s32 column, row;
  s32 nearEdge, farEdge;
  s32 offset;

  probe->callback = 0;
  objectEvent = Map_FindObjectEventRegion(&direction, &probe->objectEventSlot,
                                          &probe->footprintIndex);
  if (objectEvent == 0) {
    return 0;
  }
  mode = &objectEvent->layer;
  *mode = 2;
  {
    s32 footprint = probe->footprintIndex;

    steps = 0;
    offset = footprint * 4;
    nearEdge = gPushedObject_FootprintBounds[offset + 1];
    if (nearEdge < 0) {
      nearEdge = -nearEdge;
    }
    farEdge = gPushedObject_FootprintBounds[offset + 3];
    if (farEdge < 0) {
      farEdge = -farEdge;
    }
    height = (nearEdge + farEdge) >> 4;
    nearEdge = gPushedObject_FootprintBounds[offset];
    if (nearEdge < 0) {
      nearEdge = -nearEdge;
    }
    farEdge = gPushedObject_FootprintBounds[offset + 2];
    if (farEdge < 0) {
      farEdge = -farEdge;
    }
    width = (nearEdge + farEdge) >> 4;
  }
  position.x = objectEvent->x.fixed +
               (gPushedObject_DirectionSteps[direction] & 0xffff0000);
  {
    s32 y = objectEvent->y.fixed;

    position.y = y;
    position.z =
        objectEvent->z.fixed + (gPushedObject_DirectionSteps[direction] << 16);
    probe->position.y = y;
  }
  for (;;) {
    probe->position.z =
        position.z +
        (gPushedObject_FootprintBounds[probe->footprintIndex * 4 + 1] << 16);
    for (row = 0; row < height; row++) {
      probe->position.x =
          position.x +
          (gPushedObject_FootprintBounds[probe->footprintIndex * 4] << 16);
      for (column = 0; column < width; column++) {
        if (FieldObject_CheckMovementCollision(objectEvent, &probe->position) ==
            2) {
          goto found;
        }
        probe->position.x += 0x100000;
      }
      probe->position.z += 0x100000;
    }
    steps++;
    position.x += gPushedObject_DirectionSteps[direction] & 0xffff0000;
    position.z += gPushedObject_DirectionSteps[direction] << 16;
  }
found:
  *mode = 0;
  if (steps == 0) {
    return 0;
  }
  probe->position.x =
      objectEvent->x.fixed +
      (gPushedObject_DirectionSteps[direction] & 0xffff0000) * steps;
  probe->position.y = objectEvent->y.fixed;
  probe->position.z = objectEvent->z.fixed +
                      (gPushedObject_DirectionSteps[direction] << 16) * steps;
  return 1;
}

/// Rolls the large thing to the position probe found: the leader pushes
/// (animation 8), the thing rolls (animation 3 or 2 by the direction) with
/// SE_HEAVY_MOVE while the leader follows half a cell, and it stops with
/// SE_STOP_SLOT3 and SE_HEAVY_STOP. Its cells are blocked (metatile behavior
/// 255) at its new place, through the map grid's layer 1 offset, and its old
/// place is given back what was under it.
UNUSED static void ObjectEvent_MoveAndRedraw(struct PushedObjectProbe probe) {
  struct MapGridScrollWork *workspace;
  struct ObjectEvent *objectEvent;
  struct FieldPosition originalPosition;
  struct FieldPosition tilePosition;
  u8 *footprintTable;
  s32 direction;
  s32 horizontalExtent;
  s32 verticalExtent;

  // UB: reading the pointer slot through u32 aliases its declared type. The
  // direct pointer read moves ahead of the probe stores in native English.
#ifdef UBFIX
  workspace = gMapGridWork[0];
#else
  workspace = (struct MapGridScrollWork *)*(u32 *)gMapGridWork;
#endif
  direction = (ObjectEvent_GetById(0))->facing >> 12;
  objectEvent = ObjectEvent_GetById(probe.objectEventSlot);
  footprintTable = (u8 *)gPushedObject_FootprintBounds;
  {
    s32 extentA = gPushedObject_FootprintBounds[probe.footprintIndex * 4 + 1];
    s32 extentB;

    if (extentA < 0)
      extentA = -extentA;
    extentB = gPushedObject_FootprintBounds[probe.footprintIndex * 4 + 3];
    if (extentB < 0)
      extentB = -extentB;
    verticalExtent = (extentA + extentB) >> 4;
    extentA = gPushedObject_FootprintBounds[probe.footprintIndex * 4 + 0];
    if (extentA < 0)
      extentA = -extentA;
    extentB = gPushedObject_FootprintBounds[probe.footprintIndex * 4 + 2];
    if (extentB < 0)
      extentB = -extentB;
    horizontalExtent = (extentA + extentB) >> 4;
  }

  objectEvent->speed = 0x8000;
  objectEvent->acceleration = 0x1999;
  originalPosition.x = objectEvent->x.fixed;
  originalPosition.z = objectEvent->z.fixed;
  {
    s32 tableOffset = probe.footprintIndex << 4;
    tilePosition.x =
        (objectEvent->x.fixed + (*(s32 *)(footprintTable + tableOffset) << 16));
    tableOffset += 4;
    tilePosition.z =
        (objectEvent->z.fixed + (*(s32 *)(footprintTable + tableOffset) << 16));
    tilePosition.x >>= 20;
    tilePosition.z >>= 20;
  }

  PushedObject_FillGridAttributeRectangle(0, tilePosition.x, tilePosition.z,
                                          horizontalExtent, verticalExtent, 0);
  Engine_ObjectEventSetSpeed(0, 0x8000, 0x1999);
  Engine_ObjectEventSetAnimation(0, ANIM_PUSH);
  Engine_ScriptWait(15);

  {
    s32 horizontalDelta = probe.position.x - originalPosition.x;
    if (horizontalDelta < 0)
      horizontalDelta += 0x1ffff;
    horizontalDelta >>= 17;
    {
      s32 verticalDelta = probe.position.z - originalPosition.z;
      if (verticalDelta < 0)
        verticalDelta += 0x1ffff;
      verticalDelta >>= 17;
      Engine_ObjectEventSetDestinationOffset(0, horizontalDelta, verticalDelta);
    }
  }

#ifdef UBFIX
  ObjectEvent_GetById(0)->update = PushedObject_StopBlockedMotion;
#else
  ObjectEvent_GetById(0)->update =
      (void (*)(union FieldObject *))PushedObject_StopBlockedMotion;
#endif
  Engine_ScriptWait(4);
  if ((u32)(direction - 6) <= 7)
    ObjectEvent_SetMode(objectEvent, 3);
  else
    ObjectEvent_SetMode(objectEvent, 2);

  Engine_AudioPlayCue(SE_HEAVY_MOVE);
  Engine_ObjectSetPosition(objectEvent, probe.position.x, probe.position.y,
                           probe.position.z);
  Engine_ObjectEventWaitForMove(0);
  Engine_ObjectEventSetAnimation(0, 2);
  ObjectEvent_SetSpeed(0, 0x4ccc, 0x1999);

  {
    s32 packedOrientation = gPushedObject_DirectionSteps[direction];
    Engine_ObjectEventSetDestinationOffset(
        0, ((s16)(packedOrientation >> 16)) / 2, ((s16)packedOrientation) / 2);
  }
  if (probe.callback != 0)
    probe.callback();

  Engine_ObjectEventWaitForMove(0);
  Engine_ObjectEventSetAnimation(0, 1);
  ObjectEvent_GetById(0)->update = NULL;
  Object_WaitForMove(objectEvent);
  Engine_AudioPlayCue(SE_STOP_SLOT3);
  Engine_AudioPlayCue(SE_HEAVY_STOP);

  objectEvent->x.fixed = probe.position.x;
  objectEvent->z.fixed = probe.position.z;
  objectEvent->velocityX = 0;
  objectEvent->velocityZ = 0;
  ObjectEvent_SetMode(objectEvent, 1);

  {
    u8 *table = (u8 *)gPushedObject_FootprintBounds;
    s32 horizontalOrigin;
    s32 verticalOrigin;

    {
      s32 tableOffset = probe.footprintIndex << 4;
      probe.position.x =
          (probe.position.x + (*(s32 *)(table + tableOffset) << 16));
      tableOffset += 4;
      probe.position.z =
          (probe.position.z + (*(s32 *)(table + tableOffset) << 16));
      probe.position.x >>= 20;
      probe.position.z >>= 20;
    }
    horizontalOrigin = workspace->layers[1].offsetX >> 20;
    verticalOrigin = workspace->layers[1].offsetY >> 20;

    MapGrid_CopyCellAttributes(
        probe.position.x, probe.position.z, horizontalExtent, verticalExtent,
        horizontalOrigin + probe.position.x, verticalOrigin + probe.position.z);
    PushedObject_FillGridAttributeRectangle(0, probe.position.x,
                                            probe.position.z, horizontalExtent,
                                            verticalExtent, 255);
    PushedObject_FillGridAttributeRectangle(2, probe.position.x,
                                            probe.position.z, horizontalExtent,
                                            verticalExtent, 255);

    {
      s32 tableOffset = probe.footprintIndex << 4;
      originalPosition.x =
          (originalPosition.x + (*(s32 *)(table + tableOffset) << 16));
      tableOffset += 4;
      originalPosition.z =
          (originalPosition.z + (*(s32 *)(table + tableOffset) << 16));
      originalPosition.x >>= 20;
      originalPosition.z >>= 20;
    }
    horizontalOrigin += originalPosition.x;
    verticalOrigin += originalPosition.z;

    Engine_MapGridCopyCellAttributes(horizontalOrigin, verticalOrigin,
                                     horizontalExtent, verticalExtent,
                                     originalPosition.x, originalPosition.z);
    PushedObject_FillGridAttributeRectangle(
        2, originalPosition.x, originalPosition.z, horizontalExtent,
        verticalExtent, 0);
  }
  Map_PlayQueuedMusic();
}

/// Block large thing id's cells (metatile behavior 255) where it stands,
/// through the map grid's layer 1 offset; returns 0 when its sprite is no large
/// thing.
UNUSED static s32 Map_RedrawObjectEventFootprint(s32 id) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(id);
  struct PushedObjectProbe probe;
  u32 idx = 0;
  s32 offset;
  s32 width;
  s32 height;
  s32 x0;
  s32 x1;
  s32 z0;
  s32 z1;
  s32 dstZ;

  for (idx = 0; idx <= 5; idx++) {
    if (objectEvent->sprite->entries[0]->animationId ==
        gPushedObject_FootprintKinds[idx]) {
      probe.footprintIndex = idx;
      break;
    }
    probe.footprintIndex = 7;
  }

  if ((u32)probe.footprintIndex > 6)
    return 0;
  probe.position.x = objectEvent->x.fixed;
  probe.position.y = objectEvent->y.fixed;
  probe.position.z = objectEvent->z.fixed;
  offset = probe.footprintIndex * 4;
  z0 = gPushedObject_FootprintBounds[offset + 1];
  if (z0 < 0)
    z0 = -z0;
  z1 = gPushedObject_FootprintBounds[offset + 3];
  if (z1 < 0)
    z1 = -z1;
  height = (z0 + z1) >> 4;
  x0 = gPushedObject_FootprintBounds[offset];
  if (x0 < 0)
    x0 = -x0;
  x1 = gPushedObject_FootprintBounds[offset + 2];
  if (x1 < 0)
    x1 = -x1;
  width = (x0 + x1) >> 4;
  probe.position.x += gPushedObject_FootprintBounds[offset] << 16;
  probe.position.z += gPushedObject_FootprintBounds[offset + 1] << 16;
  probe.position.x >>= 20;
  probe.position.z >>= 20;
  idx = grid->layers[1].offsetX >> 20;
  dstZ = grid->layers[1].offsetY >> 20;
  Engine_MapGridCopyCellAttributes(probe.position.x, probe.position.z, width,
                                   height, idx + probe.position.x,
                                   dstZ + probe.position.z);
  PushedObject_FillGridAttributeRectangle(0, probe.position.x, probe.position.z,
                                          width, height, 255);
  PushedObject_FillGridAttributeRectangle(2, probe.position.x, probe.position.z,
                                          width, height, 255);
  return 1;
}
