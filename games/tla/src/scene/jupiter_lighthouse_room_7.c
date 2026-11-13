/// @file
/// The scene code of overlay 0x6a0, in ROM order.

#include "flags.h"
#include "shared/whirlwind.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/actors.h"
#include "shared/effects.h"
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

/* VIEW(first): routine gData_020031c4 as this part declared it; the joined file has another. */
extern const u8 gData_020031c4__first[] __asm__("gData_020031c4");
#define gData_020031c4 gData_020031c4__first
/* VIEW(first): routine gPaletteCyclePausesInEvents as this part declared it; the joined file has another. */
extern s32 gPaletteCyclePausesInEvents__first __asm__("gPaletteCyclePausesInEvents");
#define gPaletteCyclePausesInEvents gPaletteCyclePausesInEvents__first
extern s16 gLightSpriteCount;
extern u16 gLightSpritePriority;
void JupiterLighthouse_OpenDoor(s32 value);
void JupiterLighthouse_CloseDoor(void);
void JupiterLighthouse_CheckBlockOnCell(void);
void MapGrid_SetCellHeight(s32 x, s32 z, s32 a, s32 b);
void MapGrid_ClearAttributeBBitsAt(s32 x, s32 z, s32 a, s32 b);
void Field_TryJumpForward(void);

void Func_02000038(void) { Field_RequestDjinnBattle(21, 74); }

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

#include "jupiter_lighthouse_room_7.h"
#include "main_routine_work.h"
#include "call.h"

DEFINE_JUPITER_LIGHTHOUSE_OPEN_DOOR(JupiterLighthouse_OpenDoor, 76, 42, 4, 76,
                                    35, 81, 42, 12, 41, 12, 35)

DEFINE_JUPITER_LIGHTHOUSE_CLOSE_DOOR(JupiterLighthouse_CloseDoor, 76, 42, 4, 76,
                                     35, 71, 12, 36, 12, 35)

DEFINE_JUPITER_LIGHTHOUSE_CHECK_BLOCK_ON_CELL(
    JupiterLighthouse_CheckBlockOnCell, 16, 0xa1b, 8, 37,
    JupiterLighthouse_OpenDoor)

DEFINE_BUTTON_STEP_ON(JupiterLighthouse_StepOnButton, 0x202,
                      JupiterLighthouse_OpenDoor)

DEFINE_BUTTON_STEP_OFF(JupiterLighthouse_StepOffButton, 0x202,
                       JupiterLighthouse_CloseDoor,
                       JupiterLighthouse_CheckBlockOnCell)

