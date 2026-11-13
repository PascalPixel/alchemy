/// @file
/// Scene code of overlay 0x664, in ROM order.

#include "flags.h"
#include "../shared/actors.h"
#include "../airs_rock_inside.h"
#include "system.h"
#include "module/field/field.h"
#include "module/scene/airs_rock_inside.h"
#include "module/scene/scene.h"
#include "module/tla.h"

extern struct ActorPlacement gData_0200675c[];
extern struct ActorPlacement gData_020068f4[];
extern struct ActorPlacement gData_020069e4[];
extern struct ActorPlacement gData_02006aec[];
extern u32 gData_02006bac[];

void TilesetAnim_SuspendAll(void);
void TilesetAnim_ResumeAll(void);

/// On most frames, moves the object to a random spot: x 176 to 431 pixels, y
/// and z up to 31 and 127 pixels past the leader's. Then it keeps its action as
/// the previous one and sets mode 9 after action 212, mode 10 otherwise.
void Func_020004d4(struct Actor *object) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *self = (struct Actor *)object;

  if (Random16() & 0xff) {
    object->x = ((Random16() & 0xff) << 16) + (176 << 16);
    {
      s32 r = Random16();
      /* FAKEMATCH(pin): the leader's y pinned to r3, which the ROM loads before
       * it builds the mask; as a plain local, 12 of 53 instructions differ in
       * tla-en (24 with z plain too). */
      register s32 y asm("r3") = leader->y;

      object->y = y + ((r & 31) << 16);
    }
    {
      s32 r = Random16();
      /* FAKEMATCH(pin): as for y, the leader's z pinned to r3, so it loads
       * before the mask; as a plain local, 12 of 53 instructions differ in
       * tla-en (24 with y plain too). */
      register s32 z asm("r3") = leader->z;

      object->z = z + ((r & 127) << 16);
    }
  }
  self->unk66 = self->timer;
  if (self->timer == 212)
    Actor_SelectAnimation(object, 9);
  else
    Actor_SelectAnimation(object, 10);
}

void Func_02000550(struct Actor *object) {
  u8 *work = Ram_HeapSlots->eventWork;

  if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||
      *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0) {
    Actor_SetAnimationSpeed(object, 0);
    TilesetAnim_SuspendAll();
    return;
  }
  Actor_SetAnimationSpeed(object, 16);
  TilesetAnim_ResumeAll();
  if (BYTE_AT(object, 99) != 0) {
    if ((gFrameCount & 7) == 0)
      BYTE_AT(object, 99)--;
    return;
  }
  if (*(s16 *)((u8 *)object + 102) == 0)
    Func_020004d4(object);
  (*(s16 *)((u8 *)object + 102))--;
  /* FAKEMATCH(dead): an empty once-only block, whose loop note ends the
   * scheduling block so the timer is stored before z loads, as the ROM does; as
   * no block, 2 of 87 instructions differ in tla-en. */
  do {
  } while (0);
  object->x += Trig_Cos(((object->z >> 14) + (object->y >> 15)) << 8) / 2;
  object->y += WORD_AT(object, 48);
}

void Func_02000610(void) { Event_ResolvePendingActions(0); }

void Func_0200061c(void) { BYTE_AT(Ram_HeapSlots->effectWork, 52) = 1; }

