/// @file
/// Scene code of overlay 0x697, in ROM order.

#include "../shared/trial_road.h"
#include "event_inline.h"
#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "macros.h"
#include "text/msg_ids.h"
#include "vector3.h"
#include "field.h"
#include "event_work.h"
#include "module/scene/trial_road.h"
#include "module/window/window.h"

/// 2 copies: trial_road/s0218, trial_road/s0218.
#define DEFINE_FUNC_02000218(name, n1, n2, n3, n4, n5, n6)                     \
  void name(s32 side, s32 phase) {                                             \
    if (side == 0) {                                                           \
      if (phase == 3)                                                          \
        Func_0802b450(0, 120, 1, 2, n1, phase);                                \
      MapGrid_CopyCellsLayered(127, 3, 1, 2, n2, 3);                           \
      MapGrid_CopyCellAttributes(n3, 127, 1, 1, n1, 8);                        \
    } else {                                                                   \
      if (phase == 3)                                                          \
        Func_0802b450(0, 120, 1, 2, n4, phase);                                \
      MapGrid_CopyCellsLayered(127, 3, 1, 2, n5, 3);                           \
      MapGrid_CopyCellAttributes(n6, 127, 1, 1, n4, 8);                        \
    }                                                                          \
  }

DEFINE_FUNC_02000218(Func_02000218, 23, 57, 0, 9, 43, 1)

DEFINE_FUNC_02000218(Func_020002a4, 28, 62, 1, 4, 38, 0)

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02000330(void) {
  s32 t1, t2, t9, v_r5, v_r6;

  t1 = ((s32)Actor_GetById(gMainRoutineWork.currentOwner));
  v_r5 = t1;
  v_r6 = (s32)Ram_HeapSlots->eventWork;
  t2 = GameFlag_Test(0x9ff);
  if (t2 == 0)
    goto L_02008400;
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner,
                            (((FIELD_AT(v_r5, s32, 8) >> 20) << 4) + 8),
                            (((FIELD_AT(v_r5, s32, 16) >> 20) << 4) + 8));
  Sound_Play(229);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  v_r5 = (s32)&MsgTrialRoadGiveUpPrompt;
  FieldMessage_Show(v_r5, 13);
  v_r5 = (v_r5 + 1);
  FIELD_AT((v_r6 + 0xcb0), s16, 0) = v_r5;
  FIELD_AT((v_r6 + 0xcb2), s16, 0) = 4;
  FIELD_AT((v_r6 + 0xcb4), s16, 0) = 1;
  t9 = Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 1);
  if (t9 != 0)
    goto L_02008400;
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Event_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
  Event_Wait(10);
  Func_080d47b4(gMainRoutineWork.currentOwner, 0x109, 0);
  Event_Wait(30);
  Func_02005868();
  Import_EventSetExitRequest(9);
  return 1;
L_02008400:
  GameFlag_SetBit(0x281);
  return 0;
}

void Func_02000418(void) { GameFlag_ClearBit(0x281); }

void Func_02000428(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct TrialRoadState *state = gTrialRoadState;
  s32 found;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner,
                                   ((leader->x >> 20) << 4) + 8,
                                   ((leader->z >> 20) << 4) + 8);
  TrialRoad_Idle();
  found = Func_020000fc(0);
  if (found != 0)
    GameFlag_SetBit(state->field16[0] + 897);
  Sound_Play(124);
  if (state->mirrored == 0) {
    Actor_SetModeById(9, 6);
    GameFlag_SetBit(645);
    if (found != 0) {
      Actor_SetModeById(10, 6);
      GameFlag_SetBit(646);
    }
  } else {
    Actor_SetModeById(11, 6);
    GameFlag_SetBit(645);
    if (found != 0) {
      Actor_SetModeById(12, 6);
      GameFlag_SetBit(646);
    }
  }
  GameFlag_SetBit(644);
  Event_End();
}

/// Puts an item in the chest: "{protagonist} put {actor}'s {item} in the
/// chest." (MsgTrialRoadStoredItem) with sound 125 while the chest object
/// Func_020059b8 picks plays motion 5; chests 9 and 11 clear flag 645, the
/// others 646, and once both are clear it calls Func_02000218 with the stage
/// state's mirrored and field16[0], plays sound 138 and sets flag 642. Then the
/// item leaves the owner's slot.
void Func_020004f4(s32 item, s32 owner, s32 slot) {
  s32 kind = Func_020059b8(Party_GetLeaderActor());
  struct TrialRoadState *state = gTrialRoadState;

  Event_Start();
  Event_ResolvePendingActions(0);
  MessageArgument_Push(owner, 1);
  MessageArgument_Push(item, 2);
  FieldMessage_Show((s32)&MsgTrialRoadStoredItem, 1);
  Sound_Play(125);
  Motion_SetModeAndWaitAnimation(kind, 5);
  if (kind == 9 || kind == 11)
    GameFlag_ClearBit(645);
  else
    GameFlag_ClearBit(646);
  if (!GameFlag_Test(645) && !GameFlag_Test(646)) {
    Func_02000218(state->mirrored, state->field16[0]);
    Sound_Play(138);
    GameFlag_SetBit(642);
  }
  Func_02005870(owner, slot);
  Event_End();
}