void Func_02000184(void) {
  System_Wait(10);
  JupiterLighthouse_CheckBlockOnCell();
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
  Call4((void (*)())MapGrid_SetCellHeight, 0x2300000, 0xf00000, 0, -2);
  Call4((void (*)())MapGrid_SetCellHeight, 0x1f00000, 0x1000000, 0, -2);
  Call4((void (*)())MapGrid_SetCellHeight, 0x2100000, 0xa00000, 0, -2);
  FieldMapGrid_SetCellBehavior(0x2600000, 0xa00000, 2, 0);
  FieldMapGrid_SetCellBehavior(0x1f00000, 0xa00000, 2, 0);
  FieldMapGrid_SetCellBehavior(0x1f00000, 0xe00000, 2, 0);
  if (Event_GetFlag(0xa10) || Event_GetFlag(0xa11))
    MapGrid_SetCellHeight(0x2300000, 0xf00000, 0, 0);
  if (Event_GetFlag(0xa12) || Event_GetFlag(0xa13))
    MapGrid_SetCellHeight(0x1f00000, 0x1000000, 0, 0);
  if (Event_GetFlag(0xa14) || Event_GetFlag(0xa15))
    MapGrid_SetCellHeight(0x2100000, 0xa00000, 0, 0);
  if (Event_GetFlag(0xa10)) {
    MapGrid_CopyCellsLayered(0x60, 0x38, 7, 2, 0x60, 0xf);
    MapGrid_CopyCellsLayered(0x6f, 0x33, 1, 1, 0x63, 0xf);
    if (Event_GetFlag(0xa13)) {
      MapGrid_CopyCellsLayered(0x69, 0x33, 1, 3, 0x5f, 0xe);
      FieldMapGrid_SetCellBehavior(0x1f00000, 0xe00000, 2, 0xec);
    }
    if (Event_GetFlag(0xa15))
      MapGrid_CopyCellsLayered(0x6f, 0x35, 1, 1, 0x61, 0xa);
  } else if (Event_GetFlag(0xa11)) {
    MapGrid_CopyCellsLayered(0x62, 0x33, 5, 1, 0x62, 0xa);
    MapGrid_CopyCellsLayered(0x63, 0x34, 4, 5, 0x63, 0xb);
    FieldMapGrid_SetCellBehavior(0x2600000, 0xa00000, 2, 0xec);
    if (Event_GetFlag(0xa12))
      MapGrid_CopyCellsLayered(0x6a, 0x35, 1, 1, 0x5f, 0x10);
    if (Event_GetFlag(0xa14)) {
      MapGrid_CopyCellsLayered(0x6d, 0x33, 3, 1, 0x5f, 0xa);
      FieldMapGrid_SetCellBehavior(0x1f00000, 0xa00000, 2, 0xec);
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
    MapGrid_SetCellHeight(0x800000, 0xc00000, 0, 4);
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
    MapGrid_SetCellHeight(0x800000, 0xc00000, 0, 6);
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
    MapGrid_SetCellHeight(object->x, object->z, 0, 6);
    MapGrid_ClearAttributeBBitsAt(object->x, object->z, 0, 128);
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
void MapGrid_SetAttributeBBitsAt(s32 x, s32 z, s32 a, s32 b);

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
    Actor_SetSpriteFlags(Actor_GetById(id), 0);
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
    FieldCamera_WaitForArrival();
    Event_Wait(20);
    Event_PlaySound(151);
    Actor_SelectAnimation(object, 3);
    object->priorityFlags &= ~2;
    FieldMapGrid_SetCellBehavior(object->x, object->z, object->terrainId, 255);
    Event_Wait(30);
    EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
    FieldCamera_WaitForArrival();
    Event_End();
    return;
  }
  if (id != 20)
    return;
  if (object->x >> 20 == 8 && *(s32 *)&object->z >> 20 == 12) {
    Event_SetFlag(2586);
    Actor_SetSpriteFlags(Actor_GetById(20), 0);
    Event_Wait(30);
    Event_PlaySound(167);
    Func_020005e8(1);
    if (!Event_GetFlag(2583))
      return;
    object = Actor_GetById(9);
    Event_ClearFlag(2583);
    Event_Wait(20);
    Event_PlaySound(151);
    Actor_SelectAnimation(object, 3);
    object->priorityFlags &= ~2;
    FieldMapGrid_SetCellBehavior(object->x, object->z, object->terrainId, 255);
  } else {
    MapGrid_SetCellHeight(object->x, object->z, 0, 8);
    MapGrid_SetAttributeBBitsAt(object->x, object->z, 0, 128);
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
  FieldCamera_WaitForArrival();
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
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
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

void Leader_PushObjectAhead(void);

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
      Field_TryJumpForward();
      return;
    }
  }
  if (found == 1)
    Leader_PushObjectAhead();
}

void Func_02000e20(void) { Field_TryJumpForward(); }

#include "ram_buffer.h"

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Actors_SetCellsOpen();
void LightSprites_InitB();
void LightSprites_AddCounted();

