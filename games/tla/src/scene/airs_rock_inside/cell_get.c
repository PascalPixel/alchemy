/// @file
/// Scene code of overlay 0x664, in ROM order, from cell_get's offset up to the next object the overlay links.

#include "airs_rock_inside.h"

#include "airs_rock_inside.h"

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

void Func_0200062c(s32 layer, s32 x, s32 z, s32 cell);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
s32 GameFlag_Test(s32 flag);
void Func_02005df4(s32 x, s32 z, s32 terrain, s32 height);

/// Sets an object's cells by its flag (its action parameter): when the flag is
/// set it clears the kind byte of its layer 2 cell and its layer 0 cell, sinks
/// 4 pixels when not on its ground and takes sprite priority 3; otherwise it
/// marks both cells (0xff00 and 0xd480), rises 4 pixels and takes priority 1;
/// then it writes both cells back and moves the object's height
/// (Func_02005df4).
void Func_020006cc(struct Actor *object) {
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  s32 ground = Func_02000690(0, x, z);
  s32 cell = Func_02000690(2, x, z);
  s32 height =
      Map_GetTerrainHeight(object->terrainId, object->x, object->z) >> 19;
  s32 set = GameFlag_Test(object->action);
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
  Func_02005df4(px, pz, object->terrainId, height);
}

#include "ram_buffer.h"
#include "field_sprite.h"
#include "graphics/vram_tab.h"
#include "system/dma.h"

void wait(s32 frames);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_020006cc(struct Actor *object);

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
    firstSprite = first->animation;
    secondSprite = second->animation;
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
    GameFlag_SetBit(first->action);
    GameFlag_ClearBit(second->action);
    Func_02005d5c(first, ((struct FieldSprite *)first->animation)->frame + 1);
    Func_02005d5c(second, ((struct FieldSprite *)second->animation)->frame - 1);
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
    Actor_SetSingleChildFlags(object, 0);
    {
      s32 choice = GameFlag_Test((u16)flag);

      // The two shifts stay statements: as (u16)base, base loses its rank.
      base <<= 16;
      base = (u32)base >> 16;
      base += base << 1;
      base += choice;
    }
    Func_02005d5c(object, base);
    object->motionFlags = 0;
    object->collisionFlags &= ~1;
    object->action = flag;
    /* FAKEMATCH(wrapper): Func_020006cc called through Call1, the step behind
     * it; as a direct call with the step before it, 2 of 58 instructions differ
     * in tla-en. */
    Call1((void (*)())Func_020006cc, (s32)object);
    entry += 3;
  } while (*entry != 0xffff);
}

#include "main_routine_work.h"
#include "input.h"

