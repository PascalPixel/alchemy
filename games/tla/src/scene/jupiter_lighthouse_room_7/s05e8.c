/// @file
/// Scene code of overlay 0x6a0, in ROM order, from s05e8's offset up to the next object the overlay links.

#include "jupiter_lighthouse_room_7.h"
#include "event_inline.h"
#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "field.h"

s32 Func_020005a0(struct Actor *object);
void Func_02002b54(s32 x, s32 z, s32 a, s32 b);

void Func_020005e8(s32 mode) {
  struct Actor *object;
  s32 i, frame;

  // Goto loops: as for loops, the stored constants (3, 4, 0x208000) are hoisted
  // out of the loops and 42 of 181 instructions differ in tla-en.
  if (!Event_GetFlag(0xa1a)) {
    MapGrid_CopyCellsLayered(80, 46, 1, 7, 72, 9);
    MapGrid_CopyCellsLayered(80, 53, 8, 5, 72, 16);
    MapGrid_CopyCellsLayered(78, 47, 1, 1, 8, 74);
    Func_02002b54(0x800000, 0xc00000, 0, 4);
    if (mode == 1) {
      frame = 0;
    L_0200864c:
      i = 0;
    L_0200864e:
      object = Actor_Get(i + 10);
      if (Func_020005a0(object)) {
        object->motionFlags = 0;
        object->y -= 0x20000;
      }
      if (++i <= 5)
        goto L_0200864e;
      System_Wait(1);
      frame++;
      if (frame <= 15)
        goto L_0200864c;
    }
    i = 0;
  L_02008684:
    object = Actor_Get(i + 10);
    object->motionFlags = 3;
    i++;
    object->y = object->terrainHeight;
    if (i <= 5)
      goto L_02008684;
    /* FAKEMATCH(dead): an empty once-only block keeps the loop's exit branch
     * from being threaded to the function's end; as the plain if/else, 4 of 175
     * instructions differ in tla-en. */
    do {
    } while (0);
  } else {
    MapGrid_CopyCellsLayered(72, 46, 1, 7, 72, 9);
    MapGrid_CopyCellsLayered(72, 53, 8, 5, 72, 16);
    MapGrid_CopyCellsLayered(70, 47, 1, 1, 8, 74);
    Func_02002b54(0x800000, 0xc00000, 0, 6);
    FieldActor_Stop(gMainRoutineWork.currentOwner);
    Actor_SetPosition(20, 0, 0);
    if (mode == 1) {
      frame = 0;
    L_0200870a:
      i = 0;
    L_0200870c:
      object = Actor_Get(i + 10);
      if (Func_020005a0(object)) {
        object->motionFlags = 0;
        object->y += 0x20000;
      }
      if (++i <= 5)
        goto L_0200870c;
      System_Wait(1);
      frame++;
      if (frame <= 15)
        goto L_0200870a;
    }
    i = 0;
  L_02008744:
    object = Actor_Get(i + 10);
    if (Func_020005a0(object)) {
      object->motionFlags = 4;
      object->y = 0x208000;
    }
    if (++i <= 5)
      goto L_02008744;
  }
}

extern const u8 gData_020031c4[];
extern s32 gData_02003594;
extern s16 gData_020035fe;
extern struct Actor *gData_02003600[];
extern u16 gData_02003702;
void Func_020000b4(void);
void Func_02000100(void);
void Func_02002c54(s32 x, s32 z, s32 a, s32 b);
void Func_02001a48(const u8 *data, s32 a, s32 b);
void Func_02002bec(void);

void Func_02000778(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (id == 20) {
    Func_02002b54(object->x, object->z, 0, 6);
    Func_02002c54(object->x, object->z, 0, 128);
  }
}