s32 Scene_Initialize(void) {
  s32 t1, t2, t3, t5, t8, t15, t16, t17, t19, t20, t21, t23, t24, t25, t26, t27,
      t28, t29, v5;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  if ((u32)((FIELD_AT(&gMainRoutineWork, u16, 482) + -3) << 16) > (u32)0x10000)
    goto L_02008e6e;
  t1 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  FIELD_AT(t1, u8, 35) = (32 | FIELD_AT(t1, u8, 35));
L_02008e6e:
  t2 = ((s32)Actor_Get(8));
  FIELD_AT(t2, s32, 24) = 0xc000;
  t3 = ((s32)Actor_Get(8));
  FIELD_AT(t3, s32, 28) = 0xc000;
  Actors_SetCellsOpen((s32)&gData_020031c4);
  t5 = Event_GetFlag(0xa1b);
  if (t5 == 0)
    goto L_02008eaa;
  Actor_SetPosition(16, PIXELS(136), PIXELS(600));
  JupiterLighthouse_OpenDoor(0);
L_02008eaa:
  t8 = Event_GetFlag(0x202);
  if (t8 == 0)
    goto L_02008ebe;
  JupiterLighthouse_OpenDoor(0);
L_02008ebe:
  LightSprites_InitB();
  LightSprites_SetPriority(2);
  LightSprites_AddCounted(18, 2);
  LightSprites_AddCounted(19, 1);
  LightSprites_AddCounted(20, 0);
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

void JupiterLighthouse_LoadPalettes(s32 mode);
s32 MapGrid_GetCellHeight(s32 x, s32 z, s32 layer);

s32 Scene_PrepareMap(void) {
  struct Actor *object;
  s32 value;

  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(1);
  if (gMainRoutineWork.entrance == 12) {
    Actor_SetPosition(10, PIXELS(168), PIXELS(264));
    Actor_SetPosition(11, PIXELS(200), PIXELS(264));
    Actor_SetPosition(12, PIXELS(232), PIXELS(264));
    Actor_SetPosition(13, PIXELS(168), PIXELS(296));
    Actor_SetPosition(14, PIXELS(136), PIXELS(328));
    Actor_SetPosition(15, PIXELS(168), PIXELS(328));
  }
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) && Func_02000290() != 2) {
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
    value = MapGrid_GetCellHeight(object->x, object->z, 0);
    MapGrid_SetCellHeight(object->x, object->z, 0, value + 2);
    MapGrid_SetAttributeBBitsAt(object->x, object->z, 0, 0x80);
  }
  Func_020005e8(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

#include "shared/jupiter_lighthouse_aerie.h"
#include "dma.h"
#include "io_reg.h"
#include "field_event.h"

void PaletteCycle_SetPausesInEvents(s32 value) {
  gPaletteCyclePausesInEvents = value;
}

/* VIEW(first): routine gPaletteCycleFrame as this part declared it; the joined file has another. */
extern s32 gPaletteCycleFrame__first __asm__("gPaletteCycleFrame");
#define gPaletteCycleFrame gPaletteCycleFrame__first

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gPaletteCycleFrame = value; }
extern u32 gPaletteCycleColors[];

DEFINE_PALETTE_CYCLE_STEP(PaletteCycle_Step, gPaletteCycleFrame,
                          gPaletteCyclePausesInEvents, gPaletteCycleColors)

extern u16 gLighthousePalette13[];
extern u16 gLighthouseUnlitColors[];

void MapGrid_SetAttributeBAt();

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES(JupiterLighthouse_LoadPalettes,
                                        gPaletteCyclePausesInEvents,
                                        gPaletteCycleColors,
                                        gLighthousePalette13, PaletteCycle_Step,
                                        gLighthouseUnlitColors)

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, MapGrid_SetCellHeight,
                             MapGrid_SetAttributeBAt)

#include "frame.h"
#include "field_sprite.h"

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z,
                                                   s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

DEFINE_WHIRLWIND_CARRY_LEADER(Whirlwind_CarryLeader)

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