extern char SceneId_Ov664Room1;
void Func_02005efc(s32 a);
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005f2c(s32 skip);
void Func_02005f94(s32 cue);
void Func_02005ee4(void);
struct Actor *Func_02005ef4(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02005d8c(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02005d94(struct Actor *object);
void OverlayObject_WaitUntilIdle(struct Actor *object);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
s32 Camera_FollowActor(s32 id, s32 a);

void Func_02000988(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *ride = leader->watchedObject;
  struct Actor *rider, *dust;
  u8 *riding, *motion, *priority;
  s32 rise, dx, dz, moved, i, height;
  u16 facing;
  s32 angle, noFlags;

  Func_02005e14();
  Func_02005f2c(0);
  motion = &leader->motionFlags;
  *motion = 4;
  Actor_SetSingleChildFlags(leader, 0);
  GameFlag_SetBit(0x250);
  Func_02005f94(137);
  riding = (u8 *)ride + 99;
  rise = 0;
  while (*riding != 0) {
    leader->x = ride->x;
    leader->y = ride->y + rise;
    leader->z = ride->z;
    if (rise <= 0xbffff)
      rise += 0x1999;
    leader->angle = (u8)gFrameCount << 12;
    leader->priorityFlags ^= 2 << (gFrameCount & 1);
    wait(1);
  }
  Func_02005f94(0x120);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room1 &&
      leader->x >> 19 == 39) {
    Func_02005efc(16);
    Func_02005e1c();
    return;
  }
  priority = &leader->priorityFlags;
  facing = ride->angle;
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
      leader->angle = 0xc000;
      moved = 1;
    }
    if (gInput.held & 0x10) {
      dx = 0x100000;
      dz = 0;
      leader->angle = 0;
      moved = 1;
    }
    if (gInput.held & 0x20) {
      dx = 0xfff00000;
      leader->angle = 0x8000;
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
    leader->angle = 0xc000;
    if ((u32)(Func_02000690(2, ride->x >> 20, (ride->z - ride->y + dz) >> 20)
              << 8) >>
            16 !=
        255)
      goto found;
    dx = 0x100000;
    dz = 0;
    leader->angle = 0;
    if ((u32)(Func_02000690(2, (ride->x + dx) >> 20, (ride->z - ride->y) >> 20)
              << 8) >>
            16 !=
        255)
      goto found;
    dx = 0xfff00000;
    leader->angle = 0x8000;
  found:
    height =
        Map_GetTerrainHeight(((u8 *)rider)[98], ride->x + dx, ride->z + dz);
    leader->y = height;
    leader->terrainHeight = height;
    Func_02005d5c(leader, 6);
    wait(6);
    Func_02005f94(152);
    Func_02005d5c(leader, 7);
    leader->speedLimit = 0x20000;
    leader->acceleration = 0x8000;
    leader->velocityY = 0x40000;
    *motion = 2;
    *priority = 1;
    leader->terrainId = ((u8 *)rider)[98];
    Func_02005d8c(leader, leader->x + dx, leader->y, leader->z + dz);
    OverlayObject_WaitUntilIdle(leader);
    Func_02005d94(leader);
    Actor_SetSingleChildFlags(leader, 1);
    *motion = 3;
    break;
  default:
    angle = ride->angle;
    leader->x = (((ride->x >> 19) & ~1) + (angle >>= 13) - 1) << 19;
    leader->z = (((ride->z >> 19) & ~1) - (angle & 2) + 1) << 19;
    noFlags = 0;
    Actor_SetSingleChildFlags(leader, 1);
    leader->velocityY = 0x60000;
    *priority = 33;
    *motion = 3;
    Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
    dust = Func_02005ef4();
    dust->speedLimit = 0x80000;
    dust->acceleration = 0x10000;
    dust->motionFlags = noFlags;
    Func_02005d8c(dust, leader->x,
                  Map_GetTerrainHeight(0, leader->x, leader->z), leader->z);
    OverlayObject_WaitUntilIdle(leader);
    Func_02005d94(dust);
    break;
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02005ee4();
  GameFlag_ClearBit(0x250);
  wait(10);
  Func_02005e1c();
}

s32 Func_02000cec(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;

  return Iwram_Sqrt(dx * dx + dz * dz);
}

s32 Func_02000d18(s32 *a, s32 *b) {
  s32 dx = (*a++ - *b++) >> 16;
  s32 dy = (*a - *b + 0x60000) >> 16;

  return Iwram_Sqrt(dx * dx + dy * dy);
}

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
struct Shot {
  u8 unk00[0x62];
  u8 unk62; ///< set once it has hit the leader
  u8 unk63; ///< set with unk62, cleared when it ends
};

void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
void Func_02005f8c(struct Actor *object, s32 cue);
s32 Func_02000d18(s32 *a, s32 *b);
s32 Func_02000cec(s32 *a, s32 *b);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);

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
    if (shot->angle == 0xc000)
      Func_02005d6c(shot, gData_020061ac);
    else
      Func_02005d6c(shot, gData_02006194);
    return;
  }
  if (((struct Shot *)shot)->unk62 == 0) {
    near = 0;
    leaderPosition = &leader->x;
    shotPosition = &shot->x;
    if (gMainRoutineWork.renderMode == 2) {
      if (Func_02000d18(leaderPosition, shotPosition) <= 16 &&
          leader->z >> 20 == shot->z >> 20)
        near = 1;
    } else {
      if (Func_02000cec(leaderPosition, shotPosition) <= 8 &&
          leader->y >> 20 == shot->y >> 20)
        near = 1;
    }
    if (near && !GameFlag_Test(0x104)) {
      leader->angle = shot->angle;
      leader->watchedObject = shot;
      leader->priorityFlags &= ~1;
      ((struct FieldSprite *)leader->animation)->priority =
          ((struct FieldSprite *)shot->animation)->priority;
      ((struct EventWork *)work)->unk16a = 200;
      gMainRoutineWork.renderMode = 0;
      ((struct Shot *)shot)->unk62 = 1;
      ((struct Shot *)shot)->unk63 = 1;
    }
  }
  if (shot->angle == 0xc000) {
    shot->x += Iwram_MulQ16(Math_Cosine(shot->angle), speed);
    shot->y -= Iwram_MulQ16(Math_Sine(shot->angle), speed);
  } else {
    shot->x += Iwram_MulQ16(Math_Cosine(shot->angle), speed);
    shot->z += Iwram_MulQ16(Math_Sine(shot->angle), speed);
  }
}

