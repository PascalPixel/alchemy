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
// Cave, Kolima's forest bridge, clearing and river, Lunpa's fortress, Mercury
// Lighthouse and its entrance, Mogall Forest, the Tunnel Ruins, Vale and its
// cave): blocks pushed a cell at a time, and logs and other large things that
// roll until they meet something, redrawn into the map where they stop.
//
// A large thing is one of six footprints, by its sprite's animation
// (gStagedActor_FootprintKinds): its bounds in gStagedActor_FootprintBounds
// are four words a footprint, the left, top, right and bottom edges in pixels
// from its position.

// Pixel step by direction: X in the upper halfword, Z in the lower.
extern s32 gStagedActor_DirectionSteps[];
extern s32 gStagedActor_FootprintBounds[];

// Shared source is included here in ROM order.
#include "object_distance.h"

// Return the placed actor on the cell of the 16.16 position and at its height
// (in whole pixels), or 0. origin is not read.
static struct StagedActor *StagedActor_FindAtTile(s32 *position,
                                                  struct StagedActor *origin) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  struct FieldActor **slots = work->actors;
  u32 i;

  for (i = 8; i <= 65; i++) {
    struct StagedActor *actor = (struct StagedActor *)slots[i];

    if ((position[0] >> 20) == (actor->x.value >> 20) &&
        (position[1] / 0x10000) == (actor->y / 0x10000) &&
        (position[2] >> 20) == (actor->z.value >> 20)) {
      return actor;
    }
  }
  return 0;
}

