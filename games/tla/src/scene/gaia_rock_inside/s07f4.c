/// @file
/// Scene code of overlay 0x681, in ROM order, from s07f4's offset up to the
/// next object the overlay links.

#include "types.h"
#include "system.h"
#include "actor.h"
#include "event_inline.h"
#include "main_routine_work.h"
#include "field.h"

extern u32 gData_020050f4[];

void Func_02004d9c(struct Actor *object, s32 mode);
void Func_02004dac(struct Actor *object, const s32 *script);
s32 Math_Cosine(s32 angle);
void Effect_AdvanceMotionAtLeaderPriority(void);

struct DustSprite {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
};

s32 Func_020007f4(void) {
  struct Actor *source = Actor_Get(9);
  s32 i;

  Event_PlaySound(202);
  i = 3;
  do {
    {
      struct Actor *dust =
          (FieldActor_Create(0x11d, source->x, source->y, source->z));

      if (dust != 0) {
        dust->motionFlags = 0;
        dust->collisionFlags = 0;
        Actor_SetSingleChildFlags(dust, 0);
        Func_02004d9c(dust, 1);
        Func_02004dac(dust, gData_020050f4);
        ((struct DustSprite *)dust->sprite)->layer = 1;
        dust->bounce = Math_Cosine(((Random16Far() << 3) >> 16) << 14) >> 1;
        dust->unk4c = Math_Sin(((Random16Far() << 3) >> 16) << 14) >> 1;
        dust->gravity = 0;
        dust->timer = ((Random16Far() << 9) >> 16) - 0x100;
        dust->speedLimit = 0;
        dust->acceleration = 0;
        dust->update = Effect_AdvanceMotionAtLeaderPriority;
      }
    }
  } while (--i >= 0);
  return 0;
}

extern s32 gData_020056c4[];
extern s32 gData_02005728[];

s32 Func_020008b4(void) {
  struct Actor *object = Actor_Get(9);
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  if (leader->z >> 19 <= 45 && leader->x >> 19 <= 57)
    Func_02004dac(object, gData_02005728);
  else
    Func_02004dac(object, gData_020056c4);
  return 0;
}

#include "ram_buffer.h"
#include "call.h"
#include "scene_tables.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

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
extern struct ActorPlacement gData_02005970[];
extern struct ActorPlacement gData_020059a0[];
extern struct ActorPlacement gData_02005a54[];
extern struct ActorPlacement gData_02005afc[];
extern struct ActorPlacement gData_02005b8c[];
extern struct ActorPlacement gData_02005c7c[];

extern s32 gData_02005ee4;
extern s32 gData_02005d6c[];
extern s32 gData_02005e28[];
extern u8 gData_020023c4[];

void Animation_ApplyChildValues(struct Actor *object, s32 value);
u32 Func_02004904(void);

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

/// Unless MapGrid_GetCellBehaviorAt(2, ...) holds, copies map cell (36, 23) to
/// (38, 25), moves a leader standing on that cell aside, and sets flag 0x30a,
/// or 0x9bf once flag 0x927 is set.
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

void Func_02000cd8(void) { Call7(Func_02004ff4, 0x13, 0x18, 0, 4, 3, 0x1f, 4); }

#include "iwram_call.h"

void Func_02000cf8(void) { Func_02004ff4_(18, 17, 0, 3, 3, 31, 0); }

struct CellPoint {
  s16 position[2];
};

extern const struct CellPoint gData_020051d8[12];
extern u32 gData_020051b8[];
extern s32 gData_02005118[];
extern s32 gData_02005168[];

void *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);

struct BoulderSprite {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
};