#include "system/field_effect.h"
#include "system/vector3.h"

void Func_020002f4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
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
  if (object->action == 240) {
    if (!GameFlag_Test(0x250))
      Func_02005f8c(object, 202);
    if (object->angle == 0xc000) {
      position.x = object->x;
      position.z = object->z;
    } else {
      /* FAKEMATCH(wrapper): forwarded, so each product builds its own constant and
       * they share the routine's address; as direct calls, 106 of 153
       * instructions differ in tla-en. */
      position.x = object->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                      Math_Cosine(object->angle));
      position.z = object->z + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                      Math_Sine(object->angle));
    }
    shot = (struct Actor *)Import_ActorCreate(280, position.x,
                                                       object->y, position.z);
    Func_02005d5c(shot, 2);
    Actor_SetSingleChildFlags(shot, 0);
    ((struct FieldSprite *)shot->animation)->priority = 1;
    shot->motionFlags = 0;
    ((u8 *)shot)[0x62] = 0;
    ((u8 *)shot)[0x63] = 0;
    shot->angle = object->angle;
    shot->update = Func_02000d48;
    shot->watchedObject = object;
    object->action = 0;
    options.priority = 1;
    options.palette = 7;
    Func_020002f4(position.x, object->y, position.z - 0x60000, 0, 0, 0,
                  EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
  object->action++;
}

void Func_02001084(struct FieldActor *object, s16 timer, u8 value) {
  if (!GameFlag_Test(265)) {
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
    Func_02005d6c(shot, gData_020061cc);
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
      if (Func_02000d18(leaderPosition, shotPosition) <= 16 &&
          leader->z >> 20 == shot->z >> 20)
        near = 1;
    } else {
      if (Func_02000cec(leaderPosition, shotPosition) <= 8 &&
          leader->y >> 20 == shot->y >> 20)
        near = 1;
    }
    if (near && !GameFlag_Test(0x104)) {
      leader->angle = shot->angle;
      leader->watchedObject = shot;
      leader->priorityFlags &= ~1;
      ((struct FieldSprite *)leader->animation)->priority =
          ((struct FieldSprite *)shot->animation)->priority;
      ((struct EventWork *)work)->unk16a = 200;
      gMainRoutineWork.renderMode = 0;
      ((struct Shot *)shot)->unk62 = 1;
      ((struct Shot *)shot)->unk63 = 1;
    }
  }
  if (shot->angle == 0xc000) {
    shot->x += Iwram_MulQ16(Math_Cosine(shot->angle), speed);
    shot->y -= Iwram_MulQ16(Math_Sine(shot->angle), speed);
  } else {
    shot->x += Iwram_MulQ16(Math_Cosine(shot->angle), speed);
    shot->z += Iwram_MulQ16(Math_Sine(shot->angle), speed);
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
  if (object->action == 240) {
    shot = 0;
    if (!GameFlag_Test(0x250))
      Func_02005f8c(object, 202);
    if (object->angle == 0xc000) {
      position.x = object->x;
      position.z = object->z;
    } else {
      /* FAKEMATCH(wrapper): forwarded, so each product builds its own 0x80000
       * and they share the routine's address; as direct calls, 129 of 184
       * instructions differ in tla-en. */
      position.x = object->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                      Math_Cosine(object->angle));
      position.z = object->z + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                      Math_Sine(object->angle));
    }
    for (i = 0; i != 3; i++) {
      shot = Actor_GetById(13 + i);
      if (shot->update == 0)
        break;
    }
    shot->x = position.x;
    shot->y = object->y;
    shot->z = position.z;
    Func_02005d5c(shot, 2);
    Actor_SetSingleChildFlags(shot, 0);
    ((struct FieldSprite *)shot->animation)->priority = 1;
    shot->priorityFlags |= 32;
    shot->motionFlags = 0;
    ((u8 *)shot)[0x62] = 0;
    ((u8 *)shot)[0x63] = 0;
    shot->angle = object->angle;
    shot->watchedObject = object;
    shot->update = Func_020010b4;
    object->action = 0;
    options.priority = 1;
    options.palette = 7;
    Func_020002f4(position.x, object->y, position.z - 0x60000, 0, 0, 0,
                  EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
  object->action++;
}

