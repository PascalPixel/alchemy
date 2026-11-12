#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "local.h"
#include "io_reg.h"

extern const s32 gData_02005050[];

void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004e20(void);
u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_02001d04(void);

static __inline__ void Arrive(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, 194 << 18, 248 << 16);
  Actor_ResetAndSetPosition(id, x, z);
  Actor_GetById(id)->velocityY = 0xc0000;
  Actor_GetById(id)->gravity = 0x8000;
  Actor_GetById(id)->action = 0;
  Actor_GetById(id)->update = Func_02001d04;
}

void Func_02001d64(void) {
  s32 position[3];
  struct EffectOptions options;
  s32 i;

  Func_02004d40();
  Event_ResolvePendingActions(0);
  Sound_Play(107);
  MapGrid_SetShake(0x30000, 0x30000, 0x10000);
  Event_Wait(30);
  Camera_MoveWithinMapBounds(194 << 18, -1, 136 << 17, 1);
  Func_02004e20();
  options.type = 403;
  options.priority = 2;
  options.palette = 10;
  options.script = gData_02005050;
  options.startScaleY = 0x18000;
  options.startScaleX = 0x18000;
  position[1] = 0;
  i = 0;
loop:
  position[0] = ((Random16Far() & 31) << 16) + (190 << 18);
  position[2] = ((Random16Far() & 31) << 16) + (128 << 17);
  Func_0200015c(position[0], position[1], position[2], 0, 0x10000, 0, 0x3b0000,
                &options);
  if ((i & 7) == 0)
    Sound_Play(567);
  if (i == 32) {
    Arrive(13, 744, 216);
  } else if (i == 40) {
    Arrive(20, 776, 328);
  } else if (i == 48) {
    Arrive(14, 856, 232);
  } else if (i == 56) {
    Arrive(19, 696, 312);
  } else if (i == 64) {
    Arrive(15, 728, 280);
  } else if (i == 72) {
    Arrive(18, 824, 280);
  } else if (i == 80) {
    Arrive(16, 824, 312);
  } else if (i == 88) {
    Arrive(17, 776, 232);
  } else if (i == 112) {
    Actor_SetPosition(12, 194 << 18, 248 << 16);
    Actor_SetDestination(12, 696, 232);
    FieldCamera_MoveTo(174 << 18, -1, 232 << 16, 1);
    Actor_GetById(12)->velocityY = 0xd0000;
    Actor_GetById(12)->gravity = 0x8000;
    Actor_GetById(12)->action = 0;
    Actor_GetById(12)->update = Func_02001d04;
    MapGrid_SetShake(-1, -1, 0xe666);
  }
  wait(3);
  i++;
  if (i != 128)
    goto loop;
  FieldActor_WaitForMoveById(12);
  MapGrid_WaitForShakeToSettle();
  Sound_Play(288);
  Event_Wait(40);
  Event_SetFlag(2552);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02004e20();
  wait(10);
  Func_02004d48();
}

void Func_02002128(void) {
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;

  *(work + 0xd38) = 1;
  if (!Event_GetFlag(0x304)) {
    Actor_SetSpriteColor(Actor_Get(22), 15);
    Actor_SetPosition(22, PIXELS(632), PIXELS(312));
    Actor_Get(22)->scaleX = Actor_Get(22)->scaleY = 0x1000;
  }
}

void Func_0200217c(void) {
  if (!Event_GetFlag(0x303)) {
    Actor_SetSpriteColor(Actor_Get(21), 15);
    Actor_SetPosition(21, PIXELS(600), PIXELS(312));
    Actor_Get(21)->scaleX = Actor_Get(21)->scaleY = 0x1000;
  }
}

struct FieldActor;

void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004e20(void);
struct FieldActor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void Func_02004cb0(struct FieldActor *actor);
s32 Func_02000694(u32 layer, s32 x, s32 y, s32 value);

