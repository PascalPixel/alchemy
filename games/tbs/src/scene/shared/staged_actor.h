#include "module/field/field.h"
#include "battle.h"
#include "system_call.h"
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
#include "pushed_actor.h"
#include "fixed_point_position.h"
#include "iwram_call.h"
#include "call.h"
#include "scene.h"
#include "map_grid.h"
#include "map_grid_scroll.h"
#include "event.h"
#include "types.h"
#include "songs.h"

// Seventeen scenes with things the leader pushes (Altmiller Cave, Babi
// Lighthouse, Crossbone Isle and its rooms, Goma Cave's pillar room, Gondowan
// Cave, Kolima Forest (SceneId_KolimaForestEntrance to
// SceneId_KolimaForestCliff), Lunpa's fortress, Mercury Lighthouse and its
// entrance, Mogall Forest, the Tunnel Ruins, Vale and its cave): things
// pushed a cell at a time, such as stone pillars, and logs and other large
// things that roll until they meet something, redrawn into the map grid where
// they stop.
//
// A large thing is one of six footprints, by its sprite's animation
// (gPushedActor_FootprintKinds): its bounds in
// gPushedActor_FootprintBounds are four words a footprint, the left, top,
// right and bottom edges in pixels from its position.

/// Pixel step by direction: X in the upper halfword, Z in the lower.
extern s32 gPushedActor_DirectionSteps[];
extern s32 gPushedActor_FootprintBounds[];

#include "actor_distance.h"