void Func_02001420(struct FieldActor *object, s16 timer, u8 value) {
  if (!GameFlag_Test(265)) {
    BYTE_AT(object, 90) = 0;
    object->timer = timer;
    BYTE_AT(object, 98) = value;
    WORD_AT(object, 108) = (s32)Func_02001290;
    Actor_SetSingleChildFlags(object, 0);
  }
}

extern char SceneId_Ov664Room1, SceneId_Ov664Room2, SceneId_Ov664Room3,
    SceneId_Ov664Room5, SceneId_Ov664Room6, SceneId_Ov664Room7,
    SceneId_Ov664Room8, SceneId_Ov664Room9;

void Func_02005d7c(struct Actor *object);

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
  if (object->action == 0) {
    if (object->x == 0) {
      Func_02005d7c(object);
    } else {
      object->z = 0;
      object->x = 0;
    }
    return;
  }
  if (Func_02000cec(&leader->x, pos = &object->x) <= 8 &&
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
    if (Func_02000cec(&other->x, pos) <= 8) {
      object->speedLimit = -object->speedLimit >> 1;
      object->unk4c = 0;
      object->z = other->z - 0x80000;
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room8) {
    for (i = 0; i != 3; i++) {
      struct Actor *other2 = Actor_GetById(i + 12);
#define other other2
      if (Func_02000cec(&other->x, pos) <= 8) {
        object->speedLimit = -object->speedLimit >> 1;
        object->unk4c = 0;
        object->z = other->z - 0x80000;
      }
    }
#undef other
  }
  object->action--;
  object->z += object->speedLimit;
  object->x += object->acceleration;
  object->speedLimit -= object->unk4c;
  object->scaleX += 0xccc;
  object->scaleY += 0xccc;
}

u32 Random16Far(void);

void Func_0200160c(s32 x, s32 y, s32 z) {
  struct FieldActor *object = Import_ActorCreate(0x11d, x, y, z);

  Func_02005d5c(object, 1);
  Actor_SetSingleChildFlags(object, 0);
  Actor_SetPartAttribute(object, 6);
  Animation_SetStateFlags(object, 1);
  object->timer = 27;
  object->motionFlags = 0;
  BYTE_AT(object, 90) = 0;
  WORD_AT(object, 24) = 0x8000;
  object->speed = 0x40000;
  object->acceleration = ((Random16Far() & 0x7fff) << 1) + 0xffff8001;
  WORD_AT(object, 76) = 0x3800;
  WORD_AT(object, 108) = (s32)Func_02001458;
}

void Func_0200160c(s32 x, s32 y, s32 z);

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

    if ((frame & 1) && !GameFlag_Test(0x89b))
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
        else if (!GameFlag_Test(0x201))
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
        else if (GameFlag_Test(0x200))
          goto dripA8;
        else
          Func_0200160c(0x1080000, 0, 0x2580000);
      } else if (GameFlag_Test(0x202)) {
        goto drip268;
      } else {
        Func_0200160c(0x2880000, -0x200000, 0x880000);
      }
    }
  }
}

#include "macros.h"

void Func_020017f4(struct FieldActor *object) {
  struct FieldActor *target = (struct FieldActor *)WORD_AT(object, 104);

  object->priorityFlags ^= 2 << (gFrameCount & 1);
  SET_POSITION(object, target->x, target->y + (128 << 12), target->z);
}

extern char SceneId_Ov664Room4, SceneId_Ov664Room6, SceneId_Ov664Room7,
    SceneId_Ov664Room8, SceneId_Ov664Room9;
