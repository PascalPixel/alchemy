/// @file
/// The scene code of overlay 0x680, in ROM order.

#define OWN_Actor_SetSingleChildFlags
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "actor.h"
#include "version.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "call.h"
#include "event_work.h"
#include "iwram_call.h"
#include "worka0.h"
#include "frame.h"
#include "field/event_work.h"
#include "text/msg_ids.h"
#include "map.h"
#include "task.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

#include "scene_ids.h"

extern const u8 gData_02000f5c[];
extern const u8 gData_02000f8c[];

const struct ActorPlacement *Func_02000040(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room1)
    return (const struct ActorPlacement *)gData_02000f5c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room2)
    return (const struct ActorPlacement *)gData_02000f8c;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

struct Trigger;

void Func_02000bc0(s32 id, s32 flag);

extern const u8 gData_0200102c[];
extern const u8 gData_02001044[];
extern const u8 gData_02001194[];
extern const u8 gData_020012cc[];

const struct Trigger *Func_02000080(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room1)
    return (const struct Trigger *)gData_02001044;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room2)
    return (const struct Trigger *)gData_02001194;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room3)
    return (const struct Trigger *)gData_020012cc;
  return (const struct Trigger *)gData_0200102c;
}

void Func_020000d0(void) { Func_02000bc0(17, 0x300); }

void Func_020000e0(void) { Func_02000bc0(18, 0x301); }

void Func_020000f0(void) { Func_02000bc0(19, 0x302); }

// Sets the object's motion flags to 3 while it moves, for at most 60 frames
// until its vertical speed is 0, then clears them ten frames later. The
// editions after the Japanese one also run the given cue first.
#if !TLA_PATCH_1
void Func_02000100(s32 id) {
  struct Actor *object = Actor_Get(id);
  /* FAKEMATCH(pin): the counter fixed to r6, so the object takes r5; as a plain
   * local, 12 of 22 instructions differ in tla-ja. */
  register s32 frames asm("r6");

  object->motionFlags = 3;
  for (frames = 60; frames != 0; frames--) {
    System_Wait(1);
    if (object->velocityY == 0)
      break;
  }
  System_Wait(10);
  object->motionFlags = 0;
}
#else
void Func_02000100(s32 id, s32 cue) {
  struct Actor *object = Actor_Get(id);
  /* FAKEMATCH(pin): the flags' address fixed to r2, so it is built with 85 in
   * it first (movs r2, #85; adds r2, r2, r5) and the 3 takes r3; as a plain
   * local, 21 of 32 instructions differ in tla-en. */
  register u8 *flags asm("r2");
  u8 *held;
  /* FAKEMATCH(pin): the counter fixed to r6, so the object takes r5; as a plain
   * local, 12 of 32 instructions differ in tla-en. */
  register s32 frames asm("r6");

  flags = (u8 *)85;
  /* FAKEMATCH(barrier): an empty asm on flags, so 85 stays in a register and is
   * added to the object rather than the object copied and 85 added; without it,
   * 4 of 32 instructions differ in tla-en. */
  asm("" : "+r"(flags));
  flags += (s32)object;
  *flags = 3;
  frames = 60;
  /* FAKEMATCH(temporary): a copy of the flags' address, so it is copied to r8
   * for the calls after the counter starts; as flags itself, 9 of 32
   * instructions differ in tla-en. */
  held = flags;
  while (frames != 0) {
    System_Wait(1);
    frames--;
    if (object->velocityY == 0)
      break;
  }
  if (cue != 0)
    Event_PlaySound(cue);
  System_Wait(10);
  *held = 0;
}
#endif

void Func_02000148(void) {
  MapGrid_CopyCellAttributes(7, 90, 1, 1, 8, 91);
}

// The Japanese edition drops an object with no cue; the later editions play
// cue 181 as it drops. The first drop goes through the call layer.
#if !TLA_PATCH_1
void Func_02000100(s32 id);
#define DROP_OBJECT_CALLED(id) Func_02000100(id)
#define DROP_OBJECT(id) Func_02000100(id)
#else
void Func_02000100(s32 id, s32 cue);
#define DROP_OBJECT_CALLED(id) Func_02000100(id, 181)
#define DROP_OBJECT(id) Func_02000100(id, 181)
#endif

