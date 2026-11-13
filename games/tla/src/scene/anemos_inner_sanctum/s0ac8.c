#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "system/io_reg.h"
#include "system/main_routine_work.h"
#include "event_inline.h"
#include "field.h"

struct Work {
  u8 pad[340];
  u16 room;
};

struct Globals {
  u8 pad[108];
  struct Work *work;
};

struct Vector3 {
  s32 x;
  s32 y;
  s32 z;
};

void Event_WaitTransitionFrames(void);
void Func_0200088c();
void Func_020009b8();
void Func_02000a78();
void Actor_StartScript();
void Func_020032f8();
void DisplayBlend_DisableRunScript();
void DisplayBlend_EnableRunScript();
void Func_02003380(s32 x, s32 z, s32 a, s32 b);
void Event_Start(void);
void Event_End(void);
void Func_02003430(void);
extern u8 gData_020035cc[];
extern u8 gData_02003620[];
extern s32 gData_020036fc[][2];

void Func_02000ac8(void) {
  struct Work *work = ((struct Globals *)Ram_HeapSlots)->work;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  volatile u16 alpha = REG_BLDALPHA;
  s32 x, z;
  s32 targetX = 0;
  s32 targetZ = 0;
  s16 type = work->room + 193;
  s16 room = work->room;
  s32 index = room - 50;
  s32 base;
  u8 count;
  s32 part;
  s32 column, row, t0, t1;
  s32 i;
  u8 last;
  struct Actor *actor;
  u8 found[20];
  u8 order[20];
  struct Vector3 from;
  struct Vector3 to;

  x = ((index & 1) << 8) + 408;
  base = 0;
  count = 0;
  part = 0;
  z = (index & 2) * 3 * 32 + 168;
  switch (room) {
  case 50:
    base = 48;
    targetX = 0x208;
    targetZ = 48;
    if (GameFlag_Test(0xa87))
      goto seen;
    break;
  case 51:
    part = 3;
    base = 68;
    targetX = 0x228;
    targetZ = 48;
    if (GameFlag_Test(0xa88))
      goto seen;
    break;
  case 52:
    part = 10;
    base = 88;
    targetX = 0x1f8;
    targetZ = 40;
    if (GameFlag_Test(0xa89))
      goto seen;
    break;
  case 53:
    part = 2;
    base = 108;
    targetX = 0x238;
    targetZ = 40;
    if (GameFlag_Test(0xa8a)) {
    seen:
      GameFlag_SetBit(0x200);
      return;
    }
    break;
  }

  i = (u16)base;
  if (i != (u16)base + 18) {
  scan:
    if (GameFlag_Test(i)) {
      found[count] = i - base;
      count++;
    }
    i++;
    if (i != (u16)base + 18)
      goto scan;
  }
  if (count == 0)
    return;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  Actor_WalkTo(gMainRoutineWork.currentOwner, x, z);
  FieldCamera_MoveTo(x << 16, -1, z << 16, 1);
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  actor =
      (FieldActor_Create((u16)type, leader->x, leader->y + 0x80000, leader->z));
  actor->motionFlags = 0;
  actor->scaleY = 0x1999;
  actor->scaleX = 0x1999;
  Actor_SetSingleChildFlags((s32)actor, 0);
  Actor_StartScript(actor, gData_020035cc);
  Animation_SetStateFlags(actor, 1);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_Interpolate(60);
  DisplayBlend_EnableRunScript();
  alpha = alpha & 255;
  REG_BLDALPHA = 0x1000;
  t0 = x >> 4;
  t1 = z >> 4;
  column = t0;
  row = t1;
  MapGrid_CopyCellsTo(t0 + 63, t1 + 23, t0 + 63, t1 - 3, 3, 5);
  i = 0;
  if (alpha != 0) {
  fadeIn:
    REG_BLDALPHA = i | 0x1000;
    System_Wait(8);
    i++;
    if (i != alpha)
      goto fadeIn;
  }
  DisplayBlend_DisableRunScript();
  Event_PlaySound(246);
  FieldEvent_Wait(48);

  last = count - 1;
  for (i = 0; i != count; i++) {
    s32 pick = Random16Far();
    pick &= last;

    order[i] = found[pick];
    found[pick] = found[last];
    last--;
  }
  for (i = 0; i != count;) {
    s32 slot;

    from.x = leader->x + ((Random16Far() & 31) << 16) - 0x100000;
    from.y = leader->y + ((Random16Far() & 15) << 16) + 0x80000;
    from.z = leader->z + ((Random16Far() & 15) << 16) - 0x80000;
    slot = order[i];
    to.x = leader->x + (gData_020036fc[slot][0] << 16);
    to.y = leader->y;
    to.z = leader->z + (gData_020036fc[slot][1] << 16);
    Func_0200088c(&from, &to, part);
    if (i & 1)
      Event_PlaySound(195);
    i++;
    FieldEvent_Wait(8);
  }

  if (count != 18) {
    FieldEvent_Wait(96);
    Actor_StartScript(actor, gData_02003620);
    DisplayBlend_EnableRunScript();
    alpha = REG_BLDALPHA & 255;
    PaletteFade_ApplyTarget(0x10000, 1);
    PaletteFade_Interpolate(60);
    for (i = alpha; i != 0;) {
      REG_BLDALPHA = i | 0x1000;
      i--;
      System_Wait(8);
    }
    for (i = 0; i != count;) {
      s32 slot = order[i];

      i++;
      MapGrid_CopyCellsTo(
          64, 0, ((leader->x + (gData_020036fc[slot][0] << 16)) >> 20) + 64,
          (leader->z + (gData_020036fc[slot][1] << 16)) >> 20, 1, 1);
    }
    MapGrid_CopyCellsTo(64, 0, column + 63, row - 3, 3, 5);
    DisplayBlend_DisableRunScript();
    Event_WaitTransitionFrames();
  } else {
    Event_SetFlag((s16)work->room + 0xa55);
    Actor_StartScript(actor, gData_02003620);
    FieldEvent_Wait(60);
    FieldCamera_SetSpeed(0x40000, 0x8000);
    FieldCamera_MoveTo(0x2180000, -1, 0x780000, 1);
    Func_02003430();
    Event_PlaySound(220);
    {
      s32 a = targetX << 16;
      s32 b = targetZ << 16;

      Func_020009b8(a, b, part, 0x800);
      FieldEvent_Wait(10);
      Func_020009b8(a, b, part, 0x900);
      FieldEvent_Wait(10);
      Func_020009b8(a, b, part, 0xa00);
    }
    FieldEvent_Wait(12);
    System_Wait(16);
    PaletteFade_ApplyTarget(0x7fff, 0);
    PaletteFade_Interpolate(1);
    System_Wait(1);
    PaletteFade_ApplyTarget(0x10000, 0);
    PaletteFade_Interpolate(32);
    Event_PlaySound(0x23e);
    Func_02000a78(targetX, targetZ, part);
    FieldEvent_Wait(80);
    if (GameFlag_Test(0xa87) && GameFlag_Test(0xa88) && GameFlag_Test(0xa89) &&
        GameFlag_Test(0xa8a)) {
      Event_SetFlag(0xa8b);
      Event_PlaySound(188);
      MapGrid_CopyCellsTo(52, 7, 33, 7, 1, 2);
      System_Wait(5);
      MapGrid_CopyCellsTo(54, 7, 33, 7, 1, 2);
      Func_02003380(0x2180000, 0x880000, 0, 0);
      System_Wait(60);
    }
  }

  Func_020032f8(actor);
  Event_SetFlag(0x200);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003430();
  FieldEvent_Wait(60);
  Event_End();
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02001090(void) { GameFlag_ClearBit(0x200); }

s32 Func_02003498();
extern u8 MsgHat[];
extern u8 gData_020036e4[];
extern u8 MsgCrownBoostsLuck[];
extern u8 gData_020036ea[];
extern u8 MsgHatBoostsAgility[];
extern u8 gData_020036f6[];

void Func_0200109c(void) {
  if (gMainRoutineWork.scene == (s32)MsgHat) {
    Func_02003498((s32)gData_020036e4);
  } else {
    if (gMainRoutineWork.scene == (s32)MsgCrownBoostsLuck) {
      Func_02003498((s32)gData_020036ea);
      Field_SetCellBehaviorAt(2, 0x480000, 0x1b80000, 0xff);
    } else {
      if (gMainRoutineWork.scene == (s32)MsgHatBoostsAgility) {
        Func_02003498((s32)gData_020036f6);
        Field_SetCellBehaviorAt(2, 0x1180000, 0x1580000, 0xff);
        Field_SetCellBehaviorAt(2, 0x1180000, 0x1780000, 0xff);
        Field_SetCellBehaviorAt(2, 0x1280000, 0x1680000, 0xff);
        Field_SetCellBehaviorAt(2, 0x1180000, 0x1680000, 0xe8);
        Field_SetCellBehaviorAt(2, 0x2080000, 0x2780000, 0xff);
        Field_SetCellBehaviorAt(2, 0x2080000, 0x2680000, 0xe8);
      }
    }
  }
}

#include "scene_tables.h"

extern char SceneId_Ov6B0Room2;
extern char SceneId_Ov6B0Room3;
extern char SceneId_Ov6B0Room4;

void Func_020034a0(void);

static __inline__ void MoveObject(s32 id, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h); as a direct call, 2
   * of 243 instructions differ in tla-en. */
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

static __inline__ void SetCell(s32 a, s32 x, s32 z, s32 b) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h); as direct calls, 36
   * of 243 instructions differ in tla-en. */
  SetMapCellCollision(a, x, z, b);
}