// The leader pushes the block in front of them a cell on, unless something
// solid is beyond it or on top of it, the ground beyond refuses it or it is
// still moving: after a quarter second's push (mode 8) the block and the
// leader slide one cell together, with SE_185.
static void StagedActor_AdvancePair(void) {
  s32 dst[3];
  struct StagedActor *lead;
  struct StagedActor *next;
  struct StagedActor *blocker;
  s32 dir;
  u32 step;
  s32 rate;

  lead = (struct StagedActor *)Object_GetById(0);
  dir = lead->directionAndKind >> 12;
  step = gStagedActor_DirectionSteps[dir];
  dst[0] = lead->x.value + (step & 0xffff0000);
  dst[1] = lead->y;
  step <<= 16;
  dst[2] = lead->z.value + step;
  next = StagedActor_FindAtTile(dst, lead);
  if (next == 0)
    return;

  step = gStagedActor_DirectionSteps[dir];
  dst[0] = next->x.value + (step & 0xffff0000);
  dst[1] = next->y;
  step <<= 16;
  dst[2] = next->z.value + step;
  blocker = StagedActor_FindAtTile(dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  dst[0] = next->x.value;
  dst[1] = next->y + 0x100000;
  dst[2] = next->z.value;
  blocker = StagedActor_FindAtTile(dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  next->transitionMode = 2;
  step = gStagedActor_DirectionSteps[dir];
  dst[0] = next->x.value + (step & 0xffff0000);
  dst[1] = next->y;
  step <<= 16;
  dst[2] = next->z.value + step;
  if (FieldObject_CheckMovementCollision(next, dst) > 0)
    return;

  if (next->transitionBusy != 0)
    return;

  Object_SetMode((struct FieldActor *)lead, 8);
  rate = 0x3333;
  Engine_TaskWait(15);
  Engine_AudioPlayCue(SE_185);
  next->moveRateX = rate;
  next->moveRateZ = rate;
  Engine_ObjectSetPosition((struct FieldActor *)next, dst[0], dst[1], dst[2]);
  lead->moveRateX = rate;
  lead->moveRateZ = rate;
  Engine_ObjectSetPosition((struct FieldActor *)lead, dst[0], dst[1], dst[2]);
  FieldObject_WaitForMove(next);
  BattleFx_PlayQueuedSound();
  next->x.value = dst[0];
  next->z.value = dst[2];
  next->velocityX = 0;
  next->velocityZ = 0;
  lead->targetX = 0x80000000;
  lead->targetZ = 0x80000000;
  lead->velocityX = 0;
  lead->velocityZ = 0;
  lead->x.value = lead->x.parts.cell << 16;
  lead->z.value = lead->z.parts.cell << 16;
  Object_SetMode((struct FieldActor *)lead, 1);
}

// Set the collision code of a width by height rectangle of cells, from cell
// (x, z), to value, on map layer 0 to 2, or (3 and up) in gMapCellBuffer.
static s32 StagedActor_FillGridAttributeRectangle(u32 layer, s32 x, s32 z,
                                                  u32 width, u32 height,
                                                  s32 value) {
  struct MapState *map = gMapWork[0];
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

// The leader's movement callback while pushing a large thing: stop the leader
// when another actor that is not a large thing, or a wall, is in the way.
static s32 StagedActor_StopBlockedMotion(struct StagedActor *actor) {
  struct StagedActorProbePoint probe;
  u32 dir;
  s32 step;
  struct StagedActor *target;

  dir = actor->directionAndKind >> 12;
  step = gStagedActor_DirectionSteps[dir];
  probe.x = actor->x.value + (step & 0xffff0000);
  probe.y = actor->y;
  step <<= 16;
  probe.z = actor->z.value + step;
  target = StagedActor_FindAtTile((s32 *)&probe, actor);
  if (target != 0) {
    u32 i = 0;
    s32 kind = target->animation->entries[0]->animId;
    s32 *kinds = gStagedActor_FootprintKinds;

    do {
      if (kind == *kinds++)
        return 0;
      i++;
    } while (i <= 5);
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->targetX = 0x80000000;
    actor->targetZ = 0x80000000;
  }
  step = gStagedActor_DirectionSteps[dir];
  probe.x = actor->x.value + (step & 0xffff0000);
  probe.y = actor->y;
  step <<= 16;
  probe.z = actor->z.value + step;
  if (FieldObject_CheckMovementCollision(actor, (s32 *)&probe) > 0) {
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->targetX = 0x80000000;
    actor->targetZ = 0x80000000;
  }
  return 0;
}

// Return the large thing whose footprint covers the cell ahead of the leader
// (the leader not standing in line with its first column or row), with the
// leader's direction, its actor number and its footprint; or NULL.
static u8 *FieldScene_FindActorRegion(s32 *direction, s32 *slot,
                                      s32 *footprint) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  struct StagedActor *player = (struct StagedActor *)Object_GetById(0);
  struct FieldActor **list;
  u32 i, j;

  *direction = player->directionAndKind >> 12;
  list = work->actors + 8;
  for (i = 8; i <= 65; list++, i++) {
    struct StagedActor *actor = (struct StagedActor *)*list;
    s32 kind;
    j = 0;
    kind = actor->animation->entries[0]->animId;
    for (j = 0; j < 6; j++) {
      if (kind == gStagedActor_FootprintKinds[j]) {
        s32 x, z, left, top, right, bottom;
        *footprint = j;
        x = ((player->x.value >> 16) +
             (gStagedActor_DirectionSteps[*direction] >> 16)) >>
            4;
        z = ((player->z.value >> 16) +
             (s16)gStagedActor_DirectionSteps[*direction]) >>
            4;
        left =
            (actor->x.parts.cell + gStagedActor_FootprintBounds[j * 4 + 0]) >>
            4;
        top = (actor->z.parts.cell + gStagedActor_FootprintBounds[j * 4 + 1]) >>
              4;
        right =
            (actor->x.parts.cell + gStagedActor_FootprintBounds[j * 4 + 2]) >>
            4;
        bottom =
            (actor->z.parts.cell + gStagedActor_FootprintBounds[j * 4 + 3]) >>
            4;
        if (left <= x && x < right && top <= z && z < bottom) {
          if (j & 1) {
            if (left != (player->x.value >> 20)) {
              *slot = i;
              return (u8 *)actor;
            }
          } else if (top != (player->z.value >> 20)) {
            *slot = i;
            return (u8 *)actor;
          }
        }
      }
    }
  }
  return NULL;
}

// Find where the large thing ahead of the leader would roll to: step its
// footprint on, a cell at a time the way the leader faces, until a cell of it
// meets something (a collision result of 2). Fills probe with the thing, its
// footprint and that position; returns 0 when there is no thing or it cannot
// move.
static s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe) {
  s32 direction;
  s32 position[3];
  struct StagedActor *actor;
  s32 steps;
  s32 width, height;
  u8 *mode;
  s32 column, row;
  s32 nearEdge, farEdge;
  s32 offset;

  probe->callback = 0;
  actor = (struct StagedActor *)FieldScene_FindActorRegion(
      &direction, &probe->actorSlot, &probe->footprintIndex);
  if (actor == 0) {
    return 0;
  }
  mode = &actor->transitionMode;
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
  position[0] =
      actor->x.value + (gStagedActor_DirectionSteps[direction] & 0xffff0000);
  {
    s32 y = actor->y;

    position[1] = y;
    position[2] =
        actor->z.value + (gStagedActor_DirectionSteps[direction] << 16);
    probe->positionY = y;
  }
  for (;;) {
    probe->positionZ =
        position[2] +
        (gStagedActor_FootprintBounds[probe->footprintIndex * 4 + 1] << 16);
    for (row = 0; row < height; row++) {
      probe->positionX =
          position[0] +
          (gStagedActor_FootprintBounds[probe->footprintIndex * 4] << 16);
      for (column = 0; column < width; column++) {
        if (FieldObject_CheckMovementCollision(actor, &probe->positionX) == 2) {
          goto found;
        }
        probe->positionX += 0x100000;
      }
      probe->positionZ += 0x100000;
    }
    steps++;
    position[0] += gStagedActor_DirectionSteps[direction] & 0xffff0000;
    position[2] += gStagedActor_DirectionSteps[direction] << 16;
  }
found:
  *mode = 0;
  if (steps == 0) {
    return 0;
  }
  probe->positionX =
      actor->x.value +
      (gStagedActor_DirectionSteps[direction] & 0xffff0000) * steps;
  probe->positionY = actor->y;
  probe->positionZ =
      actor->z.value + (gStagedActor_DirectionSteps[direction] << 16) * steps;
  return 1;
}

// Roll the large thing to the position probe found: the leader pushes
// (animation 8), the thing rolls (mode 3 or 2 by the direction) with SE_239
// while the leader follows half a cell, and it stops with SE_288 and SE_213.
// Its cells are blocked (collision 255) at its new place, through the map's
// layer 1 offset, and its old place is given back what was under it.
static void SceneActor_MoveAndRedraw(struct StagedActorProbe probe) {
  // gMapWork[0] is read as a u32, the plain-C form this site matches with
  // (scene/README.md): a u32 may alias the probe's s32 fields, so the load
  // stays after the probe's four stores; read as the void * it is, it is
  // scheduled above them.
  struct MapScrollWork *workspace;
  struct StagedActor *actor;
  struct StagedActorProbePosition originalPosition;
  struct StagedActorProbePosition tilePosition;
  u8 *footprintTable;
  s32 direction;
  s32 horizontalExtent;
  s32 verticalExtent;

  workspace = (struct MapScrollWork *)*(u32 *)gMapWork;
  direction = ((struct StagedActor *)Object_GetById(0))->directionAndKind >> 12;
  actor = (struct StagedActor *)Object_GetById(probe.actorSlot);
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

  actor->moveRateX = 0x8000;
  actor->moveRateZ = 0x1999;
  originalPosition.x = actor->x.value;
  originalPosition.z = actor->z.value;
  {
    s32 tableOffset = probe.footprintIndex << 4;
    tilePosition.x =
        (actor->x.value + (*(s32 *)(footprintTable + tableOffset) << 16));
    tableOffset += 4;
    tilePosition.z =
        (actor->z.value + (*(s32 *)(footprintTable + tableOffset) << 16));
    tilePosition.x >>= 20;
    tilePosition.z >>= 20;
  }

  StagedActor_FillGridAttributeRectangle(0, tilePosition.x, tilePosition.z,
                                         horizontalExtent, verticalExtent, 0);
  Engine_ActorSetSpeed(0, 0x8000, 0x1999);
  Engine_ActorSetAnimation(0, 8);
  Engine_EventWait(15);

  {
    s32 horizontalDelta = probe.positionX - originalPosition.x;
    if (horizontalDelta < 0)
      horizontalDelta += 0x1ffff;
    horizontalDelta >>= 17;
    {
      s32 verticalDelta = probe.positionZ - originalPosition.z;
      if (verticalDelta < 0)
        verticalDelta += 0x1ffff;
      verticalDelta >>= 17;
      Engine_ActorSetDestinationOffset(0, horizontalDelta, verticalDelta);
    }
  }

  ((struct StagedActor *)Object_GetById(0))->movementCallback =
      (u32)StagedActor_StopBlockedMotion;
  Engine_EventWait(4);
  if ((u32)(direction - 6) <= 7)
    Object_SetMode((struct FieldActor *)actor, 3);
  else
    Object_SetMode((struct FieldActor *)actor, 2);

  Engine_AudioPlayCue(SE_239);
  Engine_ObjectSetPosition((struct FieldActor *)actor, probe.positionX,
                           probe.positionY, probe.positionZ);
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
  ((struct StagedActor *)Object_GetById(0))->movementCallback = 0;
  FieldObject_WaitForMove(actor);
  Engine_AudioPlayCue(SE_288);
  Engine_AudioPlayCue(SE_213);

  actor->x.value = probe.positionX;
  actor->z.value = probe.positionZ;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  Object_SetMode((struct FieldActor *)actor, 1);

  {
    u8 *table = (u8 *)gStagedActor_FootprintBounds;
    s32 horizontalOrigin;
    s32 verticalOrigin;

    {
      s32 tableOffset = probe.footprintIndex << 4;
      probe.positionX =
          (probe.positionX + (*(s32 *)(table + tableOffset) << 16));
      tableOffset += 4;
      probe.positionZ =
          (probe.positionZ + (*(s32 *)(table + tableOffset) << 16));
      probe.positionX >>= 20;
      probe.positionZ >>= 20;
    }
    horizontalOrigin = workspace->layers[1].offsetX >> 20;
    verticalOrigin = workspace->layers[1].offsetY >> 20;

    Map_CopyCellAttributes(probe.positionX, probe.positionZ, horizontalExtent,
                           verticalExtent, horizontalOrigin + probe.positionX,
                           verticalOrigin + probe.positionZ);
    StagedActor_FillGridAttributeRectangle(0, probe.positionX, probe.positionZ,
                                           horizontalExtent, verticalExtent,
                                           255);
    StagedActor_FillGridAttributeRectangle(2, probe.positionX, probe.positionZ,
                                           horizontalExtent, verticalExtent,
                                           255);

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

// Block large thing id's cells (collision 255) where it stands, through the
// map's layer 1 offset; returns 0 when its sprite is no large thing.
static s32 FieldScene_RedrawActorFootprint(s32 id) {
  struct MapScrollWork *work = gMapWork[0];
  struct StagedActor *actor = (struct StagedActor *)Object_GetById(id);
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
    if (actor->animation->entries[0]->animId ==
        gStagedActor_FootprintKinds[idx]) {
      probe.footprintIndex = idx;
      break;
    }
    probe.footprintIndex = 7;
  }

  if ((u32)probe.footprintIndex > 6)
    return 0;
  probe.positionX = actor->x.value;
  probe.positionY = actor->y;
  probe.positionZ = actor->z.value;
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
  probe.positionX += gStagedActor_FootprintBounds[offset] << 16;
  probe.positionZ += gStagedActor_FootprintBounds[offset + 1] << 16;
  probe.positionX >>= 20;
  probe.positionZ >>= 20;
  idx = work->layers[1].offsetX >> 20;
  dstZ = work->layers[1].offsetY >> 20;
  Engine_MapCopyCellAttributes(probe.positionX, probe.positionZ, width, height,
                               idx + probe.positionX, dstZ + probe.positionZ);
  StagedActor_FillGridAttributeRectangle(0, probe.positionX, probe.positionZ,
                                         width, height, 255);
  StagedActor_FillGridAttributeRectangle(2, probe.positionX, probe.positionZ,
                                         width, height, 255);
  return 1;
}