/* VIEW(s0218): tag struct EventWork15a as struct EventWork15a__s0218; module/tla.h has another definition; tried with the definition the rest of the object has, its members read value as flag, it does not compile in tla-ja: games/tla/src/scene/trial_road/s0218.c:171: dereferencing pointer to incomplete type. */
struct EventWork15a__s0218 {
  u8 unk000[0x15a];
  u16 value;
};

void Func_020005b4(s32 id) {
  s32 flag;

  if (id == 9 || id == 11)
    flag = 0x285;
  else
    flag = 0x286;
  if (!GameFlag_Test(flag)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldMessage_Show((s32)MsgTrialRoadChestBlocked, 1);
    Event_End();
  } else {
    FieldMessage_Show((s32)MsgTrialRoadPlaceItemPrompt, 13);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 1) == 0)
      ((struct EventWork15a__s0218 *)Ram_HeapSlots->eventWork)->value = 1;
  }
}

void Func_02000628(void) {}

void Func_0200062c(s32 id) {
  struct TrialRoadMapWork *map =
      (struct TrialRoadMapWork *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    u8 *cells = map->layers[0].cells;
    u8 *cell = cells + (((object->x >> 20) + ((object->z >> 20) << 7)) << 2);
    s32 bit = 128;

    cell[3] = bit | cell[3];
  }
}

void Func_02000660(s32 id) {
  struct TrialRoadMapWork *map =
      (struct TrialRoadMapWork *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    u8 *cells = map->layers[0].cells;
    u8 *cell = cells + (((object->x >> 20) + ((object->z >> 20) << 7)) << 2);

    cell[3] &= 0x7f;
  }
}

s32 Func_02000694(struct Actor *object) {
  struct Actor *owner = object->watchedObject;

  SET_POSITION(object, owner->x, owner->y + (OBJECT_BYTE(object, 99) << 16),
               owner->z);
  object->terrainHeight = owner->terrainHeight;
  return 1;
}

extern const u8 gData_02005a7c[];

void Func_020006b4(s32 id) {
  struct Actor *owner = Actor_GetById(id);
  struct Actor *object;

  if (owner != NULL) {
    object = Actor_Create(489, owner->x, owner->y, owner->z);
    if (object != NULL) {
      Actor_SetSingleChildFlags(object, 0);
      Actor_StartScript(object, gData_02005a7c);
      object->watchedObject = owner;
      object->priorityFlags |= 0x80;
      object->motionFlags = 0;
      object->collisionFlags = 0;
      OBJECT_BYTE(object, 99) = 16;
      owner->watchedObject = object;
    }
  }
}

void Func_0200070c(s32 id, s32 value) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    object = object->watchedObject;
    if (object != NULL)
      OBJECT_BYTE(object, 99) = value;
  }
}

/// While the party's render mode is 0, clears the map cell flag under the
/// object.
/// 2 copies: trial_road/s0218, trial_road/s0218.
#define DEFINE_FUNC_02000728(name, call1)                                      \
  void name(s32 a, s32 id) {                                                   \
    struct MainRoutineWork *party = &gMainRoutineWork;                         \
    u8 mode;                                                                   \
                                                                               \
    mode = party->renderMode;                                                  \
                                                                               \
    if (mode == 0)                                                             \
      call1(id);                                                               \
  }

DEFINE_FUNC_02000728(Func_02000728, Func_02000660)

/// While the party's render mode is 0, sets the map cell flag under the object.
DEFINE_FUNC_02000728(Func_02000748, Func_0200062c)

void Func_02000768(void) {
  OBJECT_BYTE(Actor_GetById(17), 98) = 0;
  GameFlag_SetBit(0x307);
}

void Func_02000784(void) { Func_020059c0(); }

void Func_0200078c(s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL)
    MapGrid_CopyCellsLayered(14, 27, 1, 1, object->x >> 20, object->z >> 20);
}