/// Copies cells by where the object with the given id has moved, its position
/// counted in 8-pixel steps: object 16, at x 11 or less while flag 0x920 is
/// clear, drops and sets the flag; object 19, while flag 0x921 is clear, copies
/// cells at x 77, drops at (79, 12) and at x 81 sets the flag.
void Func_02000164(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  struct Actor *p;

  MapGrid_CopyCellAttributes(8, 90, 1, 1, 8, 91);
  if (id == 16 && Event_GetFlag(0x920) == 0 &&
      (object->x >> 19) <= 11) {
    DROP_OBJECT_CALLED(16);
    Event_SetFlag(0x920);
    MapGrid_CopyCellAttributes(6, 27, 1, 1, 5, 27);
  }
  if (id == 19 && Event_GetFlag(0x921) == 0) {
    if ((object->x >> 19) == 77)
      MapGrid_CopyCellAttributes(33, 102, 1, 2, 37, 102);
    /* FAKEMATCH(temporary): the object read again through p for the last two
     * tests; as object throughout, 22 of 109 instructions differ in tla-en. */
    p = object;
    if ((p->x >> 19) == 79 && (p->y >> 19) == 12) {
      DROP_OBJECT(19);
      MapGrid_CopyCellAttributes(38, 46, 1, 1, 39, 46);
    }
    if ((p->x >> 19) == 81) {
      Event_SetFlag(0x921);
      {
        /* FAKEMATCH(temporary): the row in its own register; as a plain 46,
         * 2 of 109 instructions differ in tla-en. */
        s32 row = 46;

        MapGrid_CopyCellAttributes(41, row, 1, 1, 39, 46);
      }
      MapGrid_CopyCellAttributes(27, 44, 1, 1, 40, 46);
    }
  }
}

struct SceneEntrance;

void Func_02000e74(void);
void Func_02000e7c(void);
void Func_02000edc(s32 a);
void Func_02000f04(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

extern const u8 gData_020013c8[];
extern const u8 gData_020014b8[];
extern const u8 gData_02001554[];

void Func_02000258(void) {
  MapGrid_CopyCellAttributes(13, 18, 1, 1, 13, Actor_Get(8)->z >> 20);
}

void Func_0200027c(void) {
  MapGrid_CopyCellAttributes(11, 14, 1, 1, 13, Actor_Get(8)->z >> 20);
}

void Func_020002a0(void) {
  Call7((void (*)())Func_02000f04, 110, 64, 14, 12, 110, 47, 1);
}

void Func_020002c0(void) {
  struct Actor *object = Actor_Get(64);
  s32 done;

  if (object == NULL)
    return;
  done = Event_GetFlag(0xf74);
  if (done == 0) {
    Actor_SetPosition(64, PIXELS(657), PIXELS(246));
    object->motionFlags = done;
    object->y = PIXELS(80);
  }
}

void Func_020002fc(void) {
  if (Actor_Get(64) != NULL)
    Actor_SetPosition(64, 0, 0);
}

void Func_02000314(void) {
  struct Actor *object = Actor_Get(64);

  Message_ShowCentered(MsgCheckedTheAltar, 1);
  if (object != NULL && object->x != 0) {
    Func_02000edc(0x1040);
  } else if (Event_GetFlag(0xf74) == 0) {
    struct EventWork *work = Ram_HeapSlots->eventWork;

    Message_ShowCentered(MsgIMSensing, 1);
    work->unk15a = 1;
  } else {
    Message_ShowCentered(MsgICanT, 1);
  }
}

void Func_02000378(void) {
  if (Event_GetFlag(0x927)) {
    Func_02000e74();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId(MsgYouReGoing);
    FieldActor_StartRepeatedMotion(20, 1);
    FieldEvent_Wait(15);
    Event_AskYesNo(20, 0);
    Event_SetFlag(0x9ab);
    Func_02000e7c();
  }
}

void Func_020003c4(void) {
  if (Event_GetFlag(0x9ab)) {
    FieldEvent_SetMessageId(MsgYouReGoing);
    Event_AskYesNo(20, 0);
  } else {
    FieldEvent_SetMessageId(MsgIfYouRe);
    Event_ShowMessage(20, 0);
  }
}

void Func_020003fc(void) {
  if (Event_GetFlag(0x9ab)) {
    FieldEvent_SetMessageId(MsgKushinadaSSafe);
    Event_ShowMessage(20, 0);
  } else {
    FieldEvent_SetMessageId(MsgIReallyGot);
    Event_ShowMessage(20, 0);
  }
}

const struct SceneEntrance *Func_02000434(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room1)
    return (const struct SceneEntrance *)gData_020013c8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room2)
    return (const struct SceneEntrance *)gData_020014b8;
  return (const struct SceneEntrance *)gData_02001554;
}