/// An event handler from the sanctum's event table (in s2f48.c),
/// reading the leader's place in 8-pixel cells: sets the cells of rooms 3 and
/// 4, and sets flags 0xa80 to 0xa86 where the leader stands on given cells,
/// running the event at x 67, z 45 of room 3 and moving object 13 to object 12
/// at x 91, z 59 until flag 0xa84 is set.
void Func_02001160(void) {
  struct Actor *leader =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);
  s32 x;
  s32 z;

  Event_Start();
  Event_ResolvePendingActions(0);
  Func_020034a0();
  x = leader->x >> 19;
  z = leader->z >> 19;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room3) {
    SetCell(2, 0x480000, 0x1b80000, 232);
    if (x != 8)
      SetCell(2, 0x480000, 0x1a80000, 232);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4) {
    SetCell(2, 0x1180000, 0x1580000, 0);
    SetCell(2, 0x1180000, 0x1780000, 232);
    SetCell(2, 0x1280000, 0x1680000, 0);
    SetCell(2, 0x2080000, 0x2780000, 232);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room2) {
    if (x == 37 && z == 31) {
      GameFlag_SetBit(0xa80);
      Actor_SetSpritePriority(8, 2);
    }
    if (x == 55 && z == 95)
      GameFlag_SetBit(0xa81);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room3) {
    if (x == 7 && z == 53)
      GameFlag_SetBit(0xa82);
    if (x == 67 && z == 45) {
      MapGrid_CopyCellsTo(50, 22, 33, 22, 1, 1);
      Actor_SetPosition(14, 0, 0);
      Event_PlaySound(0x248);
      FieldEvent_Wait(30);
      Event_PlaySound(188);
      MapGrid_CopyCellsTo(50, 20, 31, 20, 1, 2);
      FieldEvent_Wait(5);
      MapGrid_CopyCellsTo(51, 20, 31, 20, 1, 2);
      FieldEvent_Wait(5);
      Func_02003380(0x1f00000, 0x1500000, 0, 0);
      GameFlag_SetBit(0xa83);
    }
    if (x == 93 && z == 57)
      GameFlag_SetBit(0xa84);
    if (x == 91 && z == 59) {
      if (!Event_GetFlag(0xa84)) {
        s32 fromX = (Actor_Get(12)->x >> 16) << 16;

        Actor_SetPosition(13, fromX, (Actor_Get(12)->z >> 16) << 16);
        MoveObject(12, PIXELS(728), PIXELS(472));
      }
      GameFlag_SetBit(0xa85);
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4 && x == 33 && z == 45)
    GameFlag_SetBit(0xa86);
  Event_End();
}