s32 Func_0200062c(u32 layer, s32 x, s32 y, s32 value) {
  struct AirsRockInsideMapWork *map =
      (struct AirsRockInsideMapWork *)Ram_HeapSlots->mapWork;
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

#include "../../field/object/wait_until_idle.c"

s32 Func_02000690(u32 layer, s32 x, s32 y) {
  struct AirsRockInsideMapWork *map =
      (struct AirsRockInsideMapWork *)Ram_HeapSlots->mapWork;
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

#include "types.h"
#include "actor.h"

/// Sets an object's cells by its flag (its action parameter): when the flag is
/// set it clears the kind byte of its layer 2 cell and its layer 0 cell, sinks
/// 4 pixels when not on its ground and takes sprite priority 3; otherwise it
/// marks both cells (0xff00 and 0xd480), rises 4 pixels and takes priority 1;
/// then it writes both cells back and moves the object's height
/// (MapGrid_SetCellHeight).
void Func_020006cc(struct Actor *object) {
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  s32 ground = Func_02000690(0, x, z);
  s32 cell = Func_02000690(2, x, z);
  s32 height =
      Map_GetTerrainHeight(object->terrainId, object->x, object->z) >> 19;
  s32 set = GameFlag_Get(object->timer);
  u8 *flags = &object->priorityFlags;
  s32 px = x << 20;
  s32 pz = z << 20;

  if (set) {
    cell &= 0xffff00ff;
    ground = 0;
    if (object->terrainHeight >> 19 != height)
      height -= 4;
    // Through the field: through flags the byte, not the 2, would take the
    // result register.
    object->priorityFlags |= 2;
    Scene_SetActorSpritePriority(object, 3);
  } else {
    cell |= 0xff00;
    ground |= 0xd480;
    *flags &= ~2;
    height += 4;
    Scene_SetActorSpritePriority(object, 1);
  }
  Func_0200062c(0, x, z, ground);
  Func_0200062c(2, x, z, cell);
  MapGrid_SetCellHeight(px, pz, object->terrainId, height);
}

#include "ram_buffer.h"
#include "field_sprite.h"
#include "graphics/vram_tab.h"
#include "system/dma.h"

/// One step of two paired objects trading places: the effect work's halfword
/// at 26 names the first, and its partner is the other id of its even and odd
/// pair. On step 0 the partner rises 28 pixels, its sprite tiles are cleared
/// and the first's copied over them; on steps 1 to 5 it sinks and 0x30 words
/// more are copied; through step 7 the first flattens. In mode 1 both settle,
/// the first's flag is set and the partner's cleared, their animations step
/// and their cells are written again (Func_020006cc).
void Func_020007b4(s32 mode, s32 step) {
  s16 *pair = (s16 *)((u8 *)Ram_HeapSlots->effectWork + 26);
  s32 even = *pair - ((*(u16 *)pair & 1) << 1);
  struct Actor *first = Actor_GetById(*pair);
  struct Actor *second = Actor_GetById(even + 1);
  struct FieldSprite *firstSprite, *secondSprite;
  struct VramBlockCacheEntry *entry;

  if (step == 0) {
    firstSprite = first->sprite;
    secondSprite = second->sprite;
    second->motionFlags = 0;
    second->y += -0x1c0000;
    wait(1);
    entry = &gResourceTableEntries[secondSprite->vramBlock];
    second->watchedObject = (void *)(entry->offset + OBJ_VRAM0);
    entry = &gResourceTableEntries[firstSprite->vramBlock];
    first->watchedObject = (void *)(entry->offset + OBJ_VRAM0);
    Dma_Fill32(step, second->watchedObject, entry->size);
    Dma_Set(first->watchedObject, second->watchedObject, DMA_COPY_WORDS(192),
            REG_DMA3);
    first->watchedObject = (void *)((u8 *)first->watchedObject + 0x300);
    second->watchedObject = (void *)((u8 *)second->watchedObject + 0x300);
  } else if (step <= 5) {
    second->y += 0x53333;
    Dma_Set(first->watchedObject, second->watchedObject, DMA_COPY_WORDS(48),
            REG_DMA3);
    first->watchedObject = (void *)((u8 *)first->watchedObject + 0x100);
    second->watchedObject = (void *)((u8 *)second->watchedObject + 0x100);
  }
  if (step <= 7)
    first->scaleY += -0x1f00;
  if (mode == 1) {
    first->scaleY = 0x10000;
    second->y = second->terrainHeight;
    GameFlag_Set(first->timer);
    GameFlag_Clear(second->timer);
    Actor_SelectAnimation(
        first, ((struct FieldSprite *)first->sprite)->frame + 1);
    Actor_SelectAnimation(
        second, ((struct FieldSprite *)second->sprite)->frame - 1);
    Func_020006cc(first);
    Func_020006cc(second);
  }
}

#include "call.h"

// Places the objects of a list ended by 0xffff: each entry (three halfwords)
// names an object, the first of its three script choices and the flag that
// picks between them; the object stops moving, loses its solid bit and keeps
// the flag as its action parameter.
void Func_02000908(u16 *list) {
  struct Actor *object;
  s32 flag, base;
  u16 *entry;

  if (*list == 0xffff)
    return;
  entry = list;
  do {
    object = Actor_GetById(*entry);
    base = ((s16 *)entry)[1];
    flag = ((s16 *)entry)[2];
    Actor_SetSpriteFlags(object, 0);
    {
      s32 choice = GameFlag_Get((u16)flag);

      // The two shifts stay statements: as (u16)base, base loses its rank.
      base <<= 16;
      base = (u32)base >> 16;
      base += base << 1;
      base += choice;
    }
    Actor_SelectAnimation(object, base);
    object->motionFlags = 0;
    object->collisionFlags &= ~1;
    object->timer = flag;
    /* FAKEMATCH(wrapper): Func_020006cc called through Call1, the step behind
     * it; as a direct call with the step before it, 2 of 58 instructions differ
     * in tla-en. */
    Call1((void (*)())Func_020006cc, (s32)object);
    entry += 3;
  } while (*entry != 0xffff);
}

#include "main_routine_work.h"
#include "input.h"

void FieldCamera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void FieldCamera_FollowActor(s32 id, s32 flag);

void Func_02000988(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *ride = leader->watchedObject;
  struct Actor *rider, *dust;
  u8 *riding, *motion, *priority;
  s32 rise, dx, dz, moved, i, height;
  u16 facing;
  s32 angle, noFlags;

  Event_Start();
  Event_ResolvePendingActions(0);
  motion = &leader->motionFlags;
  *motion = 4;
  Actor_SetSpriteFlags(leader, 0);
  GameFlag_Set(0x250);
  Sound_Play(137);
  riding = (u8 *)ride + 99;
  rise = 0;
  while (*riding != 0) {
    leader->x = ride->x;
    leader->y = ride->y + rise;
    leader->z = ride->z;
    if (rise <= 0xbffff)
      rise += 0x1999;
    leader->facing = (u8)gFrameCount << 12;
    leader->priorityFlags ^= 2 << (gFrameCount & 1);
    wait(1);
  }
  Sound_Play(0x120);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room1 &&
      leader->x >> 19 == 39) {
    Event_SetExitRequest(16);
    Event_End();
    return;
  }
  priority = &leader->priorityFlags;
  facing = ride->facing;
  switch (facing) {
  case 0xc000:
    rider = ride->watchedObject;
    dx = 0;
    dz = 0;
    *priority = 4;
    leader->x = (((leader->x >> 19) & ~1) + 1) << 19;
    i = 0;
    leader->y = (((ride->y >> 19) & ~1) + 3) << 19;
    leader->z = (((leader->z >> 19) & ~1) + 1) << 19;
  waiting:
    moved = 0;
    if (gInput.held & 0x40) {
      dx = 0;
      dz = 0xfff00000;
      leader->facing = 0xc000;
      moved = 1;
    }
    if (gInput.held & 0x10) {
      dx = 0x100000;
      dz = 0;
      leader->facing = 0;
      moved = 1;
    }
    if (gInput.held & 0x20) {
      dx = 0xfff00000;
      leader->facing = 0x8000;
      dz = 0;
      moved = 1;
    }
    if (moved && Func_02000690(2, (ride->x + dx) >> 20,
                               (ride->z - ride->y + dz) >> 20) >>
                         8 !=
                     255)
      goto found;
    wait(1);
    i++;
    if (i != 60)
      goto waiting;
    dx = 0;
    dz = 0xfff00000;
    leader->facing = 0xc000;
    if ((u32)(Func_02000690(2, ride->x >> 20, (ride->z - ride->y + dz) >> 20)
              << 8) >>
            16 !=
        255)
      goto found;
    dx = 0x100000;
    dz = 0;
    leader->facing = 0;
    if ((u32)(Func_02000690(2, (ride->x + dx) >> 20, (ride->z - ride->y) >> 20)
              << 8) >>
            16 !=
        255)
      goto found;
    dx = 0xfff00000;
    leader->facing = 0x8000;
  found:
    height =
        Map_GetTerrainHeight(((u8 *)rider)[98], ride->x + dx, ride->z + dz);
    leader->y = height;
    leader->terrainHeight = height;
    Actor_SelectAnimation(leader, 6);
    wait(6);
    Sound_Play(152);
    Actor_SelectAnimation(leader, 7);
    leader->speedLimit = 0x20000;
    leader->acceleration = 0x8000;
    leader->velocityY = 0x40000;
    *motion = 2;
    *priority = 1;
    leader->terrainId = ((u8 *)rider)[98];
    Actor_SetMoveTarget(leader, leader->x + dx, leader->y,
                              leader->z + dz);
    OverlayObject_WaitUntilIdle(leader);
    Actor_WaitForMove(leader);
    Actor_SetSpriteFlags(leader, 1);
    *motion = 3;
    break;
  default:
    angle = ride->facing;
    leader->x = (((ride->x >> 19) & ~1) + (angle >>= 13) - 1) << 19;
    leader->z = (((ride->z >> 19) & ~1) - (angle & 2) + 1) << 19;
    noFlags = 0;
    Actor_SetSpriteFlags(leader, 1);
    leader->velocityY = 0x60000;
    *priority = 33;
    *motion = 3;
    Call4((void (*)())FieldCamera_MoveWithinMapBounds, -1, -1, -1, 0);
    dust = FieldCamera_GetActor();
    dust->speedLimit = 0x80000;
    dust->acceleration = 0x10000;
    dust->motionFlags = noFlags;
    Actor_SetMoveTarget(dust, leader->x,
                              Map_GetTerrainHeight(0, leader->x, leader->z),
                              leader->z);
    OverlayObject_WaitUntilIdle(leader);
    Actor_WaitForMove(dust);
    break;
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  GameFlag_Clear(0x250);
  wait(10);
  Event_End();
}

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

DEFINE_POSITION_DISTANCE_ABOVE(Position_DistanceAbove)

// Func_020010b4's twin for another shot: it also stops while the render mode
// is 5, and on a cell of kind 212 it ends with one of two end scripts by its
// angle; otherwise, the first time it reaches the leader (within 16 pixels on
// the same height when the leader climbs, else within 8 on the same level) and
// flag 0x104 is clear, it carries the leader along (counter 0x16a 200, render
// mode 0); it moves 1.5 pixels a frame along its angle, upward when its angle
// is 0xc000.

#include "iwram_call.h"
#include "field/event_work.h"

extern u8 gData_020061ac[], gData_02006194[];

s32 Trig_Sin(s32 angle);

/// Func_020010b4's twin for another shot: it also stops while the render mode
/// is 5, and on a cell of kind 212 it ends with one of two end scripts by its
/// angle; otherwise, the first time it reaches the leader (within 16 pixels on
/// the same height when the leader climbs, else within 8 on the same level) and
/// flag 0x104 is clear, it carries the leader along (counter 0x16a 200, render
/// mode 0); it moves 1.5 pixels a frame along its angle, upward when its angle
/// is 0xc000.
void Func_02000d48(struct Actor *shot) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u8 *work = Ram_HeapSlots->eventWork;
  s32 near;
  s32 *leaderPosition;
  s32 *shotPosition;
  s32 speed = 0x18000;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0 ||
      gMainRoutineWork.renderMode == 5) {
    Actor_SetAnimationSpeed(shot, 0);
    return;
  }
  Actor_SetAnimationSpeed(shot, 16);
  if (Func_02000690(2, shot->x >> 20, (shot->z - shot->y) >> 20) >> 8 == 212) {
    ((struct Shot *)shot)->unk63 = 0;
    shot->update = 0;
    Call2((void (*)())Func_02005f8c, (s32)shot, 539);
    if (shot->facing == 0xc000)
      Actor_StartScript(shot, gData_020061ac);
    else
      Actor_StartScript(shot, gData_02006194);
    return;
  }
  if (((struct Shot *)shot)->unk62 == 0) {
    near = 0;
    leaderPosition = &leader->x;
    shotPosition = &shot->x;
    if (gMainRoutineWork.renderMode == 2) {
      if (Position_DistanceAbove(leaderPosition, shotPosition) <= 16 &&
          leader->z >> 20 == shot->z >> 20)
        near = 1;
    } else {
      if (Position_FlatDistance(leaderPosition, shotPosition) <= 8 &&
          leader->y >> 20 == shot->y >> 20)
        near = 1;
    }
    if (near && !GameFlag_Get(0x104)) {
      leader->facing = shot->facing;
      leader->watchedObject = shot;
      leader->priorityFlags &= ~1;
      ((struct FieldSprite *)leader->sprite)->priority =
          ((struct FieldSprite *)shot->sprite)->priority;
      ((struct EventWork *)work)->unk16a = 200;
      gMainRoutineWork.renderMode = 0;
      ((struct Shot *)shot)->unk62 = 1;
      ((struct Shot *)shot)->unk63 = 1;
    }
  }
  if (shot->facing == 0xc000) {
    shot->x += Iwram_MulQ16(Trig_Cos(shot->facing), speed);
    shot->y -= Iwram_MulQ16(Trig_Sin(shot->facing), speed);
  } else {
    shot->x += Iwram_MulQ16(Trig_Cos(shot->facing), speed);
    shot->z += Iwram_MulQ16(Trig_Sin(shot->facing), speed);
  }
}