void Func_02000d18(void) {
  struct Actor *guard;
  struct Actor *leader;
  s16 count;
  s16 i;
  s32 height = 8;
  s32 sourceX = 117;
  s32 sourceY = 20;

  Func_02004ff4(104, sourceY, 1, height, height, sourceX, sourceY);
  if (GameFlag_Test(0x9c4))
    return;
  count = 0;
  for (i = 0; i <= 11; i++) {
    if (GetMapCellCollision(2, gData_020051d8[i].position[0] << 20,
                            gData_020051d8[i].position[1] << 20) == 0)
      count++;
  }
  if (count <= 0)
    return;
  guard = Actor_Get(15);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  Func_02004e5c();
  FieldEvent_ResolvePendingActions(0);
  if (leader->z >> 19 > 53 && leader->x >> 19 > 83 && leader->x >> 19 <= 91) {
    leader->speedLimit = 0x20000;
    Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
    Actor_SetScript(gMainRoutineWork.currentOwner, gData_020051b8);
  }
  Actor_SetScript(15, gData_02005118);
  FieldEvent_Wait(60);
  Actor_SetScript(15, gData_02005168);
  FieldEvent_Wait(30);
  MapGrid_CopyCellsLayered(117, 20, 7, 8, 104, 20);
  MapGrid_CopyCellsLayered(111, 12, 1, 6, 111, 20);
  {
    s32 destX = 40;
    s32 destY = 84;
    MapGrid_CopyCellAttributes(49, destY, 8, 8, destX, destY);
  }
  FieldEvent_Wait(30);
  OverlayObject_PrepareObject(175 << 18, 0, 0x1d40000, 286);
  OverlayObject_PrepareObject(175 << 18, 0, 0x1ea0000, 286);
  OverlayObject_PrepareObject(181 << 18, 0, 0x1d40000, 286);
  OverlayObject_PrepareObject(181 << 18, 0, 0x1ea0000, 286);
  Event_PlaySound(204);
  guard->motionFlags = 3;
  ((struct BoulderSprite *)guard->sprite)->layer = 3;
  FieldEvent_Wait(10);
  Actor_SetPosition(15, 0, 0);
  Func_02004fdc();
  Event_SetFlag(0x9c4);
  FieldEvent_SetExitRequest(9);
}

extern u8 MsgGaiaRockInside05[];

void Func_02000edc(void) {
  Func_02004e5c();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgGaiaRockInside05);
  Event_ShowMessage(0xa, 0);
  Func_02004e64();
}

extern struct Actor gData_0200646c;
extern s32 gData_020064ec;
extern s32 gData_020064f0;
extern s32 Data_020064f4;
extern s32 gData_020064f8;
extern s32 gData_020055c0[];
extern s32 gData_0200559c[];

void Actor_ResetTargetsAndVelocity(u32 objectId);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);

void Func_02000f00(void) {
  struct Actor *walker = Actor_Get(9);
  struct Actor *object = Actor_Get(10);
  u8 *sprite = object->sprite;
  u8 *animation = FIELD_AT(sprite, u8 *, 40);

  Iwram_CopyWords(&gData_0200646c, object, 128);
  gData_020064f8 = animation[21];
  Data_020064f4 = sprite[24];
  {
    s32 x = object->x;
    s32 z;

    gData_020064ec = x;
    z = object->z;
    object->update = 0;
    gData_020064f0 = z;
    object->velocityZ = 0;
    object->velocityY = 0;
    object->velocityX = 0;
    object->targetX = x;
    object->targetY = object->y;
    object->targetZ = z;
  }
  Actor_ResetTargetsAndVelocity(10);
  Actor_SetAnimationSpeed(object, 10);
  object->speedLimit = 0x10000;
  if (walker->script == gData_020055c0 || walker->script == gData_0200559c) {
    walker->movementState = 1;
    Actor_SetAnimationSpeed(walker, 0);
  }
}

void Func_02000fac(void) {
  struct Actor *walker = Actor_Get(9);
  struct Actor *object = Actor_Get(10);

  if (gData_020064ec != object->x || gData_020064f0 != object->z) {
    s16 *savedZ = (s16 *)&gData_020064f0;

    Actor_WalkTo(10, gData_020064ec >> 16, savedZ[1]);
    FieldActor_WaitForMoveById(10);
  }
  if (walker->script == gData_020055c0 || walker->script == gData_0200559c) {
    walker->movementState = 0;
    Actor_SetAnimationSpeed(walker, 16);
  }
  Iwram_CopyWords(object, &gData_0200646c, 128);
  Actor_SetAnimation(10, Data_020064f4);
  Actor_SetAnimationSpeed(object, gData_020064f8);
}
