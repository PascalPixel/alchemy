/// @file
/// The scene code of overlay 0x6a0 that its scene tables belong to, in ROM
/// order; the rest of the overlay is in jupiter_lighthouse_room_7/.

#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "event_inline.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"
#include "event_work.h"
#include "module/field/field.h"

extern const u8 gData_020031c4[];
extern s32 gData_02003594;
extern s16 gData_020035fe;
extern u16 gData_02003702;
void Func_02000060(s32 value);
void Func_020000b4(void);
void Func_02000100(void);
void Func_02002b54(s32 x, s32 z, s32 a, s32 b);
void Func_02002c54(s32 x, s32 z, s32 a, s32 b);
void Func_02001a48(const u8 *data, s32 a, s32 b);
void Func_02002bec(void);

void Func_02000038(void) { Func_080d2b4c(21, 74); }

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

#include "jupiter_lighthouse_room_7/jupiter_lighthouse_room_7.h"
#include "main_routine_work.h"
#include "call.h"

void Func_02000060(s32 mode) {
  if (mode == 1) {
    Event_PlaySound(133);
    MapGrid_CopyCellsLayered(76, 42, 4, 3, 76, 35);
    System_Wait(6);
  }
  MapGrid_CopyCellsLayered(81, 42, 4, 3, 76, 35);
  MapGrid_CopyCellAttributes(12, 41, 4, 1, 12, 35);
}

void Func_020000b4(void) {
  Event_PlaySound(134);
  MapGrid_CopyCellsLayered(76, 42, 4, 3, 76, 35);
  System_Wait(6);
  MapGrid_CopyCellsLayered(71, 42, 4, 3, 76, 35);
  MapGrid_CopyCellAttributes(12, 36, 4, 1, 12, 35);
}

void Func_02000100(void) {
  struct Actor *object = Actor_Get(16);

  if (!Event_GetFlag(0xa1b) && object->x >> 20 == 8 && object->z >> 20 == 37) {
    Event_SetFlag(0xa1b);
    Func_02000060(1);
  }
}

void Func_0200013c(void) {
  if (!Event_GetFlag(514)) {
    Event_SetFlag(514);
    Func_02000060(1);
  }
}

void Func_02000160(void) {
  if (Event_GetFlag(514) == 1) {
    Event_ClearFlag(514);
    Func_020000b4();
  }
  Func_02000100();
}