#include "system/field_effect.h"
#include "system/vector3.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

// A launcher's update with a new shot each time: while none of the event work's
// four busy counters is set it counts frames in its action parameter, and at
// 240 launches a shot 8 pixels ahead along its angle with a dust effect below
// it.
void Func_02000f34(struct Actor *object) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *shot;
  struct Vector3 position;
  struct EffectOptions options;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0)
    return;
  if (object->timer == 240) {
    if (!GameFlag_Get(0x250))
      Func_02005f8c(object, 202);
    if (object->facing == 0xc000) {
      position.x = object->x;
      position.z = object->z;
    } else {
      /* FAKEMATCH(wrapper): forwarded, so each product builds its own constant
       * and they share the routine's address; as direct calls, 106 of 153
       * instructions differ in tla-en. */
      position.x = object->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                      Trig_Cos(object->facing));
      position.z = object->z + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                      Trig_Sin(object->facing));
    }
    shot = (struct Actor *)Import_ActorCreate(280, position.x, object->y,
                                              position.z);
    Actor_SelectAnimation(shot, 2);
    Actor_SetSpriteFlags(shot, 0);
    ((struct FieldSprite *)shot->sprite)->priority = 1;
    shot->motionFlags = 0;
    ((u8 *)shot)[0x62] = 0;
    ((u8 *)shot)[0x63] = 0;
    shot->facing = object->facing;
    shot->update = Func_02000d48;
    shot->watchedObject = object;
    object->timer = 0;
    options.priority = 1;
    options.palette = 7;
    FieldEffect_Spawn(position.x, object->y, position.z - 0x60000, 0, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
  object->timer++;
}