struct OrbitEffect {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[15];
  u8 priorityFlags;
  u8 unk24[4];
  s32 scale;
  u8 unk2c[28];
  s32 speed;
  u8 unk4c[24];
  u16 timer;
  s16 pause;
  struct OrbitEffect *anchor;
  void (*update)(struct OrbitEffect *);
};

u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
void Func_02000e2c(struct Actor *object, const u8 *script);
void Func_02000e5c(struct Actor *object, s32 a);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);

extern u32 gData_020015c0[];

void Func_02000474(struct OrbitEffect *effect) {
  s32 radius;
  s32 angle;
  s32 dx;
  s32 dz;

  radius = Random16Far() + 0x20000;
  angle = effect->angle;
  dx = Iwram_MulQ16(radius, Math_Cosine(angle));
  dz = Iwram_MulQ16(radius, Math_Sin(angle));
  effect->x += dx;
  effect->z += dz;
  effect->angle += 0xfff0;
  if (effect->pause != 0) {
    effect->pause--;
    effect->angle += 0x800;
  } else if ((Random16Far() << 5) >> 16 == 0) {
    effect->pause = ((Random16Far() << 4) >> 16) + 8;
  }
  if (++effect->timer == 71)
    Func_02000e2c((struct Actor *)effect, (const u8 *)gData_020015c0);
}

s32 Func_02000528(struct OrbitEffect *source) {
  s32 i;
  s32 timer;

  i = 0;
  timer = 20;
loop:
  {
    struct OrbitEffect *effect;
    s32 radius;
    s32 position[3];

    position[0] = source->x;
    position[1] = source->y + i * 0xcccc + 0x40000;
    position[2] = source->z;
    radius = Random16Far();
    FieldVector_AddPolarOffset(radius * 5 + 0x30000, Random16Far(), position);
    effect = (struct OrbitEffect *)Value4(
        (s32 (*)())Actor_Spawn, 308, position[0], position[1], position[2]);
    if (effect != NULL) {
      effect->update = Func_02000474;
      effect->timer = timer;
      effect->pause = 0;
      effect->speed = 0x1999;
      effect->scale = 0x30000;
      effect->angle = Random16Far();
      effect->priorityFlags = 0;
      Func_02000e5c((struct Actor *)effect, 2);
    }
  }
  timer -= 2;
  i++;
  if (i < 16)
    goto loop;
  return 0;
}

struct A4SlotValues {
  s16 id;
  s16 unk02;
  s32 value04;
  u8 unk08[8];
  s32 value10;
  u8 unk14[8];
};

static inline struct A4SlotValues *FindSlot(s32 id) {
  struct WorkA4 *work = WORK_A4;
  struct A4SlotValues *found = NULL;
  s32 i;

  // Default: as Func_080dae70, the index zero is copied from found.
  i = 0;

  for (i = 0; i < 8; i++) {
    if (work->slots[i].id == id) {
      found = (struct A4SlotValues *)&work->slots[i];
      break;
    }
  }
  return found;
}

void Func_020005f0(void) {
  struct A4SlotValues *slot = FindSlot(8);

  slot->value10 = 0x4000;
  slot->value04 = 64;
}

void Func_02000630(void) {
  struct A4SlotValues *slot = FindSlot(8);

  slot->value10 = 0x4000;
  slot->value04 += 128;
}

void Func_02000e24(struct Actor *object, s32 a);
void Func_02000f1c(struct Actor *object, s32 a);

extern const u8 gData_020015c4[];

void Func_02000670(void) {
  struct Actor *object = Actor_Get(17);
  u8 phase = gFrameCount;

  if (phase == 0) {
    Func_02000f1c(object, 202);
    object = Actor_Spawn(0x118, object->x, object->y, object->z);
    if (object != NULL) {
      object->acceleration = 0x20000;
      object->speedLimit = 0x20000;
      object->scaleX = 0x8000;
      object->scaleY = 0x1999;
      object->priorityFlags = phase;
      Func_02000e24(object, 2);
      FieldActor_SetMoveTarget(object, object->x - PIXELS(32), object->y, object->z);
      Func_02000e2c(object, gData_020015c4);
      Func_02000e5c(object, 2);
    }
  }
}