void Func_020007b4(void) {
  s32 y = Actor_GetById(gMainRoutineWork.currentOwner)->y;

  if (y == 0x200000)
    GameFlag_SetBit(0x208);
  else
    GameFlag_ClearBit(0x208);
}

void Func_020007e8(void) {
  if (Actor_GetById(16)->z >> 20 == 23)
    Func_020059c0();
  else
    Actor_JumpDownLedge(gMainRoutineWork.currentOwner, 6, 0);
}

void Func_02000818(s32 a, s32 id) {
  if ((u32)(id - 15) <= 1)
    Func_02000660(id);
}

void Func_0200082c(s32 a, s32 id) {
  s32 x;

  if ((u32)(id - 15) <= 1) {
    Func_0200062c(id);
    return;
  }
  x = (s32)Actor_GetById(id);
  x = ((struct Actor *)x)->x >> 20;
  id = 0;
  if (x == 13)
    id = 17;
  if (x == 15)
    id = 18;
  if (x == 17)
    id = 19;
  if (x == 19)
    id = 20;
  if (id != 0) {
    s32 flag = id + 0x203;

    if (GameFlag_Test(flag) == 0) {
      struct Actor *object = Actor_GetById(id);

      object->update = NULL;
      Animation_ApplyChildValues(object, 0);
      Actor_SetModeById(id, 6);
      Func_0200070c(id, -16);
      GameFlag_SetBit(flag);
    }
  }
}

void Func_020008a4(void) {
  if (GameFlag_Test(Func_020059b8(gMainRoutineWork.currentOwner) + 0x203))
    Func_020059c0();
}

void Func_020008d0(s32 a, s32 id) {
  struct Actor *object = Actor_GetById(id);

  Func_0200070c(id, 16);
  GameFlag_ClearBit(id + 0x203);
  if (object != NULL)
    object->priorityFlags &= 0xfd;
}

void Func_02000904(s32 a, s32 id) {
  OBJECT_BYTE(Actor_GetById(id - 5), 98) = 0;
  GameFlag_SetBit(id + 0x313);
}

void Func_02000924(s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    Func_0200070c(id, -16);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    Func_020057c8(object, 6);
  }
}

void Func_0200095c(s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    Func_0200070c(id, 16);
    object->collisionFlags = 1;
    object->priorityFlags &= 0xfd;
    Func_020057c8(object, 5);
  }
}

void Func_02000990(void) {
  OBJECT_BYTE(Actor_GetById(16), 98) = 0;
  GameFlag_SetBit(0x339);
}

extern const s8 gData_02005a88[4];

void Func_020009ac(s32 a, s32 b) {
  s32 id;

  id = 18;
  // Goto loops: as for or do-while loops, the table address and the 1 are moved
  // out of them (differ 29).
next:
  {
    struct Actor *object = Actor_GetById(id);
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;
    u8 *covered =
        &OBJECT_BYTE(Actor_GetById(gData_02005a88[(id - 18) & 3]), 99);
    s32 other;

    other = 15;
    *covered = 0;
    goto test;
  step:
    other++;
  test:
    if (other <= 17) {
      object = Actor_GetById(other);
      if (x != object->x >> 20)
        goto step;
      if (z != object->z >> 20)
        goto step;
      *covered = 1;
    }
  }
  id++;
  if (id <= 21)
    goto next;
}

void Func_02000a18(s32 id, s32 mode, s32 frame) {
  s32 other = gData_02005a88[(id - 18) & 3];
  struct Actor *object = Actor_GetById(id);
  struct Actor *partner = Actor_GetById(other);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  if (frame <= 10)
    object->scaleY = 0x10000 - (frame << 12);
  else
    object->scaleY = 0x10000;
  if (mode == 1) {
    Func_02000924(id);
    Func_0200095c(other);
    switch (id) {
    case 18:
      GameFlag_SetBit(821);
      break;
    case 19:
      GameFlag_ClearBit(821);
      break;
    case 20:
      GameFlag_SetBit(823);
      break;
    case 21:
      GameFlag_ClearBit(823);
      break;
    }
    if (partner->x >> 20 == leader->x >> 20 &&
        partner->z >> 20 == leader->z >> 20) {
      leader->velocityY = 0x80000;
      leader->x = (leader->x & 0xfff00000) + 0x80000;
      leader->z = (leader->z & 0xfff00000) + 0x80000;
    }
  }
  if (frame == 9)
    partner->scaleY = 0x13333;
  else if (frame == 11)
    partner->scaleY = 0x11999;
  else if (frame == 13)
    partner->scaleY = 0x10ccc;
  else
    partner->scaleY = 0x10000;
}