/* VIEW(first): routine gWhirlwindEndScript as this part declared it; the joined file has another. */
extern const s32 gWhirlwindEndScript__first[] __asm__("gWhirlwindEndScript");
#define gWhirlwindEndScript gWhirlwindEndScript__first
extern s32 gWhirlwindRiding;

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
s32 Trig_Cos(s32 angle);
void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE(Whirlwind_Update, MapGrid_GetCellBehavior,
                        Position_FlatDistance, gWhirlwindEndScript,
                        gWhirlwindRiding)

// ---- scene/jupiter_lighthouse_room_7/s16d4.c ----

/// Scene code of overlay 0x6a0, in ROM order.

#include "shared/map_cells.h"
#include "field_effect.h"
#include "field_coordinate.h"
#include "map.h"
#include "vram_tab.h"
#include "task.h"
#include "item_icon.h"
#include "module/scene/jupiter_lighthouse_room_7.h"
#include "module/scene/scene.h"

void Actor_SetSpriteFlags(struct Actor *object, u32 value);
void Whirlwind_Update(struct Actor *self);
/* VIEW(s16d4): routine FieldEffect_Spawn as this part declared it; the joined file has another. */
void FieldEffect_Spawn__s16d4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags, struct EffectOptions *options) __asm__("FieldEffect_Spawn");
#define FieldEffect_Spawn FieldEffect_Spawn__s16d4

DEFINE_WHIRLWIND_SPAWN(Whirlwind_Spawn, Actor_SelectAnimation, Whirlwind_Update,
                       FieldEffect_Spawn)

DEFINE_ACTOR_EMIT_RISING_SPARKS(Actor_EmitRisingSparks, FieldEffect_Spawn)

DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(
    JupiterLighthouse_PlaceRaisedActors)

DEFINE_ACTORS_SET_CELLS_OPEN(Actors_SetCellsOpen, Actor_SetCellOpen)

// Finds the current target's table row and shrinks it.
void Func_02001a48(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  if (*list == 0xffff)
    return;
  // A goto loop: as a do-while loop, 42 of 61 instructions differ in tla-en.
top:
  {
    u32 entry = *list;
    /* FAKEMATCH(dead): the effect work read through its own address; as
     * work->objectId, 21 of 61 instructions differ in tla-en. */
    s32 id = (*&work)->objectId;

    if (id == entry) {
      struct Actor *object = Actor_Get(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        Event_SetFlag(flag);
        Actor_SetCellOpen(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      return;
    }
  }
  list += 3;
  if (*list != 0xffff)
    goto top;
}

DEFINE_ACTOR_SET_CELL_OPEN(Actor_SetCellOpen, Actor_GetCellIndex,
                           Actor_SelectAnimation)

extern u32 gLightSpriteActors[];
/* VIEW(s16d4): variable gLightSpritePriority as s16 gLightSpritePriority;
 * scene/jupiter_lighthouse_room_7 has u16 gLightSpritePriority; tried as the
 * rest of the object declares it, LightSprites_Draw moves: 99 of 200
 * instructions, LightSprites_InitA moves: 0 of 36 instructions,
 * LightSprites_InitB moves: 0 of 36 instructions, 1 more in tla-ja. */
extern s16 gLightSpritePriority__s16d4 __asm__("gLightSpritePriority");

s32 BattleEffect_GetObjectPriority(struct Actor *actor);

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, gLightSpriteOams,
                          gLightSpriteCount, gLightSpriteActors,
                          gLightSpritesLarge, gLightSpritePriority__s16d4,
                          BattleEffect_GetObjectPriority,
                          Oam_AddSpriteAtDepthClamped)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority__s16d4)

DEFINE_LIGHT_SPRITES_INIT_B(LightSprites_InitB, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority__s16d4)

DEFINE_LIGHT_SPRITES_INIT_C(LightSprites_InitC, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority__s16d4)