void Func_02000184(void) {
  System_Wait(10);
  Func_02000100();
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02000194(void) {
  s32 t1, t2, t3, t5, t7, t9, t11, t13, v5, v6, v7, v8;

  t1 = ((s32)Actor_Get(18));
  v5 = t1;
  t2 = ((s32)Actor_Get(19));
  v7 = 1;
  v6 = (FIELD_AT(v5, s32, 8) >> 20);
  v8 = t2;
  v7 = -v7;
  v5 = (FIELD_AT(v5, s32, 16) >> 20);
  if (v6 != 35)
    goto L_020081d6;
  if (v5 != 15)
    goto L_020081d6;
  t3 = Event_GetFlag(0xa11);
  if (t3 != 0)
    goto L_020081d6;
  Event_SetFlag(0xa10);
  v7 = 0;
L_020081d6:
  if (v6 != 31)
    goto L_020081f8;
  if (v5 != 16)
    goto L_020081f8;
  t5 = Event_GetFlag(0xa13);
  if (t5 != 0)
    goto L_020081f8;
  Event_SetFlag(0xa12);
  v7 = 2;
L_020081f8:
  if (v6 != 33)
    goto L_0200821a;
  if (v5 != 10)
    goto L_0200821a;
  t7 = Event_GetFlag(0xa15);
  if (t7 != 0)
    goto L_0200821a;
  Event_SetFlag(0xa14);
  v7 = 4;
L_0200821a:
  v6 = (FIELD_AT(v8, s32, 8) >> 20);
  v5 = (FIELD_AT(v8, s32, 16) >> 20);
  if (v6 != 35)
    goto L_02008244;
  if (v5 != 15)
    goto L_02008244;
  t9 = Event_GetFlag(0xa10);
  if (t9 != 0)
    goto L_02008244;
  Event_SetFlag(0xa11);
  v7 = 1;
L_02008244:
  if (v6 != 31)
    goto L_02008266;
  if (v5 != 16)
    goto L_02008266;
  t11 = Event_GetFlag(0xa12);
  if (t11 != 0)
    goto L_02008266;
  Event_SetFlag(0xa13);
  v7 = 3;
L_02008266:
  if (v6 != 33)
    goto L_02008288;
  if (v5 != 10)
    goto L_02008288;
  t13 = Event_GetFlag(0xa14);
  if (t13 != 0)
    goto L_02008288;
  Event_SetFlag(0xa15);
  v7 = 5;
L_02008288:
  return v7;
}

s32 Func_02000290(void) {
  s32 count = 0;

  if (Event_GetFlag(0xa10) || Event_GetFlag(0xa12) || Event_GetFlag(0xa14))
    count = 1;
  if (Event_GetFlag(0xa11) || Event_GetFlag(0xa13) || Event_GetFlag(0xa15))
    count++;
  return count;
}

/// Copies the map cells and sets the cell values that flags 0xa10 to 0xa15 call
/// for.
void Func_020002f0(void) {
  /* FAKEMATCH(wrapper): every call forwarded through the call layer; as direct
   * calls, 63 of 303 instructions differ in tla-en, the coordinates and flags
   * shared in saved registers. */
  MapGrid_CopyCellsLayered(0x5e, 0x3c, 9, 8, 0x5e, 0xa);
  Call4((void (*)())Func_02002b54, 0x2300000, 0xf00000, 0, -2);
  Call4((void (*)())Func_02002b54, 0x1f00000, 0x1000000, 0, -2);
  Call4((void (*)())Func_02002b54, 0x2100000, 0xa00000, 0, -2);
  MapGrid_SetCellBehavior(0x2600000, 0xa00000, 2, 0);
  MapGrid_SetCellBehavior(0x1f00000, 0xa00000, 2, 0);
  MapGrid_SetCellBehavior(0x1f00000, 0xe00000, 2, 0);
  if (Event_GetFlag(0xa10) || Event_GetFlag(0xa11))
    Func_02002b54(0x2300000, 0xf00000, 0, 0);
  if (Event_GetFlag(0xa12) || Event_GetFlag(0xa13))
    Func_02002b54(0x1f00000, 0x1000000, 0, 0);
  if (Event_GetFlag(0xa14) || Event_GetFlag(0xa15))
    Func_02002b54(0x2100000, 0xa00000, 0, 0);
  if (Event_GetFlag(0xa10)) {
    MapGrid_CopyCellsLayered(0x60, 0x38, 7, 2, 0x60, 0xf);
    MapGrid_CopyCellsLayered(0x6f, 0x33, 1, 1, 0x63, 0xf);
    if (Event_GetFlag(0xa13)) {
      MapGrid_CopyCellsLayered(0x69, 0x33, 1, 3, 0x5f, 0xe);
      MapGrid_SetCellBehavior(0x1f00000, 0xe00000, 2, 0xec);
    }
    if (Event_GetFlag(0xa15))
      MapGrid_CopyCellsLayered(0x6f, 0x35, 1, 1, 0x61, 0xa);
  } else if (Event_GetFlag(0xa11)) {
    MapGrid_CopyCellsLayered(0x62, 0x33, 5, 1, 0x62, 0xa);
    MapGrid_CopyCellsLayered(0x63, 0x34, 4, 5, 0x63, 0xb);
    MapGrid_SetCellBehavior(0x2600000, 0xa00000, 2, 0xec);
    if (Event_GetFlag(0xa12))
      MapGrid_CopyCellsLayered(0x6a, 0x35, 1, 1, 0x5f, 0x10);
    if (Event_GetFlag(0xa14)) {
      MapGrid_CopyCellsLayered(0x6d, 0x33, 3, 1, 0x5f, 0xa);
      MapGrid_SetCellBehavior(0x1f00000, 0xa00000, 2, 0xec);
    }
  } else {
    if (Event_GetFlag(0xa12))
      MapGrid_CopyCellsLayered(0x6a, 0x35, 1, 1, 0x5f, 0x10);
    if (Event_GetFlag(0xa13))
      MapGrid_CopyCellsLayered(0x6b, 0x35, 1, 1, 0x5f, 0x10);
    if (Event_GetFlag(0xa14))
      MapGrid_CopyCellsLayered(0x6f, 0x34, 1, 1, 0x61, 0xa);
    if (Event_GetFlag(0xa15))
      MapGrid_CopyCellsLayered(0x6f, 0x35, 1, 1, 0x61, 0xa);
  }
}

s32 Func_020005a0(struct Actor *actor) {
  s32 i;

  for (i = 0; gData_02002c64[i][0] != -1; i++) {
    if (gData_02002c64[i][0] == actor->x >> 20 &&
        gData_02002c64[i][1] == actor->z >> 20)
      return 1;
  }
  return 0;
}

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
    Event_Start();
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
    Event_End();
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

  Actor_SetPosition(id, (x << 20) + PIXELS(8), (z << 20) + PIXELS(8));
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

  facing = (leader->facing + 0x2000) & 0xc000;
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

#include "ram_buffer.h"

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Func_020019dc();
void Func_02001db0();
void Func_02001eb8();
void Func_02001ee8();

s32 Scene_Initialize(void) {
  s32 t1, t2, t3, t5, t8, t15, t16, t17, t19, t20, t21, t23, t24, t25, t26, t27,
      t28, t29, v5;

  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1ac), s32, 0) = 0x204;
  if ((u32)((FIELD_AT(&gMainRoutineWork, u16, 482) + -3) << 16) > (u32)0x10000)
    goto L_02008e6e;
  t1 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  FIELD_AT(t1, u8, 35) = (32 | FIELD_AT(t1, u8, 35));