void Func_02000b3c(s32 a, s32 b) { Func_02000a18(18, a, b); }

void Func_02000b4c(s32 a, s32 b) { Func_02000a18(19, a, b); }

void Func_02000b5c(s32 a, s32 b) { Func_02000a18(20, a, b); }

void Func_02000b6c(s32 a, s32 b) { Func_02000a18(21, a, b); }

void Func_02000b7c(s32 id, s32 other, s32 flag, s32 side) {
  struct Actor *object = Actor_GetById(id);
  struct Actor *partner = Actor_GetById(other);
  s32 t;

  if (OBJECT_BYTE(object, 0x63) != 0)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  partner->scaleY = 0x2000;
  wait(1);
  Func_0200095c(other);
  t = 0x2000;
grow:
  Func_0200070c(id, (0x8000 - t) / 2048);
  object->scaleY = 0x10000 - t;
  partner->scaleY = t;
  wait(1);
  t += 0x2000;
  if (t <= 0x10000)
    goto grow;
  object->scaleY = 0x10000;
  Func_02000924(id);
  Sound_Play(138);
  if (side != 0)
    GameFlag_SetBit(flag);
  else
    GameFlag_ClearBit(flag);
  Event_End();
}

void Func_02000c34(void) { Func_02000b7c(18, 19, 0x335, 1); }

void Func_02000c48(void) { Func_02000b7c(19, 18, 0x335, 0); }

void Func_02000c5c(void) { Func_02000b7c(20, 21, 0x337, 1); }

void Func_02000c70(void) { Func_02000b7c(21, 20, 0x337, 0); }

void Func_02000c84(void) {
  s32 y = Actor_GetById(gMainRoutineWork.currentOwner)->y;

  if (y >= 0x100000) {
    Func_020059c0();
  } else if (Func_02005a30()) {
    Func_02005a38();
    Func_020009ac(0, 0);
  }
}

void Func_02000cc0(void) { Func_02004418(gData_02007734); }

void Func_02000cd0(s32 phase) {
  if (phase == 3)
    Func_020045ac(gData_02007734, 15);
}

void Func_02000ce4(s32 phase) {
  if (phase == 3)
    Func_020045ac(gData_02007734, 16);
}

// Blocks the three cells through an object's cell on layers 0 and 2 (collision code
// 255): the column above and below it in mode 0, the row beside it otherwise.
#include "map.h"

/// Blocks the three cells through an object's cell on layers 0 and 2 (collision
/// code 255): the column above and below it in mode 0, the row beside it
/// otherwise.
void Func_02000cf8(s32 id, s32 mode) {
  struct TrialRoadMapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(id);
  s32 index;
  struct MapCell *low;
  struct MapCell *high;

  if (object == 0)
    return;
  index = (object->x >> 20) + ((object->z >> 20) << 7);
  low = (struct MapCell *)map->layers[0].cells + index;
  high = (struct MapCell *)map->layers[2].cells + index;
  if (mode == 0) {
    low[-128].collisionCode = 255;
    low->collisionCode = 255;
    low[128].collisionCode = 255;
    high[-128].collisionCode = 255;
    high->collisionCode = 255;
    high[128].collisionCode = 255;
  } else {
    low[-1].collisionCode = 255;
    low->collisionCode = 255;
    low[1].collisionCode = 255;
    high[-1].collisionCode = 255;
    high->collisionCode = 255;
    high[1].collisionCode = 255;
  }
}

void Func_02000d70(void) {
  Map_CopyCellAttributeRect(11, 64, 11, 8, 11, 22);
  Map_CopyCellAttributeRect(79, 64, 11, 8, 79, 22);
  Func_02000cf8(16, 0);
  Func_02000cf8(17, 0);
  Func_02000cf8(18, 1);
  if (GameFlag_Test(0x253))
    MapGrid_CopyCellAttributes(0, 48, 1, 3, 19, 23);
  if (GameFlag_Test(0x254))
    MapGrid_CopyCellAttributes(0, 48, 1, 3, 19, 26);
}

s32 Func_02000df8(s32 x, s32 z) {
  struct Actor *object = Actor_GetById(15);
  struct TrialRoadMapWork *map =
      (struct TrialRoadMapWork *)Ram_HeapSlots->mapWork;
  u8 *cells;
  u8 *cell;
  s32 value;

  if (object->x >> 20 == x && object->z >> 20 == z)
    return 1;
  cells = map->layers[0].cells;
  cell = cells + ((x + (z << 7)) << 2);
  value = cell[2];
  return IsZero(value ^ 0xff);
}

