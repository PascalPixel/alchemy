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
#include "global_cells.h"
#include "mapcopy.h"
#include "staged_actor.h"
#include "fixed_point_position.h"
#include "iwram_call.h"
#include "call.h"
#include "field_runtime.h"
#include "map.h"
#include "map_scroll.h"
#include "field_event.h"
#include "types.h"
#include "songs.h"

// Seventeen scenes with things the leader pushes (Altmiller Cave, Babi
// Lighthouse, Crossbone Isle and its rooms, Goma Cave's pillar room, Gondowan
// Cave, Kolima Forest (SceneId_KolimaForestEntrance to
// SceneId_KolimaForestCliff), Lunpa's fortress, Mercury Lighthouse and its
// entrance, Mogall Forest, the Tunnel Ruins, Vale and its cave): blocks pushed
// a cell at a time, and logs and other large things that roll until they meet
// something, redrawn into the map where they stop.
//
// A large thing is one of six footprints, by its sprite's animation
// (gStagedActor_FootprintKinds): its bounds in gStagedActor_FootprintBounds
// are four words a footprint, the left, top, right and bottom edges in pixels
// from its position.

/// Pixel step by direction: X in the upper halfword, Z in the lower.
extern s32 gStagedActor_DirectionSteps[];
extern s32 gStagedActor_FootprintBounds[];

#include "object_distance.h"

/// Returns the placed actor on the cell of the 16.16 position and at its height
/// (in whole pixels), or 0. origin is not read.
static struct FieldActor *
StagedActor_FindAtTile(const struct FieldPosition *position,
                       const struct FieldActor *origin) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  struct FieldActor **slots = work->actors;
  u32 i;

  for (i = ACTOR_FIRST_PLACED;
       i < sizeof(work->actors) / sizeof(work->actors[0]); i++) {
    struct FieldActor *actor = (struct FieldActor *)slots[i];

    // UB: ObjectTable_ClearAll leaves unfilled slots null. The guard grows
    // this helper from 0x58 to 0x5c bytes in the native English trial.
#ifdef UBFIX
    if (actor == NULL)
      continue;
#endif
    if ((position->x >> 20) == (actor->x.fixed >> 20) &&
        (position->y / 0x10000) == (actor->y.fixed / 0x10000) &&
        (position->z >> 20) == (actor->z.fixed >> 20)) {
      return actor;
    }
  }
  return 0;
}