#if !TLA_PATCH_1
#define SCREEN_EFFECT 0x201
#else
#define SCREEN_EFFECT 0x204
#endif

s32 Func_02000f14();
void Actor_SetSingleChildFlags();
void Func_02000ed4();
void Func_02000dd0();
void Func_02000b2c();

s32 Func_020006ec(void) {
  struct Actor *actor;
  struct EventWork *work;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = SCREEN_EFFECT;
  gMainRoutineWork.retreatScene = (s32)&SceneId_Ov680Room1;
  gMainRoutineWork.retreatEntrance = 5;
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 32;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room1) {
    Task_Add((s32)Func_02000670, 0x480);
    Call2((void (*)())Func_02000f14, 8, 1);
    Call2((void (*)())Func_02000f14, 9, 1);
    Call2((void (*)())Func_02000f14, 10, 1);
    Call2((void (*)())Func_02000f14, 11, 1);
    Call2((void (*)())Func_02000f14, 12, 1);
    Call2((void (*)())Func_02000f14, 13, 1);
    Call2((void (*)())Func_02000f14, 14, 1);
    Func_02000f14(15, 1);
    if (GameFlag_Test(0x920)) {
      actor = Actor_GetById(16);
      actor->motionFlags = 0;
      actor->x = 0x580000;
      actor->y = 0x800000;
      Actor_SetSingleChildFlags(actor, 0);
      MapGrid_CopyCellAttributes(6, 27, 1, 1, 5, 27);
    }
    actor = Actor_GetById(19);
    actor->priorityFlags = 0;
    if (GameFlag_Test(0x921)) {
      actor->motionFlags = 0;
      actor->x = 0x2880000;
      actor->y = 0x400000;
      Actor_SetSingleChildFlags(actor, 0);
      MapGrid_CopyCellAttributes(33, 102, 1, 2, 37, 102);
      MapGrid_CopyCellAttributes(27, 44, 1, 1, 40, 46);
    } else if (GameFlag_Test(0x109)) {
      if ((actor->x >> 19) != 75) {
        MapGrid_CopyCellAttributes(33, 102, 1, 2, 37, 102);
      }
      if ((actor->x >> 19) == 79) {
        actor->motionFlags = 0;
        MapGrid_CopyCellAttributes(38, 46, 1, 1, 39, 46);
      }
    }
    if (GameFlag_Test(0x927) && !GameFlag_Test(0x9ab)) {
      Actor_SetPosition(20, 0x31e0000, 0x3200000);
      Map_CopyCellAttributeRect(53, 52, 1, 1, 49, 49);
      Map_CopyCellAttributeRect(53, 52, 1, 1, 50, 49);
      Actor_SetModeById(20, 5);
      if (!Event_GetFlag(0xa61)) {
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        FieldActor_StartRepeatedMotion(20, 3);
        Func_02000ed4(20, 1);
        FieldEvent_Wait(30);
        FieldEvent_SetMessageId((s32)MsgIMNot);
        Event_ShowMessage(20, 0);
        FieldActor_StartRepeatedMotion(20, 2);
        FieldEvent_Wait(30);
        Event_SetFlag(0xa61);
        Func_02000e7c();
      }
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room2) {
    Call2((void (*)())Func_02000f14, 8, 1);
    Call2((void (*)())Func_02000f14, 9, 1);
    Call2((void (*)())Func_02000f14, 10, 1);
    Call2((void (*)())Func_02000f14, 11, 1);
    Call2((void (*)())Func_02000f14, 12, 1);
    Call2((void (*)())Func_02000f14, 13, 1);
    Func_02000f14(14, 1);
    if ((u16)(gMainRoutineWork.entrance - 3) <= 1) {
      Func_02000dd0();
    }
    if (GameFlag_Test(0x300)) {
      Func_02000b2c(17, 0x2fc0000, -0x1870000, 0x1a80000);
    }
    if (GameFlag_Test(0x301)) {
      Func_02000b2c(18, 0x3150000, -0xf80000, 0x1a80000);
    }
    if (GameFlag_Test(0x302)) {
      Func_02000b2c(19, 0x30a0000, -0x770000, 0x1a80000);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room3) {
    actor = Actor_GetById(64);
    if (actor != 0) {
      Actor_SetPosition(64, 0, 0);
    }
    work = Ram_HeapSlots->eventWork;
    actor = work->objects[16];
    work->objects[64] = actor;
    actor = Actor_GetById(8);
    actor->motionFlags = 0;
    MapGrid_CopyCellAttributes(11, 14, 1, 1, 13, actor->z >> 20);
  }
  return 0;
}

s32 Func_02000a68(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room2 &&
      (u16)(gMainRoutineWork.entrance - 3) <= 1) {
    s32 *layer = (s32 *)&Ram_HeapSlots->mapWork->layers[1];

    layer[2] = -0x800000;
    layer[3] = 0x4480000;
  }
  return 0;
}

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);