extern const u8 gData_02005fd8[];
void FieldEvent_Wait(s32 frames);
void Func_02002bd0(void);
void Func_02005ed4(s32 a, s32 b);
s32 Func_02005dec(struct Actor *object);
void Func_02005d9c(s32 x, s32 z, s32 width, s32 height, s32 a, s32 b);
void Func_02005dac(s32 srcX, s32 srcZ, s32 width, s32 height, s32 destX,
                   s32 destZ);
void Func_02005d84(void);
void Func_02005dcc(s32 a, s32 b, s32 c);
void Func_02005eec(s32 actor, s32 a);

/// A cell of the map, and a step from one cell to the next.
struct Cell {
  s32 x, z;
};

struct EffectWork {
  u8 unknown[20];
  struct Actor *target;
};

#define TARGET(effect) ((effect)->target)

/// The gust picks up an object it passes over, to drop it at cell `at`
/// (x in the high half, z in the low).
#define PICK_UP(object, at)                                                    \
  do {                                                                         \
    (object)->watchedObject = gust;                                            \
    (object)->update = Func_020017f4;                                          \
    (object)->motionFlags = 0;                                                 \
    (object)->priorityFlags &= ~1;                                             \
    Actor_SetSingleChildFlags(object, 0);                                      \
    ((struct FieldSprite *)(object)->animation)->priority =                    \
        ((struct FieldSprite *)gust->animation)->priority;                     \
    gust->watchedObject = (object);                                            \
    carried = (at);                                                            \
  } while (0)