#include "field/event_work.h"

extern const s32 gData_02003574[];

void Func_020032e0(struct Actor *object, s32 value);
void Actor_StartScript(struct Actor *object, const s32 *script);
void Func_02003438(s32 id, s32 a);

static __inline__ struct Actor *Create(s32 type, s32 x, s32 y, s32 z) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h); as a direct call, 4
   * of 159 instructions differ in tla-en. */
  return ((struct Actor *)Import_ActorCreate(type, x, y, z));
}

/// An event handler from the sanctum's event table (in s2f48.c):
/// changes the cells at the leader's place, spawns an effect of type 0x15d
/// there and sets the flag the event work's exit selects; once flags 0x200 to
/// 0x206 are all set, it moves object 8 in room 4 (else object 9) to mode 2 and
/// sets flag 0x207.
void Func_02001398(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *object;
  s32 all;
  s32 i;
  s32 id;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4)
    MapGrid_CopyCellsTo(17, 71, leader->x >> 20, (leader->z >> 20) + 64, 1, 1);
  else
    MapGrid_CopyCellsTo(54, 87, leader->x >> 20, (leader->z >> 20) + 64, 1, 1);
  Event_PlaySound(106);
  object = Create(0x15d, ((leader->x >> 20) << 20) + 0x80000, 0,
                  ((leader->z >> 20) << 20) + 0x80000);
  object->priorityFlags |= 2;
  object->motionFlags = 0;
  Actor_SetSpriteColor(object, 4);
  object->scaleY = 0x16000;
  object->scaleX = 0x16000;
  Scene_SetActorSpritePriority(object, 2);
  Func_020032e0(object, 3);
  Actor_SetSingleChildFlags(object, 0);
  Actor_StartScript(object, gData_02003574);
  Event_SetFlag(work->exit + 0x1ce);
  all = Event_GetFlag(0x200);
  for (i = 1; i != 7; i++)
    all &= Event_GetFlag(0x200 + i);
  if (all) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_Wait(10);
    FieldCamera_SetSpeed(0xcccc, 0x1999);
    id = 9;
    if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4)
      id = 8;
    Func_02003438(id, 1);
    Func_02003430();
    FieldEvent_Wait(10);
    Event_PlaySound(0x246);
    Actor_SetAnimation(id, 2);
    Event_SetFlag(0x207);
    Actor_Get(id)->motionFlags = 4;
    FieldEvent_Wait(80);
    Event_End();
  }
}