// Jupiter Lighthouse room 7's push handler for objects 18 to 20: an object not
// on the ground falls into place (sound 188). Object 18 or 19 then checks its
// spot (Func_02000194), resets the room, plays sound 167 for the spots that
// count, and when all is set (Func_02000290 returns 2 with flag 2582) clears
// that flag and shows object 17 changing (sound 151) from the camera at (664,
// 136). Object 20 on cell (8, 12) sets flag 2586, plays sound 167 and with flag
// 2583 shows object 9 changing; off that cell it marks the cell it stands on.
// The landing wait is the rotated loop (an if around a do-while), and the flag
// and sound calls go through the call layer, each as the native has it.

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Func_02000194(void);
void Func_020002f0(void);
s32 Func_02000290(void);
void Func_02002b6c(void);
void Func_02002b74(void);
void Func_02002bd4(void);
void Func_02002ad4(struct Actor *object, s32 mode);
void Func_02002c4c(s32 x, s32 z, s32 a, s32 b);

void Func_020007a4(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 spot;
  s32 i;

  if (object->terrainHeight !=
      Map_GetTerrainHeight(object->terrainId, object->x, object->z)) {
    object->motionFlags = 3;
    wait(2);
    i = 0;
    if (object->y > object->terrainHeight || object->velocityY != 0) {
      do {
        wait(1);
        i++;
        if (i > 29)
          break;
      } while (object->y > object->terrainHeight || object->velocityY != 0);
    }
    Event_PlaySound(188);
    object->motionFlags = 0;
  }
  if ((u32)(id - 18) <= 1) {
    spot = Func_02000194();
    if (spot == -1)
      return;
    Actor_SetSingleChildFlags(Actor_GetById(id), 0);
    Event_Wait(30);
    Func_020002f0();
    if (spot == 0)
      Event_PlaySound(167);
    if (spot == 1)
      Event_PlaySound(167);
    if (spot == 3 && Event_GetFlag(2576))
      Event_PlaySound(167);
    if (spot == 4 && Event_GetFlag(2577))
      Event_PlaySound(167);
    Actor_Stop(gMainRoutineWork.currentOwner);
    Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
    if (Func_02000290() != 2 || !Event_GetFlag(2582))
      return;
    object = Actor_GetById(17);
    Event_ClearFlag(2582);
    Event_Wait(10);
    Func_02002b6c();
    FieldEvent_ResolvePendingActions(0);
    FieldCamera_MoveTo(166 << 18, -1, PIXELS(136), 1);
    Func_02002bd4();
    Event_Wait(20);
    Event_PlaySound(151);
    Func_02002ad4(object, 3);
    object->priorityFlags &= ~2;
    MapGrid_SetCellBehavior(object->x, object->z, object->terrainId, 255);
    Event_Wait(30);
    FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
    Func_02002bd4();
    Func_02002b74();
    return;
  }
  if (id != 20)
    return;
  if (object->x >> 20 == 8 && *(s32 *)&object->z >> 20 == 12) {
    Event_SetFlag(2586);
    Actor_SetSingleChildFlags(Actor_GetById(20), 0);
    Event_Wait(30);
    Event_PlaySound(167);
    Func_020005e8(1);
    if (!Event_GetFlag(2583))
      return;
    object = Actor_GetById(9);
    Event_ClearFlag(2583);
    Event_Wait(20);
    Event_PlaySound(151);
    Func_02002ad4(object, 3);
    object->priorityFlags &= ~2;
    MapGrid_SetCellBehavior(object->x, object->z, object->terrainId, 255);
  } else {
    Func_02002b54(object->x, object->z, 0, 8);
    Func_02002c4c(object->x, object->z, 0, 128);
  }
}

void Func_020009d8(void) { Event_ResolvePendingActions(0); }

void Func_020009e4(s32 a, s32 b) { Func_02001a48(gData_020031c4, a, b); }

void Func_020009f8(s32 id, s32 x, s32 z, s32 dx, s32 dz) {
  struct Actor *object = Actor_Get(id);

  Actor_SetPosition(id, (x << 20) + PIXELS(8),
                                         (z << 20) + PIXELS(8));
  object->motionFlags = 3;
  object->terrainHeight = PIXELS(-16);
  object->y = PIXELS(-16);
  object->velocityY = PIXELS(8);
  Actor_SetSpeed(id, 0x19999, 0xcccc);
  Actor_SetDestinationOffset(id, dx, dz);
  Event_PlaySound(222);
}

