/// @file
/// Scene code of overlay 0x681, in ROM order, from s0900's offset up to the next object the overlay links.

#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "event_inline.h"
#include "main_routine_work.h"
#include "call.h"
#include "scene_tables.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern s32 gData_020056c4[];
extern s32 gData_02005728[];

struct Counted {
  s32 *script;
  u8 unk04[0x57];
  u8 movementState;
  u8 unk5c[4];
  u8 count;
};

struct EventWork900 {
  u8 unk000[362];
  u16 timer;
};

void Func_02000900(struct Counted *object) {
  struct Counted *other = (struct Counted *)Actor_Get(9);

  if (object->count == 30 || other->count == 30) {
    ((struct EventWork900 *)Ram_HeapSlots->eventWork)->timer = 42;
    if (object->count == 30)
      object->count = 31;
    if (other->count == 30)
      other->count = 31;
  }
  if (object->count > 2) {
    other->movementState = 1;
    Event_SetFlag(0x206);
  }
  {
    u8 *otherCount = &other->count;

    if (*otherCount > 2 && !Event_GetFlag(0x206) &&
        other->script != gData_020056c4 && other->script != gData_02005728) {
      object->movementState = 1;
      Event_SetFlag(0x205);
    }
    if (object->count == 0) {
      other->movementState = 0;
      Event_ClearFlag(0x206);
    }
    if (*otherCount == 0) {
      object->movementState = 0;
      Event_ClearFlag(0x205);
    }
    if (object->count > 35)
      object->count = 35;
    if (*otherCount > 35)
      *otherCount = 35;
  }
}

extern char SceneId_Ov681Room2;
extern char SceneId_Ov681Room3;
extern char SceneId_Ov681Room4;
extern char SceneId_Ov681Room5;
extern u32 gData_02005970[];
extern u32 gData_020059a0[];
extern s32 gData_02005a54[];
extern u32 gData_02005afc[];
extern u32 gData_02005b8c[];
extern s32 gData_02005c7c[];

extern s32 gData_02005ee4;
extern const u8 gData_02005d6c[];
extern const u8 gData_02005e28[];
extern u8 gData_020023c4[];

void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_02004dac(struct Actor *object, const u8 *script);
u32 Func_02004904(void);
void Func_02004d9c(struct Actor *object, s32 mode);

/// Return the table for the current room.
const void *Func_020009e8(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room2)
    return gData_02005a54;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room3)
    return gData_02005afc;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room4)
    return gData_02005b8c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room5)
    return gData_02005c7c;
  if (gMainRoutineWork.entrance > 8)
    return gData_020059a0;
  return gData_02005970;
}

void Func_02000a60(struct Actor *object) {
  s32 value = 0;

  if ((gData_02005ee4 & 7) <= 0)
    value = 7;
  Animation_ApplyChildValues(object, value & 7);
  if ((gData_02005ee4 & 31) == 0) {
    object->scaleX = 0x10000;
    Func_02004dac(object, gData_02005e28);
    Event_PlaySound(106);
  }
  gData_02005ee4++;
}

void Func_02000aac(struct Actor *object) {
  s32 value = 0;

  if ((gData_02005ee4 & 7) <= 0)
    value = 7;
  Animation_ApplyChildValues(object, value & 7);
  if ((gData_02005ee4 & 31) == 0) {
    object->scaleY = 0x10000;
    Func_02004dac(object, gData_02005d6c);
    Event_PlaySound(106);
  }
  gData_02005ee4++;
}

void Func_02000af8(s32 unused, u32 id) {
  struct Actor *object = Actor_Get(id);
  u32 direction;
  s32 *state;

  direction = Func_02004904();
  state = (s32 *)(gData_020023c4 + 0x288);
  object->collisionFlags = 0;
  Func_02004d9c(object, ((direction & 0xc000) >> 14) + 2);
  if (direction & 0x4000)
    object->update = (void (*)(void))Func_02000aac;
  else
    object->update = (void (*)(void))Func_02000a60;
  object->collisionFlags = 1;
  Event_SetFlag(*state + 0xa50);
  Event_SetFlag(0x200);
  System_Wait(20);
}

void Func_02004e5c(void);
void Func_02004e64(void);
void Func_02004fdc(void);
void Func_02004ff4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

/// Unless MapGrid_GetCellBehaviorAt(2, ...) holds, copies map cell (36, 23) to (38,
/// 25), moves a leader standing on that cell aside, and sets flag 0x30a, or
/// 0x9bf once flag 0x927 is set.
void Func_02000b6c(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  Call7(Func_02004ff4, 37, 26, 0, 3, 3, 61, 0);
  if (MapGrid_GetCellBehaviorAt(2, 152 << 18, 200 << 17) == 0) {
    MapGrid_CopyCellAttributes(36, 23, 1, 1, 38, 25);
    Func_02004e5c();
    FieldEvent_ResolvePendingActions(0);
    if (leader->x >> 20 == 38 && leader->z >> 20 == 25) {
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 258);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = PIXELS(6);
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
    }
    if (!Event_GetFlag(0x927)) {
      Event_SetFlag(0x30a);
      FieldEvent_Wait(30);
      Func_02004fdc();
      FieldEvent_SetExitRequest(20);
    } else {
      Event_SetFlag(0x9bf);
      Func_02004e64();
    }
  }
}

void Func_02002cb4(s32 a);
void Func_02004f74(s32 a, s32 b);
void Func_0200500c(s32 a, s32 b, s32 c);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 GameFlag_Test_(s32 flag) { return GameFlag_Test(flag); }

static __inline__ void Func_0200500c_(s32 a, s32 b, s32 c) {
  Func_0200500c(a, b, c);
}

struct MainRoutineWork254 {
  u8 unk000[0x254];
  u16 value;
};

static __inline__ void Func_02004ff4_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  Func_02004ff4(a, b, c, d, e, f, g);
}

void Func_02000c38(void) {
  Func_02004ff4_(16, 25, 0, 5, 5, 40, 8);
  if (GameFlag_Test_(0xa63) == 0) {
    Event_SetFlag(0xa63);
    Func_02002cb4(2);
  }
}

void Func_02000c74(void) {
  Func_02004ff4_(18, 17, 0, 3, 3, 31, 0);
  if (Event_GetFlag(0x9b7) == 0) {
    Func_0200500c_(249, 328, 344);
    ((struct MainRoutineWork254 *)&gMainRoutineWork)->value = 0x19b7;
    gMainRoutineWork.battleRule24b = 2;
    Func_02004f74(106, 2);
  }
}