void Func_020021c0(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *object;
  struct FieldActor *actor;
  struct Actor *other;
  s32 i;

  object = Actor_GetById(((s8 *)work)[0xd30]);
  Actor_SetPartAttribute(object, 0);
  Sound_Play(136);
  i = 0;
  do {
    object->scaleY = object->scaleX += 0x1000;
    wait(5);
    i++;
  } while (i != 16);
  GameFlag_SetBit(((s8 *)work)[0xd30] + 0x2ee);
  if (GameFlag_Test(0x303) && GameFlag_Test(0x304)) {
    other = Actor_GetById(11);
    Func_02004d40();
    FieldEvent_ResolvePendingActions(0);
    MapGrid_SetShake(0x20000, 0x10000, 0x10000);
    Sound_Play(141);
    FieldEvent_Wait(30);
    FieldCamera_MoveTo(0x2680000, -1, 0x1280000, 1);
    Func_02004e20();
    FieldEvent_Wait(30);
    Sound_Play(0x22d);
    REG_BLDALPHA = 0x1000;
    REG_BLDCNT = 0x3f10;
    {
      struct FieldActor *prepared = OverlayObject_PrepareObject(
          other->x, other->terrainHeight, other->z + 0x300000, 312);

      other->motionFlags = 0;
      actor = prepared;
    }
    i = 0;
    do {
      other->y -= 0x10000;
      FieldEvent_Wait(5);
      i++;
    } while (i != 32);
    Sound_Play(0x22a);
    Func_02004c98(other, 4);
    Func_02004cb0(actor);
    FieldEvent_Wait(1);
    REG_BLDCNT = 0;
    Func_02000694(0, 38, 18, 0);
    Func_02004d48();
  }
}

void Func_02002318(void) {
  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 0xd30) == -1) {
    if (!Event_GetFlag(0x304))
      Actor_SetPosition(22, 0, 0);
    if (!Event_GetFlag(0x303))
      Actor_SetPosition(21, 0, 0);
  }
}

extern const u8 MsgBlazeStoneEmber[];

void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004e58(s32 item, s32 mode);

void Func_02002364(void) {
  const u8 *message;

  Func_02004d40();
  FieldEvent_ResolvePendingActions(0);
  message = MsgBlazeStoneEmber;
  Message_ShowCentered(message, 1);
  FieldEvent_SetMessageId(message + 1);
  Event_OpenMessage(12, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Party_GiveItem(460, 0);
    if (FieldPartyInventory_FindOwner(460) == -1) {
      Func_02004d48();
      return;
    }
    Func_02004e58(460, 3);
    Actor_SetAnimation(12, 2);
    Event_SetFlag(0x9f9);
  }
  Func_02004d48();
}

void Func_02004d40(void);
void Func_02004d48(void);

void Func_020023e8(void) {
  s32 width = 1;
  s32 height = 2;

  Func_02004d40();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(188);
  MapGrid_CopyCellsTo(72, 67, 15, 19, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(73, 67, 15, 19, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(74, 67, 15, 19, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(75, 67, 15, 19, width, height);
  FieldEvent_Wait(15);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_Get(gMainRoutineWork.currentOwner)->speedLimit =
      Actor_Get(gMainRoutineWork.currentOwner)->acceleration = 0x8000;
  Event_PlaySound(123);
  Actor_ResetAndSetPositionInMode2_(gMainRoutineWork.currentOwner, 248, 328);
  FieldEvent_Wait(5);
  FieldEvent_SetExitRequest(1);
  Func_02004d48();
}

extern s32 Data_020067ec;
extern s32 Data_020067f0;
extern s32 Data_020067f4;
extern s32 Data_020067f8;

s32 Func_02000658(s32 layer, s32 x, s32 z);

static inline void CopyCells(s32 srcX, s32 srcZ, s32 width, s32 height,
                             s32 destX, s32 destZ) {
  /* FAKEMATCH(wrapper): CopyCells forwarded inline; as direct calls to
   * Func_0802b450, 12 of 70 instructions differ in tla-en. */
  Func_0802b450(srcX, srcZ, width, height, destX, destZ);
}

void Func_020024ac(void) {
  struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  s32 key = (x << 16) + z;
  s32 last;
  s32 value;
  u8 *work;

  if (object->velocityX != 0 || object->velocityZ != 0)
    return;
  last = gData_02006710;
  if (last == -1 || key == last)
    return;
  x = last >> 16;
  z = last & 0xffff;
  gData_02006710 = -1;
  CopyCells(73, 72, Data_020067ec, Data_020067f0, Data_020067f4, Data_020067f8);
  if (gMainRoutineWork.scene != (s32)&SceneId_Ov692Room8)
    return;
  work = Ram_HeapSlots->eventWork;
  value = Func_02000658(2, x, z) >> 8;
  if (value != 0) {
    /* FAKEMATCH(temporary): the value plus 200 held in delay before the
       store; as *(s16 *)(work + 362) = value + 200, 6 of 70 instructions
       differ in tla-en. */
    s32 delay = value + 200;
    *(s16 *)(work + 362) = delay;
  }
}