void Event_FinishScreenTransition(void);

/// Centres the party leader on its cell and raises actor 8 (in Ov6B0Room4) or
/// actor 9 with cue 198, 4 pixels at once and then 128 more, before the screen
/// transition finishes.
void Func_02001518(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4) {
    object = Actor_Get(8);
    FieldEvent_SetExitRequest(13);
  } else {
    object = Actor_Get(9);
    FieldEvent_SetExitRequest(11);
  }
  {
    s32 x = ((leader->x >> 20) << 4) + 8;
    s32 z = ((leader->z >> 20) << 4) + 8;

    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, x, z);
  }
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 10);
  FieldEvent_Wait(30);
  object->motionFlags = 0;
  /* FAKEMATCH(wrapper): the camera move forwarded through FieldCamera_MoveTo,
   * so each -1 is built on its own, as in the ROM; as a direct call, the three
   * share one register, two instructions shorter, 12 of 100 instructions
   * differ in tla-en. */
  FieldCamera_MoveTo(-1, -1, -1, 0);
  object->speedLimit = 0x20000;
  object->acceleration = 0x800;
  Event_PlaySound(198);
  FieldActor_SetMoveTarget(object, object->x, object->y + 0x40000, object->z);
  Actor_WaitForMove(object);
  Scene_SetActorSpritePriority(leader, 1);
  Scene_SetActorSpritePriority(object, 1);
  FieldActor_SetMoveTarget(object, object->x, object->y + 0x800000, object->z);
  FieldEvent_Wait(80);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void Func_02001614(void) {
  s32 t0;
  s32 t1;

  t0 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  t1 =
      MapGrid_GetCellBehaviorAt(2, FIELD_AT(t0, s32, 8), FIELD_AT(t0, s32, 16));
  if (t1 == 0xe8) {
    FIELD_AT((t0 + 0x22), u8, 0) = 2;
  }
}