void Func_02001084(struct Actor *object, s16 timer, u8 value) {
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    BYTE_AT(object, 90) = 0;
    object->timer = timer;
    BYTE_AT(object, 98) = value;
    WORD_AT(object, 108) = (s32)Func_02000f34;
  }
}

extern u8 gData_020061cc[];

void Func_020010b4(struct Actor *shot) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u8 *work = Ram_HeapSlots->eventWork;
  s32 near;
  s32 *leaderPosition;
  s32 *shotPosition;
  s32 speed = 0x18000;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0 ||
      *(u16 *)(work + 0x1b2) == 0) {
    Actor_SetAnimationSpeed(shot, 0);
    return;
  }
  Actor_SetAnimationSpeed(shot, 16);
  if (Func_02000690(2, shot->x >> 20, (shot->z - shot->y) >> 20) >> 8 == 212) {
    {
      s32 cue = 284;

      Func_02005f8c(shot, cue + 255);
    }
    ((struct Shot *)shot)->unk63 = 0;
    shot->update = 0;
    Actor_StartScript(shot, gData_020061cc);
    /* FAKEMATCH(dead): an empty once-only block holds the next statement after
     * this one; as no block, 2 of 211 instructions differ in tla-en. */
    do {
    } while (0);
    return;
  }
  if (((struct Shot *)shot)->unk62 == 0) {
    near = 0;
    leaderPosition = &leader->x;
    shotPosition = &shot->x;
    if (gMainRoutineWork.renderMode == 2) {
      if (Position_DistanceAbove(leaderPosition, shotPosition) <= 16 &&
          leader->z >> 20 == shot->z >> 20)
        near = 1;
    } else {
      if (Position_FlatDistance(leaderPosition, shotPosition) <= 8 &&
          leader->y >> 20 == shot->y >> 20)
        near = 1;
    }
    if (near && !GameFlag_Get(0x104)) {
      leader->facing = shot->facing;
      leader->watchedObject = shot;
      leader->priorityFlags &= ~1;
      ((struct FieldSprite *)leader->sprite)->priority =
          ((struct FieldSprite *)shot->sprite)->priority;
      ((struct EventWork *)work)->unk16a = 200;
      gMainRoutineWork.renderMode = 0;
      ((struct Shot *)shot)->unk62 = 1;
      ((struct Shot *)shot)->unk63 = 1;
    }
  }
  if (shot->facing == 0xc000) {
    shot->x += Iwram_MulQ16(Trig_Cos(shot->facing), speed);
    shot->y -= Iwram_MulQ16(Trig_Sin(shot->facing), speed);
  } else {
    shot->x += Iwram_MulQ16(Trig_Cos(shot->facing), speed);
    shot->z += Iwram_MulQ16(Trig_Sin(shot->facing), speed);
  }
}

// An object's update: while none of the event work's four busy counters (0x15a,
// 0x15e, 0x166, 0x168) is set it counts frames in its action parameter, and at
// 240 (playing cue 202 unless flag 0x250 is set) it launches the first free one
// of objects 13 to 15 from 8 pixels ahead along its angle, with Func_020010b4
// as its update, and a dust effect below it.
void Func_02001290(struct Actor *object) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *shot;
  struct Vector3 position;
  struct EffectOptions options;
  s32 i;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0)
    return;
  for (i = 0; i != 3;)
    Actor_GetById(13 + i++);
  if (object->timer == 240) {
    shot = 0;
    if (!GameFlag_Get(0x250))
      Func_02005f8c(object, 202);
    if (object->facing == 0xc000) {
      position.x = object->x;
      position.z = object->z;
    } else {
      /* FAKEMATCH(wrapper): forwarded, so each product builds its own 0x80000
       * and they share the routine's address; as direct calls, 129 of 184
       * instructions differ in tla-en. */
      position.x = object->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                      Trig_Cos(object->facing));
      position.z = object->z + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                      Trig_Sin(object->facing));
    }
    for (i = 0; i != 3; i++) {
      shot = Actor_GetById(13 + i);
      if (shot->update == 0)
        break;
    }
    shot->x = position.x;
    shot->y = object->y;
    shot->z = position.z;
    Actor_SelectAnimation(shot, 2);
    Actor_SetSpriteFlags(shot, 0);
    ((struct FieldSprite *)shot->sprite)->priority = 1;
    shot->priorityFlags |= 32;
    shot->motionFlags = 0;
    ((u8 *)shot)[0x62] = 0;
    ((u8 *)shot)[0x63] = 0;
    shot->facing = object->facing;
    shot->watchedObject = object;
    shot->update = Func_020010b4;
    object->timer = 0;
    options.priority = 1;
    options.palette = 7;
    FieldEffect_Spawn(position.x, object->y, position.z - 0x60000, 0, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
  object->timer++;
}