/// Drops objects 18 and 19 once any of flags 2576 to 2581 is set: the camera
/// moves to (592, 248), each object drops (Func_020009f8) at the spot whose
/// flag is set ((35, 15), (31, 16) or (33, 10)), the six flags are cleared,
/// Func_020002f0 resets the room, both objects are waited on until they land
/// (at most 30 frames each) and stop, and the camera follows the leader again.
void Func_02000a74(void) {
  struct Actor *object;
  s32 i;

  if (Event_GetFlag(2576) + Event_GetFlag(2578) + Event_GetFlag(2580) +
          Event_GetFlag(2577) + Event_GetFlag(2579) + Event_GetFlag(2581) ==
      0)
    return;
  FieldCamera_MoveTo(148 << 18, -1, PIXELS(248), 1);
  Func_02002bd4();
  FieldEvent_Wait(10);
  if (Event_GetFlag(2576))
    Func_020009f8(18, 35, 15, -16, 0);
  if (Event_GetFlag(2577))
    Func_020009f8(19, 35, 15, -16, 0);
  if (Event_GetFlag(2578))
    Func_020009f8(18, 31, 16, 16, 0);
  if (Event_GetFlag(2579))
    Func_020009f8(19, 31, 16, 16, 0);
  if (Event_GetFlag(2580))
    Func_020009f8(18, 33, 10, -16, 0);
  if (Event_GetFlag(2581))
    Func_020009f8(19, 33, 10, -16, 0);
  Event_ClearFlag(2576);
  Event_ClearFlag(2577);
  Event_ClearFlag(2578);
  Event_ClearFlag(2579);
  Event_ClearFlag(2580);
  Event_ClearFlag(2581);
  Func_020002f0();
  System_Wait(2);
  object = Actor_Get(18);
  i = 0;
  if (object->y > object->terrainHeight || object->velocityY != 0) {
    do {
      System_Wait(1);
      i++;
      if (i > 29)
        break;
    } while (object->y > object->terrainHeight || object->velocityY != 0);
  }
  object->motionFlags = 0;
  object = Actor_Get(19);
  i = 0;
  if (object->y > object->terrainHeight || object->velocityY != 0) {
    do {
      System_Wait(1);
      i++;
      if (i > 29)
        break;
    } while (object->y > object->terrainHeight || object->velocityY != 0);
  }
  object->motionFlags = 0;
  FieldEvent_Wait(30);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02002bd4();
}

void Func_02000c7c(void) {
  struct Actor *object;
  s32 i;

  if (Event_GetFlag(0xa1a))
    Func_020009f8(20, 8, 12, 16, 0);
  Event_ClearFlag(0xa1a);
  Func_020005e8(1);
  System_Wait(2);
  object = Actor_Get(20);
  for (i = 0; i < 30; i++) {
    if (object->y <= object->terrainHeight && object->velocityY == 0)
      break;
    System_Wait(1);
  }
  object->motionFlags = 0;
}

void Func_02002c24(void);

void Func_02000cec(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *first = Actor_Get(18);
  struct Actor *second = Actor_Get(19);
  s32 position[3];
  s32 found = 0;
  s32 x;
  s32 z;
  s32 facing;

  facing = (leader->angle + 0x2000) & 0xc000;
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  FieldVector_AddPolarOffset(0x100000, facing, position);
  x = position[0] >> 20;
  z = position[2] >> 20;
  if (first->y == 0 && x == first->x >> 20 && z == first->z >> 20)
    found = 1;
  if (second->y == 0 && x == second->x >> 20 && z == second->z >> 20)
    found = 1;
  if (found == 0) {
    if (x == 35 && z == 15 && (Event_GetFlag(2576) || Event_GetFlag(2577)))
      found = 2;
    if (x == 31 && z == 16 && (Event_GetFlag(2578) || Event_GetFlag(2579)))
      found = 2;
    if (x == 33 && z == 10 && (Event_GetFlag(2580) || Event_GetFlag(2581)))
      found = 2;
    if (found == 0) {
      Func_02002bec();
      return;
    }
  }
  if (found == 1)
    Func_02002c24();
}

void Func_02000e20(void) { Func_02002bec(); }