void Func_02000ab4(void) {
  if (gMainRoutineWork.renderMode == 2) {
    struct EventWork *work = Ram_HeapSlots->eventWork;
    struct MapWork *map = Ram_HeapSlots->mapWork;
    struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
    s32 x = object->x / 0x100000;
    s32 z = (object->z - object->y + 0x7fff) / 0x100000;
    struct MapCell *cells = map->layers[2].cells;
    struct MapCell *cell = &cells[x + (z << 7)];
    s32 code = cell->collisionCode;

    if (code >= 1 && code <= 230)
      work->exit = code;
  }
}

void Func_02000b2c(s32 id, s32 x, s32 y, s32 z) {
  struct Actor *object = Actor_Get(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[2].cells;
  s32 cellX = x / 0x100000;
  s32 cellZ = (z - y + 0x7fff) / 0x100000;

  cell += cellX + (cellZ << 7);
  cell->collisionCode = 255;
  object->x = (x & 0xfff00000) + 0x80000;
  object->z = (z & 0xfff00000) + 0x80000;
  object->y = object->z - (cellZ << 20);
  object->motionFlags = 0;
  object->priorityFlags |= 2;
  Actor_SetSingleChildFlags(object, 0);
}

extern const u8 gData_020016cc[];
extern const u8 gData_02001688[];

void Func_02000ecc(void);

void Func_02000bc0(s32 id, s32 flag) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 gravity = leader->gravity;
  struct Actor *pillar;
  s32 i;

  Func_02000e74();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  pillar = Actor_Get(id);
  Func_02000b2c(id, leader->x, leader->y, leader->z);
  Func_02000e2c(pillar, gData_020016cc);
  i = 15;
dust:
  {
    struct Actor *dust =
        FieldActor_Create(255, leader->x, leader->y + 0x100000, leader->z);

    if (dust != 0) {
      s32 angle;
      s32 speed;
      u32 size;
      s32 scale;

      Func_02000e2c(dust, gData_02001688);
      angle = Random16Far();
      speed = Random16Far();
      size = Random16Far();
      scale = (size >> 1) + 0x2000;
      dust->velocityY = Iwram_MulQ16(speed * 8, Math_Cosine(angle));
      dust->velocityX = Iwram_MulQ16(speed * 8, Math_Sin(angle));
      dust->acceleration = 0;
      dust->scaleX = scale;
      dust->scaleY = scale;
      dust->gravity = 0x547a;
      dust->bounce = 0x8000;
    }
  }
  if (--i >= 0)
    goto dust;
  Event_PlaySound(0x24e);
  Event_PlaySound(217);
  MapGrid_SetShake(0x20000, 0x20000, 0xe666);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Func_02000e24(leader, 40);
  FieldEvent_Wait(40);
  Event_PlaySound(204);
  leader->motionFlags = 3;
  id = 0;
  leader->gravity = 0x547a;
  goto fall;
next:
  id++;
fall:
  if (id <= 179) {
    System_Wait(1);
    if (leader->y > leader->terrainHeight)
      goto next;
  }
  Event_PlaySound(188);
  leader->gravity = gravity;
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0);
  Func_02000e24(leader, 38);
  MapGrid_SetShake(0x10000, 0x40000, 0x10000);
  System_Wait(10);
  MapGrid_SetShake(-1, -1, 0xe666);
  System_Wait(20);
  FieldCamera_MoveTo(leader->x, leader->y, leader->z, 1);
  Func_02000ecc();
  FieldEvent_Wait(20);
  Actor_SetSingleChildFlags(leader, 1);
  Func_02000e24(leader, 1);
  leader->velocityY = 0x40000;
  System_Wait(20);
  gMainRoutineWork.renderMode = 0;
  Func_02000e7c();
  Event_SetFlag(flag);
}

void Func_02000dd0(void) {
  Task_AddFrame(Func_02000ab4, TASK_PASS_VBLANK);
}