void Func_02001420(struct Actor *object, s16 timer, u8 value) {
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    BYTE_AT(object, 90) = 0;
    object->timer = timer;
    BYTE_AT(object, 98) = value;
    WORD_AT(object, 108) = (s32)Func_02001290;
    Actor_SetSpriteFlags(object, 0);
  }
}

void Func_02001458(struct Actor *object) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *other;
  s32 *pos;
  u8 i;

  if (*(s8 *)((u8 *)Ram_HeapSlots->block32 + 0x2a01) != 0)
    return;
  if (*(u16 *)(work + 0x1b2) == 0)
    return;
  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0) {
    Actor_SetAnimationSpeed(object, 0);
    return;
  }
  Actor_SetAnimationSpeed(object, 16);
  if (object->timer == 0) {
    if (object->x == 0) {
      Actor_Release(object);
    } else {
      object->z = 0;
      object->x = 0;
    }
    return;
  }
  if (Position_FlatDistance(&leader->x, pos = &object->x) <= 8 &&
      ((s16 *)&leader->y)[1] == ((s16 *)&object->y)[1]) {
    {
      s16 *counter = (s16 *)(work + 0x16a);
      s32 value = 201;

      *counter = value;
    }
    object->unk4c = 0;
    /* FAKEMATCH(barrier): an empty asm holds the next statement after this
     * one; as no asm, 22 of 200 instructions differ in tla-en. */
    asm("");
    object->speedLimit = -object->speedLimit >> 1;
    object->z = leader->z - 0x80000;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room3) {
    other = Actor_GetById(8);
    if (Position_FlatDistance(&other->x, pos) <= 8) {
      object->speedLimit = -object->speedLimit >> 1;
      object->unk4c = 0;
      object->z = other->z - 0x80000;
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room8) {
    for (i = 0; i != 3; i++) {
      struct Actor *other2 = Actor_GetById(i + 12);
#define other other2
      if (Position_FlatDistance(&other->x, pos) <= 8) {
        object->speedLimit = -object->speedLimit >> 1;
        object->unk4c = 0;
        object->z = other->z - 0x80000;
      }
    }
#undef other
  }
  object->timer--;
  object->z += object->speedLimit;
  object->x += object->acceleration;
  object->speedLimit -= object->unk4c;
  object->scaleX += 0xccc;
  object->scaleY += 0xccc;
}

void Func_0200160c(s32 x, s32 y, s32 z) {
  struct Actor *object = Import_ActorCreate(0x11d, x, y, z);

  Actor_SelectAnimation(object, 1);
  Actor_SetSpriteFlags(object, 0);
  Actor_SetSpriteColor(object, 6);
  Actor_SetBlendMode(object, 1);
  object->timer = 27;
  object->motionFlags = 0;
  BYTE_AT(object, 90) = 0;
  WORD_AT(object, 24) = 0x8000;
  object->speedLimit = 0x40000;
  object->acceleration = ((Random16() & 0x7fff) << 1) + 0xffff8001;
  WORD_AT(object, 76) = 0x3800;
  WORD_AT(object, 108) = (s32)Func_02001458;
}

/// Every eighth frame pair, while none of the event work's four busy counters
/// is set, starts a drip (Func_0200160c) at a place chosen by scene and frame:
/// in one room one of two spots by flag 0x89b, in the other one of six by the
/// leader's position and flags 0x200 to 0x202. Places the two frame arms share
/// are one call, reached by goto from the other arm.
void Func_02001690(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  u32 frame;
  s32 scene;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0)
    return;
  frame = gFrameCount;
  if ((frame & 6) != 0)
    return;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov664Room3) {
    s32 x;

    if ((frame & 1) && !GameFlag_Get(0x89b))
      x = 0x1780000;
    else
      x = 0x1180000;
    Func_0200160c(x, 0, 0x2680000);
  } else if (scene == (s32)&SceneId_Ov664Room8) {
    if (frame & 1) {
      if (Actor_GetById(gMainRoutineWork.currentOwner)->z >> 19 > 41) {
        if (Actor_GetById(gMainRoutineWork.currentOwner)->x >> 19 <= 44)
        dripA8:
          Func_0200160c(0xa80000, 0, 0x25b0000);
        else if (!GameFlag_Get(0x201))
          Func_0200160c(0x1f80000, 0, 0x1c80000);
        else
          goto drip228;
      } else {
      drip268:
        Func_0200160c(0x2680000, -0x200000, 0x880000);
      }
    } else {
      if (Actor_GetById(gMainRoutineWork.currentOwner)->z >> 19 > 41) {
        if (Actor_GetById(gMainRoutineWork.currentOwner)->x >> 19 > 49)
        drip228:
          Func_0200160c(0x2280000, 0, 0x1c80000);
        else if (GameFlag_Get(0x200))
          goto dripA8;
        else
          Func_0200160c(0x1080000, 0, 0x2580000);
      } else if (GameFlag_Get(0x202)) {
        goto drip268;
      } else {
        Func_0200160c(0x2880000, -0x200000, 0x880000);
      }
    }
  }
}

#include "macros.h"

void Func_020017f4(struct Actor *object) {
  struct Actor *target = (struct Actor *)WORD_AT(object, 104);

  object->priorityFlags ^= 2 << (gFrameCount & 1);
  SET_POSITION(object, target->x, target->y + (128 << 12), target->z);
}

void Func_02002bd0(void);
void Camera_SetSpeedParameters(s32 a, s32 b);
void MapGrid_SetScreenShake(s32 a, s32 b, s32 c);

#define TARGET(effect) ((effect)->target)