L_02008e6e:
  t2 = ((s32)Actor_Get(8));
  FIELD_AT(t2, s32, 24) = 0xc000;
  t3 = ((s32)Actor_Get(8));
  FIELD_AT(t3, s32, 28) = 0xc000;
  Func_020019dc((s32)&gData_020031c4);
  t5 = Event_GetFlag(0xa1b);
  if (t5 == 0)
    goto L_02008eaa;
  Actor_SetPosition(16, PIXELS(136), PIXELS(600));
  Func_02000060(0);
L_02008eaa:
  t8 = Event_GetFlag(0x202);
  if (t8 == 0)
    goto L_02008ebe;
  Func_02000060(0);
L_02008ebe:
  Func_02001db0();
  Func_02001ee8(2);
  Func_02001eb8(18, 2);
  Func_02001eb8(19, 1);
  Func_02001eb8(20, 0);
  t15 = Event_GetFlag(0xa10);
  if (t15 != 0)
    goto L_02008f08;
  t16 = Event_GetFlag(0xa12);
  if (t16 != 0)
    goto L_02008f08;
  t17 = Event_GetFlag(0xa14);
  if (t17 == 0)
    goto L_02008f12;
L_02008f08:
  Actor_SetPosition(18, 0, 0);
L_02008f12:
  t19 = Event_GetFlag(0xa11);
  if (t19 != 0)
    goto L_02008f3c;
  t20 = Event_GetFlag(0xa13);
  if (t20 != 0)
    goto L_02008f3c;
  t21 = Event_GetFlag(0xa15);
  if (t21 == 0)
    goto L_02008f46;
L_02008f3c:
  Actor_SetPosition(19, 0, 0);
L_02008f46:
  t23 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  FIELD_AT(t23, u8, 35) = (64 | FIELD_AT(t23, u8, 35));
  t24 = ((s32)Actor_Get(10));
  FIELD_AT(t24, u8, 35) = (FIELD_AT(t24, u8, 35) | 128);
  t25 = ((s32)Actor_Get(11));
  FIELD_AT(t25, u8, 35) = (FIELD_AT(t25, u8, 35) | 128);
  t26 = ((s32)Actor_Get(12));
  FIELD_AT(t26, u8, 35) = (FIELD_AT(t26, u8, 35) | 128);
  t27 = ((s32)Actor_Get(13));
  FIELD_AT(t27, u8, 35) = (FIELD_AT(t27, u8, 35) | 128);
  t28 = ((s32)Actor_Get(14));
  FIELD_AT(t28, u8, 35) = (FIELD_AT(t28, u8, 35) | 128);
  t29 = ((s32)Actor_Get(15));
  v5 = (128 | FIELD_AT(t29, u8, 35));
  FIELD_AT(t29, u8, 35) = v5;
  return 0;
}

void Func_02001224(s32 mode);
s32 Func_02002b5c(s32 x, s32 z, s32 layer);