/// The leader pushes the block in front of them a cell on, unless something
/// solid is beyond it or on top of it, the ground beyond refuses it or it is
/// still moving: after a quarter second's push (animation 8) the block and the
/// leader slide one cell together, with SE_185.
UNUSED static void StagedActor_AdvancePair(void) {
  struct FieldPosition dst;
  struct FieldActor *lead;
  struct FieldActor *next;
  struct FieldActor *blocker;
  s32 dir;
  u32 step;
  s32 rate;

  lead = Object_GetById(0);
  dir = lead->facing >> 12;
  step = gStagedActor_DirectionSteps[dir];
  dst.x = lead->x.fixed + (step & 0xffff0000);
  dst.y = lead->y.fixed;
  step <<= 16;
  dst.z = lead->z.fixed + step;
  next = StagedActor_FindAtTile(&dst, lead);
  if (next == 0)
    return;

  step = gStagedActor_DirectionSteps[dir];
  dst.x = next->x.fixed + (step & 0xffff0000);
  dst.y = next->y.fixed;
  step <<= 16;
  dst.z = next->z.fixed + step;
  blocker = StagedActor_FindAtTile(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  dst.x = next->x.fixed;
  dst.y = next->y.fixed + 0x100000;
  dst.z = next->z.fixed;
  blocker = StagedActor_FindAtTile(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  next->terrainId = 2;
  step = gStagedActor_DirectionSteps[dir];
  dst.x = next->x.fixed + (step & 0xffff0000);
  dst.y = next->y.fixed;
  step <<= 16;
  dst.z = next->z.fixed + step;
  if (FieldObject_CheckMovementCollision(next, &dst) > 0)
    return;

  if (next->riseCounter != 0)
    return;

  Object_SetMode(lead, 8);
  rate = 0x3333;
  Engine_TaskWait(15);
  Engine_AudioPlayCue(SE_185);
  next->speed = rate;
  next->acceleration = rate;
  Engine_ObjectSetPosition(next, dst.x, dst.y, dst.z);
  lead->speed = rate;
  lead->acceleration = rate;
  Engine_ObjectSetPosition(lead, dst.x, dst.y, dst.z);
  FieldObject_WaitForMove(next);
  BattleFx_PlayQueuedSound();
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
  Object_SetMode(lead, 1);
}

/// Sets the collision code of a width by height rectangle of cells, from cell
/// (x, z), to value, on map layer 0 to 2, or (3 and up) in gMapCellBuffer.
static s32 StagedActor_FillGridAttributeRectangle(u32 layer, s32 x, s32 z,
                                                  u32 width, u32 height,
                                                  s32 value) {
  struct MapScrollWork *map = gMapWork[0];
  struct MapCell *cells;
  u32 row;
  u32 column;

  if (map != 0) {
    if (layer <= 2) {
      cells = map->layers[layer].cells;
    } else {
      cells = (struct MapCell *)gMapCellBuffer;
    }
    cells += x + (z << 7);
    for (row = 0; row < height; row++) {
      struct MapCell *cell = cells + (row << 7);

      for (column = 0; column < width; column++) {
        cell->collisionCode = value;
        cell++;
      }
    }
  }
  return 0;
}

// UB: the native s32 callback is called through FieldActor's void hook. The
// void form keeps 0xa4 bytes in Altmiller English but changes the return move.
#ifdef UBFIX
/// The leader's movement callback while pushing a large thing: stop the leader
/// when another actor that is not a large thing, or a wall, is in the way.
static void StagedActor_StopBlockedMotion(union FieldObject *object) {
  struct FieldActor *actor = (struct FieldActor *)object;
#else
/// The leader's movement callback while pushing a large thing: stop the leader
/// when another actor that is not a large thing, or a wall, is in the way.
static s32 StagedActor_StopBlockedMotion(struct FieldActor *actor) {
#endif
  struct FieldPosition probe;
  u32 dir;
  s32 step;
  struct FieldActor *target;

  dir = actor->facing >> 12;
  step = gStagedActor_DirectionSteps[dir];
  probe.x = actor->x.fixed + (step & 0xffff0000);
  probe.y = actor->y.fixed;
  step <<= 16;
  probe.z = actor->z.fixed + step;
  target = StagedActor_FindAtTile(&probe, actor);
  if (target != 0) {
    u32 i = 0;
    s32 kind = target->sprite->entries[0]->animId;
    s32 *kinds = gStagedActor_FootprintKinds;

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
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->targetX = 0x80000000;
    actor->targetZ = 0x80000000;
  }
  step = gStagedActor_DirectionSteps[dir];
  probe.x = actor->x.fixed + (step & 0xffff0000);
  probe.y = actor->y.fixed;
  step <<= 16;
  probe.z = actor->z.fixed + step;
  if (FieldObject_CheckMovementCollision(actor, &probe) > 0) {
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->targetX = 0x80000000;
    actor->targetZ = 0x80000000;
  }
#ifndef UBFIX
  return 0;
#endif
}

/// Returns the large thing whose footprint covers the cell ahead of the leader
/// (the leader not standing in line with its first column or row), with the
/// leader's direction, its actor number and its footprint; or NULL.
static struct FieldActor *FieldScene_FindActorRegion(s32 *direction, s32 *slot,
                                                     s32 *footprint) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  struct FieldActor *player = Object_GetById(0);
  struct FieldActor **list;
  u32 i, j;

  *direction = player->facing >> 12;
  list = work->actors + ACTOR_FIRST_PLACED;
  for (i = ACTOR_FIRST_PLACED;
       i < sizeof(work->actors) / sizeof(work->actors[0]); list++, i++) {
    struct FieldActor *actor = (struct FieldActor *)*list;
    s32 kind;
    j = 0;
    // UB: the same empty slots are scanned here; the null guard grows the
    // native English helper from 0x128 to 0x134 bytes.
#ifdef UBFIX
    if (actor == NULL)
      continue;
#endif
    kind = actor->sprite->entries[0]->animId;
    for (j = 0; j < 6; j++) {
      if (kind == gStagedActor_FootprintKinds[j]) {
        s32 x, z, left, top, right, bottom;
        *footprint = j;
        x = ((player->x.fixed >> 16) +
             (gStagedActor_DirectionSteps[*direction] >> 16)) >>
            4;
        z = ((player->z.fixed >> 16) +
             (s16)gStagedActor_DirectionSteps[*direction]) >>
            4;
        left =
            (actor->x.part.pixel + gStagedActor_FootprintBounds[j * 4 + 0]) >>
            4;
        top = (actor->z.part.pixel + gStagedActor_FootprintBounds[j * 4 + 1]) >>
              4;
        right =
            (actor->x.part.pixel + gStagedActor_FootprintBounds[j * 4 + 2]) >>
            4;
        bottom =
            (actor->z.part.pixel + gStagedActor_FootprintBounds[j * 4 + 3]) >>
            4;
        if (left <= x && x < right && top <= z && z < bottom) {
          if (j & 1) {
            if (left != (player->x.fixed >> 20)) {
              *slot = i;
              return actor;
            }
          } else if (top != (player->z.fixed >> 20)) {
            *slot = i;
            return actor;
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
StagedActor_FindClearPosition(struct StagedActorProbe *probe) {
  s32 direction;
  struct FieldPosition position;
  struct FieldActor *actor;
  s32 steps;
  s32 width, height;
  u8 *mode;
  s32 column, row;
  s32 nearEdge, farEdge;
  s32 offset;

  probe->callback = 0;
  actor = FieldScene_FindActorRegion(&direction, &probe->actorSlot,
                                     &probe->footprintIndex);
  if (actor == 0) {
    return 0;
  }
  mode = &actor->terrainId;
  *mode = 2;
  {
    s32 footprint = probe->footprintIndex;

    steps = 0;
    offset = footprint * 4;
    nearEdge = gStagedActor_FootprintBounds[offset + 1];
    if (nearEdge < 0) {
      nearEdge = -nearEdge;
    }
    farEdge = gStagedActor_FootprintBounds[offset + 3];
    if (farEdge < 0) {
      farEdge = -farEdge;
    }
    height = (nearEdge + farEdge) >> 4;
    nearEdge = gStagedActor_FootprintBounds[offset];
    if (nearEdge < 0) {
      nearEdge = -nearEdge;
    }
    farEdge = gStagedActor_FootprintBounds[offset + 2];
    if (farEdge < 0) {
      farEdge = -farEdge;
    }
    width = (nearEdge + farEdge) >> 4;
  }
  position.x =
      actor->x.fixed + (gStagedActor_DirectionSteps[direction] & 0xffff0000);
  {
    s32 y = actor->y.fixed;

    position.y = y;
    position.z =
        actor->z.fixed + (gStagedActor_DirectionSteps[direction] << 16);
    probe->position.y = y;
  }
  for (;;) {
    probe->position.z =
        position.z +
        (gStagedActor_FootprintBounds[probe->footprintIndex * 4 + 1] << 16);
    for (row = 0; row < height; row++) {
      probe->position.x =
          position.x +
          (gStagedActor_FootprintBounds[probe->footprintIndex * 4] << 16);
      for (column = 0; column < width; column++) {
        if (FieldObject_CheckMovementCollision(actor, &probe->position) == 2) {
          goto found;
        }
        probe->position.x += 0x100000;
      }
      probe->position.z += 0x100000;
    }
    steps++;
    position.x += gStagedActor_DirectionSteps[direction] & 0xffff0000;
    position.z += gStagedActor_DirectionSteps[direction] << 16;
  }
found:
  *mode = 0;
  if (steps == 0) {
    return 0;
  }
  probe->position.x =
      actor->x.fixed +
      (gStagedActor_DirectionSteps[direction] & 0xffff0000) * steps;
  probe->position.y = actor->y.fixed;
  probe->position.z =
      actor->z.fixed + (gStagedActor_DirectionSteps[direction] << 16) * steps;
  return 1;
}

/// Rolls the large thing to the position probe found: the leader pushes
/// (animation 8), the thing rolls (animation 3 or 2 by the direction) with
/// SE_239 while the leader follows half a cell, and it stops with SE_288 and
/// SE_213. Its cells are blocked (collision 255) at its new place, through the
/// map's layer 1 offset, and its old place is given back what was under it.
UNUSED static void SceneActor_MoveAndRedraw(struct StagedActorProbe probe) {
  struct MapScrollWork *workspace;
  struct FieldActor *actor;
  struct FieldPosition originalPosition;
  struct FieldPosition tilePosition;
  u8 *footprintTable;
  s32 direction;
  s32 horizontalExtent;
  s32 verticalExtent;

  // UB: reading the pointer slot through u32 aliases its declared type. The
  // direct pointer read moves ahead of the probe stores in native English.
#ifdef UBFIX
  workspace = gMapWork[0];
#else
  workspace = (struct MapScrollWork *)*(u32 *)gMapWork;
#endif
  direction = (Object_GetById(0))->facing >> 12;
  actor = Object_GetById(probe.actorSlot);
  footprintTable = (u8 *)gStagedActor_FootprintBounds;
  {
    s32 extentA = gStagedActor_FootprintBounds[probe.footprintIndex * 4 + 1];
    s32 extentB;

    if (extentA < 0)
      extentA = -extentA;
    extentB = gStagedActor_FootprintBounds[probe.footprintIndex * 4 + 3];
    if (extentB < 0)
      extentB = -extentB;
    verticalExtent = (extentA + extentB) >> 4;
    extentA = gStagedActor_FootprintBounds[probe.footprintIndex * 4 + 0];
    if (extentA < 0)
      extentA = -extentA;
    extentB = gStagedActor_FootprintBounds[probe.footprintIndex * 4 + 2];
    if (extentB < 0)
      extentB = -extentB;
    horizontalExtent = (extentA + extentB) >> 4;
  }

  actor->speed = 0x8000;
  actor->acceleration = 0x1999;
  originalPosition.x = actor->x.fixed;
  originalPosition.z = actor->z.fixed;
  {
    s32 tableOffset = probe.footprintIndex << 4;
    tilePosition.x =
        (actor->x.fixed + (*(s32 *)(footprintTable + tableOffset) << 16));
    tableOffset += 4;
    tilePosition.z =
        (actor->z.fixed + (*(s32 *)(footprintTable + tableOffset) << 16));
    tilePosition.x >>= 20;
    tilePosition.z >>= 20;
  }

  StagedActor_FillGridAttributeRectangle(0, tilePosition.x, tilePosition.z,
                                         horizontalExtent, verticalExtent, 0);
  Engine_ActorSetSpeed(0, 0x8000, 0x1999);
  Engine_ActorSetAnimation(0, 8);
  Engine_EventWait(15);

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
      Engine_ActorSetDestinationOffset(0, horizontalDelta, verticalDelta);
    }
  }

#ifdef UBFIX
  Object_GetById(0)->update = StagedActor_StopBlockedMotion;
#else
  Object_GetById(0)->update =
      (void (*)(union FieldObject *))StagedActor_StopBlockedMotion;
#endif
  Engine_EventWait(4);
  if ((u32)(direction - 6) <= 7)
    Object_SetMode(actor, 3);
  else
    Object_SetMode(actor, 2);

  Engine_AudioPlayCue(SE_239);
  Engine_ObjectSetPosition(actor, probe.position.x, probe.position.y,
                           probe.position.z);
  Engine_ActorWaitForMove(0);
  Engine_ActorSetAnimation(0, 2);
  Actor_SetSpeed(0, 0x4ccc, 0x1999);

  {
    s32 packedOrientation = gStagedActor_DirectionSteps[direction];
    Engine_ActorSetDestinationOffset(0, ((s16)(packedOrientation >> 16)) / 2,
                                     ((s16)packedOrientation) / 2);
  }
  if (probe.callback != 0)
    probe.callback();

  Engine_ActorWaitForMove(0);
  Engine_ActorSetAnimation(0, 1);
  Object_GetById(0)->update = NULL;
  FieldObject_WaitForMove(actor);
  Engine_AudioPlayCue(SE_288);
  Engine_AudioPlayCue(SE_213);

  actor->x.fixed = probe.position.x;
  actor->z.fixed = probe.position.z;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  Object_SetMode(actor, 1);

  {
    u8 *table = (u8 *)gStagedActor_FootprintBounds;
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

    Map_CopyCellAttributes(probe.position.x, probe.position.z, horizontalExtent,
                           verticalExtent, horizontalOrigin + probe.position.x,
                           verticalOrigin + probe.position.z);
    StagedActor_FillGridAttributeRectangle(0, probe.position.x,
                                           probe.position.z, horizontalExtent,
                                           verticalExtent, 255);
    StagedActor_FillGridAttributeRectangle(2, probe.position.x,
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

    Engine_MapCopyCellAttributes(horizontalOrigin, verticalOrigin,
                                 horizontalExtent, verticalExtent,
                                 originalPosition.x, originalPosition.z);
    StagedActor_FillGridAttributeRectangle(2, originalPosition.x,
                                           originalPosition.z, horizontalExtent,
                                           verticalExtent, 0);
  }
  BattleFx_PlayQueuedSound();
}

/// Block large thing id's cells (collision 255) where it stands, through the
/// map's layer 1 offset; returns 0 when its sprite is no large thing.
UNUSED static s32 FieldScene_RedrawActorFootprint(s32 id) {
  struct MapScrollWork *work = gMapWork[0];
  struct FieldActor *actor = Object_GetById(id);
  struct StagedActorProbe probe;
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
    if (actor->sprite->entries[0]->animId == gStagedActor_FootprintKinds[idx]) {
      probe.footprintIndex = idx;
      break;
    }
    probe.footprintIndex = 7;
  }

  if ((u32)probe.footprintIndex > 6)
    return 0;
  probe.position.x = actor->x.fixed;
  probe.position.y = actor->y.fixed;
  probe.position.z = actor->z.fixed;
  offset = probe.footprintIndex * 4;
  z0 = gStagedActor_FootprintBounds[offset + 1];
  if (z0 < 0)
    z0 = -z0;
  z1 = gStagedActor_FootprintBounds[offset + 3];
  if (z1 < 0)
    z1 = -z1;
  height = (z0 + z1) >> 4;
  x0 = gStagedActor_FootprintBounds[offset];
  if (x0 < 0)
    x0 = -x0;
  x1 = gStagedActor_FootprintBounds[offset + 2];
  if (x1 < 0)
    x1 = -x1;
  width = (x0 + x1) >> 4;
  probe.position.x += gStagedActor_FootprintBounds[offset] << 16;
  probe.position.z += gStagedActor_FootprintBounds[offset + 1] << 16;
  probe.position.x >>= 20;
  probe.position.z >>= 20;
  idx = work->layers[1].offsetX >> 20;
  dstZ = work->layers[1].offsetY >> 20;
  Engine_MapCopyCellAttributes(probe.position.x, probe.position.z, width,
                               height, idx + probe.position.x,
                               dstZ + probe.position.z);
  StagedActor_FillGridAttributeRectangle(0, probe.position.x, probe.position.z,
                                         width, height, 255);
  StagedActor_FillGridAttributeRectangle(2, probe.position.x, probe.position.z,
                                         width, height, 255);
  return 1;
}