/// The gust picks up an object it passes over, to drop it at cell `at`
/// (x in the high half, z in the low).
#define PICK_UP(object, at)                                                    \
  do {                                                                         \
    (object)->watchedObject = gust;                                            \
    (object)->update = Func_020017f4;                                          \
    (object)->motionFlags = 0;                                                 \
    (object)->priorityFlags &= ~1;                                             \
    Actor_SetSpriteFlags(object, 0);                                           \
    ((struct FieldSprite *)(object)->sprite)->priority =                       \
        ((struct FieldSprite *)gust->sprite)->priority;                        \
    gust->watchedObject = (object);                                            \
    carried = (at);                                                            \
  } while (0)

/// One shake of the walls (cue 157).
#define SHAKE()                                                                \
  (Sound_Play(157), Call3(MapGrid_SetScreenShake, 0x50000, 0x50000, 0x10000),  \
   Call3(MapGrid_SetScreenShake, -1, -1, 0xe666))

void Func_02001828(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectWork *effect = (struct EffectWork *)Ram_HeapSlots->effectWork;
  struct Actor *gust, *spark, *object;
  struct Vector3 velocity;
  struct EffectOptions leaves, dust;
  struct Cell dir, cell;
  s32 i, steps = 0, carried;
  // Default: nothing reads this 0 before the loop sets hit (without it, 261
  // of 1527 instructions differ in tla-en).
  s32 hit = 0;

  /* FAKEMATCH(wrapper): calls forwarded through call.h's Call2, Call3, Call4
   * and Call6, so each builds its own constants; as direct calls, 416 of 1527
   * instructions differ in tla-en. */
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(207);
  gust =
      (struct Actor *)Import_ActorCreate(280, leader->x, leader->y, leader->z);
  Actor_SelectAnimation(gust, 2);
  Actor_SetSpriteFlags(gust, 0);
  gust->scaleX = 0x1b333;
  gust->scaleY = 0x3333;
  gust->motionFlags = 0;
  /* FAKEMATCH(signature): the target's word read as long, an alias set outside
   * struct Actor's, so the load need not wait for the motion flags' store; as
   * TARGET(effect), 2 of 1527 instructions differ in tla-en. */
  ((struct FieldSprite *)gust->sprite)->priority =
      ((struct FieldSprite *)((struct Actor *)*(long *)&effect->target)->sprite)
          ->priority;
  Actor_SetMoveTarget(gust, TARGET(effect)->x, TARGET(effect)->y,
                            TARGET(effect)->z + 0xffff0000);
  gust->terrainHeight = Map_GetTerrainHeight(0, gust->x, gust->z);
  while (gust->scaleY <= 0x14ccb) {
    gust->scaleY += 0x1999;
    FieldEvent_Wait(1);
  }
  Actor_WaitForMove(gust);
  gust->z = TARGET(effect)->z + 0xffff0000;
  while (gust->scaleY <= 0x1cccb) {
    if ((gFrameCount & 3) == 0) {
      spark = (struct Actor *)Import_ActorCreate(
          0x2a0, TARGET(effect)->x, TARGET(effect)->y, TARGET(effect)->z);
      spark->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->terrainHeight = Map_GetTerrainHeight(0, spark->x, spark->z);
      spark->bounce = spark->x;
      spark->gravity = 0xcccc;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gData_02005fd8);
      Actor_SetSpriteFlags(spark, 0);
      Actor_SetSpriteColor(spark, 2);
      spark->update = Func_02002bd0;
    }
    if (gFrameCount & 1) {
      gust->scaleX += 655;
      gust->scaleY += 655;
      Actor_SetSpriteColor(TARGET(effect), 15);
    } else {
      Actor_SetSpriteColor(TARGET(effect), 4);
    }
    FieldEvent_Wait(1);
  }
  Actor_SetSpriteColor(TARGET(effect), 0);
  Sound_Play(136);
  cell.x = TARGET(effect)->x >> 20;
  cell.z = TARGET(effect)->z >> 20;
  if (leader->x >> 20 < cell.x) {
    dir.x = 1;
    dir.z = 0;
  } else if (leader->x >> 20 > cell.x) {
    dir.x = -1;
    dir.z = 0;
  } else if (leader->z >> 20 < cell.z) {
    dir.x = 0;
    dir.z = 1;
  } else {
    dir.x = 0;
    dir.z = -1;
  }
  do {
    hit = Func_02000690(2, cell.x, cell.z);
    hit >>= 8;
    if (Map_GetTerrainHeight(2, cell.x << 20, cell.z << 20) !=
        gust->terrainHeight)
      hit = 0;
    cell.x += dir.x;
    cell.z += dir.z;
    steps += 2;
  } while (hit == 0);
  Func_02000690(2, cell.x, cell.z);
  {
    s32 x;
    s32 z;

    steps -= 2;
    x = (TARGET(effect)->x >> 16) + ((steps * dir.x) << 3);
    z = (TARGET(effect)->z >> 16) + ((steps * dir.z) << 3);
    ((struct FieldSprite *)gust->sprite)->priority = 1;
    cell.x = x;
    cell.z = z;
    gust->speedLimit = 0x40000;
    gust->acceleration = 0x40000;
    Actor_SetMoveTarget(gust, cell.x << 16, gust->y, cell.z << 16);
    Call2((void (*)())Camera_SetSpeedParameters, 0x50000, 0x50000);
    Call4((void (*)())FieldCamera_MoveWithinMapBounds, x << 16, -1, z << 16, 1);
  }
  carried = 0;
  while (Actor_HasArrived(gust) == 0) {
    if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room6) {
      object = Actor_GetById(8);
      if (gust->x >> 19 == object->x >> 19 && carried == 0)
        PICK_UP(object, 0x270025);
    } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room8) {
      object = Actor_GetById(9);
      if (gust->x >> 20 == object->x >> 20 &&
          gust->z >> 20 == object->z >> 20 && carried == 0)
        PICK_UP(object, 0x290069);
    } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room9) {
      object = Actor_GetById(10);
      if (gust->x >> 20 == object->x >> 20 &&
          gust->z >> 20 == object->z >> 20 && carried == 0)
        PICK_UP(object, 0xf0033);
    }
    if ((gFrameCount & 1) == 0) {
      leaves.priority = 1;
      leaves.type = 674;
      leaves.palette = 2;
      velocity.x = Trig_Cos((gFrameCount & 0x1ff) << 12) << 1;
      velocity.y = gust->y + ((Random16() & 31) << 16);
      velocity.z = Trig_Sin((gFrameCount & 0x1ff) << 12);
      FieldEffect_Spawn(
          gust->x, velocity.y, gust->z, velocity.x, 0, velocity.z,
          EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &leaves);
    }
    FieldEvent_Wait(1);
  }
  if (carried != 0) {
    object = gust->watchedObject;
    object->x = (carried >> 16) << 19;
    object->z = carried << 19;
    object->velocityY = 0x80000;
    object->update = 0;
    object->motionFlags = 2;
    object->priorityFlags |= 1;
    object->priorityFlags &= ~6;
    Actor_SetSpriteFlags(object, 1);
    Scene_SetActorSpritePriority(object, 2);
    if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room6) {
      object->terrainHeight = 0xffe00000;
      Call6(Map_CopyCellAttributeRect, 19, 16, 1, 1, 19, 18);
      GameFlag_Set(0x89f);
    } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room8) {
      Call6(Map_CopyCellAttributeRect, 19, 38, 1, 1, 20, 38);
      OverlayObject_WaitUntilIdle(object);
      wait(1);
      OverlayObject_WaitUntilIdle(object);
      object->motionFlags = 0;
      object->terrainHeight = 0;
      object->y = 0;
      MapGrid_SetCellHeight(0x1400000, 0x3400000, 0, 4);
    } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room9) {
      OverlayObject_WaitUntilIdle(object);
      wait(1);
      OverlayObject_WaitUntilIdle(object);
      Actor_SetSpriteFlags(object, 0);
      object->motionFlags = 0;
      object->terrainHeight = 0;
      object->y = 0;
      MapGrid_SetCellHeight(0x780000, 0x1980000, 0, 4);
      if (GameFlag_Get(0x899))
        Func_02001084(object, 60, 0);
    }
  }
  if (hit == 255) {
    Sound_Play(136);
  } else {
    cell.x = (gust->x >> 16) + dir.x * 48;
    cell.z = (gust->z >> 16) + dir.z * 48;
    gust->speedLimit = 0x8000;
    gust->acceleration = 0x8000;
    Call4((void (*)())Actor_SetMoveTarget, (s32)(gust),
          (s32)(cell.x << 16), (s32)(gust->y), (s32)(cell.z << 16));
    hit -= 30;
    i = 0;
    while (Actor_HasArrived(gust) == 0) {
      if (hit > 4)
        goto shrink;
      if (i == 0) {
        Sound_Play(235);
        i = 1;
      }
      if ((gFrameCount & 1) == 0) {
        dust.priority = 1;
        {
          dust.startScaleX = dust.startScaleY =
              (Random16() & 0xffff) + 0x10000;
        }
        velocity.x = Trig_Cos((gFrameCount & 0x1ff) << 12) << 1;
        velocity.y = 0x10000;
        velocity.z = Trig_Sin((gFrameCount & 0x1ff) << 12);
        FieldEffect_Spawn(gust->x, gust->y, gust->z, velocity.x, velocity.y,
                          velocity.z,
                          EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY, &dust);
      }
      if ((gFrameCount & 3) == 0) {
        if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room4) {
          GameFlag_Set(0x300);
          cell.x = (gust->x >> 20) + 64;
          cell.z = gust->z >> 20;
          MapGrid_CopyCellPictureIndicesRect(90, 25, cell.x + 1, cell.z, 1, 3);
          MapGrid_CopyCellPictureIndicesRect(21, 88, 24, 89, 1, 2);
          Map_CopyCellAttributeRect(23, 92, 2, 1, 23, 89);
          Map_CopyCellAttributeRect(18, 33, 2, 1, 23, 25);
        } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room7) {
          cell.x = gust->x >> 20;
          cell.z = ((gust->z - gust->y) >> 20) + 64;
          if (hit == 0) {
            GameFlag_Set(772);
            MapGrid_CopyCellPictureIndicesRect(39, 82, cell.x, cell.z, 1, 1);
            Map_CopyCellAttributeRect(39, 82, 1, 1, 38, 82);
            Call6(Map_CopyCellAttributeRect, 38, 19, 1, 1, 38, 18);
          } else {
            GameFlag_Set(773);
            Call6(MapGrid_CopyCellPictureIndicesRect, 39, 82, cell.x, cell.z, 1,
                  1);
            Call6(Map_CopyCellAttributeRect, 38, 98, 1, 1, 38, 99);
            Call6(Map_CopyCellAttributeRect, 38, 35, 1, 1, 38, 37);
          }
        } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room9) {
          GameFlag_Set(774);
          cell.x = gust->x >> 20;
          Call6(MapGrid_CopyCellPictureIndicesRect, 15, 80, cell.x, 77, 1, 3);
          Call6(Map_CopyCellAttributeRect, 15, 80, 1, 1, 15, 79);
          Call6(Map_CopyCellAttributeRect, 17, 15, 2, 1, 15, 15);
        }
        Map_CenterOnFollowedPoint();
      }
      FieldEvent_Wait(1);
    }
    Sound_Play(298);
  }