/// One shake of the walls (cue 157).
#define SHAKE()                                                                \
  (Func_02005f94(157), Call3(Func_02005dcc, 0x50000, 0x50000, 0x10000),        \
   Call3(Func_02005dcc, -1, -1, 0xe666))

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
  Func_02005e14();
  Func_02005f2c(0);
  Func_02005f94(207);
  gust =
      (struct Actor *)Import_ActorCreate(280, leader->x, leader->y, leader->z);
  Func_02005d5c(gust, 2);
  Actor_SetSingleChildFlags(gust, 0);
  gust->scaleX = 0x1b333;
  gust->scaleY = 0x3333;
  gust->motionFlags = 0;
  /* FAKEMATCH(signature): the target's word read as long, an alias set outside
   * struct Actor's, so the load need not wait for the motion flags' store; as
   * TARGET(effect), 2 of 1527 instructions differ in tla-en. */
  ((struct FieldSprite *)gust->animation)->priority =
      ((struct FieldSprite *)((struct Actor *)*(long *)&effect->target)
           ->animation)
          ->priority;
  Func_02005d8c(gust, TARGET(effect)->x, TARGET(effect)->y,
                TARGET(effect)->z + 0xffff0000);
  gust->terrainHeight = Map_GetTerrainHeight(0, gust->x, gust->z);
  while (gust->scaleY <= 0x14ccb) {
    gust->scaleY += 0x1999;
    FieldEvent_Wait(1);
  }
  Func_02005d94(gust);
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
      spark->speedLimit = Random16Far() & 0xffff;
      Func_02005d5c(spark, 7);
      Func_02005d6c(spark, gData_02005fd8);
      Actor_SetSingleChildFlags(spark, 0);
      Actor_SetPartAttribute(spark, 2);
      spark->update = Func_02002bd0;
    }
    if (gFrameCount & 1) {
      gust->scaleX += 655;
      gust->scaleY += 655;
      Actor_SetPartAttribute(TARGET(effect), 15);
    } else {
      Actor_SetPartAttribute(TARGET(effect), 4);
    }
    FieldEvent_Wait(1);
  }
  Actor_SetPartAttribute(TARGET(effect), 0);
  Func_02005f94(136);
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
    ((struct FieldSprite *)gust->animation)->priority = 1;
    cell.x = x;
    cell.z = z;
    gust->speedLimit = 0x40000;
    gust->acceleration = 0x40000;
    Func_02005d8c(gust, cell.x << 16, gust->y, cell.z << 16);
    Call2((void (*)())Func_02005ed4, 0x50000, 0x50000);
    Call4((void (*)())Camera_MoveWithinMapBounds, x << 16, -1, z << 16, 1);
  }
  carried = 0;
  while (Func_02005dec(gust) == 0) {
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
      velocity.x = Math_Cosine((gFrameCount & 0x1ff) << 12) << 1;
      velocity.y = gust->y + ((Random16Far() & 31) << 16);
      velocity.z = Math_Sine((gFrameCount & 0x1ff) << 12);
      Func_020002f4(gust->x, velocity.y, gust->z, velocity.x, 0, velocity.z,
                    EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                    &leaves);
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
    Actor_SetSingleChildFlags(object, 1);
    Scene_SetActorSpritePriority(object, 2);
    if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room6) {
      object->terrainHeight = 0xffe00000;
      Call6(Func_02005dac, 19, 16, 1, 1, 19, 18);
      GameFlag_SetBit(0x89f);
    } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room8) {
      Call6(Func_02005dac, 19, 38, 1, 1, 20, 38);
      OverlayObject_WaitUntilIdle(object);
      wait(1);
      OverlayObject_WaitUntilIdle(object);
      object->motionFlags = 0;
      object->terrainHeight = 0;
      object->y = 0;
      Func_02005df4(0x1400000, 0x3400000, 0, 4);
    } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room9) {
      OverlayObject_WaitUntilIdle(object);
      wait(1);
      OverlayObject_WaitUntilIdle(object);
      Actor_SetSingleChildFlags(object, 0);
      object->motionFlags = 0;
      object->terrainHeight = 0;
      object->y = 0;
      Func_02005df4(0x780000, 0x1980000, 0, 4);
      if (GameFlag_Test(0x899))
        Func_02001084(object, 60, 0);
    }
  }
  if (hit == 255) {
    Func_02005f94(136);
  } else {
    cell.x = (gust->x >> 16) + dir.x * 48;
    cell.z = (gust->z >> 16) + dir.z * 48;
    gust->speedLimit = 0x8000;
    gust->acceleration = 0x8000;
    Call4((void (*)())Func_02005d8c, (s32)(gust), (s32)(cell.x << 16),
          (s32)(gust->y), (s32)(cell.z << 16));
    hit -= 30;
    i = 0;
    while (Func_02005dec(gust) == 0) {
      if (hit > 4)
        goto shrink;
      if (i == 0) {
        Func_02005f94(235);
        i = 1;
      }
      if ((gFrameCount & 1) == 0) {
        dust.priority = 1;
        {
          dust.startScaleX = dust.startScaleY =
              (Random16Far() & 0xffff) + 0x10000;
        }
        velocity.x = Math_Cosine((gFrameCount & 0x1ff) << 12) << 1;
        velocity.y = 0x10000;
        velocity.z = Math_Sine((gFrameCount & 0x1ff) << 12);
        Func_020002f4(gust->x, gust->y, gust->z, velocity.x, velocity.y,
                      velocity.z, EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY,
                      &dust);
      }
      if ((gFrameCount & 3) == 0) {
        if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room4) {
          GameFlag_SetBit(0x300);
          cell.x = (gust->x >> 20) + 64;
          cell.z = gust->z >> 20;
          Func_02005d9c(90, 25, cell.x + 1, cell.z, 1, 3);
          Func_02005d9c(21, 88, 24, 89, 1, 2);
          Func_02005dac(23, 92, 2, 1, 23, 89);
          Func_02005dac(18, 33, 2, 1, 23, 25);
        } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room7) {
          cell.x = gust->x >> 20;
          cell.z = ((gust->z - gust->y) >> 20) + 64;
          if (hit == 0) {
            GameFlag_SetBit(772);
            Func_02005d9c(39, 82, cell.x, cell.z, 1, 1);
            Func_02005dac(39, 82, 1, 1, 38, 82);
            Call6(Func_02005dac, 38, 19, 1, 1, 38, 18);
          } else {
            GameFlag_SetBit(773);
            Call6(Func_02005d9c, 39, 82, cell.x, cell.z, 1, 1);
            Call6(Func_02005dac, 38, 98, 1, 1, 38, 99);
            Call6(Func_02005dac, 38, 35, 1, 1, 38, 37);
          }
        } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room9) {
          GameFlag_SetBit(774);
          cell.x = gust->x >> 20;
          Call6(Func_02005d9c, 15, 80, cell.x, 77, 1, 3);
          Call6(Func_02005dac, 15, 80, 1, 1, 15, 79);
          Call6(Func_02005dac, 17, 15, 2, 1, 15, 15);
        }
        Func_02005d84();
      }
      FieldEvent_Wait(1);
    }
    Func_02005f94(298);
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
  Func_02005d7c(gust);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room4) {
    if (hit == 5) {
      SHAKE();
      Call6(Func_02005d9c, 79, 0x40, 29, 85, 5, 7);
      FieldEvent_Wait(30);
      SHAKE();
      Call6(Func_02005d9c, 69, 0x40, 29, 85, 5, 7);
      Call6(Func_02005dac, 46, 20, 5, 5, 29, 21);
      Call6(Func_02005dac, 0x20, 89, 1, 1, 31, 89);
      Call6(Func_02005dac, 50, 86, 1, 1, 33, 87);
      GameFlag_SetBit(769);
    } else if (hit == 6) {
      SHAKE();
      Call6(Func_02005d9c, 74, 0x40, 29, 85, 5, 7);
      FieldEvent_Wait(30);
      SHAKE();
      Call6(Func_02005d9c, 0x40, 0x40, 29, 85, 5, 7);
      Call6(Func_02005dac, 52, 20, 5, 5, 29, 21);
      Call6(Func_02005dac, 33, 88, 1, 1, 33, 87);
      Call6(Func_02005dac, 0x30, 88, 1, 1, 31, 89);
      GameFlag_ClearBit(769);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room6) {
    if (hit == 5) {
      SHAKE();
      Call6(Func_02005d9c, 79, 0x40, 0x20, 79, 5, 7);
      if (Actor_GetById(8)->x >> 19 == 65) {
        velocity.x = Actor_GetById(8)->x + 0xfff80000;
        velocity.y = Actor_GetById(8)->y;
        velocity.z = Actor_GetById(8)->z + 0x80000;
        Actor_SetSpritePriority(8, 2);
        for (i = 0; i != 8; i++) {
          dust.priority = 1;
          Func_020002f4(velocity.x + ((Random16Far() & 15) << 16),
                        velocity.y + ((Random16Far() & 31) << 16), velocity.z,
                        0, 0, -0x8000, EFFECT_USE_PRIORITY, &dust);
        }
        wait(30);
        SHAKE();
        Call6(Func_02005d9c, 0x40, 0x40, 0x20, 79, 5, 7);
        Actor_GetById(8)->priorityFlags |= 1;
      } else {
        FieldEvent_Wait(30);
        SHAKE();
        Call6(Func_02005d9c, 69, 0x40, 0x20, 79, 5, 7);
        Call6(Func_02005dac, 49, 15, 5, 5, 0x20, 15);
        Call6(Func_02005dac, 33, 82, 1, 1, 34, 82);
        Call6(Func_02005dac, 52, 81, 1, 1, 36, 81);
        GameFlag_ClearBit(770);
      }
    } else if (hit == 6) {
      SHAKE();
      if (Actor_GetById(8)->x >> 19 == 65) {
        velocity.x = Actor_GetById(8)->x + 0xfff80000;
        velocity.y = Actor_GetById(8)->y;
        velocity.z = Actor_GetById(8)->z;
        for (i = 0; i != 8; i++) {
          dust.priority = 1;
          Func_020002f4(velocity.x + ((Random16Far() & 15) << 16), velocity.y,
                        velocity.z - ((Random16Far() & 31) << 16), 0, 0,
                        -0x8000, EFFECT_USE_PRIORITY, &dust);
        }
        wait(20);
      } else {
        Call6(Func_02005d9c, 79, 0x40, 0x20, 79, 5, 7);
        Func_02005f94(157);
        FieldEvent_Wait(30);
        SHAKE();
        Call6(Func_02005d9c, 0x40, 0x40, 0x20, 79, 5, 7);
        Call6(Func_02005dac, 49, 21, 5, 5, 0x20, 15);
        Call6(Func_02005dac, 35, 81, 1, 1, 36, 81);
        Call6(Func_02005dac, 50, 82, 1, 1, 34, 82);
        GameFlag_SetBit(770);
      }
    }
  }
  FieldEvent_Wait(30);
  Func_02005eec(gMainRoutineWork.currentOwner, 1);
  Func_02005ee4();
  Func_02005e1c();
}

extern const u8 gData_02006238[];
extern const u8 gData_02006268[];

const u8 *Func_020025cc(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room1)
    return gData_02006268;
  return gData_02006238;
}