void Func_02001644(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  FIELD_AT((t0 + 0x22), u8, 0) = 0;
}

void Func_02001660(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 536, 360);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(50, 22, 33, 22, 1, 1);
  Actor_SetPosition(14, 0, 0);
  Event_PlaySound(0x248);
  FieldEvent_Wait(30);
  Event_PlaySound(0xbc);
  MapGrid_CopyCellsTo(50, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  MapGrid_CopyCellsTo(51, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  Event_SetFlag(0x200);
  Event_End();
}

void Func_020016fc(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(14));
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FIELD_AT(t0, s32, 8) = 0x2180000;
  FIELD_AT(t0, s32, 16) = 0x1680000;
  FIELD_AT(t0, s32, 20) = -917504;
  FIELD_AT(t0, s32, 12) = -917504;
  MapGrid_CopyCellsTo(51, 22, 33, 22, 1, 1);
  Event_PlaySound(0x248);
  FieldEvent_Wait(30);
  Event_PlaySound(0xbc);
  MapGrid_CopyCellsTo(50, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  MapGrid_CopyCellsTo(52, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  Event_ClearFlag(0x200);
  Event_End();
}

s32 Func_020030e0();

void Func_0200178c(void) {
  s32 t0;

  t0 = Func_020030e0();
  if (t0 != 0) {
    FieldEvent_SetExitRequest(0xb);
  }
}

void Func_020017a0(void) {
  s32 t0;

  t0 = Func_020030e0();
  if (t0 != 0) {
    FieldEvent_SetExitRequest(0xc);
  }
}

#include "frame.h"

extern const s32 gData_02003594[];

static __inline__ void AddPolar(s32 length, s32 angle, s32 *position) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h); as a direct call, 4
   * of 171 instructions differ in tla-en. */
  Vector_AddPolarOffsetFar(length, angle, position);
}

/// The update of an object that moves with the leader: once the leader is past
/// z 248 it settles on its cell and clears flag 0x200; otherwise it repeats the
/// leader's step, raising an effect of type 0x2a0 on each step, and stops where
/// the cell kind under it is 52 or 51, setting the event work's unk16a to 201
/// or 200. unk4c and acceleration hold the leader's last x and z.
void Func_020017b4(struct Actor *self) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 next[2];
  s32 ahead[3];
  struct Actor *dust;
  s32 kind;

  if (Actor_Get(gMainRoutineWork.currentOwner)->z >> 16 > 248) {
    self->update = 0;
    Event_ClearFlag(0x200);
    self->x = (self->x & 0xfff00000) + 0x80000;
    self->z = (self->z & 0xfff00000) + 0x80000;
    return;
  }
  ahead[0] = next[0] = self->x - (leader->x - self->unk4c);
  ahead[1] = leader->y;
  ahead[2] = next[1] = self->z + (leader->z - self->acceleration);
  if (self->unk4c != leader->x || self->acceleration != leader->z) {
    dust = (FieldActor_Create(0x2a0, self->x, self->y, self->z));
    dust->x += ((Random16Far() & 15) - 8) << 16;
    dust->y += ((Random16Far() & 31) - 4) << 16;
    dust->motionFlags = 0;
    Actor_SetSpriteColor(dust, ((u8 *)self)[0x62]);
    Scene_SetActorSpritePriority(dust, 2);
    Func_020032e0(dust, 1);
    Actor_SetSingleChildFlags(dust, 0);
    Actor_StartScript(dust, gData_02003594);
    self->unk4c = leader->x;
    self->acceleration = leader->z;
    if ((gFrameCount & 3) == 0)
      Event_PlaySound(106);
  }
  if (ahead[2] >> 16 > 248)
    return;
  AddPolar(0x70000, 0x8000 - leader->facing, ahead);
  if (MapGrid_GetCellBehaviorAt(self->terrainId, ahead[0], ahead[2]) == 255)
    return;
  {
    s32 x = next[0];

    self->x = x;
    self->z = next[1];
    kind = MapGrid_GetCellBehaviorAt(self->terrainId, x, next[1]);
  }
  if (kind == 52) {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->unk16a =
        201;
    self->update = 0;
  } else if (kind == 51) {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->unk16a =
        200;
    self->update = 0;
  }
}