DEFINE_LIGHT_SPRITES_ADD_COUNTED(LightSprites_AddCounted, gLightSpriteActors,
                                 gLightSpriteCount)

void LightSprites_SetPriority(u16 value) {
  gLightSpritePriority__s16d4 = value;
}

#include "../field/object/set_sprite_priority.c"

#include "../field/field_effect/move.c"

#undef FIELD_AT
#undef gData_020031c4
#undef gPaletteCyclePausesInEvents
#undef gPaletteCycleFrame
#undef gWhirlwindEndScript
#undef FieldEffect_Spawn
// ---- from scene/jupiter_lighthouse_room_7/s1f74.c ----
// @file
// Scene code of overlay 0x6a0, in ROM order.

#include "shared/cracked_tile.h"
#include "types.h"
#include "field_effect.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/scene/scene.h"

extern const s32 *gFieldEffect_DurationScripts[];

/* VIEW(s1f74): routine Actor_SelectAnimation as this part declared it; the joined file has another. */
void Actor_SelectAnimation__s1f74(struct FieldEffect *effect, s32 mode) __asm__("Actor_SelectAnimation");
#define Actor_SelectAnimation Actor_SelectAnimation__s1f74
/* VIEW(s1f74): routine Actor_StartScript as this part declared it; the joined file has another. */
void Actor_StartScript__s1f74(struct FieldEffect *effect, const s32 *script) __asm__("Actor_StartScript");
#define Actor_StartScript Actor_StartScript__s1f74
void Actor_SetSpriteFlags(struct Actor *object, u32 value);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = ((struct FieldEffect *)Import_ActorCreate(options->type, x, y, z));
  else
    effect = ((struct FieldEffect *)Import_ActorCreate(285, x, y, z));
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect,
                               ((struct FieldSprite *)party->sprite)->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 target;
    s32 frames;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    frames = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / frames;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (target - 0x10000) / frames;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

// The cells CrackedTiles_Step watches, and the cell it copies over the one the
// leader steps on.
#define CELL_TABLE gCrackedTiles
#define CELL_SPRITE_DRAW CrackedTiles_Update
/* VIEW(s1f74): routine Oam_AddSpriteAtDepthClamped as cell_sprite.c declares it; the
 * joined file has another. */
void Oam_AddSpriteAtDepthClamped__s1f74(void *entry, s32 priority) __asm__("Oam_AddSpriteAtDepthClamped");
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped__s1f74
#include "../field/map/cell_sprite.c"

#include "ram_buffer.h"
#include "actor.h"
#include "iwram_call.h"
#include "scene_tables.h"
#include "cell_target.h"

DEFINE_CRACKED_TILES_FIND(CrackedTiles_Find, gCrackedTiles, CrackedTiles_Update)

DEFINE_LEADER_CENTER_IN_CELL(Leader_CenterInCell)

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
DEFINE_CRACKED_TILE_CRACK(CrackedTile_Crack, Leader_CenterInCell)

// ---- scene/jupiter_lighthouse_room_7/s25a4.c ----

// Scene code of overlay 0x6a0, in ROM order.

#include "io_reg.h"
#include "field.h"
#include "event_work.h"
#include "jupiter_lighthouse_room_7.h"

struct CellTargetEntry;

/* VIEW(s25a4): tag struct CellTarget as struct CellTarget__s25a4; cell_target.h
 * has another definition; tried with the definition the rest of the object has,
 * CrackedTile_Fall moves: 2 of 131 instructions in tla-ja. */
struct CellTarget__s25a4 {
  s16 firstFlag;
  s16 flagByte;
  s16 kind;
  s16 x;
  s16 z;
  s16 count;
  s16 current;
  s16 shown;
  s16 tile;
};

#include "io_write.h"

/* VIEW(s25a4): routine CrackedTile_Fall as this part defines it; the scene's
 * module header declares it with the joined file's CellTarget. */