shrink:
  gust->acceleration = 0;
  gust->speedLimit = 0;
  gust->targetZ = 0;
  gust->targetY = 0;
  gust->targetX = 0;
  for (i = 0; i != 8; i++) {
    gust->scaleX -= 0x2666;
    gust->scaleY -= 0x4000;
    gust->y += 0x38000;
    FieldEvent_Wait(1);
  }
  Actor_Release(gust);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room4) {
    if (hit == 5) {
      SHAKE();
      Call6(MapGrid_CopyCellPictureIndicesRect, 79, 0x40, 29, 85, 5, 7);
      FieldEvent_Wait(30);
      SHAKE();
      Call6(MapGrid_CopyCellPictureIndicesRect, 69, 0x40, 29, 85, 5, 7);
      Call6(Map_CopyCellAttributeRect, 46, 20, 5, 5, 29, 21);
      Call6(Map_CopyCellAttributeRect, 0x20, 89, 1, 1, 31, 89);
      Call6(Map_CopyCellAttributeRect, 50, 86, 1, 1, 33, 87);
      GameFlag_Set(769);
    } else if (hit == 6) {
      SHAKE();
      Call6(MapGrid_CopyCellPictureIndicesRect, 74, 0x40, 29, 85, 5, 7);
      FieldEvent_Wait(30);
      SHAKE();
      Call6(MapGrid_CopyCellPictureIndicesRect, 0x40, 0x40, 29, 85, 5, 7);
      Call6(Map_CopyCellAttributeRect, 52, 20, 5, 5, 29, 21);
      Call6(Map_CopyCellAttributeRect, 33, 88, 1, 1, 33, 87);
      Call6(Map_CopyCellAttributeRect, 0x30, 88, 1, 1, 31, 89);
      GameFlag_Clear(769);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room6) {
    if (hit == 5) {
      SHAKE();
      Call6(MapGrid_CopyCellPictureIndicesRect, 79, 0x40, 0x20, 79, 5, 7);
      if (Actor_GetById(8)->x >> 19 == 65) {
        velocity.x = Actor_GetById(8)->x + 0xfff80000;
        velocity.y = Actor_GetById(8)->y;
        velocity.z = Actor_GetById(8)->z + 0x80000;
        Actor_SetSpritePriority(8, 2);
        for (i = 0; i != 8; i++) {
          dust.priority = 1;
          FieldEffect_Spawn(velocity.x + ((Random16() & 15) << 16),
                            velocity.y + ((Random16() & 31) << 16),
                            velocity.z, 0, 0, -0x8000, EFFECT_USE_PRIORITY,
                            &dust);
        }
        wait(30);
        SHAKE();
        Call6(MapGrid_CopyCellPictureIndicesRect, 0x40, 0x40, 0x20, 79, 5, 7);
        Actor_GetById(8)->priorityFlags |= 1;
      } else {
        FieldEvent_Wait(30);
        SHAKE();
        Call6(MapGrid_CopyCellPictureIndicesRect, 69, 0x40, 0x20, 79, 5, 7);
        Call6(Map_CopyCellAttributeRect, 49, 15, 5, 5, 0x20, 15);
        Call6(Map_CopyCellAttributeRect, 33, 82, 1, 1, 34, 82);
        Call6(Map_CopyCellAttributeRect, 52, 81, 1, 1, 36, 81);
        GameFlag_Clear(770);
      }
    } else if (hit == 6) {
      SHAKE();
      if (Actor_GetById(8)->x >> 19 == 65) {
        velocity.x = Actor_GetById(8)->x + 0xfff80000;
        velocity.y = Actor_GetById(8)->y;
        velocity.z = Actor_GetById(8)->z;
        for (i = 0; i != 8; i++) {
          dust.priority = 1;
          FieldEffect_Spawn(velocity.x + ((Random16() & 15) << 16),
                            velocity.y,
                            velocity.z - ((Random16() & 31) << 16), 0, 0,
                            -0x8000, EFFECT_USE_PRIORITY, &dust);
        }
        wait(20);
      } else {
        Call6(MapGrid_CopyCellPictureIndicesRect, 79, 0x40, 0x20, 79, 5, 7);
        Sound_Play(157);
        FieldEvent_Wait(30);
        SHAKE();
        Call6(MapGrid_CopyCellPictureIndicesRect, 0x40, 0x40, 0x20, 79, 5, 7);
        Call6(Map_CopyCellAttributeRect, 49, 21, 5, 5, 0x20, 15);
        Call6(Map_CopyCellAttributeRect, 35, 81, 1, 1, 36, 81);
        Call6(Map_CopyCellAttributeRect, 50, 82, 1, 1, 34, 82);
        GameFlag_Set(770);
      }
    }
  }
  FieldEvent_Wait(30);
  FieldCamera_MoveToActorById(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

extern struct ActorPlacement gData_02006238[];
extern const u8 gData_02006268[];

const u8 *Scene_GetEntrances(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room1)
    return gData_02006268;
  return gData_02006238;
}

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "scene_tables.h"

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room1)
    return gData_02006564;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room2)
    return gData_0200663c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room3)
    return gData_020066b4;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room4)
    return gData_0200675c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room5)
    return gData_020068ac;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room6)
    return gData_020068f4;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room7)
    return gData_020069e4;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room8)
    return gData_02006aec;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room9)
    return gData_02006bac;
  return gData_0200654c;
}

void Func_020026c0(void) {
  struct Actor *object = Actor_GetById(Party_GetLeaderActor());
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  s32 key = (x << 16) + z;
  s32 last;
  s32 value;
  u8 *work;

  if (object->velocityX != 0 || object->velocityZ != 0)
    return;
  last = gData_02006c0c;
  if (last == -1 || key == last)
    return;
  x = last >> 16;
  z = last & 0xffff;
  gData_02006c0c = -1;
  CopyCells(73, 72, gData_020073dc, gData_020073e0, gData_020073e4,
            gData_020073e8);
  if (gMainRoutineWork.scene != (s32)&SceneId_Ov664Room5)
    return;
  work = Ram_HeapSlots->eventWork;
  value = Func_02000690(2, x, z) >> 8;
  if (value != 0) {
    /* FAKEMATCH(temporary): the value plus 200 held in delay before the
       store; as *(s16 *)(work + 362) = value + 200, 6 of 70 instructions
       differ in tla-en. */
    s32 delay = value + 200;
    *(s16 *)(work + 362) = delay;
  }
}