s32 Scene_PrepareMap(void) {
  struct Actor *object;
  s32 value;

  Scene_SetArrivalFlags(0);
  Func_02001224(1);
  if (gMainRoutineWork.entrance == 12) {
    /* FAKEMATCH(wrapper): the placements forwarded through the call layer; as
     * direct calls to Actor_SetHorizontalPositionWithTerrain, 46 of 149
     * instructions differ in tla-en, 0xa80000 and the z values shared in saved
     * registers. */
    Actor_SetPosition(10, PIXELS(168), PIXELS(264));
    Actor_SetPosition(11, PIXELS(200), PIXELS(264));
    Actor_SetPosition(12, PIXELS(232), PIXELS(264));
    Actor_SetPosition(13, PIXELS(168), PIXELS(296));
    Actor_SetPosition(14, PIXELS(136), PIXELS(328));
    Actor_SetPosition(15, PIXELS(168), PIXELS(328));
  }
  if (!Event_GetFlag(0x109) && Func_02000290() != 2) {
    Event_ClearFlag(0xa10);
    Event_ClearFlag(0xa12);
    Event_ClearFlag(0xa14);
    Event_ClearFlag(0xa11);
    Event_ClearFlag(0xa13);
    Event_ClearFlag(0xa15);
  }
  if (Func_02000290() == 2)
    Event_ClearFlag(0xa16);
  else
    Event_SetFlag(0xa16);
  if (Event_GetFlag(0xa17))
    Event_ClearFlag(0xa1a);
  else
    Event_SetFlag(0xa1a);
  Actor_Get(18)->motionFlags = 0;
  Actor_Get(19)->motionFlags = 0;
  Actor_Get(20)->motionFlags = 0;
  Func_020002f0();
  if (!Event_GetFlag(0xa1a)) {
    object = Actor_Get(20);
    value = Func_02002b5c(object->x, object->z, 0);
    Func_02002b54(object->x, object->z, 0, value + 2);
    Func_02002c4c(object->x, object->z, 0, 0x80);
  }
  Func_020005e8(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

#include "jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "dma.h"
#include "io_reg.h"
#include "field_event.h"

void Func_0200115c(s32 value) { gData_02003594 = value; }

extern s32 gData_02003590;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gData_02003590 = value; }
extern u32 gData_020035bc[];

void Func_02001168(void) {
  u16 colors[16];
  s32 phase = (gData_02003590 / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_02003594 != 0) {
    u8 *work = Ram_HeapSlots->eventWork;
    s16 *state = (s16 *)(work + 358);

    if (*state != 0 && (*state & 0x3ff) != 153)
      return;
    if (*(s8 *)(work + 0xca4) != 0)
      return;
    if (*(s16 *)(work + 350) != 0)
      return;
  }
  i = 0;
  offset = 4;
next:
  p = colors;
  {
    /* FAKEMATCH(temporary): the slot and the table address held in locals;
     * as colors[(phase + i) % 5] = *(u16 *)((u8 *)gData_020035bc + offset),
     * 2 of 83 instructions differ in tla-en. */
    s32 slot = (phase + i) % 5;
    u8 *table = (u8 *)gData_020035bc;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_02003590++;
}

extern u16 gData_020035dc[];
extern u16 gData_020035e0[];

void Func_02002c44();

void Func_02001224(s32 mode) {
  gData_02003594 = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_020035bc, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWords(gData_020035dc, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(0xa00) || mode == 1) {
    Counter_Set(0);
    Task_Add(Func_02001168, 1152);
  } else {
    Dma_Set(gData_020035e0, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),
            REG_DMA3);
  }
}

void Func_020012a8(s32 id) {
  struct Actor *object;
  s32 height;

  object = Actor_Get(id);
  object->motionFlags = 4;
  height = object->terrainHeight;
  WORD_AT(object, 68) = 0;
  object->y = height + 0x200000;
  object->priorityFlags |= 128;
  height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
  Func_02002b54(object->x, object->z, 0, (height >> 19) + 6);
  Func_02002c44(object->x, object->z, 0, 128);
}

#include "frame.h"
#include "field_sprite.h"
#include "event_work.h"

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z,
                                                   s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

void Func_020012fc(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  u8 *p;
  s32 angle;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  i = 0;
  work->unkd46 = i;

  actor->motionFlags = 4;
  Actor_SetSingleChildFlags(actor, 0);

  while (*((u8 *)linked + 0x63) != 0) {
    actor->x = linked->x;
    actor->y = linked->y + i;
    actor->z = linked->z;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->facing = (u8)gFrameCount << 12;
    p = &actor->priorityFlags;
    *p ^= 2 << (gFrameCount & 1);
    System_Wait(1);
  }
  work->unkd44 = 1;
  work->unkd46 = 1;
  angle = linked->facing;
  actor->x = (((linked->x >> 19) & ~1) + (angle >> 13) - 1) << 19;
  actor->z = (((linked->z >> 19) & ~1) - ((angle >> 13) & 2) + 1) << 19;
  actor->velocityY = 0x60000;
  actor->priorityFlags = 1;
  actor->motionFlags = 3;
  Actor_SetSingleChildFlags(actor, 1);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  shadow = FieldCamera_GetActor();
  shadow->speedLimit = 0x80000;
  shadow->acceleration = 0x10000;
  shadow->motionFlags = 0;
  FieldActor_SetMoveTarget(
      shadow, actor->x, Map_GetTerrainHeight(0, actor->x, actor->z), actor->z);
  i = 0;
  if (actor->y > actor->terrainHeight + 0x100000) {
    do {
      System_Wait(1);
      i++;
      if (i > 59)
        break;
    } while (actor->y > actor->terrainHeight + 0x100000);
  }
  Event_PlaySound(127);
  i = 0;
  if (actor->velocityY != 0) {
    do {
      System_Wait(1);
      i++;
      if (i > 59)
        break;
    } while (actor->velocityY != 0);
  }
  Actor_WaitForMove(shadow);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  work->unkd46 = 1;
  work->exit = 0;
  Event_End();
}

s32 Func_020014e8(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;

  return Iwram_Sqrt(dx * dx + dz * dz);
}

extern const s32 gData_02003598[];
extern s32 Data_020035b8;

s32 Func_02002c34(s32 x, s32 z, s32 layer);
s32 Math_Cosine(s32 angle);
void Actor_StartScript(struct Actor *object, const s32 *script);

void Func_02001514(struct Actor *self) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 distance;
  s32 near;

  if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||
      *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0) {
    Actor_SetAnimationSpeed(self, 0);
    return;
  }
  Actor_SetAnimationSpeed(self, 16);
  if ((++*(u16 *)&self->timer & 31) == 31)
    Event_PlaySound(231);
  {
    s8 *to = self->sprite;
    s8 *from = leader->sprite;
    u32 a = from[9] & 12;
    to[9] = (s8)((to[9] & ~12) | a);
  }
  if (Func_02002c34(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Camera_WorldToScreen(position);
    distance = position[0];
    if (distance > 0x1100000)
      goto stop;
    if (distance < -0x200000)
      goto stop;
    {
      if (*((u8 *)self + 0x62) == 0) {
        distance = leader->y - self->y;
        if (distance < 0)
          distance = self->y - leader->y;
        near = 0;
        if (Func_020014e8(&leader->x, &self->x) <= 12 && distance < 0xc0000)
          near = 1;
        if (near && !Event_GetFlag(260)) {
          leader->facing = self->facing;
          leader->watchedObject = self;
          leader->priorityFlags &= ~1;
          {
            s32 delay = 200;
            *(u16 *)(work + 362) = delay;
          }
          gMainRoutineWork.renderMode = 0;
          *((u8 *)self + 0x62) = 1;
          *((u8 *)self + 0x63) = 1;
        }
      }
      {
        s32 (*multiply)(s32, s32);
        s32 cosine = Math_Cosine(self->facing);
        s32 speed = self->speedLimit;
        /* FAKEMATCH(dead): the routine address taken inside a once-only
         * block, so the speed loads first; as a plain call, 2 of 197
         * instructions differ in tla-en. */
        do {
          multiply = Iwram_MulQ16;
        } while (0);
        self->x += multiply(cosine, speed);
      }
      self->z += Iwram_MulQ16(Math_Sin(self->facing), self->speedLimit);
      return;
    }
  }
stop:
  {
    s32 zero = 0;
    *((u8 *)self + 0x63) = zero;
    self->update = zero;
    Actor_StartScript(self, gData_02003598);
    Event_PlaySound(228);
    Data_020035b8 = zero;
  }
}