void CrackedTile_Fall__s25a4(struct CellTarget__s25a4 *table, struct CellTargetEntry *entry) __asm__("CrackedTile_Fall");
DEFINE_CRACKED_TILE_FALL(CrackedTile_Fall__s25a4, CellTarget__s25a4,
                         Leader_CenterInCell)

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
#undef Actor_SelectAnimation
#undef Actor_StartScript
// ---- from scene/jupiter_lighthouse_room_7/s273c.c ----
// @file
// Scene code of overlay 0x6a0, in ROM order.

#include "shared/cracked_tile.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "cell_target.h"
#include "event_work.h"
#include "module/field/field_effect.h"
#include "module/scene/jupiter_lighthouse_room_7.h"
#include "module/scene/scene.h"

/* VIEW(s273c): routine FieldEffect_Spawn as this part declared it; the joined file has another. */
void FieldEffect_Spawn__s273c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags, struct EffectOptions *options) __asm__("FieldEffect_Spawn");
#define FieldEffect_Spawn FieldEffect_Spawn__s273c

void CrackedTile_DropIn(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  u32 i;
  struct EffectOptions options;
  s32 always;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  actor->y = 0x820000;
  actor->gravity = 0x8000;
  actor->bounce = 0;
  actor->motionFlags = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(204);
  actor->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  always = 1;
  i = 0;
  // A goto loop: as a do-while loop, 4 of 169 instructions differ in tla-en.
loop:
  /* FAKEMATCH(dead): the body tested on a flag that is always set; without
   * the test, 4 of 169 instructions differ in tla-en. */
  if (always) {
    s32 angle;
    s32 velocity[3];

    angle = i << 12;
    velocity[0] = Trig_Cos(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += (Random16() * 0xc000 >> 16) - 0x6000;
    velocity[2] += (Random16() * 0x6000 >> 16) - 0x3000;
    FieldEffect_Spawn(actor->x, actor->y, actor->z, velocity[0], velocity[1],
                      velocity[2], 0x1090001, &options);
  }
  i++;
  if (i <= 16)
    goto loop;
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  actor->gravity = 0x10000;
  actor->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Event_End();
}

extern struct CellTarget gCrackedTiles;

void GameFlag_SetByte(s32 flag, s32 value);

DEFINE_CRACKED_TILES_STEP(CrackedTiles_Step, gCrackedTiles, CrackedTile_Crack,
                          CrackedTile_Fall)

DEFINE_CRACKED_TILE_PLACE_LANDING(CrackedTile_PlaceLanding, gCrackedTiles,
                                  CrackedTile_DropIn)

#include "scene_tables.h"

// clang-format off
/* VIEW(s273c): variable gData_02002c64 as this part defines it; a header the joined file includes has another. */
s16 gData_02002c64__s273c[] __asm__("gData_02002c64") = {
    8, 16, 10, 16, 11, 16, 12, 16,
    14, 16, 15, 16, 10, 18, 12, 18,
    13, 18, 8, 20, 10, 20, 13, 20,
    15, 20, -1, 0,
};
// clang-format on

#undef FieldEffect_Spawn
// ---- from scene/data/jupiter_lighthouse_room_7.c ----

#include "macros.h"
#include "shared/tables.h"
#include "types.h"
#include "scene_tables.h"
#include "objcmd.h"
#include "module/scene/jupiter_lighthouse_room_7.h"

void Func_02000038(void);
void JupiterLighthouse_StepOnButton(void);
/* VIEW(data_7): routine Func_02000778 as this part declared it; the joined file has another. */
void Func_02000778__data_7(void) __asm__("Func_02000778");
#define Func_02000778 Func_02000778__data_7
/* VIEW(data_7): routine Func_020007a4 as this part declared it; the joined file has another. */
void Func_020007a4__data_7(void) __asm__("Func_020007a4");
#define Func_020007a4 Func_020007a4__data_7
/* VIEW(data_7): routine Func_020009e4 as this part declared it; the joined file has another. */
void Func_020009e4__data_7(void) __asm__("Func_020009e4");
#define Func_020009e4 Func_020009e4__data_7
void Func_02000e20(void);

// clang-format off
LATE_DATA DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

LATE_DATA DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

LATE_DATA DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

LATE_DATA s16 gData_020031c4[] = {
    17, 1, 0xa16, 9, 1, 0xa17, -1, 0,
};

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA DEFINE_G_SCENE_EXITS_3(260, 262, 260, 7, 6, 263, 10, 9, 12, 260)

LATE_DATA struct ActorPlacement gActorPlacements[] = {
    {340, CONDITION_ALWAYS, 1, PIXELS(564), 0, PIXELS(712), FACING_SOUTHWEST, 2, 0},
    {406, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(216), FACING_SOUTHWEST, 2, 0},
    {316, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(264), FACING_EAST, 2, 0},
    {316, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(264), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(376), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(376), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(376), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(376), FACING_EAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(600), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, 1, PIXELS(680), 0, PIXELS(136), FACING_SOUTHWEST, 2, 0},
    {428, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {428, CONDITION_ALWAYS, 1, PIXELS(696), 0, PIXELS(232), FACING_SOUTH, 2, 0},
    {428, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(200), FACING_SOUTH, 2, 0},
    {246, 0x7a, 1, PIXELS(440), 0, PIXELS(216), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gSceneEvents[] = {
    {33, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {33, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {33, 9, CONDITION_ALWAYS, 9},
    {49, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {514, 30, IF_FLAG_SET(0xa1a), EVENT(Func_02000e20)},
    {TRIGGER_STEP, 20, 0xa1b, EVENT(JupiterLighthouse_StepOnButton)},
    {TRIGGER_STEP, 21, 0xa1b, EVENT(JupiterLighthouse_StepOffButton)},
    {0x8602, 40, CONDITION_ALWAYS, EVENT(Func_02000cec)},
    {0xc602, 41, CONDITION_ALWAYS, EVENT(Func_02000cec)},
    {0x602, 42, CONDITION_ALWAYS, EVENT(Func_02000cec)},
    {0x4602, 43, CONDITION_ALWAYS, EVENT(Func_02000cec)},
    {0xc400, 21, CONDITION_ALWAYS, EVENT(Func_02000038)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 18, CONDITION_ALWAYS, EVENT(Func_02000778)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 18, CONDITION_ALWAYS, EVENT(Func_020007a4)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 19, CONDITION_ALWAYS, EVENT(Func_02000778)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, CONDITION_ALWAYS, EVENT(Func_020007a4)},
    {8, 35, CONDITION_ALWAYS, EVENT(Func_02000778)},
    {9, 35, CONDITION_ALWAYS, EVENT(Func_020007a4)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 20, CONDITION_ALWAYS, EVENT(Func_02000778)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 20, CONDITION_ALWAYS, EVENT(Func_020007a4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, 0xa1b, EVENT(Func_02000184)},
    {0x80008615, 17, 0xa16, EVENT(Func_020009d8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 17, 0xa16, EVENT(Func_020009e4)},
    {PSYNERGY_ON_ACTOR(ABILITY_POUND), 17, 0xa16, EVENT(Func_02000a74)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9, 0xa17, EVENT(Func_020009e4)},
    {PSYNERGY_ON_ACTOR(ABILITY_POUND), 9, 0xa17, EVENT(Func_02000c7c)},
    {TRIGGER_TABLE_END},
};

LATE_DATA s32 gPaletteCycleFrame[] = {
    -1,
};

LATE_DATA u32 gPaletteCyclePausesInEvents[] = {
    1,
};

LATE_DATA u32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
#undef Func_02000778
#undef Func_020007a4
#undef Func_020009e4