void Func_02000e40(s32 id, s32 dx, s32 dz, s32 axis) {
  struct Actor *object = Actor_GetById(id);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 fromX = object->x >> 20;
  s32 fromZ = object->z >> 20;
  s32 toX = fromX;
  s32 toZ = fromZ;
  s32 n;
  s32 mode = 2;
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;
  // Unused: gives the stack frame its 12 bytes.
  struct Vector3 unused;

  if ((dx != 0 && toX != x + dx) || (dz != 0 && toZ != z + dz)) {
    ((struct EventWork184 *)Ram_HeapSlots->eventWork)->value = 13;
    return;
  }
  for (n = 0; n <= 5;) {
    x = toX + dx;
    z = toZ + dz;
    if (axis == 0) {
      if (Func_02000df8(x, z - 1))
        break;
      if (Func_02000df8(x, z))
        break;
      if (Func_02000df8(x, z + 1))
        break;
    } else {
      if (Func_02000df8(x - 1, z))
        break;
      if (Func_02000df8(x, z))
        break;
      if (Func_02000df8(x + 1, z))
        break;
    }
    toX = x;
    n++;
    toZ = z;
  }
  if (fromX == toX && fromZ == toZ)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 8);
  Event_Wait(6);
  object->speedLimit = 0x8000;
  object->acceleration = 0x3333;
  Sound_Play(239);
  if (dx < 0 || dz < 0)
    mode = 3;
  Func_020057c8(object, mode);
  Actor_SetMoveTarget(object, (toX << 20) + 0x80000, 0,
                      z = (toZ << 20) + 0x80000);
  Event_Wait(6);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x4ccc, 0x3333);
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, dx << 3,
                                     dz << 3);
  Event_Wait(24);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Script_WaitForEventTimeout(object);
  if (toX <= 17) {
    Func_020057c8(object, 1);
    Sound_Play(298);
    Sound_Play(213);
  } else {
    Func_020057c8(object, 3);
    Actor_SetMoveTarget(object, 312 << 16, 0, z);
    Event_Wait(30);
    Func_020057c8(object, 8);
    Script_WaitForEventTimeout(object);
    {
      /* FAKEMATCH(pin): the 2 held in r2 while the address takes r3; as
       * object->priorityFlags = 2, 8 of 240 instructions differ in tla-en. */
      u8 *flags = &object->priorityFlags;
      register s32 value asm("r2") = 2;

      *flags = value;
    }
    Sound_Play(298);
    Sound_Play(240);
    GameFlag_SetBit(id + 579);
  }
  Event_Wait(15);
  Event_End();
  Func_02000d70();
}

void Func_0200106c(void) { Func_02000e40(16, -1, 0, 0); }

void Func_02001080(void) { Func_02000e40(16, 1, 0, 0); }

void Func_02001090(void) { Func_02000e40(17, -1, 0, 0); }

void Func_020010a4(void) { Func_02000e40(17, 1, 0, 0); }

void Func_020010b4(void) { Func_02000e40(18, 0, -1, 1); }

void Func_020010c8(void) { Func_02000e40(18, 0, 1, 1); }

void Func_020010d8(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->renderMode == 2)
    Func_020059c8();
  else
    Func_02000e40(18, 0, 1, 1);
}

void Func_02001104(s32 id, s32 a, s32 frame) {
  if (frame == 30) {
    struct Actor *object = Actor_GetById(id);
    s32 height;

    object->x = object->timer << 16;
    object->z = (s16)object->unk66 << 16;
    height = Map_GetTerrainHeight(0, object->x, object->z);
    object->terrainHeight = height;
    object->y = height;
    Func_0200078c(id);
    GameFlag_SetBit(id + 0x353);
  }
}

void Func_02001148(s32 a, s32 b) { Func_02001104(16, a, b); }

void Func_02001158(s32 a, s32 b) { Func_02001104(17, a, b); }

void Func_02001168(s32 a, s32 b) { Func_02001104(18, a, b); }

void Func_02001178(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *block = Actor_GetById(15);

  if (block->x >> 20 == leader->x >> 20 && block->z >> 20 == leader->z >> 20) {
    s32 y = leader->y;

    if (y < 0x200000)
      leader->y = 0x200000;
  }
}

void Func_020011c0(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 2;
}

void Func_020011dc(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 0;
}

void Func_020011f8(void) {
  MapGrid_CopyCellAttributes(12, 21, 1, 1, 19, 21);
  GameFlag_SetBit(0x374);
}

#include "../../field/object/set_sprite_priority.c"
