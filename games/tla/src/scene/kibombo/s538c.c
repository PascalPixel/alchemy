/// @file
/// Scene code of overlay 0x670, in ROM order, from s538c's offset up to the next object the overlay links.

#include "kibombo.h"

void Func_02002de0(void);
void Func_0200324c(void);

void Func_0200538c(void) {
  Actor_SetSingleChildFlags(Actor_GetById(9), 0);
  Actor_SetSingleChildFlags(Actor_GetById(10), 0);
  if (!GameFlag_Test(0x8fb))
    Func_02002de0();
  Func_0200324c();
  if (!GameFlag_Test(0x869))
    GameFlag_SetBit(0x201);
  Func_02005c64(gData_02006324);
  Actor_SetSpritePriority(8, 2);
  Actor_SetSingleChildFlags(Actor_GetById(11), 0);
  Actor_GetById(11)->motionFlags = 0;
  if (GameFlag_Test(0x9ea)) {
    Kibombo_ObjSetPos(Actor_GetById(11), 208 << 15, 0, 148 << 17);
    Kibombo_BlendStart(5, 18, 1, 1, 6, 18);
    wait(10);
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  if (GameFlag_Test(0x86a))
    Func_020005e0();
  if (GameFlag_Test(0x204))
    Func_02000624();
  if (GameFlag_Test(0x205))
    Func_02000668();
  if (GameFlag_Test(0x206))
    Func_020006ac();
  if (GameFlag_Test(0x207))
    Func_020006f0();
  Func_02005b8c(12);
  Func_02005b8c(13);
  Func_02005b8c(14);
  Actor_SetSpritePriority(12, 1);
  Actor_SetSpritePriority(13, 1);
  Actor_SetSpritePriority(14, 1);
}

void Func_0200538c(void);

void Func_020054e0(void) {
  s16 entrance;

  Func_0200538c();
  if (GameFlag_Test(0x8fb)) {
    Func_02003640(1, 0);
    Func_020036e0(1, 5);
  }
  wait(1);
  entrance = gMainRoutineWork.entrance;
  if (entrance == 99) {
    Func_02003640(0, 0);
    Func_020036e0(0, 0);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (entrance == 98) {
    Func_02003640(1, 0);
    Func_020036e0(1, 0);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (entrance == 97) {
    Func_02003640(1, 0);
    Func_020036e0(1, 2);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (entrance == 96) {
    Func_02003640(1, 1);
    Func_020036e0(1, 4);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
    Actor_SetModeById(14, 4);
  }
}

#include "io_reg.h"

void Func_020040cc(void);
void Func_02004294(void);
void Func_02004424(void);
void Func_020047e4(void);
void Func_0200489c(void);
void Func_02004978(void);
void Func_02004a28(void);
void Func_02003dec(void);

void Func_02005594(void) {
  u32 i;

  Func_02005a9c();
  Event_ResolvePendingActions(0);
  GameFlag_SetBit(0x869);
  GameFlag_SetBit(0x202);
  gData_02007948[0] = BG_PLTT[9];
  gData_02007948[1] = BG_PLTT[7];
  gData_02007948[2] = BG_PLTT[6];
  gData_02007948[3] = BG_PLTT[76];
  Func_0200538c();
  if (GameFlag_Test(0x8fb)) {
    Func_02003640(1, 1);
    Func_020036e0(1, 4);
  }
  wait(1);
  switch (gMainRoutineWork.entrance) {
  case 10:
    Func_020040cc();
    return;
  case 11:
    Func_02004294();
    return;
  case 12:
    Func_02004424();
    return;
  case 13:
    Func_020047e4();
    return;
  case 14:
    Func_0200489c();
    return;
  case 15:
    Func_02004978();
    return;
  case 16:
    Func_02004a28();
    return;
  }
  Actor_SetSpritePriority(17, 1);
  Actor_SetSpritePriority(7, 2);
  Actor_SetSpritePriority(26, 2);
  Actor_SetSpritePriority(5, 2);
  Actor_SetSpritePriority(6, 2);
  if (GameFlag_Test(0x92e))
    Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  else if (GameFlag_Test(0x92f)) {
    /* FAKEMATCH(temporary): a local holding the angle builds it before the
     * position; as a plain argument, 4 of 223 instructions differ in tla-en. */
    s32 angle = 0x2000;

    Kibombo_ObjPlaceFacing(7, 152 << 17, 176 << 15, angle);
  }
  if (GameFlag_Test(0x8fd)) {
    Func_020036e0(1, 6);
    Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  }
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Task_Add(Func_02002ccc, 0x480);
  for (i = 0; i <= 7; i++) {
    u8 *field = &Actor_GetById(i + 18)->field5c;
    s32 value = 2;

    *field = value;
  }
  if (GameFlag_Test(0x8f1)) {
    if (!GameFlag_Test(0x8fd))
      Func_020032b0();
  } else if (gMainRoutineWork.entrance == 5) {
    Func_02003dec();
  }
  Func_02005aa4();
  if (gMainRoutineWork.entrance == 99) {
    Func_02003640(0, 0);
    Func_020036e0(0, 0);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (gMainRoutineWork.entrance == 98) {
    Func_02003640(1, 1);
    Func_020036e0(1, 0);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (gMainRoutineWork.entrance == 97) {
    Func_02003640(1, 1);
    Func_020036e0(1, 4);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (gMainRoutineWork.entrance == 96) {
    Func_02003640(1, 1);
    Func_020036e0(1, 4);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
    Actor_SetModeById(14, 5);
  }
}

void Func_02003c14(void);
void Func_02003c84(void);

#define PALETTE_WORD(index) (*(u16 *)(0x05000000 + (index) * 2))

void Func_020057e0(void) {
  Func_02005a9c();
  Event_ResolvePendingActions(0);
  GameFlag_SetBit(514);
  gData_02007948[0] = PALETTE_WORD(169);
  gData_02007948[1] = PALETTE_WORD(167);
  gData_02007948[2] = PALETTE_WORD(166);
  if (gMainRoutineWork.entrance == 10) {
    Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                       0);
    Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
    Func_02003c84();
  } else if (gMainRoutineWork.entrance == 11) {
    Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                       0);
    Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
    Func_02003c14();
  } else {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    KIBOMBO_TASK_ADD(Func_02002d64, 1152);
    Func_02005aa4();
  }
}

#include "../../../../games/tla/src/scene/kibombo/kibombo.h"
#include "../../../../games/tla/include/scene_tables.h"
#include "version.h"
#include "fixed_math.h"

void Func_02003288(void);
void Func_02004cf4(void);
void Func_02005c74(s32 actor, s32 angle);

void Func_02005898(void) {
  struct KibomboObject *object;

  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Func_02003288();
  if (gMainRoutineWork.entrance == 9) {
    if (GameFlag_Test(0x8ef)) {
      Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
      if (GameFlag_Test(0x8ee)) {
        /* FAKEMATCH(temporary): the angle in a local of each branch, built
         * before the position; as the constant passed to
         * Kibombo_ObjPlaceFacing, 26 of 105 instructions differ in tla-en. */
        s32 angle = FACING_NORTHWEST + FACING_STEP;

        Kibombo_ObjPlaceFacing(8, PIXELS(434), PIXELS(634), angle);
      } else {
        s32 angle = FACING_NORTHWEST + FACING_STEP;

        Kibombo_ObjPlaceFacing(8, PIXELS(364), PIXELS(576), angle);
      }
    } else {
      Func_02004cf4();
    }
  }
#if TLA_PATCH_1
  if (GameFlag_Test(0x109)) {
    object = Actor_GetById(16);
    object->x -= PIXELS(14);
    object->z -= PIXELS(8);
  }
#endif
  Func_02005c74(16, 0);
  object = Actor_GetById(16);
  object->sprite->rotation = 0;
  object->scaleX = Q16_ONE;
  object->scaleY = Q16_ONE;
  object->x += PIXELS(14);
  object->z += PIXELS(8);
  Actor_SetModeById(16, 6);
  if (gMainRoutineWork.entrance == 14)
    GameFlag_SetBit(0x8ee);
}

void Func_02005994(void) { Func_02005bcc(); }