/// Returns the placed actor on the cell of the 16.16 position and at its
/// height (in whole pixels), or 0. origin is not read.
static struct Actor *
PushedActor_FindAtTile(const struct FieldPosition *position,
                       const struct Actor *origin) {
  struct FieldWork *work = gFieldWork;
  struct Actor **slots = work->actors;
  u32 i;

  for (i = ACTOR_FIRST_PLACED;
       i < sizeof(work->actors) / sizeof(work->actors[0]); i++) {
    struct Actor *actor = slots[i];

    // UB: Actor_ClearAll leaves unfilled slots null. The guard grows
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

/// The leader pushes what stands in front of them a cell on, unless
/// something solid is beyond it or on top of it, the ground beyond refuses it
/// or it is still moving: after a quarter second's push (animation 8) it and
/// the leader slide one cell together, with SE_PUSH.
UNUSED static void PushedActor_AdvancePair(void) {
  struct FieldPosition dst;
  struct Actor *lead;
  struct Actor *next;
  struct Actor *blocker;
  s32 dir;
  u32 step;
  s32 rate;

  lead = Actor_GetById(ACTOR_PARTY_LEADER);
  dir = lead->facing >> 12;
  step = gPushedActor_DirectionSteps[dir];
  dst.x = lead->x.fixed + (step & 0xffff0000);
  dst.y = lead->y.fixed;
  step <<= 16;
  dst.z = lead->z.fixed + step;
  next = PushedActor_FindAtTile(&dst, lead);
  if (next == 0)
    return;

  step = gPushedActor_DirectionSteps[dir];
  dst.x = next->x.fixed + (step & 0xffff0000);
  dst.y = next->y.fixed;
  step <<= 16;
  dst.z = next->z.fixed + step;
  blocker = PushedActor_FindAtTile(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  dst.x = next->x.fixed;
  dst.y = next->y.fixed + 0x100000;
  dst.z = next->z.fixed;
  blocker = PushedActor_FindAtTile(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  next->layer = 2;
  step = gPushedActor_DirectionSteps[dir];
  dst.x = next->x.fixed + (step & 0xffff0000);
  dst.y = next->y.fixed;
  step <<= 16;
  dst.z = next->z.fixed + step;
  if (FieldActor_CheckMovementCollision(next, &dst) > 0)
    return;

  if (next->riseCounter != 0)
    return;

  Actor_SelectAnimation(lead, 8);
  rate = 0x3333;
  WaitFrames(15);
  Sound_PlaySong(SE_PUSH);
  next->speedLimit = rate;
  next->acceleration = rate;
  FieldActor_SetMoveTarget(next, dst.x, dst.y, dst.z);
  lead->speedLimit = rate;
  lead->acceleration = rate;
  FieldActor_SetMoveTarget(lead, dst.x, dst.y, dst.z);
  Actor_WaitForMove(next);
  Scene_PlayQueuedMusic();
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
  Actor_SelectAnimation(lead, 1);
}

/// Sets the cell behavior of a width by height rectangle of cells, from
/// cell (x, z), to value, on map grid layer 0 to 2, or (3 and up) in
/// gMapGridCellBuffer.
static s32 PushedActor_FillGridAttributeRectangle(u32 layer, s32 x, s32 z,
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

// UB: the native s32 update routine is called through Actor's void update. The
// void form keeps 0xa4 bytes in Altmiller English but changes the return move.
#ifdef UBFIX
/// The leader's update routine while pushing a large thing: stop the leader
/// when another actor that is not a large thing, or a wall, is in the
/// way.
static void PushedActor_StopBlockedMotion(struct Actor *updated) {
  struct Actor *actor = (struct Actor *)updated;
#else
/// The leader's update routine while pushing a large thing: stop the leader
/// when another actor that is not a large thing, or a wall, is in the
/// way.
static s32 PushedActor_StopBlockedMotion(struct Actor *actor) {
#endif
  struct FieldPosition probe;
  u32 dir;
  s32 step;
  struct Actor *target;

  dir = actor->facing >> 12;
  step = gPushedActor_DirectionSteps[dir];
  probe.x = actor->x.fixed + (step & 0xffff0000);
  probe.y = actor->y.fixed;
  step <<= 16;
  probe.z = actor->z.fixed + step;
  target = PushedActor_FindAtTile(&probe, actor);
  if (target != 0) {
    u32 i = 0;
    s32 kind = target->sprite->entries[0]->animationId;
    s32 *kinds = gPushedActor_FootprintKinds;

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
  step = gPushedActor_DirectionSteps[dir];
  probe.x = actor->x.fixed + (step & 0xffff0000);
  probe.y = actor->y.fixed;
  step <<= 16;
  probe.z = actor->z.fixed + step;
  if (FieldActor_CheckMovementCollision(actor, &probe) > 0) {
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
/// leader's direction, its actor id and its footprint; or NULL.
static struct Actor *Scene_FindActorRegion(s32 *direction, s32 *slot,
                                           s32 *footprint) {
  struct FieldWork *work = gFieldWork;
  struct Actor *player = Actor_GetById(ACTOR_PARTY_LEADER);
  struct Actor **list;
  u32 i, j;

  *direction = player->facing >> 12;
  list = work->actors + ACTOR_FIRST_PLACED;
  for (i = ACTOR_FIRST_PLACED;
       i < sizeof(work->actors) / sizeof(work->actors[0]); list++, i++) {
    struct Actor *actor = *list;
    s32 kind;
    j = 0;
    // UB: the same empty slots are scanned here; the null guard grows the
    // native English helper from 0x128 to 0x134 bytes.
#ifdef UBFIX
    if (actor == NULL)
      continue;
#endif
    kind = actor->sprite->entries[0]->animationId;
    for (j = 0; j < 6; j++) {
      if (kind == gPushedActor_FootprintKinds[j]) {
        s32 x, z, left, top, right, bottom;
        *footprint = j;
        x = ((player->x.fixed >> 16) +
             (gPushedActor_DirectionSteps[*direction] >> 16)) >>
            4;
        z = ((player->z.fixed >> 16) +
             (s16)gPushedActor_DirectionSteps[*direction]) >>
            4;
        left =
            (actor->x.part.pixel + gPushedActor_FootprintBounds[j * 4 + 0]) >>
            4;
        top = (actor->z.part.pixel + gPushedActor_FootprintBounds[j * 4 + 1]) >>
              4;
        right =
            (actor->x.part.pixel + gPushedActor_FootprintBounds[j * 4 + 2]) >>
            4;
        bottom =
            (actor->z.part.pixel + gPushedActor_FootprintBounds[j * 4 + 3]) >>
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
PushedActor_FindClearPosition(struct PushedActorProbe *probe) {
  s32 direction;
  struct FieldPosition position;
  struct Actor *actor;
  s32 steps;
  s32 width, height;
  u8 *mode;
  s32 column, row;
  s32 nearEdge, farEdge;
  s32 offset;

  probe->callback = 0;
  actor = Scene_FindActorRegion(&direction, &probe->actorSlot,
                                &probe->footprintIndex);
  if (actor == 0) {
    return 0;
  }
  mode = &actor->layer;
  *mode = 2;
  {
    s32 footprint = probe->footprintIndex;

    steps = 0;
    offset = footprint * 4;
    nearEdge = gPushedActor_FootprintBounds[offset + 1];
    if (nearEdge < 0) {
      nearEdge = -nearEdge;
    }
    farEdge = gPushedActor_FootprintBounds[offset + 3];
    if (farEdge < 0) {
      farEdge = -farEdge;
    }
    height = (nearEdge + farEdge) >> 4;
    nearEdge = gPushedActor_FootprintBounds[offset];
    if (nearEdge < 0) {
      nearEdge = -nearEdge;
    }
    farEdge = gPushedActor_FootprintBounds[offset + 2];
    if (farEdge < 0) {
      farEdge = -farEdge;
    }
    width = (nearEdge + farEdge) >> 4;
  }
  position.x =
      actor->x.fixed + (gPushedActor_DirectionSteps[direction] & 0xffff0000);
  {
    s32 y = actor->y.fixed;

    position.y = y;
    position.z =
        actor->z.fixed + (gPushedActor_DirectionSteps[direction] << 16);
    probe->position.y = y;
  }
  for (;;) {
    probe->position.z =
        position.z +
        (gPushedActor_FootprintBounds[probe->footprintIndex * 4 + 1] << 16);
    for (row = 0; row < height; row++) {
      probe->position.x =
          position.x +
          (gPushedActor_FootprintBounds[probe->footprintIndex * 4] << 16);
      for (column = 0; column < width; column++) {
        if (FieldActor_CheckMovementCollision(actor, &probe->position) == 2) {
          goto found;
        }
        probe->position.x += 0x100000;
      }
      probe->position.z += 0x100000;
    }
    steps++;
    position.x += gPushedActor_DirectionSteps[direction] & 0xffff0000;
    position.z += gPushedActor_DirectionSteps[direction] << 16;
  }
found:
  *mode = 0;
  if (steps == 0) {
    return 0;
  }
  probe->position.x =
      actor->x.fixed +
      (gPushedActor_DirectionSteps[direction] & 0xffff0000) * steps;
  probe->position.y = actor->y.fixed;
  probe->position.z =
      actor->z.fixed + (gPushedActor_DirectionSteps[direction] << 16) * steps;
  return 1;
}

/// Rolls the large thing to the position probe found: the leader pushes
/// (animation 8), the thing rolls (animation 3 or 2 by the direction) with
/// SE_HEAVY_MOVE while the leader follows half a cell, and it stops with
/// SE_STOP_ACTION_SE and SE_HEAVY_STOP. Its cells are blocked (cell
/// behavior 255) at its new place, through the map grid's layer 1 offset, and
/// its old place is given back what was under it.
UNUSED static void Actor_MoveAndRedraw(struct PushedActorProbe probe) {
  struct MapGridScrollWork *workspace;
  struct Actor *actor;
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
  direction = (Actor_GetById(ACTOR_PARTY_LEADER))->facing >> 12;
  actor = Actor_GetById(probe.actorSlot);
  footprintTable = (u8 *)gPushedActor_FootprintBounds;
  {
    s32 extentA = gPushedActor_FootprintBounds[probe.footprintIndex * 4 + 1];
    s32 extentB;

    if (extentA < 0)
      extentA = -extentA;
    extentB = gPushedActor_FootprintBounds[probe.footprintIndex * 4 + 3];
    if (extentB < 0)
      extentB = -extentB;
    verticalExtent = (extentA + extentB) >> 4;
    extentA = gPushedActor_FootprintBounds[probe.footprintIndex * 4 + 0];
    if (extentA < 0)
      extentA = -extentA;
    extentB = gPushedActor_FootprintBounds[probe.footprintIndex * 4 + 2];
    if (extentB < 0)
      extentB = -extentB;
    horizontalExtent = (extentA + extentB) >> 4;
  }

  actor->speedLimit = 0x8000;
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

  PushedActor_FillGridAttributeRectangle(0, tilePosition.x, tilePosition.z,
                                         horizontalExtent, verticalExtent,
                                         CELL_BEHAVIOR_NORMAL);
  Actor_SetSpeedParameters(ACTOR_PARTY_LEADER, 0x8000, 0x1999);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
  Event_WaitFrames(15);

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
      Actor_OffsetPositionAndResetMotion(ACTOR_PARTY_LEADER, horizontalDelta,
                                         verticalDelta);
    }
  }

#ifdef UBFIX
  Actor_GetById(ACTOR_PARTY_LEADER)->update = PushedActor_StopBlockedMotion;
#else
  Actor_GetById(ACTOR_PARTY_LEADER)->update =
      (void (*)(struct Actor *))PushedActor_StopBlockedMotion;
#endif
  Event_WaitFrames(4);
  if ((u32)(direction - 6) <= 7)
    Actor_SelectAnimation(actor, 3);
  else
    Actor_SelectAnimation(actor, 2);

  Sound_PlaySong(SE_HEAVY_MOVE);
  FieldActor_SetMoveTarget(actor, probe.position.x, probe.position.y,
                           probe.position.z);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x4ccc, 0x1999);

  {
    s32 packedOrientation = gPushedActor_DirectionSteps[direction];
    Actor_OffsetPositionAndResetMotion(0, ((s16)(packedOrientation >> 16)) / 2,
                                       ((s16)packedOrientation) / 2);
  }
  if (probe.callback != 0)
    probe.callback();

  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_GetById(ACTOR_PARTY_LEADER)->update = NULL;
  Actor_WaitForMove(actor);
  Sound_PlaySong(SE_STOP_ACTION_SE);
  Sound_PlaySong(SE_HEAVY_STOP);

  actor->x.fixed = probe.position.x;
  actor->z.fixed = probe.position.z;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  Actor_SelectAnimation(actor, 1);

  {
    u8 *table = (u8 *)gPushedActor_FootprintBounds;
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
    PushedActor_FillGridAttributeRectangle(0, probe.position.x,
                                           probe.position.z, horizontalExtent,
                                           verticalExtent, CELL_BEHAVIOR_WALL);
    PushedActor_FillGridAttributeRectangle(2, probe.position.x,
                                           probe.position.z, horizontalExtent,
                                           verticalExtent, CELL_BEHAVIOR_WALL);

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

    MapGrid_CopyCellAttributeRect(horizontalOrigin, verticalOrigin,
                                  horizontalExtent, verticalExtent,
                                  originalPosition.x, originalPosition.z);
    PushedActor_FillGridAttributeRectangle(
        2, originalPosition.x, originalPosition.z, horizontalExtent,
        verticalExtent, CELL_BEHAVIOR_NORMAL);
  }
  Scene_PlayQueuedMusic();
}

/// Block large thing id's cells (cell behavior 255) where it stands,
/// through the map grid's layer 1 offset; returns 0 when its sprite is no large
/// thing.
UNUSED static s32 Scene_RedrawActorFootprint(s32 id) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  struct Actor *actor = Actor_GetById(id);
  struct PushedActorProbe probe;
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
    if (actor->sprite->entries[0]->animationId ==
        gPushedActor_FootprintKinds[idx]) {
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
  z0 = gPushedActor_FootprintBounds[offset + 1];
  if (z0 < 0)
    z0 = -z0;
  z1 = gPushedActor_FootprintBounds[offset + 3];
  if (z1 < 0)
    z1 = -z1;
  height = (z0 + z1) >> 4;
  x0 = gPushedActor_FootprintBounds[offset];
  if (x0 < 0)
    x0 = -x0;
  x1 = gPushedActor_FootprintBounds[offset + 2];
  if (x1 < 0)
    x1 = -x1;
  width = (x0 + x1) >> 4;
  probe.position.x += gPushedActor_FootprintBounds[offset] << 16;
  probe.position.z += gPushedActor_FootprintBounds[offset + 1] << 16;
  probe.position.x >>= 20;
  probe.position.z >>= 20;
  idx = grid->layers[1].offsetX >> 20;
  dstZ = grid->layers[1].offsetY >> 20;
  MapGrid_CopyCellAttributeRect(probe.position.x, probe.position.z, width,
                                height, idx + probe.position.x,
                                dstZ + probe.position.z);
  PushedActor_FillGridAttributeRectangle(0, probe.position.x, probe.position.z,
                                         width, height, CELL_BEHAVIOR_WALL);
  PushedActor_FillGridAttributeRectangle(2, probe.position.x, probe.position.z,
                                         width, height, CELL_BEHAVIOR_WALL);
  return 1;
}
